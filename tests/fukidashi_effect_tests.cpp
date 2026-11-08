#include "fukidashi_effect.hpp"
#include "event_script_assets.hpp"
#include <cassert>
#include <cstring>
#include <map>
using namespace ttyd;
namespace {
constexpr uint32_t Effect=0x1000,Payload=0x2000;
uint32_t bits(float f){uint32_t n;std::memcpy(&n,&f,4);return n;}
struct Fixture {
 EventScriptAssets assets;std::map<uint32_t,uint8_t> overrides;
 FukidashiServices s;FukidashiMainCursor cursor;
 bool positionReady=true,depthReady=true,displayReady=true,deleteReady=true;
 unsigned positions=0,depths=0,displays=0,deletes=0;
 int32_t camera=0,mode=0;std::array<float,3> depthPosition{};
 uint8_t byte(uint32_t a){auto i=overrides.find(a);if(i!=overrides.end())return i->second;uint8_t n;assert(assets.readByte(a,n));return n;}
 uint32_t word(uint32_t a){uint32_t n=0;for(unsigned i=0;i<4;++i)n=n<<8|byte(a+i);return n;}
 void put(uint32_t a,uint32_t n){for(unsigned i=0;i<4;++i)overrides[a+i]=uint8_t(n>>(24-8*i));}
 float real(uint32_t a){const auto n=word(a);float f;std::memcpy(&f,&n,4);return f;}
 Fixture(){
  assert(assets.load("romfs/fukidashi-effect.pmevt"));
  for(uint32_t a=Payload;a<Payload+0x58;a+=4)put(a,0);
  put(Effect+0xc,Payload);put(Payload+4,bits(1));put(Payload+8,bits(2));put(Payload+12,bits(3));put(Payload+0x4c,4);
  s.user=this;s.memory.user=this;
  s.memory.word=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->word(a);};
  s.memory.storeWord=[](void* p,uint32_t a,uint32_t n){static_cast<Fixture*>(p)->put(a,n);};
  s.byte=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->byte(a);};
  s.binary=[](void*,FukidashiBinary op,float a,float b){return op==FukidashiBinary::Add?a+b:a-b;};
  s.position=[](void* p,uint32_t payload,int32_t arg){auto& f=*static_cast<Fixture*>(p);assert(payload==Payload && arg==1);++f.positions;
   assert(!f.cursor.reset());assert(!resumeFukidashiMain(Effect,f.s,f.cursor));
   if(!f.positionReady)return false;f.put(Payload,4);f.put(Payload+4,bits(99));return true;};
  s.depth=[](void* p,const std::array<float,3>& v)->std::optional<float>{auto& f=*static_cast<Fixture*>(p);++f.depths;f.depthPosition=v;if(!f.depthReady)return {};return 12;};
  s.display=[](void* p,int32_t camera,int32_t mode,uint32_t callback,uint32_t effect,float depth){auto& f=*static_cast<Fixture*>(p);++f.displays;assert(callback==0x8013174c && effect==Effect && depth==12);f.camera=camera;f.mode=mode;return f.displayReady;};
  s.effectDelete=[](void* p,uint32_t effect){auto& f=*static_cast<Fixture*>(p);assert(effect==Effect);++f.deletes;return f.deleteReady;};
 }
 bool poll(){return resumeFukidashiMain(Effect,s,cursor);}
};
void heldChildren(){
 Fixture f;f.put(Payload+0x44,1);f.positionReady=false;
 assert(!f.poll() && f.cursor.pending()==0x80133480);assert(f.word(Payload+0x34)==1);
 f.put(Payload+0x44,0);assert(!f.poll() && f.positions==2);assert(f.word(Payload+0x34)==1);
 auto changed=f.s;changed.display=nullptr;assert(!resumeFukidashiMain(Effect,changed,f.cursor));assert(f.positions==2);
 f.positionReady=true;f.depthReady=false;assert(!f.poll() && f.cursor.pending()==0x8001086c);
 assert((f.real(Payload+4)==99 && f.depthPosition==std::array<float,3>({1,2,3})));
 f.put(Payload+0x4c,7);f.depthReady=true;f.displayReady=false;assert(!f.poll());assert(f.camera==7 && f.mode==2);
 f.put(Payload,0);f.put(Payload+0x4c,9);f.displayReady=true;assert(f.poll());assert(f.camera==7 && f.mode==2 && !f.cursor.active());
 assert(f.word(Payload+0x34)==1 && f.positions==3 && f.depths==2);
}
void originalAnimation(){
 // All seven original table entries resolve; duration-kind0 loops at byte index7 (frame5).
 for(uint32_t kind=0;kind<7;++kind){Fixture f;f.put(Payload,kind);f.put(Payload+0x40,1);f.put(Payload+0x30,10000);f.put(Payload+0x24,bits(-90));
  for(unsigned i=0;i<(kind==6?120u:150u);++i)assert(f.poll());assert(f.deletes==0 && f.word(Payload+0x30)==10000);
 }
 Fixture f;for(unsigned i=0;i<76;++i)assert(f.poll());assert(f.deletes==0);
 f.deleteReady=false;assert(!f.poll());assert(f.cursor.pending()==0x8005ea30 && f.word(Payload+0x38)==0xffffffff);
 const auto counter=f.word(Payload+0x34);assert(!f.poll() && f.word(Payload+0x34)==counter);
 f.deleteReady=true;assert(f.poll() && f.deletes==3);
 Fixture loop;loop.put(Payload+0x40,1);loop.put(Payload+0x30,10000);loop.put(Payload+0x34,16);
 assert(loop.poll());assert(loop.word(Payload+0x34)==15 && loop.word(Payload+0x38)==5);
 // Negative duration still selects the duration table but does not expire.
 loop.put(Payload+0x40,0xffffffff);loop.put(Payload+0x30,0);assert(loop.poll());assert(loop.word(Payload+0x30)==0 && loop.deletes==0);
}
void lifetimeAndSpin(){
 // Original 30/60-poll duration admits the last zero-remaining display;
 // the next update owns deletion. Rendering never supplies these polls.
 for(uint32_t duration:{30u,60u}){Fixture life;life.put(Payload+0x40,duration);life.put(Payload+0x30,duration);
  for(uint32_t i=0;i<duration;++i)assert(life.poll());
  assert(life.deletes==0 && life.displays==duration && life.word(Payload+0x30)==0);
  assert(life.poll() && life.deletes==1 && life.displays==duration);
 }
 Fixture negativeCounter;negativeCounter.put(Payload+0x34,0xffffffff);assert(negativeCounter.poll());
 assert(negativeCounter.word(Payload+0x34)==0 && negativeCounter.word(Payload+0x38)==0);
 Fixture wrap;wrap.put(Payload+0x40,1);wrap.put(Payload+0x30,0x80000000);assert(wrap.poll());
 assert(wrap.word(Payload+0x30)==0x7fffffff && wrap.deletes==0);
 Fixture f;f.put(Payload+0x40,1);f.put(Payload+0x30,0);assert(f.poll() && f.deletes==1 && f.displays==0);
 Fixture sentinel;sentinel.put(Payload+0x40,1);sentinel.put(Payload+0x30,10000);assert(sentinel.poll());assert(sentinel.word(Payload+0x30)==10000);
 Fixture spin;spin.put(Payload,6);spin.put(Payload+0x24,bits(-90));spin.put(Payload+0x40,1);spin.put(Payload+0x30,0);
 for(unsigned i=0;i<4;++i)assert(spin.poll());assert(spin.real(Payload+0x24)==0 && spin.word(Payload+0x54)==1);
 for(unsigned i=1;i<120;++i)assert(spin.poll());assert(spin.word(Payload+0x54)==120);
 for(unsigned i=0;i<9;++i)assert(spin.poll());assert(spin.real(Payload+0x24)==-81 && spin.deletes==0);
 assert(spin.poll() && spin.real(Payload+0x24)==-90 && spin.deletes==1 && spin.word(Payload+0x30)==0);
}
}
int main(){heldChildren();originalAnimation();lifetimeAndSpin();}
