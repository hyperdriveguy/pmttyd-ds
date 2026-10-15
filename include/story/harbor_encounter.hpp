#pragma once
#include "message_playback.hpp"
#include "field_scripted_jump.hpp"
#include "field_npc_battle_event.hpp"
#include <array>
#include <memory>
#include <vector>
namespace ttyd {
bool loadHarborEncounterAssets(const char* root);
// Native projection of source actor state; yaw is degrees, speed engine units/s,
// poseAt is ms. Pose override and display flags are separate source requests.
struct HarborEncounterActor {
    std::array<float,3> position{};float yaw=90;uint32_t flags=0;
    bool moving=false,poseOverride=false;float speed=0;std::string pose="M_S_1";uint64_t poseAt=0;
};
// Raw source camera units/duration/curve; curve11 is retained source default.
// Requests are observations of callbacks, not evidence the renderer finished.
struct HarborEncounterCamera {std::array<float,3> eye{},at{};int duration=0,curve=11;};
struct HarborEncounterRequest {uint32_t function=0,command=0;uint64_t time=0;std::vector<int32_t> operands;};
struct HarborEncounterRuntime;
// Original before-battle REL event, with five slots Mario/G/Crump/A/B.
// story4 seeds GSW0; battleSetup -1 means unset. HP is caller-provided, then
// actual source callbacks may change it. A pending battle service keeps running
// true: this owner never fabricates a battle result or postbattle story write.
class HarborEncounter {
public:
    bool triggered=false,finished=false,failed=false,admitted=false;
    bool controlEnabled=true,battleRequested=false,scriptedJump=false;
    int keyDepth=0,battleSetup=-1,hp=10;int32_t story=4;
    uint32_t speaker=0,pendingService=0;uint64_t now=0;
    std::array<HarborEncounterActor,5> actors{}; // Mario,Goombella,Crump,A,B.
    std::array<float,3> speakerRoot{};
    MessagePlayback message;
    std::vector<std::string> messageOrder;
    std::vector<HarborEncounterCamera> cameras;
    std::vector<HarborEncounterRequest> requests;
    // Optional actual field services; supplied owners must survive held calls.
    // The development Game battle fixture does not satisfy this binding.
    BattleSequenceEntryServices battleServices;
    // time/tick use milliseconds; resume initializes source LW0=1 (fresh0).
    // Repeated begin is rejected until the caller explicitly resets the owner.
    // floor is native jump support, distinct from VM request coordinates.
    bool begin(uint64_t time,std::array<float,3> mario,int hp,bool resume);
    void tick(uint64_t time,uint32_t keys,std::optional<float> floor={});
    bool battlePending() const;
    bool running() const{return triggered && !finished;}
    bool locked() const{return running() && (keyDepth!=0 || !controlEnabled);}
private:
    std::shared_ptr<HarborEncounterRuntime> runtime_;
};
}
