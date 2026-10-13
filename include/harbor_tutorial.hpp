#pragma once
#include "message_playback.hpp"
#include <array>
#include <memory>
#include <vector>
namespace ttyd {
bool loadHarborTutorialAssets(const char* root);
struct HarborTutorialActor {
    int32_t id=-1,kind=0,hp=0,maxHp=0,maxMoves=1,heldItem=0;
    int8_t belong=0;uint32_t movePhase=0;
    uint32_t sourceKind=0,wait=0,attack=0,damage=0,confusion=0,phase=0;
    std::array<int32_t,16> work{};
    std::array<float,3> position{},home{},rotation{};
    std::string pose,stayPose,talkPose;
    uint64_t poseAt=0;
};
struct HarborTutorialRequest {
    uint32_t function=0,command=0;uint64_t time=0;int32_t owner=-1;
    std::vector<int32_t> rawOperands;
};
struct HarborTutorialRuntime;
// Source setup2 unit initialization and guarded phase requests. This owner does
// not infer combat outcomes or advance the field story. Native IDs are an
// explicit fresh registry policy: Crump0, Mario1, spawned NPC Goombella2.
class HarborTutorial {
public:
    bool triggered=false,failed=false,initialized=false,phaseActive=false,introComplete=false,attackActive=false;
    bool statusVisible=true;
    int32_t turn=1,speaker=-1,result=0;uint32_t phaseWord=0x04000001,battleFlags=0,pendingService=0;
    uint8_t successDeclarations=0,failureDeclarations=0;
    uint64_t now=0;
    std::array<HarborTutorialActor,3> actors{};
    MessagePlayback message;std::vector<std::string> messageOrder;
    std::vector<HarborTutorialRequest> requests;
    bool begin(uint64_t milliseconds,int32_t marioHp,int32_t marioMaxHp=10);
    void tick(uint64_t milliseconds,uint32_t keys);
    // Explicit scheduler/service bridge. Refuses an overlapping phase root.
    bool dispatchPhase(int32_t sourceTurn,uint32_t sourcePhase);
    // Animation driver publishes actual completion; a tick/count alone cannot.
    void animationComplete(int32_t actorId);
    // Called by an original battle scheduler, never automatically from HP or a
    // development button. Only already installed Crump/NPC attack hooks exist.
    bool dispatchInstalledAttack(int32_t actorId);
    bool running() const{return triggered;}
private:
    std::shared_ptr<HarborTutorialRuntime> runtime_;
};
}
