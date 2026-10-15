#include "battle_audience_attack_events.hpp"
#include "battle_event_callbacks.hpp"
#include "positional_sfx_registry.hpp"
#include <cassert>
#include <cstring>
#include <map>
#include <vector>
using namespace ttyd;
struct Fixture {
  static constexpr uint32_t B = 0x10000, A = B + 0x2858,
                            R = A + 0x1bc + 5 * 0x134, ARGS = 0x80000;
  std::map<uint32_t, uint32_t> memory;
  std::vector<std::array<uint32_t, 4>> trace;
  EventEntry event;
  BattleAudienceAttackEventCursor cursor;
  unsigned blocked = 0;
  int32_t soundHandle = -1;
  bool reenter = false;
  static uint32_t bits(float f) {
    uint32_t n;
    std::memcpy(&n, &f, 4);
    return n;
  }
  Fixture() {
    event.lifetime = 1;
    event.id = 7;
    event.currentCommand = 0x8037350c;
    event.arguments = ARGS;
    memory[0x8041eb90] = B;
    memory[A + 0x138bc] = 5;
    memory[A + 0x138c0] = bits(1.8f);
    memory[A + 0x138c4] = bits(-2.8f);
    memory[A + 0x138c8] = bits(3.9f);
    memory[R] = 5;
    memory[R + 0x48] = bits(4);
    memory[R + 0x4c] = bits(5);
    memory[R + 0x50] = bits(6);
    memory[0x80422fac] = bits(32);
    memory[0x80422fb0] = bits(1);
    memory[ARGS] = 42;
    memory[ARGS + 4] = 0;
  }
  uint32_t function = 0x801465e0;
  bool effect(unsigned kind, uint32_t a, uint32_t b, uint32_t c) {
    if (reenter) {
      assert(!resumeBattleAudienceAttackEvent(event, function, services(),
                                              cursor));
      assert(!cursor.reset());
    }
    if (blocked == kind)
      return false;
    trace.push_back({kind, a, b, c});
    return true;
  }
  BattleAudienceAttackServices services() {
    BattleAudienceAttackServices s;
    s.user = this;
    s.actors.memory.user = this;
    s.actors.memory.word = [](void *p, uint32_t a) -> uint32_t {
      auto &f = *static_cast<Fixture *>(p);
      const auto n = f.memory[a];
      f.trace.push_back({0, a, n, 0});
      return n;
    };
    s.actors.memory.storeWord = [](void *p, uint32_t a, uint32_t n) {
      auto &f = *static_cast<Fixture *>(p);
      f.memory[a] = n;
      f.trace.push_back({1, a, n, 0});
    };
    s.values.user = this;
    s.values.resolve = [](void *p, EventEntry &, int32_t n) -> int32_t {
      auto &f = *static_cast<Fixture *>(p);
      f.trace.push_back({2, uint32_t(n), 0, 0});
      return n;
    };
    s.values.store = [](void *p, EventEntry &e, int32_t at, int32_t n) {
      auto &f = *static_cast<Fixture *>(p);
      e.locals[uint32_t(at) & 15] = n;
      f.trace.push_back({3, uint32_t(at), uint32_t(n), 0});
    };
    s.hit = [](void *p, int32_t a, int32_t b, const std::array<float, 3> &v,
               float scale) {
      assert(a == 0 && b == 3 && scale == 1);
      auto &f = *static_cast<Fixture *>(p);
      if (!f.effect(4, bits(v[0]), bits(v[1]), bits(v[2])))
        return false;
      f.memory[R + 0x4c] = bits(9);
      return true;
    };
    s.sound = [](void *p, uint32_t key,
                 const std::array<float, 3> &v) -> std::optional<int32_t> {
      auto &f = *static_cast<Fixture *>(p);
      assert(key == 0x802f429c);
      if (!f.effect(5, bits(v[0]), bits(v[1]), bits(v[2])))
        return {};
      return f.soundHandle;
    };
    s.pitch = [](void *p, int32_t h, int32_t v) {
      auto &f = *static_cast<Fixture *>(p);
      assert(v == 1023);
      return f.effect(6, uint32_t(h), uint32_t(v), 0);
    };
    s.truncate = [](void *p, float n) -> std::optional<int32_t> {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.effect(7, bits(n), 0, 0))
        return {};
      return int32_t(n);
    };
    return s;
  }
  bool run() {
    return resumeBattleAudienceAttackEvent(event, function, services(),
                                           cursor) == 2;
  }
};
int main() {
  Fixture ref;
  assert(ref.run());
  assert(ref.memory[Fixture::R] == 0x45);
  assert(ref.trace.back()[0] == 1); // Flag40 follows all effects/audio.
  for (unsigned stage : {4u, 5u, 6u}) {
    Fixture f;
    f.blocked = stage;
    assert(!f.run());
    auto prefix = f.trace;
    auto memory = f.memory;
    assert(!f.run() && f.trace == prefix && f.memory == memory);
    assert(f.memory[Fixture::R] == 5);
    auto wrong = f.services();
    wrong.actors.audienceLifecycle.user = &ref;
    assert(!resumeBattleAudienceAttackEvent(f.event, f.function, wrong,
                                            f.cursor) &&
           f.trace == prefix);
    f.blocked = 0;
    assert(f.run());
    assert(f.trace == ref.trace && f.memory == ref.memory);
  }
  Fixture mode;
  mode.memory[Fixture::ARGS + 4] = 1;
  assert(mode.run());
  for (const auto &r : mode.trace)
    assert(r[0] != 4 && r[0] != 5 && r[0] != 6);
  assert(mode.memory[Fixture::R] == 0x45);
  Fixture cached;
  cached.blocked = 4;
  assert(!cached.run());
  cached.memory[Fixture::A + 0x138bc] = 9;
  cached.memory[Fixture::A + 0x138c0] = Fixture::bits(100);
  cached.blocked = 0;
  assert(cached.run());
  assert(cached.memory[Fixture::R] ==
         0x45); // Captured cached slot survives wait.
  Fixture outputs;
  outputs.function = 0x801466c4;
  for (unsigned i = 0; i < 4; ++i)
    outputs.memory[Fixture::ARGS + 4 * i] = i;
  assert(outputs.run());
  assert(outputs.event.locals[0] == 5 && outputs.event.locals[1] == 1 &&
         outputs.event.locals[2] == -2 && outputs.event.locals[3] == 3);
  Fixture held;
  held.function = 0x801466c4;
  for (unsigned i = 0; i < 4; ++i)
    held.memory[Fixture::ARGS + 4 * i] = i;
  held.blocked = 7;
  assert(!held.run());
  auto prefix = held.trace;
  assert(!held.run() && prefix == held.trace);
  held.memory[Fixture::A + 0x138c0] = Fixture::bits(99);
  held.blocked = 0;
  assert(held.run());
  assert(held.event.locals[1] == 1 && held.event.locals[2] == -2);
  assert(held.trace == outputs.trace); // No repeated slot output, retained XYZ.
  Fixture skip;
  skip.function = 0x801466c4;
  for (unsigned i = 0; i < 4; ++i)
    skip.memory[Fixture::ARGS + 4 * i] = uint32_t(-250000000);
  auto ss = skip.services();
  ss.values.store = nullptr;
  ss.truncate = nullptr;
  assert(resumeBattleAudienceAttackEvent(skip.event, skip.function, ss,
                                         skip.cursor) == 2);
  Fixture re;
  re.reenter = true;
  assert(re.run());
  PositionalSfxRegistry sounds;
  assert(setQueuedBattleAudienceAttackPitch(sounds, -1, 1023) == true);
  assert(setQueuedBattleAudienceAttackPitch(sounds, 0, 1023) == true);
  sounds.slots[0].key = 123;
  sounds.slots[0].flags = 1;
  sounds.slots[0].generation = 99;
  assert(setQueuedBattleAudienceAttackPitch(sounds, 0x10100, 65537) == true &&
         sounds.slots[0].pitch == 1);
  sounds.slots[0].flags = 0;
  assert(!setQueuedBattleAudienceAttackPitch(sounds, 0, 1023));
  assert(!setQueuedBattleAudienceAttackPitch(sounds, 40, 1023));
  // Actual VM registry/value adaptation, using source EVT local destinations.
  EventNativeRuntime vm(0x81000000, {});
  assert(vm.memory.registerRam(0x8041eb90, std::vector<uint8_t>(4)));
  assert(vm.memory.registerRam(Fixture::B, std::vector<uint8_t>(0x17000)));
  assert(vm.memory.registerRam(Fixture::ARGS, std::vector<uint8_t>(16)));
  auto put = [&](uint32_t a, uint32_t n) { assert(vm.memory.writeWord(a, n)); };
  put(0x8041eb90, Fixture::B);
  put(Fixture::A + 0x138bc, 5);
  for (unsigned i = 0; i < 3; ++i)
    put(Fixture::A + 0x138c0 + 4 * i, Fixture::bits(float(i + 1)));
  for (unsigned i = 0; i < 4; ++i)
    put(Fixture::ARGS + 4 * i, 0xfe363c80 + i);
  vm.platform.truncate = [](void *, float n) -> int32_t { return int32_t(n); };
  BattleAudienceAttackEvents backend(vm);
  EventCallbackRegistry registry;
  assert(backend.bind(registry));
  assert(!backend.bind(registry));
  EventEntry e;
  assert(vm.entryAddresses.registerEntries(0x82000000, &e, 1));
  e.lifetime = 3;
  e.id = 9;
  e.arguments = Fixture::ARGS;
  e.currentCommand = 0x803733cc;
  int32_t result = -1;
  assert(registry.invoke(e, 0x801466c4, 1, result) && result == 2);
  assert(e.locals[0] == 5 && e.locals[1] == 1 && e.locals[2] == 2 &&
         e.locals[3] == 3);
  // Actual registered mode1 executes cached target flag40 without inventing
  // an effect/audio provider; the real script mode0 stays at its effect
  // frontier.
  assert(vm.memory.registerRam(0x80422fac, {0x42, 0, 0, 0, 0x3f, 0x80, 0, 0}));
  put(Fixture::ARGS, 123);
  put(Fixture::ARGS + 4, 1);
  put(Fixture::R, 5);
  e.currentCommand = 0x8037350c;
  assert(registry.invoke(e, 0x801465e0, 1, result) && result == 2);
  uint32_t flags = 0;
  assert(vm.memory.readWord(Fixture::R, flags) && flags == 0x45);
  put(Fixture::R, 5);
  put(Fixture::ARGS + 4, 0);
  e.currentCommand = 0x80373b48;
  assert(registry.invoke(e, 0x801465e0, 1, result) && result == 0);
  put(Fixture::A + 0x138bc, 9);
  assert(registry.invoke(e, 0x801465e0, 0, result) && result == 0);
  assert(vm.memory.readWord(Fixture::R, flags) && flags == 5);
  return 0;
}
