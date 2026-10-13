#include "npc_wander.hpp"
#include "npc_collision.hpp"
#include "npc_presentation.hpp"
#include <cassert>
#include <cmath>
namespace ttyd {
namespace {
uint32_t duration(int32_t frames,const NpcWanderServices& s) {
    const int32_t half=frames/2;
    return uint32_t(half)+uint32_t(s.random(s.user,half+1));
}
void turn(NpcWanderActor& actor,float heading) {
    actor.yaw=npcReviseAngle(heading);actor.moveHeading=actor.yaw;
}
}
bool npcWanderTurnReady(uint32_t flags,float retained,float target) {
    return !(flags&8) || std::fabs(target-retained)<10.f;
}
int32_t npcWanderFrames(int32_t ms,int32_t rate) {
    return int32_t(uint32_t(ms)*uint32_t(rate))/1000;
}
int32_t npcWanderRandom(uint32_t& seed,int32_t upper) {
    if(upper==0)return 0;
    seed=seed*0x41c64e6du+12345u;
    return int32_t((seed>>16)&0x7fff)%upper;
}
void initializeNpcWander(NpcWanderWork& work,NpcWanderActor& actor,float x,float z,
                         float radius,float speed,uint32_t flags,const NpcWanderServices& s) {
    assert(s.frames);work={};work.flags=flags;
    work.moveFrames=s.frames(s.user,6000);work.idleFrames=s.frames(s.user,9000);work.flipFrames=s.frames(s.user,1500);
    work.anchor={x,actor.root.y,z};work.radius=radius;actor.speed=speed;actor.flags|=0x00400000;
}
int tickNpcWander(NpcWanderWork& w,NpcWanderActor& a,const NpcWanderMario& mario,const NpcWanderServices& s) {
    assert(s.random && s.turnReady && s.angle && s.distance && s.sine && s.cosine);
    const auto ready=[&]{return s.turnReady(s.user,a);};
    switch(w.phase) {
    case 0: {
        const uint32_t chosen=duration(w.moveFrames,s);
        // Source signed division truncates, then double subtraction retains the
        // multiple of20, not the remainder. Ordinary source durations are positive.
        w.count=uint32_t((int32_t(chosen)/20)*20);w.flips=0;
        turn(a,(a.yaw+float(s.random(s.user,60)))-30.f);w.phase=1;break;
    }
    case 1: {
        const float distance=s.distance(s.user,a.root,w.anchor);
        if(ready()) {
            if(distance>w.radius) {
                if(!(w.flags&1))w.flags|=3;
                if(w.flags&2){turn(a,s.angle(s.user,a.root,w.anchor));w.flags&=~2u;}
            } else if(w.flags&1)w.flags&=~3u;
        }
        if(ready() && mario.contact==a.token && std::fabs(npcCompAngle(a.moveHeading,mario.travelHeading))<90 &&
           std::fabs(npcCompAngle(s.angle(s.user,a.root,mario.root),mario.travelHeading))<90 && distance<w.radius) {
            turn(a,a.moveHeading-180.f);w.flags&=~3u;
        }
        if(ready() && a.wall){turn(a,s.angle(s.user,a.root,w.anchor));w.flags&=~3u;}
        --w.count;if(w.count==0){w.phase=2;break;}
        if(!ready())break;
        const float cap=(20.f*a.speed)/1000.f;
        float step=a.speed*a.dt;if(step>=cap)step=cap;
        const float radians=(6.2831854820251465f*a.moveHeading)/360.f;
        a.components.x=step*s.sine(s.user,radians);a.components.z=step*-s.cosine(s.user,radians);
        const uint32_t mask=(a.soundFlags&0x100) && !(a.materialFlags&0x1800)?0x10000:0;
        const float threshold=float(a.stepPeriod)/a.speed;
        if(a.stepElapsed>threshold) {
            const auto sound=a.sounds[a.stepToggle!=0?0:1];
            if(sound){assert(s.sound);s.sound(s.user,sound,mask,a.root);}
            a.stepElapsed=0;
            const uint8_t byte=uint8_t(1-int32_t(a.stepToggle));
            a.stepToggle=byte<128?int16_t(byte):int16_t(byte)-256; // Original byte sign extension before halfword store.
        }
        if(a.sounds[1] || a.stepToggle!=0)a.stepElapsed=std::fma(1000.f,a.dt,a.stepElapsed);
        break;
    }
    case 2:
        w.count=duration(w.idleFrames,s);turn(a,(a.yaw+float(s.random(s.user,60)))-30.f);w.flips=0;
        if(w.flags&0x10000)w.flips=2+uint32_t(s.random(s.user,1000))%3;
        w.phase=3;break;
    case 3:
        --w.count;
        if(w.count==0) {
            w.count=duration(w.flipFrames,s);
            if(w.flips==0)w.phase=0;
            else {--w.flips;a.yaw=npcReviseAngle(a.yaw+180.f);} // Move heading intentionally unchanged.
        }
        break;
    default:break;
    }
    return 0;
}
}
