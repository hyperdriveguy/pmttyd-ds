#include "trouble_board_event.hpp"
#include "trouble_quest_state.hpp"
#include "event_native_runtime.hpp"
#include "event_memory_bootstrap.hpp"
#include "event_controls.hpp"
#include "event_script_assets.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <unordered_map>
namespace ttyd {
namespace {
struct TroubleBoardProgram {EventScriptAssets scripts;std::unordered_map<uint32_t,TroubleEventCall> callbacks;};
std::shared_ptr<const TroubleBoardProgram> assets;
constexpr uint32_t Functions[]={0x800e7474,0x800d2008,0x800d284c,0x800d2090,0x800d23c4,0x800d21fc,
 0x90110600,0x9010fedc,0x9010fe60,0x800e6de8,0x800eb21c,0x80070110,0x901108b0,0x800f1018,
 0x800f0fe8,0x80053158,0x800e7f8c,0x800aa3d0,0x800aa360,0x9010fe94,0x9011097c};
constexpr uint32_t Board=0x90546018,Counter=0x90546210,Installer=0x90545f30,Confirmation=0x90545b98;
int32_t signedWord(uint32_t value){return value<=INT32_MAX?int32_t(value):int32_t(int64_t(value)-0x100000000LL);}
}
bool loadTroubleBoardEventAssets(const char* root) {
 if(!root)return false;
 const std::string base=root;EventScriptAssets next;
 if(!next.load((base+"/trouble-board-events.pmevt").c_str()))return false;
 FILE* file=std::fopen((base+"/trouble-board-program.bin").c_str(),"rb");if(!file)return false;
 char magic[8];std::array<uint32_t,5> header{};
 bool ok=std::fread(magic,8,1,file)==1 && !std::memcmp(magic,"PMIRAI01",8) &&
  std::fread(header.data(),4,5,file)==5 && header==std::array<uint32_t,5>{Board,Counter,Installer,Confirmation,21};
 std::unordered_map<uint32_t,TroubleEventCall> called;uint32_t tags=0;
 for(unsigned i=0;ok && i<21;++i) {
  uint32_t address=0,tag=0;ok=std::fread(&address,4,1,file)==1 && std::fread(&tag,4,1,file)==1 &&
   tag<21 && address==Functions[tag] && !(tags&(1u<<tag)) && called.emplace(address,TroubleEventCall(tag)).second;
  if(ok)tags|=1u<<tag;
 }
 ok=ok && std::fgetc(file)==EOF;std::fclose(file);uint32_t word=0;
 for(uint32_t address:{Board,Counter,Installer,Confirmation,Confirmation+29*4})ok=ok && next.readWord(address,word);
 if(!ok)return false;
 auto program=std::make_shared<TroubleBoardProgram>();program->scripts=std::move(next);program->callbacks=std::move(called);
 assets=std::move(program);return true;
}
struct TroubleBoardEventRuntime {
 TroubleBoardEvent* owner=nullptr;TroubleEventServices services;
 const std::shared_ptr<const TroubleBoardProgram> program=assets;
 EventNativeRuntime vm;
 explicit TroubleBoardEventRuntime():vm(0x81000000,platform(this)){}
 static EventNativePlatform platform(TroubleBoardEventRuntime* r) {
  EventNativePlatform p;p.user=r;p.clock=[](void*)->uint32_t{return 243000000;};
  p.truncate=[](void*,float v){assert(std::isfinite(v) && double(v)>=INT32_MIN && double(v)<2147483648.);return int32_t(v);};
  p.divideWord=[](void*,int32_t a,int32_t b){assert(b);return a/b;};
  p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){};
  p.format=[](void* r,const EventDiagnosticCall&){++static_cast<TroubleBoardEventRuntime*>(r)->owner->diagnosticCount;};
  p.call=[](void* r,EventEntry& e,uint32_t fn,int32_t first){return static_cast<TroubleBoardEventRuntime*>(r)->call(e,fn,first!=0);};return p;
 }
 int32_t operand(EventEntry& e,unsigned n) {
  uint32_t v=0;if(n>=e.argumentCount || !vm.memory.readWord(e.arguments+n*4,v)){owner->failed=true;return 0;}return signedWord(v);
 }
 int32_t local(EventEntry& e,int n){return getEventValue(e,-30000000+n,vm.interpreterServices().operands);}
 void store(EventEntry& e,int n,int32_t value){setEventValue(e,-30000000+n,value,vm.interpreterServices().operands);}
 std::string text(uint32_t address) {
  std::string result;
  for(unsigned n=0;n<256;++n) {
   uint8_t byte=0;if(!vm.memory.readByte(address+n,byte)){owner->failed=true;return {};}
   if(!byte)return result;
   result.push_back(char(byte));
  }
  owner->failed=true;return {};
 }
 int32_t call(EventEntry& e,uint32_t fn,bool first) {
  const auto found=program->callbacks.find(fn);
  if(found==program->callbacks.end() || e.argumentCount>6){owner->failed=true;return 0;}
  const auto kind=found->second;const auto op=vm.interpreterServices().operands;
  if(kind==TroubleEventCall::Check) {
   const auto quest=local(e,0);uint32_t pointer=0;
   if(quest!=-1 && (quest<1 || quest>30 || !vm.memory.readWord(Confirmation+4*(quest-1),pointer))){owner->failed=true;return 0;}
   store(e,12,signedWord(pointer));store(e,11,local(e,0));return 2;
  }
  if(kind==TroubleEventCall::Start) {
   for(int32_t f=5405;f<=5421;++f)writeSavedSwitch(SavedSwitchFamily::GlobalFlag,f,0,op);
   for(int32_t b=1469;b<=1474;++b)writeSavedSwitch(SavedSwitchFamily::GlobalByte,b,0,op);
   return 2;
  }
  TroubleEventRequest request;request.kind=kind;request.function=fn;request.first=first;request.eventId=e.id;
  request.selection=local(e,0);request.quest=local(e,11);request.work=local(e,10);request.argumentCount=uint8_t(e.argumentCount);
  request.story=readSavedSwitch(SavedSwitchFamily::GlobalByte,0,op);
  request.activeQuest=readSavedSwitch(SavedSwitchFamily::GlobalByte,1420,op);
  for(unsigned n=0;n<e.argumentCount;++n) {request.operands[n]=operand(e,n);request.values[n]=getEventValue(e,request.operands[n],op);}
  if(kind==TroubleEventCall::Print || kind==TroubleEventCall::Select || kind==TroubleEventCall::Append)
   request.text=text(uint32_t(request.values[1]));
  if(kind==TroubleEventCall::NpcPosition || kind==TroubleEventCall::ItemDelete || kind==TroubleEventCall::ItemDeleteCheck)
   request.text=text(uint32_t(request.values[0]));
  if(kind==TroubleEventCall::RegisterCase) {request.text=text(uint32_t(request.values[2]));
   for(int n=0;n<16;++n)request.locals[n]=local(e,n);
  }
  request.switches.user=this;
  request.switches.clearFlag=[](void* p,int32_t flag){auto& r=*static_cast<TroubleBoardEventRuntime*>(p);
   assert(flag>=5405 && flag<=5421);writeSavedSwitch(SavedSwitchFamily::GlobalFlag,flag,0,r.vm.interpreterServices().operands);};
  request.switches.setByte=[](void* p,int32_t byte,uint8_t v){auto& r=*static_cast<TroubleBoardEventRuntime*>(p);
   assert(byte>=1469 && byte<=1474);writeSavedSwitch(SavedSwitchFamily::GlobalByte,byte,v,r.vm.interpreterServices().operands);};
  if(owner->failed)return 0;
  if(!services.call){owner->pendingFunction=fn;return 0;}
  const auto status=services.call(services.user,request);
  if(status==1 && (kind==TroubleEventCall::MarioDirection || kind==TroubleEventCall::PartyDirection))return 1;
  if(status!=0 && status!=2){owner->failed=true;return 0;}
  if(status==0){owner->pendingFunction=fn;return 0;}
  if(kind==TroubleEventCall::MenuInit) {
   if(!request.result){owner->failed=true;return 0;}store(e,10,request.result);
  } else if(kind==TroubleEventCall::MenuMain || kind==TroubleEventCall::Select)store(e,0,request.result);
  else if(kind==TroubleEventCall::CoinRead || kind==TroubleEventCall::ItemDeleteCheck)
   setEventValue(e,request.operands[kind==TroubleEventCall::CoinRead?0:1],request.result,op);
  return 2;
 }
 bool initialize(uint64_t time,int32_t story,TroubleBoardRoot root) {
  if(!vm.memory.registerRam(0x82000000,std::vector<uint8_t>(1024*1024)) ||
     !vm.memory.registerRam(0x8041f070,std::vector<uint8_t>(16)) ||
     !vm.memory.registerRam(0x8041e428,std::vector<uint8_t>(4)) ||
     !vm.memory.registerRam(0x8041e844,std::vector<uint8_t>(4)) || !vm.memory.importScripts(program->scripts))return false;
  auto memory=vm.memoryBinding.services();const auto start=eventSdkInitializeAllocator(0x82000000,0x82100000,1,memory);
  if(eventSdkCreateHeap(start,0x82100000,memory)!=0 || !vm.memory.writeWord(0x8041e844,0) || !vm.memory.writeWord(0x81000004,60))return false;
  vm.gp.timestamp=time*eventClockRate(243000000);vm.initializeContexts();const auto op=vm.interpreterServices().operands;
  writeSavedSwitch(SavedSwitchFamily::GlobalByte,0,story,op);
  writeSavedSwitch(SavedSwitchFamily::GlobalByte,1420,*owner->switches.quest1420,op);
  for(int32_t f=5374;f<=5421;++f)writeSavedSwitch(SavedSwitchFamily::GlobalFlag,f,*troubleQuestFlag(owner->switches,f),op);
  for(int32_t b=1469;b<=1474;++b)writeSavedSwitch(SavedSwitchFamily::GlobalByte,b,*troubleQuestByte(owner->switches,b),op);
  auto creation=vm.creationServices();auto& entry=createRootEvent(vm.schedule,root==TroubleBoardRoot::Installer?Installer:root==TroubleBoardRoot::Counter?Counter:Board,0,0x20,creation);
  owner->rootEventId=entry.id;owner->rootAlive=true;return true;
 }
 void publish() {
  const auto op=vm.interpreterServices().operands;
  owner->switches.quest1420=uint8_t(readSavedSwitch(SavedSwitchFamily::GlobalByte,1420,op));
  for(int32_t f=5374;f<=5421;++f)setTroubleQuestFlag(owner->switches,f,readSavedSwitch(SavedSwitchFamily::GlobalFlag,f,op)!=0);
  for(int32_t b=1469;b<=1474;++b)setTroubleQuestByte(owner->switches,b,readSavedSwitch(SavedSwitchFamily::GlobalByte,b,op));
 }
};
bool TroubleBoardEvent::begin(uint64_t time,int32_t story,const FieldActorTalkState& state,TroubleBoardRoot root) {
 if((root!=TroubleBoardRoot::Board && root!=TroubleBoardRoot::Counter && root!=TroubleBoardRoot::Installer) || !assets || active() || !state.trouble.valid() || !state.quest1420)return false;
 for(int32_t f=5374;f<=5421;++f)if(!troubleQuestFlag(state,f))return false;
 for(int32_t b=1469;b<=1474;++b)if(!troubleQuestByte(state,b))return false;
 TroubleBoardEvent next;next.switches=state;next.runtime_=std::make_shared<TroubleBoardEventRuntime>();next.runtime_->owner=&next;
 if(!next.runtime_->initialize(time,story,root))return false;
 next.started=true;*this=std::move(next);runtime_->owner=this;return true;
}
bool TroubleBoardEvent::synchronizeSavedState(int32_t story,const FieldActorTalkState& state) {
 if(!runtime_ || !started || failed || !state.trouble.valid() || !state.quest1420)return false;
 for(int32_t f=5374;f<=5421;++f)if(!troubleQuestFlag(state,f))return false;
 for(int32_t b=1469;b<=1474;++b)if(!troubleQuestByte(state,b))return false;
 const auto op=runtime_->vm.interpreterServices().operands;
 writeSavedSwitch(SavedSwitchFamily::GlobalByte,0,story,op);
 writeSavedSwitch(SavedSwitchFamily::GlobalByte,1420,*state.quest1420,op);
 for(int32_t f=5374;f<=5421;++f)writeSavedSwitch(SavedSwitchFamily::GlobalFlag,f,*troubleQuestFlag(state,f),op);
 for(int32_t b=1469;b<=1474;++b)writeSavedSwitch(SavedSwitchFamily::GlobalByte,b,*troubleQuestByte(state,b),op);
 switches=state;return true;
}
bool TroubleBoardEvent::appendRoot(TroubleBoardRoot root) {
 if(!runtime_ || !started || failed || rootAlive ||
    (root!=TroubleBoardRoot::Board && root!=TroubleBoardRoot::Counter))return false;
 // Reserve the source root's largest immediate constructor burst before the
 // allocator's original free-slot assertion. This is finite native admission.
 const auto& context=runtime_->vm.field;
 int free=0;for(int n=0;n<context.capacity;++n)if(!(context.entries[n].flags&1))++free;
 const int required=root==TroubleBoardRoot::Board && !*troubleQuestFlag(switches,5374)?5:2;
 if(free<required)return false;
 runtime_->owner=this;
 auto creation=runtime_->vm.creationServices();
 auto& entry=createRootEvent(runtime_->vm.schedule,root==TroubleBoardRoot::Counter?Counter:Board,0,0x20,creation);
 rootEventId=entry.id;rootAlive=true;finished=false;return true;
}
void TroubleBoardEvent::tick(uint64_t time,const TroubleEventServices& services) {
 if(!active())return;
 waitingForProvider=!services.call;
 if(waitingForProvider)return; // Do not consume original firstCall before any provider exists.
 runtime_->owner=this;runtime_->services=services;pendingFunction=0;runtime_->vm.gp.timestamp=time*eventClockRate(243000000);
 runtime_->vm.runManager();runtime_->publish();
 auto creation=runtime_->vm.creationServices();rootAlive=checkEventId(rootEventId,creation)!=0;
 finished=runtime_->vm.runtime.allocationCount==0;
}
}
