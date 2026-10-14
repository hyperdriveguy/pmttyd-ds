#pragma once
#include "field_camera_events.hpp"
#include <array>
#include <cmath>
#include <cstdint>
#include <limits>
namespace ttyd {
// Original muj_20 logo work: file,+4 Y offset,+8 alpha,+c state,+10 counter.
// Its script/global frame rate is60 here; counters advance twice per callback.
struct OpeningVoyageLogo {
    uint64_t started=0,lastTick=UINT64_MAX,acceptedTick=UINT64_MAX;
    uint32_t state=0,counter=0;
    int32_t alpha=0;float yOffset=0;
    bool active=false,accepted=false,complete=false;
    mutable bool resourcesReady=false;
    void begin(uint64_t tick){*this={};started=tick;active=true;}
    void admitResources() const {resourcesReady=true;}
    void advance(uint64_t tick,uint32_t keys,bool eligible){
        if(!active || complete || tick<started || lastTick==tick || (lastTick!=UINT64_MAX && tick<lastTick))return;
        lastTick=tick;
        if(!resourcesReady)return; // Original fileAsyncf admission precedes every state.
        switch(state){
        case 0:alpha=0;yOffset=0;counter=0;state=1;break;
        case 1:
            if(counter<180){
                alpha=int32_t(*cameraInterpolation(11,int32_t(++counter),180,0,255));
                yOffset=*cameraInterpolation(11,int32_t(++counter),180,-50,0);
            }else if(tick%15==0)state=2;
            break;
        case 2:
            if(eligible && (keys&0x1300)){counter=0;state=3;accepted=true;acceptedTick=tick;}
            break;
        case 3:
            if(counter<120){
                alpha=int32_t(*cameraInterpolation(11,int32_t(++counter),120,255,0));
                yOffset=*cameraInterpolation(11,int32_t(++counter),120,0,100);
            }else state=4;
            break;
        case 4:active=false;complete=true;break;
        }
    }
};
struct OpeningVoyageLocalTransform {std::array<float,3> position{};float rotationZ=0;};
inline OpeningVoyageLocalTransform openingVoyageLocalTransform(const std::array<float,3>& global,float theta){
    // Original callback unk_muj_000045a8: translate(-6000Y), rotateZ,
    // translate(+6000Y), then multiply the original GW0/1/2 point.
    const float phi=(5.f*global[0])/-6000.f;
    const float radians=phi*.01745329238474369f;
    const float c=std::cos(radians),s=std::sin(radians);
    const float offsetX=-s*6000.f,offsetY=c*6000.f-6000.f;
    // Four evtSetFloat writes publish through signedQ1024 event words.
    auto publish=[](float value){return std::trunc(value*1024.f)/1024.f;};
    return {{{publish((c*global[0]-s*global[1])+offsetX),publish((s*global[0]+c*global[1])+offsetY),publish(global[2])}},publish(theta+phi)};
}
// Uninvoked muj pressStartGX helper retained for source evidence; no invented
// display registration. Original palette15 mask + palette6 scrolling pattern.
inline std::array<float,2> voyagePromptSourcePoint(float u,float v,float width,float height,float x,float y){return {{x+(u-.5f)*width,y+(.5f-v)*height}};}
inline std::array<float,2> voyagePromptPatternPoint(uint32_t globalFrame,float u,float v){return {{u+float(globalFrame)/-200.f,v}};}
}
