#pragma once
#include "battle_actor.hpp"
#include "battle_audience_touch.hpp"
#include <memory>
namespace ttyd {
// Native chosen-slot input identity, supplied after actual displayed-frame tap
// admission. Tokens/generations must come from real lifecycle owners.
struct BattleAudienceSelectedInput {
  std::shared_ptr<const uint8_t> owner;
  uint32_t audience = 0;
  BattleAudienceTouchTarget target;
};
struct BattleAudienceSelectedLive {
  std::shared_ptr<const uint8_t> owner;
  uint32_t audience = 0;
  BattleAudienceTouchCandidate candidate;
};
struct BattleAudienceSelectedHolder {
  BattleAudienceSelectedInput input;
  uint32_t member = 0, item = 0, kind = 0;
};
// Native adaptation of selected GetItemOn admission: source flags1/!102,
// actual held index and signed item-state<=4 or7, AND true harmful236..239.
// Visibility/incarnation proof is mandatory and never inferred from positions.
// Reads only; a candidate scan must not overwrite the original selected cache.
std::optional<BattleAudienceSelectedHolder>
inspectSelectedBattleAudienceHolder(const BattleAudienceSelectedInput &,
                                    const BattleAudienceSelectedLive &,
                                    const BattleActorServices &);
// Re-inspects the current chosen holder, then writes the original selected
// slot/XYZ cache in source order. A previous read-only result is not reusable
// authorization; rejected/lost ownership performs no cache writes.
bool cacheSelectedBattleAudienceHolder(const BattleAudienceSelectedInput &,
                                       const BattleAudienceSelectedLive &,
                                       const BattleActorServices &);
struct BattleAudienceSelectedServices {
  BattleActorServices memory;
  void *user = nullptr;
  std::optional<uint32_t> (*mario)(void *, uint32_t battle) = nullptr;
  bool (*endActMenu)(void *, uint32_t battle) = nullptr;
  bool (*lightOff)(void *, int32_t, int32_t) = nullptr;
};
class BattleAudienceSelectedCursor {
public:
  BattleAudienceSelectedCursor();
  ~BattleAudienceSelectedCursor();
  BattleAudienceSelectedCursor(const BattleAudienceSelectedCursor &) = delete;
  BattleAudienceSelectedCursor &
  operator=(const BattleAudienceSelectedCursor &) = delete;
  bool active() const;
  void reset();

private:
  struct State;
  std::unique_ptr<State> state_;
  friend std::optional<bool> resumeSelectedBattleAudienceInterruption(
      const BattleAudienceSelectedInput &, const BattleAudienceSelectedLive &,
      const BattleAudienceSelectedServices &, BattleAudienceSelectedCursor &);
};
// No physical-trigger service exists here. Original X keeps its independent
// first-eligible GetItemOn path. Invoke only at an actually admitted source
// menu-interruption call site after displayed-frame consumption; this API does
// not create player turns, source input readiness or an AudienceAttack root.
// false rejects; nullopt retains an unavailable
// provider; true completes the source SetTarget/UI-End/type11/state29 tail.
// Keep the same input and services across waits. Current lifetime/incarnations
// are revalidated before resume; after SetTarget, display/eligibility changes
// do not restart the committed selection. Children retain their own partial
// work.
std::optional<bool> resumeSelectedBattleAudienceInterruption(
    const BattleAudienceSelectedInput &, const BattleAudienceSelectedLive &,
    const BattleAudienceSelectedServices &, BattleAudienceSelectedCursor &);
} // namespace ttyd
