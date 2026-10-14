#pragma once
#include <algorithm>
#include <cstdint>
namespace ttyd {
class OpeningMapTransition {
public:
    bool requested=false,resourcesReady=false,captureReady=false,exitComplete=false;
    bool entryRequested=false,entryAdmitted=false,entryComplete=false;
    float entryFrame=0;
    void begin(){*this={};requested=true;}
    void admitted(float exitEnd,float entryEnd){if(requested && exitEnd==0 && entryEnd==80)resourcesReady=true;}
    void captured(){if(requested && resourcesReady)captureReady=true;}
    void enter(){if(exitComplete)entryRequested=true;}
    void admitEntry(uint64_t tick){if(entryRequested && !entryAdmitted){entryAdmitted=true;entryStarted=tick;}}
    void advance(uint64_t tick){if(entryAdmitted && tick>=entryStarted)entryFrame=float(std::min<uint64_t>(tick-entryStarted,80));}
    void presented(unsigned type,float frame){
        if(resourcesReady && captureReady && type==49 && frame==0)exitComplete=true;
        if(resourcesReady && captureReady && entryAdmitted && type==50 && frame==80)entryComplete=true;
    }
private:
    uint64_t entryStarted=0;
};
}
