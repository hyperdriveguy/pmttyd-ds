#include "event_native_runtime.hpp"
#include "field_npc_clock_binding.hpp"
#include "game.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <map>
#include <tuple>
#include <vector>
using namespace ttyd;
namespace {
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
constexpr uint32_t Actor = 0x10000, Gp = 0x20000;
struct Fixture {
  std::map<uint32_t, uint32_t> words;
  using Effect = std::tuple<char, uint32_t, uint64_t>;
  std::vector<Effect> trace;
  unsigned attempts = 0, fail = ~0u;
  bool literalMutation = false, readMutation = false, busMutation = false;
  Fixture() {
    for (uint32_t a = 0; a < 0x1d8; a += 4)
      words[Actor + a] = 0;
    words[Actor + 0x1d4] = 0x10;
    words[Actor] = 0x20000;
    words[0x80415fd0] = Gp;
    time(1032750);
    words[0x800000f8] = 243000000;
    words[0x8041fd50] = bits(1000000.f);
  }
  void time(uint64_t n) {
    words[Gp + 0x38] = uint32_t(n >> 32);
    words[Gp + 0x3c] = uint32_t(n);
  }
  void pair(uint32_t offset, uint64_t n) {
    words[Actor + offset] = uint32_t(n >> 32);
    words[Actor + offset + 4] = uint32_t(n);
  }
  uint64_t pair(uint32_t offset) const {
    return uint64_t(words.at(Actor + offset)) << 32 |
           words.at(Actor + offset + 4);
  }
  bool admitted() { return attempts++ != fail; }
  FieldNpcClockServices services() {
    FieldNpcClockServices s;
    s.user = this;
    s.word = [](void *p, uint32_t a) -> std::optional<uint32_t> {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.admitted())
        return {};
      const auto it = f.words.find(a);
      if (it == f.words.end())
        return {};
      const auto v = it->second;
      f.trace.emplace_back('r', a, v);
      // A read-visible mutation verifies that an earlier captured low lane is
      // not recaptured after a later high-lane provider holds.
      if (f.readMutation && a == Gp + 0x3c)
        f.words[Gp + 0x3c] += 100;
      return v;
    };
    s.store = [](void *p, uint32_t a, uint32_t v) {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.admitted())
        return false;
      f.words[a] = v;
      f.trace.emplace_back('w', a, v);
      if (f.busMutation && a == Actor + 0x1a0)
        f.words[0x800000f8] = 162000000;
      return true;
    };
    s.divide = [](void *p, uint64_t n, uint32_t d, uint64_t &out) {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.admitted() || !nativeFieldNpcClockDivide(nullptr, n, d, out))
        return false;
      f.trace.emplace_back('d', d, n);
      return true;
    };
    s.convert = [](void *p, uint64_t n, float &out) {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.admitted())
        return false;
      assert(nativeFieldNpcClockConvert(nullptr, n, out));
      f.trace.emplace_back('f', bits(out), n);
      if (f.literalMutation)
        f.words[0x8041fd50] = bits(2000000.f);
      return true;
    };
    return s;
  }
  bool run(FieldNpcClockCursor &c) {
    return updateFieldNpcClock(Actor, services(), c);
  }
};
void values() {
  Fixture f;
  FieldNpcClockCursor c;
  assert(f.run(c) && !c.active);
  assert(f.pair(0x190) == 1032750 && f.pair(0x178) == 1032750 &&
         f.pair(0x188) == 1032750 && f.pair(0x198) == 1032750);
  for (uint32_t offset : {0x1a0u, 0x180u, 0x1a4u, 0x1a8u})
    assert(f.words[Actor + offset] == bits(.017f));
  assert(f.trace[0] == Fixture::Effect('r', Actor + 0x1d4, 0x10));
  const std::vector<uint32_t> initialReads = {
      0x80415fd0, Actor + 0x19c, Gp + 0x3c, Gp + 0x38, Actor + 0x198};
  for (unsigned i = 0; i < initialReads.size(); ++i)
    assert(std::get<0>(f.trace[i + 1]) == 'r' &&
           std::get<1>(f.trace[i + 1]) == initialReads[i]);
  assert(f.trace[6] == Fixture::Effect('w', Actor + 0x194, 1032750) &&
         f.trace[7] == Fixture::Effect('w', Actor + 0x190, 0));
  // Independent original-listing trace for this admitted input. The capture
  // order includes the low-first initial timestamp and high-first refresh.
  const std::vector<Fixture::Effect> prefix = {
      {'r', Actor + 0x1d4, 0x10},    {'r', 0x80415fd0, Gp},
      {'r', Actor + 0x19c, 0},       {'r', Gp + 0x3c, 1032750},
      {'r', Gp + 0x38, 0},           {'r', Actor + 0x198, 0},
      {'w', Actor + 0x194, 1032750}, {'w', Actor + 0x190, 0},
      {'r', 0x800000f8, 243000000},  {'r', Actor + 0x190, 0},
      {'r', Actor + 0x194, 1032750}, {'d', 60750, 1032750},
      {'r', Actor, 0x20000},         {'r', Actor + 0x17c, 0},
      {'r', Actor + 0x194, 1032750}, {'r', Actor + 0x178, 0},
      {'r', Actor + 0x190, 0},       {'w', Actor + 0x17c, 1032750},
      {'w', Actor + 0x178, 0},       {'r', Actor + 0x18c, 0},
      {'r', Actor + 0x194, 1032750}, {'r', Actor + 0x188, 0},
      {'r', Actor + 0x190, 0},       {'w', Actor + 0x18c, 1032750},
      {'w', Actor + 0x188, 0},       {'r', 0x80415fd0, Gp},
      {'r', Gp + 0x38, 0},           {'r', Gp + 0x3c, 1032750},
      {'w', Actor + 0x19c, 1032750}, {'w', Actor + 0x198, 0}};
  auto originalTrace = prefix;
  for (auto lanes : {std::pair<uint32_t, uint32_t>{0x188, 0x1a0},
                     {0x178, 0x180},
                     {0x190, 0x1a4},
                     {0x198, 0x1a8}}) {
    originalTrace.insert(originalTrace.end(),
                         {{'r', 0x800000f8, 243000000},
                          {'r', Actor + lanes.first, 0},
                          {'r', Actor + lanes.first + 4, 1032750},
                          {'d', 486, 8262000},
                          {'f', bits(17000.f), 17000},
                          {'r', 0x8041fd50, bits(1000000.f)},
                          {'w', Actor + lanes.second, bits(.017f)}});
  }
  assert(f.trace == originalTrace);
  Fixture fractional;
  fractional.time(1032750 + 123);
  FieldNpcClockCursor fc;
  assert(fractional.run(fc));
  assert(fractional.words[Actor + 0x1a4] ==
         bits(.017002f)); // 17002 integer microseconds, not17ms.
  Fixture freshBus;
  freshBus.busMutation = true;
  FieldNpcClockCursor fb;
  assert(freshBus.run(fb) && freshBus.words[Actor + 0x1a0] == bits(.017f));
  assert(freshBus.words[Actor + 0x180] == bits(.0255f) &&
         freshBus.words[Actor + 0x1a4] == bits(.0255f));
  Fixture freshLiteral;
  freshLiteral.literalMutation = true;
  FieldNpcClockCursor fl;
  assert(freshLiteral.run(fl) &&
         freshLiteral.words[Actor + 0x1a4] == bits(.0085f));
  Fixture noAccum;
  noAccum.words[Actor] = 0;
  noAccum.pair(0x178, 12345);
  FieldNpcClockCursor nc;
  assert(noAccum.run(nc) && noAccum.pair(0x178) == 0 &&
         noAccum.words[Actor + 0x180] == 0);
  // Millisecond quotient exactly500 does not clamp, including a fractional ms.
  for (uint64_t ticks : {500 * 60750ULL, 501 * 60750ULL - 1}) {
    Fixture edge;
    edge.time(ticks);
    FieldNpcClockCursor ec;
    assert(edge.run(ec) && edge.pair(0x190) == ticks);
  }
  Fixture cap;
  cap.time(501 * 60750ULL);
  FieldNpcClockCursor cc;
  assert(cap.run(cc) && cap.pair(0x190) == 16 * 60750 &&
         cap.words[Actor + 0x1a4] == bits(.016f));
  // Signed negative delta is not a large positive duration; accumulation wraps.
  Fixture negative;
  negative.time(1);
  negative.pair(0x198, 60751);
  negative.pair(0x188, 60749);
  negative.pair(0x178, 60750);
  FieldNpcClockCursor neg;
  assert(negative.run(neg) && negative.pair(0x190) == uint64_t(-60750LL) &&
         negative.pair(0x178) == 0 && negative.pair(0x188) == UINT64_MAX);
  assert(negative.words[Actor + 0x1a4] == bits(-.001f));
  Fixture wrapped;
  wrapped.pair(0x188, uint64_t(1) << 60);
  wrapped.pair(0x178, UINT64_MAX - 1032749);
  FieldNpcClockCursor wc;
  assert(wrapped.run(wc));
  assert(wrapped.pair(0x178) == 0 &&
         real(wrapped.words[Actor + 0x1a0]) <
             0); // source<<3 wraps before signed division.
}
void stopped() {
  Fixture f;
  f.words[Actor + 0x1d4] = 0;
  f.words[Actor + 0x1a4] = bits(7.f);
  f.pair(0x190, 123);
  auto before = f.words;
  FieldNpcClockCursor c;
  auto s = f.services();
  s.divide = nullptr;
  s.convert = nullptr;
  assert(updateFieldNpcClock(Actor, s, c));
  assert(f.pair(0x198) == 1032750 && f.pair(0x190) == 123 &&
         f.words[Actor + 0x1a4] == bits(7.f));
  before[Actor + 0x19c] = 1032750;
  assert(f.words == before &&
         f.trace.size() == 6); // state, GP,high,low,storelow,storehigh.
}
void holds() {
  Fixture baseline;
  baseline.readMutation = true;
  baseline.literalMutation = true;
  baseline.busMutation = true;
  FieldNpcClockCursor complete;
  assert(baseline.run(complete));
  const auto expected = baseline.words;
  const auto trace = baseline.trace;
  const auto operations = baseline.attempts;
  for (unsigned n = 0; n < operations; ++n) {
    Fixture f;
    f.readMutation = true;
    f.literalMutation = true;
    f.busMutation = true;
    f.fail = n;
    FieldNpcClockCursor c;
    assert(!f.run(c) && c.active);
    const auto phase = c.phase;
    const auto count = f.trace.size();
    auto wrong = f.services();
    wrong.user = &baseline;
    assert(!updateFieldNpcClock(Actor, wrong, c) && c.phase == phase &&
           f.trace.size() == count);
    assert(!updateFieldNpcClock(Actor + 4, f.services(), c));
    assert(
        f.run(c) && !c.active && f.words == expected &&
        f.trace ==
            trace); // Every successful primitive exactly once, in source order.
  }
  // Repeat every primitive hold along the source-only stopped branch and the
  // >500ms clamp / disabled-accumulator branch, including their low/high
  // stores.
  for (unsigned path = 0; path < 2; ++path) {
    Fixture baseline;
    if (path == 0)
      baseline.words[Actor + 0x1d4] = 0;
    else {
      baseline.time(501 * 60750ULL);
      baseline.words[Actor] = 0;
    }
    FieldNpcClockCursor done;
    assert(baseline.run(done));
    for (unsigned failure = 0; failure < baseline.attempts; ++failure) {
      Fixture f;
      if (path == 0)
        f.words[Actor + 0x1d4] = 0;
      else {
        f.time(501 * 60750ULL);
        f.words[Actor] = 0;
      }
      f.fail = failure;
      FieldNpcClockCursor held;
      assert(!f.run(held) && held.active);
      assert(f.run(held));
      assert(f.words == baseline.words && f.trace == baseline.trace);
    }
  }
  Fixture missing;
  missing.words.erase(0x8041fd50);
  FieldNpcClockCursor held;
  assert(!missing.run(held) && held.phase == 45);
  const auto at = missing.trace.size();
  missing.words[0x8041fd50] = bits(1000000.f);
  assert(missing.run(held));
  assert(std::get<0>(missing.trace[at]) == 'r' &&
         std::get<1>(missing.trace[at]) ==
             0x8041fd50); // Conversion is not repeated.
  Fixture zero;
  zero.words[0x800000f8] = 0;
  FieldNpcClockCursor invalid;
  assert(!zero.run(invalid) && invalid.phase == 11 &&
         zero.pair(0x190) == 1032750 && zero.pair(0x188) == 0);
}
void numeric() {
  uint64_t out = 123;
  assert(!nativeFieldNpcClockDivide(nullptr, 1, 0, out) && out == 123);
  assert(nativeFieldNpcClockDivide(nullptr, uint64_t(INT64_MIN), 1, out) &&
         out == uint64_t(INT64_MIN));
  assert(nativeFieldNpcClockDivide(nullptr, uint64_t(-9LL), 2, out) &&
         out == uint64_t(-4LL));
  // Source double rounding differs from a direct int64->float conversion here.
  const uint64_t n = (uint64_t(1) << 54) + (uint64_t(1) << 30) + 1;
  float converted = 0;
  assert(nativeFieldNpcClockConvert(nullptr, n, converted) &&
         converted == float(uint64_t(1) << 54));
  volatile int64_t input = int64_t(n);
  assert(static_cast<float>(input) > converted);
}
void actualOwner() {
  constexpr uint32_t Base = 0x85000000, Model = 0x86000000;
  EventNativePlatform platform;
  platform.clock = [](void *) -> uint32_t { return 243000000; };
  auto vm = std::make_unique<EventNativeRuntime>(0x81000000, platform);
  EventScriptAssets source;
  assert(source.load("romfs/door-data.pmevt") &&
         vm->memory.importScripts(source));
  auto game = std::make_unique<Game>();
  auto registry = game->initializeOriginalFieldNpcs(vm->memory, Base);
  assert(registry);
  FieldNpcClockNumbers numbers;
  numbers.divide = nativeFieldNpcClockDivide;
  numbers.convert = nativeFieldNpcClockConvert;
  assert(
      !FieldNpcClockBinding::bind(game->originalFieldNpcs, *registry, *vm, {}));
  auto binding = FieldNpcClockBinding::bind(game->originalFieldNpcs, *registry,
                                            *vm, numbers);
  assert(binding);
  auto foreign = std::make_unique<EventNativeRuntime>(0x82000000, platform);
  assert(!FieldNpcClockBinding::bind(game->originalFieldNpcs, *registry,
                                     *foreign, numbers));
  auto cold = FieldNpcRegistryOwner::cold(0x87000000);
  assert(!FieldNpcClockBinding::bind(cold, *registry, *vm, numbers));
  const auto resource =
      loadBattlePoseResource("romfs/field-native-c_kuri_h.pose-directory.bin",
                             "romfs/field-native-c_kuri_h.pmanim");
  assert(resource && game->originalFieldNpcs->poses().install(resource));
  std::vector<uint8_t> model(64);
  for (unsigned i = 0; i < 64; ++i)
    model[i] = resource->draw->byte(4 + i);
  assert(vm->memory.registerRam(Model, std::move(model)));
  uint32_t name = 0;
  assert(vm->memory.readWord(0x90524f10 + 0x5c, name));
  const auto entered = registry->enter(name, Model, {});
  assert(entered && *entered >= 0);
  auto actor = registry->lookup(name);
  assert(actor && *actor);
  auto sw = [&](uint32_t a, uint32_t v) { assert(vm->memory.writeWord(a, v)); };
  auto w = [&](uint32_t a) {
    uint32_t v;
    assert(vm->memory.readWord(a, v));
    return v;
  };
  sw(*actor + 0x1d4, w(*actor + 0x1d4) | 0x10);
  vm->gp.timestamp = 1032750;
  FieldNpcClockCursor c;
  assert(binding->advance(*actor, c));
  assert(w(*actor + 0x1a4) == bits(.017f) && w(*actor + 0x194) == 1032750 &&
         w(*actor + 0x19c) == 1032750);
  // A second facade sees the same actual record publications, not a copyback.
  auto aliases =
      FieldNpcRegistryBinding::bind(game->originalFieldNpcs, foreign->memory);
  assert(aliases);
  uint32_t delta = 0;
  assert(foreign->memory.readWord(*actor + 0x1a4, delta) &&
         delta == bits(.017f));
  FieldNpcClockCursor invalid;
  assert(!binding->advance(*actor + 4, invalid) && !invalid.active);
}
} // namespace
int main() {
  values();
  stopped();
  holds();
  numeric();
  actualOwner();
}
