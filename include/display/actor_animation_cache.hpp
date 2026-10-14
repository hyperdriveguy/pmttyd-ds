#pragma once
#include "actor_animation.hpp"
namespace ttyd {
struct ActorAnimationCache {
    std::vector<ActorVector> positions,normals;
    std::vector<uint8_t> visibility,textures;
    std::vector<float> nodes;
    std::vector<ActorSurfaceState> surfaces;
};
struct ActorAnimationCacheStores {
    void* user=nullptr;
    void (*index)(void*,int32_t)=nullptr;
    void (*fraction)(void*,float)=nullptr;
};
void allocateActorAnimationCache(const ActorAnimationPose&,ActorAnimationCache&);
bool sampleActorAnimationCache(const ActorAnimationPose&,ActorAnimationCache&,ActorAnimationSample&,
    int32_t current,int32_t next,float fraction,int32_t previous,float previousFraction,
    ActorAnimationCacheStores={});
}
