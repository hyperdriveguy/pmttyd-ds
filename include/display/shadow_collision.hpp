#pragma once
#include "shadow_work.hpp"

namespace ttyd {
// Separate source hitCheckVecFilter query, never a cached Mario support result.
// The provider must honor filter8002d380 and preserve raw hit point/normal.
struct ShadowCollisionRay {
    std::array<float,3> origin{},direction{0,-1,0};
    float length=150;
    uint32_t filter=0x8002d380;
};
struct ShadowCollisionHit {std::array<float,3> point{},normal{};};
enum class ShadowRayResult {Unavailable,Miss,Hit};
struct ShadowCollisionServices {
    void* user=nullptr;
    bool (*currentCamera)(void*)=nullptr; // Required source query; result unused.
    ShadowRayResult (*ray)(void*,const ShadowCollisionRay&,ShadowCollisionHit&)=nullptr;
};
struct ShadowCollisionCursor {
    bool active=false,cameraQueried=false,rayPending=false;
    ShadowWork* owner=nullptr;
    ShadowRecord* pool=nullptr;
    uint32_t index=0;
    ShadowCollisionRay ray{};
};
// True only after the full scan. A unavailable service retains its call inputs
// and cursor; successful earlier records are not replayed on resume.
bool shadowPrepareCollision(ShadowWork&,ShadowCollisionCursor&,const ShadowCollisionServices&);
}
