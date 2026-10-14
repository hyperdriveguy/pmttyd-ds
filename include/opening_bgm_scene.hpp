#pragma once
#include <algorithm>
#include <cstdint>
namespace ttyd {
// Original US demo requests plus the bounded ordinary streaming-volume branch.
// Simulation60Hz adapts source work->frameRate; no audible/Gekko parity claim.
class OpeningBgmScene {
public:
    struct Snapshot {
        int primaryTrack=5,tailTrack=-1;
        uint8_t primaryVolume=90,tailVolume=0;
        bool primaryFadeActive=false;uint32_t primaryFadeBase=90,primaryFadeDuration=360,primaryFadeRemaining=360;
        float tailFloatVolume=0,tailFloatDecrement=30.f/45.f;
        bool tailActive=false,offRequested=false,fadeInRequested=false;
        uint32_t sourceMode=0x201,offMilliseconds=6000,defaultOnMilliseconds=1000,replacementMilliseconds=750;
    };
    void begin(uint64_t tick){now=tick;transitionStart=0;transitioned=false;}
    void advance(uint64_t tick){if(tick>=now)now=tick;}
    void transition(){if(!transitioned){transitioned=true;transitionStart=now;}}
    Snapshot snapshot() const {
        Snapshot out;out.offRequested=transitioned;
        if(!transitioned)return out;
        const auto elapsed=now-transitionStart;
        if(elapsed<240){out.primaryFadeActive=true;out.primaryFadeRemaining=360-elapsed;out.primaryVolume=uint8_t(90*(360-elapsed)/360);return out;}
        out.primaryTrack=6;const auto replacement=elapsed-240;
        out.primaryVolume=77; // mode0x201 does not admit fade-in gate0x100.
        // psndBGMMain's remaining/duration integer publication gives30 at4s.
        // SoundSSFadeoutCh snapshots that current volume, divides by45 then
        // SoundSSMain subtracts the float decrement before truncating volume.
        float volume=30.f;const float decrement=volume/45.f;
        bool active=true;
        for(uint64_t i=0;i<std::min<uint64_t>(replacement,48);++i){if(volume==0){active=false;break;}volume=std::max(0.f,volume-decrement);}
        out.tailActive=active;out.tailTrack=out.tailActive?5:-1;out.tailVolume=uint8_t(volume);out.tailFloatVolume=volume;
        return out;
    }
private:
    uint64_t now=0,transitionStart=0;
    bool transitioned=false;
};
}
