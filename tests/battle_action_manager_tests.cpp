#include "battle_action_help.hpp"
#include "battle_action_manager.hpp"
#include <cassert>
#include <functional>
#include <map>
#include <vector>
using namespace ttyd;
struct Fixture {
  static constexpr uint32_t B = 0x10000, C = 0x40000, U = 0x70000, A = 0x80000;
  std::map<uint32_t, uint32_t> words{{0x8041eb90, B},
                                     {B + 0x1c90, U},
                                     {C + 0x1c90, U},
                                     {0x80422d68, 0x41700000},
                                     {A, 1}};
  std::map<uint32_t, uint8_t> bytes;
  std::vector<int> calls;
  std::function<void(uint32_t, uint32_t)> store;
  std::optional<int32_t> result = 1;
  bool padReady = true, displayReady = true;
  BattleActorServices memory() {
    BattleActorServices s;
    s.memory.user = this;
    s.user = this;
    s.memory.word = [](void *p, uint32_t at) {
      return static_cast<Fixture *>(p)->words[at];
    };
    s.memory.storeWord = [](void *p, uint32_t at, uint32_t v) {
      auto &f = *static_cast<Fixture *>(p);
      f.words[at] = v;
      if (f.store)
        f.store(at, v);
    };
    s.memory.storeByte = [](void *p, uint32_t at, uint8_t v) {
      static_cast<Fixture *>(p)->bytes[at] = v;
    };
    return s;
  }
  BattleActionManagerServices services() {
    BattleActionManagerServices s;
    s.memory = memory();
    s.user = this;
    s.pad = [](void *p, uint32_t at, int32_t channel) {
      auto &f = *static_cast<Fixture *>(p);
      assert(at == B + 0x1d1c && channel == 0);
      f.calls.push_back(1);
      return f.padReady;
    };
    s.main = [](void *p, uint32_t callback, uint32_t b) {
      auto &f = *static_cast<Fixture *>(p);
      assert(callback == 11 && b == B);
      f.calls.push_back(2);
      f.words[B + 0x1ca8] = 22;
      return f.result;
    };
    s.display = [](void *p, int32_t camera, int32_t cls, uint32_t callback,
                   uint32_t b, float priority) {
      auto &f = *static_cast<Fixture *>(p);
      assert(camera == 8 && cls == 1 && callback == 22 && b == B &&
             priority == 15);
      f.calls.push_back(3);
      return f.displayReady;
    };
    return s;
  }
};
int main() {
  Fixture inactive;
  inactive.words[Fixture::B + 0x1ca8] = 22;
  assert(packedBattleActionCommandManager(Fixture::B, inactive.services()) &&
         inactive.calls == std::vector<int>{1});
  Fixture f;
  f.words[Fixture::B + 0x1ca0] = 11;
  assert(packedBattleActionCommandManager(Fixture::B, f.services()) &&
         f.calls == std::vector<int>({1, 2, 3}) &&
         f.words[Fixture::B + 0x1ca0] == 11);
  f.calls.clear();
  f.result = 0;
  f.words[Fixture::B + 0x1ca4] = 33;
  f.words[Fixture::B + 0x1cac] = 44;
  assert(packedBattleActionCommandManager(Fixture::B, f.services()) &&
         !f.words[Fixture::B + 0x1ca0] && f.words[Fixture::B + 0x1ca4] == 33 &&
         f.words[Fixture::B + 0x1ca8] == 22 &&
         f.words[Fixture::B + 0x1cac] == 44 &&
         f.calls == std::vector<int>({1, 2, 3}));
  f.words[Fixture::B + 0x1ca0] = 11;
  f.result = std::nullopt;
  f.calls.clear();
  assert(!packedBattleActionCommandManager(Fixture::B, f.services()) &&
         f.calls == std::vector<int>({1, 2}) &&
         f.words[Fixture::B + 0x1ca0] == 11);
  f.padReady = false;
  f.calls.clear();
  assert(!packedBattleActionCommandManager(Fixture::B, f.services()) &&
         f.calls == std::vector<int>{1});
  f.padReady = true;
  f.result = 0;
  f.displayReady = false;
  assert(!packedBattleActionCommandManager(Fixture::B, f.services()) &&
         !f.words[Fixture::B + 0x1ca0]);
  Fixture help;
  packedBattleActionHelpSetKey(123, help.memory());
  assert(help.words[Fixture::B + 0x19008] == 123);
  help.store = [&](uint32_t at, uint32_t) {
    if (at == Fixture::B + 0x1900c)
      help.words[0x8041eb90] = Fixture::C;
  };
  packedBattleActionHelpSetDisplay(0, help.memory());
  assert(help.words[Fixture::B + 0x1900c] == 0 &&
         help.bytes[Fixture::C + 0x15fb4] == 1);
  packedBattleActionHelpSetDisplay(-1, help.memory());
  assert(help.words[Fixture::C + 0x1900c] == 0xffffffff &&
         help.bytes[Fixture::C + 0x15fb4] == 0);
  Fixture start;
  EventEntry e;
  e.arguments = Fixture::A;
  BattleUnitEventServices u;
  u.memory = start.memory().memory;
  u.values.user = &start;
  u.values.resolve = [](void *p, EventEntry &, int32_t raw) {
    static_cast<Fixture *>(p)->words[0x8041eb90] = Fixture::C;
    return raw;
  };
  assert(packedBattleStartActionCommandEvent(e, u, start.memory()) == 2 &&
         start.words[Fixture::B + 0x1c9c] == 100 &&
         start.words[Fixture::C + 0x1900c] == 2 &&
         start.bytes[Fixture::C + 0x15fb4] == 0);
  for (uint32_t on : {0u, 1u}) {
    Fixture gated;
    gated.words[Fixture::A] = on;
    gated.words[Fixture::U + 0x27c] = 0x10;
    BattleUnitEventServices s;
    s.memory = gated.memory().memory;
    s.values.resolve = [](void *, EventEntry &, int32_t raw) { return raw; };
    assert(packedBattleStartActionCommandEvent(e, s, gated.memory()) == 2 &&
           gated.words[Fixture::B + 0x1c9c] == 100 &&
           !gated.words[Fixture::B + 0x1900c]);
  }
}
