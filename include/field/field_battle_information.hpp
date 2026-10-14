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
// Original areaDataPtr/relSetBtlAddr. Registry records are caller-owned mutable
// guest memory; REL loading owns table registration and unloading.
uint32_t packedBattleAreaData(const char* name,const BattleActorServices&,
 uint32_t registry=0x803057e4);
void registerPackedBattleArea(const char* name,uint32_t setupTable,uint32_t numberTable,
 const BattleActorServices&,uint32_t registry=0x803057e4);
// Original dbGetDefData: first exact name match, missing name returns0.
int32_t packedBattleSetupNumber(uint32_t table,const char* name,const BattleActorServices&);
// Original npcSetBattleInfo. Reads the current GP from 0x80415fd0 for each
// non--1 request. Caller must admit registry/configuration/actor memory first;
// missing area/table and invalid indices have no invented fallback.
void setPackedNpcBattleInformation(uint32_t npc,int32_t index,
 const PackedNpcBattleSetupServices&,uint32_t registry=0x803057e4);
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
