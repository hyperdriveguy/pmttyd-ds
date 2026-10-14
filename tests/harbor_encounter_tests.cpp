#include "harbor_encounter.hpp"
#include "game.hpp"
#include <cassert>
#include <cmath>
#include <cstdio>
using namespace ttyd;
int main(int argc,char**){
    FieldScriptedJump jump;assert(jump.begin(15,15,12,-1000));float peak=15;int calls=0;
    while(jump.phase!=3 && calls<20){jump.tick(15);peak=std::max(peak,jump.y);++calls;}
    assert(calls==12 && std::fabs(peak-43)<.0001f && jump.y==15 && jump.velocity==0);
    assert(!jump.begin(15,16,12,-1000));
    if(argc<2){std::puts("source stationary jump binary32 trajectory and ground boundary passed");return 0;}
    assert(loadHarborEncounterAssets("romfs"));
    for(auto position:{std::array<float,3>{-25.5f,15,-99.5f},std::array<float,3>{-26,15,-100}}){
        HarborEncounter e;assert(e.begin(0,position,2,false));
        for(unsigned i=1;i<30;++i)e.tick(i*17,0,15);
        assert(!e.failed && !e.locked() && e.messageOrder.empty());
    }
    HarborEncounter e;assert(e.begin(0,{-25.5f,15,-100},2,false));
    bool sawRun=false,sawJump=false,sawLockedJumpPose=false;
    for(unsigned t=1;t<3000 && !e.battleRequested;++t){e.tick(t*17,t%4==0?0x100:0,15);assert(!e.failed);
        if(e.actors[1].moving)assert(e.actors[1].yaw==270);
        for(unsigned n=2;n<5;++n)assert(e.actors[n].yaw==270);
        sawRun|=e.actors[0].moving && e.actors[0].pose=="M_R_1";
        sawJump|=e.scriptedJump && e.actors[0].position[1]>15;
        sawLockedJumpPose|=e.scriptedJump && e.actors[0].poseOverride && (e.actors[0].flags&2) && e.actors[0].pose=="M_I_N";
    }
    assert(sawRun && sawJump && sawLockedJumpPose);
    assert(e.battleRequested && e.battleSetup==2 && e.hp==5 && e.story==4 && e.pendingService && e.locked());
    assert((e.messageOrder==std::vector<std::string>{"mac_0_022","mac_0_023","mac_0_024"}));
    for(unsigned t=3000;t<3100;++t)e.tick(t*17,0x100,15);
    assert(e.story==4 && !e.finished && e.messageOrder.size()==3);
    // Real field movement rarely lands on Q1024-exact coordinates. The event
    // reads its own root through set/getEventFloat before requesting the jump.
    for(auto position:{std::array<float,3>{0.12345f,15,-100.875f},
                       std::array<float,3>{-25.4f,15,-100.12345f},
                       std::array<float,3>{-25.4f,15,-300.12345f}}){
        HarborEncounter walked;assert(walked.begin(0,position,2,false));
        for(unsigned t=1;t<3000 && !walked.battleRequested;++t){walked.tick(t*17,t%4==0?0x100:0,15);assert(!walked.failed);}
        assert(walked.battleRequested && walked.battleSetup==2 && walked.hp==5);
        assert(walked.messageOrder==e.messageOrder && walked.story==4 && walked.locked());
    }
    HarborEncounter resume;assert(resume.begin(0,{1000,1000,1000},10,true));
    assert((resume.actors[0].position==std::array<float,3>{-220,15,90}));assert(!resume.locked());
    resume.actors[0].position={0,15,-100};
    for(unsigned t=1;t<3000 && !resume.battleRequested;++t){resume.tick(t*17,t%4==0?0x100:0,15);assert(!resume.failed);}
    assert(resume.battleRequested && resume.hp==10 && resume.story==4);
    assert(loadHarborStoryAssets("romfs"));
    CollisionTransition geometry;assert(geometry.load("romfs/gor_00"));geometry.commit();
    // Key-off during an ordinary jump keeps the vertical owner alive while
    // source mov_pos2 owns horizontal reposition. No aerial root becomes floor.
    Game air;Progress aerial;aerial.room=1;aerial.originalStory=4;aerial.elevation=.15f;
    aerial.position={-.30f,-3};air.restore(aerial);assert(air.beginHarborEncounter(false));
    Input launch;launch.confirm=true;launch.jumpHeld=true;air.tick(launch);
    assert(air.fieldAirMotion().airborne);air.progress.position.x=-.25f;
    const float initialY=air.groundHeight+air.jumpHeight;
    bool movedVertically=false;
    for(unsigned n=0;n<90 && air.fieldAirMotion().airborne;++n) {
        air.tick({});assert(!air.harborEncounter.failed);
        movedVertically|=std::fabs(air.groundHeight+air.jumpHeight-initialY)>.0001f;
    }
    assert(movedVertically && !air.fieldAirMotion().airborne && air.harborEncounter.locked());
    float landedFloor=0;assert(groundAt(air.progress.position,air.groundHeight+.1f,landedFloor));
    assert(std::fabs(air.groundHeight-landedFloor)<.001f && air.jumpHeight==0);
    Game game;Progress saved;saved.room=1;saved.elevation=.15f;saved.position={0,-1};saved.originalStory=3;
    game.restore(saved);
    for(unsigned t=0;t<6000 && !game.harborEncounter.battleRequested;++t){Input input;input.confirm=t%4==0;game.tick(input);assert(!game.harborEncounter.failed);}
    assert(game.harborEncounter.battleRequested && game.progress.originalStory==4 && game.scene==Scene::Dialogue && !game.saveRequested);
    assert(game.harborStory.finished && game.harborEncounter.messageOrder.size()==3);
    assert(game.prompt()==std::string("Development notice: original tutorial battle service pending"));
    assert(loadHarborTutorialAssets("romfs"));
    Game approach;saved.position={-1.5f,-1.1f};approach.restore(saved);
    for(unsigned t=0;t<6000 && approach.progress.originalStory==3;++t){Input input;input.confirm=t%4==0;approach.tick(input);}
    assert(approach.progress.originalStory==4 && !approach.harborEncounter.locked());
    for(unsigned t=0;t<120;++t)approach.tick(Input{});
    assert(!approach.harborEncounter.battleRequested);
    // The direct diagonal fixture walked into A_ni_03/pCube101 walls.
    // Retain those original colliders and walk the gap ahead of pCube94 instead.
    for(unsigned t=0;t<30;++t){Input input;input.physicalSnapshot=true;input.x=-1;approach.tick(input);}
    for(unsigned t=0;t<110;++t){Input input;input.physicalSnapshot=true;input.z=-1;approach.tick(input);}
    for(unsigned t=0;t<150 && !approach.harborEncounter.locked();++t){Input input;input.physicalSnapshot=true;input.x=1;approach.tick(input);}
    for(unsigned t=0;t<6000 && !approach.harborTutorial.message.active();++t){Input input;input.confirm=t%4==0;approach.tick(input);}
    assert(approach.harborTutorial.running() && approach.scene==Scene::Battle);
    Game tutorialGame;saved.position={0,-1};saved.hp=2;tutorialGame.restore(saved);
    for(unsigned t=0;t<6000 && !tutorialGame.harborTutorial.message.active();++t){Input input;input.confirm=t%4==0;tutorialGame.tick(input);assert(!tutorialGame.harborTutorial.failed);}
    assert(tutorialGame.harborTutorial.running() && tutorialGame.harborTutorial.message.id()=="mac_0_b_t1_000");
    assert(tutorialGame.progress.originalStory==4 && tutorialGame.scene==Scene::Battle && tutorialGame.progress.hp==5);
    for(unsigned t=0;t<600;++t){Input input;input.confirm=t%4==0;tutorialGame.tick(input);tutorialGame.harborTutorial.animationComplete(1);}
    assert(tutorialGame.harborTutorial.introComplete && tutorialGame.harborTutorial.result==0 && tutorialGame.harborEncounter.story==4);
    assert(!tutorialGame.saveRequested && tutorialGame.prompt()==std::string("Development notice: original tutorial action services pending"));
    std::puts("source132-command harbor event: integer admission, resume, three messages, jump, HP floor and real battle-service barrier passed");
}
