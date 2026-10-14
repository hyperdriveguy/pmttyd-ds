#pragma once
#include "battle_pose_pool.hpp"
#include "battle_part_display.hpp"
namespace ttyd {
struct BattlePoseMapGroup {uint16_t flags=0;uint8_t primaryAlpha=255,secondaryAlpha=255;};
struct BattlePoseInnerContext {
    int32_t handle=-1;uint32_t descriptor=0,current=0,next=0,pass=0;float fraction=0;
    std::shared_ptr<const BattlePoseResource> resource;BattleActMatrix model{};
};
struct BattlePoseInnerServices {
    void* user=nullptr;
    std::optional<BattlePoseMapGroup> (*mapGroup)(void*)=nullptr;
    bool (*gx)(void*,const BattleAudienceGXRequest&)=nullptr;
    bool (*matrix)(void*,BattleAudienceMatrixRequest&)=nullptr;
    bool (*callback)(void*,uint32_t,uint32_t)=nullptr;
    bool (*main)(void*,int32_t)=nullptr;
    bool (*publish)(void*,const BattlePoseInnerContext&)=nullptr;
    // Each flush resolves CURRENT output/resource/count owners; the second
    // does not reuse pointers/counts from before the first source call.
    bool (*flush)(void*,unsigned,const BattlePoseInnerContext&)=nullptr;
    bool (*invalidateVertices)(void*)=nullptr;
    bool (*traverse)(void*,const BattlePoseInnerContext&)=nullptr;
    const struct AnimationDrawTraversalServices* traversal=nullptr;
    const struct AnimationShapeServices* shape=nullptr;
};
struct BattlePoseMapCursor {unsigned step=0;};
struct BattlePoseInnerCursor {
    BattlePoseMapCursor map;
    bool active=false,requestPending=false;uint32_t phase=0,step=0,pending=0,effectivePass=0,materialCallback=0,mapFlags=0,primary=0,secondary=0;
    BattlePartDrawRequest input;BattlePoseInnerContext context;
    BattleAudienceGXRequest gx;BattleAudienceMatrixRequest matrix;BattleActMatrix operand{};
};
// Source mapTestXLU predicate order; each map branch resolves a fresh group.
std::optional<bool> battlePoseMapTranslucent(uint32_t flags,uint32_t primary,uint32_t secondary,const BattlePoseInnerServices&,BattlePoseMapCursor&);
// Ordinary unlinked, flags8-clear graph. Other facing paths hold explicitly.
// mode is the live native work owner's source W10c; built-in GX updates pass.
bool resumeBattlePoseInner(BattlePosePool&,uint32_t mode,std::optional<uint32_t>& pass,const BattlePartDrawRequest&,const BattlePoseInnerServices&,BattlePoseInnerCursor&);
}
