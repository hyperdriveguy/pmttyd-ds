#pragma once
#include "battle_actor.hpp"
#include <array>
#include <memory>
#include <optional>
namespace ttyd {
struct BattleWeaponTableContinuationRequest {
 uint32_t function=0,callsite=0;std::array<uint32_t,3> arguments{};uint8_t count=0;
 std::optional<uint32_t>* privateCursorOutput=nullptr;
 // Only the actual source indirect party producer receives this initialized
 // native word. It is not a fabricated guest SP08 address.
 uint32_t* privateCountOutput=nullptr;
};
struct BattleWeaponTableContinuationServices {
 BattleActorServices actors;void* user=nullptr;
 bool (*current)(void*)=nullptr;
 std::optional<uint32_t> (*call)(void*,const BattleWeaponTableContinuationRequest&)=nullptr;
};
class BattleWeaponTableContinuationCursor {
public:
 BattleWeaponTableContinuationCursor();~BattleWeaponTableContinuationCursor();
 BattleWeaponTableContinuationCursor(const BattleWeaponTableContinuationCursor&)=delete;
 BattleWeaponTableContinuationCursor& operator=(const BattleWeaponTableContinuationCursor&)=delete;
 bool active()const;bool stale()const;uint32_t pending()const;bool reset();
private:
 struct State;std::unique_ptr<State> state_;bool invoking_=false;
 friend std::optional<int32_t> resumePackedBattleWeaponTable(uint32_t,uint32_t,const BattleWeaponTableContinuationServices&,BattleWeaponTableContinuationCursor&);
};
// Whole original80123ec0/d50. Source count/descriptor writes and successful
// children are retained, including missing msgSearch after partial row stores.
std::optional<int32_t> resumePackedBattleWeaponTable(uint32_t battle,uint32_t type,const BattleWeaponTableContinuationServices&,BattleWeaponTableContinuationCursor&);
}
