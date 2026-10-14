#include "battle_ap_report.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <map>
using namespace ttyd;
struct Fixture {
  static constexpr uint32_t B = 0x10000, A = B + 0x2858, I = B + 0x18ffc,
                            M = 0x40000, P = 0x41000, O = 0x50000, S = 0x60000;
  std::map<uint32_t, uint32_t> words{{0x8041eb90, B},
                                     {0x80425550, 0x3f800000},
                                     {0x8042558c, 0x40000000},
                                     {0x80425590, 0x3f000000},
                                     {0x804255a0, 0x3e800000}};
  int32_t counts[13]{};
  int32_t turn = 0, draw = 99;
  uint32_t partner = 0;
  unsigned allocations = 0, draws = 0, increments = 0;
  bool allocReady = true;
  void byte(uint32_t at, uint8_t v) {
    const auto shift = (3 - (at & 3)) * 8;
    words[at & ~3] =
        (words[at & ~3] & ~(255u << shift)) | (uint32_t(v) << shift);
  }
  uint8_t byte(uint32_t at) {
    return uint8_t(words[at & ~3] >> ((3 - (at & 3)) * 8));
  }
  void real(uint32_t at, float v) {
    uint32_t bits;
    std::memcpy(&bits, &v, 4);
    words[at] = bits;
  }
  BattleActorServices memory() {
    BattleActorServices m;
    m.user = this;
    m.memory.user = this;
    m.memory.word = [](void *p, uint32_t at) {
      return static_cast<Fixture *>(p)->words[at];
    };
    m.memory.storeWord = [](void *p, uint32_t at, uint32_t v) {
      static_cast<Fixture *>(p)->words[at] = v;
    };
    m.byte = [](void *p, uint32_t at) {
      return static_cast<Fixture *>(p)->byte(at);
    };
    m.memory.storeByte = [](void *p, uint32_t at, uint8_t v) {
      static_cast<Fixture *>(p)->byte(at, v);
    };
    return m;
  }
  BattleApReportServices services() {
    BattleApReportServices s;
    s.memory = memory();
    s.user = this;
    s.allocate = [](void *p, uint32_t bytes) -> std::optional<uint32_t> {
      auto &f = *static_cast<Fixture *>(p);
      assert(bytes == 12);
      ++f.allocations;
      return f.allocReady ? std::optional<uint32_t>(O) : std::nullopt;
    };
    s.mario = [](void *, uint32_t b) {
      assert(b == B);
      return M;
    };
    s.party = [](void *p, uint32_t b) {
      assert(b == B);
      return static_cast<Fixture *>(p)->partner;
    };
    s.kindCount = [](void *p, int32_t type) {
      assert(type >= 0 && type < 13);
      return static_cast<Fixture *>(p)->counts[type];
    };
    s.wordFloat = [](void *, int32_t v) { return float(v); };
    s.fma = [](void *, float a, float b, float c) { return std::fma(a, b, c); };
    s.add = [](void *, float a, float b) { return a + b; };
    s.multiply = [](void *, float a, float b) { return a * b; };
    s.squareRoot = [](void *, double v) { return std::sqrt(v); };
    s.roundFloat = [](void *, double v) { return float(v); };
    s.truncate = [](void *, float v) -> int32_t { return int32_t(v); };
    s.breakTurn = [](void *p) { return static_cast<Fixture *>(p)->turn; };
    s.breakSlot = [](void *) { return S; };
    s.random = [](void *p, int32_t bound) {
      auto &f = *static_cast<Fixture *>(p);
      assert(bound == 100);
      ++f.draws;
      return f.draw;
    };
    s.pointIncrement = [](void *p) {
      ++static_cast<Fixture *>(p)->increments;
      return true;
    };
    return s;
  }
};
int main() {
  Fixture empty;
  empty.words[Fixture::I] = 77;
  assert(packedBattleInviteApReport(empty.services()) &&
         empty.allocations == 1 && empty.draws == 1 && !empty.increments &&
         empty.words[Fixture::A + 0x137c8] == Fixture::O &&
         empty.words[Fixture::O] == 77 && !empty.words[Fixture::I]);
  assert(packedBattleInviteApReport(empty.services()) &&
         empty.allocations == 1 && empty.draws == 2);
  Fixture fail;
  fail.allocReady = false;
  fail.words[Fixture::I] = 88;
  assert(!packedBattleInviteApReport(fail.services()) &&
         fail.words[Fixture::I] == 88 && !fail.draws);
  Fixture gain;
  gain.counts[0] = 1;
  gain.real(0x803b4de4 + 0x14, 4);
  gain.real(Fixture::I + 4, 1);
  gain.byte(Fixture::I + 8, 1);
  gain.words[Fixture::A + 0x137c4] = 5;
  assert(packedBattleBuildApReport(Fixture::I, gain.services()) &&
         gain.words[Fixture::A + 0x137c4] == 13);
  gain.turn = 1;
  gain.real(Fixture::S + 0x14, 2);
  gain.words[Fixture::A + 0x137c4] = 0;
  assert(packedBattleBuildApReport(Fixture::I, gain.services()) &&
         gain.words[Fixture::A + 0x137c4] == 16);
  gain.partner = Fixture::P;
  gain.words[Fixture::M + 0x138] = 0x20000000;
  gain.words[Fixture::P + 0x138] = 0x10000000;
  gain.words[Fixture::A + 0x137c4] = 0;
  assert(packedBattleBuildApReport(Fixture::I, gain.services()) &&
         gain.words[Fixture::A + 0x137c4] == 8);
  gain.draw = 0;
  gain.byte(Fixture::I + 0xa, 1);
  assert(packedBattleBuildApReport(Fixture::I, gain.services()) &&
         gain.increments == 1);
  auto s = gain.services();
  s.pointIncrement = nullptr;
  assert(!packedBattleInviteApReport(s) && gain.byte(Fixture::I + 0xa) == 1);
  Fixture actors;
  const auto row = Fixture::A + 0x1bc;
  actors.words[row] = 1;
  actors.byte(row + 0x1b, 3);
  assert(packedBattleAudiencePpKindCount(3, actors.memory()) == 1);
  for (uint8_t state : {uint8_t(12), uint8_t(13), uint8_t(17)}) {
    actors.byte(row + 0x19, state);
    assert(packedBattleAudiencePpKindCount(3, actors.memory()) == 0);
  }
  actors.byte(row + 0x19, 14);
  assert(packedBattleAudiencePpKindCount(3, actors.memory()) == 1);
  actors.words[row] = 0x101;
  assert(packedBattleAudiencePpKindCount(3, actors.memory()) == 0);
}
