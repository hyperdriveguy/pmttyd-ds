#pragma once
#include "field_road.hpp"
#include <array>
#include <cmath>
#include <cstdint>
#include <optional>
#include <vector>
namespace ttyd {
struct FieldDoorRecord {
    uint32_t room=0,mask=0,openNode=0,closeNode=0;
    std::array<float,3> outside{},inside{}; // EVT integer XYZ, native scale.
    std::vector<uint32_t> interiorNodes;
};
bool loadFieldDoors(const char* path);
const std::vector<FieldDoorRecord>& fieldDoors();
// Case9: exact forward HitObj identity and admitted A, after the common case
// gates and NPC-talk veto. No proximity circle or airborne replay.
inline bool fieldDoorAdmitted(uint32_t contact,uint32_t registeredNode,
                              uint32_t keys,int motion,bool mounted,bool npcTalk,
                              const RoadCaseAdmission& gate) {
    return gate.permits() && !npcTalk && (keys&0x100) &&
        ((motion>=0 && motion<=2) || mounted) && contact && contact==registeredNode;
}
struct FieldDoorState {
    uint32_t interiorMask=0;
    int selected=-1,phase=0,wait=0;
    bool opening=false;
    std::array<float,2> position{},target{},step{};
    int remaining=0;
    bool active() const{return selected>=0;}
    void movement(std::array<float,2> start,const std::array<float,3>& end) {
        position=start;target={end[0],end[2]};
        const float x=target[0]-start[0],z=target[1]-start[1],d=std::hypot(x,z);
        // Original 135 units/sec request, adapted to fixed NTSC simulation.
        constexpr float speed=135.f/6000.f;
        remaining=int((d+.01f)/speed);
        step=d?std::array<float,2>{x*speed/d,z*speed/d}:std::array<float,2>{};
        if(!remaining)position=target;
    }
    void begin(int index,const FieldDoorRecord& door,std::array<float,2> root) {
        selected=index;opening=!(interiorMask&door.mask);phase=0;wait=0;
        if(opening){interiorMask|=door.mask;movement(root,door.outside);}
        else {position=root;remaining=0;wait=30;}
    }
    // Returns true only on completion. Door rotation/light/NPC/backend services
    // remain separate from this portable movement/wait bridge.
    bool tick(const FieldDoorRecord& door) {
        if(!active())return false;
        if(remaining){position[0]+=step[0];position[1]+=step[1];if(--remaining==0)position=target;return false;}
        if(wait && --wait)return false;
        if(phase==0) {
            phase=1;
            if(opening)wait=30;else movement(position,door.outside);
        } else if(phase==1) {
            phase=2;
            if(opening)movement(position,door.inside);else wait=30;
        } else if(phase==2) {
            phase=3;
            if(opening)wait=30;
            else {interiorMask&=~door.mask;wait=2;}
        } else if(phase==3 && opening) {phase=4;wait=2;}
        else {selected=-1;return true;}
        return false;
    }
};
}
