#include "field_npc_creation_binding.hpp"
#include <cassert>
#include <cmath>
namespace ttyd {
namespace {
struct Operands {
 EventNativeRuntime& vm;bool valid=true;
 EventOperandServices services(){
  EventOperandServices s;s.user=this;
  s.contextAddress=[](void* p)->std::uint32_t{auto& a=*static_cast<Operands*>(p);auto* g=a.vm.gpAddresses.currentGlobals();if(!g){a.valid=false;return 0u;}return g->battleContext?0x803d8dd0u:0x803d8d30u;};
  s.entryAddress=[](void* p,EventEntry& e){auto& a=*static_cast<Operands*>(p);uint32_t at=0;if(!a.vm.entryAddresses.addressOf(&e,at))a.valid=false;return at;};
  s.globalAddress=[](void* p){auto& a=*static_cast<Operands*>(p);if(!a.vm.gpAddresses.currentGlobals())a.valid=false;return a.vm.gpAddresses.currentAddress();};
  s.readWord=[](void* p,uint32_t at){auto& a=*static_cast<Operands*>(p);uint32_t v=0;if(!a.vm.memory.readWord(at,v))a.valid=false;return v;};
  s.readByte=[](void* p,uint32_t at){auto& a=*static_cast<Operands*>(p);uint8_t v=0;if(!a.vm.memory.readByte(at,v))a.valid=false;return v;};
  s.truncate=[](void* p,float v){auto& a=*static_cast<Operands*>(p);if(!a.vm.platform.truncate || !std::isfinite(v) || double(v)<-2147483648. || double(v)>=2147483648.){a.valid=false;return int32_t(0);}return a.vm.platform.truncate(a.vm.platform.user,v);};return s;
 }
};
}
FieldNpcCreationBinding::FieldNpcCreationBinding(std::shared_ptr<FieldNpcRegistryOwner> o,FieldNpcRegistryBinding& r,FieldNpcSetupBinding& s,EventNativeRuntime& vm,std::shared_ptr<NativeFieldAnimation> a):owner_(std::move(o)),registry_(r),setup_(s),vm_(vm),animation_(std::move(a)){}
std::unique_ptr<FieldNpcCreationBinding> FieldNpcCreationBinding::bind(std::shared_ptr<FieldNpcRegistryOwner> owner,FieldNpcRegistryBinding& registry,FieldNpcSetupBinding& setup,EventNativeRuntime& vm,std::shared_ptr<NativeFieldAnimation> animation){
 if(!owner || !animation || !animation->ready() || owner->entryPending() || owner->poses().sharedPool()!=animation->pool() || registry.owner_!=owner || &registry.memory_!=&vm.memory || !owner->aliases(vm.memory) || !setup.bindsFieldOwner(vm,owner))return {};
 auto b=std::unique_ptr<FieldNpcCreationBinding>(new FieldNpcCreationBinding(std::move(owner),registry,setup,vm,std::move(animation)));
 if(!vm.memory.readWord(0x80415fd0,b->gp_))return {};
 auto& target=b->animation_->process().vm().memory;
 if(!b->owner_->aliases(target)){b->animationRegistry_=FieldNpcRegistryBinding::bind(b->owner_,target);if(!b->animationRegistry_)return {};}
 b->groups_=AnimationGroupRequestBinding::bind(target,b->animation_->files());if(!b->groups_ || !b->alias(0x8041fd6c))return {};
 return b;
}
FieldNpcCreationBinding::~FieldNpcCreationBinding(){
 if(caller_ && !completed_)quarantined_=true; // No cancellation/rollback.
 auto& target=animation_->process().vm().memory;
 for(auto i=aliases_.rbegin();i!=aliases_.rend();++i){const bool ok=target.unregisterExternalRam(i->address);assert(ok);(void)ok;}
}
bool FieldNpcCreationBinding::text(uint32_t a,std::string& out,unsigned limit) const{
 out.clear();for(unsigned n=0;n<limit;++n){uint8_t v=0;if(uint64_t(a)+n>UINT32_MAX || !vm_.memory.readByte(a+n,v))return false;if(!v)return true;out+=char(v);}return false;
}
bool FieldNpcCreationBinding::alias(uint32_t a){
 std::string bytes;if(!text(a,bytes,64))return false;auto& target=animation_->process().vm().memory;
 if(&target==&vm_.memory)return true;
 for(const auto& view:aliases_)if(view.address==a)return view.size==bytes.size()+1 && view.bytes==bytes && vm_.memory.sharesRamWith(target,a,view.size);
 const auto size=uint32_t(bytes.size()+1);if(!vm_.memory.shareRamTo(target,a,size))return false;aliases_.push_back({a,size,std::move(bytes)});return true;
}
bool FieldNpcCreationBinding::caller(const EventEntry& e) const{
 uint32_t gp=0,mode=0,field=0;if(!vm_.memory.readWord(0x80415fd0,gp) || gp!=gp_ || !vm_.memory.readWord(gp+0x14,mode) || mode || !vm_.memory.readWord(gp+0x168,field) || field!=owner_->fieldWorkAddress())return false;
 if(vm_.field.capacity<0 || vm_.field.capacity>256 || (vm_.field.capacity && !vm_.field.entries))return false;
 for(int n=0;n<vm_.field.capacity;++n)if(&e==&vm_.field.entries[n] && (e.flags&1))return true;return false;
}
bool FieldNpcCreationBinding::current() const{
 if(quarantined_ || !owner_->aliases(vm_.memory) || !owner_->aliases(animation_->process().vm().memory))return false;
 if(owner_->mutationOwner_ && owner_->mutationOwner_!=this)return false;
 if(owner_->pendingEntry_ && owner_->pendingEntry_!=entry_.state)return false;
 auto& target=animation_->process().vm().memory;
 for(const auto& a:aliases_){std::string bytes;if(!text(a.address,bytes,64) || bytes!=a.bytes || !vm_.memory.sharesRamWith(target,a.address,a.size))return false;}
 return !caller_ || (caller(caller_[0]) && caller_->id==id_);
}
std::optional<uint32_t> FieldNpcCreationBinding::tribe(uint32_t name){
 std::string wanted;if(!text(name,wanted,64))return {};
 for(unsigned n=0;n<328;++n){const auto row=0x803316d0+0x54*n;uint32_t p=0;if(!vm_.memory.readWord(row,p))return {};if(!p)return {};std::string candidate;if(!text(p,candidate,64))return {};if(candidate!=wanted)continue;
  // Whole stable byte/string domain before the first actor store. Source
  // still reloads every reached descriptor/pointer after this admission.
  uint8_t byte=0;for(unsigned i=0;i<0x54;++i)if(!vm_.memory.readByte(row+i,byte))return {};
  for(unsigned off:{8u,0x10u,0x14u}){uint32_t str=0;if(!vm_.memory.readWord(row+off,str))return {};if(str){std::string value;if(!text(str,value,32))return {};}}
  uint32_t zero=0;if(!vm_.memory.readWord(0x8041fd84,zero))return {};
  if(owner_->entryPending() || animation_->entryPending() || animation_->producerPending())return {};owner_->mutationOwner_=this;return row;
 }
 return {};
}
FieldNpcCreationEventServices FieldNpcCreationBinding::services(){
 FieldNpcCreationEventServices s;s.user=this;
 s.word=[](void* p,uint32_t a,uint32_t& v){return static_cast<FieldNpcCreationBinding*>(p)->vm_.memory.readWord(a,v);};
 s.half=[](void* p,uint32_t a,uint16_t& v){return static_cast<FieldNpcCreationBinding*>(p)->vm_.memory.readHalf(a,v);};
 s.value=[](void* p,EventEntry& e,int32_t raw,int32_t& v){Operands o{static_cast<FieldNpcCreationBinding*>(p)->vm_};v=getEventValue(e,raw,o.services());return o.valid;};
 s.resolve=[](void* p,EventEntry& e,uint32_t name){return static_cast<FieldNpcCreationBinding*>(p)->setup_.resolveEventActor(e,name,true);};
 s.tribe=[](void* p,uint32_t name){return static_cast<FieldNpcCreationBinding*>(p)->tribe(name);};
 s.async=[](void* p,uint32_t name,uint32_t mode)->std::optional<uint32_t>{auto& b=*static_cast<FieldNpcCreationBinding*>(p);if(!b.alias(name))return {};return b.groups_->advance(name,mode,0,b.groupBudget_);};
 s.enter=[](void* p,uint32_t name,uint32_t model)->std::optional<int32_t>{auto& b=*static_cast<FieldNpcCreationBinding*>(p);BattlePoseClock clock;if(!b.animation_->clock(clock))return {};return b.registry_.resumeEnter(name,model,clock,b.entry_,true);};
 s.copy=[](void* p,uint32_t to,uint32_t from){auto& b=*static_cast<FieldNpcCreationBinding*>(p);std::string bytes;if(!b.text(from,bytes,32))return false;const uint64_t end=uint64_t(to)+bytes.size()+1;if(uint64_t(from)<end && uint64_t(to)<uint64_t(from)+bytes.size()+1)return false;uint8_t old=0;for(unsigned i=0;i<=bytes.size();++i)if(!b.vm_.memory.readByte(to+i,old))return false;for(unsigned i=0;i<=bytes.size();++i){const bool ok=b.vm_.memory.writeByte(to+i,i<bytes.size()?uint8_t(bytes[i]):0);assert(ok);(void)ok;}return true;};
 s.animation=[](void* p,uint32_t h,uint32_t name,uint32_t force){auto& b=*static_cast<FieldNpcCreationBinding*>(p);return b.animation_->process().setAnimation(h,name,force,b.selectionBudget_);};
 s.store=[](void* p,uint32_t a,uint32_t v){const bool ok=static_cast<FieldNpcCreationBinding*>(p)->vm_.memory.writeWord(a,v);assert(ok);(void)ok;};
 s.storeHalf=[](void* p,uint32_t a,uint16_t v){const bool ok=static_cast<FieldNpcCreationBinding*>(p)->vm_.memory.writeHalf(a,v);assert(ok);(void)ok;};return s;
}
void FieldNpcCreationBinding::finish(){if(owner_->mutationOwner_==this)owner_->mutationOwner_=nullptr;}
std::optional<int32_t> FieldNpcCreationBinding::invoke(uint32_t function,EventEntry& e){
 if(!fieldNpcCreationEventFunction(function) || !current() || !caller(e))return {};
 if(!caller_){if(owner_->entryPending())return {};caller_=&e;id_=e.id;function_=function;}
 if(caller_!=&e || id_!=e.id || function_!=function)return {};
 auto result=completed_?completed_:fieldNpcCreationEvent(function,e,services(),cursor_);
 if(!result)return {};finish();caller_=nullptr;completed_.reset();entry_={};return result;
}
bool FieldNpcCreationBinding::resume(){
 if(!caller_)return !quarantined_;if(!current())return false;if(completed_)return true;
 auto r=fieldNpcCreationEvent(function_,*caller_,services(),cursor_);if(!r)return false;finish();completed_=r;return true;
}
void FieldNpcCreationBinding::retire(const EventEntry& e){if(caller_==&e && id_==e.id){quarantined_=true;}}
}
