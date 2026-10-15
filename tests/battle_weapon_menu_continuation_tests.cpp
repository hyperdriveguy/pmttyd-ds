#include <memory>
#include "battle_weapon_menu_continuation.hpp"
#include "battle_weapon_menu_main.hpp"
#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <tuple>
#include <vector>
using namespace ttyd;
struct Fixture{
 static constexpr uint32_t B=0x10000,M=B+0x171c,U=0x40000,C=0x50000,W=0x60000,P=0x70000;
 using Call=std::pair<uint32_t,std::vector<uint32_t>>;
 std::map<uint32_t,uint32_t> words{{B+0x420,1},{C,0},{C+8,3},{M+0x80,W},{M+0x84,1},{M+4,0x01000010}};
 std::map<uint32_t,uint8_t> bytes;
 std::vector<std::pair<uint32_t,unsigned>> reads;
 std::vector<std::tuple<uint32_t,uint32_t,unsigned>> writes;
 std::vector<Call> calls;uint32_t trigger=0,repeat=0;bool allowed=true,audience=false,writeCursor=true,probe=false;
 int hold=-1;unsigned attempts=0;const BattleWeaponMenuContinuationRequest* held=nullptr;
 std::optional<uint32_t>* output=nullptr;BattleWeaponMenuContinuationCursor cursor;
 std::function<void(Fixture&,uint32_t,const std::vector<uint32_t>&)> effect;
 uint32_t word(uint32_t a){if(a!=P)reads.emplace_back(a,4);return words[a];}
 uint8_t byte(uint32_t a){reads.emplace_back(a,1);return bytes[a];}
 void store(uint32_t a,uint32_t n){if(a!=P)writes.emplace_back(a,n,4);words[a]=n;}
 void put(uint32_t a,uint8_t n){writes.emplace_back(a,n,1);bytes[a]=n;}
 BattleActorServices actors(){BattleActorServices a;a.user=this;a.memory.user=this;a.memory.word=[](void*p,uint32_t a){return static_cast<Fixture*>(p)->word(a);};a.byte=[](void*p,uint32_t a){return static_cast<Fixture*>(p)->byte(a);};a.memory.storeWord=[](void*p,uint32_t a,uint32_t n){static_cast<Fixture*>(p)->store(a,n);};a.memory.storeByte=[](void*p,uint32_t a,uint8_t n){static_cast<Fixture*>(p)->put(a,n);};return a;}
 uint32_t call(uint32_t fn,const std::vector<uint32_t>&a){calls.emplace_back(fn,a);if(effect)effect(*this,fn,a);
  switch(fn){case 0x800f88cc:return U;case 0x80119df8:return allowed;case 0x8011a06c:return trigger&a[0];case 0x80119f60:return repeat&a[0];case 0x8011f524:return audience;default:return 0;}}
 BattleWeaponMenuContinuationServices native(){BattleWeaponMenuContinuationServices s;s.actors=actors();s.user=this;s.call=[](void*p,const BattleWeaponMenuContinuationRequest&q)->std::optional<uint32_t>{auto&f=*static_cast<Fixture*>(p);
  if(int(f.calls.size())==f.hold&&f.attempts++<2){if(f.held)assert(f.held==&q);else f.held=&q;
   if(f.probe){assert(!f.cursor.reset());assert(!resumePackedBattleWeaponMenuMain(B,0,f.native(),f.cursor));assert(!resumePackedBattleWeaponMenuSetup(B,0,0,f.native(),f.cursor));assert(!resumePackedBattleWeaponMenuEnd(B,f.native(),f.cursor));}return {};}
  f.held=nullptr;if(q.privateCursorOutput){if(!f.output)f.output=q.privateCursorOutput;assert(f.output==q.privateCursorOutput);if(f.writeCursor)*q.privateCursorOutput=C;}
  return f.call(q.function,{q.arguments.begin(),q.arguments.begin()+q.count});};return s;}
 BattleWeaponMenuMainServices legacy(){BattleWeaponMenuMainServices s;s.memory=actors();s.user=this;s.privateCursor=P;
 s.unit=[](void*p,uint32_t b,int32_t i){return static_cast<Fixture*>(p)->call(0x800f88cc,{b,uint32_t(i)});};s.keyAllowed=[](void*p,uint32_t b){return static_cast<Fixture*>(p)->call(0x80119df8,{b})!=0;};
 s.cursor=[](void*p,uint32_t m,uint32_t t,uint32_t out){auto&f=*static_cast<Fixture*>(p);if(f.writeCursor)f.store(out,C);f.call(0x80124cb8,{m,t});};s.update=[](void*p,uint32_t b,uint32_t t){static_cast<Fixture*>(p)->call(0x80123db0,{b,t});return true;};
 s.trigger=[](void*p,uint32_t a){return static_cast<Fixture*>(p)->call(0x8011a06c,{a});};s.repeat=[](void*p,uint32_t a){return static_cast<Fixture*>(p)->call(0x80119f60,{a});};s.sound=[](void*p,uint32_t a){static_cast<Fixture*>(p)->call(0x800d98bc,{a});};
 s.endUI=[](void*p,uint32_t b){static_cast<Fixture*>(p)->call(0x8011600c,{b});return true;};s.decide=[](void*p,uint32_t b,uint32_t t,uint32_t i){static_cast<Fixture*>(p)->call(0x80123210,{b,t,i});return true;};s.targetSetup=[](void*p,uint32_t b){static_cast<Fixture*>(p)->call(0x80120fe4,{b});return true;};
 s.lightFriend=[](void*p){static_cast<Fixture*>(p)->call(0x80138994,{});};s.lightEnemy=[](void*p){static_cast<Fixture*>(p)->call(0x8013898c,{});};s.attackAudience=[](void*p)->std::optional<bool>{return static_cast<Fixture*>(p)->call(0x8011f524,{})!=0;};s.display=[](void*p,uint32_t b){static_cast<Fixture*>(p)->call(0x8011606c,{b});};return s;}
 BattleWeaponMenuDecisionServices setupLegacy(){BattleWeaponMenuDecisionServices s;s.memory=actors();s.user=this;s.privateCursor=P;
 s.build=[](void*p,uint32_t b,uint32_t t){static_cast<Fixture*>(p)->call(0x80123ec0,{b,t});return true;};s.cursor=legacy().cursor;
 s.initialize=[](void*p,uint32_t b,uint32_t c,uint32_t rows,uint32_t f){static_cast<Fixture*>(p)->call(0x80116070,{b,c,rows,f});return true;};return s;}
 void clean(){calls.clear();reads.clear();writes.clear();}
 void omit(){words.erase(P);}
};
using Setup=std::function<void(Fixture&)>;
void parity(unsigned mode,uint32_t type,const Setup&configure={}){
 auto run=[&](Fixture&f)->std::optional<int32_t>{if(mode==0)return resumePackedBattleWeaponMenuMain(Fixture::B,type,f.native(),f.cursor);if(mode==1)return resumePackedBattleWeaponMenuSetup(Fixture::B,type,7,f.native(),f.cursor)?std::optional<int32_t>(0):std::nullopt;return resumePackedBattleWeaponMenuEnd(Fixture::B,f.native(),f.cursor)?std::optional<int32_t>(0):std::nullopt;};
 Fixture expected;if(configure)configure(expected);expected.clean();std::optional<int32_t> result;if(mode==0)result=mainPackedBattleWeaponMenu(Fixture::B,type,expected.legacy());else if(mode==1){assert(setupPackedBattleWeaponMenu(Fixture::B,type,7,expected.setupLegacy()));result=0;}else{assert(endPackedBattleWeaponMenu(Fixture::B,expected.legacy()));result=0;}expected.omit();
 Fixture immediate;if(configure)configure(immediate);immediate.clean();assert(run(immediate)==result&&!immediate.cursor.active());immediate.omit();assert(immediate.words==expected.words&&immediate.bytes==expected.bytes&&immediate.reads==expected.reads&&immediate.writes==expected.writes&&immediate.calls==expected.calls);
 for(unsigned gate=0;gate<immediate.calls.size();++gate){Fixture delayed;if(configure)configure(delayed);delayed.clean();delayed.hold=int(gate);delayed.probe=true;assert(!run(delayed)&&delayed.cursor.active());auto writes=delayed.writes;auto reads=delayed.reads;auto words=delayed.words;auto bytes=delayed.bytes;
  auto bad=delayed.native();bad.actors.audienceLifecycle.user=&expected;assert(!resumePackedBattleWeaponMenuMain(Fixture::B,type,bad,delayed.cursor));assert(!resumePackedBattleWeaponMenuSetup(Fixture::B,type,7,bad,delayed.cursor));assert(!resumePackedBattleWeaponMenuEnd(Fixture::B,bad,delayed.cursor));
  assert(!run(delayed)&&delayed.reads==reads&&delayed.writes==writes&&delayed.words==words&&delayed.bytes==bytes);assert(run(delayed)==result&&!delayed.cursor.active());delayed.omit();assert(delayed.words==immediate.words&&delayed.bytes==immediate.bytes&&delayed.reads==immediate.reads&&delayed.writes==immediate.writes&&delayed.calls==immediate.calls);
 }
}
int main(){
 parity(0,0);parity(0,0,[](auto&f){f.allowed=false;});
 for(uint32_t trigger:{0x100u,0x300u})parity(0,0,[=](auto&f){f.trigger=trigger;f.effect=[](auto&f,uint32_t fn,const auto&){if(fn==0x8011600c)f.store(Fixture::M+4,0x44);};});
 for(uint32_t enabled:{0u,1u})for(uint32_t reason:{0u,7u})parity(0,2,[=](auto&f){f.trigger=0x100;f.words[Fixture::M+0x84]=enabled;f.words[Fixture::M+0x98]=reason;});
 parity(0,0,[](auto&f){f.words[Fixture::M+0x84]=0;f.effect=[](auto&f,uint32_t fn,const auto&a){if(fn==0x8011a06c&&a[0]==0x100)f.put(Fixture::B+0x1c74,1);};});
 for(uint32_t index:{0u,1u,2u,0xffffffffu})for(uint32_t count:{0u,1u,3u,0xffffffffu})for(uint32_t triggers:{0u,0x10000u,0x20000u,0x30000u})parity(0,0,[=](auto&f){f.words[Fixture::C]=index;f.words[Fixture::C+8]=count;f.trigger=triggers;f.repeat=triggers;});
 for(uint32_t type:{0u,2u})for(uint8_t mode:{uint8_t(0),uint8_t(5),uint8_t(255)})for(uint8_t badge:{uint8_t(0),uint8_t(1),uint8_t(2)})parity(0,type,[=](auto&f){f.trigger=0x200;f.bytes[Fixture::U+0x20]=mode;f.bytes[Fixture::U+0x2f3]=badge==1;f.bytes[Fixture::U+0x2f4]=badge==2;});
 parity(0,0,[](auto&f){f.audience=true;});parity(0,0,[](auto&f){f.bytes[Fixture::B+0x1c74]=1;f.words[Fixture::B+0x1c70]=99;});
 parity(0,2,[](auto&f){f.trigger=0x100;f.words[Fixture::M+0x84]=0;f.words[Fixture::M+0x98]=1;f.effect=[](auto&f,uint32_t fn,const auto&){if(fn==0x8011600c)f.store(Fixture::M+0x98,19);};});
 parity(0,2,[](auto&f){f.trigger=0x100;f.effect=[](auto&f,uint32_t fn,const auto&){if(fn==0x80138994){f.store(Fixture::M+0x80,Fixture::W+0x100);f.store(Fixture::W+0x164,0x4110);}if(fn==0x8011600c)f.store(Fixture::M+0x540,99);};});
 parity(0,0,[](auto&f){f.repeat=0x20000;f.effect=[](auto&f,uint32_t fn,const auto&){if(fn==0x80119f60)f.store(Fixture::C,5);};});
 for(unsigned mode:{1u,2u})parity(mode,2,[](auto&f){f.effect=[](auto&f,uint32_t fn,const auto&){if(fn==0x80116070||fn==0x8011600c)f.store(Fixture::M+4,0x12000040);};});
 Fixture unknown;unknown.writeCursor=false;assert(!resumePackedBattleWeaponMenuMain(Fixture::B,99,unknown.native(),unknown.cursor));auto reads=unknown.reads;auto writes=unknown.writes;auto calls=unknown.calls;assert(!resumePackedBattleWeaponMenuMain(Fixture::B,99,unknown.native(),unknown.cursor)&&unknown.reads==reads&&unknown.writes==writes&&unknown.calls==calls);assert(unknown.cursor.reset());
 Fixture setupUnknown;setupUnknown.writeCursor=false;assert(!resumePackedBattleWeaponMenuSetup(Fixture::B,99,7,setupUnknown.native(),setupUnknown.cursor));auto setupCalls=setupUnknown.calls;auto setupReads=setupUnknown.reads;assert(!resumePackedBattleWeaponMenuSetup(Fixture::B,99,7,setupUnknown.native(),setupUnknown.cursor)&&setupUnknown.calls==setupCalls&&setupUnknown.reads==setupReads);
 Fixture setupHeld;setupHeld.hold=0;assert(!resumePackedBattleWeaponMenuSetup(Fixture::B,2,7,setupHeld.native(),setupHeld.cursor));auto heldCalls=setupHeld.calls;auto heldReads=setupHeld.reads;assert(!resumePackedBattleWeaponMenuSetup(Fixture::B,3,7,setupHeld.native(),setupHeld.cursor));assert(!resumePackedBattleWeaponMenuSetup(Fixture::B,2,8,setupHeld.native(),setupHeld.cursor));assert(!resumePackedBattleWeaponMenuSetup(Fixture::B+4,2,7,setupHeld.native(),setupHeld.cursor));assert(setupHeld.calls==heldCalls&&setupHeld.reads==heldReads);
 Fixture closed;closed.writeCursor=false;closed.allowed=false;assert(resumePackedBattleWeaponMenuMain(Fixture::B,99,closed.native(),closed.cursor)==0);
 std::cout<<"Whole weapon Main/Setup/End source parity, physical branches, nested End and failure-each-child retention passed\n";
}
