#include "battle_attack_selection.hpp"
#include "harbor_tutorial_packed.hpp"
#include <cassert>
#include <functional>
#include <map>
#include <memory>
#include <vector>
using namespace ttyd;
struct Fixture {
  static constexpr uint32_t B = 0x100000, B2 = 0x200000, U = 0x300000,
                            M = 0x400000, C = 0x500000, W = 0x600000;
  std::map<uint32_t, uint32_t> words{{0x8041eb90, B}, {U, 1},
                                     {U + 8, 222},    {B + 0x24, U},
                                     {M + 0x80, W},   {W + 0xb0, 0x1234}};
  std::map<uint32_t, uint8_t> bytes;
  std::vector<uint32_t> reads, writes;
  unsigned cursorCalls = 0, messages = 0;
  bool cursorReady = true, messageReady = false, afterCount = true;
  uint32_t handle = 0;
  std::function<void(uint32_t)> onRead;
  BattleAttackSelectionServices services() {
    BattleAttackSelectionServices s;
    s.user = this;
    s.memory.user = this;
    s.memory.memory.user = this;
    s.memory.memory.word = [](void *p, uint32_t at) {
      auto &f = *static_cast<Fixture *>(p);
      f.reads.push_back(at);
      if (f.onRead)
        f.onRead(at);
      return f.words[at];
    };
    s.memory.byte = [](void *p, uint32_t at) {
      auto &f = *static_cast<Fixture *>(p);
      f.reads.push_back(at);
      if (f.onRead)
        f.onRead(at);
      return f.bytes[at];
    };
    s.memory.memory.storeWord = [](void *p, uint32_t at, uint32_t n) {
      auto &f = *static_cast<Fixture *>(p);
      f.writes.push_back(at);
      f.words[at] = n;
    };
    s.memory.memory.storeByte = [](void *p, uint32_t at, uint8_t n) {
      auto &f = *static_cast<Fixture *>(p);
      f.writes.push_back(at);
      f.bytes[at] = n;
      if (f.afterCount && at == B + 0x16f47) {
        f.words[B + 0x19090] = 1;
        f.words[B + 0x19094] = 44;
      }
    };
    s.cursor = [](void *p, uint32_t menu, uint32_t type, uint32_t &out) {
      auto &f = *static_cast<Fixture *>(p);
      assert(menu == M && type == 0);
      ++f.cursorCalls;
      out = C;
      return f.cursorReady;
    };
    s.continueMessageReady = [](void *p, uint32_t handle) {
      auto &f = *static_cast<Fixture *>(p);
      ++f.messages;
      f.handle = handle;
      return f.messageReady;
    };
    return s;
  }
};
void heldMessage() {
  using F = Fixture;
  F f;
  f.words[F::M + 0x540] = 11;
  f.bytes[F::U + 0x20] = 7;
  auto s = f.services();
  BattleAttackSelectionCursor c;
  assert(!resumePackedBattleAttackSelection(F::U, F::M, s, c) && c.active &&
         c.phase == 2);
  assert(f.words[F::U + 0x2a0] == 0x80373d00 && f.words[F::U + 0x2ac] == 3 &&
         f.bytes[F::U + 0x20] == 6 && f.bytes[F::B + 0x16f47] == 1 &&
         f.messages == 1 && f.handle == 44);
  const auto reads = f.reads.size(), writes = f.writes.size();
  f.words[0x8041eb90] = F::B2;
  f.words[F::M + 0x540] = 12;
  f.bytes[F::M + 0x551] = 1;
  f.words[F::B + 0x19090] = 0;
  f.words[F::B + 0x19094] = 9;
  f.bytes[F::U + 0x20] = 127;
  f.bytes[F::B + 0x16f47] = 99;
  f.words[F::U + 0x2a0] = 777;
  assert(!resumePackedBattleAttackSelection(F::U, F::M, s, c) &&
         f.messages == 2 && f.handle == 44);
  assert(f.reads.size() == reads && f.writes.size() == writes &&
         f.bytes[F::U + 0x20] == 127 && f.bytes[F::B + 0x16f47] == 99 &&
         f.words[F::U + 0x2a0] == 777);
  F foreign;
  assert(
      !resumePackedBattleAttackSelection(F::U, F::M, foreign.services(), c) &&
      foreign.reads.empty());
  assert(!resumePackedBattleAttackSelection(F::U + 4, F::M, s, c));
  assert(!resumePackedBattleAttackSelection(F::U, F::M + 4, s, c));
  f.messageReady = true;
  assert(resumePackedBattleAttackSelection(F::U, F::M, s, c) && !c.active &&
         f.messages == 3 && f.handle == 44 && f.bytes[F::B + 0x16f47] == 99);
}
void cursorWait() {
  using F = Fixture;
  F f;
  f.cursorReady = false;
  f.words[F::M + 0x540] = 0;
  f.bytes[F::U + 0x20] = 9;
  auto s = f.services();
  BattleAttackSelectionCursor c;
  assert(!resumePackedBattleAttackSelection(F::U, F::M, s, c) &&
         f.writes.empty());
  f.words[F::M + 0x540] = 1;
  f.words[0x8041eb90] = F::B2;
  f.bytes[F::M + 0x551] = 1;
  f.cursorReady = true;
  assert(resumePackedBattleAttackSelection(F::U, F::M, s, c) && !c.active &&
         f.cursorCalls == 2 && f.words[F::U + 0x2a0] == 0x1234 &&
         f.bytes[F::B + 0x16f38] == 1 && f.bytes[F::B2 + 0x16f38] == 0 &&
         f.bytes[F::U + 0x20] == 9);
}
void noMessage() {
  using F = Fixture;
  F f;
  f.afterCount = false;
  f.words[F::M + 0x540] = 11;
  f.onRead = [](uint32_t at) { assert(at != F::B + 0x19094); };
  auto s = f.services();
  s.continueMessageReady = nullptr;
  BattleAttackSelectionCursor c;
  assert(resumePackedBattleAttackSelection(F::U, F::M, s, c) && !c.active &&
         !f.messages);
  for (int present : {0, 1, 2}) {
    F move;
    move.words[F::M + 0x540] = 12;
    move.words[F::B + 0x16040] = present;
    move.words[F::B + 0x19090] = 1;
    move.onRead = [](uint32_t at) {
      assert(at != F::B + 0x19090 && at != F::B + 0x19094);
    };
    auto m = move.services();
    m.continueMessageReady = nullptr;
    assert(resumePackedBattleAttackSelection(F::U, F::M, m, c));
    assert(move.words[F::U + 0x2a0] ==
               uint32_t(present == 1 ? 0x80373d3c : 0x80373d74) &&
           !move.messages && !move.bytes[F::B + 0x16f47]);
  }
  F saturated;
  saturated.words[F::M + 0x540] = 11;
  saturated.words[F::B + 0x19090] = 1;
  saturated.words[F::B + 0x19094] = 44;
  saturated.bytes[F::B + 0x16f47] = 100;
  auto full = saturated.services();
  assert(!resumePackedBattleAttackSelection(F::U, F::M, full, c));
  saturated.messageReady = true;
  assert(resumePackedBattleAttackSelection(F::U, F::M, full, c) &&
         saturated.bytes[F::B + 0x16f47] == 100);
}
void nativeAudience() {
  auto vm =
      std::make_unique<EventNativeRuntime>(0x81000000, EventNativePlatform{});
  assert(
      vm->memory.registerRam(0x80422b44, {0x42, 0xb4, 0, 0, 0xc2, 0x48, 0, 0}));
  auto p = std::make_unique<HarborTutorialPacked>(*vm);
  assert(p->provision());
  const auto B = HarborTutorialPacked::Battle,
             U = HarborTutorialPacked::Units + 0x1000, M = B + 0x171c;
  auto put = [&](uint32_t at, uint32_t n) {
    assert(vm->memory.writeWord(at, n));
  };
  auto read = [&](uint32_t at) {
    uint32_t n;
    assert(vm->memory.readWord(at, n));
    return n;
  };
  auto byte = [&](uint32_t at) {
    uint8_t n;
    assert(vm->memory.readByte(at, n));
    return n;
  };
  put(B + 0x24, U);
  put(U, 1);
  put(U + 8, 222);
  put(M, 29);
  put(M + 0x540, 11);
  assert(!p->weaponDecided && p->selectAttackEvent());
  assert(read(U + 0x2a0) == 0x80373d00 && read(U + 0x2ac) == 3 &&
         byte(U + 0x20) == 4 && byte(B + 0x16f47) == 1);
  put(M + 0x540, 12);
  put(B + 0x16040, 1);
  assert(p->selectAttackEvent() && read(U + 0x2a0) == 0x80373d3c);
  put(B + 0x16040, 0);
  assert(p->selectAttackEvent() && read(U + 0x2a0) == 0x80373d74 &&
         byte(B + 0x16f47) == 1);
  put(M + 0x540, 11);
  put(B + 0x19090, 1);
  put(B + 0x19094, 5);
  assert(!p->selectAttackEvent() && p->attackSelectionCursor.phase == 2 &&
         byte(B + 0x16f47) == 2);
  HarborTutorial scene;
  p->weaponDecided = p->commandFinalized = true;
  scene.commandInputPending = 456;
  assert(!p->buildWeaponMenu(0, scene, nullptr, nullptr));
  assert(!p->buildClassMenu(scene, nullptr, nullptr));
  assert(p->weaponDecided && p->commandFinalized &&
         scene.commandInputPending == 456 && p->attackSelectionCursor.phase == 2 &&
         byte(B + 0x16f47) == 2 && read(M + 0x540) == 11);
  put(B + 0x19090, 0);
  put(U + 0x2a0, 777);
  assert(vm->memory.writeByte(U + 0x20, 9));
  assert(!p->selectAttackEvent() && byte(B + 0x16f47) == 2 &&
         read(U + 0x2a0) == 777 && byte(U + 0x20) == 9);
  unsigned continued = 0;
  BattleAttackSelectionServices admitted;
  admitted.memory = nativeBattleActorServices(*vm);
  admitted.user = &continued;
  admitted.continueMessageReady = [](void *p, uint32_t handle) {
    assert(handle == 5);
    ++*static_cast<unsigned *>(p);
    return true;
  };
  assert(resumePackedBattleAttackSelection(U, M, admitted,
                                           p->attackSelectionCursor) &&
         continued == 1 && byte(B + 0x16f47) == 2);
  for (int32_t i = 0; i < vm->battle.capacity; ++i)
    assert(!(vm->battle.entries[i].flags & 1));
}
int main() {
  heldMessage();
  cursorWait();
  noMessage();
  nativeAudience();
}
