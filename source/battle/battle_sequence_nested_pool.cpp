#include "battle_sequence_nested_pool.hpp"
#include "battle_sequence_binding.hpp"
#include "event_native_runtime.hpp"
#include "game.hpp"
namespace ttyd {
namespace {
constexpr uint32_t MapGlobals=0x8041e808,BattleRootCell=0x8041e80c;
constexpr uint32_t BattlePoolBytes=0x64000,HeaderBytes=32;
struct Checking {bool&value;explicit Checking(bool&v):value(v){value=true;}~Checking(){value=false;}};
}
// Process-owned exclusive primary-allocation epoch. No caller can assert a
// ready bit, synthesize a root or retire/free then recreate behind this lease.
class BattleSequenceNestedPoolEpoch {
public:
 bool active=false,complete=false,claimed=false;
};
struct BattleSequenceNestedPoolReceipt::State {
 Game*game=nullptr;EventNativeRuntime*vm=nullptr;
 NativeFacadeRegistration::Proof gameLife,vmLife,bindingLife;
 std::shared_ptr<BattleSequenceProcess>process;
 std::shared_ptr<BattleSequenceStorage>storage;
 std::shared_ptr<OriginalSequence>sequence;
 std::shared_ptr<FieldNpcRegistryOwner>npcs;
 std::shared_ptr<BattleSequenceNestedPoolEpoch>epoch;
 uint32_t primary=0,nested=0;
 void*platformUser=nullptr;
 void*memoryUser=nullptr;void(*memoryFlush)(void*,uint32_t)=nullptr;void(*memoryBarrier)(void*)=nullptr;
 void(*flush)(void*,uint32_t)=nullptr;void(*barrier)(void*)=nullptr;
 mutable bool checking=false;
 bool nativeCurrent()const{
  // Both native facade registrations precede EVERY borrowed Game/VM access.
  if(!gameLife.current()||!vmLife.current()||!bindingLife.current())return false;
  return game&&vm&&process&&storage&&game->battleSequenceProcess==process&&
   process->storage==storage&&game->progress.originalPouch==storage->pouch_&&game->originalSequence==sequence&&game->originalFieldNpcs==npcs&&
   &vm->memoryBinding.memory==&vm->memory&&vm->memoryBinding.platformUser==memoryUser&&
   vm->memoryBinding.flushBlock==memoryFlush&&vm->memoryBinding.systemCall==memoryBarrier&&
   vm->platform.user==platformUser&&vm->platform.flushBlock==flush&&vm->platform.systemCall==barrier;
 }
};
BattleSequenceNestedPoolReceipt::BattleSequenceNestedPoolReceipt():state_(std::make_unique<State>()){}
BattleSequenceNestedPoolReceipt::~BattleSequenceNestedPoolReceipt()=default;
std::shared_ptr<BattleSequenceNestedPoolReceipt> BattleSequenceNestedPoolReceipt::prepare(BattleSequenceBinding&b){
 if(!b.facadeCurrent()||!b.storage_||b.storageMappings_.empty()||b.process_->nestedPoolEpoch_)return {};
 auto out=std::shared_ptr<BattleSequenceNestedPoolReceipt>(new BattleSequenceNestedPoolReceipt);auto&s=*out->state_;
 s.game=&b.game_;s.vm=&b.vm_;s.gameLife=b.gameLife_;s.vmLife=b.vmLife_;s.bindingLife=b.facade_.proof();
 s.process=b.process_;s.storage=b.storage_;s.sequence=b.game_.originalSequence;s.npcs=b.npcs_;
 s.memoryUser=b.vm_.memoryBinding.platformUser;s.memoryFlush=b.vm_.memoryBinding.flushBlock;s.memoryBarrier=b.vm_.memoryBinding.systemCall;
 s.platformUser=b.vm_.platform.user;s.flush=b.vm_.platform.flushBlock;s.barrier=b.vm_.platform.systemCall;
 if(!s.flush||!s.barrier||!s.nativeCurrent())return {};
 uint32_t oldRoot=0;if(!s.vm->memory.readWord(MapGlobals,s.primary)||!s.vm->memory.readWord(BattleRootCell,oldRoot)||oldRoot||
  !s.storage->canAllocate(s.primary,BattlePoolBytes,s.vm->memory))return {};
 s.epoch=std::make_shared<BattleSequenceNestedPoolEpoch>();
 // Reserve the exclusive epoch before any source writes/flush callbacks. It is
 // NOT complete or externally admitted until the actual producer commits.
 b.process_->nestedPoolEpoch_=s.epoch;return out;
}
bool BattleSequenceNestedPoolReceipt::complete(){
 auto&s=*state_;if(s.checking||!s.epoch||!s.nativeCurrent()||s.epoch->complete)return false;Checking guard(s.checking);
 uint32_t primary=0,nested=0,size=0;
 if(!s.vm->memory.readWord(MapGlobals,primary)||primary!=s.primary||!s.vm->memory.readWord(BattleRootCell,nested)||!nested||
  !s.storage->ownsPrimaryAllocation(nested,BattlePoolBytes,s.vm->memory)||!s.vm->memory.readWord(nested+4,size)||size!=BattlePoolBytes-HeaderBytes)return false;
 s.nested=nested;s.epoch->complete=s.epoch->active=true;return true;
}
bool BattleSequenceNestedPoolReceipt::current()const{
 const auto&s=*state_;if(s.checking||!s.epoch||!s.nativeCurrent()||!s.epoch->active||!s.epoch->complete)return false;Checking guard(s.checking);
 if(s.process->nestedPoolEpoch_!=s.epoch||!s.storage->aliases(s.vm->memory))return false;
 uint32_t primary=0,nested=0;
 return s.vm->memory.readWord(MapGlobals,primary)&&primary==s.primary&&s.vm->memory.readWord(BattleRootCell,nested)&&nested==s.nested&&
  s.storage->ownsPrimaryAllocation(s.nested,BattlePoolBytes,s.vm->memory)&&s.nativeCurrent()&&s.epoch->active;
}
bool BattleSequenceNestedPoolReceipt::accepts(const EventNativeRuntime&vm)const{return state_->vm==&vm&&current();}
uint32_t BattleSequenceNestedPoolReceipt::root()const{return current()?state_->nested:0;}
void BattleSequenceNestedPoolReceipt::retire(){if(state_->epoch)state_->epoch->active=false;}
bool BattleSequenceNestedPoolReceipt::claim(const EventNativeRuntime&vm)const{
 if(!accepts(vm)||state_->epoch->claimed)return false;
 state_->epoch->claimed=true;return true;
}
}
