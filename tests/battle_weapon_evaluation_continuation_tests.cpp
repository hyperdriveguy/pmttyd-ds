#include "battle_weapon_evaluation_continuation.hpp"
#include "battle_weapon_table.hpp"
#include "battle_weapon_menu_decision.hpp"
#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <tuple>
#include <vector>
using namespace ttyd;
namespace {
struct Fixture {
 static constexpr uint32_t B=0x10000,U=0x40000,M=B+0x171c,W=0x50000,C=0x60000,Private=0x80000,CursorSlot=0x90000;
 using Call=std::pair<uint32_t,std::vector<uint32_t>>;
 using Request=std::tuple<uint32_t,uint32_t,std::array<uint32_t,8>,uint8_t>;
 std::map<uint32_t,uint32_t> words{{B+0x420,1},{M+0x80,W},{W+0xb0,1},{U,1},{C+8,3}};
 std::map<uint32_t,uint16_t> halves;std::map<uint32_t,uint8_t> bytes;
 std::vector<Call> calls;std::vector<std::tuple<uint32_t,uint32_t,unsigned>> writes;
 std::vector<std::pair<uint32_t,unsigned>> reads;
 std::function<void(Fixture&,uint32_t,const std::vector<uint32_t>&)> mutation;
 uint32_t rawEnemy=1,affordable=1,available=10,cost=0,ap=1000;int32_t targets=1;
 bool alive=true,reentry=false;int gate=-1;unsigned waits=0,prefixes=0;
 std::optional<Request> held;BattleWeaponEvaluationCursor cursor;
 BattleActorServices actors(){BattleActorServices a;a.user=this;a.memory.user=this;
 a.memory.word=[](void* u,uint32_t p){auto& f=*static_cast<Fixture*>(u);if(p!=CursorSlot)f.reads.emplace_back(p,4);return f.words[p];};
 a.memory.half=[](void* u,uint32_t p){auto& f=*static_cast<Fixture*>(u);f.reads.emplace_back(p,2);return f.halves[p];};
 a.byte=[](void* u,uint32_t p){auto& f=*static_cast<Fixture*>(u);f.reads.emplace_back(p,1);return f.bytes[p];};
 a.memory.storeWord=[](void* u,uint32_t p,uint32_t n){auto& f=*static_cast<Fixture*>(u);if(p!=CursorSlot)f.writes.emplace_back(p,n,4);f.words[p]=n;};
 a.memory.storeByte=[](void* u,uint32_t p,uint8_t n){auto& f=*static_cast<Fixture*>(u);f.writes.emplace_back(p,n,1);f.bytes[p]=n;};return a;}
 uint32_t service(uint32_t fn,std::vector<uint32_t> args){
  calls.emplace_back(fn,args);uint32_t result=0;
  switch(fn){case 0x800f88cc:result=U+0x1000*(args[1]-1);break;case 0x80127454:result=rawEnemy;break;
   case 0x80128144:result=bytes[args[0]+0xc];break;case 0x801268fc:result=affordable;break;case 0x80123d00:result=uint32_t(targets);break;
   case 0x80126c64:result=available;break;case 0x80126968:result=cost;break;case 0x800d3c44:result=ap;break;default:break;}
  if(mutation)mutation(*this,fn,args);
  return result;
 }
 BattleWeaponEvaluationServices services(){return {actors(),this,[](void* u){return static_cast<Fixture*>(u)->alive;},[](void* u,const BattleWeaponEvaluationRequest& q)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(u);const Request request{q.function,q.callsite,q.arguments,q.count};
  if(int(f.calls.size())==f.gate&&f.waits++<2){if(!f.held){f.held=request;++f.prefixes;}else assert(f.held==request);if(f.reentry){assert(!resumePackedBattleWeaponTargetCount(B,U,W,f.services(),f.cursor));assert(!f.cursor.reset());}return {};}
  f.held.reset();if(q.privateCursorOutput)*q.privateCursorOutput=C;
  return f.service(q.function,{q.arguments.begin(),q.arguments.begin()+q.count});
 },Private};}
 std::optional<int32_t> run(unsigned mode){if(mode==0)return resumePackedBattleWeaponTargetCount(B,U,W,services(),cursor);if(mode==1)return resumePackedBattleWeaponEligibility(B,4,services(),cursor)?std::optional<int32_t>(0):std::nullopt;if(mode==2)return resumePackedBattleWeaponDecision(B,0,0,services(),cursor);return resumePackedBattleWeaponCostCheck(U,W,services(),cursor);}
 std::optional<int32_t> legacy(unsigned mode){
  if(mode==0){BattleWeaponTargetCountServices s;s.memory=actors();s.user=this;s.privateBuffer=Private;
   s.cost=[](void* u,uint32_t unit,uint32_t weapon){return int32_t(static_cast<Fixture*>(u)->service(0x801268fc,{unit,weapon}));};
   s.enemyBelong=[](void* u,uint32_t unit){return int32_t(static_cast<Fixture*>(u)->service(0x80127454,{unit}));};
   s.sample=[](void* u,uint32_t p,uint32_t w,int32_t a,int32_t e,uint32_t f,uint32_t t,int32_t d){static_cast<Fixture*>(u)->service(0x800ff5c0,{p,w,uint32_t(a),uint32_t(e),f,t,uint32_t(d)});return true;};return packedWeaponTargetCount(B,U,W,s);
  }
  if(mode==1){BattleWeaponEligibilityServices s;s.memory=actors();s.user=this;s.cursorSlot=CursorSlot;
   s.unit=[](void* u,uint32_t b,int32_t id){return static_cast<Fixture*>(u)->service(0x800f88cc,{b,uint32_t(id)});};
   s.cursor=[](void* u,uint32_t m,uint32_t t,uint32_t out){auto& f=*static_cast<Fixture*>(u);f.service(0x80124cb8,{m,t});f.words[out]=C;};
   s.targetCount=[](void* u,uint32_t b,uint32_t unit,uint32_t w)->std::optional<int32_t>{return int32_t(static_cast<Fixture*>(u)->service(0x80123d00,{b,unit,w}));};return updatePackedBattleWeaponTable(B,4,s)?std::optional<int32_t>(0):std::nullopt;
  }
  BattleWeaponMenuDecisionServices s;s.memory=actors();s.user=this;
  s.unit=[](void* u,uint32_t b,int32_t id){return static_cast<Fixture*>(u)->service(0x800f88cc,{b,uint32_t(id)});};
  s.build=[](void* u,uint32_t b,uint32_t t){static_cast<Fixture*>(u)->service(0x80123ec0,{b,t});return true;};
  s.enemyBelong=[](void* u,uint32_t unit){return int32_t(static_cast<Fixture*>(u)->service(0x80127454,{unit}));};
  s.belong=[](void* u,uint32_t unit){return int32_t(static_cast<Fixture*>(u)->service(0x80128144,{unit}));};
  s.sample=[](void* u,uint32_t p,uint32_t w,int32_t a,int32_t e,uint32_t f,uint32_t t,int32_t d){static_cast<Fixture*>(u)->service(0x800ff5c0,{p,w,uint32_t(a),uint32_t(e),f,t,uint32_t(d)});return true;};return decidePackedBattleWeapon(B,0,0,s);
 }
};
using Configure=std::function<void(Fixture&)>;
void compare(unsigned mode,const Configure& configure={},bool compareLegacy=true){
 Fixture immediate;if(configure)configure(immediate);const auto result=immediate.run(mode);assert(result);
 if(compareLegacy){Fixture original;if(configure)configure(original);assert(original.legacy(mode)==result);assert(original.calls==immediate.calls&&original.writes==immediate.writes);if(mode!=2)assert(original.reads==immediate.reads);}
 for(unsigned gate=0;gate<immediate.calls.size();++gate){Fixture delayed;if(configure)configure(delayed);delayed.gate=int(gate);delayed.reentry=true;
  assert(!delayed.run(mode));const auto writes=delayed.writes;const auto reads=delayed.reads;const auto calls=delayed.calls;
  auto swapped=delayed.services();swapped.actors.audienceLifecycle.user=&immediate;assert(!resumePackedBattleWeaponTargetCount(Fixture::B,Fixture::U,Fixture::W,swapped,delayed.cursor));
  assert(!delayed.run(mode));assert(writes==delayed.writes&&reads==delayed.reads&&calls==delayed.calls);
  assert(delayed.run(mode)==result);assert(delayed.writes==immediate.writes&&delayed.reads==immediate.reads&&delayed.calls==immediate.calls&&delayed.prefixes==1);
 }
}
void cases(){
 compare(0);compare(0,[](Fixture& f){f.words[Fixture::W+0xb0]=0;});compare(0,[](Fixture& f){f.affordable=0;});
 compare(0,[](Fixture& f){f.rawEnemy=256;f.bytes[Fixture::B+256*8+0xa]=7;f.bytes[Fixture::Private+0xa6c]=255;});
 compare(1);compare(1,[](Fixture& f){f.words[Fixture::M+0x80+28]=Fixture::W+100;f.targets=-1;f.mutation=[](Fixture& f,uint32_t fn,const auto&){if(fn==0x80123d00)f.words[Fixture::C+8]=0;};});
 compare(2);compare(2,[](Fixture& f){f.words[Fixture::W+0x68]=0x30000000;const auto buffer=Fixture::B+0x428;f.bytes[buffer+0xa6c]=3;for(unsigned n=0;n<3;++n){f.bytes[buffer+0xa6d+n]=uint8_t(n);f.halves[buffer+4+36*n]=uint16_t(n+2);f.bytes[Fixture::U+0x1000*(n+1)+0xc]=uint8_t(n==1);}});
 compare(3,{},false);compare(3,[](Fixture& f){f.available=0;f.cost=1;},false);compare(3,[](Fixture& f){f.ap=uint32_t(-1);},false);
 compare(3,[](Fixture& f){f.mutation=[](Fixture& f,uint32_t fn,const auto&){if(fn==0x800d3c44)f.bytes[Fixture::W+0x12]=20;};},false);
}
void capturedPreferenceBranch(){auto configure=[](Fixture& f){const auto buffer=Fixture::B+0x428;f.words[Fixture::W+0x68]=0x20000000;f.bytes[buffer+0xa6c]=2;f.bytes[buffer+0xa6d]=0;f.bytes[buffer+0xa6e]=1;f.halves[buffer+4]=2;f.halves[buffer+40]=3;f.bytes[Fixture::U+0x2000+0xc]=1;
 f.mutation=[](Fixture& f,uint32_t fn,const std::vector<uint32_t>& a){if(fn==0x80128144&&a[0]==Fixture::U+0x2000)f.words[Fixture::W+0x68]=0x10000000;};};
 compare(2,configure,false);Fixture f;configure(f);assert(f.run(2)==0&&f.bytes[Fixture::B+0x428+0xab7]==1);
 // Original c4/c8 ordering: each loop flag WORD precedes permutation BYTE.
 for(size_t i=0;i<f.reads.size();++i)if(f.reads[i].first==Fixture::B+0x428+0xa6d||f.reads[i].first==Fixture::B+0x428+0xa6e){assert(i&&f.reads[i-1].first==Fixture::W+0x68);}
}
void retirement(){Fixture f;f.gate=0;assert(!f.run(0));f.alive=false;const auto writes=f.writes;const auto reads=f.reads;assert(!f.run(0)&&f.cursor.stale()&&f.writes==writes&&f.reads==reads);}
}
int main(){cases();capturedPreferenceBranch();retirement();std::cout<<"Retained original weapon evaluation PASS\n";return 0;}
