#pragma once
#include "battle_actor.hpp"
#include <array>
#include <memory>
#include <optional>
namespace ttyd {
struct BattleWeaponEvaluationRequest {
 uint32_t function=0,callsite=0;std::array<uint32_t,8> arguments{};uint8_t count=0;
 std::optional<uint32_t>* privateCursorOutput=nullptr;
};
struct BattleWeaponEvaluationServices {
 BattleActorServices actors;void* user=nullptr;bool (*current)(void*)=nullptr;
 std::optional<uint32_t> (*call)(void*,const BattleWeaponEvaluationRequest&)=nullptr;
 // Actual caller-owned VM scratch for the adapted source private sampler.
 // Not an original guest SP address; no live B+428 alias is permitted.
 uint32_t privateBuffer=0;
};
class BattleWeaponEvaluationCursor {
public:
 BattleWeaponEvaluationCursor();~BattleWeaponEvaluationCursor();
 BattleWeaponEvaluationCursor(const BattleWeaponEvaluationCursor&)=delete;
 BattleWeaponEvaluationCursor& operator=(const BattleWeaponEvaluationCursor&)=delete;
 bool active()const;bool stale()const;uint32_t pending()const;bool reset();
private:
 struct State;std::unique_ptr<State> state_;bool invoking_=false;
 friend struct BattleWeaponEvaluationRunner;
};
// Complete original CheckWeaponCost801268fc/6c, CheckWeaponTargetNum80123d00/b0,
// UpdateSelectWeaponTable80123db0/110, SelectWeaponDecide80123210/150.
std::optional<int32_t> resumePackedBattleWeaponCostCheck(uint32_t unit,uint32_t weapon,const BattleWeaponEvaluationServices&,BattleWeaponEvaluationCursor&);
std::optional<int32_t> resumePackedBattleWeaponTargetCount(uint32_t battle,uint32_t unit,uint32_t weapon,const BattleWeaponEvaluationServices&,BattleWeaponEvaluationCursor&);
bool resumePackedBattleWeaponEligibility(uint32_t battle,uint32_t type,const BattleWeaponEvaluationServices&,BattleWeaponEvaluationCursor&);
std::optional<int32_t> resumePackedBattleWeaponDecision(uint32_t battle,uint32_t type,uint32_t index,const BattleWeaponEvaluationServices&,BattleWeaponEvaluationCursor&);
}
