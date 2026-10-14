#pragma once
#include "opening_bgm_scene.hpp"
#include <array>
#include <algorithm>
namespace ttyd {
// Source title group0 on + group0/group1 off. Detached SoundSS replacement
// owners keep their original floating fade, independent of group publication.
class OpeningTitleBgm {
public:
    struct Snapshot {std::array<int,3> tracks{{4,-1,-1}};std::array<uint8_t,3> volumes{{88,0,0}};};
    uint32_t onMode=512,entryOffMilliseconds=3000,idleOffMilliseconds=1000;
    void begin(const OpeningBgmScene::Snapshot& prior){
        *this={};groupTrack=prior.primaryTrack;groupBase=prior.primaryFadeActive?prior.primaryFadeBase:prior.primaryVolume;
        groupDuration=prior.primaryFadeActive?prior.primaryFadeDuration:180;
        groupRemaining=prior.primaryFadeActive?prior.primaryFadeRemaining:180;
        detachedTrack=prior.tailActive?prior.tailTrack:-1;detachedVolume=prior.tailFloatVolume;detachedDecrement=prior.tailFloatDecrement;
    }
    void tick(){
        if(groupRemaining)--groupRemaining;
        if(detachedTrack>=0){if(detachedVolume==0)detachedTrack=-1;else detachedVolume=std::max(0.f,detachedVolume-detachedDecrement);}
        if(idleOff && idleRemaining)--idleRemaining;
    }
    void offForIdle(){if(!idleOff){idleOff=true;idleRemaining=60;}}
    Snapshot snapshot() const {
        Snapshot out;out.volumes[0]=idleOff?uint8_t(88*idleRemaining/60):88;
        if(groupRemaining){out.tracks[1]=groupTrack;out.volumes[1]=uint8_t(groupBase*groupRemaining/groupDuration);}
        if(detachedTrack>=0){out.tracks[2]=detachedTrack;out.volumes[2]=uint8_t(detachedVolume);}
        return out;
    }
private:
    int groupTrack=-1,detachedTrack=-1;
    uint32_t groupBase=0,groupDuration=180,groupRemaining=0,idleRemaining=60;
    float detachedVolume=0,detachedDecrement=0;
    bool idleOff=false;
};
}
