#pragma once
#include "battle_packed_counter.hpp"
#include "battle_damage_application.hpp"
namespace ttyd {
enum class DamageAudienceRequest {GuardGood,GuardBad,EnemyNoDamage,EnemyDamage,Countered};
// Source service boundary for complete BattleCheckDamage800fb01c. Callbacks
// must execute their source services; the controller never replaces them with
// guessed success, resource writes or synthesized event completion.
struct BattleDamageOrchestrationServices {
    BattleActorServices memory;
    void* user=nullptr;
    bool (*status)(void*,uint32_t unit,int32_t type)=nullptr;
    int32_t (*acPossible)(void*,uint32_t unit)=nullptr;
    int32_t (*defenseResult)(void*)=nullptr;
    DamagePosition (*hitPosition)(void*,uint32_t unit,uint32_t part)=nullptr;
    uint32_t (*niceEntry)(void*,int32_t mode,DamagePosition)=nullptr;
    void (*systemSound)(void*,uint32_t sourceKey)=nullptr;
    void (*audience)(void*,DamageAudienceRequest,int32_t amount)=nullptr;
    int32_t (*hpDamage)(void*,uint32_t attacker,uint32_t defender,uint32_t part,uint32_t weapon,uint32_t& code,uint32_t flags)=nullptr;
    int32_t (*fpDamage)(void*,uint32_t attacker,uint32_t defender,uint32_t part,uint32_t weapon,uint32_t& code,uint32_t flags)=nullptr;
    uint32_t (*weaponStatus)(void*,uint32_t attacker,uint32_t defender,uint32_t part,uint32_t weapon,uint32_t flags)=nullptr;
    void (*counterStatus)(void*,uint32_t& result,uint32_t unit,uint32_t part,int32_t element,int32_t type,int32_t rate,int32_t chance,int32_t duration,int32_t strength)=nullptr;
    void (*emergencyRevival)(void*,uint32_t unit,uint32_t& code)=nullptr;
    void (*hitEvent)(void*,uint32_t unit,uint32_t code)=nullptr;
    void (*hitEventDirect)(void*,uint32_t unit,uint32_t code,uint32_t script)=nullptr;
    void (*directDamage)(void*,int32_t attackerId,uint32_t unit,uint32_t part,int32_t hp,int32_t fp,uint32_t code,int32_t pattern,int32_t sleepRecovery)=nullptr;
    int32_t (*irand)(void*,int32_t bound)=nullptr;
    void (*bombEntry)(void*,int32_t mode,DamagePosition,float scale)=nullptr;
    void (*spatialSound)(void*,uint32_t sourceKey,DamagePosition)=nullptr;
    void (*fogStop)(void*)=nullptr;
    void (*stopAction)(void*,uint32_t capturedBattle)=nullptr;
    void (*helpType)(void*,int32_t)=nullptr;
};
// Original helper captures its own current battle, including for null flags.
uint32_t packedBattleCheckPikkyoro(uint32_t weapon,uint32_t* flags,const BattleDamageOrchestrationServices&);
// Caller still owns the distinct PreCheckDamage callback. Returns source18/19,
//21 or-1; the event wrapper must retain its negative-result calling-ID rules.
int32_t checkPackedBattleDamage(uint32_t attacker,uint32_t defender,uint32_t part,
    uint32_t weapon,uint32_t flags,const BattleDamageOrchestrationServices&);
}
