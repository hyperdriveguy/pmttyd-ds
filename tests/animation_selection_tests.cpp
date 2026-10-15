#include "animation_selection.hpp"
#include "native_field_animation.hpp"
#include "field_party.hpp"
#include <cassert>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t W=0x90000000,P=0x91000000,J=0x92000000,E=0x93000000,H=0x94000000,D=0x95000000,T=0x96000000,G=0x97000000,S=0x98000000;
struct Op {char kind;uint32_t a,b;bool operator==(const Op& x) const{return kind==x.kind && a==x.a && b==x.b;}};
struct Fixture {
 std::map<uint32_t,uint32_t> ram;std::vector<Op> trace;unsigned calls=0,hold=0;bool held=false;void (*after)(Fixture&,char,uint32_t)=nullptr;
 Fixture(){ram={{0x80415dc0,W},{W+0x10,P},{W,J},{P+0x10,2},{J+40,E},{E+0xa0,H},{H,D},{D+0x148,3},{D+0x1ac,T},{P+0x14,0},{P+0x90,0xffffffff},{P,3},{P+0xc,0},{0x800000f8,162000000},{0x80415fd0,G},{G+0x40,0x12345678},{G+0x44,0xabcdef00},{G+0x38,0x87654321},{G+0x3c,0x76543210},{0x8041fb28,0}};}
 bool admit(char k,uint32_t a,uint32_t b=0){if(++calls==hold && !held){held=true;return false;}trace.push_back({k,a,b});return true;}
 AnimationSelectionServices services(){AnimationSelectionServices s;s.user=this;
  s.word=[](void* u,uint32_t a)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(u);auto i=f.ram.find(a);if(i==f.ram.end() || !f.admit('r',a))return {};auto v=i->second;if(f.after)f.after(f,'r',a);return v;};
  s.store=[](void* u,uint32_t a,uint32_t v){auto& f=*static_cast<Fixture*>(u);if(!f.admit('w',a,v))return false;f.ram[a]=v;if(f.after)f.after(f,'w',a);return true;};
  s.compare=[](void* u,uint32_t a,uint32_t b,int32_t& v){auto& f=*static_cast<Fixture*>(u);if(!f.admit('c',a,b))return false;v=a==T+64?0:1;if(f.after)f.after(f,'c',a);return true;};
  s.divide=[](void* u,uint64_t n,uint32_t d,uint64_t& q){auto& f=*static_cast<Fixture*>(u);assert(d==40500);if(!f.admit('d',uint32_t(n>>32),uint32_t(n)))return false;q=0x1122334455667788ull;return true;};return s;
 }
 void finish(AnimationSelectionCursor& c,uint32_t selector=S,uint32_t force=1){for(unsigned i=0;i<10000;++i)if(selectAnimation(0,selector,force,services(),c,1))return;assert(false);}
};
void exactAndHolds(){
 Fixture f;f.ram[P+0x90]=123;AnimationSelectionCursor c;f.finish(c);assert(!c.active && f.ram[P+0x14]==1 && f.ram[P]==7 && f.ram[P+0x1c]==0x55667788 && f.ram[P+0x18]==0x11223344 && f.ram[P+0x3c]==0xffffffff && !f.ram[P+0x40] && !f.ram[P+0x80] && !f.ram[P+0x84]);
 const std::vector<Op> expected={{'r',0x80415dc0,0},{'r',W+0x10,0},{'r',W,0},{'r',P+0x10,0},{'r',J+40,0},{'r',E+0xa0,0},{'r',H,0},{'r',D+0x148,0},{'r',D+0x1ac,0},{'c',T,S},{'r',D+0x1ac,0},{'c',T+64,S},{'w',P+0x14,1},{'r',P+0x90,0},{'r',P,0},{'w',P,7},{'r',P+0xc,0},{'r',0x800000f8,0},{'r',0x80415fd0,0},{'r',G+0x40,0},{'r',G+0x44,0},{'d',0x12345678,0xabcdef00},{'w',P+0x1c,0x55667788},{'r',0x8041fb28,0},{'w',P+0x18,0x11223344},{'w',P+0x3c,0xffffffff},{'w',P+0x40,0},{'w',P+0x80,0},{'w',P+0x84,0}};assert(f.trace==expected);
 for(unsigned hold=1;hold<=expected.size();++hold){Fixture x;x.ram[P+0x90]=123;x.hold=hold;AnimationSelectionCursor h;x.finish(h);assert(x.held && x.trace==expected && x.ram==f.ram);}
 Fixture id;AnimationSelectionCursor i;assert(!selectAnimation(0,S,1,id.services(),i,1));auto services=id.services();services.compare=nullptr;const auto count=id.trace.size();assert(!selectAnimation(0,S,1,services,i) && !selectAnimation(1,S,1,id.services(),i) && !selectAnimation(0,S+1,1,id.services(),i) && !selectAnimation(0,S,2,id.services(),i) && id.trace.size()==count);id.finish(i);
}
void domainsAndFreshValues(){
 for(auto n:{0u,0xffffffffu,3u})for(auto selector:std::array<uint32_t,5>{{0,1,3,0x7fffffff,S}}){Fixture f;f.ram[D+0x148]=n;AnimationSelectionCursor c;f.finish(c,selector);const uint32_t expected=n==3 && (selector==1 || selector==S)?1:0;assert(f.ram[P+0x14]==expected);}
 Fixture early;early.ram[P+0x14]=1;early.ram.erase(P+0xc);early.ram.erase(0x800000f8);AnimationSelectionCursor e;early.finish(e,1,0);assert(early.trace.back()==Op({'r',P+0x14,0}));for(auto x:early.trace)assert(x.kind!='w');
 Fixture forced;forced.ram.erase(P+0x14);AnimationSelectionCursor f;forced.finish(f,1,1);for(auto x:forced.trace)assert(!(x.kind=='r' && x.a==P+0x14));
 Fixture fresh;fresh.ram[P+0xc]=0xffffffff;fresh.after=[](Fixture& f,char k,uint32_t a){if(k=='c' && a==T){f.ram[D+0x1ac]=T-64;f.ram[D+0x148]=0;}if(k=='w' && a==P+0x1c)f.ram[0x8041fb28]=0x80000000;if(k=='r' && a==0x8041fb28)f.ram[0x8041fb28]=0;};AnimationSelectionCursor x;fresh.finish(x);assert(fresh.ram[P+0x14]==2 && fresh.ram[P+0x40]==0x80000000 && fresh.ram[P+0x84]==0x80000000);bool nonzero=false;for(auto o:fresh.trace)if(o.kind=='r' && o.a==G+0x38)nonzero=true;assert(nonzero);
 // Original raw handle multiplication/address addition wraps, with no ID gate.
 Fixture wrap;AnimationSelectionCursor w;const uint32_t h=0x10000000,base=P-h*0x170u;wrap.ram[W+0x10]=base;assert(selectAnimation(h,0,1,wrap.services(),w));assert(wrap.ram[P+0x14]==0);
}
void nativeOwner(){
 assert(loadFieldPlayerPoseAssets("romfs"));EventNativePlatform platform;platform.flushBlock=[](void*,uint32_t){};platform.systemCall=[](void*){};auto native=NativeFieldAnimation::create("romfs",NativeFileCache::create(),platform);assert(native);auto pool=native->pool();auto h=pool->enter(fieldPlayerPoseResources()[0],2,{});assert(h);auto& process=native->process();auto& memory=process.vm().memory;auto* pose=pool->pose(*h);assert(pose);
 const auto resource=pose->resource;uint32_t index=0;for(unsigned i=0;i<resource->names.size();++i)if(resource->names[i]=="M_W_1")index=i;
 const uint32_t name=0x81800000;for(unsigned i=0;i<=5;++i)assert(memory.writeByte(name+i,i<5?uint8_t("M_W_1"[i]):0));
 assert(!process.setAnimation(uint32_t(*h),name,1,1) && process.selectionPending());const auto phase=process.selectionPhase();const auto old=pose->state;assert(!process.setAnimation(uint32_t(*h)+1,name,1) && !process.setAnimation(uint32_t(*h),name+1,1) && process.selectionPhase()==phase && pose->state==old);
 assert(!native->tick(false) && !native->release(*h) && !process.main(uint32_t(*h),1) && !process.reservePresentation(uint32_t(*h),1) && !process.advance());
 pose->nativeGeneration++;assert(!process.setAnimation(uint32_t(*h),name,1));pose->nativeGeneration--;bool done=false;for(unsigned i=0;i<1000 && !done;++i)done=process.setAnimation(uint32_t(*h),name,1,1);assert(done && !process.selectionPending() && pose->word(0x14)==index && pose->word(0x3c)==0xffffffff && !pose->word(0x84));
 // Native strcmp keeps a captured left byte when its right byte is absent.
 // Redirect the NEXT directory load so a restarted comparison cannot quietly
 // fall back to row0 and masquerade as a successful retained comparison.
 auto read=[&](uint32_t a){uint32_t v;assert(memory.readWord(a,v));return v;};
 const auto work=read(0x80415dc0),model=read(read(read(read(work)+16*pose->word(0x10)+8)+0xa0)),directory=read(model+0x1ac);
 constexpr uint32_t lateName=0x83800000;uint8_t first;assert(memory.readByte(directory,first));
 assert(!process.setAnimation(uint32_t(*h),lateName,1) && process.selectionPhase()==11);
 assert(memory.writeByte(directory,uint8_t(first^1)) && memory.writeWord(model+0x1ac,0x83900000));
 std::vector<uint8_t> originalName(resource->names[0].begin(),resource->names[0].end());originalName.push_back(0);
 assert(memory.registerRam(lateName,std::move(originalName)) && process.setAnimation(uint32_t(*h),lateName,1));
 assert(memory.writeByte(directory,first) && memory.writeWord(model+0x1ac,directory));
 // Equal force0 succeeds with the clock entirely unavailable. A changed setter
 // publishes selection before holding at that original clock frontier.
 assert(memory.unregisterRam(0x800000f8));const auto same=pose->state;assert(process.setAnimation(uint32_t(*h),0,0) && pose->state==same);assert(!process.setAnimation(uint32_t(*h),0,1) && process.selectionPending() && process.selectionPhase()==19);assert(memory.registerRam(0x800000f8,{0x09,0xa7,0xec,0x80}));assert(process.setAnimation(uint32_t(*h),0,1));
 // A setup hold must not replay a completed selection when the separate
 // admitted local-time adapter rejects a changed scale before conversion.
 MarioPresentation request;request.supported=true;request.poseUploaded=true;request.selected="M_W_1";request.poseUploadSerial=17;request.poseStartFrame=1;
 const std::array<int32_t,3> handles{{*h,*h,*h}};
 assert(!native->mario(request,handles,1) && process.selectionPending());
 assert(memory.writeWord(0x8041fb40,0x7fc00000));
 for(unsigned i=0;i<1000 && process.selectionPending();++i)assert(!native->resumeMario(1));
 assert(!process.selectionPending() && native->presentationPending() && !native->resumeMario(8192));
 assert(memory.writeWord(0x8041fb40,0x41855555));
 assert(!native->resumeMario(1) && !process.selectionPending());
 for(unsigned i=0;i<10000 && native->presentationPending();++i)native->resumeMario(8192);
 assert(!native->presentationPending() && native->marioPose());
}
}
int main(){exactAndHolds();domainsAndFreshValues();nativeOwner();}
