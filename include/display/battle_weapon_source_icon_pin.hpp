#pragma once
#include "battle_weapon_ordered_publication.hpp"
#include <set>
#include <functional>
namespace ttyd {
// Native cache intent/retention only, not source readiness or GPU residency.
// Actual weapon context owns this and the actual shared ordered publication.
class BattleWeaponSourceIconPin {
public:
 // Actual long W/process-resource proof, independent of short camera lease.
 // lifetime pins the proof holder; current must be pure and must not reenter
 // Renderer/resource retirement. Prefer weak Context + actual strong tokens
 // to avoid Context->Pin->Context cycles.
 BattleWeaponSourceIconPin(const std::shared_ptr<BattleWeaponOrderedPublication>&,
                          std::shared_ptr<const void> lifetime,std::function<bool()> current);
 bool markPending(uint32_t image);
 // Drops pending intent only after the actual building/sealed owner retains it.
 bool acknowledge(uint32_t image);
 bool retains(uint32_t image)const;
 bool alive()const;
 void retire();
private:
 std::weak_ptr<BattleWeaponOrderedPublication> owner_;
 std::set<uint32_t> pending_;
 std::shared_ptr<const void> lifetime_;std::function<bool()> current_;
 mutable bool invoking_=false,invalid_=false;
 bool valid()const;
};
// Renderer stores only weak actual context pins, never a raw owner pointer.
class BattleWeaponSourceIconPins {
public:
 bool attach(const std::shared_ptr<BattleWeaponSourceIconPin>&);
 bool retains(uint32_t image)const;
 void clear(bool retirePins=false);
private:
 std::vector<std::weak_ptr<BattleWeaponSourceIconPin>> pins_;
 mutable bool invoking_=false;
};
}
