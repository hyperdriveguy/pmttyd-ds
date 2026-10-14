#include "harbor_tutorial.hpp"
#include "message_assets.hpp"
#include "event_native_runtime.hpp"
#include "event_memory_bootstrap.hpp"
#include "event_lifecycle.hpp"
#include "field_camera_events.hpp"
#include "harbor_tutorial_packed.hpp"
#include "prologue_pouch.hpp"
#include <cassert>
#include <cstdio>
#include <cstring>
#include <unordered_map>
namespace ttyd {
namespace {
EventScriptAssets scripts;MessageAssets messages;MessageFont font;
std::array<uint32_t,12> source{};std::unordered_map<uint32_t,uint32_t> callbacks;bool ready=false;
int32_t signedWord(uint32_t v){return v<=0x7fffffff?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}
}
bool loadHarborTutorialAssets(const char* root){
 if(!root)return false;
 const std::string base=root;EventScriptAssets p;MessageAssets m;MessageFont f;
 if(!p.load((base+"/harbor-tutorial-events.pmevt").c_str()) || !m.loadGlobal((base+"/messages/global.txt").c_str()) || !m.loadRoom((base+"/messages/gor_00.txt").c_str()) || !f.load((base+"/font.pmfont").c_str()))return false;
 FILE* file=std::fopen((base+"/harbor-tutorial-program.bin").c_str(),"rb");if(!file)return false;
 char magic[8];decltype(source) entries{};uint32_t count=0;
 bool ok=std::fread(magic,8,1,file)==1 && !std::memcmp(magic,"PMHTUT01",8) && std::fread(entries.data(),4,12,file)==12 && std::fread(&count,4,1,file)==1 && count>0 && count<=37;
 std::unordered_map<uint32_t,uint32_t> table;
 for(uint32_t i=0;ok && i<count;++i){uint32_t a=0,t=0;ok=std::fread(&a,4,1,file)==1 && std::fread(&t,4,1,file)==1 && a && t<37 && table.emplace(a,t).second;}
 ok=ok && std::fgetc(file)==EOF;std::fclose(file);
 for(auto a:entries){uint32_t word=0;ok=ok && p.readWord(a,word);}
 uint32_t marioKind=0,parts=0;ok=ok && p.readWord(0x80416a50,marioKind) && p.readWord(marioKind+0xb8,parts) && p.readWord(parts,parts);
 for(unsigned id=0;ok && id<6;++id){char key[24];std::snprintf(key,sizeof(key),"mac_0_b_t1_%03u",id);MessagePlayback probe;ok=probe.open(key,m.find(key),f,0);}
 if(!ok)return false;
 scripts=std::move(p);messages=std::move(m);font=std::move(f);source=entries;callbacks=std::move(table);ready=true;return true;
}
struct HarborTutorialRuntime {
 HarborTutorial* scene=nullptr;EventNativeRuntime vm;std::unique_ptr<HarborTutorialPacked> packed;
 void* resourceUser=nullptr;bool(*paperReady)(void*,uint32_t,int32_t)=nullptr;bool(*niceReady)(void*,int32_t)=nullptr;
 void* fadeUser=nullptr;void(*fadeEntry)(void*,int32_t,int32_t,uint32_t)=nullptr;bool(*fadeFinished)(void*)=nullptr;
 uint32_t initRoot=0,phaseRoot=0,attackRoot=0,executingEvent=0,pendingEvent=0;uint64_t messagePoseSerial=0;bool openingDispatched=false,phaseInterrupted=false;
 std::array<uint64_t,3> completeEpoch{};std::array<bool,3> complete{};
 struct Interpolation{int32_t mode=0,start=0,end=0,limit=0,step=0;};
 std::unordered_map<uint32_t,Interpolation> interpolations;
 HarborTutorialRuntime():vm(0x81000000,platform(this)){}
 static EventNativePlatform platform(HarborTutorialRuntime* r){EventNativePlatform p;p.user=r;p.clock=[](void*)->uint32_t{return 243000000;};
 p.truncate=[](void*,float v)->int32_t{assert(std::isfinite(v) && double(v)>=-2147483648. && double(v)<2147483648.);return int32_t(v);};
 p.divideWord=[](void*,int32_t a,int32_t b)->int32_t{assert(b);return a/b;};p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){};
 p.irand=[](void* p,int32_t bound)->int32_t{return static_cast<HarborTutorialRuntime*>(p)->packed->random.bounded(bound);};
 p.signedWordFloat=[](void*,int32_t v){return float(v);};p.addFloat=[](void*,float a,float b){return a+b;};p.subtractFloat=[](void*,float a,float b){return a-b;};p.multiplyFloat=[](void*,float a,float b){return a*b;};p.divideFloat=[](void*,float a,float b){return a/b;};p.fmaFloat=[](void*,float a,float b,float c){return std::fma(a,b,c);};p.negativeMultiplySubtractFloat=[](void*,float a,float b,float c){return -std::fma(a,b,-c);};
 p.distanceFloat=[](void*,float a,float b,float c,float d){return std::hypot(c-a,d-b);};p.headingFloat=[](void*,float a,float b,float c,float d){return float(std::atan2(double(c-a),double(b-d))*180/3.141592653589793);};p.sineDouble=[](void*,double v){return std::sin(v);};p.cosineDouble=[](void*,double v){return std::cos(v);};p.roundDoubleFloat=[](void*,double v){return float(v);};
 p.call=[](void* r,EventEntry& e,uint32_t f,int32_t first)->int32_t{return static_cast<HarborTutorialRuntime*>(r)->call(e,f,first!=0);};return p;}
 uint32_t word(uint32_t a){uint32_t v=0;if(!vm.memory.readWord(a,v))scene->failed=true;return v;}
 int32_t raw(EventEntry& e,unsigned i){if(i>=e.argumentCount){scene->failed=true;return 0;}return signedWord(word(e.arguments+i*4));}
 int32_t value(EventEntry& e,unsigned i){return getEventValue(e,raw(e,i),vm.interpreterServices().operands);}
 float real(EventEntry& e,unsigned i){return getEventFloat(e,raw(e,i),vm.interpreterServices().operands);}
 void output(EventEntry& e,unsigned i,int32_t v){setEventValue(e,raw(e,i),v,vm.interpreterServices().operands);}
 std::string text(uint32_t a){std::string s;for(unsigned i=0;i<256;++i){uint8_t c=0;if(!vm.memory.readByte(a+i,c)){scene->failed=true;return {};}if(!c)return s;s+=char(c);}scene->failed=true;return {};}
 int32_t actor(EventEntry& e,unsigned i=0){int32_t v=value(e,i);if(v==-2)v=e.owner;else if(v==-3)v=1;const auto slot=scene->visualSlot(v);if(slot<0){scene->failed=true;return 0;}return slot;}
 EventEntry& root(uint32_t a,int32_t owner,uint32_t flags=0){auto services=vm.creationServices();auto& e=createRootEvent(vm.schedule,a,10,flags,services);e.owner=owner;return e;}
 bool exists(uint32_t id){for(int32_t i=0;i<vm.battle.capacity;++i)if(vm.battle.entries[i].id==id && (vm.battle.entries[i].flags&1))return true;return false;}
 bool tablePose(HarborTutorialActor& a,int32_t type){uint32_t table=a.id==1?source[11]:word(word(a.sourceKind+0xb8)+0x48);
 for(unsigned i=0;i<80;++i){const auto name=word(table+i*8+4);if(!name)break;if(word(table+i*8)==uint32_t(type)){a.pose=text(name);a.poseAt=scene->now;const auto slot=scene->visualSlot(a.id);if(slot>=0)complete[size_t(slot)]=false;return true;}}return false;}
 int pending(uint32_t function){if(!scene->pendingService){scene->pendingService=function;pendingEvent=executingEvent;}return 0;}
 int call(EventEntry& e,uint32_t function,bool first){
 executingEvent=e.id;
 if(first){HarborTutorialRequest r{function,e.currentCommand,scene->now,e.owner,{}};for(unsigned i=0;i<e.argumentCount;++i)r.rawOperands.push_back(raw(e,i));scene->requests.push_back(std::move(r));}
 if(packed){packed->publish(*scene);int32_t result=0;if(packed->invoke(e,function,first?1:0,result)){packed->capture(*scene);if(result!=0 && scene->pendingService==function){scene->pendingService=0;pendingEvent=0;}return result;}}
 if(function==0x80104498){const auto& a=scene->actors[actor(e)];if(a.heldItem!=0)return pending(function);if(scene->pendingService==function)scene->pendingService=0;output(e,1,0);return 2;}
 if(function==0x8011384c){output(e,0,(scene->battleFlags&uint32_t(raw(e,1)))!=0?1:0);return 2;}
 auto found=callbacks.find(function);if(found==callbacks.end())return pending(function);
 switch(found->second){
 case 0:case 1:case 2:case 3:case 4:{auto& a=scene->actors[actor(e)];const auto script=uint32_t(value(e,1));switch(found->second){case 0:a.wait=script;break;case 1:a.attack=script;break;case 2:a.damage=script;break;case 3:a.confusion=script;break;case 4:a.phase=script;break;}return 2;}
 case 5:if(value(e,0)==1)scene->battleFlags|=uint32_t(raw(e,1));else scene->battleFlags&=~uint32_t(raw(e,1));return 2;
 case 6:{if(value(e,1)!=int32_t(source[5]) || value(e,2)!=0 || scene->actors[2].id!=-1)return pending(function);
 auto& a=scene->actors[2];a.id=HarborTutorial::HelperId;a.sourceKind=word(source[5]);if(a.sourceKind!=source[7]){scene->failed=true;return 0;}
 a.kind=signedWord(word(a.sourceKind));a.belong=int8_t(word(source[5]+4)>>24);a.movePhase=word(source[5]+8);a.hp=a.maxHp=int16_t(word(a.sourceKind+8)>>16);a.pose="PKR_S_1";for(unsigned i=0;i<3;++i){const auto bits=word(source[5]+0xc+4*i);std::memcpy(&a.position[i],&bits,4);}a.home=a.position;
 if(!packed || !packed->registerActor(a)){scene->failed=true;return 0;}
 output(e,0,a.id);packed->recordInit(a.id,root(source[3],a.id).id);return 2;}
 case 7:{auto& a=scene->actors[actor(e)];const auto i=value(e,1);if(i<0 || i>=16){scene->failed=true;return 0;}a.work[i]=value(e,2);return 2;}
 case 8:case 9:{auto& a=scene->actors[actor(e)];auto& p=found->second==8?a.position:a.home;for(unsigned i=0;i<3;++i)p[i]=real(e,i+1);return 2;}
 case 10:case 11:case 17:case 18:case 28:return 2; // Retain source audio/camera requests for presentation consumers.
 case 12:{const auto id=actor(e);auto& a=scene->actors[id];if(a.wait)root(a.wait,a.id);return 2;}
 case 13:output(e,0,int16_t(scene->turn));return 2;
 case 14:output(e,0,scene->phaseWord==uint32_t(raw(e,1)));return 2;
 case 15:return 2; // Fresh installed wait scripts delete themselves; no live loop to cancel.
 case 16:{const auto id=actor(e);auto& a=scene->actors[id];a.pose=text(uint32_t(value(e,2)));a.poseAt=scene->now;complete[id]=false;return 2;}
 case 19:interpolations[e.id]={value(e,0),value(e,1),value(e,2),value(e,3),0};return 2;
 case 20:{auto i=interpolations.find(e.id);if(i==interpolations.end()){scene->failed=true;return 0;}auto& p=i->second;
 const auto sample=cameraInterpolation(p.mode,p.step,p.limit,float(p.start),float(p.end));if(!sample)return pending(function);
 e.locals[0]=int32_t(*sample);e.locals[1]=p.step<p.limit?1:0;++p.step;return 2;}
 case 21:{auto& a=scene->actors[actor(e)];for(unsigned i=0;i<3;++i){const auto v=raw(e,i+1);if(v!=-1000000000)a.rotation[i]=float(value(e,i+1));}return 2;}
 case 22:return exists(uint32_t(value(e,0)))?0:2;
 case 23:scene->statusVisible=value(e,0)!=0;return 2;
 case 24:{auto& a=scene->actors[actor(e)];const auto i=value(e,1);if(i<0 || i>=16){scene->failed=true;return 0;}output(e,2,a.work[i]);return 2;}
 case 25:scene->actors[actor(e)].talkPose=text(uint32_t(value(e,1)));return 2;
 case 26:scene->actors[actor(e)].stayPose=text(uint32_t(value(e,1)));return 2;
 case 27:if(first){const auto key=text(uint32_t(value(e,1)));const auto id=actor(e,3);scene->speaker=scene->actors[id].id;
 if(!scene->message.open(key.c_str(),messages.find(key.c_str()),font,scene->now)){scene->failed=true;return 0;}scene->messageOrder.push_back(key);}
 return scene->message.active()?0:2;
 case 29:{const auto id=actor(e);if(first)e.waitCount=1;if(e.waitCount>0){--e.waitCount;return 0;}if(!complete[id] || completeEpoch[id]!=scene->actors[id].poseAt)return pending(function);scene->pendingService=0;return 2;}
 case 30:{auto& a=scene->actors[actor(e)];if(value(e,1)!=1 || !tablePose(a,value(e,2)))return pending(function);return 2;}
 case 31:output(e,1,scene->actors[actor(e)].maxHp);return 2;
 case 32:output(e,1,scene->actors[actor(e)].hp);return 2;
 case 33:output(e,0,scene->successDeclarations);return 2;
 case 34:output(e,0,scene->failureDeclarations);return 2;
 case 35:{auto& a=scene->actors[actor(e)];if(value(e,1)!=1)return pending(function);if(!tablePose(a,28))return pending(function);return 2;}
 case 36:scene->actors[actor(e)].maxMoves=value(e,1);return 2;
 }
 return pending(function);
 }
 bool initialize(){if(!vm.memory.registerRam(0x82000000,std::vector<uint8_t>(1024*1024)) || !vm.memory.registerRam(0x8041f070,std::vector<uint8_t>(16)) || !vm.memory.registerRam(0x8041e428,std::vector<uint8_t>(4)) || !vm.memory.registerRam(0x8041e844,std::vector<uint8_t>(4)) || !vm.memory.importScripts(scripts))return false;
 auto m=vm.memoryBinding.services();auto start=eventSdkInitializeAllocator(0x82000000,0x82100000,1,m);if(eventSdkCreateHeap(start,0x82100000,m)!=0 || !vm.memory.writeWord(0x8041e844,0) || !vm.memory.writeWord(0x81000004,60))return false;
 vm.gp.timestamp=scene->now*eventClockRate(243000000);vm.initializeContexts();vm.gp.battleContext=1;packed=std::make_unique<HarborTutorialPacked>(vm);if(!packed->provision())return false;return true;}
};
bool HarborTutorial::begin(uint64_t time,int32_t hp,int32_t maxHp){return beginWithContext(time,hp,maxHp,false,0);}
bool HarborTutorial::beginFresh(uint64_t time,int32_t hp,int32_t maxHp,int32_t story){return beginWithContext(time,hp,maxHp,true,story);}
bool HarborTutorial::beginFromPouch(uint64_t time,const ProloguePouch& entry,int32_t story){return beginWithContext(time,entry.half(0x70),entry.half(0x72),true,story,&entry);}
bool HarborTutorial::beginWithContext(uint64_t time,int32_t hp,int32_t maxHp,bool freshPouch,int32_t story,const ProloguePouch* entry){if(!ready || triggered)return false;*this=HarborTutorial{};triggered=true;now=time;
 actors[0].id=CrumpId;actors[0].sourceKind=source[6];actors[1].id=MarioId;actors[1].kind=222;actors[1].movePhase=0x04000002;actors[1].hp=hp;actors[1].maxHp=maxHp;actors[1].pose="M_S_1";actors[1].position=actors[1].home={-100,0,-5};
 runtime_=std::make_shared<HarborTutorialRuntime>();auto& r=*runtime_;r.scene=this;if(!r.initialize()){failed=true;return false;}actors[0].kind=signedWord(r.word(source[6]));actors[0].belong=int8_t(r.word(source[8]+4)>>24);actors[0].movePhase=r.word(source[8]+8);actors[0].hp=actors[0].maxHp=int16_t(r.word(source[6]+8)>>16);
 for(unsigned i=0;i<3;++i){const auto v=r.word(source[8]+0xc+i*4);std::memcpy(&actors[0].position[i],&v,4);}actors[0].home=actors[0].position;
 // Source mandatory entry order is system, Mario, then the setup2 opponent.
 // Registry storage is still the bounded native adapter; this is not a claim
 // that complete UnitEntry/display/equipment services have been installed.
 systemActor.id=SystemId;systemActor.sourceKind=r.word(0x80416a80);
 systemActor.kind=signedWord(r.word(systemActor.sourceKind));
 systemActor.hp=systemActor.maxHp=int16_t(r.word(systemActor.sourceKind+8)>>16);
 systemActor.belong=int8_t(r.word(0x80416a84)>>24);systemActor.movePhase=r.word(0x80416a88);
 for(unsigned i=0;i<3;++i){const auto bits=r.word(0x80416a8c+4*i);std::memcpy(&systemActor.position[i],&bits,4);}systemActor.home=systemActor.position;
 if(!r.packed->registerActor(systemActor)){failed=true;return false;}
 r.packed->recordInit(SystemId,r.root(r.word(systemActor.sourceKind+0xbc),SystemId).id);
 if(!r.packed->registerActor(actors[1])){failed=true;return false;}
 if(freshPouch){if(!r.packed->provisionFreshPouch(messages.find("name_party3"),entry)){failed=true;return false;}uint32_t pouch=0;if(!r.vm.memory.readWord(0x8041eb00,pouch) || !r.vm.memory.writeHalf(pouch+0x70,uint16_t(hp))){failed=true;return false;}if(entry){r.packed->importMarioPouch();r.packed->capture(*this);}
 writeSavedSwitch(SavedSwitchFamily::GlobalByte,0,story,r.vm.interpreterServices().operands);if(!r.packed->provisionFreshFieldInformation(source[9]+0x60eb8-0x60c90,source[9],source[10])){failed=true;return false;}r.packed->recordInit(1,r.root(r.word(actors[1].sourceKind+0xbc),1).id);}
 if(!r.packed->registerActor(actors[0])){failed=true;return false;}
 r.initRoot=r.root(source[0],CrumpId).id;r.packed->recordInit(CrumpId,r.initRoot);
 tick(time,0);return !failed;}
int32_t HarborTutorial::visualSlot(int32_t id)const{if(id<0)return -1;for(unsigned i=0;i<actors.size();++i)if(actors[i].id==id)return int32_t(i);return -1;}
HarborTutorialActor* HarborTutorial::actorById(int32_t id){if(id==SystemId && systemActor.id==SystemId)return &systemActor;const auto slot=visualSlot(id);return slot<0?nullptr:&actors[size_t(slot)];}
const HarborTutorialActor* HarborTutorial::actorById(int32_t id)const{if(id==SystemId && systemActor.id==SystemId)return &systemActor;const auto slot=visualSlot(id);return slot<0?nullptr:&actors[size_t(slot)];}
void HarborTutorial::setStartupResources(void* user,bool(*paper)(void*,uint32_t,int32_t),bool(*nice)(void*,int32_t)){if(runtime_){runtime_->resourceUser=user;runtime_->paperReady=paper;runtime_->niceReady=nice;}}
void HarborTutorial::setStartupFade(void* user,void(*entry)(void*,int32_t,int32_t,uint32_t),bool(*finished)(void*)){if(runtime_){runtime_->fadeUser=user;runtime_->fadeEntry=entry;runtime_->fadeFinished=finished;}}
void HarborTutorial::tick(uint64_t time,uint32_t keys,const BattlePadSample* physical){if(!runtime_ || failed || time<now)return;now=time;auto& r=*runtime_;r.scene=this;if(message.active())message.tick(now,keys);
 const auto speakerSlot=visualSlot(speaker);
 if(message.poseRequestSerial!=r.messagePoseSerial && speakerSlot>=0){r.messagePoseSerial=message.poseRequestSerial;auto& a=actors[size_t(speakerSlot)];const auto& pose=message.poseRequest==MessagePlayback::PoseRequest::Talk?a.talkPose:a.stayPose;if(!pose.empty()){a.pose=pose;a.poseAt=message.poseRequestedAt;}}
 r.packed->samplePhysical(time,physical);r.packed->publish(*this);r.vm.gp.timestamp=now*eventClockRate(243000000);r.vm.runManager();r.packed->publish(*this);r.packed->capture(*this);
 damageActive=r.packed->hitEventsActive();
 if(!initialized && r.packed->unitInitReady())initialized=true;
 if(initialized && !r.openingDispatched && dispatchPhase(1,0x04000001))r.openingDispatched=true;
 if(attackActive && !r.exists(r.attackRoot))attackActive=false;
 if(phaseActive && !r.exists(r.phaseRoot)){phaseActive=false;if(r.openingDispatched && !r.phaseInterrupted && phaseWord==0x04000001)introComplete=true;}
}
bool HarborTutorial::dispatchPhase(int32_t sourceTurn,uint32_t sourcePhase){if(!runtime_ || failed || !initialized || phaseActive || attackActive || damageActive || !actors[0].phase)return false;turn=sourceTurn;phaseWord=sourcePhase;runtime_->phaseInterrupted=false;runtime_->phaseRoot=runtime_->root(actors[0].phase,CrumpId,0x20).id;runtime_->packed->recordPhase(CrumpId,runtime_->phaseRoot);phaseActive=true;return true;}
bool HarborTutorial::dispatchInstalledAttack(int32_t id){const auto slot=visualSlot(id);if(!runtime_ || failed || !initialized || !introComplete || phaseActive || attackActive || damageActive || (id!=CrumpId && id!=HelperId) || slot<0 || !actors[size_t(slot)].attack)return false;pendingService=0;runtime_->attackRoot=runtime_->root(actors[size_t(slot)].attack,id).id;runtime_->packed->recordAttack(id,runtime_->attackRoot);attackActive=true;return true;}
uint32_t HarborTutorial::dispatchHit(int32_t id,uint32_t code,uint32_t script,bool direct){
 if(!runtime_ || failed || !actorById(id))return 0;
 auto& r=*runtime_;r.scene=this;r.packed->publish(*this);
 const auto root=direct?r.packed->runHitEventDirect(id,code,script):r.packed->runHitEvent(id,code);
 bool interrupted=false;
 if(phaseActive && !r.exists(r.phaseRoot)){r.phaseInterrupted=true;interrupted=true;}
 if(attackActive && !r.exists(r.attackRoot)){attackActive=false;interrupted=true;}
 if(interrupted || (r.pendingEvent && !r.exists(r.pendingEvent))){pendingService=0;r.pendingEvent=0;}
 damageActive=r.packed->hitEventsActive();r.packed->capture(*this);return root;
}
uint32_t HarborTutorial::dispatchHitEvent(int32_t id,uint32_t code){return dispatchHit(id,code,0,false);}
uint32_t HarborTutorial::dispatchDirectHitEvent(int32_t id,uint32_t code,uint32_t script){return dispatchHit(id,code,script,true);}
bool HarborTutorial::buildWeaponMenu(uint32_t type){
 if(!runtime_ || failed || !initialized || !introComplete || phaseActive || attackActive || damageActive)return false;
 auto& r=*runtime_;r.scene=this;r.packed->publish(*this);
 const bool built=r.packed->buildWeaponMenu(type,*this,&r,[](void* self,uint32_t key){auto& r=*static_cast<HarborTutorialRuntime*>(self);const auto id=r.text(key);return r.packed->cacheMenuLabel(key,messages.find(id.c_str()));});
 r.packed->capture(*this);return built;
}
bool HarborTutorial::selectWeapon(unsigned row){
 if(!runtime_ || failed || !initialized || !introComplete || phaseActive || attackActive || damageActive || row>=weaponRows.size() || !weaponRows[row].enabled)return false;
 auto& r=*runtime_;r.scene=this;r.packed->publish(*this);
 const bool selected=r.packed->selectWeapon(row,*this,&r,[](void* self,uint32_t key){auto& r=*static_cast<HarborTutorialRuntime*>(self);const auto id=r.text(key);return r.packed->cacheMenuLabel(key,messages.find(id.c_str()));});r.packed->capture(*this);return selected;
}
bool HarborTutorial::readPackedWord(uint32_t address,uint32_t& value)const{return runtime_ && runtime_->packed && runtime_->packed->readWord(address,value);}
void HarborTutorial::animationComplete(int32_t id){const auto slot=visualSlot(id);if(!runtime_ || slot<0)return;runtime_->complete[size_t(slot)]=true;runtime_->completeEpoch[size_t(slot)]=actors[size_t(slot)].poseAt;}
}
