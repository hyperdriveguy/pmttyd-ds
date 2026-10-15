#pragma once
#include <memory>
#include "battle_actor.hpp"
namespace ttyd {
struct BattleAudienceTargetContinuationServices {
 BattleActorServices actors;
 void* user=nullptr;
 // Actual source fctiwz result. Missing/unsupported conversion holds the
 // captured quotient, preserving every earlier offset/chunk store.
 std::optional<int32_t> (*truncate)(void*,float)=nullptr;
};
// One cursor owns one parent or Sub invocation. Preserve exact float bits
// (including signed zero), service users and callbacks until completion/reset.
// reset discards continuation state only; it cannot undo earlier guest writes.
class BattleAudienceTargetCursor {
public:
 BattleAudienceTargetCursor();~BattleAudienceTargetCursor();
 BattleAudienceTargetCursor(const BattleAudienceTargetCursor&)=delete;
 BattleAudienceTargetCursor& operator=(const BattleAudienceTargetCursor&)=delete;
 bool active() const;uint32_t pending() const;bool reset();
private:
 struct State;std::unique_ptr<State> state_;bool invoking_=false;
 friend struct BattleAudienceTargetRunner;
};
// Whole source801a1a74/180 and independently callable801a18cc/1a8. Source
// parent and child capture global B independently. Inputs/provider identities
// remain fixed while held; successful stores are never repeated. Exceptional
// nonfinite/nonprogressing chunk domains remain explicitly unavailable.
bool resumePackedBattleAudienceAddTargetNum(float delta,float bias,
 const BattleAudienceTargetContinuationServices&,BattleAudienceTargetCursor&);
bool resumePackedBattleAudienceAddTargetNumSub(float delta,
 const BattleAudienceTargetContinuationServices&,BattleAudienceTargetCursor&);
}
