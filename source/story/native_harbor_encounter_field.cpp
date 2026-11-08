#include "native_harbor_encounter_field.hpp"
#include "event_lifecycle.hpp"
#include "field_battle_information.hpp"
#include "battle_event_callbacks.hpp"
#include <cassert>
#include <map>
namespace ttyd {
namespace {
constexpr uint32_t CutsceneEntry=0x90505208,CutscenePlacement=0x905052f0,ActualAreaName=0x8041fa70;
constexpr uint32_t GorRecord=0x80305364,GorSetup=0x90540da8,GorNumbers=0x90540f84,SetBattleInfo=0x8004f090;
int32_t signedWord(uint32_t value){return value<=0x7fffffffu?int32_t(value):int32_t(int64_t(value)-0x100000000LL);}
struct Guard{bool&v;explicit Guard(bool&b):v(b){v=true;}~Guard(){v=false;}};
}
std::shared_ptr<const NativeHarborEncounterFieldAssets>NativeHarborEncounterFieldAssets::load(const char*root){
 if(!root)return {};
 const std::string base=root;auto result=std::make_shared<NativeHarborEncounterFieldAssets>();auto source=std::make_shared<EventScriptAssets>();
 if(!source->load((base+"/door-data.pmevt").c_str()))return {};
 for(const auto*name:{"harbor-events.pmevt","field-npc-creation.pmevt"}){EventScriptAssets child;if(!child.load((base+"/"+name).c_str())||!source->merge(child))return {};}
 if(!result->tags.load((base+"/field-npc-map-gor_00.bin").c_str()))return {};
 for(const auto*name:{"c_pkr","c_kanbu_ibt","c_zako_n","c_gara","c_suifu_b"}){const auto stem=base+"/field-native-"+name;auto pose=loadBattlePoseResource((stem+".pose-directory.bin").c_str(),(stem+".pmanim").c_str());if(!pose)return {};result->poses.push_back(std::move(pose));}
 EventScriptAssets battle;if(!battle.load((base+"/harbor-tutorial-events.pmevt").c_str()))return {};
 std::array<uint8_t,8> table{};for(unsigned i=0;i<table.size();++i)if(!battle.readByte(0x80417920+i,table[i]))return {};result->originalHeldItemTable=table;
 result->source=std::move(source);return result;
}
struct NativeHarborEncounterField::State {
 EventNativeRuntime*vm=nullptr;const HarborEncounter*facade=nullptr;
 NativeFacadeRegistration::Proof vmProof,facadeProof;
 NativeFacadeRegistration context;
 NativeHarborEncounterFieldConfig config;
 std::shared_ptr<FieldNpcRegistryOwner> owner;
 std::unique_ptr<FieldNpcRegistryBinding> ownedRegistry;FieldNpcRegistryBinding*registry=nullptr;
 std::unique_ptr<FieldNpcSetupBinding> setup;
 std::unique_ptr<NativeHarborSailors> sailors;
 std::unique_ptr<FieldNpcCreationBinding> creation;
 std::shared_ptr<NativeHarborFieldSequence> sequence;
 std::array<uint8_t,8> heldItemBytes{};bool heldItemMapped=false;
 std::map<uint32_t,FieldNpcActorEventCursor> actors;
 struct Information {EventEntry*entry=nullptr;uint32_t address=0,id=0,args=0,raw=0,name=0,index=0,actor=0,phase=0;};
 std::map<uint32_t,Information> information;
 enum class Stage{Sailors,CreateEntry,WaitEntry,CreatePlacement,WaitPlacement,Ready};Stage stage=Stage::Sailors;
 uint32_t constructor=0,pending=0,gp=0,field=0;bool stale=false,checking=false,captured=false,areaRegistered=false;
 ~State(){context.retire();if(heldItemMapped&&vmProof.current())vm->memory.unregisterExternalRam(0x80417920,heldItemBytes.data());}
 bool current(const HarborEncounter*caller){
  // Pointer comparison is identity metadata only. No facade/Game dereference.
  // Copied facades sharing a runtime reject before the original host callback.
  if(caller!=facade||checking)return false;
  Guard guard(checking);
  if(stale||!vmProof.current()||!facadeProof.current()||!config.host.current()){stale=true;return false;}
  if(!config.current(config.user,*vm,facade)){stale=true;return false;}
  if(!config.animation||!config.animation->ready())return false;
  if(!captured)return true;
  uint32_t now=0,mode=0,work=0;
  if(!vm->memory.readWord(0x80415fd0,now)||now!=gp||!vm->memory.readWord(gp+0x14,mode)||mode||!vm->memory.readWord(gp+0x168,work)||work!=field||!owner->aliases(vm->memory))return false;
  if(areaRegistered){uint32_t setup=0,numbers=0;if(!vm->memory.readWord(GorRecord+8,setup)||setup!=GorSetup||!vm->memory.readWord(GorRecord+12,numbers)||numbers!=GorNumbers){stale=true;return false;}
   for(unsigned i=0;i<4;++i){uint8_t actual=0,source=0;if(!vm->memory.readByte(gp+0x12c+i,actual)||!config.assets->source->readByte(ActualAreaName+i,source)||actual!=source){stale=true;return false;}}
  }return true;
 }
 bool sourceWord(uint32_t a,uint32_t&value){uint32_t source=0;return config.assets->source->readWord(a,source)&&vm->memory.readWord(a,value)&&source==value;}
 bool registerArea(){
  // Native Harbor map-admission contribution, using actual original bytes.
  // It never infers execution of the original REL prolog/whole FieldArea Init.
  if(!current(facade))return false;
  char area[4]{};bool zero=true,equal=true;
  for(unsigned i=0;i<4;++i){uint8_t source=0,actual=0,old=0;if(!config.assets->source->readByte(ActualAreaName+i,source)||!vm->memory.readByte(ActualAreaName+i,actual)||source!=actual||!vm->memory.readByte(gp+0x12c+i,old))return false;area[i]=char(source);zero&=old==0;equal&=old==source;}
  if(area[3]||(!zero&&!equal))return false;
  // Admit all original registry records/names before calling its total leaf.
  bool found=false;
  for(unsigned i=0;i<=33;++i){uint32_t record=0,name=0;if(!sourceWord(0x803057ec+4*i,record))return false;if(!record){if(i!=33)return false;break;}
   if(i==33||!sourceWord(record,name))return false;
   bool match=true;for(unsigned j=0;j<4;++j){uint8_t expected=0,actual=0;if(!config.assets->source->readByte(name+j,expected)||!vm->memory.readByte(name+j,actual)||expected!=actual)return false;match&=actual==uint8_t(area[j]);}
   if(match){if(record!=GorRecord)return false;found=true;}
  }
  if(!found)return false;
  uint32_t value=0;for(unsigned i=0;i<17;++i)if(!sourceWord(GorSetup+2*0x44+4*i,value))return false;
  for(unsigned i=0;i<14;++i)if(!sourceWord(GorNumbers+4*i,value))return false;
  uint32_t before=0;if(!vm->memory.readWord(GorRecord+8,before)||(before&&before!=GorSetup)||!vm->memory.readWord(GorRecord+12,before)||(before&&before!=GorNumbers))return false;
  for(unsigned i=0;i<4;++i)if(!vm->memory.writeByte(gp+0x12c+i,uint8_t(area[i])))return false;
  registerPackedBattleArea(area,GorSetup,GorNumbers,nativeBattleActorServices(*vm));areaRegistered=true;return current(facade);
 }
 std::optional<int32_t> setInformation(EventEntry&e){
  pending=SetBattleInfo;
  if(!setup->ownsCaller(e))return {};
  auto&c=information[e.id];if(!c.entry){c.entry=&e;c.id=e.id;if(!vm->entryAddresses.addressOf(&e,c.address))return {};}
  if(c.entry!=&e||c.id!=e.id)return {};
  auto value=[&](uint32_t raw,uint32_t&out){
   // Actual reached Harbor operands are immediate name/index words. Generic
   // variable/encoded operands remain at the genuine GetValue child frontier.
   const auto input=signedWord(raw);pending=0x80031260;
   if(input>-250000000&&input<=-20000000)return false;
   out=uint32_t(getEventValue(e,input,vm->interpreterServices().operands));return current(facade);
  };
  for(;;)switch(c.phase){
  case 0:if(!vm->memory.readWord(c.address+0x18,c.args))return {};++c.phase;break;
  case 1:if(!vm->memory.readWord(c.args,c.raw))return {};++c.phase;break;
  case 2:if(!value(c.raw,c.name))return {};++c.phase;break;
  case 3:if(!vm->memory.readWord(c.args+4,c.raw))return {};++c.phase;break;
  case 4:if(!value(c.raw,c.index))return {};++c.phase;break;
  case 5:{pending=0x8004f5d8;const auto actor=setup->resolveEventActor(e,c.name,true);if(!actor)return {};c.actor=*actor;++c.phase;break;}
  case 6:{pending=0x80043d80;
   if(!config.setup.admitBattle||!config.setup.random||!config.setup.admitBattle(config.setup.user,vm->memory,c.actor,signedWord(c.index))||!current(facade))return {};
   PackedNpcBattleSetupServices services;services.memory=nativeBattleActorServices(*vm);services.user=config.setup.user;services.random=config.setup.random;services.savedFlag=config.setup.savedFlag;
   setPackedNpcBattleInformation(c.actor,signedWord(c.index),services);information.erase(e.id);pending=0;return 2;
  }
  default:return {};
  }
 }
};
NativeHarborEncounterField::NativeHarborEncounterField(std::unique_ptr<State>s):state_(std::move(s)){}
NativeHarborEncounterField::~NativeHarborEncounterField(){state_->context.retire();}
std::unique_ptr<NativeHarborEncounterField>NativeHarborEncounterField::prepare(EventNativeRuntime&vm,const HarborEncounter*facade,NativeFacadeRegistration::Proof proof,NativeHarborEncounterFieldConfig config){
 if(!facade||!proof.current()||!config.host.current()||!config.current||!config.serviceLifetime||!config.animation||!config.assets||!config.assets->source)return {};
 auto s=std::make_unique<State>();s->vm=&vm;s->vmProof=vm.nativeFacadeProof();s->facade=facade;s->facadeProof=std::move(proof);s->config=std::move(config);
 if(!s->current(facade)||vm.schedule.managerActive||!vm.field.entries||vm.field.capacity<=0||vm.field.capacity>256)return {};
 uint32_t initialMode=0;if(!vm.memory.readWord(0x80415fd0,s->gp)||!vm.memory.readWord(s->gp+0x14,initialMode)||initialMode)return {};
 for(const auto address:{CutsceneEntry,CutscenePlacement})if(!admitOriginalNpcSetupScript(*s->config.assets->source,vm.memory,address))return {};
 s->owner=s->config.registry?s->config.registry:FieldNpcRegistryOwner::cold(s->config.newRegistryBase,s->config.animation->pool());
 if(!s->owner||s->owner->poses().sharedPool()!=s->config.animation->pool())return {};
 if(s->config.registryBinding)s->registry=s->config.registryBinding;
 else{s->ownedRegistry=FieldNpcRegistryBinding::bind(s->owner,vm.memory);s->registry=s->ownedRegistry.get();}
 if(!s->registry||(!s->owner->initialized()&&!s->registry->initialize())||!s->owner->aliases(vm.memory))return {};
 if(!vm.memory.readWord(0x80415fd0,s->gp)||!vm.memory.readWord(s->gp+0x168,s->field)||s->field!=s->owner->fieldWorkAddress())return {};
 s->captured=true;if(!s->current(facade)||!s->registerArea())return {};
 if(s->config.assets->originalHeldItemTable){s->heldItemBytes=*s->config.assets->originalHeldItemTable;
  if(vm.memory.unmappedRange(0x80417920,s->heldItemBytes.size())){if(!vm.memory.registerExternalRam(0x80417920,s->heldItemBytes.data(),s->heldItemBytes.size()))return {};s->heldItemMapped=true;}
  else for(unsigned i=0;i<s->heldItemBytes.size();++i){uint8_t byte=0;if(!vm.memory.readByte(0x80417920+i,byte)||byte!=s->heldItemBytes[i])return {};}
 }
 for(const auto&r:s->config.assets->poses)if(!r||(!s->owner->poses().canEnter(r->modelName,2)&&!s->owner->poses().install(r)))return {};
 FieldNpcSetupNativeServices services=s->config.setup;services.user=s.get();
 services.clock=[](void*p){auto&s=*static_cast<State*>(p);BattlePoseClock clock;const bool okay=s.current(s.facade)&&s.config.animation->clock(clock);assert(okay);(void)okay;return clock;};
 services.admitBattle=[](void*p,EventAddressSpace&m,uint32_t actor,int32_t index){auto&s=*static_cast<State*>(p);return s.current(s.facade)&&s.config.setup.admitBattle&&s.config.setup.random&&s.config.setup.admitBattle(s.config.setup.user,m,actor,index);};
 services.random=[](void*p,int32_t bound){auto&s=*static_cast<State*>(p);assert(s.config.setup.random);return s.config.setup.random(s.config.setup.user,bound);};
 services.savedFlag=[](void*p,int32_t index){auto&s=*static_cast<State*>(p);assert(s.config.setup.savedFlag);return s.config.setup.savedFlag(s.config.setup.user,index);};
 if(s->config.setup.admitMario)services.admitMario=[](void*p,EventAddressSpace&m,uint32_t actor){auto&s=*static_cast<State*>(p);return s.current(s.facade)&&s.config.setup.admitMario(s.config.setup.user,m,actor);};
 if(s->config.setup.wanderRequest)services.wanderRequest=[](void*p,EventAddressSpace&m,const FieldNpcWanderRequest&q,FieldNpcWanderResponse&r){auto&s=*static_cast<State*>(p);return s.current(s.facade)&&s.config.setup.wanderRequest(s.config.setup.user,m,q,r);};
 s->setup=FieldNpcSetupBinding::bind(s->owner,*s->registry,vm,s->config.assets->source,services);if(!s->setup)return {};
 s->sailors=NativeHarborSailors::prepare(vm,*s->registry,s->owner,s->config.animation,s->config.assets->source,s->config.assets->tags,s->config.privateTagBase,ActualAreaName);if(!s->sailors)return {};
 s->creation=FieldNpcCreationBinding::bind(s->owner,*s->registry,*s->setup,vm,s->config.animation);if(!s->creation)return {};
 s->sequence=NativeHarborFieldSequence::prepare(vm,s->owner,s->context.proof());if(!s->sequence||!s->sequence->attach(*s->setup,s->config.battle,s->sailors->regularHistory()))return {};
 return std::unique_ptr<NativeHarborEncounterField>(new NativeHarborEncounterField(std::move(s)));
}
bool NativeHarborEncounterField::current(const HarborEncounter*f){return !invoking_&&state_->current(f);}
bool NativeHarborEncounterField::initialized()const{return state_->stage==State::Stage::Ready;}
bool NativeHarborEncounterField::advance(const HarborEncounter*f){
 if(invoking_||!state_->current(f))return false;Guard guard(invoking_);auto&s=*state_;
 s.pending=0;s.creation->pump();
 if(s.creation->pending()&&!s.creation->resume()){s.pending=s.creation->pendingFunction();return false;}
 if(s.stage==State::Stage::Sailors){if(!s.sailors->advance()){s.pending=s.sailors->pending();return false;}s.stage=State::Stage::CreateEntry;}
 if(s.stage==State::Stage::CreateEntry||s.stage==State::Stage::CreatePlacement){
  bool free=false;for(int i=0;i<s.vm->field.capacity;++i)free|=!(s.vm->field.entries[i].flags&1);if(!free)return false;
  const bool placement=s.stage==State::Stage::CreatePlacement;
  auto&e=createRootEvent(s.vm->schedule,placement?CutscenePlacement:CutsceneEntry,0,0,s.vm->creationServices());s.constructor=e.id;s.stage=placement?State::Stage::WaitPlacement:State::Stage::WaitEntry;
 }return s.current(f);
}
bool NativeHarborEncounterField::handles(uint32_t fn)const{return fn==SetBattleInfo||fieldNpcCreationEventFunction(fn)||fieldNpcActorEventFunction(fn)||fieldNpcBattleEventFunction(fn);}
std::optional<int32_t>NativeHarborEncounterField::invoke(const HarborEncounter*f,uint32_t fn,EventEntry&e,bool first){
 if(invoking_||!state_->current(f)||!handles(fn))return {};Guard guard(invoking_);auto&s=*state_;std::optional<int32_t> result;
 if(fn==SetBattleInfo&&initialized())result=s.setInformation(e);
 else if(fieldNpcCreationEventFunction(fn))result=s.creation->invoke(fn,e);
 else if(fieldNpcActorEventFunction(fn)){const auto value=s.setup->invokeActorEvent(fn,e,first,s.actors[e.id]);if(value)result=value;}
 else if(initialized())result=s.sequence->invokeSource(fn,e,first);
 if(!s.current(f))return {};
 s.pending=result?0:(fn==SetBattleInfo?s.pending:fieldNpcBattleEventFunction(fn)?s.sequence->pending():fn);return result;
}
std::optional<int32_t>NativeHarborEncounterField::external(const HarborEncounter*f,uint32_t fn,EventEntry&e,bool first){
 if(invoking_||!state_->current(f)||handles(fn))return {};Guard guard(invoking_);auto&s=*state_;
 s.pending=fn;if(!s.config.call)return {};
 const auto result=s.config.call(s.config.user,*s.vm,e,fn,first);
 if(!s.current(f))return {};if(result)s.pending=0;return result;
}
void NativeHarborEncounterField::retire(const HarborEncounter*f,const EventEntry&e){
 if(invoking_){state_->stale=true;state_->context.retire();return;}
 if(!state_->current(f))return;Guard guard(invoking_);auto&s=*state_;s.creation->retire(e);s.sequence->retire(e);s.actors.erase(e.id);
 if(s.config.retiring){s.config.retiring(s.config.user,*s.vm,e);if(!s.current(f))return;}
 const auto info=s.information.find(e.id);if(info!=s.information.end()&&info->second.entry==&e){s.stale=true;s.context.retire();return;}
 if(e.id==s.constructor){if(e.opcode!=2){s.stale=true;s.context.retire();return;}s.constructor=0;
  if(s.stage==State::Stage::WaitEntry)s.stage=State::Stage::CreatePlacement;
  else if(s.stage==State::Stage::WaitPlacement)s.stage=State::Stage::Ready;
 }
}
uint32_t NativeHarborEncounterField::pending()const{return state_->pending;}
std::optional<NativeHarborBattleTicket>NativeHarborEncounterField::request(const HarborEncounter*f){return current(f)&&initialized()?state_->sequence->request():std::nullopt;}
bool NativeHarborEncounterField::entered(const HarborEncounter*f,const NativeHarborBattleTicket&t,const NativeHarborBattleIdentity&i){return current(f)&&initialized()&&state_->sequence->entered(t,i);}
bool NativeHarborEncounterField::returned(const HarborEncounter*f,const NativeHarborBattleTicket&t,const NativeHarborCompletionReceipt&r){return current(f)&&initialized()&&state_->sequence->returned(t,r);}
std::shared_ptr<FieldNpcRegistryOwner>NativeHarborEncounterField::registry()const{return state_->owner;}
std::optional<uint32_t>NativeHarborEncounterField::actor(const HarborEncounter*f,uint32_t name){return current(f)&&initialized()?state_->registry->lookup(name):std::nullopt;}
}
