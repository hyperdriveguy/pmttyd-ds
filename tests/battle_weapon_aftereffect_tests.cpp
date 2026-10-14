#include "battle_weapon_aftereffect.hpp"
#include <cassert>
#include <functional>
#include <map>
#include <vector>
using namespace ttyd;
struct Fixture {
  static constexpr uint32_t B = 0x10000, W = 0x40000, A = 0x50000;
  std::map<uint32_t, uint32_t> words{{0x8041eb90, B}, {A, W}};
  std::map<uint32_t, uint8_t> bytes;
  std::vector<int32_t> draws, bounds;
  std::vector<uint32_t> requests;
  unsigned at = 0;
  bool ready = true;
  std::function<void(unsigned)> onDraw;
  std::function<void(uint32_t)> onRequest;
  EventEntry e;
  Fixture() { e.arguments = A; }
  BattleWeaponAftereffectServices services() {
    BattleWeaponAftereffectServices s;
    s.memory.memory.user = this;
    s.memory.user = this;
    s.user = this;
    s.memory.memory.word = [](void *p, uint32_t a) {
      return static_cast<Fixture *>(p)->words[a];
    };
    s.memory.byte = [](void *p, uint32_t a) {
      return static_cast<Fixture *>(p)->bytes[a];
    };
    s.values.resolve = [](void *, EventEntry &, int32_t v) { return v; };
    s.random = [](void *p, int32_t bound) {
      auto &f = *static_cast<Fixture *>(p);
      f.bounds.push_back(bound);
      if (f.onDraw)
        f.onDraw(f.at);
      assert(f.at < f.draws.size());
      return f.draws[f.at++];
    };
    s.request = [](void *p, uint32_t fn, int32_t arg) {
      auto &f = *static_cast<Fixture *>(p);
      assert(arg == (fn == 0x80148414 ? 1 : 0));
      f.requests.push_back(fn);
      if (f.onRequest)
        f.onRequest(fn);
      return f.ready;
    };
    return s;
  }
  auto run() {
    at = 0;
    return packedBattleWeaponAftereffectEvent(e, services());
  }
};
int main() {
  for (int32_t roll : {0, 2, 3, 5, 6, 7, 10}) {
    Fixture f;
    f.bytes[Fixture::W + 0xb4] = 3;
    f.bytes[Fixture::W + 0xb5] = 3;
    f.bytes[Fixture::W + 0xb6] = 1;
    f.bytes[Fixture::W + 0xb7] = 4;
    f.draws = {roll, 99, 99, 99};
    assert(f.run() == 2 &&
           f.bounds == std::vector<int32_t>({11, 100, 100, 100}));
    const auto expected = roll < 3
                              ? std::vector<uint32_t>{0x801496f0, 0x80149684}
                          : roll < 6 ? std::vector<uint32_t>{0x801496f0}
                          : roll < 7 ? std::vector<uint32_t>{0x80149684}
                                     : std::vector<uint32_t>{};
    assert(f.requests == expected);
  }
  Fixture zero;
  zero.draws = {0, 99, 99, 99};
  assert(zero.run() == 2 && zero.bounds.front() == 0 && zero.at == 4 &&
         zero.requests.empty());
  Fixture inhibited;
  inhibited.words[Fixture::B + 0x19078] = 0x30;
  inhibited.bytes[Fixture::B + 0x180f8] = 14;
  inhibited.draws = {0, 99, 99, 99};
  assert(inhibited.run() == 2 && inhibited.requests.empty());
  Fixture restore;
  restore.bytes[Fixture::B + 0x180f8] = 6;
  restore.draws = {0, 0, 99, 99, 99};
  restore.bytes[Fixture::W + 0xb8] = 1;
  restore.onDraw = [&](unsigned i) {
    if (i == 1)
      restore.bytes[Fixture::B + 0x180f8] = 2;
  };
  assert(restore.run() == 2 &&
         restore.requests == std::vector<uint32_t>({0x80149684, 0x801495e0}));
  Fixture force;
  force.words[Fixture::B + 0x19078] = 0x380;
  force.draws = {0, 99, 99, 99};
  assert(force.run() == 2 &&
         force.requests ==
             std::vector<uint32_t>({0x80148bf0, 0x80148414, 0x801487c0}));
  Fixture falling;
  falling.bytes[Fixture::W + 0xbb] = 1;
  falling.draws = {0, 99, 99, 0};
  falling.onRequest = [&](uint32_t fn) {
    if (fn == 0x801487c0) {
      falling.bytes[Fixture::B + 0x180f8] = 0x20;
      falling.words[Fixture::B + 0x19078] = 1;
    }
  };
  assert(falling.run() == 2 && falling.at == 4 &&
         falling.requests == std::vector<uint32_t>({0x801487c0, 0x80147644}));
  Fixture object;
  object.bytes[Fixture::B + 0x180f8] = 0x20;
  object.bytes[Fixture::W + 0xbc] = 1;
  object.draws = {0, 99, 99, 99, 0};
  assert(object.run() == 2 && object.at == 5 &&
         object.requests == std::vector<uint32_t>{0x80147644});
  Fixture unavailable;
  unavailable.words[Fixture::B + 0x19078] = 0x10;
  unavailable.ready = false;
  unavailable.draws = {0};
  assert(!unavailable.run() && unavailable.at == 1 &&
         unavailable.requests == std::vector<uint32_t>{0x801496f0});
}
