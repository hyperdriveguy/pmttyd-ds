#pragma once
#include "battle_actor.hpp"
#include <array>
#include <optional>
namespace ttyd {
struct BattleStageObjectServices {
    using Point=std::array<float,3>;
    BattleActorServices actors;
    void* user=nullptr;
    bool (*translate)(void*,uint32_t name,const Point&)=nullptr;
    bool (*rotate)(void*,uint32_t name,const Point&)=nullptr;
    std::optional<uint32_t> (*groupFlags)(void*,uint32_t name)=nullptr;
    bool (*groupFlag)(void*,uint32_t name,uint32_t mask,bool on)=nullptr;
};
// Source StageObjectMain80149798: all32 records, cache writes before map calls,
// layer-specific transforms and independent visibility synchronization.
// False is native provider unavailability, not an original return/rollback.
bool packedBattleStageObjectMain(const BattleStageObjectServices&);
}
