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
    for(unsigned t=1;t<3000 && !e.battleRequested;++t){e.tick(t*17,t%4==0?0x100:0,15);assert(!e.failed);}
    assert(e.battleRequested && e.battleSetup==2 && e.hp==5 && e.story==4 && e.pendingService && e.locked());
    assert((e.messageOrder==std::vector<std::string>{"mac_0_022","mac_0_023","mac_0_024"}));
    for(unsigned t=3000;t<3100;++t)e.tick(t*17,0x100,15);
    assert(e.story==4 && !e.finished && e.messageOrder.size()==3);
    HarborEncounter resume;assert(resume.begin(0,{1000,1000,1000},10,true));
    assert((resume.actors[0].position==std::array<float,3>{-220,15,90}));assert(!resume.locked());
    resume.actors[0].position={0,15,-100};
    for(unsigned t=1;t<3000 && !resume.battleRequested;++t){resume.tick(t*17,t%4==0?0x100:0,15);assert(!resume.failed);}
    assert(resume.battleRequested && resume.hp==10 && resume.story==4);
    assert(loadHarborStoryAssets("romfs"));
    CollisionTransition geometry;assert(geometry.load("romfs/gor_00"));geometry.commit();
    Game game;Progress saved;saved.room=1;saved.elevation=.15f;saved.position={0,-1};saved.originalStory=3;
    game.restore(saved);
    for(unsigned t=0;t<6000 && !game.harborEncounter.battleRequested;++t){Input input;input.confirm=t%4==0;game.tick(input);assert(!game.harborEncounter.failed);}
    assert(game.harborEncounter.battleRequested && game.progress.originalStory==4 && game.scene==Scene::Dialogue && !game.saveRequested);
    assert(game.harborStory.finished && game.harborEncounter.messageOrder.size()==3);
    assert(game.prompt()==std::string("Development notice: original tutorial battle service pending"));
    std::puts("source132-command harbor event: integer admission, resume, three messages, jump, HP floor and real battle-service barrier passed");
}
