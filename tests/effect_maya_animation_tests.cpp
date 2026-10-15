#include "effect_maya_animation.hpp"
#include <cassert>
#include <cstring>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
struct Fixture {
  std::map<uint32_t, uint32_t> words;
  std::map<uint32_t, uint16_t> halves;
  std::vector<uint32_t> reads, stores, channels, matrices, indices;
  int allocs = 0, clears = 0, divides = 0, converts = 0, frees = 0;
  uint32_t blockedChannel = 0;
  bool clearReady = false, divideReady = true, convertReady = true,
       matrixReady = false, freeReady = false;
  static uint32_t word(void *u, uint32_t a) {
    auto &x = *static_cast<Fixture *>(u);
    x.reads.push_back(a);
    return x.words[a];
  }
  static uint16_t half(void *u, uint32_t a) {
    return static_cast<Fixture *>(u)->halves[a];
  }
  static void store(void *u, uint32_t a, uint32_t v) {
    auto &x = *static_cast<Fixture *>(u);
    x.stores.push_back(a);
    x.words[a] = v;
  }
  static void storeHalf(void *u, uint32_t a, uint16_t v) {
    static_cast<Fixture *>(u)->halves[a] = v;
  }
  void real(uint32_t a, float v) {
    uint32_t b;
    std::memcpy(&b, &v, 4);
    words[a] = b;
  }
  float real(uint32_t a) {
    float v;
    uint32_t b = words[a];
    std::memcpy(&v, &b, 4);
    return v;
  }
  EffectMayaAnimationServices services() {
    EffectMayaAnimationServices s;
    s.memory.user = this;
    s.memory.word = word;
    s.memory.half = half;
    s.memory.storeWord = store;
    s.memory.storeHalf = storeHalf;
    s.user = this;
    s.allocate = [](void *u, int32_t heap, uint32_t bytes, uint32_t &out) {
      auto &x = *static_cast<Fixture *>(u);
      assert(heap == 3 && bytes == 0x80);
      ++x.allocs;
      out = 0x500;
      return true;
    };
    s.clear = [](void *u, uint32_t p, uint32_t bytes) {
      auto &x = *static_cast<Fixture *>(u);
      ++x.clears;
      assert(p == 0x500 && bytes == 0x80);
      return x.clearReady;
    };
    s.release = [](void *u, int32_t heap, uint32_t p) {
      auto &x = *static_cast<Fixture *>(u);
      assert(heap == 3 && p == 0x500);
      ++x.frees;
      return x.freeReady;
    };
    s.divideUnsigned = [](void *u, uint64_t n, uint64_t d, uint64_t &out) {
      auto &x = *static_cast<Fixture *>(u);
      ++x.divides;
      if (!x.divideReady)
        return false;
      return effectMayaDivideUnsigned(nullptr, n, d, out);
    };
    s.convertUnsigned = [](void *u, uint64_t n, float &out) {
      auto &x = *static_cast<Fixture *>(u);
      ++x.converts;
      if (!x.convertReady)
        return false;
      return effectMayaConvertUnsigned(nullptr, n, out);
    };
    s.matrix = [](void *u, uint32_t a, uint32_t ch, uint32_t m, uint32_t i) {
      auto &x = *static_cast<Fixture *>(u);
      assert(a == 0x500);
      x.channels.push_back(ch);
      x.matrices.push_back(m);
      x.indices.push_back(i);
      return x.matrixReady && ch != x.blockedChannel;
    };
    return s;
  }
  Fixture() {
    words[0x108] = 0x200;
    words[0x200] = 2;
    words[0x204] = 0x300;
    words[0x80415fd0] = 0x1000;
    words[0x800000f8] = 4000000;
    words[0x1004] = 60;
    words[0x1040] = 0;
    words[0x1044] = 100000;
    words[0x1038] = 0;
    words[0x103c] = 500000;
    real(0x8042019c, 1000);
    real(0x804201a0, 0);
    words[0x802c1f18] = 0x43300000;
    words[0x802c1f1c] = 0x80000000;
    real(0x104, 79);
  }
};
void lifecycle() {
  Fixture x;
  auto s = x.services();
  EffectMayaAnimationCursor c;
  uint32_t result = 0;
  assert(!resumeEffectMayaAnimAllocate(0x100, s, c, result));
  assert(x.allocs == 1 && x.words[0x510] == 0);
  auto reads = x.reads;
  assert(!resumeEffectMayaAnimAllocate(0x100, s, c, result));
  assert(x.allocs == 1 && reads == x.reads);
  auto wrong = s;
  wrong.user = nullptr;
  assert(!resumeEffectMayaAnimAllocate(0x100, wrong, c, result));
  assert(!resumeEffectMayaAnimPlay(0x500, s, c));
  x.clearReady = true;
  assert(resumeEffectMayaAnimAllocate(0x100, s, c, result));
  assert(result == 0x500 && x.words[0x510] == 0x100);
  x.divideReady = false;
  assert(!resumeEffectMayaAnimPlay(result, s, c));
  reads = x.reads;
  x.words[0x1000 + 0x40] = 3;
  x.divideReady = true;
  assert(resumeEffectMayaAnimPlay(result, s, c));
  assert(x.words[0x50c] == 100 && x.words[0x508] == 0 && x.halves[0x500] == 1);
  assert(x.real(0x504) == 0);
  // Capture the selected branch, then retain subtraction/conversion and matrix
  // boundaries.
  x.words[0x1014] = 1;
  x.words[0x103c] = 1100000;
  x.convertReady = false;
  assert(!resumeEffectMayaAnimCalculate(result, s, c));
  assert(c.phase == 2);
  reads = x.reads;
  auto stores = x.stores;
  x.words[0x103c] = 0;
  x.words[0x50c] = 777;
  assert(!resumeEffectMayaAnimCalculate(result, s, c));
  assert(x.reads == reads && x.stores == stores);
  x.convertReady = true;
  assert(!resumeEffectMayaAnimCalculate(result, s, c));
  assert(c.phase == 5 && x.real(0x504) == 60);
  reads = x.reads;
  stores = x.stores;
  x.words[0x200] = 1;
  x.words[0x204] = 0x999;
  assert(!resumeEffectMayaAnimCalculate(result, s, c));
  assert(x.reads == reads && x.stores == stores);
  assert(x.channels.back() == 0x300);
  x.matrixReady = true;
  assert(resumeEffectMayaAnimCalculate(result, s, c));
  assert(x.channels.back() == 0x300 && x.matrices.back() == 0x514 &&
         x.indices.back() == 0x544);
  // Fresh loop condition observes count1; channel0 was not replayed after
  // success.
  x.words[0x200] = 2;
  x.words[0x204] = 0x300;
  x.words[0x50c] = 0;
  x.words[0x508] = 0;
  x.words[0x103c] = 2000000;
  assert(resumeEffectMayaAnimCalculate(result, s, c));
  assert(x.real(0x504) == 79 && x.halves[0x500] == 3);
  assert(x.channels.back() == 0x310 && x.matrices.back() == 0x548 &&
         x.indices.back() == 0x578);
  reads = x.reads;
  assert(resumeEffectMayaAnimCalculate(result, s, c));
  assert(x.reads == reads);
  assert(!resumeEffectMayaAnimDelete(result, s, c));
  reads = x.reads;
  x.freeReady = true;
  assert(resumeEffectMayaAnimDelete(result, s, c));
  assert(x.reads == reads && x.frees == 2);
}
void channelRetention() {
  Fixture x;
  auto s = x.services();
  EffectMayaAnimationCursor c;
  x.words[0x510] = 0x100;
  x.halves[0x500] = 1;
  x.words[0x1044] = 1000;
  x.matrixReady = true;
  x.blockedChannel = 0x310;
  assert(!resumeEffectMayaAnimCalculate(0x500, s, c));
  assert(c.index == 1 && c.phase == 5);
  assert(x.channels.size() == 2);
  auto reads = x.reads;
  auto stores = x.stores;
  x.words[0x200] = 0;
  x.words[0x204] = 0x999;
  assert(!resumeEffectMayaAnimCalculate(0x500, s, c));
  assert(reads == x.reads && stores == x.stores && x.channels.back() == 0x310);
  x.blockedChannel = 0;
  assert(resumeEffectMayaAnimCalculate(0x500, s, c));
  assert(x.channels.size() == 4 && x.channels.front() == 0x300 &&
         x.channels.back() == 0x310);
}
void numeric() {
  uint64_t out;
  float value;
  assert(effectMayaDivideUnsigned(nullptr, 0xffffffffffffffffull, 1000, out) &&
         out == 18446744073709551ull);
  assert(!effectMayaDivideUnsigned(nullptr, 1, 0, out));
  assert(effectMayaConvertUnsigned(nullptr, 0xffffffffffffffffull, value) &&
         value == 18446744073709551616.0f);
  // Original rounds binary64 first: midpoint+1 is lost there, then ties to
  // even.
  assert(effectMayaConvertUnsigned(
             nullptr, (uint64_t(1) << 63) + (uint64_t(1) << 39) + 1, value) &&
         value == 9223372036854775808.0f);
  Fixture x;
  auto s = x.services();
  EffectMayaAnimationCursor c;
  x.words[0x510] = 0x100;
  x.halves[0x500] = 1;
  x.words[0x1004] = 0xffffffff;
  x.words[0x1044] = 1000;
  x.words[0x200] = 0;
  assert(resumeEffectMayaAnimCalculate(0x500, s, c));
  assert(x.real(0x504) == -0.001f);
  x.halves[0x500] = 0;
  auto reads = x.reads;
  assert(resumeEffectMayaAnimCalculate(0x500, s, c));
  assert(x.reads == reads);
}
} // namespace
int main() {
  lifecycle();
  channelRetention();
  numeric();
}
