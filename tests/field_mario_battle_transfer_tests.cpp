#include "field_mario_battle_transfer.hpp"
#include <cassert>
#include <cstdio>
#include <map>
#include <tuple>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t M=0x83001000,P=0x83002000,Q=0x83003000,Saved=0x8041e8f4;
using Trace=std::vector<std::tuple<char,uint32_t,uint32_t>>;
struct World {
 std::map<uint32_t,uint32_t> words;Trace trace;
 std::shared_ptr<const void> life=std::make_shared<int>(1);
 unsigned accepted=0,holdAt=~0u,holds=0;bool alive=true,mutation=false,recurse=false;
 unsigned childCount=0;FieldMarioBattleTransfer* cursor=nullptr;
 std::array<std::optional<uint32_t>,3>* vector=nullptr;
 bool admit(){if(accepted==holdAt&&holds<2){++holds;return false;}++accepted;return true;}
 World(bool party=true){words={{0x8041e900,M},{M,0x12345002},{M+0x8c,11},{M+0x90,22},{M+0x94,33},{M+0x2e,20},{Saved,101},{Saved+4,102},{Saved+8,103},{0x8041ead8,P},{0x8041eadc,Q},{P,0x20000041},{Q,0x20000081},{P+0x34,3},{P+0x31,255},{0x803143e8-40+0x10,0x80001234}};if(!party){words[0x8041ead8]=words[0x8041eadc]=0;}}
 FieldMarioBattleTransferServices services(){FieldMarioBattleTransferServices s;s.lifetime=life;s.user=this;
  s.current=[](void*p){return static_cast<World*>(p)->alive;};
  s.word=[](void*p,uint32_t a)->std::optional<uint32_t>{auto&w=*static_cast<World*>(p);if(!w.admit())return {};auto i=w.words.find(a);if(i==w.words.end())return {};w.trace.emplace_back('w',a,i->second);return i->second;};
  s.half=[](void*p,uint32_t a)->std::optional<uint16_t>{auto&w=*static_cast<World*>(p);if(!w.admit())return {};auto i=w.words.find(a);if(i==w.words.end())return {};w.trace.emplace_back('h',a,i->second);return uint16_t(i->second);};
  s.byte=[](void*p,uint32_t a)->std::optional<uint8_t>{auto&w=*static_cast<World*>(p);if(!w.admit())return {};auto i=w.words.find(a);if(i==w.words.end())return {};w.trace.emplace_back('b',a,i->second);return uint8_t(i->second);};
  s.store=[](void*p,uint32_t a,uint32_t v){auto&w=*static_cast<World*>(p);if(!w.admit())return false;w.trace.emplace_back('s',a,v);w.words[a]=v;
   if(w.mutation&&a==Saved+4)w.words[M+0x94]=333;
   if(w.mutation&&a==M+0x90){w.words[Saved+8]=444;w.words[M+0x2e]=26;}
   if(w.mutation&&a==P+0x5c&&w.vector)(*w.vector)[2]=777;
   return true;};
  s.call=[](void*p,FieldMarioBattleTransferRequest&q){auto&w=*static_cast<World*>(p);if(w.recurse){assert(w.cursor);assert(!w.cursor->resume(w.services()));}if(!w.admit())return false;
   ++w.childCount;w.trace.emplace_back('c',q.function,q.arguments[0]);
   switch(q.function){
   case 0x800965fc:if(w.mutation){w.words[0x8041e900]=0x99990000;w.words[M+0x2e]=0;}break;
   case 0x80141050:q.result=w.words[0x8041ead8]?0:0xffffffff;break;
   case 0x800bf1b4:q.result=q.arguments[0]==0?P:0;break;
   case 0x800c6c9c:assert(q.privateVector);w.vector=q.privateVector;*w.vector={{51,52,53}};break;
   case 0x800b6ad8:if(w.mutation)w.words[M+0x2e]=18;break;
   case 0x8009455c:assert(!q.arguments[0]);break;
   case 0x80001234:assert(q.arguments[0]==P);break;
   default:assert(false);
   }return true;};return s;
 }
};
// Direct original body reference. Only successful requests are traced. This
// deliberately spells source reads/stores independently from retained PCs.
void reference(World&w,bool prepare){auto s=w.services();auto word=[&](uint32_t a){return *s.word(s.user,a);};auto half=[&](uint32_t a){return *s.half(s.user,a);};auto byte=[&](uint32_t a){return *s.byte(s.user,a);};auto put=[&](uint32_t a,uint32_t v){assert(s.store(s.user,a,v));};
 std::array<std::optional<uint32_t>,3> v{};
 auto call=[&](uint32_t fn,uint32_t a=0){FieldMarioBattleTransferRequest q;q.function=fn;q.arguments[0]=a;if(fn==0x800c6c9c)q.privateVector=&v;assert(s.call(s.user,q));return q.result;};
 const auto m=word(0x8041e900);
 if(prepare){const auto x=word(m+0x8c),y=word(m+0x90);put(Saved,x);put(Saved+4,y);const auto z=word(m+0x94);put(Saved+8,z);call(0x800965fc);if(half(m+0x2e)!=20)put(m,word(m)|0x1000);}
 else{put(m,word(m)&~0x1000u);if(half(m+0x2e)==20){const auto x=word(Saved),y=word(Saved+4);put(m+0x8c,x);put(m+0x90,y);const auto z=word(Saved+8);put(m+0x94,z);}if(half(m+0x2e)==26)call(0x800b6ad8);}
 const auto id=*call(0x80141050),p=*call(0x800bf1b4,id);
 if(prepare){if(p&&byte(p+0x34)==3){const auto raw=byte(p+0x31);const int32_t kind=raw<128?int32_t(raw):int32_t(raw)-256;const auto ability=word(0x803143e8+uint32_t(kind*40)+0x10);if(ability)call(ability,p);}}
 else if(p){call(0x800c6c9c,p);const auto x=*v[0],y=*v[1];put(p+0x58,x);put(p+0x5c,y);put(p+0x60,*v[2]);}
 for(unsigned i=0;i<2;++i){const auto actor=word(0x8041ead8+4*i);if(actor)put(actor,prepare?word(actor)|0x20000000:word(actor)&~0x20000000u);}
 if(!prepare){const auto motion=half(m+0x2e);if(motion==18||motion==20)call(0x8009455c);}
}
void parity(bool prepare,bool party,bool mutation,uint32_t motion){
 World original(party);original.mutation=mutation;original.words[M+0x2e]=motion;reference(original,prepare);
 for(unsigned stall=0;stall<=original.accepted;++stall){World actual(party);actual.mutation=mutation;actual.words[M+0x2e]=motion;actual.holdAt=stall;
  FieldMarioBattleTransfer cursor(prepare?MarioFieldBattlePrepare:MarioFieldBattlePost);actual.cursor=&cursor;auto services=actual.services();
  for(unsigned n=0;n<200&&!cursor.complete();++n)cursor.resume(services);
  assert(cursor.complete()&&cursor.pending()==0);assert(actual.trace==original.trace&&actual.words==original.words);const auto accepted=actual.accepted;assert(cursor.resume(services)&&actual.accepted==accepted);
 }
}
void ownership(){World w;FieldMarioBattleTransfer cursor(MarioFieldBattlePrepare);w.cursor=&cursor;w.holdAt=7;auto s=w.services();assert(!cursor.resume(s));auto changed=s;int foreign=0;changed.user=&foreign;const auto accepted=w.accepted;assert(!cursor.resume(changed)&&w.accepted==accepted);w.recurse=true;for(unsigned i=0;i<20&&!cursor.complete();++i)cursor.resume(s);assert(cursor.complete());
 World stale;FieldMarioBattleTransfer held(MarioFieldBattlePrepare);stale.holdAt=6;auto ss=stale.services();assert(!held.resume(ss));const auto prefix=stale.accepted;stale.alive=false;assert(!held.resume(ss));stale.alive=true;assert(!held.resume(ss)&&stale.accepted==prefix);
 World missing;auto sm=missing.services();sm.call=nullptr;FieldMarioBattleTransfer nullChild(MarioFieldBattlePrepare);assert(!nullChild.resume(sm)&&nullChild.pending()==0x800965fc);const auto count=missing.accepted;assert(!nullChild.resume(sm)&&missing.accepted==count);
}
struct Partial:World{
 FieldMarioBattleTransferRequest* held=nullptr;unsigned visits=0,partialEffects=0;bool release=false;
 static bool call(void*p,FieldMarioBattleTransferRequest&q){auto&w=*static_cast<Partial*>(p);
  if(q.function==0x800c6c9c){if(!w.held){w.held=&q;++w.partialEffects;assert(q.privateVector);(*q.privateVector)[0]=81;}
   assert(w.held==&q&&q.arguments[0]==P);++w.visits;if(!w.release)return false;(*q.privateVector)[1]=82;return true;}
  auto fallback=w.World::services();return fallback.call(p,q);
 }
};
void privateOutput(){Partial w;FieldMarioBattleTransfer cursor(MarioFieldBattlePost);auto s=w.services();s.call=Partial::call;
 assert(!cursor.resume(s)&&w.visits==1);assert(!cursor.resume(s)&&w.visits==2&&w.partialEffects==1);
 w.release=true;assert(!cursor.resume(s)&&w.visits==3);assert(w.words[P+0x58]==81&&w.words[P+0x5c]==82);
 // Completed child did not initialize source stack Z. The parent remains at
 // its actual late read and neither repeats child nor invents a zero lane.
 assert(!cursor.resume(s)&&w.visits==3);(*w.held->privateVector)[2]=83;assert(cursor.resume(s)&&w.visits==3&&w.words[P+0x60]==83);
}
}
int main(){for(bool prepare:{false,true})for(bool party:{false,true})for(bool mutate:{false,true})for(uint32_t motion:{0u,18u,20u,26u})parity(prepare,party,mutate,motion);ownership();privateOutput();std::puts("Whole field Prepare/Post source traces, every boundary held twice, fresh reads and owner admission pass");return 0;}
