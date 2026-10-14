#include "opening_house_sounds.hpp"
#include "audio_policy.hpp"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
namespace ttyd {
namespace {
struct Object {std::array<float,12> world;std::array<float,3> localMean;};
std::array<OpeningHouseCue,4> cues{};std::array<Object,2> objects{};bool ready=false;
const char* names[]={"SFX_EVT_OPN_PARETTA_MOVE1","SFX_EVT_POST_SHAKE1","SFX_DOOR_OPEN1","SFX_DOOR_SHUT1"};
float published(float f){return std::trunc(f*1024.f)/1024.f;}
bool objectPosition(unsigned object,float parameter,std::array<float,3>& result){
    if(!ready || object>1 || !std::isfinite(parameter))return false;
    const auto& o=objects[object];auto p=o.localMean;
    if(object==0){const float degrees=published(parameter*.5546875f),angle=degrees*.01745329238474369f;
        const float c=std::cos(angle),s=std::sin(angle);p[0]=c*o.localMean[0]+s*o.localMean[2];p[2]=-s*o.localMean[0]+c*o.localMean[2];}
    else p[0]+=parameter/10.f;
    for(unsigned r=0;r<3;++r)result[r]=published((o.world[r*4]*p[0]+o.world[r*4+1]*p[1]+o.world[r*4+2]*p[2]+o.world[r*4+3])*10.f);
    return true;
}
}
bool loadOpeningHouseSoundAssets(const char* root){
    if(!root)return false;
    FILE* f=std::fopen((std::string(root)+"/opening-house-sounds.bin").c_str(),"rb");if(!f)return false;
    char magic[8];uint32_t count=0;std::array<OpeningHouseCue,4> next{};std::array<Object,2> points{};
    bool ok=std::fread(magic,8,1,f)==1 && !std::memcmp(magic,"PMHSND01",8) && std::fread(&count,4,1,f)==1 && count==4 && std::fread(next.data(),sizeof(next),1,f)==1 && std::fread(points.data(),sizeof(points),1,f)==1 && std::fgetc(f)==EOF;
    std::fclose(f);const uint32_t indices[]={972,973,568,589};
    for(unsigned i=0;i<4;++i)ok=ok && next[i].name[31]==0 && !std::strcmp(next[i].name.data(),names[i]) && next[i].tableIndex==indices[i] && next[i].rate>=8000 && next[i].rate<=48000 && next[i].frames>0;
    for(const auto& point:points){for(float x:point.world)ok=ok && std::isfinite(x);for(float x:point.localMean)ok=ok && std::isfinite(x);}
    const char* clips[]={"flight","postbox","door-open","door-shut"};
    for(unsigned i=0;ok && i<4;++i){
        FILE* pcm=std::fopen((std::string(root)+"/opening-house-"+clips[i]+".wav").c_str(),"rb");
        if(!pcm){ok=false;break;}unsigned char header[44]{};PcmHeader format;
        ok=std::fread(header,44,1,pcm)==1 && pcmHeader(header,format) && format.channels==1 && format.rate==next[i].rate && format.frames==next[i].frames;
        if(ok){ok=std::fseek(pcm,0,SEEK_END)==0 && std::ftell(pcm)==long(44+next[i].frames*2);}
        std::fclose(pcm);
    }
    if(!ok)return false;
    cues=next;objects=points;ready=true;return true;
}
const OpeningHouseCue* openingHouseCue(unsigned cue){return ready && cue<4?&cues[cue]:nullptr;}
void OpeningHouseSounds::begin(uint64_t tick){
    *this=OpeningHouseSounds{};beginTick=tick;
    requests.push_back({OpeningHouseAudioRequest::Operation::EnvironmentOn,0xd10,0x110,tick,-1,0,"ENV_OPN_AAA1"});
    requests.push_back({OpeningHouseAudioRequest::Operation::BgmScope,0xd20,0,tick,-1,1});
}
void OpeningHouseSounds::startFlight(uint64_t tick){flightStarted=true;phase=Phase::Delay;count=0;deadline=tick+180;}
void OpeningHouseSounds::sfx(unsigned cue,uint32_t command,uint64_t tick,const std::array<float,3>& position,bool resolved){
    OpeningHouseAudioRequest r;r.command=command;r.tick=tick;r.cue=int32_t(cue);r.name=names[cue];r.position=position;r.positionResolved=resolved;requests.push_back(r);
}
void OpeningHouseSounds::advance(uint64_t tick,const std::array<float,3>& npc){
    if(!flightStarted || flightFinished)return;
    const std::array<float,3> p={published(npc[0]),published(npc[1]),-100};
    while(tick>=deadline && !flightFinished){
        if(phase==Phase::Delay){phase=Phase::Initial;count=0;}
        if(phase==Phase::Initial && count==14){phase=Phase::Holding;count=0;}
        if(phase==Phase::Holding && count && departurePublished){phase=Phase::Final;count=0;}
        if(phase==Phase::Final && count==17){phase=Phase::Complete;flightFinished=true;break;}
        sfx(0,phase==Phase::Initial?0xf38:phase==Phase::Holding?0xf80:0xfdc,deadline,p,true);
        ++count;deadline+=phase==Phase::Holding?24:12;
    }
}
void OpeningHouseSounds::postbox(uint64_t tick,uint32_t command,float parameter){std::array<float,3> p{};const bool resolved=objectPosition(1,parameter,p);sfx(1,command,tick,p,resolved);}
void OpeningHouseSounds::door(uint64_t tick,uint32_t command,float parameter,bool open){std::array<float,3> p{};const bool resolved=objectPosition(0,parameter,p);sfx(open?2:3,command,tick,p,resolved);}
void OpeningHouseSounds::interior(uint64_t tick){
    requests.push_back({OpeningHouseAudioRequest::Operation::EnvironmentOff,0x1590,0x4800,tick});
    requests.push_back({OpeningHouseAudioRequest::Operation::EnvironmentLpf,0x159c,0,tick,-1,800});
}
}
