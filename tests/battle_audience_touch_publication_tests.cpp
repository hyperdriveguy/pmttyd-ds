#include "battle_audience_touch_publication.hpp"
#include <cassert>
#include <limits>
using namespace ttyd;
namespace {
const BattleAudienceTouchTarget Target{5, 6, 7, 8};
RetainedIconCamera camera() {
  RetainedIconCamera c;
  c.projection = {{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}};
  c.view = {{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}}};
  c.viewport = {0, 0, 240, 400};
  c.sourceAspect = 400.f / 240;
  return c;
}
BattleAudienceTouchSourceFrame
source(const BattleAudienceTouchPublicationKey &key) {
  BattleAudienceTouchSourceFrame s;
  s.pass = {key.owner, key.sequence, true};
  s.camera = camera();
  s.quads.push_back({Target,
                     {{{-.2f, -2.2f, 0},
                       {.2f, -2.2f, 0},
                       {.2f, -1.8f, 0},
                       {-.2f, -1.8f, 0}}}});
  s.eligibility.push_back({Target, true, true, true});
  return s;
}
void publicationAndConsumption() {
  BattleAudienceTouchPublication p;
  auto owner = std::make_shared<const uint8_t>(0);
  BattleAudienceTouchPublicationKey key{owner, 0x10000, 1};
  auto s = source(key);
  assert(!p.physicalSelectionAvailable());
  assert(!p.publish(key, s));
  assert(!p.admit(key));
  p.bind(owner, 0, true);
  assert(!p.physicalSelectionAvailable());
  p.bind(owner, key.allocation, false);
  assert(!p.publish(key, s));
  p.bind(owner, key.allocation, true);
  assert(p.physicalSelectionAvailable());
  assert(!p.frame());
  assert(!p.queueTap(key, 1, 160, 120));
  assert(p.publish(key, s));
  auto borrowed = p.frame();
  assert(borrowed && borrowed->key.sequence == 1);
  // Publication owns copied geometry/camera and source eligibility.
  s.quads[0].world[0][0] = 999;
  s.camera.view[0][3] = 999;
  s.eligibility[0].dangerous = false;
  assert(!p.queueTap(key, 1, 160, 120)); // Not actually drawn yet.
  auto future = key;
  future.sequence = 2;
  assert(!p.admit(future));
  assert(!p.queueTap(future, 1, 160, 120));
  assert(p.admit(key));
  assert(p.queueTap(key, 1, 160, 120));
  assert(!p.queueTap(key, 1, 160, 120));
  assert(!p.queueTap(key, 2, 160, 120)); // One pending edge.
  auto live = source(key).eligibility;
  assert(!p.consumeTap(future, 1, live));
  assert(!p.consumeTap(key, 2, live));
  auto request = p.consumeTap(key, 1, live);
  assert(request && request->target == Target);
  assert(request->frame.owner == owner &&
         request->frame.allocation == key.allocation &&
         request->frame.sequence == 1);
  assert(request->inputSerial == 1 && request->x == 160 && request->y == 120);
  assert(!p.consumeTap(key, 1, live));
  assert(!p.queueTap(key, 2, 160, 120));
  assert(p.queueTap(key, 3, 160, 120));
  live[0].dangerous = false;
  assert(!p.consumeTap(key, 3, live));
  live[0].dangerous = true;
  assert(!p.consumeTap(key, 3, live));
  assert(!p.queueTap(key, 3, 160, 120));
  assert(p.queueTap(key, 4, 160, 120));
  live[0].target.itemGeneration++;
  assert(!p.consumeTap(key, 4, live));
  live = source(key).eligibility;
  assert(p.queueTap(key, 5, 160, 120));
  live[0].interruptible = false;
  assert(!p.consumeTap(key, 5, live));
  live = source(key).eligibility;
  assert(p.queueTap(key, 6, 160, 120));
  live[0].visible = false;
  assert(!p.consumeTap(key, 6, live));
  assert(!p.queueTap(key, 7, -1, 120));
  assert(!p.queueTap(key, 7, 160, 120));
  assert(!p.queueTap(key, 8, 320, 120));
  assert(!p.queueTap(key, 9, 160, 240));
  assert(!p.queueTap(key, 10, std::numeric_limits<float>::quiet_NaN(), 120));
  assert(!p.queueTap(key, 0, 160, 120));
  // A newer source publication clears old draw admission and queued touch.
  assert(p.queueTap(key, 11, 160, 120));
  assert(p.publish(future, source(future)));
  assert(!p.admit(key));
  assert(!p.consumeTap(key, 11, source(key).eligibility));
  assert(!p.queueTap(future, 12, 160, 120));
  assert(p.admit(future));
  assert(!p.queueTap(future, 11, 160, 120));
  assert(p.queueTap(future, 12, 160, 120));
  assert(p.consumeTap(future, 12, source(future).eligibility));
  assert(borrowed->key.sequence == 1 &&
         borrowed->source.quads[0].world[0][0] == -.2f);
  // Losing the independent physical provider forbids a touch-only interface.
  p.bind(owner, key.allocation, false);
  assert(!p.physicalSelectionAvailable());
  assert(!p.frame());
  assert(!p.queueTap(future, 13, 160, 120));
  p.bind(owner, key.allocation, true);
  assert(p.physicalSelectionAvailable());
  assert(!p.publish(future, source(future)));
  auto next = future;
  next.sequence = 3;
  assert(p.publish(next, source(next)));
  auto replacement = std::make_shared<const uint8_t>(0);
  p.bind(replacement, key.allocation, true);
  assert(!p.frame());
  assert(!p.admit(next));
  assert(!p.publish(next, source(next)));
  auto newKey = key;
  newKey.owner = replacement;
  assert(p.publish(newKey, source(newKey)));
  assert(p.admit(newKey));
  assert(p.queueTap(newKey, 1, 160, 120));
  auto wrongAllocation = newKey;
  ++wrongAllocation.allocation;
  assert(!p.consumeTap(wrongAllocation, 1, source(newKey).eligibility));
  p.retire();
  assert(!p.frame() && !p.physicalSelectionAvailable());
  assert(!p.consumeTap(newKey, 1, source(newKey).eligibility));
  assert(borrowed->geometry.hit(borrowed->source.pass, 160, 120,
                                borrowed->source.eligibility) == Target);
}
void sourceValidation() {
  auto owner = std::make_shared<const uint8_t>(0);
  BattleAudienceTouchPublicationKey key{owner, 0x20000, 1};
  BattleAudienceTouchPublication p;
  p.bind(owner, key.allocation, true);
  auto s = source(key);
  auto bad = s;
  bad.pass.complete = false;
  assert(!p.publish(key, bad));
  bad = s;
  bad.pass.sequence = 2;
  assert(!p.publish(key, bad));
  bad = s;
  bad.pass.owner = std::make_shared<const uint8_t>(0);
  assert(!p.publish(key, bad));
  bad = s;
  bad.camera.viewport.width = 0;
  assert(!p.publish(key, bad));
  bad = s;
  bad.camera.projection[0][0] = std::numeric_limits<float>::infinity();
  assert(!p.publish(key, bad));
  bad = s;
  bad.quads[0].target.actorGeneration = 0;
  assert(!p.publish(key, bad));
  bad = s;
  bad.eligibility[0].target.itemGeneration++;
  assert(!p.publish(key, bad));
  bad = s;
  bad.eligibility.clear();
  assert(!p.publish(key, bad));
  bad = s;
  bad.eligibility.push_back(bad.eligibility[0]);
  assert(!p.publish(key, bad));
  bad = s;
  bad.quads.push_back(bad.quads[0]);
  bad.eligibility.push_back(bad.eligibility[0]);
  assert(!p.publish(key, bad));
  bad = s;
  bad.quads[0].world[0][0] = std::numeric_limits<float>::quiet_NaN();
  assert(!p.publish(key, bad));
  auto wrong = key;
  wrong.sequence = 0;
  assert(!p.publish(wrong, source(wrong)));
  wrong = key;
  ++wrong.allocation;
  assert(!p.publish(wrong, source(wrong)));
  assert(p.publish(key, s));
  assert(p.admit(key));
  assert(!p.publish(key, s));
  assert(p.queueTap(key, 1, 160, 120));
  assert(p.consumeTap(key, 1, s.eligibility));
  // Malformed future data cannot replace a known displayed frame.
  wrong = key;
  wrong.sequence = 2;
  bad = source(wrong);
  bad.pass.complete = false;
  assert(!p.publish(wrong, bad));
  assert(p.frame()->key.sequence == 1);
  assert(p.queueTap(key, 2, 160, 120));
  assert(p.consumeTap(key, 2, s.eligibility));
}
void geometryAndLiveAmbiguity() {
  auto owner = std::make_shared<const uint8_t>(0);
  BattleAudienceTouchPublicationKey key{owner, 0x30000, 1};
  BattleAudienceTouchPublication p;
  p.bind(owner, key.allocation, true);
  auto s = source(key);
  s.camera.view[0][3] = .5f;
  assert(p.publish(key, s));
  assert(p.admit(key));
  assert(!p.queueTap(key, 1, 160, 120));
  assert(p.queueTap(key, 2, 260, 120));
  assert(p.consumeTap(key, 2, s.eligibility));
  key.sequence = 2;
  s = source(key);
  const BattleAudienceTouchTarget second{9, 10, 11, 12};
  s.quads.push_back({second, s.quads[0].world});
  s.eligibility.push_back({second, true, false, true});
  assert(p.publish(key, s));
  assert(p.admit(key));
  assert(!p.queueTap(key, 3, 160, 120));
  // Only an exact source incarnation explicitly hidden permits pass-through.
  key.sequence = 3;
  s.pass.sequence = 3;
  s.eligibility[1].visible = false;
  assert(p.publish(key, s));
  assert(p.admit(key));
  assert(p.queueTap(key, 4, 160, 120));
  auto live = s.eligibility;
  live[1].target.actorGeneration++;
  assert(!p.consumeTap(key, 4, live));
  assert(p.queueTap(key, 5, 160, 120));
  live = s.eligibility;
  live.pop_back();
  assert(!p.consumeTap(key, 5, live));
  assert(p.queueTap(key, 6, 160, 120));
  live = s.eligibility;
  live.push_back(live[1]);
  assert(!p.consumeTap(key, 6, live));
  // An actual empty completed audience remains empty with physical X available.
  key.sequence = 4;
  s = source(key);
  s.quads.clear();
  s.eligibility.clear();
  assert(p.publish(key, s));
  assert(p.admit(key));
  assert(!p.queueTap(key, 7, 160, 120));
  assert(p.physicalSelectionAvailable());
}
} // namespace
int main() {
  publicationAndConsumption();
  sourceValidation();
  geometryAndLiveAmbiguity();
}
