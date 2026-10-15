#pragma once
#include "battle_actor.hpp"
#include <optional>
// Gauge uses unit halfwords1fc(mode), 1fe(flags), 200(timer), 202(duration),
//204(fade timer), 206(lifetime), words208/20c(HP snapshots), 210/214(ratios), hex.
// These source counters do not measure native rendered frames.
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
