#include "native_harbor_field_sequence.hpp"
#include "field_npc_registry_owner.hpp"
#include "field_npc_battle_binding.hpp"
#include "event_native_runtime.hpp"
namespace ttyd {
namespace {
constexpr uint32_t FieldSequence=2,BattleSequence=4;
constexpr uint32_t FieldNpc=8,FieldInformation=0x20,InformationResult=0x10;
struct Guard{bool&b;explicit Guard(bool&v):b(v){b=true;}~Guard(){b=false;}};
bool sameOwner(const std::shared_ptr<const void>&a,const std::shared_ptr<const void>&b){return a&&b&&a==b&&!a.owner_before(b)&&!b.owner_before(a);}
}
struct NativeHarborFieldSequence::State {
 EventNativeRuntime*vm=nullptr;std::shared_ptr<FieldNpcRegistryOwner> owner;
 NativeFacadeRegistration::Proof vmProof,contextProof;
 enum class Phase{Field,Requested,Battle,Returned};Phase phase=Phase::Field;
 uint32_t gp=0,field=0,npc=0,p0=0,p1=0,result=0;
 NativeHarborBattleIdentity battle;OriginalSequence nativeSequence;
 std::unique_ptr<FieldNpcBattleBinding> sourceBinding;BattleSequenceEntryServices backend;
 std::optional<int32_t> sourceResult;EventEntry* sourceEntry=nullptr;uint32_t sourceId=0,sourceFunction=0;bool poisoned=false;
 std::shared_ptr<const NativeHarborBattleTicket::Seal> ticket;
 bool current()const{
  if(poisoned||!vmProof.current()||!contextProof.current()||(phase==Phase::Battle&&!battle.current()))return false;
  uint32_t now=0,mode=0,work=0;
  return owner&&owner->initialized()&&owner->aliases(vm->memory)&&vm->memory.readWord(0x80415fd0,now)&&now==gp&&vm->memory.readWord(gp+0x14,mode)&&!mode&&vm->memory.readWord(gp+0x168,work)&&work==field;
 }
 bool npcCurrent()const{uint32_t n=0,flags=0;return current()&&npc&&vm->memory.readWord(field+FieldNpc,n)&&n==npc&&owner->ownsFieldActor(npc)&&vm->memory.readWord(npc,flags)&&(flags&1);}
};
struct NativeHarborBattleTicket::Seal {std::weak_ptr<NativeHarborFieldSequence::State> state;};
NativeHarborFieldSequence::NativeHarborFieldSequence(std::shared_ptr<State>s):state_(std::move(s)){}
NativeHarborFieldSequence::~NativeHarborFieldSequence()=default;
std::shared_ptr<NativeHarborFieldSequence>NativeHarborFieldSequence::prepare(EventNativeRuntime&vm,std::shared_ptr<FieldNpcRegistryOwner>owner,NativeFacadeRegistration::Proof context){
 if(!owner||!owner->initialized()||!context.current())return {};
 auto s=std::make_shared<State>();s->vm=&vm;s->owner=std::move(owner);s->vmProof=vm.nativeFacadeProof();s->contextProof=std::move(context);s->field=s->owner->fieldWorkAddress();
 if(!vm.memory.readWord(0x80415fd0,s->gp)||!s->current())return {};
 s->nativeSequence.importNativeField();
 return std::shared_ptr<NativeHarborFieldSequence>(new NativeHarborFieldSequence(std::move(s)));
}
bool NativeHarborFieldSequence::current()const{return state_&&state_->current();}
bool NativeHarborFieldSequence::attach(FieldNpcSetupBinding&setup,const BattleSequenceEntryServices&backend,std::shared_ptr<FieldNpcRegularEventHistory>history){
 if(invoking_||!current()||state_->sourceBinding||state_->phase!=State::Phase::Field||state_->vm->schedule.managerActive)return false;
 Guard guard(invoking_);auto&s=*state_;s.backend=backend;
 BattleSequenceEntryServices guarded;guarded.user=&s;
 guarded.call=[](void*p,BattleSequenceCall&q){auto&s=*static_cast<State*>(p);if(!s.current()||!s.backend.call)return false;const bool completed=s.backend.call(s.backend.user,q);return s.current()&&completed;};
 s.sourceBinding=FieldNpcBattleBinding::bind(s.owner,setup,*s.vm,s.nativeSequence,guarded,std::move(history));return bool(s.sourceBinding);
}
bool NativeHarborFieldSequence::finishSource(){
 auto&s=*state_;if(!s.sourceResult)return false;
 if(s.sourceFunction!=0x8004ef54||*s.sourceResult!=0)return true;
 if(s.phase!=State::Phase::Field)return false;
 uint32_t npc=0,result=0;if(s.nativeSequence.state.requested!=int32_t(BattleSequence)||!s.vm->memory.readWord(s.field+FieldNpc,npc)||!s.owner->ownsFieldActor(npc)||!s.vm->memory.readWord(s.field+FieldInformation+InformationResult,result)||result)return false;
 uint32_t flags=0;if(!s.vm->memory.readWord(npc,flags)||!(flags&1))return false;
 s.npc=npc;s.p0=s.nativeSequence.state.p0;s.p1=s.nativeSequence.state.p1;auto seal=std::make_shared<NativeHarborBattleTicket::Seal>();seal->state=state_;s.ticket=std::move(seal);s.phase=State::Phase::Requested;return true;
}
std::optional<int32_t>NativeHarborFieldSequence::invokeSource(uint32_t function,EventEntry&e,bool first){
 if(invoking_||!current()||!state_->sourceBinding||!fieldNpcBattleEventFunction(function))return {};
 Guard guard(invoking_);auto&s=*state_;
 if(s.sourceEntry&&(s.sourceEntry!=&e||s.sourceId!=e.id||s.sourceFunction!=function))return {};
 if(!s.sourceEntry){s.sourceEntry=&e;s.sourceId=e.id;s.sourceFunction=function;}
 if(!s.sourceResult){auto result=s.sourceBinding->invoke(function,e,first);if(!result)return {};s.sourceResult=result;}
 if(!s.current()||!finishSource())return {};
 const auto result=s.sourceResult;s.sourceResult.reset();s.sourceEntry=nullptr;s.sourceId=s.sourceFunction=0;return result;
}
void NativeHarborFieldSequence::retire(const EventEntry&e){
 if(invoking_){if(state_)state_->poisoned=true;return;}
 if(!current()||!state_->sourceBinding)return;
 state_->sourceBinding->retire(e);
 if(state_->sourceEntry==&e&&state_->sourceId==e.id)state_->poisoned=true;
}
uint32_t NativeHarborFieldSequence::pending()const{return current()&&state_->sourceBinding?state_->sourceBinding->pendingFunction():0;}
std::optional<NativeHarborBattleTicket>NativeHarborFieldSequence::request()const{
 if(invoking_||!current()||state_->vm->schedule.managerActive||state_->phase!=State::Phase::Requested||!state_->npcCurrent())return {};
 return NativeHarborBattleTicket(state_->ticket);
}
bool NativeHarborFieldSequence::entered(const NativeHarborBattleTicket&t,const NativeHarborBattleIdentity&battle){
 if(invoking_||!current()||state_->vm->schedule.managerActive||!t.seal_||t.seal_!=state_->ticket||t.seal_->state.lock()!=state_||!battle.current())return false;
 Guard guard(invoking_);auto&s=*state_;
 if(s.phase==State::Phase::Battle)return s.battle.current()&&sameOwner(s.battle.lifetime(),battle.lifetime());
 if(s.phase!=State::Phase::Requested||!s.npcCurrent())return false;
 s.battle=battle;s.nativeSequence.state.previous=s.nativeSequence.state.current;s.nativeSequence.state.current=int32_t(BattleSequence);s.phase=State::Phase::Battle;return true;
}
bool NativeHarborFieldSequence::returned(const NativeHarborBattleTicket&t,const NativeHarborCompletionReceipt&r){
 if(invoking_||!current()||state_->vm->schedule.managerActive||!t.seal_||t.seal_!=state_->ticket||t.seal_->state.lock()!=state_)return false;
 Guard guard(invoking_);auto&s=*state_;
 if(!s.battle.completed(r)||!sameOwner(s.battle.lifetime(),r.battleLifetime)||(r.result!=1&&r.result!=3&&r.result!=4&&r.result!=5))return false;
 if(s.phase==State::Phase::Returned){uint32_t actual=0;return s.result==uint32_t(r.result)&&s.vm->memory.readWord(s.field+FieldInformation+InformationResult,actual)&&actual==s.result;}
 if(s.phase!=State::Phase::Battle||!s.npcCurrent())return false;
 if(!s.vm->memory.writeWord(s.field+FieldInformation+InformationResult,uint32_t(r.result)))return false;
 s.result=uint32_t(r.result);s.nativeSequence.state.previous=s.nativeSequence.state.current;s.nativeSequence.state.current=int32_t(FieldSequence);s.nativeSequence.state.requested=int32_t(FieldSequence);s.phase=State::Phase::Returned;return true;
}
}
