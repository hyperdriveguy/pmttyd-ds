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
    void consider(const NpcTalkBody&,const NpcTalkPlayer&,const NpcAngleTable&,const FieldWallServices&);
};
}
