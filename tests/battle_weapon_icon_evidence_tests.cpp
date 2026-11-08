#include "battle_weapon_icon_evidence.hpp"
#include <cassert>
#include <cstring>
#include <functional>
#include <iostream>
using namespace ttyd;
namespace {
struct Process {
  bool alive = true;
  std::function<void()> callback;
};
std::shared_ptr<SourceGxInheritedState>
history(const std::shared_ptr<Process> &p) {
  auto h = std::make_shared<SourceGxInheritedState>();
  SourceGxInheritedBinding b;
  b.process = p;
  b.user = p.get();
  b.current = [](void *u) {
    auto &p = *static_cast<Process *>(u);
    if (p.callback)
      p.callback();
    return p.alive;
  };
  b.coldCurrent = [](void *) { return true; };
  assert(h->bind(b));
  assert(h->initializeCold());
  return h;
}
void exactIdentity() {
  auto process = std::make_shared<Process>();
  auto first = history(process), second = history(process);
  auto a = first->capture(), b = second->capture();
  assert(a && b);
  assert(a->stages() == b->stages() && a->size() == b->size());
  assert(std::memcmp(a->evidence().data(), b->evidence().data(),
                     sizeof(a->evidence())) == 0);
  auto evidence = BattleWeaponIconEvidence::create(
      first, 2, process, [process] { return process->alive; });
  assert(evidence);
  assert(first->ownsSnapshot(a) && !first->ownsSnapshot(b));
  assert(evidence->retain(first, a));
  assert(!evidence->retain(second, b));
  assert(!evidence->retain(first, b));
  assert(evidence->size() == 1);
  for (unsigned n = 0; n < 1000; ++n) {
    auto fresh = first->capture();
    assert(fresh.get() != a.get());
    assert(evidence->retain(first, fresh));
  }
  assert(evidence->size() == 1);
  auto three = first->capture(3);
  assert(three);
  assert(evidence->retain(first, three));
  assert(evidence->size() == 2);
  // New byte-distinct callsite is a distinct source evidence object, even if
  // its scalar result/profile remains admissible. Capacity does not discard it.
  SourceGxInheritedCall call;
  call.function = 0x802b9380;
  call.callsite = 0x80123458;
  call.count = 1;
  call.integers[0] = 1;
  assert(first->apply(call));
  auto changed = first->capture();
  assert(changed);
  assert(!evidence->retain(first, changed));
  assert(evidence->size() == 2 && evidence->current());
  assert(first->invalidate());
  assert(!first->capture());
  assert(evidence->current()); // immutable old packet history survives future
                               // unknown state
  second->retire();
  assert(!second->ownsSnapshot(b));
  assert(evidence->current());
  first->retire();
  assert(!evidence->current());
  auto replacement = history(process);
  assert(!evidence->retain(replacement, replacement->capture()));
}
void retirementAndReentry() {
  auto process = std::make_shared<Process>();
  auto h = history(process);
  auto e = BattleWeaponIconEvidence::create(
      h, 2, process, [process] { return process->alive; });
  auto s = h->capture();
  assert(e->retain(h, s));
  process->callback = [&] {
    assert(!e->current());
    assert(!e->retain(h, s));
  };
  assert(e->current());
  process->callback = {};
  assert(e->current());
  process->callback = [&] { e->retire(); };
  assert(!e->current());
  process->callback = {};
  assert(!e->current());
  auto fresh = BattleWeaponIconEvidence::create(
      h, 2, process, [process] { return process->alive; });
  assert(fresh);
  assert(fresh->retain(h, s));
  process->alive = false;
  assert(!fresh->current());
  process->alive = true;
  assert(!fresh->current());
}
} // namespace
int main() {
  exactIdentity();
  retirementAndReentry();
  std::cout << "actual owner icon evidence tests passed\n";
  return 0;
}
