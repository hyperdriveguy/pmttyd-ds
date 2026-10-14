#include "battle_action_feedback.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <map>
#include <tuple>
#include <vector>
using namespace ttyd;
struct Fixture {
  static constexpr uint32_t B = 0x10000, W = 0x40000, A = 0x50000;
  std::map<uint32_t, uint32_t> words{{0x8041eb90, B},
                                     {A, W},
                                     {0x804221a4, 0x3e800000},
                                     {0x804221a0, 0x3f000000}};
  std::map<uint32_t, uint8_t> bytes{{W + 0x19, 7}, {W + 0x1a, 8}};
  std::map<uint32_t, uint16_t> halves;
  std::vector<std::tuple<uint32_t, int32_t, int32_t>> calls;
  bool ready = true, good = false;
  unsigned declarations = 0;
  EventEntry e;
  Fixture() { e.arguments = A; }
  BattleActorServices memory() {
    BattleActorServices m;
    m.user = this;
    m.memory.user = this;
    m.memory.word = [](void *p, uint32_t at) {
      return static_cast<Fixture *>(p)->words[at];
    };
    m.memory.storeWord = [](void *p, uint32_t at, uint32_t v) {
      static_cast<Fixture *>(p)->words[at] = v;
    };
    m.memory.half = [](void *p, uint32_t at) {
      return static_cast<Fixture *>(p)->halves[at];
    };
    m.byte = [](void *p, uint32_t at) {
      return static_cast<Fixture *>(p)->bytes[at];
    };
    m.memory.storeByte = [](void *p, uint32_t at, uint8_t v) {
      static_cast<Fixture *>(p)->bytes[at] = v;
    };
    return m;
  }
  BattleActionFeedbackServices services() {
    BattleActionFeedbackServices s;
    s.memory = memory();
    s.user = this;
    s.values.resolve = [](void *, EventEntry &, int32_t v) { return v; };
    s.signedWordFloat = [](void *, int32_t v) { return float(v); };
    s.fma = [](void *, float a, float b, float c) { return std::fma(a, b, c); };
    s.audience = [](void *p, uint32_t w, bool good) {
      auto &f = *static_cast<Fixture *>(p);
      assert(w == W);
      f.good = good;
      ++f.declarations;
      return f.ready;
    };
    return s;
  }
  auto declare(int32_t value) {
    words[A + 4] = uint32_t(value);
    return packedBattleDeclareActionResultEvent(e, services());
  }
  uint32_t actor(unsigned id = 0) { return B + 0x2858 + 0x1bc + 0x134 * id; }
  BattleAudienceFeedbackServices audience() {
    BattleAudienceFeedbackServices s;
    s.memory = memory();
    s.user = this;
    s.request = [](void *p, uint32_t f, int32_t a, int32_t b) {
      auto &x = *static_cast<Fixture *>(p);
      x.calls.emplace_back(f, a, b);
      return x.ready;
    };
    return s;
  }
};
int main() {
  Fixture f;
  assert(f.declare(-1) == 2 && f.bytes[Fixture::B + 0x16f59] == 1 &&
         !f.bytes[Fixture::B + 0x19074] && !f.good &&
         !f.words[Fixture::B + 0x18ffc]);
  for (uint8_t count : {uint8_t(199), uint8_t(200), uint8_t(255)}) {
    f.bytes[Fixture::B + 0x16f59] = count;
    assert(f.declare(-1) == 2 &&
           f.bytes[Fixture::B + 0x16f59] == (count == 199 ? 200 : count));
  }
  assert(f.declare(-2) == 2 && f.good && f.bytes[Fixture::B + 0x16f58] == 1 &&
         f.bytes[Fixture::B + 0x19074] == 1 &&
         f.words[Fixture::B + 0x19000] == 0 &&
         f.words[Fixture::B + 0x18ffc] == Fixture::W &&
         f.bytes[Fixture::B + 0x19005] == 7 &&
         f.bytes[Fixture::B + 0x19006] == 8);
  assert(f.declare(2) == 2 && f.words[Fixture::B + 0x19000] == 0x3f800000);
  f.ready = false;
  assert(!f.declare(0) && f.bytes[Fixture::B + 0x16f58] == 3);
  Fixture audience;
  assert(packedBattleAudienceWaitingCount(audience.memory()) == 0);
  audience.words[audience.actor()] = 1;
  audience.halves[audience.actor() + 0x1c] = 0xffff;
  for (uint8_t mode : {uint8_t(0), uint8_t(3), uint8_t(4), uint8_t(255)}) {
    audience.bytes[audience.actor() + 0x19] = mode;
    assert(packedBattleAudienceWaiting(0, audience.memory()) ==
           (mode <= 3 ? 1 : 0));
  }
  audience.bytes[audience.actor() + 0x19] = 0;
  for (uint32_t flags : {3u, 0x101u, 0x11u}) {
    audience.words[audience.actor()] = flags;
    assert(!packedBattleAudienceWaiting(0, audience.memory()));
  }
  audience.words[audience.actor()] = 1;
  assert(packedBattleAudienceActionFeedback(Fixture::W, true,
                                            audience.audience()) &&
         audience.calls == std::vector<std::tuple<uint32_t, int32_t, int32_t>>(
                               {{0x801a0aec, 180, 120}, {0x801a12fc, 0, 0}}) &&
         audience.words[Fixture::B + 0x2858] == 0x10);
  audience.calls.clear();
  audience.words[Fixture::W + 0x1c] = 0x80189c38;
  assert(
      packedBattleAudienceActionFeedback(Fixture::W, false,
                                         audience.audience()) &&
      audience.calls ==
          std::vector<std::tuple<uint32_t, int32_t, int32_t>>(
              {{0x801a0fd0, 0, 0}, {0x801a12fc, 0, 0}, {0x801a1520, 7, 0}}) &&
      audience.words[Fixture::B + 0x2858] == 0x30);
  audience.ready = false;
  const auto old = audience.words[Fixture::B + 0x2858];
  assert(!packedBattleAudienceActionFeedback(Fixture::W, true,
                                             audience.audience()) &&
         audience.words[Fixture::B + 0x2858] == old);
  Fixture empty;
  auto s = empty.audience();
  s.request = nullptr;
  assert(packedBattleAudienceActionFeedback(Fixture::W, false, s) &&
         empty.words[Fixture::B + 0x2858] == 0x20);
}
