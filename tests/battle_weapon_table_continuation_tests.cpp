#include "battle_weapon_table_continuation.hpp"
#include "battle_weapon_table.hpp"
#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <tuple>
#include <vector>
using namespace ttyd;
namespace {
struct Fixture {
 static constexpr uint32_t B=0x10000,M=B+0x171c,U=0x40000,P=0x50000,C=0x60000,CS=0x70000,N=CS+4;
 using Call=std::pair<uint32_t,std::vector<uint32_t>>;
 using Access=std::tuple<uint32_t,uint32_t,unsigned>;
 std::map<uint32_t,uint32_t> words{{B+0x420,1},{C,99},{C+4,5},{C+8,77},{U+0x18,0}};
 std::map<uint32_t,uint16_t> halves;std::map<uint32_t,uint8_t> bytes;
 std::vector<Call> calls;std::vector<Access> writes;std::vector<std::pair<uint32_t,unsigned>> reads;
 std::function<void(Fixture&,uint32_t)> mutation;
 BattleWeaponTableContinuationCursor cursor;
 bool alive=true,recursive=false,writeCursor=true,holdUnitData=false;unsigned dataWaits=0;int gate=-1;unsigned attempts=0,prefixes=0;
 std::optional<std::tuple<uint32_t,uint32_t,std::array<uint32_t,3>,uint8_t>> held;
 void read(uint32_t a,unsigned width){if(a!=CS&&a!=N)reads.emplace_back(a,width);}
 void write(uint32_t a,uint32_t n,unsigned width){if(a!=CS&&a!=N)writes.emplace_back(a,n,width);}
 BattleActorServices actors(){BattleActorServices a;a.user=this;a.memory.user=this;
 a.memory.word=[](void* u,uint32_t at){auto& f=*static_cast<Fixture*>(u);f.read(at,4);return f.words[at];};
 a.memory.half=[](void* u,uint32_t at){auto& f=*static_cast<Fixture*>(u);f.read(at,2);return f.halves[at];};
 a.byte=[](void* u,uint32_t at){auto& f=*static_cast<Fixture*>(u);f.read(at,1);return f.bytes[at];};
 a.memory.storeWord=[](void* u,uint32_t at,uint32_t v){auto& f=*static_cast<Fixture*>(u);f.write(at,v,4);f.words[at]=v;};
 a.memory.storeHalf=[](void* u,uint32_t at,uint16_t v){auto& f=*static_cast<Fixture*>(u);f.write(at,v,2);f.halves[at]=v;};return a;}
 uint32_t service(uint32_t fn,std::vector<uint32_t> a){
  calls.emplace_back(fn,a);uint32_t result=0;
  switch(fn){case 0x800f88cc:result=U;break;case 0x800d5eb0:result=P;break;
   case 0x800d3300:result=a[0]==300?2:0;break;case 0x80124c64:result=167;break;case 0x80124c10:result=164;break;
   case 0x80081b8c:result=a[0]+1;break;default:break;}
  if(mutation)mutation(*this,fn);
  return result;
 }
 BattleWeaponTableServices legacyServices(){BattleWeaponTableServices s;s.memory=actors();s.user=this;s.cursorSlot=CS;s.countSlot=N;
 s.unit=[](void* u,uint32_t b,int32_t id){return static_cast<Fixture*>(u)->service(0x800f88cc,{b,uint32_t(id)});};
 s.pouch=[](void* u){return static_cast<Fixture*>(u)->service(0x800d5eb0,{});};
 s.cursor=[](void* u,uint32_t m,uint32_t t,uint32_t out){auto& f=*static_cast<Fixture*>(u);f.service(0x80124cb8,{m,t});if(f.writeCursor)f.words[out]=C;};
 s.equipped=[](void* u,int32_t id){return int32_t(static_cast<Fixture*>(u)->service(0x800d3300,{uint32_t(id)}));};
 s.jumpIcon=[](void* u){return uint16_t(static_cast<Fixture*>(u)->service(0x80124c64,{}));};s.hammerIcon=[](void* u){return uint16_t(static_cast<Fixture*>(u)->service(0x80124c10,{}));};
 s.message=[](void* u,uint32_t key){return static_cast<Fixture*>(u)->service(0x80081b8c,{key});};
 s.party=[](void* u,uint32_t fn,uint32_t m,uint32_t out){auto& f=*static_cast<Fixture*>(u);f.service(fn,{m});f.words[out]=3;return true;};
 s.update=[](void* u,uint32_t b,uint32_t type){static_cast<Fixture*>(u)->service(0x80123db0,{b,type});return true;};return s;}
 BattleWeaponTableContinuationServices services(){return {actors(),this,[](void* u){return static_cast<Fixture*>(u)->alive;},[](void* u,const BattleWeaponTableContinuationRequest& q)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(u);
  if(q.function==0x8012733c){if(f.holdUnitData&&f.dataWaits++<2)return {};return packedUnitData(q.arguments[0],int32_t(q.arguments[1]),f.actors());}
  const auto key=std::make_tuple(q.function,q.callsite,q.arguments,q.count);
  if(int(f.calls.size())==f.gate&&f.attempts++<2){if(!f.held){f.held=key;++f.prefixes;}else assert(f.held==key);if(f.recursive){assert(!resumePackedBattleWeaponTable(B,0,f.services(),f.cursor));assert(!f.cursor.reset());}return {};}
  f.held.reset();if(q.privateCursorOutput&&f.writeCursor)*q.privateCursorOutput=C;
  const auto result=f.service(q.function,std::vector<uint32_t>(q.arguments.begin(),q.arguments.begin()+q.count));if(q.privateCountOutput)*q.privateCountOutput=3;
  return result;
 }};}
 auto run(uint32_t type){return resumePackedBattleWeaponTable(B,type,services(),cursor);}
 void clear(){calls.clear();writes.clear();reads.clear();}
};
using Configure=std::function<void(Fixture&)>;
void compare(uint32_t type,const Configure& configure={}){
 Fixture expected;if(configure)configure(expected);expected.clear();const auto result=makePackedBattleWeaponTable(Fixture::B,type,expected.legacyServices());assert(result);
 Fixture actual;if(configure)configure(actual);actual.clear();assert(actual.run(type)==result);assert(actual.calls==expected.calls);assert(actual.writes==expected.writes);assert(actual.reads==expected.reads);
 for(unsigned gate=0;gate<actual.calls.size();++gate){Fixture held;if(configure)configure(held);held.clear();held.gate=int(gate);held.recursive=true;
  assert(!held.run(type));const auto writes=held.writes;const auto reads=held.reads;const auto calls=held.calls;
  auto bad=held.services();bad.actors.audienceLifecycle.user=&actual;assert(!resumePackedBattleWeaponTable(Fixture::B,type,bad,held.cursor));assert(!resumePackedBattleWeaponTable(Fixture::B+4,type,held.services(),held.cursor));
  assert(!held.run(type));assert(held.writes==writes&&held.reads==reads&&held.calls==calls);
  assert(held.run(type)==result);assert(held.calls==actual.calls&&held.writes==actual.writes&&held.reads==actual.reads);assert(held.prefixes==1&&!held.cursor.active());
 }
}
void cases(){
 for(uint32_t type:{0u,1u}){compare(type,[type](Fixture& f){f.bytes[Fixture::P+(type==0?0x98:0x99)]=3;});compare(type,[type](Fixture& f){f.bytes[Fixture::P+(type==0?0x98:0x99)]=255;});
  compare(type,[type](Fixture& f){f.bytes[Fixture::P+(type==0?0x98:0x99)]=3;f.mutation=[type](Fixture& f,uint32_t fn){if(fn==(type==0?0x80124c64u:0x80124c10u))f.bytes[Fixture::P+(type==0?0x98:0x99)]=1;};});
  compare(type,[type](Fixture& f){f.words[type==0?0x80416d80:0x80416da0]=300;const auto item=0x803108a8+40*300;f.words[item+0x24]=0x80000;f.words[0x80008]=1;f.halves[item+0x20]=321;});
 }
 compare(0,[](Fixture& f){f.bytes[Fixture::P+0x98]=1;f.words[Fixture::B+0xef4]=0x09000000;f.words[Fixture::B+0x424]=1;f.words[Fixture::U+0x18]=0x80000;f.words[0x80000]=68;f.words[0x80004]=0x90000;f.words[0x803538a8]=50;});
 compare(2);compare(2,[](Fixture& f){f.halves[Fixture::P+0x192]=100;const auto item=0x803108a8+4000;f.halves[item+0x10]=2;f.words[item+4]=77;f.halves[item+0x20]=250;});
 compare(2,[](Fixture& f){f.halves[Fixture::P+0x192]=uint16_t(-1);const auto item=0x803108a8-40;f.halves[item+0x10]=2;f.words[item+0x24]=0x90000;});
 compare(4,[](Fixture& f){f.halves[Fixture::P+0x8c]=5;f.words[0x80355ca8]=0x80000;f.words[0x80355cb0]=0x80020;f.mutation=[](Fixture& f,uint32_t fn){if(fn==0x80081b8c)f.halves[Fixture::P+0x8c]=0;};});
 compare(6,[](Fixture& f){f.words[Fixture::U+0x18]=0x80000;f.words[0x80000]=1;f.words[0x80004]=44;});compare(15);
}
void retainedUnitDataAndUnknownCursor(){
 Fixture f;f.bytes[Fixture::P+0x98]=1;f.words[Fixture::B+0xef4]=0x09000000;f.words[Fixture::B+0x424]=1;f.words[Fixture::U+0x18]=0x80000;f.words[0x80000]=68;f.words[0x80004]=0x90000;f.holdUnitData=true;
 assert(!f.run(0));const auto writes=f.writes;const auto reads=f.reads;const auto calls=f.calls;
 assert(!f.run(0));assert(f.writes==writes&&f.reads==reads&&f.calls==calls);
 f.words[Fixture::B+0x424]=9;assert(f.run(0)==1&&f.words[Fixture::M+0x80]==0x90000);
 Fixture unknown;unknown.bytes[Fixture::P+0x98]=1;unknown.writeCursor=false;assert(!unknown.run(0));assert(unknown.cursor.pending()==0x80124cb8);
 const auto completeRows=unknown.writes;const auto completedCalls=unknown.calls;assert(!unknown.run(0));assert(unknown.writes==completeRows&&unknown.calls==completedCalls);
}
void retirement(){Fixture f;f.bytes[Fixture::P+0x98]=1;f.gate=0;assert(!f.run(0));const auto before=f.writes;f.alive=false;assert(!f.run(0)&&f.cursor.stale());assert(f.writes==before);f.alive=true;assert(!f.run(0));}
}
int main(){cases();retainedUnitDataAndUnknownCursor();retirement();std::cout<<"Retained original whole weapon table PASS\n";return 0;}
