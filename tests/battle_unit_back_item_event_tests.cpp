#include "battle_unit_back_item_event.hpp"
#include "event_manager.hpp"
#include <cassert>
#include <iostream>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t Arguments = 0x85000000, BattleGlobal = 0x8041eb90,
                   Battle = 0x84000000, Unit = 0x86000000, Info = 0x87000000,
                   Setup = 0x88000000, Flag = 0x40000000, Item = 79;
struct Fixture {
  EventEntry event;
  BattleUnitEventServices services;
  std::map<uint32_t, uint32_t> memory;
  std::vector<std::pair<char, uint32_t>> trace;
  unsigned writes = 0;
  bool mutateGlobal = false, mutateItem = false, mutateSetup = false;
  Fixture() {
    event.arguments = Arguments;
    event.argumentCount = 1;
    memory[Arguments] = 2;
    memory[BattleGlobal] = Battle;
    memory[Unit + 0x1c] = Flag;
    memory[Unit + 0x308] = Item;
    memory[Battle + 0x2738] = Info;
    memory[Info + 0xc] = Setup;
    for (unsigned i = 0; i < 9; ++i)
      memory[Setup + 0x7c + 4 * i] = 0;
    services.memory.user = this;
    services.user = this;
    services.values.user = this;
    services.memory.word = [](void *p, uint32_t at) {
      auto &f = *static_cast<Fixture *>(p);
      f.trace.push_back({'R', at});
      assert(f.memory.count(at));
      auto v = f.memory.at(at);
      if (f.mutateItem && at == Battle + 0x2738)
        f.memory[Unit + 0x308] = 188;
      return v;
    };
    services.memory.storeWord = [](void *p, uint32_t at, uint32_t value) {
      auto &f = *static_cast<Fixture *>(p);
      f.trace.push_back({'W', at});
      ++f.writes;
      f.memory[at] = value;
      if (f.mutateSetup && f.writes == 1)
        f.memory[Info + 0xc] = 0x89000000;
    };
    services.values.resolve = [](void *p, EventEntry &, int32_t raw) {
      auto &f = *static_cast<Fixture *>(p);
      f.trace.push_back({'V', uint32_t(raw)});
      if (f.mutateGlobal)
        f.memory[BattleGlobal] = 0x8a000000;
      return raw;
    };
    services.translate = [](void *p, EventEntry &, int32_t actor) {
      auto &f = *static_cast<Fixture *>(p);
      f.trace.push_back({'T', uint32_t(actor)});
      return actor;
    };
    services.unit = [](void *p, uint32_t battle, int32_t actor) {
      auto &f = *static_cast<Fixture *>(p);
      f.trace.push_back({'U', battle});
      assert(battle == Battle && actor == 2);
      return Unit;
    };
  }
};
} // namespace
int main() {
  for (unsigned occupied = 0; occupied <= 8; ++occupied) {
    Fixture f;
    for (unsigned i = 0; i < occupied; ++i)
      f.memory[Setup + 0x7c + 4 * i] = 101 + i;
    assert(battleGetBackItemEvent(f.event, f.services) == 2);
    assert(f.writes == 2 && f.memory[Unit + 0x308] == 0 &&
           f.memory[Setup + 0x7c + 4 * occupied] == Item);
    std::vector<std::pair<char, uint32_t>> expected{{'R', BattleGlobal},
                                                    {'R', Arguments},
                                                    {'V', 2},
                                                    {'T', 2},
                                                    {'U', Battle},
                                                    {'R', Unit + 0x1c},
                                                    {'R', Unit + 0x308},
                                                    {'R', Battle + 0x2738},
                                                    {'R', Info + 0xc}};
    for (unsigned i = 0; i < (occupied < 8 ? occupied + 1 : 8); ++i)
      expected.push_back({'R', Setup + 0x7c + 4 * i});
    expected.push_back({'W', Setup + 0x7c + 4 * occupied});
    expected.push_back({'W', Unit + 0x308});
    assert(f.trace == expected);
  }
  {
    Fixture f;
    f.memory[Unit + 0x1c] = 0;
    f.memory.erase(Unit + 0x308);
    f.memory.erase(Battle + 0x2738);
    assert(battleGetBackItemEvent(f.event, f.services) == 2 && f.writes == 0 &&
           f.trace.size() == 6);
  }
  {
    Fixture f;
    f.memory[Unit + 0x308] = 0;
    f.memory.erase(Battle + 0x2738);
    assert(battleGetBackItemEvent(f.event, f.services) == 2 && f.writes == 0 &&
           f.trace.size() == 7);
  }
  {
    Fixture f;
    f.mutateGlobal = f.mutateItem = f.mutateSetup = true;
    assert(battleGetBackItemEvent(f.event, f.services) == 2 && f.writes == 2);
    assert(f.memory[Setup + 0x7c] == Item && f.memory[Unit + 0x308] == 0 &&
           f.memory[BattleGlobal] == 0x8a000000 &&
           f.memory[Info + 0xc] == 0x89000000);
  }
  std::cout << "Complete GetBackItem closed paths, eight scans/one-past store "
               "and captured owners pass\n";
}
