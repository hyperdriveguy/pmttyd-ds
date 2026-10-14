#pragma once
#include "battle_sequence_startup.hpp"
#include "event_creation.hpp"
namespace ttyd {
struct BattleFirstActServices {
 BattleActorServices memory;
 void* user=nullptr;
 uint16_t (*fieldMarioMotion)(void*)=nullptr;
 uint32_t (*unitData)(void*,uint32_t unit,int32_t key)=nullptr;
 EventEntry& (*create)(void*,uint32_t script,int32_t priority,uint32_t flags)=nullptr;
 // Other first-attack modes use their separate original weapon/actor services.
 // Missing provider leaves that state unchanged and reports unsupported=false.
 void (*firstAttack)(void*,uint32_t battle,int32_t sourceType)=nullptr;
 bool (*eventExists)(void*,uint32_t id)=nullptr;
 bool (*allEventsReady)(void*,uint32_t battle)=nullptr;
 bool (*concluded)(void*,uint32_t battle)=nullptr;
 int32_t (*statusMessage)(void*,uint32_t battle)=nullptr;
 int32_t (*statusAnnounce)(void*,uint32_t battle)=nullptr;
 void (*statusAdjust)(void*,uint32_t battle)=nullptr;
 void (*statusEventOn)(void*)=nullptr;
 void (*statusSystemOn)(void*)=nullptr;
 void (*consumeReserve)(void*)=nullptr;
 void (*audiencePerAct)(void*)=nullptr;
 int32_t (*audienceReaction)(void*)=nullptr;
 int32_t (*slotReaction)(void*)=nullptr;
 void (*wallClose)(void*)=nullptr;
 void (*nozzleCheck)(void*,int32_t mode)=nullptr;
 int32_t (*status)(void*,uint32_t unit,int32_t type)=nullptr;
 void (*fadeEntry)(void*,int32_t type,int32_t rawDuration,uint32_t color)=nullptr;
 bool (*fadeFinished)(void*)=nullptr;
 uint32_t (*sound)(void*,uint32_t sourceKey)=nullptr;
 void (*soundOff)(void*,uint32_t voice)=nullptr;
};
// Full ordinary firstAttack0 entry and shared states1..14 of btlseqFirstAct.
// true means the source controller state was handled, not battle completion.
// false only leaves an unsupported nonzero first-attack entry for its provider.
bool packedBattleFirstAct(uint32_t battle,const BattleFirstActServices&);
}
