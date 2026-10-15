#include "battle_packed_recovery.hpp"
#include "battle_present_item.hpp"
#include "harbor_tutorial_packed.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <functional>
#include <map>
#include <memory>
#include <vector>
using namespace ttyd;
struct Fixture {
  static constexpr uint32_t B = 0x100000, B2 = 0x200000, U = 0x300000,
                            Other = 0x310000, Pos = 0x400000;
  std::map<uint32_t, uint32_t> w{{0x8041eb90, B},
                                 {B + 0x20, U},
                                 {0x802efe78, 0x43300000},
                                 {0x802efe7c, 0x80000000}};
  std::map<uint32_t, uint16_t> h;
  std::map<uint32_t, uint8_t> b;
  std::vector<uint32_t> reads, writes;
  std::function<void(uint32_t)> onRead, onWrite;
  bool ready = false, math = true;
  unsigned effects = 0, converts = 0, fmas = 0, adds = 0;
  uint32_t kind = 99;
  std::array<float, 3> effect{};
  Fixture() {
    real(U + 0x114, 2);
    real(0x804225f8, 5);
    real(U + 0x3c, 1);
    real(U + 0x40, 2);
    real(U + 0x44, 3);
    h[U + 0xce] = 10;
    h[U + 0x108] = 10;
    h[U + 0x10c] = 5;
    h[U + 0x10e] = 10;
    h[U + 0x112] = 5;
  }
  void real(uint32_t at, float v) {
    uint32_t n;
    std::memcpy(&n, &v, 4);
    w[at] = n;
  }
  float real(uint32_t at) {
    float v;
    const auto n = w[at];
    std::memcpy(&v, &n, 4);
    return v;
  }
  BattleActorServices memory() {
    BattleActorServices a;
    a.user = this;
    a.memory.user = this;
    a.byte = [](void *p, uint32_t at) {
      return static_cast<Fixture *>(p)->b[at];
    };
    a.memory.word = [](void *p, uint32_t at) {
      auto &f = *static_cast<Fixture *>(p);
      f.reads.push_back(at);
      const auto n = f.w[at];
      if (f.onRead)
        f.onRead(at);
      return n;
    };
    a.memory.half = [](void *p, uint32_t at) {
      auto &f = *static_cast<Fixture *>(p);
      f.reads.push_back(at);
      const auto n = f.h[at];
      if (f.onRead)
        f.onRead(at);
      return n;
    };
    a.memory.storeWord = [](void *p, uint32_t at, uint32_t n) {
      auto &f = *static_cast<Fixture *>(p);
      f.w[at] = n;
      f.writes.push_back(at);
      if (f.onWrite)
        f.onWrite(at);
    };
    a.memory.storeHalf = [](void *p, uint32_t at, uint16_t n) {
      auto &f = *static_cast<Fixture *>(p);
      f.h[at] = n;
      f.writes.push_back(at);
      if (f.onWrite)
        f.onWrite(at);
    };
    a.memory.storeByte = [](void *p, uint32_t at, uint8_t n) {
      auto &f = *static_cast<Fixture *>(p);
      f.b[at] = n;
      f.writes.push_back(at);
      if (f.onWrite)
        f.onWrite(at);
    };
    return a;
  }
  BattlePresentItemServices services() {
    BattlePresentItemServices s;
    s.actors = memory();
    s.user = this;
    s.privatePosition = Pos;
    s.subtractDoubleSingle = [](void *p, double a,
                                double b) -> std::optional<float> {
      auto &f = *static_cast<Fixture *>(p);
      ++f.converts;
      if (!f.math)
        return {};
      return float(a - b);
    };
    s.fma = [](void *p, float a, float b, float c) -> std::optional<float> {
      ++static_cast<Fixture *>(p)->fmas;
      return std::fma(a, b, c);
    };
    s.add = [](void *p, float a, float b) -> std::optional<float> {
      ++static_cast<Fixture *>(p)->adds;
      return a + b;
    };
    s.effect = [](void *p, uint32_t kind, int32_t n,
                  const std::array<float, 3> &v) {
      auto &f = *static_cast<Fixture *>(p);
      assert(n == 1);
      ++f.effects;
      f.kind = kind;
      f.effect = v;
      if (f.ready)
        f.w[0x8041eb90] = B2;
      return f.ready;
    };
    return s;
  }
};
void branches() {
  using F = Fixture;
  for (int item : {339, 340, -1, 0, 1, 122, 125, 338})
    for (int target : {-2, -1, 0, 63, 64}) {
      F f;
      f.w[F::B + 0x1603c] = uint32_t(item);
      f.w[F::B + 0x16044] = uint32_t(target);
      f.w[F::B + 0x1720] = 0xffffffff;
      f.w[F::B + 0x1c5c] = 4;
      f.w[F::B + 0x171c] = 12;
      f.onWrite = [&](uint32_t at) {
        if (at == F::B + 0x1720)
          f.w[F::B + 0x1c5c] = 6;
      };
      BattlePresentItemCursor c;
      const bool generic =
          item > 0 && item < 339 && target >= -1 && target < 64;
      assert(resumePackedBattlePresentItem(f.services(), c) ==
                 (generic ? 1 : 0) &&
             !c.active);
      if (generic) {
        assert(f.b[F::B + 0x1c74] == 2 && f.w[F::B + 0x1720] == 0xffeeeeee &&
               f.w[F::B + 0x1c60] == 6 && f.w[F::B + 0x1c5c] == 12 &&
               f.w[F::B + 0x171c] == 29);
        assert(f.w[F::B + 0x1603c] == uint32_t(item));
      } else
        assert(f.writes.empty());
      if (item >= 339)
        assert(f.reads.size() == 3); // captured B, getter's fresh B, then item.
    }
}
void recovery() {
  using F = Fixture;
  for (int item : {123, 124}) {
    F f;
    f.w[F::B + 0x1603c] = item;
    BattlePresentItemCursor c;
    auto s = f.services();
    assert(!resumePackedBattlePresentItem(s, c));
    assert(f.h[F::U + (item == 123 ? 0x10c : 0x112)] == 6 && f.effects == 1 &&
           f.effect[0] == 1 && f.effect[1] == 27 && f.effect[2] == 3 &&
           f.kind == uint32_t(item == 123 ? 0 : 1));
    f.real(F::U + 0x40, 99);
    f.real(F::U + 0x114, 99);
    f.real(F::Pos + 4, 99);
    assert(!resumePackedBattlePresentItem(s, c));
    assert(f.h[F::U + (item == 123 ? 0x10c : 0x112)] == 6 &&
           f.effect[1] == 27 && f.converts == 1 && f.fmas == 1 && f.adds == 1);
    F other;
    assert(!resumePackedBattlePresentItem(other.services(), c) &&
           other.writes.empty());
    f.w[F::B2 + 0x1603c] = 99;
    f.ready = true;
    assert(resumePackedBattlePresentItem(s, c) == 0 && !c.active);
    assert(f.w[F::B + 0x1603c] == uint32_t(item) && f.w[F::B2 + 0x1603c] == 0 &&
           f.w[F::B + 0x171c] == 0);
  }
  F held;
  held.w[F::B + 0x1603c] = 123;
  held.math = false;
  BattlePresentItemCursor pending;
  assert(!resumePackedBattlePresentItem(held.services(), pending) &&
         held.h[F::U + 0x10c] == 6 && !held.effects);
  held.math = true;
  assert(!resumePackedBattlePresentItem(held.services(), pending) &&
         held.h[F::U + 0x10c] == 6 && held.effects == 1);
  // Scale/5 are loaded before the single conversion; position components follow
  // it.
  F order;
  order.w[F::B + 0x1603c] = 123;
  auto s = order.services();
  s.subtractDoubleSingle = [](void *p, double a,
                              double b) -> std::optional<float> {
    auto &f = *static_cast<F *>(p);
    f.real(F::U + 0x114, 9);
    f.real(0x804225f8, 9);
    f.real(F::Pos + 4, 8);
    return float(a - b);
  };
  BattlePresentItemCursor c;
  assert(!resumePackedBattlePresentItem(s, c) && order.effect[1] == 33);
  // Fused cancellation retains 2^-23; a separately rounded multiply gives0.
  F fused;
  fused.w[F::B + 0x1603c] = 123;
  fused.h[F::U + 0xce] = 0xfffd;
  fused.w[F::U + 0x114] = 0x3fd55555;
  fused.real(F::U + 0x40, 0);
  BattlePresentItemCursor fusion;
  assert(!resumePackedBattlePresentItem(fused.services(), fusion));
  assert(fused.effect[1] == std::ldexp(1.f, -23));
  // GetPos aliases y: writing X into y changes the next Y load.
  F alias;
  alias.w[F::B + 0x1603c] = 123;
  s = alias.services();
  s.privatePosition = F::U + 0x40;
  assert(
      !resumePackedBattlePresentItem(s, c)); // different active owner rejected
  c = {};
  assert(!resumePackedBattlePresentItem(s, c) && alias.effect[0] == 1 &&
         alias.effect[1] == 26 && alias.effect[2] == 1);
}
void packedRecovery() {
  using F = Fixture;
  for (int kind : {-1, 223, 224, 230, 231}) {
    F boundary;
    boundary.w[F::U + 8] = uint32_t(kind);
    boundary.w[F::B + 0x24] = F::Other;
    boundary.w[F::Other + 8] = 222;
    boundary.h[F::Other + 0x10e] = 8;
    boundary.h[F::Other + 0x112] = 8;
    const bool partner = kind >= 224 && kind < 231;
    assert(packedBattleMaxFp(F::U, boundary.memory()) == (partner ? 8 : 10));
    recoverPackedBattleFp(F::U, 1, boundary.memory());
    assert(boundary.h[F::U + 0x112] == (partner ? 5 : 6));
    assert(boundary.h[F::Other + 0x112] == 8);
  }
  F wrap;
  wrap.h[F::U + 0x112] = 32767;
  recoverPackedBattleFp(F::U, 1, wrap.memory());
  assert(wrap.h[F::U + 0x112] == 0x8000);

  F hp;
  hp.h[F::U + 0x10c] = 32767;
  recoverPackedBattleHp(F::U, 1, hp.memory());
  assert(hp.h[F::U + 0x10c] == 0x8000);
  hp.h[F::U + 0x10c] = 9;
  hp.onWrite = [&](uint32_t at) {
    if (at == F::U + 0x10c)
      hp.h[F::U + 0x108] = 4;
  };
  recoverPackedBattleHp(F::U, 1, hp.memory());
  assert(hp.h[F::U + 0x10c] == 4);
  F fp;
  fp.w[F::U + 8] = 224;
  fp.w[F::B + 0x20 + 4] = F::Other;
  fp.w[F::Other + 8] = 222;
  fp.h[F::Other + 0x10e] = 8;
  fp.h[F::Other + 0x112] = 8;
  recoverPackedBattleFp(F::U, 1, fp.memory());
  assert(fp.h[F::Other + 0x112] == 8 && fp.h[F::U + 0x112] == 5);
  F switches;
  switches.w[F::U + 8] = 224;
  switches.w[F::B + 0x20 + 4] = F::Other;
  switches.w[F::Other + 8] = 222;
  switches.h[F::Other + 0x10e] = 8;
  switches.h[F::U + 0x112] = 5;
  switches.onRead = [&](uint32_t at) {
    if (at == F::Other + 0x10e) {
      switches.w[F::U + 8] = 222;
      switches.w[F::B + 0x20 + 4] = 0;
    }
  };
  recoverPackedBattleFp(F::U, 1, switches.memory());
  assert(switches.h[F::U + 0x112] == 6);
}
void nativeGeneric() {
  auto vm =
      std::make_unique<EventNativeRuntime>(0x81000000, EventNativePlatform{});
  assert(
      vm->memory.registerRam(0x80422b44, {0x42, 0xb4, 0, 0, 0xc2, 0x48, 0, 0}));
  auto p = std::make_unique<HarborTutorialPacked>(*vm);
  assert(p->provision());
  const auto B = HarborTutorialPacked::Battle;
  auto put = [&](uint32_t at, uint32_t n) {
    assert(vm->memory.writeWord(at, n));
  };
  auto read = [&](uint32_t at) {
    uint32_t n;
    assert(vm->memory.readWord(at, n));
    return n;
  };
  put(B + 0x1603c, 236);
  put(B + 0x16044, 0xffffffff);
  put(B + 0x171c, 13);
  put(B + 0x1c5c, 1);
  put(B + 0x1720, 0xffffffff);
  assert(p->presentItem() == 1 && read(B + 0x171c) == 29 &&
         read(B + 0x1c5c) == 12 && read(B + 0x1c60) == 1 &&
         read(B + 0x1603c) == 236 && read(B + 0x1720) == 0xffeeeeee &&
         (read(B + 0x1c74) >> 24) == 2);
  // Native recovery applies once, then holds at its unbound real effect.
  assert(vm->memory.registerRam(0x802efe78, {0x43, 0x30, 0, 0, 0x80, 0, 0, 0}));
  assert(vm->memory.registerRam(0x804225f8, {0x40, 0xa0, 0, 0}));
  vm->platform.subtractDouble = [](void *, double a, double b) {
    return a - b;
  };
  vm->platform.roundDoubleFloat = [](void *, double a) { return float(a); };
  vm->platform.fmaFloat = [](void *, float a, float b, float c) {
    return std::fma(a, b, c);
  };
  vm->platform.addFloat = [](void *, float a, float b) { return a + b; };
  const auto U = HarborTutorialPacked::Units;
  put(B + 0x20, U);
  put(B + 0x16044, 0);
  put(B + 0x1603c, 123);
  assert(vm->memory.writeHalf(U + 0x108, 10));
  assert(vm->memory.writeHalf(U + 0x10c, 5));
  assert(vm->memory.writeHalf(U + 0xce, 10));
  put(U + 0x114, 0x3f800000);
  assert(!p->presentItem() && p->presentItemCursor.phase == 5);
  assert((read(U + 0x10c) >> 16) == 6 && read(B + 0x1603c) == 123);
  assert(!p->presentItem() && (read(U + 0x10c) >> 16) == 6);
  BattlePresentItemServices complete;
  complete.actors = nativeBattleActorServices(*vm);
  complete.privatePosition = 0x84600020;
  complete.effect = [](void *, uint32_t kind, int32_t amount,
                       const std::array<float, 3> &pos) {
    assert(kind == 0 && amount == 1 && pos[1] == 15);
    return true;
  };
  assert(resumePackedBattlePresentItem(complete, p->presentItemCursor) == 0 &&
         read(B + 0x1603c) == 0);
  for (int32_t i = 0; i < vm->battle.capacity; ++i)
    assert(!(vm->battle.entries[i].flags & 1));
}
int main() {
  branches();
  recovery();
  packedRecovery();
  nativeGeneric();
}
