#pragma once
#include "battle_actor.hpp"
#include <array>
#include <optional>

namespace ttyd {
inline constexpr std::array<uint32_t,11> battleMainRequests{{
    0x800f7b28,0x8011a43c,0x8011f1ec,0x800fa5b0,0x80149798,
    0x801aa168,0x80139150,0x802168a4,0x801a836c,0x80203aa0,0x801024f0
}};
struct BattleMainServices {
    BattleActorServices actors;
    void* user=nullptr;
    // capturedBattle is owner context, not an invented argument to every ABI.
    // False means an unavailable native provider, not an original callee return.
    bool (*request)(void*,uint32_t function,uint32_t capturedBattle)=nullptr;
};
// Upper BattleMain800f8e28. An incomplete provider never produces an engine
// result. Each successful invocation executes all eleven services afresh.
std::optional<int32_t> packedBattleMain(const BattleMainServices&);
// Lower battleMain800f904c: null owner succeeds without work. False reports an
// incomplete provider; completion only ORs the captured owner's end bit.
bool packedBattleMainWrapper(const BattleMainServices&);
bool packedBattleSequenceEndCheck(const BattleActorServices&);
}
