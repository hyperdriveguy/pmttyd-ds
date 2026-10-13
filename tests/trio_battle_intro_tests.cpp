#include "professor_story.hpp"
#include "game.hpp"
#include <cassert>
#include <iostream>
using namespace ttyd;
int main() {
    assert(loadProfessorStoryAssets("romfs") && loadUndergroundStoryAssets("romfs") && loadTrioBattleAssets("romfs"));
    const auto& units=trioBattleUnitSpecs();
    assert(units[0].kind==1 && units[1].kind==3 && units[2].kind==2 && units[3].kind==0xb4);
    for(unsigned i=0;i<3;++i){assert(units[i].hp==2 && units[i].fp==0);for(auto d:units[i].defense)assert(d==0);}
    assert(units[3].hp==10 && (units[3].partAttributes&0x10000));
    assert(units[1].counterAttributes==1 && (units[2].attributes&4) && (units[2].partAttributes&0x200000));
    ProfessorStory intro;assert(intro.beginTrioBattleIntro(0));
    assert((intro.actors[0].position==std::array<float,3>({-100,0,-5}) && intro.actors[1].position==std::array<float,3>({-140,0,5})));
    assert(intro.battleIntro && intro.lectureUnitId==6 && intro.lectureMaxMoves==0 && intro.actors[2].hidden);
    bool moved=false,statusHidden=false;
    for(uint64_t t=17;t<60000 && !intro.finished && !intro.failed;t+=17) {
        intro.tick(t,0x100,false);moved|=intro.actors[2].moving;statusHidden|=!intro.battleStatusVisible;
    }
    assert(!intro.failed && intro.finished && intro.battleIntroStage==2 && moved && statusHidden && intro.battleStatusVisible);
    assert(intro.messageOrder==std::vector<std::string>({"mac_0_092_01","mac_0_092_02","mac_0_092_03","mac_0_092_04","mac_0_092_05"}));
    assert((intro.actors[2].position==std::array<float,3>({-60,0,-30})));
    assert(intro.actors[2].yaw==90 && intro.actors[2].pose=="S_1");
    for(uint64_t t=intro.now+17,stop=intro.now+1000;t<stop;t+=17)intro.tick(t,0,false);
    assert(!intro.message.active());
    assert(intro.cameras.size()==5 && intro.lectureWork[0]==0);
    ProfessorStory later;assert(later.beginTrioBattleIntro(0));later.battleTurn=2;
    for(uint64_t t=17;t<10000 && !later.finished;t+=17)later.tick(t,0x100,false);
    assert(later.finished && later.messageOrder.empty());
    CollisionTransition collision;assert(collision.load("romfs/tik_01"));collision.commit();
    Progress checkpoint;checkpoint.room=5;checkpoint.originalStory=11;checkpoint.originalFlag1207=true;checkpoint.position={-1,.7f};
    Game game;game.restore(checkpoint);Input advance;advance.confirm=true;
    for(unsigned i=0;i<3600 && !game.trioBattleActive;++i)game.tick(advance);
    assert(game.trioBattleActive && game.fieldBattleState().npc==3 && !game.fieldControlEnabled());
    assert(game.fieldBattleState().result==0 && game.progress.originalStory==11 && game.professorStory.pendingService);
    const auto fieldPosition=game.progress.position;
    for(unsigned i=0;i<3600 && !game.trioBattleIntro.finished;++i)game.tick(advance);
    assert(game.trioBattleIntro.finished && game.progress.position.x==fieldPosition.x && game.progress.position.z==fieldPosition.z);
    assert(game.professorStory.messageOrder.size()==7 && game.trioBattleIntro.messageOrder.size()==5 && !game.saveRequested);
    std::cout<<"Original four-unit metadata, Frankly init/entry, phase gate, five exact messages and status/camera requests passed\n";
}
