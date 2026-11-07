#include "harbor_tutorial.hpp"
#include "trio_attack.hpp"
#include <cassert>
#include <iostream>
#include <memory>
using namespace ttyd;
int main() {
  assert(loadProfessorStoryAssets("romfs") &&
         loadUndergroundStoryAssets("romfs") && loadTrioBattleAssets("romfs") &&
         loadTrioAttackAssets("romfs") && loadHarborTutorialAssets("romfs"));
  auto pouch = createProloguePouch();
  assert(pouch);
  pouch->half(0x70, 7);
  pouch->half(0x74, 2);
  pouch->half(20, 6);
  pouch->half(0x7a, 150);
  pouch->half(0x7c, 300);
  pouch->half(0x8c, 3);
  ProfessorStory scene;
  assert(scene.beginTrioBattleIntro(0));
  TrioBattleCommands menu;
  assert(menu.begin(scene));
  menu.input(0x100, 0);
  menu.input(0x100, 0);
  TrioAttack battle;
  assert(!battle.statusSnapshot() && battle.begin(menu, scene, 10, pouch));
  auto s = battle.statusSnapshot();
  assert(s && s->values[0] == 7 && s->values[1] == 10 && s->values[2] == 2 &&
         s->values[3] == 5 && s->values[4] == 1 && s->values[5] == 6 &&
         s->values[9] == 150 && s->values[10] == 300 &&
         s->partnerPortrait == 428 && s->gaugeCount == 3 &&
         s->gauge[0].icon == 463 && s->gauge[1].icon == 447 &&
         s->gauge[2].icon == 455);
  HarborTutorial harbor;
  assert(!harbor.statusSnapshot() && harbor.beginFromPouch(0, *pouch, 4));
  s = harbor.statusSnapshot();
  assert(s && s->values[0] == 7 && s->values[2] == 2 && s->values[4] == 0 &&
         !s->partnerPortrait && s->values[9] == 150 && s->gaugeCount == 3);
  uint32_t packed = 0;
  assert(harbor.readPackedWord(0x8410110c, packed) &&
         s->values[0] == int16_t(packed >> 16));
  std::cout
      << "actual initialized Trio/Harbor status snapshot API tests PASS\n";
}
