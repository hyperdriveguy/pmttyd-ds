#pragma once
#include <memory>
#include "battle_act_draw.hpp"
namespace ttyd {
// Every optional models an actually unwritten source private stack cell.
struct BattleWeaponDrawSlot {
 std::optional<BattleActMatrix> matrix;
 std::optional<uint32_t> label,enabled,cost,bp;
 std::optional<uint16_t> icon;
};
struct BattleWeaponDrawOutput {
 std::optional<float> helpX,helpY;
 std::vector<BattleActDrawSlot> classes;
 std::array<BattleWeaponDrawSlot,7> weapons;
};
struct BattleWeaponDrawRequest {
 uint32_t function=0,callsite=0;
 std::array<uint32_t,4> arguments{};uint8_t count=0;
 // Stable borrowed original private outputs; only original drawing children.
 const BattleWeaponDrawOutput* output=nullptr;
 std::optional<std::array<uint32_t,3>> cursorWindow;
 std::optional<float> x,y;
};
struct BattleWeaponDrawServices {
 BattleActDrawServices math;
 void* user=nullptr;
 // Actual owner/pass/Camera8 and queued80121ce8 lease; false latches stale.
 bool (*current)(void*)=nullptr;
 // Exact Unit_GetWeaponCost/DrawMainIcon/DrawSubIcon/cursor/help children.
 // Child must retain its complete invocation through partial waits. No empty
 // success or source readiness prefix substitutes for whole raster completion.
 std::optional<uint32_t> (*request)(void*,const BattleWeaponDrawRequest&)=nullptr;
 // Original per-instruction conversion/FMA policy for finite source operands.
 // fctiwz is used on live W98 at each original site, never a cached scroll.
 std::optional<int32_t> (*truncate)(void*,float)=nullptr;
 std::optional<float> (*fma)(void*,float,float,float)=nullptr;
 std::optional<BattleActMatrix> (*identity)(void*)=nullptr;
};
class BattleWeaponDrawCursor {
public:
 BattleWeaponDrawCursor();~BattleWeaponDrawCursor();
 BattleWeaponDrawCursor(const BattleWeaponDrawCursor&)=delete;
 BattleWeaponDrawCursor& operator=(const BattleWeaponDrawCursor&)=delete;
 bool active()const;bool stale()const;uint32_t pending()const;bool reset();
private:
 struct State;std::unique_ptr<State> state_;bool invoking_=false;
 friend bool resumePackedBattleWeaponQueuedDraw(uint32_t,uint32_t,const BattleWeaponDrawServices&,BattleWeaponDrawCursor&);
};
// Complete queued80121ce8→Disp8011604c→DrawWeaponWin80115dac→Common/Sub1.
// Source ignores camera and passed B after wrappers; keep both as admission
// metadata without substituting them for the draw body's fresh global capture.
bool resumePackedBattleWeaponQueuedDraw(uint32_t camera,uint32_t battle,const BattleWeaponDrawServices&,BattleWeaponDrawCursor&);
}
