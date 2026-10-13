#include "opening_house_scene.hpp"
#include "actor_animation.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <cstdio>
using namespace ttyd;
int main(int argc,char**) {
    OpeningHouseScene house;house.begin(0);
    assert(!house.messageReady(0) && !house.interior && !house.musicStarted);
    house.advance(89);assert(!house.musicStarted);house.advance(90);assert(house.musicStarted);
    house.advance(150);assert(std::fabs(house.eye[2]-800.5f)<.0001f);
    house.advance(240);assert(house.actors[2].visible && house.actors[2].moving);
    assert(house.eye[2]>717 && house.eye[2]<884);
    house.advance(300);assert(house.eye[2]==717 && !house.messageReady(0));
    uint64_t tick=300;
    while(!house.messageReady(0)){house.advance(++tick);assert(tick<2000);}
    assert((house.actors[2].position==std::array<float,3>{210,25,-30}));
    const auto delivery=tick;house.advance(tick+500);tick+=500;
    assert(house.messageReady(0) && !house.messageReady(1) && house.actors[2].visible);
    house.messageComplete(1);house.advance(tick);assert(house.messageReady(0));
    house.messageComplete(0);house.advance(tick);assert(house.actors[2].moving);
    bool carrying=false,walking=false,openingDoor=false,closingDuringWalk=false;
    float previousDoor=0;
    while(!house.messageReady(1)){
        house.advance(++tick);assert(tick<6000);
        carrying|=!std::strcmp(house.actors[1].pose,"L_11") && house.actors[1].moving;
        walking|=!std::strcmp(house.actors[1].pose,"L_6") && house.actors[1].moving;
        openingDoor|=house.doorAngle>0 && house.doorAngle<180;
        closingDuringWalk|=house.actors[1].moving && house.doorAngle<previousDoor && house.doorAngle>0;
        assert(std::trunc(house.doorAngle*1024.f)==house.doorAngle*1024.f);
        previousDoor=house.doorAngle;
        assert(!house.messageReady(2));
    }
    assert(openingDoor && closingDuringWalk);
    assert(carrying && walking && house.interior && !house.actors[2].visible);
    assert((house.actors[1].position==std::array<float,3>{25,30,-155}));
    assert(!std::strcmp(house.actorPose(1,true),"L_7"));
    house.messageComplete(1);house.advance(tick);
    const uint64_t cameraStart=tick;
    while(!house.messageReady(2)){house.advance(++tick);assert(tick<7000);}
    assert(tick>=cameraStart+120);
    assert((house.eye==std::array<float,3>{-376,154,65}));
    assert((house.actors[1].position==std::array<float,3>{-52,30,-275}));
    assert((house.actors[0].position==std::array<float,3>{-85,30,-310}));
    house.messageComplete(2);house.advance(tick);const auto readStart=tick;
    house.advance(readStart+17);assert(!house.messageReady(3));
    house.advance(readStart+18);tick=readStart+18;assert(house.messageReady(3));
    assert(!std::strcmp(house.actorPose(1,false),"L_8") && !std::strcmp(house.actorPose(1,true),"L_9"));
    house.messageComplete(3);house.advance(tick);house.advance(tick+17);assert(!house.complete);
    house.advance(tick+18);assert(house.complete);
    // Sparse callers must execute the same simulation updates and gates.
    OpeningHouseScene batched,individual;batched.begin(100);individual.begin(100);
    batched.advance(100+delivery);for(uint64_t i=101;i<=100+delivery;++i)individual.advance(i);
    assert(batched.messageReady(0) && individual.messageReady(0));
    assert(batched.actors[2].position==individual.actors[2].position && batched.eye==individual.eye);
    batched.advance(99);assert(batched.tick==100+delivery);
    if(argc>1){
        ActorAnimationPack luigi;assert(luigi.load("romfs/c_luigi.pmanim"));
        for(const auto* name:{"L_5","L_6","L_7","L_8","L_9","L_11"}){
            const auto* pose=luigi.pose(name);assert(pose);ActorAnimationSample sample;
            assert(pose->evaluate(0,sample) && !sample.vertices.empty());
        }
    }
    std::puts("Original house request order, authored endpoints, waits and simulation cadence passed");
}
