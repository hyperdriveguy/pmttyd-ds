#pragma once
#include "event_callback_registry.hpp"
#include "event_native_runtime.hpp"
#include "battle_data_child.hpp"
#include "battle_actor.hpp"
#include "battle_majinai_query.hpp"
#include "battle_event_utilities.hpp"
#include "battle_turn_first.hpp"
#include "battle_packed_move.hpp"
#include "effect_deletion.hpp"
#include "battle_turn_rule.hpp"
#include "battle_unit_event.hpp"
#include "battle_packed_status.hpp"
#include "battle_packed_selection.hpp"
#include "battle_packed_position.hpp"
#include "battle_first_attack.hpp"
#include "battle_packed_part.hpp"
#include "battle_packed_hit.hpp"
#include "battle_packed_takeoff.hpp"
#include "battle_packed_choice.hpp"
#include "battle_packed_sampling.hpp"
#include "battle_move_frame_event.hpp"
#include "battle_packed_movement_sound.hpp"

namespace ttyd {
struct BattleDataChildBackend {
    void* user=nullptr;
    int32_t (*translate)(void*,EventEntry&,int32_t)=nullptr;
    uint32_t (*currentBattle)(void*)=nullptr;
    uint32_t (*unit)(void*,uint32_t battle,int32_t id)=nullptr;
};
struct BattleTurnFirstBackend {
    EventNativeRuntime* runtime=nullptr;
    void* user=nullptr;
    void (*judgeTurnRule)(void*)=nullptr; // Null selects recovered packed rule judgment.
    void (*resetMove)(void*,uint32_t unit)=nullptr; // Null selects recovered packed-byte implementation.
    void (*softDelete)(void*,uint32_t effect)=nullptr; // Null selects recovered record/heap3 implementation.
};
// Backend and runtime must outlive returned services. Closed local turn body;
// Defaults implement recovered rule/move/effect services on shared packed bytes.
EffectDeletionServices nativeEffectDeletionServices(EventNativeRuntime&);
BattleTurnFirstServices nativeBattleTurnFirstServices(BattleTurnFirstBackend&);
struct BattleTakeoffBackend {
    EventNativeRuntime* runtime=nullptr;
    void* user=nullptr;
    int32_t (*sample)(void*,BattleCandidateBuffer&,uint32_t,int32_t,int32_t,uint32_t,uint32_t,int32_t)=nullptr; // Null selects native sampler.
    uint32_t (*monosiri)(void*,int32_t kind)=nullptr;
};
BattlePackedMovementSoundServices nativeBattleMovementSoundServices(EventNativeRuntime&,const BattleSoundPlaybackServices&);
BattleMoveFrameServices nativeBattleMoveFrameServices(EventNativeRuntime&);
bool bindBattleGetMoveFrameCallback(EventCallbackRegistry&,EventNativeRuntime&);
BattlePackedChoiceServices nativeBattleChoiceServices(EventNativeRuntime&);
BattlePackedSamplingServices nativeBattleSamplingServices(EventNativeRuntime&,void* user=nullptr,uint32_t (*monosiri)(void*,int32_t)=nullptr);
// Complete native sampler/chooser defaults;Monosiri mask1 requires its provider.
BattleTakeoffCoreServices nativeBattleTakeoffCoreServices(BattleTakeoffBackend&);
BattlePackedTakeoffServices nativeBattleTakeoffServices(EventNativeRuntime&,const BattleTakeoffCoreServices&);
// Services and runtime must outlive registration. Sampling/choice are required.
bool bindBattleTakeoffCallback(EventCallbackRegistry&,BattlePackedTakeoffServices&);
BattlePartPositionServices nativeBattlePartPositionServices(EventNativeRuntime&);
bool bindBattleGetPartsPosCallback(EventCallbackRegistry&,EventNativeRuntime&);
bool bindBattleGetHitPosCallback(EventCallbackRegistry&,EventNativeRuntime&);
BattleFirstAttackServices nativeBattleFirstAttackServices(EventNativeRuntime&);
bool bindBattleFirstAttackTargetCallback(EventCallbackRegistry&,EventNativeRuntime&);
BattlePackedSelectionServices nativeBattlePackedSelectionServices(EventNativeRuntime&);
bool bindBattleSetPosCallback(EventCallbackRegistry&,EventNativeRuntime&);
bool bindBattleGetBodyIdCallback(EventCallbackRegistry&,EventNativeRuntime&);
bool bindBattleSetMoveSpeedCallback(EventCallbackRegistry&,EventNativeRuntime&);
bool bindBattleSetFallAccelCallback(EventCallbackRegistry&,EventNativeRuntime&);
bool bindBattleGetPosCallback(EventCallbackRegistry&,EventNativeRuntime&);
bool bindBattleGetHomePosCallback(EventCallbackRegistry&,EventNativeRuntime&);
bool bindBattleSelectedTargetCallback(EventCallbackRegistry&,EventNativeRuntime&);
bool bindBattleCommandWeaponCallback(EventCallbackRegistry&,EventNativeRuntime&);
BattleUnitEventServices nativeBattleUnitEventServices(EventNativeRuntime&);
bool bindBattleStartWaitCallback(EventCallbackRegistry&,EventNativeRuntime&);
bool bindBattleStopWaitCallback(EventCallbackRegistry&,EventNativeRuntime&);
bool bindBattleWaitGuardMoveCallback(EventCallbackRegistry&,EventNativeRuntime&);
bool bindBattleCheckStatusCallback(EventCallbackRegistry&,EventNativeRuntime&);
bool bindBattleCheckDataCallback(EventCallbackRegistry&,EventNativeRuntime&);
bool bindBattleGetHpCallback(EventCallbackRegistry&,EventNativeRuntime&);
bool bindBattleGetMaxHpCallback(EventCallbackRegistry&,EventNativeRuntime&);
bool bindBattleGetUnitWorkCallback(EventCallbackRegistry&,EventNativeRuntime&);
bool bindBattleSetUnitWorkCallback(EventCallbackRegistry&,EventNativeRuntime&);
BattleActorServices nativeBattleActorServices(EventNativeRuntime&);
BattleDataChildServices nativeBattleDataChildServices(EventNativeRuntime&,const BattleDataChildBackend&);
// Complete recovered actor translation and registry queries on runtime bytes.
BattleDataChildServices nativeBattleDataChildServices(EventNativeRuntime&);
// Registers actual RunDataEventChild at original8010a010. Service object must
// outlive registration. Registry does not stub any other Jump callback.
bool bindBattleDataChildCallback(EventCallbackRegistry&,BattleDataChildServices&);
int32_t nativeBattleJumpSetting(EventEntry&,EventNativeRuntime&);
bool bindBattleJumpSettingCallback(EventCallbackRegistry&,EventNativeRuntime&);
BattleMajinaiQueryServices nativeBattleMajinaiQueryServices(EventNativeRuntime&);
bool bindBattleMajinaiQueryCallback(EventCallbackRegistry&,EventNativeRuntime&);
bool bindPouchMajinaiSetCallback(EventCallbackRegistry&,EventNativeRuntime&);
BattleEventUtilityServices nativeBattleEventUtilityServices(EventNativeRuntime&);
bool bindBattleFlagQueryCallback(EventCallbackRegistry&,EventNativeRuntime&);
bool bindBattleFramesToMillisecondsCallback(EventCallbackRegistry&,EventNativeRuntime&);
}
