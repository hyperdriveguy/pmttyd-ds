#include "native_field_mario_key.hpp"
#include <utility>
namespace ttyd {
namespace {struct Guard{bool&v;explicit Guard(bool&b):v(b){v=true;}~Guard(){v=false;}};}
NativeFieldMarioKey::NativeFieldMarioKey(EventNativeRuntime&v,NativeFieldMarioKeyBinding b):vm_(&v),vmProof_(v.nativeFacadeProof()),binding_(std::move(b)){}
std::unique_ptr<NativeFieldMarioKey>NativeFieldMarioKey::create(EventNativeRuntime&vm,NativeFieldMarioKeyBinding b){
 const auto&w=b.world;
 if(!b.lifetime||!b.host.current()||!b.current||!w.admit||!w.storage.mario||!w.storage.read||!w.storage.write||!w.byte||!w.storeByte)return {};
 auto owner=std::unique_ptr<NativeFieldMarioKey>(new NativeFieldMarioKey(vm,std::move(b)));if(!owner->current())return {};return owner;
}
bool NativeFieldMarioKey::current(){
 if(stale_||checking_)return false;Guard guard(checking_);
 if(!vmProof_.current()||!binding_.host.current()){stale_=true;return false;}
 if(!binding_.current(binding_.user,*vm_)){stale_=true;return false;}return true;
}
std::optional<int32_t>NativeFieldMarioKey::invoke(uint32_t fn,EventEntry&e,bool){
 if(invoking_||fn!=NativeFieldMarioKeyFunction||!current())return {};Guard guard(invoking_);
 uint32_t address=0,args=0,raw=0;
 // Native preadmission is outside the original body. Fixed immediate input
 // reaches no unknown tagged-value memory. No source setter precedes this.
 if(!(e.flags&1)||e.argumentCount!=1||!vm_->entryAddresses.addressOf(&e,address)||!vm_->memory.readWord(address+0x18,args)||args!=e.arguments||!vm_->memory.readWord(args,raw)||raw>1)return {};
 if(!current())return {};
 FieldMarioPreparationOperands operands;operands.user=vm_;
 operands.word=[](void*p,uint32_t a){auto&vm=*static_cast<EventNativeRuntime*>(p);const auto s=vm.interpreterServices().operands;return s.readWord(s.user,a);};
 operands.value=[](void*p,EventEntry&entry,int32_t value){return getEventValue(entry,value,static_cast<EventNativeRuntime*>(p)->interpreterServices().operands);};
 const auto result=fieldMarioKeyEvent(e,operands,binding_.world);
 if(!current())return {};return result==2?std::optional<int32_t>{result}:std::nullopt;
}
}
