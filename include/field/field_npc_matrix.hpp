#pragma once
#include "battle_audience_matrix.hpp"
#include <optional>
namespace ttyd {
// Original numeric calls; camera result is its actual source address. Angle
// takes fromX/fromZ/toX/toZ, revise takes lane0. False has no effects.
struct FieldNpcMatrixNumber {
  uint32_t function = 0, word = 0;
  std::array<float, 4> args{};
  float result = 0;
};
struct FieldNpcMatrixServices {
  void *user = nullptr;
  std::optional<uint32_t> (*word)(void *, uint32_t) = nullptr;
  bool (*number)(void *, FieldNpcMatrixNumber &) = nullptr;
  bool (*matrix)(void *, BattleAudienceMatrixRequest &) = nullptr;
};
// One source matrix visit: actor/raw slot/providers captured only at entry.
// Later call arguments cannot replace that owner while active. pending holds a
// stable numeric/SDK request; output/rotation are native3x4 matrix payloads.
struct FieldNpcMatrixCursor {
  bool active = false, pending = false;
  FieldNpcMatrixServices owner;
  uint32_t actor = 0, slot = 0, phase = 0, camera = 0, bits = 0, axis = 0;
  float zero = 0, one = 0, correction = 0, reflection = 0, angle = 0, first = 0;
  std::array<float, 4> values{};
  BattleActMatrix output{}, rotation{};
  FieldNpcMatrixNumber number;
  BattleAudienceMatrixRequest math;
};
// Complete mtx_setup8004ca80. slot=-1 uses current root; all other signed slot
// bits use original wrapping actor+a4+12*slot addressing. No pose/Main/draw.
// Values are captured at original reads, including repeated pivot/scale reads.
// Retain source mappings and producer pass across holds; successful primitives
// are never replayed. Result is published only on completion; cursor resets.
// No actor-slot bounds policy is added here. The word provider admits actual
// mappings, including wrapped negative history slots. Unlike the clock helper,
// this cursor uses captured arguments rather than rejecting changed retry args.
bool advanceFieldNpcMatrix(uint32_t actor, int32_t slot,
                           const FieldNpcMatrixServices &,
                           FieldNpcMatrixCursor &, BattleActMatrix &result);
} // namespace ttyd
