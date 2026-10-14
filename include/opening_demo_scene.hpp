#pragma once
#include "opening_house_scene.hpp"
#include "demo_chest_effects.hpp"
#include "npc_presentation.hpp"
namespace ttyd {
// Source-request presentation bridge for dmo's original marketplace event.
// Linear movement/camera and source paper capture/effect services are separate.
class OpeningDemoScene {
public:
    std::array<OpeningSceneActor,3> actors; // Peach, merchant, golden chest
    DemoChestEffects effects;
    std::array<float,3> eye{0,385,1473},at{0,100,0};
    uint64_t tick=0;
    bool active=false,waitingForTitle=false,wallsVisible=false,musicStarted=false;
    bool crystalRequested=false,radiationRequested=false;
    float revealProgress=0;
    unsigned nextMessage=0;
    bool loadFacing(const char* path){facingReady=angles.load(path);return facingReady;}
    void reset();
    std::array<NpcPresentation,2> facing;
    void begin(uint64_t simulationTick);
    void advance(uint64_t simulationTick);
    bool messageReady(unsigned index) const;
    void messageComplete(unsigned index);
    const char* actorPose(unsigned actor,bool talking) const;
    // Scene-local batch groups; never reinterpret these as field story groups.
    uint64_t visibleMapGroups() const{return wallsVisible?3:1;}
    bool mapGroupVisible(uint32_t group) const{return group<2 && (visibleMapGroups()&(uint64_t(1)<<group));}
private:
    enum class Kind {Wait,Move,Camera,Tribe,Pose,Reverse,Message,Lift,Handoff,Walls,Open};
    struct Request {Kind kind=Kind::Wait;unsigned actor=0;std::array<float,3> point{},aim{};uint64_t duration=0;const char* pose=nullptr;const char* talk=nullptr;};
    struct Motion {std::array<float,3> start{},target{};uint64_t begin=0,end=0;bool active=false;};
    std::vector<Request> requests;
    std::array<Motion,3> motion{};
    Motion cameraEye,cameraAt;
    std::array<const char*,2> idle{"P_S_7","S_1"},walk{"P_W_7","W_1"};
    size_t cursor=0;
    bool started=false,waitingMessage=false,handoff=false,opened=false;
    uint64_t deadline=0,beginTick=0,handoffTick=0,openTick=0;
    unsigned handoffStage=0,handoffSteps=0;
    bool updated=false;
    uint64_t updateTick=0;
    float chestOffset=0;
    NpcAngleTable angles;bool facingReady=false;
    void program();void step();void update();
    void move(unsigned,std::array<float,3>,float speed);
    void pose(unsigned,const char*,const char* talk=nullptr);
    void tribe(unsigned);
};
}
