#include "harbor_encounter.hpp"
#include "harbor_story.hpp"
#include "message_assets.hpp"
#include "event_native_runtime.hpp"
#include "event_memory_bootstrap.hpp"
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <unordered_map>
namespace ttyd {
namespace {
EventScriptAssets scripts;MessageAssets messages;MessageFont font;uint32_t script=0;
std::array<uint32_t,4> names{};std::unordered_map<uint32_t,uint32_t> callbacks;bool ready=false;
int32_t signedWord(uint32_t v){return v<=0x7fffffff?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}
}
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
    struct Move {std::array<float,3> start{},target{};uint64_t begin=0,end=0;bool active=false;};
    std::array<Move,5> moves{};FieldScriptedJump jump;bool jumpLive=false;
    std::unordered_map<uint32_t,uint64_t> waits;
    EventNativeRuntime vm;
    HarborEncounterRuntime():vm(0x81000000,platform(this)){}
    static EventNativePlatform platform(HarborEncounterRuntime* r) {
        EventNativePlatform p;p.user=r;p.clock=[](void*)->uint32_t{return 243000000;};
        p.truncate=[](void*,float v)->int32_t{assert(std::isfinite(v) && double(v)>=-2147483648. && double(v)<2147483648.);return int32_t(v);};
        p.divideWord=[](void*,int32_t a,int32_t b){assert(b);return a/b;};
        p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){};
        p.call=[](void* p,EventEntry& e,uint32_t fn,int32_t first){return static_cast<HarborEncounterRuntime*>(p)->call(e,fn,first!=0);};return p;
    }
    int32_t operand(EventEntry& e,unsigned i){uint32_t v=0;if(i>=e.argumentCount || !vm.memory.readWord(e.arguments+4*i,v)){scene->failed=true;return 0;}return signedWord(v);}
    int32_t value(EventEntry& e,unsigned i){return getEventValue(e,operand(e,i),vm.interpreterServices().operands);}
    float number(EventEntry& e,unsigned i){return getEventFloat(e,operand(e,i),vm.interpreterServices().operands);}
    std::string string(uint32_t a){std::string result;for(unsigned i=0;i<256;++i){uint8_t c=0;if(!vm.memory.readByte(a+i,c)){scene->failed=true;return {};}if(!c)return result;result.push_back(char(c));}scene->failed=true;return {};}
    unsigned npc(EventEntry& e,unsigned i){const auto a=uint32_t(value(e,i));for(unsigned n=0;n<4;++n)if(names[n]==a)return n+1;scene->failed=true;return 1;}
    void outPosition(EventEntry& e,unsigned i,unsigned actor){for(unsigned k=0;k<3;++k)setEventFloat(e,operand(e,i+k),scene->actors[actor].position[k],vm.interpreterServices().operands);}
    int delay(EventEntry& e,bool first,uint64_t duration){if(first)waits[e.id]=scene->now+duration;if(scene->now<waits[e.id])return 0;waits.erase(e.id);return 2;}
    int move(EventEntry& e,bool first,unsigned actor,unsigned x,unsigned z,unsigned speed){
        auto& m=moves[actor];auto& a=scene->actors[actor];
        if(first){m.start=a.position;m.target={number(e,x),a.position[1],number(e,z)};
            const float rate=number(e,speed);if(!(rate>0)){scene->failed=true;return 0;}
            // Native movement backend adaptation. The original event owns the
            // resolved operands, blocking continuation and independent inlines.
            const float distance=std::hypot(m.target[0]-m.start[0],m.target[2]-m.start[2]);
            m.begin=scene->now;m.end=m.begin+uint64_t((distance+1)/rate*1000);m.active=true;a.moving=true;
            if(m.target[0]!=m.start[0])a.yaw=m.target[0]>m.start[0]?90:270;
        }
        return m.active?0:2;
    }
    void advance(){for(unsigned i=0;i<5;++i){auto& m=moves[i];if(!m.active)continue;
        const float t=std::min(1.f,float(scene->now-m.begin)/float(m.end-m.begin));
        for(unsigned k=0;k<3;++k)scene->actors[i].position[k]=m.start[k]+(m.target[k]-m.start[k])*t;
        if(t==1){m.active=false;scene->actors[i].moving=false;}
    }}
    int32_t call(EventEntry& e,uint32_t function,bool first){
        const auto found=callbacks.find(function);if(found==callbacks.end()){scene->failed=true;return 0;}
        if(first){HarborEncounterRequest r{function,e.currentCommand,scene->now,{}};for(unsigned i=0;i<e.argumentCount;++i)r.operands.push_back(operand(e,i));scene->requests.push_back(std::move(r));}
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
        case 11:scene->actors[0].pose=string(uint32_t(value(e,0)));scene->actors[0].poseAt=scene->now;return 2;
        case 12:return move(e,first,npc(e,0),1,2,4);
        case 13:if(first){const auto key=string(uint32_t(value(e,1)));const unsigned who=npc(e,3);
            constexpr uint32_t offsets[]={0x14b28,0x14b40,0x14b5c,0x14b6c};scene->speaker=offsets[who-1];scene->speakerRoot=scene->actors[who].position;
            if(!scene->message.open(key.c_str(),messages.find(key.c_str()),font,scene->now)){scene->failed=true;return 0;}scene->messageOrder.push_back(key);}
            return scene->message.active()?0:2;
        case 14:if(first){HarborEncounterCamera r;for(unsigned i=0;i<3;++i){r.eye[i]=number(e,i);r.at[i]=number(e,i+3);}r.duration=value(e,6);r.curve=value(e,7);scene->cameras.push_back(r);}return 2;
        case 15:{auto& a=scene->actors[0];const auto mask=uint32_t(value(e,1));if(value(e,0)==1)a.flags|=mask;else a.flags&=~mask;return 2;}
        case 17:{auto& a=scene->actors[0];if(first){const float x=number(e,0),y=number(e,1),z=number(e,2);
            // The preceding source get-position callback stores Q1024 event
            // floats. Admit only that exact round-trip of this stationary
            // root, rather than demanding unencoded C++ float equality.
            const auto eventCoordinate=[](float coordinate){return float(int32_t(coordinate*1024.f))/1024.f;};
            if(x!=eventCoordinate(a.position[0]) || z!=eventCoordinate(a.position[2]) || value(e,4)!=0 || !jump.begin(a.position[1],y,int(float(value(e,3))*.06f),number(e,5))){scene->pendingService=function;return 0;}jumpLive=true;}
            if(!jumpLive)return 0;
            const int result=jump.tick(floor);a.position[1]=jump.y;if(result){jumpLive=false;}return result;}
        case 18:setEventValue(e,operand(e,0),scene->hp,vm.interpreterServices().operands);return 2;
        case 19:scene->hp=value(e,0);return 2;
        case 20:if(npc(e,0)!=2){scene->failed=true;return 0;}scene->battleSetup=value(e,1);return 2;
        case 21:scene->battleRequested=true;scene->pendingService=function;return 0;
        default:scene->pendingService=function;return 0; // Unbound wait/registry services never fabricate completion.
        }
    }
    bool initialize(bool resume){
        if(!vm.memory.registerRam(0x82000000,std::vector<uint8_t>(1024*1024)) ||
           !vm.memory.registerRam(0x8041f070,std::vector<uint8_t>(16)) || !vm.memory.registerRam(0x8041e428,std::vector<uint8_t>(4)) ||
           !vm.memory.registerRam(0x8041e844,std::vector<uint8_t>(4)) || !vm.memory.importScripts(scripts))return false;
        const auto m=vm.memoryBinding.services();const auto start=eventSdkInitializeAllocator(0x82000000,0x82100000,1,m);
        if(eventSdkCreateHeap(start,0x82100000,m)!=0 || !vm.memory.writeWord(0x8041e844,0) || !vm.memory.writeWord(0x81000004,60))return false;
        vm.gp.timestamp=scene->now*eventClockRate(243000000);vm.initializeContexts();
        writeSavedSwitch(SavedSwitchFamily::GlobalByte,0,4,vm.interpreterServices().operands);
        auto creation=vm.creationServices();auto& root=createRootEvent(vm.schedule,script,10,0,creation);root.locals[0]=resume?1:0;return true;
    }
};
bool HarborEncounter::begin(uint64_t time,std::array<float,3> mario,int health,bool resume){
    if(!ready || triggered)return false;
    *this=HarborEncounter{};triggered=true;now=time;hp=health;
    actors[0].position=mario;
    for(unsigned i=1;i<5;++i){actors[i].position={float(i==1?55:i==2?105:i==3?135:160),15,float(i<3?-300:-330)};actors[i].yaw=i==1?90:270;actors[i].flags=0x40000620;}
    runtime_=std::make_shared<HarborEncounterRuntime>();runtime_->scene=this;
    if(!runtime_->initialize(resume)){failed=true;return false;}tick(time,0,15);return !failed;
}
void HarborEncounter::tick(uint64_t time,uint32_t keys,std::optional<float> support){
    if(!running() || failed || time<now || !runtime_)return;
    now=time;auto& r=*runtime_;r.scene=this;r.floor=support;r.advance();
    if(message.active())message.tick(now,keys);
    r.vm.gp.timestamp=now*eventClockRate(243000000);r.vm.runManager();
    admitted=keyDepth!=0;story=readSavedSwitch(SavedSwitchFamily::GlobalByte,0,r.vm.interpreterServices().operands);
    finished=r.vm.runtime.allocationCount==0;
}
}
