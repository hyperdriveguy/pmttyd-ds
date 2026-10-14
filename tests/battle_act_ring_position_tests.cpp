#include "battle_act_ring_position.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <map>
using namespace ttyd;
int main() {
  std::map<uint32_t, uint32_t> memory{
      {0x8041eb90, 0x10000}, {0x10420, 2}, {0x10028, 0x20000}};
  auto real = [&](int32_t offset, float f) {
    uint32_t n;
    std::memcpy(&n, &f, 4);
    memory[0x80427440u + uint32_t(offset)] = n;
  };
  real(-20220, 22.5);
  real(-20216, 105);
  real(-20500, 0);
  real(-20212, -180);
  real(-20224, 4.71238899230957f);
  real(-20240, 80);
  real(-20284, 35);
  struct Context {
    std::map<uint32_t, uint32_t> *memory;
    std::array<float, 3> position;
    bool sinReady = true;
    unsigned cosCalls = 0, sinCalls = 0;
  };
  Context c{&memory, {-300, 100, 5}};
  BattleActRingPositionServices s;
  s.user = &c;
  s.memory.memory.user = &memory;
  s.memory.memory.word = [](void *p, uint32_t a) {
    return (*static_cast<std::map<uint32_t, uint32_t> *>(p))[a];
  };
  s.project = [](void *p,
                 uint32_t address) -> std::optional<std::array<float, 3>> {
    assert(address == 0x2003c);
    auto &c = *static_cast<Context *>(p);
    (*c.memory)[0x8041eb90] = 0x90000;
    return c.position;
  };
  s.cosine = [](void *p, double a) -> std::optional<double> {
    ++static_cast<Context *>(p)->cosCalls;
    return std::cos(a);
  };
  s.sine = [](void *p, double a) -> std::optional<double> {
    auto &c = *static_cast<Context *>(p);
    ++c.sinCalls;
    return c.sinReady ? std::optional<double>(std::sin(a)) : std::nullopt;
  };
  std::array<float, 3> center{};
  assert(packedBattleActRingCenter(center, s));
  assert((center == std::array<float, 3>{-180, 205, 0}));
  memory[0x8041eb90] = 0x10000;
  c.position = {10, 30, 50};
  assert(packedBattleActRingCenter(center, s));
  assert((center == std::array<float, 3>{32.5, 135, 0}));
  std::array<float, 2> offset{9, 10};
  assert(packedBattleActRingOffset(4.71238899230957f, offset, s));
  assert(offset[0] == 80 && offset[1] == 0);
  c.sinReady = false;
  offset = {9, 10};
  assert(!packedBattleActRingOffset(4.71238899230957f, offset, s));
  assert(offset[0] == 80 && offset[1] == 10 && c.cosCalls == 2 &&
         c.sinCalls == 2);
  s.cosine = nullptr;
  offset = {9, 10};
  assert(!packedBattleActRingOffset(0, offset, s));
  assert((offset == std::array<float, 2>{9, 10}));
  s.project = nullptr;
  center = {1, 2, 3};
  assert(!packedBattleActRingCenter(center, s));
  assert((center == std::array<float, 3>{1, 2, 3}));
}
