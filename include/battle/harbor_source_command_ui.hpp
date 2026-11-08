#pragma once
#include "battle_actor.hpp"
#include "battle_weapon_evaluation_continuation.hpp"
#include <array>
#include <functional>
#include <memory>
#include <optional>
namespace ttyd {
// One original child boundary, never a modal/case acknowledgement. A provider
// returning unavailable retains its own partial effects and captured operands.
struct HarborSourceCommandUiRequest {
 uint32_t function=0,callsite=0;
 std::array<uint32_t,4> arguments{};
 uint8_t count=0;
};
struct HarborSourceCommandUiBinding {
 BattleActorServices actors;
 uint32_t battle=0;
 // Packed owns this UI; a weak actual-runtime registration avoids a cycle.
 // Every invocation locks it before touching actors/user or invoking current.
 std::weak_ptr<const void> runtime;
 std::function<bool()> current;
 void* user=nullptr;
 std::optional<uint32_t> (*child)(void*,const HarborSourceCommandUiRequest&)=nullptr;
 // Optional reads distinguish unknown native history from a written zero.
 std::optional<uint32_t> (*word)(void*,uint32_t)=nullptr;
 // Additional ORIGINAL leaf providers. Unknown math/sampling stays held.
 void* byteUser=nullptr;
 std::optional<uint8_t> (*byte)(void*,uint32_t)=nullptr;
 std::optional<uint32_t> (*evaluationChild)(void*,const BattleWeaponEvaluationRequest&)=nullptr;
 void* evaluationUser=nullptr;
 uint32_t privateSamplingBuffer=0;
 std::optional<double> (*power)(void*,double,double)=nullptr;
 std::optional<int32_t> (*truncate)(void*,double)=nullptr;
};
// Persistent real Input/Act/Weapon/ClassTable/UI/Audience composition. No
// sampling, menu-state fabrication, presentation acknowledgement or attack
// scheduling occurs here. One engaged result completes one source invocation.
class HarborSourceCommandUi {
public:
 explicit HarborSourceCommandUi(HarborSourceCommandUiBinding);
 ~HarborSourceCommandUi();
 HarborSourceCommandUi(const HarborSourceCommandUi&)=delete;
 HarborSourceCommandUi& operator=(const HarborSourceCommandUi&)=delete;
 std::optional<int32_t> commandInput();
 bool pending()const;
 uint32_t pendingFunction()const;
 bool stale()const;
 uint32_t battleAddress()const;
private:
 struct State;std::unique_ptr<State> state_;
};
}
