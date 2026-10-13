#pragma once
#include "npc_presentation.hpp"
#include "field_wall.hpp"
namespace ttyd {
// Source-unit semantic inputs. Registry lifetime/event pointers are native tokens.
struct NpcTalkBody {
    uintptr_t token=0;
    FieldWallVector root;
    float width=0,height=0;
    uint32_t flags=0;
    bool talkEvent=false;
};
struct NpcTalkPlayer {
    FieldWallVector root;
    float width=20,height=37,displayTurn=0,cameraSnapshot=0,viewYaw=0;
    uint16_t motion=0;
    uint32_t flags=0,viewFlags=0,partyFlags=0;
    int8_t keyDepth=0,partyType=0;
    int backgroundMode=0,controlMode=0;
    bool talkable() const;
    float displayDirection() const;
};
struct NpcTalkScan {
    uintptr_t candidate=0,nearActor=0;
    float distance=1000;
    bool consider(const NpcTalkBody&,const NpcTalkPlayer&,const NpcAngleTable&,const FieldWallServices&);
};
// fbatEncountCheck's visited actor result. Skipped scan gates must not call
// this owner. Event handles are supplied by the native registry/scheduler.
struct NpcTalkTurn {
    uint32_t state=0,secondaryFlags=0;
    float yaw=0,savedYaw=0;
};
struct NpcTalkTurnServices {
    void* user=nullptr;
    void (*stopRegular)(void*)=nullptr;
    void (*startRegular)(void*)=nullptr;
};
void updateNpcTalkTurn(NpcTalkTurn&,bool admitted,uint32_t mainFlags,
                      FieldWallVector npc,FieldWallVector mario,
                      const NpcAngleTable&,const NpcTalkTurnServices&);
// npcCheckInterrupt's fallthrough when all six script slots are proven null.
// Caller owns the flags1/2 NPC-main gate and invokes this before candidate scan.
void consumeNpcNullInterrupts(uint32_t& secondaryFlags,uint32_t& state);
}
