#pragma once
#include "field_npc_clock_binding.hpp"
#include "field_npc_draw.hpp"
#include "field_npc_main_auto_pose.hpp"
#include "field_npc_main_detection.hpp"
#include "field_npc_main_entry.hpp"
#include "field_npc_main_display.hpp"
#include "field_npc_main_shadow.hpp"
#include "field_npc_main_history.hpp"
#include "field_npc_main_tail.hpp"
#include "field_npc_main_movement.hpp"
#include "field_npc_mario_detection.hpp"
namespace ttyd {
class AnimationPoolMemory;
// Admission is read-only; sound false has no effects and remains pending.
// Original stage effects are explicit providers on the same guest memory.
// bind() requires base clock/Mario admission, not every later provider; a missing
// movement/draw/queue/audio/field owner holds exactly when its stage is reached.
// Separate stable context for retained original animation children. False can
// follow source stores: the SAME request/provider/pass resumes that child. An
// installed provider is exclusive; holds never fall through to legacy handlers.
struct FieldNpcAnimationServices {
  void* user=nullptr;
  bool (*request)(void*,EventAddressSpace&,const FieldNpcDrawRequest&,FieldNpcDrawResponse&)=nullptr;
};
struct FieldNpcMainEntryNativeServices {
  void *user = nullptr;
  FieldNpcClockNumbers numbers;
  bool (*admitMario)(void *, EventAddressSpace &, uint32_t) = nullptr;
  bool (*movement)(void *, EventAddressSpace &, const FieldNpcMainMoveRequest &,
                   FieldNpcMainMoveResponse &) = nullptr;
  // Explicit numeric adaptation and real source-clock quotient admission.
  bool (*estimate)(void *, double, double &) = nullptr;
  bool (*poseClock)(void *, BattlePoseClock &) = nullptr;
  bool (*detection)(void *, EventAddressSpace &,
                    const FieldNpcDetectionRequest &,
                    FieldNpcDetectionResponse &) = nullptr;
  bool (*marioDetection)(void *, EventAddressSpace &,
                         const FieldNpcMarioRequest &,
                         FieldNpcMarioResponse &) = nullptr;
  // Camera, SDK matrix and dispEntry provider. Own the same camera/queue pass
  // through holds and deferred dispatch; false is atomic/no-effect. A queue
  // NaN rejection is acknowledged success. No fallback camera or registration.
  bool (*display)(void *, EventAddressSpace &, const FieldNpcDisplayRequest &,
                  FieldNpcDisplayResponse &) = nullptr;
  // Same shadow pool/frame through holds; actual zero slot is valid.
  bool (*shadow)(void *, EventAddressSpace &, const FieldNpcShadowRequest &,
                 FieldNpcShadowResponse &) = nullptr;
  // Real field encounter/talk/battle and Kpa score providers; false no effects.
  bool (*tail)(void *, EventAddressSpace &, const FieldNpcTailRequest &,
               FieldNpcTailResponse &) = nullptr;
  // Deferred NPC draw providers retain the same field camera/queue/animation
  // producer pass. Scalar/matrix and legacy draw effects are atomic on false;
  // retained animation children below can suspend after original source stores.
  bool (*drawNumber)(void *, EventAddressSpace &, FieldNpcMatrixNumber &) = nullptr;
  bool (*drawMatrix)(void *, EventAddressSpace &, BattleAudienceMatrixRequest &) = nullptr;
  bool (*draw)(void *, EventAddressSpace &, const FieldNpcDrawRequest &,
               FieldNpcDrawResponse &) = nullptr;
  bool (*sound)(void *, EventAddressSpace &,
                const FieldNpcMainEntryRequest &) = nullptr;
  FieldNpcAnimationServices animation;
};
// One suspended full actor scan. Every child belongs to the current actor;
// retire child metadata only after history and the parent final clear finish.
// Aggregate phases0 entry,1 body through shadow,2 history,3 final clear and
// next entry,4 FieldTail. Preserve old child captures while next entry holds;
// retirement at the next boundary prevents history/clock replay.
struct FieldNpcMainActorsCursor {
  const void *binding = nullptr;
  uint8_t phase = 0;
  FieldNpcMainEntryCursor entry;
  FieldNpcMainMoveCursor movement;
  FieldNpcAutoPoseCursor pose;
  FieldNpcDetectionCursor detection;
  FieldNpcMarioCursor mario;
  FieldNpcDisplayCursor display;
  FieldNpcShadowCursor shadow;
  FieldNpcHistoryCursor history;
};
enum class FieldNpcActorsFrontier { Pending, FieldTail };
// Whole Main phases0 actors,1 field tail,2 terminal. Unlike the clock helper,
// completion stays terminal; caller resets for a NEW source invocation/pass.
struct FieldNpcMainCursor {
  const void *binding = nullptr;
  uint8_t phase = 0;
  FieldNpcMainActorsCursor actors;
  FieldNpcTailCursor tail;
};
// Retains actual NPC owner; borrows VM, registry mapping and animation alias.
// Keep all mappings/owners alive through every parent cursor/frontier.
class FieldNpcMainEntryBinding {
public:
  static std::unique_ptr<FieldNpcMainEntryBinding>
  bind(std::shared_ptr<FieldNpcRegistryOwner>, FieldNpcRegistryBinding &,
       EventNativeRuntime &, AnimationPoolMemory &,
       const FieldNpcMainEntryNativeServices &);
  // These stage APIs resume their prerequisites; held/completed children stay
  // in caller cursors. A frontier names the next source work, not whole Main.
  // advanceDraw is implemented separately in field_npc_draw_binding.cpp.
  FieldNpcMainFrontier advance(FieldNpcMainEntryCursor &);
  FieldNpcMoveFrontier advanceMovement(FieldNpcMainEntryCursor &,
                                       FieldNpcMainMoveCursor &);

  FieldNpcMoveFrontier advanceAutomaticPose(FieldNpcMainEntryCursor &,
                                            FieldNpcMainMoveCursor &,
                                            FieldNpcAutoPoseCursor &);

  FieldNpcDetectionFrontier advanceDetection(FieldNpcMainEntryCursor &,
                                             FieldNpcMainMoveCursor &,
                                             FieldNpcAutoPoseCursor &,
                                             FieldNpcDetectionCursor &);

  FieldNpcDetectionFrontier advanceMarioDetection(FieldNpcMainEntryCursor &,
                                                  FieldNpcMainMoveCursor &,
                                                  FieldNpcAutoPoseCursor &,
                                                  FieldNpcDetectionCursor &,
                                                  FieldNpcMarioCursor &);

  FieldNpcDisplayFrontier advanceDisplay(FieldNpcMainEntryCursor &,
      FieldNpcMainMoveCursor &, FieldNpcAutoPoseCursor &,
      FieldNpcDetectionCursor &, FieldNpcMarioCursor &, FieldNpcDisplayCursor &);

  FieldNpcDisplayFrontier advanceShadow(FieldNpcMainEntryCursor &,
      FieldNpcMainMoveCursor &, FieldNpcAutoPoseCursor &,
      FieldNpcDetectionCursor &, FieldNpcMarioCursor &, FieldNpcDisplayCursor &,
      FieldNpcShadowCursor &);

  // Complete actor traversal only; returned FieldTail still requires the field tail.
  FieldNpcActorsFrontier advanceActors(FieldNpcMainActorsCursor &);

  // One complete source invocation only after all real child effects and final
  // retained-F byte clear. Reset after completion/pass retirement, not on hold.
  bool advanceMain(FieldNpcMainCursor &);

  // Complete supported queued callback only. Does not register/dispatch it or
  // replace the normal renderer. Keep this binding alive until completion.
  bool advanceDraw(uint32_t callback, uint32_t actor, FieldNpcDrawCursor &, uint32_t camera = 0);

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
