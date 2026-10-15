#pragma once
#include "battle_class_backend.hpp"
#include "battle_class_footer.hpp"
namespace ttyd {
// Original common-bank4..16 regions only. GPU texture admission is a separate
// native owner; a valid region never acknowledges an unbound draw request.
class BattleClassFooterAssets {
public:
  bool load(const char *catalog);
  const BattleClassRegion *surface(uint32_t index) const {
    return ready_ && index >= 4 && index <= 16 ? &regions_[index - 4] : nullptr;
  }

private:
  bool ready_ = false;
  std::array<BattleClassRegion, 13> regions_{};
};
// Retained native bridge for the original footer bodies. Raster requests must
// reach actual common-bank resources/GPU admission; no fallback
// acknowledgements.
struct BattleClassFooterCursor {
  uint32_t operation = 0, phase = 0, child = 0, order = 0, battle = 0, work = 0,
           mario = 0, party = 0, camera = 0, unit = 0, index = 0, color = 0,
           texture = 0;
  bool queued = false, partyFirst = false, partyChild = false;
  int32_t type = 0;
  float x = 0, y = 0, angle = 0, scale = 0;
  std::array<float, 3> point{};
  BattleActMatrix a{}, b{};
  BattleClassRasterRequest request{};
  BattleClassFooterServices owner;
};
bool resumeBattleClassPartyFooter(const BattleClassFooterServices &,
                                  BattleClassFooterCursor &);
bool resumeBattleClassPinchFooter(const BattleClassFooterServices &,
                                  BattleClassFooterCursor &);
// Whole original party-change Sub: Y rotation, TransApply, TexturePlane2.
// Invoke using a separate child cursor from the party footer's button provider.
bool resumeBattleClassFooterButton(int32_t type, float x, float y, float angle,
                                   const BattleClassFooterServices &,
                                   BattleClassFooterCursor &);
} // namespace ttyd
