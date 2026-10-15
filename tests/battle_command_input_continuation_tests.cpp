#include <memory>
#include "battle_command_input_continuation.hpp"
#include "battle_command_input.hpp"
#include "battle_target_selection.hpp"
#include <cassert>
#include <functional>
#include <iostream>
#include <tuple>
#include <vector>
using namespace ttyd;
struct Fixture {
 static constexpr uint32_t B=0x10000,U=0x40000,M=B+0x171c,C=0x80000,P=0x90000,W=0xa0000,E=0xb0000,F=B+0x428;
 std::vector<uint8_t> ram=std::vector<uint8_t>(0x100000);
 using Call=std::tuple<uint32_t,std::vector<uint32_t>>;
 std::vector<Call> accepted;
 std::vector<std::tuple<uint32_t,uint32_t,uint32_t>> writes;
 std::vector<std::pair<uint32_t,unsigned>> reads;
 std::function<uint32_t(Fixture&,uint32_t,const std::vector<uint32_t>&,uint32_t)> effect;
 int hold=-1,attempts=0;const BattleCommandInputContinuationRequest* held=nullptr;
 std::optional<uint32_t>* output=nullptr;bool probe=false,writeCursor=true;unsigned prefixes=0;
 BattleCommandInputContinuationCursor cursor;
 Fixture(){put(B+0x420,1,4);put(C,0,4);put(P,C,4);put(M+0x80,W,4);put(W+0x70,0x123456,4);put(W+0x64,0x01000000,4);put(E+0x15c,99,4);put(F+0xa6c,2,1);put(F+0xa6d,0,1);put(F+0xa6e,1,1);put(F+4,4,2);put(F+6,1,2);put(F+40,5,2);put(F+42,2,2);}
 uint32_t get(uint32_t a,unsigned n){assert(uint64_t(a)+n<=ram.size());if(a!=P)reads.emplace_back(a,n);uint32_t v=0;for(unsigned i=0;i<n;++i)v=(v<<8)|ram[a+i];return v;}
 void put(uint32_t a,uint32_t v,unsigned n){assert(uint64_t(a)+n<=ram.size());if(a!=P)writes.emplace_back(a,v,n);for(unsigned i=0;i<n;++i)ram[a+n-1-i]=uint8_t(v>>(8*i));}
 BattleActorServices actors(){BattleActorServices a;a.user=this;a.memory.user=this;
 a.memory.word=[](void*p,uint32_t v){return static_cast<Fixture*>(p)->get(v,4);};
 a.memory.half=[](void*p,uint32_t v){return uint16_t(static_cast<Fixture*>(p)->get(v,2));};
 a.byte=[](void*p,uint32_t v){return uint8_t(static_cast<Fixture*>(p)->get(v,1));};
 a.memory.storeWord=[](void*p,uint32_t v,uint32_t n){static_cast<Fixture*>(p)->put(v,n,4);};
 a.memory.storeByte=[](void*p,uint32_t v,uint8_t n){static_cast<Fixture*>(p)->put(v,n,1);};return a;}
 uint32_t service(uint32_t fn,const std::vector<uint32_t>&args){
  accepted.emplace_back(fn,args);uint32_t result=0;
  if(fn==0x800f88cc)result=U;
  if(fn==0x800374ac)result=E;
  if(fn==0x80036604)result=1;
  return effect?effect(*this,fn,args,result):result;
 }
 BattleCommandInputContinuationServices services(){BattleCommandInputContinuationServices s;s.actors=actors();s.user=this;
 s.call=[](void*p,const BattleCommandInputContinuationRequest&q)->std::optional<uint32_t>{auto&f=*static_cast<Fixture*>(p);
  assert((q.function==0x80124cb8)==bool(q.privateCursorOutput));
  if(q.privateCursorOutput){if(!f.output)f.output=q.privateCursorOutput;assert(f.output==q.privateCursorOutput);}
  if(int(f.accepted.size())==f.hold&&f.attempts++<2){
   if(!f.held){f.held=&q;++f.prefixes;}else assert(f.held==&q);
   if(f.probe){auto bad=f.services();bad.actors.audienceLifecycle.user=&f.cursor;assert(!resumePackedBattleCommandInput(B,bad,f.cursor));assert(!f.cursor.reset());assert(!resumePackedBattleCommandInput(B,f.services(),f.cursor));}
   return {};
  }
  f.held=nullptr;
  std::vector<uint32_t> args(q.arguments.begin(),q.arguments.begin()+q.count);
  if(f.writeCursor&&q.privateCursorOutput&&q.arguments[1]!=99)*q.privateCursorOutput=C;
  return f.service(q.function,args);
 };return s;}
 std::optional<int32_t> run(){return resumePackedBattleCommandInput(B,services(),cursor);}
 std::optional<int32_t> legacy(){BattleCommandInputServices s;s.user=this;s.memory=actors();s.privateCursor=P;
 s.request=[](void*p,const BattleCommandInputRequest&q)->std::optional<uint32_t>{auto&f=*static_cast<Fixture*>(p);std::vector<uint32_t>args(q.arguments.begin(),q.arguments.begin()+q.count);auto fn=uint32_t(q.function);if(fn==0x80124cb8){if(args[1]!=99)f.put(P,C,4);args.resize(2);}return f.service(fn,args);};
 s.target=[](void*p,uint32_t b,uint32_t u,uint32_t out){auto&f=*static_cast<Fixture*>(p);BattleTargetSelectionServices t;t.memory=f.actors();t.user=p;t.privateCursor=out;
 t.cursor=[](void*p,uint32_t m,uint32_t type,uint32_t out){auto&f=*static_cast<Fixture*>(p);if(type!=99)f.put(out,C,4);f.service(0x80124cb8,{m,type});};
 t.refresh=[](void*p,uint32_t b){static_cast<Fixture*>(p)->service(0x800ff5a0,{b});return true;};
 t.repeat=[](void*p,uint32_t k){return static_cast<Fixture*>(p)->service(0x80119f60,{k});};
 t.trigger=[](void*p,uint32_t k){return static_cast<Fixture*>(p)->service(0x8011a06c,{k});};
 t.sound=[](void*p,uint32_t k){static_cast<Fixture*>(p)->service(0x800d98bc,{k});};
 t.endTarget=[](void*p,uint32_t b){static_cast<Fixture*>(p)->service(0x801209e8,{b});return true;};
 t.help=[](void*p,int32_t n){static_cast<Fixture*>(p)->service(0x8021690c,{uint32_t(n)});};
 t.commandPose=[](void*p,uint32_t u){static_cast<Fixture*>(p)->service(0x801264cc,{u});return true;};
 t.offEnemyLight=[](void*p){static_cast<Fixture*>(p)->service(0x80138988,{});};
 t.offLight=[](void*p,int32_t a,int32_t b){static_cast<Fixture*>(p)->service(0x80138998,{uint32_t(a),uint32_t(b)});};
 t.pay=[](void*p,uint32_t u,uint32_t w){static_cast<Fixture*>(p)->service(0x80126840,{u,w});return true;};
 t.audience=[](void*p)->std::optional<bool>{return static_cast<Fixture*>(p)->service(0x8011f524,{})!=0;};return processPackedBattleTargetSelection(b,u,t);};
 return packedBattleCommandInput(B,s);}
 void clearTrace(){accepted.clear();writes.clear();reads.clear();}
};
using Configure=std::function<void(Fixture&)>;
void compare(uint32_t state,const Configure& configure={}){
 Fixture expected;expected.put(Fixture::M,state,4);if(configure)configure(expected);expected.clearTrace();auto result=expected.legacy();assert(result);
 Fixture immediate;immediate.put(Fixture::M,state,4);if(configure)configure(immediate);immediate.clearTrace();auto actual=immediate.run();
 assert(actual==result&&!immediate.cursor.active());assert(immediate.ram==expected.ram&&immediate.accepted==expected.accepted&&immediate.writes==expected.writes&&immediate.reads==expected.reads);
 for(unsigned gate=0;gate<immediate.accepted.size();++gate){
  Fixture delayed;delayed.put(Fixture::M,state,4);if(configure)configure(delayed);delayed.clearTrace();delayed.hold=int(gate);delayed.probe=true;
  assert(!delayed.run()&&delayed.cursor.active()&&delayed.cursor.pending());assert(delayed.accepted.size()==gate);
  auto ram=delayed.ram;auto writes=delayed.writes;auto reads=delayed.reads;
  auto lifecycle=delayed.services();lifecycle.actors.audienceLifecycle.user=&immediate;assert(!resumePackedBattleCommandInput(Fixture::B,lifecycle,delayed.cursor));
  lifecycle=delayed.services();lifecycle.actors.audienceLifecycle.event=[](void*,BattleAudienceLifecycleEvent,uint32_t,uint32_t){};assert(!resumePackedBattleCommandInput(Fixture::B,lifecycle,delayed.cursor));
  auto wrong=delayed.services();wrong.user=&immediate;assert(!resumePackedBattleCommandInput(Fixture::B,wrong,delayed.cursor));assert(!resumePackedBattleCommandInput(Fixture::B+4,delayed.services(),delayed.cursor));
  assert(!delayed.run());assert(delayed.ram==ram&&delayed.writes==writes&&delayed.reads==reads);
  assert(delayed.run()==result&&!delayed.cursor.active()&&delayed.prefixes==1);
  assert(delayed.ram==immediate.ram&&delayed.accepted==immediate.accepted&&delayed.writes==immediate.writes&&delayed.reads==immediate.reads);
 }
}
int main(){
 for(uint32_t s=0;s<32;++s){compare(s);}
 compare(0xffffffff);
 compare(0,[](auto&f){f.put(Fixture::B+0x1c74,2,1);f.put(Fixture::M+0x540,11,4);f.put(Fixture::M+0x544,2,4);});
 for(uint32_t mode=5;mode<=9;++mode)compare(0,[=](auto&f){f.put(Fixture::U+0x20,mode,1);});
 for(uint32_t state:{5u,6u})for(uint32_t type:{0u,1u,2u})for(uint32_t bits:{0u,1u,2u})compare(state,[=](auto&f){f.put(Fixture::M+0x540,type,4);f.put(Fixture::U+0x2f3,bits==1,1);f.put(Fixture::U+0x2f4,bits==2,1);});
 for(uint32_t type:{0u,1u})compare(5,[=](auto&f){f.put(Fixture::M+0x540,type,4);f.put(Fixture::B+0xef4,type?0x400:0x200,4);});
 for(uint32_t state:{10u,11u})compare(state,[](auto&f){f.put(Fixture::M+0x540,99,4);});
 // Original source returns0 even after confirming and storing29: no fabricated immediate progress.
 for(uint32_t operation:{0u,2u,3u})compare(11,[=](auto&f){f.put(Fixture::M+0x540,2,4);f.put(Fixture::M+0x550,operation,1);f.effect=[](auto&,uint32_t fn,const auto&a,uint32_t v)->uint32_t{if(fn==0x80119f60)return 1u;if(fn==0x8011a06c)return uint32_t(a[0]==0x100);return v;};});
 compare(11,[](auto&f){f.put(Fixture::B+0xef4,4,4);f.effect=[](auto&f,uint32_t fn,const auto&a,uint32_t v)->uint32_t{if(fn==0x8011a06c)return 1u;if(fn==0x801264cc){f.put(Fixture::M+0x540,0,4);f.put(Fixture::B+0xef4,0x200,4);}if(fn==0x800ff5a0){f.put(Fixture::F+0xa6d,1,1);f.put(Fixture::F+0xa6e,0,1);}if(fn==0x80119f60)return uint32_t(a[0]==0x40000);return v;};});
 for(uint32_t state:{27u,28u})for(uint32_t mask:{0u,0x100u,0x200u,0x300u})compare(state,[=](auto&f){f.effect=[=](auto&,uint32_t fn,const auto&a,uint32_t v){return fn==0x8011a06c?uint32_t((a[0]&mask)!=0):v;};});
 compare(2,[](auto&f){f.effect=[](auto&f,uint32_t fn,const auto&,uint32_t v)->uint32_t{if(fn==0x8012230c){f.put(Fixture::M,29,4);return 1u;}return v;};});
 compare(11,[](auto&f){f.effect=[](auto&f,uint32_t fn,const auto&,uint32_t v)->uint32_t{if(fn==0x8011f524)return 1u;if(fn==0x8011f5f0&&f.get(Fixture::B+0x1c74,1)){f.put(Fixture::B+0x1c74,0,1);f.put(Fixture::M,29,4);}return v;};});
 // Request operands are captured before a hold; later parent reads stay fresh.
 Fixture operand;operand.put(Fixture::M,30,4);operand.clearTrace();operand.hold=0;
 assert(!operand.run());operand.put(Fixture::B+0x420,9,4);assert(!operand.run());assert(operand.run()==0);
 assert(std::get<1>(operand.accepted.front())[1]==1);
 Fixture fresh;fresh.put(Fixture::M,5,4);fresh.put(Fixture::M+0x540,2,4);fresh.clearTrace();fresh.hold=4;
 assert(!fresh.run()&&fresh.cursor.pending()==0x80122068);fresh.put(Fixture::M+0x540,4,4);
 assert(!fresh.run());assert(fresh.run()==0);
 bool setup=false,main=false;for(const auto& entry:fresh.accepted){if(std::get<0>(entry)==0x80122068){setup=true;assert(std::get<1>(entry)[1]==2);}if(std::get<0>(entry)==0x80121d0c){main=true;assert(std::get<1>(entry)[1]==4);}}assert(setup&&main);
 for(uint32_t state:{10u,11u}){
  Fixture unknown;unknown.writeCursor=false;unknown.put(Fixture::M,state,4);unknown.clearTrace();
  assert(!unknown.run()&&unknown.cursor.pending()==0x80124cb8);
  auto writes=unknown.writes;auto reads=unknown.reads;auto calls=unknown.accepted;
  assert(!unknown.run()&&unknown.writes==writes&&unknown.reads==reads&&unknown.accepted==calls);assert(unknown.cursor.reset());
 }
 Fixture noUse;noUse.writeCursor=false;noUse.put(Fixture::M,30,4);assert(noUse.run()==0&&!noUse.cursor.active());
 Fixture wrapped;auto wrapServices=wrapped.services();wrapServices.actors.memory.word=[](void*,uint32_t a)->uint32_t{assert(a==0u-0x171c+0x420);return 1u;};assert(resumePackedBattleCommandInput(0u-0x171c,wrapServices,wrapped.cursor)==1&&wrapped.accepted.size()==1);
 compare(11,[](auto&f){f.put(Fixture::W+0x64,0,4);f.put(Fixture::F+0xa6c,255,1);});
 compare(11,[](auto&f){f.put(Fixture::M+0x540,2,4);f.put(Fixture::M+0x550,2,1);f.effect=[](auto&f,uint32_t fn,const auto&a,uint32_t v)->uint32_t{if(fn==0x8011a06c)return uint32_t(a[0]==0x100);if(fn==0x801209e8)f.put(Fixture::M+0x540,99,4);if(fn==0x80126840){assert(a[1]==0x803553a8);f.put(Fixture::U+0x20,7,1);}return v;};});
 std::cout<<"Whole30state Input and inline target source parity, failure-each-child prefix retention and physical control ordering passed\n";
}
