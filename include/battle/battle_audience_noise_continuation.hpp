#pragma once
#include "battle_actor.hpp"
#include <array>
#include <memory>
#include <optional>
namespace ttyd {
// Original Noise frame private SP08..47. No incoming byte is assumed written.
struct BattleAudienceNoisePrivate {
  std::array<std::optional<uint8_t>, 64> bytes{};
};
struct BattleAudienceNoiseCall {
  uint32_t function = 0, callsite = 0;
  std::array<uint32_t, 2> arguments{};
  // Whole strcpy writes this owned non-guest output. Sound1 reads name only.
  BattleAudienceNoisePrivate *copyOutput = nullptr;
  const BattleAudienceNoisePrivate *name = nullptr;
  uint32_t originalName = 0;
};
struct BattleAudienceNoiseServices {
  BattleActorServices actors;
  std::shared_ptr<const void> lifetime;
  void *user = nullptr;
  bool (*current)(void *) = nullptr;
  // Engaged zero is actual completion; nullopt retains the exact whole child.
  std::optional<uint32_t> (*call)(void *,
                                  const BattleAudienceNoiseCall &) = nullptr;
};
class BattleAudienceNoiseCursor {
public:
  BattleAudienceNoiseCursor();
  ~BattleAudienceNoiseCursor();
  BattleAudienceNoiseCursor(const BattleAudienceNoiseCursor &) = delete;
  BattleAudienceNoiseCursor &
  operator=(const BattleAudienceNoiseCursor &) = delete;
  bool active() const;
  uint32_t pending() const;
  bool reset(); // Metadata only; no source undo or stop.
private:
  struct State;
  std::unique_ptr<State> state_;
  bool invoking_ = false;
  friend bool
  resumePackedBattleAudienceNoise(const BattleAudienceNoiseServices &,
                                  BattleAudienceNoiseCursor &,
                                  const BattleAudienceNoisePrivate *);
};
// Whole original801a0b88/164. Optional inherited private state is captured
// once; its owner/address must remain stable. Default source unknowns remain
// unknown. Supplying a native policy is external to this source continuation.
bool resumePackedBattleAudienceNoise(
    const BattleAudienceNoiseServices &, BattleAudienceNoiseCursor &,
    const BattleAudienceNoisePrivate *inherited = nullptr);
} // namespace ttyd
