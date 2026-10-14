#pragma once
#include "battle_unit_event.hpp"
namespace ttyd {
struct BattleStatusWindowServices {
 BattleUnitEventServices units;
 void* user=nullptr;
 bool (*isOpen)(void*)=nullptr;
 void (*open)(void*)=nullptr;
 void (*close)(void*)=nullptr;
};
int32_t packedBattleStatusWindowCheck(const BattleStatusWindowServices&);
void packedBattleStatusWindowSet(uint32_t field,bool on,const BattleStatusWindowServices&);
int32_t battleStatusWindowOnOffEvent(EventEntry&,const BattleStatusWindowServices&);
}
