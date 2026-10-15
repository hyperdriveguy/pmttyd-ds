#include "native_field_animation.hpp"
#include "animation_native_inner.hpp"
#include "field_npc_main_entry_binding.hpp"
#include "field_party.hpp"
#include <cassert>
#include <cstring>
#include <cmath>
using namespace ttyd;
namespace {
struct Fixture {
 std::shared_ptr<NativeFieldAnimation> native;bool valid=true;int token=0;uint64_t pass=73;uint32_t handle=0;
 Fixture(){assert(loadFieldPlayerPoseAssets("romfs"));EventNativePlatform p;p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){};native=NativeFieldAnimation::create("romfs",NativeFileCache::create(),p);assert(native);const auto h=native->pool()->enter(fieldPlayerPoseResources()[0],2,{});assert(h);handle=uint32_t(*h);assert(native->pool()->selectName(*h,"M_W_1",true,{}));pose().word(0,pose().word(0)&~8u);pose().word(0xf4,0xffffff80);for(unsigned i=0;i<17;++i)assert(native->tick(false));}
 EventAddressSpace& memory(){return native->process().vm().memory;}
 BattlePose& pose(){return *native->pool()->pose(int32_t(handle));}
 uint32_t word(uint32_t a){uint32_t w;assert(memory().readWord(a,w));return w;}
 bool draw(){FieldNpcDrawRequest q;q.function=0x8003b4d8;q.args={handle,0,2};q.scalars={0,1};q.matrix={1,0,0,4,0,1,0,5,0,0,1,6};FieldNpcDrawResponse r;auto s=native->npcAnimationServices();return s.request(s.user,memory(),q,r);}
};
void actualParentAndRepairs(){
 Fixture f;auto& n=*f.native;const auto beforeCurrent=f.pose().word(0x28),beforeNext=f.pose().word(0x2c),beforeFraction=f.pose().word(0x30);const auto work=f.word(0x80415dc0),record=n.process().animationMemory()->poseAddress(f.handle);
 struct Probe {Fixture* f;bool gxReady=false,flushReady=false,traverseReady=false;unsigned gx=0,flush=0,invalidate=0,traversalAttempts=0;AnimationInnerTraversal traversal{};} probe{&f};
 NativeNpcInnerBackend b;b.user=&probe;
 b.gx=[](void* u,EventAddressSpace& m,const BattleAudienceGXRequest& q){auto& p=*static_cast<Probe*>(u);assert(&m==&p.f->memory());if(!p.gxReady)return false;const uint32_t functions[]{0x802b93d8,0x802b932c,0x802b940c,0x802b8d60};assert(q.function==functions[p.gx]);++p.gx;return true;};
 b.flush=[](void* u,EventAddressSpace& m,uint32_t a,uint32_t bytes){auto& p=*static_cast<Probe*>(u);assert(&m==&p.f->memory());assert(a==p.f->pose().word(p.flush?0x54:0x4c));uint8_t byte;if(bytes)assert(m.readByte(a,byte) && m.readByte(a+bytes-1,byte));if(!p.flushReady)return false;++p.flush;return true;};
 b.invalidate=[](void* u,EventAddressSpace&){++static_cast<Probe*>(u)->invalidate;return true;};
 b.traverse=[](void* u,EventAddressSpace&,const AnimationInnerTraversal& q){auto& p=*static_cast<Probe*>(u);++p.traversalAttempts;p.traversal=q;return p.traverseReady;};
 assert(n.configureNpcInnerBackend(b) && n.beginNpcPass(&f.token,f.pass,{},[&f]{return f.valid;}));
 assert(!f.draw() && n.npcDrawPending() && !probe.gx && !n.endNpcPass(&f.token,f.pass));
 assert(!n.configureNpcInnerBackend(b));
 // Missing second MODEL-stack pair holds after the acknowledged first pair.
 std::vector<uint8_t> stack;assert(f.memory().unregisterRam(0x803d8e70,&stack));assert(f.memory().registerRam(0x803d8e70,std::vector<uint8_t>(8)));
 probe.gxReady=true;for(unsigned i=0;i<1000 && f.word(0x803d8e70)!=0x3f800000;++i)assert(!f.draw());
 assert(f.word(0x803d8e70)==0x3f800000 && probe.gx==4 && !probe.flush);
 assert(f.memory().writeWord(0x803d8e70,0x40000000));f.valid=false;assert(!f.draw() && !probe.flush);f.valid=true;
 assert(f.memory().registerRam(0x803d8e78,std::vector<uint8_t>(stack.begin()+8,stack.end())));
 assert(!f.draw() && f.word(0x803d8e70)==0x40000000 && !probe.flush);
 assert(f.word(0x8041e89c)==0 && f.word(0x8041e8a0)==0x803d8e70 && f.word(work+0xe0)==3 && f.word(work+0x18)==record);
 const auto quantization=n.process().quantization().writes();assert(quantization==4);
 probe.flushReady=true;assert(!f.draw() && probe.flush==2 && probe.invalidate==1 && probe.traversalAttempts==1);
 assert(probe.traversal.parent==0xffffffff && probe.traversal.current==beforeCurrent && probe.traversal.next==beforeNext);uint32_t fraction;std::memcpy(&fraction,&probe.traversal.fraction,4);assert(fraction==beforeFraction);
 assert(!f.draw() && probe.flush==2 && probe.gx==4 && probe.invalidate==1 && n.process().quantization().writes()==quantization);
 probe.traverseReady=true;assert(f.draw() && !n.npcDrawPending() && n.endNpcPass(&f.token,f.pass));
 assert(n.configureNpcInner({}) && n.tick(false));
} // GPU/traversal acknowledgements above are explicit receipts, not a renderer.
void cameraOwnerChanges(){
 Fixture f;auto& n=*f.native;NativeNpcInnerBackend b;
 b.gx=[](void*,EventAddressSpace&,const BattleAudienceGXRequest&){return true;};
 b.flush=[](void*,EventAddressSpace&,uint32_t,uint32_t){return true;};
 b.invalidate=[](void*,EventAddressSpace&){return true;};
 b.traverse=[](void*,EventAddressSpace&,const AnimationInnerTraversal&){return true;};
 assert(n.configureNpcInnerBackend(b));
 auto first=std::make_shared<CameraManager>(),second=std::make_shared<CameraManager>();
 first->camera(4)->eye={0,0,10};first->camera(4)->at={0,0,0};first->camera(4)->viewPublished=true;
 second->camera(4)->eye={10,0,0};second->camera(4)->at={0,0,0};second->camera(4)->viewPublished=true;
 // Explicit published camera fixtures, not source cameraMain execution.
 for(unsigned i=0;i<2;++i){f.pose().word(0,f.pose().word(0)|8|0x100);f.pose().word(0x78,0x3f800000);f.pose().word(0x3c,0);
  assert(n.beginNpcPass(&f.token,++f.pass,i?second:first,[&f]{return f.valid;}));bool done=false;for(unsigned v=0;v<1000 && !done;++v)done=f.draw();assert(done && n.endNpcPass(&f.token,f.pass));volatile float quarter=float(std::atan2(1.,0.));volatile float heading=quarter*57.29579162597656f;volatile float rear=heading+180.f;assert(f.pose().scalar(0x74)==(i?float(rear):0.f) && f.pose().scalar(0x70)==-90.f);
 }
}

}
int main(){actualParentAndRepairs();cameraOwnerChanges();}
