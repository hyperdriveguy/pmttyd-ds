#pragma once
#include <cstdint>
namespace ttyd {
// Used sys_01/prologue event protocol. Actual resource, fade and display-work
// acknowledgments belong to the renderer; elapsed time cannot supply them.
struct OpeningHeading {
    uint64_t now=0,enteredAt=0,deadline=0;
    unsigned phase=0;
    bool entryRequested=false,entryComplete=false,displayRequested=false;
    bool displayReady=false,startFadeRequested=false,startFadeComplete=false;
    bool startRequested=false,started=false,inputReady=false;
    bool exitRequested=false,exitComplete=false,complete=false;
    void begin(uint64_t time){*this={};now=enteredAt=time;entryRequested=true;}
    void tick(uint64_t time,uint32_t keys) {
        if(time<now || complete)return;
        now=time;
        switch(phase) {
        case 0:if(entryComplete){displayRequested=true;deadline=now+3000;phase=1;}break;
        case 1:if(now>=deadline){deadline=now+2000;phase=2;}break;
        case 2:if(now>=deadline){startFadeRequested=true;phase=3;}break;
        case 3:if(startFadeComplete){startRequested=true;phase=4;}break;
        case 4:if(displayReady && started){deadline=now+3000;phase=5;}break;
        case 5:if(now>=deadline){inputReady=true;if(keys&0x1300){inputReady=false;exitRequested=true;phase=6;}}break;
        case 6:if(exitComplete)complete=true;break;
        }
    }
};
}
