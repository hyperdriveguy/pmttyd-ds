#pragma once
#include "harbor_tutorial.hpp"
#include "event_native_runtime.hpp"
#include "battle_event_callbacks.hpp"
#include "battle_damage_application.hpp"
#include "battle_camera_requests.hpp"
#include "battle_packed_generic_jump.hpp"
#include "display_queue.hpp"
#include <unordered_map>
#include <unordered_set>
#include <optional>
namespace ttyd {
// Native storage/provider policy. All gameplay fields copied below retain source
// definitions. Packed entry allocation/initialization remains a scoped adapter.
class HarborTutorialPacked {
public:
 static constexpr uint32_t Battle=0x84000000,Units=0x84100000,Parts=0x84200000;
 EventNativeRuntime& vm;EventCallbackRegistry registry;BattleRandom random{0};
 int32_t chosenUnit=-1,chosenPart=0;
 BattleMarioJumpBackend marioJumpBackend;BattlePackedMarioJumpServices marioJump;uint32_t marioJumpUnit=0;
 BattleTakeoffBackend takeoffBackend;BattlePackedTakeoffServices takeoff;
 BattlePackedMovePositionServices movement;BattlePackedGenericJumpServices jump;
 BattleCameraRequestServices camera;bool physicalCurrent=false;uint64_t sampledAt=UINT64_MAX;
 void samplePhysical(uint64_t milliseconds,const BattlePadSample*);
 bool actionManager(uint64_t milliseconds);
 bool actionDisplay(uint64_t milliseconds,HarborTutorial&);
 DisplayQueue actionDisplays;
 explicit HarborTutorialPacked(EventNativeRuntime& r):vm(r){}
 bool provision();
 bool provisionFreshPouch(const char* originalYoshiName,const ProloguePouch* entry=nullptr);
 void importMarioPouch();
 bool provisionFreshFieldInformation(uint32_t configuration,uint32_t expectedGroup,uint32_t expectedStage);
 bool registerActor(HarborTutorialActor&);
 bool unitInitReady();
 std::optional<int32_t> actMenu(HarborTutorial&,void*,uint32_t(*)(void*,uint32_t),bool setup,uint32_t flag=0);
 bool classUiRelease(uint32_t);
 void captureClassRows(HarborTutorial&);
 bool buildClassMenu(HarborTutorial&,void*,uint32_t(*)(void*,uint32_t));
 bool buildWeaponMenu(uint32_t type,HarborTutorial&,void* messageUser,uint32_t(*message)(void*,uint32_t));
 bool updateWeaponEligibility(uint32_t type);
 std::optional<int32_t> countWeaponTargets(uint32_t battle,uint32_t unit,uint32_t weapon);
 void captureWeaponRows(uint32_t type,HarborTutorial&);
 bool openWeaponMenu(uint32_t type,HarborTutorial&,void*,uint32_t(*)(void*,uint32_t),uint32_t setupFlag=0);
 std::optional<int32_t> commandInput(HarborTutorial&,void*,uint32_t(*)(void*,uint32_t));
 std::optional<int32_t> inputWeaponMenu(HarborTutorial&,void*,uint32_t(*)(void*,uint32_t));
 void weaponMenuPresented();
 bool selectAttackEvent();
 bool weaponDecided=false,commandFinalized=false;
 bool commandInputBlocked() const{return commandUnbound!=0;}
 bool selectWeapon(unsigned row,HarborTutorial&,void* messageUser,uint32_t(*message)(void*,uint32_t));
 uint32_t cacheMenuLabel(uint32_t key,const char* originalText);
 uint32_t runHitEvent(int32_t id,uint32_t code);
 uint32_t runHitEventDirect(int32_t id,uint32_t code,uint32_t script);
 bool hitEventsActive();
 void recordInit(int32_t id,uint32_t event);void recordPhase(int32_t id,uint32_t event);void recordAttack(int32_t id,uint32_t event);
 void publish(const HarborTutorial&);void capture(HarborTutorial&);
 bool invoke(EventEntry&,uint32_t function,int32_t first,int32_t& result);
 bool readWord(uint32_t address,uint32_t& value) const{return vm.memory.readWord(address,value);}
private:
 BattlePadSample sampledPhysical{};
 uint64_t actionManagedAt=UINT64_MAX,actionDisplayedAt=UINT64_MAX;
 std::array<bool,4> registered{};
 static constexpr uint32_t MenuRam=0x84600000;
 std::unordered_set<uint32_t> classUiAllocations;
 uint32_t commandUnbound=0;
 uint32_t menuTextAt=MenuRam+0x1000;std::unordered_map<uint32_t,uint32_t> menuText;

 bool commandPose(uint32_t unit,HarborTutorial&);
 bool aftereffectEvent(EventEntry&,int32_t&);
 bool inviteApReport();
 bool actionEvent(EventEntry&,uint32_t,int32_t&);
 uint32_t damageSoundUnit=Units+0x1000;
 bool checkDamageCore(uint32_t attacker,uint32_t defender,uint32_t part,uint32_t weapon,uint32_t flags,int32_t& result);
 void readDamage(unsigned id);void writeDamage(unsigned id);
 DamageApplicationServices damageApplication();
 std::array<DamageUnit,4> damageUnits{};std::array<DamagePart,4> damageParts{};
 std::array<std::array<int8_t,5>,4> defenses{},defenseAttributes{};
 std::array<std::array<std::string,4>,4> damageSounds{};
 std::vector<int32_t> rumbleRequests;
 std::vector<DamageEffect> damageEffects;std::vector<DamageNotice> damageNotices;
 std::vector<std::pair<DamageAudienceRequest,int32_t>> damageAudienceRequests;
 bool freshPouch=false;uint32_t nextSoundHandle=1;
 std::vector<HarborTutorialSoundRequest> sounds;
 uint32_t requestSound(uint32_t unit,uint32_t key,const std::array<float,3>&);
 uint32_t word(uint32_t) const;void word(uint32_t,uint32_t);
 uint8_t byte(uint32_t) const;void byte(uint32_t,uint8_t);
 void half(uint32_t,int32_t);void real(uint32_t,float);float real(uint32_t) const;
 int32_t raw(EventEntry&,unsigned) const;int32_t value(EventEntry&,unsigned);
 void output(EventEntry&,unsigned,int32_t);uint32_t actor(EventEntry&);
};
}
