#pragma once
#include "battle_act_menu.hpp"
#include "battle_act_ui.hpp"
#include "battle_weapon_menu_main.hpp"
#include <memory>
namespace ttyd {
struct BattleActMenuContinuationRequest {
  BattleActService function = BattleActService::Unit;
  uint32_t callsite = 0;
  std::array<uint32_t, 4> arguments{};
  uint8_t count = 0;
  // Stable cursor-owned native output for original GetCursorPtr only. Missing
  // write stays unknown; zero is a written original word. Never a guest SP08.
  std::optional<uint32_t> *privateCursorOutput = nullptr;
};
struct BattleActMenuContinuationServices {
  BattleActorServices memory;
  void *user = nullptr;
  std::optional<uint32_t> (*request)(
      void *, const BattleActMenuContinuationRequest &) = nullptr;
};
// Main/Setup/End/Position entry,B/flag and provider identities stay fixed.
// Native SP08 is independently unknown until written; actual memory/resource
// lifetimes remain caller-owned. Reset abandons metadata,not prior source work.
class BattleActMenuContinuationCursor {
public:
  BattleActMenuContinuationCursor();
  ~BattleActMenuContinuationCursor();
  BattleActMenuContinuationCursor(const BattleActMenuContinuationCursor &) =
      delete;
  BattleActMenuContinuationCursor &
  operator=(const BattleActMenuContinuationCursor &) = delete;
  bool active() const;
  uint32_t pending() const;
  bool reset();

private:
  struct State;
  std::unique_ptr<State> state_;
  bool invoking_ = false;
  friend struct BattleActMenuContinuationRunner;
};
std::optional<int32_t>
resumePackedBattleActMenuMain(uint32_t,
                              const BattleActMenuContinuationServices &,
                              BattleActMenuContinuationCursor &);
bool resumePackedBattleActMenuSetup(uint32_t, uint32_t flag,
                                    const BattleActMenuContinuationServices &,
                                    BattleActMenuContinuationCursor &);
bool resumePackedBattleActMenuEnd(uint32_t,
                                  const BattleActMenuContinuationServices &,
                                  BattleActMenuContinuationCursor &);
std::optional<int32_t>
resumePackedBattleChangePosition(uint32_t,
                                 const BattleActMenuContinuationServices &,
                                 BattleActMenuContinuationCursor &);
// Actual UI Init/End children, separately retained from their parent menu.
// Class UI has its own retained free/allocate phases and layout (rows W4,
// unlike weapon W8). Callers serialize actual VM/maps through synchronous
// groups and retain allocation owners; this cursor has no live lease predicate.
class BattleActUiContinuationCursor {
public:
  BattleActUiContinuationCursor();
  ~BattleActUiContinuationCursor();
  BattleActUiContinuationCursor(const BattleActUiContinuationCursor &) = delete;
  BattleActUiContinuationCursor &
  operator=(const BattleActUiContinuationCursor &) = delete;
  bool active() const;
  uint32_t pending() const;
  bool reset();

private:
  struct State;
  std::unique_ptr<State> state_;
  bool invoking_ = false;
  friend struct BattleActUiContinuationRunner;
};
bool resumePackedBattleActUiInit(uint32_t battle, uint32_t cursor,
                                 uint32_t rows, uint32_t flag,
                                 const BattleActUiServices &,
                                 BattleActUiContinuationCursor &);
bool resumePackedBattleActUiEnd(uint32_t battle, const BattleActUiServices &,
                                BattleActUiContinuationCursor &);
// Admission guard around the existing original audience-check continuation;
// it does not reimplement target/query/type publication source logic.
// Pins the existing source audience cursor and provider table. Admission is
// required even if an early source branch might not need a later callback;
// it supplies no fallback target,item or successful missing child.
class BattleActMenuAudienceAdmissionCursor {
public:
  BattleActMenuAudienceAdmissionCursor();
  ~BattleActMenuAudienceAdmissionCursor();
  BattleActMenuAudienceAdmissionCursor(
      const BattleActMenuAudienceAdmissionCursor &) = delete;
  BattleActMenuAudienceAdmissionCursor &
  operator=(const BattleActMenuAudienceAdmissionCursor &) = delete;
  bool active() const;
  uint32_t pending() const;
  bool reset();

private:
  struct State;
  std::unique_ptr<State> state_;
  bool invoking_ = false;
  friend std::optional<bool>
  resumeAdmittedBattleActMenuAudience(const BattleMenuAudienceServices &,
                                      BattleActMenuAudienceAdmissionCursor &);
};
std::optional<bool>
resumeAdmittedBattleActMenuAudience(const BattleMenuAudienceServices &,
                                    BattleActMenuAudienceAdmissionCursor &);
} // namespace ttyd
