#pragma once
#include "message_playback.hpp"
#include "battle_defense.hpp"
#include "battle_damage_orchestration.hpp"
#include <array>
#include <memory>
#include <vector>
namespace ttyd {
bool loadHarborTutorialAssets(const char* root);
struct HarborTutorialActor {
    int32_t id=-1,kind=0,hp=0,maxHp=0,maxMoves=1,heldItem=0;
    int8_t belong=0;uint32_t movePhase=0;
    uint32_t sourceKind=0,wait=0,attack=0,damage=0,confusion=0,phase=0,entry=0;
    std::array<int32_t,16> work{};
    std::array<float,3> position{},home{},rotation{};
    std::string pose,stayPose,talkPose;
    uint64_t poseAt=0;
};
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
struct ProloguePouch;
struct HarborTutorialRuntime;
// Source setup2 unit initialization and guarded phase requests. This owner does
// not infer combat outcomes or advance the field story. Native IDs follow the
// original fresh registry order: system0, Mario1, Crump2, spawned helper3.
// Three visual slots remain Crump/Mario/helper and must not index registry IDs.
class HarborTutorial {
public:
    bool triggered=false,failed=false,initialized=false,phaseActive=false,introComplete=false,attackActive=false,damageActive=false;
    bool statusVisible=true;
    int32_t turn=1,speaker=-1,result=0,chosenUnit=-1,chosenPart=0;uint32_t phaseWord=0x04000001,battleFlags=0,pendingService=0;
    uint8_t successDeclarations=0,failureDeclarations=0;
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
    // Source damage producer bridge: original reaction roots, no HP/result synthesis.
    uint32_t dispatchHitEvent(int32_t actorId,uint32_t code);
    uint32_t dispatchDirectHitEvent(int32_t actorId,uint32_t code,uint32_t script);
    bool buildWeaponMenu(uint32_t type);
    bool selectWeapon(unsigned row);
    std::vector<std::array<int32_t,2>> weaponTargets;
    uint32_t weaponMenuType=0;std::vector<HarborTutorialWeaponRow> weaponRows;
    bool readPackedWord(uint32_t address,uint32_t& value) const;
    bool running() const{return triggered;}
private:
    uint32_t dispatchHit(int32_t,uint32_t,uint32_t,bool direct);
    bool beginWithContext(uint64_t,int32_t,int32_t,bool freshPouch,int32_t originalStory,const ProloguePouch* entry=nullptr);
    std::shared_ptr<HarborTutorialRuntime> runtime_;
};
}
