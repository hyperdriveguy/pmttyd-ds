#pragma once
#include <memory>
#include <array>
#include <optional>
#include "battle_actor.hpp"
namespace ttyd {
struct BattleWeaponUiRequest {
 uint32_t function=0,callsite=0,caller=0;
 std::array<uint32_t,2> arguments{};uint8_t count=0;
};
struct BattleWeaponUiServices {
 BattleActorServices actors;void* user=nullptr;
 // Required actual allocation/context lease. Freeze memory/maps through each
 // synchronous source group. False poisons retained ownership until reset.
 bool (*allocationCurrent)(void*)=nullptr;
 // Exact original MapAlloc8002ff58(heap,bytes)/MapFree8002fdc4(heap,pointer).
 // Engaged0 allocation is actual pointer0, not unavailable or repaired success.
 std::optional<uint32_t> (*request)(void*,const BattleWeaponUiRequest&)=nullptr;
};
// Distinct Init/End invocation metadata and provider identities. Actual lease
// predicate is checked between synchronous source groups/after map children;
// caller freezes all memory/maps within each group. Observed loss poisons.
class BattleWeaponUiCursor {
public:
 BattleWeaponUiCursor();~BattleWeaponUiCursor();
 BattleWeaponUiCursor(const BattleWeaponUiCursor&)=delete;
 BattleWeaponUiCursor& operator=(const BattleWeaponUiCursor&)=delete;
 bool active()const;bool stale()const;uint32_t pending()const;bool reset();
private:
 struct State;std::unique_ptr<State> state_;bool invoking_=false;
 friend bool resumePackedBattleWeaponUiInit(uint32_t,uint32_t,uint32_t,uint32_t,const BattleWeaponUiServices&,BattleWeaponUiCursor&);
 friend bool resumePackedBattleWeaponUiEnd(uint32_t,const BattleWeaponUiServices&,BattleWeaponUiCursor&);
};
// Whole original Init80116070/74 + Common8011794c/120 + Common2/3, and
// End8011600c/40. Passed Init B is source-ignored metadata; actual current B
// captured by Common owns allocation. Native private outputs are source0.
bool resumePackedBattleWeaponUiInit(uint32_t ignoredBattle,uint32_t cursor,uint32_t rows,uint32_t flags,const BattleWeaponUiServices&,BattleWeaponUiCursor&);
bool resumePackedBattleWeaponUiEnd(uint32_t battle,const BattleWeaponUiServices&,BattleWeaponUiCursor&);
}
