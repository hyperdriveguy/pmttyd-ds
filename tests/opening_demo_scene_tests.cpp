#include "opening_demo_scene.hpp"
#include "actor_animation.hpp"
#include <cassert>
#include <cstring>
#include <cstdio>
using namespace ttyd;
int main(int argc,char**){
    if(argc>1)assert(loadDemoChestEffectAssets("romfs"));
    OpeningDemoScene scene;scene.begin(0);assert(scene.active && !scene.messageReady(0));
    assert(scene.mapGroupVisible(0) && !scene.mapGroupVisible(1) && !scene.mapGroupVisible(2) && !scene.mapGroupVisible(64));
    assert((scene.actors[0].position==std::array<float,3>{-2,0,550}));assert(scene.actors[0].yaw==90);
    scene.advance(179);assert(scene.revealProgress<1);scene.advance(180);assert(scene.revealProgress==1);
    scene.advance(239);assert(!scene.musicStarted);scene.advance(240);assert(scene.musicStarted);
    uint64_t tick=240;bool merchantWalkFallback=false;
    for(unsigned index=0;index<13;++index){
        while(!scene.messageReady(index)){scene.advance(++tick);assert(tick<10000);if(scene.actors[1].moving){merchantWalkFallback=true;assert(!std::strcmp(scene.actors[1].pose,"W_1") && !std::strcmp(scene.actorPose(1,false),"Z_1"));}}
        if(index==0)assert((scene.actors[0].position==std::array<float,3>{0,0,450}));
        if(index==4)assert(!std::strcmp(scene.actorPose(0,false),"P_B_2") && !std::strcmp(scene.actorPose(0,true),"P_T_7"));
        if(index==5)assert(scene.actors[0].position[0]==-55);
        if(index==7)assert(scene.actors[0].position[0]==-80 && !std::strcmp(scene.actorPose(0,true),"P_T_13"));
        if(index==8)assert(scene.actors[2].position[1]==10 && scene.actors[2].position[0]<-125);
        if(index==9)assert(!scene.mapGroupVisible(1));
        if(index==10)assert(scene.wallsVisible && scene.eye[0]==-450 && scene.mapGroupVisible(1) && scene.visibleMapGroups()==3);
        const auto position=scene.actors[2].position;
        for(unsigned i=0;i<4;++i)scene.advance(tick);assert(scene.actors[2].position==position);
        // Leave the last payload visible long enough for its independent
        // original transfer child to finish; no text content is simulated here.
        if(index==12){scene.advance(tick+300);tick+=300;assert(!std::strcmp(scene.actorPose(0,true),"P_T_14"));}
        scene.messageComplete(index);scene.advance(tick);
    }
    while(!scene.waitingForTitle){scene.advance(++tick);assert(tick<10000);}
    assert(scene.crystalRequested && scene.radiationRequested && merchantWalkFallback);
    assert(!std::strcmp(scene.actors[0].pose,"P_B_4"));assert(!std::strcmp(scene.actors[2].pose,"A_1"));
    assert(scene.actors[0].position[0]==-80 && scene.actors[2].yaw==225);
    const auto chest=scene.actors[2].position;scene.advance(tick+600);assert(scene.waitingForTitle && scene.actors[2].position==chest);
    OpeningDemoScene sparse,dense;sparse.begin(50);dense.begin(50);sparse.advance(900);
    for(uint64_t i=51;i<=900;++i)dense.advance(i);
    assert(sparse.actors[0].position==dense.actors[0].position && sparse.eye==dense.eye && sparse.messageReady(0));
    if(argc>1){
        for(const auto* bank:{"c_peach","c_nazo","demo-chest"}){
            ActorAnimationPack pack;assert(pack.load((std::string("romfs/")+bank+".pmanim").c_str()));
            if(!std::strcmp(bank,"c_peach"))for(const auto* name:{"P_Z_1","P_S_1","P_T_1","P_W_1","P_S_7","P_T_13","P_W_7","P_S_4","P_T_4","P_W_4","P_S_8","P_T_14","P_W_8","P_B_1","P_B_2","P_T_7","P_H_1","P_B_3","P_B_4"}){
                const auto* pose=pack.pose(name);assert(pose);ActorAnimationSample sample;assert(pose->evaluate(0,sample) && !sample.vertices.empty());
            }
            else {assert(pack.pose("S_1") && pack.pose("A_1"));if(!std::strcmp(bank,"c_nazo"))assert(pack.firstPose()->name=="Z_1" && !pack.pose("W_1"));
                else {ActorAnimationSample sample;assert(pack.pose("A_1")->evaluate(0,sample) && !sample.vertices.empty());bool opaque=false;for(const auto& batch:sample.batches)opaque|=batch.pass==1;assert(opaque);}
            }
        }
    }
    std::puts("Original demo marketplace request order, message gates, transfer and title guard passed");
}
