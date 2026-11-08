#pragma once
#include "battle_actor.hpp"
#include <array>
#include <memory>
#include <optional>
namespace ttyd {
// Original linked calls only. Source branches, scans, local memory writes and
// private captures belong to the parent; no opaque state/branch completion.
struct BattleStageSetupRequest {
  uint32_t function = 0, callsite = 0;
  std::array<uint32_t, 3> integers{};
};
struct BattleStageSetupServices {
  BattleActorServices actors;
  std::shared_ptr<const void> lifetime;
  void *user = nullptr;
  bool (*current)(void *) = nullptr;
  std::optional<uint32_t> (*call)(void *,
                                  const BattleStageSetupRequest &) = nullptr;
  std::optional<float> (*subtractToSingle)(void *, double, double) = nullptr;
  std::optional<float> (*fma)(void *, float, float, float) = nullptr;
};
// Complete source void parents. True means this invocation completed, never
// renderer readiness. Providers retain their own whole linked child through a
// missing result; engaged zero remains the original zero. Source RAM/topology
// must remain admitted through each synchronous sequence of native accesses.
#define TTYD_STAGE_SETUP_CURSOR(Name, Fn)                                      \
  class Name {                                                                 \
  public:                                                                      \
    Name();                                                                    \
    ~Name();                                                                   \
    Name(const Name &) = delete;                                               \
    Name &operator=(const Name &) = delete;                                    \
    bool active() const;                                                       \
    uint32_t pending() const;                                                  \
    bool reset();                                                              \
                                                                               \
  private:                                                                     \
    struct State;                                                              \
    std::unique_ptr<State> state_;                                             \
    bool invoking_ = false;                                                    \
    friend bool Fn(const BattleStageSetupServices &, Name &);                  \
  };                                                                           \
  bool Fn(const BattleStageSetupServices &, Name &);
TTYD_STAGE_SETUP_CURSOR(BattleStageObjectInitCursor,
                        resumePackedBattleStageObjectInit)
TTYD_STAGE_SETUP_CURSOR(BattleSpotlightInitCursor,
                        resumePackedBattleSpotlightInit)
TTYD_STAGE_SETUP_CURSOR(BattleStageInitCursor, resumePackedBattleStageInit)
#undef TTYD_STAGE_SETUP_CURSOR
} // namespace ttyd
