#pragma once
#include "battle_actor.hpp"
#include <optional>
namespace ttyd {
void initializePackedBattleHpGauge(uint32_t unit,const BattleActorServices&);
float battleHpGaugeCurve(int32_t mode,int32_t step,int32_t duration,float start,float end);
struct BattleHpGaugeServices {
    BattleActorServices actors;
    void* user=nullptr;
    std::optional<float> (*interpolate)(void*,int32_t mode,int32_t step,int32_t duration,float start,float end)=nullptr;
};
bool packedBattleHpGaugeMain(uint32_t unit,const BattleHpGaugeServices&);
}
