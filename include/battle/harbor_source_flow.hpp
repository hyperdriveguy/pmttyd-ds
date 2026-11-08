#pragma once
#include <memory>
#include <functional>
#include "event_native_runtime.hpp"
#include "battle_first_act_continuation.hpp"
#include "battle_turn_continuation.hpp"
#include "battle_phase_move_continuation.hpp"
#include "battle_act_continuation.hpp"
namespace ttyd {
struct HarborSourceFlowBinding {
 EventNativeRuntime*vm=nullptr;
 // Actual allocation/process ownership, separate from short camera epochs.
 // VM is borrowed and must outlive this owner and every held source invocation.
 std::shared_ptr<const void>lifetime;
 std::function<bool()>current;
 // Only original reached child effects not owned by this VM adapter. False/
 // nullopt retains that child's partial work; successful0 remains source0.
 std::function<bool(uint32_t function,uint32_t capturedBattle)>main;
 std::function<std::optional<uint32_t>(const BattleFirstActRequest&)>startup;
 std::function<std::optional<BattleTurnOutput>(const BattleTurnRequest&)>turn;
 std::function<std::optional<BattleActOutput>(const BattleActSequenceRequest&)>act;
 std::function<std::optional<int32_t>(uint32_t battle)>commandInput;
 std::function<bool(uint32_t battle)>end;
 std::function<bool(uint32_t sourceKey)>pinchSound;
 std::function<std::optional<int32_t>(uint32_t item)>removeItem;
};
// Concrete shared-VM Main→Init/FirstAct/Pinch/Turn→Phase/Move/Act graph. No
// simulation tick, field event manager or renderer scheduling is invented here.
// Caller invokes resumeMain at its admitted Main point, then advances the real
// outer process only on completion. All successful source children stay retained.
class HarborSourceFlow {
public:
 static std::unique_ptr<HarborSourceFlow>create(HarborSourceFlowBinding);
 ~HarborSourceFlow();
 HarborSourceFlow(const HarborSourceFlow&)=delete;
 HarborSourceFlow&operator=(const HarborSourceFlow&)=delete;
 bool resumeMain();
 bool active()const;uint32_t pending()const;bool current()const;
private:
 HarborSourceFlow();struct State;std::unique_ptr<State>state_;mutable bool invoking_=false;
};
}
