#include "harbor_encounter.hpp"
#include "harbor_story.hpp"
#include "message_assets.hpp"
#include "event_native_runtime.hpp"
#include "event_memory_bootstrap.hpp"
#include "harbor_tutorial.hpp"
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <unordered_map>
namespace ttyd {
namespace {
EventScriptAssets scripts;MessageAssets messages;MessageFont font;uint32_t script=0;
std::array<uint32_t,4> names{};std::unordered_map<uint32_t,uint32_t> callbacks;bool ready=false;
// Preserve a guest word's two's-complement sign without an out-of-range
// uint32_t-to-int32_t conversion; event pointers and negative operand tags share it.
int32_t signedWord(uint32_t v){return v<=0x7fffffff?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}
}
// PMHBR001 stores native little-endian u32 entry, four actor-name addresses,
// callback count, then (source function address, converter service index) pairs.
// Header8/word4 are pack widths; <=24 services mirrors harbor_event_assets.py.
// Duplicate/null callbacks, trailing bytes, bad entry or US022..024 text fail
// before publication. Numeric tags below must stay aligned with that converter.
bool loadHarborEncounterAssets(const char* root) {
    if(!root)return false;
    const std::string base=root;
    EventScriptAssets p;MessageAssets m;MessageFont f;
    if(!p.load((base+"/harbor-events.pmevt").c_str()) || !m.loadGlobal((base+"/messages/global.txt").c_str()) ||
       !m.loadRoom((base+"/messages/gor_00.txt").c_str()) || !f.load((base+"/font.pmfont").c_str()))return false;
    FILE* file=std::fopen((base+"/harbor-program.bin").c_str(),"rb");if(!file)return false;
    char magic[8];uint32_t entry=0,count=0;std::array<uint32_t,4> keys{};
    bool ok=std::fread(magic,8,1,file)==1 && !std::memcmp(magic,"PMHBR001",8) && std::fread(&entry,4,1,file)==1 &&
        std::fread(keys.data(),4,4,file)==4 && std::fread(&count,4,1,file)==1 && count>0 && count<=24;
    std::unordered_map<uint32_t,uint32_t> table;
    for(uint32_t i=0;ok && i<count;++i){uint32_t a=0,t=0;ok=std::fread(&a,4,1,file)==1 && std::fread(&t,4,1,file)==1 && a && t<24 && table.emplace(a,t).second;}
    uint32_t word=0;ok=ok && std::fgetc(file)==EOF && p.readWord(entry,word);std::fclose(file);
    for(int id=22;ok && id<=24;++id){char key[24];std::snprintf(key,sizeof(key),"mac_0_%03d",id);MessagePlayback probe;ok=probe.open(key,m.find(key),f,0);}
    if(!ok)return false;
    scripts=std::move(p);messages=std::move(m);font=std::move(f);script=entry;names=keys;callbacks=std::move(table);ready=true;return true;
}
struct HarborEncounterRuntime {
    HarborEncounter* scene=nullptr;std::optional<float> floor;
    std::shared_ptr<ProloguePouch> pouch;
    struct Move {std::array<float,3> start{},target{};uint64_t begin=0,end=0;bool active=false;};
    std::array<Move,5> moves{};FieldScriptedJump jump;bool jumpLive=false;
    std::unordered_map<uint32_t,uint64_t> waits;
    std::unordered_map<uint32_t,std::unique_ptr<NativeFieldNpcBattleScriptCall>> battleCalls;
    std::vector<std::unique_ptr<NativeFieldNpcBattleScriptCall>> retiredBattleCalls;
    uint32_t retiredBattlePending=0;
    EventNativeRuntime vm;
    std::unique_ptr<NativeHarborEncounterField> nativeField;
    bool nativeResume=false,nativeRootStarted=false;
    // Authored guest GP0x81000000; clock callback is the GameCube243MHz rate,
    // not handheld wall time. GP timestamp converts caller ms via eventClockRate.
    // Arena is CPU-owned: guest flush/system-call hooks have no GPU consumer.
    HarborEncounterRuntime():vm(0x81000000,platform(this)){}
    static EventNativePlatform platform(HarborEncounterRuntime* r) {
        EventNativePlatform p;p.user=r;p.clock=[](void*)->uint32_t{return 243000000;};
        p.truncate=[](void*,float v)->int32_t{assert(std::isfinite(v) && double(v)>=-2147483648. && double(v)<2147483648.);return int32_t(v);};
        p.divideWord=[](void*,int32_t a,int32_t b){assert(b);return a/b;};
        p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){};
        p.call=[](void* p,EventEntry& e,uint32_t fn,int32_t first){return static_cast<HarborEncounterRuntime*>(p)->call(e,fn,first!=0);};
        p.retiring=[](void* p,const EventEntry& e){auto& r=*static_cast<HarborEncounterRuntime*>(p);if(r.nativeField)r.nativeField->retire(r.scene,e);auto i=r.battleCalls.find(e.id);if(i==r.battleCalls.end())return;
            if(!i->second->releaseForRetirement()){r.retiredBattlePending=i->second->pending();r.retiredBattleCalls.push_back(std::move(i->second));}
            r.battleCalls.erase(i);
        };return p;
    }
    // Arguments are guest32-bit words at stride4. Strings cap256 bytes including
    // the required terminator; unknown actor names fail rather than aliasing.
    // NPC names map four source identities to slots1..4 (Mario is slot0).
    int32_t operand(EventEntry& e,unsigned i){uint32_t v=0;if(i>=e.argumentCount || !vm.memory.readWord(e.arguments+4*i,v)){scene->failed=true;return 0;}return signedWord(v);}
    int32_t value(EventEntry& e,unsigned i){return getEventValue(e,operand(e,i),vm.interpreterServices().operands);}
    float number(EventEntry& e,unsigned i){return getEventFloat(e,operand(e,i),vm.interpreterServices().operands);}
    std::string string(uint32_t a){std::string result;for(unsigned i=0;i<256;++i){uint8_t c=0;if(!vm.memory.readByte(a+i,c)){scene->failed=true;return {};}if(!c)return result;result.push_back(char(c));}scene->failed=true;return {};}
    unsigned npc(EventEntry& e,unsigned i){const auto a=uint32_t(value(e,i));for(unsigned n=0;n<4;++n)if(names[n]==a)return n+1;scene->failed=true;return 1;}
    void outPosition(EventEntry& e,unsigned i,unsigned actor){for(unsigned k=0;k<3;++k)setEventFloat(e,operand(e,i+k),scene->actors[actor].position[k],vm.interpreterServices().operands);}
    // Return0 retains the current VM command;2 advances immediately. Per-entry
    // deadlines initialize only on first callback admission, not each resume.
    int delay(EventEntry& e,bool first,uint64_t duration){if(first)waits[e.id]=scene->now+duration;if(scene->now<waits[e.id])return 0;waits.erase(e.id);return 2;}
    // Native speed bridge: (distance+1)/rate seconds, truncated to ms with a
    // minimum1ms; +1 is existing adapter policy, not measured source timing.
    // Mario rate>60 chooses run; 180/pi converts atan2 to retained degree yaw.
    // NPC auto-poses are owned outside this movement callback.
    int move(EventEntry& e,bool first,unsigned actor,unsigned x,unsigned z,unsigned speed){
        auto& m=moves[actor];auto& a=scene->actors[actor];
        if(first){m.start=a.position;m.target={number(e,x),a.position[1],number(e,z)};
            const float rate=number(e,speed);if(!(rate>0)){scene->failed=true;return 0;}
            // Native movement backend adaptation. The original event owns the
            // resolved operands, blocking continuation and independent inlines.
            const float distance=std::hypot(m.target[0]-m.start[0],m.target[2]-m.start[2]);
            m.begin=scene->now;m.end=m.begin+std::max<uint64_t>(1,uint64_t((distance+1)/rate*1000));m.active=true;a.moving=true;a.speed=rate;
            if(actor==0){a.poseOverride=false;a.pose=rate>60?"M_R_1":"M_W_1";a.poseAt=scene->now;}
            if(actor==0 && distance>0)a.yaw=npcReviseAngle(std::atan2(m.target[0]-m.start[0],m.start[2]-m.target[2])*57.29577951308232f);
        }
        return m.active?0:2;
    }
    // Run before VM dispatch; Mario Y remains owned by field/jump support,
    // while NPC movement interpolates all axes. Completion releases the callback
    // on the same visit's manager pass and restores Mario's ordinary stay pose.
    void advance(){for(unsigned i=0;i<5;++i){auto& m=moves[i];if(!m.active)continue;
        const float t=std::min(1.f,float(scene->now-m.begin)/float(m.end-m.begin));
        for(unsigned k=0;k<3;++k)if(i!=0 || k!=1)scene->actors[i].position[k]=m.start[k]+(m.target[k]-m.start[k])*t;
        if(t==1){m.active=false;auto& a=scene->actors[i];a.moving=false;a.speed=0;
            if(i==0){a.pose="M_S_1";a.poseAt=scene->now;}}
    }}
    int32_t call(EventEntry& e,uint32_t function,bool first){
        if(nativeField){
            const auto result=nativeField->handles(function)?nativeField->invoke(scene,function,e,first):nativeField->external(scene,function,e,first);
            scene->pendingService=result?0:nativeField->pending();return result?*result:0;
        }
        const auto found=callbacks.find(function);if(found==callbacks.end()){scene->failed=true;return 0;}
        if(first){HarborEncounterRequest r{function,e.currentCommand,scene->now,{}};for(unsigned i=0;i<e.argumentCount;++i)r.operands.push_back(operand(e,i));scene->requests.push_back(std::move(r));}
        // Converter indices, not original event opcodes:0 key-depth,1 control,
        //2/3 Mario pos/yaw,4/5 NPC pos/yaw,6 NPC flags,7 Mario getpos,
        //8 environment,9 Mario move,10 reaction,11 Mario pose,12 NPC move,
        //13 print,14 camera,15 Mario display flags,16 SFX,17 jump,18/19 HP,
        //20 battle info,21 battle start,22 wait end,23 registry query.
        // First-admission trace stores RAW operand words, not resolved values.
        switch(found->second){
        case 0:if(value(e,0)==0)++scene->keyDepth;else --scene->keyDepth;return 2;
        case 1:scene->controlEnabled=value(e,0)!=0;return 2;
        case 2:for(unsigned i=0;i<3;++i)scene->actors[0].position[i]=number(e,i);return 2;
        case 3:if(first)scene->actors[0].yaw=number(e,0);return delay(e,first,uint64_t(std::max<int32_t>(0,value(e,1))));
        case 4:for(unsigned i=0;i<3;++i)scene->actors[npc(e,0)].position[i]=number(e,i+1);return 2;
        case 5:scene->actors[npc(e,0)].yaw=number(e,1);return 2;
        case 6:{auto& a=scene->actors[npc(e,1)];const auto mask=uint32_t(value(e,2));if(value(e,0)==1)a.flags|=mask;else a.flags&=~mask;return 2;}
        case 7:outPosition(e,1,0);return 2;
        case 8:case 10:case 16:return 2; // Retained nonblocking environment/effect/sound requests; backend open.
        case 9:return move(e,first,0,0,1,2);
        case 11:scene->actors[0].poseOverride=true;scene->actors[0].pose=string(uint32_t(value(e,0)));scene->actors[0].poseAt=scene->now;return 2;
        case 12:return move(e,first,npc(e,0),1,2,4);
        case 13:if(first){const auto key=string(uint32_t(value(e,1)));const unsigned who=npc(e,3);
            // Original gor REL speaker tokens Goombella/Crump/A/B. These differ
            // from relocated name pointers used to look up the NPC slots.
            constexpr uint32_t offsets[]={0x14b28,0x14b40,0x14b5c,0x14b6c};scene->speaker=offsets[who-1];scene->speakerRoot=scene->actors[who].position;
            if(!scene->message.open(key.c_str(),messages.find(key.c_str()),font,scene->now)){scene->failed=true;return 0;}scene->messageOrder.push_back(key);}
            return scene->message.active()?0:2;
        case 14:if(first){HarborEncounterCamera r;for(unsigned i=0;i<3;++i){r.eye[i]=number(e,i);r.at[i]=number(e,i+3);}r.duration=value(e,6);r.curve=value(e,7);scene->cameras.push_back(r);}return 2;
        case 15:{auto& a=scene->actors[0];const auto mask=uint32_t(value(e,1));if(value(e,0)==1)a.flags|=mask;else a.flags&=~mask;return 2;}
        // Scoped stationary jump only: roll operand4 must0, duration ms*.06
        // truncates to60Hz ticks. Mask0x2 suppresses automatic jump poses; phase2
        // selects landing pose. Unsupported domain holds at pendingService.
        case 17:{auto& a=scene->actors[0];if(first){const float x=number(e,0),y=number(e,1),z=number(e,2);
            // The preceding source get-position callback stores Q1024 event
            // floats. Admit only that exact round-trip of this stationary
            // root, rather than demanding unencoded C++ float equality.
            const auto eventCoordinate=[](float coordinate){return float(int32_t(coordinate*1024.f))/1024.f;};
            if(x!=eventCoordinate(a.position[0]) || z!=eventCoordinate(a.position[2]) || value(e,4)!=0 || !jump.begin(a.position[1],y,int(float(value(e,3))*.06f),number(e,5))){scene->pendingService=function;return 0;}jumpLive=scene->scriptedJump=true;
            if(!(a.flags&2)){a.pose="M_J_1B";a.poseAt=scene->now;}}
            if(!jumpLive)return 0;
            const int result=jump.tick(floor);a.position[1]=jump.y;if(!(a.flags&2) && jump.phase==2 && a.pose!="M_J_1C"){a.pose="M_J_1C";a.poseAt=scene->now;}
            if(result){jumpLive=scene->scriptedJump=false;if(!(a.flags&2)){a.pose="M_S_1";a.poseAt=scene->now;}}return result;}
        case 18:scene->hp=pouch?pouch->currentHP():scene->hp;
            setEventValue(e,operand(e,0),scene->hp,vm.interpreterServices().operands);return 2;
        case 19:if(pouch){pouch->setHP(value(e,0));scene->hp=pouch->currentHP();}
            else scene->hp=value(e,0);
            return 2;
        case 20:if(npc(e,0)!=2){scene->failed=true;return 0;}scene->battleSetup=value(e,1);return 2;
        case 21:case 22:case 23:{
            if(found->second==21)scene->battleRequested=true;
            int32_t result=0;
            auto& cursor=battleCalls[e.id];
            if(!cursor)cursor=std::make_unique<NativeFieldNpcBattleScriptCall>();
            if(!cursor->step(function,e,first,vm,scene->battleServices,result)){scene->pendingService=cursor->pending();return 0;}
            battleCalls.erase(e.id);scene->pendingService=0;return result;
        }
        default:scene->pendingService=function;return 0; // Unbound wait/registry services never fabricate completion.
        }
    }
    // Private1MiB guest heap0x82000000..0x82100000, SDK descriptor words at
    //0x8041f070 (16 bytes), current heap0x8041e428 and heap handle0x8041e844.
    // GP+4 holds60Hz; GSW0=4 is original story entry. Root priority10/flags0
    // and LW0 resume branch are VM setup, not a parallel native story machine.
    bool initialize(bool resume,const EventScriptAssets* original=nullptr){
        if(!vm.memory.registerRam(0x82000000,std::vector<uint8_t>(1024*1024)) ||
           !vm.memory.registerRam(0x8041f070,std::vector<uint8_t>(16)) || !vm.memory.registerRam(0x8041e428,std::vector<uint8_t>(4)) ||
           !vm.memory.registerRam(0x8041e844,std::vector<uint8_t>(4)) || !vm.memory.importScripts(original?*original:scripts))return false;
        const auto m=vm.memoryBinding.services();const auto start=eventSdkInitializeAllocator(0x82000000,0x82100000,1,m);
        if(eventSdkCreateHeap(start,0x82100000,m)!=0 || !vm.memory.writeWord(0x8041e844,0) || !vm.memory.writeWord(0x81000004,60))return false;
        vm.gp.timestamp=scene->now*eventClockRate(243000000);vm.initializeContexts();
        writeSavedSwitch(SavedSwitchFamily::GlobalByte,0,4,vm.interpreterServices().operands);
        if(original){nativeResume=resume;return true;}
        auto creation=vm.creationServices();auto& root=createRootEvent(vm.schedule,script,10,0,creation);root.locals[0]=resume?1:0;return true;
    }
};
// Native pre-admission placements: floor15, roots55/105/135/160 and Z-300/-330;
// Goombella yaw90, others270, source NPC flags0x40000620. Original resume branch
// may overwrite these. First manager visit runs immediately with support15.
bool HarborEncounter::begin(uint64_t time,std::array<float,3> mario,int health,bool resume,std::shared_ptr<ProloguePouch> pouch){
    if(!ready || triggered)return false;
    *this=HarborEncounter{};triggered=true;now=time;hp=pouch?pouch->currentHP():health;
    actors[0].position=mario;
    for(unsigned i=1;i<5;++i){actors[i].position={float(i==1?55:i==2?105:i==3?135:160),15,float(i<3?-300:-330)};actors[i].yaw=i==1?90:270;actors[i].flags=0x40000620;}
    runtime_=std::make_shared<HarborEncounterRuntime>();runtime_->scene=this;runtime_->pouch=std::move(pouch);
    if(!runtime_->initialize(resume)){failed=true;return false;}tick(time,0,15);return !failed;
}
// Same-time calls are allowed; backwards time/failure stops dispatch. Advance
// movement then text then VM, so callback completion sees current text/roots.
// admitted tracks nonzero key-depth (not a permanent latch); finished requires
// allocationCount==0, including child events, not merely root return.
bool HarborEncounter::beginNativeField(uint64_t time,std::array<float,3> mario,int health,bool resume,NativeHarborEncounterFieldConfig config){
    if(!ready||triggered||!config.assets||!config.assets->source)return false;
    *this=HarborEncounter{};triggered=true;now=time;hp=health;actors[0].position=mario;
    runtime_=std::make_shared<HarborEncounterRuntime>();auto&r=*runtime_;r.scene=this;
    if(!r.initialize(resume,config.assets->source.get())){failed=true;return false;}
    r.nativeField=NativeHarborEncounterField::prepare(r.vm,this,nativeFieldFacade_.proof(),std::move(config));
    if(!r.nativeField){failed=true;return false;}tick(time,0,15);return !failed;
}
bool HarborEncounter::nativeFieldVmIs(const EventNativeRuntime&vm)const{return runtime_&&&runtime_->vm==&vm;}
std::optional<NativeHarborBattleTicket> HarborEncounter::nativeBattleRequest(){return runtime_&&runtime_->nativeField?runtime_->nativeField->request(this):std::nullopt;}
bool HarborEncounter::nativeBattleEntered(const NativeHarborBattleTicket&t,const HarborTutorial&battle){
    if(!runtime_||!runtime_->nativeField)return false;const auto identity=battle.nativeFieldBattleIdentity();return identity&&runtime_->nativeField->entered(this,t,*identity);
}
bool HarborEncounter::nativeBattleReturn(const NativeHarborBattleTicket&t,const NativeHarborCompletionReceipt&receipt){return runtime_&&runtime_->nativeField&&runtime_->nativeField->returned(this,t,receipt);}
bool HarborEncounter::battlePending() const{return runtime_ && (!runtime_->battleCalls.empty() || !runtime_->retiredBattleCalls.empty());}
void HarborEncounter::tick(uint64_t time,uint32_t keys,std::optional<float> support){
    if(runtime_ && runtime_->retiredBattlePending){pendingService=runtime_->retiredBattlePending;finished=false;return;}
    if(!running() || failed || time<now || !runtime_)return;
    auto& r=*runtime_;
    if(r.nativeField&&!r.nativeField->current(this))return;
    now=time;r.scene=this;r.floor=support;
    if(r.nativeField){
        if(!r.nativeField->advance(this)){pendingService=r.nativeField->pending();return;}
        if(r.nativeField->initialized()&&!r.nativeRootStarted){
            auto&root=createRootEvent(r.vm.schedule,script,10,0,r.vm.creationServices());root.locals[0]=r.nativeResume?1:0;r.nativeRootStarted=true;
        }
    }else r.advance();
    if(r.pouch)hp=r.pouch->currentHP();
    if(message.active())message.tick(now,keys);
    r.vm.gp.timestamp=now*eventClockRate(243000000);r.vm.runManager();
    if(r.retiredBattlePending){pendingService=r.retiredBattlePending;finished=false;return;}
    admitted=keyDepth!=0;story=readSavedSwitch(SavedSwitchFamily::GlobalByte,0,r.vm.interpreterServices().operands);
    finished=r.vm.runtime.allocationCount==0 && (!r.nativeField||r.nativeRootStarted);
}
}
