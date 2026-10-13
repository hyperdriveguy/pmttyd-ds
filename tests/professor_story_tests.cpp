#include "professor_story.hpp"
#include "game.hpp"
#include <cassert>
#include <iostream>
using namespace ttyd;
int main() {
    assert(loadProfessorStoryAssets("romfs"));ProfessorStory s;
    assert(s.begin(0,{-215,0,-95},{-245,0,-95}));
    for(uint64_t t=0;t<1000;t+=17)s.tick(t,0x100,true);
    assert(s.messageOrder.empty() && s.actors[2].position[0]==-115 && s.actors[2].position[2]==-225);
    float jumpPeak=0;
    for(uint64_t t=1000;t<400000 && s.locked();t+=17){s.tick(t,0x100,false);jumpPeak=std::max(jumpPeak,s.actors[2].position[1]);}
    assert(!s.failed && s.finished && s.story==9 && s.keysEnabled && s.temporaryProfessor);
    const char* suffixes[]={"066_kur","066_01","067","067_01","067_02","067_03","067_04","067_05","067_06","068_kur","069","069_01","069_02","069_03","070_kur","071","072_kur","073","073_01","074_kur","075","076_kur","077","078_kur","079","080","081","081_01_kur","082"};
    assert(s.messageOrder.size()==29);
    for(unsigned i=0;i<29;++i)assert(s.messageOrder[i]==std::string("mac_0_")+suffixes[i]);
    assert(s.actors[0].pose=="M_S_1");
    assert(s.actors[2].position[0]==-140 && s.actors[2].position[2]==-225);
    assert(s.presentationRequests.size()==119 && s.cameras.size()==17);
    assert(jumpPeak>29 && jumpPeak<=30.001f);
    assert(!s.itemVisible && std::fabs(s.itemScale-.5498046875f)<.00001f && s.music==0);
    clearCollision();assert(loadFieldDoors("romfs/doors.pmdoor"));
    assert(loadCollision("romfs/gor_02.walls") && loadGround("romfs/gor_02.ground") && loadHead("romfs/gor_02.head"));
    Progress p;p.room=2;p.originalStory=8;p.position={-2.15f,-.15f};
    Game g;g.restore(p);Input forward;forward.z=-1;
    for(int i=0;i<24;++i)g.tick(forward);
    Input a;a.confirm=true;g.tick(a);
    assert(g.fieldDoor.active() && g.professorStory.locked());
    for(int i=0;i<24000 && (g.fieldDoor.active() || g.professorStory.locked());++i)g.tick(a);
    assert(g.professorStory.finished && !g.professorStory.failed && g.progress.originalStory==9);
    assert(g.scene==Scene::Explore && g.professorStory.messageOrder.size()==29 && !g.saveRequested);
    clearCollision();
    std::cout<<"Original REL first-talk: 29 ordered messages, concurrent waits and story9 passed\n";
}
