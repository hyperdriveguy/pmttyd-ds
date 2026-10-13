#pragma once
#include "field_wall.hpp"
#include <cstdint>
namespace ttyd {
// Source Event+c4 work. Units and frame/RNG providers belong to their callers.
struct NpcWanderWork {
    uint32_t flags=0,phase=0,count=0,flips=0;
    int32_t moveFrames=0,idleFrames=0,flipFrames=0;
    FieldWallVector anchor;
    float radius=0;
};
static_assert(sizeof(NpcWanderWork)==44,"Original wander work fields");
struct NpcWanderActor {
    uintptr_t token=0;
    FieldWallVector root,components;
    float yaw=0,moveHeading=0,speed=0,dt=0,stepElapsed=0;
    uint32_t flags=0,soundFlags=0,materialFlags=0;
    uintptr_t wall=0,sounds[2]{};
    uint16_t stepPeriod=0;
    int16_t stepToggle=0;
};
struct NpcWanderMario {FieldWallVector root;float travelHeading=0;uintptr_t contact=0;};
struct NpcWanderServices {
    void* user=nullptr;
    int32_t (*frames)(void*,int32_t milliseconds)=nullptr;
    int32_t (*random)(void*,int32_t upperExclusive)=nullptr;
    bool (*turnReady)(void*,const NpcWanderActor&)=nullptr;
    float (*angle)(void*,FieldWallVector from,FieldWallVector to)=nullptr;
    float (*distance)(void*,FieldWallVector from,FieldWallVector to)=nullptr; // Horizontal source distABf (XZ).
    float (*sine)(void*,float radians)=nullptr;
    float (*cosine)(void*,float radians)=nullptr;
    void (*sound)(void*,uintptr_t sourceSound,uint32_t mask,FieldWallVector root)=nullptr;
    // Sound provider resolves the source name and submits (ID|mask,root,255,255,0,8).
};
void initializeNpcWander(NpcWanderWork&,NpcWanderActor&,float anchorX,float anchorZ,
                         float radius,float speed,uint32_t flags,const NpcWanderServices&);
// Original USER_FUNC remains blocked (return0); this does not consume components
// or move roots. NPC-main, event pause/lifetime and auto-pose are separate owners.
int tickNpcWander(NpcWanderWork&,NpcWanderActor&,const NpcWanderMario&,const NpcWanderServices&);
bool npcWanderTurnReady(uint32_t poseFlags,float retained,float target);
int32_t npcWanderFrames(int32_t milliseconds,int32_t liveFrameRate);
int32_t npcWanderRandom(uint32_t& sharedSeed,int32_t upperExclusive);
}
