#include "battle_unit_coin.hpp"
#include "event_manager.hpp"
#include <cassert>
#include <iostream>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t Args = 0x81000000, Battle = 0x82000000, Unit = 0x83000000,
                   Kind = 0x84000000, OtherKind = 0x85000000;
struct Fixture {
  EventEntry event;
  BattleUnitCoinCursor cursor;
  BattleUnitCoinServices services;
  std::map<uint32_t, uint32_t> words;
  std::map<uint32_t, uint8_t> bytes;
  std::vector<std::pair<uint32_t, uint32_t>> trace;
  std::vector<int32_t> random{0, 50, 99};
  size_t rng = 0;
  int hold = -1, remaining = 0;
  bool alive = true, reenter = false, mutate = false;
  int32_t translated = 1, stored = -777, destination = 0;
  unsigned writes = 0;
  Fixture() {
    event.flags = 1;
    event.id = 3;
    event.arguments = Args;
    event.currentCommand = 0x95544f10;
    event.argumentCount = 2;
    words = {{Args, 123},
             {Args + 4, uint32_t(-30000000)},
             {0x8041eb90, Battle},
             {Battle + 0x24, Unit},
             {Unit + 0x10, Kind}};
    bytes = {{Kind + 0x10, 3},        {Kind + 0x11, 51},
             {Kind + 0x12, 7},        {OtherKind + 0x10, 1},
             {OtherKind + 0x11, 100}, {OtherKind + 0x12, 99}};
    services.user = this;
    services.lifetime = std::make_shared<int>(0);
    services.current = [](void *p) {
      auto &f = *static_cast<Fixture *>(p);
      if (f.reenter)
        assert(!resumeBattleGetCoinEvent(f.event, f.services, f.cursor));
      return f.alive;
    };
    services.word = [](void *p, uint32_t a) -> std::optional<uint32_t> {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.enter(0, a))
        return {};
      auto i = f.words.find(a);
      return i == f.words.end() ? std::optional<uint32_t>{} : i->second;
    };
    services.byte = [](void *p, uint32_t a) -> std::optional<uint8_t> {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.enter(1, a))
        return {};
      auto i = f.bytes.find(a);
      return i == f.bytes.end() ? std::optional<uint8_t>{} : i->second;
    };
    services.value = [](void *p, EventEntry &,
                        int32_t raw) -> std::optional<int32_t> {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.enter(2, uint32_t(raw)))
        return {};
      return raw;
    };
    services.translate = [](void *p, EventEntry &,
                            int32_t raw) -> std::optional<int32_t> {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.enter(3, uint32_t(raw)))
        return {};
      return f.translated;
    };
    services.random = [](void *p, int32_t bound) -> std::optional<int32_t> {
      auto &f = *static_cast<Fixture *>(p);
      assert(bound == 100);
      if (!f.enter(4, uint32_t(bound)))
        return {};
      assert(f.rng < f.random.size());
      if (f.mutate)
        f.words[Unit + 0x10] = OtherKind;
      return f.random[f.rng++];
    };
    services.writeWord = [](void *p, uint32_t at, uint32_t value) {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.enter(6, at))
        return false;
      f.words[at] = value;
      ++f.writes;
      return true;
    };
    services.store = [](void *p, EventEntry &, int32_t dest, int32_t value) {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.enter(5, uint32_t(dest)))
        return false;
      f.stored = value;
      f.destination = dest;
      ++f.writes;
      return true;
    };
  }
  bool enter(uint32_t k, uint32_t a) {
    if (int(trace.size()) == hold && remaining) {
      --remaining;
      return false;
    }
    trace.emplace_back(k, a);
    return true;
  }
  std::optional<int32_t> resume() {
    return resumeBattleGetCoinEvent(event, services, cursor);
  }
};
} // namespace
int main() {
  Fixture baseline;
  assert(baseline.resume() == 2 && baseline.stored == 9 && baseline.rng == 3 &&
         baseline.writes == 1 && baseline.destination == -30000000);
  const std::vector<std::pair<uint32_t, uint32_t>> prefix = {
      {0, Args},        {2, 123},           {0, Args + 4},    {3, 123},
      {0, 0x8041eb90},  {0, Battle + 0x24}, {0, Unit + 0x10}, {1, Kind + 0x12},
      {0, Unit + 0x10}, {1, Kind + 0x10},   {4, 100},         {0, Unit + 0x10},
      {1, Kind + 0x11}};
  assert(std::equal(prefix.begin(), prefix.end(), baseline.trace.begin()));
  assert(baseline.resume() == 2 && baseline.writes == 1 && baseline.rng == 3);
  for (unsigned at = 0; at < baseline.trace.size(); ++at) {
    Fixture f;
    f.hold = int(at);
    f.remaining = 2;
    assert(!f.resume() && !f.resume());
    assert(f.resume() == 2 && f.trace == baseline.trace && f.rng == 3 &&
           f.writes == 1 && f.stored == 9);
  }
  for (int32_t id : {-100, -1, 64, 100}) {
    Fixture f;
    f.translated = id;
    assert(f.resume() == 2 && f.stored == 0 && !f.rng && f.trace.size() == 6);
  }
  {
    Fixture f;
    f.words[Battle + 0x24] = 0;
    assert(f.resume() == 2 && f.stored == 0 && !f.rng);
  }
  {
    Fixture f;
    f.bytes[Kind + 0x10] = 0;
    assert(f.resume() == 2 && f.stored == 7 && !f.rng);
  }
  {
    Fixture f;
    f.mutate = true;
    assert(f.resume() == 2 && f.stored == 8 && f.rng == 1);
  }
  {
    Fixture f;
    f.random = {-1, 51, INT32_MAX};
    assert(f.resume() == 2 && f.stored == 8);
  }
  {
    Fixture f;
    f.reenter = true;
    assert(f.resume() == 2);
  }
  {
    Fixture f;
    f.hold = 11;
    f.remaining = 1;
    assert(!f.resume() && f.rng == 1);
    auto changed = f.services;
    changed.user = nullptr;
    assert(!resumeBattleGetCoinEvent(f.event, changed, f.cursor));
    f.alive = false;
    assert(!f.resume());
    f.alive = true;
    assert(f.resume() == 2 && f.rng == 3);
  }
  {
    Fixture f;
    f.hold = 2;
    f.remaining = 1;
    assert(!f.resume());
    f.words[Args] = 999;
    assert(f.resume() == 2);
    assert(f.trace[3].first == 3 && f.trace[3].second == 123);
  }
  {
    Fixture f;
    f.bytes[Kind + 0x10] = 255;
    f.bytes[Kind + 0x11] = 255;
    f.bytes[Kind + 0x12] = 255;
    f.random.assign(255, 99);
    assert(f.resume() == 2 && f.stored == 510 && f.rng == 255);
  }

  {
    constexpr uint32_t Info = 0x86000000;
    Fixture baseline;
    baseline.words[Battle + 0x2738] = Info;
    baseline.words[Info + 0x14] = UINT32_MAX;
    assert(resumeBattleStoreCoinEvent(baseline.event, baseline.services,
                                      baseline.cursor) == 2);
    assert(baseline.words[Info + 0x14] == 122 && baseline.writes == 1 &&
           !baseline.rng);
    assert(resumeBattleStoreCoinEvent(baseline.event, baseline.services,
                                      baseline.cursor) == 2 &&
           baseline.writes == 1);
    assert(!resumeBattleGetCoinEvent(baseline.event, baseline.services,
                                     baseline.cursor));
    for (unsigned at = 0; at < baseline.trace.size(); ++at) {
      Fixture f;
      f.words[Battle + 0x2738] = Info;
      f.words[Info + 0x14] = UINT32_MAX;
      f.hold = int(at);
      f.remaining = 2;
      assert(!resumeBattleStoreCoinEvent(f.event, f.services, f.cursor));
      assert(!resumeBattleStoreCoinEvent(f.event, f.services, f.cursor));
      assert(resumeBattleStoreCoinEvent(f.event, f.services, f.cursor) == 2 &&
             f.trace == baseline.trace && f.words[Info + 0x14] == 122 &&
             f.writes == 1);
    }
    Fixture f;
    f.words[Battle + 0x2738] = Info;
    f.words[Info + 0x14] = 10;
    f.words[Args] = uint32_t(-1);
    assert(resumeBattleStoreCoinEvent(f.event, f.services, f.cursor) == 2 &&
           f.words[Info + 0x14] == 9);
    Fixture missing;
    missing.words[Battle + 0x2738] = Info;
    assert(!resumeBattleStoreCoinEvent(missing.event, missing.services,
                                       missing.cursor) &&
           !missing.writes);
  }
  std::cout << "Whole GetCoin event/unit getter source order, every held "
               "read/RNG/store, mutation and zero paths pass\n";
}
