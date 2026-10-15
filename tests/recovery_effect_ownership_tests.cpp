#include "effect_maya_animation.hpp"
#include "effect_maya_math_adapter.hpp"
#include "effect_maya_matrix.hpp"
#include "effect_pool.hpp"
#include "recovery_effect.hpp"
#include "recovery_effect_resources.hpp"
#include <cassert>
#include <cstring>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
struct Owner {
  std::map<uint32_t, uint8_t> memory;
  RecoveryEffectServices recovery;
  EffectMayaAnimationServices maya;
  EffectMayaMatrixServices curve;
  EffectMayaMatrixCursor curveCall;
  EffectPoolServices pool;
  EffectMainCursor driver;
  RecoveryEntryCursor entry;
  RecoveryMainCursor main;
  EffectMayaAnimationCursor mayaCall;
  uint32_t next = 0x200000, effect = 0x100000, animation = 0;
  unsigned effectAllocations = 0, heapAllocations = 0, clears = 0, sounds = 0,
           matrices = 0, tickReads = 0;
  bool matrixReady = false;
  std::vector<uint32_t> draws;
  static Owner &self(void *p) { return *static_cast<Owner *>(p); }
  uint8_t byte(uint32_t a) {
    const auto it = memory.find(a);
    assert(it != memory.end());
    return it->second;
  }
  static uint32_t word(void *p, uint32_t a) {
    auto &o = self(p);
    uint32_t v = 0;
    for (unsigned i = 0; i < 4; ++i)
      v = (v << 8) | o.byte(a + i);
    return v;
  }
  static uint16_t half(void *p, uint32_t a) {
    return uint16_t((uint16_t(self(p).byte(a)) << 8) | self(p).byte(a + 1));
  }
  static void storeByte(void *p, uint32_t a, uint8_t v) {
    auto &o = self(p);
    assert(o.memory.count(a));
    o.memory[a] = v;
  }
  static void storeHalf(void *p, uint32_t a, uint16_t v) {
    storeByte(p, a, uint8_t(v >> 8));
    storeByte(p, a + 1, uint8_t(v));
  }
  static void storeWord(void *p, uint32_t a, uint32_t v) {
    for (unsigned i = 0; i < 4; ++i)
      storeByte(p, a + i, uint8_t(v >> (24 - 8 * i)));
  }
  void region(uint32_t a, uint32_t n) {
    for (unsigned i = 0; i < n; ++i) {
      assert(!memory.count(a + i));
      memory[a + i] = 0;
    }
  }
  float scalar(uint32_t a) {
    uint32_t v = word(this, a);
    float f;
    std::memcpy(&f, &v, 4);
    return f;
  }
  Owner() {
    RecoveryEffectResources resource;
    assert(resource.load("romfs/recovery-effect.bin"));
    for (const auto &span : resource.spans())
      for (unsigned i = 0; i < span.bytes.size(); ++i) {
        assert(!memory.count(span.address + i));
        memory[span.address + i] = span.bytes[i];
      }
    region(effect, 0x28);
    region(0x80415fd0, 4);
    region(0x800000f8, 4);
    region(0x110000, 0x200);
    region(0x80415ee0, 4);
    region(0x120000, 0x18);
    storeWord(this, 0x80415ee0, 0x120000);
    storeWord(this, 0x120000, 1);
    storeWord(this, 0x120004, effect);
    storeWord(this, 0x80415fd0, 0x110000);
    storeWord(this, 0x800000f8, 162000000);
    storeWord(this, 0x110004, 60);
    storeWord(this, 0x110014, 0);
    storeWord(this, 0x110044, 40500000);
    storeWord(this, 0x11001c, 10);
    auto &m = maya.memory;
    m.user = this;
    m.word = word;
    m.half = half;
    m.storeWord = storeWord;
    m.storeHalf = storeHalf;
    m.storeByte = storeByte;
    maya.user = this;
    maya.divideUnsigned = effectMayaDivideUnsigned;
    maya.convertUnsigned = effectMayaConvertUnsigned;
    maya.allocate = [](void *p, int32_t heap, uint32_t n, uint32_t &out) {
      auto &o = self(p);
      assert(heap == 3 && n == 0x4c);
      out = o.next;
      o.next += 0x10000;
      o.region(out, n);
      ++o.heapAllocations;
      return true;
    };
    maya.clear = [](void *p, uint32_t a, uint32_t n) {
      auto &o = self(p);
      ++o.clears;
      for (unsigned i = 0; i < n; ++i)
        storeByte(p, a + i, 0);
      return true;
    };
    curve.memory = m;
    curve.user = this;
    curve.byte = [](void *p, uint32_t a) { return self(p).byte(a); };
    bindEffectMayaMatrixMathAdapter(curve);
    pool.memory = m;
    pool.user = this;
    pool.update = [](void *p, uint32_t callback, uint32_t e) {
      auto &o = self(p);
      assert(callback == 0x80142bc4 && e == o.effect);
      return resumeRecoveryEffectMain(e, o.recovery, o.main);
    };
    maya.matrix = [](void *p, uint32_t a, uint32_t channel, uint32_t out,
                     uint32_t index) {
      auto &o = self(p);
      ++o.matrices;
      assert(a == o.animation && channel == 0x80417a2c && out == a + 0x14 &&
             index == a + 0x44);
      if (!o.matrixReady)
        return false;
      return resumeEffectMayaAnimMatrix(a, channel, out, index, o.curve,
                                        o.curveCall);
    };
    auto &s = recovery;
    s.memory = m;
    s.user = this;
    s.byte = [](void *p, uint32_t a) { return self(p).byte(a); };
    s.effectEntry = [](void *p) -> std::optional<uint32_t> {
      auto &o = self(p);
      ++o.effectAllocations;
      return effectPoolEntry(o.pool);
    };
    s.heapAllocate = [](void *p, int32_t heap,
                        uint32_t n) -> std::optional<uint32_t> {
      auto &o = self(p);
      assert(heap == 3 && n == 0x88);
      auto a = o.next;
      o.next += 0x10000;
      o.region(a, n);
      ++o.heapAllocations;
      return a;
    };
    s.mayaAllocate = [](void *p, uint32_t d) -> std::optional<uint32_t> {
      auto &o = self(p);
      uint32_t a;
      if (!resumeEffectMayaAnimAllocate(d, o.maya, o.mayaCall, a))
        return {};
      o.animation = a;
      return a;
    };
    s.mayaPlay = [](void *p, uint32_t a) {
      auto &o = self(p);
      return resumeEffectMayaAnimPlay(a, o.maya, o.mayaCall);
    };
    s.mayaCalc = [](void *p, uint32_t a) {
      auto &o = self(p);
      return resumeEffectMayaAnimCalculate(a, o.maya, o.mayaCall);
    };
    s.tick = [](void *p) {
      auto &o = self(p);
      ++o.tickReads;
      return word(p, word(p, 0x80415fd0) + 0x1c);
    };
    s.binary = [](void *, RecoveryBinary op, float a, float b) {
      switch (op) {
      case RecoveryBinary::Add:
        return a + b;
      case RecoveryBinary::Subtract:
        return a - b;
      case RecoveryBinary::Multiply:
        return a * b;
      case RecoveryBinary::Divide:
        return a / b;
      }
      assert(false);
      return 0.f;
    };
    s.sfx = [](void *p, uint32_t sound, const std::array<float, 3> &xyz) {
      auto &o = self(p);
      ++o.sounds;
      assert(sound == 0x802f3c0c && xyz[0] == 2 && xyz[1] == 3 && xyz[2] == 4);
      return true;
    };
    s.depth = [](void *, const std::array<float, 3> &) -> std::optional<float> {
      return 7.f;
    };
    s.display = [](void *p, int32_t camera, int32_t type, uint32_t callback,
                   uint32_t e, float z) {
      auto &o = self(p);
      assert(camera == 4 && e == o.effect && z == 7.f &&
             type == int32_t(o.draws.size() + 1));
      o.draws.push_back(callback);
      return true;
    };
  }
};
} // namespace
int main() {
  Owner o;
  const auto effect =
      resumeRecoveryEffectEntry(0, 1, {2, 3, 4}, o.recovery, o.entry);
  assert(effect == o.effect && !o.entry.active && o.effectAllocations == 1 &&
         o.heapAllocations == 2 && o.clears == 1 && o.sounds == 1);
  const auto payload = Owner::word(&o, o.effect + 0xc);
  assert(Owner::word(&o, o.effect + 0x10) == 0x80142bc4 &&
         Owner::word(&o, o.animation + 0x10) == 0x80417a44 &&
         Owner::word(&o, o.animation + 0xc) == 1000 &&
         Owner::half(&o, o.animation) == 1);
  const unsigned ticks = o.tickReads;
  assert(!resumeEffectMain(o.pool, o.driver));
  assert(o.main.active && o.mayaCall.operation == 3 && o.matrices == 1 &&
         o.tickReads == ticks && o.draws.empty());
  assert(Owner::word(&o, payload + 0x2c) == 0 &&
         o.scalar(o.animation + 4) == 0.f);
  // A live clock update while the matrix owner is unavailable cannot replay
  // Calculate's acknowledged elapsed conversion or recovery's constructor.
  Owner::storeWord(&o, 0x110044, 405000000);
  Owner::storeWord(&o, 0x11001c, 11);
  assert(!resumeEffectMain(o.pool, o.driver) && o.matrices == 2 &&
         o.heapAllocations == 2 && o.sounds == 1 &&
         o.scalar(o.animation + 4) == 0.f);
  o.matrixReady = true;
  assert(resumeEffectMain(o.pool, o.driver) && !o.main.active &&
         !o.mayaCall.operation);
  assert(o.matrices == 3 && o.heapAllocations == 2 && o.sounds == 1 &&
         Owner::half(&o, payload + 0x2c) == 1);
  assert(o.draws == std::vector<uint32_t>({0x801422f4, 0x801420fc}));
  assert(!o.driver.active && !o.curveCall.active &&
         Owner::word(&o, o.effect) == 1);
  // Actual source key0: zero scale about -1.484375 pivot, translated Y5.
  assert(o.scalar(o.animation + 0x14) == 0 &&
         o.scalar(o.animation + 0x14 + 20) == 0 &&
         o.scalar(o.animation + 0x14 + 40) == 0);
  assert(o.scalar(o.animation + 0x14 + 28) == 3.515625f &&
         o.scalar(o.animation + 0x44) == 0);
  // 150ms at signed GP frequency60 reaches source key9, whose original
  // rotation/translation are zero and scale is identity. No matrix stub
  // remains.
  Owner::storeWord(&o, 0x110044, 46575000);
  assert(resumeEffectMayaAnimCalculate(o.animation, o.maya, o.mayaCall));
  assert(o.scalar(o.animation + 4) == 9 && o.scalar(o.animation + 0x14) == 1 &&
         o.scalar(o.animation + 0x14 + 20) == 1 &&
         o.scalar(o.animation + 0x14 + 40) == 1);
  assert(o.scalar(o.animation + 0x14 + 12) == 0 &&
         o.scalar(o.animation + 0x14 + 28) == 0 &&
         o.scalar(o.animation + 0x14 + 44) == 0);
  Owner::storeWord(&o, 0x110044,
                   93960000); // Elapsed1320ms→79.2; original clamps79.
  assert(resumeEffectMayaAnimCalculate(o.animation, o.maya, o.mayaCall));
  assert(o.scalar(o.animation + 4) == 79 && Owner::half(&o, o.animation) == 3 &&
         o.scalar(o.animation + 0x14 + 20) == 1.5f &&
         o.scalar(o.animation + 0x14 + 28) == 8.7421875f);
}
