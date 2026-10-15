#pragma once
#include "battle_audience_aux_callback.hpp"
#include <vector>
namespace ttyd {
// Matrix API tokens are original SDK addresses; axis122 is ASCII z.
// Workspace32 operations/12 matrices is native bounded storage, not animation FPS.
struct AnimationNodeOperation {uint32_t function=0,target=0,left=0,right=0,axis=122;float radians=0;std::array<float,3> xyz{};};
// step/pending are operation progress; requestPending keeps one stable payload
// until the provider acknowledges, including after stack-level publication.
struct AnimationNodeCursor {
    bool active=false,requestPending=false;uint32_t step=0,pending=0,count=0;
    std::array<AnimationNodeOperation,32> operations{};std::array<BattleActMatrix,12> matrices{};
    BattleAudienceMatrixRequest request;
};
// Captures the24-float node/parent inputs before all SDK calls. SDK stack
// publication occurs after component generation and before mask dispatch.
bool resumeAnimationNodeDraw(const std::array<float,24>& node,bool joint,const std::array<float,3>& parent,
    std::vector<BattleActMatrix>& stack,void* user,bool (*matrix)(void*,BattleAudienceMatrixRequest&),AnimationNodeCursor&);
}
