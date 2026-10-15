#pragma once
#include "field_npc_clock_binding.hpp"
#include "field_npc_main_auto_pose.hpp"
#include "field_npc_main_entry.hpp"
#include "field_npc_main_movement.hpp"
namespace ttyd {
class AnimationPoolMemory;
// Admission is read-only; sound false has no effects and remains pending.
struct FieldNpcMainEntryNativeServices {
  void *user = nullptr;
  FieldNpcClockNumbers numbers;
  bool (*admitMario)(void *, EventAddressSpace &, uint32_t) = nullptr;
  bool (*movement)(void *, EventAddressSpace &, const FieldNpcMainMoveRequest &,
                   FieldNpcMainMoveResponse &) = nullptr;
  // Explicit numeric adaptation and real source-clock quotient admission.
  bool (*estimate)(void *, double, double &) = nullptr;
  bool (*poseClock)(void *, BattlePoseClock &) = nullptr;
  bool (*sound)(void *, EventAddressSpace &,
                const FieldNpcMainEntryRequest &) = nullptr;
};
// Retains actual NPC owner; borrows VM, registry mapping and animation alias.
// Keep all mappings/owners alive through every parent cursor/frontier.
class FieldNpcMainEntryBinding {
public:
  static std::unique_ptr<FieldNpcMainEntryBinding>
  bind(std::shared_ptr<FieldNpcRegistryOwner>, FieldNpcRegistryBinding &,
       EventNativeRuntime &, AnimationPoolMemory &,
       const FieldNpcMainEntryNativeServices &);
  FieldNpcMainFrontier advance(FieldNpcMainEntryCursor &);
  FieldNpcMoveFrontier advanceMovement(FieldNpcMainEntryCursor &,
                                       FieldNpcMainMoveCursor &);

  FieldNpcMoveFrontier advanceAutomaticPose(FieldNpcMainEntryCursor &,
                                            FieldNpcMainMoveCursor &,
                                            FieldNpcAutoPoseCursor &);

private:
  FieldNpcMainEntryBinding(std::shared_ptr<FieldNpcRegistryOwner>,
                           EventNativeRuntime &, AnimationPoolMemory &,
                           const FieldNpcMainEntryNativeServices &);
  FieldNpcMainEntryServices services();
  std::shared_ptr<FieldNpcRegistryOwner> owner_;
  EventNativeRuntime &vm_;
  AnimationPoolMemory &poses_;
  FieldNpcMainEntryNativeServices native_;
};
} // namespace ttyd
