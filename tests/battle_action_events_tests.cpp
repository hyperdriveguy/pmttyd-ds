#include "battle_action_events.hpp"
#include <algorithm>
#include <cassert>
#include <map>
#include <vector>
using namespace ttyd;
struct Fixture {
  static constexpr uint32_t B = 0x10000, C = 0x40000, A = 0x70000;
  std::map<uint32_t, uint32_t> words{{0x8041eb90, B}, {A, 0x87654321}};
  std::map<uint32_t, uint8_t> bytes;
  std::vector<uint32_t> reads;
  unsigned evaluated = 0, removed = 0;
  int32_t raw = 0, value = 0;
  BattleActionEventServices services() {
    BattleActionEventServices s;
    s.user = this;
    s.memory.memory.user = this;
    s.memory.user = this;
    s.memory.memory.word = [](void *p, uint32_t at) {
      auto &f = *static_cast<Fixture *>(p);
      f.reads.push_back(at);
      return f.words[at];
    };
    s.memory.memory.storeWord = [](void *p, uint32_t at, uint32_t v) {
      static_cast<Fixture *>(p)->words[at] = v;
    };
    s.memory.memory.storeByte = [](void *p, uint32_t at, uint8_t v) {
      static_cast<Fixture *>(p)->bytes[at] = v;
    };
    s.values.user = this;
    s.values.store = [](void *p, EventEntry &, int32_t raw, int32_t v) {
      auto &f = *static_cast<Fixture *>(p);
      f.raw = raw;
      f.value = v;
    };
    s.evaluate = [](void *p, uint32_t b,
                    uint32_t callback) -> std::optional<int32_t> {
      auto &f = *static_cast<Fixture *>(p);
      assert(b == B && callback == 11);
      ++f.evaluated;
      f.words[b + 0x1cb8] = 2;
      f.words[0x8041eb90] = C;
      return 0;
    };
    s.remove = [](void *p, uint32_t b, uint32_t callback) {
      auto &f = *static_cast<Fixture *>(p);
      assert(b == B && callback == 22);
      ++f.removed;
      f.words[b + 0x1c9c] = 1000;
      return true;
    };
    return s;
  }
};
int main() {
  EventEntry e;
  e.arguments = Fixture::A;
  Fixture pending;
  pending.words[Fixture::B + 0x1cb8] = 1;
  assert(packedBattleResultActionEvent(e, pending.services()) == 0 &&
         !pending.evaluated && pending.bytes.empty());
  Fixture null;
  null.words[Fixture::B + 0x1cb8] = 0;
  null.words[Fixture::B + 0x1900c] = 2;
  assert(packedBattleResultActionEvent(e, null.services()) == 2 &&
         std::count(null.reads.begin(), null.reads.end(),
                    Fixture::B + 0x1cb8) == 2 &&
         !null.evaluated && null.words[Fixture::B + 0x1900c] == 0 &&
         null.bytes[Fixture::B + 0x15fb4] == 1);
  Fixture eval;
  eval.words[Fixture::B + 0x1ca4] = 11;
  eval.words[Fixture::B + 0x1cb8] = 1;
  assert(packedBattleResultActionEvent(e, eval.services()) == 2 &&
         eval.evaluated == 1 && eval.words[Fixture::B + 0x1cb8] == 2 &&
         eval.words[Fixture::C + 0x1900c] == 0 &&
         eval.bytes[Fixture::C + 0x15fb4] == 1);
  Fixture blocked;
  blocked.words[Fixture::B + 0x1ca4] = 11;
  blocked.words[Fixture::B + 0x1cb8] = 1;
  auto s = blocked.services();
  s.evaluate = [](void *, uint32_t, uint32_t) -> std::optional<int32_t> {
    return 2;
  };
  assert(packedBattleResultActionEvent(e, s) == 0 && blocked.bytes.empty());
  s.evaluate = [](void *, uint32_t, uint32_t) -> std::optional<int32_t> {
    return std::nullopt;
  };
  assert(!packedBattleResultActionEvent(e, s) &&
         blocked.words[Fixture::B + 0x1cb8] == 1);
  Fixture stop;
  stop.words[Fixture::B + 0x1cac] = 22;
  stop.words[Fixture::B + 0x1ca0] = 33;
  stop.words[Fixture::B + 0x1cb4] = 4;
  assert(packedBattleStopActionEvent(e, stop.services()) == 2 &&
         stop.removed == 1 && stop.words[Fixture::B + 0x1c9c] == 1000 &&
         stop.words[Fixture::B + 0x1cac] == 22 &&
         stop.words[Fixture::B + 0x1ca0] == 33 &&
         stop.words[Fixture::B + 0x1cb4] == 4 &&
         stop.bytes[Fixture::B + 0x15fb4] == 1);
  s = stop.services();
  s.remove = nullptr;
  assert(!packedBattleStopActionEvent(e, s));
  stop.words[Fixture::B + 0x1cac] = 0;
  assert(packedBattleStopActionEvent(e, s) == 2);
  Fixture read;
  read.words[Fixture::B + 0x1cb8] = 0xffffffff;
  read.words[Fixture::B + 0x1cb4] = 9;
  auto r = read.services();
  assert(packedBattleReadActionEvent(e, false, r) == 2 &&
         uint32_t(read.raw) == 0x87654321 && read.value == -1 &&
         !read.evaluated);
  assert(packedBattleReadActionEvent(e, true, r) == 2 && read.value == 9 &&
         read.words[Fixture::B + 0x1cb4] == 9);
}
