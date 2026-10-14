#pragma once
#include <array>
#include <cstdint>
#include <cstddef>
#include <vector>
#include "opening_npc_glide.hpp"
namespace ttyd {
struct OpeningSceneActor {
    std::array<float,3> position{};
    float yaw=0;
    const char* pose="S_1";
    const char* talkPose=nullptr;
    uint64_t poseTick=0;
    bool visible=false,moving=false;
};
// Bounded native presentation of aaa's original evt_prologue. Authored
// requests/order and explicit waits are retained; walking/heading, camera clock
// and complete NPC physics/display/effect services remain native adapters.
class OpeningHouseScene {
public:
    std::array<OpeningSceneActor,3> actors; // Mario, Luigi, Parakarry
    std::array<float,3> eye{},at{};
    bool interior=false,complete=false,musicStarted=false;
    unsigned nextMessage=0;
    float doorAngle=0,postboxX=0;
    // evt_prologue's independent 4000ms foreground parallax inline: ki2 +X,
    // ki1 -X. This is a finite source translation, not an ambient sway loop.
    float treeX=0;
    OpeningNpcGlide parakarryGlide;
    uint64_t tick=0;
    void begin(uint64_t simulationTick);
    void advance(uint64_t simulationTick);
    bool messageReady(unsigned index) const;
    void messageComplete(unsigned index);
    const char* actorPose(unsigned actor,bool talking) const;
private:
    enum class Kind {Wait,Set,Move,ParallelMove,Glide,Pose,Yaw,Visible,Camera,Interior,Door,Postbox,Message};
    struct Request {
        Kind kind=Kind::Wait;unsigned actor=0;
        std::array<float,3> point{},aim{};
        uint64_t duration=0;
        const char* pose=nullptr;const char* talk=nullptr;
        uint32_t sourceCommand=0;
    };
    struct Motion {std::array<float,3> start{},target{};uint64_t begin=0,end=0;bool active=false;};
    std::vector<Request> requests;
    std::array<Motion,3> motion{};
    Motion cameraEye,cameraAt,doorMotion;
    size_t cursor=0;
    bool started=false,waitingMessage=false;
    uint64_t deadline=0;
    uint64_t beginTick=0;
    uint64_t glideBegin=0,glideResume=0,glideUpdated=0;
    void appendProgram();
    void updateMotion();
    void startMove(unsigned,const std::array<float,3>&);
    void step();
};
}
