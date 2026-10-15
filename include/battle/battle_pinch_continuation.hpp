#pragma once
#include <memory>
#include "battle_audience_target_continuation.hpp"
namespace ttyd {
struct BattlePinchServices {
 BattleActorServices actors;
 void* user=nullptr;
 std::optional<int32_t> (*status)(void*,uint32_t unit,int32_t id)=nullptr;
 std::optional<int32_t> (*truncate)(void*,float)=nullptr;
 // Retained actual psndSFXOn owner, original key address. False must retain any
 // partial child effects; no fabricated sound handle/readiness is supplied.
 bool (*sound)(void*,uint32_t sourceKey)=nullptr;
};
// Pin direct-unit versus all-units entry, input, mode and provider identities.
// Completion consumes the cursor; reset only drops native continuation state.
// Earlier status flags/countdown/audience writes survive either a hold or reset.
class BattlePinchCursor {
public:
 BattlePinchCursor();~BattlePinchCursor();
 BattlePinchCursor(const BattlePinchCursor&)=delete;
 BattlePinchCursor& operator=(const BattlePinchCursor&)=delete;
 bool active() const;uint32_t pending() const;bool reset();
private:
 struct State;std::unique_ptr<State> state_;bool invoking_=false;
 friend struct BattlePinchRunner;
};
// Complete BattleCheckAllPinchStatus8011efd8/214, composing complete per-unit
// 80126e00/100, MarioDanger8019f440/38, MarioPinch8019f478/38 and retained
// AddTargetNum801a1a74. No guessed actor filters or rendering cadence.
bool resumePackedBattleCheckAllPinch(uint32_t battle,int32_t mode,
 const BattlePinchServices&,BattlePinchCursor&);
// Same per-unit source body independently, actual nonnull unit required.
bool resumePackedBattleUnitPinch(uint32_t unit,int32_t mode,
 const BattlePinchServices&,BattlePinchCursor&);
}
