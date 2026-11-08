#include "harbor_source_command_ui.hpp"
#include <cassert>
#include <iostream>
#include <map>
#include <tuple>
#include <vector>
using namespace ttyd;
namespace {
struct Fixture {
 static constexpr uint32_t B=0x100000,U=0x200000,M=B+0x171c,W=0x300000,Weapon=0x400000;
 using Call=std::tuple<uint32_t,uint32_t,std::array<uint32_t,8>,uint8_t>;
 std::map<uint32_t,uint8_t> ram;
 std::vector<Call> calls;
 std::vector<std::tuple<uint32_t,uint32_t,unsigned>> writes;
 std::shared_ptr<int> runtime=std::make_shared<int>(1);
 std::unique_ptr<HarborSourceCommandUi> ui;
 bool current=true,reenter=false;int holdIndex=-1;uint32_t hold=0;unsigned waits=0,prefixes=0;
 std::optional<Call> held;
 Fixture(){put(0x8041eb90,B,4);put(B+0x20+4,U,4);put(U+8,222,4);put(U,1,4);put(B+0x420,1,4);put(B+0x1c78,W,4);put(M+0x80,Weapon,4);put(M+0x84,1,4);put(M+0x4b0+8,1,4);put(M+0x498+8,2,4);put(M+8,0,4);put(M+28,1,4);put(M+0xc,1,4);put(M+0x20,1,4);}
 uint32_t get(uint32_t a,unsigned n){uint32_t value=0;for(unsigned i=0;i<n;++i)value=(value<<8)|ram[a+i];return value;}
 void put(uint32_t a,uint32_t v,unsigned n){writes.emplace_back(a,v,n);for(unsigned i=0;i<n;++i)ram[a+n-1-i]=uint8_t(v>>(8*i));}
 BattleActorServices actors(){BattleActorServices a;a.user=this;a.memory.user=this;
 a.byte=[](void* u,uint32_t p){return uint8_t(static_cast<Fixture*>(u)->get(p,1));};
 a.memory.word=[](void* u,uint32_t p){return static_cast<Fixture*>(u)->get(p,4);};
 a.memory.half=[](void* u,uint32_t p){return uint16_t(static_cast<Fixture*>(u)->get(p,2));};
 a.memory.storeWord=[](void* u,uint32_t p,uint32_t v){static_cast<Fixture*>(u)->put(p,v,4);};
 a.memory.storeHalf=[](void* u,uint32_t p,uint16_t v){static_cast<Fixture*>(u)->put(p,v,2);};
 a.memory.storeByte=[](void* u,uint32_t p,uint8_t v){static_cast<Fixture*>(u)->put(p,v,1);};return a;}
 void bind(){HarborSourceCommandUiBinding b;b.actors=actors();b.battle=B;b.runtime=runtime;b.current=[this]{return current;};b.user=this;
 b.word=[](void* u,uint32_t a)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(u);if(a==0x8041e80c&&!f.ram.count(a))return {};return f.get(a,4);};
 b.byteUser=this;b.byte=[](void* u,uint32_t at)->std::optional<uint8_t>{return uint8_t(static_cast<Fixture*>(u)->get(at,1));};
 b.privateSamplingBuffer=0x700000; // Explicit TEST VM scratch, not guest SP.
 b.child=[](void* u,const HarborSourceCommandUiRequest& q)->std::optional<uint32_t>{return static_cast<Fixture*>(u)->service(q.function,q.callsite,{q.arguments[0],q.arguments[1],q.arguments[2],q.arguments[3]},q.count);};
 b.evaluationUser=this;b.evaluationChild=[](void* u,const BattleWeaponEvaluationRequest& q)->std::optional<uint32_t>{assert(q.function==0x800ff5c0||q.function==0x800ff5a0);return static_cast<Fixture*>(u)->service(q.function,q.callsite,q.arguments,q.count);};
 ui=std::make_unique<HarborSourceCommandUi>(std::move(b));writes.clear();}
 std::optional<uint32_t> service(uint32_t fn,uint32_t site,const std::array<uint32_t,8>& args,uint8_t count){
  const Call call{fn,site,args,count};
  if((fn==hold||int(calls.size())==holdIndex)&&waits++<2){if(!held){held=call;++prefixes;}else assert(held==call);if(reenter)assert(!ui->commandInput());return {};}
  held.reset();calls.push_back(call);if(fn==0x8002ff58)return W;return 0u;
 }
 unsigned count(uint32_t fn)const{unsigned result=0;for(const auto& q:calls)if(std::get<0>(q)==fn)++result;return result;}
};
void retainedSourcePrefix(){Fixture f;f.put(Fixture::M,29,4);f.hold=0x80120268;f.reenter=true;f.bind();
 assert(!f.ui->commandInput()&&f.ui->pending()&&f.ui->pendingFunction()==0x80120268);
 const auto before=f.writes;assert(!f.ui->commandInput());assert(f.writes==before);
 assert(f.ui->commandInput()==1&&!f.ui->pending());assert(f.count(0x80120024)==1&&f.count(0x80120268)==1&&f.prefixes==1);
}
void readinessIsSourceOwned(){Fixture f;f.put(Fixture::M,2,4);f.put(Fixture::W+0x14,0,1);f.put(Fixture::B+0xf44,0x100,4);f.bind();
 assert(f.ui->commandInput()==0);assert(f.get(Fixture::M,4)==2&&f.get(Fixture::W+0x14,1)==0);assert(!f.count(0x800d98bc));
}
void physicalClassNavigation(){Fixture f;f.put(Fixture::M,2,4);f.put(Fixture::W+0x14,1,1);f.put(Fixture::B+0xfbc,0x80000,4);f.hold=0x800d98bc;f.reenter=true;f.bind();
 assert(!f.ui->commandInput());const auto selected=f.get(Fixture::M+0x498,4);assert(selected==1);
 const auto before=f.writes;f.put(Fixture::B+0xfbc,0,4);const auto noReplay=f.writes;
 assert(!f.ui->commandInput());assert(f.writes==noReplay);assert(f.ui->commandInput()==0);
 assert(f.get(Fixture::M+0x498,4)==selected&&f.count(0x800d98bc)==1&&f.prefixes==1);assert(before.size()+1==noReplay.size());
}
void physicalWeaponCancel(){Fixture f;f.put(Fixture::M,7,4);f.put(Fixture::W+0x14,1,1);f.put(Fixture::B+0xf44,0x200,4);f.put(0x8041e80c,0x500000,4);f.hold=0x8002fdc4;f.bind();
 assert(!f.ui->commandInput());assert(f.count(0x800d98bc)==1);const auto before=f.writes;
 assert(!f.ui->commandInput());assert(f.writes==before&&f.count(0x800d98bc)==1);
 // The cancel's source result reenters state1 and performs actual Act Setup.
 // Its first missing label is a real child; no legacy fallback is admitted.
 f.current=false;assert(!f.ui->commandInput()&&f.ui->stale());assert(f.writes==before);
}
void targetSetupPrefix(){Fixture f;constexpr uint32_t Part=0x600000,Definition=0x610000;
 f.put(Fixture::M,5,4);f.put(Fixture::B+0xef4,0x200,4);f.put(Fixture::U+0x14,Part,4);f.put(Part+4,Definition,4);f.put(Part+0x1ac,1,4);f.put(Definition,1,4);
 f.hold=0x801276ac;f.bind();assert(!f.ui->commandInput());assert(f.get(Fixture::B+0x1720,4)&0x10000);
 const auto before=f.writes;assert(!f.ui->commandInput());assert(f.writes==before);
 assert(f.ui->commandInput()==0);assert(f.count(0x800ff5c0)==1&&f.count(0x801276ac)==1);
 for(const auto& q:f.calls)if(std::get<0>(q)==0x801276ac){assert(std::get<2>(q)[0]==Part&&std::get<2>(q)[1]==40&&std::get<1>(q)==0x80121024);}
}
void targetEndFreshFlags(){Fixture f;const auto buffer=Fixture::B+0x428;
 f.put(Fixture::M,11,4);f.put(Fixture::B+0x1720,0x01010000,4);f.put(Fixture::B+0xf44,0x200,4);
 f.put(buffer+0xa6c,1,1);f.put(buffer+4,2,2);f.put(buffer+6,1,2);f.hold=0x8012c028;f.bind();
 assert(!f.ui->commandInput());assert(f.get(Fixture::B+0x1720,4)==0x01010000);assert(f.count(0x800d98bc)==1);
 const auto before=f.writes;assert(!f.ui->commandInput());assert(f.writes==before);
 f.put(Fixture::B+0x1720,0x01010002,4);assert(f.ui->commandInput()==0);
 assert((f.get(Fixture::B+0x1720,4)&0x01010000)==0);assert(f.get(Fixture::B+0x1720,4)&2);
 assert(f.count(0x800d98bc)==1&&f.count(0x8012c028)==1&&f.count(0x800ff5a0)==1);
}
void completeTraceAcrossEveryExternalHold(){
 for(unsigned scenario=0;scenario<3;++scenario){
  auto setup=[scenario](Fixture& f){f.put(Fixture::M,scenario==0?29u:scenario==1?2u:7u,4);f.put(Fixture::W+0x14,1,1);f.put(0x8041e80c,0x500000,4);f.put(Fixture::B+0xfbc,scenario==1?0x80000:0,4);f.put(Fixture::B+0xf44,scenario==2?0x200:0,4);};
  Fixture immediate;setup(immediate);immediate.bind();const auto expected=immediate.ui->commandInput();assert(expected);
  for(unsigned gate=0;gate<immediate.calls.size();++gate){Fixture delayed;setup(delayed);delayed.bind();delayed.holdIndex=int(gate);delayed.reenter=true;
   assert(!delayed.ui->commandInput());const auto writes=delayed.writes;const auto calls=delayed.calls;
   assert(!delayed.ui->commandInput());assert(delayed.writes==writes&&delayed.calls==calls);
   assert(delayed.ui->commandInput()==expected);assert(delayed.ram==immediate.ram&&delayed.writes==immediate.writes&&delayed.calls==immediate.calls);assert(delayed.prefixes==1);
  }
 }
}
void runtimeRetirement(){Fixture f;f.put(Fixture::M,29,4);f.hold=0x80120268;f.bind();assert(!f.ui->commandInput());const auto before=f.writes;const auto calls=f.calls;f.runtime.reset();assert(!f.ui->commandInput()&&f.ui->stale());assert(f.writes==before&&f.calls==calls);}
void currentRetirementAndWeakOwnership(){Fixture f;f.put(Fixture::M,29,4);f.bind();assert(f.runtime.use_count()==1);f.current=false;const auto before=f.writes;assert(!f.ui->commandInput()&&f.ui->stale()&&f.writes==before);}
}
int main(){retainedSourcePrefix();readinessIsSourceOwned();physicalClassNavigation();physicalWeaponCancel();targetSetupPrefix();targetEndFreshFlags();completeTraceAcrossEveryExternalHold();runtimeRetirement();currentRetirementAndWeakOwnership();std::cout<<"Harbor retained source command UI PASS\n";return 0;}
