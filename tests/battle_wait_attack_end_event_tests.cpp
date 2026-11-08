#include "battle_wait_attack_end_event.hpp"
#include "event_manager.hpp"
#include <cassert>
#include <iostream>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t BattleGlobal = 0x8041eb90, Battle = 0x84000000,
                   OtherBattle = 0x84010000, Attacker = 0x85000000,
                   Other = 0x85001000, EventId = 77;
struct Fixture {
  BattleActorServices actors;
  EventCreationServices events;
  EventGlobals globals;
  EventContext field, battle;
  EventEntry fieldEntries[2], battleEntries[2];
  std::map<uint32_t, uint32_t> memory;
  std::vector<std::pair<char, uint32_t>> trace;
  unsigned stores = 0;
  bool mutate = false;
  Fixture() {
    memory[BattleGlobal] = Battle;
    memory[Battle + 0x420] = 1;
    memory[Battle + 0x24] = Attacker;
    memory[Attacker + 0x2a8] = EventId;
    globals.battleContext = 1;
    field.capacity = battle.capacity = 2;
    field.entries = fieldEntries;
    battle.entries = battleEntries;
    fieldEntries[0].id = battleEntries[0].id = EventId;
    fieldEntries[0].flags = 0;
    battleEntries[0].flags = 1;
    actors.memory.user = this;
    actors.memory.word = [](void *p, uint32_t at) {
      auto &f = *static_cast<Fixture *>(p);
      f.trace.push_back({'R', at});
      assert(f.memory.count(at));
      return f.memory.at(at);
    };
    actors.memory.storeWord = [](void *p, uint32_t at, uint32_t value) {
      auto &f = *static_cast<Fixture *>(p);
      f.trace.push_back({'S', at});
      ++f.stores;
      f.memory[at] = value;
    };
    events.manager.user = this;
    events.manager.globals = [](void *p) {
      auto &f = *static_cast<Fixture *>(p);
      f.trace.push_back({'G', 0});
      if (f.mutate) {
        f.memory[BattleGlobal] = OtherBattle;
        f.memory[OtherBattle + 0x420] = 2;
        f.memory[OtherBattle + 0x28] = Other;
        f.memory[Other + 0x2a8] = 99;
      }
      return &f.globals;
    };
    events.manager.context = [](void *p, bool battle) {
      auto &f = *static_cast<Fixture *>(p);
      f.trace.push_back({'C', uint32_t(battle)});
      return battle ? &f.battle : &f.field;
    };
  }
};
} // namespace
int main() {
  Fixture f;
  assert(battleWaitAttackEndEvent(f.actors, f.events) == 0 && f.stores == 0 &&
         f.memory[Attacker + 0x2a8] == EventId);
  const std::vector<std::pair<char, uint32_t>> expected{{'R', BattleGlobal},
                                                        {'R', Battle + 0x420},
                                                        {'R', Battle + 0x24},
                                                        {'R', Attacker + 0x2a8},
                                                        {'G', 0},
                                                        {'C', 1}};
  assert(f.trace == expected);
  f.battleEntries[0].flags = 0;
  f.trace.clear();
  assert(battleWaitAttackEndEvent(f.actors, f.events) == 2 && f.stores == 1 &&
         f.memory[Attacker + 0x2a8] == 0);
  auto after = expected;
  after.push_back({'S', Attacker + 0x2a8});
  assert(f.trace == after);
  {
    Fixture v;
    v.memory[Attacker + 0x2a8] = 0;
    v.events = {};
    assert(battleWaitAttackEndEvent(v.actors, v.events) == 2 && v.stores == 0 &&
           v.trace.size() == 4);
  }
  for (int32_t id : {-1, 64}) {
    Fixture v;
    v.memory[Battle + 0x420] = uint32_t(id);
    v.events = {};
    assert(battleWaitAttackEndEvent(v.actors, v.events) == 2 &&
           v.trace.size() == 2 && v.stores == 0);
  }
  {
    Fixture v;
    v.memory[Battle + 0x24] = 0;
    v.events = {};
    assert(battleWaitAttackEndEvent(v.actors, v.events) == 2 &&
           v.trace.size() == 3);
  }
  {
    Fixture v;
    v.globals.battleContext = 0;
    assert(battleWaitAttackEndEvent(v.actors, v.events) == 2 && v.stores == 1);
  }
  {
    Fixture v;
    v.battleEntries[0].flags = 2;
    assert(battleWaitAttackEndEvent(v.actors, v.events) == 2);
  }
  {
    Fixture v;
    v.mutate = true;
    v.battleEntries[0].flags = 0;
    assert(battleWaitAttackEndEvent(v.actors, v.events) == 2 &&
           v.memory[Attacker + 0x2a8] == 0 && v.memory[Other + 0x2a8] == 99);
    v.mutate = false;
    v.battleEntries[0].id = 99;
    v.battleEntries[0].flags = 1;
    v.trace.clear();
    assert(battleWaitAttackEndEvent(v.actors, v.events) == 0 &&
           v.trace[0] == std::make_pair('R', BattleGlobal) &&
           v.trace[1] == std::make_pair('R', OtherBattle + 0x420));
  }
  std::cout << "Complete WaitAttackEnd fresh attacker, real selected event "
               "context and live/dead barrier pass\n";
}
