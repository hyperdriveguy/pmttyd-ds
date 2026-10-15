#include "water_recovery.hpp"
#include <algorithm>
#include <cmath>
namespace ttyd {
// Water entry fall-.06, depth thresholdroot-1.6, pause40, launch.2 then
// min.1 ascent: source-derived native constants, not measured frame timings.
void WaterRecovery::begin(std::array<float,3> start,std::array<float,3> destination) {
    *this=WaterRecovery{};position=start;target=destination;root=start[1];state=0;velocity=-.06f;
}
// Phases0/1 sink/wait, 10/11 launch, 12/14 travel-or-pause, 13 hold,
// 20 return fall, 30/31 damage wait, 32 get-up, 33 dead. Void enters15.
// Return derivative tuple(.34,-.002,-.01, 0) integrates before positional
// step max(-.1, velocity), distinct from derivative terminal cap-.18.
WaterEvent WaterRecovery::tick(int& hp) {
    if(state==15) {
        position[0]+=travelX;position[2]+=travelZ;
        if(--counter<=0){state=20;fallVelocity=.34f;fallAcceleration=-.002f;fallJerk=-.01f;fallJerkRate=0.f;}
    } else if(state==0) {
        if(position[1]>=root-1.6f)position[1]+=velocity;
        else {state=1;counter=40;velocity=.003f;}
    } else if(state==1) {
        if(--counter<=0){state=10;counter=0;velocity=.2f;}
    } else if(state==10 || state==11) {
        if(state==10 && ++counter==2)state=11;
        position[1]+=velocity;velocity=std::max(.1f,velocity-.01f);
        if(position[1]>=root+5) {
            const float distance=std::hypot(target[0]-position[0],target[2]-position[2]);
            counter=int(distance/.08f);
            if(distance>0){travelX=.08f*(target[0]-position[0])/distance;travelZ=.08f*(target[2]-position[2])/distance;}
            state=12;if(counter==0){state=14;counter=10;}
            followDelta=(target[1]-root)/counter;
        }
    } else if(state==12 || state==14) {
        if(state==12 && counter>0) {
            position[0]+=travelX;position[2]+=travelZ;
        }
        if(--counter<=0) {
            fallVelocity=.34f;fallAcceleration=-.002f;fallJerk=-.01f;fallJerkRate=0.f;
            state=state==12?13:20;counter=1;
        }
    } else if(state==13) {
        if(--counter<=0)state=20;
    } else if(state==20) {
        fallJerk=std::min(0.f,fallJerk+fallJerkRate);fallAcceleration+=fallJerk;fallVelocity+=fallAcceleration;
        if(fallVelocity<=-.18f){fallVelocity=-.18f;fallAcceleration=fallJerk=fallJerkRate=0;}
        float step=std::max(-.1f,fallVelocity);
        if(position[1]+step<=target[1]) {position[1]=target[1];state=31;counter=49;return WaterEvent::Landed;}
        position[1]+=step;
    } else if(state==30 || state==31) {
        if(state==30){state=31;counter=50;}
        if(--counter<=0) {
            hp=std::max(0,hp-1);
            if(hp==0){state=33;return WaterEvent::Dead;}
            state=32;counter=12;
            // Source transition32 begins this same call; counter 12 becomes11.
            // Landed31 already starts 49, whereas external30 sets50 then decrements.
            --counter; // Original falls through into state32 on this tick.
            return WaterEvent::GetUp;
        }
    } else if(state==32 && --counter<=0) {state=-1;return WaterEvent::Complete;}
    return WaterEvent::None;
}
// Travel count truncates distance/.08. Coincident roots still move one-.08
// Z step before the countdown finishes, following original heading0(-Z).
void WaterRecovery::beginVoid(std::array<float,3> start,std::array<float,3> destination) {
    *this=WaterRecovery{};kind=RecoveryKind::Void;position=start;target=destination;root=start[1];state=15;
    const float x=target[0]-position[0],z=target[2]-position[2],distance=std::hypot(x,z);
    counter=int(distance/.08f);
    if(distance>0){travelX=x*.08f/distance;travelZ=z*.08f/distance;}
    else travelZ=-.08f; // Source coincident angleABf returns heading0 (-Z); phase0 still moves.
}
}
