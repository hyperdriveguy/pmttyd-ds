#include "native_field_animation.hpp"
#include "animation_native_inner.hpp"
#include "field_npc_main_entry_binding.hpp"
#include "field_party.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
using namespace ttyd;
namespace {
uint32_t bits(float f){uint32_t w;std::memcpy(&w,&f,4);return w;}
struct Fixture {
 std::shared_ptr<NativeFieldAnimation> native;bool valid=true;int token=0;uint64_t pass=91;uint32_t handle=0;
 Fixture(){assert(loadFieldPlayerPoseAssets("romfs"));EventNativePlatform p;p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){};native=NativeFieldAnimation::create("romfs",NativeFileCache::create(),p);assert(native);const auto h=native->pool()->enter(fieldPlayerPoseResources()[0],2,{});assert(h);handle=uint32_t(*h);assert(native->pool()->selectName(*h,"M_W_1",true,{}));pose().word(0,pose().word(0)&~8u);pose().word(0xf4,0xffffff80);for(unsigned i=0;i<17;++i)assert(native->tick(false));}
 EventAddressSpace& memory(){return native->process().vm().memory;}
 BattlePose& pose(){return *native->pool()->pose(int32_t(handle));}
 uint32_t word(uint32_t a){uint32_t w;assert(memory().readWord(a,w));return w;}
 void store(uint32_t a,uint32_t v){assert(memory().writeWord(a,v));}
 bool draw(){FieldNpcDrawRequest q;q.function=0x8003b4d8;q.args={handle,0,2};q.scalars={0,1};q.matrix={1,0,0,4,0,1,0,5,0,0,1,6};FieldNpcDrawResponse r;auto s=native->npcAnimationServices();return s.request(s.user,memory(),q,r);}
};
void originalGraphAndLease(){
 Fixture f;auto& n=*f.native;
 struct Probe {Fixture* f;bool nodeReady=false,renderReady=false;unsigned nodes=0,renders=0,holds=0,legacy=0,flush=0;uint32_t heldShape=0,heldLevel=0,current=0;} probe{&f};probe.current=f.pose().word(0x28);
 NativeNpcGroupBackend groups;groups.user=&probe;
 groups.node=[](void* u,EventAddressSpace& m,const AnimationGroupNode& q){auto& p=*static_cast<Probe*>(u);assert(&m==&p.f->memory());if(!p.nodeReady)return false;uint32_t nodeX,nodeY,nodeZ;assert(m.readWord(q.node,nodeX) && m.readWord(q.node+4,nodeY) && m.readWord(q.node+8,nodeZ));const auto level=p.f->word(0x8041e89c)+1,base=0x803d8e70+48*level;p.f->store(0x8041e89c,level);p.f->store(0x8041e8a0,base);for(unsigned i=0;i<12;++i)p.f->store(base+4*i,i%5==0?bits(1):0);p.f->store(base+0xc,nodeX);p.f->store(base+0x1c,nodeY);p.f->store(base+0x2c,nodeZ);++p.nodes;return true;};
 groups.render=[](void* u,EventAddressSpace& m,uint32_t shape,uint32_t current){auto& p=*static_cast<Probe*>(u);assert(&m==&p.f->memory() && current==p.current);if(shape!=0xffffffff && !p.renderReady){p.heldShape=shape;p.heldLevel=p.f->word(0x8041e89c);++p.holds;return false;}++p.renders;return true;};
 NativeNpcInnerBackend b;b.groups=groups;b.user=&probe;
 b.gx=[](void*,EventAddressSpace&,const BattleAudienceGXRequest&){return true;};
 b.flush=[](void* u,EventAddressSpace&,uint32_t,uint32_t){++static_cast<Probe*>(u)->flush;return true;};
 b.invalidate=[](void*,EventAddressSpace&){return true;};
 b.traverse=[](void* u,EventAddressSpace&,const AnimationInnerTraversal&){++static_cast<Probe*>(u)->legacy;return true;};
 assert(n.configureNpcInnerBackend(b) && n.beginNpcPass(&f.token,f.pass,{},[&f]{return f.valid;}));
 assert(!f.draw() && n.npcDrawPending() && !probe.nodes && !probe.legacy);
 assert(!n.endNpcPass(&f.token,f.pass) && !n.configureNpcInnerBackend(b) && !n.tick(false));
 probe.nodeReady=true;for(unsigned i=0;i<1000 && !probe.holds;++i)assert(!f.draw());assert(probe.holds && probe.nodes && !probe.legacy && probe.flush==2);
 const auto nodes=probe.nodes;const auto renders=probe.renders;const auto level=probe.heldLevel;assert(!f.draw() && probe.nodes==nodes && probe.renders==renders && f.word(0x8041e89c)==level);
 f.valid=false;const auto holds=probe.holds;assert(!f.draw() && probe.holds==holds);f.valid=true;probe.renderReady=true;
 bool done=false;for(unsigned i=0;i<1000 && !done;++i)done=f.draw();assert(done && probe.renders>renders && !probe.legacy && n.endNpcPass(&f.token,f.pass));assert(f.word(0x8041e89c)==0 && f.word(0x8041e8a0)==0x803d8e70);
 assert(n.configureNpcInner({}) && n.tick(false));
} // Node/GPU acknowledgements above are explicit fixtures, not production SDK/GPU work.
struct CopyFixture {
 Fixture f;static constexpr uint32_t A=0x81f00000,S=0x81f10000,O=0x81f20000;
 NativeNpcGroupBackend backend;unsigned nodes=0;bool valid=true;
 CopyFixture(uint32_t destination){auto& m=f.memory();assert(m.registerRam(A,std::vector<uint8_t>(0x1000)) && m.registerRam(S,std::vector<uint8_t>(56)));auto w=f.word(0x80415dc0);f.store(w+0x18,A);f.store(w+0x20,A+0x200);f.store(A+0x200+0x1a8,A+0x400);f.store(A+0x5c,A+0x600);assert(m.writeByte(A+0x600,255));f.store(A+0x64,A+0x700);f.store(A+0x164,A+0x800);f.store(A+0x800,destination);f.store(A,0x80);f.store(A+0x154,0);f.store(A+0x400+0x54,0);f.store(A+0x400+0x50,0);f.store(A+0x400+0x4c,0);f.store(A+0x400+0x48,0xffffffff);f.store(A+0x400+0x40,0xffffffff);f.store(A+0x400+0x44,0xffffffff);f.store(0x8041e89c,0);for(unsigned i=0;i<14;++i)f.store(S+4*i,bits(float(i+1)));
 backend.user=this;backend.node=[](void* u,EventAddressSpace&,const AnimationGroupNode&){auto& c=*static_cast<CopyFixture*>(u);++c.nodes;c.f.store(0x8041e89c,1);c.f.store(0x8041e8a0,S);return true;};backend.render=[](void*,EventAddressSpace&,uint32_t,uint32_t){return true;};}
};
void rawPairCopyAndRepair(){
 {CopyFixture c(CopyFixture::S+8);AnimationGroupBinding b(c.f.memory(),c.backend,[&c]{return c.valid;});AnimationInnerTraversal q{0xffffffff,0,3,5,.5f};bool done=false;for(unsigned i=0;i<1000 && !done;++i)done=b.advance(q,1);assert(done && c.nodes==1);for(unsigned pair=0;pair<7;++pair){assert(c.f.word(CopyFixture::S+pair*8)==bits(1));assert(c.f.word(CopyFixture::S+pair*8+4)==bits(2));}}
 {CopyFixture c(CopyFixture::S);c.backend.node=[](void* u,EventAddressSpace&,const AnimationGroupNode&){auto& x=*static_cast<CopyFixture*>(u);++x.nodes;x.f.store(0x8041e89c,1);x.f.store(0x8041e8a0,CopyFixture::S+8);return true;};AnimationGroupBinding b(c.f.memory(),c.backend,[&c]{return c.valid;});AnimationInnerTraversal q{0xffffffff,0,3,5,.5f};assert(b.advance(q) && c.nodes==1);for(unsigned i=0;i<12;++i)assert(c.f.word(CopyFixture::S+4*i)==bits(float(i+3)));}
 {CopyFixture c(CopyFixture::O);assert(c.f.memory().registerRam(CopyFixture::O,std::vector<uint8_t>(12)));AnimationGroupBinding b(c.f.memory(),c.backend,[&c]{return c.valid;});AnimationInnerTraversal q{0xffffffff,0,3,5,.5f};assert(!b.advance(q) && c.nodes==1 && c.f.word(CopyFixture::O)==bits(1) && c.f.word(CopyFixture::O+8)==0);c.f.store(CopyFixture::O,bits(999));c.f.store(CopyFixture::S+8,bits(303));c.f.store(CopyFixture::S+12,bits(404));c.f.store(CopyFixture::S+16,bits(505));c.valid=false;assert(!b.advance(q));c.valid=true;std::vector<uint8_t> repaired;assert(c.f.memory().unregisterRam(CopyFixture::O,&repaired));repaired.resize(48);assert(c.f.memory().registerRam(CopyFixture::O,std::move(repaired)));assert(b.advance(q));assert(c.nodes==1 && c.f.word(CopyFixture::O)==bits(999) && c.f.word(CopyFixture::O+8)==bits(3) && c.f.word(CopyFixture::O+12)==bits(4) && c.f.word(CopyFixture::O+16)==bits(505));}
}
void nativeSpecialBlend(){
 for(bool exceptional:{false,true}){CopyFixture c(CopyFixture::O);c.f.store(CopyFixture::A,0x20);c.f.store(CopyFixture::A+0x120,CopyFixture::A+0x804);c.f.store(CopyFixture::A+0x804,0);c.f.store(CopyFixture::A+0x11c,4);c.f.store(CopyFixture::A+0x14,0);
  uint32_t oldFactor;if(!c.f.memory().readWord(0x8041fba0,oldFactor))assert(c.f.memory().registerRam(0x8041fba0,std::vector<uint8_t>(4)));c.f.store(0x8041fba0,exceptional?0x7fc00000:bits(.25f));
  c.backend.material=[](void*,EventAddressSpace&,uint32_t shape,uint32_t current){assert(shape==0xffffffff && current==3);return true;};
  c.backend.vivian=[](void* u,EventAddressSpace&,const AnimationGroupVivian& q){auto& x=*static_cast<CopyFixture*>(u);assert(q.matrix==CopyFixture::S && q.saved==CopyFixture::A+0x124 && q.parameters[2]==0 && q.parameters[3]==0);for(unsigned i=0;i<12;++i)x.f.store(q.saved+4*i,bits(20.f+i));return true;};
  c.backend.render=[](void*,EventAddressSpace&,uint32_t,uint32_t){assert(false);return false;};
  AnimationGroupBinding b(c.f.memory(),c.backend,[&c]{return c.valid;});AnimationInnerTraversal q{0xffffffff,0,3,5,.5f};
  if(exceptional){assert(!b.advance(q) && c.nodes==1 && c.f.word(CopyFixture::A)==0);c.f.store(0x8041fba0,bits(.25f));assert(!b.advance(q) && c.nodes==1 && c.f.word(CopyFixture::A+0x124)==bits(20));}
  else {assert(b.advance(q) && c.nodes==1 && c.f.word(CopyFixture::A)==0);for(unsigned i:{0u,1u,2u,4u,5u,6u,8u,9u,10u})assert(c.f.word(CopyFixture::A+0x124+4*i)==bits(std::fma(.25f,-19.f,20.f+i)));for(unsigned i:{3u,7u,11u})assert(c.f.word(CopyFixture::A+0x124+4*i)==bits(20.f+i));}
 } // Explicit mutable factor/effect fixtures; original effect rendering is separate.
}

}
int main(){originalGraphAndLease();rawPairCopyAndRepair();nativeSpecialBlend();}
