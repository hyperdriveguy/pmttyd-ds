#include "animation_texture_binding.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <map>
#include <functional>
#include <memory>
using namespace ttyd;
namespace {
using namespace animation_texture_source;
constexpr uint32_t Ids=0x82000000,Bases=Ids+0x100,States=Ids+0x200,Descriptors=Ids+0x400,Resource=Ids+0x600,Handle=Ids+0x800,Pointer=Ids+0xa00,Payload=Ids+0xc00,Table=Ids+0xe00,Image=Ids+0x1000,Palette=Ids+0x1100;
constexpr uint32_t Targets[]{0x8003e178,0x8003e198,0x8003e1a0,0x8003e1a8,0x8003e1c0,0x8003e1c8,0x8003e1e0,0x8003e1f8};
uint32_t bits(float f){uint32_t w;std::memcpy(&w,&f,4);return w;}float real(uint32_t w){float f;std::memcpy(&f,&w,4);return f;}
struct Event {uint32_t kind=0,a=0,b=0;bool operator==(const Event& e)const{return kind==e.kind && a==e.a && b==e.b;}};
struct Probe {
 std::map<uint32_t,uint8_t> ram;std::vector<Event> trace;std::vector<AnimationTextureCall> calls;std::vector<AnimationSourceMatrixCall> matrices;
 unsigned stop=~0u;bool stopped=false;std::function<void(const AnimationTextureCall&,AnimationTextureLocals&)> effect;
 std::function<void(const AnimationSourceMatrixCall&)> matrixEffect;
 std::unique_ptr<AnimationSourceTextureCursor> cursorOwner=std::make_unique<AnimationSourceTextureCursor>();
 AnimationSourceTextureCursor& cursor=*cursorOwner;AnimationSourceTextureInput input{1,Ids,Bases,States,1,Descriptors,Resource};
 void word(uint32_t a,uint32_t v){for(unsigned i=0;i<4;++i)ram[a+i]=uint8_t(v>>(24-8*i));}
 void half(uint32_t a,uint16_t v){ram[a]=v>>8;ram[a+1]=v;}
 bool admit(){if(trace.size()==stop && !stopped){stopped=true;return false;}return true;}
 std::optional<uint32_t> read(uint32_t a,unsigned n){if(!admit())return {};uint32_t v=0;for(unsigned i=0;i<n;++i){auto p=ram.find(a+i);if(p==ram.end())return {};v=v*256+p->second;}trace.push_back({n,a,v});return v;}
 void image(uint32_t p,uint32_t data=0x12340000){half(p,8);half(p+2,16);word(p+4,6);word(p+8,data);word(p+12,1);word(p+16,2);word(p+20,5);word(p+24,1);word(p+28,bits(.25f));ram[p+32]=1;ram[p+33]=2;ram[p+34]=4;}
 Probe(){for(unsigned i=0;i<9;++i){word(Ids+4*i,0);}word(Bases,0);word(Bases+4,0xffffffff);ram[States]=0;for(unsigned i=1;i<6;++i)word(States+4*i,bits(i==3 || i==4?1.f:0.f));word(Descriptors+4,0);word(Descriptors+8,0);word(Resource+8,Handle);word(Handle+0xa0,Pointer);word(Pointer,Payload);word(Payload+8,Table);word(Table,Image);word(Table+4,0);image(Image);half(Palette,16);word(Palette+4,2);word(Palette+8,0x45670000);
  word(Zero,0);word(One,bits(1));word(Half,bits(.5f));word(NegativeHalf,bits(-.5f));word(Degrees,0x3c8efa35);word(ByteBias,0x43300000);word(ByteBias+4,0);for(unsigned i=0;i<8;++i)word(Dispatch+4*i,Targets[i]);
 }
 AnimationSourceTextureServices services(){AnimationSourceTextureServices s;s.user=this;
  s.word=[](void* u,uint32_t a){return static_cast<Probe*>(u)->read(a,4);};
  s.doubleWord=[](void* u,uint32_t a)->std::optional<std::array<uint32_t,2>>{auto& p=*static_cast<Probe*>(u);if(!p.admit())return {};std::array<uint32_t,2> pair{};for(unsigned i=0;i<8;++i){auto b=p.ram.find(a+i);if(b==p.ram.end())return {};pair[i/4]=pair[i/4]*256+b->second;}p.trace.push_back({8,a,pair[0]^pair[1]});return pair;};
  s.byte=[](void* u,uint32_t a)->std::optional<uint8_t>{auto v=static_cast<Probe*>(u)->read(a,1);return v?std::optional<uint8_t>(*v):std::nullopt;};
  s.half=[](void* u,uint32_t a)->std::optional<uint16_t>{auto v=static_cast<Probe*>(u)->read(a,2);return v?std::optional<uint16_t>(*v):std::nullopt;};
  s.number=[](void* u,AnimationTextureArithmetic op,std::array<uint32_t,3> a)->std::optional<uint32_t>{auto& p=*static_cast<Probe*>(u);if(!p.admit())return {};p.trace.push_back({20+uint32_t(op),a[0],a[1]});if(op==AnimationTextureArithmetic::ByteToFloat){assert(a[1]==0x43300000 && a[2]==0);return bits(float(a[0]));}volatile float out=op==AnimationTextureArithmetic::Multiply?real(a[0])*real(a[1]):real(a[0])-real(a[1]);return bits(out);};
  s.call=[](void* u,const AnimationTextureCall& q,AnimationTextureLocals& local,uint32_t& result){auto& p=*static_cast<Probe*>(u);if(!p.admit())return false;p.trace.push_back({10,q.function,q.arguments[0].value});p.calls.push_back(q);if(q.function==Get){auto a=p.read(q.arguments[0].value+8,4);if(!a){p.trace.pop_back();p.calls.pop_back();return false;}result=*a+8*q.arguments[1].value;}if(p.effect)p.effect(q,local);return true;};
  s.matrix=[](void* u,const AnimationSourceMatrixCall& q,AnimationTextureLocals& l){auto& p=*static_cast<Probe*>(u);if(!p.admit())return false;p.trace.push_back({11,q.function,q.output.value});p.matrices.push_back(q);l.matrices[q.output.value]={1,0,0,0,0,1,0,0,0,0,1,0};l.matrixAvailable[q.output.value]=true;if(p.matrixEffect)p.matrixEffect(q);return true;};return s;
 }
 bool run(unsigned visits=128){return resumeAnimationSourceTexture(input,services(),cursor,visits);}
 void drain(){for(unsigned i=0;i<1000;++i)if(run())return;assert(false);}
};
void ordinaryAndAllHolds(){
 Probe baseline;baseline.word(States+20,bits(30));baseline.drain();
 std::vector<uint32_t> funcs;for(const auto& q:baseline.calls)funcs.push_back(q.function);
 assert((funcs==std::vector<uint32_t>{Get,Get,Init,Lod,Load,Generator,LoadMatrix}));
 assert(baseline.matrices.size()==7); // Five rotation calls, translation, then R*M.
 assert(baseline.matrices[0].xyz[0]==.5f && baseline.matrices[1].axis==122);
 assert(baseline.calls[2].arguments[2].value==16 && baseline.calls[2].arguments[3].value==8 && baseline.calls[2].arguments[5].value==1 && baseline.calls[2].arguments[6].value==2);
 assert(baseline.calls[3].reals[0]==bits(2) && baseline.calls[3].reals[1]==bits(4) && baseline.calls[3].reals[2]==bits(.25f));
 for(unsigned i=0;i<baseline.trace.size();++i){Probe held;held.word(States+20,bits(30));held.stop=i;held.drain();assert(held.stopped && held.trace==baseline.trace && held.calls.size()==baseline.calls.size() && held.matrices.size()==baseline.matrices.size());}
 Probe budget;budget.word(States+20,bits(30));while(!budget.run(1)){}assert(budget.trace==baseline.trace);
}
void liveHeadersAndCapturedCi(){
 Probe p;p.word(Table+4,Palette);const uint32_t image2=Image+0x400,image3=Image+0x800;p.image(image2,0x22220000);p.image(image3,0x33330000);p.word(image3+20,77);p.ram[image3+33]=9;
 p.effect=[&](const auto& q,auto&){if(q.function==InitPalette){p.word(Table,image2);p.word(image2+12,99);p.ram[image2+33]=0;p.ram[image2+34]=0;}if(q.function==InitCi)p.word(Table,image3);};p.drain();
 std::vector<uint32_t> fs;for(const auto& q:p.calls)fs.push_back(q.function);assert((fs==std::vector<uint32_t>{Get,Get,InitPalette,InitCi,Lod,LoadPalette,Load,Generator,LoadMatrix}));
 assert(p.calls[3].arguments[1].value==0x22220000 && p.calls[3].arguments[5].value==1 && p.calls[3].arguments[7].value==1);assert(p.calls[4].arguments[1].value==77 && p.calls[4].reals[0]==bits(9));
 // The second getter re-resolves U. Its palette does not reclassify branch.
 Probe branch;unsigned gets=0;branch.effect=[&](const auto& q,auto&){if(q.function==Get && ++gets==1){branch.word(Resource+8,Handle+0x400);branch.word(Handle+0x4a0,Pointer+0x400);branch.word(Pointer+0x400,Payload+0x400);branch.word(Payload+0x408,Table+0x400);branch.word(Table+0x400,Image);branch.word(Table+0x404,Palette);}};branch.drain();assert(branch.calls[1].arguments[0].value==Payload+0x400);assert(branch.calls[2].function==Init);
 Probe ci;ci.word(Table+4,Palette);gets=0;ci.effect=[&](const auto& q,auto&){if(q.function==Get && ++gets==2)ci.word(Table+4,0);};assert(!ci.run() && ci.cursor.ci && ci.cursor.phase==45);for(const auto& q:ci.calls)assert(q.function!=Init);
}
void capturesBranchesAndUnboundedSlots(){
 Probe empty;empty.input.count=0xffffffff;empty.ram.clear();empty.word(Zero,bits(7));assert(!empty.run() && empty.cursor.phase==101);empty.word(Zero,bits(99));empty.word(One,bits(1));assert(empty.run() && empty.cursor.zero==bits(7) && empty.trace.size()==2);
 Probe atomic;for(unsigned i=4;i<8;++i)atomic.ram.erase(ByteBias+i);assert(!atomic.run() && atomic.cursor.phase==59 && atomic.cursor.biasHi==0);atomic.word(ByteBias,0x43300001);atomic.word(ByteBias+4,0);assert(!atomic.run(1) && atomic.cursor.biasHi==0x43300001 && atomic.cursor.phase==61);
 Probe nine;nine.input.count=9;nine.word(Descriptors+8,1);nine.drain();unsigned generators=0;for(const auto& q:nine.calls)if(q.function==Generator)++generators;assert(generators==9);
 Probe live;live.word(States+20,bits(10));live.matrixEffect=[&](const auto& q){if(q.function==0x8029e230 && q.output.value==AnimationTextureLocals::Scratch){live.word(Degrees,bits(2));live.word(One,bits(4));live.word(States+4,bits(100));}};live.drain();assert(live.matrices[1].radians==-20 && live.matrices[5].xyz[0]==0 && live.matrices[5].xyz[1]==3); // Captured state; live operation literals.
 Probe dispatch;dispatch.word(Descriptors+8,1);dispatch.word(Dispatch+8,0xdeadbeef);assert(!dispatch.run() && dispatch.cursor.phase==153);dispatch.word(Dispatch+8,Targets[2]);assert(!dispatch.run() && dispatch.cursor.target==0xdeadbeef);
}

void diagnosticsAndLiveSecondPass(){
 Probe p;const uint32_t work=Ids+0x2000,pose=work+0x200,files=work+0x400,file=work+0x600,ptr=work+0x800,model=work+0xa00,names=work+0xc00;
 p.word(Bases,7);p.input.descriptorCount=0;p.word(Work,work);p.word(work+0x18,pose);p.word(work,files);p.word(pose+0x10,2);p.word(pose+0x14,3);p.word(files+16*2+8,file);p.word(file+0xa0,ptr);p.word(ptr,model);p.word(model+0x1ac,names);
 p.effect=[&](const auto& q,auto&){if(q.function==Diagnostic)p.word(Bases+4,12);if(q.function==Load){p.word(Ids,1);for(unsigned i=1;i<6;++i)p.word(States+24+4*i,bits(i==3 || i==4?1.f:i==1?13.f:0.f));}};
 p.drain();assert(p.calls[0].function==Diagnostic && p.calls[0].arguments[1].value==model+4 && p.calls[0].arguments[2].value==names+64*3);assert(p.calls[3].function==Init && p.calls[3].arguments[5].value==2 && p.calls[3].arguments[6].value==2);assert(p.matrices[0].xyz[0]==13);
}
void allDispatchTargetsAndCiHolds(){
 for(unsigned target=0;target<8;++target){Probe p;p.word(Descriptors+8,1);p.word(States+20,bits(45));p.word(States+12,bits(2));p.word(States+16,bits(3));p.word(Dispatch+28,Targets[target]);p.drain();assert(p.cursor.mask==7);assert(p.calls[0].function==Generator && p.calls[0].arguments[3].value==(target?30u:60u));assert(p.calls.size()==(target?2u:1u));assert(p.matrices.size()==7+(target==7?2:target==3 || target==5 || target==6?1:0));
 const unsigned selected[]{0,3,2,0,1,0,0,2};if(target)assert(p.calls[1].arguments[0].value==selected[target]);
 if(target==3 || target==5 || target==6 || target==7){const auto& q=p.matrices[7];assert(q.function==0x8029dd64 && q.left.value==(target==6?2u:3u) && q.right.value==(target==5 || target==6?1u:2u) && q.output.value==0);}
 if(target==7){const auto& q=p.matrices[8];assert(q.function==0x8029dd64 && q.left.value==0 && q.right.value==1 && q.output.value==2);}}
 Probe original;original.word(Table+4,Palette);original.drain();
 for(unsigned i=0;i<original.trace.size();++i){Probe held;held.word(Table+4,Palette);held.stop=i;held.drain();assert(held.stopped && held.trace==original.trace);}
}
struct Ram {
 EventEntryAddresses entries;EventContextAddresses contexts{entries};EventRuntimeGlobals globals{0,1,1,0};EventSchedule schedule;EventRuntimeAddresses runtime{globals,schedule};EventGpAddresses gp;EventAddressSpace memory{entries,contexts,runtime,gp};
};
void actualVmAndSdk(bool complex=false){
 auto r=std::make_unique<Ram>();Probe p;if(complex){p.word(States+4,bits(4));p.word(States+8,bits(5));p.word(States+12,bits(2));p.word(States+16,bits(3));p.word(States+20,bits(90));}EventScriptAssets original;assert(original.load("romfs/animation-boot/memory.pmevt"));
 for(const auto span:std::array<std::pair<uint32_t,unsigned>,9>{{{Zero,4},{One,4},{Half,4},{NegativeHalf,4},{Degrees,4},{ByteBias,8},{Dispatch,32},{animation_sdk::Zero-4,8},{animation_sdk::ConcatTail,8}}})for(unsigned i=0;i<span.second;++i){uint8_t b;assert(original.readByte(span.first+i,b));p.ram[span.first+i]=b;}

 for(auto i=p.ram.begin();i!=p.ram.end();){const uint32_t base=i->first;std::vector<uint8_t> bytes;do{bytes.push_back(i->second);++i;}while(i!=p.ram.end() && i->first==base+bytes.size());assert(r->memory.registerRam(base,std::move(bytes)));}
 bool current=true,ready=false;unsigned calls=0;AnimationTextureLocals* first=nullptr;
 struct Backend {bool* ready;unsigned* calls;AnimationTextureLocals** first;bool complex;} backend{&ready,&calls,&first,complex};
 AnimationTextureBackend child{&backend,[](void* u,const AnimationTextureCall& q,AnimationTextureLocals& local,uint32_t&){auto& b=*static_cast<Backend*>(u);if(!*b.first)*b.first=&local;assert(*b.first==&local);if(!*b.ready)return false;++*b.calls;if(q.function==LoadMatrix){const auto slot=q.arguments[0].value;assert(local.matrixAvailable[slot]);const auto& m=local.matrices[slot];if(!b.complex)assert(m[0]==1 && m[5]==1 && m[10]==1 && m[3]==0 && m[7]==0);
 else{const BattleActMatrix expected{0,3,0,-7,-2,0,0,-3,0,0,1,0};for(unsigned i=0;i<12;++i)assert(std::abs(m[i]-expected[i])<1e-5f);}}return true;}};
 AnimationTextureBinding binding(r->memory,child,{},[&]{return current;});AnimationRenderLocals local;uint32_t result=91;AnimationRenderCall q;q.function=Function;q.count=7;const uint32_t values[]{1,Ids,Bases,States,1,Descriptors,Resource};for(unsigned i=0;i<7;++i)q.arguments[i]={i==0 || i==4?AnimationRenderKind::Scalar:AnimationRenderKind::Guest,values[i]};auto s=binding.services();assert(!s.call(s.user,q,local,result) && binding.active() && !calls);
 auto changed=q;changed.arguments[3].value=States+24;assert(!s.call(s.user,changed,local,result));AnimationRenderLocals wrong;assert(!s.call(s.user,q,wrong,result));assert(!s.fifo(s.user,0xcc008000,0));current=false;assert(!s.call(s.user,q,local,result));current=true;ready=true;for(unsigned i=0;i<10 && binding.active();++i)s.call(s.user,q,local,result);assert(!binding.active() && calls==5 && result==91);
}
}
int main(){ordinaryAndAllHolds();liveHeadersAndCapturedCi();capturesBranchesAndUnboundedSlots();diagnosticsAndLiveSecondPass();allDispatchTargetsAndCiHolds();actualVmAndSdk();actualVmAndSdk(true);}
