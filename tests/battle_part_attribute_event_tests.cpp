#include "battle_part_attribute_event.hpp"
#include "event_manager.hpp"
#include <cassert>
#include <iostream>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t Args = 0x85000000, BattleGlobal = 0x8041eb90,
                   Battle = 0x84000000, AlternateBattle = 0x84010000,
                   Unit = 0x86000000, Part = 0x87000000, Next = 0x87001000,
                   Definition = 0x88000000, OtherDefinition = 0x88001000;
struct Fixture {
  EventEntry event;
  BattleUnitEventServices services;
  std::map<uint32_t, uint32_t> memory;
  std::vector<std::pair<char, uint32_t>> trace;
  unsigned stores = 0;
  bool mutate = false;
  Fixture() {
    event.arguments = Args;
    event.argumentCount = 3;
    memory[Args] = 2;
    memory[Args + 4] = 1;
    memory[Args + 8] = 0x03000000;
    memory[BattleGlobal] = Battle;
    memory[Unit + 0x14] = Part;
    memory[Part] = 0;
    memory[Part + 0x1ac] = 0x40;
    services.user = this;
    services.values.user = this;
    services.memory.user = this;
    services.memory.word = [](void *p, uint32_t at) {
      auto &f = *static_cast<Fixture *>(p);
      f.trace.push_back({'R', at});
      assert(f.memory.count(at));
      return f.memory.at(at);
    };
    services.memory.storeWord = [](void *p, uint32_t at, uint32_t v) {
      auto &f = *static_cast<Fixture *>(p);
      f.trace.push_back({'S', at});
      ++f.stores;
      f.memory[at] = v;
    };
    services.values.resolve = [](void *p, EventEntry &, int32_t raw) {
      auto &f = *static_cast<Fixture *>(p);
      f.trace.push_back({'V', uint32_t(raw)});
      if (f.mutate && raw == 1) {
        f.memory[BattleGlobal] = AlternateBattle;
        f.memory[Args + 8] = 0x80000000;
      }
      return raw;
    };
    services.translate = [](void *p, EventEntry &, int32_t id) {
      auto &f = *static_cast<Fixture *>(p);
      f.trace.push_back({'T', uint32_t(id)});
      return id;
    };
    services.unit = [](void *p, uint32_t b, int32_t id) {
      auto &f = *static_cast<Fixture *>(p);
      f.trace.push_back({'U', b});
      assert(id == 2 && b == (f.mutate ? AlternateBattle : Battle));
      if (f.mutate)
        f.memory[Args + 8] = 0;
      return Unit;
    };
  }
};
} // namespace
int main() {
  Fixture f;
  assert(battleOnPartAttributeEvent(f.event, f.services) == 2 &&
         f.stores == 1 && f.memory[Part + 0x1ac] == 0x03000040);
  const std::vector<std::pair<char, uint32_t>> expected{{'R', Args},
                                                        {'V', 2},
                                                        {'T', 2},
                                                        {'R', Args + 4},
                                                        {'V', 1},
                                                        {'R', Args + 8},
                                                        {'R', BattleGlobal},
                                                        {'U', Battle},
                                                        {'R', Unit + 0x14},
                                                        {'R', Part},
                                                        {'R', Part + 0x1ac},
                                                        {'S', Part + 0x1ac}};
  assert(f.trace == expected);
  {
    Fixture v;
    v.memory[Args + 4] = 999;
    assert(battleOnPartAttributeEvent(v.event, v.services) == 2 &&
           v.stores == 1);
  }
  {
    Fixture v;
    v.memory[Args + 4] = uint32_t(-1);
    v.memory.erase(Part);
    assert(battleOnPartAttributeEvent(v.event, v.services) == 2 &&
           v.stores == 1);
    for (auto q : v.trace)
      assert(q != std::make_pair('R', Part));
  }
  {
    Fixture v;
    v.memory[Part] = Next;
    v.memory[Part + 4] = Definition;
    v.memory[Definition] = 7;
    v.memory[Next] = 0;
    v.memory[Next + 4] = OtherDefinition;
    v.memory[OtherDefinition] = 1;
    v.memory[Next + 0x1ac] = 0x80;
    assert(battleOnPartAttributeEvent(v.event, v.services) == 2 &&
           v.memory[Next + 0x1ac] == 0x03000080 &&
           v.memory[Part + 0x1ac] == 0x40);
  }
  {
    Fixture v;
    v.mutate = true;
    assert(battleOnPartAttributeEvent(v.event, v.services) == 2 &&
           v.memory[Part + 0x1ac] == 0x80000040 && v.memory[Args + 8] == 0);
  }
  for (uint32_t mask : {0u, 0xffffffffu, 0x80000000u}) {
    Fixture v;
    v.memory[Args + 8] = mask;
    assert(battleOnPartAttributeEvent(v.event, v.services) == 2 &&
           v.stores == 1 && v.memory[Part + 0x1ac] == (0x40 | mask));
    unsigned values = 0;
    for (auto q : v.trace)
      values += q.first == 'V';
    assert(values == 2);
  }
  std::cout << "Complete OnPartsAttribute raw-mask/capture and genuine "
               "linkedpart lookup pass\n";
}
