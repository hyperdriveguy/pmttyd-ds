#pragma once
#include "battle_actor.hpp"
#include <array>
#include <memory>
#include <optional>
namespace ttyd {
enum class BattleActChild {
  Unit,
  Mario,
  NiceAsync,
  AudienceInit,
  Status,
  CommandInit,
  CameraSpeed,
  CommandInput,
  UnitData,
  WindowOff,
  CreateEvent,
  Random,
  DeleteEvent,
  WaitEvents,
  AdjustMessages,
  Messages,
  Announcements,
  WindowOn,
  SetStatus,
  CheckStatusFlag,
  ClearStatusFlag,
  Belong,
  PhaseEnd,
  ConsumeItems,
  AudiencePerAct,
  AudienceReaction,
  Concluded,
  BreakSlotReaction,
  EventExists
};
// One original child, never an opaque action-state handler. Source stack
// outputs are private native values; no synthetic guest output addresses.
struct BattleActSequenceRequest {
  BattleActChild child = BattleActChild::Unit;
  uint32_t function = 0, callsite = 0;
  std::array<uint32_t, 4> arguments{};
};
// value is a raw source scalar/guest address; real is CameraSpeed's float
// result. They are separate ABI lanes, not interchangeable readiness flags.
struct BattleActOutput {
  uint32_t value = 0;
  float real = 0;
};
struct BattleActServices {
  BattleActorServices actors;
  void *user = nullptr;
  // nullopt holds this exact child. Provider owns its partial child work;
  // engaged zero (including a failed original evtEntry) is complete.
  std::optional<BattleActOutput> (*call)(void *,
                                         const BattleActSequenceRequest &) = nullptr;
};
// One invocation pins B and all provider identities, retaining source PC,
// pointers and operands across holds. Caller pins real allocations/child owners;
// callback equality alone does not prevent address reuse. Reset has no rollback.
class BattleActCursor {
public:
  BattleActCursor();
  ~BattleActCursor();
  BattleActCursor(const BattleActCursor &) = delete;
  BattleActCursor &operator=(const BattleActCursor &) = delete;
  bool active() const;
  uint32_t pending() const;
  bool reset();

private:
  struct State;
  std::unique_ptr<State> state_;
  bool invoking_ = false;
  friend bool resumePackedBattleAct(uint32_t, const BattleActServices &,
                                    BattleActCursor &);
};
// Complete original btlseqAct8011b2dc/8ac, one actual invocation. Original
// sequence transitions and fallthrough remain separate from native completion.
bool resumePackedBattleAct(uint32_t battle, const BattleActServices &,
                           BattleActCursor &);
} // namespace ttyd
