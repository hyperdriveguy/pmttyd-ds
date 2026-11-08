#pragma once
#include "battle_ending_fade.hpp"
#include "source_battle_voice.hpp"
#include <array>
#include <cstdint>
#include <initializer_list>
#include <memory>
#include <optional>
namespace ttyd {
struct NativeHarborFinishingRequest {
  uint32_t function = 0, callsite = 0;
  std::array<uint32_t, 4> arguments{};
  uint8_t count = 0;
  SourceBattleVoiceCursor *voice = nullptr;
  const uint32_t *fadeColor =
      nullptr; // Actual source word in stable owned cell.
};
struct NativeHarborFinishingServices {
  std::shared_ptr<const void> lifetime;
  void *user = nullptr;
  bool (*current)(void *) = nullptr;
  // Actual Flow ReadyForFinishing+counter/model/birth proof, not just EXP/HP.
  bool (*ordinaryRewardDone)(void *) = nullptr;
  std::optional<uint32_t> (*word)(void *, uint32_t) = nullptr;
  std::optional<uint8_t> (*byte)(void *, uint32_t) = nullptr;
  bool (*writeWord)(void *, uint32_t, uint32_t) = nullptr;
  // Whole retained child owners: engaged zero is executed, empty is held.
  std::optional<uint32_t> (*call)(
      void *, const NativeHarborFinishingRequest &) = nullptr;
  std::optional<int32_t> (*refreshRate)(void *) = nullptr;
  // True only after actual complete upper AND lower opaque frame submission.
  // Key is this actual incarnation's reached fade-entry simulation visit.
  std::optional<bool> (*opaquePresented)(void *, uint64_t fadeVisit) = nullptr;
};
class NativeHarborFinishing {
public:
  enum class Phase {
    OrdinaryBranch,
    UpgradeGate,
    VictoryPose,
    PoseWait,
    AfterPose,
    AudienceAndNpc,
    FadeEntry,
    FadeWait,
    OpaqueWait,
    Ready
  };
  struct Row {
    uint64_t visit = 0;
    uint32_t entrySequence = 0, sequence = 0, pending = 0;
    int32_t timer = 0;
  };
  NativeHarborFinishing(uint32_t actualBattle, NativeHarborFinishingServices);
  NativeHarborFinishing(const NativeHarborFinishing &) = delete;
  NativeHarborFinishing &operator=(const NativeHarborFinishing &) = delete;
  // Retain exact original visit through a hold. Later/older visits cannot skip
  // an acknowledged prefix. Completed duplicate visits have no effects.
  bool advance(uint64_t simulationVisit);
  bool ready() const;
  Phase phase() const { return phase_; }
  uint32_t pending() const { return pending_; }
  uint32_t sourceSequence() const { return sequence_; }
  int32_t timer() const { return timer_; }
  const Row &row() const { return row_; }
  const BattleEndingFade &fade() const { return fade_; }
  uint64_t fadeVisit() const { return fadeVisit_; }

private:
  bool current() const;
  std::optional<uint32_t> call(uint32_t function, uint32_t site,
                               std::initializer_list<uint32_t> args,
                               SourceBattleVoiceCursor *voice = nullptr,
                               const uint32_t *color = nullptr);
  NativeHarborFinishingRequest request_;
  uint32_t battle_;
  NativeHarborFinishingServices services_;
  Phase phase_ = Phase::OrdinaryBranch, visitPhase_ = Phase::OrdinaryBranch;
  uint32_t sequence_ = 0x0700000e, pending_ = 0, step_ = 0;
  uint32_t info_ = 0, setup_ = 0, mario_ = 0, party_ = 0, flags_ = 0, key_ = 0,
           npcWork_ = 0, npc_ = 0, color_ = 0;
  int32_t timer_ = 0, rate_ = 0;
  bool relief_ = false, invoking_ = false, hasVisit_ = false,
       visitDone_ = false;
  mutable bool checking_ = false;
  uint64_t visit_ = 0, fadeVisit_ = 0;
  Row row_;
  BattleEndingFade fade_;
  SourceBattleVoiceCursor voice_;
};
} // namespace ttyd
