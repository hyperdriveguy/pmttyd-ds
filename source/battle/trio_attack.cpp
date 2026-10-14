#include "trio_attack.hpp"
#include "battle_event_callbacks.hpp"
#include "event_memory_bootstrap.hpp"
#include "event_memory_release.hpp"
#include <cmath>
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <algorithm>
#include "battle_action_services.hpp"
#include "battle_timing_a.hpp"
#include "battle_weapon_cost.hpp"
#include "battle_weapon_power.hpp"
#include "battle_damage_application.hpp"
#include "event_lifecycle.hpp"
#include "battle_unit_resources.hpp"
#include "battle_phase.hpp"
#include "battle_conclusion.hpp"
#include "battle_general_rule.hpp"
#include "battle_audience_sound.hpp"
#include "battle_main.hpp"
#include "battle_hit_code_events.hpp"
#include "battle_attack_selection.hpp"
#include "battle_attack_registration.hpp"
#include "battle_status_window.hpp"
#include "battle_camera_requests.hpp"
#include "battle_camera_frame.hpp"
#include "battle_stage_objects.hpp"
#include "battle_stage_work.hpp"
#include "battle_action_help.hpp"
#include "battle_audience_init.hpp"
#include "battle_audience_resources.hpp"
#include "battle_audience_target.hpp"
#include "battle_audience_main.hpp"
#include "battle_break_slot.hpp"
#include "battle_hp_gauge.hpp"
#include "battle_audience_window.hpp"
#include "battle_audience_sound_main.hpp"
#include "battle_stage_floor.hpp"
#include "battle_sequence_startup.hpp"
#include "battle_drop_material.hpp"
#include "battle_packed_pouch.hpp"
#include "battle_dive.hpp"
#include "battle_hammer.hpp"
#include "field_camera_events.hpp"
#include "npc_presentation.hpp"
#include "battle_ending_fade.hpp"
#include "battle_packed_pose_lookup.hpp"
#include "battle_packed_unit_resources.hpp"
#include "battle_pose_pool.hpp"
#include "battle_packed_part.hpp"
#include <map>
namespace ttyd {
namespace {
EventScriptAssets scripts;NpcAngleTable sourceAngles;bool assetsReady=false;
std::string fieldPartnerAnimationPath;
std::weak_ptr<const ActorAnimationPack> fieldPartnerAnimationCache;
std::shared_ptr<const ActorAnimationPack> fieldMarioBank;
std::array<std::shared_ptr<const BattlePoseResource>,2> playerBattleResources;
std::shared_ptr<const BattlePoseResource> franklyBattleResource;
std::string stageMapRoot;
std::array<uint32_t,7> stageDescriptors{};
uint32_t sourceStage=0;
uint32_t audienceSetupConfig=0;
BattleAudienceResources audienceResources;
std::shared_ptr<const ActorAnimationPack> fieldPartnerAnimationBank() {
    if(auto cached=fieldPartnerAnimationCache.lock())return cached;
    auto bank=std::make_shared<ActorAnimationPack>();
    if(!bank->load(fieldPartnerAnimationPath.c_str()) || !bank->pose("PKR_S_1"))return {};
    fieldPartnerAnimationCache=bank;return bank;
}
std::array<std::array<uint32_t,4>,3> reactionUnits{};uint32_t reactionGroup=0,franklyPhase=0;
constexpr uint32_t Battle=0x84000000,Units=0x84100000,Parts=0x84200000,Kinds=0x84300000,Work=0x84400000;
constexpr std::array<unsigned,7> visual{{0,0,1,3,5,4,2}};
void required(bool ok){if(!ok)std::abort();}
uint32_t bits(float v){uint32_t b;std::memcpy(&b,&v,4);return b;}
}
std::shared_ptr<const ActorAnimationPack> fieldMarioAnimationBank() {return fieldMarioBank;}
bool loadTrioAttackAssets(const char* root) {
    if(!root || !loadProloguePouchAssets(root) || !audienceResources.load(root))return false;
    EventScriptAssets next;if(!next.load((std::string(root)+"/trio-attack-events.pmevt").c_str()))return false;
    uint32_t command=0;if(!next.readWord(0x80355cf0,command) || command!=0x5005b)return false;
    FILE* f=std::fopen((std::string(root)+"/trio-attack-units.bin").c_str(),"rb");if(!f)return false;
    char magic[8];uint32_t count=0;decltype(reactionUnits) units{};uint32_t group=0,phase=0;
    const bool valid=std::fread(magic,8,1,f)==1 && !std::memcmp(magic,"PMTHIT3",8) && std::fread(&count,4,1,f)==1 && count==3 && std::fread(units.data(),16,3,f)==3 && std::fread(&group,4,1,f)==1 && std::fread(&phase,4,1,f)==1 && std::fgetc(f)==EOF;
    std::fclose(f);if(!valid)return false;
    if(!sourceAngles.load((std::string(root)+"/npc-angle.bin").c_str()))return false;
    auto marioBank=std::make_shared<ActorAnimationPack>();
    if(!marioBank->load((std::string(root)+"/characters.pmanim").c_str()))return false;
    for(const char* name:{"M_S_1","M_S_1R","M_I_5"}) {
        const auto* base=marioBank->pose(name);ActorAnimationSample sample;
        if(!base || !base->evaluate(base->start,sample) || sample.vertices.empty())return false;
    }
    fieldMarioBank=std::move(marioBank);
    std::array<std::shared_ptr<const BattlePoseResource>,2> battleResources;
    for(unsigned i=0;i<battleResources.size();++i){
        const auto prefix=std::string(root)+(i==0?"/trio-attack-mario":"/trio-attack-goombella");
        battleResources[i]=loadBattlePoseResource((prefix+".pose-directory.bin").c_str(),(prefix+".pmanim").c_str());
        if(!battleResources[i] || battleResources[i]->modelName!=(i==0?"b_mario":"c_pkr"))return false;
    }
    const auto professorPrefix=std::string(root)+"/professor";
    auto professorResource=loadBattlePoseResource((professorPrefix+".pose-directory.bin").c_str(),(professorPrefix+".pmanim").c_str());
    if(!professorResource || professorResource->modelName!="c_kuri_h")return false;
    franklyBattleResource=std::move(professorResource);
    fieldPartnerAnimationPath=std::string(root)+"/partner.pmanim";fieldPartnerAnimationCache.reset();
    FILE* stageFile=std::fopen((std::string(root)+"/trio-stage-objects.bin").c_str(),"rb");if(!stageFile)return false;
    std::array<uint32_t,8> stageData{};char stageMagic[8];
    const bool stageValid=std::fread(stageMagic,8,1,stageFile)==1 && !std::memcmp(stageMagic,"PMSTOBJ1",8) && std::fread(stageData.data(),4,8,stageFile)==8 && std::fgetc(stageFile)==EOF;
    std::fclose(stageFile);if(!stageValid)return false;
    uint32_t descriptorWord=0;
    for(unsigned i=1;i<stageData.size();++i)if(!next.readWord(stageData[i],descriptorWord) || !descriptorWord)return false;
    sourceStage=stageData[0];std::copy(stageData.begin()+1,stageData.end(),stageDescriptors.begin());stageMapRoot=root;
    FILE* configFile=std::fopen((std::string(root)+"/trio-audience-setup.bin").c_str(),"rb");if(!configFile)return false;
    char configMagic[8];uint32_t config=0;
    const bool configValid=std::fread(configMagic,8,1,configFile)==1 && !std::memcmp(configMagic,"PMAUCFG1",8) && std::fread(&config,4,1,configFile)==1 && std::fgetc(configFile)==EOF;
    std::fclose(configFile);if(!configValid || !next.readWord(config+0x1c,descriptorWord) || descriptorWord)return false;
    audienceSetupConfig=config;
    scripts=std::move(next);reactionUnits=units;reactionGroup=group;franklyPhase=phase;playerBattleResources=std::move(battleResources);assetsReady=true;return true;
}
struct TrioAttackRuntime {
    EventNativeRuntime vm;
    EventCallbackRegistry registry;
    BattleDataChildServices child;
    BattleTakeoffBackend takeoffBackend;
    BattlePackedTakeoffServices takeoff;
    BattlePackedMovePositionServices movement;
    BattleMarioJumpBackend jumpBackend;BattlePackedMarioJumpServices jump;
    std::shared_ptr<BattleRandom> randomOwner;BattleRandom& random;
    std::shared_ptr<PositionalSfxRegistry> publicSfx;
    BattlePadWork generalPad,privatePad;BattleTriggerHistory history;
    std::array<DamageUnit,7> damageUnits{};std::array<DamagePart,7> damageParts{};
    std::array<std::array<int8_t,5>,7> defenses{},defenseAttributes{};
    std::vector<int32_t> rumbleRequests;
    std::vector<DamageEffect> damageEffects;std::vector<std::string> soundRequests;std::vector<DamageNotice> notices;
    ProfessorStory* scene=nullptr;
    uint64_t now=0,sampledAt=UINT64_MAX;uint32_t root=0,pending=0;
    bool done=false;int32_t readyActor=-1;
    std::vector<ProfessorServiceRequest> trace;
    std::vector<TrioAudienceAudioRequest> audienceAudioTrace;
    std::shared_ptr<ProloguePouch> entryPouch;
    std::array<PartnerBattleSnapshot,7> partySnapshots{};
    BattlePosePool poses;
    BattleCameraFrame cameraFrame;
    uint64_t cameraClockAt=UINT64_MAX;
    bool cameraHistoryComplete=true;
    std::optional<TrioPoseDrawRequest> poseDraw;
    std::optional<CameraDepthSnapshot> cameraDepth;
    std::optional<std::array<float,4>> cameraDepthLensAdapter;
    DisplayQueue displayQueue;
    std::shared_ptr<ShadowWork> shadows;
    ShadowMainCursor shadowMainCursor;
    bool displayActive=false,displayUnitPrefixDone=false;
    int32_t displayIndex=0;uint32_t displayPart=0;
    uint8_t displayFooter=0,stageRegistration=0;bool stockExpMutationDone=false,stageGetterDone=false;
    bool waitingPoseDepth=false;
    std::optional<float> waitingPosePriority;
    std::optional<TrioShadowDrawRequest> shadowDraw;
    bool registerPoseDraw() {
        if(!poseDraw || (!waitingPosePriority && !cameraDepth)){pending=0x8001086c;waitingPoseDepth=true;return false;}
        const auto& draw=*poseDraw;
        if(!waitingPosePriority)waitingPosePriority=cameraQueueDepth(*cameraDepth,draw.depth);
        const float priority=*waitingPosePriority;
        endingRequest(0x80010bc4,{draw.camera,draw.drawClass,int32_t(0x80101774),int32_t(draw.part)});
        const auto admitted=displayQueue.enter(uint32_t(draw.camera),draw.drawClass,0x80101774,draw.part,priority);
        if(admitted==DisplayAdmission::Unavailable){pending=0x80010bc4;waitingPoseDepth=true;return false;}
        waitingPoseDepth=false;
        const auto part=draw.part;
        if(word(part+0x1ac)&0x04000000){pending=0x80010bc4;return false;}
        return completePartShadow(part);
    }
    bool completePartSoundTail(uint32_t part){
        endingRequest(0x801261f8,{int32_t(part)});
        if(word(part+0x4d0) && word(part+0x1c0)!=0xffffffff){pending=0x80039cf8;return false;}
        return true;
    }
    bool completePartShadow(uint32_t part){
        const auto unit=word(part+0x4ec);
        if((word(unit+0x104)&0x20000000) || (word(part+0x1ac)&0x02000000))return completePartSoundTail(part);
        endingRequest(0x80128b10,{int32_t(part)});
        BattlePartPositionServices position;position.memory=nativeBattleActorServices(vm);
        position.fma=[](void*,float a,float b,float c){return std::fma(a,b,c);};
        position.add=[](void*,float a,float b){return a+b;};
        auto shadowPosition=packedPartWorldPosition(part,position);
        const float worldY=shadowPosition[1];
        if(word(unit+0x1c)&0x10000){pending=0x800f86f4;endingRequest(pending,{int32_t(Battle),int32_t(unit)});return false;}
        endingRequest(0x800f7b84,{int32_t(Battle)});
        const float floor=packedBattleStageFloorHeight(Battle,shadowPosition[0],shadowPosition[1],shadowPosition[2],position.memory.memory);
        shadowPosition[1]=floor+real(0x80422228);
        float factor=worldY<=floor?real(0x8042224c):(real(0x80422270)-(worldY-floor))/real(0x80422270);
        if(double(factor)>1.)factor=real(0x8042224c);
        if(factor<real(0x80422274))factor=real(0x80422274);
        float radius=float(int16_t(half(part+0x1a2)))*real(unit+0x90);
        radius=real(part+0x60)*radius;radius=real(unit+0x114)*radius;
        radius=factor*radius;radius=radius*real(part+0x78);
        if((word(unit+0x1c)&0x10000) && radius<real(0x80422230))radius=real(0x80422230);
        if(radius<real(0x80422228))radius=real(0x80422278);
        shadowDraw=TrioShadowDrawRequest{shadowPosition,radius};
        endingRequest(0x8002be98);
        if(!shadows || !shadows->enter(shadowPosition,radius)){pending=0x8002be98;return false;}
        return completePartSoundTail(part);
    }
    std::map<uint32_t,int32_t> partPoseHandles;
    std::array<TrioRewardIcon,19> rewardIcons{};
    uint64_t rewardIconClockAt=UINT64_MAX,gaugeClockAt=UINT64_MAX;
    PointGetEffect pointGet;uint64_t pointGetClockAt=UINT64_MAX;
    BattleEndingFade endingFade;uint64_t endingFadeClockAt=UINT64_MAX;bool endingExported=false;uint32_t* fieldNpcBattleFlags=nullptr;
    FieldMarioBattleState* fieldMario=nullptr;
    FieldPartyRegistry* fieldParties=nullptr;
    FieldPartyAppearanceServices fieldAppearance;
    StageMapObjects stageBackground,stageForeground;
    std::shared_ptr<BattleSpotlightState> spotlight;
    bool spotlightInitialized=false;
    explicit TrioAttackRuntime(std::shared_ptr<BattleRandom> rng):vm(0x81000000,platform(this)),randomOwner(std::move(rng)),random(*randomOwner) {}
    ~TrioAttackRuntime(){if(spotlightInitialized)publishSpotlight();}
    static EventNativePlatform platform(TrioAttackRuntime* self) {
        EventNativePlatform p;p.user=self;p.clock=[](void*)->uint32_t{return 243000000;};
        p.call=[](void* p,EventEntry& e,uint32_t fn,int32_t first)->int32_t{return static_cast<TrioAttackRuntime*>(p)->call(e,fn,first);};
        p.truncate=[](void*,float v)->int32_t{return int32_t(v);};p.divideWord=[](void*,int32_t a,int32_t b)->int32_t{return a/b;};
        p.irand=[](void* p,int32_t bound){return int32_t(static_cast<TrioAttackRuntime*>(p)->random.bounded(bound));};
        p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){};
        p.signedWordFloat=[](void*,int32_t v){return float(v);};p.addFloat=[](void*,float a,float b){return a+b;};
        p.subtractFloat=[](void*,float a,float b){return a-b;};p.multiplyFloat=[](void*,float a,float b){return a*b;};p.divideFloat=[](void*,float a,float b){return a/b;};
        p.fmaFloat=[](void*,float a,float b,float c){return std::fma(a,b,c);};p.negativeMultiplySubtractFloat=[](void*,float a,float b,float c){return -std::fma(a,b,-c);};
        p.distanceFloat=[](void*,float a,float b,float c,float d){return std::hypot(c-a,d-b);};
        p.headingFloat=[](void*,float a,float b,float c,float d){return float(std::atan2(double(c-a),double(b-d))*180/3.141592653589793);};
        p.sineDouble=[](void*,double v){return std::sin(v);};p.cosineDouble=[](void*,double v){return std::cos(v);};p.roundDoubleFloat=[](void*,double v){return float(v);};
        p.addDouble=[](void*,double a,double b){return a+b;};p.subtractDouble=[](void*,double a,double b){return a-b;};p.multiplyDouble=[](void*,double a,double b){return a*b;};p.divideDouble=[](void*,double a,double b){return a/b;};
        p.fmaDouble=[](void*,double a,double b,double c){return std::fma(a,b,c);};p.fmodDouble=[](void*,double a,double b){return std::fmod(a,b);};return p;
    }
    uint32_t word(uint32_t a)const{uint32_t v=0;required(vm.memory.readWord(a,v));return v;}
    void word(uint32_t a,uint32_t v){required(vm.memory.writeWord(a,v));}
    uint8_t byte(uint32_t a)const{uint8_t v=0;required(vm.memory.readByte(a,v));return v;}
    void byte(uint32_t a,uint8_t v){required(vm.memory.writeByte(a,v));}
    int16_t half(uint32_t a)const{uint16_t v=0;required(vm.memory.readHalf(a,v));return int16_t(v);}
    void half(uint32_t a,int32_t v){required(vm.memory.writeHalf(a,uint16_t(v)));}
    void real(uint32_t a,float v){word(a,bits(v));}
    float real(uint32_t a)const{const auto v=word(a);float f;std::memcpy(&f,&v,4);return f;}
    void publishSpotlight() {
        const auto w=word(0x804175a0);
        for(unsigned i=0;i<3;++i)spotlight->work[i]=word(w+4*i);
        spotlight->entryColor=word(0x804295d4);
    }
    BattleSpotlightServices spotlightServices() {
        BattleSpotlightServices s;s.memory=vm.memoryBinding.services();s.user=this;
        s.allocate=[](void* p,uint32_t size)->std::optional<uint32_t>{auto& r=*static_cast<TrioAttackRuntime*>(p);r.endingRequest(0x800f8934);const auto a=eventMapAllocate(r.word(0x8041e80c),size,r.vm.memoryBinding.services());return a?std::optional<uint32_t>(a):std::nullopt;};
        s.free=[](void* p,uint32_t a){auto& r=*static_cast<TrioAttackRuntime*>(p);r.endingRequest(0x800f8908);eventMapFree(r.word(0x8041e80c),a,r.vm.memoryBinding.services());return true;};
        s.fma=[](void*,float a,float b,float c){return std::fma(a,b,c);};
        s.display=[](void* p,int32_t,int32_t,uint32_t,uint32_t,float){auto& r=*static_cast<TrioAttackRuntime*>(p);r.endingRequest(0x80010bc4);r.pending=0x80010bc4;return false;};
        return s;
    }
    StageMapObjects* stageMapOwner(const std::string& name) {
        if(stageForeground.flags(name))return &stageForeground;
        if(stageBackground.flags(name))return &stageBackground;
        return nullptr;
    }
    bool initializeStageObjects() {
        if(half(0x85000000+0x88)!=0)return false; // Signed rank halfword; following halfword is level.
        if(!stageBackground.load((stageMapRoot+"/bti_01.mapobj").c_str()) || !stageForeground.load((stageMapRoot+"/stg_00_1.mapobj").c_str()))return false;
        word(Battle+0x18110,0);word(Battle+0x18114,0x80374c38);byte(Battle+0x182a6,255);word(Battle+0x182b8,0);word(Battle+0x182bc,0);
        // Typed current map root is retained from the original hierarchy pack.
        if(!stageForeground.root() || !stageForeground.flag(stageForeground.root(),2,false))return false;
        auto flag=[&](const std::string& name,bool on,uint32_t mask=1){auto* owner=stageMapOwner(name);return owner && owner->flag(name,mask,on);};
        for(const auto* name:{"A1","A2","B","C"})if(auto* owner=stageMapOwner(name)){owner->flag(name,1,true);owner->flag(name,2,false);}
        if(!flag("escape_wall",true))return false;
        for(unsigned i=0;i<stageDescriptors.size();++i) {
            const auto r=Battle+0x1715c+0x7c*i,d=stageDescriptors[i];
            word(r,i+1);word(r+0x64,d);word(r+0x6c,0xffffffff);
            for(unsigned axis=0;axis<3;++axis)word(r+4+4*axis,word(d+8+4*axis));
            if(i==5)word(r+8,word(Battle+0x16f24));
            for(unsigned axis=0;axis<3;++axis)word(r+0x1c+4*axis,0);
            for(unsigned value=0;value<12;++value)word(r+0x34+4*value,word(0x802f42b8+4*value));
            if(!flag(text(word(d)),i>=5))return false;
            if(i>=5)word(r+0x68,word(r+0x68)|1);
        }
        // All five original setup4 script fields are null. Keep a real gate.
        for(const auto offset:{0x190u,0x1a0u,0x1a4u,0x1a8u,0x1acu})if(word(sourceStage+offset))return false;
        byte(Battle+0x180f8,byte(Battle+0x180f8)|0x20);
        return flag("if_body",true) && flag("if_wire",true);
    }
    bool updateStageObjects() {
        BattleStageObjectServices s;s.actors=nativeBattleActorServices(vm);s.user=this;
        s.translate=[](void* p,uint32_t name,const StageMapObjects::Point& value){auto& r=*static_cast<TrioAttackRuntime*>(p);auto* m=r.stageMapOwner(r.text(name));return m && m->translate(r.text(name),value);};
        s.rotate=[](void* p,uint32_t name,const StageMapObjects::Point& value){auto& r=*static_cast<TrioAttackRuntime*>(p);auto* m=r.stageMapOwner(r.text(name));return m && m->rotate(r.text(name),value);};
        s.groupFlags=[](void* p,uint32_t name)->std::optional<uint32_t>{auto& r=*static_cast<TrioAttackRuntime*>(p);auto* m=r.stageMapOwner(r.text(name));return m?m->flags(r.text(name)):std::nullopt;};
        s.groupFlag=[](void* p,uint32_t name,uint32_t mask,bool on){auto& r=*static_cast<TrioAttackRuntime*>(p);auto* m=r.stageMapOwner(r.text(name));return m && m->flag(r.text(name),mask,on);};
        return packedBattleStageObjectMain(s);
    }
    bool updateStageWork() {
        BattleStageWorkServices s;s.actors=nativeBattleActorServices(vm);s.user=this;
        s.spotlightMain=[](void* p){auto& r=*static_cast<TrioAttackRuntime*>(p);r.endingRequest(0x8013800c);const bool ready=packedBattleSpotlightMain(r.spotlightServices());r.publishSpotlight();return ready;};
        s.activeEntry=[](void* p,uint32_t){static_cast<TrioAttackRuntime*>(p)->pending=0x80139150;return false;};
        s.freeScratch=[](void* p,uint32_t allocation){auto& r=*static_cast<TrioAttackRuntime*>(p);r.endingRequest(0x800f8908);eventMapFree(r.word(0x8041e80c),allocation,r.vm.memoryBinding.services());return true;};
        s.interpolate=[](void* p,int32_t,int32_t,int32_t,float,float)->std::optional<float>{auto& r=*static_cast<TrioAttackRuntime*>(p);r.endingRequest(0x80006c8c);r.pending=0x80006c8c;return std::nullopt;};
        return packedBattleStageWorkMain(s);
    }
    BattleAudienceInitServices audienceInitServices() {
        BattleAudienceInitServices s;s.actors=nativeBattleActorServices(vm);s.user=this;
        s.random=[](void* p,int32_t bound){auto& r=*static_cast<TrioAttackRuntime*>(p);r.endingRequest(0x80005da0,{bound});return r.random.bounded(bound);};
        s.fma=[](void*,float a,float b,float c){return std::fma(a,b,c);};
        s.setting=[](void* p){auto& r=*static_cast<TrioAttackRuntime*>(p);r.endingRequest(0x801a60c4);if(initializePackedSetup4ZeroAudience(r.audienceInitServices()))return true;r.pending=0x801a60c4;return false;};
        s.texture=[](void* p,const char* path)->std::optional<uint32_t>{auto& r=*static_cast<TrioAttackRuntime*>(p);r.endingRequest(0x801a86ac);return audienceResources.handle(path);};
        s.noise=[](void* p){auto& r=*static_cast<TrioAttackRuntime*>(p);r.endingRequest(0x801a0b88);return noiseAlwaysBattleAudienceSound(r.audienceSoundServices());};
        s.target=[](void* p){auto& r=*static_cast<TrioAttackRuntime*>(p);r.endingRequest(0x801a1c60);if(updatePackedBattleAudienceZeroTarget(nativeBattleActorServices(r.vm)))return true;r.pending=0x801a1708;return false;};
        return s;
    }
    BattleBreakSlotServices slotServices() {
        BattleBreakSlotServices s;s.actors=nativeBattleActorServices(vm);s.user=this;
        s.allocate=[](void* p,uint32_t bytes)->std::optional<uint32_t>{auto& r=*static_cast<TrioAttackRuntime*>(p);r.endingRequest(0x800f8934,{int32_t(bytes)});const auto address=eventMapAllocate(r.word(0x8041e80c),bytes,r.vm.memoryBinding.services());if(!address)r.pending=0x800f8934;return address;};
        s.free=[](void* p,uint32_t address){auto& r=*static_cast<TrioAttackRuntime*>(p);r.endingRequest(0x800f8908,{int32_t(address)});if(address)eventMapFree(r.word(0x8041e80c),address,r.vm.memoryBinding.services());return true;};
        return s;
    }
    bool updateAudience() {
        BattleAudienceMainServices s;s.actors=nativeBattleActorServices(vm);s.user=this;
        s.request=[](void* p,uint32_t fn) {
            auto& r=*static_cast<TrioAttackRuntime*>(p);r.endingRequest(fn);
            const auto actors=nativeBattleActorServices(r.vm);
            if(fn==0x801a1cdc){r.byte(r.word(0x8041eb90)+0x2858+0x1375c,1);return true;}
            if(fn==0x801a5ca8) {
                BattleAudienceControlServices control;control.actors=actors;control.user=&r;
                control.actor=[](void* p,uint32_t,int32_t index,bool){auto& r=*static_cast<TrioAttackRuntime*>(p);r.endingRequest(0x80192a0c,{index});r.pending=0x80192a0c;return false;};
                control.random=[](void* p,int32_t bound){auto& r=*static_cast<TrioAttackRuntime*>(p);r.endingRequest(0x80005da0,{bound});return r.random.bounded(bound);};
                control.itemOn=[](void* p,int32_t index){auto& r=*static_cast<TrioAttackRuntime*>(p);r.endingRequest(0x801a5aac,{index,0,1});auto result=packedBattleAudienceEmptyItemOn(index,nativeBattleActorServices(r.vm));if(!result)r.pending=0x801a5aac;return result;};
                if(packedBattleAudienceControl(control))return true;
            } else if(fn==0x801a4f0c) {
                if(packedBattleAudienceInactiveItems(actors))return true;
            } else if(fn==0x801a4998) {
                if(packedBattleAudienceInactiveApSources(actors))return true;
            } else if(fn==0x801a4570) {packedBattleAudienceWindowMain(actors);return true;
            } else if(fn==0x8019f97c) {
                BattleAudienceSoundMainServices sound;sound.actors=actors;sound.user=&r;
                sound.noise=[](void* p){auto& r=*static_cast<TrioAttackRuntime*>(p);r.endingRequest(0x8019f7b4);return noiseMainBattleAudienceSound(r.audienceSoundServices());};
                sound.check=[](void* p,int32_t handle){auto& r=*static_cast<TrioAttackRuntime*>(p);r.endingRequest(0x800d95a0,{handle});auto v=r.publicSfx->check(handle);if(!v)r.pending=0x800d95a0;return v;};
                sound.stop=[](void* p,int32_t handle){auto& r=*static_cast<TrioAttackRuntime*>(p);return r.audienceSoundServices().stop(&r,handle);};
                sound.setVolume=[](void* p,int32_t handle,uint8_t volume){auto& r=*static_cast<TrioAttackRuntime*>(p);r.endingRequest(0x800d97d0,{handle,volume});if(r.publicSfx->setVolume(handle,volume))return true;r.pending=0x800d97d0;return false;};
                sound.truncate=[](void* p,float value)->std::optional<int32_t>{if(std::isfinite(value) && value<2147483648.f && value>=-2147483648.f)return int32_t(value);static_cast<TrioAttackRuntime*>(p)->pending=0x80006c8c;return {};};
                if(packedBattleAudienceSoundMain(sound))return true;
            }
            if(!r.pending)r.pending=fn;
            return false;
        };
        return packedBattleAudienceMain(s);
    }
    int32_t raw(EventEntry& e,unsigned i)const{return int32_t(word(e.arguments+4*i));}
    int32_t value(EventEntry& e,unsigned i){return getEventValue(e,raw(e,i),vm.interpreterServices().operands);}
    void output(EventEntry& e,unsigned i,int32_t v){setEventValue(e,raw(e,i),v,vm.interpreterServices().operands);}
    uint32_t unit(EventEntry& e,unsigned operand=0){const int32_t id=translateBattleActor(&e,value(e,operand),nativeBattleActorServices(vm));return word(Battle+0x20+4*id);}
    std::string text(uint32_t address)const{std::string s;for(unsigned i=0;i<128;++i){const auto c=byte(address+i);if(!c)return s;s+=char(c);}return {};}
    uint32_t selectedWeapon(){const auto selected=packedSelectedCommandWeapon(Battle,nativeBattlePackedSelectionServices(vm));required(selected.has_value());return *selected;}
    int32_t call(EventEntry& e,uint32_t fn,int32_t first) {
        if(first){ProfessorServiceRequest r{fn,e.currentCommand,now,{}};for(unsigned i=0;i<e.argumentCount;++i)r.rawOperands.push_back(raw(e,i));trace.push_back(std::move(r));}
        int32_t result;if(registry.invoke(e,fn,first,result))return result;
        switch(fn) {
        case 0x80212794:{const auto u=unit(e);if(word(u+0x104)&0x20){if(!pending)pending=0x801267cc;return 0;}output(e,1,-1);return 2;}
        case 0x801123d0:{const auto u=unit(e);std::array<float,3> position{};for(unsigned i=0;i<3;++i)position[i]=float(value(e,i+1));for(unsigned i=0;i<3;++i)real(u+0x30+4*i,position[i]);return 2;}
        case 0x8010e368:{const auto u=unit(e);word(u+0x104,word(u+0x104)&~uint32_t(raw(e,1)));return 2;}
        case 0x8010e1b8:{const auto p=packedUnitPart(unit(e),value(e,1),nativeBattleActorServices(vm).memory);word(p+0x1ac,word(p+0x1ac)&~uint32_t(raw(e,2)));return 2;}
        case 0x80106468:{const auto p=packedUnitPart(unit(e),value(e,1),nativeBattleActorServices(vm).memory);word(p+0x1bc,uint32_t(raw(e,2)));return 2;}
        case 0x8010dae0:word(unit(e)+0x18,uint32_t(raw(e,1)));return 2;
        case 0x8010dc44:word(unit(e)+8,uint32_t(value(e,1)));return 2;
        case 0x8010a500:case 0x8010a388:case 0x8010a55c:case 0x8010a32c:{const auto u=unit(e);const unsigned offset=fn==0x8010a500?0x28c:fn==0x8010a388?0x2a0:fn==0x8010a55c?0x2b0:0x2a4;word(u+offset,uint32_t(raw(e,1)));if(fn==0x8010a388)word(u+0x2ac,0);return 2;}
        case 0x80104b08:{const auto u=unit(e);word(u+0x1c4,uint32_t(value(e,1)));word(u+0x1c8,uint32_t(value(e,2)));return 2;}
        case 0x80104ddc:case 0x80104f64:{const auto u=unit(e);std::array<int32_t,5> args{};for(unsigned i=0;i<5;++i)args[i]=value(e,i+1);const auto base=u+(fn==0x80104ddc?0x1dc:0x1cc);word(base,args[0]);word(base+4,args[1]);for(unsigned i=0;i<3;++i)half(base+8+2*i,args[i+2]);return 2;}
        case 0x8011319c:{
            const auto u=unit(e),p=packedUnitPart(u,value(e,1),nativeBattleActorServices(vm).memory),record=uint32_t(raw(e,2));const auto id=uint32_t(value(e,3));
            if(!record || word(record)!=id || word(u+8)!=2 || id!=1){if(!pending)pending=fn;return 0;}
            word(p+4,record);word(p+8,word(record+4));for(unsigned o:{0x40u,0x44u,0x38u,0x3cu})word(p+(o==0x40?0x1ac:o==0x44?0x1b0:o==0x38?0x1b4:0x1b8),word(record+o));
            for(unsigned axis=0;axis<3;++axis){real(p+0x24+4*axis,real(record+0xc+4*axis));real(p+0x48+4*axis,0);real(p+0x54+4*axis,0);}real(p+0x170,real(record+0x18));real(p+0x174,real(record+0x1c));real(p+0x17c,0);real(p+0x180,0);half(p+0x1a0,0);
            // Both authored variants use c_kuribo. The preloaded native bank
            // reuses its geometry/pose IDs; the following source pose request resets animation.
            return 2;
        }
        case 0x8015235c:if(raw(e,1)!=0){if(!pending)pending=fn;return 0;}soundRequests.push_back(text(uint32_t(raw(e,0))));return 2; // Original request retained; returned voice is discarded by this source call.
        case 0x80115648:output(e,0,int8_t(byte(word(0x8041eb00)+0x99)));return 2;
        case 0x80107ad4:word(Battle+0x1cc4,uint32_t(raw(e,0)));return 2;
        case 0x80107a64:for(unsigned i=0;i<8;++i){const auto parameter=value(e,i);if(uint32_t(parameter)!=0xf1194d80)word(Battle+0x1cc8+4*i,uint32_t(parameter));}return 2;
        case 0x8010510c:return battleStageFloorPositionEvent(e,nativeBattleUnitEventServices(vm),vm.platform.user,vm.platform.signedWordFloat);
        case 0x80056120:{std::array<int32_t,4> args{};for(unsigned i=0;i<4;++i)args[i]=value(e,i);e.locals[11]=args[0];e.locals[12]=args[1];e.locals[13]=args[2];e.locals[14]=0;e.locals[15]=args[3];return 2;}
        case 0x80055f90:{if(e.locals[11]!=5){if(!pending)pending=fn;return 0;}const auto sample=cameraInterpolation(e.locals[11],e.locals[14],e.locals[15],float(e.locals[12]),float(e.locals[13]));required(sample.has_value());setEventFloat(e,raw(e,0),*sample,vm.interpreterServices().operands);output(e,1,e.locals[14]<e.locals[15]?1:0);e.locals[14]=int32_t(uint32_t(e.locals[14])+1);return 2;}
        case 0x80105020:{const auto u=unit(e);std::array<int32_t,7> a{};for(unsigned i=0;i<7;++i)a[i]=value(e,i+1);const auto record=u+0x18c;word(record,uint32_t(a[0]));word(record+4,uint32_t(a[1]));half(record+0xc,a[2]);half(record+0xe,a[3]);half(record+0x10,a[3]);half(record+0x14,0);half(record+8,a[5]);half(record+0xa,a[6]);word(record+0x18,0xffffffff);return 2;}
        case 0x80107694:{const auto u=unit(e),w=uint32_t(value(e,1));word(Battle+0x1cb0,0);readPad(Battle+0x1d1c,privatePad);DefenseContext context;context.hp=half(u+0x10c);context.simplifiers=byte(u+0x305);context.unsimplifiers=byte(u+0x306);context.automatic=byte(u+0x307);context.weaponGate=byte(w+0x13);DefenseCommand command;command.overrideCounter=half(Battle+0x1d18);command.evaluate(privatePad,context);half(Battle+0x1d18,command.overrideCounter);word(Battle+0x1cb0,command.result);writePad(Battle+0x1d1c,privatePad);return 2;}
        case 0x80113f1c:return battleMarkUnit1feEvent(e,nativeBattleUnitEventServices(vm));
        case 0x80113708:output(e,0,word(Battle+0xf18)==uint32_t(raw(e,1)));return 2;
        case 0x801137b8:output(e,0,half(word(0x8041eb90)));return 2;
        case 0x80104498:{const auto u=unit(e);if(word(u+0x308)){if(!pending)pending=fn;return 0;}output(e,1,0);return 2;}
        case 0x8010eed4:{const auto side=int8_t(byte(unit(e)+0xc));output(e,1,side==0?1:side==1?0:side);return 2;}
        case 0x8010b664:{const auto actor=translateBattleActor(&e,value(e,0),nativeBattleActorServices(vm));const int32_t side=int8_t(value(e,1));const auto w=uint32_t(value(e,2)),attributes=word(w+0x68);int32_t direction=int8_t(-byte(Battle+0xa+8*side));if(attributes&0x10000000)direction=int8_t(-direction);
            samplePackedBattleCandidates({nativeBattleActorServices(vm),Battle+0x428},w,actor,side,word(w+0x64),attributes,direction,nativeBattleSamplingServices(vm));return 2;}
        case 0x8010b5e4:{const auto w=uint32_t(value(e,0));std::array<int32_t,2> selected{};CandidateChoiceOutputs outputs;outputs.user=&selected;outputs.write=[](void* p,bool unitOutput,int32_t v){(*static_cast<std::array<int32_t,2>*>(p))[unitOutput?0:1]=v;};
            choosePackedBattleCandidate({nativeBattleActorServices(vm),Battle+0x428},word(w+0x7c),nativeBattleChoiceServices(vm),outputs);output(e,1,selected[0]);output(e,2,selected[1]);return 2;}
        case 0x8010ad00: { // Source fresh no-protector domain; retain both output writes.
            (void)unit(e);const int32_t target=translateBattleActor(&e,value(e,1),nativeBattleActorServices(vm));const int32_t part=value(e,2);
            for(unsigned id=1;id<7;++id)if(int32_t(word(Units+id*0x1000+0x140))==target){if(!pending)pending=fn;return 0;}
            output(e,1,target);output(e,2,part);return 2;
        }
        case 0x801059a0: { // Ordinary item0/FP0/AP0 still records its action count.
            const auto u=unit(e),weapon=uint32_t(value(e,1));
            if(word(weapon+8) || byte(weapon+0x11) || byte(weapon+0x12)){if(!pending)pending=fn;return 0;}
            uint16_t fp=0;required(vm.memory.readHalf(u+0x112,fp));required(vm.memory.writeHalf(u+0x112,fp));
            if(word(u+8)==222){auto points=byte(Battle+0x16f4b),count=byte(Battle+0x16f4c);addActionPoint(points,0);addActionCount(count);byte(Battle+0x16f4b,points);byte(Battle+0x16f4c,count);}
            return 2;
        }
        case 0x801058e0:output(e,1,byte(uint32_t(value(e,0))+0x6e));return 2;
        case 0x80107b38:{const auto u=unit(e);const int32_t base=value(e,1);byte(Battle+0x1cbc,base);byte(Battle+0x1cbd,std::clamp(base-int32_t(byte(u+0x305))+int32_t(byte(u+0x306)),int32_t(0),int32_t(6)));return 2;}
        case 0x801077ec: {
            const auto u=unit(e);const int32_t id=value(e,1),arg=value(e,3);const auto options=uint32_t(raw(e,2));
            word(Battle+0x1c90,u);word(Battle+0x1c94,options);word(Battle+0x1c9c,0);word(Battle+0x1cb4,0);word(Battle+0x1cb8,1);
            for(unsigned o:{0x1ca0u,0x1ca8u,0x1ca4u,0x1cacu})word(Battle+o,0);
            word(Battle+0x1c98,arg);word(Battle+0x1cc0,0);
            for(uint32_t r=0x80364348;word(r);r+=20)if(word(r)==uint32_t(id)) {
                word(Battle+0x1ca0,word(r+4));if(!(word(u+0x27c)&0x10))word(Battle+0x1ca8,word(r+12));
                word(Battle+0x1ca4,word(r+8));word(Battle+0x1cac,word(r+16));break;
            }
            return 2;
        }
        case 0x8010e5a8:byte(unit(e)+0x189,value(e,1));return 2;
        case 0x8010e4bc:return battleResetUnitFacingEvent(e,nativeBattleUnitEventServices(vm),nativeBattleActorServices(vm));
        case 0x8010b85c:case 0x8010b7c4:{const auto u=unit(e);const int32_t amount=value(e,2),sign=int8_t(byte(u+0x189));const auto x=value(e,1);output(e,1,int32_t(uint32_t(x)+(fn==0x8010b85c?uint32_t(amount)*uint32_t(sign):0u-uint32_t(amount)*uint32_t(sign))));return 2;}
        case 0x8010e684: {
            const auto u=unit(e);const int32_t actorPart=value(e,1),target=value(e,2),targetPart=value(e,3),mode=raw(e,4);
            if(actorPart!=-1 || (mode!=1 && mode!=16)){if(!pending)pending=fn;return 0;}
            float x=float(target);if(mode==16){const int32_t id=translateBattleActor(&e,target,nativeBattleActorServices(vm));const auto t=word(Battle+0x20+4*id),part=packedUnitPart(t,targetPart,nativeBattleActorServices(vm).memory);if(word(part+0x1ac)&0x10000000){if(!pending)pending=fn;return 0;}x=real(t+0x3c)+real(part+0x24);}
            const float delta=x-real(u+0x3c);output(e,5,delta>0?1:delta<0?-1:int8_t(byte(u+0x189)));return 2;
        }
        case 0x80106b20: {
            const auto u=unit(e);const int32_t partId=value(e,1),type=value(e,2);const auto p=packedUnitPart(u,partId,nativeBattleActorServices(vm).memory);
            uint32_t table=word(p+0x1bc),name=0;if(!table){if(!pending)pending=fn;return 0;}
            const uint32_t fallback=type<=28?28:type<=39?39:69;
            if(type<70)for(unsigned i=0;i<70;++i){const auto key=word(table+8*i);if(key>=70)break;if(key==uint32_t(type) || key==fallback){name=word(table+8*i+4);break;}}
            if(!name){if(!pending)pending=fn;return 0;}changePose(u,p,text(name),type>=29 && type<=39);return 2;
        }
        case 0x80106bb8:{const auto u=unit(e);const auto p=packedUnitPart(u,value(e,1),nativeBattleActorServices(vm).memory);changePose(u,p,text(uint32_t(value(e,2))),false);return 2;}
        case 0x80105890:output(e,0,byte(selectedWeapon()+0x6e));return 2;
        case 0x80105928:{const auto u=unit(e),w=selectedWeapon();if(word(w+8) || byte(w+0x11) || byte(w+0x12)){if(!pending)pending=fn;return 0;}if(word(u+8)>=224 && word(u+8)<231){auto points=byte(Battle+0x16f4d),count=byte(Battle+0x16f4e);addActionPoint(points,0);addActionCount(count);byte(Battle+0x16f4d,points);byte(Battle+0x16f4e,count);}return 2;}
        case 0x8010aaa8:{const auto a=unit(e),d=unit(e,1),p=packedUnitPart(d,value(e,2),nativeBattleActorServices(vm).memory),w=selectedWeapon();const uint32_t flags=uint32_t(raw(e,3));for(unsigned o=0x118;o<=0x135;++o)if(byte(a+o) || byte(d+o)){if(!pending)pending=fn;return 0;}if(byte(w+0x10)!=100 || (word(p+0x1b0)&2) || word(Battle+0x182bc) || (word(d+0x104)&0x18) || (word(p+0x1ac)&(0x4000|0x40)) || byte(d+0x2e0) || byte(d+0x2e1) || byte(d+0x2e2)){if(!pending)pending=fn;return 0;}if(word(w+0x74)&4)word(a+0x27c,word(a+0x27c)|8);if(!(flags&0x100000) && !(word(w+0x74)&8))(void)random.bounded(100);output(e,4,1);return e.flags&1?2:255;}
        case 0x8010a688:{const auto a=unit(e),d=unit(e,1),p=packedUnitPart(d,value(e,2),nativeBattleActorServices(vm).memory),w=selectedWeapon();if((word(d+8)!=1 && word(d+8)!=2) || word(p+0x1b0) || w!=0x80393ea8){if(!pending)pending=fn;return 0;}word(d+0x258,p);const auto result=checkGroundDamage(a,d,p,w,uint32_t(raw(e,3)));if(result>=0)output(e,4,result);return result>=0 && (e.flags&1)?2:255;}
        case 0x8011355c:{const auto u=unit(e);for(unsigned i=0;i<3;++i)if(raw(e,i+1)!=-250000000)setEventFloat(e,raw(e,i+1),real(u+0x3c+4*i),vm.interpreterServices().operands);return 2;}
        case 0x80111894:{const auto u=unit(e);for(unsigned i=0;i<3;++i)if(raw(e,i+1)!=-250000000)output(e,i+1,int32_t(real(u+0x6c+4*i)));return 2;}
        case 0x8010e614:output(e,1,int8_t(byte(unit(e)+0x189)));return 2;
        case 0x8010b8f4:{const auto u=unit(e);const int32_t amount=value(e,2),sign=int8_t(byte(u+0x188))?int8_t(byte(u+0x188)):int8_t(byte(u+0x189));output(e,1,int32_t(uint32_t(value(e,1))+uint32_t(amount)*uint32_t(sign)));return 2;}
        case 0x801103dc:case 0x8011049c:{const auto id=translateBattleActor(&e,value(e,0),nativeBattleActorServices(vm));if(id==-1)return 2;const auto u=word(Battle+0x20+4*id);for(unsigned i=0;i<3;++i){const float v=getEventFloat(e,raw(e,i+1),vm.interpreterServices().operands);if(v!=-250000000.f)real(u+0x90+4*i,fn==0x801103dc?real(u+0x90+4*i)+v:v);}return 2;}
        case 0x80112978:{const auto u=unit(e);for(unsigned i=0;i<3;++i){const float v=float(value(e,i+1));if(v!=-250000000.f)real(u+0x54+4*i,v);}return 2;}
        case 0x93105b2c:case 0x93105a14:case 0x80189910:{std::array<float,4> p{};for(unsigned i=0;i<4;++i)p[i]=getEventFloat(e,raw(e,i),vm.interpreterServices().operands);const int32_t previous=value(e,4);const float dx=p[2]-p[0],dy=p[3]-p[1];output(e,4,dx==0 && dy==0?previous:int32_t(sourceAngles.angle(dx,dy,0,0)));return 2;}
        case 0x801072e4:return 2; // Retained original stylish-audience request; producer adapter remains open.
        case 0x8010a78c: {
            BattleCheckDamageEventServices services;services.units=nativeBattleUnitEventServices(vm);services.user=this;
            services.part=[](void* self,uint32_t u,int32_t id){auto& r=*static_cast<TrioAttackRuntime*>(self);return packedUnitPart(u,id,nativeBattleActorServices(r.vm).memory);};
            services.checkDamage=[](void* self,uint32_t a,uint32_t d,uint32_t p,uint32_t w,uint32_t f,int32_t& result){return static_cast<TrioAttackRuntime*>(self)->checkDamageCore(a,d,p,w,f,result);};
            services.eventExists=[](void* self,uint32_t id){auto& runtime=static_cast<TrioAttackRuntime*>(self)->vm;for(int32_t i=0;i<runtime.battle.capacity;++i)if(runtime.battle.entries[i].id==id && (runtime.battle.entries[i].flags&1))return true;return false;};
            return battleCheckDamageEvent(e,services);
        }
        case 0x801064d8:{const auto u=unit(e),p=packedUnitPart(u,value(e,1),nativeBattleActorServices(vm).memory);for(unsigned o=0x118;o<=0x135;++o)if(byte(u+o)){if(!pending)pending=fn;return 0;}if(word(u+0x104)&0x20000){if(!pending)pending=fn;return 0;}const auto table=word(p+0x1bc);uint32_t name=0;for(unsigned i=0;i<28 && word(table+8*i+4);++i)if(word(table+8*i)==28){name=word(table+8*i+4);break;}if(!name){if(!pending)pending=fn;return 0;}changePose(u,p,text(name),false);word(p+0x204,(word(p+0x204)&~uint32_t(2|0x10|0x80|4))|1);return 2;}
        case 0x801141a4:word(Battle+0x1c70,0);byte(Battle+0x1c74,0);return 2;
        case 0x8010dcb0:{const auto kind=int32_t(word(unit(e)+8));output(e,1,kind==222 || (kind>=224 && kind<231));return 2;}
        case 0x801140fc:{const auto u=unit(e);if(!u)return 2;for(unsigned o:{0x2a8u,0x2b4u})for(int i=0;i<vm.battle.capacity;++i)if(word(u+o) && (vm.battle.entries[i].flags&1) && vm.battle.entries[i].id==word(u+o))return 0;return 2;}
        case 0x80105e50:{const auto u=unit(e);const int32_t type=value(e,1),duration=value(e,2);(void)value(e,3);const bool on=value(e,4)!=0;if(type!=27){if(!pending)pending=fn;return 0;}byte(u+0x135,on?duration:0);return 2;}
        case 0x80111720:case 0x801115f8:{const auto u=unit(e);std::array<float,3> v{};for(unsigned i=0;i<3;++i)v[i]=float(value(e,i+1));for(unsigned i=0;i<3;++i){const float old=real(u+0x6c+4*i);real(u+0x6c+4*i,angle(fn==0x801115f8?old+v[i]:raw(e,i+1)==-250000000?old:v[i]));}return 2;}
        case 0x80110f9c:{const auto u=unit(e);for(unsigned i=0;i<3;++i){const float v=float(value(e,i+1));if(v!=-250000000.0f)real(u+0x78+4*i,v);}return 2;}
        case 0x8006d970:return 2; // Original effect request retained; particle provider pending.
        case 0x8010d914:{const auto u=unit(e);if(!u){output(e,1,0);return 2;}uint32_t k=word(u+0x10);int32_t amount=byte(k+0x12);for(unsigned i=0;i<byte(k+0x10);++i){const auto draw=random.bounded(100);k=word(u+0x10);if(draw<byte(k+0x11))++amount;}output(e,1,amount);return 2;}
        case 0x8010d8d8:{const auto w=word(Battle+0x2738);word(w+0x14,word(w+0x14)+uint32_t(value(e,0)));return 2;}
        case 0x8010d9bc:{const auto u=unit(e);if(!u){output(e,1,0);return 2;}const int32_t player=half(word(0x8041eb00)+0x8a),enemy=byte(u+0xd);int32_t exp=0;if(player<=enemy){const auto setup=word(word(Battle+0x2738)+0xc),group=word(setup+0x18);const auto type=int32_t(word(group));const float factor=real(type<=2?0x804226a8:type==3?0x804226b4:type==4?0x804226b8:0x804226bc);exp=byte(word(u+0x10)+0xf)+int32_t((factor*float(enemy-player))*real(0x804226c0));}output(e,1,exp);return 2;}
        case 0x8010d980:word(Battle+0xf00,word(Battle+0xf00)+uint32_t(value(e,0)));return 2;
        case 0x80113f88:{const auto u=unit(e),k=word(u+0x10);for(unsigned i=0;i<3;++i)output(e,i+1,int32_t(float(value(e,i+1))+real(k+0x90+4*i)));return 2;}
        case 0x8010da28:{const auto u=unit(e);if((word(u+0x1c)&0x40000000) && word(u+0x308)){const auto setup=word(word(Battle+0x2738)+0xc);unsigned i=0;while(i<8 && word(setup+0x7c+4*i))++i;word(setup+0x7c+4*i,word(u+0x308));word(u+0x308,0);}return 2;}
        case 0x80103e34:return battleStatusWindowOnOffEvent(e,statusWindowServices());
        case 0x80105df8:clearStatus(unit(e));return 2;
        case 0x80113304:{const auto u=unit(e);if(!u)return 2;half(u+0x10c,0);clearStatus(u);const uint32_t mode=uint32_t(raw(e,1));if(mode&2)word(u+0x104,word(u+0x104)|0x20000);else {byte(u+0x135,1);if(!(mode&1))word(u+0x1c,word(u+0x1c)|0x80000000);}return 2;}
        case 0x80106dfc: {
            if(first){const auto id=translateBattleActor(&e,value(e,0),nativeBattleActorServices(vm));word(Battle+0x271c,0);for(unsigned i=0;i<3;++i)word(Battle+0x2720+4*i,value(e,i+1));word(Battle+0x272c,id);word(Battle+0x2730,0);word(Battle+0x2734,value(e,4));}
            const auto id=int32_t(word(Battle+0x272c));const uint32_t a=id>=0 && id<64?word(Battle+0x20+4*id):0;const int32_t t=int32_t(word(Battle+0x271c)+1);word(Battle+0x271c,t);
            int32_t result=0;const int32_t lo=int32_t(word(Battle+0x2720)),hi=int32_t(word(Battle+0x2724)),end=int32_t(word(Battle+0x2728));
            if(!a || (word(a+0x27c)&0x10))result=1;
            else {const int32_t scan=int32_t(word(Battle+0x2734));if(t<=1 && scan>0)for(int32_t i=0;i<scan;++i)if(history.entries[i]&0x100){result=-1;break;}
                if(!result && ((history.entries[0]&0x100) || (byte(a+0x307) && lo<t && t<=hi)))result=t<=lo?-1:t<=hi?2:-2;
                if(!result && t>=end)result=1;
            }
            if(result){word(Battle+0x2730,result);return 1;}return 0;
        }
        case 0x80106d7c:{const auto result=int32_t(word(Battle+0x2730)),count=int32_t(word(Battle+0x271c));if(raw(e,0)!=-250000000)output(e,0,result);if(raw(e,1)!=-250000000)output(e,1,count);return 2;}
        case 0x80139898:return 2; // Original no-stylish audience request retained.
        case 0x801075c4:{const int32_t actor=value(e,0),input=value(e,1);int32_t result=std::clamp(input+int32_t(byte(Battle+0x1cbd))-1,int32_t(0),int32_t(6));if(actor!=-1 && actor!=-5 && (word(unit(e)+0x278)&255)==30)result=-1;output(e,2,result);return 2;}
        case 0x80106fa0:return 2; // Original success-effect request retained in trace.
        case 0x8010f2e8:return battleCheckHitCodeEvent(e,nativeBattleUnitEventServices(vm));
        case 0x8010f0e8:return battleCheckHitPatternEvent(e,nativeBattleUnitEventServices(vm));
        case 0x8010f374:return battleGetHitCodeEvent(e,nativeBattleUnitEventServices(vm));
        case 0x8010f458:return battleGetHitDamageEvent(e,nativeBattleUnitEventServices(vm),nativeBattleActorServices(vm));
        case 0x8010ed8c:output(e,1,int32_t(word(unit(e)+8)));return 2;
        case 0x80105b40:return battleUnitFlagEvent(e,true,nativeBattleUnitEventServices(vm));
        case 0x80105ae0:return battleUnitFlagEvent(e,false,nativeBattleUnitEventServices(vm));
        case 0x8010e430:return battleCheckUnitAttributeEvent(e,nativeBattleUnitEventServices(vm));
        case 0x8011389c: {
            return eventLive(uint32_t(value(e,0)))?0:2;
        }
        case 0x8010a154: {
            const auto a=word(Battle+0x20+4*word(Battle+0x420));if(!a || !word(a+0x2a8))return 2;
            const auto id=word(a+0x2a8);for(int i=0;i<vm.battle.capacity;++i)if((vm.battle.entries[i].flags&1) && vm.battle.entries[i].id==id)return 0;
            word(a+0x2a8,0);return 2;
        }
        case 0x80107758:if(word(Battle+0x1cac)==0x8012a7c0)word(Battle+0x1c9c,1000);else if(word(Battle+0x1cac)==0x80129fc0)word(Battle+0x1c9c,1005);return 2;
        case 0x8010bf8c:return divePosition(e,first);
        case 0x8010d208:return genericJump(e,first);
        // Feedback/effect providers retain original requests; source event waits
        // and unit/reaction state remain executing, rather than flattened timers.
        case 0x8010749c:case 0x80139af4:case 0x8010436c:case 0x801054e8:return 2;
        case 0x80113950:case 0x801138d8:output(e,1,0);return 2; // Fresh unequipped drain badges.
        case 0x8010abac: { // Bounded fresh trio precheck. Changed status/badges/fog await generic binding.
            const auto a=unit(e),d=unit(e,1),p=packedUnitPart(d,value(e,2),nativeBattleActorServices(vm).memory),w=uint32_t(value(e,3));const uint32_t flags=value(e,4);
            for(unsigned o=0x118;o<=0x135;++o)if(byte(a+o) || byte(d+o)){if(!pending)pending=fn;return 0;}
            if(byte(d+0x2e0) || byte(d+0x2e1) || byte(d+0x2e2) || word(Battle+0x182bc) || (word(d+0x104)&0x18) || (word(p+0x1ac)&(0x4000|0x40)) || (word(p+0x1b0)&2) || byte(w+0x10)!=100){if(!pending)pending=fn;return 0;}
            if(word(w+0x74)&4)word(a+0x27c,word(a+0x27c)|8);
            if(!(flags&0x100000) && !(word(w+0x74)&8))(void)random.bounded(100);
            output(e,5,1);return 2;
        }
        case 0x80106f40:{const bool on=value(e,0)!=0;const auto mask=uint32_t(raw(e,1));word(Battle+0x1f50,on?word(Battle+0x1f50)|mask:word(Battle+0x1f50)&~mask);return 2;}
        case 0x80106ef0:output(e,0,timing().successWindow(byte(Battle+0x1cbd)));return 2;
        case 0x80107708:{if(word(Battle+0x1ca4)==0x8012a174)return word(Battle+0x1cb8)&1?0:2;auto c=timing();TimingAContext context;context.difficulty=byte(Battle+0x1cbd);context.setupOptions=word(Battle+0x1c94);context.actorFlags=word(word(Battle+0x1c90)+0x27c);context.battleFlags=word(Battle+0xef4);c.evaluate(history,context,random);storeTiming(c);return c.result&1?0:2;}
        case 0x80107660:output(e,0,int32_t(word(Battle+0x1cb8)));return 2;
        case 0x80107548:output(e,0,int32_t(word(Battle+0x1cb4)));return 2;
        case 0x80107788: {
            int32_t state=int32_t(word(Battle+0x1c9c));const uint32_t actorFlags=word(word(Battle+0x1c90)+0x27c);
            ActionCommandView view;view.state=&state;view.actorFlags=&actorFlags;view.help=[](void*,int32_t){};
            ActionValueServices values;values.user=&e;values.resolve=[](void*,int32_t v){return v;};
            startActionCommand(view,value(e,0),values);word(Battle+0x1c9c,state);return 2;
        }
        // Requests mutate the actual packed camera owner; Main remains a
        // separate frame service rather than an event-side smoothing step.
        case 0x8012b3d0:case 0x8012bfd0:case 0x8012b8cc:case 0x8012ba14:case 0x8012be38:case 0x8012bc70:case 0x8012b108:case 0x8012b670:case 0x8012b994:case 0x8012b254:{
            BattleCameraRequestServices s;s.positions=nativeBattlePartPositionServices(vm);s.values=s.positions.unit.values;s.user=this;
            s.signedWordFloat=[](void* self,int32_t v){auto& r=*static_cast<TrioAttackRuntime*>(self);return r.vm.platform.signedWordFloat(r.vm.platform.user,v);};
            s.rumbleOn=[](void* self,int32_t channel){static_cast<TrioAttackRuntime*>(self)->rumbleRequests.push_back(channel);};
            if(fn==0x8012bfd0)return battleCameraModeEvent(e,s);
            if(fn==0x8012b8cc)return battleCameraSpeedEvent(e,s);
            if(fn==0x8012ba14)return battleCameraZoomEvent(e,s);
            if(fn==0x8012be38)return battleCameraHomingEvent(e,s);
            if(fn==0x8012bc70)return battleCameraHomingPartsEvent(e,s);
            if(fn==0x8012b108)return battleCameraOffsetEvent(e,s);
            if(fn==0x8012b670)return battleCameraMoveToEvent(e,cameraFrame,s);
            if(fn==0x8012b994)return battleCameraAddZoomEvent(e,s);
            if(fn==0x8012b254)return battleCameraAxisEvent(e,8,s);
            return battleCameraShakeHorizontalEvent(e,s);
        }
        case 0x80104700:case 0x80104508:return 2; // Retained original camera/SE/voice requests; presentation providers remain open.
        default:break;
        }
        // Deliberately no success fallback. A pending owner remains suspended
        // until its callback is implemented; it cannot spend a turn or win.
        if(!pending)pending=fn;
        return 0;
    }
    float angle(float v) const {float result=float(std::fmod(double(v),360.0));if(std::isnan(result))result=0;if(result<0)result+=360;if(result>=360)result=0;return result;}
    void clearStatus(uint32_t u) {
        // Packed status export is completed from the recovered clear contract.
        BattleStatusUnit status;clearUnitStatus(status);for(unsigned i=0;i<30;++i)byte(u+0x118+i,status.statusBytes[i]);
        for(unsigned i=0;i<6;++i){const auto at=u+0xae8+12*i;byte(at,0xff);for(unsigned j=1;j<4;++j)byte(at+j,0);word(at+4,0);} // Retain each word8.
    }
    bool initialize(const TrioBattleCommands& menu,ProfessorStory& s,int16_t hp,std::shared_ptr<ProloguePouch> supplied) {
        scene=&s;now=s.now;
        const bool suppliedEntry=bool(supplied);
        entryPouch=supplied?std::move(supplied):createProloguePouch();
        if(!entryPouch)return false;
        // Legacy development checkpoints supply only current Mario HP. Their
        // remaining original fields use the explicit recovered initial-stage
        // snapshot, not a claim to deserialize an original saved inventory.
        if(!suppliedEntry)entryPouch->half(0x70,hp);
        for(auto span:std::array<std::pair<uint32_t,unsigned>,15>{{{0x85600000,0x64040},{0x8041e808,16},{0x82000000,0x100000},{0x8041f070,16},{0x8041e428,4},{0x8041e844,4},{0x8041eb90,4},{0x8041eb00,4},{0x8041ebdc,12},{0x804295d4,4},{Battle,0x19100},{Units,0x10000},{Parts,0x10000},{Kinds,0x1000},{Work,0x14000}}})
            if(!vm.memory.registerRam(span.first,std::vector<uint8_t>(span.second)))return false;
        std::vector<uint8_t> packedPouch(0x600);std::copy(entryPouch->bytes.begin(),entryPouch->bytes.end(),packedPouch.begin());
        if(!vm.memory.registerRam(0x85000000,std::move(packedPouch)) || !vm.memory.importScripts(scripts))return false;
        if(word(0x804175a0)!=0x8041ebdc)return false;
        for(unsigned i=0;i<3;++i)word(0x8041ebdc+4*i,spotlight->work[i]);
        word(0x804295d4,spotlight->entryColor);
        auto m=vm.memoryBinding.services();const auto start=eventSdkInitializeAllocator(0x82000000,0x82100000,1,m);
        if(eventSdkCreateHeap(start,0x82100000,m)!=0)return false;
        // Declared E-only primary reservation: room for the original400KiB
        // battle subpool and mandatory32-byte map headers. Other native work
        // remains separately mapped; this does not reproduce prior allocations.
        word(0x8041e808,0x85600000);word(0x85600000,0);word(0x85600004,0x64020);half(0x85600008,0);
        allocateEventBattleMapPool(m);
        word(0x8041e844,0);vm.gp.refreshRate=60;vm.gp.timestamp=now*eventClockRate(243000000);vm.initializeContexts();vm.gp.battleContext=1;
        half(Battle+8,2);half(Battle+0x10,1);
        initializePackedBattleStageWorkStorage(Battle,nativeBattleActorServices(vm).memory);
        for(unsigned i=0;i<64;++i)word(Battle+0x16f5c+8*i,0xffffffff);
        for(unsigned i=0;i<28;++i)byte(Battle+0x18da0+12*i,0xff);
        half(Battle,0);word(Battle+0xf14,0x0300004c);word(Battle+0xf18,0x04000002);word(Battle+0xf1c,0x05000009);word(Battle+0xf20,0x06000007);
        word(Battle+0x2738,Work);word(Work+0xc,Work+0x100);word(Work+0x118,reactionGroup);
        word(0x8041eb90,Battle);word(0x8041eb00,0x85000000);byte(Battle+0xa,1);byte(Battle+0x12,255);
        BattleCameraRequestServices cameraInit;cameraInit.positions=nativeBattlePartPositionServices(vm);initializeBattleCamera(Battle,cameraInit);
        word(Battle+0xef8,word(Battle+0xef8)|2u); // Original btlDispInit publication; other Init services stay separate.
        if(!initializePackedSetup4AudienceParameters(audienceSetupConfig,Work+0x100,audienceInitServices()))return false;
        word(Battle+0x420,menu.actor);word(Battle+0x424,6);word(Battle+0x1c5c,0);word(Battle+0x171c+0x4b0,0);word(Battle+0x171c+0x80,menu.attack->weapon);
        for(unsigned i=0;i<7;++i){const auto snapshot=Battle+0x162a0+44*i;partySnapshots[i].value=real(0x80422174);real(snapshot,partySnapshots[i].value);word(snapshot+0x24,0);for(unsigned j=0;j<30;++j)byte(snapshot+4+j,0);half(snapshot+0x28,0);half(snapshot+0x2a,0);}
        for(unsigned i=0;i<menu.candidates.bytes.size();++i)byte(Battle+0x428+i,menu.candidates.bytes[i]);
        for(unsigned id=1;id<7;++id) {
            const uint32_t u=Units+id*0x1000,p=Parts+id*0x1000,k=Kinds+id*0x200;
            word(Battle+0x20+id*4,u);word(u,id);word(u+8,menu.units[id].kind);byte(u+0xc,menu.units[id].belong);
            word(u+0x140,0xffffffff);byte(u+0x21,id==6?0:1);byte(u+0x22,id==6?0:1);byte(u+0x23,id==6?0:1);word(u+0x2c,id<3?0x04000002:0x04000004);word(u+0x10,k);word(u+0x14,p);byte(k+0xb4,1);word(p+4,k+0x100);word(k+0x100,1);word(p+0x4ec,u);
            byte(u+0x189,s.actors[visual[id]].yaw>0 && s.actors[visual[id]].yaw<180?1:255);byte(p+0xbd,id<3?1:255);real(u+0x114,1);for(unsigned o=0x90;o<=0xa4;o+=4)real(u+o,1);
            for(unsigned axis=0;axis<3;++axis){real(u+0x3c+4*axis,s.actors[visual[id]].position[axis]);real(u+0x30+4*axis,s.actors[visual[id]].position[axis]);}
            if(id>=3) {
                const auto& spec=trioBattleUnitSpecs()[id-3];
                if(id==6){
                    const auto kind=word(word(reactionGroup+4)+3*0x30),definition=word(kind+0xb8);
                    word(u+0x10,kind);word(u+0x18,word(kind+0xc0));word(p+4,definition);
                    for(unsigned offset:{0x40u,0x44u,0x38u,0x3cu,0x48u})word(p+(offset==0x40?0x1ac:offset==0x44?0x1b0:offset==0x38?0x1b4:offset==0x3c?0x1b8:0x1bc),word(definition+offset));
                    for(unsigned offset:{0x108u,0x10au,0x10cu})half(u+offset,half(kind+8));
                    for(unsigned offset:{0x10eu,0x110u,0x112u})half(u+offset,half(kind+0xa));
                }
                if(id<=5){const auto kind=reactionUnits[id-3][0],definition=word(kind+0xb8);word(u+0x10,kind);word(u+0x18,word(kind+0xc0));word(u+0x2b0,reactionUnits[id-3][1]);word(u+0x28c,reactionUnits[id-3][2]);word(u+0x2a0,reactionUnits[id-3][3]);word(p+4,definition);word(p+0x1bc,word(definition+0x48));word(p+0x1b4,word(definition+0x38));word(p+0x1b8,word(definition+0x3c));}
                byte(u+0xd,byte(word(u+0x10)+0xe));half(u+0x108,spec.hp);
                word(u+0x104,spec.attributes);word(p+0x1ac,spec.partAttributes);word(p+0x1b0,spec.counterAttributes);
                required(vm.memory.writeHalf(u+0x10c,spec.hp));required(vm.memory.writeHalf(p+0x1a0,spec.forwardBias));
                for(unsigned axis=0;axis<3;++axis){real(p+0x24+axis*4,spec.partPosition[axis]);real(p+0x170+axis*4,spec.hitOffset[axis]);real(p+0x188+axis*4,spec.samplingOffset[axis]);}
            } else {
                word(p+0x1ac,1);
                if(id<3) {
                    const uint32_t kind=id==1?0x80372afc:0x80393bec;
                    word(u+0x28c,id==1?0x80417c38:0x80418480);word(u+0x2b0,id==1?0x80417c08:0x80393e68);
                    word(u+0x10,kind);byte(u+0xd,uint8_t(half(0x85000000+0x8a)));word(u+0x104,word(kind+0xac));word(u+0x18,word(kind+0xc0));
                    const uint32_t source=word(kind+0xb8);const unsigned count=byte(kind+0xb4);
                    // Mario has three authored parts. Keep primary addresses
                    // stable and place his extra parts in the unused unit0
                    // partition; a third 0x500 record in unit1 overlaps unit2.
                    const auto partAddress=[&](unsigned j){return id==1 && j?Parts+0x500*(j-1):p+0x500*j;};
                    for(unsigned j=0;j<count;++j) {
                        const uint32_t record=source+0x4c*j,part=partAddress(j);word(part,j+1<count?partAddress(j+1):0);word(part+4,record);word(part+0x4ec,u);byte(part+0xbd,1);
                        for(unsigned axis=0;axis<3;++axis){real(part+0x24+axis*4,real(record+0xc+axis*4));real(part+0x170+axis*4,real(record+0x18+axis*4));real(part+0x188+axis*4,real(record+0x24+axis*4));}
                        for(unsigned offset:{0x40u,0x44u,0x38u,0x3cu,0x48u})word(part+(offset==0x40?0x1ac:offset==0x44?0x1b0:offset==0x38?0x1b4:offset==0x3c?0x1b8:0x1bc),word(record+offset));
                    }
                    for(unsigned offset:{0x108u,0x10au,0x10cu})half(u+offset,half(kind+8));
                    for(unsigned offset:{0x10eu,0x110u,0x112u})half(u+offset,half(kind+0xa));
                }
            }
        }
        word(Units+6*0x1000+0x298,franklyPhase);
        for(unsigned id=1;id<7;++id){const auto unit=Units+id*0x1000;half(unit+0xcc,half(word(unit+0x10)+0x16));half(unit+0xce,half(word(unit+0x10)+0x18));initializePackedBattleHpGauge(unit,nativeBattleActorServices(vm));byte(unit+0x311,255);byte(unit+0x312,255);for(auto part=word(unit+0x14);part;part=word(part)){half(part+0x1a2,half(word(part+4)+0x30));half(part+0x1a4,half(word(part+4)+0x32));real(part+0x78,1);for(unsigned axis=0;axis<3;++axis){real(part+0x6c+4*axis,1);real(part+0x60+4*axis,1);real(part+0x3c+4*axis,0);real(part+0x48+4*axis,0);real(part+0x54+4*axis,0);}real(part+0x208,1);real(part+0x20c,1);word(part+0x4f0,0xffffffff);word(part+0x1c0,0xffffffff);byte(part+0x1c4,0);byte(part+0x214,0);byte(part+0x215,0);}}
        // Real player and surviving lecture-Frankly mode1 pose owners.
        // Enemy factories remain explicit providers when they survive display.
        for(const unsigned id:{1u,2u,6u})for(auto part=word(Units+id*0x1000+0x14);part;part=word(part)){
            const auto table=word(part+0x1bc),modelName=word(word(part+4)+8);
            if(!table || !modelName){partPoseHandles.emplace(part,-1);continue;}
            const auto name=packedBattlePoseName(part,28,nativeBattleActorServices(vm).memory);
            if(!name)return false;
            const auto initialName=text(name);
            for(unsigned i=0;i<=initialName.size();++i)byte(part+0x1c4+i,i==initialName.size()?0:uint8_t(initialName[i]));
            const auto& resource=id==6?franklyBattleResource:playerBattleResources[id-1];if(text(modelName)!=resource->modelName)return false;
            const auto handle=poses.enter(resource,1,{now,now});if(!handle)return false;
            word(part+0x1c0,uint32_t(*handle));partPoseHandles.emplace(part,*handle);
        }
        for(unsigned id=1;id<7;++id)clearStatus(Units+id*0x1000);
        UnitResourceServices resources;resources.user=entryPouch.get();resources.pouch=[](void* p){return static_cast<ProloguePouch*>(p)->bytes.data();};resources.snapshots=&partySnapshots;
        for(unsigned id=1;id<=2;++id){const auto u=Units+id*0x1000;readDamage(id);auto& d=damageUnits[id];d.baseHp=half(u+0x10a);d.baseFp=half(u+0x110);d.hpPlusCount=byte(u+0x2f1);d.fpPlusCount=byte(u+0x2f2);importUnitResources(d,resources);
            half(u+0x10a,d.baseHp);half(u+0x108,d.maxHp);half(u+0x10c,d.hp);half(u+0x110,d.baseFp);half(u+0x10e,d.maxFp);half(u+0x112,d.fp);
            if(id==2){real(u+0x114,d.snapshotValue);word(u+0x138,d.flags);half(u+0x13c,d.snapshot13c);half(u+0x13e,d.snapshot13e);for(unsigned j=0;j<30;++j)byte(u+0x118+j,d.statusBytes[j]);}}
        BattleTurnFirstBackend firstTurn;firstTurn.runtime=&vm;battleTurnFirstProcess(Battle,nativeBattleTurnFirstServices(firstTurn));
        buildBattlePhaseTables(Battle,phaseServices());
        child=nativeBattleDataChildServices(vm);takeoffBackend.runtime=&vm;takeoff=nativeBattleTakeoffServices(vm,nativeBattleTakeoffCoreServices(takeoffBackend));BattleSoundPlaybackServices playback;playback.user=this;playback.trigger=[](void* p,uint32_t,uint32_t key,int32_t,int32_t,const std::array<float,3>&){auto& r=*static_cast<TrioAttackRuntime*>(p);r.soundRequests.push_back(r.text(key));return uint32_t(-1); // Explicit native request failure; no public slot allocated.
};playback.position=[](void*,uint32_t,const std::array<float,3>&){};playback.off=[](void*,uint32_t){};movement=nativeBattleMovePositionServices(vm,playback);
        bool ok=bindBattleJumpSettingCallback(registry,vm) && bindBattleFlagQueryCallback(registry,vm) && bindBattleSelectedTargetCallback(registry,vm) && bindBattleCommandWeaponCallback(registry,vm) &&
            bindBattleWaitGuardMoveCallback(registry,vm) && bindBattleDataChildCallback(registry,child) && bindBattleMajinaiQueryCallback(registry,vm) && bindBattleGetPosCallback(registry,vm) &&
            bindBattleGetHomePosCallback(registry,vm) && bindBattleSetPosCallback(registry,vm) && bindBattleSetMoveSpeedCallback(registry,vm) && bindBattleTakeoffCallback(registry,takeoff) &&
            bindBattleMovePositionCallback(registry,movement) && bindBattleGetMoveFrameCallback(registry,vm) && bindBattleGetHitPosCallback(registry,vm) && bindBattleMarioJumpParameterCallback(registry,vm) && bindBattleStartWaitCallback(registry,vm) && bindBattleCheckStatusCallback(registry,vm) && bindBattleCheckDataCallback(registry,vm) && bindBattleSetFallAccelCallback(registry,vm) && bindBattleFramesToMillisecondsCallback(registry,vm);
        jumpBackend.runtime=&vm;jumpBackend.user=this;jumpBackend.play=[](void*,uint32_t,const std::array<float,3>&)->uint32_t{return 0;};
        jump=nativeBattleMarioJumpServices(jumpBackend);ok=ok && bindBattleMarioJumpPositionCallback(registry,jump);
        if(!ok)return false;
        if(!initializeStageObjects())return false;
        if(!initializePackedBattleSpotlight(spotlightServices()))return false;
        spotlightInitialized=true;publishSpotlight();
        endingRequest(0x801a83f8);if(!initializePackedBattleAudience(audienceInitServices()))return false;
        endingRequest(0x8020507c);if(!initializePackedBattleBreakSlot(slotServices()))return false;
        // Project original Init state5 window requests at harness entry.
        // Complete startup sequencing/resource readiness remains separate.
        packedBattleStatusWindowSet(0x2748,true,statusWindowServices());
        packedBattleStatusWindowSet(0x2744,true,statusWindowServices());
        return launch(menu);

    }
    void readDamage(unsigned id) {
        auto& d=damageUnits[id];const auto u=Units+id*0x1000,p=Parts+id*0x1000;
        d.kind=int32_t(word(u+8));d.belong=int8_t(byte(u+0xc));d.hp=half(u+0x10c);d.maxHp=half(u+0x108);d.fp=half(u+0x112);d.maxFp=half(u+0x10e);
        d.flags=word(u+0x27c);d.statusFlags=word(u+0x138);d.attributes=word(u+0x104);d.damageSequence=half(u+0x260);d.attackSequence=int32_t(word(u+0x280));
        d.jumpPowerBonus=byte(u+0x2ff);d.hammerPowerBonus=byte(u+0x300);
        for(unsigned i=0;i<30;++i)d.statusBytes[i]=byte(u+0x118+i);
        d.hpBefore=int32_t(word(u+0x208));d.hpAfter=int32_t(word(u+0x20c));
        d.damageFlags=half(u+0x1fe);d.field200=half(u+0x200);d.field202=half(u+0x202);d.field204=half(u+0x204);d.field206=half(u+0x206);
        damageParts[id].attributes=word(p+0x1ac);
        if(id>=3 && id<=5){defenses[id]=trioBattleUnitSpecs()[id-3].defense;for(unsigned i=0;i<5;++i)defenseAttributes[id][i]=int8_t(trioBattleUnitSpecs()[id-3].defenseAttributes[i]);}
        damageParts[id].defense=defenses[id].data();damageParts[id].defenseAttributes=defenseAttributes[id].data();
    }
    void writeDamage(unsigned id) {
        const auto& d=damageUnits[id];const auto u=Units+id*0x1000;
        half(u+0x10c,d.hp);word(u+0x27c,d.flags);word(u+0x208,d.hpBefore);word(u+0x20c,d.hpAfter);word(u+0x264,d.requestedHp);word(u+0x268,d.requestedFp);word(u+0x26c,d.consumedFp);
        half(u+0x200,d.field200);half(u+0x204,d.field204);
        byte(u+0x270,d.lastHp);byte(u+0x271,d.lastFp);byte(u+0x272,d.lastConsumedFp);word(u+0x274,d.pattern);half(u+0x1fe,d.damageFlags);
    }
    DamageApplicationServices application() {
        DamageApplicationServices s;s.user=this;
        s.getUnit=[](void* p,int32_t id)->DamageUnit* {return id>=1 && id<7?&static_cast<TrioAttackRuntime*>(p)->damageUnits[id]:nullptr;};
        s.bodyPart=[](void* p,DamageUnit& d)->DamagePart* {auto& r=*static_cast<TrioAttackRuntime*>(p);return &r.damageParts[&d-r.damageUnits.data()];};
        s.hitPosition=[](void* p,DamageUnit& d,DamagePart&) {auto& r=*static_cast<TrioAttackRuntime*>(p);const unsigned id=&d-r.damageUnits.data();auto xyz=packedHitPosition(Units+id*0x1000,Parts+id*0x1000,nativeBattlePartPositionServices(r.vm));return DamagePosition{xyz[0],xyz[1],xyz[2]};};
        s.effect=[](void* p,const DamageEffect& effect){static_cast<TrioAttackRuntime*>(p)->damageEffects.push_back(effect);};
        s.sound=[](void* p,const char* key,DamagePosition){if(key)static_cast<TrioAttackRuntime*>(p)->soundRequests.emplace_back(key);};
        s.notice=[](void* p,DamageUnit&,DamageNotice notice){static_cast<TrioAttackRuntime*>(p)->notices.push_back(notice);};
        s.hitEvent=[](void* p,DamageUnit& d,uint32_t code){auto& r=*static_cast<TrioAttackRuntime*>(p);const unsigned id=&d-r.damageUnits.data();r.writeDamage(id);r.hitEvent(Units+id*0x1000,code);};
        s.inventory.itemCount=[](void*,int32_t)->int32_t{return 0;};return s;
    }
    void hitEvent(uint32_t u,uint32_t code) {
        const auto script=word(u+0x2b0);if(!script)return;
        for(unsigned offset:{0x2b4u,0x290u,0x29cu,0x2a8u}){const auto id=word(u+offset);if(id)deleteEventId(id,vm.creationServices());word(u+offset,0);}
        auto& e=createRootEvent(vm.schedule,script,10,0x20,vm.creationServices());e.owner=int32_t(word(u));word(u+0x278,code);word(u+0x2b4,e.id);
    }
    bool checkDamageCore(uint32_t a,uint32_t d,uint32_t p,uint32_t w,uint32_t flags,int32_t& result) {
        const bool playerHit=(word(d+8)==1 && !word(p+0x1b0) && w==0x803538a8) || (word(a+8)==222 && (word(d+8)==1 || word(d+8)==3) && word(p+0x1b0)<=1 && w==0x80353de8);
        const bool enemyHit=(word(a+8)>=1 && word(a+8)<=3) && (word(d+8)==222 || word(d+8)==224) && !word(p+0x1b0) && word(w+0x1c)==0x80189d30 && !word(w+0x40);
        if(!playerHit && !enemyHit){if(!pending)pending=0x8010a78c;return false;}
        word(d+0x258,p);
        if(enemyHit && !(word(w+0x74)&0x20000) && packedACPossibility(d,nativeBattleActorServices(vm)) && int32_t(word(Battle+0x1cb0))==5){
            const auto ai=word(a),di=word(d);readDamage(ai);readDamage(di);word(a+0x280,word(a+0x280)+1);byte(d+0x273,word(a+0x280));
            applyDirectDamage(ai,damageUnits[di],&damageParts[di],0,0,flags|0x80000|0x100|42,byte(w+0x6d),0,random,application());
            applyDirectDamage(-5,damageUnits[ai],nullptr,1,0,311,0,1,random,application());result=-1;return true; // Original wrapper handles the negative result.
        }
        result=checkGroundDamage(a,d,p,w,flags);return true;
    }
    int32_t checkGroundDamage(uint32_t a,uint32_t d,uint32_t p,uint32_t w,uint32_t flags) {
        if((word(w+0x74)&0x4000) && (word(p+0x1ac)&0x800) && (flags&0x100))flags|=0x400000;
        const auto ai=word(a),di=word(d);readDamage(ai);readDamage(di);auto& attacker=damageUnits[ai];auto& defender=damageUnits[di];auto& part=damageParts[di];
        DamageWeapon weapon=w==0x80353de8?ordinaryHammerWeapon():ordinaryJumpWeapon();weapon.specialFlags=word(w+0x74);weapon.element=byte(w+0x6c);if(word(w+0x1c)==0x80189d30){weapon.hpPower=defaultWeaponPower;weapon.defaultPower=int32_t(word(w+0x20));if(!(weapon.specialFlags&0x20000) && packedACPossibility(d,nativeBattleActorServices(vm)) && int32_t(word(Battle+0x1cb0))>3)flags|=0x40000;}if(w==0x80393ea8){weapon.specialFlags=word(w+0x74);weapon.hpPower=[](DamageUnit*,DamageWeapon& weapon,DamageUnit&,DamagePart&){auto& r=*static_cast<TrioAttackRuntime*>(weapon.callbackData);const int32_t level=r.half(0x85000000+14+0xa),index=std::min<int32_t>(2*level+int32_t((r.word(Battle+0x1cb8)>>1)&1),5);return int32_t(r.word(0x80393ea8+0x20+4*index));};}for(unsigned i=0;i<6;++i)weapon.powerWords[i]=int32_t(word(w+0x20+4*i));
        struct PowerContext{TrioAttackRuntime* runtime;uint32_t weapon;};PowerContext context{this,w};
        WeaponPowerServices power;power.user=&context;power.actionResult=[](void* p){return static_cast<PowerContext*>(p)->runtime->word(Battle+0x1cb8);};power.pouchLevel=[](void* p,MarioPowerKind kind)->uint8_t{auto& r=*static_cast<PowerContext*>(p)->runtime;return r.byte(r.word(0x8041eb00)+(kind==MarioPowerKind::Jump?0x98:0x99));};
        power.bonus=[](void*,const DamageUnit& u,MarioPowerKind kind){return kind==MarioPowerKind::Jump?u.jumpPowerBonus:u.hammerPowerBonus;};
        power.powerWord=[](void* p,int32_t index)->int32_t {auto& context=*static_cast<PowerContext*>(p);return int32_t(context.runtime->word(context.weapon+0x20+4*index));};weapon.callbackData=w==0x80393ea8?static_cast<void*>(this):static_cast<void*>(&power);
        DamageBattleState state;state.actionResult=word(Battle+0x1cb8);uint32_t code=0;const auto hp=calculateHpDamage(&attacker,defender,part,weapon,code,flags,state,storedDamageStatusServices());const auto fp=calculateFpDamage(&attacker,defender,part,weapon,code,flags,state);
        defender.damageSequence=int16_t(uint16_t(defender.damageSequence)+1);half(d+0x260,defender.damageSequence);
        applyDirectDamage(ai,defender,&part,hp,fp,code,byte(w+0x6d),1,random,application());
        word(a+0x27c,attacker.flags);word(a+0x280,word(a+0x280)+1);byte(d+0x273,word(a+0x280));return hp || fp?18:19;
    }
    int32_t divePosition(EventEntry& e,bool first) {
        const auto u=unit(e);std::array<int32_t,8> args{};for(unsigned i=0;i<8;++i)args[i]=value(e,i+1);
        const auto curve=args[5];if(args[6]!=0 || (curve!=0 && curve!=1 && curve!=4)){if(!pending)pending=0x8010bf8c;return 0;}
        auto distance=[&](const std::array<float,3>& a,const std::array<float,3>& b){const float x=b[0]-a[0],y=b[1]-a[1],z=b[2]-a[2];return std::sqrt(std::fma(z,z,std::fma(x,x,y*y)));};
        auto vector=[&](unsigned o){return std::array<float,3>{real(u+o),real(u+o+4),real(u+o+8)};};
        if(first){e.initialized=1;word(u+0x16c,args[3]);const auto start=vector(0x3c);for(unsigned axis=0;axis<3;++axis)real(u+0x148+4*axis,start[axis]);setPackedMovePosition(u,false,start,nativeBattleActorServices(vm).memory);setPackedMovePosition(u,true,{float(args[0]),float(args[1]),float(args[2])},nativeBattleActorServices(vm).memory);
            const auto target=vector(0x160);real(u+0x180,distance(start,target));if(!word(u+0x16c))word(u+0x16c,uint32_t(int32_t(real(u+0x180)/real(u+0x170))));else real(u+0x170,real(u+0x180)/float(int32_t(word(u+0x16c))));if(!word(u+0x16c))return 2;
            real(u+0x184,0);real(u+0x17c,0);if(target[0]!=start[0])byte(u+0x188,target[0]>start[0]?1:255);loadPackedUnitSoundMode(3,args[7],u+0x1c4,u+0x18c,nativeBattleActorServices(vm).memory);
        }
        const int32_t elapsed=int32_t(real(u+0x184)),count=int32_t(word(u+0x16c));for(unsigned axis=0;axis<3;++axis)real(u+0x154+4*axis,battleDiveInterpolate(curve,elapsed,count,real(u+0x148+4*axis),real(u+0x160+4*axis)));
        real(u+0x184,real(u+0x184)+1);const auto target=vector(0x160);if(real(u+0x184)>float(count)){for(unsigned axis=0;axis<3;++axis)real(u+0x3c+4*axis,target[axis]);endPackedUnitMovementSound(u,movement.sound);return 2;}
        const auto current=vector(0x154);float remaining=distance(current,target);if(remaining==0)remaining=1;if(real(u+0x180)==0)real(u+0x180,1);const float degrees=180.f*(1.f-remaining/real(u+0x180));const float sine=float(std::sin(double(degrees)*3.141592653589793/180.));const float amplitude=float(args[4]),offset=amplitude<0?-sine*(-amplitude):sine*amplitude;
        for(unsigned axis=0;axis<3;++axis)real(u+0x3c+4*axis,current[axis]+(axis==1?offset:0));
        controlPackedUnitMovementSound(u,movement.sound);return 0;
    }
    int32_t genericJump(EventEntry& e,bool first) {
        const int32_t selector=value(e,0),x=value(e,1),y=value(e,2),z=value(e,3),requested=value(e,4);(void)value(e,5);
        const auto id=translateBattleActor(&e,selector,nativeBattleActorServices(vm));const auto u=word(Battle+0x20+4*id);
        if(first) {
            e.initialized=1;if(requested)word(u+0x16c,requested);
            setPackedMovePosition(u,true,{float(x),float(y),float(z)},nativeBattleActorServices(vm).memory);
            setPackedMovePosition(u,false,{real(u+0x3c),real(u+0x40),real(u+0x44)},nativeBattleActorServices(vm).memory);
            real(u+0x17c,vm.platform.headingFloat(this,real(u+0x3c),real(u+0x44),float(x),float(z)));real(u+0x180,std::hypot(float(x)-real(u+0x3c),float(z)-real(u+0x44)));
            const float dx=float(x)-real(u+0x3c),dy=float(y)-real(u+0x40);if(dx!=0)byte(u+0x188,dx>0?1:255);
            if(!word(u+0x16c))word(u+0x16c,uint32_t(int32_t(real(u+0x180)/real(u+0x170))));else real(u+0x170,real(u+0x180)/float(int32_t(word(u+0x16c))));
            const float n=float(int32_t(word(u+0x16c)));real(u+0x180,-std::fma(n,real(u+0x170),-real(u+0x180)));if(!word(u+0x16c))return 2;
            real(u+0x178,std::fma(real(u+0x174)*n,.5f,dy/n));real(u+0x170,real(u+0x170)+real(u+0x180)/n);
        }
        real(u+0x158,real(u+0x158)+real(u+0x178));real(u+0x178,real(u+0x178)-real(u+0x174));if(real(u+0x178)<0 && real(u+0x158)<real(u+0x164))real(u+0x158,real(u+0x164));
        integratePackedMoveHorizontal(u+0x154,u+0x15c,real(u+0x170),real(u+0x17c),movement);
        for(unsigned axis=0;axis<3;++axis)real(u+0x3c+4*axis,real(u+0x154+4*axis));
        word(u+0x16c,word(u+0x16c)-1);
        if(int32_t(word(u+0x16c))>0)return 0;
        for(unsigned axis=0;axis<3;++axis)real(u+0x3c+4*axis,real(u+0x160+4*axis));
        return 1;
    }
    void changePose(uint32_t u,uint32_t p,const std::string& name,bool force) {
        for(unsigned i=0;i<=name.size();++i)byte(p+0x1c4+i,i==name.size()?0:unsigned(name[i]));
        auto flags=(word(p+0x204)|3)&~uint32_t(0x80|0x200|0x10|8);if(force)flags|=8;word(p+0x204,flags);
        const unsigned id=word(u);if(force || scene->actors[visual[id]].pose!=name){scene->actors[visual[id]].pose=name;scene->actors[visual[id]].poseAt=now;}
    }
    TimingACommand timing() const {
        TimingACommand c;c.finishCountdown=int32_t(word(Battle+0x1f54));c.state=int32_t(word(Battle+0x1c9c));c.flags=word(Battle+0x1f50);c.classification=int32_t(word(Battle+0x1ce8));c.result=word(Battle+0x1cb8);c.count=int32_t(word(Battle+0x1cb4));
        for(unsigned i=0;i<11;++i)c.scratch[i]=word(Battle+0x1f20+4*i);
        return c;
    }
    void storeTiming(const TimingACommand& c) {
        word(Battle+0x1f54,c.finishCountdown);word(Battle+0x1c9c,c.state);word(Battle+0x1f50,c.flags);word(Battle+0x1ce8,c.classification);word(Battle+0x1cb8,c.result);word(Battle+0x1cb4,c.count);
        for(unsigned i=0;i<11;++i)word(Battle+0x1f20+4*i,c.scratch[i]);
    }
    BattlePhaseServices phaseServices() {
        BattlePhaseServices s;s.actors=nativeBattleActorServices(vm);
        s.real=[](void* p,uint32_t a){auto& f=*static_cast<EventNativeRuntime*>(p);uint32_t v=0;required(f.memory.readWord(a,v));float out;std::memcpy(&out,&v,4);return out;};
        s.status=[](void* p,uint32_t u,int32_t id){auto& f=*static_cast<EventNativeRuntime*>(p);BattleStatusUnit unit;for(unsigned i=0;i<30;++i)required(f.memory.readByte(u+0x118+i,unit.statusBytes[i]));return checkStatus(&unit,id);};return s;
    }
    bool eventLive(uint32_t id)const {for(int i=0;i<vm.battle.capacity;++i)if((vm.battle.entries[i].flags&1) && vm.battle.entries[i].id==id)return true;return false;}
    bool barrier() {
        bool ready=true;for(unsigned i=0;i<64;++i){const auto u=word(Battle+0x20+4*i);if(!u)continue;for(unsigned o:{0x29cu,0x2a8u,0x2b4u,0x2bcu}){const auto id=word(u+o);if(id){if(eventLive(id))ready=false;else word(u+o,0);}}}
        if(!ready)return false;
        for(unsigned i=0;i<64;++i)if(word(Battle+0x16f5c+8*i)!=0xffffffff){if(!pending)pending=0x800f78fc;return false;} // Queued reaction provider remains open.
        for(unsigned o:{0x182b4u,0x182c4u,0x180fcu,0x182c8u,0x182c0u}){const auto id=word(Battle+o);if(id){if(eventLive(id))return false;word(Battle+o,0);}}return true;
    }
    bool messagesIdle(){for(unsigned i=0;i<28;++i)if(byte(Battle+0x18da0+12*i)!=0xff){if(!pending)pending=0x801956f8;return false;}return true;} // Original empty message work; AnnouceMain unit leaf returns0.
    bool concluded() {
        unsigned count=0;for(unsigned g=0;g<3;++g){const uint16_t config=uint16_t(half(Battle+8+8*g));word(Battle+0xc+8*g,0);bool end=config==0;
            if(config&1){bool found=false;for(unsigned i=0;i<64;++i){const auto u=word(Battle+0x20+4*i);if(u && int8_t(byte(u+0xc))==int(g) && !byte(u+0x135) && !(word(u+0x104)&(0x20000|0x10000000))){found=true;break;}}end=!found;}
            if(!end && (config&2)){bool found=false;for(unsigned i=0;i<64;++i){const auto u=word(Battle+0x20+4*i);if(u && int8_t(byte(u+0xc))==int(g) && (word(u+0x104)&0x40000) && !byte(u+0x135) && !(word(u+0x104)&0x10000000)){found=true;break;}}end=!found;}
            if(end){word(Battle+0xc+8*g,1);++count;}}
        return count>=2 || (word(Battle+0xef4)&0x30);
    }
    bool audienceIdle(){const auto base=(Battle+0x2858),entry=word(base+4);if(entry && eventLive(word(entry+0x15c)))return false;word(base+4,0);return true;}
    bool breakSlotIdle(){const auto work=Battle+0x1616c;const int32_t state=int32_t(word(work+4));if(int32_t(word(work+0xc))>=3){if(!pending)pending=0x8020342c;return false;}if(state>=1 && state<=8)return false;const auto entry=word(work+0x130);if(entry && eventLive(word(entry+0x15c)))return false;word(work+0x130,0);return true;}
    void reserveItems(){for(unsigned i=0;i<4;++i){if(word(Battle+0x19060+4*i)){if(!pending)pending=0x800f74b4;return;}word(Battle+0x19060+4*i,0);}}
    void endingRequest(uint32_t function,std::initializer_list<int32_t> operands={}) {
        trace.push_back({function,0,now,std::vector<int32_t>(operands)});
    }
    BattleAudienceSoundServices audienceSoundServices() {
        BattleAudienceSoundServices s;s.actors=nativeBattleActorServices(vm);s.user=this;
        s.play=[](void* p,uint32_t key,bool assigned,int32_t right,int32_t left)->std::optional<int32_t> {
            auto& r=*static_cast<TrioAttackRuntime*>(p);r.endingRequest(0x8019fcec);
            r.audienceAudioTrace.push_back({assigned?std::optional<uint32_t>{key}:std::nullopt,right,left,std::nullopt});
            if(assigned){r.pending=0x8019fcec;return std::nullopt;} // Known source key needs an actual positional stream provider.
            // Explicit native unavailable-key request failure. Original stack
            // bytes/key lookup are unproven; no name or nonnegative voice minted.
            r.audienceAudioTrace.back().handle=-1;return int32_t(-1);
        };
        s.stop=[](void* p,int32_t handle) {
            auto& r=*static_cast<TrioAttackRuntime*>(p);r.endingRequest(0x800d95f8,{handle});
            if(r.publicSfx->off(handle,{}))return true;
            r.pending=0x800d95f8;return false;
        };
        s.volume=[](void* p,int32_t handle)->std::optional<uint8_t> {auto& r=*static_cast<TrioAttackRuntime*>(p);r.endingRequest(0x800d9558,{handle});const auto v=r.publicSfx->getVolume(handle);if(!v)r.pending=0x800d9558;return v;};
        return s;
    }
    bool endingClap() {
        endingRequest(0x801a0cec,{-1,0});
        if(clapBattleAudienceSound(-1,0,audienceSoundServices()))return true;
        if(!pending)pending=0x801a0cec;
        return false;
    }
    bool endingAudienceTarget(float delta,float bias) {
        endingRequest(0x801a1a74,{int32_t(bits(delta)),int32_t(bits(bias))});
        if(packedBattleAudienceAddTargetNum(delta,bias,nativeBattleActorServices(vm)))return true;
        pending=0x801a18cc;return false;
    }
    void endingStatus(uint32_t u,int32_t id) {
        BattleStatusUnit status;status.flags=word(u+0x27c);
        for(unsigned i=0;i<30;++i)status.statusBytes[i]=byte(u+0x118+i);
        for(unsigned i=0;i<5;++i)status.moveBytes[i]=byte(u+0x20+i);
        setStatus(status,id,0,0);
        for(unsigned i=0;i<30;++i)byte(u+0x118+i,status.statusBytes[i]);
        for(unsigned i=0;i<5;++i)byte(u+0x20+i,status.moveBytes[i]);
    }
    void initializeExpIcons(uint32_t end) {
        endingRequest(0x802149ec,{int32_t(end)});
        rewardIconClockAt=UINT64_MAX; // Source per-record clock starts at creation; iconMain follows event/battle processing.
        const int32_t tens=half(end+0xa)/10;
        for(unsigned i=0;i<19;++i){auto& icon=rewardIcons[i];icon.hidden=true;icon.animationTick=0;icon.scale=real(i<10?0x80427238:0x8042723c);
            // Opaque native preallocated icon handles, retaining source19-entry
            // lifetime and original ID404 art; no original icon ABI is invented.
            word(end+0x34+4*i,0x85100000+0x80*i);word(end+0xcc+4*i,0);
            icon.position={i<10?std::fma(real(0x80427234),float(i),std::fma(-real(0x80427230),float(tens),real(0x8042722c))):std::fma(-real(0x80427244),float(i-9),real(0x80427240)),i<10?real(0x804271e0):real(0x804271c0),real(0x804271c0)};
            for(unsigned axis=0;axis<3;++axis)real(end+0x118+12*i+4*axis,icon.position[axis]);
        }
        word(end+0x30,word(end+0x30)|2);
    }
    void endingDisplay() {
        endingRequest(0x80215088,{8,int32_t(Battle)});const auto end=word(Battle+0xf28);if(!end)return;
        if(word(end+0x30)&2){
            const int32_t remaining=half(end+0xa),tens=remaining/10,ones=remaining-tens*10;
            for(unsigned i=0;i<19;++i){auto& icon=rewardIcons[i];const bool shown=i<10?int32_t(i)<tens:int32_t(i-10)<ones;icon.hidden=!shown;
                if(shown){const int32_t old=int32_t(word(end+0x80+4*i));word(end+0x80+4*i,old<=0?64:std::min<int32_t>(old+64,255));
                    if(i<10){const int32_t phase=int32_t(word(end+0xcc+4*i));if(phase<16)word(end+0xcc+4*i,phase+1);real(end+0x118+12*i,std::fma(real(0x80427234),float(i),std::fma(-real(0x80427230),float(tens),real(0x8042722c))));}}
                else{word(end+0x80+4*i,0);if(i<10)word(end+0xcc+4*i,0);}
                icon.alpha=uint8_t(word(end+0x80+4*i));for(unsigned axis=0;axis<3;++axis)icon.position[axis]=real(end+0x118+12*i+4*axis);
            }
            endingRequest(0x80010bc4,{8,1,int32_t(0x8021471c),int32_t(Battle)}); // Native viewport/window provider is separate from original GX registration.
        }
    }
    void iconClock(uint64_t time) {
        const auto end=word(Battle+0xf28);if(!end || !(word(end+0x30)&2) || rewardIconClockAt==time)return;
        rewardIconClockAt=time;for(auto& icon:rewardIcons)++icon.animationTick; // Hidden flag2 does not suppress source animated1000 clock.
    }
    void endingBodyType(uint32_t u,int32_t type){
        endingRequest(0x80127640,{int32_t(u),type});const auto memory=nativeBattleActorServices(vm).memory;
        const auto p=packedUnitPart(u,packedBodyPartId(u,memory),memory),name=packedBattlePoseName(p,type,memory);
        if(!name){pending=0x8012772c;return;}changePose(u,p,text(name),type>=29 && type<=39);
    }
    void ending() {
        const auto info=word(Battle+0x2738),mario=battleMarioUnit(Battle,nativeBattleActorServices(vm)),party=battlePartyUnit(Battle,nativeBattleActorServices(vm));
        auto state=word(Battle+0xf24);endingRequest(0x80212890);
        // E0 remains zero before the unbound reward presentation owner. Whistle's
        // source range07..1b is not admitted by this bounded implementation.
        if(state==0x07000000){
            endingRequest(0x80125398);for(unsigned i=0;i<64;++i){const auto u=word(Battle+0x20+4*i);if(u)byte(u+0x311,0xff);}
            BattleConclusionServices services;services.actors=nativeBattleActorServices(vm);services.user=this;
            services.enemyBelong=[](void* p,uint32_t u){return packedEnemyBelong(u,nativeBattleActorServices(static_cast<TrioAttackRuntime*>(p)->vm));};
            const auto result=battleInitialEndResult(Battle,services);if(result){word(info+0x10,uint32_t(*result));endingRequest(0x80114508,{int32_t(info),*result});}
            if(word(info+0x10)!=1){pending=0x80215234;return;}
            if(word(info+0x1c)){word(Battle+0xf24,0x07000003);pending=0x80215430;return;}
            endingRequest(0x8021290c);const auto group=word(word(info+0xc)+0x18),sets=word(group+4),pouch=word(0x8041eb00),a=(Battle+0x2858);
            const int32_t level=half(pouch+0x8a)+1;
            if(int32_t(byte(word(sets)+0xe))>level){endingRequest(0x8019effc);if(!endingAudienceTarget(real(0x804249a0)*real(a+0x13778),real(0x80424988)))return;}
            uint32_t sum=0;for(uint32_t i=0;i<word(group);++i)sum+=byte(word(sets+0x30*i)+0xe);
            const int32_t threshold=int32_t(2*sum)/level/3,turn=half(Battle);
            if(turn<threshold){const float delta=float(threshold-turn);endingRequest(0x8019efa4,{int32_t(bits(delta))});const float product=real(0x8042499c)*(real(0x80424998)*delta);if(!endingAudienceTarget(product*real(a+0x13778),real(0x80424988)))return;}
            word(Battle+0xf24,0x07000005);endingRequest(0x80212844);
            auto& koura=createRootEvent(vm.schedule,0x803b0c50,10,0,vm.creationServices());koura.owner=int32_t(word(mario));
            endingRequest(0x800f8934,{0x2ac});const auto end=eventMapAllocate(word(0x8041e80c),0x2ac,vm.memoryBinding.services());required(end!=0);word(Battle+0xf28,end);
            endingRequest(0x802151b4);word(end+0x30,0);word(end,0);word(end+0x29c,0);word(end+0x2a0,random.bounded(120)+60);word(end+0x2a4,0);
            endingDisplay();return;
        }
        if(state==0x07000005){
            // The alternate party body-table/status-flag branch needs its
            // original owner. Preserve it rather than clearing the flag here.
            if(party && (word(party+0x138)&1)){pending=0x801277d8;return;}
            reserveItems();if(pending)return;
            for(int32_t id=0;id<25;++id){if(party)endingStatus(party,id);endingStatus(mario,id);}
            if(word(mario+0x104)&0x10){pending=0x802155b4;return;}
            word(Battle+0xf24,0x07000006);state=0x07000006;
        }
        if(state==0x07000006){
            if(!barrier()){endingDisplay();return;}
            if(!endingClap())return;
            endingRequest(0x802151c4);
            for(unsigned i=0;i<64;++i){const auto u=word(Battle+0x20+4*i);if(!u)continue;const auto script=battleUnitData(u,0x3f,nativeBattleActorServices(vm).memory);if(script){auto& e=createRootEvent(vm.schedule,script,10,0,vm.creationServices());e.owner=int32_t(word(u));}}
            BattleDropMaterialServices drops;drops.actors=nativeBattleActorServices(vm);drops.user=this;
            drops.random=[](void* p,int32_t bound){auto& r=*static_cast<TrioAttackRuntime*>(p);r.endingRequest(0x80005da0,{bound});return int32_t(r.random.bounded(bound));};
            drops.story=[](void* p){return static_cast<TrioAttackRuntime*>(p)->scene->story;};
            drops.equippedBadgeCount=[](void* p,int32_t badge){auto& r=*static_cast<TrioAttackRuntime*>(p);r.endingRequest(0x800d3300,{badge});return packedPouchEquippedBadgeCount(r.word(0x8041eb00),badge,nativeBattleActorServices(r.vm).memory);};
            endingRequest(0x801141bc,{int32_t(info)});battleInformationSetDropMaterial(info,drops);
            if(byte(word(0x8041eb00)+0x5ba)==3){pending=0x80215718;return;}
            word(Battle+0xf24,0x07000008);
            endingDisplay();
            return;
        }
        if(state==0x07000008){
            endingRequest(0x80147bd0);if(word(Battle+0x182b8)!=0){pending=0x80147bd0;return;} // Exact fresh no-fog early return; active effect lifetime remains required.
            const auto end=word(Battle+0xf28);word(end+0xc,0);endingRequest(0x800f7bd4,{int32_t(Battle)});
            const int32_t stock=int32_t(word(Battle+0xf00));half(end+8,stock<=0?1:stock>=100?100:stock);
            if(byte(Battle+0x18ff9)==3)half(end+8,2*half(end+8));
            if(half(word(0x8041eb00)+0x8a)>=99)half(end+8,0);
            if(half(end+8)>100)half(end+8,100);
            half(end+0xa,half(end+8));word(Battle+0xef8,word(Battle+0xef8)&~1u);
            if(half(end+8)>0)initializeExpIcons(end);
            word(end+4,120);BattleCameraRequestServices camera;camera.positions=nativeBattlePartPositionServices(vm);
            endingRequest(0x8012b958,{0,2});setBattleCameraSpeed(Battle,0,2,camera);
            const int32_t cameraMode=(word(Battle+0xef4)&0x100)?18:17;
            endingRequest(0x8012c028,{0,cameraMode});setBattleCameraMode(Battle,0,cameraMode,camera);
            endingRequest(0x8012b198,{0,0,0,0});setBattleCameraOffset(Battle,0,0,0,0,camera);
            word(Battle+0xf24,0x07000009);state=0x07000009;
            endingRequest(0x801a0aec,{60,60});if(!endingClap())return;endingRequest(0x801a08c0);
            endingRequest(0x800daefc,{0x400});endingRequest(0x800db7a0,{1,int32_t(party && byte(party+0x135)?0x8030ea98:0x8030ea88)}); // Original WIN2/WIN1 keys; complete voice/camera/crowd routing remains an adapter.
        }
        if(state==0x07000009){const auto end=word(Battle+0xf28);word(end+4,word(end+4)-1);if(int32_t(word(end+4))<=0)word(Battle+0xf24,0x0700000a);endingDisplay();return;}
        if(state==0x0700000a){const auto end=word(Battle+0xf28);if(half(end+8)<=0){pending=0x80215938;return;}
            for(const auto u:{mario,party}){if(!u)continue;const auto script=battleUnitData(u,0x40,nativeBattleActorServices(vm).memory);if(!script){pending=0x8012733c;return;}auto& e=createRootEvent(vm.schedule,script,10,0,vm.creationServices());e.owner=int32_t(word(u));word(u+0x2a8,e.id);}
            endingRequest(0x8021b70c,{0,int32_t(half(end+8)),0,90,0,0});
            pointGet.enter(half(end+8),0,90,0,0);pointGetClockAt=UINT64_MAX;
            word(end+0x29c,0x85200000); // Opaque native effect owner, not an invented original effEntry ABI.
            word(Battle+0xf24,0x0700000b);endingDisplay();return;
        }
        if(state==0x0700000b){const auto end=word(Battle+0xf28),pouch=word(0x8041eb00);
            if(int16_t(half(end+0xa))>0){word(end+4,word(end+4)+1);endingRequest(0x80119fc8,{0x300});
                if((word(Battle+0x1034)&0x300) && int32_t(word(end+4))<1000)word(end+4,word(end+4)+1000);
                const auto counter=word(end+4);
                if(!(counter&3) || int32_t(counter)>=1000){half(end+0xa,half(end+0xa)-1);half(pouch+0x96,half(pouch+0x96)+1);
                    if(!(counter&7)){endingRequest(0x800d98bc,{int32_t(0x802feaa8)});soundRequests.emplace_back("SFX_STAR_POINT_GET1");}
                }
                if(int16_t(half(pouch+0x96))>=100){half(pouch+0x96,0);word(end+0xc,1);if(int16_t(half(pouch+0x8a))+1>=99)half(end+0xa,0);}
            }else{const auto counter=int32_t(word(end+4));word(end+4,counter>=1000?10:counter<120?140-counter:20);word(Battle+0xf24,0x0700000c);}
            endingDisplay();return;
        }
        if(state==0x0700000c){const auto end=word(Battle+0xf28);word(end+4,word(end+4)-1);
            if(int32_t(word(end+4))<=0){word(end+4,60);endingRequest(0x802146c0,{int32_t(end)});
                word(end+0x30,word(end+0x30)&~2u); // Source deletion preserves icon records and their pointers.
                word(Battle+0xf24,0x0700000d);endingRequest(0x8005e9c0,{int32_t(word(end+0x29c))});if(pointGet.active)pointGet.softDelete=true;
                if(word(end+0xc)==0)word(end+4,5);
            }endingDisplay();return;
        }
        if(state==0x0700000d){const auto end=word(Battle+0xf28);word(end+4,word(end+4)-1);if(int32_t(word(end+4))<=0){word(end+0x29c,0);word(Battle+0xf24,0x0700000e);}endingDisplay();return;}
        if(state==0x0700000e){if(word(Battle+0xef4)&0x400000){pending=0x8041b8c0;return;}word(Battle+0xf24,0x07000010);endingDisplay();return;}
        if(state==0x07000010){const auto end=word(Battle+0xf28);word(end+0x30,word(end+0x30)&~2u);if(word(end+0xc)){pending=0x80215cc0;return;}word(Battle+0xf24,0x07000019);endingDisplay();return;}
        if(state==0x07000019){const auto end=word(Battle+0xf28),flags=word(mario+0x138),setup=word(info+0xc);
            if(!(flags&0x30000000) && (!party || (flags==0 && !byte(party+0x135)))){const uint32_t key=(word(setup+4)&0x20000000)?0x802feafc:0x802feb14;endingRequest(0x8012619c,{int32_t(mario),int32_t(key),int32_t(0xf1194d80),0});soundRequests.push_back(text(key));}
            endingRequest(0x801275f4,{int32_t(mario),int32_t(0x80427298)});const auto memory=nativeBattleActorServices(vm).memory;changePose(mario,packedUnitPart(mario,packedBodyPartId(mario,memory),memory),text(0x80427298),false);
            word(end+4,60);word(Battle+0xf24,0x0700001a);endingDisplay();return;
        }
        if(state==0x0700001a){const auto end=word(Battle+0xf28);word(end+4,word(end+4)-1);if(int32_t(word(end+4))<=0){endingBodyType(mario,42);if(party && !(word(party+8)==225 && int8_t(byte(party+0x12b))>=1))endingBodyType(party,42);if(!pending)word(Battle+0xf24,0x0700001b);}endingDisplay();return;}
        if(state==0x0700001b){word(Battle+0xf24,0x0700001c);endingDisplay();return;}
        if(state==0x0700001c){word(Battle+0xf24,0x0700001d);endingRequest(0x8019fddc,{0,30});setAllBattleAudienceVolumes(0,30,nativeBattleActorServices(vm));endingRequest(0x8004829c);if(fieldNpcBattleFlags)*fieldNpcBattleFlags|=0x100; // Source F8 NPC140 view supplied by the field owner.
            endingDisplay();return;}
        if(state==0x0700001d){endingRequest(0x80013368,{10,200,255});endingFade.enter(200);endingFadeClockAt=UINT64_MAX;word(Battle+0xf24,0x0700001e);endingRequest(0x800daefc,{0x201});endingDisplay();return;}
        if(state==0x0700001e){endingRequest(0x80010d94);if(endingFade.finished)word(Battle+0xf24,0x0700001f);endingDisplay();return;}
        if(state==0x0700001f){if(!endingExported){const auto setup=word(info+0xc);endingRequest(0x800f8444,{party?int32_t(word(party+8)):0});
                if((word(setup+4)&0x40) && int16_t(half(mario+0x10c))<=0)half(mario+0x10c,1);
                endingRequest(0x800f8a68,{int32_t(Battle)});PackedUnitResourceServices resources;resources.memory=nativeBattleActorServices(vm);resources.user=this;resources.pouch=[](void* p){return static_cast<TrioAttackRuntime*>(p)->word(0x8041eb00);};resources.partyIndex=[](void*,int32_t kind){return battlePartyIndex(kind);};
                exportPackedUnitResources(Battle,mario,resources);if(party)exportPackedUnitResources(Battle,party,resources);
                // Native field/battle views share the retained canonical pouch.
                // This copy publishes exact packed writes, including EXP, without healing or rebuilding inventory.
                const auto pouch=word(0x8041eb00);for(unsigned i=0;i<entryPouch->bytes.size();++i)entryPouch->bytes[i]=byte(pouch+i);
                endingExported=true;
                const auto end=word(Battle+0xf28);endingRequest(0x800f8908,{int32_t(end)});if(end)eventMapFree(word(0x8041e80c),end,vm.memoryBinding.services());
                word(Battle+0xf28,0);packedBattleSetSequence(Battle,0,4,nativeBattleActorServices(vm).memory);
            }
            if(!fieldMario){pending=0x8005e0c0;return;}
            endingRequest(0x8005e0c0);restoreFieldMarioAfterBattle(*fieldMario);
            if(!fieldParties){pending=party?0x801412fc:0x80141198;return;}
            if(party) {
                const auto kind=battlePartyIndex(int32_t(word(party+8)));
                endingRequest(0x801412fc,{kind});
                if(kind<0 || kind>=8){pending=0x801412fc;return;} // Other constructor domains remain their own providers.
                if(entryPouch->half(unsigned(14*kind))&1) {
                    if(fieldMario->partyIndices[0]<128){pending=0x800be69c;return;}
                    endingRequest(0x800bea68,{kind});endingRequest(0x800be144,{kind});
                    const auto slot=fieldParties->allocateFirst();
                    if(slot<0){pending=0x800bea68;return;} // Entry wrapper requires a free slot.
                    endingRequest(0x801a94f0);fieldParties->initializeBeforePose(slot,uint8_t(kind),*fieldMario);
                    if(kind!=1){pending=0x800be144;return;} // Other definition/resource records retain their provider.
                    const uint32_t resource=(fieldParties->pointer(slot)->word(4)&0x8000)?0x802cb340:0x804213e0;
                    endingRequest(0x8004176c,{int32_t(resource),2});
                    const auto pose=fieldParties->enterPose(resource,2,resource==0x804213e0?fieldPartnerAnimationBank():nullptr,now);
                    if(pose<0){pending=0x8004176c;return;}
                    fieldParties->pointer(slot)->word(0xc,uint32_t(pose));
                    endingRequest(0x800414b0,{pose});fieldParties->poses[unsigned(pose)]->paperOff();
                    endingRequest(0x800593fc);
                    const auto marioPose=fieldMario->poseHandles[unsigned(fieldMarioAnimationId(*fieldMario))];
                    endingRequest(0x8004069c,{marioPose});
                    const auto* materialOwner=fieldParties->pose(marioPose);
                    if(!materialOwner){pending=0x8004069c;return;}
                    endingRequest(0x80040730,{pose,int32_t(materialOwner->materialFlags)});
                    fieldParties->pose(pose)->materialFlags|=materialOwner->materialFlags;
                    endingRequest(0x800593fc);
                    const auto marioLightPose=fieldMario->poseHandles[unsigned(fieldMarioAnimationId(*fieldMario))];
                    endingRequest(0x80040684,{marioLightPose});
                    const auto* lightOwner=fieldParties->pose(marioLightPose);
                    if(!lightOwner){pending=0x80040684;return;}
                    endingRequest(0x800406f0,{pose,int32_t(lightOwner->materialLightFlags)});
                    fieldParties->pose(pose)->materialLightFlags|=lightOwner->materialLightFlags;
                    endingRequest(0x80140ea4,{kind});
                    const int16_t hp=entryPouch->half(unsigned(14*kind+6));
                    if(!fieldParties->requestFreshGoombellaPose(slot,hp)){pending=0x800be144;return;}
                    if(fieldParties->currentDefinition!=0x80314410){pending=0x800be144;return;}
                    endingRequest(0x800c4fa0,{slot});fieldParties->kuriboInit(slot);
                    if(!fieldAppearance.ready()){pending=0x800bea68;return;}
                    auto* actor=fieldParties->pointer(slot);
                    actor->word(0x70,0x3f800000);actor->word(0x74,0x3f800000);actor->word(0x78,0x3f800000);actor->bytes[0x38]=10;
                    actor->scalar(0x110,fieldAppearance.display(fieldAppearance.user));
                    actor->scalar(0x10c,fieldAppearance.display(fieldAppearance.user));
                    auto appearance=[&]() {
                        endingRequest(0x800c6718,{slot});FieldWallVector position;
                        fieldPartyAppearPosition2(actor->scalar(0xf4),slot,position,fieldAppearance); // Original caller ignores success/failure.
                        actor->scalar(0x58,position.x);actor->scalar(0x5c,position.y);actor->scalar(0x60,position.z);
                        actor->word(0xb8,actor->word(0x58));actor->word(0xbc,actor->word(0x5c));actor->word(0xc0,actor->word(0x60));
                        if(!(actor->word(4)&2)) {
                            endingRequest(0x80140ea4,{kind});
                            if(!fieldParties->requestGoombellaPose(slot,entryPouch->half(unsigned(14*kind+6))))return false;
                        }
                        actor->bytes[0x34]=1;return true;
                    };
                    if(!appearance()){pending=0x800bea68;return;}
                    fieldMario->partyIndices[0]=uint8_t(slot); // Source publishes only after first placement/pose/mode.
                    endingRequest(0x800bf1b4,{slot});endingRequest(0x800bbc48,{slot});
                    if((actor->word(8)&8) || actor->bytes[0x38]!=10){pending=0x800bbc48;return;}
                    endingRequest(0x800bb544,{slot});
                    if(!appearance()){pending=0x800bb544;return;} // Fresh Hello repeats placement and HP, never the16-count jump path.
                } // Real source -1 return: no unlock, kill or constructor side effects.
            }else {
                endingRequest(0x80141198);
                for(const auto index:fieldMario->partyIndices)if(index<128){pending=0x800be69c;return;}
            }
            endingRequest(0x80141050);const auto index=fieldParties->current(*fieldMario);
            endingRequest(0x800bf1b4,{index});if(auto* actor=fieldParties->pointer(index))actor->word(4,actor->word(4)&~1u);
            endingRequest(0x8018ef8c);judgeBattleGeneralRule(nativeBattleActorServices(vm));
            endingRequest(0x8019fd68,{1});
            if(!stopBattleAudienceSound(1,audienceSoundServices())){if(!pending)pending=0x8019fd68;return;}
            pending=0x800f8e28;return; // Uppercase engine's remaining services/completion and sequence/exit owners follow real ending.
        }
        pending=0x80215234;
    }
    void postAction() {
        auto state=word(Battle+0xf20);
        if(state==0x06000007){if(!barrier())return;word(Battle+0xf20,++state);} // Adjust is empty under original ff message work.
        if(state==0x06000008){if(!barrier() || !messagesIdle())return;word(Battle+0xf20,++state);}
        if(state==0x06000009){if(!(word(Battle+0xef4)&0x30))packedBattleStatusWindowSet(0x2744,true,statusWindowServices());const auto u=word(Battle+0x20+4*word(Battle+0x420));if(u){word(u+0x1c,word(u+0x1c)&~0x04000000);if(word(u+0x27c)&8){byte(u+0x129,0);word(u+0x27c,word(u+0x27c)&~8);}if(byte(Battle+0x18ff9)==1)byte(Battle+0x18ff9,0);word(u+0x25c,0);consumeBattleActorMove(Battle,u,phaseServices());}reserveItems();if(pending)return;word(Battle+0xef4,word(Battle+0xef4)&~4u);word(Battle+0xf20,++state);}
        // Audience/BreakSlot producers are explicit native presentation adapters;
        // actual readiness fields/events are polled, never inferred from HP.
        if(state==0x0600000a){if(!audienceIdle())return;if(!concluded() && !breakSlotIdle())return;word(Battle+0xf20,++state);}
        if(state==0x0600000b){if(!messagesIdle())return;word(Battle+0xf20,++state);}
        if(state==0x0600000c){if(!barrier())return;const auto u=word(Battle+0x20+4*word(Battle+0x420));if(u){word(u+0x2cc,0);const auto script=word(u+0x2c8);if(script){auto& e=createRootEvent(vm.schedule,script,10,0,vm.creationServices());e.owner=int32_t(word(u));word(u+0x2cc,e.id);const auto wait=word(u+0x290);if(wait)deleteEventId(wait,vm.creationServices());word(u+0x290,0);}}word(Battle+0xf20,++state);}
        if(state==0x0600000d){const auto u=word(Battle+0x20+4*word(Battle+0x420));if(u && word(u+0x2cc)){if(eventLive(word(u+0x2cc)))return;word(u+0x2cc,0);}word(Battle+0xf20,++state);}
        if(state==0x0600000e && barrier()){word(Battle+0xf1c,word(Battle+0xf1c)+1);done=true;} // Move10 executes on the next outer call.
    }
    void runPhaseEvent(uint32_t u,unsigned selector) {
        const auto script=word(u+(selector?0x294:0x298));
        auto& e=createRootEvent(vm.schedule,script,10,0x20,vm.creationServices());
        e.owner=int32_t(word(u));word(u+0x29c,e.id);
    }
    bool phaseAudienceIdle() {
        (void)word(0x8041eb00);const auto a=(Battle+0x2858),flags=word(a);if(!(flags&8))return true;
        for(unsigned i=0;i<200;++i){const auto f=word(a+0x1bc+0x134*i);if((f&1) && !(f&(0x100|2|0x10))){if(!pending)pending=0x801a6ce8;return false;}}
        for(uint32_t i=0;i<word(a+0x13904);++i){const auto category=word(a+0x138cc+4*i);if(category!=4 && category!=9 && category!=10 && category!=11 && category!=12 && category!=13){if(!pending)pending=0x801a6ce8;return false;}}
        const auto state=word(a+0x137dc);
        if(state==0 || state==5 || state==7){if(state==0)word(a+0x137dc,5);if(flags&0x40000)word(a+0x137dc,15);else {word(a+0x137dc,7);word(a+0x137dc,13);}return false;}
        if(state==13){word(a+0x13904,0);word(a+0x137dc,12);word(a+0x137e0,0);word(a+0x137dc,15);return false;}
        if(state==15){const auto slot=Battle+0x1616c+0x10,next=word(slot)-1;word(slot,int32_t(next)<0?0:next);word(a,flags&~8u);word(a+0x13904,0);return true;}
        if(state==20){word(a,flags&~8u);word(a+0x13904,0);return true;}
        if(!pending)pending=0x801a6ce8;
        return false;
    }
    void addAudiencePhase(int32_t category) {
        const auto a=(Battle+0x2858);const int32_t count=int32_t(word(a+0x13904));
        for(int32_t i=0;i<count;++i)if(word(a+0x138cc+4*uint32_t(i))==uint32_t(category))return;
        word(a+0x138cc+4*uint32_t(count),uint32_t(category));word(a+0x13904,word(a+0x13904)+1);
    }
    bool audienceTurnEnd() {
        const auto b=word(0x8041eb90),mario=battleMarioUnit(b,nativeBattleActorServices(vm)),a=(b+0x2858),pouch=word(0x8041eb00);
        const auto setup=word(word(b+0x2738)+0xc),group=word(setup+0x18);
        if(word(a)&0x40000)return true;
        const auto chance=word(a+0x13908)+3;word(a+0x13908,int32_t(chance)>100?100:chance);
        uint32_t sum=0;const auto sets=word(group+4);for(uint32_t i=0;i<word(group);++i)sum+=byte(word(sets+0x30*i)+0xe);
        const int32_t threshold=std::max<int32_t>(5,int32_t(2*sum)/byte(mario+0xd)),turn=half(b);
        if(turn>=threshold){if(!pending)pending=0x801a1a74;return false;} // Source AddTargetNum needs its target-size provider.
        const auto draw=random.bounded(100);
        if(int32_t(draw)<int32_t(word(a+0x13908))){for(int32_t category:{4,9,10,11,12,13})addAudiencePhase(category);word(a+0x13908,0);}
        if(!(word(a)&0x40000)){
            int32_t target=int32_t(real(a+0x1377c)+real(a+0x13780));if(target<0)target=0;if(target>int32_t(word(a+0x13790)))target=int32_t(word(a+0x13790));
            int32_t current=0;for(unsigned i=0;i<200;++i){const auto record=a+0x1bc+0x134*i,flags=word(record);if((flags&1) && !(flags&(0x100|2)) && byte(record+0x19)!=12)++current;}
            const int32_t amount=int32_t(uint32_t(target)-uint32_t(current));
            // Source AddAudienceNum early exits remain valid for the explicit
            // empty crowd adapter; allocation/movement for nonzero amounts is pending.
            const auto capacity=byte(pouch+0x8c);if(capacity && amount && !(word(b+0xef4)&0x100)){if(!pending)pending=0x801a1708;return false;}
        }
        return true;
    }
    bool emptyTurnPass(uint32_t state) {
        static constexpr std::array<int32_t,23> recovery{{1,2,3,5,6,7,8,9,10,11,4,12,13,14,15,0,23,24,18,19,20,21,22}};
        const auto actors=nativeBattleActorServices(vm);
        for(unsigned i=0;i<64;++i){const auto u=word(Battle+0x20+4*i);if(!u || half(u+0x10c)<=0)continue;
            if(checkPackedBattleStatus(u,27,actors))continue;
            bool admitted=false;uint32_t provider=0;
            if(state==6 || state==8 || state==10 || state==12){const int32_t id=state==6?4:state==8?8:state==10?23:24;const auto value=getPackedBattleStatus(u,id,actors);admitted=int8_t(value.strength)>0;provider=state==6?0x8011d898:state==8?0x8011d978:state==10?0x8011db60:0x8011dd08;}
            else if(state==14 || state==16){admitted=half(Battle)>1 && byte(u+(state==14?0x2ee:0x2ef));provider=state==14?0x8011de64:0x8011e01c;}
            else {admitted=getPackedBattleStatus(u,recovery[(state-18)/2],actors).duration!=0;provider=0x80127890;}
            if(admitted){if(!pending)pending=provider;return false;}
        }
        return true;
    }
    void handoff() {
        if(word(Battle+0xf14)==0x0300004d){if(!audienceTurnEnd())return;word(Battle+0xf14,0x03000001);word(Battle+0xef4,word(Battle+0xef4)&~0x2000u);return;}
        if(word(Battle+0xf14)==0x03000001){BattleTurnFirstBackend backend;backend.runtime=&vm;battleTurnFirstProcess(Battle,nativeBattleTurnFirstServices(backend));word(Battle+0xf14,0x03000002);}
        if(word(Battle+0xf14)==0x03000002){word(Battle+0xf14,0x03000003);}
        if(word(Battle+0xf14)==0x03000003){if(half(Battle)!=1)word(Battle+0xf14,0x03000005);else {if(!pending)pending=0x8011c588;return;}}
        if(word(Battle+0xf14)==0x03000005)word(Battle+0xf14,0x03000006);
        while(word(Battle+0xf14)>=0x03000006 && word(Battle+0xf14)<=0x0300003e){const auto state=word(Battle+0xf14)-0x03000000;if(!emptyTurnPass(state))return;word(Battle+0xf14,word(Battle+0xf14)+2);}
        while(word(Battle+0xf14)>=0x03000040 && word(Battle+0xf14)<=0x03000044){const auto field=Battle+0x19050+2*(word(Battle+0xf14)-0x03000040);if(word(field))word(field,word(field)-1);word(Battle+0xf14,word(Battle+0xf14)+2);}
        while(word(Battle+0xf14)==0x03000046 || word(Battle+0xf14)==0x03000048){const auto field=word(Battle+0xf14)==0x03000046?0x13c:0x13e;
            for(unsigned i=0;i<64;++i){const auto u=word(Battle+0x20+4*i);if(!u || half(u+0x10c)<=0 || checkPackedBattleStatus(u,27,nativeBattleActorServices(vm)))continue;const auto timer=uint16_t(half(u+field));if(timer)half(u+field,timer-1);}word(Battle+0xf14,word(Battle+0xf14)+2);
        }
        if(word(Battle+0xf14)==0x0300004a){word(Battle+0xf14,0x0300004b);word(Battle+0xf14,0x0300004c);word(Battle+0xf18,0x04000000);return;}
        if(word(Battle+0xf18)==0x04000006){word(Battle+0xf14,word(Battle+0xf14)+1);word(Battle+0xf18,0x04000000);return;}
        if(word(Battle+0xf18)==0x04000000){word(Battle+0xf18,0x04000001);word(Battle+0xf1c,0x05000000);return;}
        for(;;) {
            const auto state=word(Battle+0xf1c);
            if(state==0x05000000){buildBattlePhaseTables(Battle,phaseServices());word(Battle+0xf1c,state+1);continue;}
            if(state==0x05000001){word(Battle+0xf1c,state+1);continue;}
            if(state==0x05000002){if(byte(0x85000000+0x5ba)==2 && word(Battle+0xf18)==0x04000004){if(!pending)pending=0x8011bc9c;return;}word(Battle+0xf1c,state+2);continue;}
            if(state==0x05000003 || state==0x05000005){if(!barrier())return;word(Battle+0xf1c,state+1);continue;}
            if(state==0x05000004){for(unsigned i=0;i<64;++i){const auto u=word(Battle+0x20+4*i);if(u && word(u+0x294))runPhaseEvent(u,1);}word(Battle+0xf1c,state+1);continue;}
            if(state==0x05000006){for(unsigned i=0;i<64;++i){const auto pair=Battle+0x220+8*i,id=word(pair);if(id==0xffffffff)continue;const auto u=word(Battle+0x20+4*id);if(!u || !word(u+0x298))continue;runPhaseEvent(u,0);word(pair,0xffffffff);word(Battle+0xf1c,state+1);return;}word(Battle+0xf1c,0x0500000a);continue;}
            if(state==0x05000007){if(!barrier())return;if(concluded()){word(Battle+4,3);word(Battle+0xf24,0x07000000);return;}word(Battle+0xf1c,0x05000006);continue;}
            if(state==0x05000008){if(word(Battle+0xf18)==0x04000004){prepareActor(false);done=false;readyActor=-1;enemyAction();}return;}
            if(state==0x0500000a){if(concluded()){word(Battle+4,3);word(Battle+0xf24,0x07000000);return;}for(unsigned i=0;i<64;++i){const auto u=word(Battle+0x20+4*i);if(u){word(u+0x264,0);word(u+0x27c,word(u+0x27c)&~0x1000u);}}
                if(battlePhaseEnded(Battle,phaseServices()) || (word(Battle+0xef4)&0x2000)){if(byte(Battle+0x18ff9)==2)byte(Battle+0x18ff9,0);
                    // Native audience inventory/presentation is empty. The source
                    // phase4 producer needs its original slots/effects before binding.
                    if(word(Battle+0xf18)==0x04000004){const auto a=(Battle+0x2858);const auto draw=random.bounded(100);word(a+0x137f4,draw<80?0:draw<90?1:draw<95?2:3);word(a+0x137f0,0);word(a+0x137f8,0);word(a,word(a)|8);word(a+0x137dc,0);}
                    word(Battle+0xf1c,state+1);return;}word(Battle+0xf1c,0x05000008);return;}
            if(state==0x0500000b){if(!phaseAudienceIdle())return;if(barrier())word(Battle+0xf1c,state+1);return;}
            if(state==0x0500000c){if(messagesIdle()){word(Battle+0xf18,word(Battle+0xf18)+1);word(Battle+0xf1c,0x05000000);}return;}
            return;
        }
    }
    bool statusWindowOpen=true;
    BattleStatusWindowServices statusWindowServices(){
        BattleStatusWindowServices s;s.units=nativeBattleUnitEventServices(vm);s.user=this;
        s.isOpen=[](void* self){return static_cast<TrioAttackRuntime*>(self)->statusWindowOpen;};
        s.open=[](void* self){auto& r=*static_cast<TrioAttackRuntime*>(self);r.statusWindowOpen=true;r.endingRequest(0x8013c3b8);};
        s.close=[](void* self){auto& r=*static_cast<TrioAttackRuntime*>(self);r.statusWindowOpen=false;r.endingRequest(0x8013c390);};return s;
    }
    void registerSelectedAttack(){
        root=0;BattleAttackRegistrationServices s;s.memory=nativeBattleUnitEventServices(vm).memory;s.events=vm.creationServices();s.schedule=&vm.schedule;s.user=this;
        s.unit=[](void* self,uint32_t b,int32_t id){return battleRegistryUnit(b,id,nativeBattleActorServices(static_cast<TrioAttackRuntime*>(self)->vm));};
        s.status=[](void* self,uint32_t u,int32_t type)->int32_t{return checkPackedBattleStatus(u,type,nativeBattleActorServices(static_cast<TrioAttackRuntime*>(self)->vm))?1:0;};
        s.irand=[](void* self,int32_t bound){return static_cast<TrioAttackRuntime*>(self)->random.bounded(bound);};
        s.windowOff=[](void* self){auto& r=*static_cast<TrioAttackRuntime*>(self);packedBattleStatusWindowSet(0x2744,false,r.statusWindowServices());};
        s.create=[](void* self,uint32_t script,int32_t priority,uint32_t flags){auto& r=*static_cast<TrioAttackRuntime*>(self);auto& entry=createRootEvent(r.vm.schedule,script,priority,flags,r.vm.creationServices());r.root=entry.id;return &entry;};
        s.advance=[](void* self,uint32_t b,int32_t channel){auto& r=*static_cast<TrioAttackRuntime*>(self);packedBattleIncrementSequence(b,uint32_t(channel),nativeBattleUnitEventServices(r.vm).memory);};
        registerBattleAttackState6(Battle,s);
    }
    void enemyAction() {
        const auto u=word(Battle+0x20+4*word(Battle+0x420));required(u!=0);
        const auto state=word(Battle+0xf20);
        if(state==0x06000000){word(u+0x284,0);word(u+0x2d0,0);const auto audience=(Battle+0x2858);word(audience+0x137d4,0);word(audience,word(audience)&~0x1ff0u);word(Battle+0xf20,0x06000003);return;}
        if(state==0x06000003){word(u+0x2ac,1);word(u+0x1c,word(u+0x1c)|0x04000000);word(Battle+0xf20,0x06000006);return;}
        if(state!=0x06000006)return;
        registerSelectedAttack();
    }
    void prepareActor(bool command=true){for(unsigned i=0;i<12;++i)byte(Battle+0x18ffc+i,0);for(unsigned i=0;i<64;++i){const auto u=word(Battle+0x20+4*i);if(!u)continue;word(u+0x27c,word(u+0x27c)&0x80002000);for(unsigned o:{0x264u,0x268u,0x26cu,0x280u})word(u+o,0);half(u+0x260,0);for(unsigned o=0x270;o<=0x273;++o)byte(u+o,0);}
        readyActor=selectBattlePhaseActor(Battle,phaseServices());const auto u=word(Battle+0x20+4*readyActor);word(Battle+0x420,readyActor);word(u+0x27c,word(u+0x27c)|0x1000);word(Battle+0xf1c,0x05000009);word(Battle+0xf20,command?0x06000002:0x06000000);
        for(unsigned i=0;i<64;++i){const auto id=word(Battle+0x120+4*i);if(id==0xffffffff)continue;const auto p=word(Battle+0x20+4*id);if(p && byte(p+0x20)==1)byte(p+0x20,0);}word(u+0x284,0);word(u+0x2d0,0);
    }
    bool launch(const TrioBattleCommands& menu){const auto weapon=menu.attack->weapon;word(Battle+0x420,menu.actor);word(Battle+0x1c5c,menu.actor==2?6:menu.command);word(Battle+0x171c+0x540,menu.actor==2?6:menu.command);word(Battle+0x171c+(menu.actor==2?0x4c8:menu.command==0?0x4b0:0x4bc),0);word(Battle+0x171c+0x80,weapon);for(unsigned i=0;i<menu.candidates.bytes.size();++i)byte(Battle+0x428+i,menu.candidates.bytes[i]);
        const auto u=word(Battle+0x20+4*menu.actor);
        BattleAttackSelectionServices selection;selection.memory=nativeBattleActorServices(vm);selection.user=this;
        selection.cursor=[](void* self,uint32_t command,uint32_t type,uint32_t& cursor){auto& r=*static_cast<TrioAttackRuntime*>(self);return findPackedCommandCursor(command,type,cursor,nativeBattlePackedSelectionServices(r.vm));};
        if(!packedSetBattleAttackEvent(u,Battle+0x171c,selection))return false;
        word(Battle+0xef4,word(Battle+0xef4)|4);word(Battle+0xf20,0x06000006);word(Battle+0xf1c,0x05000009);registerSelectedAttack();done=false;readyActor=-1;return true;
    }
    bool hasBrokenUnit() const {for(unsigned id=0;id<64;++id){const auto u=word(Battle+0x20+4*id);if(u && (word(u+0x1c)&0x80000000))return true;}return false;}
    void checkBrokenUnits() {
        // Original Main checks broken units before pad/action services. Native
        // units/parts are preallocated; retired bytes remain for diagnostics.
        for(unsigned id=0;id<64;++id) {
            const auto u=word(Battle+0x20+4*id);if(!u || !(word(u+0x1c)&0x80000000))continue;
            for(unsigned offset:{0x290u,0x29cu,0x2b4u}){const auto event=word(u+offset);if(event)deleteEventId(event,vm.creationServices());word(u+offset,0);}
            for(unsigned i=0;i<64;++i)if(word(Battle+0x120+4*i)==word(u))word(Battle+0x120+4*i,0xffffffff);
            if((word(u+0x1c)&0x40000000) && int8_t(byte(u+0xe))>=0){const auto setup=word(word(Battle+0x2738)+0xc);word(setup+0x5c+4*byte(u+0xe),word(u+0x308));}
            // No per-unit external effect/pose/extension allocations are made
            // by this bridge. Shared immutable animation packs remain loaded.
            word(Battle+0x20+4*id,0); // Source registry publication is last.
        }
    }
    void readPad(uint32_t base,BattlePadWork& pad) {
        auto snapshot=[&](unsigned o,BattlePadSnapshot& p){p.buttons=uint16_t(half(base+o));p.mainX=int8_t(byte(base+o+2));p.mainY=int8_t(byte(base+o+3));p.subX=int8_t(byte(base+o+4));p.subY=int8_t(byte(base+o+5));p.leftTrigger=byte(base+o+6);p.rightTrigger=byte(base+o+7);p.analogA=byte(base+o+8);p.analogB=byte(base+o+9);p.retainedA=byte(base+o+10);p.retainedB=byte(base+o+11);};
        snapshot(0,pad.current);snapshot(12,pad.previous);
        for(unsigned i=0;i<30;++i){pad.triggers.entries[i]=word(base+0x18+4*i);pad.repeats.entries[i]=word(base+0x90+4*i);pad.held.entries[i]=word(base+0x108+4*i);pad.released[i]=word(base+0x180+4*i);}
        pad.repeatPhase=byte(base+0x1f8);pad.repeatCounter=byte(base+0x1f9);
    }
    void writePad(uint32_t base,const BattlePadWork& pad) {
        auto snapshot=[&](unsigned o,const BattlePadSnapshot& p){half(base+o,p.buttons);byte(base+o+2,uint8_t(p.mainX));byte(base+o+3,uint8_t(p.mainY));byte(base+o+4,uint8_t(p.subX));byte(base+o+5,uint8_t(p.subY));byte(base+o+6,p.leftTrigger);byte(base+o+7,p.rightTrigger);byte(base+o+8,p.analogA);byte(base+o+9,p.analogB);byte(base+o+10,p.retainedA);byte(base+o+11,p.retainedB);};
        snapshot(0,pad.current);snapshot(12,pad.previous);
        for(unsigned i=0;i<30;++i){word(base+0x18+4*i,pad.triggers.entries[i]);word(base+0x90+4*i,pad.repeats.entries[i]);word(base+0x108+4*i,pad.held.entries[i]);word(base+0x180+4*i,pad.released[i]);}
        byte(base+0x1f8,pad.repeatPhase);byte(base+0x1f9,pad.repeatCounter);
    }
    HammerCommand hammer() const {
        HammerCommand c;c.state=int32_t(word(Battle+0x1c9c));c.count=int32_t(word(Battle+0x1cb4));c.result=word(Battle+0x1cb8);c.mode=word(Battle+0x1cc4);
        c.chargeSteps=int32_t(word(Battle+0x1cc8));c.period=int32_t(word(Battle+0x1ccc));c.suppression=int32_t(word(Battle+0x1cd0));c.suppressionLatch=int32_t(word(Battle+0x1cd4));c.finalSound=word(Battle+0x1cd8)!=0;
        c.enteredCharge=word(Battle+0x1ce8)!=0;c.stepCount=int32_t(word(Battle+0x1cec));c.classification=int32_t(word(Battle+0x1cf0));c.specialDelay=int32_t(word(Battle+0x1c98));c.forcedCounter=half(Battle+0x1d18);
        for(unsigned i=0;i<11;++i)c.scratch[i]=word(Battle+0x1f20+4*i);
        c.countdown=int32_t(word(Battle+0x1f4c));c.stage=int32_t(word(Battle+0x1f50));c.finishCountdown=int32_t(word(Battle+0x1f54));for(unsigned i=0;i<12;++i)c.handles[i]=int32_t(word(Battle+0x1f58+4*i));return c;
    }
    void storeHammer(const HammerCommand& c) {
        word(Battle+0x1c9c,c.state);word(Battle+0x1cb4,c.count);word(Battle+0x1cb8,c.result);word(Battle+0x1cc4,c.mode);word(Battle+0x1cc8,c.chargeSteps);word(Battle+0x1ccc,c.period);
        word(Battle+0x1cd0,c.suppression);word(Battle+0x1cd4,c.suppressionLatch);word(Battle+0x1cd8,c.finalSound);word(Battle+0x1ce8,c.enteredCharge);word(Battle+0x1cec,c.stepCount);word(Battle+0x1cf0,c.classification);word(Battle+0x1c98,c.specialDelay);half(Battle+0x1d18,c.forcedCounter);
        for(unsigned i=0;i<11;++i)word(Battle+0x1f20+4*i,c.scratch[i]);
        word(Battle+0x1f4c,c.countdown);word(Battle+0x1f50,c.stage);word(Battle+0x1f54,c.finishCountdown);for(unsigned i=0;i<12;++i)word(Battle+0x1f58+4*i,c.handles[i]);
        if(!c.mainLive)for(unsigned o:{0x1ca0u,0x1ca8u,0x1ca4u,0x1cacu})word(Battle+o,0);
    }
    void actionManager() {
        if(word(Battle+0x1ca0)==0x8012aae0){auto c=timing();const int result=c.tick();storeTiming(c);if(!result)for(unsigned o:{0x1ca0u,0x1ca4u,0x1ca8u,0x1cacu})word(Battle+o,0);}
        else if(word(Battle+0x1ca0)==0x8012a17c){auto c=hammer();const auto u=word(Battle+0x1c90);HammerContext context;context.held=word(Battle+0x1034);context.setupOptions=word(Battle+0x1c94);context.actorFlags=word(u+0x27c);context.automatic=byte(u+0x307)!=0;context.difficulty=byte(Battle+0x1cbd);
            HammerSoundSink sounds;sounds.user=this;sounds.play=[](void* p,const char* key)->int32_t{static_cast<TrioAttackRuntime*>(p)->soundRequests.emplace_back(key);return -1;};const auto result=c.tick(context,random,sounds);if(result<0){if(!pending)pending=0x8012a17c;return;}storeHammer(c);if(result==0)word(Battle+0x1ca0,0);
        }
    }
    bool displayPrefix() {
        (void)word(0x8041eb90); // Original retained B; footer is still pending.
        const auto actors=nativeBattleActorServices(vm);
        if(!displayActive){displayActive=true;displayIndex=0;displayUnitPrefixDone=false;displayFooter=0;stockExpMutationDone=false;stageRegistration=0;stageGetterDone=false;}
        for(;displayIndex<64;++displayIndex) {
            const auto unit=battleRegistryUnit(word(0x8041eb90),displayIndex,actors);if(!unit)continue;
            if(!displayUnitPrefixDone){
            if(word(unit+8)==223){pending=0x801024f0;return false;}
            const auto old=byte(unit+0x312);endingRequest(0x801277ac,{int32_t(unit),2});word(unit+0x1c,word(unit+0x1c)&~2u);
            const auto target=byte(unit+0x311);
            if(target!=old){endingRequest(0x801277bc,{int32_t(unit),2});word(unit+0x1c,word(unit+0x1c)|2);byte(unit+0x312,uint8_t(target>old?std::min<int>(old+12,target):std::max<int>(old-2,target)));}
            endingRequest(0x80125d88,{int32_t(unit)});
            BattleHpGaugeServices gauge;gauge.actors=actors;gauge.user=this;
            gauge.interpolate=[](void* p,int32_t mode,int32_t step,int32_t duration,float start,float end)->std::optional<float>{auto& r=*static_cast<TrioAttackRuntime*>(p);r.endingRequest(0x80006c8c,{mode,step,duration});return battleHpGaugeCurve(mode,step,duration,start,end);};
            if(!packedBattleHpGaugeMain(unit,gauge)){pending=0x80125d88;return false;}
            word(unit+0x114,word(0x8042224c));
            for(const auto id:{11,10}) {
                endingRequest(0x80127f34,{int32_t(unit),id});const auto value=getPackedBattleStatus(unit,id,actors);const auto strength=int8_t(value.strength);
                if(int8_t(value.duration)>0 && (id==11?strength<0:strength>0)) {
                    const auto factor=id==11?std::max(real(0x80422234),std::fma(real(0x80422234),float(strength),real(0x8042224c))):std::min(real(0x80422250),real(0x8042224c)+float(strength));
                    real(unit+0x114,real(unit+0x114)*factor);
                }
            }
            endingRequest(0x801966f0,{int32_t(unit)});
            constexpr unsigned status[]{1,6,8,9,0},handle[]{0x324,0x330,0x328,0x32c,0x334};
            constexpr uint32_t leaf[]{0x80196538,0x801963ac,0x80196240,0x801960ac,0x80195f8c};
            constexpr uint32_t remove[]{0x801964f8,0x8019636c,0x80196200,0x8019606c,0x80195f48};
            for(unsigned i=0;i<5;++i) {
                endingRequest(leaf[i],{int32_t(unit)});
                const bool active=i==4?((word(word(0x8041eb90)+0xef8)&1) && (word(unit+0x104)&0x10)):checkPackedBattleStatus(unit,status[i],actors);
                if(active){pending=leaf[i];return false;}
                endingRequest(remove[i],{int32_t(unit)});
                if(const auto oldHandle=word(unit+handle[i])){pending=i==4?0x8001ae08:i==3?0x8005e9c0:0x8005ea30;endingRequest(pending,{int32_t(oldHandle+(i==4?0x18:0))});return false;}
            }
            endingRequest(0x80253ca8,{int32_t(unit)});half(unit+0xadc,0);
            const auto current=word(0x8041eb90);
            if((word(current+0xef4)&0x10000) || (word(current+0xef8)&1)){pending=0x80253ca8;return false;}
            displayPart=word(unit+0x14);displayUnitPrefixDone=true;
            }
            for(auto part=displayPart;part;part=word(part)) {
                displayPart=part;
                const auto binding=partPoseHandles.find(part);
                if(binding==partPoseHandles.end()){pending=0x801011fc;endingRequest(pending,{int32_t(unit)});return false;}
                if(word(part+0x1c0)==0xffffffff)continue;
                {
                    uint32_t selectedName=part+0x1c4;
                    float rate=real(0x8042224c);
                    if(!(word(part+0x204)&2)){
                        endingRequest(0x80103084,{int32_t(unit)});
                        int32_t type=27;
                        if(!(word(unit+0x104)&0x20000)){
                            type=28;
                            for(uint32_t address=0x80416ab0;;address+=4){
                                const auto candidate=int32_t(word(address));if(candidate==-1)break;
                                endingRequest(0x80127814,{int32_t(unit),int8_t(candidate)});
                                if(checkPackedBattleStatus(unit,int8_t(candidate),actors)){type=candidate;break;}
                            }
                        }
                        const auto search=[&](int32_t requested){
                            endingRequest(0x80103cd0,{int32_t(word(part+0x1bc)),requested});
                            auto table=word(part+0x1bc);
                            if(table)for(unsigned index=0;index<28;++index,table+=8){const auto tag=int32_t(word(table));if(tag==requested || tag==28)return word(table+4);}
                            return uint32_t(0);
                        };
                        const auto kind=int32_t(word(unit+8));
                        if(kind>=1 && kind<216)selectedName=search(type);
                        else{
                            bool specialized=false;
                            for(const auto choice:{std::pair<uint32_t,int32_t>{0x20000000,57},{0x10000000,56}}){
                                if(type!=28)break;
                                endingRequest(0x80127814,{int32_t(unit),22});
                                if(!checkPackedBattleStatus(unit,22,actors) && (word(unit+0x138)&choice.first)){
                                    endingRequest(0x8012772c,{int32_t(part),choice.second});
                                    selectedName=packedBattlePoseName(part,choice.second,actors.memory);
                                    if(!selectedName)selectedName=search(28);specialized=true;break;
                                }
                            }
                            if(!specialized)selectedName=search(type);
                            endingRequest(0x801277f8,{int32_t(unit),0x01000000});
                            if(word(unit+0x138)&0x01000000){
                                endingRequest(0x8012724c,{int32_t(unit)});
                                if(packedCanAct(unit,actors)){
                                    endingRequest(0x8012772c,{int32_t(part),45});selectedName=packedBattlePoseName(part,45,actors.memory);
                                }
                            }
                        }
                        endingRequest(0x80127814,{int32_t(unit),19});
                        if(checkPackedBattleStatus(unit,19,actors))rate=real(0x80422250);
                        else{endingRequest(0x80127814,{int32_t(unit),20});if(checkPackedBattleStatus(unit,20,actors))rate=real(0x80422234);}
                    }
                    endingRequest(0x80100a4c,{int32_t(part)});real(part+0x208,rate);
                    const auto handle=int32_t(word(part+0x1c0));endingRequest(0x8004142c,{handle});
                    if(handle!=binding->second || !poses.setRate(handle,real(part+0x208)*real(part+0x20c))){pending=0x8004142c;return false;}
                    if(word(part+0x204)&1){
                        word(part+0x204,word(part+0x204)&~1u);word(part+0x204,word(part+0x204)&~0x200u);
                        const bool force=word(part+0x204)!=0;endingRequest(0x80041170,{handle,int32_t(selectedName),force?1:0});
                        if(!selectedName || !poses.selectName(handle,text(selectedName),force,{now,now})){pending=0x80041170;return false;}
                        byte(part+0x214,0);bool effect=false;
                        const auto transparent=[&](){endingRequest(0x80127814,{int32_t(unit),18});return checkPackedBattleStatus(unit,18,actors) || byte(part+0x4f3)!=255;};
                        if(word(part+0x204)&4)byte(part+0x215,0);else{byte(part+0x215,1);if(transparent()){byte(part+0x215,7);effect=true;}}
                        const auto flags=word(part+0x204);
                        if(!(flags&2) || (flags&8) || (flags&0x80)){
                            byte(part+0x215,1);if(transparent()){byte(part+0x215,7);effect=true;}
                            endingRequest(0x80127814,{int32_t(unit),7});if(checkPackedBattleStatus(unit,7,actors)){byte(part+0x215,4);effect=true;}
                        }
                        if(effect && (word(part+0x1ac)&0x8000)){effect=false;byte(part+0x215,0);}
                        if(!(word(part+0x204)&4)){
                            const auto old=word(part+0x204);if(effect && !(old&0x40))word(part+0x204,old|0x40);else if(!effect && (old&0x40))word(part+0x204,old&~0x40u);
                        }
                    }
                    endingRequest(0x80101060,{int32_t(part)});
                    const auto owner=word(part+0x4ec);
                    const bool gravity=(word(word(0x8041eb90)+0xef4)&0x40000000) && !(word(owner+0x104)&0x1004);
                    const auto settle=[&](unsigned offset,unsigned period,bool active){
                        auto value=uint16_t(half(part+offset));if(active){half(part+offset,uint16_t(value+1));return;}
                        if(!value)return;
                        half(part+offset,uint16_t(value%period));value=uint16_t(half(part+offset));
                        if(value>=period/2)half(part+offset,uint16_t(value+1));else if(value)half(part+offset,uint16_t(value-1));
                    };
                    settle(0x1a6,120,gravity);
                    endingRequest(0x80100f34,{int32_t(part)});
                    bool floating=(word(owner+0x1c)&0x100000) && (word(owner+0x1c)&0x200000);
                    if(floating){endingRequest(0x80127814,{int32_t(owner),27});floating=!checkPackedBattleStatus(owner,27,actors);}
                    settle(0x1a8,90,floating);
                    endingRequest(0x80100e6c,{int32_t(part)});
                    const auto mode=byte(part+0x215);
                    if(mode==2 || mode==3 || mode==7){pending=0x80100d28;endingRequest(pending,{int32_t(part)});return false;}
                    if(mode==4 || mode==5 || mode==6){pending=0x80100a80;endingRequest(pending,{int32_t(part)});return false;}
                    if(mode){
                        endingRequest(0x80100e00,{int32_t(part)});
                        if(!byte(part+0x214)){
                            byte(part+0x214,1);endingRequest(0x80040a28,{handle,0,0});
                            if(!poses.clearEffect(handle)){pending=0x80040a28;return false;}
                            if(word(word(part+0x4ec)+0x1c)&0x01000000){endingRequest(0x800406f0,{handle,8});poses.pose(handle)->word(0xec,poses.pose(handle)->word(0xec)|8);}
                        }
                    }
                    endingRequest(0x80128b10,{int32_t(part)});
                    BattlePartPositionServices position;position.memory=actors;
                    position.fma=[](void*,float a,float b,float c){return std::fma(a,b,c);};position.add=[](void*,float a,float b){return a+b;};
                    const auto capturedPosition=packedPartWorldPosition(part,position);
                    endingRequest(0x8003ed2c,{handle});
                    if(!poses.main(handle,{now,now})){pending=0x8003ed2c;return false;}
                    auto depth=capturedPosition;depth[2]+=real(part+0x4f8);
                    byte(part+0x4f7,uint8_t(int16_t(half(word(part+4)+0x34))));
                    endingRequest(0x80127814,{int32_t(unit),18});
                    if(checkPackedBattleStatus(unit,18,actors) || (word(unit+0x104)&8))byte(part+0x4f7,uint8_t((int32_t(byte(part+0x4f7))*128)/255));
                    byte(part+0x4f7,uint8_t((int32_t(byte(part+0x4f7))*int32_t(byte(part+0x4f3)))/255));
                    for(unsigned channel=0;channel<3;++channel)byte(part+0x4f4+channel,byte(part+0x4f0+channel));
                    if((word(unit+0x104)&0x01000000) || (word(part+0x1ac)&0x01000000) || ((word(part+0x1ac)&1) && (word(unit+0x104)&0x10))){if(!completePartShadow(part))return false;continue;}
                    int32_t drawClass=2;
                    if(byte(part+0x4f7)==255){
                        endingRequest(0x80039c0c,{handle});const auto translucent=poses.testTranslucent(handle);
                        if(!translucent){pending=0x80039c0c;return false;}drawClass=*translucent?2:1;
                    }
                    if(word(part+0x204)&0x100){if(!completePartShadow(part))return false;continue;}
                    waitingPosePriority.reset();
                    poseDraw=TrioPoseDrawRequest{part,4,drawClass,depth};
                    endingRequest(0x8001086c);if(!registerPoseDraw())return false;continue;
                }
            }
            displayPart=0;
            uint32_t callback=0;
            if(word(unit+0x308)){
                const auto flags=word(unit+0x1c);
                if((flags&0x40000000) && !(flags&0x08000000))callback=0x80101548;
                else if(!((flags&0x04000000) && !(flags&0x02000000)) && !(flags&0x08000000))callback=0x80101678;
            }else if(word(Battle+0xef4)&0x20000)callback=0x801015ec;
            if(callback){
                endingRequest(0x80010bc4,{4,2,int32_t(callback),int32_t(unit)});
                if(displayQueue.enter(4,2,callback,unit,real(0x80422240))==DisplayAdmission::Unavailable){pending=0x80010bc4;return false;}
            }
            displayUnitPrefixDone=false;
        }
        if(displayFooter==0){
            endingRequest(0x8010045c);
            const auto current=word(0x8041eb90);
            if(!stockExpMutationDone){
                if(int32_t(word(current+0xf00))>int32_t(word(current+0xf04))){
                    word(current+0xf08,word(current+0xf08)+1u);
                    if(int32_t(word(current+0xf08))>=4)word(current+0xf04,word(current+0xf04)+1u);
                }
                stockExpMutationDone=true;
            }
            if(word(current+0xef8)&2){
                endingRequest(0x80010bc4,{8,1,int32_t(0x80100258),0});
                if(displayQueue.enter(8,1,0x80100258,0,real(0x80422238))==DisplayAdmission::Unavailable){pending=0x80010bc4;return false;}
            }
            displayFooter=1;
        }
        if(displayFooter==1){
            endingRequest(0x80139084);
            if(!stageGetterDone){
                const auto current=word(0x8041eb90);const auto id=int32_t(word(current+0x420));
                endingRequest(0x800f88cc,{int32_t(current),id});
                (void)battleRegistryUnit(current,id,actors);stageGetterDone=true;
            }
            constexpr uint32_t callbacks[]{0x80138da8,0x801389a0};
            constexpr float priorities[]{5000,6000};
            for(;stageRegistration<2;++stageRegistration){
                const auto callback=callbacks[stageRegistration];endingRequest(0x80010bc4,{4,2,int32_t(callback),0});
                if(displayQueue.enter(4,2,callback,0,priorities[stageRegistration])==DisplayAdmission::Unavailable){pending=0x80010bc4;return false;}
            }
            displayFooter=2;
        }
        endingRequest(0x80122870,{int32_t(Battle)});
        pending=0x80119e24;endingRequest(pending);return false; // Actual palette cycle owner precedes command queues.
    }
    void completionPass(const BattlePadSample& sample) {
        struct Context {TrioAttackRuntime& r;const BattlePadSample& sample;} context{*this,sample};
        BattleMainServices services;services.actors=nativeBattleActorServices(vm);services.user=&context;
        services.request=[](void* p,uint32_t fn,uint32_t captured) {
            auto& c=*static_cast<Context*>(p);auto& r=c.r;r.endingRequest(fn);
            // This native runtime owns one stable BattleWork. An owner swap
            // requires its real allocator/lifetime binding, never aliasing B.
            if(captured!=Battle){r.pending=fn;return false;}
            if(fn==0x800f7b28){r.checkBrokenUnits();return !r.pending;}
            if(fn==0x8011a43c) {
                for(unsigned channel=0;channel<4;++channel) {
                    const auto b=r.word(0x8041eb90);BattlePadWork pad;
                    const auto address=b+0xf2c+0x1fc*channel;r.readPad(address,pad);
                    // Only physical channel0 is bound on the 3DS. Other native
                    // channels have explicit inactive samples, not GC evidence.
                    pad.update(channel==0?c.sample:BattlePadSample{});r.writePad(address,pad);
                    if(channel==0){r.generalPad=pad;r.history=pad.triggers;}
                }
                return true;
            }
            if(fn==0x8011f1ec) {
                const auto b=r.word(0x8041eb90);
                if(r.word(b+4)!=4){r.pending=fn;return false;}
                BattleSequenceManagerServices manager;manager.memory=nativeBattleActorServices(r.vm).memory;
                packedBattleSequenceManager(b,manager); // Source bank4 default, no dispatch.
                return true;
            }
            if(fn==0x800fa5b0) {
                r.readPad(captured+0x1d1c,r.privatePad);r.privatePad.update(c.sample);
                r.writePad(captured+0x1d1c,r.privatePad);r.actionManager();return !r.pending;
            }
            if(fn==0x80149798){if(r.updateStageObjects())return true;r.pending=fn;return false;}
            if(fn==0x801aa168) {
                for(unsigned i=0;i<16;++i) {
                    const auto icon=Battle+0x182cc+0x9c*i;
                    if((r.half(icon)&1) && r.word(icon+0x1c)){r.pending=fn;return false;}
                }
                return true; // Actual live inactive bank; never cleared to pass this gate.
            }
            if(fn==0x80139150)return r.updateStageWork();
            if(fn==0x802168a4) {
                BattleActionHelpServices help;help.actors=nativeBattleActorServices(r.vm);help.user=&r;
                help.display=[](void* p,int32_t,int32_t,uint32_t,uint32_t,float){auto& runtime=*static_cast<TrioAttackRuntime*>(p);runtime.endingRequest(0x80010bc4);runtime.pending=0x80010bc4;return false;};
                return packedBattleActionHelpMain(help);
            }
            if(fn==0x801a836c)return r.updateAudience();
            if(fn==0x80203aa0){if(packedBattleBreakSlotIdleMain(nativeBattleActorServices(r.vm)))return true;r.pending=fn;return false;}
            if(fn==0x801024f0)return r.displayPrefix();
            r.pending=fn;return false; // Real stage/map/render/audio providers remain required.
        };
        pending=0;
        (void)packedBattleMainWrapper(services);
    }
    void updateCamera(uint64_t time) {
        if(cameraClockAt==time)return;cameraClockAt=time;
        BattleCameraFrameServices camera;camera.requests.positions=nativeBattlePartPositionServices(vm);camera.user=this;
        camera.interpolate=[](void*,int32_t curve,int32_t step,int32_t duration,float start,float end){return cameraInterpolation(curve,step,duration,start,end);};
        camera.rumbleOff=[](void*,int32_t){}; // New2DS XL has no rumble motor; native hardware adapter.
        if(!updateBattleCameraFrame(Battle,cameraFrame,camera))cameraHistoryComplete=false;
        if(cameraDepthLensAdapter){
            const auto& lens=*cameraDepthLensAdapter;
            cameraDepth=makeCameraDepthSnapshot(cameraFrame.eye,cameraFrame.at,cameraFrame.up,lens[0],lens[1],lens[2],lens[3]);
        }
    }
    void tick(uint64_t time,ProfessorStory& s,uint32_t triggers,const BattlePadSample* physical){
        scene=&s;now=time;
        if(waitingPoseDepth && (waitingPosePriority || cameraDepth)){
            pending=0;
            if(registerPoseDraw()){displayPart=word(poseDraw->part);displayPrefix();}
        }
        if(pending==0x800f8e28 && sampledAt!=time) {
            sampledAt=time;BattlePadSample sample;
            if(physical)sample=*physical;else {sample.snapshot.buttons=uint16_t(triggers);sample.pressed=triggers;}
            vm.gp.timestamp=time*eventClockRate(243000000);
            endingRequest(0x8002db8c);
            if(!shadows || !shadows->main(displayQueue,shadowMainCursor)){pending=0x8002db8c;return;}
            completionPass(sample);
        }
        if(!pending){if(sampledAt!=time){sampledAt=time;checkBrokenUnits();BattlePadSample sample;if(physical)sample=*physical;else {sample.snapshot.buttons=uint16_t(triggers);sample.pressed=triggers;}readPad(Battle+0xf2c,generalPad);generalPad.update(sample);writePad(Battle+0xf2c,generalPad);history=generalPad.triggers;vm.gp.timestamp=time*eventClockRate(243000000);if(word(Battle+4)==3)ending();else if(!done){if(word(Battle+0xf20)<0x06000007)enemyAction();else postAction();}else handoff();readPad(Battle+0x1d1c,privatePad);privatePad.update(sample);writePad(Battle+0x1d1c,privatePad);actionManager();if(!updateStageObjects() && !pending)pending=0x80149798;}updateCamera(time);vm.runManager();if(!pending && gaugeClockAt!=time){gaugeClockAt=time;BattleHpGaugeServices gauge;gauge.actors=nativeBattleActorServices(vm);gauge.interpolate=[](void*,int32_t mode,int32_t step,int32_t duration,float start,float end)->std::optional<float>{return battleHpGaugeCurve(mode,step,duration,start,end);};for(int32_t id=0;id<64;++id){const auto u=battleRegistryUnit(word(0x8041eb90),id,gauge.actors);if(u)packedBattleHpGaugeMain(u,gauge);}}if(hasBrokenUnit())done=false;}iconClock(time);if(pointGetClockAt!=time){pointGetClockAt=time;pointGet.tick();}if(endingFadeClockAt!=time){endingFadeClockAt=time;endingFade.tick(60);}stageBackground.recalculate();stageForeground.recalculate();publish(s);
    }
    void publish(ProfessorStory& s){for(unsigned id=1;id<7;++id){const uint32_t u=word(Battle+0x20+4*id);if(!u){s.actors[visual[id]].hidden=true;continue;}for(unsigned axis=0;axis<3;++axis)s.actors[visual[id]].position[axis]=real(u+0x3c+4*axis);s.actors[visual[id]].yaw=byte(u+0x189)==1?90:270;s.actors[visual[id]].battleFlags=word(u+0x1c);for(unsigned axis=0;axis<3;++axis){s.actors[visual[id]].battleRotation[axis]=real(u+0x60+4*axis)+real(u+0x6c+4*axis);s.actors[visual[id]].battlePivot[axis]=real(u+0x78+4*axis);s.actors[visual[id]].battleScale[axis]=(axis<2?real(u+0x114):1.f)*real(u+0x9c+4*axis)*real(u+0x90+4*axis);s.actors[visual[id]].battleDisplayOffset[axis]=real(u+0x54+4*axis);}}}
};
bool TrioAttack::begin(const TrioBattleCommands& menu,ProfessorStory& scene,int16_t hp,std::shared_ptr<ProloguePouch> entryPouch,uint32_t* fieldNpcBattleFlags,FieldMarioBattleState* fieldMario,FieldPartyRegistry* fieldParties,const FieldPartyAppearanceServices* appearance,std::shared_ptr<BattleSpotlightState> spotlight,std::shared_ptr<BattleRandom> random,std::shared_ptr<PositionalSfxRegistry> sounds,std::shared_ptr<ShadowWork> shadows){if(!assetsReady || !menu.attack || !((menu.actor==1 && (menu.attack->weapon==0x803538a8 || menu.attack->weapon==0x80353de8)) || (menu.actor==2 && menu.attack->weapon==0x80393ea8)))return false;if(runtime_)return runtime_->done && runtime_->readyActor==menu.actor && runtime_->launch(menu);auto r=std::make_shared<TrioAttackRuntime>(random?std::move(random):std::make_shared<BattleRandom>(1));r->shadows=shadows?std::move(shadows):std::make_shared<ShadowWork>();r->publicSfx=sounds?std::move(sounds):std::make_shared<PositionalSfxRegistry>();r->spotlight=spotlight?std::move(spotlight):std::make_shared<BattleSpotlightState>();r->fieldNpcBattleFlags=fieldNpcBattleFlags;r->fieldMario=fieldMario;r->fieldParties=fieldParties;if(appearance)r->fieldAppearance=*appearance;if(!r->initialize(menu,scene,hp,std::move(entryPouch)))return false;runtime_=std::move(r);return true;}
void TrioAttack::tick(uint64_t time,ProfessorStory& scene,uint32_t triggers,const BattlePadSample* physical){if(runtime_)runtime_->tick(time,scene,triggers,physical);}
bool TrioAttack::statusWindowVisible()const{return !runtime_ || runtime_->statusWindowOpen;}
bool TrioAttack::active()const{return runtime_ && !runtime_->done;}
bool TrioAttack::finished()const{return runtime_ && runtime_->done;}
int32_t TrioAttack::commandActor()const{if(!runtime_ || runtime_->pending || !runtime_->done || runtime_->word(Battle+0xf18)!=0x04000002)return -1;if(runtime_->readyActor>=0)return runtime_->readyActor;if(runtime_->word(Battle+0xf1c)!=0x05000008)return -1;for(unsigned i=0;i<64;++i){const int32_t id=int32_t(runtime_->word(Battle+0x120+4*i));if(id<0 || id>=64)continue;const auto u=runtime_->word(Battle+0x20+4*id);if(u && int8_t(runtime_->byte(u+0x22))>0 && runtime_->byte(u+0x20)!=10 && runtime_->byte(u+0x20)!=1 && (!runtime_->byte(u+0x135) || (runtime_->word(u+0x104)&0x10000)))return id;}return -1;}
bool TrioAttack::prepareCommands(TrioBattleCommands& menu,const ProfessorStory& scene){if(!runtime_ || !runtime_->done || runtime_->pending || runtime_->word(Battle+0xf18)!=0x04000002 || runtime_->word(Battle+0xf1c)!=0x05000008)return false;runtime_->prepareActor();if(!menu.begin(scene,runtime_->readyActor))return false;std::array<bool,7> present{};for(unsigned id=1;id<7;++id){present[id]=runtime_->word(Battle+0x20+4*id)!=0;runtime_->readDamage(id);}menu.syncUnitState(runtime_->damageUnits,present,&runtime_->damageParts);return true;}
uint32_t TrioAttack::pendingFunction()const{return runtime_?runtime_->pending:0;}
const std::vector<ProfessorServiceRequest>& TrioAttack::requests()const{static const std::vector<ProfessorServiceRequest> empty;return runtime_?runtime_->trace:empty;}
const std::vector<TrioAudienceAudioRequest>& TrioAttack::audienceAudioRequests()const{static const std::vector<TrioAudienceAudioRequest> empty;return runtime_?runtime_->audienceAudioTrace:empty;}
bool TrioAttack::readWord(uint32_t a,uint32_t& v)const{return runtime_ && runtime_->vm.memory.readWord(a,v);}
const BattlePosePool* TrioAttack::battlePosePool()const{return runtime_?&runtime_->poses:nullptr;}
std::optional<TrioPoseDrawRequest> TrioAttack::poseDrawRequest()const{return runtime_?runtime_->poseDraw:std::nullopt;}
void TrioAttack::setCameraDepthLensAdapter(float fov,float aspect,float nearPlane,float farPlane){
    if(!runtime_)return;
    runtime_->cameraDepthLensAdapter=std::array<float,4>{fov,aspect,nearPlane,farPlane};
    const auto& frame=runtime_->cameraFrame;
    runtime_->cameraDepth=makeCameraDepthSnapshot(frame.eye,frame.at,frame.up,fov,aspect,nearPlane,farPlane);
}
void TrioAttack::setCameraDepthSnapshot(std::optional<CameraDepthSnapshot> snapshot){
    if(!runtime_)return;
    runtime_->cameraDepthLensAdapter.reset();runtime_->cameraDepth=std::move(snapshot);
}
const CameraDepthSnapshot* TrioAttack::cameraDepthSnapshot()const{return runtime_ && runtime_->cameraDepth?&*runtime_->cameraDepth:nullptr;}
const DisplayQueue* TrioAttack::displayQueue()const{return runtime_?&runtime_->displayQueue:nullptr;}
std::optional<TrioShadowDrawRequest> TrioAttack::shadowDrawRequest()const{return runtime_?runtime_->shadowDraw:std::nullopt;}
const ShadowWork* TrioAttack::shadowWork()const{return runtime_?runtime_->shadows.get():nullptr;}
const BattleCameraFrame* TrioAttack::cameraFrameState()const{return runtime_?&runtime_->cameraFrame:nullptr;}
bool TrioAttack::cameraHistoryComplete()const{return runtime_ && runtime_->cameraHistoryComplete;}
const BattlePose* TrioAttack::battlePose(uint32_t part)const{
    if(!runtime_)return nullptr;
    const auto binding=runtime_->partPoseHandles.find(part);
    if(binding==runtime_->partPoseHandles.end() || runtime_->word(part+0x1c0)!=uint32_t(binding->second))return nullptr;
    return runtime_->poses.pose(binding->second);
}
}

namespace ttyd {
const PointGetEffect* TrioAttack::rewardEffect() const {return runtime_ && runtime_->pointGet.active?&runtime_->pointGet:nullptr;}
std::shared_ptr<ProloguePouch> TrioAttack::exportedPouch() const {return runtime_ && runtime_->endingExported?runtime_->entryPouch:nullptr;}
const StageMapObjects* TrioAttack::stageMap(bool background)const{return runtime_?(background?&runtime_->stageBackground:&runtime_->stageForeground):nullptr;}
uint8_t TrioAttack::endingFadeAlpha() const {return runtime_?runtime_->endingFade.alpha():0;}
bool TrioAttack::rewardPresentationActive() const {return runtime_ && runtime_->word(Battle+4)==3 && runtime_->word(Battle+0xf24)>=0x07000008;}
const std::array<TrioRewardIcon,19>* TrioAttack::rewardIcons() const {if(!runtime_)return nullptr;const auto end=runtime_->word(Battle+0xf28);return end && (runtime_->word(end+0x30)&2)?&runtime_->rewardIcons:nullptr;}
}
