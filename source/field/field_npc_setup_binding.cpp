#include "field_npc_setup_binding.hpp"
#include "field_battle_information.hpp"
#include "event_native_runtime.hpp"
#include "camera_manager.hpp"
#include <cmath>
#include <algorithm>
#include <cassert>
namespace ttyd {
namespace {int32_t signedWord(uint32_t v){return v<=0x7fffffff?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}}
bool admitOriginalNpcSetupScript(const EventScriptAssets& source,EventAddressSpace& memory,uint32_t address){
 if(address&3)return false;
 unsigned labels=0;
 for(unsigned words=0;words<65536;){uint32_t command=0,live=0;
  if(!source.readWord(address,command) || !memory.readWord(address,live) || live!=command)return false;
  uint32_t operand=0,liveOperand=0;
  if(uint64_t(address)+4>UINT32_MAX || !source.readWord(address+4,operand) || !memory.readWord(address+4,liveOperand) || operand!=liveOperand)return false;
  const auto op=command&0xffff,count=command>>16;
  if(count>=0x8000 || count+1>65536-words || uint64_t(address)+4*(count+1)>0x100000000ULL)return false;
  for(unsigned n=1;n<=count;++n){uint32_t a=0,b=0;if(!source.readWord(address+4*n,a) || !memory.readWord(address+4*n,b) || a!=b)return false;}
  if(op==1)return true;
  if(op==3 && (++labels>16 || count<1))return false;
  address+=4*(count+1);words+=count+1;
 }
 return false;
}
FieldNpcSetupBinding::FieldNpcSetupBinding(std::shared_ptr<FieldNpcRegistryOwner> owner,FieldNpcRegistryBinding& binding,EventNativeRuntime& vm,std::shared_ptr<const EventScriptAssets> source,const FieldNpcSetupNativeServices& services,uint32_t mode):owner_(std::move(owner)),binding_(binding),vm_(vm),source_(std::move(source)),services_(services),mode_(mode){}
std::unique_ptr<FieldNpcSetupBinding> FieldNpcSetupBinding::bind(std::shared_ptr<FieldNpcRegistryOwner> owner,FieldNpcRegistryBinding& binding,EventNativeRuntime& vm,std::shared_ptr<const EventScriptAssets> source,const FieldNpcSetupNativeServices& services){
 if(!owner || !owner->initialized() || binding.owner_!=owner || &binding.memory_!=&vm.memory || !source || !services.clock || !services.admitBattle || !services.random)return {};
 uint32_t gp=0,mode=0,field=0;
 if(!vm.memory.readWord(0x80415fd0,gp) || uint64_t(gp)+0x16b>UINT32_MAX || !vm.memory.readWord(gp+0x14,mode) || !vm.memory.readWord(gp+0x168,field) || field!=owner->fieldWorkAddress())return {};
 return std::unique_ptr<FieldNpcSetupBinding>(new FieldNpcSetupBinding(std::move(owner),binding,vm,std::move(source),services,mode));
}
uint32_t FieldNpcSetupBinding::word(uint32_t a) const{uint32_t v=0;const bool ok=vm_.memory.readWord(a,v);assert(ok);(void)ok;return v;}
bool FieldNpcSetupBinding::readable(uint32_t a,unsigned size) const{if(uint64_t(a)+size>0x100000000ULL)return false;uint8_t v=0;for(unsigned n=0;n<size;++n)if(!vm_.memory.readByte(a+n,v))return false;return true;}
bool FieldNpcSetupBinding::text(uint32_t a,std::string& out,unsigned limit) const{out.clear();for(unsigned n=0;n<limit;++n){uint8_t v=0;if(uint64_t(a)+n>UINT32_MAX || !vm_.memory.readByte(a+n,v))return false;if(!v)return true;out.push_back(char(v));}return false;}
bool FieldNpcSetupBinding::safeDelete(uint32_t id,const EventEntry& caller) const{
 if(!id)return true;
 const auto& context=mode_?vm_.battle:vm_.field;
 if(context.capacity<0 || context.capacity>256 || (context.capacity && !context.entries))return false;
 std::vector<const EventEntry*> pending;std::vector<bool> seen(unsigned(context.capacity));
 for(int n=0;n<context.capacity;++n)if((context.entries[n].flags&1) && context.entries[n].id==id)pending.push_back(&context.entries[n]);
 while(!pending.empty()){const auto* e=pending.back();pending.pop_back();int index=-1;
  for(int n=0;n<context.capacity;++n)if(e==&context.entries[n]){index=n;break;}
  if(index<0 || e==&caller || e->unresolvedLinkMask)return false;
  if(seen[unsigned(index)])return false; // Exclude cyclic/duplicated deletion graphs.
  seen[unsigned(index)]=true;
  if(e->waitingParent){bool valid=false;for(int n=0;n<context.capacity;++n)valid|=e->waitingParent==&context.entries[n];if(!valid)return false;}
  if(e->waitingChild)pending.push_back(e->waitingChild);
  for(int n=0;n<context.capacity;++n)if((context.entries[n].flags&1) && context.entries[n].brotherOwner==e)pending.push_back(&context.entries[n]);
 }
 return true;
}
bool FieldNpcSetupBinding::admit(const EventEntry& caller,uint32_t table,bool first){
 uint32_t gp=0,mode=0,field=0;if(!vm_.memory.readWord(0x80415fd0,gp) || uint64_t(gp)+0x16b>UINT32_MAX || !vm_.memory.readWord(gp+0x14,mode) || mode!=mode_ || !vm_.memory.readWord(gp+0x168,field) || field!=owner_->fieldWorkAddress())return false;
 uint16_t fieldMode=0;if(!vm_.memory.readHalf(field,fieldMode) || !binding_.activeActors())return false;
 const auto& context=mode_?vm_.battle:vm_.field;bool registered=false;
 for(int n=0;n<context.capacity;++n)registered|=&caller==&context.entries[n] && (caller.flags&1);
 if(!registered)return false;
 auto actorStorage=[&](uint32_t a){return (a>=owner_->addresses_[0] && (a-owner_->addresses_[0])%0x340==0 && uint64_t(a)-owner_->addresses_[0]<36*0x340u) || (a>=owner_->addresses_[2] && (a-owner_->addresses_[2])%0x340==0 && uint64_t(a)-owner_->addresses_[2]<16*0x340u);};
 for(unsigned n=0;n<4096;++n){const uint64_t at=uint64_t(table)+uint64_t(n)*0x5c;if(at+4>0x100000000ULL || !readable(uint32_t(at),4))return false;const auto row=uint32_t(at),name=word(row);uint32_t original=0;
  if(!source_->readWord(row,original) || original!=name)return false;
  if(!name)return true;
  for(unsigned k=4;k<0x5c;k+=4){uint32_t original=0,live=0;if(!source_->readWord(row+k,original) || !vm_.memory.readWord(row+k,live) || original!=live)return false;}
  std::string textName;if(!text(name,textName,32))return false;const auto actor=binding_.lookup(name);if(!actor)return false;if(!*actor){if(first)continue;return false;}
  const auto a=*actor;if(!actorStorage(a) || !readable(a,0x340))return false;
  if(first){if(!readable(row,0x5c))return false;const auto descriptor=word(a+0x28),handle=word(a+0x104);if(!readable(descriptor,0x18) || handle>=256)return false;const auto* pose=owner_->poses().poses().pose(int32_t(handle));if(!pose)return false;
   for(unsigned off:{8u,0x10u,0x14u}){const auto pointer=word(descriptor+off);if(!pointer)continue;std::string name;if(!text(pointer,name,32))return false;if(off==8 && !owner_->poses().animationReady(pose->resource->modelName,name))return false;}
   const auto init=word(row+12);if(init && (!admitOriginalNpcSetupScript(*source_,vm_.memory,init) || !safeDelete(word(a+0x118),caller)))return false;
   const auto index=signedWord(word(row+0x58));if(index!=-1 && (!services_.admitBattle || !services_.random || !services_.admitBattle(services_.user,vm_.memory,a,index)))return false;
   for(unsigned k=0;k<4;++k){const auto child=word(a+0x330+4*k);if(child && !actorStorage(child))return false;}
  }else{
   // A genuine poll returns at its first live init. Admit only that reached
   // read domain rather than rescanning every regular script each wait tick.
   const auto init=word(a+0x118);
   if(init)for(int k=0;k<context.capacity;++k)if((context.entries[k].flags&1) && context.entries[k].id==init)return true;
   const auto regular=word(a+0x124);if(regular && (!admitOriginalNpcSetupScript(*source_,vm_.memory,regular) || !safeDelete(word(a+0x11c),caller)))return false;}
 }
 return false;
}
FieldNpcSetupWorld FieldNpcSetupBinding::world(){
 FieldNpcSetupWorld w;w.user=this;
 w.fieldPointer=[](void* p){auto& b=*static_cast<FieldNpcSetupBinding*>(p);return b.word(b.word(0x80415fd0)+0x168);};
 w.admit=[](void* p,const EventEntry& e,uint32_t table,bool first,const FieldNpcSetupOperands&){return static_cast<FieldNpcSetupBinding*>(p)->admit(e,table,first);};
 w.npc=[](void* p,uint32_t name,bool asserting,const FieldNpcSetupOperands&){auto& b=*static_cast<FieldNpcSetupBinding*>(p);const auto a=b.binding_.lookup(name,asserting?PackedNpcLookup::Release:PackedNpcLookup::NoAssert);assert(a);return *a;};
 w.word=[](void* p,uint32_t a){return static_cast<FieldNpcSetupBinding*>(p)->word(a);};
 w.store=[](void* p,uint32_t a,uint32_t v){const bool ok=static_cast<FieldNpcSetupBinding*>(p)->vm_.memory.writeWord(a,v);assert(ok);(void)ok;};
 w.copyString=[](void* p,uint32_t to,uint32_t from){auto& m=static_cast<FieldNpcSetupBinding*>(p)->vm_.memory;uint8_t v=0;do{const bool ok=m.readByte(from++,v) && m.writeByte(to++,v);assert(ok);(void)ok;}while(v);};
 w.setAnimation=[](void* p,uint32_t h,uint32_t a,int32_t mode){auto& b=*static_cast<FieldNpcSetupBinding*>(p);std::string name;const bool ok=b.text(a,name,32) && mode==1 && b.owner_->poses().selectName(int32_t(h),name,true,b.services_.clock(b.services_.user));assert(ok);(void)ok;};
 w.mode=[](void* p,int32_t mode){auto& b=*static_cast<FieldNpcSetupBinding*>(p);const auto field=b.word(b.word(0x80415fd0)+0x168);const bool ok=b.vm_.memory.writeHalf(field,uint16_t(mode));assert(ok);(void)ok;};
 w.battle=[](void* p,uint32_t actor,uint32_t setup){auto& b=*static_cast<FieldNpcSetupBinding*>(p);PackedNpcBattleSetupServices s;s.memory.memory.user=&b.vm_.memory;
  s.memory.memory.word=[](void* p,uint32_t a){uint32_t v=0;const bool ok=static_cast<EventAddressSpace*>(p)->readWord(a,v);assert(ok);(void)ok;return v;};
  s.memory.memory.half=[](void* p,uint32_t a){uint16_t v=0;const bool ok=static_cast<EventAddressSpace*>(p)->readHalf(a,v);assert(ok);(void)ok;return v;};
  s.memory.memory.storeWord=[](void* p,uint32_t a,uint32_t v){const bool ok=static_cast<EventAddressSpace*>(p)->writeWord(a,v);assert(ok);(void)ok;};
  s.memory.memory.storeByte=[](void* p,uint32_t a,uint8_t v){const bool ok=static_cast<EventAddressSpace*>(p)->writeByte(a,v);assert(ok);(void)ok;};
  s.memory.user=&b.vm_.memory;s.memory.byte=[](void* p,uint32_t a){uint8_t v=0;const bool ok=static_cast<EventAddressSpace*>(p)->readByte(a,v);assert(ok);(void)ok;return v;};s.user=b.services_.user;s.random=b.services_.random;s.savedFlag=b.services_.savedFlag;setPackedNpcBattleInformation(actor,signedWord(setup),s);
 };return w;
}
int32_t FieldNpcSetupBinding::invoke(EventEntry& e,bool first,FieldNpcSetupCursor& c,const FieldNpcSetupOperands& o,const FieldNpcSetupEvents& events){
 if(first)c={};
 c.firstVisit(first);
 uint32_t gp=0,mode=0,field=0;if(!vm_.memory.readWord(0x80415fd0,gp) || uint64_t(gp)+0x16b>UINT32_MAX || !vm_.memory.readWord(gp+0x14,mode) || mode!=mode_ || !vm_.memory.readWord(gp+0x168,field) || field!=owner_->fieldWorkAddress())return 0;
 return fieldNpcSetupEvent(e,first,c,o,world(),events);
}
}

namespace ttyd {
namespace {
// Execute the existing operand router once with checked reads and a deferred
// output word. A failed request publishes nothing; successful outputs retain
// the source setter's quantization and namespace asymmetry.
struct ActorOperands {
 EventNativeRuntime& vm;bool valid=true;
 bool hasWrite=false;uint32_t destination=0,value=0;
 explicit ActorOperands(EventNativeRuntime& v):vm(v){}
 EventOperandServices services(){
  EventOperandServices s;s.user=this;
  s.contextAddress=[](void* p){auto& a=*static_cast<ActorOperands*>(p);const auto* g=a.vm.gpAddresses.currentGlobals();if(!g){a.valid=false;return uint32_t(0);}return uint32_t(g->battleContext?0x803d8dd0:0x803d8d30);};
  s.entryAddress=[](void* p,EventEntry& e){auto& a=*static_cast<ActorOperands*>(p);uint32_t address=0;if(!a.vm.entryAddresses.addressOf(&e,address))a.valid=false;return address;};
  s.globalAddress=[](void* p){auto& a=*static_cast<ActorOperands*>(p);if(!a.vm.gpAddresses.currentGlobals())a.valid=false;return a.vm.gpAddresses.currentAddress();};
  s.readWord=[](void* p,uint32_t address){auto& a=*static_cast<ActorOperands*>(p);uint32_t v=0;if(!a.vm.memory.readWord(address,v))a.valid=false;return v;};
  s.readByte=[](void* p,uint32_t address){auto& a=*static_cast<ActorOperands*>(p);uint8_t v=0;if(!a.vm.memory.readByte(address,v))a.valid=false;return v;};
  s.writeWord=[](void* p,uint32_t address,uint32_t value){auto& a=*static_cast<ActorOperands*>(p);assert(!a.hasWrite);a.hasWrite=true;a.destination=address;a.value=value;};
  s.truncate=[](void* p,float f)->int32_t{auto& a=*static_cast<ActorOperands*>(p);if(!a.vm.platform.truncate || !std::isfinite(f) || double(f)<-2147483648. || double(f)>=2147483648.){a.valid=false;return 0;}return a.vm.platform.truncate(a.vm.platform.user,f);};
  return s;
 }
 bool publish(){if(!valid)return false;return !hasWrite || vm.memory.writeWord(destination,value);}
};
}
int32_t FieldNpcSetupBinding::invokeActorEvent(uint32_t fn,EventEntry& e,bool first,FieldNpcActorEventCursor& cursor){
 if(first)cursor={};
 uint32_t gp=0,mode=0;if(!vm_.memory.readWord(0x80415fd0,gp) || uint64_t(gp)+0x17>UINT32_MAX || !vm_.memory.readWord(gp+0x14,mode) || mode!=mode_)return 0;
 const auto& context=mode?vm_.battle:vm_.field;if(context.capacity<0 || context.capacity>256 || (context.capacity && !context.entries))return 0;bool registered=false;
 for(int n=0;n<context.capacity;++n)registered|=&e==&context.entries[n] && (e.flags&1);
 if(!registered)return 0;
 FieldNpcActorEventServices s;s.user=this;
 s.word=[](void* p,uint32_t a,uint32_t& v){return static_cast<FieldNpcSetupBinding*>(p)->vm_.memory.readWord(a,v);};
 s.value=[](void* p,EventEntry& e,int32_t raw,int32_t& v){ActorOperands a(static_cast<FieldNpcSetupBinding*>(p)->vm_);v=getEventValue(e,raw,a.services());return a.valid;};
 s.real=[](void* p,EventEntry& e,int32_t raw,float& v){ActorOperands a(static_cast<FieldNpcSetupBinding*>(p)->vm_);v=getEventFloat(e,raw,a.services());return a.valid;};
 s.setReal=[](void* p,EventEntry& e,int32_t raw,float v){ActorOperands a(static_cast<FieldNpcSetupBinding*>(p)->vm_);setEventFloat(e,raw,v,a.services());return a.publish();};
 s.resolve=[](void* p,EventEntry& e,uint32_t name,bool dereference)->std::optional<uint32_t>{
  auto& b=*static_cast<FieldNpcSetupBinding*>(p);std::string text;if(!b.text(name,text,32))return {};
  auto owned=[&](uint32_t a){for(unsigned bank:{0u,2u})if(a>=b.owner_->addresses_[bank] && (a-b.owner_->addresses_[bank])%0x340==0 && uint64_t(a)-b.owner_->addresses_[bank]<uint64_t(bank?16:36)*0x340)return b.owner_->span(a,0x340)!=nullptr;return false;};
  uint32_t actor=0;
  if(text=="me")actor=e.field170;
  else if(text=="party" || text=="(^x^)party")actor=0xffffffff;
  else if(text=="extparty")actor=0xfffffffe;
  else if(text=="mario")actor=0xfffffffd;
  else if(text=="master" || text=="slave_0" || text=="slave_1" || text=="slave_2" || text=="slave_3"){
   const auto self=e.field170;if(!owned(self))return {};actor=b.word(self+(text=="master"?0x32c:0x330+4*unsigned(text.back()-'0')));
  }else {const auto found=b.binding_.lookup(name,PackedNpcLookup::Release);if(!found)return {};actor=*found;}
  if(dereference && !owned(actor))return {};
  return actor;
 };
 s.store=[](void* p,uint32_t a,uint32_t v){const bool ok=static_cast<FieldNpcSetupBinding*>(p)->vm_.memory.writeWord(a,v);assert(ok);(void)ok;};
 s.cameraHeading=[](void* p,uint32_t index)->std::optional<float>{auto& b=*static_cast<FieldNpcSetupBinding*>(p);const auto* camera=b.services_.cameras?b.services_.cameras->camera(index):nullptr;if(!camera || !camera->viewPublished)return {};return camera->yaw;};
 return fieldNpcActorEvent(fn,e,first,cursor,s);
}
}
