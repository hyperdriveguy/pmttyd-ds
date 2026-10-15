#include "battle_audience_item_on.hpp"
#include "battle_audience_selected.hpp"
#include "battle_weapon_menu_main.hpp"
#include <cassert>
#include <cstring>
#include <map>
using namespace ttyd;
namespace {
struct Fixture {
  static constexpr uint32_t B = 0x10000, A = B + 0x2858, U = 0x60000,
                            Output = 0x70000;
  std::map<uint32_t, uint32_t> words;
  std::map<uint32_t, uint16_t> halves;
  std::map<uint32_t, uint8_t> bytes;
  std::vector<std::array<uint32_t, 3>> trace;
  std::shared_ptr<const uint8_t> owner = std::make_shared<const uint8_t>(0);
  BattleAudienceSelectedInput input{owner, A, {5, 1, 7, 8}};
  BattleAudienceSelectedLive live{owner, A, {input.target, true, true, true}};
  bool marioReady = true, endReady = true, lightReady = true,
       mutatePositionOnCache = false;
  unsigned marioCalls = 0, endCalls = 0, lightCalls = 0;
  uint32_t endType = 0, lightFlags = 0;
  static uint32_t member(unsigned slot) { return A + 0x1bc + slot * 0x134; }
  static uint32_t item(unsigned index) { return A + 0xf25c + index * 0x48; }
  void real(uint32_t p, float v) {
    uint32_t n;
    std::memcpy(&n, &v, 4);
    words[p] = n;
  }
  float real(uint32_t p) {
    float v;
    const auto n = words[p];
    std::memcpy(&v, &n, 4);
    return v;
  }
  Fixture() {
    words[0x8041eb90] = B;
    words[B + 0xef4] = 0x80;
    words[B + 0x1c5c] = 3;
    words[U + 0x104] = 0;
    // Actual fixture records: first eligible holder is good, chosen slot5
    // harmful.
    for (unsigned slot : {0u, 5u}) {
      const unsigned index = slot ? 1 : 0;
      words[member(slot)] = 5;
      bytes[member(slot) + 0x19] = 7;
      halves[member(slot) + 0x1c] = uint16_t(index);
      words[item(index)] = 3;
      words[item(index) + 4] = 1;
      words[item(index) + 0xc] = slot;
      words[item(index) + 0x10] = slot ? 236 : 121;
      real(member(slot) + 0x48, float(slot + 10));
      real(member(slot) + 0x4c, float(slot + 20));
      real(member(slot) + 0x50, float(slot + 30));
    }
    real(0x804249cc, 3);
    real(0x80424988, 0);
  }
  BattleActorServices actors() {
    BattleActorServices a;
    a.user = this;
    a.memory.user = this;
    a.memory.word = [](void *p, uint32_t at) {
      auto &f = *static_cast<Fixture *>(p);
      const auto n = f.words[at];
      f.trace.push_back({0, at, n});
      return n;
    };
    a.memory.half = [](void *p, uint32_t at) {
      auto &f = *static_cast<Fixture *>(p);
      const auto n = f.halves[at];
      f.trace.push_back({1, at, n});
      return n;
    };
    a.byte = [](void *p, uint32_t at) {
      auto &f = *static_cast<Fixture *>(p);
      const auto n = f.bytes[at];
      f.trace.push_back({2, at, n});
      return n;
    };
    a.memory.storeWord = [](void *p, uint32_t at, uint32_t n) {
      auto &f = *static_cast<Fixture *>(p);
      f.words[at] = n;
      f.trace.push_back({3, at, n});
      if (f.mutatePositionOnCache && at == A + 0x138bc)
        f.real(member(5) + 0x48, 999);
    };
    a.memory.storeHalf = [](void *p, uint32_t at, uint16_t n) {
      auto &f = *static_cast<Fixture *>(p);
      f.halves[at] = n;
      f.trace.push_back({4, at, n});
    };
    a.memory.storeByte = [](void *p, uint32_t at, uint8_t n) {
      auto &f = *static_cast<Fixture *>(p);
      f.bytes[at] = n;
      f.trace.push_back({5, at, n});
    };
    return a;
  }
  BattleAudienceSelectedServices services() {
    BattleAudienceSelectedServices s;
    s.memory = actors();
    s.user = this;
    s.mario = [](void *p, uint32_t b) -> std::optional<uint32_t> {
      auto &f = *static_cast<Fixture *>(p);
      assert(b == B);
      ++f.marioCalls;
      if (!f.marioReady)
        return {};
      return U;
    };
    s.endActMenu = [](void *p, uint32_t b) {
      auto &f = *static_cast<Fixture *>(p);
      assert(b == B);
      ++f.endCalls;
      if (!f.endReady)
        return false;
      if (f.endType)
        f.words[B + 0x1c5c] = f.endType;
      return true;
    };
    s.lightOff = [](void *p, int32_t x, int32_t y) {
      auto &f = *static_cast<Fixture *>(p);
      assert(x == 0 && y == 1);
      ++f.lightCalls;
      if (!f.lightReady)
        return false;
      if (f.lightFlags)
        f.words[B + 0xef4] = f.lightFlags;
      return true;
    };
    return s;
  }
  bool writes() const {
    for (const auto &op : trace)
      if (op[0] >= 3)
        return true;
    return false;
  }
  unsigned stores(uint32_t at) const {
    unsigned n = 0;
    for (const auto &op : trace)
      if (op[0] >= 3 && op[1] == at)
        ++n;
    return n;
  }
};
void queryAndCache() {
  Fixture f;
  auto a = f.actors();
  auto holder = inspectSelectedBattleAudienceHolder(f.input, f.live, a);
  assert(holder && holder->member == Fixture::member(5) &&
         holder->item == Fixture::item(1) && holder->kind == 236);
  assert(!f.writes());
  // X still performs the original first-eligible query, including good gifts.
  packedBattleAudienceItemOn({Fixture::Output, 0, 0, 0, 0}, a);
  assert(f.words[Fixture::Output] == 0 && f.words[Fixture::A + 0x138bc] == 0);
  f.trace.clear();
  f.mutatePositionOnCache = true;
  assert(cacheSelectedBattleAudienceHolder(f.input, f.live, a));
  assert(f.words[Fixture::A + 0x138bc] == 5);
  assert(f.real(Fixture::A + 0x138c0) ==
         999); // Source index publication precedes fresh XYZ reads.
  assert(f.real(Fixture::A + 0x138c4) == 25 &&
         f.real(Fixture::A + 0x138c8) == 38);
  std::vector<uint32_t> stores;
  for (const auto &op : f.trace)
    if (op[0] == 3)
      stores.push_back(op[1]);
  assert((stores ==
          std::vector<uint32_t>{Fixture::A + 0x138bc, Fixture::A + 0x138c0,
                                Fixture::A + 0x138c4, Fixture::A + 0x138c8}));
  // A stale inspected result cannot authorize a subsequent write.
  f.words[Fixture::item(1) + 0x10] = 121;
  f.trace.clear();
  assert(!cacheSelectedBattleAudienceHolder(f.input, f.live, a));
  assert(!f.writes());
}
void queryBoundaries() {
  for (uint32_t kind :
       {0u, 121u, 235u, 236u, 237u, 238u, 239u, 240u, 0xffffffffu}) {
    Fixture f;
    f.words[Fixture::item(1) + 0x10] = kind;
    assert(bool(inspectSelectedBattleAudienceHolder(
               f.input, f.live, f.actors())) == (kind >= 236 && kind < 240));
    assert(!f.writes());
  }
  for (uint32_t state : {0u, 1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u, 0xffffffffu}) {
    Fixture f;
    f.words[Fixture::item(1) + 4] = state;
    assert(bool(inspectSelectedBattleAudienceHolder(f.input, f.live,
                                                    f.actors())) ==
           (state <= 4 || state == 7 || state == 0xffffffffu));
  }
  for (uint32_t flags : {0u, 1u, 5u, 3u, 0x101u, 0x105u, 0x21u}) {
    Fixture f;
    f.words[Fixture::member(5)] = flags;
    assert(bool(inspectSelectedBattleAudienceHolder(f.input, f.live,
                                                    f.actors())) ==
           bool((flags & 1) && !(flags & 0x102)));
  }
  for (unsigned failure = 0; failure < 12; ++failure) {
    Fixture f;
    switch (failure) {
    case 0:
      f.live.owner = std::make_shared<const uint8_t>(0);
      break;
    case 1:
      ++f.live.audience;
      break;
    case 2:
      ++f.live.candidate.target.actorGeneration;
      break;
    case 3:
      ++f.live.candidate.target.itemGeneration;
      break;
    case 4:
      f.live.candidate.visible = false;
      break;
    case 5:
      f.live.candidate.dangerous = false;
      break;
    case 6:
      f.live.candidate.interruptible = false;
      break;
    case 7:
      f.halves[Fixture::member(5) + 0x1c] = 0xffff;
      break;
    case 8:
      f.halves[Fixture::member(5) + 0x1c] = 0;
      break;
    case 9:
      f.words[0x8041eb90] = 0x90000;
      break;
    case 10:
      f.input.target.slot = f.live.candidate.target.slot = 200;
      break;
    case 11:
      f.input.target.itemGeneration = f.live.candidate.target.itemGeneration =
          0;
      break;
    }
    assert(!inspectSelectedBattleAudienceHolder(f.input, f.live, f.actors()));
    assert(!cacheSelectedBattleAudienceHolder(f.input, f.live, f.actors()));
    assert(!f.writes());
  }
}
void retainedInterruption() {
  for (unsigned blocked = 0; blocked < 3; ++blocked) {
    Fixture f;
    f.endType = 9;
    f.lightFlags = 0x100;
    f.marioReady = blocked != 0;
    f.endReady = blocked != 1;
    f.lightReady = blocked != 2;
    auto s = f.services();
    BattleAudienceSelectedCursor c;
    assert(!resumeSelectedBattleAudienceInterruption(f.input, f.live, s, c));
    assert(c.active());
    const auto trace = f.trace;
    const auto chosenStores = f.stores(Fixture::member(5));
    assert(!resumeSelectedBattleAudienceInterruption(f.input, f.live, s, c));
    assert(f.trace == trace);
    auto wrong = s;
    wrong.user = nullptr;
    assert(
        !resumeSelectedBattleAudienceInterruption(f.input, f.live, wrong, c));
    assert(f.trace == trace);
    auto different = f.input;
    ++different.target.itemGeneration;
    assert(!resumeSelectedBattleAudienceInterruption(different, f.live, s, c));
    assert(f.trace == trace);
    if (blocked == 1) {
      assert(f.words[Fixture::member(5)] & 0x20);
      assert(f.words[Fixture::item(1)] == 3 &&
             f.halves[Fixture::member(5) + 0x1c] == 1);
      assert(f.words[Fixture::item(0)] == 0 &&
             f.halves[Fixture::member(0) + 0x1c] == 0xffff);
      // A committed selection retains original operands even if display
      // eligibility changes.
      f.live.candidate.visible = false;
      f.live.candidate.dangerous = false;
      f.live.candidate.interruptible = false;
    }
    f.marioReady = f.endReady = f.lightReady = true;
    const auto result =
        resumeSelectedBattleAudienceInterruption(f.input, f.live, s, c);
    assert(result && *result && !c.active());
    assert(f.words[Fixture::A + 0x138bc] == 5);
    assert(f.words[Fixture::B + 0x1c60] == 9 &&
           f.words[Fixture::B + 0x1c5c] == 11 &&
           f.words[Fixture::B + 0x171c] == 29);
    assert(f.words[Fixture::B + 0xef4] == 0x104);
    assert(f.stores(Fixture::A + 0x138bc) == 1 &&
           f.stores(Fixture::member(5)) == 1);
    if (blocked != 0)
      assert(chosenStores == 1);
  }
}
void originalTailComparison() {
  Fixture original, selected;
  // When source X independently chooses the same slot, every cache/target/UI
  // source store agrees (apart from X's private query output).
  for (auto *f : {&original, &selected}) {
    f->words[Fixture::member(0)] = 0;
    f->endType = 9;
    f->lightFlags = 0x100;
  }
  BattleMenuAudienceServices s;
  s.memory = original.actors();
  s.user = &original;
  s.privateItem = Fixture::Output;
  s.trigger = [](void *, uint32_t mask) {
    assert(mask == 0x400);
    return mask;
  };
  s.mario = [](void *, uint32_t b) {
    assert(b == Fixture::B);
    return Fixture::U;
  };
  s.itemOn = [](void *p, uint32_t output, int32_t, int32_t, int32_t, int32_t) {
    auto &f = *static_cast<Fixture *>(p);
    packedBattleAudienceItemOn({output, 0, 0, 0, 0}, f.actors());
    return true;
  };
  s.setTarget = [](void *p, int32_t target) {
    auto &f = *static_cast<Fixture *>(p);
    packedBattleAudienceSetTarget(target, f.actors());
  };
  s.endActMenuReady = [](void *p, uint32_t b) {
    auto &f = *static_cast<Fixture *>(p);
    assert(b == Fixture::B);
    f.words[b + 0x1c5c] = f.endType;
    return true;
  };
  s.lightOff = [](void *p, int32_t x, int32_t y) {
    auto &f = *static_cast<Fixture *>(p);
    assert(x == 0 && y == 1);
    f.words[Fixture::B + 0xef4] = f.lightFlags;
  };
  BattleMenuAudienceCursor originalCursor;
  const auto ordinary = resumePackedBattleMenuAudienceCheck(s, originalCursor);
  assert(ordinary && *ordinary);
  auto adapted = selected.services();
  BattleAudienceSelectedCursor selectedCursor;
  const auto result = resumeSelectedBattleAudienceInterruption(
      selected.input, selected.live, adapted, selectedCursor);
  assert(result && *result);
  const auto writes = [](const Fixture &f) {
    std::vector<std::array<uint32_t, 3>> result;
    for (const auto &op : f.trace)
      if (op[0] >= 3 && op[1] != Fixture::Output)
        result.push_back(op);
    return result;
  };
  assert(writes(original) == writes(selected));
}
void rejectAndRetire() {
  for (unsigned kind = 0; kind < 3; ++kind) {
    Fixture f;
    if (kind == 0)
      f.words[Fixture::B + 0xef4] |= 0x40000000;
    else if (kind == 1)
      f.words[Fixture::U + 0x104] = 0x10;
    else
      f.words[Fixture::item(1) + 0x10] = 121;
    auto s = f.services();
    BattleAudienceSelectedCursor c;
    const auto result =
        resumeSelectedBattleAudienceInterruption(f.input, f.live, s, c);
    assert(result && !*result && !c.active());
    assert(!f.writes());
  }
  Fixture f;
  f.endReady = false;
  auto s = f.services();
  BattleAudienceSelectedCursor c;
  assert(!resumeSelectedBattleAudienceInterruption(f.input, f.live, s, c));
  const auto writes = f.trace;
  auto lost = f.live;
  lost.candidate.target.itemGeneration++;
  const auto result =
      resumeSelectedBattleAudienceInterruption(f.input, lost, s, c);
  assert(result && !*result && !c.active());
  assert(writes == f.trace);
  assert(f.endCalls ==
         1); // No release of storage belonging to a replacement lifetime.
}
} // namespace
int main() {
  queryAndCache();
  queryBoundaries();
  retainedInterruption();
  rejectAndRetire();
  originalTailComparison();
}
