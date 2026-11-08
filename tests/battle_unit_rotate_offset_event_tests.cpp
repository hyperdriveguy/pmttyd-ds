#include "battle_unit_rotate_offset_event.hpp"
#include "event_manager.hpp"
#include "event_native_runtime.hpp"
#include "harbor_tutorial_packed.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t Args = 0x85000000, BattleGlobal = 0x8041eb90,
                   Battle = 0x84000000, Unit = 0x85001000, Bias = 0x802ee608,
                   Sentinel = 0x804226c4;
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
  EventEntry event;
  BattleUnitRotationEventServices services;
  std::map<uint32_t, uint32_t> memory;
  std::vector<std::pair<char, uint32_t>> trace;
  unsigned rounds = 0, stores = 0;
  bool mutate = false;
  Fixture() {
    event.arguments = Args;
    event.argumentCount = 4;
    memory[Args] = 2;
    memory[Args + 4] = 1;
    memory[Args + 8] = uint32_t(-250000000);
    memory[Args + 12] = uint32_t(-80);
    memory[BattleGlobal] = Battle;
    memory[Bias] = 0x43300000;
    memory[Bias + 4] = 0x80000000;
    memory[Sentinel] = bits(-250000000.f);
    memory[Unit + 0x78] = bits(33);
    memory[Unit + 0x7c] = bits(44);
    memory[Unit + 0x80] = bits(55);
    services.units.memory.user = this;
    services.units.memory.word = [](void *p, uint32_t at) {
      auto &f = *static_cast<Fixture *>(p);
      f.trace.push_back({'W', at});
      assert(f.memory.count(at));
      return f.memory.at(at);
    };
    services.units.memory.storeWord = [](void *p, uint32_t at, uint32_t value) {
      auto &f = *static_cast<Fixture *>(p);
      f.trace.push_back({'S', at});
      ++f.stores;
      f.memory[at] = value;
      if (f.mutate && f.stores == 1)
        f.memory[Sentinel] = bits(40);
    };
    services.units.user = this;
    services.units.values.user = this;
    services.units.values.resolve = [](void *p, EventEntry &, int32_t raw) {
      auto &f = *static_cast<Fixture *>(p);
      f.trace.push_back({'V', uint32_t(raw)});
      return raw;
    };
    services.units.translate = [](void *p, EventEntry &, int32_t id) {
      auto &f = *static_cast<Fixture *>(p);
      f.trace.push_back({'T', uint32_t(id)});
      return id;
    };
    services.units.unit = [](void *p, uint32_t b, int32_t id) {
      auto &f = *static_cast<Fixture *>(p);
      f.trace.push_back({'U', b});
      assert(b == Battle && id == 2);
      return Unit;
    };
    services.angle.motion.query.common.position.numericUser = this;
    services.angle.subtract = [](void *p, double a, double b) {
      auto &f = *static_cast<Fixture *>(p);
      f.trace.push_back({'D', f.rounds});
      return a - b;
    };
    services.angle.motion.roundDouble = [](void *p, double a) {
      auto &f = *static_cast<Fixture *>(p);
      f.trace.push_back({'R', f.rounds});
      ++f.rounds;
      if (f.mutate) {
        if (f.rounds == 1)
          f.memory[Args + 8] = 99;
        if (f.rounds == 2)
          f.memory[Args + 12] = 88;
        if (f.rounds == 3)
          f.memory[BattleGlobal] = 0x87000000;
      }
      return float(a);
    };
  }
};
} // namespace
int main() {
  // Actual native Packed admission rejects missing numeric owners before any
  // source argument read or unit write, even with an unmapped command domain.
  {
    EventNativeRuntime vm(0x81000000, {});
    HarborTutorialPacked packed(vm);
    EventEntry e;
    int32_t result = 17;
    assert(!packed.invoke(e, 0x80110f9c, 0, result) && result == 17);
    vm.platform.subtractDouble = [](void *, double a, double b) {
      return a - b;
    };
    assert(!packed.invoke(e, 0x80110f9c, 0, result) && result == 17);
    vm.platform.subtractDouble = nullptr;
    vm.platform.roundDoubleFloat = [](void *, double a) { return float(a); };
    assert(!packed.invoke(e, 0x80110f9c, 0, result) && result == 17);
  }

  Fixture f;
  assert(battleSetUnitRotateOffsetEvent(f.event, f.services) == 2);
  assert(f.rounds == 3 && f.stores == 2);
  assert(real(f.memory[Unit + 0x78]) == 1 &&
         real(f.memory[Unit + 0x7c]) == 44 &&
         real(f.memory[Unit + 0x80]) == -80);
  const std::vector<std::pair<char, uint32_t>> expected = {
      {'W', Args},
      {'V', 2},
      {'T', 2},
      {'W', Args + 4},
      {'V', 1},
      {'W', Args + 8},
      {'W', Bias},
      {'W', Bias + 4},
      {'D', 0},
      {'R', 0},
      {'V', uint32_t(-250000000)},
      {'W', Args + 12},
      {'W', Bias},
      {'W', Bias + 4},
      {'D', 1},
      {'R', 1},
      {'V', uint32_t(-80)},
      {'W', BattleGlobal},
      {'W', Bias},
      {'W', Bias + 4},
      {'D', 2},
      {'R', 2},
      {'U', Battle},
      {'W', Sentinel},
      {'S', Unit + 0x78},
      {'W', Sentinel},
      {'W', Sentinel},
      {'S', Unit + 0x80}};
  assert(f.trace == expected);
  Fixture mutation;
  mutation.mutate = true;
  assert(battleSetUnitRotateOffsetEvent(mutation.event, mutation.services) ==
         2);
  assert(mutation.rounds == 3 && mutation.stores == 3);
  assert(real(mutation.memory[Unit + 0x7c]) == -250000000.f &&
         real(mutation.memory[Unit + 0x80]) == -80);
  assert(mutation.memory[Args + 8] == 99 && mutation.memory[Args + 12] == 88 &&
         mutation.memory[BattleGlobal] == 0x87000000);
  for (int32_t x : std::array<int32_t, 6>{0, 1, -1, 16777217, 2147483647,
                                          int32_t(0x80000000u)}) {
    Fixture v;
    v.memory[Args + 4] = uint32_t(x);
    v.memory[Args + 8] = 0;
    v.memory[Args + 12] = 0;
    assert(battleSetUnitRotateOffsetEvent(v.event, v.services) == 2);
    assert(bits(real(v.memory[Unit + 0x78])) == bits(float(x)));
  }
  std::cout << "Complete SetRotateOffset source capture, shared conversion, "
               "fresh sentinel and stores pass\n";
}
