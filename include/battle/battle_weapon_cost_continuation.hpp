#pragma once
#include <cstdint>
#include <memory>
#include <optional>
namespace ttyd {
struct PackedWeaponCostServices {
 void*user=nullptr;std::shared_ptr<const void>lifetime;
 bool(*current)(void*)=nullptr;
 std::optional<uint32_t>(*word)(void*,uint32_t)=nullptr;
 std::optional<uint8_t>(*byte)(void*,uint32_t)=nullptr;
 // Actual pouchEquipCheckBadge800d3300, pow80272620 and fctiwz children.
 std::optional<uint32_t>(*equippedCount)(void*,uint32_t item)=nullptr;
 std::optional<double>(*power)(void*,double,double)=nullptr;
 std::optional<int32_t>(*truncate)(void*,double)=nullptr;
};
// One unit/weapon invocation pins provider/control-block identity through holds.
// A completed scalar0 consumes the cursor; absent result retains it. Reentry/
// reset during a callback is refused; observed current loss stales until reset.
class PackedWeaponCostCursor {
public:
 PackedWeaponCostCursor();~PackedWeaponCostCursor();
 PackedWeaponCostCursor(const PackedWeaponCostCursor&)=delete;
 PackedWeaponCostCursor&operator=(const PackedWeaponCostCursor&)=delete;
 bool active()const;bool stale()const;uint32_t pending()const;bool reset();
private:
 struct State;std::unique_ptr<State>state_;bool invoking_=false;
 friend std::optional<uint32_t>resumePackedWeaponCost(uint32_t,uint32_t,const PackedWeaponCostServices&,PackedWeaponCostCursor&);
};
// Whole BtlUnit_GetWeaponCost80126968/158 over actual packed addresses.
// Item0 skips pouch count; a resulting cost0 skips unit reduction. Nonzero
// item still queries its equipped count even when captured base FP is0.
std::optional<uint32_t>resumePackedWeaponCost(uint32_t unit,uint32_t weapon,const PackedWeaponCostServices&,PackedWeaponCostCursor&);
}
