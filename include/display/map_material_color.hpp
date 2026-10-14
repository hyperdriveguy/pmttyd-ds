#pragma once
#include "shadow_display.hpp"
namespace ttyd {
// Current map-bank work projection. Source cold clear zeros these bytes;
// authored fog and subsequent EVT writers own changes independently.
struct MapMaterialGroup {
    uint16_t flags=0;
    std::array<uint32_t,3> colors{};
    uint32_t fogType=0,fogColor=0;
    float fogStart=0,fogEnd=0;
};
struct MapMaterialColorServices {
    void* user=nullptr;
    const MapMaterialGroup* (*group)(void*)=nullptr;
    const CameraState* (*currentCamera)(void*)=nullptr;
    bool (*gx)(void*,const ShadowGXRequest&)=nullptr;
};
struct MapMaterialColorCursor {
    bool active=false,ready=false;uint32_t function=0,pending=0,flags=0,c1=0,c2=0;
    const CameraState* camera=nullptr;
    ShadowGXRequest request;
};
// Each arithmetic stage truncates separately. C2 RED controls alpha.
uint32_t mapMaterialLastStageColor(uint32_t flags,uint32_t c1,uint32_t c2,const MapMaterialGroup&);
// Fog resolves current camera BEFORE map group, even when disabled. Held GX
// requests retain captured values; no completed getter is replayed.
bool resumeMapMaterialColor(uint32_t function,uint32_t flags,uint32_t c1,uint32_t c2,
    const MapMaterialColorServices&,MapMaterialColorCursor&);
}
