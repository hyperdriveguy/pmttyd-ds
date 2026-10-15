#include "battle_action_class_table_continuation.hpp"
#include <cassert>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t B = 0x10000, M = B + 0x171c, U = 0x40000, P = 0x50000,
                   S = 0x60000, D = 0x70000, C = M + 0x498;
struct Fixture {
  std::map<uint32_t, uint32_t> w{
      {B + 0x420, 1}, {B + 0x2738, S}, {S + 12, D},
      {U + 8, 222},   {C + 4, 55},     {M, 99},
      {M + 4, 123},   {M + 0x540, 77}, {M + 200, 0x9876}};
  std::map<uint32_t, uint8_t> bytes{{P + 0x98, 1}, {P + 0x99, 1}};
  std::map<uint32_t, uint16_t> halves{{M + 0x16, 0xabcd}};
  std::vector<std::array<uint32_t, 4>> trace;
  uint32_t mutation = 0, messageValue = 0x80000;
  unsigned accepted = 0, hold = 0, flagReads = 0;
  bool recurse = false;
  BattleActionClassTableCursor *cursor = nullptr;
  uint32_t word(uint32_t a) {
    uint32_t v = w[a];
    if (mutation == 3 && a == B + 0xef4)
      v = ++flagReads == 1 ? 0x08000000 : 0;
    trace.push_back({0, a, v});
    return v;
  }
  void put(uint32_t a, uint32_t v) {
    w[a] = v;
    trace.push_back({1, a, v});
    if (mutation == 5 && a == C + 8)
      w[a] = 2;
  }
  uint8_t byte(uint32_t a) {
    auto v = bytes[a];
    trace.push_back({2, a, v});
    return v;
  }
  uint16_t half(uint32_t a) {
    auto v = halves[a];
    trace.push_back({3, a, v});
    return v;
  }
  void ph(uint32_t a, uint16_t v) {
    halves[a] = v;
    trace.push_back({4, a, v});
  }
  bool call(uint32_t fn, uint32_t a = 0, uint32_t b = 0) {
    if (recurse) {
      assert(cursor);
      assert(!resumePackedBattleActionClassTable(B, services(), *cursor));
      assert(!cursor->reset());
    }
    if (hold && accepted + 1 == hold)
      return false;
    ++accepted;
    trace.push_back({5, fn, a, b});
    return true;
  }
  BattleActionClassServices services() {
    BattleActionClassServices s;
    s.user = this;
    s.memory.user = this;
    s.memory.memory.user = this;
    s.memory.memory.word = [](void *p, uint32_t a) -> uint32_t {
      return static_cast<Fixture *>(p)->word(a);
    };
    s.memory.memory.storeWord = [](void *p, uint32_t a, uint32_t v) {
      static_cast<Fixture *>(p)->put(a, v);
    };
    s.memory.memory.half = [](void *p, uint32_t a) -> uint16_t {
      return static_cast<Fixture *>(p)->half(a);
    };
    s.memory.memory.storeHalf = [](void *p, uint32_t a, uint16_t v) {
      static_cast<Fixture *>(p)->ph(a, v);
    };
    s.memory.byte = [](void *p, uint32_t a) -> uint8_t {
      return static_cast<Fixture *>(p)->byte(a);
    };
    s.unit = [](void *p, uint32_t b, int32_t id) -> std::optional<uint32_t> {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.call(0x800f88cc, b, uint32_t(id)))
        return {};
      return U;
    };
    s.pouch = [](void *p) -> std::optional<uint32_t> {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.call(0x800d5eb0))
        return {};
      return P;
    };
    s.message = [](void *p, uint32_t key) -> std::optional<uint32_t> {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.call(0x80081b8c, key))
        return {};
      if (f.mutation == 1) {
        if (key == 0x802f0018) {
          f.w[U + 8] = 224;
          f.bytes[P + 0x98] = 2;
        }
        if (key == 0x802f002c)
          f.w[U + 8] = 222;
        if (key == 0x802f0044)
          f.bytes[P + 0x99] = 3;
        if (key == 0x802f0074)
          f.w[B + 0xef4] = 0x08000000;
        if (key == 0x802f008c)
          f.w[B + 0xef4] |= 0x10000000;
      }
      if (f.mutation == 2 && key == 0x802f0074)
        f.w[B + 0xef4] = 0x08000000;
      if (f.mutation == 4 && key == 0x802f0018) {
        f.w[S + 12] = 0x71000;
        f.w[0x71004] = 0;
        f.w[D + 4] = 0x02000000;
        f.w[0x8041eb00] = 0x52000;
        f.bytes[P + 0x99] = 2;
        f.bytes[0x52099] = 0;
      }
      return f.messageValue ? key + 4 : uint32_t(0);
    };
    return s;
  }
};
void compare(Fixture input, uint32_t battle = B) {
  auto original = input;
  const auto expected =
      makePackedBattleActionClassTable(battle, original.services());
  assert(expected);
  for (unsigned hold = 0; hold <= original.accepted; ++hold) {
    auto f = input;
    f.hold = hold;
    BattleActionClassTableCursor c;
    if (hold) {
      assert(!resumePackedBattleActionClassTable(battle, f.services(), c));
      const auto trace = f.trace;
      const auto words = f.w;
      const auto bytes = f.bytes;
      const auto halves = f.halves;
      assert(c.active() && c.pending());
      assert(!resumePackedBattleActionClassTable(battle, f.services(), c));
      assert(f.trace == trace && f.w == words && f.bytes == bytes &&
             f.halves == halves);
      auto wrong = f.services();
      wrong.user = &original;
      assert(!resumePackedBattleActionClassTable(battle, wrong, c));
      wrong = f.services();
      wrong.memory.audienceLifecycle.user = &original;
      assert(!resumePackedBattleActionClassTable(battle, wrong, c));
      wrong = f.services();
      wrong.message = nullptr;
      assert(!resumePackedBattleActionClassTable(battle, wrong, c));
      assert(!resumePackedBattleActionClassTable(battle + 4, f.services(), c));
      assert(f.trace == trace);
    }
    f.hold = 0;
    assert(resumePackedBattleActionClassTable(battle, f.services(), c) ==
           expected);
    assert(!c.active() && !c.pending());
    assert(f.trace == original.trace && f.w == original.w &&
           f.bytes == original.bytes && f.halves == original.halves);
  }
}
} // namespace
int main() {
  compare(Fixture{});
  for (uint32_t kind :
       {0u, 221u, 222u, 223u, 224u, 225u, 230u, 231u, 0xffffffffu})
    for (uint32_t boots : {0u, 1u, 2u, 3u, 127u, 128u, 255u})
      for (uint32_t hammer : {0u, 1u, 2u, 3u, 127u, 128u, 255u}) {
        Fixture f;
        f.w[U + 8] = kind;
        f.bytes[P + 0x98] = uint8_t(boots);
        f.bytes[P + 0x99] = uint8_t(hammer);
        compare(f);
      }
  for (uint32_t battleFlags :
       {0u, 0x00800000u, 0x01000000u, 0x02000000u, 0x04000000u, 0x08000000u,
        0x10000000u, 0xffffffffu})
    for (uint32_t actorFlags : {0u, 16u})
      for (uint32_t definitionFlags :
           {0u, 0x01000000u, 0x02000000u, 0x03000000u}) {
        Fixture f;
        f.w[B + 0xef4] = battleFlags;
        f.w[U + 0x104] = actorFlags;
        f.w[D + 4] = definitionFlags;
        f.halves[P + 0x8c] = 1;
        compare(f);
      }
  for (uint32_t kind : {222u, 224u, 230u})
    for (uint32_t block : {0u, 1u, 255u}) {
      Fixture f;
      f.w[U + 8] = kind;
      f.bytes[U + 0x12b] = uint8_t(block);
      f.bytes[U + 0x300] = uint8_t(block);
      f.bytes[U + 0x2ff] = uint8_t(block);
      f.halves[U + 0x13c] = uint16_t(block);
      f.halves[U + 0x13e] = uint16_t(block);
      compare(f);
    }
  for (uint32_t counter : {0u, 1u, 0x7fffffffu, 0x80000000u, 0xffffffffu}) {
    Fixture f;
    f.w[B + 0x19050] = counter;
    f.w[B + 0x19054] = counter;
    f.w[B + 0x19058] = counter;
    compare(f);
  }
  for (uint32_t index : {0u, 4u, 7u, 0x7fffffffu, 0x80000000u, 0xffffffffu}) {
    Fixture f;
    f.w[C] = index;
    compare(f);
  }
  for (uint32_t mutation : {1u, 2u, 3u, 4u, 5u}) {
    Fixture f;
    f.mutation = mutation;
    if (mutation == 1)
      f.halves[P + 0x8c] = 1;
    if (mutation == 3)
      f.w[U + 8] = 223;
    if (mutation == 5)
      f.w[C] = 7;
    compare(f);
  }
  Fixture seven;
  seven.mutation = 1;
  seven.halves[P + 0x8c] = 1;
  BattleActionClassTableCursor c;
  assert(resumePackedBattleActionClassTable(B, seven.services(), c) == 7);
  const std::array<uint32_t, 7> types{0, 6, 1, 2, 4, 5, 13};
  for (unsigned i = 0; i < 7; ++i)
    assert(seven.w[M + 20 * i + 8] == types[i]);
  assert(seven.w[C + 8] == 7 && seven.w[M + 0x4a4 + 8] == 0 &&
         seven.halves[M + 0x14] == 335 && seven.halves[M + 40 + 0x14] == 339);
  Fixture zeroLabel;
  zeroLabel.messageValue = 0;
  compare(zeroLabel);
  Fixture wrapped;
  const uint32_t b = 0u - 0x171c;
  wrapped.w[b + 0x420] = 17;
  wrapped.w[b + 0x2738] = S;
  compare(wrapped, b);
  assert(resumePackedBattleActionClassTable(b, wrapped.services(), c) == P);
  // A held first message cannot replay emitted row stores or its old gates.
  // Later icon level and subsequent class eligibility remain fresh.
  Fixture late;
  late.hold = 3;
  assert(!resumePackedBattleActionClassTable(B, late.services(), c));
  assert(c.pending() == 0x80081b8c && late.w[M + 12] == 1);
  late.bytes[P + 0x98] = 2;
  late.w[U + 0x104] = 16;
  late.w[U + 8] = 224;
  late.hold = 0;
  assert(resumePackedBattleActionClassTable(B, late.services(), c) == 4);
  assert(late.w[M + 12] == 1 && late.halves[M + 0x14] == 335 &&
         late.w[C + 8] == 4);
  Fixture actor;
  actor.hold = 1;
  assert(!resumePackedBattleActionClassTable(B, actor.services(), c));
  actor.w[B + 0x420] = 33;
  actor.hold = 0;
  assert(resumePackedBattleActionClassTable(B, actor.services(), c));
  bool oldId = false;
  for (const auto &r : actor.trace)
    if (r[0] == 5 && r[1] == 0x800f88cc)
      oldId = r[3] == 1;
  assert(oldId);
  Fixture setup;
  setup.hold = 2;
  assert(!resumePackedBattleActionClassTable(B, setup.services(), c));
  setup.w[S + 12] = 0x71000;
  setup.w[0x71004] = 0x01000000;
  setup.hold = 0;
  assert(resumePackedBattleActionClassTable(B, setup.services(), c));
  assert(setup.w[M + 12] == 0 && setup.w[M + 24] == 3);
  Fixture no;
  auto unavailable = no.services();
  unavailable.unit = nullptr;
  assert(!resumePackedBattleActionClassTable(B, unavailable, c));
  assert(no.trace.size() == 1);
  const auto trace = no.trace;
  assert(!resumePackedBattleActionClassTable(B, unavailable, c) &&
         no.trace == trace);
  assert(c.reset());
  Fixture re;
  re.recurse = true;
  re.cursor = &c;
  assert(resumePackedBattleActionClassTable(B, re.services(), c));
  return 0;
}
