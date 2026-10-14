#pragma once
#include "shadow_display.hpp"
#include "battle_act_draw.hpp"
namespace ttyd {
struct MapTevInput {int32_t slots=0;uint32_t mode=0,flags=0;BattleActMatrix model{};};
struct MapTevCounters {uint32_t stages=1,maps=0,generators=0,indirect=0;};
struct MapTevServices {
    void* user=nullptr;
    bool (*gx)(void*,const ShadowGXRequest&)=nullptr;
    // Real clock/projected-source/SDK/callback continuation owns these source
    // blocks. A missing provider holds; no projected effect is silently skipped.
    bool (*branch)(void*,uint32_t entry,const MapTevInput&,MapTevCounters&,uint32_t& pending)=nullptr;
};
struct MapTevCursor {
    bool active=false,queued=false,callbackReady=false;uint32_t phase=0,stage=0,step=0,pending=0;
    MapTevInput input;MapTevCounters counts,callbackCounts;ShadowGXRequest request;
};
// Original fixed bases, ordinary/no-texture/generic bases, constant stage,
// ordered optional dependencies and low-byte final publication.
bool resumeMapMaterialTev(const MapTevInput&,const MapTevServices&,MapTevCursor&);
}
