#include "animation_render_sdk_binding.hpp"
#include "animation_render_binding.hpp"
#include "animation_native_inner.hpp"
#include "native_field_animation.hpp"
#include "field_npc_main_entry_binding.hpp"
#include "field_party.hpp"
#include "animation_shape_render.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
uint32_t bits(float f){uint32_t w;std::memcpy(&w,&f,4);return w;}float real(uint32_t w){float f;std::memcpy(&f,&w,4);return f;}
using K=AnimationRenderKind;using A=AnimationRenderArgument;
constexpr uint32_t M=0x82000000,Left=0x82001000,Out=0x82002000;
BattleActMatrix identity(){return {1,0,0,0,0,1,0,0,0,0,1,0};}
AnimationRenderCall call(uint32_t fn,std::initializer_list<A> args){AnimationRenderCall q;q.function=fn;q.count=args.size();unsigned i=0;for(auto a:args)q.arguments[i++]=a;return q;}
void select(CameraManager& bank,unsigned index){
 for(unsigned i=0;i<13;++i){bank.camera(i)->publication.flags=1;}
 bank.camera(index)->publication.flags=0;
 auto queue=std::make_unique<DisplayQueue>();CameraDrawCursor cursor;CameraDrawServices s;s.letterbox=[](void*,DisplayQueue&){return true;};s.request=[](void*,const CameraDrawRequest&){return false;};assert(!bank.draw(*queue,s,cursor) && bank.currentIndex()==index);
}
struct Ram {
 EventEntryAddresses entries;EventContextAddresses contexts{entries};EventRuntimeGlobals globals{0,1,1,0};EventSchedule schedule;EventRuntimeAddresses runtime{globals,schedule};EventGpAddresses gp;EventAddressSpace memory{entries,contexts,runtime,gp};
 bool valid=true;unsigned delegates=0,fifo=0;std::shared_ptr<CameraManager> cameras=std::make_shared<CameraManager>();
 void matrix(uint32_t a,const BattleActMatrix& m,bool allocate=true){if(allocate)assert(memory.registerRam(a,std::vector<uint8_t>(48)));for(unsigned i=0;i<12;++i)assert(memory.writeWord(a+4*i,bits(m[i])));}
 AnimationSourceRenderChildren backend(){return {this,[](void* u,const AnimationRenderCall&,AnimationRenderLocals&,uint32_t&){++static_cast<Ram*>(u)->delegates;return true;},[](void* u,uint32_t,uint16_t){++static_cast<Ram*>(u)->fifo;return true;}};}
 AnimationRenderSdkBinding binding(){return AnimationRenderSdkBinding(memory,cameras,backend(),[this]{return valid;});}
 void view(unsigned index,const BattleActMatrix& m){auto& c=*cameras->camera(index);for(unsigned r=0;r<3;++r)for(unsigned k=0;k<4;++k)c.matrices.view[r][k]=m[4*r+k];c.viewPublished=true;}
 void tail(){assert(memory.registerRam(0x8041e458,{0,0,0,0,0x3f,0x80,0,0}));}
};
bool invoke(AnimationRenderSdkBinding& b,const AnimationRenderCall& q,AnimationRenderLocals& l,uint32_t& result){auto s=b.services();return s.call(s.user,q,l,result);}
void cameraAndLazyView(){
 Ram f;auto b=f.binding();AnimationRenderLocals l;uint32_t result=99;const auto current=call(0x8000f2e4,{}),camera4=call(0x8000f2fc,{{K::Scalar,4}});
 assert(!invoke(b,current,l,result) && result==99 && !f.delegates);f.cameras->camera(4)->mode=2;f.cameras->camera(4)->projectionKind=1;assert(invoke(b,camera4,l,result) && result==AnimationRenderSdkBinding::CameraBase+4*0x280 && !f.cameras->camera(4)->viewPublished);
 select(*f.cameras,7);assert(invoke(b,current,l,result));const uint32_t token=result;assert(token==AnimationRenderSdkBinding::CameraBase+7*0x280);f.tail();f.matrix(M,identity());auto concat=call(0x8029dd64,{{K::Guest,token+0x11c},{K::Guest,M},{K::Matrix,0}});
 assert(!invoke(b,concat,l,result) && !l.matrices[0] && !f.delegates);assert(!invoke(b,camera4,l,result)); // A held owned SDK call cannot become a getter or external call.
 auto external=call(0x802ba2fc,{{K::Matrix,0},{K::Scalar,0}});assert(!invoke(b,external,l,result) && !f.delegates);auto fifo=b.services();assert(!fifo.fifo(fifo.user,0xcc008000,1) && !f.fifo);
 select(*f.cameras,4);auto view=identity();view[3]=7;f.view(7,view);f.view(4,identity());assert(invoke(b,concat,l,result) && l.matrices[0] && (*l.matrices[0])[3]==7);assert(invoke(b,current,l,result) && result==AnimationRenderSdkBinding::CameraBase+4*0x280);assert(invoke(b,camera4,l,result) && result==AnimationRenderSdkBinding::CameraBase+4*0x280);
 assert(invoke(b,external,l,result) && f.delegates==1);
 // No-camera shape-1 returns before any bridge admission; missing current holds.
 AnimationRenderBinding source(f.memory,b.services(),[&]{return f.valid;});assert(source.advance(0xffffffff));
 Ram absent;auto empty=absent.binding();assert(!invoke(empty,current,l,result));
 assert(absent.memory.registerRam(AnimationRenderSdkBinding::CameraBase,std::vector<uint8_t>(4)));select(*absent.cameras,4);assert(!invoke(empty,camera4,l,result) && !absent.delegates);
}
void cameraCaptureAndRowRepair(){
 Ram f;select(*f.cameras,4);f.view(4,identity());f.tail();assert(f.memory.registerRam(M,std::vector<uint8_t>(8)));assert(f.memory.writeWord(M,bits(1)));auto b=f.binding();AnimationRenderLocals l;uint32_t result=0;
 auto concat=call(0x8029dd64,{{K::Guest,AnimationRenderSdkBinding::CameraBase+4*0x280+0x11c},{K::Guest,M},{K::Matrix,0}});
 assert(!invoke(b,concat,l,result) && !l.matrices[0]); // Captured A00 and B00; B08 unavailable.
 auto changed=concat;changed.arguments[2].value=1;assert(!invoke(b,changed,l,result));AnimationRenderLocals wrong;assert(!invoke(b,concat,wrong,result));
 f.cameras->camera(4)->matrices.view[0][0]=8;f.cameras->camera(4)->matrices.view[1][1]=3;assert(f.memory.writeWord(M,bits(99)));assert(f.memory.registerRam(M+8,std::vector<uint8_t>(40)));for(unsigned i=2;i<12;++i)assert(f.memory.writeWord(M+4*i,bits(identity()[i])));
 f.valid=false;assert(!invoke(b,concat,l,result));f.valid=true;assert(invoke(b,concat,l,result));assert((*l.matrices[0])[0]==1 && (*l.matrices[0])[5]==3); // A00/B00 pinned, later A10 remains fresh.
 // Unsupported bytes inside a token never fall through to real RAM.
 auto bad=call(0x8029dd64,{{K::Guest,AnimationRenderSdkBinding::CameraBase+4*0x280+0x15c},{K::Guest,M},{K::Matrix,0}});assert(!invoke(b,bad,l,result) && !f.delegates);
}
void normalAndPoint(){
 Ram f;auto b=f.binding();AnimationRenderLocals l;uint32_t status=99;
 auto normal=call(0x8029df28,{{K::Matrix,0},{K::Matrix,1}});l.matrices[0]=BattleActMatrix{2,0,0,real(0x7fc12345),0,4,0,INFINITY,0,0,8,-INFINITY};assert(invoke(b,normal,l,status) && status==1);assert(bits((*l.matrices[1])[0])==0x3f000000 && bits((*l.matrices[1])[5])==0x3e800000 && bits((*l.matrices[1])[10])==0x3e000000);assert((*l.matrices[1])[3]==0 && (*l.matrices[1])[7]==0 && (*l.matrices[1])[11]==0);
 auto singular=identity();singular[5]=0;l.matrices[0]=singular;l.matrices[1].reset();assert(invoke(b,normal,l,status) && !status && !l.matrices[1]);BattleActMatrix prior;for(auto& x:prior)x=real(0x7fc12345);l.matrices[1]=prior;assert(invoke(b,normal,l,status) && !status && std::memcmp(l.matrices[1]->data(),prior.data(),48)==0);
 auto tiny=identity();tiny[0]=tiny[5]=tiny[10]=1e-8f;l.matrices[0]=tiny;assert(invoke(b,normal,l,status) && status==1 && !std::isfinite((*l.matrices[1])[0]));
 auto point=call(0x8029e6a4,{{K::Guest,M},{K::Vector,0},{K::Vector,0}});auto transform=identity();transform[0]=2;transform[5]=3;transform[10]=4;transform[3]=4;transform[7]=5;transform[11]=6;
 assert(f.memory.registerRam(M,std::vector<uint8_t>(32)));for(unsigned i=0;i<8;++i)assert(f.memory.writeWord(M+4*i,bits(transform[i])));l.vectors[0]={bits(1),bits(2),bits(3)};
 assert(!invoke(b,point,l,status) && l.vectors[0][0]==bits(6) && l.vectors[0][1]==bits(2) && l.vectors[0][2]==bits(3));
 auto changed=point;changed.arguments[0].value=M+4;assert(!invoke(b,changed,l,status));assert(!invoke(b,normal,l,status) && !f.delegates);AnimationRenderLocals wrong;assert(!invoke(b,point,wrong,status));
 assert(f.memory.writeWord(M,bits(99)));l.vectors[0][0]=bits(100);l.vectors[0][1]=bits(200);l.vectors[0][2]=bits(300);f.valid=false;assert(!invoke(b,point,l,status));f.valid=true;
 assert(f.memory.registerRam(M+32,std::vector<uint8_t>(16)));for(unsigned i=8;i<12;++i)assert(f.memory.writeWord(M+4*i,bits(transform[i])));assert(invoke(b,point,l,status));assert((l.vectors[0]==std::array<uint32_t,3>{bits(100),bits(11),bits(18)})); // completed X stays; captured inputYZ drives later outputs.
 // Independent paired arithmetic goldens, rather than a duplicated expression.
 auto cancellation=animationShapeNativePointRow({16777216.f,-16777216.f,1,0},{1,1,1});assert(cancellation && bits(*cancellation)==0);
 auto fused=animationShapeNativePointRow({-1,0,real(0x3f800001),0},{1,0,real(0x3f7ffffe)});assert(fused && bits(*fused)==0xa8800000);
 auto negative=animationShapeNativePointRow({-0.f,-0.f,-0.f,-0.f},{1,1,1});assert(negative && bits(*negative)==0x80000000);
}
void sharedSdkTraceAndStorage(){
 Ram f;f.matrix(Out,identity());
 struct Probe {std::map<uint32_t,uint32_t> words;std::vector<uint32_t> reads;unsigned hold=~0u;bool held=false;AnimationSourceMatrixCall* mutate=nullptr;std::optional<BattleActMatrix> output;} p;
 for(unsigned i=0;i<12;++i){p.words[M+4*i]=bits(identity()[i]);p.words[Left+4*i]=bits(identity()[i]);}p.words[0x8041e458]=0;p.words[0x8041e45c]=bits(1);
 AnimationSdkGuestPairs pairs{&p,[](void* u,uint32_t a){auto& p=*static_cast<Probe*>(u);return p.words.count(a)!=0;},[](void* u,uint32_t a,std::array<uint32_t,2>& words){auto& p=*static_cast<Probe*>(u);if(p.reads.size()==p.hold && !p.held){p.held=true;return false;}p.reads.push_back(a);if(p.mutate){p.mutate->output.value=Out;p.mutate=nullptr;}words={p.words.at(a),p.words.at(a+4)};return true;}};
 AnimationSdkLocals local{&p,1,nullptr,[](void* u,uint32_t slot,const BattleActMatrix& m){assert(slot==0);static_cast<Probe*>(u)->output=m;return true;}};
 AnimationSourceMatrixCall q;q.function=0x8029dd64;q.left={true,Left};q.right={true,M};q.output={false,0};auto original=q;
 AnimationSdkMatrixBinding sdk(f.memory,[&]{return f.valid;},pairs);p.mutate=&q;assert(sdk.advance(q,local) && p.output && q.output.value==Out);q=original;
 std::vector<uint32_t> expected{Left,M,M+8,M+16,Left+16,0x8041e458,M+24,Left+8,Left+24,M+32,M+40,Left+32,Left+40};assert(p.reads==expected);
 for(unsigned i=0;i<13;++i){p.reads.clear();p.hold=i;p.held=false;p.output.reset();AnimationSdkMatrixBinding held(f.memory,[&]{return f.valid;},pairs);assert(!held.advance(q,local) && !p.output);auto wrong=local;wrong.count=2;assert(!held.advance(q,wrong));assert(held.advance(q,local) && p.output && p.reads==expected);}
}

constexpr uint32_t B=0x803d8e70,N=0x81f00000;
struct NativeFixture {
 std::shared_ptr<NativeFieldAnimation> native;bool valid=true;int token=0;uint64_t pass=95;uint32_t handle=0;
 NativeFixture(){assert(loadFieldPlayerPoseAssets("romfs"));EventNativePlatform p;p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){};native=NativeFieldAnimation::create("romfs",NativeFileCache::create(),p);assert(native);const auto h=native->pool()->enter(fieldPlayerPoseResources()[0],2,{});assert(h);handle=uint32_t(*h);assert(native->pool()->selectName(*h,"M_W_1",true,{}));pose().word(0,pose().word(0)&~8u);pose().word(0xf4,0xffffff80);for(unsigned i=0;i<17;++i)assert(native->tick(false));}
 EventAddressSpace& memory(){return native->process().vm().memory;}
 BattlePose& pose(){return *native->pool()->pose(int32_t(handle));}
 uint32_t word(uint32_t a){uint32_t w;assert(memory().readWord(a,w));return w;}
 void store(uint32_t a,uint32_t w){assert(memory().writeWord(a,w));}
 bool draw(){FieldNpcDrawRequest q;q.function=0x8003b4d8;q.args={handle,0,2};q.scalars={0,1};q.matrix={1,0,0,4,0,1,0,5,0,0,1,6};FieldNpcDrawResponse r;auto s=native->npcAnimationServices();return s.request(s.user,memory(),q,r);}
 void simple(bool transformed=true){assert(memory().registerRam(N,std::vector<uint8_t>(96)));for(unsigned i=0;i<24;++i)store(N+4*i,bits(i>=3 && i<=5?1.f:0.f));if(transformed){for(unsigned i=0;i<3;++i){store(N+4*i,bits(3.f+i));store(N+0xc+4*i,bits(2.f+i));}}store(0x8041e89c,0);store(0x8041e8a0,B);const float initial[]{1,0,0,4,0,1,0,5,0,0,1,6};for(unsigned i=0;i<12;++i)store(B+4*i,bits(initial[i]));}
};
void originalGraphRealSdkAndOwnerRefresh(){
 NativeFixture f;auto& n=*f.native;
 struct Probe {bool ready=false;unsigned holds=0,positions=0,normals=0,points=0,fifo=0,legacy=0;std::optional<BattleActMatrix> first;} p;
 NativeNpcGroupBackend groups;groups.nativeRenderSdk=true;groups.renderChildren.user=&p;
 groups.renderChildren.call=[](void* u,const AnimationRenderCall& q,AnimationRenderLocals& local,uint32_t&){auto& p=*static_cast<Probe*>(u);
  assert(q.function!=0x8000f2e4 && q.function!=0x8000f2fc && q.function!=0x8029dd64 && q.function!=0x8029df28 && q.function!=0x8029e6a4);
  if(q.function==0x802ba2fc){assert(local.matrices[0]);if(!p.ready){if(!p.first)p.first=local.matrices[0];assert(std::memcmp(p.first->data(),local.matrices[0]->data(),48)==0);++p.holds;return false;}if(!p.positions && p.first)assert(std::memcmp(p.first->data(),local.matrices[0]->data(),48)==0);++p.positions;}
  if(q.function==0x802ba34c)++p.normals;
  if(q.function==0x800208c0){for(auto w:local.vectors[1])assert(std::isfinite(real(w)));++p.points;}
  return true;
 };groups.renderChildren.fifo=[](void* u,uint32_t a,uint16_t){assert(a==0xcc008000);++static_cast<Probe*>(u)->fifo;return true;};
 NativeNpcInnerBackend backend;backend.groups=groups;backend.user=&p;backend.gx=[](void*,EventAddressSpace&,const BattleAudienceGXRequest&){return true;};backend.flush=[](void*,EventAddressSpace&,uint32_t,uint32_t){return true;};backend.invalidate=[](void*,EventAddressSpace&){return true;};backend.traverse=[](void* u,EventAddressSpace&,const AnimationInnerTraversal&){++static_cast<Probe*>(u)->legacy;return true;};assert(n.configureNpcInnerBackend(backend));
 for(unsigned pass=0;pass<2;++pass){auto camera=std::make_shared<CameraManager>();select(*camera,pass?7:4);for(unsigned i:{4u,7u}){auto& c=*camera->camera(i);c.viewPublished=true;c.eye={0,0,10};c.at={0,0,0};for(unsigned row=0;row<3;++row)for(unsigned col=0;col<4;++col)c.matrices.view[row][col]=row==col?1.f:0.f;}
  assert(n.beginNpcPass(&f.token,++f.pass,camera,[&]{return f.valid;}));
  if(!pass){for(unsigned i=0;i<2000 && !p.holds;++i){assert(!f.draw());}assert(p.holds && p.first && !p.positions);const auto held=p.holds;f.valid=false;assert(!f.draw() && p.holds==held);f.valid=true;camera->camera(4)->matrices.view[0][0]=4;assert(!f.draw() && !n.endNpcPass(&f.token,f.pass) && !n.configureNpcInnerBackend(backend) && !n.tick(false));p.ready=true;}
  auto positions=p.positions;bool done=false;for(unsigned i=0;i<10000 && !done;++i)done=f.draw();assert(done && p.positions>positions && p.normals && p.points && p.fifo && !p.legacy && n.endNpcPass(&f.token,f.pass));assert(f.word(0x8041e89c)==0 && f.word(0x8041e8a0)==B);
 }
 assert(n.tick(false));
} // Original nodes, raw renderer, camera access and SDK math execute. Only GPU/material/cache effects are fixture acknowledgements; no pixels/hardware parity claimed.
}
int main(){cameraAndLazyView();cameraCaptureAndRowRepair();normalAndPoint();sharedSdkTraceAndStorage();originalGraphRealSdkAndOwnerRefresh();}
