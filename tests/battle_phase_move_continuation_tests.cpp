#include <memory>
#include "battle_phase_move_continuation.hpp"
#include "battle_phase.hpp"
#include "battle_sequence_startup.hpp"
#include "battle_majinai_query.hpp"
#include <cassert>
#include <cstring>
#include <functional>
#include <iostream>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
struct Fixture {
 static constexpr uint32_t B=0x10000000,C=0x11000000,U=0x20000000,P=0x30000000,E=0x40000000;
 std::map<uint32_t,uint8_t> bytes;
 struct Write {uint32_t address,value,size;};std::vector<Write> writes;
 std::vector<uint32_t> reads;std::vector<BattleMoveRequest> calls;
 int stop=-1;bool release=false;unsigned completed=0;
 uint32_t wait=0,concluded=0,ended=0,reaction=0,message=0,announce=0,event=E;
 std::map<uint32_t,uint32_t> statuses;
 std::function<void(const BattleMoveRequest&)> onCall;
 std::function<void(uint32_t)> onStore;
 BattleMoveCursor* borrowedMove=nullptr;BattlePhaseCursor* borrowedPhase=nullptr;
 Fixture(uint32_t index=0){word(0x8041eb90,B);word(0x8041eb00,P);word(B+0xf14,0x0300004c);word(B+0xf18,0x04000002);word(B+0xf1c,0x05000000+index);
  for(unsigned i=0;i<64;++i){word(B+0x120+4*i,0xffffffff);word(B+0x220+8*i,0xffffffff);word(B+0x224+8*i,100+i);}
  word(E+0x15c,123);byte(P+0x5b8,2);writes.clear();reads.clear();}
 void byte(uint32_t a,uint8_t n){bytes[a]=n;}
 uint8_t byte(uint32_t a)const{auto i=bytes.find(a);if(i!=bytes.end())return i->second;
  assert((a>=B && a<B+0x19100) || (a>=C && a<C+0x19100) || (a>=U && a<U+0x40000) || (a>=P && a<P+0x1000) || (a>=E && a<E+0x10000) || a<0x1000);return 0;}
 void word(uint32_t a,uint32_t n){for(unsigned j=0;j<4;++j)byte(a+j,uint8_t(n>>(24-8*j)));}
 uint32_t word(uint32_t a){reads.push_back(a);uint32_t n=0;for(unsigned j=0;j<4;++j)n=(n<<8)|byte(a+j);return n;}
 void half(uint32_t a,uint16_t n){byte(a,uint8_t(n>>8));byte(a+1,uint8_t(n));}
 uint16_t half(uint32_t a){reads.push_back(a);return uint16_t(uint16_t(byte(a))<<8|byte(a+1));}
 uint32_t actor(unsigned slot,int kind=224,uint8_t state=0,uint8_t remaining=1){const auto u=U+0x1000*slot;
  word(B+0x20+4*slot,u);word(B+0x120+4*slot,slot);word(u,slot);word(u+8,uint32_t(kind));word(u+0x2c,0x04000002);byte(u+0x20,state);byte(u+0x22,remaining);return u;}
 BattleActorServices actors(){BattleActorServices s;s.user=this;s.memory.user=this;
  s.byte=[](void* u,uint32_t a){auto& f=*static_cast<Fixture*>(u);f.reads.push_back(a);return f.byte(a);};
  s.memory.word=[](void* u,uint32_t a){return static_cast<Fixture*>(u)->word(a);};
  s.memory.half=[](void* u,uint32_t a){return static_cast<Fixture*>(u)->half(a);};
  s.memory.storeWord=[](void* u,uint32_t a,uint32_t n){auto& f=*static_cast<Fixture*>(u);f.writes.push_back({a,n,4});f.word(a,n);if(f.onStore)f.onStore(a);};
  s.memory.storeHalf=[](void* u,uint32_t a,uint16_t n){auto& f=*static_cast<Fixture*>(u);f.writes.push_back({a,n,2});f.half(a,n);if(f.onStore)f.onStore(a);};
  s.memory.storeByte=[](void* u,uint32_t a,uint8_t n){auto& f=*static_cast<Fixture*>(u);f.writes.push_back({a,n,1});f.byte(a,n);if(f.onStore)f.onStore(a);};return s;
 }
 BattlePhaseServices phase(){BattlePhaseServices s;s.actors=actors();s.real=[](void* u,uint32_t a){const auto n=static_cast<Fixture*>(u)->word(a);float f;std::memcpy(&f,&n,4);return f;};
  s.status=[](void* u,uint32_t actor,int32_t){auto& f=*static_cast<Fixture*>(u);return f.statuses[actor]!=0;};return s;}
 BattleMoveServices services(){BattleMoveServices s;s.actors=actors();s.user=this;s.call=[](void* u,const BattleMoveRequest& q)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(u);
  if(int(f.completed)==f.stop && !f.release)return {};
  if(f.borrowedMove){assert(!f.borrowedMove->reset());assert(!resumePackedBattleMove(B,f.services(),*f.borrowedMove));}
  if(f.borrowedPhase){assert(!f.borrowedPhase->reset());assert(!resumePackedBattlePhase(B,f.services(),*f.borrowedPhase));}
  f.calls.push_back(q);++f.completed;if(f.onCall)f.onCall(q);
  switch(q.function){
  case 0x8011b0d0:assert(q.integers[0]==B);buildBattlePhaseTables(B,f.phase());return 0;
  case 0x800f7718:case 0x800f76d4:{BattleMajinaiQueryServices s;s.memory=f.actors().memory;s.user=&f;s.byte=f.actors().byte;
   if(q.function==0x800f7718)battleMajinaiDone(s);else battleMajinaiEndCheck(s);return 0;}
  case 0x800374ac:assert(q.integers[0]==0x803634f8 && q.integers[1]==10 && q.integers[2]==0);return f.event;
  case 0x8011ace8:assert(q.integers[0]==B);return f.wait;
  case 0x8011aa34:assert(q.integers[0]==B);return f.concluded;
  case 0x801297bc:assert(q.integers[0]>=U && q.integers[0]<U+0x40000 && q.integers[1]<=1);return 0;
  case 0x800050b4:assert(q.integers[0]==B+0x18ffc && !q.integers[1] && q.integers[2]==12);return eventFillMemory(q.integers[0],0,12,f.actors().memory);
  case 0x80127814:assert(q.integers[1]==27);return f.statuses[q.integers[0]];
  case 0x8011b2dc:assert(q.integers[0]==B);return 0; // Admission fixture, not replacement source Act.
  case 0x80127434:assert(q.integers[1]==0);f.actors().memory.storeWord(&f,q.integers[0]+0x264,0);return 0;
  case 0x8011bb88:return f.ended;
  case 0x801a7c68:assert(q.integers[0]>=0x04000000 && q.integers[0]<=0x04000006);return 0;
  case 0x801a6ce8:return f.reaction;
  case 0x801956f8:return f.message;
  case 0x80195af8:return f.announce;
  default:assert(false);return {};
  }
 };return s;}
 uint32_t move(){return word(B+0xf1c)-0x05000000;}
 unsigned callsTo(uint32_t fn)const{unsigned n=0;for(const auto& q:calls)if(q.function==fn)++n;return n;}
};
void dispatch(){
 const uint32_t final[]{5,5,5,3,5,5,8,7,9,9,8,11,0,14,0};
 for(uint32_t index=0;index<15;++index){Fixture f(index);f.actor(0);BattleMoveCursor c;
  assert(resumePackedBattleMove(Fixture::B,f.services(),c) && !c.active() && f.move()==final[index]);
  if(index==8)assert(!f.callsTo(0x8011b2dc));
 }
 for(uint32_t state:{0x04ffffffu,0x0500000fu,0u,0xffffffffu}){Fixture f;f.word(Fixture::B+0xf1c,state);BattleMoveCursor c;
  assert(resumePackedBattleMove(Fixture::B,f.services(),c) && f.calls.empty() && f.writes.empty());}
 for(uint32_t state:{0x03ffffffu,0x04000000u,0x04000001u,0x04000005u,0x04000006u,0x04000007u,0x84000001u}){
  Fixture f(9);f.word(Fixture::B+0xf18,state);BattlePhaseCursor c;assert(resumePackedBattlePhase(Fixture::B,f.services(),c));
  if(state==0x04000000)assert(f.word(Fixture::B+0xf18)==state+1 && f.move()==0 && f.calls.empty());
  else if(state==0x04000006)assert(f.word(Fixture::B+0xf14)==0x0300004d && f.word(Fixture::B+0xf18)==0x04000000 && f.calls.empty());
  else assert(f.callsTo(0x8011b2dc)==uint32_t(state>0x04000000 && state<0x04000006));
 }
}
void heldPrefixes(){
 for(uint32_t index:{0u,2u,4u,6u,7u,8u,9u,10u,11u,12u}){
  Fixture reference(index);const auto u=reference.actor(0,222);reference.word(u+0x294,1);reference.word(u+0x298,1);
  if(index==2){reference.byte(Fixture::P+0x5ba,2);reference.word(Fixture::B+0xf18,0x04000004);}
  if(index==6)reference.word(Fixture::B+0x220,0);
  BattleMoveCursor baseline;assert(resumePackedBattleMove(Fixture::B,reference.services(),baseline));
  for(unsigned stop=0;stop<reference.completed;++stop){Fixture f(index);const auto unit=f.actor(0,222);f.word(unit+0x294,1);f.word(unit+0x298,1);
   if(index==2){f.byte(Fixture::P+0x5ba,2);f.word(Fixture::B+0xf18,0x04000004);}
   if(index==6)f.word(Fixture::B+0x220,0);
   f.stop=int(stop);BattleMoveCursor c;
   assert(!resumePackedBattleMove(Fixture::B,f.services(),c) && c.active() && c.pending());const auto stores=f.writes.size(),reads=f.reads.size();
   assert(!resumePackedBattleMove(Fixture::B,f.services(),c) && f.writes.size()==stores && f.reads.size()==reads && f.completed==stop);
   auto wrong=f.services();wrong.user=nullptr;assert(!resumePackedBattleMove(Fixture::B,wrong,c));assert(!resumePackedBattleMove(Fixture::C,f.services(),c));
   wrong=f.services();wrong.actors.audienceLifecycle.user=&reference;
   assert(!resumePackedBattleMove(Fixture::B,wrong,c));
   wrong=f.services();wrong.actors.audienceLifecycle.event=[](void*,BattleAudienceLifecycleEvent,uint32_t,uint32_t){};
   assert(!resumePackedBattleMove(Fixture::B,wrong,c));
   assert(f.writes.size()==stores && f.reads.size()==reads && f.completed==stop);
   f.release=true;f.borrowedMove=&c;assert(resumePackedBattleMove(Fixture::B,f.services(),c));
   assert(f.bytes==reference.bytes && f.calls.size()==reference.calls.size());
   for(size_t j=0;j<f.calls.size();++j)assert(f.calls[j].function==reference.calls[j].function && f.calls[j].integers==reference.calls[j].integers);
  }
 }
 Fixture f(9);f.stop=0;BattlePhaseCursor phase;assert(!resumePackedBattlePhase(Fixture::B,f.services(),phase));
 const auto heldReads=f.reads.size(),heldWrites=f.writes.size();
 auto wrong=f.services();wrong.actors.audienceLifecycle.user=&phase;
 assert(!resumePackedBattlePhase(Fixture::B,wrong,phase));
 wrong=f.services();wrong.actors.audienceLifecycle.event=[](void*,BattleAudienceLifecycleEvent,uint32_t,uint32_t){};
 assert(!resumePackedBattlePhase(Fixture::B,wrong,phase));
 assert(f.reads.size()==heldReads && f.writes.size()==heldWrites && !f.completed);
 f.word(Fixture::B+0xf18,0x04000006);f.release=true;f.borrowedPhase=&phase;assert(resumePackedBattlePhase(Fixture::B,f.services(),phase));
 assert(f.callsTo(0x8011b2dc)==1 && f.word(Fixture::B+0xf14)==0x0300004c && f.word(Fixture::B+0xf18)==0x04000006);
}
void events(){
 Fixture m(2);const auto mario=m.actor(0,222);m.byte(Fixture::P+0x5ba,2);m.word(Fixture::B+0xf18,0x04000004);BattleMoveCursor mc;
 assert(resumePackedBattleMove(Fixture::B,m.services(),mc));assert(m.move()==3 && m.byte(Fixture::B+0x18ff9)==2 && m.byte(Fixture::P+0x5b8)==1);
 assert(m.word(Fixture::E+0x160)==0 && m.word(mario+0x2a8)==123);
 const std::vector<uint32_t> expected{0x800f7718,0x800f76d4,0x800374ac,0x8011ace8};for(size_t j=0;j<expected.size();++j)assert(m.calls[j].function==expected[j]);
 Fixture late(2);const auto lm=late.actor(0,222);late.byte(Fixture::P+0x5ba,2);late.word(Fixture::B+0xf18,0x04000004);
 late.onCall=[&](const BattleMoveRequest& q){if(q.function==0x800374ac)late.word(lm,77);};
 late.onStore=[&](uint32_t a){if(a==Fixture::E+0x160)late.word(Fixture::E+0x15c,456);};BattleMoveCursor lc;
 assert(resumePackedBattleMove(Fixture::B,late.services(),lc) && late.word(Fixture::E+0x160)==77 && late.word(lm+0x2a8)==456);
 // Engaged0 from evtEntry is not an async hold. This fixture explicitly admits
 // low guest memory solely to check original unguarded event+160/+15c accesses.
 Fixture zero(2);const auto zu=zero.actor(0,222);zero.byte(Fixture::P+0x5ba,2);zero.word(Fixture::B+0xf18,0x04000004);zero.event=0;zero.word(0x15c,17);BattleMoveCursor zc;
 assert(resumePackedBattleMove(Fixture::B,zero.services(),zc) && zero.word(zu+0x2a8)==17);
 Fixture fan(4);for(unsigned j=0;j<3;++j){const auto u=fan.actor(j);fan.word(u+0x294,1);fan.byte(u+0x22,0);fan.byte(u+0x20,10);}BattleMoveCursor fc;
 assert(resumePackedBattleMove(Fixture::B,fan.services(),fc) && fan.callsTo(0x801297bc)==3 && fan.move()==5);
 Fixture serial(6);const auto u=serial.actor(1);serial.word(u+0x298,1);serial.word(Fixture::B+0x220,7);serial.word(Fixture::B+0x228,1);BattleMoveCursor sc;
 assert(resumePackedBattleMove(Fixture::B,serial.services(),sc) && serial.move()==7 && serial.callsTo(0x801297bc)==1 && !serial.callsTo(0x8011ace8));
 assert(serial.word(Fixture::B+0x220)==7 && serial.word(Fixture::B+0x228)==0xffffffff && serial.word(Fixture::B+0x22c)==101);
 serial.wait=1;assert(resumePackedBattleMove(Fixture::B,serial.services(),sc) && serial.move()==8 && serial.callsTo(0x801297bc)==1); // no replay of hole;10 then nextactor8
}
void selection(){
 Fixture f(8);const auto defer=f.actor(0,224,1),blocked=f.actor(1),chosen=f.actor(2,224,0,1);f.statuses[blocked]=1;
 f.word(chosen+0x27c,0xffffffff);f.word(chosen+0x284,0xaabbccdd);f.word(chosen+0x264,88);f.word(chosen+0x280,99);BattleMoveCursor c;
 assert(resumePackedBattleMove(Fixture::B,f.services(),c) && f.word(Fixture::B+0x420)==2 && !f.byte(defer+0x20));
 assert(f.word(chosen+0x27c)==0x80003000 && f.word(chosen+0x284)==0xaabbccdd && f.word(chosen+0x264)==0 && f.word(chosen+0x280)==0);
 assert(f.move()==9 && f.word(Fixture::B+0xf20)==0x06000000 && !f.callsTo(0x8011b2dc));
 const std::vector<std::array<uint32_t,2>> offsets{{0x27c,4},{0x264,4},{0x270,1},{0x260,2},{0x273,1},{0x268,4},{0x271,1},{0x26c,4},{0x272,1},{0x280,4}};
 size_t start=0;while(f.writes[start].address!=defer+0x27c)++start;
 for(size_t j=0;j<offsets.size();++j)assert(f.writes[start+j].address==defer+offsets[j][0] && f.writes[start+j].size==offsets[j][1]);
 Fixture captured(8);const auto first=captured.actor(0,224,1),second=captured.actor(1);captured.onCall=[&](const BattleMoveRequest& q){if(q.function==0x80127814 && q.integers[0]==first)captured.byte(first+0x20,0);};BattleMoveCursor cc;
 assert(resumePackedBattleMove(Fixture::B,captured.services(),cc) && captured.word(Fixture::B+0x420)==1 && second==Fixture::U+0x1000);
 Fixture changed(8);const auto old=changed.actor(0),fresh=Fixture::U+0x3000;changed.word(fresh+8,224);changed.word(fresh+0x27c,0x55);
 changed.onCall=[&](const BattleMoveRequest& q){if(q.function==0x80127814)changed.word(Fixture::B+0x20,fresh);};BattleMoveCursor changedCursor;
 assert(resumePackedBattleMove(Fixture::B,changed.services(),changedCursor) && changed.word(fresh+0x27c)==0x1055 && changed.word(old+0x27c)==0);
 for(int item:{239,240,246,247,253,254}){Fixture flags(8);flags.actor(0,222);flags.half(Fixture::P+0x38a,uint16_t(item));flags.byte(Fixture::P+0x98,255);flags.byte(Fixture::P+0x99,255);BattleMoveCursor cursor;
  assert(resumePackedBattleMove(Fixture::B,flags.services(),cursor));const auto wanted=item>=240 && item<=246?0x400u:item>=247 && item<=253?0x200u:0x600u;assert((flags.word(Fixture::B+0xef4)&0x600)==wanted);}
 Fixture both(8);both.actor(0,222);both.half(Fixture::P+0x38a,240);both.half(Fixture::P+0x38c,247);BattleMoveCursor bc;
 assert(resumePackedBattleMove(Fixture::B,both.services(),bc) && !(both.word(Fixture::B+0xef4)&0x600));
}
void endings(){
 for(uint32_t index:{7u,10u}){Fixture f(index);const auto u=f.actor(0);f.word(u+0x264,77);f.wait=1;f.concluded=1;BattleMoveCursor c;
  assert(resumePackedBattleMove(Fixture::B,f.services(),c) && f.word(Fixture::B+4)==3 && f.word(Fixture::B+0xf24)==0x07000000 && f.word(u+0x264)==77);}
 Fixture forced(10);forced.actor(0);forced.word(Fixture::B+0xef4,0x2000);forced.byte(Fixture::B+0x18ff9,2);BattleMoveCursor fc;
 assert(resumePackedBattleMove(Fixture::B,forced.services(),fc) && forced.move()==11 && forced.byte(Fixture::B+0x18ff9)==0 && forced.callsTo(0x801a7c68)==1);
 Fixture reaction(11);reaction.wait=1;reaction.reaction=0x100;BattleMoveCursor rc;
 assert(resumePackedBattleMove(Fixture::B,reaction.services(),rc) && reaction.move()==12 && !reaction.callsTo(0x801956f8));
 Fixture busy(11);busy.reaction=1;busy.wait=1;BattleMoveCursor bc;assert(resumePackedBattleMove(Fixture::B,busy.services(),bc) && busy.move()==11 && !busy.callsTo(0x8011ace8));
 Fixture announce(12);announce.message=0x100;BattleMoveCursor ac;assert(resumePackedBattleMove(Fixture::B,announce.services(),ac) && announce.move()==12 && announce.callsTo(0x80195af8)==1);
 Fixture fresh(10);fresh.actor(0);fresh.ended=1;fresh.stop=3;BattleMoveCursor c;assert(!resumePackedBattleMove(Fixture::B,fresh.services(),c) && c.pending()==0x801a7c68);
 fresh.word(Fixture::B+0xf18,0x04000004);fresh.release=true;assert(resumePackedBattleMove(Fixture::B,fresh.services(),c));assert(fresh.calls.back().integers[0]==0x04000002); // call argument captured before hold
}
}
int main(){dispatch();heldPrefixes();events();selection();endings();std::cout<<"Whole source Phase/Move15state graph, exact boundaries/scans/writes, typed children and retained prefixes passed\n";}
