#pragma once
#include "battle_actor.hpp"
#include "event_creation.hpp"
#include <array>
namespace ttyd {
struct BattleUnitEntryServices {
 BattleActorServices memory;void* user=nullptr;
 void (*storeByte)(void*,uint32_t,uint8_t)=nullptr;
 uint32_t (*allocate)(void*,uint32_t size)=nullptr;
 uint32_t (*pouch)(void*)=nullptr;
 // The exact original helper address is retained; provider executes that helper.
 void (*unary)(void*,uint32_t sourceFunction,uint32_t unit)=nullptr;
 void (*vector)(void*,uint32_t sourceFunction,uint32_t unitOrPart,const std::array<float,3>&)=nullptr;
 uint32_t (*bodyPart)(void*,uint32_t unit)=nullptr;
 uint32_t (*poseName)(void*,uint32_t part,int32_t type)=nullptr;
 EventEntry& (*create)(void*,uint32_t script,int32_t priority,uint32_t flags)=nullptr;
};
// Full BtlUnit_Entry80128fe0 storage/control ordering. Providers must be complete
// before entry starts: no unbound display/status/allocation services are skipped.
// Returns0 only for original full-registry or initial allocation failure routes.
uint32_t packedBattleUnitEntry(uint32_t battle,uint32_t definition,const BattleUnitEntryServices&);
}
