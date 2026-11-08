#include "battle_unit_exp.hpp"
#include "event_manager.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstring>
#include <iostream>
#include <map>
#include <tuple>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t Args = 0x81000000, Battle = 0x82000000, Unit = 0x83000000,
                   Kind = 0x84000000, Pouch = 0x85000000, Info = 0x86000000,
                   Setup = 0x87000000, Group = 0x88000000;
uint32_t bits(float f) {
  uint32_t w;
  std::memcpy(&w, &f, 4);
  return w;
}
float roundFloat(float v) {
  volatile float f = v;
  return f;
}
struct Fixture {
  EventEntry event;
  BattleUnitExpCursor cursor;
  BattleUnitExpServices services;
  std::map<uint32_t, uint32_t> words;
  std::vector<std::tuple<unsigned, uint64_t, uint64_t>> trace;
  int hold = -1, remaining = 0;
  bool alive = true, reenter = false, mutate = false;
  int32_t translated = 1, output = -777;
  unsigned writes = 0;
  Fixture() {
    event.flags = 1;
    event.id = 3;
    event.currentCommand = 0x95544f2c;
    event.arguments = Args;
    event.argumentCount = 2;
    words = {{Args, 123},
             {Args + 4, uint32_t(-30000000)},
             {0x8041eb90, Battle},
             {0x8041eb00, Pouch},
             {Battle + 0x24, Unit},
             {Battle + 0x2738, Info},
             {Info + 0xc, Setup},
             {Setup + 0x18, Group},
             {Group, 4},
             {Pouch + 0x8a, 1},
             {Unit + 0x0d, 7},
             {Unit + 0x10, Kind},
             {Kind + 0xf, 17},
             {0x804226a8, bits(1.f)},
             {0x804226b4, bits(1.1f)},
             {0x804226b8, bits(1.3f)},
             {0x804226bc, bits(1.5f)},
             {0x804226c0, bits(.5f)},
             {Battle + 0xf00, UINT32_MAX}};
    services.user = this;
    services.lifetime = std::make_shared<int>(0);
    services.current = [](void *p) {
      auto &f = *static_cast<Fixture *>(p);
      if (f.reenter)
        assert(!resumeBattleGetExpEvent(f.event, f.services, f.cursor));
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
      auto i = f.words.find(a);
      return i == f.words.end() ? std::optional<uint8_t>{} : uint8_t(i->second);
    };
    services.half = [](void *p, uint32_t a) -> std::optional<uint16_t> {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.enter(2, a))
        return {};
      auto i = f.words.find(a);
      return i == f.words.end() ? std::optional<uint16_t>{}
                                : uint16_t(i->second);
    };
    services.doubleBits = [](void *p, uint32_t a) -> std::optional<uint64_t> {
      auto &f = *static_cast<Fixture *>(p);
      assert(a == 0x802f00b8);
      if (!f.enter(3, a))
        return {};
      return UINT64_C(0x4330000080000000);
    };
    services.value = [](void *p, EventEntry &,
                        int32_t raw) -> std::optional<int32_t> {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.enter(4, uint32_t(raw)))
        return {};
      return raw;
    };
    services.translate = [](void *p, EventEntry &,
                            int32_t raw) -> std::optional<int32_t> {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.enter(5, uint32_t(raw)))
        return {};
      return f.translated;
    };
    services.subtractDoubleFloat = [](void *p, double a,
                                      double b) -> std::optional<float> {
      auto &f = *static_cast<Fixture *>(p);
      uint64_t aa, bb;
      std::memcpy(&aa, &a, 8);
      std::memcpy(&bb, &b, 8);
      if (!f.enter(6, aa, bb))
        return {};
      if (f.mutate)
        f.words[Kind + 0xf] = 99;
      return float(a - b);
    };
    services.multiply = [](void *p, float a, float b) -> std::optional<float> {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.enter(7, bits(a), bits(b)))
        return {};
      return roundFloat(a * b);
    };
    services.truncate = [](void *p, float a) -> std::optional<int32_t> {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.enter(8, bits(a)))
        return {};
      return int32_t(std::trunc(a));
    };
    services.store = [](void *p, EventEntry &, int32_t dest, int32_t value) {
      auto &f = *static_cast<Fixture *>(p);
      assert(dest == -30000000);
      if (!f.enter(9, uint32_t(dest), uint32_t(value)))
        return false;
      f.output = value;
      ++f.writes;
      return true;
    };
    services.writeWord = [](void *p, uint32_t at, uint32_t value) {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.enter(10, at, value))
        return false;
      f.words[at] = value;
      ++f.writes;
      return true;
    };
  }
  bool enter(unsigned k, uint64_t a, uint64_t b = 0) {
    if (int(trace.size()) == hold && remaining) {
      --remaining;
      return false;
    }
    trace.emplace_back(k, a, b);
    return true;
  }
  std::optional<int32_t> resume() {
    return resumeBattleGetExpEvent(event, services, cursor);
  }
};
} // namespace
int main() {
  Fixture baseline;
  assert(baseline.resume() == 2 && baseline.output == 20 &&
         baseline.writes == 1);
  assert(baseline.resume() == 2 && baseline.writes == 1);
  // Actual source prefetch order: secondB before Pouch, signedhalf before unit
  // byte, information pointer even on higher-player-level early0.
  const std::vector<std::tuple<unsigned, uint64_t, uint64_t>> prefix = {
      {0, Args, 0},           {4, 123, 0},
      {0, Args + 4, 0},       {5, 123, 0},
      {0, 0x8041eb90, 0},     {0, Battle + 0x24, 0},
      {0, 0x8041eb90, 0},     {0, 0x8041eb00, 0},
      {2, Pouch + 0x8a, 0},   {1, Unit + 0xd, 0},
      {0, Battle + 0x2738, 0}};
  assert(std::equal(prefix.begin(), prefix.end(), baseline.trace.begin()));
  for (unsigned at = 0; at < baseline.trace.size(); ++at) {
    Fixture f;
    f.hold = int(at);
    f.remaining = 2;
    assert(!f.resume() && !f.resume());
    assert(f.resume() == 2 && f.trace == baseline.trace && f.output == 20 &&
           f.writes == 1);
  }
  for (int32_t type :
       std::array<int32_t, 9>{INT32_MIN, -1, 0, 1, 2, 3, 4, 5, INT32_MAX})
    for (int16_t player : {int16_t(-32768), int16_t(-1), int16_t(0), int16_t(1),
                           int16_t(4), int16_t(255), int16_t(32767)})
      for (uint8_t enemy : {uint8_t(0), uint8_t(1), uint8_t(4), uint8_t(255)})
        for (uint8_t base : {uint8_t(0), uint8_t(17), uint8_t(255)}) {
          Fixture f;
          f.words[Group] = uint32_t(type);
          f.words[Pouch + 0x8a] = uint16_t(player);
          f.words[Unit + 0xd] = enemy;
          f.words[Kind + 0xf] = base;
          const float factor = type >= 0 && type <= 2 ? 1.f
                               : type == 3            ? 1.1f
                               : type == 4            ? 1.3f
                                                      : 1.5f;
          const int32_t expected =
              player > enemy
                  ? 0
                  : int32_t(base) +
                        int32_t(std::trunc(roundFloat(
                            roundFloat(factor *
                                       float(int32_t(enemy) - player)) *
                            .5f)));
          assert(f.resume() == 2 && f.output == expected);
          if (player > enemy) {
            assert(f.trace.size() == prefix.size() + 1);
            for (const auto &q : f.trace)
              assert(std::get<0>(q) != 6 && std::get<0>(q) != 7 &&
                     std::get<0>(q) != 8);
          }
        }
  {
    Fixture f;
    f.words[Battle + 0x24] = 0;
    assert(!f.resume() && !f.writes);
    assert(std::get<1>(f.trace.back()) == 0xd);
  }
  {
    Fixture f;
    f.words[Pouch + 0x8a] = 10;
    f.words.erase(Battle + 0x2738);
    assert(!f.resume() && !f.writes);
  }
  {
    Fixture f;
    f.mutate = true;
    assert(f.resume() == 2 && f.output == 102);
  }
  {
    Fixture f;
    f.reenter = true;
    assert(f.resume() == 2);
  }
  {
    Fixture f;
    f.hold = 12;
    f.remaining = 1;
    assert(!f.resume());
    auto wrong = f.services;
    wrong.user = nullptr;
    assert(!resumeBattleGetExpEvent(f.event, wrong, f.cursor));
    f.alive = false;
    assert(!f.resume());
    f.alive = true;
    assert(f.resume() == 2);
  }
  {
    Fixture baseline;
    assert(resumeBattleStoreExpEvent(baseline.event, baseline.services,
                                     baseline.cursor) == 2);
    assert(baseline.words[Battle + 0xf00] == 122 && baseline.writes == 1);
    assert(!baseline.resume());
    for (unsigned at = 0; at < baseline.trace.size(); ++at) {
      Fixture f;
      f.hold = int(at);
      f.remaining = 2;
      assert(!resumeBattleStoreExpEvent(f.event, f.services, f.cursor) &&
             !resumeBattleStoreExpEvent(f.event, f.services, f.cursor));
      assert(resumeBattleStoreExpEvent(f.event, f.services, f.cursor) == 2 &&
             f.trace == baseline.trace && f.words[Battle + 0xf00] == 122 &&
             f.writes == 1);
    }
  }
  std::cout << "Whole GetExp/StoreExp source order, signed/type branchmatrix, "
               "everyread/math/store hold and zero-vs-unavailable pass\n";
}
