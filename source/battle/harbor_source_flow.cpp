#include "harbor_source_flow.hpp"
#include "battle_main_continuation.hpp"
#include "battle_sequence_startup.hpp"
#include "battle_attack_selection.hpp"
#include "battle_packed_recovery.hpp"
#include "battle_packed_status_recovery.hpp"
#include "battle_init_sequence_continuation.hpp"
#include "battle_pinch_continuation.hpp"
#include "battle_event_barrier_continuation.hpp"
#include "battle_event_callbacks.hpp"
#include "battle_conclusion.hpp"
#include "battle_phase.hpp"
#include "battle_majinai_query.hpp"
#include "battle_audience_turn_end_continuation.hpp"
#include "event_memory_opcodes.hpp"
#include "event_lifecycle.hpp"
#include <cmath>
#include <cstring>
#include <cstdlib>
namespace ttyd {namespace {
struct Guard{bool&v;explicit Guard(bool&b):v(b){v=true;}~Guard(){v=false;}};
void required(bool v){if(!v)std::abort();}
}
struct HarborSourceFlow::State {
 HarborSourceFlowBinding b;mutable bool invalid=false,checking=false;uint32_t pending=0;
 BattleMainCursor main;BattleSequenceDispatchCursor sequence;BattleInitSequenceCursor init;BattleFirstActCursor first;
 BattleTurnCursor turn;BattlePhaseCursor phase;BattleActCursor act;BattlePinchCursor pinch;
 BattleEventBarrierCursor barrier,consume;BattleAudienceTurnEndCursor audienceEnd;
 BattleTurnFirstBackend turnFirst;
 const BattleMoveRequest*moveBorrow=nullptr;BattleTurnRequest moveLinked;
 bool current()const{if(invalid||checking)return false;Guard guard(checking);if(!b.lifetime||!b.vm||!b.current||!b.current())invalid=true;return !invalid;}
 BattleActorServices actors()const{return nativeBattleActorServices(*b.vm);}
 uint32_t word(uint32_t a)const{uint32_t v;required(b.vm->memory.readWord(a,v));return v;}
 uint16_t half(uint32_t a)const{uint16_t v;required(b.vm->memory.readHalf(a,v));return v;}
 uint8_t byte(uint32_t a)const{uint8_t v;required(b.vm->memory.readByte(a,v));return v;}
 void store(uint32_t a,uint32_t v){required(b.vm->memory.writeWord(a,v));}
 float real(uint32_t a)const{const auto v=word(a);float f;std::memcpy(&f,&v,4);return f;}
 std::optional<uint32_t>createEvent(uint32_t script,int32_t priority,uint32_t flags){auto&e=createRootEvent(b.vm->schedule,script,priority,flags,b.vm->creationServices());uint32_t p;required(b.vm->entryAddresses.addressOf(&e,p));return p;}
 BattlePhaseServices phaseServices(){BattlePhaseServices s;s.actors=actors();s.real=[](void*p,uint32_t a){uint32_t v;required(static_cast<EventNativeRuntime*>(p)->memory.readWord(a,v));float f;std::memcpy(&f,&v,4);return f;};s.status=[](void*p,uint32_t u,int32_t id){return checkPackedBattleStatus(u,id,nativeBattleActorServices(*static_cast<EventNativeRuntime*>(p)));};return s;}
 int32_t concluded(uint32_t battle){BattleConclusionServices s;s.actors=actors();s.user=this;s.checkStatus=[](void*p,uint32_t u,int32_t id){return int32_t(checkPackedBattleStatus(u,id,static_cast<State*>(p)->actors()));};return battleCheckConcluded(battle,s);}
 BattleEventBarrierServices barrierServices(){BattleEventBarrierServices s;s.actors=actors();s.user=this;s.eventExists=[](void*p,uint32_t id)->std::optional<int32_t>{auto&r=*static_cast<State*>(p);if(!r.current())return {};return checkEventId(id,r.b.vm->creationServices());};s.concluded=[](void*p,uint32_t battle)->std::optional<int32_t>{auto&r=*static_cast<State*>(p);return r.current()?std::optional<int32_t>(r.concluded(battle)):std::nullopt;};s.create=[](void*p,uint32_t script,int32_t priority,uint32_t flags){auto&r=*static_cast<State*>(p);return r.current()?r.createEvent(script,priority,flags):std::nullopt;};s.removeItem=[](void*p,uint32_t item)->std::optional<int32_t>{auto&r=*static_cast<State*>(p);const auto v=r.current()&&r.b.removeItem?r.b.removeItem(item):std::nullopt;return r.current()?v:std::nullopt;};return s;}
 // A value is returned only for a whole actual admitted VM leaf. The handled
 // lane distinguishes an unavailable known child from an unknown function.
 std::optional<uint32_t>common(uint32_t fn,const std::array<uint32_t,8>&a,bool&handled){handled=true;pending=fn;if(!current())return {};const auto memory=actors();
  switch(fn){
  case 0x800374ac:return createEvent(a[0],int32_t(a[1]),a[2]);
  case 0x80036604:return uint32_t(checkEventId(a[0],b.vm->creationServices()));
  case 0x800f8728:return battlePartyUnit(a[0],memory);
  case 0x800f87a8:return battleMarioUnit(a[0],memory);
  case 0x80128144:return byte(a[0]+0xc);
  case 0x8012733c:return packedUnitData(a[0],a[1],memory);
  case 0x80127814:return checkPackedBattleStatus(a[0],int32_t(a[1]),memory);
  case 0x80127890:return uint32_t(checkPackedBattleRecoveryStatus(a[0],int32_t(a[1]),memory));
  case 0x801266d4:resetPackedBattleMoveStatus(a[0],memory);return 0;
  case 0x80126bec:recoverPackedBattleHp(a[0],int32_t(a[1]),memory);return 0;
  case 0x80126b58:recoverPackedBattleFp(a[0],int32_t(a[1]),memory);return 0;
  case 0x80005da0:if(!b.vm->platform.irand)return {};return uint32_t(b.vm->platform.irand(b.vm->platform.user,int32_t(a[0])));
  case 0x8011a06c:return word(currentBattleAddress(memory)+0xf44)&a[0];
  case 0x8011aa34:return uint32_t(concluded(a[0]));
  case 0x8011ace8:{const auto v=resumeBattleAllEventBarrier(a[0],barrierServices(),barrier);if(!v){pending=barrier.pending()?barrier.pending():fn;return {};}return uint32_t(*v);}
  case 0x800f74b4:if(!resumeBattleConsumeReservedItems(barrierServices(),consume)){pending=consume.pending()?consume.pending():fn;return {};}return 0;
  case 0x8011b1ec:battleTurnFirstProcess(a[0],nativeBattleTurnFirstServices(turnFirst));return 0;
  case 0x8011b0d0:buildBattlePhaseTables(a[0],phaseServices());return 0;
  case 0x8011bb88:return battlePhaseEnded(currentBattleAddress(memory),phaseServices());
  case 0x801297bc:{const auto script=word(a[0]+(a[1]?0x294:0x298));if(!script)return 0;const auto p=createEvent(script,10,0x20);if(!p)return {};store(*p+0x160,word(a[0]));store(a[0]+0x29c,word(*p+0x15c));return word(*p+0x15c);}
  case 0x800f7718:case 0x800f76d4:{auto s=nativeBattleMajinaiQueryServices(*b.vm);if(fn==0x800f7718)battleMajinaiDone(s);else battleMajinaiEndCheck(s);return 0;}
  case 0x800050b4:return eventFillMemory(a[0],int32_t(a[1]),a[2],memory.memory);
  case 0x80127434:{const auto low=uint16_t(a[1]);store(a[0]+0x264,low<0x8000?low:uint32_t(int32_t(low)-65536));return 0;}
  default:handled=false;return {};
  }
 }
 BattleFirstActContinuationServices startupServices(){BattleFirstActContinuationServices s;s.actors=actors();s.lifetime=b.lifetime;s.user=this;s.current=[](void*p){return static_cast<State*>(p)->current();};s.call=[](void*p,const BattleFirstActRequest&q)->std::optional<uint32_t>{auto&r=*static_cast<State*>(p);std::array<uint32_t,8>a{};for(unsigned i=0;i<4;++i)a[i]=q.integers[i];bool handled=false;const auto v=r.common(q.function,a,handled);if(handled){if(v)r.pending=0;return v;}
   if(q.function==0x80129994){BattleStartupServices ready;ready.memory=r.actors();ready.user=&r;ready.eventExists=[](void*p,uint32_t id){auto&s=*static_cast<State*>(p);return checkEventId(id,s.b.vm->creationServices())!=0;};return packedBattleUnitInitReady(q.integers[0],ready);}
   if(q.function==0x800feaf8){if(!q.firstAttackUnits)return {};uint32_t attacker,target;const auto result=packedFirstAttackPair(q.integers[0],&attacker,&target,r.actors());(*q.firstAttackUnits)[0]=attacker;(*q.firstAttackUnits)[1]=target;return result;}
   const auto out=r.current()&&r.b.startup?r.b.startup(q):std::nullopt;if(out)r.pending=0;return out;};return s;}
 BattleTurnServices turnServices(){BattleTurnServices s;s.actors=actors();s.user=this;s.call=[](void*p,const BattleTurnRequest&q)->std::optional<BattleTurnOutput>{auto&r=*static_cast<State*>(p);const auto v=r.turnCall(q);if(v)r.pending=0;return r.current()?v:std::nullopt;};return s;}
 std::optional<BattleTurnOutput>turnCall(const BattleTurnRequest&q){if(!current())return {};BattleTurnOutput out;bool handled=false;const auto v=common(q.function,q.integers,handled);if(handled){if(v)out.value=*v;return v?std::optional<BattleTurnOutput>(out):std::nullopt;}
  switch(q.function){
  case 0x80127f34:{const auto s=getPackedBattleStatus(q.integers[0],int32_t(q.integers[1]),actors());out.duration=s.duration;out.strength=s.strength;return out;}
  case 0x80125d08:out.value=(word(q.integers[0]+0x104)&0x10)?1:word(q.integers[0]+8)==228&&int32_t(word(q.integers[0]+0x228))>0;return out;
  case 0x80128c7c:for(unsigned i=0;i<3;++i)out.position[i]=real(q.integers[0]+0x3c+4*i);return out;
  case 0x80128414:out.value=uint32_t(int32_t(int16_t(half(q.integers[0]+0xce))));return out;
  case 0x80128c98:out.value=uint32_t(packedBodyPartId(q.integers[0],actors().memory));return out;
  case 0x80128cd8:out.value=packedUnitPart(q.integers[0],int32_t(q.integers[1]),actors().memory);return out;
  case 0x80128b10:out.position=packedPartWorldPosition(q.integers[0],nativeBattlePartPositionServices(*b.vm));return out;
  case 0x8011c2b4:if(!resumePackedBattlePhase(q.integers[0],moveServices(),phase)){if(!pending||pending==q.function)pending=phase.pending();return {};}return out;
  case 0x8019f298:{BattleAudienceTurnEndServices s;s.actors=actors();s.user=this;s.divide=[](void*p,int32_t a,int32_t d)->std::optional<int32_t>{auto&r=*static_cast<State*>(p);if(!r.current()||!d||(a==(-2147483647-1)&&d==-1)||!r.b.vm->platform.divideWord)return {};return r.b.vm->platform.divideWord(r.b.vm->platform.user,a,d);};s.truncate=[](void*p,float f)->std::optional<int32_t>{auto&r=*static_cast<State*>(p);if(!r.current()||!std::isfinite(f)||double(f)<-2147483648.||double(f)>=2147483648.||!r.b.vm->platform.truncate)return {};return r.b.vm->platform.truncate(r.b.vm->platform.user,f);};s.random=[](void*p,int32_t n)->std::optional<int32_t>{auto&r=*static_cast<State*>(p);if(!r.current()||!r.b.vm->platform.irand)return {};return r.b.vm->platform.irand(r.b.vm->platform.user,n);};if(!resumePackedBattleAudienceTurnEnd(s,audienceEnd)){pending=audienceEnd.pending();return {};}return out;}
  default:{const auto v=b.turn?b.turn(q):std::nullopt;return current()?v:std::nullopt;}
  }
 }
 BattleMoveServices moveServices(){BattleMoveServices s;s.actors=actors();s.user=this;s.call=[](void*p,const BattleMoveRequest&q)->std::optional<uint32_t>{auto&r=*static_cast<State*>(p);if(!r.current())return {};r.pending=q.function;if(q.function==0x8011b2dc){if(!resumePackedBattleAct(q.integers[0],r.actServices(),r.act)){if(!r.pending||r.pending==q.function)r.pending=r.act.pending();return {};}return 0;}
   bool handled=false;const auto v=r.common(q.function,q.integers,handled);if(handled){if(v)r.pending=0;return v;}
   // Shared original Turn ABI owns stable converted arguments across waits.
   if(!r.moveBorrow){r.moveBorrow=&q;r.moveLinked={};r.moveLinked.function=q.function;r.moveLinked.integers=q.integers;}else if(r.moveBorrow!=&q||r.moveLinked.function!=q.function||r.moveLinked.integers!=q.integers)return {};
   const auto output=r.turnCall(r.moveLinked);if(!output)return {};r.moveBorrow=nullptr;return output->value;};return s;}
 BattleActServices actServices(){BattleActServices s;s.actors=actors();s.user=this;s.call=[](void*p,const BattleActSequenceRequest&q)->std::optional<BattleActOutput>{auto&r=*static_cast<State*>(p);if(!r.current())return {};r.pending=q.function;const auto&a=q.arguments;BattleActOutput out;switch(q.child){
   case BattleActChild::CommandInit:r.store(a[0]+0x171c,0);r.store(a[0]+0xef4,r.word(a[0]+0xef4)|0x80);return out;
   case BattleActChild::CommandInput:{const auto v=r.b.commandInput?r.b.commandInput(a[0]):std::nullopt;if(!v||!r.current())return {};out.value=uint32_t(*v);return out;}
   case BattleActChild::Unit:out.value=battleRegistryUnit(a[0],int32_t(a[1]),r.actors());return out;
   case BattleActChild::Mario:out.value=battleMarioUnit(a[0],r.actors());return out;
   case BattleActChild::CheckStatusFlag:out.value=a[0]?(r.word(a[0]+0x138)&a[1]):0;return out;
   case BattleActChild::ClearStatusFlag:r.store(a[0]+0x138,r.word(a[0]+0x138)&~a[1]);return out;
   case BattleActChild::Belong:out.value=r.byte(a[0]+0xc);return out;
   case BattleActChild::PhaseEnd:out.value=battlePhaseEnded(currentBattleAddress(r.actors()),r.phaseServices());return out;
   case BattleActChild::DeleteEvent:deleteEventId(a[0],r.b.vm->creationServices());return out;
   case BattleActChild::EventExists:out.value=checkEventId(a[0],r.b.vm->creationServices());return out;
   default:break;}
   std::array<uint32_t,8>args{};for(unsigned i=0;i<4;++i)args[i]=a[i];bool handled=false;const auto value=r.common(q.function,args,handled);if(handled){if(value)out.value=*value;return value?std::optional<BattleActOutput>(out):std::nullopt;}const auto v=r.b.act?r.b.act(q):std::nullopt;return r.current()?v:std::nullopt;};return s;}
 bool sequenceChild(const BattleSequenceDispatchRequest&q){if(!current())return false;pending=q.function;switch(q.function){
  case 0x8011ee40:if(resumePackedBattleSequenceInit(q.battle,startupServices(),init))return true;if(!pending||pending==q.function)pending=init.pending();return false;
  case 0x8011e5c0:if(resumePackedBattleFirstAct(q.battle,startupServices(),first))return true;if(!pending||pending==q.function)pending=first.pending();return false;
  case 0x8011efd8:{BattlePinchServices s;s.actors=actors();s.user=this;s.status=[](void*p,uint32_t u,int32_t id)->std::optional<int32_t>{auto&r=*static_cast<State*>(p);return r.current()?std::optional<int32_t>(checkPackedBattleStatus(u,id,r.actors())):std::nullopt;};s.truncate=[](void*p,float f)->std::optional<int32_t>{auto&r=*static_cast<State*>(p);if(!r.current()||!std::isfinite(f)||double(f)<-2147483648.||double(f)>=2147483648.||!r.b.vm->platform.truncate)return {};return r.b.vm->platform.truncate(r.b.vm->platform.user,f);};s.sound=[](void*p,uint32_t key){auto&r=*static_cast<State*>(p);return r.current()&&r.b.pinchSound&&r.b.pinchSound(key);};if(resumePackedBattleCheckAllPinch(q.battle,q.mode,s,pinch))return true;pending=pinch.pending();return false;}
  case 0x8011c510:if(resumePackedBattleTurn(q.battle,turnServices(),turn))return true;if(!pending||pending==q.function)pending=turn.pending();return false;
  case 0x80215234:pending=q.function;return b.end&&b.end(q.battle)&&current();
  default:pending=q.function;return false;
  }}
 bool mainChild(uint32_t fn,uint32_t battle){pending=fn;if(!current())return false;if(fn==0x8011f1ec){BattleSequenceDispatchServices s;s.actors=actors();s.user=this;s.request=[](void*p,const BattleSequenceDispatchRequest&q){return static_cast<State*>(p)->sequenceChild(q);};if(resumePackedBattleSequenceManager(s,sequence))return true;if(!pending)pending=sequence.pending();return false;}const bool complete=b.main&&b.main(fn,battle);return current()&&complete;}
};
HarborSourceFlow::HarborSourceFlow()=default;HarborSourceFlow::~HarborSourceFlow()=default;
std::unique_ptr<HarborSourceFlow>HarborSourceFlow::create(HarborSourceFlowBinding b){if(!b.vm||!b.lifetime||!b.current)return {};auto p=std::unique_ptr<HarborSourceFlow>(new HarborSourceFlow);p->state_=std::make_unique<State>();p->state_->b=std::move(b);if(!p->state_->current())return {};p->state_->turnFirst.runtime=p->state_->b.vm;return p;}
bool HarborSourceFlow::resumeMain(){if(invoking_||!state_)return false;Guard guard(invoking_);auto&s=*state_;if(!s.current())return false;s.pending=0;BattleMainServices main;main.actors=s.actors();main.user=&s;main.request=[](void*p,uint32_t fn,uint32_t b){return static_cast<State*>(p)->mainChild(fn,b);};const auto done=resumePackedBattleMainWrapper(main,s.main);if(!done&&!s.pending)s.pending=s.main.pending();if(done)s.pending=0;return done&&s.current();}
bool HarborSourceFlow::active()const{return state_&&state_->main.active();}uint32_t HarborSourceFlow::pending()const{return state_?state_->pending:0;}bool HarborSourceFlow::current()const{if(invoking_||!state_)return false;Guard guard(invoking_);return state_->current();}
}
