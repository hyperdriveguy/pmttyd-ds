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
// Free item0/base0 weapons need neither a unit read nor a pouch count.
std::optional<uint32_t>resumePackedWeaponCost(uint32_t unit,uint32_t weapon,const PackedWeaponCostServices&,PackedWeaponCostCursor&);
}
