#pragma once
#include "battle_actor.hpp"
#include <array>
namespace ttyd {
struct BattleUnitSetupServices {
 BattleActorServices memory;
 void* user=nullptr;
 void (*storeByte)(void*,uint32_t,uint8_t)=nullptr;
 uint32_t (*pouch)(void*)=nullptr;
 uint32_t (*entry)(void*,uint32_t originalDefinition)=nullptr;
 void (*equip)(void*,uint32_t unit,int32_t sourceMode,int32_t item)=nullptr;
 void (*pouchParameters)(void*,uint32_t unit)=nullptr;
 std::array<float,3> (*basePosition)(void*,uint32_t unit)=nullptr;
 void (*setBasePosition)(void*,uint32_t unit,const std::array<float,3>&)=nullptr;
 void (*setPosition)(void*,uint32_t unit,const std::array<float,3>&)=nullptr;
};
// Complete Btl_UnitSetup800f8aac. Entry owns actual packed allocation/init roots;
// the source system definition is mandatory and must not be omitted by renderer.
int32_t packedBattleUnitSetup(uint32_t battle,const BattleUnitSetupServices&);
}
