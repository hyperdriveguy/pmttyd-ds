#include "field_party_script_call.hpp"
#include "event_native_runtime.hpp"
namespace ttyd {
bool NativeFieldPartyScriptCall::same(const FieldPartyScriptBindings& b) const {
 return b.parties==binding_.parties && b.mario==binding_.mario && b.currentIndex==binding_.currentIndex && (b.pouch==binding_.pouch || (function_==0x800f0a8c && !binding_.pouch)) && b.root==binding_.root && b.parties->nativeAnimation==native_ && b.parties->originalPool==pool_;
}
bool NativeFieldPartyScriptCall::releaseForRetirement(){
 if(position_ || lifetime_)return false;
 modeLease_.reset();actor_=nullptr;return true;
}
BattleSequenceEntryServices NativeFieldPartyScriptCall::removalServices(){
 BattleSequenceEntryServices s;s.user=this;
 s.word=[](void* p,uint32_t a)->std::optional<uint32_t>{uint32_t v=0;auto& r=*static_cast<NativeFieldPartyScriptCall*>(p);return r.vm_->memory.readWord(a,v)?std::optional<uint32_t>(v):std::nullopt;};
 s.byte=[](void* p,uint32_t a)->std::optional<uint8_t>{auto& r=*static_cast<NativeFieldPartyScriptCall*>(p);if(a<FieldMarioOwnerAddress+0x245 || a>FieldMarioOwnerAddress+0x246)return {};return r.binding_.mario->partyIndices[a-FieldMarioOwnerAddress-0x245];};
 s.call=[](void* p,BattleSequenceCall& c){return static_cast<NativeFieldPartyScriptCall*>(p)->call(c);};return s;
}
bool NativeFieldPartyScriptCall::call(BattleSequenceCall& c){
 if(c.function==0x8005e0c0){c.result=FieldMarioOwnerAddress;return true;}
 if(c.function!=0x800be69c && c.function!=0x800be594)return false;
 if(!lifetime_)lifetime_=NativeFieldPartyLifetime::create(binding_.root.c_str(),*binding_.parties,*binding_.mario);
 if(!lifetime_){pending_=c.function;return false;}
 if(!lifetime_->kill(int32_t(c.args[0]),c.function==0x800be594)){pending_=lifetime_->pending();if(!pending_)pending_=c.function;return false;}
 c.result=0;return true;
}
int32_t NativeFieldPartyScriptCall::step(EventEntry& e,bool first,uint32_t fn,EventNativeRuntime& vm,const FieldPartyScriptBindings& b,uint64_t now){
 pending_=0;
 if(stage_!=Stage::Start && (&e!=entry_ || e.id!=id_ || &vm!=vm_ || fn!=function_)){pending_=function_;return 0;}
 if(bound_ && (!b.bound() || !same(b))){pending_=function_;return 0;}
 // A callback may wait before acquiring its mode lease. Validate the table
 // identity before touching a previously captured actor on every retry.
 if(actor_ && stage_!=Stage::Done && binding_.parties->pointer(index_)!=actor_){pending_=function_;return 0;}
 for(;;)switch(stage_){
 case Stage::Start:{
  if(fn!=0x800e3120 && fn!=0x800e2f00 && fn!=0x800e2da0 && fn!=0x800e2e0c && fn!=0x800eae70 && fn!=0x800eaeb8 && fn!=0x800f0a8c){pending_=fn;return 0;}
  vm_=&vm;entry_=&e;id_=e.id;function_=fn;first_=first;next(Stage::Bind);break;}
 case Stage::Bind:
  if(!b.bound()){pending_=fn;return 0;}
  binding_=b;native_=b.parties->nativeAnimation;pool_=b.parties->originalPool;bound_=true;next(Stage::Address);break;
 case Stage::Address:{
  const auto op=vm.interpreterServices().operands;
  if(!op.entryAddress || !(address_=op.entryAddress(op.user,e))){pending_=fn;return 0;}
  next(Stage::Dispatch);break;}
 case Stage::Dispatch:
  if(fn==0x800e3120 || fn==0x800e2f00){
   if(!position_)position_=std::make_unique<NativeFieldPartyPositionEvent>(binding_.root,*binding_.parties,*binding_.mario,binding_.pouch);
   const auto result=position_->step(e,first_,fn==0x800e2f00,vm.memory,vm.interpreterServices().operands,b.bank,now,b.appearance);
   if(result!=2){pending_=position_->pending();if(!pending_)pending_=fn;return 0;}next(Stage::Done);break;
  }
  if(fn==0x800e2da0 || fn==0x800e2e0c){
   int32_t result=0;if(!removeFieldPartyEvent(address_,fn==0x800e2da0,removalServices(),removal_,result)){if(!pending_)pending_=removal_.pending;return 0;}
   next(Stage::Done);break;
  }
  args_=e.arguments;next(fn==0x800f0a8c?Stage::JoinRaw:Stage::ModeRaw);break;
 case Stage::JoinRaw:
  if(!vm.memory.readWord(args_,raw_)){pending_=fn;return 0;}next(Stage::JoinValue);break;
 case Stage::JoinValue:
  kind_=getEventValue(e,int32_t(raw_),vm.interpreterServices().operands);next(Stage::Join);break;
 case Stage::Join:
  if(kind_<0){pending_=0x80140f74;return 0;} // Original unchecked negative row domain is not a native byte owner.
  if(kind_<8){if(!binding_.pouch)binding_.pouch=b.pouch;if(!binding_.pouch){pending_=0x80140f74;return 0;}const unsigned offset=14*unsigned(kind_);binding_.pouch->half(offset,int16_t(uint16_t(binding_.pouch->half(offset))|1));}
  next(Stage::Done);break;
 case Stage::ModeRaw:
  if(!vm.memory.readWord(args_,selector_)){pending_=fn;return 0;}next(Stage::ModeLookup);break;
 case Stage::ModeLookup:
  if(selector_)index_=1;
  else {if(!binding_.currentIndex || !*binding_.currentIndex){pending_=fn;return 0;}index_=**binding_.currentIndex;}
  next(Stage::ModeLease);break;
 case Stage::ModeLease:
  // Serialize BEFORE capturing the source table pointer, so an outstanding
  // allocation/removal cannot replace that incarnation across a native wait.
  if(!binding_.parties->nativeEntryOwner.expired() || !binding_.parties->nativeRemovalOwner.expired()){pending_=fn;return 0;}
  binding_.parties->nativeEntryOwner=modeLease_;next(Stage::ModeActor);break;
 case Stage::ModeActor:
  if(index_>=2){pending_=0x800bf1b4;return 0;}
  actor_=binding_.parties->pointer(index_);if(!actor_){modeLease_.reset();next(Stage::Done);break;}next(Stage::ModeGate);break;
 case Stage::ModeGate:
  if(fn==0x800eae70 && actor_->bytes[0x34]!=0){modeLease_.reset();next(Stage::Done);break;}
  newMode_=fn==0x800eae70?1:0;next(Stage::ModeOuter);break;
 case Stage::ModeOuter:
  oldMode_=actor_->bytes[0x34];if(oldMode_!=9 && oldMode_!=3)actor_->bytes[0x35]=uint8_t(oldMode_);
  oldMode_=actor_->bytes[0x34];next(Stage::ModeInner);break;
 case Stage::ModeInner:
  if(oldMode_==1 || oldMode_==2){next(Stage::Slit);break;}
  if(oldMode_==3 || oldMode_==4 || oldMode_==11){pending_=0x800bcb98;return 0;}
  if(oldMode_==12)actor_->word(4,actor_->word(4)&~0x100000u);
  next(Stage::ModeFinish);break;
 case Stage::Slit:
  if(!issued_){slit_=(actor_->word(0)&0x1000) || (actor_->word(4)&0x100);issued_=true;}
  if(slit_){pending_=0x8014c448;return 0;}
  next(Stage::ModeFinish);break;
 case Stage::ModeFinish:
  actor_->bytes[0x34]=uint8_t(newMode_);actor_->word(8,actor_->word(8)|8);modeLease_.reset();next(Stage::Done);break;
 case Stage::Done:return 2;
 }
}
}
