#pragma once
#include <algorithm>
#include <array>
#include <cstdint>
namespace ttyd {
// Used ordinary group0 (mode512) branch. A new On replaces group ownership;
// its old SoundSS stream receives the source default750ms floating tail.
class OpeningMapBgm {
public:
    struct Snapshot{std::array<int,3> tracks{{-1,-1,-1}};std::array<uint8_t,3> volumes{{0,0,0}};};
    void begin(uint64_t tick){*this={};now=tick;}
    void advance(uint64_t tick){
        if(tick<now)return;
        for(;now<tick;++now){
            if(fading && remaining)--remaining;
            if(tail>=0){if(tailVolume==0)tail=-1;else tailVolume=std::max(0.f,tailVolume-tailDecrement);}
        }
    }
    void on(int track,bool originalOnRequest=false){
        if(track==primary && !(originalOnRequest && fading))return;
        if(primary>=0 && volume()){tail=primary;tailVolume=volume();tailDecrement=tailVolume/45.f;}
        primary=track;base=track==0?70:track==7?80:track==8?86:track==9?78:127;fading=false;remaining=0;
    }
    void off(uint32_t milliseconds=2000){if(primary>=0 && !fading){base=volume();fading=true;duration=std::max<uint32_t>(1,milliseconds*60/1000);remaining=duration;}}
    Snapshot snapshot()const{
        Snapshot out;out.tracks[0]=fading && !remaining?-1:primary;out.volumes[0]=volume();
        if(tail>=0){out.tracks[1]=tail;out.volumes[1]=uint8_t(tailVolume);}return out;
    }
private:
    uint64_t now=0;int primary=-1,tail=-1;uint32_t base=0,remaining=0,duration=120;
    float tailVolume=0,tailDecrement=0;bool fading=false;
    uint8_t volume()const{return uint8_t(fading?base*remaining/duration:base);}
};
}
