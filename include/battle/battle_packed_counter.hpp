#pragma once
#include "battle_actor.hpp"
#include <array>
namespace ttyd {
// Original 0x30-byte scratch storage; offsets remain explicit for CheckDamage.
struct BattleCounterWork {
 std::array<uint32_t,12> words{};
 void init();
};
struct BattlePackedCounterServices {
 BattleActorServices memory;
 void* user=nullptr;
 bool (*status)(void*,uint32_t unit,int32_t type)=nullptr;
};
// Original BattleCheckCounter800fa678. Initializes only count(+28), and can
// overwrite the final counter code while retaining several accumulated gates.
// Caller owns BattleInitCounterPreCheckWork before this call and flags by ref.
int32_t packedBattleCheckCounter(BattleCounterWork&,uint32_t attacker,
 uint32_t defender,uint32_t part,uint32_t weapon,uint32_t& flags,
 const BattlePackedCounterServices&);
}
