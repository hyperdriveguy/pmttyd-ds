#include "harbor_tutorial.hpp"
#include "harbor_native_command_menu.hpp"
#include <unordered_set>
#include "harbor_source_audience.hpp"
#include "battle_class_menu_messages.hpp"
#include <cstdlib>
#include "native_battle_sound.hpp"
#include "native_harbor_completion.hpp"
#include "native_harbor_reward_counter.hpp"
#include "battle_data_child.hpp"
#include "native_harbor_stylish.hpp"
#include "native_harbor_nice.hpp"
#include "native_harbor_smoke.hpp"
#include "native_harbor_star_point.hpp"
#include "battle_unit_coin.hpp"
#include "battle_unit_exp.hpp"
#include "battle_star_point_offset.hpp"
#include "native_harbor_nice_visual.hpp"
#include "native_ui_startup.hpp"
#include "event_gp_language_alias.hpp"
#include <map>
#include "battle_command_palette_memory.hpp"
#include "message_assets.hpp"
#include "event_native_runtime.hpp"
#include "event_memory_bootstrap.hpp"
#include "event_lifecycle.hpp"
#include "field_camera_events.hpp"
#include "harbor_tutorial_packed.hpp"
#include "prologue_pouch.hpp"
#include "battle_attack_registration.hpp"
#include "battle_status_window.hpp"
#include "battle_conclusion.hpp"
#include "battle_packed_takeoff.hpp"
#include "battle_event_callbacks.hpp"
#include "battle_packed_status.hpp"
#include "battle_sequence_startup.hpp"
#include "battle_menu_file_cache.hpp"
#include <cassert>
#include <cstdio>
#include <cstring>
#include <unordered_map>
namespace ttyd {
namespace {
EventScriptAssets scripts;MessageAssets messages;MessageFont font;
std::shared_ptr<BattleMenuFileCache> menuFiles;
// PMHTUT01 catalog has 12 source pointers and at most 37 callback indices.
// Indices are converter dispatch IDs, not source opcodes or native function
// pointers. Asset reload publishes only after complete validation below.
std::array<uint32_t,12> source{};std::unordered_map<uint32_t,uint32_t> callbacks;bool ready=false;
int32_t signedWord(uint32_t v){return v<=0x7fffffff?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}
}
// Load original global/gor_00 US messages and font with the source EVT pack;
// probe all six mac_0_b_t1_000..005 messages. No dialogue is authored here.
// Catalog is native little-endian 32-bit data; reject duplicates/trailing bytes,
// invalid callback IDs or unreadable roots/Mario part table before publication.
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
 auto files=std::make_shared<BattleMenuFileCache>();
 if(!files->load((base+"/battle-class-common.tpl").c_str()))return false;
 if(!stageHarborAudienceFiles(*files->files(),base.c_str()))return false;
 menuFiles=std::move(files);
 scripts=std::move(p);messages=std::move(m);font=std::move(f);source=entries;callbacks=std::move(table);ready=true;return true;
}
struct HarborSourceMenuRegistration {std::weak_ptr<HarborTutorialRuntime> runtime;};
struct HarborTutorialRuntime {
 std::shared_ptr<HarborSourceDisplayPass> sourceDisplay;
 std::shared_ptr<HarborSourceMenuFactory> sourceMenuFactory;
 std::shared_ptr<const MessageAssets> sourceMenuMessages=std::make_shared<const MessageAssets>(messages);
 // Shared backing outlives this VM and its borrowed external mappings. Asset
 // reload replaces the catalog owner without invalidating an existing battle.
 std::shared_ptr<BattleMenuFileCache> files=menuFiles;
 HarborTutorial* scene=nullptr;EventNativeRuntime vm;std::unique_ptr<HarborTutorialPacked> packed;
 // Immutable native admission identity, distinct from mutable scene projection.
 const HarborTutorial* nativeAdmissionFacade=nullptr;NativeFacadeRegistration::Proof nativeAdmissionProof;
 std::shared_ptr<const uint8_t>statusLife=std::make_shared<const uint8_t>(0);std::unique_ptr<NativeBattleStatusReader>statusReader;
 uint32_t statusBattle=0;std::shared_ptr<const uint8_t>statusBattleOwner;
 std::unique_ptr<BattleMenuFileBinding> menuBinding;BattleMenuResourceCursor menuCursor;
 std::weak_ptr<NativeHarborCompletion> nativeCompletion;bool nativeCompletionFrozen=false;
 std::weak_ptr<NativeHarborRewardCounter> nativeRewardCounter;bool nativeRewardCounterMinted=false;
 uint32_t nativeVictoryUnit=0,nativeVictoryRoot=0;bool nativeVictoryTracked=false;
 std::shared_ptr<BattleCommandPalette> commandPalette;
 void* resourceUser=nullptr;bool(*paperReady)(void*,uint32_t,int32_t)=nullptr;bool(*niceReady)(void*,int32_t)=nullptr;
 void* fadeUser=nullptr;void(*fadeEntry)(void*,int32_t,int32_t,uint32_t)=nullptr;bool(*fadeFinished)(void*)=nullptr;
 uint32_t initRoot=0,phaseRoot=0,attackRoot=0,executingEvent=0,pendingEvent=0;uint64_t messagePoseSerial=0;bool openingDispatched=false,phaseInterrupted=false;
 std::array<uint64_t,3> completeEpoch{};std::array<bool,3> complete{};
 // One interpolation state per source event ID; limit/step are source loop
 // counts, not screen frames. Pose-completion epochs are separate millisecond
 // generations so an old finished animation cannot acknowledge a new pose.
 struct Interpolation{int32_t mode=0,start=0,end=0,limit=0,step=0;};
 std::unordered_map<uint32_t,Interpolation> interpolations;
 struct NiceCall {NativeHarborNiceCursor cursor;std::map<uint32_t,uint32_t>entryReads;};
 std::map<uint64_t,NiceCall>niceCalls;NiceCall*niceInvoking=nullptr;
 NativeHarborNiceConsumer nativeNice;std::shared_ptr<NativeUiStartup>niceStartup;
 std::shared_ptr<EventGpLanguageAlias>niceLanguage;
 bool niceCurrent()const{return niceStartup&&niceStartup->current()&&niceLanguage&&niceLanguage->current()&&nativeNice.lifetime&&nativeNice.current&&nativeNice.current(nativeNice.user);}
 std::optional<uint64_t>niceSpawn(const NativeHarborNiceSpawn&q){
  if(!niceInvoking||!niceCurrent()||!nativeNice.spawn||q.type>4)return {};
  // Captured source Entry/Draw resource operands, separate from the exact344
  // parent. Native allocation/playback adapts the effect-pool/GX lifecycle.
  auto load=[&](uint32_t a)->std::optional<uint32_t>{auto&cache=niceInvoking->entryReads;auto i=cache.find(a);if(i!=cache.end())return i->second;uint32_t v;if(!vm.memory.readWord(a,v))return {};cache.emplace(a,v);return v;};
  const auto gp=load(0x80415fd0);if(!gp)return {};
  const auto language=load(*gp+0x16c);if(!language)return {};
  const auto mode=load(*gp+0x14);if(!mode)return {};
  const auto names=load(0x80417af8+4u**language);if(!names)return {};
  const auto key=load(*names+4*q.type);if(!key)return {};
  const auto animation=load(0x80422ed0);if(!animation||*animation!=0x415f3100)return {};
  const auto base=load(0x80422eb8);if(!base)return {};
  NativeHarborNiceVisualRequest request;request.effect=q;request.sourceKey=*key;request.context=*mode!=0;request.language=*language;
  std::memcpy(&request.modelScale,&*base,4);
  if(*language==1&&(q.type==3||q.type==4)){const auto extra=load(0x80422ebc);if(!extra)return {};float factor;std::memcpy(&factor,&*extra,4);request.modelScale*=factor;}
  const auto fade=load(0x802f3ea0+24*q.type+4u**language);if(!fade)return {};
  std::memcpy(&request.fadeStart,&*fade,4);request.milliseconds=scene->now;
  return nativeNice.spawn(nativeNice.user,request);
 }
 int niceEffect(EventEntry&e){
  if(!niceCurrent()||!packed||!packed->sourceSound||niceInvoking)return pending(0x80106fa0);
  const uint64_t key=(uint64_t(e.id)<<32)|e.currentCommand;auto&call=niceCalls[key];
  niceInvoking=&call;struct Guard{NiceCall*&p;~Guard(){p=nullptr;}}guard{niceInvoking};
  NativeHarborNiceServices s;s.user=this;s.lifetime=nativeNice.lifetime;
  s.current=[](void*p){return static_cast<HarborTutorialRuntime*>(p)->niceCurrent();};
  s.word=[](void*p,uint32_t a)->std::optional<uint32_t>{uint32_t v;if(!static_cast<HarborTutorialRuntime*>(p)->vm.memory.readWord(a,v))return {};return v;};
  s.value=[](void*p,EventEntry&e,int32_t raw)->std::optional<int32_t>{return getEventValue(e,raw,static_cast<HarborTutorialRuntime*>(p)->vm.interpreterServices().operands);};
  s.spawn=[](void*p,const NativeHarborNiceSpawn&q){return static_cast<HarborTutorialRuntime*>(p)->niceSpawn(q);};
  s.scale=[](void*p,uint64_t id,float value){auto&r=*static_cast<HarborTutorialRuntime*>(p);return r.nativeNice.scale&&r.nativeNice.scale(r.nativeNice.user,id,value);};
  s.sound=[](void*p,uint32_t name,const std::array<float,3>&pos){return static_cast<HarborTutorialRuntime*>(p)->packed->sourceSound->position(name,pos);};
  s.pitch=[](void*p,int32_t handle,int32_t value){return static_cast<HarborTutorialRuntime*>(p)->packed->sourceSound->pitch(handle,value);};
  if(!resumeNativeHarborNice(e,s,call.cursor))return pending(0x80106fa0);
  niceCalls.erase(key);if(scene->pendingService==0x80106fa0){scene->pendingService=0;pendingEvent=0;}return 2;
 }
 std::unordered_map<uint64_t,NativeHarborStylishCursor>nativeStylish;
 struct SmokeSource {std::weak_ptr<HarborTutorialRuntime>runtime;};
 NativeHarborSmokeConsumer nativeSmoke;NativeHarborSmokeSource smokeSource;
 std::map<uint64_t,NativeHarborSmokeCursor>smokeCalls;uint64_t smokeVisit=0;
 bool smokeCurrent()const{return nativeSmoke.lifetime&&nativeSmoke.current&&nativeSmoke.current(nativeSmoke.user);}
 int smokeEffect(EventEntry&e){
  if(!packed||!smokeCurrent())return pending(0x8006d970);
  const uint64_t key=(uint64_t(e.id)<<32)|e.currentCommand;auto&call=smokeCalls[key];
  NativeHarborSmokeServices s;s.user=this;s.lifetime=nativeSmoke.lifetime;
  s.current=[](void*p){return static_cast<HarborTutorialRuntime*>(p)->smokeCurrent();};
  s.word=[](void*p,uint32_t at)->std::optional<uint32_t>{uint32_t v;if(!static_cast<HarborTutorialRuntime*>(p)->vm.memory.readWord(at,v))return {};return v;};
  s.byte=[](void*p,uint32_t at)->std::optional<uint8_t>{uint8_t v;if(!static_cast<HarborTutorialRuntime*>(p)->vm.memory.readByte(at,v))return {};return v;};
  s.value=[](void*p,EventEntry&e,int32_t raw)->std::optional<int32_t>{return getEventValue(e,raw,static_cast<HarborTutorialRuntime*>(p)->vm.interpreterServices().operands);};
  s.floating=[](void*p,EventEntry&e,int32_t raw)->std::optional<float>{return getEventFloat(e,raw,static_cast<HarborTutorialRuntime*>(p)->vm.interpreterServices().operands);};
  s.spawn=[](void*p,const NativeHarborSmokeRequest&q){auto&r=*static_cast<HarborTutorialRuntime*>(p);return r.nativeSmoke.spawn?r.nativeSmoke.spawn(r.nativeSmoke.user,q,r.smokeSource):std::optional<uint64_t>{};};
  if(!resumeNativeHarborSmoke(e,s,call))return pending(0x8006d970);
  smokeCalls.erase(key);if(scene->pendingService==0x8006d970){scene->pendingService=0;pendingEvent=0;}return 2;
 }
 std::map<uint64_t,BattleUnitCoinCursor>coinCalls;
 int coinEvent(EventEntry&e,uint32_t function){
  if(!packed)return pending(function);
  const uint64_t key=(uint64_t(e.id)<<32)|e.currentCommand;auto&cursor=coinCalls[key];
  BattleUnitCoinServices s;s.user=this;s.lifetime=statusLife;
  s.current=[](void*p){auto&r=*static_cast<HarborTutorialRuntime*>(p);uint32_t b=0;return r.packed&&r.scene&&!r.scene->failed&&r.vm.memory.readWord(0x8041eb90,b)&&b==r.packed->battleAddress();};
  s.word=[](void*p,uint32_t at)->std::optional<uint32_t>{uint32_t v;if(!static_cast<HarborTutorialRuntime*>(p)->vm.memory.readWord(at,v))return {};return v;};
  s.byte=[](void*p,uint32_t at)->std::optional<uint8_t>{uint8_t v;if(!static_cast<HarborTutorialRuntime*>(p)->vm.memory.readByte(at,v))return {};return v;};
  s.value=[](void*p,EventEntry&e,int32_t raw)->std::optional<int32_t>{return getEventValue(e,raw,static_cast<HarborTutorialRuntime*>(p)->vm.interpreterServices().operands);};
  s.translate=[](void*p,EventEntry&e,int32_t selector)->std::optional<int32_t>{auto&r=*static_cast<HarborTutorialRuntime*>(p);return translateBattleActor(&e,selector,nativeBattleActorServices(r.vm));};
  s.random=[](void*p,int32_t bound)->std::optional<int32_t>{return static_cast<HarborTutorialRuntime*>(p)->packed->random.bounded(bound);};
  s.store=[](void*p,EventEntry&e,int32_t destination,int32_t value){setEventValue(e,destination,value,static_cast<HarborTutorialRuntime*>(p)->vm.interpreterServices().operands);return true;};
  s.writeWord=[](void*p,uint32_t at,uint32_t value){return static_cast<HarborTutorialRuntime*>(p)->vm.memory.writeWord(at,value);};
  auto result=function==0x8010d914?resumeBattleGetCoinEvent(e,s,cursor):resumeBattleStoreCoinEvent(e,s,cursor);if(!result)return pending(function);
  coinCalls.erase(key);if(scene->pendingService==function){scene->pendingService=0;pendingEvent=0;}return *result;
 }
 std::map<uint64_t,BattleUnitExpCursor>expCalls;
 int expEvent(EventEntry&e,uint32_t function){
  if(!packed)return pending(function);
  const uint64_t key=(uint64_t(e.id)<<32)|e.currentCommand;auto&cursor=expCalls[key];
  BattleUnitExpServices s;s.user=this;s.lifetime=statusLife;
  s.current=[](void*p){auto&r=*static_cast<HarborTutorialRuntime*>(p);uint32_t b=0;return r.packed&&r.scene&&!r.scene->failed&&r.vm.memory.readWord(0x8041eb90,b)&&b==r.packed->battleAddress();};
  s.word=[](void*p,uint32_t at)->std::optional<uint32_t>{uint32_t v;if(!static_cast<HarborTutorialRuntime*>(p)->vm.memory.readWord(at,v))return {};return v;};
  s.byte=[](void*p,uint32_t at)->std::optional<uint8_t>{uint8_t v;if(!static_cast<HarborTutorialRuntime*>(p)->vm.memory.readByte(at,v))return {};return v;};
  s.half=[](void*p,uint32_t at)->std::optional<uint16_t>{uint16_t v;if(!static_cast<HarborTutorialRuntime*>(p)->vm.memory.readHalf(at,v))return {};return v;};
  s.doubleBits=[](void*p,uint32_t at)->std::optional<uint64_t>{auto&r=*static_cast<HarborTutorialRuntime*>(p);uint32_t high,low;if(!r.vm.memory.readWord(at,high)||!r.vm.memory.readWord(at+4,low))return {};return (uint64_t(high)<<32)|low;};
  s.value=[](void*p,EventEntry&e,int32_t raw)->std::optional<int32_t>{return getEventValue(e,raw,static_cast<HarborTutorialRuntime*>(p)->vm.interpreterServices().operands);};
  s.translate=[](void*p,EventEntry&e,int32_t selector)->std::optional<int32_t>{auto&r=*static_cast<HarborTutorialRuntime*>(p);return translateBattleActor(&e,selector,nativeBattleActorServices(r.vm));};
  s.subtractDoubleFloat=[](void*p,double a,double b)->std::optional<float>{auto&r=*static_cast<HarborTutorialRuntime*>(p);if(!r.vm.platform.subtractDouble||!r.vm.platform.roundDoubleFloat)return {};return r.vm.platform.roundDoubleFloat(r.vm.platform.user,r.vm.platform.subtractDouble(r.vm.platform.user,a,b));};
  s.multiply=[](void*p,float a,float b)->std::optional<float>{auto&r=*static_cast<HarborTutorialRuntime*>(p);if(!r.vm.platform.multiplyFloat)return {};return r.vm.platform.multiplyFloat(r.vm.platform.user,a,b);};
  s.truncate=[](void*p,float value)->std::optional<int32_t>{auto&r=*static_cast<HarborTutorialRuntime*>(p);if(!r.vm.platform.truncate||!std::isfinite(value)||double(value)<-2147483648.||double(value)>=2147483648.)return {};return r.vm.platform.truncate(r.vm.platform.user,value);};
  s.store=[](void*p,EventEntry&e,int32_t destination,int32_t value){setEventValue(e,destination,value,static_cast<HarborTutorialRuntime*>(p)->vm.interpreterServices().operands);return true;};
  s.writeWord=[](void*p,uint32_t at,uint32_t value){return static_cast<HarborTutorialRuntime*>(p)->vm.memory.writeWord(at,value);};
  auto result=function==0x8010d9bc?resumeBattleGetExpEvent(e,s,cursor):resumeBattleStoreExpEvent(e,s,cursor);if(!result)return pending(function);
  expCalls.erase(key);if(scene->pendingService==function){scene->pendingService=0;pendingEvent=0;}return *result;
 }
 std::map<uint64_t,BattleStarPointOffsetCursor>starOffsetCalls;
 int starOffsetEvent(EventEntry&e){
  constexpr uint32_t Function=0x80113f88;if(!packed)return pending(Function);
  const uint64_t key=(uint64_t(e.id)<<32)|e.currentCommand;auto&cursor=starOffsetCalls[key];
  BattleStarPointOffsetServices s;s.user=this;s.lifetime=statusLife;
  s.current=[](void*p){auto&r=*static_cast<HarborTutorialRuntime*>(p);uint32_t b=0;return r.packed&&r.scene&&!r.scene->failed&&r.vm.memory.readWord(0x8041eb90,b)&&b==r.packed->battleAddress();};
  s.word=[](void*p,uint32_t at)->std::optional<uint32_t>{uint32_t v;if(!static_cast<HarborTutorialRuntime*>(p)->vm.memory.readWord(at,v))return {};return v;};
  s.doubleBits=[](void*p,uint32_t at)->std::optional<uint64_t>{auto&r=*static_cast<HarborTutorialRuntime*>(p);uint32_t high,low;if(!r.vm.memory.readWord(at,high)||!r.vm.memory.readWord(at+4,low))return {};return (uint64_t(high)<<32)|low;};
  s.value=[](void*p,EventEntry&e,int32_t raw)->std::optional<int32_t>{return getEventValue(e,raw,static_cast<HarborTutorialRuntime*>(p)->vm.interpreterServices().operands);};
  s.translate=[](void*p,EventEntry&e,int32_t selector)->std::optional<int32_t>{auto&r=*static_cast<HarborTutorialRuntime*>(p);return translateBattleActor(&e,selector,nativeBattleActorServices(r.vm));};
  s.subtractDoubleFloat=[](void*p,double a,double b)->std::optional<float>{auto&r=*static_cast<HarborTutorialRuntime*>(p);if(!r.vm.platform.subtractDouble||!r.vm.platform.roundDoubleFloat)return {};return r.vm.platform.roundDoubleFloat(r.vm.platform.user,r.vm.platform.subtractDouble(r.vm.platform.user,a,b));};
  s.add=[](void*p,float a,float b)->std::optional<float>{auto&r=*static_cast<HarborTutorialRuntime*>(p);if(!r.vm.platform.addFloat)return {};return r.vm.platform.addFloat(r.vm.platform.user,a,b);};
  s.truncate=[](void*p,float value)->std::optional<int32_t>{auto&r=*static_cast<HarborTutorialRuntime*>(p);if(!r.vm.platform.truncate||!std::isfinite(value)||double(value)<-2147483648.||double(value)>=2147483648.)return {};return r.vm.platform.truncate(r.vm.platform.user,value);};
  s.store=[](void*p,EventEntry&e,int32_t destination,int32_t value){setEventValue(e,destination,value,static_cast<HarborTutorialRuntime*>(p)->vm.interpreterServices().operands);return true;};
  auto result=resumeBattleStarPointOffset(e,s,cursor);if(!result)return pending(Function);
  starOffsetCalls.erase(key);if(scene->pendingService==Function){scene->pendingService=0;pendingEvent=0;}return *result;
 }
 struct StarPointSource {std::weak_ptr<HarborTutorialRuntime>runtime;const EventEntry*event=nullptr;uint32_t id=0,command=0,args=0;bool published=false;};
 NativeHarborStarPointConsumer nativeStarPoint;NativeHarborStarPointSource starPointSource;
 EventScriptAssets starPointMemory;uint64_t starPointVisit=0;
 struct StarPointCall {NativeHarborStarPointCursor cursor;NativeHarborStarPointSource source;};
 std::map<uint64_t,StarPointCall>starPointCalls;StarPointCall*starPointInvoking=nullptr;
 struct NativeEffectDispatch {const EventEntry*event=nullptr;uint32_t id=0,command=0,args=0,name=0;std::string literal;};
 std::map<uint64_t,NativeEffectDispatch>nativeEffectDispatch;
 bool starPointCurrent()const{return nativeStarPoint.lifetime&&nativeStarPoint.current&&nativeStarPoint.current(nativeStarPoint.user);}
 int starPointEffect(EventEntry&e){
  constexpr uint32_t EffectFunction=0x8006d970;if(!packed||!starPointCurrent()||starPointInvoking)return pending(EffectFunction);
  const uint64_t key=(uint64_t(e.id)<<32)|e.currentCommand;auto&call=starPointCalls[key];
  if(!call.source.lifetime){auto owner=std::make_shared<StarPointSource>(*static_cast<StarPointSource*>(starPointSource.user));owner->event=&e;owner->id=e.id;owner->command=e.currentCommand;owner->args=e.arguments;call.source=starPointSource;call.source.user=owner.get();call.source.lifetime=owner;}
  starPointInvoking=&call;struct Guard {StarPointCall*&p;~Guard(){p=nullptr;}}guard{starPointInvoking};
  NativeHarborStarPointServices s;s.user=this;s.lifetime=nativeStarPoint.lifetime;
  s.current=[](void*p){return static_cast<HarborTutorialRuntime*>(p)->starPointCurrent();};
  s.word=[](void*p,uint32_t at)->std::optional<uint32_t>{uint32_t v;if(!static_cast<HarborTutorialRuntime*>(p)->vm.memory.readWord(at,v))return {};return v;};
  s.byte=[](void*p,uint32_t at)->std::optional<uint8_t>{auto&r=*static_cast<HarborTutorialRuntime*>(p);constexpr uint32_t OriginalEmptyLiteral=0x8042033c;if(at>=OriginalEmptyLiteral&&at<OriginalEmptyLiteral+4){uint32_t word;if(!r.starPointMemory.readWord(OriginalEmptyLiteral,word))return {};return uint8_t(word>>(24-8*(at-OriginalEmptyLiteral)));}uint8_t v;if(!r.vm.memory.readByte(at,v))return {};return v;};
  s.value=[](void*p,EventEntry&e,int32_t raw)->std::optional<int32_t>{return getEventValue(e,raw,static_cast<HarborTutorialRuntime*>(p)->vm.interpreterServices().operands);};
  s.floating=[](void*p,EventEntry&e,int32_t raw)->std::optional<float>{return getEventFloat(e,raw,static_cast<HarborTutorialRuntime*>(p)->vm.interpreterServices().operands);};
  s.spawn=[](void*p,const NativeHarborStarPointRequest&q)->std::optional<uint64_t>{auto&r=*static_cast<HarborTutorialRuntime*>(p);if(!r.starPointCurrent()||!r.nativeStarPoint.spawn)return {};if(!r.starPointInvoking)return {};auto&source=r.starPointInvoking->source;auto result=r.nativeStarPoint.spawn(r.nativeStarPoint.user,q,source);if(result&&*result)static_cast<StarPointSource*>(source.user)->published=true;return result;};
  if(!resumeNativeHarborStarPoint(e,s,call.cursor))return pending(EffectFunction);
  starPointCalls.erase(key);if(scene->pendingService==EffectFunction){scene->pendingService=0;pendingEvent=0;}return 2;
 }
 int finiteNativeEffect(EventEntry&e){
  constexpr uint32_t EffectFunction=0x8006d970;
  if(!packed||!scene||scene->failed||!(e.flags&1)||e.argumentCount<14)return pending(EffectFunction);
  // Additional read-only native admission, never source getter completion.
  // Only actual mapped literal operands are classified. Dynamic/encoded names
  // remain unavailable before either child has executed any source prefix.
  uint32_t name;if(!vm.memory.readWord(e.arguments+4,name))return pending(EffectFunction);
  std::string literal;bool terminated=false;for(unsigned i=0;i<16;++i){uint8_t ch;if(!vm.memory.readByte(name+i,ch))return pending(EffectFunction);if(!ch){terminated=true;break;}literal+=char(ch);}
  if(!terminated||(literal!="kemuri_test"&&literal!="star_point"))return pending(EffectFunction);
  const uint64_t key=(uint64_t(e.id)<<32)|e.currentCommand;auto found=nativeEffectDispatch.find(key);
  if(found==nativeEffectDispatch.end()){NativeEffectDispatch d;d.event=&e;d.id=e.id;d.command=e.currentCommand;d.args=e.arguments;d.name=name;d.literal=literal;found=nativeEffectDispatch.emplace(key,std::move(d)).first;}
  const auto&d=found->second;if(d.event!=&e||d.id!=e.id||d.command!=e.currentCommand||d.args!=e.arguments||d.name!=name||d.literal!=literal)return pending(EffectFunction);
  const int result=literal=="kemuri_test"?smokeEffect(e):starPointEffect(e);
  if(result==2)nativeEffectDispatch.erase(key);return result;
 }
 HarborTutorialRuntime():vm(0x81000000,platform(this)){}
 // Source clock 243MHz feeds VM tick conversion; native timestamps are ms.
 // GP base 0x81000000 is emulated RAM. Arithmetic callbacks explicitly preserve
 // operation boundaries; these host math choices are not measured Gekko parity.
 // Flush/system-call hooks here are scoped no-ops, not real SDK cache services.
 static EventNativePlatform platform(HarborTutorialRuntime* r){EventNativePlatform p;p.user=r;p.clock=[](void*)->uint32_t{return 243000000;};
 p.truncate=[](void*,float v)->int32_t{assert(std::isfinite(v) && double(v)>=-2147483648. && double(v)<2147483648.);return int32_t(v);};
 p.divideWord=[](void*,int32_t a,int32_t b)->int32_t{assert(b);return a/b;};p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){};
 p.irand=[](void* p,int32_t bound)->int32_t{return static_cast<HarborTutorialRuntime*>(p)->packed->random.bounded(bound);};
 p.signedWordFloat=[](void*,int32_t v){return float(v);};p.addFloat=[](void*,float a,float b){return a+b;};p.subtractFloat=[](void*,float a,float b){return a-b;};p.multiplyFloat=[](void*,float a,float b){return a*b;};p.divideFloat=[](void*,float a,float b){return a/b;};p.fmaFloat=[](void*,float a,float b,float c){return std::fma(a,b,c);};p.negativeMultiplySubtractFloat=[](void*,float a,float b,float c){return -std::fma(a,b,-c);};
 p.distanceFloat=[](void*,float a,float b,float c,float d){return std::hypot(c-a,d-b);};p.headingFloat=[](void*,float a,float b,float c,float d){return float(std::atan2(double(c-a),double(b-d))*180/3.141592653589793);};p.sineDouble=[](void*,double v){return std::sin(v);};p.cosineDouble=[](void*,double v){return std::cos(v);};p.roundDoubleFloat=[](void*,double v){return float(v);};
 p.addDouble=[](void*,double a,double b){return a+b;};p.subtractDouble=[](void*,double a,double b){return a-b;};p.multiplyDouble=[](void*,double a,double b){return a*b;};p.divideDouble=[](void*,double a,double b){return a/b;};p.fmaDouble=[](void*,double a,double b,double c){return std::fma(a,b,c);};p.fmodDouble=[](void*,double a,double b){return std::fmod(a,b);};
 p.call=[](void* r,EventEntry& e,uint32_t f,int32_t first)->int32_t{return static_cast<HarborTutorialRuntime*>(r)->call(e,f,first!=0);};return p;}
 uint32_t word(uint32_t a){uint32_t v=0;if(!vm.memory.readWord(a,v))scene->failed=true;return v;}
 int32_t raw(EventEntry& e,unsigned i){if(i>=e.argumentCount){scene->failed=true;return 0;}return signedWord(word(e.arguments+i*4));}
 int32_t value(EventEntry& e,unsigned i){return getEventValue(e,raw(e,i),vm.interpreterServices().operands);}
 float real(EventEntry& e,unsigned i){return getEventFloat(e,raw(e,i),vm.interpreterServices().operands);}
 void output(EventEntry& e,unsigned i,int32_t v){setEventValue(e,raw(e,i),v,vm.interpreterServices().operands);}
 // 256 is a native bounded guest-string read; missing NUL/read failure marks
 // scene failed. Actor selectors -2 owner/-3 Mario translate to VISUAL slots;
 // system 0 has no visual slot. Event roots retain native priority 10.
 std::string text(uint32_t a){std::string s;for(unsigned i=0;i<256;++i){uint8_t c=0;if(!vm.memory.readByte(a+i,c)){scene->failed=true;return {};}if(!c)return s;s+=char(c);}scene->failed=true;return {};}
 int32_t actor(EventEntry& e,unsigned i=0){int32_t v=value(e,i);if(v==-2)v=e.owner;else if(v==-3)v=1;const auto slot=scene->visualSlot(v);if(slot<0){scene->failed=true;return 0;}return slot;}
 EventEntry& root(uint32_t a,int32_t owner,uint32_t flags=0){auto services=vm.creationServices();auto& e=createRootEvent(vm.schedule,a,10,flags,services);e.owner=owner;return e;}
 BattleStatusWindowServices statusWindowServices(){
  BattleStatusWindowServices s;s.units=nativeBattleUnitEventServices(vm);s.user=this;
  s.isOpen=[](void* self){return static_cast<HarborTutorialRuntime*>(self)->scene->statusVisible;};
  s.open=[](void* self){static_cast<HarborTutorialRuntime*>(self)->scene->statusVisible=true;};
  s.close=[](void* self){static_cast<HarborTutorialRuntime*>(self)->scene->statusVisible=false;};
  return s; // Native visibility projection; source slide animation is separate.
 }
 // State 6 attack registration owns actual source event creation/window hide
 // and sequence increment. A status can suppress creation: root 0 is still a
 // completed registration, not permission to invent a fallback attack.
 void registerSelectedAttack(){
  attackRoot=0;BattleAttackRegistrationServices s;s.memory=nativeBattleActorServices(vm).memory;s.events=vm.creationServices();s.schedule=&vm.schedule;s.user=this;
  s.unit=[](void* self,uint32_t b,int32_t id){return battleRegistryUnit(b,id,nativeBattleActorServices(static_cast<HarborTutorialRuntime*>(self)->vm));};
  s.status=[](void* self,uint32_t u,int32_t type)->int32_t{return checkPackedBattleStatus(u,type,nativeBattleActorServices(static_cast<HarborTutorialRuntime*>(self)->vm))?1:0;};
  s.irand=[](void* self,int32_t bound){return static_cast<HarborTutorialRuntime*>(self)->packed->random.bounded(bound);};
  s.windowOff=[](void* self){auto& r=*static_cast<HarborTutorialRuntime*>(self);packedBattleStatusWindowSet(0x2744,false,r.statusWindowServices());};
  s.create=[](void* self,uint32_t script,int32_t priority,uint32_t flags){auto& r=*static_cast<HarborTutorialRuntime*>(self);auto& e=createRootEvent(r.vm.schedule,script,priority,flags,r.vm.creationServices());r.attackRoot=e.id;return &e;};
  s.advance=[](void* self,uint32_t b,int32_t channel){auto& r=*static_cast<HarborTutorialRuntime*>(self);packedBattleIncrementSequence(b,uint32_t(channel),nativeBattleActorServices(r.vm).memory);};
  registerBattleAttackState6(packed->battleAddress(),s);
 }
 bool exists(uint32_t id){for(int32_t i=0;i<vm.battle.capacity;++i)if(vm.battle.entries[i].id==id && (vm.battle.entries[i].flags&1))return true;return false;}
 // Source pose tables are (type, name) 8-byte rows; 80 is a native scan bound.
 // Mario uses catalog 11, others kind part-definition+0x48. New matched pose
 // invalidates native completion; names remain original guest strings.
 bool tablePose(HarborTutorialActor& a,int32_t type){uint32_t table=a.id==1?source[11]:word(word(a.sourceKind+0xb8)+0x48);
 for(unsigned i=0;i<80;++i){const auto name=word(table+i*8+4);if(!name)break;if(word(table+i*8)==uint32_t(type)){a.pose=text(name);a.poseAt=scene->now;const auto slot=scene->visualSlot(a.id);if(slot>=0)complete[size_t(slot)]=false;return true;}}return false;}
 // First unbound function pins diagnostic plus executing event ID. Keep that
 // event identity so cancellation can retire stale diagnostics without claiming
 // another pending child completed. 0 is the source wait return.
 int pending(uint32_t function){if(!scene->pendingService){scene->pendingService=function;pendingEvent=executingEvent;}return 0;}
 // Trace RAW operands only on first visit. Packed publish/invoke/capture
 // takes precedence over converter-index fallback; missing callbacks wait.
 // Projection callbacks that merely retain audio/camera requests below do not
 // prove original playback/rendering or full service implementation.
 int call(EventEntry& e,uint32_t function,bool first){
 executingEvent=e.id;
 if(first){HarborTutorialRequest r{function,e.currentCommand,scene->now,e.owner,{}};for(unsigned i=0;i<e.argumentCount;++i)r.rawOperands.push_back(raw(e,i));scene->requests.push_back(std::move(r));}
 if(function==0x80113f88)return starOffsetEvent(e);
 if(function==0x8010d9bc||function==0x8010d980)return expEvent(e,function);
 if(function==0x8010d914||function==0x8010d8d8)return coinEvent(e,function);
 if(function==0x8006d970)return finiteNativeEffect(e);
 if(function==0x80106fa0)return niceEffect(e);
 if(function==0x80106dfc){
  if(!packed)return pending(function);
  const uint64_t key=(uint64_t(e.id)<<32)|e.currentCommand;auto&cursor=nativeStylish[key];
  const auto value=nativeHarborStylishStart(vm,e,first,packed->hasNativePhysicalSample(),cursor);
  if(!value)return pending(function);
  if(*value){nativeStylish.erase(key);if(scene->pendingService==function){scene->pendingService=0;pendingEvent=0;}}
  return *value;
 }
 if(function==0x80106d7c){const auto value=nativeHarborStylishResult(vm,e);if(!value)return pending(function);if(scene->pendingService==function){scene->pendingService=0;pendingEvent=0;}return *value;}
 if(packed){packed->publish(*scene);int32_t result=0;if(packed->invoke(e,function,first?1:0,result)){packed->capture(*scene);if(result!=0 && scene->pendingService==function){scene->pendingService=0;pendingEvent=0;}return result;}}
 if(function==0x80104498){const auto& a=scene->actors[actor(e)];if(a.heldItem!=0)return pending(function);if(scene->pendingService==function)scene->pendingService=0;output(e,1,0);return 2;}
 if(function==0x8011384c){output(e,0,(scene->battleFlags&uint32_t(raw(e,1)))!=0?1:0);return 2;}
 if(function==0x801115f8 || function==0x80111720)return pending(function);
 auto found=callbacks.find(function);if(found==callbacks.end())return pending(function);
 switch(found->second){
 case 0:case 1:case 2:case 3:case 4:{auto& a=scene->actors[actor(e)];const auto script=uint32_t(value(e,1));switch(found->second){case 0:a.wait=script;break;case 1:a.attack=script;break;case 2:a.damage=script;break;case 3:a.confusion=script;break;case 4:a.phase=script;break;}return 2;}
 case 5:if(value(e,0)==1)scene->battleFlags|=uint32_t(raw(e,1));else scene->battleFlags&=~uint32_t(raw(e,1));return 2;
 case 6:{if(value(e,1)!=int32_t(source[5]) || value(e,2)!=0 || scene->actors[2].id!=-1)return pending(function);
 auto& a=scene->actors[2];a.id=HarborTutorial::HelperId;a.sourceKind=word(source[5]);if(a.sourceKind!=source[7]){scene->failed=true;return 0;}
 a.kind=signedWord(word(a.sourceKind));a.belong=int8_t(word(source[5]+4)>>24);a.movePhase=word(source[5]+8);a.hp=a.maxHp=int16_t(word(a.sourceKind+8)>>16);a.pose="PKR_S_1";for(unsigned i=0;i<3;++i){const auto bits=word(source[5]+0xc+4*i);std::memcpy(&a.position[i],&bits,4);}a.home=a.position;
 if(!packed || !packed->registerActor(a)){scene->failed=true;return 0;}
 output(e,0,a.id);packed->recordInit(a.id,root(source[3],a.id).id);return 2;}
 // Dispatch 7 work index 0..15; 8/9 position/home XYZ; 16 direct pose;
 // 19/20 per-event interpolation writes locals[0]=value, 1=continue then advances;
 // 21 rotation sentinel -1000000000 keeps an axis; 22 waits on live event ID.
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
 case 23:return battleStatusWindowOnOffEvent(e,statusWindowServices());
 case 24:{auto& a=scene->actors[actor(e)];const auto i=value(e,1);if(i<0 || i>=16){scene->failed=true;return 0;}output(e,2,a.work[i]);return 2;}
 case 25:scene->actors[actor(e)].talkPose=text(uint32_t(value(e,1)));return 2;
 case 26:scene->actors[actor(e)].stayPose=text(uint32_t(value(e,1)));return 2;
 // Message ID/speaker are captured on first visit; wait until real playback
 // becomes inactive. 29 enforces one initial visit delay AND actual matching
 // pose completion; elapsed time alone never completes the animation.
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
 // Native VM heap 0x82000000..0x82100000 (1MiB), SDK allocator globals at 0x8041f070/
 // 0x8041e428/0x8041e844, GP+4 rate 60 and battle context 1. This isolated VM/storage
 // does not implement the full process scheduler or original battle lifetime.
 bool initialize(){if(!files || !files->bind(vm.memory))return false;
 menuBinding=std::make_unique<BattleMenuFileBinding>(files,nativeBattleActorServices(vm));
 if(!vm.memory.registerRam(0x82000000,std::vector<uint8_t>(1024*1024)) || !vm.memory.registerRam(0x8041f070,std::vector<uint8_t>(16)) || !vm.memory.registerRam(0x8041e428,std::vector<uint8_t>(4)) || !vm.memory.registerRam(0x8041e844,std::vector<uint8_t>(4)) || !vm.memory.importScripts(scripts))return false;
 auto m=vm.memoryBinding.services();auto start=eventSdkInitializeAllocator(0x82000000,0x82100000,1,m);if(eventSdkCreateHeap(start,0x82100000,m)!=0 || !vm.memory.writeWord(0x8041e844,0) || !vm.memory.writeWord(0x81000004,60))return false;
 vm.gp.timestamp=scene->now*eventClockRate(243000000);vm.initializeContexts();vm.gp.battleContext=1;packed=std::make_unique<HarborTutorialPacked>(vm);if(!packed->provision())return false;
 // Explicit native fresh window context, matching the trio bridge. Subsequent
 // visibility is arbitrated through the complete original setter/check.
 packedBattleStatusWindowSet(0x2748,true,statusWindowServices());
 packedBattleStatusWindowSet(0x2744,true,statusWindowServices());return true;}
};
bool HarborTutorial::begin(uint64_t time,int32_t hp,int32_t maxHp){return beginWithContext(time,hp,maxHp,false,0);}
bool HarborTutorial::beginFresh(uint64_t time,int32_t hp,int32_t maxHp,int32_t story){return beginWithContext(time,hp,maxHp,true,story);}
bool HarborTutorial::beginFromPouch(uint64_t time,const ProloguePouch& entry,int32_t story){return beginWithContext(time,entry.half(0x70),entry.half(0x72),true,story,&entry);}
// Reset only after ready/not-triggered admission. Original visual slots are
// Crump 0/Mario 1/helper 2 but registry IDs 2/1/3; invisible system ID 0 enters first.
// Mario kind 222, source move phase 0x04000002 and (-100, 0,-5) are native entry
// context. Original setup/kind tables supply opponent/system fields.
bool HarborTutorial::beginWithContext(uint64_t time,int32_t hp,int32_t maxHp,bool freshPouch,int32_t story,const ProloguePouch* entry){if(!ready || triggered)return false;*this=HarborTutorial{};triggered=true;now=time;
 actors[0].id=CrumpId;actors[0].sourceKind=source[6];actors[1].id=MarioId;actors[1].kind=222;actors[1].movePhase=0x04000002;actors[1].hp=hp;actors[1].maxHp=maxHp;actors[1].pose="M_S_1";actors[1].position=actors[1].home={-100,0,-5};
 runtime_=std::make_shared<HarborTutorialRuntime>();
 sourceMenuRegistration_=std::make_shared<HarborSourceMenuRegistration>();sourceMenuRegistration_->runtime=runtime_;
 auto& r=*runtime_;r.scene=this;r.nativeAdmissionFacade=this;r.nativeAdmissionProof=nativeMenuRegistration_.proof();if(!r.initialize()){failed=true;return false;}actors[0].kind=signedWord(r.word(source[6]));actors[0].belong=int8_t(r.word(source[8]+4)>>24);actors[0].movePhase=r.word(source[8]+8);actors[0].hp=actors[0].maxHp=int16_t(r.word(source[6]+8)>>16);
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
 // Fresh entry installs pouch and known story byte 0, then source field battle
 // configuration: catalog[9]-relative(0x60eb8-0x60c90), expected group[9]/stage[10].
 // Legacy begin deliberately lacks this pouch/Mario-kind-init admission.
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
// Reject backward milliseconds. Message pose requests preserve their own
// publication time; source VM scheduling/action admission is scoped native
// composition, not a renderer frame loop or complete source battle Main.
// Intro completion requires original phase-root retirement without interruption;
// no HP threshold or development key advances story/outcomes here.
void HarborTutorial::tick(uint64_t time,uint32_t keys,const BattlePadSample* physical){if(!runtime_ || failed || time<now || runtime_->nativeCompletionFrozen)return;now=time;auto& r=*runtime_;r.scene=this;
 // Native smoke playback clock is actual accepted simulation visits, never
 // presentation/draw invocations. Missing admission cannot advance events.
 if(r.nativeSmoke.lifetime){if(r.smokeVisit==UINT64_MAX||!r.smokeCurrent()||!r.nativeSmoke.advance||!r.nativeSmoke.advance(r.nativeSmoke.user,r.smokeVisit+1))return;++r.smokeVisit;}
 if(r.nativeStarPoint.lifetime){if(r.starPointVisit==UINT64_MAX||!r.starPointCurrent()||!r.nativeStarPoint.advance||!r.nativeStarPoint.advance(r.nativeStarPoint.user,r.starPointVisit+1))return;++r.starPointVisit;}
 if(message.active())message.tick(now,keys);
 const auto speakerSlot=visualSlot(speaker);
 if(message.poseRequestSerial!=r.messagePoseSerial && speakerSlot>=0){r.messagePoseSerial=message.poseRequestSerial;auto& a=actors[size_t(speakerSlot)];const auto& pose=message.poseRequest==MessagePlayback::PoseRequest::Talk?a.talkPose:a.stayPose;if(!pose.empty()){a.pose=pose;a.poseAt=message.poseRequestedAt;}}
 const bool heldActionManager=r.packed->actionManagerPending();
 if(!heldActionManager){r.packed->samplePhysical(time,physical);r.packed->publish(*this);}
 // A held source manager owns its original sample and command work. Do not
 // overwrite general history/guest fields or advance events around its wait.
 // This is scoped native admission, not the complete original Main scheduler.
 if(!r.packed->actionManager(time)){
  if(!pendingService)pendingService=0x800fa5b0;
  actionDisplayPending=!r.packed->actionDisplay(time,*this);return;
 }
 if(heldActionManager && pendingService==0x800fa5b0)pendingService=0;
 const auto admittedTime=heldActionManager?r.packed->actionManagerSampleTime():now;
 r.vm.gp.timestamp=admittedTime*eventClockRate(243000000);r.vm.runManager();r.packed->publish(*this);r.packed->capture(*this);
 actionDisplayPending=!r.packed->actionDisplay(time,*this);
 damageActive=r.packed->hitEventsActive();
 if(!initialized && r.packed->unitInitReady())initialized=true;
 if(initialized && !r.openingDispatched && dispatchPhase(1,0x04000001))r.openingDispatched=true;
 if(attackActive && !r.exists(r.attackRoot))attackActive=false;
 if(phaseActive && !r.exists(r.phaseRoot)){phaseActive=false;if(r.openingDispatched && !r.phaseInterrupted && phaseWord==0x04000001)introComplete=true;}
}
const DisplayQueue* HarborTutorial::actionDisplayQueue()const{return runtime_?&runtime_->packed->actionDisplays:nullptr;}
bool HarborTutorial::dispatchPhase(int32_t sourceTurn,uint32_t sourcePhase){if(!runtime_ || failed || !initialized || phaseActive || attackActive || damageActive || !actors[0].phase)return false;turn=sourceTurn;phaseWord=sourcePhase;runtime_->phaseInterrupted=false;runtime_->phaseRoot=runtime_->root(actors[0].phase,CrumpId,0x20).id;runtime_->packed->recordPhase(CrumpId,runtime_->phaseRoot);phaseActive=true;return true;}
bool HarborTutorial::dispatchInstalledAttack(int32_t id){const auto slot=visualSlot(id);if(!runtime_ || failed || !initialized || !introComplete || phaseActive || attackActive || damageActive || (id!=CrumpId && id!=HelperId) || slot<0 || !actors[size_t(slot)].attack)return false;pendingService=0;runtime_->attackRoot=runtime_->root(actors[size_t(slot)].attack,id).id;runtime_->packed->recordAttack(id,runtime_->attackRoot);attackActive=true;return true;}
// Require explicit initialized/intro/idle/target/command context. Source bank
// 0x06000006 and battle bit 4 select the admitted attack route; finalization may
// already own selection. Registration can complete with no root under status.
bool HarborTutorial::dispatchSelectedAttack(){
 if(!runtime_ || failed || !initialized || !introComplete || phaseActive || attackActive || damageActive || weaponMenuOpen || weaponTargets.empty() || runtime_->packed->commandInputBlocked() || (!runtime_->packed->weaponDecided && !runtime_->packed->commandFinalized))return false;
 auto& r=*runtime_;r.scene=this;r.packed->publish(*this);
 // Explicit confirmed-command scheduler entry, analogous to the trio launch;
 // not an implementation of Input's pending/progress loop or target UI.
 const auto b=r.packed->battleAddress();
 if(!r.vm.memory.writeWord(b+0x420,MarioId) || !r.vm.memory.writeWord(b+0x171c+0x540,weaponMenuType))return false;
 if(!r.packed->commandFinalized && !r.packed->selectAttackEvent())return false;
 r.packed->commandFinalized=false;
 battleFlags|=4;r.vm.memory.writeWord(b+0xef4,battleFlags);
 r.vm.memory.writeWord(b+0xf20,0x06000006);
 pendingService=0;r.registerSelectedAttack();r.packed->capture(*this);
 weaponTargetMenuOpen=false;attackActive=r.attackRoot!=0;
 return true; // Admission succeeded even if a source status suppressed a root.
}
// Hit-event producer may cancel phase/attack roots. Clear stale diagnostics
// only for actual retired owners; interrupted opening cannot mark introComplete.
// This bridge observes roots and captures source writes, never synthesizes HP.
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
bool HarborTutorial::initializeClassMenuResources(){
 if(!runtime_ || failed)return false;
 return initializePackedBattleMenuResources(runtime_->menuBinding->services(),runtime_->menuCursor);
}
bool HarborTutorial::endClassMenuResources(){
 if(!runtime_ || failed)return false;
 return endPackedBattleMenuResources(runtime_->menuBinding->services(),runtime_->menuCursor);
}
bool HarborTutorial::openClassMenu(){
 if(!runtime_ || failed || !initialized || !introComplete || phaseActive || attackActive || damageActive)return false;
 auto& r=*runtime_;r.scene=this;r.packed->publish(*this);
 if(!r.packed->buildClassMenu(*this,&r,[](void* self,uint32_t key){auto& r=*static_cast<HarborTutorialRuntime*>(self);const auto id=r.text(key);return r.packed->cacheMenuLabel(key,messages.find(id.c_str()));}))return false;
 r.vm.memory.writeWord(r.packed->battleAddress()+0x171c,0);
 const auto result=r.packed->commandInput(*this,&r,[](void* self,uint32_t key){auto& r=*static_cast<HarborTutorialRuntime*>(self);const auto id=r.text(key);return r.packed->cacheMenuLabel(key,messages.find(id.c_str()));});
 r.packed->capture(*this);return result.has_value();
}
// Returned binding borrows actual VM memory but retains runtime/allocation
// identity. No-match cursor lookup is completed with absent output. current()
// checks BOTH fresh lifetime token and live B+0x1c78 before any held draw resumes.
std::optional<HarborClassMenuDrawBinding> HarborTutorial::classMenuDrawBinding(
 const BattleActDrawServices& linked)const{
 if(!runtime_ || failed || !classMenuOpen)return {};
 auto& r=*runtime_;const auto w=r.word(HarborTutorialPacked::Battle+0x1c78);
 const auto token=r.packed->classUiLifetime(w);if(!token)return {};
 HarborClassMenuDrawBinding out;out.owner=token;out.allocation=w;
 out.display.draw=linked;out.display.draw.memory=nativeBattleActorServices(r.vm);
 out.display.user=&r;
 out.display.cursorLookup=[](void* user,uint32_t menu,uint32_t type,std::optional<uint32_t>& result){
  auto& r=*static_cast<HarborTutorialRuntime*>(user);uint32_t cursor=0;
  if(findPackedCommandCursor(menu,type,cursor,nativeBattlePackedSelectionServices(r.vm)))result=cursor;
  return true; // Original no-match is completed lookup, whose output is unused.
 };
 out.current=[runtime=runtime_,owner=out.owner,w](){
  uint32_t actual=0;return runtime->packed->classUiLifetime(w)==owner &&
    runtime->packed->readWord(HarborTutorialPacked::Battle+0x1c78,actual) && actual==w;
 };
 return out;
}
// Process-owned palette binds once per runtime; same owner may reattach,
// different owner is rejected. Do not reset source color history on each battle
// entry/display; caller owns the shared palette across scene lifetimes.
bool HarborTutorial::bindNativeStarPoint(const NativeHarborStarPointConsumer&consumer,const char*sourceImage){
 if(!runtime_||!runtime_->packed||!consumer.lifetime||!consumer.current||!consumer.current(consumer.user)||!consumer.spawn||!consumer.advance||!sourceImage)return false;
 auto&r=*runtime_;if(r.nativeStarPoint.lifetime)return r.nativeStarPoint.lifetime==consumer.lifetime&&!r.nativeStarPoint.lifetime.owner_before(consumer.lifetime)&&!consumer.lifetime.owner_before(r.nativeStarPoint.lifetime)&&r.nativeStarPoint.user==consumer.user&&r.nativeStarPoint.current==consumer.current&&r.nativeStarPoint.spawn==consumer.spawn&&r.nativeStarPoint.advance==consumer.advance;
 EventScriptAssets memory;NativeHarborStarPointData data;if(!memory.load(sourceImage)||!data.load(sourceImage))return false;
 auto owner=std::make_shared<HarborTutorialRuntime::StarPointSource>();owner->runtime=runtime_;
 NativeHarborStarPointSource source;source.user=owner.get();source.lifetime=owner;
 source.current=[](void*p){auto&proof=*static_cast<HarborTutorialRuntime::StarPointSource*>(p);auto r=proof.runtime.lock();uint32_t b=0;if(!r||!r->packed||!r->scene||r->scene->failed||!r->vm.memory.readWord(0x8041eb90,b)||b!=r->packed->battleAddress()||!r->starPointCurrent())return false;return proof.published||!proof.event||((proof.event->flags&1)&&proof.event->id==proof.id&&proof.event->currentCommand==proof.command&&proof.event->arguments==proof.args);};
 source.randomRaw=[](void*p)->std::optional<int32_t>{auto r=static_cast<HarborTutorialRuntime::StarPointSource*>(p)->runtime.lock();if(!r||!r->packed)return {};return r->packed->random.next();};
 source.math=[](void*p,NativeHarborStarMath op,float a,float b,float c)->std::optional<float>{auto r=static_cast<HarborTutorialRuntime::StarPointSource*>(p)->runtime.lock();if(!r)return {};auto&s=r->vm.platform;switch(op){case NativeHarborStarMath::Add:if(s.addFloat)return s.addFloat(s.user,a,b);break;case NativeHarborStarMath::Subtract:if(s.subtractFloat)return s.subtractFloat(s.user,a,b);break;case NativeHarborStarMath::Multiply:if(s.multiplyFloat)return s.multiplyFloat(s.user,a,b);break;case NativeHarborStarMath::Divide:if(s.divideFloat)return s.divideFloat(s.user,a,b);break;case NativeHarborStarMath::MultiplyAdd:if(s.fmaFloat)return s.fmaFloat(s.user,a,b,c);break;case NativeHarborStarMath::MultiplySubtract:if(s.fmaFloat)return s.fmaFloat(s.user,a,b,-c);break;}return {};};
 r.starPointMemory=std::move(memory);r.nativeStarPoint=consumer;r.starPointSource=std::move(source);return true;
}
bool HarborTutorial::bindNativeSmoke(const NativeHarborSmokeConsumer&consumer){
 if(!runtime_||!runtime_->packed||!consumer.lifetime||!consumer.current||!consumer.current(consumer.user)||!consumer.spawn||!consumer.advance)return false;
 auto&r=*runtime_;if(r.nativeSmoke.lifetime)return r.nativeSmoke.lifetime==consumer.lifetime&&r.nativeSmoke.user==consumer.user&&r.nativeSmoke.current==consumer.current&&r.nativeSmoke.spawn==consumer.spawn&&r.nativeSmoke.advance==consumer.advance;
 auto owner=std::make_shared<HarborTutorialRuntime::SmokeSource>();owner->runtime=runtime_;
 NativeHarborSmokeSource source;source.user=owner.get();source.lifetime=owner;
 source.current=[](void*p){auto r=static_cast<HarborTutorialRuntime::SmokeSource*>(p)->runtime.lock();uint32_t b=0;return r&&r->packed&&r->scene&&!r->scene->failed&&r->vm.memory.readWord(0x8041eb90,b)&&b==r->packed->battleAddress()&&r->smokeCurrent();};
 source.random=[](void*p,uint32_t bound)->std::optional<uint32_t>{auto r=static_cast<HarborTutorialRuntime::SmokeSource*>(p)->runtime.lock();if(!r||!r->packed||bound!=NativeHarborSmokeVisual::DelayBound)return {};return uint32_t(r->packed->random.bounded(int32_t(bound)));};
 source.sin=[](void*p,float angle)->std::optional<float>{auto r=static_cast<HarborTutorialRuntime::SmokeSource*>(p)->runtime.lock();if(!r||!r->vm.platform.sineDouble||!r->vm.platform.roundDoubleFloat)return {};return r->vm.platform.roundDoubleFloat(r->vm.platform.user,r->vm.platform.sineDouble(r->vm.platform.user,double(angle)));};
 source.cos=[](void*p,float angle)->std::optional<float>{auto r=static_cast<HarborTutorialRuntime::SmokeSource*>(p)->runtime.lock();if(!r||!r->vm.platform.cosineDouble||!r->vm.platform.roundDoubleFloat)return {};return r->vm.platform.roundDoubleFloat(r->vm.platform.user,r->vm.platform.cosineDouble(r->vm.platform.user,double(angle)));};
 r.nativeSmoke=consumer;r.smokeSource=std::move(source);return true;
}
bool HarborTutorial::bindNativeNice(std::shared_ptr<NativeUiStartup> startup,const NativeHarborNiceConsumer&consumer){
 if(!runtime_||!runtime_->packed||!startup||!startup->current()||!consumer.lifetime||!consumer.current||!consumer.current(consumer.user)||!consumer.spawn||!consumer.scale)return false;
 auto&r=*runtime_;const auto process=startup->fieldProcess();if(!process)return false;
 if(r.nativeNice.lifetime)return r.niceStartup==startup&&r.nativeNice.lifetime==consumer.lifetime&&r.nativeNice.user==consumer.user&&r.nativeNice.current==consumer.current&&r.nativeNice.spawn==consumer.spawn&&r.nativeNice.scale==consumer.scale&&r.niceLanguage&&r.niceLanguage->current();
 auto&sourceGp=process->process().vm().gpAddresses;
 auto alias=std::make_shared<EventGpLanguageAlias>(sourceGp,sourceGp.currentAddress(),startup->lifetime(),[startup]{return startup->current();});
 if(!alias->current()||!r.vm.gpAddresses.bindLanguageAlias(r.vm.gpAddresses.currentAddress(),alias))return false;
 r.niceStartup=std::move(startup);r.niceLanguage=std::move(alias);r.nativeNice=consumer;return true;
}
bool HarborTutorial::bindPublicSound(std::shared_ptr<PositionalSfxRegistry> registry){
 if(!runtime_ || !runtime_->packed || !registry)return false;
 auto& r=*runtime_;
 if(r.packed->sourceSound)return r.packed->sourceSound->usesRegistry(registry.get());
 // Independent weak proof prevents a retained sound owner from extending a
 // Runtime->Packed->sound->Runtime cycle or dereferencing an old facade.
 struct Proof{std::weak_ptr<HarborTutorialRuntime> runtime;uint32_t battle=0;};
 auto proof=std::make_shared<Proof>();proof->runtime=runtime_;proof->battle=r.packed->battleAddress();
 if(!proof->battle)return false;
 NativeBattleSoundBinding binding;binding.vm=&r.vm;binding.random=&r.packed->random;
 binding.registry=std::move(registry);binding.lifetime=proof;binding.user=proof.get();
 binding.current=[](void* user){auto held=static_cast<Proof*>(user)->runtime.lock();uint32_t b=0;
  return held && held->packed && held->vm.memory.readWord(0x8041eb90,b) &&
   b==static_cast<Proof*>(user)->battle;
 };
 r.packed->sourceSound=std::make_shared<NativeBattleSound>(binding);return true;
}
std::shared_ptr<NativeBattleSound> HarborTutorial::nativeGameplaySound()const{
 return runtime_&&runtime_->packed&&!failed?runtime_->packed->sourceSound:nullptr;
}
std::optional<int32_t> HarborTutorial::startPublicMenuSound(uint32_t key){
 if(!runtime_ || !runtime_->packed || !runtime_->packed->sourceSound)return {};
 auto held=runtime_;return held->packed->sourceSound->menu(key);
}
bool HarborTutorial::bindCommandDisplayPalette(std::shared_ptr<BattleCommandPalette> palette){
 if(!runtime_ || failed || !palette)return false;
 auto& r=*runtime_;
 if(r.commandPalette)return r.commandPalette==palette;
 if(!bindBattleCommandPaletteMemory(r.vm.memory,*palette))return false;
 r.commandPalette=std::move(palette);return true;
}
bool HarborTutorial::setHarborSourceDisplayPass(std::shared_ptr<HarborSourceDisplayPass> display){
 const auto r=runtime_;if(!r||failed||!display||!display->runtimeIs(r)||display->battleAddress()!=r->packed->battleAddress())return false;
 if(r->sourceDisplay)return r->sourceDisplay==display;
 r->sourceDisplay=std::move(display);return true;
}
std::shared_ptr<HarborSourceMenuFactory> HarborTutorial::sourceMenuFactory() const {
 const auto r=runtime_;if(!r||failed||!r->sourceDisplay)return {};
 if(!sourceMenuRegistration_){sourceMenuRegistration_=std::make_shared<HarborSourceMenuRegistration>();sourceMenuRegistration_->runtime=r;}
 if(sourceMenuRegistration_->runtime.lock()!=r)return {};
 if(r->sourceMenuFactory)return r->sourceMenuFactory;
 const std::weak_ptr<HarborTutorialRuntime> weak=r;
 const std::weak_ptr<HarborSourceMenuRegistration> registration=sourceMenuRegistration_;
 const uint32_t b=r->packed->battleAddress();if(!b||b!=r->sourceDisplay->battleAddress())return {};
 HarborSourceMenuFactoryBinding binding;binding.vm=&r->vm;binding.battle=b;binding.runtime=r;binding.display=r->sourceDisplay;binding.files=r->files;binding.messages=r->sourceMenuMessages;
 binding.current=[weak,registration,b,files=r->files]{const auto r=weak.lock();const auto slot=registration.lock();return r&&slot&&slot->runtime.lock()==r&&r->files==files&&r->packed->battleAddress()==b;};
 binding.uiLifetime=[weak](uint32_t w){const auto r=weak.lock();return r?r->packed->classUiLifetime(w):nullptr;};
 binding.random=[weak](uint32_t bound){const auto r=weak.lock();if(!r)std::abort();return uint32_t(r->packed->random.bounded(int32_t(bound)));};
 for(const auto&key:originalBattleClassMenuMessageKeys())if(const auto pointer=r->packed->cachedMenuLabelAddress(key.source))binding.existingMessages.emplace_back(key.id,*pointer);
 r->sourceMenuFactory=HarborSourceMenuFactory::create(std::move(binding));return r->sourceMenuFactory;
}
std::optional<TrioClassMenuFrameBinding> HarborTutorial::classMenuFrameBinding(const BattleActDrawServices& linked)const{const auto factory=sourceMenuFactory();return factory?factory->classBinding(linked):std::nullopt;}
std::optional<TrioWeaponMenuFrameBinding> HarborTutorial::weaponMenuFrameBinding(const BattleWeaponDrawServices& linked)const{const auto factory=sourceMenuFactory();return factory?factory->weaponBinding(linked):std::nullopt;}
std::shared_ptr<BattleWeaponSourceMemory> HarborTutorial::weaponMenuSourceMemory(std::shared_ptr<NativeUiStartup> startup,const char* image)const{const auto factory=sourceMenuFactory();return factory?factory->weaponMemory(std::move(startup),image):nullptr;}
bool HarborTutorial::registerCommandDisplay(DisplayQueue& queue,BattleFooterCursor& cursor){
 if(!runtime_ || failed || !runtime_->commandPalette)return false;
 return registerBattleCommandDisplay(runtime_->packed->battleAddress(),
   *runtime_->commandPalette,queue,cursor,nativeBattleActorServices(runtime_->vm));
}
const BattleAudienceIncarnationOwner* HarborTutorial::audienceIncarnations()const{
 return runtime_ && !failed?runtime_->vm.audienceIncarnations.get():nullptr;
}
// Source UI+0x14 is explicit presentation acknowledgement, not creation or draw
// registration. Menu resource Init/End elsewhere retain separate file refs;
// rendering, queue registration and VM destruction do not synthesize End.

std::optional<HarborNativeMenuSnapshot> HarborTutorial::nativeCommandMenu()const {
 constexpr uint32_t CommandWorkOffset=0x1c78,CandidateBufferOffset=0x428,CandidateSelectionOffset=0xab7;
 constexpr uint32_t UnitPartHeadOffset=0x14,UnitKindOffset=8,PartDefinitionOffset=4,PartNameOffset=8,YoshiKind=226,NameByteLimit=256;
 const auto r=runtime_;
 if(!r||failed||!initialized||!introComplete||phaseActive||attackActive||damageActive||message.active()||r->sourceDisplay||!sourceMenuRegistration_||sourceMenuRegistration_->runtime.lock()!=r)return {};
 HarborNativeMenuSnapshot snapshot;snapshot.facade=this;snapshot.runtime=r;snapshot.registration=sourceMenuRegistration_;snapshot.facadeRegistration=nativeMenuRegistration_.proof();snapshot.battle=r->packed->battleAddress();snapshot.modelTick=now;snapshot.turn=turn;snapshot.phase=phaseWord;
 if(!snapshot.battle||!r->packed->readWord(snapshot.battle+CommandWorkOffset,snapshot.work))return {};
 if(weaponTargetMenuOpen){
  snapshot.kind=HarborNativeMenuKind::Target;snapshot.type=weaponMenuType;snapshot.menuOwner=r->packed->nativeWeaponPresentationLifetime();
  uint8_t selected=0;if(!r->vm.memory.readByte(snapshot.battle+CandidateBufferOffset+CandidateSelectionOffset,selected))return {};snapshot.selected=int8_t(selected);
  for(const auto& target:weaponTargets){
   const uint32_t unit=r->packed->unitAddress(target[0]);uint32_t part=0,kind=0;if(!unit||!r->packed->readWord(unit+UnitPartHeadOffset,part)||!r->packed->readWord(unit+UnitKindOffset,kind)||kind==YoshiKind)return {};
   std::unordered_set<uint32_t> visited;
   uint32_t firstNext=0;if(!part||!r->packed->readWord(part,firstNext))return {};
   if(target[1]>=0&&firstNext)while(part){if(!visited.insert(part).second)return {};uint32_t next=0,definition=0,id=0;if(!r->packed->readWord(part+PartDefinitionOffset,definition)||!r->packed->readWord(definition,id))return {};if(id==uint32_t(target[1]))break;if(!r->packed->readWord(part,next))return {};part=next;}
   uint32_t name=0;if(!part||!r->packed->readWord(part+PartNameOffset,name)||!name)return {};std::string key;bool terminated=false;for(unsigned offset=0;offset<NameByteLimit;++offset){uint8_t byte=0;if(!r->vm.memory.readByte(name+offset,byte))return {};if(!byte){terminated=true;break;}key.push_back(char(byte));}if(!terminated||key.empty())return {};const auto* label=messages.find(key.c_str());if(!label||!*label)return {};
   HarborNativeMenuRow row{uint32_t(target[0]),uint32_t(target[1]),0,true,label};row.sourceName=name;snapshot.rows.push_back(std::move(row));
  }
 }else if(weaponMenuOpen){snapshot.kind=HarborNativeMenuKind::Weapon;snapshot.type=weaponMenuType;snapshot.selected=weaponRowIndex;snapshot.menuOwner=r->packed->nativeWeaponPresentationLifetime();
  if(!snapshot.work||snapshot.work!=r->packed->nativeWeaponPresentationWork())return {};
  for(const auto& row:weaponRows){HarborNativeMenuRow item{row.weapon,uint32_t(row.item),row.icon,row.enabled,row.label};item.sourceName=row.nameAddress;item.sourceReason=uint32_t(row.reason);item.sourceOrdinal=uint32_t(row.ordinal);item.sourceItem=uint32_t(row.item);snapshot.rows.push_back(std::move(item));}
 }else if(classMenuOpen){snapshot.kind=HarborNativeMenuKind::Class;snapshot.selected=classRowIndex;snapshot.menuOwner=r->packed->classUiLifetime(snapshot.work);
  for(const auto& row:classRows){HarborNativeMenuRow item{row.type,row.reason,row.icon,row.enabled,row.label};item.sourceName=row.nameAddress;item.sourceReason=row.reason;snapshot.rows.push_back(std::move(item));}
 }else return {};
 if(!snapshot.menuOwner||snapshot.rows.empty()||snapshot.selected<0||size_t(snapshot.selected)>=snapshot.rows.size())return {};
 return snapshot;
}
bool HarborTutorial::nativeCommandMenuCurrent(const HarborNativeMenuSnapshot& snapshot)const {
 const auto actual=nativeCommandMenu();return actual&&sameHarborNativeMenu(snapshot,*actual);
}

void HarborTutorial::classMenuPresented() const{if(runtime_ && classMenuOpen){uint32_t w=0;if(runtime_->packed->readWord(HarborTutorialPacked::Battle+0x1c78,w) && w)runtime_->vm.memory.writeByte(w+0x14,1);}}
bool HarborTutorial::buildClassMenu(){
 if(!runtime_ || failed || !initialized || !introComplete || phaseActive || attackActive || damageActive)return false;
 auto& r=*runtime_;r.scene=this;r.packed->publish(*this);
 const bool built=r.packed->buildClassMenu(*this,&r,[](void* self,uint32_t key){auto& r=*static_cast<HarborTutorialRuntime*>(self);const auto id=r.text(key);return r.packed->cacheMenuLabel(key,messages.find(id.c_str()));});
 r.packed->capture(*this);return built;
}
// Explicit native confirmed-class bridge into Input state 7; ordinary source
// command completion/target confirmation remain separate caller operations.
bool HarborTutorial::openWeaponMenu(uint32_t type){
 if(!runtime_ || failed || !initialized || !introComplete || phaseActive || attackActive || damageActive)return false;auto& r=*runtime_;r.scene=this;r.packed->publish(*this);
 const bool opened=r.packed->openWeaponMenu(type,*this,&r,[](void* self,uint32_t key){auto& r=*static_cast<HarborTutorialRuntime*>(self);const auto id=r.text(key);return r.packed->cacheMenuLabel(key,messages.find(id.c_str()));});
 // Explicit native confirmed-class entry into source Input state7.
 if(opened)r.vm.memory.writeWord(r.packed->battleAddress()+0x171c,7);
 return opened;
}
void HarborTutorial::weaponMenuPresented() const{if(runtime_ && weaponMenuOpen)runtime_->packed->weaponMenuPresented();}
std::optional<int32_t> HarborTutorial::inputWeaponMenu(const BattlePadSample& sample){
 if(!runtime_ || failed || !weaponMenuOpen || phaseActive || attackActive || damageActive)return std::nullopt;auto& r=*runtime_;r.scene=this;r.packed->samplePhysical(now,&sample);r.packed->publish(*this);
 return r.packed->inputWeaponMenu(*this,&r,[](void* self,uint32_t key){auto& r=*static_cast<HarborTutorialRuntime*>(self);const auto id=r.text(key);return r.packed->cacheMenuLabel(key,messages.find(id.c_str()));});
}
std::optional<int32_t> HarborTutorial::inputCommand(const BattlePadSample& sample){
 if(!runtime_ || failed || !initialized || !introComplete || phaseActive || attackActive || damageActive)return std::nullopt;
 auto& r=*runtime_;r.scene=this;r.packed->samplePhysical(now,&sample);r.packed->publish(*this);
 const auto result=r.packed->commandInput(*this,&r,[](void* self,uint32_t key){auto& r=*static_cast<HarborTutorialRuntime*>(self);const auto id=r.text(key);return r.packed->cacheMenuLabel(key,messages.find(id.c_str()));});
 r.packed->capture(*this);return result;
}
bool HarborTutorial::readPackedWord(uint32_t address,uint32_t& value)const{return runtime_ && runtime_->packed && runtime_->packed->readWord(address,value);}
std::shared_ptr<NativeHarborRewardCounter>HarborTutorial::beginNativeRewardCounter(){
 if(!runtime_||!runtime_->packed||!sourceMenuRegistration_)return {};
 auto r=runtime_;
 // A copy shares VM storage but cannot become the battle's admitted facade.
 if(r->nativeAdmissionFacade!=this||!r->nativeAdmissionProof.current()||r->scene!=this||r->nativeCompletionFrozen)return {};
 if(auto owner=r->nativeRewardCounter.lock())return owner;
 if(r->nativeRewardCounterMinted)return {}; // Cancellation cannot award the same stock twice.
 if(failed||!initialized||!introComplete||message.active()||attackActive||damageActive||phaseActive||pendingService||r->vm.schedule.managerActive||r->packed->hitEventsActive())return {};
 if(r->vm.battle.capacity<0||r->vm.battle.capacity>256||(r->vm.battle.capacity&&!r->vm.battle.entries))return {};
 for(int32_t i=0;i<r->vm.battle.capacity;++i){const auto&e=r->vm.battle.entries[i];if(!(e.flags&1))continue;
  bool wait=false;for(const auto&actor:actors)wait|=actor.id>=0&&actor.wait&&e.owner==actor.id&&e.restartScript==actor.wait;
  wait|=systemActor.id>=0&&systemActor.wait&&e.owner==systemActor.id&&e.restartScript==systemActor.wait;
  if(!wait)return {};
 }
 const auto conclusion=nativeGameplayConclusion();if(!conclusion||*conclusion!=1||!r->packed->sourceSound)return {};
 constexpr uint32_t BattleGlobal=0x8041eb90,PouchGlobal=0x8041eb00;
 struct Binding {
  std::shared_ptr<HarborTutorialRuntime>runtime;
  std::weak_ptr<HarborSourceMenuRegistration>registration;
  const HarborTutorial*facade=nullptr;uint32_t battle=0,pouch=0;
  std::shared_ptr<const uint8_t>allocation;
  bool current(){
   auto slot=registration.lock();auto&r=*runtime;uint32_t b=0,p=0;
   return slot&&slot->runtime.lock()==runtime&&r.packed&&r.nativeAdmissionFacade==facade&&r.nativeAdmissionProof.current()&&r.scene==facade&&!r.nativeCompletionFrozen&&r.vm.gp.battleContext==1&&
    r.vm.memory.readWord(BattleGlobal,b)&&b==battle&&r.packed->sourceAllocationLifetime(b)==allocation&&r.vm.memory.readWord(PouchGlobal,p)&&p==pouch;
  }
 };
 auto binding=std::make_shared<Binding>();binding->runtime=r;binding->registration=sourceMenuRegistration_;binding->facade=this;
 if(!r->vm.memory.readWord(BattleGlobal,binding->battle)||!binding->battle||!r->vm.memory.readWord(PouchGlobal,binding->pouch)||!binding->pouch)return {};
 binding->allocation=r->packed->sourceAllocationLifetime(binding->battle);
 if((!binding->allocation&&binding->battle!=HarborTutorialPacked::Battle)||!binding->current())return {};
 NativeHarborRewardCounterServices services;services.lifetime=binding;services.user=binding.get();
 services.current=[](void*p){return static_cast<Binding*>(p)->current();};
 services.word=[](void*p,uint32_t at)->std::optional<uint32_t>{auto&b=*static_cast<Binding*>(p);uint32_t v;if(!b.current()||!b.runtime->vm.memory.readWord(at,v))return {};return v;};
 services.half=[](void*p,uint32_t at)->std::optional<uint16_t>{auto&b=*static_cast<Binding*>(p);uint16_t v;if(!b.current()||!b.runtime->vm.memory.readHalf(at,v))return {};return v;};
 services.byte=[](void*p,uint32_t at)->std::optional<uint8_t>{auto&b=*static_cast<Binding*>(p);uint8_t v;if(!b.current()||!b.runtime->vm.memory.readByte(at,v))return {};return v;};
 services.writeHalf=[](void*p,uint32_t at,uint16_t v){auto&b=*static_cast<Binding*>(p);return b.current()&&b.runtime->vm.memory.writeHalf(at,v);};
 services.sound=[](void*p){auto&b=*static_cast<Binding*>(p);constexpr uint32_t StarPointSound=0x802feaa8;return b.current()&&b.runtime->packed->sourceSound&&b.runtime->packed->sourceSound->menu(StarPointSound).has_value();};
 auto owner=std::make_shared<NativeHarborRewardCounter>(binding->battle,binding->pouch,std::move(services));r->nativeRewardCounter=owner;r->nativeRewardCounterMinted=true;return owner;
}
bool HarborTutorial::startNativeRewardVictoryEvent(){
 const auto proof=nativeGameplayFacadeProof();if(!proof.current())return false;
 auto&r=*runtime_;
 if(failed||r.nativeCompletionFrozen||r.vm.schedule.managerActive||!r.packed)return false;
 auto counter=r.nativeRewardCounter.lock();if(!counter||!counter->admit()||counter->total()<=0)return false;
 if(r.nativeVictoryTracked)return true;
 if(counter->phase()!=NativeHarborRewardCounter::Phase::Ready)return false;
 constexpr uint32_t SourceDataKey=0x40,TrackedEventOffset=0x2a8;
 // The original Crump setup has no active party. A nonnull party needs its
 // actual resource/appearance owner and is outside this bounded native entry.
 const auto battle=r.packed->battleAddress();const auto actors=nativeBattleActorServices(r.vm);
 if(!battle||battlePartyUnit(battle,actors))return false;
 if(!r.nativeVictoryRoot){
  const auto mario=battleMarioUnit(battle,actors);if(!mario)return false;
  const auto script=battleUnitData(mario,SourceDataKey,actors.memory);if(!script)return false;
  uint32_t owner=0,tracked=0;if(!r.vm.memory.readWord(mario,owner)||!r.vm.memory.readWord(mario+TrackedEventOffset,tracked))return false;
  // Native preflight for the original constructor's flat label scan. It
  // really reads the lookahead operand even at opcode1; supply no sentinel.
  constexpr unsigned NativeCommandBudget=256,SourceLabelCapacity=16;
  uint32_t at=script;unsigned labels=0;bool terminated=false;
  for(unsigned i=0;i<NativeCommandBudget;++i){uint32_t header=0,lookahead=0;
   if(!r.vm.memory.readWord(at,header)||!r.vm.memory.readWord(at+4,lookahead))return false;
   const auto count=int16_t(header>>16);if(count<0||uint64_t(at)+4+4*uint32_t(count)>UINT32_MAX)return false;
   for(int32_t arg=0;arg<count;++arg){uint32_t value=0;if(!r.vm.memory.readWord(at+4+4*uint32_t(arg),value))return false;}
   if((header&0xffff)==1){terminated=true;break;}
   if((header&0xffff)==3&&++labels>SourceLabelCapacity)return false;
   at+=4+4*uint32_t(count);
  }
  if(!terminated)return false;
  bool vacant=false;for(int32_t i=0;i<r.vm.battle.capacity;++i)vacant|=!(r.vm.battle.entries[i].flags&1);
  if(!vacant)return false;
  // Actual source priority10/flags0/root ownerID. Retain its real ID before
  // the tracked-unit store so a held store cannot create a second event.
  auto&e=r.root(script,int32_t(owner));r.nativeVictoryRoot=e.id;r.nativeVictoryUnit=mario;
 }
 if(!r.vm.memory.writeWord(r.nativeVictoryUnit+TrackedEventOffset,r.nativeVictoryRoot))return false;
 r.nativeVictoryTracked=true;return true;
}
bool HarborTutorial::nativeRewardVictoryEventComplete()const{
 const auto proof=nativeGameplayFacadeProof();if(!proof.current())return false;
 const auto&r=*runtime_;if(failed||r.nativeCompletionFrozen||r.vm.schedule.managerActive||!r.nativeVictoryTracked||!r.nativeVictoryRoot)return false;
 for(int32_t i=0;i<r.vm.battle.capacity;++i)if((r.vm.battle.entries[i].flags&1)&&r.vm.battle.entries[i].id==r.nativeVictoryRoot)return false;
 return true;
}
std::shared_ptr<NativeHarborCompletion>HarborTutorial::beginNativeCompletion(int32_t result){
 if(!runtime_||!runtime_->packed||!sourceMenuRegistration_)return {};
 auto r=runtime_;
 if(auto prior=r->nativeCompletion.lock())return result==1?prior:nullptr;
 if(r->nativeCompletionFrozen)return {}; // Explicit cancellation never replays partial teardown.
 if(!initialized||!introComplete||message.active()||r->vm.schedule.managerActive||pendingService||r->packed->hitEventsActive())return {};
 // Unit deletion can hide U+2b4 before its actual damage root retires.
 // Native admission permits only known ordinary wait roots to be cancelled;
 // every other original event must finish, regardless of registry visibility.
 if(r->vm.battle.capacity<0||r->vm.battle.capacity>256||(r->vm.battle.capacity&&!r->vm.battle.entries))return {};
 for(int32_t i=0;i<r->vm.battle.capacity;++i){const auto& e=r->vm.battle.entries[i];if(!(e.flags&1))continue;
  bool wait=false;
  for(const auto& actor:actors)wait|=actor.id>=0&&actor.wait&&e.owner==actor.id&&e.restartScript==actor.wait;
  wait|=systemActor.id>=0&&systemActor.wait&&e.owner==systemActor.id&&e.restartScript==systemActor.wait;
  if(!wait)return {};
 }
 const auto actual=nativeGameplayConclusion();if(!actual||*actual!=result||result!=1)return {};
 if(r->menuCursor.phase || (r->sourceDisplay&&r->sourceDisplay->pass()&&!r->sourceDisplay->complete(r->sourceDisplay->pass())))return {};
 uint32_t battle=0;if(!r->vm.memory.readWord(0x8041eb90,battle)||!battle)return {};
 const auto allocation=r->packed->sourceAllocationLifetime(battle);
 if(!allocation&&battle!=HarborTutorialPacked::Battle)return {};
 const std::weak_ptr<HarborSourceMenuRegistration>registration=sourceMenuRegistration_;
 const std::weak_ptr<HarborTutorialRuntime>weak=r;
 NativeHarborCompletionBinding binding;binding.vm=&r->vm;binding.packed=r->packed.get();binding.lifetime=r;binding.menuFiles=r->files;binding.result=result;
 binding.current=[registration,weak,battle,allocation]{
  const auto slot=registration.lock();if(!slot)return false;
  const auto expected=weak.lock();if(!expected||slot->runtime.lock()!=expected||!expected->packed)return false;
  uint32_t current=0;return expected->vm.memory.readWord(0x8041eb90,current)&&current==battle&&expected->packed->sourceAllocationLifetime(current)==allocation;
 };
 auto owner=std::make_shared<NativeHarborCompletion>(std::move(binding));
 r->nativeCompletion=owner;r->nativeCompletionFrozen=true;return owner;
}
std::optional<NativeHarborBattleIdentity>HarborTutorial::nativeFieldBattleIdentity()const{
 if(!triggered||failed||!initialized||!runtime_||runtime_->nativeCompletionFrozen||!sourceMenuRegistration_||sourceMenuRegistration_->runtime.lock()!=runtime_)return {};
 if(runtime_->nativeAdmissionFacade!=this||!runtime_->nativeAdmissionProof.current()||runtime_->scene!=this)return {};
 const std::weak_ptr<HarborSourceMenuRegistration>slot=sourceMenuRegistration_;
 const std::weak_ptr<HarborTutorialRuntime>weak=runtime_;
 const auto current=[slot,weak]{const auto owner=slot.lock();if(!owner)return false;const auto runtime=weak.lock();return runtime&&runtime->nativeAdmissionProof.current()&&runtime->nativeAdmissionFacade&&runtime->scene==runtime->nativeAdmissionFacade&&owner->runtime.lock()==runtime;};
 return NativeHarborBattleIdentity(runtime_,current,[current,weak](const NativeHarborCompletionReceipt& candidate){
  if(!current())return false;
  const auto runtime=weak.lock();if(!runtime)return false;
  const auto completion=runtime->nativeCompletion.lock();if(!completion)return false;
  const auto receipt=completion->receipt();if(!receipt)return false;
  const auto& actual=receipt->battleLifetime;const auto& supplied=candidate.battleLifetime;
  return actual&&supplied&&actual==supplied&&!actual.owner_before(supplied)&&!supplied.owner_before(actual)&&receipt->result==candidate.result&&receipt->pouch.bytes==candidate.pouch.bytes;
 });
}
std::optional<HarborNativeBodyDisplayAttributes>HarborTutorial::nativeBodyDisplayAttributes(int32_t actorId)const{
 const auto proof=nativeGameplayFacadeProof();if(!proof.current()||!runtime_||!runtime_->packed)return {};
 const auto owner=runtime_;uint32_t battle=0;
 if(!owner->vm.memory.readWord(0x8041eb90,battle)||battle!=owner->packed->battleAddress())return {};
 struct Context{HarborTutorialRuntime*runtime;const HarborTutorial*facade;NativeFacadeRegistration::Proof proof;};
 Context context{owner.get(),this,proof};HarborNativeBodyDisplayServices services;
 services.user=&context;services.lifetime=owner;
 services.current=[](void*p){const auto&c=*static_cast<Context*>(p);return c.proof.current()&&c.runtime->nativeAdmissionFacade==c.facade&&c.runtime->scene==c.facade;};
 services.word=[](void*p,uint32_t a)->std::optional<uint32_t>{uint32_t value;const auto&c=*static_cast<Context*>(p);if(!c.runtime->vm.memory.readWord(a,value))return {};return value;};
 return harborNativeBodyDisplayAttributes(owner->packed->unitAddress(actorId),services);
}
std::weak_ptr<const void>HarborTutorial::nativeGameplayLifetime()const{return runtime_;}
NativeFacadeRegistration::Proof HarborTutorial::nativeGameplayFacadeProof()const{
 if(!runtime_||runtime_->nativeAdmissionFacade!=this||!runtime_->nativeAdmissionProof.current()||runtime_->scene!=this)return {};
 return runtime_->nativeAdmissionProof;
}

std::optional<int32_t>HarborTutorial::nativeGameplayConclusion(){
 if(!runtime_||!runtime_->packed||failed||attackActive||damageActive||phaseActive)return {};
 auto&r=*runtime_;r.scene=this;r.packed->publish(*this);
 BattleConclusionServices services;services.actors=nativeBattleActorServices(r.vm);services.user=&r.vm;
 services.checkStatus=[](void*p,uint32_t u,int32_t status){return int32_t(checkPackedBattleStatus(u,status,nativeBattleActorServices(*static_cast<EventNativeRuntime*>(p))));};
 services.enemyBelong=[](void*p,uint32_t u){return packedEnemyBelong(u,nativeBattleActorServices(*static_cast<EventNativeRuntime*>(p)));};
 const auto battle=r.packed->battleAddress();if(!battleCheckConcluded(battle,services))return {};
 return battleInitialEndResult(battle,services);
}
bool HarborTutorial::nativeGameplayNextTurn(){
 if(!runtime_||!runtime_->packed||failed||attackActive||damageActive||phaseActive)return false;
 auto&r=*runtime_;r.scene=this;r.packed->publish(*this);
 BattleTurnFirstBackend backend;backend.runtime=&r.vm;
 const auto battle=r.packed->battleAddress();
 battleTurnFirstProcess(battle,nativeBattleTurnFirstServices(backend));
 uint16_t next=0;const bool mapped=r.vm.memory.readHalf(battle,next);assert(mapped);(void)mapped;
 turn=int16_t(next);r.packed->capture(*this);return true;
}
std::optional<NativeBattleStatusSnapshot>HarborTutorial::statusSnapshot()const{
 if(!runtime_||!runtime_->packed||failed)return {};
 auto&r=*runtime_;if(!r.statusReader){
  r.statusBattle=r.packed->battleAddress();r.statusBattleOwner=r.packed->sourceAllocationLifetime(r.statusBattle);
  if(!r.statusBattleOwner&&r.statusBattle!=HarborTutorialPacked::Battle)return {};
  r.statusReader=std::make_unique<NativeBattleStatusReader>(r.vm,r.statusLife,&r,[](void*p){auto&r=*static_cast<HarborTutorialRuntime*>(p);uint32_t b=0;return r.packed&&r.vm.memory.readWord(0x8041eb90,b)&&b==r.statusBattle&&r.packed->sourceAllocationLifetime(b)==r.statusBattleOwner&&r.vm.memory.contiguousRam(b,0x19100);});
 }
 return r.statusReader->snapshot();
}
bool HarborTutorial::prepareAudienceDrawing(const BattleAudienceDrawServices& supplied)const{if(!runtime_ || !runtime_->packed)return false;auto services=supplied;services.actors=nativeBattleActorServices(runtime_->vm);return drawPackedBattleAudience(services);}
// Actual animation driver acknowledges current actor pose generation. Reusing
// actor ID or waiting another simulation visit is insufficient for case 29.
void HarborTutorial::animationComplete(int32_t id){const auto slot=visualSlot(id);if(!runtime_ || slot<0)return;runtime_->complete[size_t(slot)]=true;runtime_->completeEpoch[size_t(slot)]=actors[size_t(slot)].poseAt;}
}
