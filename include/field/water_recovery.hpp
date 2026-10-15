#pragma once
#include <array>
namespace ttyd {
enum class WaterEvent { None, Landed, GetUp, Complete, Dead };
enum class RecoveryKind { Water, Void };
// Shared ordinary-Mario water/void return fall, landing and damage stage math.
// Drop-back lanes are the recovered mot_bottomless values (0.34/-0.002/-0.01/0.0,
// DOL 0x80091bd4); the remaining counters/parameters are source-derived, not
// Dolphin observations, and no original timing/cadence claim is made.
// State-1 inactive, 33 dead; other states are source motion labels, not frame
// counts. Positions/derivatives are native engine*.01; counter counts admitted
// simulation visits. Field event/audio/camera ownership remains separate.
struct WaterRecovery {
    RecoveryKind kind=RecoveryKind::Water;
    int state=-1,counter=0;
    std::array<float,3> position{},target{};
    float root=0,velocity=0,fallVelocity=0,fallAcceleration=0,fallJerk=0,fallJerkRate=0;
    float travelX=0,travelZ=0;
    float followDelta=0; // mot_damage2c0: (returnY - captured admission rootY) / travelTicks.
    bool active() const {return state>=0 && state!=33;}
    void begin(std::array<float,3> start,std::array<float,3> destination);
    void beginVoid(std::array<float,3> start,std::array<float,3> destination);
    WaterEvent tick(int& hp);
};
}
