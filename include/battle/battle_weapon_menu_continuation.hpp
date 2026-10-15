#pragma once
#include <memory>
#include <array>
#include <optional>
#include "battle_actor.hpp"
namespace ttyd {
struct BattleWeaponMenuContinuationRequest {
 uint32_t function=0,callsite=0;
 std::array<uint32_t,4> arguments{};uint8_t count=0;
 std::optional<uint32_t>* privateCursorOutput=nullptr;
};
struct BattleWeaponMenuContinuationServices {
 BattleActorServices actors;void* user=nullptr;
 std::optional<uint32_t> (*call)(void*,const BattleWeaponMenuContinuationRequest&)=nullptr;
};
class BattleWeaponMenuContinuationCursor {
public:
 BattleWeaponMenuContinuationCursor();~BattleWeaponMenuContinuationCursor();
 BattleWeaponMenuContinuationCursor(const BattleWeaponMenuContinuationCursor&)=delete;
 BattleWeaponMenuContinuationCursor& operator=(const BattleWeaponMenuContinuationCursor&)=delete;
 bool active()const;uint32_t pending()const;uint32_t callsite()const;bool reset();
private:
 struct State;std::unique_ptr<State> state_;bool invoking_=false;
 friend std::optional<int32_t> resumePackedBattleWeaponMenuMain(uint32_t,uint32_t,const BattleWeaponMenuContinuationServices&,BattleWeaponMenuContinuationCursor&);
 friend bool resumePackedBattleWeaponMenuSetup(uint32_t,uint32_t,uint32_t,const BattleWeaponMenuContinuationServices&,BattleWeaponMenuContinuationCursor&);
 friend bool resumePackedBattleWeaponMenuEnd(uint32_t,const BattleWeaponMenuContinuationServices&,BattleWeaponMenuContinuationCursor&);
};
// Whole original Main80121d0c/35c, Setup80122068/6c, End80121cb0/38.
// Engaged0 completes a source Main invocation; nullopt retains unavailable work.
// Main composes its actual End child internally, preserving UI-before-flag order.
std::optional<int32_t> resumePackedBattleWeaponMenuMain(uint32_t battle,uint32_t type,const BattleWeaponMenuContinuationServices&,BattleWeaponMenuContinuationCursor&);
bool resumePackedBattleWeaponMenuSetup(uint32_t battle,uint32_t type,uint32_t flags,const BattleWeaponMenuContinuationServices&,BattleWeaponMenuContinuationCursor&);
bool resumePackedBattleWeaponMenuEnd(uint32_t battle,const BattleWeaponMenuContinuationServices&,BattleWeaponMenuContinuationCursor&);
}
