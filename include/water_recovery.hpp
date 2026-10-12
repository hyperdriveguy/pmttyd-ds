#pragma once
#include <array>
namespace ttyd {
enum class WaterEvent { None, Landed, Complete, Dead };
enum class RecoveryKind { Water, Void };
// Shared ordinary-Mario water/void return fall, landing and damage stage math.
// Authored counters/parameters are source-derived, not Dolphin observations.
struct WaterRecovery {
    RecoveryKind kind=RecoveryKind::Water;
    int state=-1,counter=0;
    std::array<float,3> position{},target{};
    float root=0,velocity=0,fallVelocity=0,fallAcceleration=0,fallJerk=0,fallJerkRate=0;
    float travelX=0,travelZ=0;
    bool active() const {return state>=0 && state!=33;}
    void begin(std::array<float,3> start,std::array<float,3> destination);
    void beginVoid(std::array<float,3> start,std::array<float,3> destination);
    WaterEvent tick(int& hp);
};
}
