#pragma once
#include "field_camera_events.hpp"
#include "opening_heading_stardust.hpp"
#include <array>
#include <cstdint>
namespace ttyd {
// Used US sys display title0. Simulation cadence bridges the original display
// callback; actual resource/frame admissions remain separate native owners.
struct OpeningHeadingDisplay {
    struct Letter {float x=0,y=50,z=0,angle=90;int32_t delay=0,phase=0;uint32_t frame=0;uint8_t texture=0;};
    std::array<Letter,8> letters{};
    uint64_t tick=0;
    uint32_t state=0,palette=0,ringReveal=0;
    float backgroundAngle=0,ringRotation=0;
    std::array<bool,8> sparkleRequested{};
    OpeningHeadingStardust stardust;
    void begin(uint64_t at){
        *this={};tick=at;constexpr uint8_t order[]={19,20,21,22,21,23,24,25};
        for(unsigned i=0;i<8;++i)letters[i]={float(32*i+16)-128,50,0,90,int32_t(1+20*i),0,0,order[i]};
    }
    void admit(){if(!state)state=1;}
    bool start(){if(state!=1)return false;state=2;return true;}
    void advance(uint64_t at){
        if(at<tick)return;
        while(tick<at){++tick;stardust.step();
            if(state){backgroundAngle=backgroundAngle+2.5f;if(backgroundAngle>180){backgroundAngle=0;palette=(palette+1)%4;}}
            if(state!=2)continue;
            for(unsigned i=0;i<8;++i){auto& c=letters[i];
                if(c.phase==0){if(--c.delay<0){c.delay=0;c.phase=1;sparkleRequested[i]=true;stardust.spawn(i,c.x,c.y);}}
                else if(c.phase==1){c.angle=*cameraInterpolation(5,int32_t(c.frame),90,90,1800);if(++c.frame>90)c.phase=2;}
                else if(i==7){ringRotation=ringRotation+.8f;if(ringRotation>360)ringRotation=ringRotation-360;ringReveal=std::min<uint32_t>(ringReveal+48,0x4000);}
            }
        }
    }
};
}
