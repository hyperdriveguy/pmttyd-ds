#pragma once
#include <cstdint>
namespace ttyd {
// aaa evt_prologue2 / mapdraw. One source callback per simulation tick;
// resource admission and the opaque frame publication belong to the renderer.
class OpeningMapDisplay {
public:
    uint8_t alpha=0;
    bool drawRequested=false,resourceReady=false,opaqueRequested=false,opaquePresented=false;
    bool flag0=false,musicOffRequested=false,environmentOffRequested=false,complete=false;
    unsigned poseStage=0;
    void begin(uint64_t tick){*this={};now=start=tick;}
    void admitted(){if(drawRequested && !resourceReady){resourceReady=true;alpha=2;lastDraw=now;}}
    void presented(uint8_t value){if(opaqueRequested && value==255)opaquePresented=true;}
    void tick(uint64_t tick){
        if(tick<now)return;
        now=tick;
        if(!drawRequested){const auto elapsed=now-start;poseStage=elapsed<18?0:elapsed<78?1:2;if(elapsed>=168)drawRequested=true;}
        if(resourceReady && !flag0){
            const auto steps=now-lastDraw;lastDraw=now;
            const auto value=uint64_t(alpha)+2*steps;
            if(value>255){alpha=255;opaqueRequested=true;}else alpha=uint8_t(value);
            if(opaquePresented){flag0=true;flagTick=now;}
        }
        if(flag0 && now-flagTick>=60){musicOffRequested=true;environmentOffRequested=true;complete=true;}
    }
private:
    uint64_t now=0,start=0,lastDraw=0,flagTick=0;
};
}
