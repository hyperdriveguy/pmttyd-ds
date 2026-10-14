#pragma once
#include <algorithm>
#include <cmath>
#include <cstdint>
#include "field_camera_events.hpp"
namespace ttyd {
// Original gor evt_mezame WW0(depth)/WW1(eyelid) requests. The native clock
// adapts source WAIT_MSEC/one-frame interpolation publication to60Hz.
struct OpeningWake {
    uint64_t now=0,deadline=0,effectsAt=0;
    unsigned phase=0,messageIndex=0;
    bool ready=false,complete=false;
    float depth=50,eyelid=240,viewShiftY=0;
    uint64_t simulationTick=0;
    static float publish(float value){return std::trunc(value*1024.f)/1024.f;}
    static float value(unsigned curve,uint64_t elapsed,unsigned duration,float a,float b) {
        const float raw=*cameraInterpolation(curve,int32_t(std::min<uint64_t>(elapsed,duration)),duration,a,b);
        return publish(publish(raw)/10.f);
    }
    void begin(uint64_t time){*this={};now=time;simulationTick=time*60/1000;deadline=time+4000;}
    bool messageReady(unsigned index) const{return ready && messageIndex==index;}
    void messageComplete(unsigned index) {
        if(!messageReady(index))return;
        ready=false;
        if(index==0){phase=1;deadline=now+2000;}
        else if(index==1){phase=2;effectsAt=now+300;deadline=effectsAt+7800;}
        else {phase=3;deadline=now+2300;}
    }
    void tick(uint64_t time) {
        if(time<now || complete)return;
        now=time;
        while(simulationTick<time*60/1000){++simulationTick;viewShiftY=std::fma(.30000001192092896f,std::sin(float(simulationTick)/50.f),viewShiftY);}
        if(phase==2 && now>=effectsAt) {
            const uint64_t t=now-effectsAt;
            if(t<2000)eyelid=value(11,t,2000,2400,1200);
            else if(t<2100)eyelid=value(1,t-2000,100,1200,2400);
            else if(t<2200)eyelid=value(1,t-2100,100,2400,1200);
            else if(t<2300)eyelid=value(1,t-2200,100,1200,2400);
            else if(t<2800)eyelid=240;
            else {eyelid=value(1,t-2800,2000,2400,0);depth=value(1,t-2800,2000,500,0);}
        }
        if(!ready && now>=deadline) {
            if(phase==3)complete=true;
            else {messageIndex=phase;ready=true;}
        }
    }
};
}
