#pragma once
#include "battle_class_backend.hpp"
#include "native_facade_registration.hpp"
#include "window_frame.hpp"
#include <optional>
#include <string>
namespace ttyd {
// Presentation-only 400x240 reflow of the manual Harbor source command model.
// No source display/camera callback or guest instruction completion is implied.
enum class HarborNativeMenuKind { Class, Weapon, Target };
struct HarborNativeMenuRow {
  uint32_t identity = 0, detail = 0;
  uint16_t icon = 0;
  bool enabled = false;
  std::string label;
  uint32_t sourceName = 0, sourceReason = 0, sourceOrdinal = 0, sourceItem = 0;
  bool operator==(const HarborNativeMenuRow &other) const;
};
struct HarborNativeMenuSnapshot {
  HarborNativeMenuKind kind = HarborNativeMenuKind::Class;
  const void *facade = nullptr;
  std::shared_ptr<const void> runtime;
  std::weak_ptr<const void> registration;
  NativeFacadeRegistration::Proof facadeRegistration;
  std::shared_ptr<const uint8_t> menuOwner;
  uint32_t battle = 0, work = 0, type = 0, phase = 0;
  uint64_t modelTick = 0;
  int32_t selected = -1, turn = 0;
  std::vector<HarborNativeMenuRow> rows;
};
// Exact copied model equality plus still-live registration; callers separately
// obtain both snapshots from the actual facade/source VM at the draw boundary.
bool sameHarborNativeMenu(const HarborNativeMenuSnapshot &,
                          const HarborNativeMenuSnapshot &);
struct HarborNativeMenuPlan {
  HarborNativeMenuSnapshot snapshot;
  BattleClassBackend batch;
  std::optional<WindowFrameMesh> window;
};
// Atomic CPU construction: failure exposes no prefix. All text is original
// copied label bytes; unsupported controls/missing glyphs/icons reject.
std::optional<HarborNativeMenuPlan> prepareHarborNativeMenu(
    const HarborNativeMenuSnapshot &, const BattleClassSurfaceLease &,
    const BattleFontLease &,
    const std::function<const MessageIconFrame *(uint32_t)> &);
bool validHarborNativeMenuPlan(const HarborNativeMenuPlan &);
} // namespace ttyd
