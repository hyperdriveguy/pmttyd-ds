#include "field_area_event.hpp"
#include "animation_group_prefetch.hpp"
#include "field_door_data.hpp"
#include "field_door_parameter.hpp"
#include "field_map_flags.hpp"
#include "field_bero_info.hpp"
#include "field_bero_exec.hpp"
#include "event_native_runtime.hpp"
#include "battle_event_callbacks.hpp"
#include "field_battle_information.hpp"
#include "event_memory_bootstrap.hpp"
#include "event_lifecycle.hpp"
#include <cassert>
#include <map>
#include <cmath>
#include <cstdio>
#include <cstring>
namespace ttyd {
namespace {
struct DoorProgram {uint32_t room,script,record;};
struct BattleRegistration {uint32_t room,name,setup,numbers,first,count;};
struct Program {EventScriptAssets scripts;std::vector<BattleRegistration> battles;std::vector<DoorProgram> entries;std::vector<std::array<uint32_t,2>> initializers;};
std::shared_ptr<const Program> assets;
int32_t signedWord(uint32_t v){return v<=INT32_MAX?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}
bool battleNumbersReadable(const EventScriptAssets& scripts,uint32_t table){
 for(unsigned i=0;i<256;++i){const uint64_t entry=uint64_t(table)+8*i;uint32_t name=0,value=0;
  if(entry+8>0x100000000ULL || !scripts.readWord(uint32_t(entry),name) || !scripts.readWord(uint32_t(entry)+4,value))return false;
  if(!name)return true;
  bool ended=false;for(unsigned j=0;j<64;++j){uint8_t byte=0;if(uint64_t(name)+j>=0x100000000ULL || !scripts.readByte(name+j,byte))return false;if(!byte){ended=true;break;}}
  if(!ended)return false;
 }
 return false;
}
uint32_t bits(float v){uint32_t out;std::memcpy(&out,&v,4);return out;}
}
bool loadFieldAreaEventAssets(const char* root) {
 if(!root)return false;
 auto next=std::make_shared<Program>();const std::string base=root;
 if(!next->scripts.load((base+"/door-data.pmevt").c_str()))return false;
 FILE* f=std::fopen((base+"/field-area-events.bin").c_str(),"rb");if(!f)return false;
 char magic[8];uint32_t count=0;
 bool ok=std::fread(magic,8,1,f)==1 && !std::memcmp(magic,"PMFAEV02",8) && std::fread(&count,4,1,f)==1 && count && count<=256;
 for(unsigned n=0;ok && n<count;++n){DoorProgram e{};uint32_t word=0;ok=std::fread(&e,sizeof(e),1,f)==1 && e.room<=65535 && !(e.script&3) && !(e.record&3) && next->scripts.readWord(e.script,word) && (word==0x0005005b || word==0x0006005b);
  for(unsigned k=0;ok && k<18;++k)ok=next->scripts.readWord(e.record+4*k,word);
  if(e.script==0x8033156c)ok=ok && next->scripts.readWord(0x803316a0,word) && word==0x800e8288;
  if(ok)next->entries.push_back(e);
 }
 ok=ok && std::fgetc(f)==EOF;std::fclose(f);if(!ok)return false;
 f=std::fopen((base+"/field-area-init.bin").c_str(),"rb");if(!f)return false;
 ok=std::fread(magic,8,1,f)==1 && !std::memcmp(magic,"PMFAIN01",8) && std::fread(&count,4,1,f)==1 && count && count<=256;
 for(unsigned n=0;ok && n<count;++n){std::array<uint32_t,2> e{};uint32_t word=0;ok=std::fread(e.data(),sizeof(e),1,f)==1 && e[0]<=65535 && !(e[1]&3) && next->scripts.readWord(e[1],word) && word==0x00010022;
  for(const auto& prior:next->initializers)if(prior[0]==e[0])ok=false;
  if(ok)next->initializers.push_back(e);
 }
 ok=ok && std::fgetc(f)==EOF;std::fclose(f);if(!ok)return false;
 // Validate the original registry before publishing the candidate assets.
 for(unsigned i=0;i<=33;++i){uint32_t record=0,name=0,a=0,b=0;uint8_t c=0;
  if(!next->scripts.readWord(0x803057ec+4*i,record))return false;
  if(i==33){if(record)return false;break;}
  if(!record || !next->scripts.readWord(record,name) || !name || !next->scripts.readWord(record+8,a) || !next->scripts.readWord(record+12,b) || a || b)return false;
  for(unsigned j=0;j<4;++j)if(!next->scripts.readByte(name+j,c) || (j<3 ? c<'a' || c>'z' : c!=0))return false;
 }
 f=std::fopen((base+"/field-area-battle.bin").c_str(),"rb");if(!f)return false;
 ok=std::fread(magic,8,1,f)==1 && !std::memcmp(magic,"PMFABT01",8) && std::fread(&count,4,1,f)==1 && count && count<=256;
 for(unsigned i=0;ok && i<count;++i){BattleRegistration e{};uint32_t word=0;uint8_t byte=0;char name[4]{};
  ok=std::fread(&e,sizeof(e),1,f)==1 && e.room<=65535 && !(e.name&3) && !(e.setup&3) && !(e.numbers&3) && e.count && uint64_t(e.first)+e.count<=0x80000000ULL;
  for(unsigned j=0;ok && j<4;++j){ok=next->scripts.readByte(e.name+j,byte) && (j<3 ? byte>='a' && byte<='z' : byte==0);name[j]=char(byte);}
  bool found=false;
  for(unsigned j=0;ok && j<33;++j){uint32_t record=0,text=0;ok=next->scripts.readWord(0x803057ec+4*j,record) && next->scripts.readWord(record,text);bool equal=true;
   for(unsigned k=0;ok && k<4;++k){ok=next->scripts.readByte(text+k,byte);equal=equal && byte==uint8_t(name[k]);}if(equal)found=true;
  }
  ok=ok && found && battleNumbersReadable(next->scripts,e.numbers);
  // Native finite-profile admission; count does not assert the source table's full size.
  for(uint64_t j=0;ok && j<e.count;++j){const uint64_t row=uint64_t(e.setup)+0x44*(e.first+j);ok=row+0x44<=0x100000000ULL && next->scripts.readWord(uint32_t(row),word) && word;
   for(unsigned k=4;ok && k<0x44;k+=4)ok=next->scripts.readWord(uint32_t(row)+k,word);
  }
  for(const auto& prior:next->battles)if(prior.room==e.room)ok=false;
  if(ok)next->battles.push_back(e);
 }
 ok=ok && std::fgetc(f)==EOF;std::fclose(f);if(!ok)return false;
 assets=std::move(next);return true;
}
struct FieldAreaEventRuntime {
 FieldAreaEvent* owner;
 std::shared_ptr<const Program> program=assets;
 FieldAreaEventServices services;
 EventNativeRuntime vm;
 FieldCasePool cases;
 uint32_t room=UINT32_MAX,initializer=0;
 std::optional<uint32_t> initId;
 std::vector<DoorProgram> entries;
 struct Root {uint32_t id,entryIndex;std::optional<std::array<int32_t,16>> bank;bool returned=false;};
 std::vector<Root> roots;
 std::map<uint32_t,FieldPreviousPartyCursor> previousParty; // Real event IDs; lifetime is source elapsed time, not a generation.
 std::map<uint32_t,FieldNpcSetupCursor> npcSetup;
 explicit FieldAreaEventRuntime(FieldAreaEvent* p):owner(p),vm(0x81000000,platform(this)){}
 static EventNativePlatform platform(FieldAreaEventRuntime* r) {
  EventNativePlatform p;p.user=r;p.clock=[](void*)->uint32_t{return 243000000;};
  p.truncate=[](void*,float v){assert(std::isfinite(v) && double(v)>=INT32_MIN && double(v)<2147483648.);return int32_t(v);};
  p.divideWord=[](void*,int32_t a,int32_t b){assert(b);return a/b;};
  p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){};
  p.format=[](void* p,const EventDiagnosticCall&){static_cast<FieldAreaEventRuntime*>(p)->owner->failed=true;};
  p.retiring=[](void* p,const EventEntry& e){auto& r=*static_cast<FieldAreaEventRuntime*>(p);r.previousParty.erase(e.id);r.npcSetup.erase(e.id);for(auto& root:r.roots)if(root.id==e.id){root.bank=e.locals;root.returned=e.opcode==2;}};
  p.call=[](void* p,EventEntry& e,uint32_t fn,int32_t first){auto& r=*static_cast<FieldAreaEventRuntime*>(p);const auto result=r.call(e,fn,first!=0);if(result==0){r.owner->pendingFunction=fn;r.owner->pendingEventId=e.id;}return result;};return p;
 }
 uint32_t word(uint32_t a){uint32_t v=0;if(!vm.memory.readWord(a,v))owner->failed=true;return v;}
 int32_t value(EventEntry& e,unsigned n){return getEventValue(e,signedWord(word(e.arguments+4*n)),vm.interpreterServices().operands);}
 bool text(uint32_t address,std::string& out) {
  out.clear();for(unsigned n=0;n<64;++n){uint8_t b=0;if(!vm.memory.readByte(address+n,b))return false;if(!b)return true;out.push_back(char(b));}return false;
 }
 int32_t call(EventEntry& e,uint32_t fn,bool first) {
  // The source first-call reset belongs to the event even when a world
  // provider is not yet installed; USER_FUNC will retain first=false later.
  if(fn==0x8007a154 && first)e.waitCount=0;
  if(fn==0x8007a154 && services.prefetch) {
   AnimationGroupPollServices s;s.user=services.prefetch;s.poll=[](void* p,const char* name){return static_cast<AnimationGroupPrefetch*>(p)->poll(name);};
   return fieldBeroReadMarioPera(e,false,s);
  }
  if(fn==0x80054f3c) {
   // Capture args and GP before reading the fresh raw destination, then GP word0.
   const auto args=e.arguments,gp=vm.gpAddresses.currentAddress();uint32_t raw=0,flags=0;
   if(!vm.memory.readWord(args,raw) || !vm.memory.readWord(gp,flags))return 0;
   setEventValue(e,signedWord(raw),signedWord(flags),vm.interpreterServices().operands);return 2;
  }
  if(fn==0x8007aa04) {
   const auto args=e.arguments;if(!services.beroExec)return 0;
   const auto bits=services.beroExec->read();if(!bits)return 0;
   uint32_t raw=0;if(!vm.memory.readWord(args,raw))return 0;
   setEventValue(e,signedWord(raw),signedWord(*bits),vm.interpreterServices().operands);return 2;
  }
  if(fn==0x8007aa34) {
   if(!services.beroExec)return 0;
   FieldBeroExecEventServices s;s.user=this;
   s.word=[](void* p,uint32_t a){return static_cast<FieldAreaEventRuntime*>(p)->word(a);};
   s.value=[](void* p,EventEntry& e,int32_t raw){auto& r=*static_cast<FieldAreaEventRuntime*>(p);return getEventValue(e,raw,r.vm.interpreterServices().operands);};
   s.owner=[](void* p){return static_cast<FieldAreaEventRuntime*>(p)->services.beroExec;};
   return fieldBeroExecOnOffEvent(e,s);
  }
  if(fn==0x800e7474 && services.mario.admit) {
   FieldMarioPreparationOperands o;o.user=this;
   o.word=[](void* p,uint32_t a){return static_cast<FieldAreaEventRuntime*>(p)->word(a);};
   o.value=[](void* p,EventEntry& e,int32_t raw){auto& r=*static_cast<FieldAreaEventRuntime*>(p);return getEventValue(e,raw,r.vm.interpreterServices().operands);};
   return fieldMarioKeyEvent(e,o,services.mario);
  }
  if((fn==0x800e73ec || fn==0x800e7410) && services.mario.admit)return fieldMarioBackgroundEvent(fn==0x800e7410,services.mario);
  if(fn==0x800e7314 && services.mario.admit)return fieldMarioCameraYOffEvent(services.mario);
  if(fn==0x800e3058) {
   if(first)previousParty.erase(e.id); // Source restart preserves ID but starts a new callback invocation.
   auto& cursor=previousParty[e.id];cursor.firstVisit(first);
   if(services.previousParty.mario) {
    const auto result=fieldPreviousPartyEvent(e,first,cursor,this,[](void* p,uint32_t a){return static_cast<FieldAreaEventRuntime*>(p)->word(a);},services.previousParty);
    if(result==2)previousParty.erase(e.id);
    return result;
   }
  }
  if(fn==0x8005323c) {
   if(first)npcSetup.erase(e.id);
   auto& cursor=npcSetup[e.id];cursor.firstVisit(first);
   if(services.npcSetup.fieldPointer) {
    FieldNpcSetupOperands o;o.user=this;
    o.word=[](void* p,uint32_t a){return static_cast<FieldAreaEventRuntime*>(p)->word(a);};
    o.value=[](void* p,EventEntry& e,int32_t raw){auto& r=*static_cast<FieldAreaEventRuntime*>(p);return getEventValue(e,raw,r.vm.interpreterServices().operands);};
    FieldNpcSetupEvents events;events.user=this;
    events.erase=[](void* p,uint32_t id){deleteEventId(id,static_cast<FieldAreaEventRuntime*>(p)->vm.creationServices());};
    events.alive=[](void* p,uint32_t id){return checkEventId(id,static_cast<FieldAreaEventRuntime*>(p)->vm.creationServices())!=0;};
    events.create=[](void* p,uint32_t script,uint8_t priority,uint8_t flags)->EventEntry* {
     auto& r=*static_cast<FieldAreaEventRuntime*>(p);const auto creation=r.vm.creationServices();
     auto* gp=creation.manager.globals(creation.manager.user);auto* context=creation.manager.context(creation.manager.user,gp->battleContext!=0);
     for(int n=0;n<context->capacity;++n)if(!(context->entries[n].flags&1))return &createRootEvent(r.vm.schedule,script,priority,flags,creation);
     return nullptr;
    };
    const auto result=fieldNpcSetupEvent(e,first,cursor,o,services.npcSetup,events);
    if(result==2)npcSetup.erase(e.id);
    return result;
   }
  }
  if(fn==0x8007a76c) {
   if(!services.beroInfo || services.beroInfo->room()!=room || !services.beroInfo->registered())return 0;
   const auto args=e.arguments;const auto operands=vm.interpreterServices().operands;
   const auto index=getEventValue(e,signedWord(word(args)),operands);const auto length=services.beroInfo->length(index);
   if(!length)return 0;
   const auto destination=signedWord(word(args+4));
   const bool work=(destination>-220000000 && destination<=-180000000) || (destination>-60000000 && destination<=-20000000);
   const float converted=*length*1024.f;
   if(work && (!std::isfinite(converted) || double(converted)<INT32_MIN || double(converted)>=2147483648.))return 0;
   setEventFloat(e,destination,*length,operands);return owner->failed?0:2;
  }
  if(fn==0x80079b74 && services.position.mario) {
   const auto& world=services.position;if(!world.write || !world.half)return 0;
   if(services.geometry.admitRoom && !services.geometry.admitRoom(services.geometry.user,room))return 0;
   const auto args=e.arguments;const auto capturedMario=world.mario(world.user);if(!capturedMario)return 0;
   const auto op=vm.interpreterServices().operands;
   const auto mode=getEventValue(e,signedWord(word(args)),op);
   const auto distance=getEventFloat(e,signedWord(word(args+4)),op);
   const auto xyzAddress=uint32_t(e.locals[6]);std::array<int32_t,3> xyz;
   xyz[0]=signedWord(word(xyzAddress));const auto nameAddress=uint32_t(e.locals[3]);const auto direction=e.locals[5];
   xyz[1]=signedWord(word(xyzAddress+4));xyz[2]=signedWord(word(xyzAddress+8));
   std::string name;if(xyz[0]==100000 && !text(nameAddress,name))return 0;
   if(owner->failed)return 0;
   const auto result=fieldBeroNormalPosition(xyz,name,direction,mode,distance,services.geometry);
   // Native finite READF admission precedes publication; it is not a source branch.
   if(!result || !fieldBeroStartPublication(result->start))return 0;
   const uint32_t addresses[]={0x8041e9f8,0x8041e9f4,0x8041e9f0,0x8041e9ec,0x8041e9e8,0x8041e9e4};
   const float values[]={result->start.x,result->start.y,result->start.z,result->finish.x,result->finish.y,result->finish.z};
   uint32_t old=0;for(auto address:addresses)if(!vm.memory.readWord(address,old))return 0;
   for(unsigned n=0;n<6;++n)if(!vm.memory.writeWord(addresses[n],bits(values[n]))){owner->failed=true;return 0;}
   const auto recoveryMario=world.mario(world.user);assert(recoveryMario);
   world.write(world.user,recoveryMario+0x2e4,bits(result->finish.x));
   world.write(world.user,recoveryMario+0x2e8,bits(result->recovery.y));
   world.write(world.user,recoveryMario+0x2ec,bits(result->finish.z));
   if(world.half(world.user,capturedMario+0x2e)==25) {
    const auto reloadMario=world.mario(world.user);assert(reloadMario);world.write(world.user,reloadMario+0x2e0,1);
   }
   return 2;
  }
  if(services.position.mario && (fn==0x800e7180 || fn==0x800eb558 || fn==0x800a785c)) {
   const auto& world=services.position;FieldPositionOperands o;o.user=this;
   o.word=[](void* p,uint32_t a){return static_cast<FieldAreaEventRuntime*>(p)->word(a);};
   o.scalar=[](void* p,EventEntry& e,int32_t raw){auto& r=*static_cast<FieldAreaEventRuntime*>(p);return getEventFloat(e,raw,r.vm.interpreterServices().operands);};
   if(fn==0x800e7180)return world.write?fieldMarioSetPositionEvent(e,o,world):0;
   if(fn==0x800eb558)return world.party?fieldPartySetPositionEvent(e,o,world):0;
   if(!world.camera || !world.read || !world.write || !world.roadReset || !world.shiftReset)return 0;
   return fieldCameraRoadResetEvent(world);
  }
  if(fn==0x8007a50c){const auto address=uint32_t(e.locals[0]);if(!services.beroInfo || !services.beroExec || !services.map || services.map->room()!=room)return 0;return services.beroInfo->registerOriginal(address,*services.map,*services.beroExec)?2:0;}
  if(fn==0x8007a214){
   if(!services.beroInfo || services.beroInfo->room()!=room || !services.beroInfo->registered())return 0;
   if(!initId)return services.beroInfo->intoInfo(e)?2:0;
   const auto gp=vm.gpAddresses.currentAddress();uint32_t flags=0;if(!vm.memory.readWord(gp,flags))return 0;
   std::string incoming;bool terminated=false;
   for(unsigned n=0;n<64;++n){uint8_t byte=0;if(!vm.memory.readByte(gp+0x11c+n,byte))return 0;if(!byte){terminated=true;break;}incoming.push_back(char(byte));}
   if(!terminated || !services.beroInfo->publishIncoming(incoming,flags))return 0;
   return services.beroInfo->intoInfo(e,gp+0x11c)?2:0;
  }
  if(fn==0x8007a63c){const auto number=value(e,0);if(!services.beroInfo || services.beroInfo->room()!=room)return 0;services.beroInfo->setCurrentNumber(number);return 2;}
  if(fn==0x800e81f4) {
   if(!services.doors)return 0;
   FieldDoorParameterServices s;s.user=this;
   s.admit=[](void* p,uint32_t record,uint32_t){auto& r=*static_cast<FieldAreaEventRuntime*>(p);uint32_t word=0;for(unsigned n=0;n<18;++n)if(!r.services.doors->readWord(record+4*n,word))return false;return true;};
   s.word=[](void* p,uint32_t a){auto& r=*static_cast<FieldAreaEventRuntime*>(p);uint32_t v=0;if(!r.services.doors->readWord(a,v) && !r.vm.memory.readWord(a,v))r.owner->failed=true;return v;};
   s.value=[](void* p,EventEntry& e,int32_t raw){auto& r=*static_cast<FieldAreaEventRuntime*>(p);return getEventValue(e,raw,r.vm.interpreterServices().operands);};
   s.setValue=[](void* p,EventEntry& e,int32_t raw,int32_t v){auto& r=*static_cast<FieldAreaEventRuntime*>(p);setEventValue(e,raw,v,r.vm.interpreterServices().operands);};
   return fieldDoorParameterEvent(e,s);
  }
  if(fn==0x80038468) {
   const auto mode=value(e,0),on=value(e,1);const auto address=uint32_t(value(e,2));const auto mask=uint32_t(value(e,3));
   std::string name;if(mode!=1 || !services.map || services.map->room()!=room || !text(address,name))return 0;
   return services.map->mapFlag(name,mask,on!=0)?2:0;
  }
  if(fn==0x80074664) {
   const auto address=uint32_t(value(e,0));const auto recurse=value(e,1),on=value(e,2);std::string name;
   if(recurse!=1 || !services.map || services.map->room()!=room || !text(address,name))return 0;
   return services.map->hitEnabled(name,on!=0)?2:0;
  }
  if(fn==0x80070110) {
   FieldCaseEventServices s;s.user=this;
   s.word=[](void* p,uint32_t a){return static_cast<FieldAreaEventRuntime*>(p)->word(a);};
   s.value=[](void* p,EventEntry& e,int32_t raw){auto& r=*static_cast<FieldAreaEventRuntime*>(p);return getEventValue(e,raw,r.vm.interpreterServices().operands);};
   s.setValue=[](void* p,EventEntry& e,int32_t raw,int32_t v){auto& r=*static_cast<FieldAreaEventRuntime*>(p);setEventValue(e,raw,v,r.vm.interpreterServices().operands);};
   s.text=[](void* p,uint32_t a,std::string& out){return static_cast<FieldAreaEventRuntime*>(p)->text(a,out);};
   s.owner=[](void* p){return &static_cast<FieldAreaEventRuntime*>(p)->cases;};return fieldRunCaseEvent(e,false,s);
  }
  if(fn==0x800e8090){const auto record=uint32_t(value(e,0));const auto hide=value(e,1);return services.hideNpcs?services.hideNpcs(services.user,record,hide!=0):0;}
  if(!services.call)return 0;
  const auto operands=vm.interpreterServices().operands;
  FieldAreaNativeCall request{e,fn,first,operands,vm.memory,vm.schedule,vm.creationServices()};const auto result=services.call(services.user,request);
  if(result!=0 && result!=1 && result!=2){owner->failed=true;return 0;}
  if(fn==0x800e3058 && result!=0)previousParty.erase(e.id);
  if(fn==0x8005323c && result!=0)npcSetup.erase(e.id);
  return result;
 }
 bool initialize(uint64_t milliseconds) {
  if(!vm.memory.registerRam(0x82000000,std::vector<uint8_t>(0x80000)) || !vm.memory.registerRam(0x8041f070,std::vector<uint8_t>(16)) || !vm.memory.registerRam(0x8041e428,std::vector<uint8_t>(4)) || !vm.memory.registerRam(0x8041e844,std::vector<uint8_t>(4)) || !vm.memory.registerRam(0x8041e9e4,std::vector<uint8_t>(24)) || !vm.memory.importScripts(program->scripts))return false;
  for(const auto& b:program->battles)if(b.room==room){char name[4]{};
   for(unsigned i=0;i<4;++i){uint8_t byte=0;if(!vm.memory.readByte(b.name+i,byte))return false;name[i]=char(byte);}
   registerPackedBattleArea(name,b.setup,b.numbers,nativeBattleActorServices(vm));
  }
  const auto memory=vm.memoryBinding.services();const auto start=eventSdkInitializeAllocator(0x82000000,0x82080000,1,memory);
  if(eventSdkCreateHeap(start,0x82080000,memory)!=0 || !vm.memory.writeWord(0x8041e844,0) || !vm.memory.writeWord(0x81000004,60))return false;
  vm.gp.timestamp=milliseconds*eventClockRate(243000000);vm.initializeContexts();return true;
 }
};
bool FieldAreaEvent::begin(uint32_t room,uint64_t milliseconds) {
 if(started || !assets)return false;
 auto next=std::make_shared<FieldAreaEventRuntime>(this);next->room=room;for(const auto& e:next->program->entries)if(e.room==room)next->entries.push_back(e);
 for(const auto& e:next->program->initializers)if(e[0]==room)next->initializer=e[1];
 if((next->entries.empty() && !next->initializer) || !next->initialize(milliseconds))return false;
 runtime_=std::move(next);started=true;failed=false;completed_.clear();return true;
}
std::optional<uint32_t> FieldAreaEvent::appendAreaInit(const FieldAreaGpState& state) {
 if(!runtime_ || failed || !runtime_->initializer || runtime_->initId || !runtime_->roots.empty() || !completed_.empty())return {};
 auto& r=*runtime_;for(unsigned n=0;n<FieldCasePool::Capacity;++n)if(r.cases.active(n))return {};
 if(!r.vm.memory.writeWord(0x81000000,state.systemFlags))return {};
 for(unsigned n=0;n<state.bytes.size();++n)if(!r.vm.memory.writeByte(0x81000000+FieldAreaGpState::Begin+n,state.bytes[n]))return {};
 auto& e=createRootEvent(r.vm.schedule,r.initializer,0,0,r.vm.creationServices());
 if(!r.vm.memory.writeWord(0x81000164,e.id)){failed=true;return {};}
 r.initId=e.id;r.roots.push_back({e.id,UINT32_MAX,{}});return e.id;
}
std::optional<uint32_t> FieldAreaEvent::areaInitEventId() const{return runtime_?runtime_->initId:std::nullopt;}
bool FieldAreaEvent::eventAlive(uint32_t id) const{return runtime_ && checkEventId(id,runtime_->vm.creationServices());}
uint32_t FieldAreaEvent::liveEventCount() const{return runtime_?runtime_->vm.runtime.allocationCount:0;}
std::optional<uint32_t> FieldAreaEvent::battleSetupConfiguration(int32_t index) const {
 if(!runtime_ || failed)return {};
 if(index==-1)return uint32_t(0);
 auto& r=*runtime_;uint32_t gp=0;if(!r.vm.memory.readWord(0x80415fd0,gp))return {};char prefix[4]{};
 for(unsigned i=0;i<3;++i){uint8_t byte=0;if(!r.vm.memory.readByte(gp+0x12c+i,byte))return {};prefix[i]=char(byte);}
 for(const auto& b:r.program->battles)if(b.room==r.room){char name[4]{};
  for(unsigned i=0;i<4;++i){uint8_t byte=0;if(!r.vm.memory.readByte(b.name+i,byte))return {};name[i]=char(byte);}
  if(std::memcmp(prefix,name,4))continue;
  uint32_t record=0,setup=0,numbers=0;
  for(unsigned j=0;j<=33;++j){uint32_t candidate=0,text=0;if(!r.vm.memory.readWord(0x803057ec+4*j,candidate))return {};if(!candidate)break;
   if(j==33 || !r.vm.memory.readWord(candidate,text))return {};
   bool equal=true;
   for(unsigned k=0;k<4;++k){uint8_t byte=0;if(!r.vm.memory.readByte(text+k,byte))return {};equal=equal && byte==uint8_t(prefix[k]);}
   if(equal){record=candidate;break;}
  }
  if(!record || !r.vm.memory.readWord(record+8,setup) || !r.vm.memory.readWord(record+12,numbers) || setup!=b.setup || numbers!=b.numbers)return {};
  const auto row=index>0?uint32_t(index):0u;
  if(row<b.first || uint64_t(row)>=uint64_t(b.first)+b.count)return {};
  return setup+0x44u*row;
 }
 return {};
}
std::optional<uint32_t> FieldAreaEvent::appendDoorEntry(uint32_t entryIndex,const std::array<int32_t,16>& bank) {
 if(!runtime_ || failed || runtime_->initId || entryIndex>=runtime_->entries.size())return {};
 const auto creation=runtime_->vm.creationServices();
 auto* gp=creation.manager.globals(creation.manager.user);
 auto* context=creation.manager.context(creation.manager.user,gp->battleContext!=0);
 bool free=false;for(int n=0;n<context->capacity;++n)if(!(context->entries[n].flags&1)){free=true;break;}
 if(!free)return {}; // Native capacity admission, before source constructor.
 const auto data=runtime_->entries[entryIndex];
 auto& e=createRootEvent(runtime_->vm.schedule,data.script,0,0,runtime_->vm.creationServices());e.locals=bank;e.locals[0]=signedWord(data.record);
 runtime_->roots.push_back({e.id,entryIndex,{}});return e.id;
}
void FieldAreaEvent::tick(uint64_t milliseconds,const FieldAreaEventServices& s) {
 if(!runtime_ || failed)return;
 auto& r=*runtime_;r.services=s;pendingFunction=pendingEventId=0;r.vm.gp.timestamp=milliseconds*eventClockRate(243000000);r.vm.runManager();
 for(auto i=r.roots.begin();i!=r.roots.end();)if(!checkEventId(i->id,r.vm.creationServices())){if(!i->bank){failed=true;return;}completed_.push_back({i->id,i->entryIndex,*i->bank,i->returned});i=r.roots.erase(i);}else ++i;
}
uint32_t FieldAreaEvent::doorEntryCount() const{return runtime_?uint32_t(runtime_->entries.size()):0;}
const FieldCasePool* FieldAreaEvent::cases() const{return runtime_?&runtime_->cases:nullptr;}
}
