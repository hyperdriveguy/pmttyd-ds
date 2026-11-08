#include "battle_audience_init_continuation.hpp"
#include "battle_audience_population.hpp"
#include "battle_timing_a.hpp"
#include "event_script_assets.hpp"
#include "field_battle_information.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t Battle = 0x10000, Audience = Battle + 0x2858,
                   Pouch = 0x40000, Setup = 0x50000, Information = 0x51000;
uint32_t bits(float f) {
  uint32_t v;
  std::memcpy(&v, &f, 4);
  return v;
}
float real(uint32_t b) {
  float f;
  std::memcpy(&f, &b, 4);
  return f;
}
int32_t integer(uint32_t v) {
  return v < 0x80000000u ? int32_t(v) : int32_t(int64_t(v) - 0x100000000ll);
}
double wide(uint64_t b) {
  double f;
  std::memcpy(&f, &b, 8);
  return f;
}
struct Op {
  uint32_t kind, address, value;
  bool operator==(const Op &o) const {
    return kind == o.kind && address == o.address && value == o.value;
  }
};
struct Fixture {
  EventScriptAssets original, harbor;
  std::vector<uint8_t> ram = std::vector<uint8_t>(0x100000);
  std::map<uint32_t, uint8_t> globals;
  std::vector<Op> operations;
  BattleRandom rng{1};
  std::vector<int32_t> bounds;
  std::vector<BattleAudienceSetupRequest> calls;
  std::vector<BattleAudienceSetupNumberRequest> numbers;
  std::shared_ptr<const uint8_t> owner = std::make_shared<const uint8_t>(0);
  BattleAudienceSetupServices services;
  BattleAudienceInitCursor init;
  BattleAudienceSettingCursor setting;
  size_t holdCall = SIZE_MAX, holdNumber = SIZE_MAX;
  unsigned callAttempts = 0, numberAttempts = 0, resets = 0, entries = 0;
  bool alive = true, reenter = false, onlySetting = true;
  unsigned partialTpl = 0;
  const BattleAudienceSetupRequest *heldCallAddress = nullptr;
  const BattleAudienceSetupNumberRequest *heldNumberAddress = nullptr;
  uint8_t byte(uint32_t a) {
    if (a < ram.size())
      return ram[a];
    auto i = globals.find(a);
    if (i != globals.end())
      return i->second;
    uint8_t v;
    if (original.readByte(a, v) || harbor.readByte(a, v))
      return v;
    std::cerr << "unavailable original fixture byte " << std::hex << a << '\n';
    std::abort();
  }
  uint32_t word(uint32_t a) {
    const auto v = (uint32_t(byte(a)) << 24) | (uint32_t(byte(a + 1)) << 16) |
                   (uint32_t(byte(a + 2)) << 8) | byte(a + 3);
    operations.push_back({0, a, v});
    return v;
  }
  uint16_t half(uint32_t a) {
    const auto v = uint16_t((uint16_t(byte(a)) << 8) | byte(a + 1));
    operations.push_back({1, a, v});
    return v;
  }
  void put(uint32_t a, uint8_t v) {
    if (a < ram.size())
      ram[a] = v;
    else
      globals[a] = v;
  }
  void word(uint32_t a, uint32_t v) {
    operations.push_back({2, a, v});
    for (unsigned i = 0; i < 4; ++i)
      put(a + i, uint8_t(v >> (24 - 8 * i)));
  }
  void half(uint32_t a, uint16_t v) {
    operations.push_back({3, a, v});
    put(a, uint8_t(v >> 8));
    put(a + 1, uint8_t(v));
  }
  void byteStore(uint32_t a, uint8_t v) {
    operations.push_back({4, a, v});
    put(a, v);
  }
  uint32_t base() { return word(0x8041eb90) + 0x2858; }
  bool run() {
    return onlySetting ? resumePackedBattleAudienceSetting(services, setting)
                       : resumePackedBattleAudienceInit(services, init);
  }
  uint32_t pending() {
    return onlySetting ? setting.pending() : init.pending();
  }
  Fixture(unsigned seed = 1) {
    assert(original.load("romfs/trio-attack-events.pmevt"));
    assert(harbor.load("romfs/harbor-tutorial-events.pmevt"));
    rng.state = seed;
    word(0x8041eb90, Battle);
    word(0x8041eb00, Pouch);
    word(Battle + 0x2738, Information);
    word(Information + 0xc, Setup);
    half(Pouch + 0x8a, 1);
    // Explicit TEST setup4 parameters for broad differential comparison. The
    // separate originalHarbor test reads all original setup2 ranges instead.
    for (unsigned i = 0; i < 16; ++i)
      put(Setup + 0xb0 + i, i == 10 ? 5 : i >= 13 ? 1 : 0);
    auto &s = services;
    auto &m = s.actors.memory;
    s.user = this;
    s.lifetime = owner;
    s.actors.user = this;
    m.user = this;
    m.word = [](void *p, uint32_t a) {
      return static_cast<Fixture *>(p)->word(a);
    };
    m.half = [](void *p, uint32_t a) {
      return static_cast<Fixture *>(p)->half(a);
    };
    m.storeWord = [](void *p, uint32_t a, uint32_t v) {
      static_cast<Fixture *>(p)->word(a, v);
    };
    m.storeHalf = [](void *p, uint32_t a, uint16_t v) {
      static_cast<Fixture *>(p)->half(a, v);
    };
    m.storeByte = [](void *p, uint32_t a, uint8_t v) {
      static_cast<Fixture *>(p)->byteStore(a, v);
    };
    s.actors.byte = [](void *p, uint32_t a) {
      auto &f = *static_cast<Fixture *>(p);
      const auto v = f.byte(a);
      f.operations.push_back({5, a, v});
      return v;
    };
    s.actors.audienceLifecycle.user = this;
    s.actors.audienceLifecycle.event =
        [](void *p, BattleAudienceLifecycleEvent e, uint32_t a, uint32_t) {
          auto &f = *static_cast<Fixture *>(p);
          if (e == BattleAudienceLifecycleEvent::Reset) {
            ++f.resets;
            f.operations.push_back({6, a, 0});
          }
        };
    s.current = [](void *p) {
      auto &f = *static_cast<Fixture *>(p);
      if (f.reenter) {
        assert(!f.run());
        assert(f.onlySetting ? !f.setting.reset() : !f.init.reset());
      }
      return f.alive;
    };
    s.call = [](void *p, const BattleAudienceSetupRequest &q) {
      return static_cast<Fixture *>(p)->call(q);
    };
    s.number = [](void *p, const BattleAudienceSetupNumberRequest &q) {
      return static_cast<Fixture *>(p)->number(q);
    };
    operations.clear();
  }
  BattleAudiencePopulationServices population() {
    BattleAudiencePopulationServices p;
    p.actors = services.actors;
    p.user = this;
    p.random = [](void *u, int32_t n) {
      auto &f = *static_cast<Fixture *>(u);
      f.bounds.push_back(n);
      return f.rng.bounded(n);
    };
    p.randomRaw = [](void *u) -> std::optional<int32_t> {
      return static_cast<Fixture *>(u)->rng.next();
    };
    p.truncate = [](void *, float f) -> std::optional<int32_t> {
      if (!std::isfinite(f) || f < -2147483648.f || f >= 2147483648.f)
        return {};
      return int32_t(f);
    };
    p.fma = [](void *, float a, float b, float c) { return std::fma(a, b, c); };
    return p;
  }
  std::optional<BattleAudienceSetupOutput>
  call(const BattleAudienceSetupRequest &q) {
    if (calls.size() == holdCall && ++callAttempts <= 2) {
      if (heldCallAddress)
        assert(heldCallAddress == &q);
      else
        heldCallAddress = &q;
      return {};
    }
    calls.push_back(q);
    BattleAudienceSetupOutput out;
    switch (q.function) {
    case 0x80005da0:
      bounds.push_back(integer(q.arguments[0]));
      out.value = uint32_t(rng.bounded(integer(q.arguments[0])));
      break;
    case 0x800d3bac:
      out.singleBits = word(word(0x8041eb00) + 0x84);
      break;
    case 0x8000519c:
      assert(q.privateCopy && q.privateCopy->destination == q.arguments[0] &&
             q.arguments[2] == 48);
      for (auto v : q.privateCopy->bytes)
        if (!v)
          return {};
      // TEST nonalias ordinary RAM copy policy; original SP ordering is not
      // fabricated. Native integration must admit its own actual whole owner.
      for (unsigned i = 0; i < 48; ++i)
        byteStore(q.arguments[0] + i, *q.privateCopy->bytes[i]);
      break;
    case 0x801a25e0: {
      const auto v = packedBattleAudienceFreeSeatUniform(population());
      assert(v);
      out.value = uint32_t(*v);
      break;
    }
    case 0x801a21a0: {
      const auto v = packedBattleAudienceFreeFrontThenBack(population());
      assert(v);
      out.value = uint32_t(*v);
      break;
    }
    case 0x801a3400: {
      const auto v = packedBattleAudienceEntry(
          integer(q.arguments[0]), integer(q.arguments[1]),
          integer(q.arguments[2]), population());
      assert(v);
      out.value = uint32_t(*v);
      ++entries;
      break;
    }
    case 0x801a86ac:
      assert(q.arguments[0] == 0x802fa710);
      out.value = 0x71000;
      break;
    case 0x801a6050: {
      assert(q.arguments[0] == 0 && q.arguments[1] == 12 &&
             q.arguments[2] == 0x802fa734);
      const auto a = base();
      word(a + 0x10, 0x72000);
      byteStore(a + 0x18, 12);
      break;
    }
    case 0x801a5a0c: {
      const auto a = base();
      bounds.push_back(100);
      const auto v = rng.bounded(100);
      word(a + 0x137f4, v < 80 ? 0 : v < 90 ? 1 : v < 95 ? 2 : 3);
      word(a + 0x137f0, 0);
      word(a + 0x137f8, 0);
      break;
    }
    // Explicit TEST audio/target whole owners, not actual audio/actor proof.
    case 0x801a0b88:
    case 0x801a1c60:
      break;
    case 0x8019fe28:
      assert((q.arguments == std::array<uint32_t, 3>{2, 60, 180}));
      break;
    default:
      assert(false);
    }
    return out;
  }
  std::optional<uint32_t> number(const BattleAudienceSetupNumberRequest &q) {
    if (numbers.size() == holdNumber && ++numberAttempts <= 2) {
      if (heldNumberAddress)
        assert(heldNumberAddress == &q);
      else
        heldNumberAddress = &q;
      return {};
    }
    numbers.push_back(q);
    const auto a = q.operands[0], b = q.operands[1], c = q.operands[2];
    switch (q.operation) {
    case BattleAudienceSetupNumeric::SubtractDoubleToSingle:
      return bits(float(wide(a) - wide(b)));
    case BattleAudienceSetupNumeric::AddSingle:
      return bits(real(uint32_t(a)) + real(uint32_t(b)));
    case BattleAudienceSetupNumeric::MultiplySingle:
      return bits(real(uint32_t(a)) * real(uint32_t(b)));
    case BattleAudienceSetupNumeric::FmaSingle:
      return bits(
          std::fma(real(uint32_t(a)), real(uint32_t(b)), real(uint32_t(c))));
    case BattleAudienceSetupNumeric::TruncateSingle: {
      const auto f = real(uint32_t(a));
      if (!std::isfinite(f) || f < -2147483648.f || f >= 2147483648.f)
        return {};
      return uint32_t(int32_t(f));
    }
    case BattleAudienceSetupNumeric::DivideSigned: {
      const auto n = integer(uint32_t(a)), d = integer(uint32_t(b));
      if (!d || (n == INT32_MIN && d == -1))
        return {};
      return uint32_t(n / d);
    }
    }
    return {};
  }
  void positive(unsigned n, unsigned mode) {
    half(Pouch + 0x8c, 1);
    word(Battle + 0x19078, 0x10000);
    word(Battle + 0x19084, n);
    word(Setup + 0xac, mode);
  }
};
void referenceMatrix() {
  for (unsigned mode = 0; mode < 7; ++mode)
    for (unsigned seed = 1; seed <= 8; ++seed)
      for (unsigned count : {0u, 7u, 77u}) {
        Fixture f(seed), reference(seed);
        f.positive(count, mode);
        reference.positive(count, mode);
        if (!f.run()) {
          std::cerr << "setting held mode=" << mode << " seed=" << seed
                    << " count=" << count << " pending=" << std::hex
                    << f.pending() << "\n";
          std::abort();
        }
        assert(
            initializePackedBattleAudiencePopulation(reference.population()));
        assert(f.ram == reference.ram && f.rng.state == reference.rng.state &&
               f.bounds == reference.bounds);
      }
}
void waits() {
  Fixture baseline;
  baseline.onlySetting = false;
  assert(baseline.run());
  for (size_t i = 0; i < baseline.calls.size(); ++i) {
    Fixture f;
    f.onlySetting = false;
    f.holdCall = i;
    f.reenter = true;
    assert(!f.run());
    const auto prefix = f.operations;
    assert(!f.run() && f.operations == prefix);
    f.reenter = false;
    assert(f.run());
    assert(f.operations == baseline.operations && f.ram == baseline.ram &&
           f.bounds == baseline.bounds && f.resets == 1);
  }
  for (size_t i = 0; i < baseline.numbers.size(); ++i) {
    Fixture f;
    f.onlySetting = false;
    f.holdNumber = i;
    assert(!f.run());
    const auto prefix = f.operations;
    assert(!f.run() && f.operations == prefix);
    assert(f.run());
    assert(f.operations == baseline.operations && f.ram == baseline.ram);
  }
}
void branchWaits() {
  // Cold entry exhausts every individual callback occurrence above. Positive
  // modes additionally cover each distinct source child/arithmetic site.
  for (unsigned mode = 0; mode < 7; ++mode) {
    Fixture baseline;
    baseline.positive(mode == 0 ? 77 : 7, mode);
    assert(baseline.run());
    std::set<uint32_t> reached;
    for (size_t i = 0; i < baseline.calls.size(); ++i) {
      if (!reached.insert(baseline.calls[i].callsite).second)
        continue;
      Fixture f;
      f.positive(mode == 0 ? 77 : 7, mode);
      f.holdCall = i;
      assert(!f.run());
      const auto prefix = f.operations;
      assert(!f.run() && f.operations == prefix);
      assert(f.run() && f.operations == baseline.operations &&
             f.ram == baseline.ram);
    }
    reached.clear();
    for (size_t i = 0; i < baseline.numbers.size(); ++i) {
      if (!reached.insert(baseline.numbers[i].source).second)
        continue;
      Fixture f;
      f.positive(mode == 0 ? 77 : 7, mode);
      f.holdNumber = i;
      assert(!f.run());
      const auto prefix = f.operations;
      assert(!f.run() && f.operations == prefix);
      assert(f.run() && f.operations == baseline.operations &&
             f.ram == baseline.ram);
    }
  }
}
void overridesAndRanks() {
  for (unsigned override = 1; override <= 13; ++override) {
    Fixture f(11), reference(11);
    f.word(Battle + 0x19088, override);
    reference.word(Battle + 0x19088, override);
    assert(f.run() &&
           initializePackedBattleAudiencePopulation(reference.population()));
    assert(f.ram == reference.ram && f.bounds == reference.bounds);
  }
  for (int rank : {-1, 0, 1, 2, 3, 4}) {
    Fixture f;
    f.half(Pouch + 0x88, uint16_t(rank));
    f.half(Pouch + 0x8c, 1);
    f.word(Audience + 0x13790, 123);
    assert(f.run());
    assert(f.word(Audience + 0x13790) ==
           (rank >= 0 && rank <= 3 ? uint32_t(50 * (rank + 1)) : 123));
  }
}
void ownership() {
  Fixture f;
  f.holdCall = 0;
  assert(!f.run());
  const auto prefix = f.operations;
  auto s = f.services;
  s.user = nullptr;
  assert(!resumePackedBattleAudienceSetting(s, f.setting));
  s = f.services;
  s.lifetime = std::shared_ptr<const void>(f.owner.get(), [](const void *) {});
  assert(!resumePackedBattleAudienceSetting(s, f.setting));
  s = f.services;
  s.actors.audienceLifecycle.user = &s;
  assert(!resumePackedBattleAudienceSetting(s, f.setting));
  f.alive = false;
  assert(!f.run());
  f.alive = true;
  assert(!f.run() && f.operations == prefix);
  assert(f.setting.reset());
}
void originalHarborParameters() {
  Fixture f(7);
  std::ifstream catalog("romfs/harbor-tutorial-program.bin", std::ios::binary);
  std::array<uint8_t, 56> header{};
  catalog.read(reinterpret_cast<char *>(header.data()), header.size());
  assert(catalog);
  auto little = [&](unsigned i) {
    const auto o = 8 + 4 * i;
    return uint32_t(header[o]) | (uint32_t(header[o + 1]) << 8) |
           (uint32_t(header[o + 2]) << 16) | (uint32_t(header[o + 3]) << 24);
  };
  const auto configuration = little(9) + 0x60eb8 - 0x60c90;
  PackedNpcBattleSetupServices setup;
  setup.memory = f.services.actors;
  setup.user = &f;
  setup.random = [](void *p, int32_t n) {
    auto &f = *static_cast<Fixture *>(p);
    f.bounds.push_back(n);
    return f.rng.bounded(n);
  };
  setup.savedFlag = [](void *, int32_t) -> int32_t {
    assert(false && "unexpected source Harbor flag branch");
    return 0;
  };
  setupPackedNpcBattleInformation(Setup - 0x230, configuration, setup);
  assert(f.word(Setup) == little(10));
  const auto setupRandom = f.bounds.size();
  assert(setupRandom >= 16);
  f.onlySetting = false;
  assert(f.run());
  assert(f.resets == 1 && f.entries == 0 && f.word(Audience + 0x13790) == 0);
  assert(f.word(Audience + 0x13784) == 0 &&
         f.word(Audience + 0x1375c + 0x18) == 0);
  unsigned zero = 0;
  for (size_t i = setupRandom; i < f.bounds.size(); ++i)
    zero += f.bounds[i] == 0;
  assert(zero ==
         300); // True empty source list still issues every shuffle draw.
}
void retainedCaptures() {
  Fixture f;
  f.onlySetting = false;
  constexpr uint32_t OtherBattle = 0x70000,
                     OtherAudience = OtherBattle + 0x2858;
  const auto originalCall = f.services.call;
  assert(originalCall);
  f.services.call = [](void *p, const BattleAudienceSetupRequest &q)
      -> std::optional<BattleAudienceSetupOutput> {
    auto &f = *static_cast<Fixture *>(p);
    if (q.function == 0x801a86ac && f.partialTpl < 2) {
      if (f.partialTpl == 0) {
        f.word(0x8041eb90, OtherBattle);
        // Real source GetExist then GetActor must use their own fresh global.
        f.word(OtherAudience + 0x1bc, 1);
        f.word(OtherAudience + 0x1bc + 10 * 0x134, 1);
        f.word(OtherAudience + 0x1bc + 11 * 0x134, 1);
        f.put(OtherAudience + 0x1bc + 11 * 0x134 + 0x19, 12);
      }
      ++f.partialTpl;
      return {};
    }
    return f.call(q);
  };
  assert(!f.run());
  const auto prefix = f.operations;
  assert(!f.run() && f.operations == prefix);
  assert(f.run());
  assert(f.word(Audience + 0xc) == 0x71000 && f.word(OtherAudience + 0xc) == 0);
  assert(f.word(Audience + 0x13788) == 1 && f.word(Audience + 0x1378c) == 1 &&
         f.word(Audience + 0x13784) == 2);
  assert(f.word(Audience + 0x1375c + 0x18) == bits(2));
  assert(f.word(OtherAudience + 0x10) == 0x72000);
  size_t bias = SIZE_MAX, publish = SIZE_MAX;
  for (size_t i = 0; i < f.operations.size(); ++i) {
    const auto &op = f.operations[i];
    if (op.kind == 0 && op.address == 0x802fa328)
      bias = i;
    if (op.kind == 2 && op.address == Audience + 0x13784 && op.value == 2)
      publish = i;
  }
  assert(bias < publish); // Original LFD precedes count publication.
}
void privateUnknowns() {
  Fixture f;
  f.half(Pouch + 0x8a, 0);
  assert(!f.run());
  assert(f.pending() == 0x801a61e4);
  const auto prefix = f.operations;
  assert(!f.run() && f.operations == prefix);
  Fixture divisor;
  for (unsigned i = 0; i < 16; ++i)
    divisor.put(Setup + 0xb0 + i, 0);
  assert(!divisor.run() && divisor.pending() == 0x801a6758);
}
} // namespace
int main() {
  referenceMatrix();
  waits();
  branchWaits();
  overridesAndRanks();
  ownership();
  originalHarborParameters();
  retainedCaptures();
  privateUnknowns();
  std::cout << "retained whole audience Init/Setting differential, waits and "
               "ownership PASS (TEST linked owners)\n";
}
