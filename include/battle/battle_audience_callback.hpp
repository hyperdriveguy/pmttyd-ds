#pragma once
#include "battle_actor.hpp"
#include "battle_audience_draw.hpp"
#include "camera_manager.hpp"
namespace ttyd {
struct BattleAudienceCallbackServices {
    BattleActorServices actors;void* user=nullptr;
    const CameraState* (*camera)(void*,uint32_t)=nullptr;
    const DisplayRecord* (*currentRecord)(void*)=nullptr;
    bool (*request)(void*,const BattleAudienceGXRequest&)=nullptr;
};
struct BattleAudienceCallbackCursor {
    bool active=false;uint32_t stage=0,step=0,index=0,pending=0,owner=0,record=0;
    const CameraState* camera=nullptr;
};
// Actual callback's retained prefix and complete empty-admission domain. An
// admitted actor remains unavailable; never infer an empty bank from a hold.
bool resumePackedBattleAudienceCallback(const BattleAudienceCallbackServices&,BattleAudienceCallbackCursor&);
}
