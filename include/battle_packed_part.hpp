#pragma once
#include "battle_unit_event.hpp"
#include "battle_actor.hpp"

namespace ttyd {
struct BattlePartPositionServices {
    BattleUnitEventServices unit;
    BattleActorServices memory;
    void* numericUser=nullptr;
    float (*fma)(void*,float,float,float)=nullptr;
    float (*add)(void*,float,float)=nullptr;
};
uint32_t packedUnitPart(uint32_t unit,int32_t part,const EventMemoryServices&);
uint32_t packedRegistryPart(int32_t unit,int32_t part,const BattleActorServices&);
void copyPackedPartPosition(uint32_t part,bool absolute,const std::array<uint32_t,3>& outputs,const EventMemoryServices&);
std::array<float,3> packedPartWorldPosition(uint32_t part,const BattlePartPositionServices&);
void writePackedPartWorldPosition(uint32_t part,const std::array<uint32_t,3>& outputs,const BattlePartPositionServices&);
int32_t packedPartPositionEvent(EventEntry&,const BattlePartPositionServices&);
}
