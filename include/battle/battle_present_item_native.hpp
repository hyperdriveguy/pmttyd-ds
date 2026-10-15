#pragma once
#include <memory>
#include <array>
#include <optional>
#include "battle_actor.hpp"
namespace ttyd {
struct BattlePresentItemNativeServices {
 BattleActorServices actors;
 void* user=nullptr;
 // Required actual allocation lease; caller freezes it through synchronous
 // source memory/recovery primitives. False poisons this invocation until reset.
 bool (*allocationCurrent)(void*)=nullptr;
 std::optional<float> (*subtractDoubleSingle)(void*,double,double)=nullptr;
 std::optional<float> (*fma)(void*,float,float,float)=nullptr;
 std::optional<float> (*add)(void*,float,float)=nullptr;
 // Actual retained whole effRecoveryEntry child; false is unavailable, not a
 // source return value. The same owned position survives every held call.
 bool (*effect)(void*,uint32_t,int32_t,const std::array<float,3>&)=nullptr;
};
// Captures actual recovery and private XYZ/math/effect prefixes exactly once.
// Stable callbacks do not suffice without the real allocation lease; observed
// loss poisons until reset. Reset cannot undo HP/FP or prior source stores.
class BattlePresentItemNativeCursor {
public:
 BattlePresentItemNativeCursor();~BattlePresentItemNativeCursor();
 BattlePresentItemNativeCursor(const BattlePresentItemNativeCursor&)=delete;
 BattlePresentItemNativeCursor& operator=(const BattlePresentItemNativeCursor&)=delete;
 bool active()const;bool stale()const;uint32_t pending()const;bool reset();
private:
 struct State;std::unique_ptr<State> state_;bool invoking_=false;
 friend std::optional<int32_t> resumeNativeBattlePresentItem(const BattlePresentItemNativeServices&,BattlePresentItemNativeCursor&);
};
// Whole80120024 with native-owned disjoint XYZ corresponding to the original
// private stack output. No guest stack address, source timing or effect invented.
std::optional<int32_t> resumeNativeBattlePresentItem(const BattlePresentItemNativeServices&,BattlePresentItemNativeCursor&);
}
