#include "battle_camera_frame.hpp"
#include <cstring>
#include <cassert>
namespace ttyd {
namespace {
uint32_t word(const BattleCameraRequestServices& s,uint32_t a){return s.positions.memory.memory.word(s.positions.memory.memory.user,a);}
void word(const BattleCameraRequestServices& s,uint32_t a,uint32_t v){s.positions.memory.memory.storeWord(s.positions.memory.memory.user,a,v);}
uint16_t half(const BattleCameraRequestServices& s,uint32_t a){return s.positions.memory.memory.half(s.positions.memory.memory.user,a);}
void half(const BattleCameraRequestServices& s,uint32_t a,uint16_t v){s.positions.memory.memory.storeHalf(s.positions.memory.memory.user,a,v);}
float real(const BattleCameraRequestServices& s,uint32_t a){const auto bits=word(s,a);float v;std::memcpy(&v,&bits,4);return v;}
void real(const BattleCameraRequestServices& s,uint32_t a,float v){uint32_t bits;std::memcpy(&bits,&v,4);word(s,a,bits);}
float single(float v){volatile float result=v;return result;}
CameraDepthPoint vector(const BattleCameraRequestServices& s,uint32_t a){return {real(s,a),real(s,a+4),real(s,a+8)};}
void vector(const BattleCameraRequestServices& s,uint32_t a,CameraDepthPoint v){for(unsigned i=0;i<3;++i)real(s,a+4*i,v[i]);}
bool move(uint32_t b,BattleCameraFrame& c,CameraDepthPoint eye,CameraDepthPoint at,bool relative,bool clamp,
    CameraDepthPoint& de,CameraDepthPoint& da,const BattleCameraRequestServices& s){
    int32_t xy=int16_t(half(s,b+0x277c)),z=int16_t(half(s,b+0x277e));
    if(xy<0 || xy>=5){xy=1;half(s,b+0x277c,1);}if(z<0 || z>=6){z=1;half(s,b+0x277e,1);}
    if((xy!=1 && xy!=3) || (z!=1 && z!=3))return false;
    const auto mario=battleMarioUnit(b,s.positions.memory);
    if(relative){
        if(word(s,b+0xef4)&0x100){const float home=real(s,mario+0x38),offset=single(-home*.25f);eye[2]=single(eye[2]+home);at[2]=single(at[2]+home);eye[1]=single(eye[1]+offset);at[1]=single(at[1]+offset);}
        const auto offset=vector(s,b+0x27b0);for(unsigned i=0;i<3;++i){eye[i]=single(eye[i]+offset[i]);at[i]=single(at[i]+offset[i]);}
    }
    float extent=single(single(600.f-eye[2])*.5f);if(extent<0)extent=0;
    if(clamp){const float lo=single(-100.f-extent),hi=single(100.f+extent);for(auto* p:{&eye,&at}){if((*p)[0]<lo)(*p)[0]=lo;else if((*p)[0]>hi)(*p)[0]=hi;}}
    auto axis=[&](float& current,float target,float& delta,int32_t speed){delta=single(target-current);if(speed==1)delta=single(delta/5.f);current=single(current+delta);};
    axis(c.eye[0],eye[0],de[0],xy);axis(c.eye[1],eye[1],de[1],xy);
    axis(c.at[0],at[0],da[0],xy);axis(c.at[1],at[1],da[1],xy);c.up={0,1,0};
    axis(c.eye[2],eye[2],de[2],z);axis(c.at[2],at[2],da[2],z);return true;
}
}
void initializeBattleCamera(uint32_t b,const BattleCameraRequestServices& s){
    for(unsigned i=0;i<0x104;i+=4)word(s,b+0x2754+i,0);
    word(s,b+0x2754,0);const auto type=int32_t(word(s,word(s,b+0x2738)+8));
    if(type==0)word(s,b+0x2760,14);else if(type>=1 && type<=8)word(s,b+0x2760,15);else if(type==9)word(s,b+0x2760,16);
    word(s,b+0x275c,0);real(s,b+0x2778,0);half(s,b+0x277c,1);half(s,b+0x277e,1);
    for(unsigned i=0;i<3;++i)word(s,b+0x27b0+4*i,word(s,0x802f0240+4*i));word(s,b+0x2758,0);
}
bool setBattleCameraMoveTo(uint32_t b,int32_t priority,int32_t duration,int32_t curve,CameraDepthPoint eye,CameraDepthPoint at,const BattleCameraFrame& c,const BattleCameraRequestServices& s){
    if(priority<int32_t(word(s,b+0x275c)))return false;
    vector(s,b+0x27e0,c.eye);vector(s,b+0x27ec,eye);vector(s,b+0x27f8,c.at);vector(s,b+0x2804,at);
    word(s,b+0x27d8,uint32_t(duration));word(s,b+0x27dc,uint32_t(duration));word(s,b+0x27d4,uint32_t(curve));return true;
}
int32_t battleCameraMoveToEvent(EventEntry& e,const BattleCameraFrame& c,const BattleCameraRequestServices& s){
    assert(s.values.resolve && s.values.resolveFloat);const auto args=e.arguments;
    auto integer=[&](unsigned i){return s.values.resolve(s.values.user,e,int32_t(word(s,args+4*i)));};
    const int32_t priority=integer(0);CameraDepthPoint eye{},at{};
    for(unsigned i=0;i<6;++i)(i<3?eye:at)[i%3]=s.values.resolveFloat(s.values.user,e,int32_t(word(s,args+4*(i+1))));
    const int32_t duration=integer(7),curve=integer(8);
    setBattleCameraMoveTo(word(s,0x8041eb90),priority,duration,curve,eye,at,c,s);return 2;
}
bool updateBattleCameraFrame(uint32_t b,BattleCameraFrame& c,const BattleCameraFrameServices& services){
    const auto& s=services.requests;const auto mode=word(s,b+0x2760);
    if(mode!=3)word(s,b+0x27d8,0);
    c.eye=vector(s,b+0x2780);c.at=vector(s,b+0x2798);const auto oldEye=c.eye,oldAt=c.at;
    vector(s,b+0x278c,c.eye);vector(s,b+0x27a4,c.at);CameraDepthPoint de{},da{};
    if(mode==15 || mode==16){
        half(s,b+0x277c,3);half(s,b+0x277e,3);const float x=mode==16?-100.f:0.f;
        if(!move(b,c,{x,110,500},{x,60,0},true,true,de,da,s))return false;
    }else if(mode==17 || mode==18){
        const auto mario=battleMarioUnit(b,s.positions.memory),party=battlePartyUnit(b,s.positions.memory);
        if(!mario)return false;auto center=vector(s,mario+0x30);
        if(party){const auto home=vector(s,party+0x30);for(unsigned i=0;i<3;++i)center[i]=single(single(center[i]+home[i])*.5f);}
        if(mode==18)center[1]=single(center[1]-20.f);
        half(s,b+0x277c,1);half(s,b+0x277e,1);
        if(!move(b,c,{center[0],single(center[1]+100.f),single(center[2]+350.f)},
            {center[0],single(center[1]+50.f),0},false,true,de,da,s))return false;
    }else if(mode==14){
        const auto counter=word(s,b+0x2758)+1;word(s,b+0x2758,counter);
        if(counter==1){
            const int32_t rank=int16_t(half(s,word(s,0x8041eb00)+0x88));const float depths[]{750,830,920,1000};const float z=rank>=0 && rank<4?depths[rank]:0;
            half(s,b+0x277c,3);half(s,b+0x277e,3);if(!move(b,c,{0,110,z},{0,60,0},true,true,de,da,s))return false;
        }else if(counter==10){word(s,b+0x2758,0);setBattleCameraMode(b,0,3,s);setBattleCameraMoveTo(b,0,120,4,{0,110,750},{0,60,0},c,s);}
    }else if(mode==3){
        const int32_t remaining=int32_t(word(s,b+0x27d8));if(remaining>0){
            const int16_t saved=int16_t(half(s,b+0x277c));word(s,b+0x27d8,uint32_t(remaining)-1);
            if(!services.interpolate)return false;
            CameraDepthPoint eye{},at{};for(unsigned i=0;i<6;++i){
                const int32_t duration=int32_t(word(s,b+0x27dc)),step=int32_t(uint32_t(duration)-word(s,b+0x27d8)),curve=int32_t(word(s,b+0x27d4));
                const auto value=services.interpolate(services.user,curve,step,duration,real(s,b+(i<3?0x27e0:0x27f8)+4*(i%3)),real(s,b+(i<3?0x27ec:0x2804)+4*(i%3)));if(!value)return false;(i<3?eye:at)[i%3]=*value;
            }
            half(s,b+0x277c,3);half(s,b+0x277e,3);
            if(!move(b,c,eye,at,false,false,de,da,s))return false;half(s,b+0x277c,uint16_t(saved));
        }
    }else if(mode<=20)return false;
    if(c.eye[2]>=1000)c.eye[2]=1000;
    const auto flags=word(s,b+0x2754);for(unsigned i=0;i<3;++i)if(flags&(4u<<i)){c.eye[i]=oldEye[i];c.at[i]=oldAt[i];de[i]=da[i]=0;}
    vector(s,b+0x2780,c.eye);vector(s,b+0x2798,c.at);vector(s,b+0x27bc,de);vector(s,b+0x27c8,da);
    if(!(flags&3) && services.rumbleOff)services.rumbleOff(services.user,0);if((flags&0x40) && services.rumbleOff)services.rumbleOff(services.user,0);
    return !(flags&3); // Actual tremor providers remain unavailable after baseline publication.
}
}
