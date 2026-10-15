#include "recovery_effect.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <map>
#include <string>
#include <vector>
using namespace ttyd;
namespace {
uint32_t bits(float f) {
  uint32_t n;
  std::memcpy(&n, &f, 4);
  return n;
}
struct Fixture {
  std::map<uint32_t, uint8_t> mem;
  uint32_t now = 10;
  int entryCalls = 0, heapCalls = 0, mayaCalls = 0, playCalls = 0,
      calcCalls = 0;
  int soundCalls = 0, childCalls = 0, mayaDeletes = 0, deletes = 0;
  bool admitMaya = true, admitSound = true, admitChild = true,
       admitDelete = true, admitDisplay = true;
  std::vector<uint32_t> sounds, displays;
  std::vector<std::string> order;
  std::array<float, 3> childPosition{};
  RecoveryEffectServices services;
  static Fixture &get(void *p) { return *static_cast<Fixture *>(p); }
  static uint32_t word(void *p, uint32_t a) {
    auto &f = get(p);
    uint32_t n = 0;
    for (int i = 0; i < 4; ++i)
      n = n * 256 + f.mem[a + i];
    return n;
  }
  static void put(void *p, uint32_t a, uint32_t n) {
    auto &f = get(p);
    for (int i = 3; i >= 0; --i) {
      f.mem[a + i] = uint8_t(n);
      n >>= 8;
    }
  }
  static uint16_t half(void *p, uint32_t a) {
    auto &f = get(p);
    return uint16_t(f.mem[a] * 256 + f.mem[a + 1]);
  }
  static void ph(void *p, uint32_t a, uint16_t n) {
    auto &f = get(p);
    f.mem[a] = uint8_t(n >> 8);
    f.mem[a + 1] = uint8_t(n);
  }
  static void pb(void *p, uint32_t a, uint8_t n) { get(p).mem[a] = n; }
  Fixture() {
    auto &s = services;
    s.user = this;
    s.memory.user = this;
    s.memory.word = word;
    s.memory.half = half;
    s.memory.storeWord = put;
    s.memory.storeHalf = ph;
    s.memory.storeByte = pb;
    s.byte = [](void *p, uint32_t a) { return get(p).mem[a]; };
    s.effectEntry = [](void *p) -> std::optional<uint32_t> {
      ++get(p).entryCalls;
      return 0x1000;
    };
    s.heapAllocate = [](void *p, int32_t heap,
                        uint32_t n) -> std::optional<uint32_t> {
      auto &f = get(p);
      ++f.heapCalls;
      assert(heap == 3);
      assert(n == word(p, 0x1008) * 0x44);
      return 0x2000;
    };
    s.mayaAllocate = [](void *p, uint32_t d) -> std::optional<uint32_t> {
      auto &f = get(p);
      assert(d == 0x80417a44);
      ++f.mayaCalls;
      if (!f.admitMaya)
        return {};
      return 0x3000;
    };
    s.mayaPlay = [](void *p, uint32_t a) {
      assert(a == 0x3000);
      ++get(p).playCalls;
      return true;
    };
    s.mayaCalc = [](void *p, uint32_t a) {
      assert(a == 0x3000);
      ++get(p).calcCalls;
      return true;
    };
    s.tick = [](void *p) { return get(p).now; };
    s.binary = [](void *, RecoveryBinary op, float a, float b) {
      volatile float result = 0;
      switch (op) {
      case RecoveryBinary::Add:
        result = a + b;
        break;
      case RecoveryBinary::Subtract:
        result = a - b;
        break;
      case RecoveryBinary::Multiply:
        result = a * b;
        break;
      case RecoveryBinary::Divide:
        result = a / b;
        break;
      }
      return result;
    };
    // Test-only numerical providers. These do not claim original libm parity.
    s.sine = [](void *, float a) -> std::optional<float> {
      return float(std::sin(double(a)));
    };
    s.cosine = [](void *, float a) -> std::optional<float> {
      return float(std::cos(double(a)));
    };
    s.interpolate = [](void *, int32_t mode, int32_t n, int32_t total, float a,
                       float b) -> std::optional<float> {
      assert(mode == 1 || mode == 11);
      return a + (b - a) * float(n) / float(total);
    };
    s.distance = [](void *, float x, float y, float a,
                    float b) -> std::optional<float> {
      return float(std::hypot(a - x, b - y));
    };
    s.random = [](void *) -> std::optional<int32_t> { return 0; };
    s.sfx = [](void *p, uint32_t a, const std::array<float, 3> &) {
      auto &f = get(p);
      ++f.soundCalls;
      if (!f.admitSound)
        return false;
      f.sounds.push_back(a);
      f.order.push_back("sfx");
      return true;
    };
    s.stardust = [](void *p, int32_t kind, const std::array<float, 3> &v,
                    int32_t count, int32_t life, float a, float b) {
      auto &f = get(p);
      ++f.childCalls;
      assert(kind == 2 && count == 2 && life == 30 && a == 30 && b == 30);
      if (!f.admitChild)
        return false;
      f.childPosition = v;
      f.order.push_back("child");
      return true;
    };
    s.mayaDelete = [](void *p, uint32_t a) {
      auto &f = get(p);
      assert(a == 0x3000);
      ++f.mayaDeletes;
      f.order.push_back("mayaDelete");
      return true;
    };
    s.effectDelete = [](void *p, uint32_t a) {
      auto &f = get(p);
      assert(a == 0x1000);
      ++f.deletes;
      if (!f.admitDelete)
        return false;
      f.order.push_back("delete");
      return true;
    };
    s.depth = [](void *,
                 const std::array<float, 3> &v) -> std::optional<float> {
      assert(v[0] == 1 && v[1] == 2 && v[2] == 3);
      return 7;
    };
    s.display = [](void *p, int32_t camera, int32_t cls, uint32_t cb,
                   uint32_t e, float z) {
      auto &f = get(p);
      assert(camera == 4 && e == 0x1000 && z == 7);
      assert(cls == (cb == 0x801422f4 ? 1 : 2));
      if (!f.admitDisplay)
        return false;
      f.displays.push_back(cb);
      return true;
    };
    const std::pair<uint32_t, float> constants[] = {
        {0x80422dcc, 0},
        {0x80422dc8, 90},
        {0x80422dd0, 1},
        {0x80422df0, 20},
        {0x80422e20, 6.2831854820251465f},
        {0x80422e24, 360},
        {0x80422e28, 12},
        {0x80422e2c, 0.7f},
        {0x80422e30, 0.99f},
        {0x80422e34, 30},
        {0x80422e38, -30},
        {0x80422e3c, -15}};
    for (auto n : constants)
      put(this, n.first, bits(n.second));
  }
  void create(int kind, int amount) {
    RecoveryEntryCursor c;
    assert(resumeRecoveryEffectEntry(kind, amount, {1, 2, 3}, services, c) ==
           0x1000);
    assert(!c.active);
  }
};
void entryWaits() {
  Fixture f;
  RecoveryEntryCursor c;
  f.admitMaya = false;
  assert(!resumeRecoveryEffectEntry(0, 1, {1, 2, 3}, f.services, c));
  assert(c.phase == 2 && f.entryCalls == 1 && f.heapCalls == 1 &&
         f.playCalls == 0);
  assert(!resumeRecoveryEffectEntry(0, 2, {1, 2, 3}, f.services, c));
  assert(f.mayaCalls == 1);
  f.admitMaya = true;
  f.admitSound = false;
  assert(!resumeRecoveryEffectEntry(0, 1, {1, 2, 3}, f.services, c));
  assert(c.phase == 5 && f.playCalls == 1 && f.soundCalls == 1);
  assert(Fixture::word(&f, 0x1008) == 2 &&
         Fixture::word(&f, 0x1010) == 0x80142bc4);
  assert(Fixture::word(&f, 0x2034) == 10 &&
         Fixture::word(&f, 0x2040) == bits(-30));
  f.admitSound = true;
  assert(resumeRecoveryEffectEntry(0, 1, {1, 2, 3}, f.services, c) == 0x1000);
  assert(f.entryCalls == 1 && f.heapCalls == 1 && f.playCalls == 1);
  assert(f.sounds == std::vector<uint32_t>{0x802f3c0c});
  assert(Fixture::word(&f, 0x2048) == 255 && f.mem[0x2046] == 0);
  Fixture other;
  assert(!resumeRecoveryEffectEntry(6, 1, {1, 2, 3}, other.services, c));
  assert(other.entryCalls == 0);
  for (int kind : {1, 2, 3, 4, 5, 7, 8}) {
    Fixture g;
    g.create(kind, 1);
    assert(g.mem[0x2001] == ((kind == 2 || kind == 3)   ? 0
                             : (kind == 4 || kind == 5) ? 20
                                                        : 1));
  }
}
void mainWaitsAndLifecycle() {
  Fixture f;
  f.create(1, 1);
  RecoveryMainCursor c;
  f.admitDisplay = false;
  assert(!resumeRecoveryEffectMain(0x1000, f.services, c));
  assert(c.phase == 112);
  assert(Fixture::half(&f, 0x202c) == 1 && f.calcCalls == 1);
  auto reads = f.mem;
  assert(!resumeRecoveryEffectMain(0x1000, f.services, c));
  assert(f.mem == reads && f.calcCalls == 1);
  f.admitDisplay = true;
  assert(resumeRecoveryEffectMain(0x1000, f.services, c));
  assert((f.displays == std::vector<uint32_t>{0x801422f4, 0x801420fc}));
  for (int i = 0; i < 9; ++i) {
    ++f.now;
    assert(resumeRecoveryEffectMain(0x1000, f.services, c));
  }
  assert(f.mem[0x2046] == 0 && Fixture::half(&f, 0x202c) == 10);
  ++f.now;
  assert(resumeRecoveryEffectMain(0x1000, f.services, c));
  assert(f.mem[0x2046] == 1);
  for (int i = 0; i < 10; ++i) {
    ++f.now;
    assert(resumeRecoveryEffectMain(0x1000, f.services, c));
  }
  assert(Fixture::half(&f, 0x2070) == 10 && f.mem[0x2046] == 1);
  ++f.now;
  assert(resumeRecoveryEffectMain(0x1000, f.services, c));
  assert(f.mem[0x2046] == 2);
  for (int i = 0; i < 10; ++i) {
    ++f.now;
    assert(resumeRecoveryEffectMain(0x1000, f.services, c));
  }
  assert(f.mem[0x2046] == 3);
  for (int i = 0; i < 40; ++i) {
    ++f.now;
    assert(resumeRecoveryEffectMain(0x1000, f.services, c));
  }
  assert(Fixture::half(&f, 0x2070) == 40);
  ++f.now;
  assert(resumeRecoveryEffectMain(0x1000, f.services, c));
  assert(f.mem[0x2046] == 4);
  ++f.now;
  assert(resumeRecoveryEffectMain(0x1000, f.services, c));
  assert(f.mem[0x2046] == 5);
  for (int i = 0; i < 16; ++i) {
    ++f.now;
    assert(resumeRecoveryEffectMain(0x1000, f.services, c));
  }
  assert(Fixture::word(&f, 0x2064) == bits(0));
  ++f.now;
  assert(resumeRecoveryEffectMain(0x1000, f.services, c));
  assert(f.mem[0x2046] == 6);
  f.admitChild = false;
  ++f.now;
  assert(!resumeRecoveryEffectMain(0x1000, f.services, c));
  assert(c.phase == 101);
  const auto rootCounter = Fixture::half(&f, 0x202c);
  const auto calc = f.calcCalls;
  assert(!resumeRecoveryEffectMain(0x1000, f.services, c));
  assert(Fixture::half(&f, 0x202c) == rootCounter && f.calcCalls == calc);
  Fixture::put(&f, 0x80422e34, bits(999)); // Held child call keeps scale30.
  f.admitChild = true;
  f.admitDelete = false;
  assert(!resumeRecoveryEffectMain(0x1000, f.services, c));
  assert(c.phase == 104);
  assert(f.mayaDeletes == 1 && f.sounds.size() == 2 &&
         f.sounds.back() == 0x802f3bb8);
  assert((f.childPosition == std::array<float, 3>{1, -18, 3}));
  f.admitDelete = true;
  assert(resumeRecoveryEffectMain(0x1000, f.services, c));
  assert(f.mayaDeletes == 1 && f.sounds.size() == 2);
  assert((f.order == std::vector<std::string>{"sfx", "child", "sfx",
                                              "mayaDelete", "delete"}));
}
void retainedOperandsAndIdentity() {
  Fixture f;
  RecoveryEntryCursor e;
  f.admitMaya = false;
  float nan;
  const uint32_t nanBits = 0x7fc01234;
  std::memcpy(&nan, &nanBits, 4);
  const std::array<float, 3> request{nan, 0.0f, 3};
  assert(!resumeRecoveryEffectEntry(0, 1, request, f.services, e));
  assert(!resumeRecoveryEffectEntry(0, 1, request, f.services, e));
  assert(f.mayaCalls == 2); // Same NaN payload is the same pending request.
  auto changed = request;
  changed[1] = -0.0f;
  assert(!resumeRecoveryEffectEntry(0, 1, changed, f.services, e));
  assert(f.mayaCalls == 2); // Different source operand bits reject reuse.
  auto other = f.services;
  other.byte = [](void *p, uint32_t a) { return Fixture::get(p).mem[a]; };
  assert(!resumeRecoveryEffectEntry(0, 1, request, other, e));
  assert(f.mayaCalls == 2);

  Fixture g;
  RecoveryEntryCursor allocation;
  g.services.heapAllocate = nullptr;
  assert(!resumeRecoveryEffectEntry(0, 1, {1, 2, 3}, g.services, allocation));
  assert(allocation.phase == 1 && allocation.heapBytes == 0x88);
  Fixture::put(&g, 0x1008, 99);
  g.services.heapAllocate = [](void *p, int32_t heap,
                               uint32_t bytes) -> std::optional<uint32_t> {
    assert(heap == 3 && bytes == 0x88);
    ++Fixture::get(p).heapCalls;
    return {}; // Capture proof without claiming an allocated destination.
  };
  assert(!resumeRecoveryEffectEntry(0, 1, {1, 2, 3}, g.services, allocation));
  assert(g.entryCalls == 1 && g.heapCalls == 1);

  Fixture h;
  h.create(0, 1);
  Fixture::pb(&h, 0x2046, 3);
  Fixture::ph(&h, 0x2070, 7);
  auto interpolate = h.services.interpolate;
  h.services.interpolate = nullptr;
  RecoveryMainCursor main;
  assert(!resumeRecoveryEffectMain(0x1000, h.services, main));
  assert(main.phase == 30 && main.counter == 8);
  Fixture::ph(&h, 0x2070, 90);
  Fixture::put(&h, 0x80422e24, bits(100));
  assert(!resumeRecoveryEffectMain(0x1000, h.services, main));
  assert(Fixture::half(&h, 0x2070) == 90 && h.calcCalls == 1);
  h.services.interpolate = interpolate;
  h.services.distance = nullptr;
  assert(!resumeRecoveryEffectMain(0x1000, h.services, main));
  assert(main.phase == 31 && main.angle == 72); // Original 0..360/count8/40.
  Fixture::put(&h, 0x204c, bits(999));
  const auto args = main.operands;
  assert(!resumeRecoveryEffectMain(0x1000, h.services, main));
  assert(main.operands == args); // Pending distance keeps original XYZ.
  auto badOwner = h.services;
  badOwner.byte = other.byte;
  assert(!resumeRecoveryEffectMain(0x1000, badOwner, main));
  assert(main.phase == 31 && h.calcCalls == 1);
}
void cumulativeSourceCompletion() {
  Fixture f;
  f.create(0, 2);
  Fixture::pb(&f, 0x2046, 6); // One complete; second remains unlaunched.
  f.now = 12;
  RecoveryMainCursor c;
  assert(resumeRecoveryEffectMain(0x1000, f.services, c));
  assert(f.deletes == 1 && Fixture::half(&f, 0x202c) ==
                               2); // r25 accumulates across elapsed iterations.
  Fixture g;
  g.create(7, 1);
  Fixture::pb(&g, 0x2046, 6);
  assert(resumeRecoveryEffectMain(0x1000, g.services, c));
  assert(g.deletes == 1 && g.childCalls == 0 && g.sounds.size() == 1);
  Fixture h;
  h.create(4, 1);
  assert(resumeRecoveryEffectMain(0x1000, h.services, c));
  assert((h.displays == std::vector<uint32_t>{0x801422f4, 0x80141c40}));
  assert(Fixture::word(&h, 0x2000 + 10 * 0x44 + 0xc) == bits(-15));
}
} // namespace
int main() {
  entryWaits();
  mainWaitsAndLifecycle();
  cumulativeSourceCompletion();
  retainedOperandsAndIdentity();
}
