#include "trio_battle_commands.hpp"
#include <cassert>
#include <cstring>
#include <iostream>
using namespace ttyd;
int main() {
    assert(loadProfessorStoryAssets("romfs") && loadUndergroundStoryAssets("romfs") && loadTrioBattleAssets("romfs"));
    ProfessorStory scene;assert(scene.beginTrioBattleIntro(0));
    TrioBattleCommands menu;assert(menu.begin(scene));
    assert(!std::strcmp(menu.commandName(0),"Normal Jump") && !std::strcmp(menu.commandName(1),"Normal Hammer"));
    assert(!std::strcmp(menu.targetName(3),"Goomba"));
    menu.input(0x100,0);assert(menu.state==TrioBattleCommands::State::Target && menu.count()==3);
    assert((menu.selected()==std::array<int32_t,2>{3,1}));
    assert((menu.target(1)==std::array<int32_t,2>{4,1})); // Spike is selectable.
    assert((menu.target(2)==std::array<int32_t,2>{5,1}));
    const auto& b=menu.candidates.bytes;
    assert(b[8]==0 && b[9]==30 && b[10]==0 && b[11]==30); // Sample offset30, not hit22.
    assert(b[44]==0 && b[45]==70 && b[46]==0 && b[47]==30);
    assert(b[80]==0 && b[81]==110 && b[82]==0 && b[83]==70);
    menu.input(0,0x40000);assert(menu.selected()[0]==5); // Reverse wraps.
    menu.input(0,0x80000);assert(menu.selected()[0]==3);
    menu.input(0,0xc0000);assert(menu.selected()[0]==3); // Both directions execute.
    menu.chooseTarget(1);menu.units[3].hp=0;menu.refreshTargets();
    assert(menu.count()==3 && menu.selected()[0]==4); // HP alone is not status27.
    setStatus(menu.units[3],27,1,0);menu.refreshTargets();
    assert(menu.count()==2 && menu.selected()[0]==4); // Retain pair after row compaction.
    menu.units[4].attributes|=0x40;menu.refreshTargets();assert(menu.count()==1 && menu.selected()[0]==5);
    menu.input(0x300,0);assert(menu.state==TrioBattleCommands::State::Command && !menu.attack); // Cancel wins.
    assert(menu.begin(scene));menu.chooseCommand(1);menu.input(0x100,0);
    assert(menu.count()==1 && menu.selected()[0]==3);
    menu.input(0,0x80000);assert(menu.selected()[0]==3);
    setStatus(menu.units[3],27,1,0);menu.refreshTargets();assert(menu.count()==1 && menu.selected()[0]==4);
    auto copied=menu;menu.units[4].attributes|=0x40;copied.refreshTargets();assert(copied.count()==1); // Owned views.
    copied.input(0x100,0);assert(copied.state==TrioBattleCommands::State::AttackReady && copied.attack);
    assert(copied.attack->actor==1 && copied.attack->unit==4 && copied.attack->part==1 && copied.attack->weapon==0x80353de8);
    copied.input(0x200,0);assert(!copied.attack && copied.state==TrioBattleCommands::State::Target);
    menu.refreshTargets();assert(menu.count()==0);menu.input(0x100,0);assert(menu.state==TrioBattleCommands::State::Target && !menu.attack);
    TrioBattleCommands partner;assert(partner.begin(scene,2));assert(!std::strcmp(partner.commandName(0),"Headbonk"));
    partner.input(0x100,0);assert(partner.count()==3);partner.chooseTarget(1);partner.input(0x100,0);
    assert(partner.attack && partner.attack->actor==2 && partner.attack->unit==4 && partner.attack->weapon==0x80393ea8);
    assert(!partner.begin(scene,6));
    std::cout<<"Source weapon IDs, sampler geometry/admission, pair refresh, cancel priority and owned dispatch passed\n";
}
