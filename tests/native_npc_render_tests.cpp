#include "native_field_animation.hpp"
#include "animation_native_inner.hpp"
#include "field_npc_main_entry_binding.hpp"
#include "field_party.hpp"
#include "animation_render_binding.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
using namespace ttyd;
namespace {
constexpr uint32_t B=0x803d8e70,N=0x81f00000;
uint32_t bits(float f){uint32_t w;std::memcpy(&w,&f,4);return w;}float real(uint32_t w){float f;std::memcpy(&f,&w,4);return f;}
struct Fixture {
 std::shared_ptr<NativeFieldAnimation> native;bool valid=true;int token=0;uint64_t pass=95;uint32_t handle=0;
 Fixture(){assert(loadFieldPlayerPoseAssets("romfs"));EventNativePlatform p;p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){};native=NativeFieldAnimation::create("romfs",NativeFileCache::create(),p);assert(native);const auto h=native->pool()->enter(fieldPlayerPoseResources()[0],2,{});assert(h);handle=uint32_t(*h);assert(native->pool()->selectName(*h,"M_W_1",true,{}));pose().word(0,pose().word(0)&~8u);pose().word(0xf4,0xffffff80);for(unsigned i=0;i<17;++i)assert(native->tick(false));}
 EventAddressSpace& memory(){return native->process().vm().memory;}
 BattlePose& pose(){return *native->pool()->pose(int32_t(handle));}
 uint32_t word(uint32_t a){uint32_t w;assert(memory().readWord(a,w));return w;}
 void store(uint32_t a,uint32_t w){assert(memory().writeWord(a,w));}
 bool draw(){FieldNpcDrawRequest q;q.function=0x8003b4d8;q.args={handle,0,2};q.scalars={0,1};q.matrix={1,0,0,4,0,1,0,5,0,0,1,6};FieldNpcDrawResponse r;auto s=native->npcAnimationServices();return s.request(s.user,memory(),q,r);}
 void simple(bool transformed=true){assert(memory().registerRam(N,std::vector<uint8_t>(96)));for(unsigned i=0;i<24;++i)store(N+4*i,bits(i>=3 && i<=5?1.f:0.f));if(transformed){for(unsigned i=0;i<3;++i){store(N+4*i,bits(3.f+i));store(N+0xc+4*i,bits(2.f+i));}}store(0x8041e89c,0);store(0x8041e8a0,B);const float initial[]{1,0,0,4,0,1,0,5,0,0,1,6};for(unsigned i=0;i<12;++i)store(B+4*i,bits(initial[i]));}
};
void originalGraphRawRenderAndLease(){
 Fixture f;auto& n=*f.native;
 struct Probe {Fixture* f;bool ready=false;unsigned calls=0,fifo=0,held=0,legacy=0,begins=0,attrs=0,remaining=0;uint16_t heldValue=0;uint32_t level=0;} p{&f};
 NativeNpcGroupBackend g;g.renderChildren.user=&p;
 g.renderChildren.call=[](void* u,const AnimationRenderCall& q,AnimationRenderLocals&,uint32_t& result){auto& p=*static_cast<Probe*>(u);++p.calls;
  if(q.function==0x8000f2e4 || q.function==0x8000f2fc)result=0x81e00000;
  if(q.function==0x8002f33c)result=q.arguments[0].value;
  if(q.function==0x8029dd64){assert(q.arguments[0].value==0x81e0011c);auto matrix=q.arguments[1].value;for(unsigned i=0;i<12;++i)assert(std::isfinite(real(p.f->word(matrix+4*i))));}
  if(q.function==0x802b4520){assert(p.remaining==0);p.attrs=0;}
  if(q.function==0x802b3c08)++p.attrs;
  if(q.function==0x802b5a6c){assert(p.remaining==0 && q.arguments[0].value==0xa0 && q.arguments[1].value==0);++p.begins;p.remaining=q.arguments[2].value*p.attrs;}
  return true;
 };
 g.renderChildren.fifo=[](void* u,uint32_t address,uint16_t value){auto& p=*static_cast<Probe*>(u);assert(address==0xcc008000 && p.remaining);if(!p.ready){++p.held;p.heldValue=value;p.level=p.f->word(0x8041e89c);return false;}++p.fifo;--p.remaining;return true;};
 NativeNpcInnerBackend backend;backend.groups=g;backend.user=&p;backend.gx=[](void*,EventAddressSpace&,const BattleAudienceGXRequest&){return true;};backend.flush=[](void*,EventAddressSpace&,uint32_t,uint32_t){return true;};backend.invalidate=[](void*,EventAddressSpace&){return true;};backend.traverse=[](void* u,EventAddressSpace&,const AnimationInnerTraversal&){++static_cast<Probe*>(u)->legacy;return true;};assert(n.configureNpcInnerBackend(backend) && n.beginNpcPass(&f.token,f.pass,{},[&]{return f.valid;}));
 for(unsigned i=0;i<1000 && !p.held;++i){assert(!f.draw());}
 assert(p.held && p.begins && p.level && !p.legacy);
 const auto calls=p.calls;const auto level=p.level;const auto value=p.heldValue;assert(!f.draw() && p.calls==calls && p.heldValue==value && f.word(0x8041e89c)==level);assert(!n.endNpcPass(&f.token,f.pass) && !n.configureNpcInnerBackend(backend) && !n.tick(false));
 f.valid=false;auto held=p.held;assert(!f.draw() && p.held==held && p.calls==calls);f.valid=true;p.ready=true;
 bool done=false;for(unsigned i=0;i<10000 && !done;++i)done=f.draw();assert(done && p.fifo && !p.remaining && !p.legacy && n.endNpcPass(&f.token,f.pass));assert(f.word(0x8041e89c)==0 && f.word(0x8041e8a0)==B);assert(n.tick(false));
} // CPU nodes and raw source RAM/index traversal are real. Camera/shape SDK/GPU/cache/material children above are explicit receipts, not production effects.
void emptyAndMissingChildren(){
 Fixture f;AnimationRenderBinding b(f.memory(),{},[&]{return f.valid;});f.valid=false;assert(!b.advance(0xffffffff));f.valid=true;assert(b.advance(0xffffffff));assert(!b.advance(0));assert(!b.advance(0xffffffff));
}
}
int main(){originalGraphRawRenderAndLease();emptyAndMissingChildren();}
