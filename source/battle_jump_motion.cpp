#include "battle_jump_motion.hpp"
#include <cassert>
#include <cmath>

namespace ttyd {
namespace {
constexpr float Pi=3.1415927410125732f,TwoPi=6.2831854820251465f;
uint32_t word(const uint8_t* p) {
    return uint32_t(p[0])<<24|uint32_t(p[1])<<16|uint32_t(p[2])<<8|p[3];
}
void word(uint8_t* p,uint32_t v) {p[0]=v>>24;p[1]=v>>16;p[2]=v>>8;p[3]=v;}
void halfCopy(uint8_t* d,const uint8_t* s) {
    const uint16_t v=uint16_t(s[0])<<8|s[1];d[0]=v>>8;d[1]=v;
}
void halfZero(uint8_t* p) {p[0]=p[1]=0;}
float add(float a,float b) {return a+b;}
float sub(float a,float b) {return a-b;}
float mul(float a,float b) {return a*b;}
float div(float a,float b) {return a/b;}
int32_t decrement(int32_t n) {
    const uint32_t v=static_cast<uint32_t>(n)-1;
    return v<=0x7fffffff?static_cast<int32_t>(v):static_cast<int32_t>(int64_t(v)-0x100000000LL);
}
int32_t wrappedAdd(int32_t a,int32_t b) {
    const uint32_t v=static_cast<uint32_t>(a)+static_cast<uint32_t>(b);
    return v<=0x7fffffff?static_cast<int32_t>(v):static_cast<int32_t>(int64_t(v)-0x100000000LL);
}
float sine(const JumpMotionServices& s,float angle) {
    assert(s.sin);return static_cast<float>(s.sin(s.user,static_cast<double>(angle)));
}
float radians(float angle) {return div(mul(TwoPi,angle),360.f);}
void advancePhase(MarioJumpUnit& u,int32_t mode,const JumpMotionServices& s) {
    const float q=sine(s,radians(u.angle));
    const double factor=std::fma(mode<3?0.53:0.8,static_cast<double>(q),1.0);
    u.phase=static_cast<float>(std::fma(static_cast<double>(u.phaseIncrement),factor,static_cast<double>(u.phase)));
}
JumpVector get(MarioJumpUnit& u,const JumpMotionServices& s) {
    assert(s.getPosition);return s.getPosition(s.user,u);
}
void set(MarioJumpUnit& u,JumpVector p,const JumpMotionServices& s) {
    assert(s.setPosition);s.setPosition(s.user,u,p);
}
}
void loadUnitSoundMode(int32_t type,int32_t mode,const uint8_t* source,uint8_t* d) {
    assert(d);halfZero(d+0x14);word(d+0x18,0xffffffff);
    if(mode==-1) {
        unsigned offset=0;bool valid=true,parameters=true;
        switch(type) {
        case 0:offset=8;break;
        case 1:offset=0x18;break;
        case 2:offset=0;parameters=false;break;
        case 3:offset=0x28;break;
        default:valid=false;break;
        }
        if(valid) {
            assert(source);word(d,word(source+offset));word(d+4,word(source+offset+4));
            if(parameters) {
                halfCopy(d+0xc,source+offset+8);halfCopy(d+0xe,source+offset+10);
                halfCopy(d+0x10,source+offset+12);
            } else {halfZero(d+0xc);halfZero(d+0xe);halfZero(d+0x10);}
        } else {word(d,0);word(d+4,0);halfZero(d+0xc);halfZero(d+0xe);halfZero(d+0x10);}
    } else if(mode!=-2) {
        word(d,0);word(d+4,0);halfZero(d+0xc);halfZero(d+0xe);halfZero(d+0x10);
    }
    halfCopy(d+0x12,d+0xc);
}
float reviseJumpAngle(float input,const JumpMotionServices& s) {
    assert(s.fmod);float result=static_cast<float>(s.fmod(s.user,input,360.0));
    if(std::isnan(result))result=0.f;
    if(result<0.f) {result=add(result,360.f);if(result>=360.f)result=0.f;}
    return result;
}
void setMoveCurrentPosition(MarioJumpUnit& u,JumpVector p) {
    if(p.x!=-250000000.f)u.baseline.x=p.x;
    if(p.y!=-250000000.f)u.baseline.y=p.y;
    if(p.z!=-250000000.f)u.baseline.z=p.z;
}
void setMoveTargetPosition(MarioJumpUnit& u,JumpVector p) {
    if(p.x!=-250000000.f)u.target.x=p.x;
    if(p.y!=-250000000.f)u.target.y=p.y;
    if(p.z!=-250000000.f)u.target.z=p.z;
}
int marioJumpMotion(MarioJumpUnit& u,MarioJumpEvent& e,const MarioJumpArguments& a,
                    bool firstCall,const JumpMotionServices& s) {
    const JumpVector input{a.x,a.y,a.z};
    if(firstCall) {
        e.phase=1;
        if(a.mode==1)e.phase=2;
        else {
            setMoveTargetPosition(u,input);
            const auto current=get(u,s);setMoveCurrentPosition(u,current);
            assert(s.heading && s.distance);
            u.heading=s.heading(s.user,current.x,current.z,input.x,input.z);
            u.distance=s.distance(s.user,current.x,current.z,input.x,input.z);
            const float dx=sub(input.x,current.x);
            if(dx>0)u.direction=1;else if(dx<0)u.direction=-1;
            u.remaining=a.count;
            if(u.remaining==0) {
                assert(s.truncate);u.remaining=s.truncate(s.user,div(u.distance,u.speed));
            } else u.speed=div(u.distance,static_cast<float>(u.remaining));
            const float n=static_cast<float>(u.remaining);
            const float residual=std::fma(-n,u.speed,u.distance);
            if(u.remaining==0)return 2;
            u.delta={div(sub(u.target.x,u.baseline.x),n),div(sub(u.target.y,u.baseline.y),n),
                     div(sub(u.target.z,u.baseline.z),n)};
            u.phase=0.f;u.phaseIncrement=div(Pi,n);u.speed=add(u.speed,div(residual,n));
            u.angle=90.f;
            u.angleStep=a.mode<3?div(360.f,n):static_cast<float>(360/u.remaining);
            u.height=static_cast<float>(static_cast<double>(u.distance)-20.0);
            u.height=static_cast<float>(static_cast<double>(u.height)/6.0);
            u.height=static_cast<float>(static_cast<double>(u.height)+47.0);
            if(a.mode==2)u.height=static_cast<float>(static_cast<double>(u.height)*1.12);
            else if(a.mode==4)u.height=static_cast<float>(static_cast<double>(u.height)*1.25);
            advancePhase(u,a.mode,s);
            loadUnitSoundMode(2,a.soundMode,u.soundSource.data(),u.soundMode.data());
            const uint32_t sound=word(u.soundMode.data());
            if(sound) {assert(s.playSound);s.playSound(s.user,sound,u.baseline);}
        }
    }
    switch(e.phase) {
    case 1: {
        u.baseline.x=add(u.baseline.x,u.delta.x);
        u.baseline.y=add(u.baseline.y,u.delta.y);
        u.baseline.z=add(u.baseline.z,u.delta.z);
        auto p=get(u,s);u.lastY=p.y;
        p={u.baseline.x,std::fma(u.height,sine(s,u.phase),u.baseline.y),u.baseline.z};
        set(u,p,s);
        if(u.target.y>p.y && u.remaining<3) {p.y=u.target.y;set(u,p,s);}
        u.lastY=p.y;advancePhase(u,a.mode,s);
        u.angle=reviseJumpAngle(add(u.angle,u.angleStep),s);
        u.remaining=decrement(u.remaining);
        if(u.remaining!=0)return 0;
        p.y=u.target.y;set(u,p,s);
        u.phase=-sub(u.lastY,u.lastY);u.phaseIncrement=1.7999999523162842f;
        return 1;
    }
    case 2:
        assert(s.eligible);
        if(!s.eligible(s.user,u))return 2;
        u.phase=-sub(u.lastY,u.lastY);u.phaseIncrement=1.7999999523162842f;
        u.remaining=1;u.angle=90.f;u.angleStep=static_cast<float>(360/u.remaining);
        u.height=static_cast<float>(static_cast<double>(std::fabs(sub(u.lastY,u.lastY)))/16.5);
        get(u,s);e.phase=3;return 0;
    case 3: {
        const float q1=sine(s,radians(u.angle));
        const float product=mul(u.height,q1);
        u.baseline.x=static_cast<float>(static_cast<double>(u.baseline.x)+static_cast<double>(product)/33.0);
        const float q2=sine(s,radians(u.angle));
        u.baseline.y=std::fma(-u.height,q2,u.baseline.y);
        u.angle=reviseJumpAngle(add(u.angle,u.angleStep),s);
        set(u,u.baseline,s);u.remaining=decrement(u.remaining);
        return u.remaining==0?1:0;
    }
    default:return 0;
    }
}
int marioJumpMotionEvent(MarioJumpEvent& e,const std::array<int32_t,7>& operands,
                         bool firstCall,const JumpEventServices& event,const JumpMotionServices& motion) {
    assert(event.resolve && event.translateActor && event.unit);
    MarioJumpArguments a;
    a.actor=event.translateActor(event.user,event.resolve(event.user,operands[0]));
    a.x=static_cast<float>(event.resolve(event.user,operands[1]));
    a.y=static_cast<float>(event.resolve(event.user,operands[2]));
    a.z=static_cast<float>(event.resolve(event.user,operands[3]));
    a.count=event.resolve(event.user,operands[4]);
    a.mode=event.resolve(event.user,operands[5]);a.soundMode=event.resolve(event.user,operands[6]);
    auto* u=event.unit(event.user,a.actor);assert(u);
    return marioJumpMotion(*u,e,a,firstCall,motion);
}
int32_t marioJumpCount(MarioJumpUnit& u,float x,float z,int32_t requested,
                       const JumpMotionServices& s) {
    const auto current=get(u,s);assert(s.distance);
    const float distance=s.distance(s.user,current.x,current.z,x,z);
    int32_t candidate=requested;
    if(requested==0) {assert(s.truncate);candidate=s.truncate(s.user,div(distance,u.speed));}
    if(distance>60.f) {
        assert(s.truncate);
        const float extra=mul(sub(distance,60.f),0.15000000596046448f);
        candidate=wrappedAdd(candidate,s.truncate(s.user,extra));
    }
    return requested==0 || candidate>requested?candidate:requested;
}
int marioJumpParameterEvent(const std::array<int32_t,5>& operands,
                            const JumpEventServices& event,const JumpMotionServices& motion) {
    assert(event.resolve && event.translateActor && event.unit);
    const int32_t actor=event.translateActor(event.user,event.resolve(event.user,operands[0]));
    const float x=static_cast<float>(event.resolve(event.user,operands[1]));
    event.resolve(event.user,operands[2]); // Required read, discarded integer Y.
    const float z=static_cast<float>(event.resolve(event.user,operands[3]));
    const int32_t destination=operands[4];
    const int32_t requested=event.resolve(event.user,destination);
    auto* u=event.unit(event.user,actor);assert(u);
    const int32_t count=marioJumpCount(*u,x,z,requested,motion);
    if(count!=-250000000) {assert(event.store);event.store(event.user,destination,count);}
    return 2;
}
}
