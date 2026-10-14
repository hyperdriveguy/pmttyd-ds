#pragma once
#include "battle_audience_callback.hpp"
namespace ttyd {
struct BattleAudienceMatrixRequest {
    uint32_t function=0,axis=122;BattleActMatrix left{},right{},output{};
    std::array<float,3> xyz{};float radians=0;
};
struct BattleAudienceIconRequest {
    uint32_t function=0,flags=0,icon=0,rgba=0;BattleActMatrix matrix{};
};
}
