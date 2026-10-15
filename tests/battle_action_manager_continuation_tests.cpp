#include "battle_action_manager_continuation.hpp"
#include <cassert>
#include <cstring>
#include <map>
#include <vector>
using namespace ttyd;
struct Fixture {
  static constexpr uint32_t B = 0x1000, C = 0x8000;
  std::map<uint32_t, uint32_t> memory{
      {B + 0x1ca0, 11}, {B + 0x1ca8, 12},         {B + 0x1ca4, 21},
      {B + 0x1cac, 23}, {0x80422d68, 0x41700000}, {0x8041eb90, B}};
  std::vector<std::array<uint32_t, 4>> trace;
  unsigned block = 0;
  bool mutate = true, reenter = false;
  int32_t result = 0;
  BattleActionManagerCursor *cursor = nullptr;
  bool effect(unsigned stage, uint32_t x, uint32_t y, uint32_t z) {
    if (reenter) {
      assert(cursor);
      assert(!resumePackedBattleActionCommandManager(B, services(), *cursor));
      assert(!cursor->reset());
    }
    if (block == stage)
      return false;
    trace.push_back({stage, x, y, z});
    return true;
  }
  BattleActionManagerServices services() {
    BattleActionManagerServices s;
    s.user = this;
    s.memory.memory.user = this;
    s.memory.memory.word = [](void *p, uint32_t a) -> uint32_t {
      auto &f = *static_cast<Fixture *>(p);
      const auto v = f.memory[a];
      f.trace.push_back({4, a, v, 0});
      return v;
    };
    s.memory.memory.storeWord = [](void *p, uint32_t a, uint32_t v) {
      auto &f = *static_cast<Fixture *>(p);
      f.memory[a] = v;
      f.trace.push_back({5, a, v, 0});
    };
    s.pad = [](void *p, uint32_t a, int32_t channel) {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.effect(1, a, uint32_t(channel), 0))
        return false;
      if (f.mutate) {
        f.memory[B + 0x1ca0] = 13;
        f.memory[0x8041eb90] = C;
      }
      return true;
    };
    s.main = [](void *p, uint32_t fn, uint32_t b) -> std::optional<int32_t> {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.effect(2, fn, b, 0))
        return {};
      if (f.mutate) {
        f.memory[B + 0x1ca8] = 14;
        f.memory[B + 0x1ca0] = 99;
        f.memory[0x80422d68] = 0x41a00000;
      }
      return f.result;
    };
    s.display = [](void *p, int32_t camera, int32_t cls, uint32_t fn,
                   uint32_t b, float priority) {
      auto &f = *static_cast<Fixture *>(p);
      assert(camera == 8 && cls == 1);
      uint32_t bits;
      std::memcpy(&bits, &priority, 4);
      return f.effect(3, fn, b, bits);
    };
    return s;
  }
};
int main() {
  for (bool mutate : {false, true})
    for (int32_t result : {0, 1, -1}) {
      Fixture ref;
      ref.mutate = mutate;
      ref.result = result;
      assert(packedBattleActionCommandManager(Fixture::B, ref.services()));
      for (unsigned hold = 0; hold <= 3; ++hold) {
        Fixture f;
        f.mutate = mutate;
        f.result = result;
        f.block = hold;
        BattleActionManagerCursor c;
        if (hold) {
          assert(!resumePackedBattleActionCommandManager(Fixture::B,
                                                         f.services(), c));
          assert(c.active() && c.pending());
          auto prefix = f.trace;
          auto memory = f.memory;
          assert(!resumePackedBattleActionCommandManager(Fixture::B,
                                                         f.services(), c));
          assert(f.trace == prefix && f.memory == memory);
          assert(!resumePackedBattleActionCommandManager(Fixture::C,
                                                         f.services(), c));
          auto wrong = f.services();
          wrong.memory.audienceLifecycle.user = &ref;
          assert(!resumePackedBattleActionCommandManager(Fixture::B, wrong, c));
          assert(f.trace == prefix && f.memory == memory);
        }
        f.block = 0;
        assert(resumePackedBattleActionCommandManager(Fixture::B, f.services(),
                                                      c));
        assert(!c.active() && !c.pending());
        assert(f.memory == ref.memory && f.trace == ref.trace);
      }
    }
  Fixture null;
  null.mutate = false;
  null.memory[Fixture::B + 0x1ca0] = 0;
  auto n = null.services();
  n.main = nullptr;
  n.display = nullptr;
  BattleActionManagerCursor nc;
  assert(resumePackedBattleActionCommandManager(Fixture::B, n, nc));
  assert(null.trace.size() == 2); // pad, then main-pointer read only.
  Fixture hidden;
  hidden.mutate = false;
  hidden.result = 0;
  hidden.memory[Fixture::B + 0x1ca8] = 0;
  auto hs = hidden.services();
  hs.display = nullptr;
  assert(resumePackedBattleActionCommandManager(Fixture::B, hs, nc));
  assert(hidden.memory[Fixture::B + 0x1ca4] == 21 &&
         hidden.memory[Fixture::B + 0x1cac] == 23);
  // A held Main captures callback13, but rereads display/priority after its
  // result.
  Fixture fresh;
  fresh.block = 2;
  BattleActionManagerCursor fc;
  assert(!resumePackedBattleActionCommandManager(Fixture::B, fresh.services(),
                                                 fc));
  fresh.memory[Fixture::B + 0x1ca0] = 77;
  fresh.block = 0;
  assert(
      resumePackedBattleActionCommandManager(Fixture::B, fresh.services(), fc));
  assert(fresh.trace[2][1] == 13);
  assert(fresh.trace.back()[1] == 14 && fresh.trace.back()[3] == 0x41a00000);
  // The display callback and priority are retained once that call begins.
  Fixture display;
  display.block = 3;
  BattleActionManagerCursor dc;
  assert(!resumePackedBattleActionCommandManager(Fixture::B, display.services(),
                                                 dc));
  display.memory[Fixture::B + 0x1ca8] = 88;
  display.memory[0x80422d68] = 0;
  const auto before = display.trace;
  display.block = 0;
  assert(resumePackedBattleActionCommandManager(Fixture::B, display.services(),
                                                dc));
  assert(display.trace.size() == before.size() + 1 &&
         display.trace.back()[1] == 14 &&
         display.trace.back()[3] == 0x41a00000);
  Fixture absent;
  auto as = absent.services();
  as.main = nullptr;
  BattleActionManagerCursor ac;
  assert(!resumePackedBattleActionCommandManager(Fixture::B, as, ac));
  auto prefix = absent.trace;
  assert(!resumePackedBattleActionCommandManager(Fixture::B, as, ac) &&
         prefix == absent.trace);
  assert(!resumePackedBattleActionCommandManager(Fixture::B, absent.services(),
                                                 ac) &&
         prefix == absent.trace);
  Fixture re;
  BattleActionManagerCursor rc;
  re.cursor = &rc;
  re.reenter = true;
  assert(resumePackedBattleActionCommandManager(Fixture::B, re.services(), rc));
  return 0;
}
