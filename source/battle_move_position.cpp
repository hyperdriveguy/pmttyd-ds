#include "battle_move_position.hpp"
#include <cassert>
#include <cmath>

namespace ttyd {
namespace {
float add(float a,float b) {return a+b;}
float sub(float a,float b) {return a-b;}
float mul(float a,float b) {return a*b;}
float div(float a,float b) {return a/b;}
int32_t signedWord(uint32_t word) {
    return word<=0x7fffffff?static_cast<int32_t>(word):static_cast<int32_t>(int64_t(word)-0x100000000LL);
}
int32_t resolve(const JumpEventServices& s,int32_t raw) {assert(s.resolve);return s.resolve(s.user,raw);}
int32_t actor(const JumpEventServices& s,int32_t raw) {assert(s.translateActor);return s.translateActor(s.user,resolve(s,raw));}
MarioJumpUnit& unit(const JumpEventServices& s,int32_t id) {assert(s.unit);auto* u=s.unit(s.user,id);assert(u);return *u;}
}
void integrateMoveHorizontal(float* x,float* z,float speed,float heading,const JumpMotionServices& s) {
    assert(x && z && s.sin && s.cos);
    const float rad=div(mul(6.2831854820251465f,heading),360.f);
    const float sine=static_cast<float>(s.sin(s.user,rad));
    const float cosine=static_cast<float>(s.cos(s.user,rad));
    *x=add(*x,mul(speed,sine));
    *z=add(*z,mul(-speed,cosine));
}
int32_t queryMoveFrame(MarioJumpUnit& u,float x,float z,float requestedSpeed,const JumpMotionServices& s) {
    assert(s.getPosition && s.distance && s.truncate);
    const auto current=s.getPosition(s.user,u);
    const float distance=s.distance(s.user,current.x,current.z,x,z);
    float speed=requestedSpeed;
    if(speed==0.f)speed=u.speed;
    if(speed==0.f)speed=1.f;
    int32_t n=s.truncate(s.user,div(distance,speed));
    const float residual=std::fma(-static_cast<float>(n),speed,distance);
    if(residual>0.f)n=signedWord(static_cast<uint32_t>(n)+1);
    if(n==0)n=1;
    return n;
}
int queryMoveFrameEvent(const std::array<int32_t,6>& operands,const JumpEventServices& event,const JumpMotionServices& motion) {
    const int32_t id=actor(event,operands[0]);
    const float x=static_cast<float>(resolve(event,operands[1]));
    resolve(event,operands[2]); // Y is resolved but discarded, not evtGetFloat.
    const float z=static_cast<float>(resolve(event,operands[3]));
    assert(event.resolveFloat);const float speed=event.resolveFloat(event.user,operands[4]);
    const int32_t output=operands[5];
    const int32_t n=queryMoveFrame(unit(event,id),x,z,speed,motion);
    if(output!=-250000000) {assert(event.store);event.store(event.user,output,n);}
    return 2;
}
int movePosition(MarioJumpUnit& u,MarioJumpEvent& e,const MovePositionArguments& a,bool firstCall,
                 const JumpMotionServices& s,const MovementSoundServices& sound) {
    if(firstCall) {
        e.phase=1;u.remaining=a.count;assert(s.getPosition);
        const auto current=s.getPosition(s.user,u);
        setMoveCurrentPosition(u,current);setMoveTargetPosition(u,a.destination);
        assert(s.heading && s.distance);
        u.heading=s.heading(s.user,current.x,current.z,a.destination.x,a.destination.z);
        u.distance=s.distance(s.user,current.x,current.z,a.destination.x,a.destination.z);
        const float dx=sub(a.destination.x,current.x);
        if(dx>0.f)u.direction=1;else if(dx<0.f)u.direction=-1;
        if(u.remaining==0) {
            assert(s.truncate);u.remaining=s.truncate(s.user,div(u.distance,u.speed));
            if(u.remaining==0)u.remaining=1;
            const float n=static_cast<float>(u.remaining);
            const float residual=std::fma(-n,u.speed,u.distance);
            u.speed=add(u.speed,div(residual,n));
        } else u.speed=div(u.distance,static_cast<float>(u.remaining));
        loadUnitSoundMode(a.soundType,a.soundMode,u.soundSource.data(),u.soundMode.data());
    }
    integrateMoveHorizontal(&u.baseline.x,&u.baseline.z,u.speed,u.heading,s);
    assert(s.setPosition);s.setPosition(s.user,u,u.baseline);
    u.remaining=signedWord(static_cast<uint32_t>(u.remaining)-1);
    if(u.remaining>0) {assert(sound.control);sound.control(sound.user,u);return 0;}
    s.setPosition(s.user,u,u.target);
    assert(sound.end);sound.end(sound.user,u);return 1;
}
int movePositionEvent(MarioJumpEvent& e,const std::array<int32_t,7>& operands,bool firstCall,
                      const JumpEventServices& event,const JumpMotionServices& motion,const MovementSoundServices& sound) {
    const int32_t id=actor(event,operands[0]);MovePositionArguments a;
    a.destination.x=static_cast<float>(resolve(event,operands[1]));
    a.destination.y=static_cast<float>(resolve(event,operands[2]));
    a.destination.z=static_cast<float>(resolve(event,operands[3]));
    a.count=resolve(event,operands[4]);a.soundMode=resolve(event,operands[5]);a.soundType=resolve(event,operands[6]);
    return movePosition(unit(event,id),e,a,firstCall,motion,sound);
}
}
