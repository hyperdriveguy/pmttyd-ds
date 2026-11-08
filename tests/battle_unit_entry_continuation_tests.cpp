#include "battle_unit_entry.hpp"
#include "battle_unit_entry_continuation.hpp"
#include <map>
#include "battle_packed_unit_initialization.hpp"
#include "battle_packed_unit_resources.hpp"
#include "battle_unit_resources.hpp"
#include "battle_packed_unit_vectors.hpp"
#include "battle_packed_pose_lookup.hpp"
#include "battle_packed_display_entry.hpp"
#include "battle_packed_part.hpp"
#include "battle_jump_motion.hpp"
#include <cmath>
#include "battle_event_callbacks.hpp"
#include <cassert>
#include <deque>
#include <vector>
#include <iostream>
#include <cstring>
using namespace ttyd;
struct Fixture {
 EventNativeRuntime vm{0x81000000,{}};uint32_t heap=0x10000;bool failAllocation=false;std::vector<uint32_t> allocations,calls,poseEntries;std::deque<EventEntry> roots;
 Fixture(bool constants=true){assert(vm.memory.registerRam(0x1000,std::vector<uint8_t>(0x30000)));if(constants)assert(vm.memory.registerRam(0x804226a0,std::vector<uint8_t>{0xff,0xff,0xff,0xff,0,0,0,0,0x3f,0x80,0,0}));w(0x5000,0x6000);w(0x5008,0x04000002);w(0x6000,222);w(0x60b8,0x7000);w(0x60bc,0x9876);b(0x60b4,2);b(0x5004,0);b(0x600e,8);h(0x6008,10);h(0x600a,5);h(0x808a,3);h(0x808e,10);h(0x8090,5);h(0x8070,10);h(0x8074,5);b(0x100a,1);w(0x7040,1);w(0x7000,1);w(0x704c,2);w(0x7048,0xabcd);w(0x7094,0xdef0);w(0x5018,4);}
 void w(uint32_t a,uint32_t v){assert(vm.memory.writeWord(a,v));}uint32_t w(uint32_t a){uint32_t v;assert(vm.memory.readWord(a,v));return v;}void b(uint32_t a,uint8_t v){assert(vm.memory.writeByte(a,v));}uint8_t b(uint32_t a){uint8_t v;assert(vm.memory.readByte(a,v));return v;}void h(uint32_t a,uint16_t v){assert(vm.memory.writeHalf(a,v));}uint16_t h(uint32_t a){uint16_t v;assert(vm.memory.readHalf(a,v));return v;}
 BattleUnitEntryServices services(){BattleUnitEntryServices s;s.memory=nativeBattleActorServices(vm);s.user=this;s.storeByte=[](void* p,uint32_t a,uint8_t v){static_cast<Fixture*>(p)->b(a,v);};s.allocate=[](void* p,uint32_t size){auto& f=*static_cast<Fixture*>(p);f.allocations.push_back(size);if(f.failAllocation)return uint32_t(0);const auto address=f.heap;f.heap+=(size+3)&~3u;return address;};s.pouch=[](void*){return uint32_t(0x8000);};s.unary=[](void* p,uint32_t fn,uint32_t u){auto& f=*static_cast<Fixture*>(p);f.calls.push_back(fn);initializePackedBattleUnitHelper(fn,u,nativeBattleActorServices(f.vm));if(fn==0x80127134){PackedUnitResourceServices rs;rs.memory=nativeBattleActorServices(f.vm);rs.pouch=[](void*){return uint32_t(0x8000);};rs.partyIndex=[](void*,int32_t kind)->int32_t{return battlePartyIndex(kind);};importPackedUnitResources(0x1000,u,rs);}assert(f.w(0x1020)==u);if(fn==0x801011fc)assert(f.w(u+0x2d8)==0xa000 && f.w(u+0x2dc)==0xb000);};s.vector=[](void* p,uint32_t fn,uint32_t,const std::array<float,3>& values){auto& f=*static_cast<Fixture*>(p);f.calls.push_back(fn);if(fn==0x80128524 || fn==0x80128574 || fn==0x801284a4 || fn==0x80128470)assert((values==std::array<float,3>{1,1,1}));else assert((values==std::array<float,3>{0,0,0}));};s.bodyPart=[](void* p,uint32_t u){return static_cast<Fixture*>(p)->w(u+0x14);};s.poseName=[](void* p,uint32_t,int32_t type){static_cast<Fixture*>(p)->calls.push_back(uint32_t(type));assert(type==65 || type==43);return uint32_t(type==65?0xa000:0xb000);};s.create=[](void* p,uint32_t script,int32_t priority,uint32_t flags)->EventEntry&{auto& f=*static_cast<Fixture*>(p);assert(script==0x9876 && priority==10 && flags==0);f.calls.push_back(script);f.roots.emplace_back();f.roots.back().id=90;return f.roots.back();};return s;}
};
struct Retained {
 Fixture fixture;
 std::shared_ptr<const uint8_t> lifetime=std::make_shared<const uint8_t>(0);
 bool live=true, hold=false, reenter=false;
 unsigned calls=0;
 BattleUnitEntryCursor cursor;
 std::map<const BattleUnitEntryRequest*,unsigned> attempts;
 const BattleUnitEntryRequest* pending=nullptr;
 BattleUnitEntryRequest saved;
 std::vector<uint8_t> image;
 std::vector<BattleUnitEntryRequest> accepted;
 std::vector<uint8_t> bytes(){std::vector<uint8_t> out(0x30000);for(unsigned i=0;i<out.size();++i)out[i]=fixture.b(0x1000+i);return out;}
 BattleUnitEntryContinuationServices services(){
  BattleUnitEntryContinuationServices s;s.actors=nativeBattleActorServices(fixture.vm);s.lifetime=lifetime;s.user=this;
  s.current=[](void* p){return static_cast<Retained*>(p)->live;};
  s.call=[](void* p,const BattleUnitEntryRequest& q)->std::optional<BattleUnitEntryOutput>{
   auto& r=*static_cast<Retained*>(p);++r.calls;
   if(r.reenter){assert(!resumePackedBattleUnitEntry(0x1000,0x5000,r.services(),r.cursor));assert(!r.cursor.reset());}
   if(r.hold){
    if(!r.pending){r.pending=&q;r.saved=q;r.image=r.bytes();}
    else {assert(r.pending==&q && r.saved.function==q.function && r.saved.callsite==q.callsite && r.saved.integers==q.integers && r.saved.vector==q.vector);assert(r.bytes()==r.image);}
    if(++r.attempts[&q]<3)return {};
    r.pending=nullptr;r.attempts.erase(&q);
   }
   r.accepted.push_back(q);auto legacy=r.fixture.services();BattleUnitEntryOutput out;
   const auto a=q.integers[0],b=q.integers[1],c=q.integers[2];
   switch(q.function){
   case 0x800f88cc:out.value=battleRegistryUnit(a,int32_t(b),legacy.memory);break;
   case 0x800f88b4:setBattleRegistryUnit(a,int32_t(b),c,legacy.memory);break;
   case 0x800f8934:out.value=legacy.allocate(legacy.user,a);break;
   case 0x800050b4:out.value=eventFillMemory(a,int32_t(b),c,legacy.memory.memory);break;
   case 0x800d5eb0:out.value=legacy.pouch(legacy.user);break;
   case 0x80128c98:out.value=packedBodyPartId(a,legacy.memory.memory);break;
   case 0x80128cd8:out.value=packedUnitPart(a,int32_t(b),legacy.memory.memory);break;
   case 0x8012772c:out.value=legacy.poseName(legacy.user,a,int32_t(b));break;
   case 0x800374ac:out.event=&legacy.create(legacy.user,a,int32_t(b),c);break;
   default:
    if(q.function==0x80128c6c || q.function==0x801283d8 || q.function==0x801289a0 ||
     q.function==0x80128680 || q.function==0x801288b4 || q.function==0x80128604 ||
     q.function==0x80128524 || q.function==0x80128574 || q.function==0x801284a4 ||
     q.function==0x80128470 || q.function==0x80128654 || q.function==0x801287b4 || q.function==0x801285d0)
      legacy.vector(legacy.user,q.function,a,q.vector);
    else legacy.unary(legacy.user,q.function,a);
   }
   return out;
  };return s;
 }
 uint32_t run(){for(unsigned i=0;i<3000;++i){const auto result=resumePackedBattleUnitEntry(0x1000,0x5000,services(),cursor);if(result)return *result;}assert(false);return 0;}
};
struct DisplayFixture {
 Fixture f;BattleDisplayEntryCursor cursor;
 std::shared_ptr<const uint8_t> lifetime=std::make_shared<const uint8_t>(0);
 bool live=true,hold=true;unsigned attempt=0,entries=0;
 const BattleUnitEntryRequest* held=nullptr;BattleUnitEntryRequest saved;
 std::vector<uint32_t> calls;
 DisplayFixture(){
  f.w(0x9014,0xa000);for(unsigned i=0;i<4;++i){const auto p=0xa000+0x500*i;
   f.w(p,i<3?p+0x500:0);f.w(p+4,0xc000);f.w(p+0x1bc,i?0xc100:0);f.w(p+0x1c0,77);}
  f.w(0xc008,0xc300);f.w(0xc100,28);f.w(0xc104,0xc200);
  f.b(0xc200,'Z');f.b(0xc201,'_');f.b(0xc202,'1');f.b(0xc203,0);
 }
 BattleUnitEntryContinuationServices services(){BattleUnitEntryContinuationServices s;
  s.actors=nativeBattleActorServices(f.vm);s.user=this;s.lifetime=lifetime;
  s.current=[](void* p){return static_cast<DisplayFixture*>(p)->live;};
  s.call=[](void* p,const BattleUnitEntryRequest& q)->std::optional<BattleUnitEntryOutput>{
   auto& r=*static_cast<DisplayFixture*>(p);
   assert(!resumePackedBattleDisplayEntry(0x9000,r.services(),r.cursor) && !r.cursor.reset());
   if(r.hold){if(!r.held){r.held=&q;r.saved=q;}else assert(r.held==&q && q.function==r.saved.function && q.callsite==r.saved.callsite && q.integers==r.saved.integers);
    if(++r.attempt<3)return {};
    r.attempt=0;r.held=nullptr;}
   r.calls.push_back(q.function);BattleUnitEntryOutput out;
   switch(q.function){
   case 0x80103cd0:out.value=searchPackedBattlePoseTable(q.integers[0],int32_t(q.integers[1]),sMemory(r));break;
   case 0x8026d5fc:{unsigned i=0;for(;;++i){const auto c=r.f.b(q.integers[1]+i);r.f.b(q.integers[0]+i,c);if(!c)break;}out.value=q.integers[0];break;}
   case 0x8004176c:assert(q.integers[0]==0xc300 && q.integers[1]==1);out.value=++r.entries;break;
   default:assert(false);
   }return out;
  };return s;
 }
 static EventMemoryServices sMemory(DisplayFixture& r){return nativeBattleActorServices(r.f.vm).memory;}
 void run(){for(unsigned i=0;i<100;++i)if(resumePackedBattleDisplayEntry(0x9000,services(),cursor))return;assert(false);}
};
void displayTests(){
 DisplayFixture f;f.run();assert(f.entries==3 && f.f.w(0xa1c0)==0xffffffff && f.f.w(0xa6c0)==1 && f.f.w(0xabc0)==2 && f.f.w(0xb0c0)==3);
 assert(f.calls.size()==9);const auto calls=f.calls;f.run();assert(f.calls==calls);
 DisplayFixture nullBank;nullBank.f.w(0xc008,0);nullBank.run();assert(nullBank.entries==0 && nullBank.calls.empty());
 DisplayFixture retired;assert(!resumePackedBattleDisplayEntry(0x9000,retired.services(),retired.cursor));const auto storage=retired.f.w(0xa6c0);
 retired.live=false;assert(!resumePackedBattleDisplayEntry(0x9000,retired.services(),retired.cursor));retired.live=true;
 assert(!resumePackedBattleDisplayEntry(0x9000,retired.services(),retired.cursor) && retired.f.w(0xa6c0)==storage);
}
int main(){
 displayTests();
 for(uint32_t kind:{220u,222u,208u,224u,230u,231u})for(bool script:{false,true}){
  Fixture expected;expected.w(0x6000,kind);if(!script)expected.w(0x60bc,0);
  const auto original=packedBattleUnitEntry(0x1000,0x5000,expected.services());
  Retained now;now.fixture.w(0x6000,kind);if(!script)now.fixture.w(0x60bc,0);
  now.hold=true;now.reenter=true;assert(now.run()==original);
  for(unsigned i=0;i<0x30000;++i)assert(expected.b(0x1000+i)==now.fixture.b(0x1000+i));
  assert(expected.allocations==now.fixture.allocations && expected.calls==now.fixture.calls);
  assert(expected.roots.size()==now.fixture.roots.size());if(script)assert(now.fixture.roots[0].owner==0);
  const auto completed=now.calls;assert(now.run()==original && now.calls==completed);
  assert(!resumePackedBattleUnitEntry(0x1000,0x5030,now.services(),now.cursor));
  now.live=false;assert(!resumePackedBattleUnitEntry(0x1000,0x5000,now.services(),now.cursor));
  now.live=true;assert(!resumePackedBattleUnitEntry(0x1000,0x5000,now.services(),now.cursor));
 }
 Retained full;for(unsigned i=0;i<64;++i)full.fixture.w(0x1020+4*i,1);full.hold=true;assert(full.run()==0 && full.fixture.allocations.empty());
 Retained noheap;noheap.fixture.failAllocation=true;noheap.hold=true;assert(noheap.run()==0 && noheap.fixture.w(0x1020)==0);
 Retained retirement;retirement.hold=true;assert(!resumePackedBattleUnitEntry(0x1000,0x5000,retirement.services(),retirement.cursor));
 const auto count=retirement.calls;retirement.live=false;assert(!resumePackedBattleUnitEntry(0x1000,0x5000,retirement.services(),retirement.cursor) && retirement.calls==count);
 // Source body lookup chooses the first flagged body, independently of the
 // cached last flagged definition ID; both pose requests keep that same part.
 Retained bodies;bodies.fixture.w(0x708c,1);bodies.hold=true;const auto u=bodies.run();assert(bodies.fixture.w(u+0x2d4)==2);
 uint32_t first=0;for(const auto& q:bodies.accepted)if(q.function==0x8012772c){if(!first)first=q.integers[0];assert(q.integers[0]==first);}assert(first==bodies.fixture.w(u+0x14));
 std::cout<<"Retained unit construction: source definitions, every child held twice, early publication, first body, allocation returns, reentry and retirement passed\n";
}
