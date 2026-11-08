#include "battle_audience_noise_continuation.hpp"
#include "battle_audience_sound.hpp"
#include "battle_audience_sound_start.hpp"
#include "battle_event_callbacks.hpp"
#include "battle_menu_file_cache.hpp"
#include "battle_timing_a.hpp"
#include "field_battle_information.hpp"
#include "harbor_source_audience.hpp"
#include "harbor_source_audience_noise.hpp"
#include "prologue_pouch.hpp"
#include <atomic>
#include <cassert>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
using namespace ttyd;
namespace {
constexpr uint32_t WorkBytes = 0x19098, AudienceOffset = 0x2858,
                   ActorOffset = 0x1bc, ActorStride = 0x134,
                   NoiseOffset = 0x133fc + 2 * 0x24;
struct Fixture {
  // Optional real cache remains alive until after its borrowing VM.
  std::shared_ptr<BattleMenuFileCache> files;
  std::shared_ptr<EventScriptAssets> data =
      std::make_shared<EventScriptAssets>();
  EventNativeRuntime vm{0x81000000, {}};
  std::unique_ptr<HarborSourceMapPool> pool;
  std::shared_ptr<const uint8_t> lifetime = std::make_shared<const uint8_t>(0);
  BattleRandom random{7};
  std::shared_ptr<PositionalSfxRegistry> registry =
      std::make_shared<PositionalSfxRegistry>();
  std::shared_ptr<NativeBattleSound> sound;
  std::unique_ptr<HarborSourceAudienceNoise> native;
  uint32_t battle = 0;
  bool alive = true, reenter = false;
  unsigned arithmetic = 0;
  Fixture() {
    assert(data->load("romfs/harbor-tutorial-events.pmevt") &&
           vm.memory.importScripts(*data));
    vm.platform.user = this;
    vm.platform.flushBlock = [](void *p, uint32_t at) {
      auto &f = *static_cast<Fixture *>(p);
      uint8_t b;
      for (unsigned i = 0; i < 32; ++i)
        assert(f.vm.memory.readByte((at & ~31u) + i, b));
    };
    vm.platform.systemCall = [](void *) {
      std::atomic_thread_fence(std::memory_order_seq_cst);
    };
    vm.platform.truncate = [](void *p, float f) {
      ++static_cast<Fixture *>(p)->arithmetic;
      assert(std::isfinite(f) && f >= -2147483648.f && f < 2147483648.f);
      return int32_t(f);
    };
    vm.platform.multiplyFloat = [](void *p, float a, float b) {
      ++static_cast<Fixture *>(p)->arithmetic;
      return a * b;
    };
    vm.platform.divideFloat = [](void *p, float a, float b) {
      ++static_cast<Fixture *>(p)->arithmetic;
      return a / b;
    };
    vm.platform.addFloat = [](void *p, float a, float b) {
      ++static_cast<Fixture *>(p)->arithmetic;
      return a + b;
    };
    vm.platform.subtractDouble = [](void *p, double a, double b) {
      ++static_cast<Fixture *>(p)->arithmetic;
      return a - b;
    };
    vm.platform.roundDoubleFloat = [](void *p, double a) {
      ++static_cast<Fixture *>(p)->arithmetic;
      return float(a);
    };
    pool = HarborSourceMapPool::bind(vm.memory, vm.memoryBinding.services());
    assert(pool);
    const auto b = pool->allocate(pool->root(), WorkBytes);
    assert(b);
    battle = *b;
    assert(vm.memory.registerRam(0x8041eb90, std::vector<uint8_t>(4)));
    word(0x8041eb90, battle);
    word(vm.gpAddresses.currentAddress() + 4, 60);
    word(vm.gpAddresses.currentAddress() + 0x174, 4);
    NativeBattleSoundBinding binding;
    binding.vm = &vm;
    binding.random = &random;
    binding.registry = registry;
    binding.lifetime = lifetime;
    binding.user = this;
    binding.current = [](void *p) { return static_cast<Fixture *>(p)->alive; };
    sound = std::make_shared<NativeBattleSound>(binding);
  }
  uint32_t audience() const { return battle + AudienceOffset; }
  uint32_t record() const { return audience() + NoiseOffset; }
  uint32_t word(uint32_t a) const {
    uint32_t v;
    assert(vm.memory.readWord(a, v));
    return v;
  }
  void word(uint32_t a, uint32_t v) { assert(vm.memory.writeWord(a, v)); }
  uint8_t byte(uint32_t a) const {
    uint8_t v;
    assert(vm.memory.readByte(a, v));
    return v;
  }
  void crowd(unsigned count) {
    for (unsigned i = 0; i < 200; ++i) {
      word(audience() + ActorOffset + i * ActorStride, i < count ? 1 : 0);
      assert(vm.memory.writeByte(
          audience() + ActorOffset + i * ActorStride + 0x19, 0));
    }
  }
  std::vector<uint8_t> snapshot() const {
    std::vector<uint8_t> b(WorkBytes);
    for (unsigned i = 0; i < WorkBytes; ++i)
      b[i] = byte(battle + i);
    return b;
  }
  HarborSourceAudienceNoiseBinding
  nativeBinding(HarborAudienceNoisePrivatePolicy policy) {
    HarborSourceAudienceNoiseBinding b;
    b.vm = &vm;
    b.pool = pool.get();
    b.battle = battle;
    b.lifetime = lifetime;
    b.sourceData = data;
    b.sound = sound;
    b.registry = registry;
    b.policy = policy;
    b.current = [this] {
      if (reenter && native) {
        assert(!native->resume(request()));
        assert(!native->retire());
        assert(!native->complete());
      }
      return alive;
    };
    return b;
  }
  static BattleAudienceSetupRequest request() {
    BattleAudienceSetupRequest q;
    q.function = 0x801a0b88;
    q.callsite = 0x801a8584;
    return q;
  }
};
struct Protocol {
  Fixture &f;
  BattleAudienceNoiseCursor cursor;
  BattleAudienceNoiseServices services;
  std::map<uint32_t, unsigned> attempts;
  const BattleAudienceNoiseCall *held = nullptr;
  unsigned copies = 0, plays = 0, volumes = 0;
  uint32_t handle = 77;
  bool waits = true, mutate = false, reenter = false;
  Protocol(Fixture &v) : f(v) {
    services.actors = nativeBattleActorServices(f.vm);
    services.lifetime = f.lifetime;
    services.user = this;
    services.current = [](void *p) {
      return static_cast<Protocol *>(p)->f.alive;
    };
    services.call =
        [](void *p,
           const BattleAudienceNoiseCall &q) -> std::optional<uint32_t> {
      return static_cast<Protocol *>(p)->call(q);
    };
  }
  std::optional<uint32_t> call(const BattleAudienceNoiseCall &q) {
    if (reenter) {
      assert(!resumePackedBattleAudienceNoise(services, cursor));
      assert(!cursor.reset());
    }
    if (held)
      assert(held == &q);
    held = &q;
    auto &n = attempts[q.function];
    ++n;
    if (q.function == 0x8026d5fc) {
      assert(q.copyOutput && !q.name && q.originalName == q.arguments[0]);
      uint8_t b;
      unsigned i = 0;
      do {
        assert(f.vm.memory.readByte(q.originalName + i, b));
        q.copyOutput->bytes[i++] = b;
      } while (b);
      if (waits && n <= 2)
        return {};
      ++copies;
      held = nullptr;
      return 0;
    }
    if (q.function == 0x8019fcec) {
      assert(q.name && !q.copyOutput);
      assert(q.arguments[0] == uint32_t(battleAudienceHalfCount(
                                   true, nativeBattleActorServices(f.vm))) &&
             q.arguments[1] == uint32_t(battleAudienceHalfCount(
                                   false, nativeBattleActorServices(f.vm))));
      if (q.originalName) {
        assert(q.name->bytes[0]);
      } else
        for (auto b : q.name->bytes)
          assert(!b);
      if (waits && n <= 2)
        return {};
      if (mutate) {
        f.word(f.record(), 0x100);
        f.word(f.audience(), 0x400);
      }
      ++plays;
      held = nullptr;
      return handle;
    }
    assert(q.function == 0x800d9558 && q.arguments[0] == handle);
    assert(f.word(f.record()) & 1);
    if (waits && n <= 2)
      return {};
    ++volumes;
    held = nullptr;
    return 64;
  }
  bool resume() { return resumePackedBattleAudienceNoise(services, cursor); }
};
void differential() {
  for (unsigned count : {0, 1, 49, 50, 99, 100, 149, 150, 200})
    for (bool active : {false, true})
      for (bool rejected : {false, true}) {
        Fixture f, reference;
        f.crowd(count);
        reference.crowd(count);
        if (active) {
          f.word(f.record(), 0x81);
          reference.word(reference.record(), 0x81);
        }
        Protocol p(f);
        p.reenter = true;
        if (rejected)
          p.handle = 0xffffffffu;
        unsigned visits = 0;
        while (!p.resume()) {
          assert(++visits < 12);
        }
        BattleAudienceSoundServices s;
        s.actors = nativeBattleActorServices(reference.vm);
        s.user = &p;
        s.play = [](void *p, uint32_t, bool, int32_t,
                    int32_t) -> std::optional<int32_t> {
          return int32_t(static_cast<Protocol *>(p)->handle);
        };
        s.volume = [](void *, int32_t) -> std::optional<uint8_t> { return 64; };
        assert(noiseAlwaysBattleAudienceSound(s));
        assert(f.snapshot() == reference.snapshot());
        assert(p.plays == (active ? 0 : 1));
        assert(p.volumes == ((active || rejected) ? 0 : 1));
        const auto plays = p.plays, volumes = p.volumes;
        assert(p.resume() && p.plays == plays && p.volumes == volumes);
      }
}
void holdsAndIdentity() {
  Fixture f;
  f.crowd(3);
  Protocol p(f);
  p.mutate = true;
  while (!p.resume()) {
  }
  assert(f.word(f.record()) == 0x101 && f.word(f.audience()) == 0x2400);
  Fixture g;
  g.crowd(1);
  Protocol changed(g);
  assert(!changed.resume());
  changed.services.actors.audienceLifecycle.user = &g;
  assert(!changed.resume() && changed.plays == 0);
  Fixture h;
  Protocol stale(h);
  assert(!stale.resume());
  h.alive = false;
  assert(!stale.resume());
  h.alive = true;
  assert(!stale.resume());
  Fixture active;
  active.word(active.record(), 1);
  Protocol noChild(active);
  noChild.services.call = nullptr;
  assert(noChild.resume());
}
void getterTrace() {
  Fixture f;
  f.crowd(1);
  Protocol p(f);
  struct Trace {
    EventMemoryServices original;
    unsigned globals = 0;
  } trace{p.services.actors.memory};
  auto &m = p.services.actors.memory;
  m.user = &trace;
  m.word = [](void *v, uint32_t at) {
    auto &t = *static_cast<Trace *>(v);
    if (at == 0x8041eb90)
      ++t.globals;
    return t.original.word(t.original.user, at);
  };
  m.storeWord = [](void *v, uint32_t at, uint32_t value) {
    auto &t = *static_cast<Trace *>(v);
    t.original.storeWord(t.original.user, at, value);
  };
  m.storeByte = [](void *v, uint32_t at, uint8_t value) {
    auto &t = *static_cast<Trace *>(v);
    t.original.storeByte(t.original.user, at, value);
  };
  while (!p.resume()) {
  };
  // Parent A + soundPtr A +
  // two(discardedRLBase+100*(Actor,ExistBase,ExistActor)).
  assert(trace.globals == 604);
  assert(p.resume() && trace.globals == 604);
}
void defaultUnknown() {
  Fixture f;
  f.native = HarborSourceAudienceNoise::create(
      f.nativeBinding(HarborAudienceNoisePrivatePolicy::PreserveUnknown));
  assert(f.native);
  f.reenter = true;
  const auto seed = f.random.state;
  assert(!f.native->resume(Fixture::request()) &&
         f.native->pending() == 0x8019fcec);
  assert(!f.native->resume(Fixture::request()) && f.arithmetic == 0 &&
         f.registry->allocationGeneration == 0 && f.random.state == seed);
  assert(f.word(f.record()) == 0 && f.word(f.record() + 4) == 0);
  assert(f.native->retire() && !f.native->resume(Fixture::request()));
}
void actualPositive() {
  for (unsigned count : {1, 49, 50, 99, 100, 149, 150, 200}) {
    Fixture f;
    f.crowd(count);
    f.native = HarborSourceAudienceNoise::create(
        f.nativeBinding(HarborAudienceNoisePrivatePolicy::PreserveUnknown));
    assert(f.native);
    const auto seed = f.random.state;
    assert(f.native->resume(Fixture::request()) && f.native->complete());
    const auto handle = f.word(f.record() + 4);
    assert(handle != 0xffffffffu && f.registry->allocationGeneration == 1);
    const auto &slot = f.registry->slots[handle & 255];
    BattleAudienceSoundStartServices expected;
    expected.actors = nativeBattleActorServices(f.vm);
    expected.user = &f;
    expected.truncate = [](void *, float v) -> std::optional<int32_t> {
      return int32_t(v);
    };
    const auto spatial = packedBattleAudienceSoundInfo1(
        battleAudienceHalfCount(true, expected.actors),
        battleAudienceHalfCount(false, expected.actors), expected);
    assert(spatial && slot.position == spatial->position &&
           slot.volume == spatial->volume);
    assert(slot.flags & 1);
    assert(slot.flags & 2);
    assert(slot.engine == -1);
    assert(f.byte(f.record() + 0x1c) == slot.volume &&
           (f.word(f.audience()) & 0x2000));
    const auto arithmetic = f.arithmetic;
    assert(f.native->resume(Fixture::request()) &&
           f.registry->allocationGeneration == 1 && f.random.state == seed &&
           f.arithmetic == arithmetic);
  }
}
void emptyPolicy() {
  Fixture f;
  // Independent actualVM proof of the FIRST lookup result. The table may
  // contain empty names; preserve the first match instead of asserting a miss.
  uint8_t zero = 1;
  assert(f.vm.memory.readByte(0x804218c8, zero) && zero == 0);
  unsigned firstEmpty = 4242;
  for (unsigned i = 0; i < 4242; ++i) {
    const auto name = f.word(0x80316b28 + 20 * i);
    assert(name);
    if (f.byte(name) == 0) {
      firstEmpty = i;
      break;
    }
  }
  assert(firstEmpty == 0 && f.word(0x80316b28 + 20 * firstEmpty + 4) == 0);
  std::cout << "Actual original first empty-name match row=" << firstEmpty
            << " descriptor=0\n";
  f.native = HarborSourceAudienceNoise::create(
      f.nativeBinding(HarborAudienceNoisePrivatePolicy::EmptyColdPrivateName));
  assert(f.native);
  const auto seed = f.random.state;
  assert(f.native->resume(Fixture::request()) && f.native->complete());
  assert(f.word(f.record() + 4) == 0xffffffffu && f.word(f.record()) == 0 &&
         !(f.word(f.audience()) & 0x2000));
  assert(f.registry->allocationGeneration == 0 && f.random.state == seed);
}
void nativeBackendHold() {
  Fixture f;
  f.crowd(1);
  // Explicit TEST existing started-engine domain. The finite real owner refuses
  // before public On mutations until that external lifetime is actually gone.
  auto &occupied = f.registry->slots[0];
  occupied.key = 2520;
  occupied.flags = 0;
  occupied.engine = 99;
  f.native = HarborSourceAudienceNoise::create(
      f.nativeBinding(HarborAudienceNoisePrivatePolicy::PreserveUnknown));
  assert(f.native);
  assert(!f.native->resume(Fixture::request()) &&
         f.native->pending() == 0x800d9934);
  const auto arithmetic = f.arithmetic;
  assert(arithmetic && !f.registry->allocationGeneration);
  assert(!f.native->resume(Fixture::request()) && f.arithmetic == arithmetic &&
         !f.registry->allocationGeneration);
  occupied.key = -1;
  occupied.engine = -1;
  assert(f.native->resume(Fixture::request()) &&
         f.registry->allocationGeneration == 1 &&
         f.arithmetic == arithmetic + 1);
}
void wholeAudienceNative() {
  for (bool policy : {false, true}) {
    Fixture f;
    f.files = std::make_shared<BattleMenuFileCache>();
    assert(f.files->load("romfs/battle-class-common.tpl") &&
           stageHarborAudienceFiles(*f.files->files(), "romfs") &&
           f.files->bind(f.vm.memory));
    f.vm.platform.irand = [](void *p, int32_t n) {
      return static_cast<Fixture *>(p)->random.bounded(n);
    };
    f.vm.platform.divideWord = [](void *, int32_t a, int32_t b) {
      assert(b);
      return a / b;
    };
    f.vm.platform.fmaFloat = [](void *, float a, float b, float c) {
      return std::fma(a, b, c);
    };
    constexpr uint32_t Pouch = 0x85002000, Npc = 0x85000000,
                       Information = 0x85001000;
    assert(loadProloguePouchAssets("romfs"));
    const auto pouch = createProloguePouch();
    assert(pouch);
    assert(f.vm.memory.registerRam(
        Pouch, std::vector<uint8_t>(pouch->bytes.begin(), pouch->bytes.end())));
    assert(f.vm.memory.registerRam(0x8041eb00, std::vector<uint8_t>(4)) &&
           f.vm.memory.registerRam(Npc, std::vector<uint8_t>(0x400)) &&
           f.vm.memory.registerRam(Information, std::vector<uint8_t>(16)));
    f.word(0x8041eb00, Pouch);
    f.word(f.battle + 0x2738, Information);
    f.word(Information + 12, Npc + 0x230);
    std::ifstream in("romfs/harbor-tutorial-program.bin", std::ios::binary);
    std::array<uint8_t, 56> catalog{};
    in.read(reinterpret_cast<char *>(catalog.data()), catalog.size());
    assert(in);
    auto entry = [&](unsigned i) {
      const auto at = 8 + 4 * i;
      return uint32_t(catalog[at]) | uint32_t(catalog[at + 1]) << 8 |
             uint32_t(catalog[at + 2]) << 16 | uint32_t(catalog[at + 3]) << 24;
    };
    PackedNpcBattleSetupServices setup;
    setup.memory = nativeBattleActorServices(f.vm);
    setup.user = &f;
    setup.random = f.vm.platform.irand;
    setup.savedFlag = [](void *, int32_t) -> int32_t {
      assert(false);
      return 0;
    };
    setupPackedNpcBattleInformation(Npc, entry(9) + 0x60eb8 - 0x60c90, setup);
    assert(f.word(Npc + 0x230) == entry(10));
    HarborSourceAudienceBinding b;
    b.vm = &f.vm;
    b.pool = f.pool.get();
    b.battle = f.battle;
    b.lifetime = f.lifetime;
    b.current = [&] { return f.alive; };
    b.sourceData = f.data;
    b.files = f.files->files();
    b.child = [&](const BattleAudienceSetupRequest &q)
        -> std::optional<BattleAudienceSetupOutput> {
      assert(q.function == 0x801a0b88);
      if (!f.native)
        f.native = HarborSourceAudienceNoise::create(f.nativeBinding(
            policy ? HarborAudienceNoisePrivatePolicy::EmptyColdPrivateName
                   : HarborAudienceNoisePrivatePolicy::PreserveUnknown));
      assert(f.native);
      if (!f.native->resume(q))
        return {};
      return BattleAudienceSetupOutput{};
    };
    auto audience = HarborSourceAudience::create(std::move(b));
    assert(audience);
    if (!policy) {
      assert(!audience->resume() && f.native->pending() == 0x8019fcec);
      const auto seed = f.random.state;
      const auto entryAddress = f.word(f.audience() + 0xc),
                 refs = f.word(entryAddress) & 0xffff;
      assert(!audience->resume() && f.random.state == seed &&
             (f.word(entryAddress) & 0xffff) == refs &&
             !f.registry->allocationGeneration);
    } else {
      assert(audience->resume() && audience->complete() &&
             f.native->complete());
      assert(f.word(f.record() + 4) == 0xffffffffu && f.word(f.record()) == 0 &&
             f.word(f.record() + 0x18) == 180 &&
             f.byte(f.record() + 0x20) == 60);
      const auto seed = f.random.state;
      assert(audience->resume() && f.random.state == seed &&
             !f.registry->allocationGeneration);
    }
  }
}
void nativeInvalidation() {
  Fixture actual, wrongVm;
  auto wrong =
      wrongVm.nativeBinding(HarborAudienceNoisePrivatePolicy::PreserveUnknown);
  wrong.sound = actual.sound;
  wrong.registry = actual.registry;
  assert(!HarborSourceAudienceNoise::create(std::move(wrong)));
  Fixture f;
  f.native = HarborSourceAudienceNoise::create(
      f.nativeBinding(HarborAudienceNoisePrivatePolicy::PreserveUnknown));
  assert(f.native && !f.native->resume(Fixture::request()));
  const auto old = f.battle;
  assert(f.pool->free(f.pool->root(), old));
  assert(f.pool->allocate(f.pool->root(), WorkBytes) == old);
  assert(!f.native->resume(Fixture::request()));
  Fixture g;
  g.native = HarborSourceAudienceNoise::create(
      g.nativeBinding(HarborAudienceNoisePrivatePolicy::PreserveUnknown));
  assert(g.native && !g.native->resume(Fixture::request()));
  g.vm.platform.divideFloat = [](void *, float, float) { return 0.f; };
  assert(!g.native->resume(Fixture::request()));
}
} // namespace
int main() {
  differential();
  holdsAndIdentity();
  getterTrace();
  defaultUnknown();
  actualPositive();
  emptyPolicy();
  nativeBackendHold();
  wholeAudienceNative();
  nativeInvalidation();
  std::cout << "Retained Noise source branches/holds and actual positive "
               "pending sound PASS; separate empty-private TEST policy proves "
               "actual first-row zero descriptor, not source write or audible "
               "playback\n";
}
