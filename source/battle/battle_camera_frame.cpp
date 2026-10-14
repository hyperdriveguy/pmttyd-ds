#include "battle_camera_frame.hpp"
#include <cstring>
#include <cassert>
#include <cmath>
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
    const auto mario=battleMarioUnit(b,s.positions.memory);
    if(relative){
        if(word(s,b+0xef4)&0x100){const float home=real(s,mario+0x38),offset=single(-home*.25f);eye[2]=single(eye[2]+home);at[2]=single(at[2]+home);eye[1]=single(eye[1]+offset);at[1]=single(at[1]+offset);}
        const auto offset=vector(s,b+0x27b0);for(unsigned i=0;i<3;++i){eye[i]=single(eye[i]+offset[i]);at[i]=single(at[i]+offset[i]);}
    }
    float extent=single(single(600.f-eye[2])*.5f);if(extent<0)extent=0;
    if(clamp){const float lo=single(-100.f-extent),hi=single(100.f+extent);for(auto* p:{&eye,&at}){if((*p)[0]<lo)(*p)[0]=lo;else if((*p)[0]>hi)(*p)[0]=hi;}}
    auto axis=[&](float& current,float target,float& delta,int32_t speed){
        constexpr float divisors[]{1.5f,15.f,5.f,1.f,200.f,100.f};
        delta=single(target-current);if(speed!=3)delta=single(delta/divisors[speed]);current=single(current+delta);
    };
    axis(c.eye[0],eye[0],de[0],xy);axis(c.eye[1],eye[1],de[1],xy);
    axis(c.at[0],at[0],da[0],xy);axis(c.at[1],at[1],da[1],xy);c.up={0,1,0};
    axis(c.eye[2],eye[2],de[2],z);axis(c.at[2],at[2],da[2],z);return true;
}
bool tremor(uint32_t b,BattleCameraFrame& c,const BattleCameraFrameServices& services,BattleCameraTremorCursor& cursor){
    const auto& s=services.requests;
    while(cursor.axis<2){
        const uint32_t axis=cursor.axis,mask=1u<<axis,totalAddress=b+0x2830+4*axis,timerAddress=b+0x2838+4*axis;
        if(cursor.stage==0){
            if(!(word(s,b+0x2754)&mask)){++cursor.axis;continue;}
            cursor.speedXY=half(s,b+0x277c);cursor.speedZ=half(s,b+0x277e);
            const uint32_t remaining=word(s,timerAddress),total=word(s,totalAddress);
            half(s,b+0x277c,3);cursor.phase=(total-remaining)&2u;half(s,b+0x277e,3);
            const uint32_t liveTotal=word(s,totalAddress),liveRemaining=word(s,timerAddress);
            cursor.curve=int32_t(word(s,b+0x2828+4*axis));
            cursor.start=real(s,b+0x2840+8*axis);cursor.end=real(s,b+0x2844+8*axis);
            cursor.step=int32_t(liveTotal-liveRemaining)/2;cursor.duration=int32_t(liveTotal)/2;cursor.stage=1;
        }
        if(cursor.stage==1){
            if(!services.interpolate)return false;
            const auto amplitude=services.interpolate(services.user,cursor.curve,cursor.step,cursor.duration,cursor.start,cursor.end);
            if(!amplitude)return false;real(s,b+0x2850+4*axis,*amplitude);cursor.stage=2;
        }
        if(cursor.stage==2){
            const int32_t remaining=int32_t(word(s,timerAddress));
            if(remaining>0){
                auto eye=c.eye,at=c.at;const float amplitude=real(s,b+0x2850+4*axis);
                eye[axis]=single(cursor.phase==0?eye[axis]+amplitude:eye[axis]-amplitude);
                at[axis]=single(cursor.phase==0?at[axis]+amplitude:at[axis]-amplitude);
                CameraDepthPoint de{},da{};
                if(!move(b,c,eye,at,false,false,de,da,s))return false;
            }else if(remaining==0)word(s,b+0x2754,word(s,b+0x2754)&~mask);
            half(s,b+0x277c,cursor.speedXY);half(s,b+0x277e,cursor.speedZ);
            word(s,timerAddress,word(s,timerAddress)-1u);++cursor.axis;cursor.stage=0;
        }
    }
    cursor.active=false;return true;
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
bool updateBattleCameraFrame(uint32_t b,BattleCameraFrame& c,const BattleCameraFrameServices& services,BattleCameraTremorCursor* continuation){
    BattleCameraTremorCursor local;auto& cursor=continuation?*continuation:local;
    if(!cursor.active){
    const auto& s=services.requests;const auto mode=word(s,b+0x2760);
    if(mode!=3)word(s,b+0x27d8,0);
    c.eye=vector(s,b+0x2780);c.at=vector(s,b+0x2798);const auto oldEye=c.eye,oldAt=c.at;
    vector(s,b+0x278c,c.eye);vector(s,b+0x27a4,c.at);CameraDepthPoint de{},da{};
    if(mode==0 || mode==15 || mode==16){
        if(mode!=0){half(s,b+0x277c,3);half(s,b+0x277e,3);}const float x=mode==16?-100.f:0.f;
        if(!move(b,c,{x,110,mode==0?750.f:500.f},{x,60,0},true,true,de,da,s))return false;
    }else if(mode==7 || mode==10 || mode==11){
        const auto first=battleRegistryUnit(b,int32_t(word(s,b+0x2764)),s.positions.memory);
        uint32_t part=0;bool missing=!first;
        if(first){part=packedRegistryPart(int32_t(word(s,b+0x2764)),int32_t(word(s,b+0x276c)),s.positions.memory);if(mode!=7 && !part)missing=true;}
        if(mode==11){
            const auto second=battleRegistryUnit(b,int32_t(word(s,b+0x2768)),s.positions.memory);
            uint32_t secondPart=0;if(second)secondPart=packedRegistryPart(int32_t(word(s,b+0x2768)),int32_t(word(s,b+0x2770)),s.positions.memory);
            if(!second || !secondPart)missing=true;
            if(!missing){
                const auto a=packedPartWorldPosition(part,s.positions),hit=packedHitPosition(second,secondPart,s.positions);
                float wa=single(float(int16_t(half(s,first+0xcc)))-24.f),wb=single(float(int16_t(half(s,second+0xcc)))-24.f);if(wa<0)wa=0;if(wb<0)wb=0;
                float denominator=single(real(s,b+0x281c)-real(s,b+0x2810));if(denominator==0)denominator=10;
                float ratio=single(single(hit[0]-a[0])/denominator);if(ratio<.5f)ratio=.5f;else if(ratio>1)ratio=1;
                const float g=single(1.f-ratio),q=-std::fma(1.5f,single(wa+wb),-single(2.f*real(s,b+0x2778))),z=single(g*q),x=single(g*real(s,b+0x281c));
                float y=single(single(real(s,b+0x2820)/80.f)*single(a[1]-real(s,b+0x2820)));if(y<0)y=0;else if(y>100)y=100;
                const float lift=float(std::fma(20.0,-double(g),double(y))),t=single(-z/9.f);
                if(!move(b,c,{x,single(single(110.f+lift)+t),single(750.f-z)}, {x,single(single(60.f+lift)+t),-z},true,true,de,da,s))return false;
            }
        }else if(!missing){
            auto p=mode==7?vector(s,first+0x3c):packedHitPosition(first,part,s.positions);
            p[1]=single(p[1]+single(single(float(int16_t(half(s,first+0xce)))-50.f)*.5f));
            float width=single(float(int16_t(half(s,first+0xcc)))-24.f);if(width<0)width=0;
            const float q=-std::fma(1.5f,width,-real(s,b+0x2778));if(p[1]<0)p[1]=0;else if(p[1]>100)p[1]=100;const float t=single(-q/9.f);
            if(!move(b,c,{p[0],single(single(110.f+p[1])+t),single(750.f-q)}, {p[0],single(single(60.f+p[1])+t),-q},true,true,de,da,s))return false;
        }
        if(missing)setBattleCameraMode(b,0,0,s);
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
    if(!(word(s,b+0x2754)&3) && services.rumbleOff)services.rumbleOff(services.user,0);
    if((word(s,b+0x2754)&0x40) && services.rumbleOff)services.rumbleOff(services.user,0);
    cursor={true,0,0};
    }
    return tremor(b,c,services,cursor);
}
}
