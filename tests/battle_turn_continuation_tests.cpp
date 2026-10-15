#include <memory>
#include "battle_turn_continuation.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t B=0x10000,U=0x40000,V=0x42000,E=0x70000,P=0x80000;
uint32_t bits(float f){uint32_t n;std::memcpy(&n,&f,4);return n;}
struct Fixture {
 std::map<uint32_t,uint8_t> bytes;std::vector<BattleTurnRequest> calls;std::vector<uint32_t> reads;
 uint32_t recovering=0,hold=0,wait=0,conclusion=0,burnActive=0;int holds=0;
 int holdAt=-1;bool heldAt=false;
 bool allocation=false,guard=false,mutateHealAttribute=false;uint8_t strength=2;
 Fixture(uint32_t index){word(B+0xf14,0x03000000+index);half(B,2);word(0x802efb78,0x43300000);word(0x802efb7c,0x80000000);word(0x80422594,bits(900));word(0x8042256c,bits(1));}
 uint32_t word(uint32_t a){reads.push_back(a);return uint32_t(bytes[a])<<24|uint32_t(bytes[a+1])<<16|uint32_t(bytes[a+2])<<8|bytes[a+3];}
 uint16_t half(uint32_t a){reads.push_back(a);return uint16_t(uint32_t(bytes[a])<<8|bytes[a+1]);}
 void word(uint32_t a,uint32_t n){for(unsigned i=0;i<4;++i)bytes[a+i]=uint8_t(n>>(24-8*i));}
 void half(uint32_t a,uint16_t n){bytes[a]=uint8_t(n>>8);bytes[a+1]=uint8_t(n);}
 void actor(uint32_t u,unsigned slot){word(B+0x20+4*slot,u);word(u,slot);half(u+0x10c,5);half(u+0x108,10);half(u+0x112,3);half(u+0x10e,10);word(u+0x114,bits(2));}
 BattleTurnServices services(){BattleTurnServices s;s.user=this;s.actors.user=this;s.actors.memory.user=this;
 s.actors.memory.word=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->word(a);};
 s.actors.memory.half=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->half(a);};
 s.actors.memory.storeWord=[](void* p,uint32_t a,uint32_t n){static_cast<Fixture*>(p)->word(a,n);};
 s.actors.memory.storeHalf=[](void* p,uint32_t a,uint16_t n){static_cast<Fixture*>(p)->half(a,n);};
 s.actors.byte=[](void* p,uint32_t a){auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);return f.bytes[a];};
 s.call=[](void* p,const BattleTurnRequest& r)->std::optional<BattleTurnOutput>{auto& f=*static_cast<Fixture*>(p);
  if(r.function==f.hold && f.holds-->0)return {};
  if(!f.heldAt && int(f.calls.size())==f.holdAt){f.heldAt=true;return {};}
  f.calls.push_back(r);BattleTurnOutput o;
  switch(r.function){
  case 0x80127814:o.value=0;break;
  case 0x80127890:o.value=r.integers[0] && r.integers[1]==f.recovering?1:0;break;
  case 0x80127f34:o.strength=f.strength;break;
  case 0x80125d08:o.value=f.guard;break;
  case 0x8011ace8:o.value=f.wait;break;
  case 0x8011aa34:o.value=f.conclusion;break;
  case 0x800374ac:o.value=f.allocation?E:0;f.word(E+0x15c,77);break;
  case 0x80128414:o.value=10;break;
  case 0x80128c98:o.value=7;break;
  case 0x80128cd8:o.value=P;break;
  case 0x80128b10:case 0x80128c7c:o.position={1,2,3};break;
  case 0x80126bec:f.half(r.integers[0]+0x10c,uint16_t(f.half(r.integers[0]+0x10c)+r.integers[1]));if(f.mutateHealAttribute)f.word(r.integers[0]+0x104,0x40000000);break;
  case 0x80195d7c:o.value=f.burnActive && r.integers[0]==U;break;
  case 0x8011b1ec:f.half(B,2);break;
  default:break;
  }return o;};return s;
 }
 unsigned count(uint32_t fn)const{unsigned n=0;for(const auto& r:calls)n+=r.function==fn;return n;}
 const BattleTurnRequest& call(uint32_t fn)const{for(const auto& r:calls)if(r.function==fn)return r;assert(false);return calls.front();}
 bool run(BattleTurnCursor& c){return resumePackedBattleTurn(B,services(),c);}
};
void dispatch(){
 for(unsigned i=0;i<78;++i){Fixture f(i);BattleTurnCursor c;assert(f.run(c));assert(!c.active());
  if(i&1 && i>=7 && i<=0x49 && i!=9){assert(f.word(B+0xf14)==0x03000000+i);assert(f.count(0x8011ace8)==1);}
 }
 Fixture f(78);BattleTurnCursor c;assert(f.run(c)&&f.calls.empty());assert(f.word(B+0xf14)==0x0300004e);
 Fixture g(0xffffffffu);assert(g.run(c)&&g.calls.empty());
}
void recovery(){
 Fixture f(0x12);f.actor(U,2);f.recovering=1;f.allocation=true;f.hold=0x800d98bc;f.holds=2;BattleTurnCursor c;
 assert(!f.run(c));assert(c.pending()==0x800d98bc);assert(f.count(0x80127890)==64 && f.count(0x800374ac)==1);
 assert(f.word(U+0x2a8)==77 && f.word(E+0x160)==2);assert(!f.run(c));assert(f.count(0x80127890)==64);
 assert(f.run(c));assert(f.word(B+0xf14)==0x03000013);assert(f.count(0x8011ace8)==0);
 Fixture slow(0x3a);slow.actor(U,1);slow.recovering=20;slow.hold=0x801266d4;slow.holds=1;
 assert(!slow.run(c));assert(slow.count(0x80127890)==1);assert(slow.run(c));assert(slow.count(0x801266d4)==1);
 assert(slow.count(0x800374ac)==1 && slow.count(0x8011ace8)==1);
 Fixture empty(0x12);assert(empty.run(c));assert(empty.count(0x80127890)==16*64); // Later seven explicit-null skips.
 assert(empty.word(B+0xf14)==0x0300004c && empty.word(B+0xf18)==0x04000000);
}
void intro(){
 Fixture f(4);f.word(B+0xefc,3);f.hold=0x8011a06c;f.holds=2;BattleTurnCursor c;
 assert(!f.run(c));assert(f.word(B+0xefc)==2);assert(!f.run(c));assert(f.word(B+0xefc)==2);assert(f.run(c));
 assert(f.call(0x80010bc4).reals[0]==900);assert(f.word(B+0xefc)==2);
 Fixture zero(4);assert(zero.run(c));assert(zero.word(B+0xf14)==0x03000005);
 Fixture first(3);first.half(B,1);first.word(B+0x2738,0x90000);first.bytes[0x90018]=1;
 assert(first.run(c));assert(first.word(B+0xefc)==119);assert(first.word(B+0xf14)==0x03000004);
}
void poisonAndBurn(){
 Fixture f(6);f.actor(U,1);f.actor(V,2);BattleTurnCursor c;assert(f.run(c));assert(f.count(0x800fdfd4)==2);
 const auto& q=f.call(0x800fdfd4);assert(q.integers[0]==uint32_t(-5)&&q.integers[3]==2&&q.integers[5]==279);
 assert(f.word(B+0xf14)==0x03000007);
 Fixture burn(8);burn.actor(U,1);burn.actor(V,2);burn.burnActive=1;burn.hold=0x80195f38;burn.holds=1;
 assert(!burn.run(c));assert(burn.half(B+2)==1);assert(burn.run(c));assert(burn.count(0x80195f38)==2 && burn.count(0x80195d7c)==2);
 assert(burn.half(B+2)==1);assert(burn.word(B+0xf14)==0x03000009);
 burn.calls.clear();burn.burnActive=0;assert(burn.run(c));assert(burn.count(0x80195f38)==0&&burn.count(0x80195d7c)==2);assert(burn.half(B+2)==2);
 assert(burn.run(c));assert(burn.word(B+0xf14)==0x03000009); // Real wait result0.
}
void regenerationAndFreeze(){
 Fixture f(0xa);f.actor(U,1);f.mutateHealAttribute=true;f.hold=0x8014335c;f.holds=1;BattleTurnCursor c;
 assert(!f.run(c));assert(f.half(U+0x10c)==7 && f.word(U+0x208)==5 && f.word(U+0x20c)==7);
 assert(f.run(c));assert(f.count(0x80126bec)==1);const auto& q=f.call(0x8014335c);assert(q.reals[1]==32 && q.integers[1]==2);
 Fixture happy(0xe);happy.actor(U,1);happy.bytes[U+0x2ee]=3;happy.hold=0x80005da0;happy.holds=1;
 assert(!happy.run(c));assert(happy.run(c));assert(happy.count(0x80005da0)==3);assert(happy.half(U+0x10c)==8);
 assert(happy.call(0x8014335c).reals[1]==22);assert(happy.word(B+0xf14)==0x0300000f && happy.count(0x8011ace8)==0);
 Fixture ice(0x20);ice.actor(U,1);ice.recovering=9;assert(ice.run(c));assert(ice.count(0x800374ac)==0 && ice.count(0x800d98bc)==0);
 const auto& freeze=ice.call(0x801c3690);assert(freeze.reals[1]==12 && freeze.reals[3]==1);assert(ice.call(0x800fdfd4).integers[5]==282);
}
void everyChildHold(){
 for(unsigned i:{3u,4u,6u,8u,0xau,0xcu,0xeu,0x10u,0x12u,0x20u,0x3au,0x4cu,0x4du,7u}){
  auto prepare=[&](){Fixture f(i);f.actor(U,1);f.recovering=i==0x20?9:i==0x3a?20:1;f.bytes[U+0x2ee]=2;f.bytes[U+0x2ef]=2;f.word(B+0xefc,3);f.burnActive=1;f.reads.clear();return f;};
  auto direct=prepare();BattleTurnCursor done;assert(direct.run(done));
  for(unsigned n=0;n<direct.calls.size();++n){auto f=prepare();f.holdAt=int(n);BattleTurnCursor c;assert(!f.run(c));assert(c.active());assert(f.run(c));
   assert(f.calls.size()==direct.calls.size() && f.bytes==direct.bytes && f.reads==direct.reads);
   for(unsigned k=0;k<f.calls.size();++k){const auto& a=f.calls[k];const auto& b=direct.calls[k];assert(a.function==b.function && a.integers==b.integers && a.reals==b.reals);}
  }
 }
}
void ownerAndWait(){
 Fixture f(7);f.wait=1;f.conclusion=1;f.hold=0x8011aa34;f.holds=1;BattleTurnCursor c;
 assert(!f.run(c));auto changed=f.services();changed.user=nullptr;assert(!resumePackedBattleTurn(B,changed,c));assert(!resumePackedBattleTurn(B+4,f.services(),c));
 assert(f.run(c));assert(f.count(0x8011ace8)==1 && f.word(B+4)==3 && f.word(B+0xf24)==0x07000000);
 Fixture timers(0x40);timers.word(B+0x19050,3);timers.word(B+0x19054,2);timers.word(B+0x19058,1);timers.actor(U,1);timers.half(U+0x13c,2);timers.half(U+0x13e,1);
 assert(timers.run(c));assert(timers.word(B+0x19050)==2&&timers.word(B+0x19054)==1&&timers.word(B+0x19058)==0);assert(timers.half(U+0x13c)==1&&timers.half(U+0x13e)==0);
}
}
int main(){dispatch();recovery();intro();poisonAndBurn();regenerationAndFreeze();everyChildHold();ownerAndWait();}
