#pragma once
#include <cstdint>

namespace ttyd {
// A pass-local cache: every first draw establishes state regardless of what
// UI, capture or actor rendering left behind. No draw reordering is permitted.
class MapDrawCache {
public:
    struct Changes { bool texture, cull, depth; };
    Changes apply(const void* texture, uint32_t wrapS, uint32_t wrapT,
                  uint32_t cull, uint32_t depthMask) {
        const Changes changed{
            !valid || texture!=lastTexture || wrapS!=lastS || wrapT!=lastT,
            !valid || cull!=lastCull,
            !valid || depthMask!=lastMask
        };
        valid=true;lastTexture=texture;lastS=wrapS;lastT=wrapT;
        lastCull=cull;lastMask=depthMask;
        return changed;
    }
private:
    bool valid=false;
    const void* lastTexture=nullptr;
    uint32_t lastS=0,lastT=0,lastCull=0,lastMask=0;
};
}
