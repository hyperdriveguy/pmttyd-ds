#pragma once
#include "battle_actor.hpp"
// Drop selection writes original setup outputs only; it grants no pouch
// resources. Source tables/resource maxima/RNG domains are caller-admitted;
// invalid counts are not normalized into a different distribution.
namespace ttyd {
struct BattleDropMaterialServices {
    BattleActorServices actors;
    void* user=nullptr;
    int32_t (*random)(void*,int32_t bound)=nullptr;
    int32_t (*story)(void*)=nullptr;
    int32_t (*equippedBadgeCount)(void*,int32_t id)=nullptr;
};
// Original BattleInformationSetDropMaterial801141bc. The caller owns valid
// original group/table records, nonzero Mario resource maxima and field info.
// Writes setup item/heart/flower fields; grants nothing directly to the pouch.
void battleInformationSetDropMaterial(uint32_t information,const BattleDropMaterialServices&);
}
