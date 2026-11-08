// Explicit TEST initialized prefix; this exercises actual facade/runtime birth
// and registration, without asserting battle simulation or teardown completed.
#ifndef HARBOR_FIELD_ORIGIN_IMPLEMENTATION
#define HARBOR_FIELD_ORIGIN_IMPLEMENTATION "../source/battle/harbor_tutorial.cpp"
#endif
#include HARBOR_FIELD_ORIGIN_IMPLEMENTATION
#include <cassert>
#include <cstdio>
namespace ttyd {
struct NativeHarborCompletionFixture {
 static void initialized(HarborTutorial&t){t.initialized=true;}
 static void projectedAt(HarborTutorial&t){t.runtime_->scene=&t;}
 static void test(){
  assert(loadHarborTutorialAssets("romfs")&&loadProloguePouchAssets("romfs"));
  HarborTutorial original;const auto pouch=initializeProloguePouch("Yoshi");assert(original.beginFromPouch(0,pouch,4));initialized(original);
  auto identity=original.nativeFieldBattleIdentity();assert(identity&&identity->current());
  HarborTutorial copy=original;assert(!copy.nativeFieldBattleIdentity()&&identity->current());
  projectedAt(copy);assert(!copy.nativeFieldBattleIdentity()&&!identity->current());
  projectedAt(original);assert(original.nativeFieldBattleIdentity()&&identity->current());
  HarborTutorial assigned;assigned=original;assert(!assigned.nativeFieldBattleIdentity());
  // Moving does not invent an admission transfer. Original birth proof dies;
  // both existing identity and destination must hold until explicit new entry.
  HarborTutorial moved(std::move(original));assert(!identity->current()&&!moved.nativeFieldBattleIdentity());
  projectedAt(moved);assert(!moved.nativeFieldBattleIdentity());
  moved=HarborTutorial{};assert(moved.beginFromPouch(0,pouch,4));initialized(moved);assert(moved.nativeFieldBattleIdentity());
  const auto replacement=moved.nativeFieldBattleIdentity();assert(replacement&&replacement->current());
  moved=HarborTutorial{};assert(!replacement->current());
  // A shared runtime/source slot remaining in a copy cannot revive the dead
  // original facade proof, even at the same mutable projection pointer.
  projectedAt(copy);assert(!copy.nativeFieldBattleIdentity()&&!identity->current());
 }
};
}
int main(){ttyd::NativeHarborCompletionFixture::test();std::puts("Actual admission-facade proof rejects copies/moves/replacement independently of mutable scene projection");return 0;}
