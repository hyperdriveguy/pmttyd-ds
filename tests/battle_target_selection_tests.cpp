#include "battle_attack_selection.hpp"
#include "battle_target_selection.hpp"
#include <cassert>
#include <functional>
#include <map>
#include <string>
#include <vector>
using namespace ttyd;
struct Fixture {
  static constexpr uint32_t B = 0x10000, M = B + 0x171c, U = 0x40000,
                            C = 0x50000, W = 0x60000, Slot = 0x70000,
                            T = B + 0x428;
  std::map<uint32_t, uint32_t> words{
      {Slot, C}, {C, 0}, {M + 0x80, W}, {W + 0x64, 0x01000000}, {M + 0x540, 0}};
  std::map<uint32_t, uint16_t> halves;
  std::map<uint32_t, uint8_t> bytes;
  std::vector<std::string> calls;
  std::vector<uint32_t> sounds, repeatMasks, triggerMasks;
  uint32_t repeats = 0, triggers = 0, paid = 0;
  bool ready = true;
  std::optional<bool> audience = false;
  std::function<void()> onRefresh, onEnd, onPose, onSelect;
  Fixture() { targets({{1, 1}, {2, 1}}); }
  void targets(std::vector<std::pair<int16_t, int16_t>> rows) {
    bytes[T + 0xa6c] = uint8_t(rows.size());
    for (unsigned i = 0; i < rows.size(); ++i) {
      bytes[T + 0xa6d + i] = uint8_t(i);
      halves[T + 4 + 36 * i] = uint16_t(rows[i].first);
      halves[T + 6 + 36 * i] = uint16_t(rows[i].second);
    }
  }
  BattleActorServices memory() {
    BattleActorServices s;
    s.user = this;
    s.memory.user = this;
    s.memory.word = [](void *p, uint32_t at) {
      return static_cast<Fixture *>(p)->words[at];
    };
    s.memory.half = [](void *p, uint32_t at) {
      return static_cast<Fixture *>(p)->halves[at];
    };
    s.byte = [](void *p, uint32_t at) {
      return static_cast<Fixture *>(p)->bytes[at];
    };
    s.memory.storeWord = [](void *p, uint32_t at, uint32_t v) {
      static_cast<Fixture *>(p)->words[at] = v;
    };
    s.memory.storeByte = [](void *p, uint32_t at, uint8_t v) {
      static_cast<Fixture *>(p)->bytes[at] = v;
    };
    return s;
  }
  BattleTargetSelectionServices services() {
    BattleTargetSelectionServices s;
    s.memory = memory();
    s.user = this;
    s.privateCursor = Slot;
    s.cursor = [](void *p, uint32_t menu, uint32_t type, uint32_t out) {
      auto &f = *static_cast<Fixture *>(p);
      assert(menu == M && type == f.words[M + 0x540]);
      f.words[out] = C;
    };
    s.refresh = [](void *p, uint32_t buffer) {
      auto &f = *static_cast<Fixture *>(p);
      assert(buffer == T);
      f.calls.push_back("refresh");
      if (f.onRefresh)
        f.onRefresh();
      return f.ready;
    };
    s.repeat = [](void *p, uint32_t mask) {
      auto &f = *static_cast<Fixture *>(p);
      f.repeatMasks.push_back(mask);
      return f.repeats & mask;
    };
    s.trigger = [](void *p, uint32_t mask) {
      auto &f = *static_cast<Fixture *>(p);
      f.triggerMasks.push_back(mask);
      return f.triggers & mask;
    };
    s.sound = [](void *p, uint32_t key) {
      static_cast<Fixture *>(p)->sounds.push_back(key);
    };
    s.endTarget = [](void *p, uint32_t b) {
      auto &f = *static_cast<Fixture *>(p);
      assert(b == B);
      f.calls.push_back("end");
      if (f.onEnd)
        f.onEnd();
      return true;
    };
    s.help = [](void *p, int32_t mode) {
      static_cast<Fixture *>(p)->calls.push_back("help" + std::to_string(mode));
    };
    s.commandPose = [](void *p, uint32_t u) {
      auto &f = *static_cast<Fixture *>(p);
      assert(u == U);
      f.calls.push_back("pose");
      if (f.onPose)
        f.onPose();
      return true;
    };
    s.offEnemyLight = [](void *p) {
      static_cast<Fixture *>(p)->calls.push_back("enemyOff");
    };
    s.offLight = [](void *p, int32_t a, int32_t b) {
      static_cast<Fixture *>(p)->calls.push_back("off" + std::to_string(a) +
                                                 std::to_string(b));
    };
    s.pay = [](void *p, uint32_t u, uint32_t w) {
      auto &f = *static_cast<Fixture *>(p);
      assert(u == U);
      f.paid = w;
      f.calls.push_back("pay");
      return true;
    };
    s.audience = [](void *p) {
      auto &f = *static_cast<Fixture *>(p);
      f.calls.push_back("audience");
      return f.audience;
    };
    return s;
  }
  bool run() { return processPackedBattleTargetSelection(B, U, services()); }
};
int main() {
  Fixture remap;
  remap.bytes[Fixture::T + 0xab7] = 1;
  remap.onRefresh = [&] { remap.targets({{2, 1}, {1, 1}, {2, 1}}); };
  assert(remap.run() && remap.bytes[Fixture::T + 0xab7] == 0);
  Fixture part;
  part.bytes[Fixture::T + 0xab7] = 1;
  part.onRefresh = [&] { part.targets({{2, 2}, {2, 1}}); };
  assert(part.run() && part.bytes[Fixture::T + 0xab7] == 1);
  Fixture missing;
  missing.bytes[Fixture::T + 0xab7] = 1;
  missing.onRefresh = [&] { missing.targets({{3, 1}}); };
  assert(missing.run() && missing.bytes[Fixture::T + 0xab7] == 0);
  Fixture perm;
  perm.bytes[Fixture::T + 0xa6d] = 1;
  perm.onRefresh = [&] { perm.targets({{1, 1}, {2, 1}}); };
  assert(perm.run() && perm.bytes[Fixture::T + 0xab7] == 1);
  Fixture both;
  both.repeats = 0xc0000;
  assert(both.run() && both.bytes[Fixture::T + 0xab7] == 0 &&
         both.sounds.size() == 2 &&
         both.repeatMasks == std::vector<uint32_t>({0x80000, 0x40000}));
  Fixture reverse;
  reverse.repeats = 0x40000;
  assert(reverse.run() && reverse.bytes[Fixture::T + 0xab7] == 1);
  Fixture single;
  single.targets({{1, 1}});
  single.repeats = 0xc0000;
  assert(single.run() && single.sounds.empty() &&
         !single.bytes[Fixture::T + 0xab7]);
  Fixture fixed;
  fixed.words[Fixture::W + 0x64] = 0;
  fixed.repeats = 0xc0000;
  assert(fixed.run() && fixed.repeatMasks.empty());
  Fixture unavailable;
  unavailable.ready = false;
  assert(!unavailable.run() && unavailable.triggerMasks.empty());
  Fixture cancel;
  cancel.triggers = 0x300;
  cancel.words[Fixture::B + 0xef4] = 0x204;
  cancel.onPose = [&] {
    cancel.words[Fixture::M + 0x540] = 1;
    cancel.words[Fixture::B + 0xef4] = 0x400;
  };
  assert(cancel.run() && cancel.words[Fixture::M] == 1 &&
         cancel.calls == std::vector<std::string>(
                             {"refresh", "end", "help0", "pose", "enemyOff"}) &&
         cancel.triggerMasks == std::vector<uint32_t>{0x200} &&
         cancel.sounds == std::vector<uint32_t>{2521});
  Fixture confirm;
  confirm.triggers = 0x100;
  confirm.onEnd = [&] {
    assert(confirm.words[Fixture::M] == 29);
    confirm.words[Fixture::B + 0xef4] = 0x80;
  };
  assert(confirm.run() && confirm.words[Fixture::B + 0xef4] == 0x84 &&
         confirm.calls == std::vector<std::string>({"refresh", "off11", "end",
                                                    "help2", "audience"}) &&
         !confirm.bytes[Fixture::B + 0x1c74]);
  for (uint8_t op : {uint8_t(2), uint8_t(3)}) {
    Fixture item;
    item.words[Fixture::M + 0x540] = 2;
    item.bytes[Fixture::M + 0x550] = op;
    item.triggers = 0x100;
    assert(item.run() && item.paid == (op == 2 ? 0x803553a8u : 0x80355468u) &&
           item.bytes[Fixture::U + 0x20] == (op == 2 ? 8 : 5));
  }
  Fixture retained;
  retained.words[Fixture::M + 0x540] = 2;
  retained.bytes[Fixture::M + 0x550] = 2;
  retained.triggers = 0x100;
  retained.onRefresh = [&] { retained.words[Fixture::M + 0x540] = 0; };
  assert(retained.run() && retained.paid == 0x803553a8);
  Fixture already;
  already.words[Fixture::M + 0x540] = 2;
  already.bytes[Fixture::M + 0x550] = 2;
  already.bytes[Fixture::U + 0x20] = 255;
  already.triggers = 0x100;
  assert(already.run() && !already.paid);
  Fixture interrupt;
  interrupt.triggers = 0x100;
  interrupt.audience = true;
  assert(interrupt.run() && interrupt.bytes[Fixture::B + 0x1c74] == 1 &&
         interrupt.calls ==
             std::vector<std::string>({"refresh", "off11", "end", "help2",
                                       "audience", "end", "help0"}));
  Fixture unbound;
  unbound.audience = std::nullopt;
  assert(!unbound.run() && !unbound.bytes[Fixture::B + 0x1c74]);
  Fixture finalize;
  BattleCommandFinalizeServices f;
  f.memory = finalize.memory();
  f.user = &finalize;
  f.selectAttack = [](void *p, uint32_t u, uint32_t m) {
    auto &f = *static_cast<Fixture *>(p);
    assert(u == Fixture::U && m == Fixture::M);
    f.calls.push_back("select");
    f.words[m + 4] = 0x01000080;
    f.words[Fixture::B + 0xef8] = 0x101;
    return true;
  };
  f.offLight = finalize.services().offLight;
  assert(finalizePackedBattleCommand(Fixture::B, Fixture::U, f) == 1 &&
         finalize.words[Fixture::M + 4] == 0x80 &&
         finalize.words[Fixture::B + 0xef8] == 0x100 &&
         finalize.calls == std::vector<std::string>({"select", "off00"}));
  f.selectAttack = [](void *, uint32_t, uint32_t) { return false; };
  finalize.calls.clear();
  assert(!finalizePackedBattleCommand(Fixture::B, Fixture::U, f) &&
         finalize.calls.empty());
  // Finalization consumes the shared original selector, publishing the actual
  // Jump row event/category and Mario usage counter without creating a root.
  Fixture jump;
  jump.words[0x8041eb90] = Fixture::B;
  jump.words[Fixture::M + 0x540] = 0;
  jump.words[Fixture::W + 0xb0] = 0x80355cf0;
  jump.words[Fixture::U + 8] = 222;
  BattleCommandFinalizeServices original;
  original.memory = jump.memory();
  original.user = &jump;
  original.selectAttack = [](void *p, uint32_t u, uint32_t menu) {
    auto &j = *static_cast<Fixture *>(p);
    BattleAttackSelectionServices selector;
    selector.memory = j.memory();
    selector.cursor = [](void *, uint32_t m, uint32_t t, uint32_t &out) {
      assert(m == Fixture::M && t == 0);
      out = Fixture::C;
      return true;
    };
    return packedSetBattleAttackEvent(u, menu, selector);
  };
  original.offLight = jump.services().offLight;
  assert(finalizePackedBattleCommand(Fixture::B, Fixture::U, original) == 1);
  assert(jump.words[Fixture::U + 0x2a0] == 0x80355cf0);
  assert(jump.words[Fixture::U + 0x2ac] == 0);
  assert(jump.bytes[Fixture::B + 0x16f38] == 1);
  assert(jump.words[Fixture::U + 0x2a8] == 0);
}
