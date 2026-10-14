#pragma once
#include "animation_draw_traversal.hpp"
#include "camera_manager.hpp"
namespace ttyd {
// Source calls are separate from native GPU policy. No flattened actor mesh
// replaces the original shape bases, draw selectors or low-halfword streams.
struct AnimationShapeCall {
    uint32_t function=0,draw=0;
    std::array<uint32_t,7> arguments{};
    std::optional<BattleActMatrix> matrix;
    ActorVector origin{};
    std::shared_ptr<const AnimationDrawResource> resource;
    const BattlePose* pose=nullptr;
    std::array<uint32_t,8> uvBases{};
    std::array<uint16_t,11> indices{};
};
struct AnimationShapeMath {
    uint32_t function=0,status=0;
    BattleActMatrix input{},output{};
    ActorVector vector{},vectorOutput{};
};
struct AnimationShapeServices {
    void* user=nullptr;
    const CameraState* (*currentCamera)(void*)=nullptr;
    const CameraState* (*cameraByIndex)(void*,uint32_t)=nullptr;
    bool (*concat)(void*,BattleAudienceMatrixRequest&)=nullptr;
    bool (*math)(void*,AnimationShapeMath&)=nullptr;
    bool (*call)(void*,const AnimationShapeCall&)=nullptr;
    bool (*material)(void*,const AnimationShapeCall&,uint32_t& pending)=nullptr;
};
struct AnimationShapeCursor {
    bool active=false,queued=false;uint32_t phase=0,pending=0,draw=0,drawCount=0,drawIndex=0;
    uint32_t positionBase=0,normalBase=0,colorBase=0,slots=0,attribute=0,attributeStep=0;
    uint32_t run=0,runCount=0,first=0,count=0,vertex=0;
    int32_t handle=-1;const CameraState* fogCamera=nullptr;
    AnimationShapeRequest input;std::shared_ptr<const BattlePoseResource> resource;
    std::array<uint32_t,8> uvBases{};
    BattleActMatrix viewModel{};std::optional<BattleActMatrix> normal;
    BattleAudienceMatrixRequest concat;AnimationShapeMath math;AnimationShapeCall request;
};
// Admitted native domain: retained resource/pose owner, selector0 CPU outputs,
// no kind1/2 override objects. Unsupported owners retain their real boundary.
bool resumeAnimationShape(BattlePosePool&,int32_t,const std::optional<uint32_t>& effectivePass,
    const AnimationShapeRequest&,const AnimationShapeServices&,AnimationShapeCursor&);
// Explicit finite native numeric adapter: 1/det substitutes for PPC fres;
// source cofactor/FMA/refinement/store rules are retained. No bit-parity claim.
bool animationShapeNativeMath(AnimationShapeMath&);
}
