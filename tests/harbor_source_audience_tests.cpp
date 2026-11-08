#include "battle_event_callbacks.hpp"
#include "battle_menu_file_cache.hpp"
#include "battle_timing_a.hpp"
#include "field_battle_information.hpp"
#include "harbor_source_audience.hpp"
#include "prologue_pouch.hpp"
#include <atomic>
#include <cassert>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
using namespace ttyd;
namespace {
constexpr uint32_t Pouch = 0x85002000, Npc = 0x85000000,
                   Information = 0x85001000;
struct Fixture {
  // Shared cache outlives the borrowing VM, including destruction.
  std::shared_ptr<BattleMenuFileCache> files =
      std::make_shared<BattleMenuFileCache>();
  std::shared_ptr<EventScriptAssets> source =
      std::make_shared<EventScriptAssets>();
  EventNativeRuntime vm{0x81000000, {}};
  std::unique_ptr<HarborSourceMapPool> pool;
  BattleRandom random{3};
  std::shared_ptr<const uint8_t> life = std::make_shared<const uint8_t>(0);
  std::unique_ptr<HarborSourceAudience> owner;
  uint32_t battle = 0;
  unsigned draws = 0, flushes = 0, barriers = 0, noiseAttempts = 0;
  bool alive = true, reenter = false, testAudio = false;
  const BattleAudienceAddNumInvocation *heldAdd = nullptr;
  unsigned addAttempts = 0;
  unsigned partialAudio = 0;
  Fixture(bool stage = true) {
    assert(source->load("romfs/harbor-tutorial-events.pmevt") &&
           vm.memory.importScripts(*source));
    assert(files->load("romfs/battle-class-common.tpl"));
    if (stage)
      assert(stageHarborAudienceFiles(*files->files(), "romfs"));
    assert(files->bind(vm.memory));
    vm.platform.user = this;
    vm.platform.flushBlock = [](void *p, uint32_t at) {
      auto &f = *static_cast<Fixture *>(p);
      uint8_t v;
      for (unsigned i = 0; i < 32; ++i)
        assert(f.vm.memory.readByte((at & ~31u) + i, v));
      ++f.flushes;
    };
    vm.platform.systemCall = [](void *p) {
      auto &f = *static_cast<Fixture *>(p);
      std::atomic_thread_fence(std::memory_order_seq_cst);
      ++f.barriers;
    };
    vm.platform.irand = [](void *p, int32_t n) {
      auto &f = *static_cast<Fixture *>(p);
      ++f.draws;
      return f.random.bounded(n);
    };
    vm.platform.truncate = [](void *, float f) -> int32_t {
      return int32_t(f);
    };
    vm.platform.divideWord = [](void *, int32_t n, int32_t d) -> int32_t {
      return n / d;
    };
    vm.platform.addFloat = [](void *, float a, float b) { return a + b; };
    vm.platform.multiplyFloat = [](void *, float a, float b) { return a * b; };
    vm.platform.fmaFloat = [](void *, float a, float b, float c) {
      return std::fma(a, b, c);
    };
    vm.platform.subtractDouble = [](void *, double a, double b) {
      return a - b;
    };
    vm.platform.roundDoubleFloat = [](void *, double a) { return float(a); };
    pool = HarborSourceMapPool::bind(vm.memory, vm.memoryBinding.services());
    assert(pool && flushes && barriers);
    const auto allocation = pool->allocate(pool->root(), 0x19098);
    assert(allocation && *allocation);
    battle = *allocation;
    assert(vm.memory.registerRam(0x8041eb90, std::vector<uint8_t>(4)));
    assert(vm.memory.registerRam(0x8041eb00, std::vector<uint8_t>(4)));
    assert(vm.memory.registerRam(Npc, std::vector<uint8_t>(0x400)));
    assert(vm.memory.registerRam(Information, std::vector<uint8_t>(16)));
    assert(loadProloguePouchAssets("romfs"));
    auto pouch = createProloguePouch();
    assert(pouch);
    assert(vm.memory.registerRam(
        Pouch, std::vector<uint8_t>(pouch->bytes.begin(), pouch->bytes.end())));
    w(0x8041eb90, battle);
    w(0x8041eb00, Pouch);
    w(battle + 0x2738, Information);
    w(Information + 12, Npc + 0x230);
    std::ifstream in("romfs/harbor-tutorial-program.bin", std::ios::binary);
    std::array<uint8_t, 56> catalog{};
    in.read(reinterpret_cast<char *>(catalog.data()), catalog.size());
    assert(in);
    auto little = [&](unsigned i) {
      const auto p = 8 + 4 * i;
      return uint32_t(catalog[p]) | uint32_t(catalog[p + 1]) << 8 |
             uint32_t(catalog[p + 2]) << 16 | uint32_t(catalog[p + 3]) << 24;
    };
    PackedNpcBattleSetupServices setup;
    setup.memory = nativeBattleActorServices(vm);
    setup.user = this;
    setup.random = vm.platform.irand;
    setup.savedFlag = [](void *, int32_t) -> int32_t {
      assert(false);
      return 0;
    };
    setupPackedNpcBattleInformation(Npc, little(9) + 0x60eb8 - 0x60c90, setup);
    assert(word(Npc + 0x230) == little(10));
    owner = HarborSourceAudience::create(binding());
    assert(owner);
  }
  HarborSourceAudienceBinding binding() {
    HarborSourceAudienceBinding b;
    b.vm = &vm;
    b.pool = pool.get();
    b.battle = battle;
    b.lifetime = life;
    b.files = files->files();
    b.sourceData = source;
    b.current = [this] {
      if (reenter && owner) {
        assert(!owner->resume());
        assert(!owner->retire());
        assert(!owner->current());
      }
      return alive;
    };
    b.child = [this](const BattleAudienceSetupRequest &q)
        -> std::optional<BattleAudienceSetupOutput> {
      assert(q.function == 0x801a0b88);
      ++noiseAttempts;
      if (!testAudio)
        return {}; // Actual frontend absent, never acknowledge noise.
      // Explicit TEST whole-child protocol only. The first modeled audio effect
      // is retained across two waits. This is not a native sound completion.
      if (partialAudio < 2) {
        ++partialAudio;
        return {};
      }
      return BattleAudienceSetupOutput{};
    };
    b.addAudience = [this](const BattleAudienceAddNumInvocation &q)
        -> std::optional<uint32_t> {
      if (heldAdd)
        assert(heldAdd == &q);
      heldAdd = &q;
      ++addAttempts;
      assert(q.battle == battle && q.audience == audience() &&
             q.pouch == Pouch && q.amount == 1 && q.capacity == 1);
      return {}; // Genuine positive suffix remains absent.
    };
    return b;
  }
  uint32_t word(uint32_t a) {
    uint32_t v;
    assert(vm.memory.readWord(a, v));
    return v;
  }
  void w(uint32_t a, uint32_t v) { assert(vm.memory.writeWord(a, v)); }
  uint32_t audience() const { return battle + 0x2858; }
  uint32_t payload(uint32_t h) { return word(word(h + 0xa0)); }
  void checkBank(uint32_t entry, uint32_t count) {
    const auto p = payload(entry);
    assert(p && word(p) == 0x0020af30 && word(p + 4) == count &&
           word(p + 8) == p + 12);
    const auto image = word(p + 12);
    assert(image >= p && word(image + 8) > p);
    uint8_t flag;
    assert(vm.memory.readByte(image + 0x23, flag) && flag == 1);
  }
};
void nativeBoundary() {
  Fixture f;
  f.reenter = true;
  assert(!f.owner->resume());
  assert(f.owner->pending() == 0x801a0b88 && !f.owner->complete());
  const auto a = f.audience(), normal = f.word(a + 0xc),
             guest = f.word(a + 0x10);
  assert(normal && guest && normal != guest && normal != 1 && guest != 2);
  f.checkBank(normal, 205);
  f.checkBank(guest, 19);
  assert(f.word(normal) & 0xffff);
  const auto refs = f.word(normal) & 0xffff;
  const auto draws = f.draws;
  assert(draws >= 300);
  assert(!f.owner->resume() && f.draws == draws &&
         (f.word(normal) & 0xffff) == refs);
  f.alive = false;
  assert(!f.owner->resume());
  f.alive = true;
  assert(!f.owner->resume());
  assert(f.word(a + 0xc) == normal && f.word(0x8041eb90) == f.battle);
  assert(f.owner->retire() && !f.owner->current());
}
void absentCatalog() {
  Fixture f(false);
  assert(!f.owner->resume() && f.owner->pending() == 0x8018a61c);
  const auto draws = f.draws;
  assert(!f.owner->resume() && f.draws == draws && f.noiseAttempts == 0);
}
void allocationReuse() {
  Fixture f;
  assert(!f.owner->resume());
  const auto old = f.battle;
  assert(f.pool->free(f.pool->root(), old));
  const auto fresh = f.pool->allocate(f.pool->root(), 0x19098);
  assert(fresh && *fresh == old);
  assert(!f.owner->resume() && !f.owner->current());
}
void sdkReplacement() {
  Fixture f;
  assert(!f.owner->resume());
  const auto draws = f.draws;
  f.vm.platform.fmaFloat = [](void *, float, float, float) { return 0.f; };
  assert(!f.owner->resume() && !f.owner->current() && f.draws == draws);
}
void missingPositiveSuffix() {
  Fixture f;
  assert(
      !f.owner->resume()); // Source initialization reached real missing noise.
  // TEST adversarial state producer after that held boundary, not actor birth
  // or native story evidence. NumToTarget counts state11 but excludes state12.
  assert(f.vm.memory.writeHalf(Pouch + 0x8c, 1));
  f.w(f.audience() + 0x13790, 50);
  f.w(f.audience() + 0x1377c, 0x40000000); // binary32 two
  f.w(f.audience() + 0x13780, 0);
  f.w(f.audience() + 0x1bc, 1);
  f.w(f.audience() + 0x1bc + 0x134, 1);
  assert(f.vm.memory.writeByte(f.audience() + 0x1bc + 0x19, 11));
  assert(f.vm.memory.writeByte(f.audience() + 0x1bc + 0x134 + 0x19, 12));
  f.testAudio = true;
  assert(!f.owner->resume());
  assert(!f.owner->resume());
  assert(!f.owner->resume() && f.owner->pending() == 0x801a176c &&
         f.addAttempts == 1);
  const auto draws = f.draws;
  // A later empty pouch cannot recapture/replay an already-held positive child.
  assert(f.vm.memory.writeHalf(Pouch + 0x8c, 0));
  f.w(f.audience() + 0x1377c, 0);
  assert(!f.owner->resume() && f.addAttempts == 2 && f.draws == draws);
}
void modeledSuffix() {
  Fixture f;
  f.testAudio = true;
  assert(!f.owner->resume());
  const auto draws = f.draws;
  assert(!f.owner->resume() && f.draws == draws);
  assert(f.owner->resume() && f.owner->complete());
  assert(f.owner->resume() && f.draws == draws && f.partialAudio == 2);
  const auto sound = f.audience() + 0x133fc + 2 * 0x24;
  assert(f.word(sound + 0x18) == 180);
  uint8_t target;
  assert(f.vm.memory.readByte(sound + 0x20, target) && target == 60);
}
} // namespace
int main() {
  nativeBoundary();
  absentCatalog();
  allocationReuse();
  sdkReplacement();
  modeledSuffix();
  missingPositiveSuffix();
  std::cout
      << "actual VM/map allocation/TPL owner audience binding, real "
         "missing-audio boundary PASS; final-suffix protocol uses TEST audio\n";
}
