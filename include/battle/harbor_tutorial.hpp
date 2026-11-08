#pragma once
#include "native_battle_status.hpp"
#include "battle_class_display.hpp"
#include "harbor_source_menu_factory.hpp"
#include <functional>
#include "message_playback.hpp"
#include "battle_defense.hpp"
#include "battle_damage_orchestration.hpp"
#include "battle_timing_display.hpp"
#include "battle_audience_draw.hpp"
#include <array>
#include <memory>
#include <vector>
#include <optional>
namespace ttyd {
struct PositionalSfxRegistry;
bool loadHarborTutorialAssets(const char* root);
// Native projection of an admitted source actor; scripts/kind are guest tokens.
// work has the original 16 unit-work slots. poseAt is a millisecond generation
// used by completion acknowledgements, not a source animation frame counter.
struct HarborTutorialActor {
    int32_t id=-1,kind=0,hp=0,maxHp=0,maxMoves=1,heldItem=0;
    int8_t belong=0;uint32_t movePhase=0;
    uint32_t sourceKind=0,wait=0,attack=0,damage=0,confusion=0,phase=0,entry=0;
    std::array<int32_t,16> work{};
    std::array<float,3> position{},home{},rotation{};
    std::string pose,stayPose,talkPose;
    uint64_t poseAt=0;
};
// First-visit trace retains original function/command/owner and RAW operands.
// Recording a request proves invocation only, not downstream audio/draw completion.
struct HarborTutorialRequest {
    uint32_t function=0,command=0;uint64_t time=0;int32_t owner=-1;
    std::vector<int32_t> rawOperands;
};
struct HarborTutorialSoundRequest {enum class Operation {Trigger,Position,Off};Operation operation=Operation::Trigger;uint32_t handle=0,key=0;int32_t owner=-1;uint64_t time=0;std::string name;std::array<float,3> position{};};
struct HarborTutorialCameraState {uint32_t flags=0;int32_t priority=0,mode=0,owner=-1,target=-1,ownerPart=-1,targetPart=-1;int16_t speed=0,remaining=0;float zoom=0;std::array<float,3> home{},hit{};};
struct HarborTutorialWeaponRow {
 uint32_t weapon=0,nameAddress=0;bool enabled=false;int32_t reason=0,ordinal=0,item=0;uint16_t icon=0;
 std::string label;
};
struct HarborTutorialClassRow {
 uint32_t type=0,nameAddress=0,reason=0;uint16_t icon=0;bool enabled=false;
 std::string label;
};
struct ProloguePouch;
struct HarborTutorialRuntime;
struct HarborSourceMenuRegistration;
class DisplayQueue;
struct BattleCommandPalette;
struct BattleFooterCursor;
class BattleAudienceIncarnationOwner;
struct HarborClassMenuDrawBinding {
    BattleClassDisplayServices display;
    std::shared_ptr<const uint8_t> owner;
    uint32_t allocation=0;
    // Holds the actual runtime and verifies source W plus allocation generation.
    // Call before each dispatch/resume; retained keys alone do not prove life.
    std::function<bool()> current;
};
// Source setup2 unit initialization and guarded phase requests. This owner does
// not infer combat outcomes or advance the field story. Native IDs follow the
// original fresh registry order: system0, Mario1, Crump2, spawned helper3.
// Three visual slots remain Crump/Mario/helper and must not index registry IDs.
class HarborTutorial {
public:
    bool triggered=false,failed=false,initialized=false,phaseActive=false,introComplete=false,attackActive=false,damageActive=false;
    bool statusVisible=true;
    bool actionDisplayPending=false;
    std::vector<BattleTimingIconRequest> actionCommandIcons;
    int32_t turn=1,speaker=-1,result=0,chosenUnit=-1,chosenPart=0;uint32_t phaseWord=0x04000001,battleFlags=0,pendingService=0;
    uint8_t successDeclarations=0,failureDeclarations=0;
    // Native clock is monotonic milliseconds. phaseWord 0x04000001 is the source
    // opening-phase selector; turn 1 is its entry context, not an HP-driven turn loop.
    // speaker/chosenUnit are registry IDs (-1 absent); chosenPart 0 is unselected.
    uint64_t now=0;
    static constexpr int32_t SystemId=0,MarioId=1,CrumpId=2,HelperId=3;
    HarborTutorialActor systemActor; // Actual invisible source owner, excluded from visual slots.
    std::array<HarborTutorialActor,3> actors{};
    int32_t visualSlot(int32_t registryId) const;
    HarborTutorialActor* actorById(int32_t registryId);
    const HarborTutorialActor* actorById(int32_t registryId) const;
    HarborTutorialCameraState camera;
    MessagePlayback message;std::vector<std::string> messageOrder;
    std::vector<HarborTutorialRequest> requests;
    std::vector<HarborTutorialSoundRequest> soundRequests;
    std::vector<int32_t> rumbleRequests;
    std::vector<DamageEffect> damageEffects;
    std::vector<DamageNotice> damageNotices;
    std::vector<std::pair<DamageAudienceRequest,int32_t>> damageAudienceRequests;
    bool begin(uint64_t milliseconds,int32_t marioHp,int32_t marioMaxHp=10);
    // Explicit original new-game entry context; creates the verified fresh pouch
    // and executes Mario kind+bc initialization before admitting intro phase.
    bool beginFresh(uint64_t milliseconds,int32_t marioHp,int32_t marioMaxHp,int32_t originalStory);
    bool beginFromPouch(uint64_t milliseconds,const ProloguePouch&,int32_t originalStory);
    // Attach after beginFresh; clear before provider teardown. These bindings
    // await the complete source scheduler owner and do not advance intro alone.
    void setStartupResources(void* user,bool(*paperReady)(void*,uint32_t,int32_t),bool(*niceReady)(void*,int32_t));
    void setStartupFade(void* user,void(*entry)(void*,int32_t,int32_t,uint32_t),bool(*finished)(void*));
    void tick(uint64_t milliseconds,uint32_t keys,const BattlePadSample* physical=nullptr);
    // Explicit scheduler/service bridge. Refuses an overlapping phase root.
    bool dispatchPhase(int32_t sourceTurn,uint32_t sourcePhase);
    // Animation driver publishes actual completion; a tick/count alone cannot.
    void animationComplete(int32_t actorId);
    // Called by an original battle scheduler, never automatically from HP or a
    // development button. Only already installed Crump/NPC attack hooks exist.
    bool dispatchInstalledAttack(int32_t actorId);
    // Explicit scheduler bridge after original target confirmation. Consumes
    // the last successful source weapon decision; never runs from HP or tick.
    bool dispatchSelectedAttack();
    // Source damage producer bridge: original reaction roots, no HP/result synthesis.
    uint32_t dispatchHitEvent(int32_t actorId,uint32_t code);
    uint32_t dispatchDirectHitEvent(int32_t actorId,uint32_t code,uint32_t script);
    // Explicit original Init/End call boundaries for the scheduler owner. Each
    // successful Init acquires one reference; rendering and VM destruction do
    // not synthesize a matching End. Complete BattleInit/End remain unbound.
    bool initializeClassMenuResources();
    bool endClassMenuResources();
    // Explicit process-owned palette binding after begin/import, before source
    // command display registration. Reuses the same boot owner across battles;
    // never initializes/reset its color history at entry or per display pass.
    bool bindCommandDisplayPalette(std::shared_ptr<BattleCommandPalette>);
    // Same process public pending-slot owner used by original voice/menu On.
    // Attach after begin; no synthesized handle/sample or audible-voice receipt.
    bool bindPublicSound(std::shared_ptr<PositionalSfxRegistry>);
    std::optional<int32_t> startPublicMenuSound(uint32_t key);
    // Original CommandDisplay registration only. The caller retains queue and
    // cursor across waits, once per simulation display pass, and dispatches all
    // registered callbacks through the camera owner. No draw/ack/input occurs.
    bool registerCommandDisplay(DisplayQueue&,BattleFooterCursor&);
    // Bind once to this actual runtime; no receipt or camera mode is manufactured.
    bool setHarborSourceDisplayPass(std::shared_ptr<HarborSourceDisplayPass>);
    std::shared_ptr<HarborSourceMenuFactory> sourceMenuFactory() const;
    std::optional<TrioClassMenuFrameBinding> classMenuFrameBinding(const BattleActDrawServices&) const;
    std::optional<TrioWeaponMenuFrameBinding> weaponMenuFrameBinding(const BattleWeaponDrawServices&) const;
    std::shared_ptr<BattleWeaponSourceMemory> weaponMenuSourceMemory(std::shared_ptr<NativeUiStartup>,const char* image) const;
    // Borrowed actual-VM lifecycle metadata; unknown births remain unknown.
    // Keep this scene alive and still prove current scene/display/visibility
    // before using an identity. This getter does not initialize an audience.
    const BattleAudienceIncarnationOwner* audienceIncarnations() const;
    bool openClassMenu();
    // Actual VM memory/cursor-lookup binding for an admitted source display.
    // Linked camera/math/footer providers and source display cadence remain
    // caller-owned. Does not register/execute/acknowledge a display by itself.
    std::optional<HarborClassMenuDrawBinding> classMenuDrawBinding(
        const BattleActDrawServices& linked) const;
    void classMenuPresented();
    bool classMenuOpen=false;
    bool buildClassMenu();
    std::vector<HarborTutorialClassRow> classRows;
    int32_t classRowIndex=0;
    bool buildWeaponMenu(uint32_t type);
    bool openWeaponMenu(uint32_t type);
    void weaponMenuPresented();
    std::optional<int32_t> inputWeaponMenu(const BattlePadSample&);
    // Complete source Input owner with established menu/provider context.
    std::optional<int32_t> inputCommand(const BattlePadSample&);
    uint32_t commandInputPending=0;
    bool weaponMenuOpen=false,weaponTargetMenuOpen=false;int32_t weaponRowIndex=0;
    std::vector<uint32_t> weaponMenuSounds;
    bool selectWeapon(unsigned row);
    std::vector<std::array<int32_t,2>> weaponTargets;
    uint32_t weaponMenuType=0;std::vector<HarborTutorialWeaponRow> weaponRows;
    // Borrowed original action-command display requests for this simulation tick.
    const DisplayQueue* actionDisplayQueue() const;
    bool readPackedWord(uint32_t address,uint32_t& value) const;
    std::optional<NativeBattleStatusSnapshot> statusSnapshot() const;
    bool prepareAudienceDrawing(const BattleAudienceDrawServices&) const;
    bool running() const{return triggered;}
private:
    friend struct HarborSourceMenuFactoryFixture;
    uint32_t dispatchHit(int32_t,uint32_t,uint32_t,bool direct);
    bool beginWithContext(uint64_t,int32_t,int32_t,bool freshPouch,int32_t originalStory,const ProloguePouch* entry=nullptr);
    // Shared because held draw bindings can keep the VM/cache alive. Such a
    // binding must still validate current UI ownership before each dispatch.
    std::shared_ptr<HarborTutorialRuntime> runtime_;
    // Copies share one actual-runtime registration; a new begin gives only
    // that facade a fresh slot. The slot never pins the VM it identifies.
    mutable std::shared_ptr<HarborSourceMenuRegistration> sourceMenuRegistration_;
};
}
