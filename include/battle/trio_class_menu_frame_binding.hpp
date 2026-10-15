#pragma once
#include "battle_class_native_frame.hpp"
namespace ttyd {
// Returned only for a real currently owned class UI allocation. Predicates and
// lookup/RNG closures retain the actual runtime. The public Trio facade must
// outlive use; replacing its runtime makes the old binding unavailable.
struct TrioClassMenuFrameBinding {
  BattleClassNativeFrameBinding native;
  BattleClassDisplayServices display;
  std::function<const char*(uint32_t)> label;
  std::function<uint32_t(uint32_t)> random;
  std::function<std::optional<uint32_t>(uint32_t)> resourceWord;
};
}
