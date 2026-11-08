#pragma once
#include "battle_audience_matrix.hpp"
#include <optional>
namespace ttyd {
// Fixed camera/atan2 ABI: argument4 is the camera selector; scalars carry
// negative X/Z deltas. word is camera token,number the double atan2 result.
struct AnimationFacingCall {
  uint32_t function = 0, argument = 0, word = 0;
  std::array<float, 2> scalars{};
  double number = 0;
};
struct AnimationFacingServices {
  void *user = nullptr;
  std::optional<uint32_t> (*word)(void *, uint32_t) = nullptr;
  bool (*store)(void *, uint32_t, uint32_t) = nullptr;
  bool (*call)(void *, AnimationFacingCall &) = nullptr;
  bool (*matrix)(void *, BattleAudienceMatrixRequest &) = nullptr;
};
struct AnimationHeadingCursor {
  bool active = false, pending = false;
  uint32_t phase = 0, camera = 0;
  AnimationFacingServices owner;
  AnimationFacingCall call;
  std::array<float, 4> fields{};
  float value = 0, factor = 0, turn = 0, bound = 0;
};
struct AnimationSignedWrapCursor {
  bool active = false, add = false;
  uint32_t phase = 0;
  AnimationFacingServices owner;
  float left = 0, right = 0, value = 0, turn = 0, bound = 0;
};
// Durable copied operands and sub-cursors. This owns no producer/VM lifetime;
// abandoning it cannot undo acknowledged pose writes or SDK child effects.
struct AnimationFacingCursor {
  bool active = false, inner = false, pending = false, rear = false;
  uint32_t pose = 0, phase = 0;
  AnimationFacingServices owner;
  float incoming = 0, scale = 0, angle = 0, literal = 0, other = 0, target = 0,
        old = 0, difference = 0, turn = 0;
  BattleActMatrix model{}, operand{};
  BattleAudienceMatrixRequest math;
  AnimationHeadingCursor heading;
  AnimationSignedWrapCursor wrap;
};
// Complete optional Main tail80040540..40648 ONLY; caller must retain a
// successfully completed evaluation boundary and earlier null-descriptor exit.
// False may follow captured reads/stores: retain this child, never replay Main.
bool advanceAnimationMainFacing(uint32_t pose, const AnimationFacingServices &,
                                AnimationFacingCursor &);
// Complete Inner facing + uniform scale8003be8c..c210 for either flag8 state.
// Uses actual pose/camera/constant VM addresses, not detached typed metadata.
// Output publishes only at completion. Stores/SDK effects occur in source
// order.
bool advanceAnimationInnerFacing(uint32_t pose, float incoming, float scale,
                                 const BattleActMatrix &,
                                 const AnimationFacingServices &,
                                 AnimationFacingCursor &,
                                 BattleActMatrix &output);
// All primitive false/empty results have no effects. Owners/mappings stay live
// across holds. Source wrap loops yield after one backedge per resume (native
// scheduling policy), without a finite clamp or total iteration cap.
} // namespace ttyd
