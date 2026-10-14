#include "battle_class_icon_material.hpp"
#include <cassert>
using namespace ttyd;
int main() {
  const auto stages = disabledClassIconProgram();
  assert(stages.size() == 3);
  assert(stages[0].channel == ClassIconChannel::Red &&
         stages[1].channel == ClassIconChannel::Green &&
         stages[2].channel == ClassIconChannel::Blue);
  for (const auto &stage : stages)
    assert(stage.coefficient == 85);
  assert(!stages[0].addPrevious && stages[1].addPrevious &&
         stages[2].addPrevious);
  assert(!stages[0].recordAlpha && !stages[1].recordAlpha &&
         stages[2].recordAlpha);
  assert(disabledClassIconColor(0xff0000ff, 0x010203ff) == 0x555555ff);
  assert(disabledClassIconColor(0x00ff00ff, 0xffffffff) == 0x555555ff);
  assert(disabledClassIconColor(0x0000ffff, 0x000000ff) == 0x555555ff);
  assert(disabledClassIconColor(0xffffff80, 0xff000080) == 0xffffff40);
  assert(disabledClassIconColor(0x123456ff, 0x12345600) == 0x34343400);
  assert(disabledClassIconColor(0x000000ff, 0x808080ff) == 0x000000ff);
}
