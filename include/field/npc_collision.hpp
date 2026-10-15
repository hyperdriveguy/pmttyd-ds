#pragma once
#include "npc_presentation.hpp"
#include "field_wall.hpp"
namespace ttyd {
// Engine dimensions/root with borrowed actor token. previous XZ is a retained
// rollback owner, not necessarily the previous rendered position.
struct NpcSideBody {
    uintptr_t token=0;
    FieldWallVector root,previous;
    float width=0,height=0;
    uint32_t flags=0,state=0;
};
struct NpcSidePlayer {
    FieldWallVector root;
    float width=20,height=37,collisionWidth=20,travelHeading=0;
    uintptr_t hitNpc=0;
};
struct NpcSideWallRequest {int32_t mode=0;float speed=0,heading=0,width=20,height=37;};
// A valid collision must call wall correction; no silent optional no-op.
struct NpcSideServices {
    void* user=nullptr;
    void (*wall)(void*,FieldWallVector&,const NpcSideWallRequest&)=nullptr;
};
struct NpcSideTrig {float sine=0,cosine=1;};
float npcCompAngle(float from,float to);
NpcSideTrig npcSideTrig(float degrees);
// Original point sphere query, independent of caller flags/talk admission.
bool npcHitSphere(const NpcSideBody&,FieldWallVector point,float extraRadius,float& distance);
// Direct NPC-main gates/state publication; timer producers are separate owners.
bool npcSideUpdateAdmitted(NpcSideBody&);
// Wall service is required on admitted contact. Previous XZ rollback persists on a miss.
bool respondNpcSide(NpcSideBody&,NpcSidePlayer&,const NpcAngleTable&,const NpcSideServices&);
}
