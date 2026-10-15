#include <memory>
#include "battle_weapon_ui_continuation.hpp"
#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <tuple>
#include <vector>
using namespace ttyd;
int32_t signedWord(uint32_t n){return n<0x80000000u?int32_t(n):int32_t(int64_t(n)-0x100000000LL);}
struct Fixture {
 static constexpr uint32_t B=0x10000,B2=0x30000,U=0x50000,V=0x60000,C=0x70000,W=0x80000,Old=0x90000,Rows=0xa0000;
 std::map<uint32_t,uint32_t> words{{0x8041eb90,B},{0x8041e80c,0xb0000},{B+0x1c78,Old},{B+0x420,1},{B+0x24,U},{B+0x28,V},{U+8,222},{V+8,224},{C,17},{C+8,19},{B+0x1bb4,2},{B+0x1bbc,5},{B+0x1bc0,1},{B+0x1bc8,3},{0x8042242c,0x11223344}};
 std::map<uint32_t,uint8_t> bytes;
 using Write=std::tuple<uint32_t,uint32_t,unsigned>;
 std::vector<std::pair<uint32_t,unsigned>> reads;std::vector<Write>writes;
 using Request=std::tuple<uint32_t,uint32_t,uint32_t,std::array<uint32_t,2>>;
 std::vector<Request> calls;
 std::function<void(Fixture&,uint32_t)> onRead,onWrite;
 std::function<void(Fixture&,const BattleWeaponUiRequest&)> onRequest;
 int hold=-1;unsigned attempts=0;const BattleWeaponUiRequest* held=nullptr;
 bool alive=true,probe=false;uint32_t allocated=W;BattleWeaponUiCursor cursor;
 uint32_t word(uint32_t a){reads.emplace_back(a,4);const auto value=words[a];if(onRead)onRead(*this,a);return value;}
 void store(uint32_t a,uint32_t n){words[a]=n;writes.emplace_back(a,n,4);if(onWrite)onWrite(*this,a);}
 void put(uint32_t a,uint8_t n){bytes[a]=n;writes.emplace_back(a,n,1);if(onWrite)onWrite(*this,a);}
 BattleActorServices actors(){BattleActorServices a;a.user=this;a.memory.user=this;a.memory.word=[](void*p,uint32_t a){return static_cast<Fixture*>(p)->word(a);};a.memory.storeWord=[](void*p,uint32_t a,uint32_t n){static_cast<Fixture*>(p)->store(a,n);};a.memory.storeByte=[](void*p,uint32_t a,uint8_t n){static_cast<Fixture*>(p)->put(a,n);};return a;}
 std::optional<uint32_t> request(const BattleWeaponUiRequest&q){assert(q.count==2);if(int(calls.size())==hold&&attempts++<2){if(held)assert(held==&q);else held=&q;if(probe){assert(!cursor.reset());assert(!resumePackedBattleWeaponUiInit(0xdead,C,Rows,1,services(),cursor));assert(!resumePackedBattleWeaponUiEnd(B,services(),cursor));}return {};}
  held=nullptr;calls.emplace_back(q.function,q.callsite,q.caller,q.arguments);if(onRequest)onRequest(*this,q);return q.function==0x8002ff58?allocated:0;
 }
 BattleWeaponUiServices services(){BattleWeaponUiServices s;s.actors=actors();s.user=this;s.allocationCurrent=[](void*p){return static_cast<Fixture*>(p)->alive;};s.request=[](void*p,const BattleWeaponUiRequest&q){return static_cast<Fixture*>(p)->request(q);};return s;}
 uint32_t map(uint32_t fn,uint32_t site,uint32_t caller,uint32_t arg){BattleWeaponUiRequest q;q.function=fn;q.callsite=site;q.caller=caller;q.arguments={word(0x8041e80c),arg};q.count=2;const auto result=request(q);assert(result);return *result;}
 // Direct recovered source-body oracle, no readiness policy or retained stack.
 void originalInit(uint32_t flags){
  uint32_t work=0,sub=0;const auto battle=word(0x8041eb90);const auto previous=word(battle+0x1c78);
  if(previous)map(0x8002fdc4,0x800f8920,0x8011797c,previous);
  store(battle+0x1c78,map(0x8002ff58,0x800f8948,0x80117984,0x124));work=word(battle+0x1c78);
  store(work,C);store(work+0xc,0);store(work+0x10,0);put(work+0x14,0);store(work+0x18,word(C));
  const auto unit=battleRegistryUnit(battle,signedWord(word(battle+0x420)),actors());sub=battle+(word(unit+8)==222?0x1bb4:0x1bc0);
  store(work+0x98,word(0x8042242c));for(uint32_t j=0;j<7;++j){store(work+0xa4+4*j,0);store(work+0xc0+4*j,1);store(work+0xdc+4*j,0-j);store(work+0xf8+4*j,0);}
  store(work+8,Rows);uint32_t index=word(sub),i=0;
  for(;;){const auto n=word(sub+8);if(signedWord(i)>=signedWord(n))break;++index;if(signedWord(index)>=signedWord(n))index=0;const auto row=work+4*index;
   store(row+0x48,0);store(row+0x84,i);store(row+0x20,0);store(row+0x20,word(row+0x20)|1);
   const auto n2=word(sub+8);store(row+0x34,(flags&1)?(i==n2-1?0:10):(i==n2-1?1:11));++i;
  }
 }
 void originalEnd(){const auto old=word(B+0x1c78);if(old){map(0x8002fdc4,0x800f8920,0x8011602c,old);store(B+0x1c78,0);}}
};
using Setup=std::function<void(Fixture&)>;
void compare(bool end,uint32_t flags,const Setup& setup={}){
 Fixture expected;if(setup)setup(expected);if(end)expected.originalEnd();else expected.originalInit(flags);
 Fixture immediate;if(setup)setup(immediate);auto run=[&](Fixture&f){return end?resumePackedBattleWeaponUiEnd(Fixture::B,f.services(),f.cursor):resumePackedBattleWeaponUiInit(0xdead,Fixture::C,Fixture::Rows,flags,f.services(),f.cursor);};
 assert(run(immediate)&&!immediate.cursor.active());assert(immediate.words==expected.words&&immediate.bytes==expected.bytes&&immediate.reads==expected.reads&&immediate.writes==expected.writes&&immediate.calls==expected.calls);
 for(unsigned gate=0;gate<immediate.calls.size();++gate){Fixture delayed;if(setup)setup(delayed);delayed.hold=int(gate);delayed.probe=true;assert(!run(delayed)&&delayed.cursor.active());auto reads=delayed.reads;auto writes=delayed.writes;auto words=delayed.words;
  auto bad=delayed.services();bad.actors.audienceLifecycle.user=&expected;assert(!resumePackedBattleWeaponUiInit(0xdead,Fixture::C,Fixture::Rows,flags,bad,delayed.cursor));assert(!resumePackedBattleWeaponUiEnd(Fixture::B,bad,delayed.cursor));
  assert(!run(delayed)&&delayed.reads==reads&&delayed.writes==writes&&delayed.words==words);assert(run(delayed));assert(delayed.words==immediate.words&&delayed.bytes==immediate.bytes&&delayed.reads==immediate.reads&&delayed.writes==immediate.writes&&delayed.calls==immediate.calls);
 }
}
int main(){
 for(uint32_t flags:{0u,1u,2u,3u})for(uint32_t count:{0u,1u,3u,5u,0xffffffffu})for(uint32_t index:{0u,1u,2u,0xffffffffu})compare(false,flags,[=](auto&f){f.words[Fixture::B+0x1bbc]=count;f.words[Fixture::B+0x1bb4]=index;});
 compare(false,0,[](auto&f){f.words[Fixture::U+8]=224;});compare(false,1,[](auto&f){f.words[Fixture::B+0x1c78]=0;});
 compare(false,1,[](auto&f){f.onRequest=[](auto&f,const auto&q){if(q.function==0x8002fdc4){f.store(0x8041e80c,0xb1000);f.store(0x8041eb90,Fixture::B2);}};});
 compare(false,0,[](auto&f){f.onWrite=[](auto&f,uint32_t a){if(a==Fixture::B+0x1c78)f.words[a]=Fixture::W+0x1000;if(a==Fixture::W+0x1000+0x18)f.words[Fixture::B+0x420]=2;};});
 compare(false,0,[](auto&f){f.onWrite=[](auto&f,uint32_t a){if(a==Fixture::W+0x24)f.words[a]=0x100;};});
 compare(false,1,[](auto&f){f.onWrite=[](auto&f,uint32_t a){if(a==Fixture::W+0x84+4*3)f.words[Fixture::B+0x1bbc]=4;};});
 // Source pointer0 is not missing. This fixture explicitly admits low guest
 // memory to observe original unchecked writes; production must admit memory.
 compare(false,0,[](auto&f){f.allocated=0;});
 compare(true,0);compare(true,0,[](auto&f){f.words[Fixture::B+0x1c78]=0;});compare(true,0,[](auto&f){f.onRequest=[](auto&f,const auto&){f.store(0x8041eb90,Fixture::B2);f.store(Fixture::B+0x1c78,0x1234);};});
 Fixture fresh;fresh.hold=0;assert(!resumePackedBattleWeaponUiInit(7,Fixture::C,Fixture::Rows,0,fresh.services(),fresh.cursor));fresh.words[0x8041e80c]=0xc0000;assert(!resumePackedBattleWeaponUiInit(7,Fixture::C,Fixture::Rows,0,fresh.services(),fresh.cursor));assert(resumePackedBattleWeaponUiInit(7,Fixture::C,Fixture::Rows,0,fresh.services(),fresh.cursor));assert(std::get<3>(fresh.calls[0])[0]==0xb0000&&std::get<3>(fresh.calls[1])[0]==0xc0000);
 for(unsigned boundary:{0u,1u}){Fixture stale;unsigned calls=0;stale.onRequest=[&](auto&f,const auto&){if(calls++==boundary)f.alive=false;};assert(!resumePackedBattleWeaponUiInit(0xdead,Fixture::C,Fixture::Rows,0,stale.services(),stale.cursor)&&stale.cursor.stale());auto reads=stale.reads;auto writes=stale.writes;stale.alive=true;assert(!resumePackedBattleWeaponUiInit(0xdead,Fixture::C,Fixture::Rows,0,stale.services(),stale.cursor)&&stale.reads==reads&&stale.writes==writes);assert(stale.cursor.reset());}
 Fixture absent;auto missing=absent.services();missing.allocationCurrent=nullptr;assert(!resumePackedBattleWeaponUiInit(0,Fixture::C,Fixture::Rows,0,missing,absent.cursor)&&absent.reads.empty());
 std::cout<<"Whole distinct weapon UI source graph, map/free retention, subtype class-cursor and native ownership passed\n";
}
