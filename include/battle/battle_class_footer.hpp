#pragma once
#include "battle_class_raster.hpp"
namespace ttyd {
struct BattleClassFooterServices {
  BattleClassRasterServices raster;
  void *user = nullptr;
  std::optional<std::array<float, 3>> (*project)(void *, uint32_t) = nullptr;
  // Original GetItemOn writes four private outputs; only its first output is
  // consumed by the local footer body. No unused output/status is fabricated.
  std::optional<int32_t> (*audienceItem)(void *, uint32_t) = nullptr;
  std::optional<bool> (*changePosition)(void *, uint32_t) = nullptr;
  bool (*sound)(void *, uint32_t) = nullptr;
  bool (*button)(void *, int32_t, float, float, float) = nullptr;
};
bool packedBattleClassPartyButton(const BattleClassFooterServices &);
bool packedBattleClassPinchMarks(const BattleClassFooterServices &);
bool packedBattleClassMarioPinch(const BattleClassFooterServices &);
bool packedBattleClassPartyPinch(const BattleClassFooterServices &);
bool packedBattleClassButton(int32_t, float, float, float,
                             const BattleClassFooterServices &);
} // namespace ttyd
