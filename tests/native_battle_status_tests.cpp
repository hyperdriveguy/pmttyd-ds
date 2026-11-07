#include "event_native_runtime.hpp"
#include "native_battle_status.hpp"
#include <cassert>
#include <iostream>
#include <memory>
using namespace ttyd;
namespace {
struct Fixture {
  EventNativeRuntime vm{0x81000000, EventNativePlatform{}};
  bool alive = true;
  std::shared_ptr<int> life = std::make_shared<int>(1);
  std::unique_ptr<NativeBattleStatusReader> reader;
  static constexpr uint32_t B = 0x84000000, M = 0x84101000, P = 0x84102000,
                            pouch = 0x85000000;
  Fixture() {
    vm.gp.battleContext = 1;
    assert(vm.memory.registerRam(B, std::vector<uint8_t>(0x120)) &&
           vm.memory.registerRam(M, std::vector<uint8_t>(0x120)) &&
           vm.memory.registerRam(P, std::vector<uint8_t>(0x120)) &&
           vm.memory.registerRam(pouch, std::vector<uint8_t>(0x100)) &&
           vm.memory.registerRam(0x8041eb00, std::vector<uint8_t>(4)) &&
           vm.memory.registerRam(0x8041eb90, std::vector<uint8_t>(4)));
    word(0x8041eb90, B);
    word(0x8041eb00, pouch);
    word(B + 0x24, M);
    word(B + 0x28, P);
    word(M + 8, 222);
    word(P + 8, 224);
    half(M + 0x10c, 7);
    half(M + 0x108, 10);
    half(M + 0x112, 4);
    half(M + 0x10e, 5);
    half(P + 0x10c, 9);
    half(P + 0x108, 15);
    half(pouch + 0x7a, 150);
    half(pouch + 0x7c, 999);
    half(pouch + 0x96, 37);
    half(pouch + 0x78, 123);
    half(pouch + 0x8c, 7);
    reader =
        std::make_unique<NativeBattleStatusReader>(vm, life, this, [](void *p) {
          return static_cast<Fixture *>(p)->alive;
        });
  }
  void word(uint32_t a, uint32_t v) { assert(vm.memory.writeWord(a, v)); }
  void half(uint32_t a, uint16_t v) { assert(vm.memory.writeHalf(a, v)); }
};
void actualPackedResources() {
  Fixture f;
  auto s = f.reader->snapshot();
  const std::array<int16_t, 11> expected{7,  10, 4,   5,   1,  9,
                                         15, 37, 123, 150, 999};
  assert(s && s->values == expected && s->partnerPortrait == 428 &&
         s->gaugeCount == 4);
  assert(s->gauge[0].icon == 463 && s->gauge[0].x == 32 &&
         s->gauge[1].icon == 447 && s->gauge[2].icon == 455 &&
         s->gauge[3].icon == 455 && s->gauge[1].y == 12);
  f.half(Fixture::M + 0x10c, 3);
  s = f.reader->snapshot();
  assert(s && s->values[0] == 3);
}
void sourcePartnerAliasesAndColor() {
  Fixture f;
  const uint16_t expected[]{428, 429, 431, 438, 439, 430, 440};
  for (uint32_t k = 224; k < 231; ++k) {
    f.word(Fixture::P + 8, k);
    for (unsigned color = 0; color < 8; ++color) {
      f.half(Fixture::pouch + 56, uint16_t(color << 13));
      auto s = f.reader->snapshot();
      assert(s && s->partnerPortrait ==
                      (k == 226 ? uint16_t(color < 6 ? 431 + color : 437)
                                : expected[k - 224]));
    }
  }
  f.word(Fixture::P + 0x1c, 8);
  auto s = f.reader->snapshot();
  assert(s && s->values[4] == 0 && s->values[5] == 0 && !s->partnerPortrait);
}
void gaugeSourceBehavior() {
  Fixture f;
  for (unsigned flags = 0; flags < 256; ++flags)
    for (int ap : {-1, 0, 1, 6, 7, 99, 100, 101, 799, 999}) {
      f.half(Fixture::pouch + 0x8c, uint16_t(flags));
      f.half(Fixture::pouch + 0x7a, uint16_t(ap));
      auto s = f.reader->snapshot();
      assert(s);
      unsigned capacity = 0;
      for (unsigned b = 0; b < 8; ++b)
        if (flags & (1u << b))
          capacity += 100;
      const unsigned n = unsigned(std::max(0, std::min(ap, int(capacity)))),
                     full = n / 100, remainder = n % 100;
      const unsigned partial =
          remainder ? std::max(1u, remainder * 15 / 99) : 0;
      unsigned count = partial ? 1 : 0;
      if (partial)
        assert(s->gauge[0].icon == 456 + partial);
      for (unsigned b = 0; b < 8 && flags & (1u << b); ++b) {
        assert(s->gauge[count].icon == (b < full ? 447 + b : 455));
        ++count;
      }
      assert(s->gaugeCount == count);
    }
}
void capturedPouchAndFreshNextGetter() {
  Fixture f;
  std::vector<uint8_t> old;
  assert(f.vm.memory.extractRam(Fixture::pouch, 0x100, old) &&
         f.vm.memory.registerRam(
             Fixture::pouch,
             std::vector<uint8_t>(old.begin(), old.begin() + 0x7b)));
  assert(!f.reader->snapshot() && f.reader->pending() == 0x800d3c44);
  const uint32_t next = 0x85001000;
  assert(f.vm.memory.registerRam(next, std::vector<uint8_t>(0x100)));
  f.half(next + 0x7a, 250);
  f.half(next + 0x7c, 456);
  f.half(next + 0x8c, 3);
  f.word(0x8041eb00, next);
  assert(f.vm.memory.registerRam(
      Fixture::pouch + 0x7b,
      std::vector<uint8_t>(old.begin() + 0x7b, old.end())));
  auto v = f.reader->snapshot();
  assert(v && v->values[9] == 150 && v->values[10] == 456 &&
         v->gaugeCount == 3);
}
void unknownDoesNotPublishOrRecapture() {
  Fixture f;
  std::vector<uint8_t> m;
  assert(f.vm.memory.extractRam(Fixture::M, 0x120, m) &&
         f.vm.memory.registerRam(
             Fixture::M, std::vector<uint8_t>(m.begin(), m.begin() + 9)));
  assert(!f.reader->snapshot() && f.reader->pending() == 0x800f87a8);
  f.word(Fixture::B + 0x24, 0);
  assert(f.vm.memory.registerRam(Fixture::M + 9,
                                 std::vector<uint8_t>(m.begin() + 9, m.end())));
  auto s = f.reader->snapshot();
  assert(s && s->values[0] == 7);
  f.alive = false;
  assert(!f.reader->snapshot());
  f.alive = true;
  assert(!f.reader->snapshot());
}
} // namespace
int main() {
  actualPackedResources();
  sourcePartnerAliasesAndColor();
  capturedPouchAndFreshNextGetter();
  gaugeSourceBehavior();
  unknownDoesNotPublishOrRecapture();
  std::cout << "actual packed status/partner/color/AP plan tests PASS\n";
}
