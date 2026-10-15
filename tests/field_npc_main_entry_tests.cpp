#include "animation_initialize.hpp"
#include "animation_pool_memory.hpp"
#include "animation_tangent_memory.hpp"
#include "event_native_runtime.hpp"
#include "field_npc_main_entry_binding.hpp"
#include "display_queue.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <limits>
#include <map>
#include <tuple>
using namespace ttyd;
namespace {
constexpr uint32_t Actor = 0x10000, GP = 0x20000, Pose = 0x30000,
                   Mario = 0x40000;
uint32_t bits(float f) {
  uint32_t v;
  std::memcpy(&v, &f, 4);
  return v;
}
float real(uint32_t v) {
  float f;
  std::memcpy(&f, &v, 4);
  return f;
}
struct Fixture {
  std::map<uint32_t, uint32_t> words;
  std::map<uint32_t, uint16_t> halves;
  using Effect = std::tuple<char, uint32_t, uint64_t>;
  std::vector<Effect> trace;
  std::vector<FieldNpcMainEntryRequest> calls;
  unsigned attempts = 0, fail = ~0u, sounds = 0;
  bool kpa = false, changeFlags = false, clearQuery = false,
       changeContext = false, soundReady = true;
  Fixture() {
    for (unsigned n = 0; n < 2; ++n) {
      auto a = Actor + n * 0x340;
      for (unsigned o = 0; o < 0x340; o += 4)
        words[a + o] = 0;
      halves[a + 0x320] = 0;
      halves[a + 0x31e] = 0;
    }
    words[Actor] = 3 | 0x8000 | 0x100000;
    words[Actor + 0x1d4] = 0x10010;
    words[Actor + 0x1a4] = bits(7.f);
    halves[Actor + 0x320] = 0xffff;
    words[0x80415fd0] = GP;
    words[GP + 0x14] = 0;
    words[GP + 0x38] = 0;
    words[GP + 0x3c] = 1032750;
    words[0x8041e8ac] = UINT32_MAX;
    words[0x8041e8b8] = 2;
    words[0x8041e8c0] = Actor;
    words[Pose] = 0x20;
    words[Mario + 0x1d8] = 0x1234;
    words[Mario + 0x1dc] = 0x5678;
    words[0x800000f8] = 243000000;
    words[0x8041fd50] = bits(1000000.f);
    words[0x8041fc88] = bits(365.f);
    words[0x8041fcf0] = bits(100.f);
    words[0x8041fc80] = bits(.5f);
    words[0x8041fc94] = bits(1.f);
  }
  bool admitted() { return attempts++ != fail; }
  FieldNpcMainEntryServices services() {
    FieldNpcMainEntryServices s;
    s.clock.user = this;
    s.clock.word = [](void *p, uint32_t a) -> std::optional<uint32_t> {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.admitted())
        return {};
      auto it = f.words.find(a);
      if (it == f.words.end())
        return {};
      f.trace.emplace_back('r', a, it->second);
      return it->second;
    };
    s.clock.store = [](void *p, uint32_t a, uint32_t v) {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.admitted())
        return false;
      f.words[a] = v;
      f.trace.emplace_back('w', a, v);
      return true;
    };
    s.clock.divide = [](void *p, uint64_t n, uint32_t d, uint64_t &out) {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.admitted() || !nativeFieldNpcClockDivide(nullptr, n, d, out))
        return false;
      f.trace.emplace_back('d', d, n);
      if (f.clearQuery)
        f.words[Actor] &= ~2u;
      return true;
    };
    s.clock.convert = [](void *p, uint64_t n, float &out) {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.admitted())
        return false;
      nativeFieldNpcClockConvert(nullptr, n, out);
      f.trace.emplace_back('f', bits(out), n);
      return true;
    };
    s.half = [](void *p, uint32_t a) -> std::optional<uint16_t> {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.admitted() || !f.halves.count(a))
        return {};
      f.trace.emplace_back('h', a, f.halves[a]);
      return f.halves[a];
    };
    s.storeHalf = [](void *p, uint32_t a, uint16_t v) {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.admitted())
        return false;
      f.halves[a] = v;
      f.trace.emplace_back('H', a, v);
      return true;
    };
    s.bank = [](void *p, uint32_t b, uint32_t count, uint32_t) {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.admitted())
        return false;
      f.trace.emplace_back('b', b, count);
      return true;
    };
    s.request = [](void *p, const FieldNpcMainEntryRequest &q, uint32_t &out) {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.admitted() || (q.function == 0x800d9848 && !f.soundReady))
        return false;
      f.trace.emplace_back('c', q.function, q.args[0]);
      f.calls.push_back(q);
      if (q.function == 0x8005e0c0) {
        out = Mario;
        if (f.changeContext)
          f.words[GP + 0x14] = 1;
      } else if (q.function == 0x80039cf8) {
        out = Pose;
        if (f.changeFlags)
          f.words[Actor] &= ~0x8001u;
      } else if (q.function == 0x8026d464) {
        assert(q.args[0] == GP + 0x12c && q.args[1] == 0x802c1394);
        out = f.kpa ? 0 : 1;
      } else if (q.function == 0x800d9848) {
        assert(q.args[0] == 0x802c139c && q.args[1] == Actor + 0x8c);
        ++f.sounds;
        out = 0;
      } else
        assert(false);
      return true;
    };
    return s;
  }
  FieldNpcMainFrontier run(FieldNpcMainEntryCursor &c) {
    return advanceFieldNpcMainEntry(services(), c);
  }
};
void ordinary() {
  Fixture f;
  FieldNpcMainEntryCursor c;
  assert(f.run(c) == FieldNpcMainFrontier::Movement && c.active &&
         c.actor == Actor && c.factor == 1);
  assert(f.words[0x8041e8ac] == 0 && f.words[Mario + 0x1dc] == 0 &&
         f.words[Mario + 0x1d8] == 0x1234 && f.words[Pose] == 0x120);
  assert(f.halves[Actor + 0x320] == 0xfffe &&
         f.words[Actor + 0x1a4] == bits(7.f) &&
         f.words[Actor + 0x19c] == 1032750);
  assert(f.words[Actor + 0x1d4] == 0x10000 && f.words[Actor + 0x1d0] == 0 &&
         f.words[Actor + 0x98] == 0);
  const std::vector<Fixture::Effect> expected = {
      {'r', 0x80415fd0, GP},
      {'r', GP + 0x14, 0},
      {'c', 0x8005e0c0, 0},
      {'w', Mario + 0x1dc, 0},
      {'r', 0x8041e8ac, UINT32_MAX},
      {'r', 0x8041e8b8, 2},
      {'r', 0x8041e8c0, Actor},
      {'b', 0x8041e8b4, 2},
      {'w', 0x8041e8ac, 0},
      {'r', Actor, 3 | 0x8000 | 0x100000},
      {'r', Actor + 0x104, 0},
      {'c', 0x80039cf8, 0},
      {'r', Pose, 0x20},
      {'w', Pose, 0x120},
      {'r', Actor, 3 | 0x8000 | 0x100000},
      {'r', Actor + 0x1d4, 0x10010},
      {'w', Actor + 0x1d4, 0x10000},
      {'h', Actor + 0x320, 0xffff},
      {'H', Actor + 0x320, 0xfffe},
      {'r', Actor + 0x1d4, 0x10000},
      {'r', 0x80415fd0, GP},
      {'r', GP + 0x38, 0},
      {'r', GP + 0x3c, 1032750},
      {'w', Actor + 0x19c, 1032750},
      {'w', Actor + 0x198, 0},
      {'r', Actor, 3 | 0x8000 | 0x100000},
      {'r', Actor + 0x1d4, 0x10000},
      {'w', Actor + 0x1d4, 0x10000},
      {'r', Actor + 0x1d4, 0x10000},
      {'w', Actor + 0x1d4, 0x10000},
      {'r', 0x80415fd0, GP},
      {'c', 0x8026d464, GP + 0x12c},
      {'r', Actor + 0x1d4, 0x10000},
      {'r', 0x8041fc94, bits(1.f)},
      {'r', Actor + 0x1d4, 0x10000}};
  assert(f.trace == expected);
  const auto visits = f.attempts;
  assert(f.run(c) == FieldNpcMainFrontier::Movement &&
         f.attempts ==
             visits); // Frontier cannot silently finish/visit next actor.
  Fixture captured;
  captured.changeFlags = true;
  captured.changeContext = true;
  FieldNpcMainEntryCursor cc;
  assert(captured.run(cc) == FieldNpcMainFrontier::Movement &&
         cc.bank == 0x8041e8b4 && captured.words[Pose] == 0x120 &&
         !(captured.words[Actor] & 1));
}
void gates() {
  for (uint32_t count : {0u, 0xffffffffu, 0x80000000u}) {
    Fixture f;
    f.words[0x8041e8b8] = count;
    FieldNpcMainEntryCursor c;
    assert(f.run(c) == FieldNpcMainFrontier::FieldTail &&
           f.words[0x8041e8ac] == 0 && f.calls.size() == 1);
  }
  for (uint32_t flags : {0u, 1u, 2u}) {
    Fixture f;
    f.words[Actor] = flags;
    FieldNpcMainEntryCursor c;
    assert(f.run(c) == FieldNpcMainFrontier::FieldTail &&
           f.words[Actor + 0x1d4] == 0x10010 &&
           f.halves[Actor + 0x320] == 0xffff && f.calls.size() == 1);
  }
  Fixture fresh;
  fresh.words[Actor] = 3;
  fresh.words[Actor + 0x1d4] = 0;
  fresh.clearQuery = true;
  FieldNpcMainEntryCursor c;
  assert(fresh.run(c) == FieldNpcMainFrontier::FieldTail &&
         fresh.words[Actor + 0x1d4] == 0 &&
         fresh.words[Actor + 0x1a4] == bits(.017f));
  assert(
      fresh.words[Actor + 0x98] ==
      0); // Fresh query rejection clears10, bypasses snapshot and actor body.
  Fixture alternate;
  alternate.words[GP + 0x14] = 0x1234;
  alternate.words[0x8041e8cc] = 0;
  alternate.words[0x8041e8d4] = 0xdeadbeef;
  FieldNpcMainEntryCursor ac;
  assert(alternate.run(ac) == FieldNpcMainFrontier::FieldTail &&
         ac.bank == 0x8041e8c8);
}
void special() {
  for (float height : {365.f, 366.f, std::numeric_limits<float>::quiet_NaN(),
                       -std::numeric_limits<float>::infinity(),
                       std::numeric_limits<float>::infinity()}) {
    Fixture f;
    f.kpa = true;
    f.words[Actor + 0x90] = bits(height);
    FieldNpcMainEntryCursor c;
    assert(f.run(c) == FieldNpcMainFrontier::Movement);
    const bool entered = !(height > 365);
    assert(bool(f.words[Actor + 0x1d4] & 0x100000) == entered &&
           f.sounds == unsigned(entered) && c.factor == (entered ? .5f : 1.f));
    assert(f.words[Actor + 0x1d0] == (entered ? bits(100.f) : 0));
    assert(f.halves[Actor + 0x31e] == unsigned(entered));
  }
  Fixture retained;
  retained.kpa = true;
  retained.words[Actor + 0x1d4] |= 0x100000;
  FieldNpcMainEntryCursor c;
  assert(retained.run(c) == FieldNpcMainFrontier::Movement &&
         retained.sounds == 0 && (retained.words[Actor + 0x1d4] & 0x200000));
  Fixture noHalf;
  noHalf.kpa = true;
  noHalf.words[Actor + 0x1d4] |= 0x10000000;
  noHalf.words[Actor + 0x1d0] = bits(17.f);
  FieldNpcMainEntryCursor nh;
  assert(noHalf.run(nh) == FieldNpcMainFrontier::Movement && nh.factor == 1 &&
         noHalf.words[Actor + 0x1d0] == bits(17.f) &&
         noHalf.halves[Actor + 0x31e] == 1);
  Fixture audio;
  audio.kpa = true;
  audio.soundReady = false;
  FieldNpcMainEntryCursor hold;
  assert(audio.run(hold) == FieldNpcMainFrontier::Pending && hold.phase == 37 &&
         (audio.words[Actor + 0x1d4] & 0x100000));
  auto trace = audio.trace;
  assert(audio.run(hold) == FieldNpcMainFrontier::Pending &&
         audio.trace == trace);
  audio.soundReady = true;
  assert(audio.run(hold) == FieldNpcMainFrontier::Movement &&
         audio.sounds == 1);
}
void holds() {
  for (unsigned path = 0; path < 4; ++path) {
    auto setup = [&](Fixture &f) {
      if (path == 1) {
        f.kpa = true;
        f.words[Actor + 0x1d4] = 0;
        f.words[Actor] = 3;
      }
      if (path == 2) {
        f.words[Actor] = 3;
        f.words[Actor + 0x1d4] = 0;
        f.clearQuery = true;
      }
      if (path == 3)
        f.words[Actor] = 0;
    };
    Fixture baseline;
    setup(baseline);
    FieldNpcMainEntryCursor complete;
    const auto result = baseline.run(complete);
    assert(result != FieldNpcMainFrontier::Pending);
    for (unsigned failure = 0; failure < baseline.attempts; ++failure) {
      Fixture f;
      setup(f);
      f.fail = failure;
      FieldNpcMainEntryCursor c;
      assert(f.run(c) == FieldNpcMainFrontier::Pending && c.active);
      auto other = f.services();
      other.clock.user = &baseline;
      const auto before = f.trace;
      assert(advanceFieldNpcMainEntry(other, c) ==
                 FieldNpcMainFrontier::Pending &&
             f.trace == before);
      assert(f.run(c) == result && f.words == baseline.words &&
             f.halves == baseline.halves && f.trace == baseline.trace &&
             f.sounds == baseline.sounds);
    }
  }
}
struct NativeWorld {
  std::array<EventEntry, 3> fieldEvents{}, battleEvents{};
  std::shared_ptr<BattlePosePool> pool = std::make_shared<BattlePosePool>();
  std::unique_ptr<EventNativeRuntime> vm;
  std::unique_ptr<AnimationPoolMemory> view;
  std::unique_ptr<AnimationTangentMemory> tangents;
  std::shared_ptr<FieldNpcRegistryOwner> owner;
  std::unique_ptr<FieldNpcRegistryBinding> registry;
  std::unique_ptr<FieldNpcMainEntryBinding> entry;
  uint32_t next = 0x8a000000, actor = 0, pose = 0;
  bool marioReady = false, movementReady = false;
  unsigned movementCalls = 0, poseClockCalls = 0;
  bool poseClockReady = false, cloudReady = false, interruptReady = false;
  unsigned cloudCalls = 0, interruptCalls = 0, vivianCalls = 0;
  bool marioServicesReady = false;
  bool displayReady = false, queueReady = false;
  std::unique_ptr<DisplayQueue> displayQueue = std::make_unique<DisplayQueue>();
  BattlePoseClock selectedClock{1234, 5678};
  static constexpr uint32_t Work = 0x803d9470, M = 0x8d000000;
  uint32_t word(uint32_t a) {
    uint32_t v;
    assert(vm->memory.readWord(a, v));
    return v;
  }
  void store(uint32_t a, uint32_t v) { assert(vm->memory.writeWord(a, v)); }
  NativeWorld() {
    EventNativePlatform platform;
    platform.clock = [](void *) -> uint32_t { return 243000000; };
    vm = std::make_unique<EventNativeRuntime>(0x81000000, platform);
    assert(
        vm->memory.registerRam(0x80415dc0, std::vector<uint8_t>(4)) &&
        vm->memory.registerRam(Work, std::vector<uint8_t>(0x110 + 178 * 4)) &&
        vm->memory.registerRam(0x8041fbb4, std::vector<uint8_t>(4)) &&
        vm->memory.registerRam(0x802c1070, std::vector<uint8_t>(8)));
    store(0x80415dc0, Work);
    store(0x8041fbb4, 0x3c8efa33);
    store(0x802c1070, 0x43300000);
    store(0x802c1074, 0x80000000);
    AnimationInitializeServices s;
    s.memory.user = this;
    s.memory.word = [](void *p, uint32_t a) -> std::optional<uint32_t> {
      uint32_t v;
      return static_cast<NativeWorld *>(p)->vm->memory.readWord(a, v)
                 ? std::optional<uint32_t>(v)
                 : std::nullopt;
    };
    s.memory.store = [](void *p, uint32_t a, uint32_t v) {
      return static_cast<NativeWorld *>(p)->vm->memory.writeWord(a, v);
    };
    s.memory.call = [](void *p, BattleSequenceCall &c) {
      auto &f = *static_cast<NativeWorld *>(p);
      if (c.function == 0x800300f0) {
        assert(c.args[0] == 0);
        c.result = f.next;
        f.next += c.args[1] + 0x100;
        return f.vm->memory.registerRam(c.result,
                                        std::vector<uint8_t>(c.args[1]));
      }
      if (c.function == 0x8018a61c) {
        c.result = 0;
        return true;
      }
      return c.function == 0x80189d38;
    }; // Explicit allocation and null-file fixtures, not production file/SDK
       // startup.
    s.doubleWord = [](void *p, uint32_t a) -> std::optional<uint64_t> {
      auto &f = *static_cast<NativeWorld *>(p);
      return uint64_t(f.word(a)) << 32 | f.word(a + 4);
    };
    s.tangent = nativeAnimationTangent;
    AnimationInitializeCursor init;
    assert(initializeAnimation(s, init));
    view = AnimationPoolMemory::bind(pool, vm->memory);
    assert(view && view->aliases(*pool, vm->memory));
    tangents = AnimationTangentMemory::bind(pool, vm->memory);
    assert(tangents && tangents->current());
    EventScriptAssets source;
    assert(source.load("romfs/door-data.pmevt") &&
           vm->memory.importScripts(source));
    owner = FieldNpcRegistryOwner::cold(0x85000000, pool);
    registry = FieldNpcRegistryBinding::bind(owner, vm->memory);
    assert(registry && registry->initialize());
    auto resource =
        loadBattlePoseResource("romfs/field-native-c_kuri_h.pose-directory.bin",
                               "romfs/field-native-c_kuri_h.pmanim");
    assert(resource && owner->poses().install(resource));
    std::vector<uint8_t> model(64);
    for (unsigned i = 0; i < 64; ++i)
      model[i] = resource->draw->byte(i + 4);
    assert(vm->memory.registerRam(0x86000000, std::move(model)));
    const auto name = word(0x90524f10 + 0x5c);
    assert(registry->enter(name, 0x86000000, {}));
    auto found = registry->lookup(name);
    assert(found && *found);
    actor = *found;
    pose = word(actor + 0x104);
    store(actor, word(actor) | 0x8000);
    assert(vm->memory.registerRam(0x8041e900, std::vector<uint8_t>(4)) &&
           vm->memory.registerRam(M, std::vector<uint8_t>(0x300)));
    store(0x8041e900, M);
    store(M + 0x1d8, 0x12345678);
    store(M + 0x1dc, 0x87654321);
    FieldNpcMainEntryNativeServices n;
    n.user = this;
    n.display = [](void *p, EventAddressSpace &memory,
                   const FieldNpcDisplayRequest &q, FieldNpcDisplayResponse &out) {
      auto &f = *static_cast<NativeWorld *>(p);
      assert(&memory == &f.vm->memory);
      if (!f.displayReady) return false;
      if (q.function == 0x8000f2fc) {
        assert(q.args[0] == 4); out.word = 0x8e000000; return true;
      }
      if (q.function == 0x8029e6a4) {
        assert(q.args[0] == 0x8e00011c && q.args[1] == f.actor + 0x8c);
        out.point = {1,2,3}; return true;
      }
      if (q.function == 0x8029eb08) {
        assert(q.args[0] == 0x8e00015c);
        out.point = {0,0,-.5f}; return true;
      }
      assert(q.function == 0x80010bc4);
      if (!f.queueReady) return false;
      return f.displayQueue->enter(q.args[0], q.args[1], q.args[2], q.args[3],
                                  q.depth) != DisplayAdmission::Unavailable;
    }; // Explicit camera/matrix fixtures; actual shared native queue.

    n.numbers.divide = nativeFieldNpcClockDivide;
    n.numbers.convert = nativeFieldNpcClockConvert;
    n.marioDetection = [](void *p, EventAddressSpace &memory,
                          const FieldNpcMarioRequest &q,
                          FieldNpcMarioResponse &out) {
      auto &f = *static_cast<NativeWorld *>(p);
      assert(&memory == &f.vm->memory);
      if (!f.marioServicesReady)
        return false;
      if (q.function == 0x801865b8) {
        ++f.vivianCalls;
        out.word = 0;
        return true;
      }
      assert(q.function == 0x80007328 || q.function == 0x8000703c ||
             q.function == 0x800071f0);
      out.scalar = 0;
      return true;
    }; // Explicit Vivian/angle fixtures; sequence/Kpa MUST use actual VM words.
    n.detection = [](void *p, EventAddressSpace &memory,
                     const FieldNpcDetectionRequest &q,
                     FieldNpcDetectionResponse &out) {
      auto &f = *static_cast<NativeWorld *>(p);
      assert(&memory == &f.vm->memory);
      if (q.function == 0x80187f10) {
        if (!f.cloudReady)
          return false;
        ++f.cloudCalls;
        out.power = 1;
        assert(q.point[0] == f.word(f.actor + 0x8c));
        return true;
      }
      assert(q.function == 0x800440f8 && q.argument == f.actor);
      if (!f.interruptReady)
        return false;
      ++f.interruptCalls;
      out.word = 0xdeadbeef;
      return true;
    }; // Explicit cloud/interrupt fixtures; lookup MUST use the real VM.
    n.estimate = nativeFieldNpcReciprocalSqrtEstimate;
    n.poseClock = [](void *p, BattlePoseClock &out) {
      auto &f = *static_cast<NativeWorld *>(p);
      if (!f.poseClockReady)
        return false;
      ++f.poseClockCalls;
      out = f.selectedClock;
      return true;
    }; // Explicit source-clock quotient fixture, never a rendering-frame clock.
    n.movement = [](void *p, EventAddressSpace &memory,
                    const FieldNpcMainMoveRequest &q,
                    FieldNpcMainMoveResponse &) {
      auto &f = *static_cast<NativeWorld *>(p);
      if (!f.movementReady)
        return false;
      assert(&memory == &f.vm->memory && q.args[0] == f.actor &&
             q.function == 0x80048d9c);
      ++f.movementCalls;
      return memory.writeWord(q.args[0] + 0x90, bits(9));
    }; // Explicit gravity fixture, not a replacement production child.
    n.admitMario = [](void *p, EventAddressSpace &, uint32_t m) {
      return static_cast<NativeWorld *>(p)->marioReady && m == M;
    };
    entry = FieldNpcMainEntryBinding::bind(owner, *registry, *vm, *view, n);
    assert(entry);
    auto foreign = std::make_unique<EventNativeRuntime>(0x82000000, platform);
    assert(
        !FieldNpcMainEntryBinding::bind(owner, *registry, *foreign, *view, n));
    BattlePosePool other;
    assert(!view->aliases(other, vm->memory));
  }
};
void actualAliases() {
  NativeWorld f;
  FieldNpcMainEntryCursor c;
  assert(f.entry->advance(c) == FieldNpcMainFrontier::Pending && c.phase == 2 &&
         f.word(0x8041e8ac) == 0 &&
         f.word(NativeWorld::M + 0x1dc) == 0x87654321);
  f.marioReady = true;
  f.vm->gp.timestamp = 1032750;
  assert(f.entry->advance(c) == FieldNpcMainFrontier::Movement &&
         c.actor == f.actor && c.factor == 1);
  assert(f.word(NativeWorld::M + 0x1dc) == 0 &&
         f.word(NativeWorld::M + 0x1d8) == 0x12345678 &&
         f.word(0x8041e8ac) == 1 && f.word(f.actor + 0x1a4) == bits(.017f));
  assert(f.pool->pose(int32_t(f.pose)) &&
         (f.pool->pose(int32_t(f.pose))->word(0) & 0x100) &&
         f.word(f.view->poseAddress(f.pose)) ==
             f.pool->pose(int32_t(f.pose))->word(0));
  // Compose the movement section using the exact retained actor/VM. Missing
  // child services suspend after earlier original publications, without a
  // replay of Main entry, clock, or position updates.
  f.store(f.actor, f.word(f.actor) | 0x40000008);
  f.store(f.actor + 0x1b4, bits(4));
  f.store(f.actor + 0x1bc, bits(6));
  FieldNpcMainMoveCursor move;
  const auto oldX = real(f.word(f.actor + 0x8c));
  assert(f.entry->advanceMovement(c, move) == FieldNpcMoveFrontier::Pending &&
         move.phase == 29 && f.movementCalls == 0);
  assert(real(f.word(f.actor + 0x8c)) == oldX + 4 &&
         f.word(f.actor + 0x1b4) == 0 && f.word(0x8041e8ac) == 1);
  f.movementReady = true;
  assert(f.entry->advanceMovement(c, move) ==
             FieldNpcMoveFrontier::AutomaticPose &&
         f.movementCalls == 1 && move.delta[1] == 9 && c.phase == 60);
  assert(f.entry->advanceMovement(c, move) ==
             FieldNpcMoveFrontier::AutomaticPose &&
         f.movementCalls == 1 && f.word(0x8041e8ac) == 1);
  // Use the same real pose for automatic selection, retaining the descriptor
  // and local movement delta. Clock unavailability must not restart the pose
  // or publish phase; the later real shared-pool restart happens once.
  constexpr uint32_t Descriptor = 0x8e000000, AnimName = 0x8e000100;
  assert(f.vm->memory.registerRam(Descriptor, std::vector<uint8_t>(64)));
  const auto *poseBefore = f.pool->pose(int32_t(f.pose));
  assert(poseBefore && poseBefore->resource->names.size() > 1);
  const auto originalName = poseBefore->resource->names.at(1);
  std::vector<uint8_t> nameBytes(originalName.begin(), originalName.end());
  nameBytes.push_back(0);
  assert(f.vm->memory.registerRam(AnimName, std::move(nameBytes)));
  f.store(f.actor, f.word(f.actor) | 0x400);
  f.store(f.actor + 0x28, Descriptor);
  f.store(Descriptor + 0x3c, bits(10000));
  f.store(Descriptor + 0x18, AnimName);
  f.store(f.actor + 0x108, 0);
  FieldNpcAutoPoseCursor automatic;
  const auto oldTimestamp = poseBefore->timestamp();
  assert(f.entry->advanceAutomaticPose(c, move, automatic) ==
             FieldNpcMoveFrontier::Pending &&
         automatic.phase == 42 && f.poseClockCalls == 0 &&
         f.pool->pose(int32_t(f.pose))->timestamp() == oldTimestamp &&
         f.word(f.actor + 0x108) == 0);
  f.poseClockReady = true;
  assert(f.entry->advanceAutomaticPose(c, move, automatic) ==
             FieldNpcMoveFrontier::Detection &&
         f.poseClockCalls == 1 && f.word(f.actor + 0x108) == 2);
  const auto *selected = f.pool->pose(int32_t(f.pose));
  const auto expectedStamp = selected->word(0xc) ? f.selectedClock.modeNonzero
                                                 : f.selectedClock.modeZero;
  assert(selected->word(0x14) == 1 && selected->timestamp() == expectedStamp &&
         selected->word(0x3c) == 0xffffffff && selected->word(0x40) == 0 &&
         f.word(f.view->poseAddress(f.pose) + 0x14) == 1);
  assert(f.entry->advanceAutomaticPose(c, move, automatic) ==
             FieldNpcMoveFrontier::Detection &&
         f.poseClockCalls == 1 && f.word(0x8041e8ac) == 1 && c.phase == 60);
  // Raw nonnegative selector is an index, including null=>descriptor0. Force1
  // restarts even though the two changed parent phases can select the same
  // clip.
  for (uint32_t selector : {1u, 0u, 0x7fffffffu}) {
    f.store(Descriptor + 0x18, selector);
    f.store(f.actor + 0x108, 0);
    f.selectedClock.modeZero += 9;
    f.selectedClock.modeNonzero += 9;
    FieldNpcAutoPoseCursor numeric;
    assert(f.entry->advanceAutomaticPose(c, move, numeric) ==
           FieldNpcMoveFrontier::Detection);
    selected = f.pool->pose(int32_t(f.pose));
    assert(selected->word(0x14) == (selector == 1 ? 1u : 0u));
    assert(selected->timestamp() == (selected->word(0xc)
                                         ? f.selectedClock.modeNonzero
                                         : f.selectedClock.modeZero));
  }
  // Source regular-ID0 bypasses lookup and interrupt. Real event lookup uses
  // this VM's current context, first allocated ID match (including stopped),
  // and packed raw byte10 rather than scheduler type+c.
  FieldNpcDetectionCursor noRegular;
  f.store(f.actor + 0x11c, 0);
  assert(f.entry->advanceDetection(c, move, automatic, noRegular) ==
             FieldNpcDetectionFrontier::Display &&
         f.interruptCalls == 0);
  constexpr uint32_t EventBase = 0x8f000000, BattleEventBase = 0x8f100000;
  assert(
      f.vm->entryAddresses.registerEntries(EventBase, f.fieldEvents.data(), 3));
  assert(f.vm->entryAddresses.registerEntries(BattleEventBase,
                                              f.battleEvents.data(), 3));
  f.vm->field.entries = f.fieldEvents.data();
  f.vm->field.capacity = 3;
  f.vm->battle.entries = f.battleEvents.data();
  f.vm->battle.capacity = 3;
  f.fieldEvents[0].id = 42;
  f.fieldEvents[0].flags = 3;
  f.fieldEvents[1].id = 42;
  f.fieldEvents[1].flags = 1;
  f.fieldEvents[1].type = 5;
  f.fieldEvents[1].rawBytes[0] = 1;
  f.store(f.actor + 0x11c, 42);
  f.store(f.actor + 0x13c, 0x90000000);
  f.store(f.actor + 0x140, 0x80);
  FieldNpcDetectionCursor stopped;
  assert(f.entry->advanceDetection(c, move, automatic, stopped) ==
             FieldNpcDetectionFrontier::Pending &&
         stopped.phase == 181 && stopped.event == EventBase &&
         f.cloudCalls == 0);
  f.interruptReady = true;
  assert(f.entry->advanceDetection(c, move, automatic, stopped) ==
             FieldNpcDetectionFrontier::Display &&
         f.interruptCalls == 1);
  f.fieldEvents[0].flags = 0;
  FieldNpcDetectionCursor cloud;
  assert(f.entry->advanceDetection(c, move, automatic, cloud) ==
             FieldNpcDetectionFrontier::Pending &&
         cloud.phase == 14 && cloud.eventClass == 1 &&
         cloud.event == EventBase + 0x1b0 && f.cloudCalls == 0);
  f.cloudReady = true;
  assert(f.entry->advanceDetection(c, move, automatic, cloud) ==
             FieldNpcDetectionFrontier::Display &&
         f.cloudCalls == 1 && f.interruptCalls == 2 &&
         f.word(f.actor + 0x140) == 0x88);
  assert(f.entry->advanceDetection(c, move, automatic, cloud) ==
             FieldNpcDetectionFrontier::Display &&
         f.cloudCalls == 1 && f.interruptCalls == 2 && f.word(0x8041e8ac) == 1);
  f.fieldEvents[1].rawBytes[0] = 5;
  f.fieldEvents[1].type = 1;
  f.store(f.actor + 0x1f8, 0);
  FieldNpcDetectionCursor rawClass;
  assert(f.entry->advanceDetection(c, move, automatic, rawClass) ==
             FieldNpcDetectionFrontier::Mario &&
         rawClass.eventClass == 5 && f.cloudCalls == 1);
  // A later live GP context selects battle events, independent of the NPC bank
  // captured by entry. Empty/missing events still enter Interrupt, not success.
  f.vm->gp.battleContext = 1;
  f.battleEvents[0].flags = 1;
  f.battleEvents[0].id = 42;
  f.battleEvents[0].rawBytes[0] = 8;
  FieldNpcDetectionCursor battleEvent;
  assert(f.entry->advanceDetection(c, move, automatic, battleEvent) ==
             FieldNpcDetectionFrontier::Mario &&
         battleEvent.event == BattleEventBase && battleEvent.eventClass == 8 &&
         c.bank == 0x8041e8b4);
  f.vm->gp.battleContext = 0;
  f.store(f.actor + 0x11c, 43);
  FieldNpcDetectionCursor missingEvent;
  assert(f.entry->advanceDetection(c, move, automatic, missingEvent) ==
             FieldNpcDetectionFrontier::Display &&
         missingEvent.event == 0 && f.interruptCalls == 3);
  // Complete the retained Mario stage with actual mutable Mario/sequence
  // bytes. A missing source sequence cannot be inferred from the native scene.
  f.store(f.actor + 0x11c, 42);
  f.store(f.actor + 0x13c, 0);
  f.fieldEvents[1].rawBytes[0] = 2;
  f.store(f.actor + 0x140, 0x80);
  f.store(NativeWorld::M, 2);
  FieldNpcDetectionCursor avoidPrefix;
  FieldNpcMarioCursor avoid;
  assert(
      f.entry->advanceMarioDetection(c, move, automatic, avoidPrefix, avoid) ==
          FieldNpcDetectionFrontier::Display &&
      f.word(f.actor + 0x140) == 0x82 && f.interruptCalls == 4);
  f.store(NativeWorld::M, 0);
  f.store(f.actor + 0x140, 0x80);
  f.store(f.actor + 0x20c, bits(1000));
  f.store(f.actor + 0x218, bits(1000));
  f.store(f.actor + 0x228, bits(1000));
  FieldNpcMarioCursor admitted;
  assert(f.entry->advanceMarioDetection(c, move, automatic, avoidPrefix,
                                        admitted) ==
             FieldNpcDetectionFrontier::Pending &&
         admitted.phase == 106);
  assert(f.vm->memory.registerRam(0x80415190, std::vector<uint8_t>(4)));
  f.store(0x80415190, 2);
  constexpr uint32_t OtherMario = 0x8d001000;
  assert(f.vm->memory.registerRam(OtherMario, std::vector<uint8_t>(0x300)));
  f.store(OtherMario, 0x82000000);
  f.store(0x8041e900, OtherMario);
  assert(f.entry->advanceMarioDetection(c, move, automatic, avoidPrefix,
                                        admitted) ==
             FieldNpcDetectionFrontier::Pending &&
         admitted.phase == 109 && admitted.admissionMario == NativeWorld::M);
  f.marioServicesReady = true;
  f.store(0x80415190,
          1); // already captured eligibility2; no re-request on resume
  assert(f.entry->advanceMarioDetection(c, move, automatic, avoidPrefix,
                                        admitted) ==
             FieldNpcDetectionFrontier::Display &&
         f.word(f.actor + 0x140) == 0x80 && f.vivianCalls == 1 &&
         f.interruptCalls == 5);
  f.store(0x8041e900, NativeWorld::M);
  f.store(0x80415190, 2);
  // Kpa mode2 obtains the real work pointer+298 and level+4; radius table comes
  // from the original pack and numeric phase does not use a guessed level.
  constexpr uint32_t KpaWork = 0x8d010000;
  assert(f.vm->memory.registerRam(KpaWork, std::vector<uint8_t>(8)));
  assert(f.vm->memory.writeByte(NativeWorld::M + 0x3c, 2));
  f.store(NativeWorld::M + 0x298, KpaWork);
  f.store(KpaWork + 4, 3);
  f.store(f.actor, f.word(f.actor) | 0x08000000);
  f.fieldEvents[1].rawBytes[0] = 1;
  f.store(f.actor + 0x220, bits(1000));
  FieldNpcDetectionCursor proximityPrefix;
  FieldNpcMarioCursor proximity;
  assert(f.entry->advanceMarioDetection(c, move, automatic, proximityPrefix,
                                        proximity) ==
             FieldNpcDetectionFrontier::Display &&
         proximity.distance == 0 && f.word(f.actor + 0x140) == 0x81 &&
         f.interruptCalls == 6);
  assert(f.entry->advanceMarioDetection(c, move, automatic, proximityPrefix,
                                        proximity) ==
             FieldNpcDetectionFrontier::Display &&
         f.interruptCalls == 6 && f.word(0x8041e8ac) == 1 && c.phase == 60);
  // Display material leaves must mutate an inactive actual shared pose row.
  const auto beforeDisplayFlags = f.word(f.actor);
  f.store(f.actor, 0x40);
  f.store(f.actor + 0x104, 255);
  f.store(f.actor + 0x114, 0x123456ff);
  f.store(f.actor + 0x1f4, 4);
  f.store(f.view->poseAddress(255) + 0xe8, 0x200);
  FieldNpcDisplayCursor display;
  assert(f.entry->advanceDisplay(c, move, automatic, proximityPrefix, proximity,
                                 display) == FieldNpcDisplayFrontier::Pending);
  assert(display.phase == 2 && f.pool->slot(255)->word(0xe8) == 0x200);
  f.displayReady = true;
  assert(f.entry->advanceDisplay(c, move, automatic, proximityPrefix, proximity,
                                 display) == FieldNpcDisplayFrontier::Pending);
  assert(display.phase == 39 && f.displayQueue->count() == 0 &&
         f.pool->slot(255)->word(0xe8) == 0x240 &&
         f.pool->slot(255)->word(0xf0) == 0x123456ff);
  f.store(f.view->poseAddress(255) + 0xe8, 0x400);
  f.store(f.actor + 0x114, 0xffffffff);
  f.queueReady = true;
  assert(f.entry->advanceDisplay(c, move, automatic, proximityPrefix, proximity,
                                 display) == FieldNpcDisplayFrontier::Shadow);
  assert(f.displayQueue->count() == 3 && f.pool->slot(255)->word(0xe8) == 0x400 &&
         f.pool->slot(255)->word(0xf0) == 0x123456ff);
  for (unsigned i = 0; i < 3; ++i) {
    auto record = f.displayQueue->record(i);
    assert(record && record->camera == 4 && record->drawClass == i &&
           record->callback == 0x8004c50c && record->userdata == f.actor);
  }
  assert(c.phase == 60 && f.word(0x8041e8ac) == 1);
  f.store(f.actor, beforeDisplayFlags);
  // Source getter is address arithmetic, not active/clip readiness. An explicit
  // cold-slot handle input must mutate the SAME inactive row without a new
  // gate.
  f.store(f.actor + 0x104, 255);
  FieldNpcMainEntryCursor cold;
  assert(f.entry->advance(cold) == FieldNpcMainFrontier::Movement &&
         f.pool->slot(255)->word(0) == 0x100);
  f.vm->gp.battleContext = 17;
  FieldNpcMainEntryCursor alternate;
  assert(f.entry->advance(alternate) == FieldNpcMainFrontier::FieldTail &&
         alternate.bank == 0x8041e8c8 && alternate.count == 16 &&
         alternate.index == 16);
  f.vm->gp.battleContext = 0;
  f.store(0x8041e8b8, 37);
  const auto before = f.word(0x8041e8ac);
  FieldNpcMainEntryCursor oversized;
  assert(f.entry->advance(oversized) == FieldNpcMainFrontier::Pending &&
         oversized.phase == 7 &&
         f.word(0x8041e8ac) ==
             before); // Refuse positive out-of-bank domain; never clamp.
}
} // namespace
int main() {
  ordinary();
  gates();
  special();
  holds();
  actualAliases();
}
