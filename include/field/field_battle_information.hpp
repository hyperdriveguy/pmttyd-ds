#pragma once
#include "battle_actor.hpp"
namespace ttyd {
struct PackedNpcBattleSetupServices {
 BattleActorServices memory;
 void* user=nullptr;
 int32_t (*random)(void*,int32_t bound)=nullptr;
 int32_t (*savedFlag)(void*,int32_t index)=nullptr;
};
// Original npcSetupBattleInfo: configuration pointer already selected by the
// numeric map-area setup table. Native memory remains caller-owned.
void setupPackedNpcBattleInformation(uint32_t npc,uint32_t configuration,
 const PackedNpcBattleSetupServices&);
void initializePackedBattleInformation(uint32_t information,const EventMemoryServices&);
struct PackedFieldBattleInformationServices {
 BattleActorServices memory;
 void* user=nullptr;
 int32_t (*marioPartyId)(void*)=nullptr;
 uint32_t (*party)(void*,int32_t id)=nullptr;
 int32_t (*partyKind)(void*,int32_t id)=nullptr;
};
// Exact battle_init80073940..80073b08 information preparation region; callers
// own prior field/audio/map preparation and the subsequent BattleInit call.
uint32_t preparePackedFieldBattleInformation(uint32_t fieldWork,uint32_t npc,
 uint32_t pouch,const PackedFieldBattleInformationServices&);
}
