#include "native_field_animation.hpp"
#include "animation_native_inner.hpp"
#include "field_npc_main_entry_binding.hpp"
#include "field_party.hpp"
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
bool finish(AnimationNodeBinding& b,const AnimationGroupNode& q){for(unsigned i=0;i<1000;++i)if(b.advance(q,3))return true;return false;}
void nativeMatricesAndGates(){
 {Fixture f;f.simple();AnimationNodeBinding b(f.memory(),[&]{return f.valid;});assert(finish(b,{N,false,{}}));const float expected[]{2,0,0,7,0,3,0,9,0,0,4,11};for(unsigned i=0;i<12;++i)assert(f.word(B+48+i*4)==bits(expected[i]));assert(f.word(0x8041e89c)==1 && f.word(0x8041e8a0)==B+48);}
 {Fixture f;f.simple(false);f.store(N+0x20,bits(45));AnimationNodeBinding b(f.memory(),[&]{return f.valid;});assert(finish(b,{N,false,{}}));volatile float angle=90.f*real(0x3c8efa35);const float s=float(std::sin(double(angle))),c=float(std::cos(double(angle)));assert(f.word(B+48)==bits(c) && f.word(B+48+4)==bits(-s) && f.word(B+48+0x10)==bits(s) && f.word(B+48+0x14)==bits(c));}
 {Fixture f;f.simple(false);f.store(N+0x14,0x7fc00000);AnimationNodeBinding b(f.memory(),[&]{return f.valid;});assert(finish(b,{N,true,{1,1,real(0x7fc00000)}}));assert(f.word(B+48+0x28)==bits(1));} // Unused NaN Z does not become a native all-input finite clamp.
 {Fixture f;f.simple(false);f.store(0x80429358,bits(2));AnimationNodeBinding b(f.memory(),[&]{return f.valid;});assert(finish(b,{N,false,{}}));assert(f.word(B+48)==bits(2) && f.word(B+48+0x14)==bits(2) && f.word(B+48+0x28)==bits(2));}
 {Fixture f;f.simple(false);f.store(0x8041e458,bits(2));f.store(0x8041e45c,bits(3));AnimationNodeBinding b(f.memory(),[&]{return f.valid;});assert(finish(b,{N,false,{}}));assert(f.word(B+48+8)==bits(8) && f.word(B+48+0x18)==bits(10) && f.word(B+48+0x28)==bits(13));assert(f.word(B+48+0xc)==bits(12) && f.word(B+48+0x1c)==bits(15) && f.word(B+48+0x2c)==bits(18));}
}
void finalConcatRepairAndSnapshot(){
 Fixture f;f.simple();std::vector<uint8_t> stack;assert(f.memory().unregisterRam(B,&stack));stack.resize(64);assert(f.memory().registerRam(B,std::move(stack)));f.store(B+48+8,bits(123));AnimationNodeBinding b(f.memory(),[&]{return f.valid;});AnimationGroupNode q{N,false,{}};assert(!b.advance(q) && f.word(0x8041e89c)==1 && f.word(B+48)==bits(2) && f.word(B+48+8)==bits(123));
 f.store(B+48,bits(999));f.store(B,bits(100));f.store(N,bits(500));f.store(0x8041e458,bits(55));f.store(0x8041e89c,9);f.valid=false;assert(!b.advance(q));f.valid=true;assert(f.memory().registerRam(B+64,std::vector<uint8_t>(0x600-64)));assert(finish(b,q));assert(f.word(B+48)==bits(999) && f.word(B+48+8)==0 && f.word(B+48+0xc)==bits(7) && f.word(B+48+0x14)==bits(3) && f.word(B+48+0x2c)==bits(11));assert(f.word(0x8041e89c)==9 && f.word(0x8041e8a0)==B+48);
}
void concatInputCaptureFrontiers(){
 Fixture f;f.simple();assert(f.memory().unregisterRam(B));assert(f.memory().registerRam(B,std::vector<uint8_t>(8)) && f.memory().registerRam(B+8,std::vector<uint8_t>(8)) && f.memory().registerRam(B+48,std::vector<uint8_t>(48)));f.store(B,bits(1));f.store(B+4,0);f.store(B+8,0);f.store(B+12,bits(4));AnimationNodeBinding b(f.memory(),[&]{return f.valid;});AnimationGroupNode q{N,false,{}};
 assert(!b.advance(q));f.store(B,bits(100));f.store(B+12,bits(44));f.store(0x8041e458,bits(2));f.store(0x8041e45c,bits(3));assert(f.memory().registerRam(B+16,std::vector<uint8_t>(8)));f.store(B+16,0);f.store(B+20,bits(1));assert(!b.advance(q));assert(f.word(B+48)==0);f.store(B+12,bits(444));f.store(0x8041e458,bits(22));f.store(0x8041e45c,bits(33));assert(f.memory().registerRam(B+24,std::vector<uint8_t>(24)));const float rest[]{0,5,0,0,1,6};for(unsigned i=0;i<6;++i)f.store(B+24+4*i,bits(rest[i]));assert(finish(b,q));const float expected[]{2,0,88,135,0,3,10,19,0,0,16,23};for(unsigned i=0;i<12;++i)assert(f.word(B+48+4*i)==bits(expected[i]));
}

void physicallyAliasedFinalConcat(){
 std::vector<uint8_t> storage(56);Fixture f;f.simple();assert(f.memory().unregisterRam(B));assert(f.memory().registerExternalRam(B,storage.data(),48) && f.memory().registerExternalRam(B+48,storage.data()+8,48));for(unsigned i=0;i<12;++i)f.store(B+4*i,bits(float(i+1)));AnimationNodeBinding b(f.memory(),[&]{return f.valid;});assert(finish(b,{N,false,{}}));const float expected[]{2,6,12,30,10,18,28,82,18,30,44,134};for(unsigned i=0;i<12;++i)assert(f.word(B+48+i*4)==bits(expected[i]));
} // Explicit external aliases establish the SDK capture case, not a source runtime observation.
void originalGraphOwnsRealNodes(){
 Fixture f;auto& n=*f.native;struct Probe{Fixture* f;bool ready=false;unsigned attempts=0,done=0,legacy=0;uint32_t current=0,level=0;} p{&f};p.current=f.pose().word(0x28);
 NativeNpcGroupBackend g;g.user=&p;g.render=[](void* u,EventAddressSpace& m,uint32_t shape,uint32_t current){auto& p=*static_cast<Probe*>(u);assert(&m==&p.f->memory() && current==p.current);const auto matrix=p.f->word(0x8041e8a0);for(unsigned i=0;i<12;++i)assert(std::isfinite(real(p.f->word(matrix+4*i))));if(shape!=0xffffffff && !p.ready){++p.attempts;p.level=p.f->word(0x8041e89c);return false;}++p.done;return true;};
 NativeNpcInnerBackend backend;backend.groups=g;backend.user=&p;backend.gx=[](void*,EventAddressSpace&,const BattleAudienceGXRequest&){return true;};backend.flush=[](void*,EventAddressSpace&,uint32_t,uint32_t){return true;};backend.invalidate=[](void*,EventAddressSpace&){return true;};backend.traverse=[](void* u,EventAddressSpace&,const AnimationInnerTraversal&){++static_cast<Probe*>(u)->legacy;return true;};assert(n.configureNpcInnerBackend(backend) && n.beginNpcPass(&f.token,f.pass,{},[&]{return f.valid;}));
 for(unsigned i=0;i<1000 && !p.attempts;++i)assert(!f.draw());
 assert(p.attempts && p.level && !p.legacy);auto level=p.level;assert(!f.draw() && f.word(0x8041e89c)==level && !n.endNpcPass(&f.token,f.pass) && !n.configureNpcInnerBackend(backend));f.valid=false;auto attempts=p.attempts;assert(!f.draw() && p.attempts==attempts);f.valid=true;p.ready=true;bool done=false;for(unsigned i=0;i<1000 && !done;++i)done=f.draw();assert(done && p.done && !p.legacy && n.endNpcPass(&f.token,f.pass));assert(f.word(0x8041e89c)==0 && f.word(0x8041e8a0)==B);assert(n.tick(false));
} // Actual node CPU math above; GPU/cache receipts remain explicit test fixtures.
}
int main(){nativeMatricesAndGates();finalConcatRepairAndSnapshot();concatInputCaptureFrontiers();physicallyAliasedFinalConcat();originalGraphOwnsRealNodes();}
