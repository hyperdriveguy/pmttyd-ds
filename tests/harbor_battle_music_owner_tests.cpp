#include "harbor_battle_music_owner.hpp"
#include <array>
#include <cassert>
#include <iostream>
using namespace ttyd;
namespace {
struct Fixture {
  HarborBattleMusicOwner owner;
  HarborBattleMusicBinding binding;
  HarborBattleMusicServices services;
  std::shared_ptr<const void> runtime = std::make_shared<int>(1),
                              proof = std::make_shared<int>(2),
                              backend = std::make_shared<int>(3);
  uint64_t generation = 1;
  int32_t track = -1;
  unsigned starts = 0, stops = 0, mixes = 0;
  bool alive = true, native = true, holdStart = false, holdMix = false,
       holdStop = false, reenter = false;
  bool on = true;
  Fixture() {
    binding = {runtime, proof, this, [](void *p) {
                 auto &f = *static_cast<Fixture *>(p);
                 if (f.reenter) {
                   assert(!f.owner.update(f.runtime, true));
                   assert(!f.owner.retire(f.runtime));
                   assert(!f.owner.begin(f.binding, f.services, true, true));
                 }
                 return f.alive;
               }};
    services.lifetime = backend;
    services.user = this;
    services.current = [](void *p) {
      return static_cast<Fixture *>(p)->native;
    };
    services.start = [](void *p, int32_t track,
                        bool enabled) -> std::optional<HarborBattleMusicLease> {
      auto &f = *static_cast<Fixture *>(p);
      if (f.holdStart)
        return {};
      f.track = track;
      f.on = enabled;
      ++f.starts;
      return HarborBattleMusicLease{0, ++f.generation, track};
    };
    services.valid = [](void *p, const HarborBattleMusicLease &q) {
      auto &f = *static_cast<Fixture *>(p);
      return q.generation == f.generation && q.track == f.track;
    };
    services.mix = [](void *p, const HarborBattleMusicLease &q, bool enabled) {
      auto &f = *static_cast<Fixture *>(p);
      if (f.holdMix)
        return false;
      assert(q.generation == f.generation);
      ++f.mixes;
      f.on = enabled;
      return true;
    };
    services.stop = [](void *p, const HarborBattleMusicLease &q) {
      auto &f = *static_cast<Fixture *>(p);
      if (f.holdStop)
        return false;
      if (q.generation == f.generation) {
        ++f.stops;
        f.track = -1;
        ++f.generation;
      }
      return true;
    };
  }
};
} // namespace
int main() {
  Fixture f;
  assert(f.owner.begin(f.binding, f.services, false, false));
  assert(f.starts == 1 && f.track == 12 && !f.on);
  assert(f.owner.update(f.runtime, true) && f.starts == 1 && f.on);
  f.holdStop = true;
  assert(!f.owner.begin(f.binding, f.services, true, true) && f.stops == 0 &&
         f.starts == 1);
  f.holdStop = false;
  f.holdStart = true;
  assert(!f.owner.begin(f.binding, f.services, true, true) && f.stops == 1 &&
         f.starts == 1);
  assert(!f.owner.begin(f.binding, f.services, true, true) && f.stops == 1);
  f.holdStart = false;
  f.holdMix = true;
  assert(!f.owner.begin(f.binding, f.services, true, true) && f.starts == 2 &&
         f.track == 2);
  assert(!f.owner.update(f.runtime, true) && f.starts == 2);
  f.holdMix = false;
  assert(f.owner.update(f.runtime, true) && f.starts == 2);
  assert(f.owner.begin(f.binding, f.services, false, false) && f.track == 2 &&
         f.starts == 2 && !f.on);
  auto changed = f.binding;
  changed.proof = std::make_shared<int>(4);
  assert(!f.owner.begin(changed, f.services, true, true));
  auto falseOwner =
      std::shared_ptr<const void>(f.runtime.get(), [](const void *) {});
  auto falseBinding = f.binding;
  falseBinding.runtime = falseOwner;
  assert(!f.owner.begin(falseBinding, f.services, true, true));
  assert(!f.owner.retire(falseOwner));
  auto foreign = std::make_shared<int>(5);
  assert(!f.owner.retire(foreign));
  f.reenter = true;
  assert(f.owner.update(f.runtime, true));
  f.reenter = false;
  f.alive = false;
  assert(!f.owner.update(f.runtime, true));
  f.holdStop = true;
  assert(!f.owner.retire(f.runtime) && f.stops == 1);
  f.holdStop = false;
  assert(f.owner.retire(f.runtime) && f.stops == 2);
  assert(f.owner.retire(f.runtime) && f.stops == 2 &&
         !f.owner.begin(f.binding, f.services, true, true));
  // Actual replacement stream generation survives stale-lease retirement.
  Fixture g;
  assert(g.owner.begin(g.binding, g.services, true, true));
  ++g.generation;
  g.track = 12;
  assert(!g.owner.update(g.runtime, true));
  assert(g.owner.retire(g.runtime) && g.stops == 0 && g.track == 12);
  Fixture h;
  h.holdStart = true;
  assert(!h.owner.begin(h.binding, h.services, true, true));
  h.native = false;
  assert(!h.owner.retire(h.runtime));
  h.native = true;
  assert(h.owner.retire(h.runtime) && h.starts == 0);
  Fixture released;
  auto runtimeWeak = std::weak_ptr<const void>(released.runtime);
  auto proofWeak = std::weak_ptr<const void>(released.proof);
  assert(released.owner.begin(released.binding, released.services, true, true));
  assert(released.owner.retire(released.runtime));
  released.binding = {};
  released.runtime.reset();
  released.proof.reset();
  assert(runtimeWeak.expired() && proofWeak.expired());
  std::cout
      << "Actual stream lease ownership, no-adoption/generation replacement, "
         "retained transitions/mute/retirement/reentry pass\n";
}
