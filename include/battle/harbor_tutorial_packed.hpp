#pragma once
#include "harbor_tutorial.hpp"
#include "event_native_runtime.hpp"
#include "battle_event_callbacks.hpp"
#include "battle_damage_application.hpp"
#include "battle_camera_requests.hpp"
#include "battle_packed_generic_jump.hpp"
#include "display_queue.hpp"
#include "battle_weapon_menu_main.hpp"
#include "battle_present_item.hpp"
#include "battle_attack_selection.hpp"
#include "battle_action_manager_continuation.hpp"
#include "harbor_source_map_pool.hpp"
#include "harbor_source_command_ui.hpp"
#include <unordered_map>
#include <unordered_set>
#include <optional>
namespace ttyd {
class NativeBattleSound;
// Native storage/provider policy. All gameplay fields copied below retain source
// definitions. Packed entry allocation/initialization remains a scoped adapter.
class HarborTutorialPacked {
public:
 // Disjoint native guest RAM regions, not source allocation addresses. Unit/
 // part slots use stride 0x1000; provision maps 0x4000 bytes each for four IDs 0..3.
 // The registry itself retains source 64-entry layout; most entries stay null.
 static constexpr uint32_t Battle=0x84000000,Units=0x84100000,Parts=0x84200000;
 EventNativeRuntime& vm;EventCallbackRegistry registry;BattleRandom random{0};
 int32_t chosenUnit=-1,chosenPart=0;
 BattleMarioJumpBackend marioJumpBackend;BattlePackedMarioJumpServices marioJump;uint32_t marioJumpUnit=0;
 BattleTakeoffBackend takeoffBackend;BattlePackedTakeoffServices takeoff;
 BattlePackedMovePositionServices movement;BattlePackedGenericJumpServices jump;
 BattleCameraRequestServices camera;bool physicalCurrent=false;uint64_t sampledAt=UINT64_MAX;
 void samplePhysical(uint64_t milliseconds,const BattlePadSample*);
 bool actionManager(uint64_t milliseconds);
 bool actionManagerPending()const{return actionManagerCursor.active();}
 uint64_t actionManagerSampleTime()const{return actionInvocationAt;}
 bool actionDisplay(uint64_t milliseconds,HarborTutorial&);
 DisplayQueue actionDisplays;
 explicit HarborTutorialPacked(EventNativeRuntime& r):vm(r){}
 bool provision();
 // Native callback/backend configuration only; no guest mapping or stores.
 // Invoke once on a fresh callback registry, before any source event runs.
 bool registerEventServices();
 // Actual source pointer/registry; legacy fixed storage remains a native bootstrap policy.
 uint32_t battleAddress() const;
 uint32_t unitAddress(int32_t id) const;
 std::shared_ptr<const uint8_t> sourceAllocationLifetime(uint32_t address) const;
 bool provisionFreshPouch(const char* originalYoshiName,const ProloguePouch* entry=nullptr);
 void importMarioPouch();
 bool provisionFreshFieldInformation(uint32_t configuration,uint32_t expectedGroup,uint32_t expectedStage);
 bool registerActor(HarborTutorialActor&);
 bool unitInitReady();
 std::optional<int32_t> actMenu(HarborTutorial&,void*,uint32_t(*)(void*,uint32_t),bool setup,uint32_t flag=0);
 bool classUiRelease(uint32_t);
 std::shared_ptr<const uint8_t> classUiLifetime(uint32_t at) const{
  const auto it=classUiLifetimes.find(at);return it==classUiLifetimes.end()?nullptr:it->second;
 }
 std::shared_ptr<const uint8_t> nativeWeaponPresentationLifetime()const{return nativeWeaponPresentationOwner;}
 uint32_t nativeWeaponPresentationWork()const{constexpr uint32_t NativeWeaponWorkOffset=0xc00;return MenuRam+NativeWeaponWorkOffset;}
 bool menuAudienceItemOn(uint32_t output);
 std::optional<bool> attackAudienceMenu(HarborTutorial&);
 BattleMenuAudienceCursor audienceMenuCursor;
 std::shared_ptr<NativeBattleSound> sourceSound;
 BattlePresentItemCursor presentItemCursor;
 std::optional<int32_t> presentItem();
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
 BattleAttackSelectionCursor attackSelectionCursor;
 // Native admission latches: weapon decision and source command completion
 // are separate; neither implicitly schedules an attack or changes the story.
 bool weaponDecided=false,commandFinalized=false;
 bool commandInputBlocked() const{return commandUnbound!=0;}
 // Persistent actual source command scheduling; bind once to the real runtime.
 bool bindSourceCommandUi(HarborSourceCommandUiBinding);
 std::optional<int32_t> resumeSourceCommandInput(HarborTutorial&);
 bool sourceCommandInputPending()const;
 std::optional<uint32_t> sourceCommandMapChild(const HarborSourceCommandUiRequest&);
 std::optional<uint32_t> sourceCommandEvaluationChild(const BattleWeaponEvaluationRequest&);
 bool selectWeapon(unsigned row,HarborTutorial&,void* messageUser,uint32_t(*message)(void*,uint32_t));
 uint32_t cacheMenuLabel(uint32_t key,const char* originalText);
 std::optional<uint32_t> cachedMenuLabelAddress(uint32_t key) const {
  const auto at=menuText.find(key);return at==menuText.end()?std::nullopt:std::optional<uint32_t>(at->second);
 }
 uint32_t runHitEvent(int32_t id,uint32_t code);
 uint32_t runHitEventDirect(int32_t id,uint32_t code,uint32_t script);
 bool hitEventsActive();
 void recordInit(int32_t id,uint32_t event);void recordPhase(int32_t id,uint32_t event);void recordAttack(int32_t id,uint32_t event);
 // Selective two-way projection, not a whole packed-state snapshot. Callers
 // publish before admitted source work, capture afterward; pending child owners
 // must not have their captured guest work overwritten by a new projection.
 void publish(const HarborTutorial&);void capture(HarborTutorial&);
 bool invoke(EventEntry&,uint32_t function,int32_t first,int32_t& result);
 bool readWord(uint32_t address,uint32_t& value) const{return vm.memory.readWord(address,value);}
private:
 std::shared_ptr<HarborSourceCommandUi> sourceCommandUi;
 std::unique_ptr<HarborSourceMapPool> sourceMapPool_;
 friend struct HarborDynamicAddressesFixture;
 uint32_t attackSelectionBattle=0,attackSelectionUnit=0;std::shared_ptr<const uint8_t> attackSelectionBattleOwner,attackSelectionUnitOwner;
 BattlePadSample sampledPhysical{};
 uint64_t actionManagedAt=UINT64_MAX,actionDisplayedAt=UINT64_MAX;
 BattleActionManagerCursor actionManagerCursor;
 uint32_t actionBattle=0;std::shared_ptr<const uint8_t> actionBattleOwner;
 BattlePadSample actionPhysical{};uint64_t actionInvocationAt=UINT64_MAX;
 bool actionDisplayReady=false;
 std::array<bool,4> registered{};
 // Native 16KiB private arena. Low words are service scratch; 0xc00 legacy
 // weapon UI; 0x1000..0x3fff cached original text. Source tables remain in Battle.
 // Heap-backed class UI has independent SDK allocations and lifetime tokens.
 static constexpr uint32_t MenuRam=0x84600000;
 std::unordered_set<uint32_t> classUiAllocations;
 // Fresh native lifetime for each actual SDK class-UI allocation. Reusing its
 // guest address must not admit an old held draw or displayed publication.
 std::unordered_map<uint32_t,std::shared_ptr<const uint8_t>> classUiLifetimes;
 // Native legacy-weapon setup identity, minted only by its real setup bridge.
 std::shared_ptr<const uint8_t> nativeWeaponPresentationOwner;
 // First unavailable immediate Input path latches until weapon-table rebuild.
 // It prevents replaying partially completed source work without a continuation.
 // Zero means no latched block, not proof all providers have been installed.
 uint32_t commandUnbound=0;
 uint32_t menuTextAt=MenuRam+0x1000;std::unordered_map<uint32_t,uint32_t> menuText;

 bool commandPose(uint32_t unit,HarborTutorial&);
 bool aftereffectEvent(EventEntry&,int32_t&);
 bool inviteApReport();
 bool actionEvent(EventEntry&,uint32_t,int32_t&);
 uint32_t damageSoundUnit=0; // Set to the actual addressed defender by directDamage.
 bool checkDamageCore(uint32_t attacker,uint32_t defender,uint32_t part,uint32_t weapon,uint32_t flags,int32_t& result);
 void readDamage(unsigned id);void writeDamage(unsigned id);
 DamageApplicationServices damageApplication();
 // Four admitted registry IDs; five elemental entries per defense table,
 // four owned damage-sound strings per actor. Sound handles start 1 (0 absent).
 // These vectors are requests consumed by capture, not renderer/audio owners.
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
