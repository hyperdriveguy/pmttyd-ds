#include "professor_story.hpp"
#include "message_assets.hpp"
#include "event_native_runtime.hpp"
#include "event_memory_bootstrap.hpp"
#include "actor_animation.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <unordered_map>
namespace ttyd {
namespace {
MessageAssets messages;EventScriptAssets scripts;MessageFont font;
uint32_t script=0,fenceScript=0;std::unordered_map<uint32_t,uint32_t> callbacks;bool ready=false;
std::vector<uint32_t> fenceNodes;float fenceFrames=0;
int32_t signedWord(uint32_t v){return v<=0x7fffffff?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}
}
const std::vector<uint32_t>& professorFenceCollisionNodes(){return fenceNodes;}
bool loadProfessorStoryAssets(const char* root) {
    if(!root)return false;
    const std::string base=root;
    EventScriptAssets p;MessageAssets m;MessageFont f;
    if(!p.load((base+"/professor-events.pmevt").c_str()) || !m.loadRoom((base+"/messages/gor_02.txt").c_str()) ||
       !m.loadGlobal((base+"/messages/global.txt").c_str()) || !f.load((base+"/font.pmfont").c_str()))return false;
    FILE* file=std::fopen((base+"/professor-program.bin").c_str(),"rb");if(!file)return false;
    char magic[8];uint32_t entry=0,fence=0,lecture=0,count=0;
    bool ok=std::fread(magic,8,1,file)==1 && !std::memcmp(magic,"PMPROG2",8) &&
        std::fread(&entry,4,1,file)==1 && std::fread(&fence,4,1,file)==1 && std::fread(&lecture,4,1,file)==1 &&
        std::fread(&count,4,1,file)==1 && count==63;
    std::unordered_map<uint32_t,uint32_t> next;
    for(uint32_t i=0;ok && i<count;++i){uint32_t a=0,t=0;ok=std::fread(&a,4,1,file)==1 && std::fread(&t,4,1,file)==1 && a && t<63 && next.emplace(a,t).second;}
    uint32_t word=0;ok=ok && std::fgetc(file)==EOF && p.readWord(entry,word) && p.readWord(fence,word) && p.readWord(lecture,word);std::fclose(file);
    if(!ok)return false;
    ActorAnimationPack paper;if(!paper.load((base+"/professor-fence.pmanim").c_str()))return false;
    const auto* anim=paper.pose("A_1");if(!anim || anim->loop || anim->start!=0 || anim->end!=140)return false;
    file=std::fopen((base+"/professor-fence.bin").c_str(),"rb");if(!file)return false;
    uint32_t n=0;ok=std::fread(magic,8,1,file)==1 && !std::memcmp(magic,"PMFENCE1",8) && std::fread(&n,4,1,file)==1 && n>0 && n<1024;
    std::vector<uint32_t> nodes(ok?n:0);ok=ok && std::fread(nodes.data(),4,n,file)==n && std::fgetc(file)==EOF;std::fclose(file);
    if(!ok || !std::is_sorted(nodes.begin(),nodes.end()))return false;
    fenceNodes=std::move(nodes);fenceFrames=anim->end;
    scripts=std::move(p);messages=std::move(m);font=std::move(f);script=entry;fenceScript=fence;callbacks=std::move(next);ready=true;return true;
}
struct ProfessorRuntime {
    ProfessorStory* scene=nullptr;bool doorBusy=true;uint32_t keys=0,rootId=0;
    struct Move {std::array<float,3> start{},target{};uint64_t begin=0,end=0;bool active=false,jump=false;float apex=0;};
    std::array<Move,3> moves{};
    std::unordered_map<uint32_t,uint64_t> waits;
    EventNativeRuntime vm;
    ProfessorRuntime():vm(0x81000000,platform(this)) {}
    static EventNativePlatform platform(ProfessorRuntime* owner) {
        EventNativePlatform p;p.user=owner;p.clock=[](void*)->uint32_t{return 243000000;};
        p.truncate=[](void*,float v)->int32_t{assert(std::isfinite(v) && double(v)>=-2147483648. && double(v)<2147483648.);return int32_t(v);};
        p.divideWord=[](void*,int32_t a,int32_t b){assert(b);return a/b;};
        p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){}; // CPU-owned native arena, no GPU consumers.
        p.call=[](void* p,EventEntry& e,uint32_t fn,int32_t first){return static_cast<ProfessorRuntime*>(p)->call(e,fn,first!=0);};return p;
    }
    std::string string(uint32_t a) {
        std::string result;for(unsigned i=0;i<256;++i){uint8_t c=0;if(!vm.memory.readByte(a+i,c)){scene->failed=true;return {};}
            if(!c)return result;
            result.push_back(char(c));}scene->failed=true;return {};
    }
    int32_t operand(EventEntry& e,unsigned n) {
        uint32_t v=0;
        if(n>=e.argumentCount || !vm.memory.readWord(e.arguments+4*n,v)){scene->failed=true;return 0;}
        return signedWord(v);
    }
    int32_t value(EventEntry& e,unsigned n){return getEventValue(e,operand(e,n),vm.interpreterServices().operands);}
    float number(EventEntry& e,unsigned n){return getEventFloat(e,operand(e,n),vm.interpreterServices().operands);}
    uint32_t pointer(EventEntry& e,unsigned n){return uint32_t(value(e,n));}
    void positionOut(EventEntry& e,unsigned first,unsigned actor) {
        const auto op=vm.interpreterServices().operands;
        for(unsigned axis=0;axis<3;++axis)setEventFloat(e,operand(e,first+axis),scene->actors[actor].position[axis],op);
    }
    int delay(EventEntry& e,bool first,uint64_t duration) {
        if(first)waits[e.id]=scene->now+duration;
        if(scene->now<waits[e.id])return 0;
        waits.erase(e.id);return 2;
    }
    int move(EventEntry& e,bool first,unsigned actor,unsigned x,unsigned z,unsigned speed) {
        auto& m=moves[actor];auto& a=scene->actors[actor];
        if(first) {
            m.start=a.position;m.target={number(e,x),a.position[1],number(e,z)};
            const float heading=std::atan2(m.target[0]-m.start[0],m.start[2]-m.target[2])*57.29577951308232f;
            const float travel=heading<0?heading+360:heading;
            if(travel>=10 && travel<=170)a.yaw=90;
            else if(travel>=190 && travel<=350)a.yaw=270;
            const float distance=std::hypot(m.target[0]-m.start[0],m.target[2]-m.start[2]),rate=number(e,speed);
            if(!(rate>0)){scene->failed=true;return 0;}
            // Native linear movement bridge for the source speed request. Full
            // original NPC/party/collision/ease stepping remains a separate owner.
            m.jump=false;m.begin=scene->now;m.end=m.begin+uint64_t((distance+1)/rate*1000);m.active=m.end>m.begin;a.moving=m.active;
        }
        return m.active?0:2;
    }
    void advance() {
        for(unsigned i=0;i<3;++i)if(moves[i].active) {
            auto& m=moves[i];const float t=std::min(1.f,float(scene->now-m.begin)/float(m.end-m.begin));
            for(unsigned k=0;k<3;++k)scene->actors[i].position[k]=m.start[k]+(m.target[k]-m.start[k])*t;
            if(m.jump){const float c=-4*m.apex,b=(m.target[1]-m.start[1])-c;scene->actors[i].position[1]=m.start[1]+t*(b+c*t);}
            if(t==1){m.active=false;scene->actors[i].moving=false;}
        }
    }
    int print(EventEntry& e,bool first,bool party) {
        if(first) {
            auto key=string(pointer(e,party?0:1));if(party)key+="_kur";
            scene->speaker=party || string(pointer(e,3))=="party"?1:2;
            scene->speakerRoot=scene->actors[scene->speaker].position;
            if(!scene->message.open(key.c_str(),messages.find(key.c_str()),font,scene->now)){scene->failed=true;return 0;}
            scene->messageOrder.push_back(key);
        }
        return scene->message.active() && !scene->message.stopped()?0:2;
    }
    int32_t call(EventEntry& e,uint32_t function,bool first) {
        const auto found=callbacks.find(function);if(found==callbacks.end()){scene->failed=true;return 0;}
        const unsigned tag=found->second;
        if(first){ProfessorServiceRequest r{function,e.currentCommand,scene->now,{}};
            for(unsigned i=0;i<e.argumentCount;++i)r.rawOperands.push_back(operand(e,i));
            scene->presentationRequests.push_back(std::move(r));}
        auto setPosition=[&](unsigned actor,unsigned index){for(unsigned i=0;i<3;++i)scene->actors[actor].position[i]=number(e,index+i);scene->actors[actor].hidden=scene->actors[actor].position[1]==-1000;};
        switch(tag) {
        case 0:if(first)scene->cameras.push_back({{},{},value(e,0),value(e,1),true});return 2;
        case 1:if(first){ProfessorCameraRequest r;for(unsigned i=0;i<3;++i){r.eye[i]=number(e,i);r.at[i]=number(e,i+3);}r.duration=value(e,6);r.curve=value(e,7);scene->cameras.push_back(r);}return 2;
        case 3:return doorBusy?0:2;
        case 5:for(unsigned i=0;i<3;++i)scene->itemPosition[i]=number(e,2+i);scene->itemVisible=true;return 2;
        case 7:for(unsigned i=0;i<3;++i)scene->itemPosition[i]=number(e,1+i);scene->itemVisible=scene->itemPosition[1]!=-1000;return 2;
        case 8:scene->itemScale=number(e,1);return 2;
        case 9:if(string(pointer(e,2))=="S_saku04")scene->fenceHidden=value(e,0)!=0;return 2;
        case 11:positionOut(e,1,0);return 2;
        case 12:scene->keysEnabled=value(e,0)!=0;return 2;
        case 13:return move(e,first,0,0,1,2);
        case 14:if(first)scene->actors[0].yaw=number(e,0);return delay(e,first,uint64_t(std::max<int32_t>(0,value(e,1))));
        case 15:scene->temporaryProfessor=true;for(unsigned i=0;i<3;++i)scene->actors[2].position[i]=number(e,2+i);scene->actors[2].hidden=false;return 2;
        case 16:scene->actors[0].pose=string(pointer(e,0));scene->actors[0].poseAt=scene->now;return 2;
        case 17:return print(e,first,false);
        case 18:return print(e,first,true);
        case 20:positionOut(e,1,2);return 2;
        case 21: {
            auto& m=moves[2];
            if(first) {
                m.start=scene->actors[2].position;
                for(unsigned i=0;i<3;++i)m.target[i]=number(e,1+i);
                m.begin=scene->now;m.end=m.begin+uint64_t(std::max<int32_t>(0,value(e,4)));
                m.apex=number(e,5);m.jump=true;m.active=m.end>m.begin;
            }
            return m.active?0:2;
        }
        case 22:return move(e,first,2,1,2,4);
        case 23:setPosition(2,1);return 2;
        case 24:scene->actors[2].yaw=number(e,1);return 2;
        case 26:return move(e,first,1,1,2,3);
        case 28:if(first)scene->actors[1].yaw=number(e,1);return delay(e,first,uint64_t(std::max<int32_t>(0,value(e,2))));
        case 30:scene->music=-1;return 2;
        case 31:scene->music=pointer(e,1)?1:0;return 2;
        case 35:positionOut(e,1,value(e,0)==1?2:1);return 2;
        case 36:if(value(e,0)==1)scene->temporaryProfessor=false;return 2;
        case 41:scene->captureRequested=true;return 2;
        case 43:
            if(!scene->captureReady)return 0;
            for(unsigned i=0;i<4;++i)setEventValue(e,operand(e,i+1),scene->captureBounds[i],vm.interpreterServices().operands);
            return 2;
        case 46:scene->fencePose=string(pointer(e,1));scene->fencePoseAt=scene->now;return 2;
        case 47:scene->fenceVisible=value(e,1)!=0;return 2;
        case 50:
            if(first)return 0; // Original wait does not complete on admission.
            return scene->fencePose=="A_1" && double(scene->now-scene->fencePoseAt)*60>=double(fenceFrames)*1000?2:0;
        case 52:scene->fenceVisible=false;return 2;
        case 54:
            if(string(pointer(e,0))=="A_saku_04" && value(e,1)==1 && value(e,2)==0)scene->fenceCollisionOff=true;
            return 2;
        case 55: {
            float a[8];for(unsigned i=0;i<8;++i)a[i]=number(e,i);
            bool hit=true;
            for(unsigned axis=0;axis<2;++axis) {
                const float x=a[axis],y=a[axis+2],c=a[axis+4],d=a[axis+6];
                if(x>=y?((x<c && x<d)||(y>c && y>d)):((y<c && y<d)||(x>c && x>d)))hit=false;
            }
            if(hit) {
                const float u=a[1]-a[3],v=a[0]-a[2];
                const float c=std::fma(v,a[5]-a[1],u*(a[0]-a[4])),d=std::fma(v,a[7]-a[1],u*(a[0]-a[6]));
                hit=!(c*d>0);
            }
            if(hit) {
                const float u=a[5]-a[7],v=a[4]-a[6];
                const float c=std::fma(v,a[1]-a[5],u*(a[4]-a[0])),d=std::fma(v,a[3]-a[5],u*(a[4]-a[2]));
                hit=!(c*d>0);
            }
            setEventValue(e,operand(e,8),hit?1:0,vm.interpreterServices().operands);return 2;
        }
        case 56:case 57:case 58:case 59:case 60:case 61:case 62:
            scene->pendingService=function;return 0; // Original continuation waits for its native owner.
        // Source music,effects,item/paper/display/party-flag requests are retained
        // above for native consumers. They do not fabricate story/dialogue.
        default:return 2;
        }
    }
    bool initialize(uint32_t entry,int32_t initialStory) {
        if(!vm.memory.registerRam(0x82000000,std::vector<uint8_t>(1024*1024)) ||
           !vm.memory.registerRam(0x8041f070,std::vector<uint8_t>(16)) ||
           !vm.memory.registerRam(0x8041e428,std::vector<uint8_t>(4)) ||
           !vm.memory.registerRam(0x8041e844,std::vector<uint8_t>(4)) || !vm.memory.importScripts(scripts))return false;
        auto m=vm.memoryBinding.services();const auto start=eventSdkInitializeAllocator(0x82000000,0x82100000,1,m);
        if(eventSdkCreateHeap(start,0x82100000,m)!=0)return false;
        if(!vm.memory.writeWord(0x8041e844,0) || !vm.memory.writeWord(0x81000004,60))return false;
        vm.gp.timestamp=scene->now*eventClockRate(243000000);vm.initializeContexts();
        const auto op=vm.interpreterServices().operands;writeSavedSwitch(SavedSwitchFamily::GlobalByte,0,initialStory,op);
        auto creation=vm.creationServices();rootId=createRootEvent(vm.schedule,entry,10,0,creation).id;return true;
    }
};
bool ProfessorStory::begin(uint64_t time,std::array<float,3> mario,std::array<float,3> goombella) {
    if(!ready || triggered)return false;
    *this=ProfessorStory{};triggered=true;now=time;
    actors[0].position=mario;actors[0].pose="M_S_1";actors[1].position=goombella;actors[1].pose="PKR_S_1";
    runtime_=std::make_shared<ProfessorRuntime>();runtime_->scene=this;
    if(!runtime_->initialize(script,8)){failed=true;return false;}tick(time,0,true);return !failed;
}
bool ProfessorStory::beginFence(uint64_t time,std::array<float,3> mario,std::array<float,3> goombella) {
    if(!ready || running() || !temporaryProfessor)return false;
    const auto professor=actors[2];
    *this=ProfessorStory{};triggered=true;fenceStarted=true;temporaryProfessor=true;story=9;now=time;
    actors[0].position=mario;actors[0].pose="M_S_1";actors[1].position=goombella;actors[2]=professor;
    runtime_=std::make_shared<ProfessorRuntime>();runtime_->scene=this;
    if(!runtime_->initialize(fenceScript,9)){failed=true;return false;}tick(time,0,true);return !failed;
}
void ProfessorStory::tick(uint64_t time,uint32_t keys,bool doorBusy) {
    if(!running() || failed || time<now || !runtime_)return;
    now=time;
    auto& r=*runtime_;r.scene=this;r.keys=keys;r.doorBusy=doorBusy;r.advance();
    if(message.active())message.tick(now,keys);
    r.vm.gp.timestamp=now*eventClockRate(243000000);r.vm.runManager();
    story=readSavedSwitch(SavedSwitchFamily::GlobalByte,0,r.vm.interpreterServices().operands);
    finished=r.vm.runtime.allocationCount==0;
}
}
