#pragma once
#include "harbor_tutorial.hpp"
#include <memory>
#include <optional>
namespace ttyd {
// Port scheduling policy for the existing manual setup2 scene. Original EVT,
// command, timing, damage and dialogue owners still decide their own outcomes.
// This is deliberately distinct from the retained whole original Main owner.
class NativeHarborGameplay {
public:
 enum class Stage {Opening,PlayerPhase,PlayerPhaseWait,CommandOpen,Command,
   PlayerAttack,Feedback,FeedbackWait,OpponentPhase,OpponentPhaseWait,
   OpponentAttack,OpponentAttackWait,ClosingPhase,ClosingPhaseWait,Conclusion};
 // One visit per increasing simulation tick. Missing physical input stays
 // missing; message edges are not converted to action/defense samples.
 bool advance(HarborTutorial&,uint64_t simulationTick,uint64_t milliseconds,
              uint32_t messageKeys,const BattlePadSample* physical);
 // Explicit policy retirement at actual facade replacement; no source End.
 bool reset();
 Stage stage()const{return stage_;}
 std::optional<int32_t> conclusion()const{return conclusion_;}
 uint32_t pending()const{return pending_;}
private:
 std::shared_ptr<const void> runtime_;
 const HarborTutorial* scene_=nullptr;
 uint64_t tick_=0,time_=0;bool sampled_=false,invoking_=false;
 Stage stage_=Stage::Opening;int32_t round_=1;
 std::optional<int32_t> conclusion_;uint32_t pending_=0;
};
}
