#include "field_npc_battle_binding.hpp"
#include "event_native_runtime.hpp"
namespace ttyd {
namespace {int32_t signedWord(uint32_t v){return v<=0x7fffffffu?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}}
FieldNpcRegularEventHistory::FieldNpcRegularEventHistory(std::shared_ptr<FieldNpcRegistryOwner> owner,EventNativeRuntime& vm):owner_(std::move(owner)),vm_(vm){}
std::shared_ptr<FieldNpcRegularEventHistory> FieldNpcRegularEventHistory::bind(std::shared_ptr<FieldNpcRegistryOwner> owner,EventNativeRuntime& vm){
 uint32_t gp=0,field=0;if(!owner || !owner->initialized() || !vm.memory.readWord(0x80415fd0,gp) || uint64_t(gp)+0x16b>UINT32_MAX || !vm.memory.readWord(gp+0x168,field) || field!=owner->fieldWorkAddress())return {};
 return std::shared_ptr<FieldNpcRegularEventHistory>(new FieldNpcRegularEventHistory(std::move(owner),vm));
}
bool FieldNpcRegularEventHistory::matches(const std::shared_ptr<FieldNpcRegistryOwner>& owner,const EventNativeRuntime& vm) const{return owner_==owner && &vm_==&vm;}
bool FieldNpcRegularEventHistory::context() const{return vm_.field.capacity>=0 && vm_.field.capacity<=256 && (!vm_.field.capacity || vm_.field.entries);}
bool FieldNpcRegularEventHistory::regular(const EventEntry& e,uint32_t actor) const{
 uint32_t script=0;return (e.flags&1) && e.rawBytes[0]==1 && e.field170==actor && owner_->ownsFieldActor(actor) && vm_.memory.readWord(actor+0x124,script) && e.restartScript==script;
}
void FieldNpcRegularEventHistory::retire(const EventEntry& e){
 if(!context())return;
 bool local=false;for(int n=0;n<vm_.field.capacity;++n)if(&vm_.field.entries[n]==&e){local=true;break;}
 if(!local)return;
 retired_.erase(e.id); // An actual reused ID cannot revive another birth's proof.
 if(regular(e,e.field170))retired_[e.id]={{e.field170,e.restartScript}};
}
bool FieldNpcRegularEventHistory::owns(uint32_t id,uint32_t actor) const{
 if(!context())return false;
 for(int n=0;n<vm_.field.capacity;++n){const auto& e=vm_.field.entries[n];if((e.flags&1) && e.id==id)return regular(e,actor);}
 auto i=retired_.find(id);uint32_t script=0;
 return i!=retired_.end() && i->second[0]==actor && vm_.memory.readWord(actor+0x124,script) && i->second[1]==script;
}
struct FieldNpcBattleBinding::Call {
 FieldNpcBattleBinding& binding;EventEntry& entry;uint32_t id,function,address;bool first;
 FieldNpcBattleEventCursor cursor;
 Call(FieldNpcBattleBinding& b,EventEntry& e,uint32_t fn,uint32_t a,bool f):binding(b),entry(e),id(e.id),function(fn),address(a),first(f){}
};
FieldNpcBattleBinding::FieldNpcBattleBinding(std::shared_ptr<FieldNpcRegistryOwner> owner,FieldNpcSetupBinding& setup,EventNativeRuntime& vm,OriginalSequence& sequence,const BattleSequenceEntryServices& backend,uint32_t gp,std::shared_ptr<FieldNpcRegularEventHistory> regular):owner_(std::move(owner)),setup_(setup),vm_(vm),sequence_(sequence),backend_(backend),regular_(std::move(regular)),gp_(gp){}
FieldNpcBattleBinding::~FieldNpcBattleBinding()=default;
std::unique_ptr<FieldNpcBattleBinding> FieldNpcBattleBinding::bind(std::shared_ptr<FieldNpcRegistryOwner> owner,FieldNpcSetupBinding& setup,EventNativeRuntime& vm,OriginalSequence& sequence,const BattleSequenceEntryServices& backend,std::shared_ptr<FieldNpcRegularEventHistory> regular){
 uint32_t gp=0,field=0,mode=0;
 if(!owner || !owner->initialized() || !setup.bindsFieldOwner(vm,owner) || !vm.memory.readWord(0x80415fd0,gp) || uint64_t(gp)+0x16b>UINT32_MAX || !vm.memory.readWord(gp+0x14,mode) || mode || !vm.memory.readWord(gp+0x168,field) || field!=owner->fieldWorkAddress())return {};
 if(!regular)regular=FieldNpcRegularEventHistory::bind(owner,vm);
 if(!regular || !regular->matches(owner,vm))return {};
 return std::unique_ptr<FieldNpcBattleBinding>(new FieldNpcBattleBinding(std::move(owner),setup,vm,sequence,backend,gp,std::move(regular)));
}
bool FieldNpcBattleBinding::current() const{
 uint32_t gp=0,field=0,mode=0;return vm_.memory.readWord(0x80415fd0,gp) && gp==gp_ && vm_.memory.readWord(gp+0x14,mode) && !mode && vm_.memory.readWord(gp+0x168,field) && field==owner_->fieldWorkAddress();
}
bool FieldNpcBattleBinding::pending() const{return !calls_.empty() || !retired_.empty();}
void FieldNpcBattleBinding::retire(const EventEntry& e){
 regular_->retire(e);
 auto i=calls_.find(e.id);if(i==calls_.end() || &i->second->entry!=&e)return;
 // Native cancellation of an opaque child is not bound. Preserve the cursor
 // and borrowed provider identity; never let a reused ID resume it.
 if(i->second->cursor.active){pending_=i->second->cursor.pending;if(!pending_)pending_=i->second->function;retired_.push_back(std::move(i->second));}
 calls_.erase(i);
}
BattleSequenceEntryServices FieldNpcBattleBinding::services(Call& call){
 BattleSequenceEntryServices s;s.user=&call;
 s.word=[](void* p,uint32_t a)->std::optional<uint32_t>{auto& c=*static_cast<Call*>(p);auto& b=c.binding;uint32_t v=0;
  if(!b.vm_.memory.readWord(a,v))return {};
  if(c.function==0x8004f894 && a==c.cursor.npc+0x11c && v){
   // Sailor scripts can return before their source postbattle query. Preserve
   // the exact retained ID when its same-VM retirement was actually observed.
   if(!b.regular_->owns(v,c.cursor.npc))return {};
  }return v;
 };
 s.store=[](void* p,uint32_t a,uint32_t v){return static_cast<Call*>(p)->binding.vm_.memory.writeWord(a,v);};
 s.call=[](void* p,BattleSequenceCall& child){auto& c=*static_cast<Call*>(p);auto& b=c.binding;
  switch(child.function){
  case 0x80031260:
   if(child.args[0]!=c.address)return false;
   child.result=uint32_t(getEventValue(c.entry,signedWord(child.args[1]),b.vm_.interpreterServices().operands));return true;
  case 0x80030cfc:
   if(child.args[0]!=c.address)return false;
   setEventValue(c.entry,signedWord(child.args[1]),signedWord(child.args[2]),b.vm_.interpreterServices().operands);return true;
  case 0x8004829c:{uint32_t gp=0;return b.vm_.memory.readWord(0x80415fd0,gp) && b.vm_.memory.readWord(gp+0x168,child.result);}
  case 0x8004f5d8:{
   if(child.args[0]!=c.address)return false;
   auto actor=b.setup_.resolveEventActor(c.entry,child.args[1],true);uint32_t flags=0;
   if(!actor || !b.owner_->ownsFieldActor(*actor) || !b.vm_.memory.readWord(*actor,flags) || !(flags&1))return false;
   child.result=*actor;return true;
  }
  case 0x80114508:
   if(child.args[0]!=b.owner_->fieldWorkAddress()+0x20)return false;
   return b.vm_.memory.writeWord(child.args[0]+0x10,child.args[1]);
  case 0x80114500:
   if(child.args[0]!=b.owner_->fieldWorkAddress()+0x20)return false;
   return b.vm_.memory.readWord(child.args[0]+0x10,child.result);
  case 0x8002e218:b.sequence_.request(signedWord(child.args[0]),child.args[1],child.args[2]);return true;
  case 0x8002e210:child.result=uint32_t(b.sequence_.state.current);return true;
  default:return b.backend_.call && b.backend_.call(b.backend_.user,child);
  }
 };return s;
}
std::optional<int32_t> FieldNpcBattleBinding::invoke(uint32_t fn,EventEntry& e,bool first){
 if(!fieldNpcBattleEventFunction(fn))return {};
 if(quarantined())return {};
 pending_=fn;
 uint32_t address=0;if(!vm_.entryAddresses.addressOf(&e,address))return {};
 auto& call=calls_[e.id];if(!call)call=std::make_unique<Call>(*this,e,fn,address,first);
 if(&call->entry!=&e || call->id!=e.id || call->function!=fn || call->address!=address)return {};
 if(!current() || !setup_.ownsCaller(e))return {};
 int32_t result=0;
 if(!fieldNpcBattleEvent(fn,address,call->first,services(*call),call->cursor,result)){pending_=call->cursor.pending;return {};}
 calls_.erase(e.id);pending_=0;return result;
}
}
