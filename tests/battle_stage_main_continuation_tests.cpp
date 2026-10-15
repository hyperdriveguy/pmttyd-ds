#include "battle_stage_main_continuation.hpp"
#include <cassert>
#include <cstring>
#include <map>
#include <vector>
using namespace ttyd;
struct F {
  static constexpr uint32_t B = 0x1000, W = B + 0x163fc, R = B + 0x1715c;
  std::map<uint32_t, uint32_t> mem;
  std::vector<std::array<uint32_t, 3>> trace;
  unsigned calls = 0, block = 0;
  bool reenter = false, mutate = false;
  BattleStageObjectCursor *oc = nullptr;
  BattleStageWorkCursor *wc = nullptr;
  static uint32_t bits(float f) {
    uint32_t n;
    std::memcpy(&n, &f, 4);
    return n;
  }
  F() {
    mem[0x8041eb90] = B;
    for (unsigned i = 0; i < 3; ++i) {
      auto r = R + 0x7c * i;
      mem[r] = 1;
      mem[r + 0x64] = 0x9000 + 8 * i;
      mem[0x9000 + 8 * i] = 100 + i;
      mem[0x9006 + 8 * i] = i == 0 ? 2 : i == 1 ? 3 : 1;
      mem[r + 0x68] = 3;
      for (unsigned a = 0; a < 12; ++a)
        mem[r + 4 + 4 * a] = bits(float(a + 1));
    }
    for (unsigned i = 0; i < 4; ++i) {
      const unsigned count[] = {0xaf0, 0xaf4, 0xb10, 0xb14};
      const unsigned dur[] = {0xaf8, 0xafc, 0xb18, 0xb1c};
      mem[W + count[i]] = 5;
      mem[W + dur[i]] = 10;
      const unsigned start[] = {0xaea, 0xaeb, 0xb0a, 0xb0b};
      const unsigned end[] = {0xaec, 0xaed, 0xb0c, 0xb0d};
      const unsigned mode[] = {0xb00, 0xb01, 0xb20, 0xb21};
      mem[W + start[i]] = 10 + i;
      mem[W + end[i]] = 30 + i;
      mem[W + mode[i]] = 255;
    }
    mem[W + 4] = 1;
    mem[W + 4 + 0xe8 + 0xe4] = 0x7000;
  }
  BattleActorServices actors() {
    BattleActorServices a;
    a.user = this;
    a.memory.user = this;
    a.memory.word = [](void *p, uint32_t x) {
      auto &f = *static_cast<F *>(p);
      auto v = f.mem[x];
      f.trace.push_back({0, x, v});
      return v;
    };
    a.memory.half = [](void *p, uint32_t x) {
      auto &f = *static_cast<F *>(p);
      auto v = uint16_t(f.mem[x]);
      f.trace.push_back({1, x, v});
      return v;
    };
    a.byte = [](void *p, uint32_t x) {
      auto &f = *static_cast<F *>(p);
      auto v = uint8_t(f.mem[x]);
      f.trace.push_back({2, x, v});
      return v;
    };
    a.memory.storeWord = [](void *p, uint32_t x, uint32_t v) {
      auto &f = *static_cast<F *>(p);
      f.mem[x] = v;
      f.trace.push_back({3, x, v});
    };
    a.memory.storeByte = [](void *p, uint32_t x, uint8_t v) {
      auto &f = *static_cast<F *>(p);
      f.mem[x] = v;
      f.trace.push_back({4, x, v});
    };
    return a;
  }
  bool call(uint32_t tag, uint32_t arg, uint32_t value = 0) {
    if (reenter) {
      if (oc) {
        assert(!resumePackedBattleStageObjectMain(objects(), *oc));
        assert(!oc->reset());
      }
      if (wc) {
        assert(!resumePackedBattleStageWorkMain(work(), *wc));
        assert(!wc->reset());
      }
    }
    if (++calls == block)
      return false;
    trace.push_back({tag, arg, value});
    if (mutate) {
      mem[0x8041eb90] = 0x4000;
      mem[0x9000] += 1;
      mem[R + 0x14] = bits(float(trace.size()));
      mem[R + 8] = bits(float(trace.size() + 1));
    }
    return true;
  }
  BattleStageObjectServices objects() {
    BattleStageObjectServices s;
    s.actors = actors();
    s.user = this;
    s.translate = [](void *p, uint32_t n,
                     const BattleStageObjectServices::Point &v) {
      auto &f = *static_cast<F *>(p);
      return f.call(5, n, bits(v[1]));
    };
    s.rotate = [](void *p, uint32_t n,
                  const BattleStageObjectServices::Point &v) {
      auto &f = *static_cast<F *>(p);
      return f.call(6, n, bits(v[1]));
    };
    s.groupFlags = [](void *p, uint32_t n) -> std::optional<uint32_t> {
      auto &f = *static_cast<F *>(p);
      if (!f.call(7, n))
        return {};
      return 0;
    };
    s.groupFlag = [](void *p, uint32_t n, uint32_t, bool b) {
      return static_cast<F *>(p)->call(8, n, b);
    };
    return s;
  }
  BattleStageWorkServices work() {
    BattleStageWorkServices s;
    s.actors = actors();
    s.user = this;
    s.spotlightMain = [](void *p) { return static_cast<F *>(p)->call(9, 0); };
    s.activeEntry = [](void *p, uint32_t e) {
      return static_cast<F *>(p)->call(10, e);
    };
    s.freeScratch = [](void *p, uint32_t e) {
      return static_cast<F *>(p)->call(11, e);
    };
    s.interpolate = [](void *p, int32_t mode, int32_t elapsed, int32_t duration,
                       float start, float end) -> std::optional<float> {
      auto &f = *static_cast<F *>(p);
      if (!f.call(12, uint32_t(mode), uint32_t(elapsed)))
        return {};
      return start + (end - start) * float(elapsed) / float(duration);
    };
    s.truncate = [](void *p, float n) -> std::optional<int32_t> {
      auto &f = *static_cast<F *>(p);
      if (!f.call(13, bits(n)))
        return {};
      return int32_t(n);
    };
    return s;
  }
};
int main() {
  for (bool mutations : {false, true}) {
    F ref;
    ref.mutate = mutations;
    assert(packedBattleStageObjectMain(ref.objects()));
    const unsigned count = ref.calls;
    for (unsigned hold = 0; hold <= count; ++hold) {
      F f;
      f.mutate = mutations;
      f.block = hold;
      BattleStageObjectCursor c;
      if (hold) {
        assert(!resumePackedBattleStageObjectMain(f.objects(), c));
        auto prefix = f.trace;
        auto memory = f.mem;
        f.block = f.calls + 1;
        assert(!resumePackedBattleStageObjectMain(f.objects(), c));
        assert(f.trace == prefix && f.mem == memory);
        auto wrong = f.objects();
        wrong.actors.audienceLifecycle.user = &ref;
        assert(!resumePackedBattleStageObjectMain(wrong, c));
        assert(f.trace == prefix);
      }
      f.block = 0;
      assert(resumePackedBattleStageObjectMain(f.objects(), c));
      assert(!c.active());
      assert(f.trace == ref.trace && f.mem == ref.mem);
    }
  }
  F wr;
  assert(packedBattleStageWorkMain(wr.work()));
  for (unsigned hold = 0; hold <= wr.calls; ++hold) {
    F f;
    f.block = hold;
    BattleStageWorkCursor c;
    if (hold) {
      assert(!resumePackedBattleStageWorkMain(f.work(), c));
      auto prefix = f.trace;
      auto memory = f.mem;
      f.block = f.calls + 1;
      assert(!resumePackedBattleStageWorkMain(f.work(), c));
      assert(f.trace == prefix && f.mem == memory);
      auto wrong = f.work();
      wrong.actors.audienceLifecycle.user = &wr;
      assert(!resumePackedBattleStageWorkMain(wrong, c));
      assert(f.trace == prefix);
    }
    f.block = 0;
    assert(resumePackedBattleStageWorkMain(f.work(), c));
    assert(!c.active());
    assert(f.trace == wr.trace && f.mem == wr.mem);
  }

  // A held interpolation retains its original operands after its decrement.
  F held;
  held.block = 4;
  BattleStageWorkCursor hc;
  assert(!resumePackedBattleStageWorkMain(held.work(), hc));
  assert(held.mem[F::W + 0xaf0] == 4);
  held.mem[F::W + 0xaf0] = 99;
  held.mem[F::W + 0xaf8] = 1;
  held.mem[F::W + 0xaea] = 200;
  held.mem[F::W + 0xaec] = 220;
  held.block = 0;
  assert(resumePackedBattleStageWorkMain(held.work(), hc));
  assert(held.mem[F::W + 0xaf0] == 99 && held.mem[F::W + 0xae8] == 22);
  // Missing numeric provider still retains the source's committed decrement.
  F absent;
  auto missing = absent.work();
  missing.interpolate = nullptr;
  BattleStageWorkCursor ac;
  assert(!resumePackedBattleStageWorkMain(missing, ac));
  auto prefix = absent.trace;
  assert(!resumePackedBattleStageWorkMain(missing, ac) &&
         absent.trace == prefix);
  assert(absent.mem[F::W + 0xaf0] == 4);
  // Changed bindings cannot turn a held missing service into a different call.
  assert(!resumePackedBattleStageWorkMain(absent.work(), ac) &&
         absent.trace == prefix);
  F o;
  o.reenter = true;
  BattleStageObjectCursor oc;
  o.oc = &oc;
  assert(resumePackedBattleStageObjectMain(o.objects(), oc));
  F w;
  w.reenter = true;
  BattleStageWorkCursor wc;
  w.wc = &wc;
  assert(resumePackedBattleStageWorkMain(w.work(), wc));
  return 0;
}
