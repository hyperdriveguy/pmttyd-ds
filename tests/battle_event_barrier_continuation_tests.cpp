#include "battle_attack_selection.hpp"
#include "battle_conclusion.hpp"
#include "battle_event_barrier_continuation.hpp"
#include <cassert>
#include <map>
#include <vector>
using namespace ttyd;
struct F {
  static constexpr uint32_t B = 0x1000, C = 0x40000, U = 0x80000, V = 0x81000,
                            E = 0x90000;
  std::map<uint32_t, uint32_t> mem;
  std::vector<std::array<uint32_t, 4>> trace;
  unsigned accepted = 0, block = 0;
  bool reentry = false, conclusion = false, allocationFailure = false,
       swapConsume = false;
  BattleEventBarrierCursor *cursor = nullptr;
  F() {
    mem[0x8041eb90] = B;
    for (auto b : {B, C})
      for (unsigned i = 0; i < 64; ++i) {
        mem[b + 0x16f5c + 8 * i] = 0xffffffff;
        mem[b + 0x16f60 + 8 * i] = 0;
      }
    mem[B + 0x20] = U;
    mem[B + 0x24] = V;
    mem[U] = 0;
    mem[V] = 1;
    mem[U + 0x18] = 0xa0000;
    mem[0xa0000] = 7;
    mem[0xa0004] = 0xb0000;
    mem[0xa0008] = 0;
    mem[E + 0x15c] = 123;
  }
  BattleActorServices actors() {
    BattleActorServices a;
    a.memory.user = this;
    a.memory.word = [](void *p, uint32_t x) -> uint32_t {
      auto &f = *static_cast<F *>(p);
      const auto v = f.mem[x];
      f.trace.push_back({0, x, v, 0});
      return v;
    };
    a.memory.storeWord = [](void *p, uint32_t x, uint32_t v) {
      auto &f = *static_cast<F *>(p);
      f.mem[x] = v;
      f.trace.push_back({1, x, v, 0});
    };
    return a;
  }
  bool effect(uint32_t kind, uint32_t a, uint32_t b = 0, uint32_t c = 0) {
    if (reentry) {
      assert(cursor);
      assert(!resumeBattleAllEventBarrier(B, services(), *cursor));
      assert(!cursor->reset());
    }
    if (block && accepted + 1 == block)
      return false;
    ++accepted;
    trace.push_back({kind, a, b, c});
    return true;
  }
  BattleEventBarrierServices services() {
    BattleEventBarrierServices s;
    s.actors = actors();
    s.user = this;
    s.eventExists = [](void *p, uint32_t id) -> std::optional<int32_t> {
      auto &f = *static_cast<F *>(p);
      if (!f.effect(2, id))
        return {};
      return id == 77 ? 1 : 0;
    };
    s.removeItem = [](void *p, uint32_t item) -> std::optional<int32_t> {
      auto &f = *static_cast<F *>(p);
      if (!f.effect(3, item))
        return {};
      if (f.swapConsume)
        f.mem[0x8041eb90] = C;
      return 0;
    };
    s.concluded = [](void *p, uint32_t b) -> std::optional<int32_t> {
      auto &f = *static_cast<F *>(p);
      if (!f.effect(4, b))
        return {};
      return f.conclusion ? 1 : 0;
    };
    s.create = [](void *p, uint32_t script, int32_t priority,
                  uint32_t flags) -> std::optional<uint32_t> {
      auto &f = *static_cast<F *>(p);
      assert(priority == 10 && flags == 0);
      if (!f.effect(5, script, uint32_t(priority), flags))
        return {};
      return f.allocationFailure ? uint32_t(0) : E;
    };
    return s;
  }
  void queue(unsigned i, int32_t id, uint32_t key) {
    mem[B + 0x16f5c + 8 * i] = uint32_t(id);
    mem[B + 0x16f60 + 8 * i] = key;
  }
  int32_t immediateReaction() {
    auto s = services();
    const auto b = currentBattleAddress(s.actors);
    auto &m = s.actors.memory;
    for (unsigned i = 0; i < 64; ++i) {
      const auto r = b + 0x16f5c + 8 * i;
      const auto raw = m.word(m.user, r);
      if (raw == 0xffffffff)
        continue;
      const auto key = m.word(m.user, r + 4);
      const auto unit = battleRegistryUnit(b, int32_t(raw), s.actors);
      if (!unit) {
        releasePackedBattleAfterReaction(int32_t(raw), key, s.actors);
        return immediateReaction();
      }
      const auto consumer = currentBattleAddress(s.actors);
      for (unsigned j = 0; j < 4; ++j) {
        const auto at = consumer + 0x19060 + 4 * j, item = m.word(m.user, at);
        if (item)
          assert(s.removeItem(s.user, item).has_value());
        m.storeWord(m.user, at, 0);
      }
      const auto result = s.concluded(s.user, b);
      assert(result);
      if (*result)
        return 0;
      releasePackedBattleAfterReaction(int32_t(raw), key, s.actors);
      const auto script = packedUnitData(unit, key, s.actors);
      const auto event = s.create(s.user, script, 10, 0);
      assert(event);
      if (*event) {
        m.storeWord(m.user, unit + 0x2a8, m.word(m.user, *event + 0x15c));
        m.storeWord(m.user, *event + 0x160, m.word(m.user, unit));
      }
      return 1;
    }
    return 0;
  }
  int32_t immediate() {
    BattleConclusionServices s;
    s.actors = actors();
    s.user = this;
    s.eventExists = [](void *p, uint32_t id) {
      auto &f = *static_cast<F *>(p);
      return *f.services().eventExists(p, id) != 0;
    };
    s.afterReaction = [](void *p) {
      return static_cast<F *>(p)->immediateReaction();
    };
    return battleAllEventBarrier(B, s);
  }
  void prepare() {
    mem[U + 0x29c] = 10;
    mem[V + 0x2bc] = 11;
    queue(0, 9, 4);
    queue(1, 0, 7);
    mem[B + 0x19060] = 121;
    mem[B + 0x19064] = 121;
    mem[B + 0x182b4] = 20;
  }
};
int main() {
  for (bool concluded : {false, true})
    for (bool fail : {false, true}) {
      F oracle;
      oracle.prepare();
      oracle.conclusion = concluded;
      oracle.allocationFailure = fail;
      const auto expected = oracle.immediate();
      const auto count = oracle.accepted;
      for (unsigned hold = 0; hold <= count; ++hold) {
        F f;
        f.prepare();
        f.conclusion = concluded;
        f.allocationFailure = fail;
        f.block = hold;
        BattleEventBarrierCursor c;
        if (hold) {
          assert(!resumeBattleAllEventBarrier(F::B, f.services(), c));
          const auto prefix = f.trace;
          const auto mem = f.mem;
          assert(!resumeBattleAllEventBarrier(F::B, f.services(), c) &&
                 f.trace == prefix && f.mem == mem);
          auto wrong = f.services();
          wrong.actors.audienceLifecycle.user = &oracle;
          assert(!resumeBattleAllEventBarrier(F::B, wrong, c) &&
                 f.trace == prefix);
          assert(!resumeBattleUnitEventBarrier(F::B, f.services(), c));
        }
        f.block = 0;
        assert(resumeBattleAllEventBarrier(F::B, f.services(), c) == expected);
        assert(!c.active() && !c.pending() && f.mem == oracle.mem &&
               f.trace == oracle.trace);
      }
    }
  F live;
  live.mem[F::U + 0x29c] = 77;
  live.mem[F::U + 0x2a8] = 12;
  live.mem[F::V + 0x2bc] = 13;
  BattleEventBarrierCursor c;
  assert(resumeBattleAllEventBarrier(F::B, live.services(), c) == 0);
  assert(live.mem[F::U + 0x29c] == 77 && live.mem[F::U + 0x2a8] == 0 &&
         live.mem[F::V + 0x2bc] == 0);
  assert(live.accepted == 3);
  F outer;
  outer.mem[F::B + 0x182b4] = 10;
  outer.mem[F::B + 0x182c4] = 77;
  outer.mem[F::B + 0x180fc] = 12;
  assert(resumeBattleAllEventBarrier(F::B, outer.services(), c) == 0);
  assert(outer.mem[F::B + 0x182b4] == 0 && outer.mem[F::B + 0x180fc] == 12);
  // Standalone reserve consumer captures its own global once, even if the
  // actual removal callback changes it; repeated items are separate calls.
  F reserve;
  reserve.swapConsume = true;
  reserve.mem[F::B + 0x19060] = 5;
  reserve.mem[F::B + 0x19064] = 5;
  reserve.mem[F::C + 0x19064] = 9;
  reserve.block = 2;
  assert(!resumeBattleConsumeReservedItems(reserve.services(), c));
  assert(reserve.mem[F::B + 0x19060] == 0 && reserve.mem[F::B + 0x19064] == 5);
  auto prefix = reserve.trace;
  assert(!resumeBattleConsumeReservedItems(reserve.services(), c) &&
         prefix == reserve.trace);
  reserve.block = 0;
  assert(resumeBattleConsumeReservedItems(reserve.services(), c));
  assert(reserve.mem[F::B + 0x19064] == 0 && reserve.mem[F::C + 0x19064] == 9);
  F compact;
  compact.queue(0, 9, 4);
  compact.queue(1, 9, 4);
  compact.queue(2, 0, 7);
  releasePackedBattleAfterReaction(9, 4, compact.actors());
  assert(compact.mem[F::B + 0x16f5c] == 0 && compact.mem[F::B + 0x16f60] == 7 &&
         compact.mem[F::B + 0x16f64] == 0xffffffff);
  F nullScript;
  nullScript.queue(0, 0, 99);
  assert(resumeBattleAfterReaction(nullScript.services(), c) == 1);
  assert(nullScript.mem[F::U + 0x2a8] == 123 &&
         nullScript.mem[F::E + 0x160] == 0);
  bool sawNull = false;
  for (const auto &e : nullScript.trace)
    if (e[0] == 5 && e[1] == 0)
      sawNull = true;
  assert(sawNull);
  // A held allocation owns its already-resolved script. Release and GetData
  // have completed; a new queue occupant must survive, while the event/unit
  // IDs are fresh source reads after allocation succeeds.
  F allocate;
  allocate.queue(0, 0, 7);
  allocate.block = 2;
  assert(!resumeBattleAfterReaction(allocate.services(), c));
  assert(c.pending() == 0x800374ac);
  allocate.mem[0xa0004] = 0xc0000;
  allocate.mem[F::U] = 45;
  allocate.mem[F::E + 0x15c] = 456;
  allocate.queue(0, 1, 42);
  allocate.block = 0;
  assert(resumeBattleAfterReaction(allocate.services(), c) == 1);
  assert(
      allocate.mem[F::U + 0x2a8] == 456 && allocate.mem[F::E + 0x160] == 45 &&
      allocate.mem[F::B + 0x16f5c] == 1 && allocate.mem[F::B + 0x16f60] == 42);
  assert((allocate.trace.back() ==
          std::array<uint32_t, 4>{1, F::E + 0x160, 45, 0}));
  bool capturedScript = false;
  for (const auto &e : allocate.trace)
    if (e[0] == 5 && e[1] == 0xb0000)
      capturedScript = true;
  assert(capturedScript);
  F idle;
  auto noProviders = idle.services();
  noProviders.eventExists = nullptr;
  noProviders.removeItem = nullptr;
  noProviders.concluded = nullptr;
  noProviders.create = nullptr;
  assert(resumeBattleAllEventBarrier(F::B, noProviders, c) == 1);
  F re;
  re.prepare();
  re.reentry = true;
  re.cursor = &c;
  assert(resumeBattleAllEventBarrier(F::B, re.services(), c) == 0);
  return 0;
}
