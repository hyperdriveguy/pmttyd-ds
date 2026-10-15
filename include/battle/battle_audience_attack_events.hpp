#pragma once
#include "battle_actor.hpp"
#include "event_callback_registry.hpp"
#include <array>
#include <memory>
#include <optional>
namespace ttyd {
// Numeric/sound nullopt means unavailable; engaged sound -1 is a valid
// source handle. hit/pitch false retain provider-owned partial work. A true hit
// also acknowledges an actual completed allocation failure (scalar ignored).
struct BattleAudienceAttackServices {
  BattleActorServices actors;
  EventDispatchServices values;
  void *user = nullptr;
  std::optional<int32_t> (*truncate)(void *, float) = nullptr;
  bool (*hit)(void *, int32_t, int32_t, const std::array<float, 3> &,
              float) = nullptr;
  std::optional<int32_t> (*sound)(void *, uint32_t,
                                  const std::array<float, 3> &) = nullptr;
  bool (*pitch)(void *, int32_t, int32_t) = nullptr;
};
// Cursor pins EventEntry address, lifetime, ID, command, args, function and
// all service identities. Reset cannot undo already stored EVT outputs, effects
// or sound. Keep backend/VM/providers alive until held entries are retired.
class BattleAudienceAttackEventCursor {
public:
  BattleAudienceAttackEventCursor();
  ~BattleAudienceAttackEventCursor();
  BattleAudienceAttackEventCursor(const BattleAudienceAttackEventCursor &) =
      delete;
  BattleAudienceAttackEventCursor &
  operator=(const BattleAudienceAttackEventCursor &) = delete;
  bool active() const;
  bool reset();
  uint32_t pending() const;

private:
  struct State;
  std::unique_ptr<State> state_;
  bool invoking_ = false;
  friend std::optional<int32_t>
  resumeBattleAudienceAttackEvent(EventEntry &, uint32_t,
                                  const BattleAudienceAttackServices &,
                                  BattleAudienceAttackEventCursor &);
};
// Real _attack_aud801465e0 and _get_attack_aud_no801466c4 callbacks. Typed
// private locals model source stack outputs; no guest stack address is
// invented. False providers own partial child work. Value2 completes, nullopt
// retains.
std::optional<int32_t>
resumeBattleAudienceAttackEvent(EventEntry &, uint32_t,
                                const BattleAudienceAttackServices &,
                                BattleAudienceAttackEventCursor &);
struct PositionalSfxRegistry;
// Exact psndSFX_pit no-op/queued branches. nullopt leaves the live descriptor/
// engine branch explicitly unavailable; generation is not checked by source.
std::optional<bool> setQueuedBattleAudienceAttackPitch(PositionalSfxRegistry &,
                                                       int32_t handle,
                                                       int32_t pitch);
class EventNativeRuntime;
// Registry adapter owns one cursor per actual EventEntry lifetime/command.
// Backend and runtime outlive registry/entries. Bind once; providers are copied
// into each admitted invocation and cannot be exchanged while it is held.
class BattleAudienceAttackEvents {
public:
  explicit BattleAudienceAttackEvents(EventNativeRuntime &);
  ~BattleAudienceAttackEvents();
  BattleAudienceAttackEvents(const BattleAudienceAttackEvents &) = delete;
  BattleAudienceAttackEvents &
  operator=(const BattleAudienceAttackEvents &) = delete;
  void *user = nullptr;
  bool (*hit)(void *, int32_t, int32_t, const std::array<float, 3> &,
              float) = nullptr;
  std::optional<int32_t> (*sound)(void *, uint32_t,
                                  const std::array<float, 3> &) = nullptr;
  bool (*pitch)(void *, int32_t, int32_t) = nullptr;
  bool bind(EventCallbackRegistry &);

private:
  struct State;
  std::unique_ptr<State> state_;
  int32_t invoke(EventEntry &, uint32_t);
};
} // namespace ttyd
