#pragma once
#include "event_memory_opcodes.hpp"
#include "battle_audience_lifecycle.hpp"

// Guest-address services for the 64-slot source registry; selectors and slots
// are different namespaces. Callers must admit original memory before lookup.
namespace ttyd {
struct BattleActorServices {
    EventMemoryServices memory;
    void* user=nullptr;
    uint8_t (*byte)(void*,uint32_t)=nullptr;
    BattleAudienceLifecycleHook audienceLifecycle;
};
uint32_t currentBattleAddress(const BattleActorServices&);
uint32_t battleRegistryUnit(uint32_t battle,int32_t id,const BattleActorServices&);
void setBattleRegistryUnit(uint32_t battle,int32_t id,uint32_t unit,const BattleActorServices&);
uint32_t battleMarioUnit(uint32_t battle,const BattleActorServices&);
uint32_t battlePartyUnit(uint32_t battle,const BattleActorServices&);
uint32_t battleSystemUnit(uint32_t battle,const BattleActorServices&);
int32_t translateBattleActor(EventEntry* event,int32_t selector,const BattleActorServices&);
}
