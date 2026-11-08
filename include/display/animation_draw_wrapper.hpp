#pragma once
#include "battle_act_draw.hpp"
#include <optional>
namespace ttyd {
struct AnimationDrawWrapperInput {
 uint32_t handle=0,pass=0;BattleActMatrix matrix{};float angle=0,scale=1;
};
struct AnimationInnerDrawRequest {
 uint32_t pose=0,pass=0;BattleActMatrix matrix{};float angle=0,scale=1;
};
struct AnimationDrawWrapperServices {
 void* user=nullptr;
 std::optional<uint32_t> (*word)(void*,uint32_t)=nullptr;
 // Original8003bc68, r3 is a guest pose RECORD pointer. Copied native matrix
 // represents caller storage, never a fabricated guest stack address. A false
 // child may retain effects; same request/provider must resume it.
 bool (*inner)(void*,const AnimationInnerDrawRequest&)=nullptr;
};
// Pending includes budget/provider holds. The two named unfinished frontiers
// retain active state; neither can acknowledge successful empty geometry.
enum class AnimationDrawWrapperFrontier {Pending,LinkedPreparation,CallerRegister,Complete};
struct AnimationDrawWrapperCursor {
 bool active=false;uint32_t phase=0,work=0,pool=0,pose=0,index=0,models=0,entry=0,holder=0,model=0,flags=0,link=0;
 AnimationDrawWrapperInput input;AnimationInnerDrawRequest request;
 AnimationDrawWrapperServices owner;
};
// Original8003b4d8 entry + complete initially-unlinked branch, including fresh
// final link recheck. Linked preparation and incoming-r30 dispatch are explicit
// unfinished frontiers, never successful empty draws. No wrapper VM stores on
// the admitted unlinked route; Inner can perform retained source effects.
// Default 64 is a native pump budget,not source timing or a total count cap.
AnimationDrawWrapperFrontier resumeAnimationDrawWrapper(const AnimationDrawWrapperInput&,const AnimationDrawWrapperServices&,AnimationDrawWrapperCursor&,unsigned visits=64);
}
