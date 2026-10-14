#pragma once
#include "animation_node_draw.hpp"
#include "battle_pose_inner.hpp"
namespace ttyd {
struct AnimationGroupPublication {uint32_t kind=0,group=0,destination=0;BattleActMatrix matrix{};};
struct AnimationShapeRequest {int32_t shape=-1;uint32_t key=0,pass=0;BattleActMatrix model{};};
struct AnimationDrawTraversalServices {
    void* user=nullptr;bool (*matrix)(void*,BattleAudienceMatrixRequest&)=nullptr;
    bool (*publish)(void*,const AnimationGroupPublication&)=nullptr;
    bool (*callback)(void*,uint32_t identity,uint32_t data,uint32_t group,BattleActMatrix&)=nullptr;
    bool (*render)(void*,const AnimationShapeRequest&,uint32_t& pending)=nullptr;
};
struct AnimationDrawFrame {
    int32_t group=-1,parent=-1;uint32_t phase=0,callback=0,callbackData=0,callbackLevel=0;
    AnimationDrawGroup captured;AnimationNodeCursor node;
    AnimationGroupPublication publication;AnimationShapeRequest render;
};
struct AnimationDrawTraversalCursor {
    bool active=false;uint32_t pending=0;BattlePoseInnerContext context;
    std::vector<BattleActMatrix> stack;std::vector<AnimationDrawFrame> frames;
};
bool resumeAnimationDrawTraversal(BattlePosePool&,const BattlePoseInnerContext&,const AnimationDrawTraversalServices&,AnimationDrawTraversalCursor&);
}
