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
    struct FieldOwner {FieldBattleWork work;int sequence=2;unsigned lookups=0,prepares=0,marioOn=0,partyOn=0,stops=0,posts=0;} owner;
    FieldBattleServices services;services.user=&owner;services.work=[](void* p)->FieldBattleWork&{return static_cast<FieldOwner*>(p)->work;};
    services.npc=[](void* p,int32_t name)->uintptr_t{++static_cast<FieldOwner*>(p)->lookups;assert(name==3);return 33;};
    services.prepareMario=[](void* p){++static_cast<FieldOwner*>(p)->prepares;};
    services.marioControl=[](void* p,bool on){if(on)++static_cast<FieldOwner*>(p)->marioOn;};
    services.partyId=[](void*){return 0;};services.party=[](void*,int32_t index)->uintptr_t{assert(index==0);return 11;};
    services.partyControl=[](void* p,uintptr_t party,bool on){assert(party==11);if(on)++static_cast<FieldOwner*>(p)->partyOn;};
    services.soundFlag=[](void*,uint32_t mask){assert(mask==0x10);};
    services.setSequence=[](void* p,int32_t value){static_cast<FieldOwner*>(p)->sequence=value;};services.sequence=[](void* p){return static_cast<FieldOwner*>(p)->sequence;};
    services.stopBattleBgm=[](void* p){++static_cast<FieldOwner*>(p)->stops;};services.postMario=[](void* p){++static_cast<FieldOwner*>(p)->posts;};
    ProfessorStory returned;assert(returned.beginUnderground(0,11,{-400,0,-130},{-420,0,-130},true));returned.fieldBattle=services;
    returned.actors[0].position={-100,0,70};uint64_t time=17;
    for(;time<30000 && !returned.pendingService && !returned.failed;time+=17)returned.tick(time,0x100,false);
    assert(!returned.failed && returned.pendingService==0x8004ef54 && owner.sequence==4 && owner.work.npc==33 && owner.work.result==0 && owner.prepares==1);
    owner.work.result=1;returned.tick(time,0,false);time+=17;
    assert(returned.pendingService==0x8004eed8 && owner.lookups==2 && owner.prepares==1 && owner.marioOn==0 && returned.story==11);
    owner.sequence=2;owner.work.result=0;returned.tick(time,0,false);time+=17;assert(returned.pendingService==0x8004eed8 && owner.marioOn==0);
    owner.work.result=1;
    for(;time<40000 && returned.messageOrder.size()==7 && !returned.failed;time+=17)returned.tick(time,0,false);
    assert(!returned.failed && returned.messageOrder.size()==8 && returned.messageOrder.back()=="mac_0_093");
    assert(owner.marioOn==1 && owner.partyOn==1 && owner.stops==1 && owner.posts==1 && returned.pendingService==0);
    assert(returned.speaker==3 && returned.story==11 && returned.actors[4].pose=="PTK_D_1");
    assert((returned.actors[4].position==std::array<float,3>{-55,20,35}));
    std::cout<<"Original underground placement, battle start/wait gates, original093 and deleted movement ownership passed\n";
}
