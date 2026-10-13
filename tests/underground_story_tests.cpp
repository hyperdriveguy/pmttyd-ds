#include "professor_story.hpp"
#include <cassert>
#include <iostream>
using namespace ttyd;
int main() {
    assert(loadProfessorStoryAssets("romfs") && loadUndergroundStoryAssets("romfs"));
    ProfessorStory scene;assert(scene.beginUnderground(0,11,{-400,0,-130},{-420,0,-130},true));
    assert(scene.underground && !scene.locked() && scene.temporaryProfessor);
    assert((scene.actors[3].position==std::array<float,3>({110,75,65})));
    assert((scene.actors[4].position==std::array<float,3>({140,95,35})));
    assert((scene.actors[5].position==std::array<float,3>({155,75,85})));
    for(uint64_t t=17;t<500;t+=17)scene.tick(t,0,false);
    assert(scene.messageOrder.empty() && scene.keysEnabled && scene.story==11);
    scene.actors[0].position={-100,0,70};
    for(uint64_t t=510;t<30000 && !scene.pendingService && !scene.failed;t+=17)scene.tick(t,0x100,false);
    assert(!scene.failed && scene.pendingService && scene.battleSetup==4 && !scene.keysEnabled);
    const std::vector<std::string> expected={"mac_0_086","mac_0_087","mac_0_088","mac_0_089_kur","mac_0_090","mac_0_091","mac_0_092"};
    assert(scene.messageOrder==expected && scene.story==11 && !scene.finished);
    assert(scene.actors[0].pose=="M_N_7" && scene.actors[3].pose=="KUR_R_1");
    assert((scene.actors[2].position==std::array<float,3>({-170,0,70})));
    assert((scene.actors[1].position==std::array<float,3>({-135,0,70})));
    assert(scene.undergroundGroupShown && scene.cameras.size()==4);
    std::cout<<"Original underground placement, X crossing, seven messages, concurrent actor moves and setup4 wait passed\n";
}
