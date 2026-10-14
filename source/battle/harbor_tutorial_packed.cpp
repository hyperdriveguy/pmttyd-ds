#include "harbor_tutorial_packed.hpp"
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include "event_lifecycle.hpp"
#include "battle_packed_hit_precheck.hpp"
#include "battle_packed_pad.hpp"
#include "battle_packed_pouch.hpp"
#include "field_battle_information.hpp"
#include "battle_sequence_startup.hpp"
#include "battle_stage_floor.hpp"
#include "battle_packed_hit_event.hpp"
#include "battle_hit_code_events.hpp"
#include "prologue_pouch.hpp"
#include "battle_unit_resources.hpp"
#include "battle_weapon_cost.hpp"
#include "battle_facing.hpp"
#include "battle_action_help.hpp"
namespace ttyd {
namespace {void required(bool ok){if(!ok)std::abort();}int32_t signByte(uint8_t b){return b<128?int32_t(b):int32_t(b)-256;}}
uint32_t HarborTutorialPacked::word(uint32_t a)const{uint32_t v=0;required(vm.memory.readWord(a,v));return v;}
void HarborTutorialPacked::word(uint32_t a,uint32_t v){required(vm.memory.writeWord(a,v));}
uint8_t HarborTutorialPacked::byte(uint32_t a)const{uint8_t v=0;required(vm.memory.readByte(a,v));return v;}
void HarborTutorialPacked::byte(uint32_t a,uint8_t v){required(vm.memory.writeByte(a,v));}
void HarborTutorialPacked::half(uint32_t a,int32_t v){required(vm.memory.writeHalf(a,uint16_t(v)));}
void HarborTutorialPacked::real(uint32_t a,float v){uint32_t bits=0;std::memcpy(&bits,&v,4);word(a,bits);}
float HarborTutorialPacked::real(uint32_t a)const{const auto bits=word(a);float v;std::memcpy(&v,&bits,4);return v;}
int32_t HarborTutorialPacked::raw(EventEntry& e,unsigned i)const{assert(i<e.argumentCount);return int32_t(word(e.arguments+4*i));}
int32_t HarborTutorialPacked::value(EventEntry& e,unsigned i){return getEventValue(e,raw(e,i),vm.interpreterServices().operands);}
void HarborTutorialPacked::output(EventEntry& e,unsigned i,int32_t v){setEventValue(e,raw(e,i),v,vm.interpreterServices().operands);}
uint32_t HarborTutorialPacked::actor(EventEntry& e){const auto id=translateBattleActor(&e,value(e,0),nativeBattleActorServices(vm));return word(Battle+0x20+4u*uint32_t(id));}
bool HarborTutorialPacked::provision(){
 for(auto span:std::array<std::pair<uint32_t,unsigned>,5>{{{0x8041eb90,4},{Battle,0x19400},{Units,0x4000},{Parts,0x4000},{MenuRam,0x4000}}})if(!vm.memory.registerRam(span.first,std::vector<uint8_t>(span.second)))return false;
 word(0x8041eb90,Battle);initializePackedBattleStageFloorFields(Battle,nativeBattleActorServices(vm).memory);half(Battle+8,2);half(Battle+16,1);half(Battle+24,0);byte(Battle+0xa,1);byte(Battle+0x12,255); // Fresh source group-policy adapter.
 for(unsigned i=0;i<64;++i){word(Battle+0x120+4*i,0xffffffff);word(Battle+0x16f5c+8*i,0xffffffff);}
 BattlePadWork pad;pad.init();writePackedBattlePad(Battle+0x1d1c,pad,nativeBattleActorServices(vm));writePackedBattlePad(Battle+0xf2c,pad,nativeBattleActorServices(vm));
 BattleSoundPlaybackServices playback;playback.user=this;playback.trigger=[](void* self,uint32_t u,uint32_t key,int32_t sentinel,int32_t zero,const std::array<float,3>& pos){assert(sentinel==-250000000 && zero==0);return static_cast<HarborTutorialPacked*>(self)->requestSound(u,key,pos);};
 playback.position=[](void* self,uint32_t handle,const std::array<float,3>& pos){HarborTutorialSoundRequest r;r.operation=HarborTutorialSoundRequest::Operation::Position;r.handle=handle;r.position=pos;static_cast<HarborTutorialPacked*>(self)->sounds.push_back(r);};
 playback.off=[](void* self,uint32_t handle){HarborTutorialSoundRequest r;r.operation=HarborTutorialSoundRequest::Operation::Off;r.handle=handle;static_cast<HarborTutorialPacked*>(self)->sounds.push_back(r);};
 marioJumpBackend.runtime=&vm;marioJumpBackend.user=this;marioJumpBackend.play=[](void* self,uint32_t key,const std::array<float,3>& position){auto& p=*static_cast<HarborTutorialPacked*>(self);return p.requestSound(p.marioJumpUnit,key,position);};marioJump=nativeBattleMarioJumpServices(marioJumpBackend);
 takeoffBackend.runtime=&vm;takeoff=nativeBattleTakeoffServices(vm,nativeBattleTakeoffCoreServices(takeoffBackend));
 movement=nativeBattleMovePositionServices(vm,playback);jump.movement=movement;jump.user=this;jump.floor=[](void* self,uint32_t battle,float x,float y,float z){auto& p=*static_cast<HarborTutorialPacked*>(self);return packedBattleStageFloorHeight(battle,x,y,z,nativeBattleActorServices(p.vm).memory);};jump.play=[](void* self,uint32_t u,uint32_t key,const std::array<float,3>& pos){static_cast<HarborTutorialPacked*>(self)->requestSound(u,key,pos);};camera.positions=nativeBattlePartPositionServices(vm);camera.values=camera.positions.unit.values;
 camera.user=this;camera.signedWordFloat=[](void* self,int32_t v){auto& p=*static_cast<HarborTutorialPacked*>(self);return p.vm.platform.signedWordFloat(p.vm.platform.user,v);};
 camera.rumbleOn=[](void* self,int32_t channel){static_cast<HarborTutorialPacked*>(self)->rumbleRequests.push_back(channel);};
 return registry.bind(0x80113f1c,&vm,[](void* p,EventEntry& e,int32_t){return battleMarkUnit1feEvent(e,nativeBattleUnitEventServices(*static_cast<EventNativeRuntime*>(p)));}) && registry.bind(0x8010f458,&vm,[](void* p,EventEntry& e,int32_t){auto& vm=*static_cast<EventNativeRuntime*>(p);return battleGetHitDamageEvent(e,nativeBattleUnitEventServices(vm),nativeBattleActorServices(vm));}) && registry.bind(0x80105b40,&vm,[](void* p,EventEntry& e,int32_t){return battleUnitFlagEvent(e,true,nativeBattleUnitEventServices(*static_cast<EventNativeRuntime*>(p)));}) && registry.bind(0x80105ae0,&vm,[](void* p,EventEntry& e,int32_t){return battleUnitFlagEvent(e,false,nativeBattleUnitEventServices(*static_cast<EventNativeRuntime*>(p)));}) && registry.bind(0x8010a010,&vm,[](void* p,EventEntry& e,int32_t){return runBattleDataChild(e,nativeBattleDataChildServices(*static_cast<EventNativeRuntime*>(p)));}) && registry.bind(0x8010e4bc,&vm,[](void* p,EventEntry& e,int32_t){auto& vm=*static_cast<EventNativeRuntime*>(p);return battleResetUnitFacingEvent(e,nativeBattleUnitEventServices(vm),nativeBattleActorServices(vm));}) && registry.bind(0x8010e430,&vm,[](void* p,EventEntry& e,int32_t){return battleCheckUnitAttributeEvent(e,nativeBattleUnitEventServices(*static_cast<EventNativeRuntime*>(p)));}) && registry.bind(0x8010f0e8,&vm,[](void* p,EventEntry& e,int32_t){return battleCheckHitPatternEvent(e,nativeBattleUnitEventServices(*static_cast<EventNativeRuntime*>(p)));}) && registry.bind(0x8010f2e8,&vm,[](void* p,EventEntry& e,int32_t){return battleCheckHitCodeEvent(e,nativeBattleUnitEventServices(*static_cast<EventNativeRuntime*>(p)));}) &&
 registry.bind(0x8010f374,&vm,[](void* p,EventEntry& e,int32_t){return battleGetHitCodeEvent(e,nativeBattleUnitEventServices(*static_cast<EventNativeRuntime*>(p)));}) && bindBattleCheckDataCallback(registry,vm) && bindBattleGetBodyIdCallback(registry,vm) && bindBattleSetMoveSpeedCallback(registry,vm) && bindBattleMovePositionCallback(registry,movement) && bindBattleJumpSettingCallback(registry,vm) && bindBattleGetPosCallback(registry,vm) && bindBattleGetHomePosCallback(registry,vm) && bindBattleGetHitPosCallback(registry,vm) && bindBattleGetUnitWorkCallback(registry,vm) && bindBattleSetUnitWorkCallback(registry,vm) && bindBattleGetHpCallback(registry,vm) && bindBattleGetMaxHpCallback(registry,vm) && bindBattleStartWaitCallback(registry,vm) && bindBattleStopWaitCallback(registry,vm) && bindBattleSelectedTargetCallback(registry,vm) && bindBattleCommandWeaponCallback(registry,vm) && bindBattleWaitGuardMoveCallback(registry,vm) && bindBattleMajinaiQueryCallback(registry,vm) && bindBattleTakeoffCallback(registry,takeoff) && bindBattleGetMoveFrameCallback(registry,vm) && bindBattleMarioJumpParameterCallback(registry,vm) && bindBattleSetFallAccelCallback(registry,vm) && bindBattleFramesToMillisecondsCallback(registry,vm) && bindBattleMarioJumpPositionCallback(registry,marioJump);
}
uint32_t HarborTutorialPacked::requestSound(uint32_t u,uint32_t key,const std::array<float,3>& pos){HarborTutorialSoundRequest r;r.handle=nextSoundHandle++;r.key=key;r.owner=int32_t(word(u));r.position=pos;for(unsigned i=0;i<128;++i){const auto c=byte(key+i);if(!c)break;r.name+=char(c);}sounds.push_back(r);return r.handle;}
bool HarborTutorialPacked::provisionFreshPouch(const char* name,const ProloguePouch* entry){
 if(!name || freshPouch || !vm.memory.registerRam(0x8041eb00,std::vector<uint8_t>(4)) || !vm.memory.registerRam(0x84400000,std::vector<uint8_t>(0x5d4)))return false;
 if(entry){for(unsigned i=0;i<entry->bytes.size();++i)byte(0x84400000+i,entry->bytes[i]);word(0x8041eb00,0x84400000);freshPouch=true;return true;}
 struct Provider{const char* name;bool allocated=false;} provider{name};PackedPouchInitializationServices services;services.memory=nativeBattleActorServices(vm).memory;services.user=&provider;
 services.allocate=[](void* p,int32_t heap,uint32_t size){auto& v=*static_cast<Provider*>(p);assert(heap==0 && size==0x5d4 && !v.allocated);v.allocated=true;return uint32_t(0x84400000);};
 services.message=[](void* p,const char* key){assert(!std::strcmp(key,"name_party3"));return static_cast<Provider*>(p)->name;};
 freshPouch=initializePackedPouch(services)==0x84400000;return freshPouch;
}
void HarborTutorialPacked::importMarioPouch(){
 ProloguePouch copy;const auto p=word(0x8041eb00);for(unsigned i=0;i<copy.bytes.size();++i)copy.bytes[i]=byte(p+i);
 readDamage(1);auto& d=damageUnits[1];d.hpPlusCount=byte(Units+0x1000+0x2f1);d.fpPlusCount=byte(Units+0x1000+0x2f2);
 UnitResourceServices services;services.user=&copy;services.pouch=[](void* self){return static_cast<ProloguePouch*>(self)->bytes.data();};importUnitResources(d,services);
 const auto u=Units+0x1000;half(u+0x10a,d.baseHp);half(u+0x108,d.maxHp);half(u+0x10c,d.hp);half(u+0x110,d.baseFp);half(u+0x10e,d.maxFp);half(u+0x112,d.fp);
}
bool HarborTutorialPacked::provisionFreshFieldInformation(uint32_t c,uint32_t group,uint32_t stage){
 if(!freshPouch || !vm.memory.registerRam(0x84500000,std::vector<uint8_t>(0x580)) || !vm.memory.registerRam(0x84501000,std::vector<uint8_t>(0x340)))return false;
 // Explicit fresh NPC/Fbat allocation adapter; only source-owned information
 // fields are consumed, not uninitialized field model/control internals.
 const auto f=uint32_t(0x84500000),npc=uint32_t(0x84501000);word(vm.gpAddresses.currentAddress()+0x168,f);word(f+8,npc);
 PackedNpcBattleSetupServices setup;setup.memory=nativeBattleActorServices(vm);setup.user=this;setup.random=[](void* self,int32_t bound){return static_cast<HarborTutorialPacked*>(self)->random.bounded(bound);};setup.savedFlag=[](void* self,int32_t index){auto& p=*static_cast<HarborTutorialPacked*>(self);return readSavedSwitch(SavedSwitchFamily::GlobalFlag,index,p.vm.interpreterServices().operands);};
 setupPackedNpcBattleInformation(npc,c,setup);if(word(npc+0x248)!=group || word(npc+0x230)!=stage)return false;
 PackedFieldBattleInformationServices services;services.memory=nativeBattleActorServices(vm);services.marioPartyId=[](void*)->int32_t{return 0;};services.party=[](void*,int32_t id){assert(id==0);return uint32_t(0);};services.partyKind=[](void*,int32_t)->int32_t{std::abort();};
 const auto info=preparePackedFieldBattleInformation(f,npc,word(0x8041eb00),services);word(Battle+0x2738,info);return true;
}
void HarborTutorialPacked::samplePhysical(uint64_t time,const BattlePadSample* sample){physicalCurrent=sample!=nullptr;if(!sample || sampledAt==time)return;sampledAt=time;auto general=readPackedBattlePad(Battle+0xf2c,nativeBattleActorServices(vm));general.update(*sample);writePackedBattlePad(Battle+0xf2c,general,nativeBattleActorServices(vm));auto pad=readPackedBattlePad(Battle+0x1d1c,nativeBattleActorServices(vm));pad.update(*sample);writePackedBattlePad(Battle+0x1d1c,pad,nativeBattleActorServices(vm));}
bool HarborTutorialPacked::registerActor(HarborTutorialActor& a){
 if(a.id<0 || a.id>3 || registered[a.id])return false;
 const uint32_t id=uint32_t(a.id),u=Units+id*0x1000,p=Parts+id*0x1000,k=a.id==1?word(0x80416a50):a.sourceKind;
 if(!k || word(k)!=uint32_t(a.kind))return false;
 a.sourceKind=k;const auto source=word(k+0xb8);const auto count=byte(k+0xb4);if(!count || count>3)return false;
 registered[id]=true;word(Battle+0x20+4*id,u);word(u,id);word(u+8,a.kind);word(u+0x10,k);word(u+0x14,p);word(u+0x18,word(k+0xc0));byte(u+0xc,uint8_t(a.belong));word(u+0x104,word(k+0xac));byte(u+0xd,byte(k+0xe));
 word(u+0x140,0xffffffff);real(u+0x114,1);byte(u+0x21,1);byte(u+0x22,1);byte(u+0x23,1);
 half(u+0x108,a.maxHp);half(u+0x10c,a.hp);half(u+0x10e,int16_t(word(k+8)));half(u+0x112,int16_t(word(k+8))); // Source FP kind halfword.
 byte(u+0x189,a.belong==1?255:1);
 for(unsigned j=0;j<count;++j){const auto definition=source+0x4c*j,part=p+0x500*j;word(part,j+1<count?part+0x500:0);word(part+4,definition);word(part+0x4ec,u);byte(part+0xbd,a.belong==1?255:1);word(part+0x1c0,0xffffffff);
  if(word(definition+0x40)&1)word(u+0x2d4,word(definition));
  for(unsigned axis=0;axis<3;++axis){real(part+0x24+4*axis,real(definition+0xc+4*axis));real(part+0x170+4*axis,real(definition+0x18+4*axis));real(part+0x188+4*axis,real(definition+0x24+4*axis));}
  for(auto pair:std::array<std::pair<uint32_t,uint32_t>,5>{{{0x1ac,0x40},{0x1b0,0x44},{0x1b4,0x38},{0x1b8,0x3c},{0x1bc,0x48}}})word(part+pair.first,word(definition+pair.second));
 }
 return true;
}
bool HarborTutorialPacked::unitInitReady(){
 BattleStartupServices services;services.memory=nativeBattleActorServices(vm);services.user=this;
 services.eventExists=[](void* self,uint32_t id){auto& runtime=static_cast<HarborTutorialPacked*>(self)->vm;
  for(int32_t slot=0;slot<runtime.battle.capacity;++slot)if((runtime.battle.entries[slot].flags&1) && runtime.battle.entries[slot].id==id)return true;
  return false;};
 return packedBattleUnitInitReady(Battle,services);
}
uint32_t HarborTutorialPacked::runHitEvent(int32_t id,uint32_t code){
 if(id<0 || id>=4 || !registered[size_t(id)])return 0;
 return packedBattleRunHitEvent(word(Battle+0x20+4u*uint32_t(id)),code,nativeBattleUnitEventServices(vm));
}
uint32_t HarborTutorialPacked::runHitEventDirect(int32_t id,uint32_t code,uint32_t script){
 if(id<0 || id>=4 || !registered[size_t(id)])return 0;
 return packedBattleRunHitEventDirect(word(Battle+0x20+4u*uint32_t(id)),code,script,nativeBattleUnitEventServices(vm));
}
bool HarborTutorialPacked::hitEventsActive(){
 for(unsigned id=0;id<4;++id)if(registered[id]){
  const auto handle=word(word(Battle+0x20+4*id)+0x2b4);
  if(handle)for(int32_t slot=0;slot<vm.battle.capacity;++slot)if((vm.battle.entries[slot].flags&1) && vm.battle.entries[slot].id==handle)return true;
 }
 return false; // Observation only: original cleanup ownership remains separate.
}
void HarborTutorialPacked::recordInit(int32_t id,uint32_t event){word(Units+uint32_t(id)*0x1000+0x288,event);}
void HarborTutorialPacked::recordPhase(int32_t id,uint32_t event){word(Units+uint32_t(id)*0x1000+0x29c,event);}
void HarborTutorialPacked::recordAttack(int32_t id,uint32_t event){const auto u=Units+uint32_t(id)*0x1000;word(u+0x2a8,event);const auto wait=word(u+0x290);if(wait)deleteEventId(wait,vm.creationServices());word(u+0x290,0);}
void HarborTutorialPacked::publish(const HarborTutorial& scene){
 word(Battle+0xef4,scene.battleFlags);half(Battle,scene.turn);word(Battle+0xf18,scene.phaseWord);byte(Battle+0x16f58,scene.successDeclarations);byte(Battle+0x16f59,scene.failureDeclarations);
 for(unsigned slot=0;slot<4;++slot){const auto& a=slot?scene.actors[slot-1]:scene.systemActor;if(a.id<0 || a.id>3 || !registered[a.id])continue;const auto u=Units+uint32_t(a.id)*0x1000;
  half(u+0x108,a.maxHp);half(u+0x10c,a.hp);word(u+0x308,a.heldItem);word(u+0x2c,a.movePhase);byte(u+0x21,a.maxMoves);if(a.maxMoves==0){byte(u+0x22,0);}
  for(unsigned i=0;i<16;++i)word(u+0x218+4*i,a.work[i]);
  for(unsigned axis=0;axis<3;++axis){real(u+0x3c+4*axis,a.position[axis]);real(u+0x30+4*axis,a.home[axis]);}
  for(auto pair:std::array<std::pair<uint32_t,uint32_t>,6>{{{0x28c,a.wait},{0x298,a.phase},{0x2a0,a.attack},{0x2a4,a.confusion},{0x2b0,a.damage},{0x2b8,a.entry}}})word(u+pair.first,pair.second);
 }
}
void HarborTutorialPacked::capture(HarborTutorial& scene){
 scene.battleFlags=word(Battle+0xef4);
 for(unsigned slot=0;slot<4;++slot){auto& a=slot?scene.actors[slot-1]:scene.systemActor;if(a.id<0 || a.id>3 || !registered[a.id])continue;const auto u=Units+uint32_t(a.id)*0x1000;
  uint16_t v=0;required(vm.memory.readHalf(u+0x10c,v));a.hp=int16_t(v);required(vm.memory.readHalf(u+0x108,v));a.maxHp=int16_t(v);
  for(unsigned axis=0;axis<3;++axis){a.position[axis]=real(u+0x3c+4*axis);a.home[axis]=real(u+0x30+4*axis);}
  for(unsigned i=0;i<16;++i)a.work[i]=int32_t(word(u+0x218+4*i));
  a.maxMoves=signByte(byte(u+0x21));a.wait=word(u+0x28c);a.phase=word(u+0x298);a.attack=word(u+0x2a0);a.confusion=word(u+0x2a4);a.damage=word(u+0x2b0);a.entry=word(u+0x2b8);
 }
 scene.chosenUnit=chosenUnit;scene.chosenPart=chosenPart;
 for(auto v:rumbleRequests)scene.rumbleRequests.push_back(v);
 rumbleRequests.clear();
 for(auto& effect:damageEffects)scene.damageEffects.push_back(std::move(effect));
 damageEffects.clear();
 for(auto& notice:damageNotices)scene.damageNotices.push_back(std::move(notice));
 damageNotices.clear();
 for(auto& request:damageAudienceRequests)scene.damageAudienceRequests.push_back(std::move(request));
 damageAudienceRequests.clear();
 for(auto& r:sounds){r.time=scene.now;scene.soundRequests.push_back(std::move(r));}sounds.clear();
 auto& c=scene.camera;c.flags=word(Battle+0x2754);c.priority=int32_t(word(Battle+0x275c));c.mode=int32_t(word(Battle+0x2760));c.owner=int32_t(word(Battle+0x2764));c.target=int32_t(word(Battle+0x2768));c.ownerPart=int32_t(word(Battle+0x276c));c.targetPart=int32_t(word(Battle+0x2770));c.zoom=real(Battle+0x2778);uint16_t halfword=0;required(vm.memory.readHalf(Battle+0x277c,halfword));c.speed=int16_t(halfword);required(vm.memory.readHalf(Battle+0x277e,halfword));c.remaining=int16_t(halfword);for(unsigned i=0;i<3;++i){c.home[i]=real(Battle+0x2810+4*i);c.hit[i]=real(Battle+0x281c+4*i);}
}
bool HarborTutorialPacked::invoke(EventEntry& e,uint32_t function,int32_t first,int32_t& result){
 if(function==0x80107708 || function==0x80107758 || function==0x80107660 || function==0x80107548)return actionEvent(e,function,result);
 if(function==0x8010a78c){
  BattleCheckDamageEventServices services;services.units=nativeBattleUnitEventServices(vm);services.user=this;
  services.part=[](void* self,uint32_t u,int32_t id){auto& p=*static_cast<HarborTutorialPacked*>(self);return packedUnitPart(u,id,nativeBattleActorServices(p.vm).memory);};
  services.checkDamage=[](void* self,uint32_t a,uint32_t d,uint32_t part,uint32_t w,uint32_t flags,int32_t& out){return static_cast<HarborTutorialPacked*>(self)->checkDamageCore(a,d,part,w,flags,out);};
  services.eventExists=[](void* self,uint32_t id){auto& runtime=static_cast<HarborTutorialPacked*>(self)->vm;for(int32_t i=0;i<runtime.battle.capacity;++i)if(runtime.battle.entries[i].id==id && (runtime.battle.entries[i].flags&1))return true;return false;};
  result=battleCheckDamageEvent(e,services);return result!=0;
 }
 if(function==0x801093a8)marioJumpUnit=actor(e); // Native sound metadata captures the actual caller unit.
 if(registry.invoke(e,function,first,result))return true;
 switch(function){
 case 0x80106f40:{const auto on=value(e,0);const auto mask=uint32_t(raw(e,1));const auto flags=word(Battle+0x1f50);word(Battle+0x1f50,on?flags|mask:flags&~mask);result=2;return true;}
 case 0x80106ef0:{TimingACommand timing;const auto window=timing.successWindow(byte(Battle+0x1cbd));if(window<0)return false;output(e,0,window);result=2;return true;}
 case 0x80107788:result=packedBattleStartActionCommandEvent(e,nativeBattleUnitEventServices(vm),nativeBattleActorServices(vm));return true;
 case 0x8010e684:{BattleFacingServices s;s.positions=nativeBattlePartPositionServices(vm);s.user=vm.platform.user;s.signedWordFloat=vm.platform.signedWordFloat;s.subtract=vm.platform.subtractFloat;result=battleCalculateFaceDirectionEvent(e,s);return true;}
 case 0x8010e5a8:result=battleChangeFaceDirectionEvent(e,nativeBattleUnitEventServices(vm));return true;
 case 0x801058e0:output(e,1,byte(uint32_t(value(e,0))+0x6e));result=2;return true;
 case 0x80107b38:{const auto u=actor(e);const auto base=value(e,1);byte(Battle+0x1cbc,uint8_t(base));const auto adjusted=int32_t(uint32_t(base)-byte(u+0x305)+byte(u+0x306));byte(Battle+0x1cbd,uint8_t(std::clamp(adjusted,int32_t(0),int32_t(6))));result=2;return true;}
 case 0x801077ec:{
  const auto u=actor(e);const auto id=value(e,1),argument=value(e,3);const auto options=uint32_t(raw(e,2));
  word(Battle+0x1c90,u);word(Battle+0x1c94,options);word(Battle+0x1c9c,0);word(Battle+0x1cb4,0);word(Battle+0x1cb8,1);
  for(auto offset:{0x1ca0u,0x1ca8u,0x1ca4u,0x1cacu})word(Battle+offset,0);
  word(Battle+0x1c98,uint32_t(argument));word(Battle+0x1cc0,0);
  for(uint32_t record=0x80364348;word(record);record+=20)if(word(record)==uint32_t(id)){
   word(Battle+0x1ca0,word(record+4));if(!(word(u+0x27c)&0x10))word(Battle+0x1ca8,word(record+12));
   word(Battle+0x1ca4,word(record+8));word(Battle+0x1cac,word(record+16));break;
  }
  result=2;return true;
 }
 case 0x801059a0:{
  const auto u=actor(e),weapon=uint32_t(value(e,1));
  if(word(weapon+8) || byte(weapon+0x11) || byte(weapon+0x12))return false;
  const auto id=word(u);if(id>3)return false;readDamage(id);
  DamageWeapon w;w.itemId=0;w.fpCost=0;w.apCost=0;
  struct Payment{HarborTutorialPacked* p;uint32_t id,battle;};Payment c{this,id,0};
  WeaponCostServices cost;cost.user=&c;
  cost.captureWork=[](void* self)->void*{auto& c=*static_cast<Payment*>(self);c.battle=c.p->word(0x8041eb90);return &c.battle;};
  cost.getFp=[](void* self,DamageUnit& d){auto& p=*static_cast<Payment*>(self)->p;return getUnitFp(d,p.damageApplication());};
  cost.setFp=[](void* self,DamageUnit& d,int32_t fp){auto& c=*static_cast<Payment*>(self);setUnitFp(d,fp,c.p->damageApplication());c.p->writeDamage(c.id);d.kind=int32_t(c.p->word(Units+c.id*0x1000+8));};
  cost.point=[](void* self,void* work,uint32_t field,int32_t amount){auto& p=*static_cast<Payment*>(self)->p;const auto at=*static_cast<uint32_t*>(work)+field;auto v=p.byte(at);addActionPoint(v,amount);p.byte(at,v);};
  cost.count=[](void* self,void* work,uint32_t field){auto& p=*static_cast<Payment*>(self)->p;const auto at=*static_cast<uint32_t*>(work)+field;auto v=p.byte(at);addActionCount(v);p.byte(at,v);};
  cost.addAp=[](void* self,int32_t delta){auto& p=*static_cast<Payment*>(self)->p;const auto at=p.word(0x8041eb00);std::array<uint8_t,0x5d4> bytes{};for(unsigned j=0;j<bytes.size();++j)bytes[j]=p.byte(at+j);addPouchAp(bytes.data(),delta);p.byte(at+0x7a,bytes[0x7a]);p.byte(at+0x7b,bytes[0x7b]);};
  payWeaponCost(damageUnits[id],w,cost);result=2;return true;
 }
 case 0x8010ad00:{
  (void)actor(e);const auto target=translateBattleActor(&e,value(e,1),nativeBattleActorServices(vm));const auto part=value(e,2);
  // Complete wrapper in the proven no-protector domain. Query the live entire
  // registry; any matching protector retains the original unbound core.
  const auto b=word(0x8041eb90);
  for(unsigned id=0;id<64;++id){const auto u=word(b+0x20+4*id);if(u && int32_t(word(u+0x140))==target)return false;}
  output(e,1,target);output(e,2,part);chosenUnit=target;chosenPart=part;result=2;return true;
 }
 case 0x8010a500:case 0x8010a388:case 0x8010a55c:case 0x8010a32c:case 0x8010a4a4:{const auto u=actor(e);const uint32_t offset=function==0x8010a500?0x28c:function==0x8010a388?0x2a0:function==0x8010a55c?0x2b0:function==0x8010a32c?0x2a4:0x2b8;word(u+offset,uint32_t(raw(e,1)));if(function==0x8010a388)word(u+0x2ac,0);result=2;return true;}
 case 0x8010fce0:{const auto battle=word(0x8041eb90);const auto id=translateBattleActor(&e,value(e,0),nativeBattleActorServices(vm));if(id!=-1){const auto u=word(battle+0x20+4u*uint32_t(id));byte(u+0x21,uint8_t(value(e,1)));if(signByte(byte(u+0x22))>signByte(byte(u+0x21)))byte(u+0x22,byte(u+0x21));}result=2;return true;}
 case 0x80104b08:{const auto u=actor(e),launch=uint32_t(value(e,1)),land=uint32_t(value(e,2));word(u+0x1c4,launch);word(u+0x1c8,land);result=2;return true;}
 case 0x80104ddc:case 0x80104f64:{const auto u=actor(e);std::array<int32_t,5> args{};for(unsigned i=0;i<5;++i)args[i]=value(e,i+1);const auto bank=u+(function==0x80104ddc?0x1dc:0x1cc);word(bank,uint32_t(args[0]));word(bank+4,uint32_t(args[1]));half(bank+8,args[2]);half(bank+10,args[3]);half(bank+12,args[4]);result=2;return true;}
 case 0x8010f4cc:{const auto selector=value(e,0),id=translateBattleActor(&e,selector,nativeBattleActorServices(vm)),part=value(e,1),index=value(e,2),v=value(e,3);const auto u=word(Battle+0x20+4u*uint32_t(id)),p=packedUnitPart(u,part,nativeBattleActorServices(vm).memory);word(p+0x130+4u*uint32_t(index),uint32_t(v));result=2;return true;}
 case 0x801063f4:{const auto selector=value(e,0),id=translateBattleActor(&e,selector,nativeBattleActorServices(vm)),part=value(e,1);const auto u=word(Battle+0x20+4u*uint32_t(id)),p=packedUnitPart(u,part,nativeBattleActorServices(vm).memory);word(p+0x4d0,uint32_t(raw(e,2)));word(p+0x4d4,0);word(p+0x4d8,word(0x804226a4));word(p+0x4dc,word(0x804226a4));result=2;return true;}
 case 0x80145404:{if(!freshPouch)return false;(void)actor(e);const auto pouch=word(0x8041eb00);const auto memory=nativeBattleActorServices(vm).memory;const auto firstBadge=packedPouchEquippedBadgeCount(pouch,327,memory),secondBadge=packedPouchEquippedBadgeCount(pouch,328,memory);if(first && firstBadge==0 && secondBadge==0){result=2;return true;}return false;}
 case 0x80107694:{if(!physicalCurrent)return false;const auto u=actor(e),weapon=uint32_t(value(e,1));word(Battle+0x1cb0,0);DefenseContext context;uint16_t hp=0,override=0;required(vm.memory.readHalf(u+0x10c,hp));context.hp=int16_t(hp);context.simplifiers=byte(u+0x305);context.unsimplifiers=byte(u+0x306);context.automatic=byte(u+0x307);context.weaponGate=byte(weapon+0x13);required(vm.memory.readHalf(Battle+0x1d18,override));DefenseCommand command;command.overrideCounter=int16_t(override);auto pad=readPackedBattlePad(Battle+0x1d1c,nativeBattleActorServices(vm));command.evaluate(pad,context);half(Battle+0x1d18,command.overrideCounter);word(Battle+0x1cb0,command.result);writePackedBattlePad(Battle+0x1d1c,pad,nativeBattleActorServices(vm));result=2;return true;}

 case 0x8012b3d0:result=battleCameraShakeHorizontalEvent(e,camera);return true;
 case 0x8012bfd0:result=battleCameraModeEvent(e,camera);return true;
 case 0x8012bc70:result=battleCameraHomingPartsEvent(e,camera);return true;
 case 0x8012be38:result=battleCameraHomingEvent(e,camera);return true;
 case 0x8012b8cc:result=battleCameraSpeedEvent(e,camera);return true;
 case 0x8012ba14:result=battleCameraZoomEvent(e,camera);return true;
 case 0x8010b7c4:case 0x8010b85c:{const auto selector=value(e,0),id=translateBattleActor(&e,selector,nativeBattleActorServices(vm)),amount=value(e,2);const auto u=word(Battle+0x20+4u*uint32_t(id));const auto direction=signByte(byte(u+0x189)),x=value(e,1);const uint32_t delta=uint32_t(amount)*uint32_t(direction);output(e,1,int32_t(uint32_t(x)+(function==0x8010b85c?delta:0u-delta)));result=2;return true;}
 case 0x8010abac:{const auto a=actor(e);const auto defenderSelector=value(e,1),defenderId=translateBattleActor(&e,defenderSelector,nativeBattleActorServices(vm));const auto d=word(Battle+0x20+4u*uint32_t(defenderId));const auto p=packedUnitPart(d,value(e,2),nativeBattleActorServices(vm).memory),weapon=uint32_t(value(e,3));const auto flags=uint32_t(raw(e,4));
  BattlePackedHitPrecheckServices services;services.memory=nativeBattleActorServices(vm);services.user=this;services.status=[](void* self,uint32_t u,int32_t id){auto& p=*static_cast<HarborTutorialPacked*>(self);return checkPackedBattleStatus(u,id,nativeBattleActorServices(p.vm));};services.random=[](void* self,int32_t bound)->int32_t{return static_cast<HarborTutorialPacked*>(self)->random.bounded(bound);};services.multiply=[](void*,float a,float b){return a*b;};services.truncate=[](void* self,float v)->int32_t{auto& p=*static_cast<HarborTutorialPacked*>(self);return p.vm.platform.truncate(p.vm.platform.user,v);};
  const auto code=packedBattleHitPrecheck(a,d,p,weapon,flags,services);if(code<0)result=255;else{output(e,5,code);result=e.flags&1?2:255;}return true;}
 case 0x8010510c:result=battleStageFloorPositionEvent(e,nativeBattleUnitEventServices(vm),vm.platform.user,vm.platform.signedWordFloat);return true;
 case 0x8010d208:result=packedGenericJumpPositionEvent(e,first,jump);return true;
 case 0x8010d0f0:if(jump.floor){result=packedGenericJumpContinueEvent(e,first,jump);return true;}break;
 }

 if(function==0x801064d8){const auto u=actor(e),p=packedUnitPart(u,value(e,1),nativeBattleActorServices(vm).memory);word(p+0x204,word(p+0x204)&~uint32_t(2|0x10));word(p+0x204,word(p+0x204)|1);word(p+0x204,word(p+0x204)&~uint32_t(0x80));word(p+0x204,word(p+0x204)&~uint32_t(4));} // Fresh automatic pose projection remains in the presentation adapter.
 if(function==0x8010eed4){const auto u=actor(e);const auto b=signByte(byte(u+0xc));output(e,1,b==0?1:b==1?0:b);result=2;return true;}
 if(function==0x8010b664){const auto id=translateBattleActor(&e,value(e,0),nativeBattleActorServices(vm));const auto b=int8_t(value(e,1));const auto weapon=uint32_t(value(e,2)),attributes=word(weapon+0x68);int32_t bias=signByte(uint8_t(-byte(Battle+8u*uint32_t(int32_t(b))+0xa)));
  if(attributes&0x10000000)bias=signByte(uint8_t(-bias));
  const auto services=nativeBattleSamplingServices(vm);samplePackedBattleCandidates({nativeBattleActorServices(vm),Battle+0x428},weapon,id,b,word(weapon+0x64),attributes,bias,services);result=2;return true;}
 if(function==0x8010b5e4){const auto weapon=uint32_t(value(e,0));struct Outputs{HarborTutorialPacked* self;EventEntry* event;} outputs{this,&e};CandidateChoiceOutputs out;out.user=&outputs;
  out.write=[](void* p,bool unit,int32_t value){auto& o=*static_cast<Outputs*>(p);o.self->output(*o.event,unit?1:2,value);if(unit)o.self->chosenUnit=value;else o.self->chosenPart=value;};
  choosePackedBattleCandidate({nativeBattleActorServices(vm),Battle+0x428},word(weapon+0x7c),nativeBattleChoiceServices(vm),out);result=2;return true;}
 return false;
}
}
