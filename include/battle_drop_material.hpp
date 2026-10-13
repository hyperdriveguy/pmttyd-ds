#pragma once
#include "battle_actor.hpp"
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
