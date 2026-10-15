#include "animation_initialize.hpp"
#include "battle_audience_aux_callback.hpp"
#include "npc_presentation.hpp"
#include "animation_pool_memory.hpp"
#include "animation_tangent_memory.hpp"
#include "event_native_runtime.hpp"
#include "field_npc_main_entry_binding.hpp"
#include "display_queue.hpp"
#include "shadow_work.hpp"
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
  bool scanning = false, clockReady = true, pauseClockAfterGravity = false;
  uint32_t movementHoldActor = 0;
  bool poseClockReady = false, cloudReady = false, interruptReady = false;
  unsigned cloudCalls = 0, interruptCalls = 0, vivianCalls = 0;
  bool marioServicesReady = false;
  bool tailReady = false, scoreReady = false;
  uint32_t tailSecondField = 0, tailOtherMario = 0;
  unsigned encounters = 0, scores = 0;
  bool displayReady = false, queueReady = false, shadowReady = false;
  std::unique_ptr<ShadowWork> shadows = std::make_unique<ShadowWork>();
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
    n.tail = [](void *p, EventAddressSpace &memory,
                const FieldNpcTailRequest &q, FieldNpcTailResponse &) {
      auto &f = *static_cast<NativeWorld *>(p);
      assert(&memory == &f.vm->memory);
      if (!f.tailReady) return false;
      if (q.function == 0x8004727c) {
        ++f.encounters;
        return memory.writeWord(f.vm->gpAddresses.currentAddress()+0x168,
                                 f.tailSecondField);
      }
      assert(q.function == 0x80216994);
      if (!f.scoreReady) return false;
      const std::array<uint32_t,3> expected{{bits(3),bits(64),bits(5)}};
      assert(q.args[0]==5 && q.point==expected);
      ++f.scores;return true;
    }; // Explicit encounter/score fixtures; interpolation/Kpa leaves are real.

    n.shadow = [](void *p, EventAddressSpace &memory,
                  const FieldNpcShadowRequest &q, FieldNpcShadowResponse &out) {
      auto &f = *static_cast<NativeWorld *>(p);
      assert(&memory == &f.vm->memory);
      if (!f.shadowReady) return false;
      if (q.function == 0x8001577c) {
        assert(q.scalars[1] == 1 && q.scalars[6] == 100);
        out.word = 0x8f000000; out.hit[1] = -5; return true;
      }
      if (q.function == 0x8002be98) {
        const auto slot = f.shadows->enter(
            {q.scalars[0], q.scalars[1], q.scalars[2]}, q.scalars[3]);
        if (!slot) return false; out.word = *slot; return true;
      }
      assert(q.function == 0x8002bcfc && q.args[0] == 0 && q.args[1] == 0);
      f.shadows->record(0)->flags &= ~1u; return true;
    }; // Explicit collision fixture; SAME actual native shadow pool.
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

    n.numbers.user = this;
    n.numbers.divide = [](void *p, uint64_t v, uint32_t d, uint64_t &out) {
      return static_cast<NativeWorld *>(p)->clockReady &&
             nativeFieldNpcClockDivide(nullptr, v, d, out);
    };
    n.numbers.convert = [](void *p, uint64_t v, float &out) {
      return static_cast<NativeWorld *>(p)->clockReady &&
             nativeFieldNpcClockConvert(nullptr, v, out);
    };
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
      if (!f.movementReady || q.args[0] == f.movementHoldActor)
        return false;
      assert(&memory == &f.vm->memory &&
             (q.args[0] == f.actor || (f.scanning && q.args[0] == f.actor + 2*0x340)) &&
             q.function == 0x80048d9c);
      ++f.movementCalls;
      if (f.pauseClockAfterGravity) f.clockReady = false;
      return memory.writeWord(q.args[0] + 0x90, bits(9));
    }; // Explicit gravity fixture, not a replacement production child.
    n.admitMario = [](void *p, EventAddressSpace &, uint32_t m) {
      const auto &f = *static_cast<NativeWorld *>(p);
      return f.marioReady && (m == M || (f.tailOtherMario && m == f.tailOtherMario));
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
  // Shadow suffix holds before collision, then updates actual inactive pose
  // strided translations and the shared ShadowWork index0, never a fake actor.
  f.store(f.actor, 0x04080000 | 0x100);
  f.store(f.actor + 0x110, 255);
  f.store(f.actor + 0x8c, bits(0));
  f.store(f.actor + 0x90, bits(0));
  f.store(f.actor + 0x94, bits(0));
  f.store(f.actor + 0x14c, bits(6));
  f.store(f.actor + 0xe0, bits(2));
  f.store(f.actor + 0x154, bits(4));
  f.store(f.view->poseAddress(255), 0);
  FieldNpcShadowCursor shadow;
  assert(f.entry->advanceShadow(c, move, automatic, proximityPrefix, proximity,
                                display, shadow) == FieldNpcDisplayFrontier::Pending);
  assert(shadow.phase == 57 && f.shadows->count() == 0 &&
         f.pool->slot(255)->word(0) == 0);
  f.shadowReady = true;
  f.queueReady = false;
  assert(f.entry->advanceShadow(c, move, automatic, proximityPrefix, proximity,
                                display, shadow) == FieldNpcDisplayFrontier::Pending);
  assert(shadow.phase == 92 && f.shadows->count() == 1 &&
         f.shadows->record(0)->position[1] == 0 &&
         f.shadows->record(0)->radius == 16 && f.shadows->record(0)->flags == 0 &&
         f.pool->slot(255)->word(0) == 0x20 &&
         f.pool->slot(255)->word(0x130) == bits(0) &&
         f.pool->slot(255)->word(0x140) == bits(-5) &&
         f.pool->slot(255)->word(0x150) == bits(0));
  f.store(f.view->poseAddress(255) + 0x140, bits(42));
  f.queueReady = true;
  assert(f.entry->advanceShadow(c, move, automatic, proximityPrefix, proximity,
                                display, shadow) == FieldNpcDisplayFrontier::History);
  assert(f.shadows->count() == 1 && f.displayQueue->count() == 7 &&
         f.pool->slot(255)->word(0x140) == bits(42) && c.phase == 60 &&
         f.word(0x8041e8ac) == 1);
  assert(f.displayQueue->record(3)->callback == 0x8004bc3c);
  for (unsigned i = 4; i < 7; ++i) {
    auto record = f.displayQueue->record(i);
    assert(record && record->camera == 1 && record->drawClass == i - 4 &&
           record->callback == 0x8004bf28 && record->userdata == f.actor);
  }
  f.store(f.view->poseAddress(255), 0);
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
void actorScan() {
  auto firstWorld=std::make_unique<NativeWorld>();
  auto &f=*firstWorld;
  f.marioReady = true; f.movementReady = true; f.scanning = true;
  f.vm->gp.timestamp = 1032750;
  const auto first = f.actor, second = first + 2*0x340;
  // Actual owned raw bank, two live slots separated by inactive slots.
  for (unsigned o=0; o<0x340; o+=4) f.store(second+o, f.word(first+o));
  f.store(0x8041e8b8, 4); f.store(0x8041e8ac, 1);
  for (auto a : {first, second}) {
    f.store(a, 3 | 0x800 | 0x100 | 0x40000008);
    f.store(a+0x1d4, 0); f.store(a+0x11c, 0);
    f.store(a+0x8c, bits(a==first?10:20));
    f.store(a+0x90, bits(0)); f.store(a+0x94, bits(30));
    f.store(a+0xa4, 111); f.store(a+0xc8, 222);
  }
  for (auto a : {first+0x340, first+3*0x340}) {
    f.store(a, 0); f.store(a+0x98, 333); f.store(a+0x1d4, 0x10);
  }
  auto scanOwner=std::make_unique<FieldNpcMainActorsCursor>();
  auto &scan=*scanOwner;
  f.movementHoldActor = second;
  f.pauseClockAfterGravity = true;
  assert(f.entry->advanceActors(scan) == FieldNpcActorsFrontier::Pending);
  assert(scan.phase==3 && scan.entry.phase==23 && scan.entry.actor==second &&
         scan.entry.clock.active && scan.history.actor==first &&
         scan.history.phase==3 && f.word(first+0x98)==bits(10));
  // Retired body captures remain intact while the next actor's entry suspends.
  f.store(first+0x8c, bits(42));
  assert(f.entry->advanceActors(scan)==FieldNpcActorsFrontier::Pending &&
         f.movementCalls==1 && f.word(first+0x98)==bits(10));
  f.pauseClockAfterGravity = false; f.clockReady = true;
  assert(f.entry->advanceActors(scan)==FieldNpcActorsFrontier::Pending);
  assert(scan.phase==1 && scan.entry.actor==second && scan.entry.index==2 &&
         !scan.entry.clock.active && f.word(second+0x19c)==1032750 && scan.movement.phase==29 &&
         f.movementCalls==1 && f.word(0x8041e8ac)==2 &&
         f.word(first+0x98)==bits(10) && f.word(first+0x9c)==bits(9) &&
         f.word(first+0xa4)==bits(10) && f.word(first+0xd4)==222 &&
         !(f.word(first+0x1d4)&0x10) && f.word(second+0x98)!=bits(20));
  // Captured bank/count/array must survive this later mutation. Late query loss
  // does not cancel an already admitted body or its late history/final clear.
  f.store(0x8041e8b8, 1); f.store(0x8041e8c0, 0);
  f.store(second, f.word(second)&~3u); f.store(0x8041e8ac, 3);
  f.movementHoldActor = 0;
  assert(f.entry->advanceActors(scan) == FieldNpcActorsFrontier::FieldTail);
  assert(scan.entry.phase==90 && scan.entry.index==4 &&
         scan.entry.actor==first+4*0x340 && f.movementCalls==2 &&
         f.word(0x8041e8ac)==3 && f.word(second+0x98)==bits(20) &&
         f.word(second+0x9c)==bits(9) && f.word(second+0xa4)==111 &&
         !(f.word(second+0x1d4)&0x10) && !scan.history.active && !scan.movement.active);
  for (auto a : {first+0x340, first+3*0x340})
    assert(f.word(a+0x98)==333 && f.word(a+0x1d4)==0x10);
  assert(f.entry->advanceActors(scan)==FieldNpcActorsFrontier::FieldTail &&
         f.movementCalls==2 && f.word(0x8041e8ac)==3);
  auto otherWorld=std::make_unique<NativeWorld>();
  auto &other=*otherWorld;
  assert(other.entry->advanceActors(scan)==FieldNpcActorsFrontier::Pending);
  // Continue the SAME already-retired actor pass into its original field tail.
  const auto TailF=f.owner->fieldWorkAddress();
  constexpr uint32_t TailF2=0x8e010000,
                     OtherPlayer=0x8d020000, BadWork=0x8f300000, FireWork=0x8f400000;
  assert(f.word(f.vm->gpAddresses.currentAddress()+0x168)==TailF &&
         f.vm->memory.registerRam(TailF2,std::vector<uint8_t>(0x580)));
  f.tailSecondField=TailF2;
  f.store(f.vm->gpAddresses.currentAddress()+0x168,TailF);
  f.store(TailF+0x550,1);f.store(TailF+0x554,99);f.store(TailF+0x558,2);
  assert(f.vm->memory.writeHalf(TailF,1));
  assert(f.vm->memory.writeByte(TailF+0x54c,0xbb) &&
         f.vm->memory.writeByte(TailF+0x54d,0xaa) &&
         f.vm->memory.writeByte(TailF+0x54e,0xcc) &&
         f.vm->memory.writeByte(TailF2+0x54d,0xdd));
  f.store(TailF2+0x57c,5);f.store(TailF2+0x578,2);f.store(TailF2+0x574,9);
  f.store(NativeWorld::M,1);f.store(NativeWorld::M+0x298,BadWork);
  assert(f.vm->memory.writeHalf(NativeWorld::M+0x50,0xffff) &&
         f.vm->memory.writeByte(NativeWorld::M+0x3c,2));
  f.store(NativeWorld::M+0x8c,bits(3));f.store(NativeWorld::M+0x90,bits(4));
  f.store(NativeWorld::M+0x94,bits(5));
  auto main=std::make_unique<FieldNpcMainCursor>();main->actors=std::move(scan);
  f.displayReady=true;f.queueReady=false;
  assert(!f.entry->advanceMain(*main) && main->phase==1 && main->tail.phase==27 &&
         f.word(TailF+0x55c)==1 && f.word(TailF+0x560)==0xc4009b33 &&
         f.encounters==0 && f.movementCalls==2 && f.word(0x8041e8ac)==3);
  f.queueReady=true;
  assert(!f.entry->advanceMain(*main) && main->tail.phase==41 &&
         f.displayQueue->count()==1 && f.encounters==0);
  f.tailReady=true;
  assert(!f.entry->advanceMain(*main) && main->tail.phase==48 &&
         f.encounters==1 && main->tail.mario==NativeWorld::M && main->tail.jump==1);
  // Fire helper fetches fresh Mario inside its atomic read-only admission;
  // score still uses the parent's retained original Mario.
  assert(f.vm->memory.registerRam(OtherPlayer,std::vector<uint8_t>(0x300)) &&
         f.vm->memory.registerRam(FireWork,std::vector<uint8_t>(0x140)));
  f.tailOtherMario=OtherPlayer;f.store(OtherPlayer,1);
  f.store(OtherPlayer+0x298,FireWork);f.store(FireWork+0x13c,20);
  f.store(0x8041e900,OtherPlayer);
  assert(!f.entry->advanceMain(*main) && main->tail.phase==56 &&
         main->tail.fire==1 && f.encounters==1 && f.scores==0);
  f.store(NativeWorld::M+0x90,bits(100));f.store(TailF2+0x57c,99);
  f.store(f.vm->gpAddresses.currentAddress()+0x168,0);
  f.scoreReady=true;
  assert(f.entry->advanceMain(*main) && main->phase==2 && f.scores==1 &&
         f.encounters==1 && f.word(TailF2+0x578)==0 && f.word(TailF2+0x57c)==0 &&
         f.word(TailF2+0x574)==9 && f.word(TailF+0x54c)==0xbb00cc00 &&
         f.word(TailF2+0x54c)==0x00dd0000 && f.word(TailF+0x55c)==1);
  assert(f.entry->advanceMain(*main) && f.scores==1 && f.encounters==1 &&
         f.displayQueue->count()==1 && f.word(0x8041e8ac)==3);
  assert(!other.entry->advanceMain(*main));
  // Fresh whole invocation with zero actors reaches an ordinary mode0 tail.
  other.marioReady=true;other.store(0x8041e8b8,0);
  const auto OtherF=other.owner->fieldWorkAddress();
  assert(other.word(other.vm->gpAddresses.currentAddress()+0x168)==OtherF);
  other.store(other.vm->gpAddresses.currentAddress()+0x168,OtherF);
  assert(other.vm->memory.writeByte(OtherF+0x54d,0xaa));
  auto empty=std::make_unique<FieldNpcMainCursor>();
  assert(other.entry->advanceMain(*empty) && empty->phase==2 &&
         other.word(0x8041e8ac)==1 && other.word(OtherF+0x54c)==0);
  // Real Kpa query leaves: signed nonzero jump half and both positive fire
  // windows, including gaps/negative values and the prerequisite flags bit.
  assert(other.vm->memory.registerRam(TailF2,std::vector<uint8_t>(0x600)) &&
         other.vm->memory.registerRam(FireWork,std::vector<uint8_t>(0x140)));
  other.tailReady=true;other.tailSecondField=TailF2;
  assert(other.vm->memory.writeHalf(OtherF,1) &&
         other.vm->memory.writeByte(NativeWorld::M+0x3c,2));
  other.store(NativeWorld::M+0x298,FireWork);
  for(uint32_t kind : {19u,20u,23u,24u,39u,40u,43u,44u,0xffffffffu})
    for(uint32_t flags : {0u,1u}) for(uint16_t jumpHalf : {0,0xffff}) {
      other.store(other.vm->gpAddresses.currentAddress()+0x168,OtherF);
      other.store(FireWork+0x13c,kind);other.store(NativeWorld::M,flags);
      assert(other.vm->memory.writeHalf(NativeWorld::M+0x50,jumpHalf));
      other.store(TailF2+0x574,9);other.store(TailF2+0x578,1);other.store(TailF2+0x57c,5);
      auto query=std::make_unique<FieldNpcMainCursor>();
      assert(other.entry->advanceMain(*query));
      const bool fire=flags && (kind==20 || kind==23 || kind==40 || kind==43);
      assert(other.word(TailF2+0x574)==(fire?9u:0u) &&
             other.word(TailF2+0x578)==(jumpHalf?0u:1u) &&
             other.word(TailF2+0x57c)==(jumpHalf?0u:5u) && other.scores==0);
    }


}
void actualDrawCallbacks() {
  auto storage = std::make_unique<NativeWorld>();
  auto &w = *storage;
  struct Probe {
    NativeWorld *w;
    bool ready = false, gpuReady = false;
    unsigned syncs = 0, clears = 0, reads = 0, boxes = 0, blurCameras = 0;
    unsigned mains = 0, draws = 0, setters = 0;
    std::vector<uint32_t> names;
  } probe{&w};
  constexpr uint32_t Record = 0x8f000000, File = 0x8f010000,
                     Holder = 0x8f020000, Model = 0x8f030000,
                     Directory = 0x8f040000, Camera = 0x8f050000;
  auto &memory = w.vm->memory;
  for (auto pair :
       std::array<std::pair<uint32_t, unsigned>, 6>{{{Record, 16},
                                                     {File, 0xa4},
                                                     {Holder, 4},
                                                     {Model, 0x1b0},
                                                     {Directory, 64},
                                                     {Camera, 0x118}}})
    assert(memory.registerRam(pair.first, std::vector<uint8_t>(pair.second)));
  FieldNpcMainEntryNativeServices n;
  n.user = &probe;
  n.numbers.divide = nativeFieldNpcClockDivide;
  n.numbers.convert = nativeFieldNpcClockConvert;
  n.admitMario = [](void *, EventAddressSpace &, uint32_t) { return true; };
  n.drawNumber = [](void *, EventAddressSpace &, FieldNpcMatrixNumber &q) {
    assert(q.function == 0x80007328);
    q.result = npcReviseAngle(q.args[0]);
    return true;
  };
  n.drawMatrix = [](void *, EventAddressSpace &,
                    BattleAudienceMatrixRequest &q) {
    BattleAudienceAuxServices math;
    bindAudienceAuxMathAdapter(math);
    return math.matrix(nullptr, q);
  };
  n.draw = [](void *p, EventAddressSpace &m, const FieldNpcDrawRequest &q,
              FieldNpcDrawResponse &r) {
    auto &f = *static_cast<Probe *>(p);
    assert(&m == &f.w->vm->memory);
    switch (q.function) {
    case 0x80007328:
      r.number = npcReviseAngle(q.scalars[0]);
      return true;
    case 0x80010864:
      r.word = Record;
      return true;
    case 0x8000f2fc:
      assert(q.args[0] == 4 || q.args[0] == 9);
      if (q.args[0] == 9)
        ++f.blurCameras;
      r.word = Camera;
      return true;
    case 0x802725a0:
      r.number = 0;
      return true;
    case 0x80041170:
      assert(q.args[0] == f.w->pose && q.args[2] == 0);
      ++f.setters;
      f.names.push_back(q.args[1]);
      return true;
    case 0x8003ed2c:
      if (!f.ready)
        return false;
      assert(q.args[0] == f.w->pose);
      ++f.mains;
      return true;
    case 0x80005ce4:
      if (!f.gpuReady)
        return false;
      ++f.syncs;
      return true;
    case 0x802b69d0:
      if (!f.gpuReady)
        return false;
      ++f.clears;
      return true;
    case 0x802b6a08:
      if (!f.gpuReady)
        return false;
      ++f.reads;
      r.bounds = {7, 8, 99, 100};
      return true;
    case 0x80072f38:
      if (!f.gpuReady)
        return false;
      assert(q.args[0] == 999 &&
             q.bounds == (std::array<uint16_t, 4>{7, 8, 99, 100}));
      ++f.boxes;
      return true;
    case 0x8003b4d8:
      if (!f.ready)
        return false;
      assert(q.args[0] == f.w->pose && q.args[2] >= 1 && q.args[2] <= 3);
      ++f.draws;
      return true;
    default:
      assert(false);
      return false;
    }
  }; // Explicit field camera/Main/draw fixture, not a production renderer.
  auto binding =
      FieldNpcMainEntryBinding::bind(w.owner, *w.registry, *w.vm, *w.view, n);
  assert(binding);
  w.store(w.actor, w.word(w.actor) | 0x02000000);
  w.store(w.actor + 0x1d4, 0x00400000);
  w.store(w.actor + 0x114, 0xff7f03ab);
  w.store(w.actor + 0xf0, 0);
  w.store(w.actor + 0x144, bits(90));
  FieldNpcDrawCursor c;
  assert(!binding->advanceDraw(0x8004c7e0, w.actor, c));
  auto *actual = w.pool->slot(w.pose);
  assert(actual && actual->word(0xf0) == 0x7f3f01ab &&
         actual->word(0xe8) & 0x40);
  auto foreign =
      FieldNpcMainEntryBinding::bind(w.owner, *w.registry, *w.vm, *w.view, n);
  assert(foreign && !foreign->advanceDraw(0x8004c7e0, w.actor, c));
  // No color/flag replay after the held Main; typed and VM views are identical.
  w.store(w.view->poseAddress(w.pose) + 0xf0, 0x12345678);
  probe.ready = true;
  assert(binding->advanceDraw(0, w.actor + 0x340, c) && !c.active &&
         actual->word(0xf0) == 0x12345678 && probe.mains == 1 &&
         probe.draws == 3);
  // True descriptor pointer getter, source strcmp, table and actual pose alias.
  w.store(w.actor, w.word(w.actor) | 0x00800000);
  const uint32_t poseAddress = w.view->poseAddress(w.pose),
                 modelCache = w.word(NativeWorld::Work);
  w.store(poseAddress + 0x10, 0);
  w.store(poseAddress + 0x14, 0);
  w.store(poseAddress + 0x74, bits(-180));
  w.store(modelCache + 8, File);
  w.store(File + 0xa0, Holder);
  w.store(Holder, Model);
  w.store(Model + 0x1ac, Directory);
  const uint32_t sourceName = w.word(0x803066f8);
  for (unsigned i = 0; i < 4; ++i) {
    uint8_t ch;
    assert(memory.readByte(sourceName + i, ch) &&
           memory.writeByte(Directory + i, ch));
  }
  assert(binding->advanceDraw(0x8004c50c, w.actor, c) && probe.setters == 1 &&
         probe.names.back() == w.word(0x803066fc));
  // Raw material/get-pose leaves must not add an active/resource gate.
  w.store(w.actor + 0x104, 255);
  w.pose = 255;
  w.store(w.actor, w.word(w.actor) & ~0x00800000);
  assert(binding->advanceDraw(0x8004c7e0, w.actor, c));
  assert(w.pool->slot(255)->word(0) == 0 &&
         w.pool->slot(255)->word(0xf0) == 0x7f3f01ab &&
         w.pool->slot(255)->word(0xe8) & 0x40);
  // Missing native Main/draw and changed pool mappings hold; no empty success.
  probe.ready = false;
  assert(!binding->advanceDraw(0x8004c7e0, w.actor, c));
  const auto draws = probe.draws;
  w.store(NativeWorld::Work + 0x14, 255);
  probe.ready = true;
  assert(!binding->advanceDraw(0, w.actor, c) && probe.draws == draws);
  w.store(NativeWorld::Work + 0x14, 256);
  assert(binding->advanceDraw(0, w.actor, c));
  // Explicit synchronized raster fixture: no callback starts before GPU ready.
  w.store(w.actor + 0x110, 999);
  const auto beforeMain = probe.mains;
  assert(!binding->advanceDraw(0x8004bf28, w.actor, c, 9));
  assert(probe.mains == beforeMain && probe.syncs == 0);
  probe.gpuReady = true;
  assert(binding->advanceDraw(0, w.actor, c, 4));
  assert(probe.syncs == 2 && probe.clears == 1 && probe.reads == 1 &&
         probe.boxes == 1);
  // Raw inactive shared material flags are saved/restored; color is not
  // restored.
  w.store(w.view->poseAddress(255) + 0xe8, 0x100);
  const auto beforeBlur = probe.draws;
  assert(binding->advanceDraw(0x8004bc3c, w.actor, c, 9));
  assert(probe.draws == beforeBlur + 12 && probe.blurCameras == 2);
  assert(w.pool->slot(255)->word(0) == 0 &&
         w.pool->slot(255)->word(0xe8) == 0x100 &&
         w.pool->slot(255)->word(0xf0) == 0xffffff67);
}
} // namespace
int main() {
  ordinary();
  gates();
  special();
  holds();
  actualAliases();
  actorScan();
  actualDrawCallbacks();
}
