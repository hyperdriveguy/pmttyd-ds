#pragma once
#include "battle_actor.hpp"
namespace ttyd {
uint32_t packedBattleGetSequence(uint32_t battle,uint32_t channel,const EventMemoryServices&);
void packedBattleSetSequence(uint32_t battle,uint32_t channel,uint32_t value,const EventMemoryServices&);
void packedBattleIncrementSequence(uint32_t battle,uint32_t channel,const EventMemoryServices&);
struct BattleStartupServices {
 BattleActorServices memory;
 void* user=nullptr;
 uint32_t (*mario)(void*)=nullptr;
 int32_t (*marioPartyId)(void*)=nullptr;
 uint32_t (*party)(void*,int32_t id)=nullptr;
 void (*unitSetup)(void*,uint32_t battle)=nullptr;
 bool (*eventExists)(void*,uint32_t id)=nullptr;
 bool (*paperReady)(void*,uint32_t sourceKey,int32_t context)=nullptr;
 bool (*niceReady)(void*,int32_t mode)=nullptr;
 void (*statusEventOn)(void*)=nullptr;
 void (*statusSystemOn)(void*)=nullptr;
};
// Complete BattleCheckEndUnitInitEvent80129994, cleanup continues after live ID.
bool packedBattleUnitInitReady(uint32_t battle,const BattleStartupServices&);
// Complete btlseqInit8011ee40. Required callbacks must be bound before execution;
// Paper/Nice probes reflect actual resource admission, never elapsed time.
void packedBattleSequenceInit(uint32_t battle,const BattleStartupServices&);
struct BattleSequenceManagerServices {
 EventMemoryServices memory;
 void* user=nullptr;
 void (*init)(void*,uint32_t)=nullptr;
 void (*firstAct)(void*,uint32_t)=nullptr;
 void (*turn)(void*,uint32_t)=nullptr;
 void (*end)(void*,uint32_t)=nullptr;
 void (*pinch)(void*,uint32_t,int32_t mode)=nullptr;
};
// The captured battle owner is explicit; exact source branch/callback order.
void packedBattleSequenceManager(uint32_t battle,const BattleSequenceManagerServices&);
struct BattleNiceAdmissionServices {
 BattleActorServices memory;
 void* user=nullptr;
 bool (*animationReady)(void*,uint32_t sourceKey,int32_t mode,int32_t zero)=nullptr;
};
// Complete effNiceAsync80144d44: language is read anew on every one of7 slots;
// all nonnull rows are queried even after a false result.
bool packedBattleNiceReady(int32_t mode,const BattleNiceAdmissionServices&);
}
