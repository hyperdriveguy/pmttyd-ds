#include "battle_facing.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <map>
#include <vector>
using namespace ttyd;
struct Fixture {
  static constexpr uint32_t B = 0x1000, U = 0x20000, V = 0x21000, P = 0x30000,
                            Q = 0x31000, A = 0x50000;
  std::map<uint32_t, uint32_t> words{
      {0x8041eb90, B}, {U + 0x14, P}, {V + 0x14, Q}};
  std::map<uint32_t, uint8_t> bytes{{U + 0x189, 255}, {U + 0x188, 2},
                                    {P + 0xbd, 254},  {P + 0xbc, 3},
                                    {U + 0xc, 0},     {B + 0xa, 4}};
  std::vector<int32_t> translated, resolved;
  std::vector<uint32_t> byteReads;
  int32_t result = 0, output = 0;
  EventEntry event;
  Fixture() {
    event.arguments = A;
    real(U + 0x3c, 10);
    real(V + 0x3c, 20);
  }
  void real(uint32_t at, float value) {
    uint32_t bits;
    std::memcpy(&bits, &value, 4);
    words[at] = bits;
  }
  BattleFacingServices services() {
    BattleFacingServices s;
    s.user = this;
    s.signedWordFloat = [](void *, int32_t v) { return float(v); };
    s.subtract = [](void *, float a, float b) { return a - b; };
    auto &p = s.positions;
    p.numericUser = this;
    p.add = [](void *, float a, float b) { return a + b; };
    p.memory.user = this;
    p.memory.byte = [](void *self, uint32_t at) {
      auto &f = *static_cast<Fixture *>(self);
      f.byteReads.push_back(at);
      return f.bytes[at];
    };
    auto &u = p.unit;
    u.user = this;
    u.memory.user = this;
    u.memory.word = [](void *self, uint32_t at) {
      return static_cast<Fixture *>(self)->words[at];
    };
    u.memory.storeByte = [](void *self, uint32_t at, uint8_t v) {
      static_cast<Fixture *>(self)->bytes[at] = v;
    };
    u.values.user = this;
    u.values.resolve = [](void *self, EventEntry &, int32_t raw) {
      static_cast<Fixture *>(self)->resolved.push_back(raw);
      return raw;
    };
    u.values.store = [](void *self, EventEntry &, int32_t token,
                        int32_t value) {
      auto &f = *static_cast<Fixture *>(self);
      f.output = token;
      f.result = value;
    };
    u.translate = [](void *self, EventEntry &, int32_t id) {
      static_cast<Fixture *>(self)->translated.push_back(id);
      return id == -2 ? 1 : id;
    };
    u.unit = [](void *, uint32_t b, int32_t id) {
      assert(b == B && (id == 1 || id == 2));
      return id == 1 ? U : V;
    };
    return s;
  }
  int32_t run(uint32_t mode, int32_t x = 2, int32_t part = -1,
              int32_t otherPart = -1) {
    translated.clear();
    byteReads.clear();
    resolved.clear();
    const int32_t args[]{-2, part, x, otherPart, int32_t(mode), 0x123456};
    for (unsigned i = 0; i < 6; ++i)
      words[A + 4 * i] = uint32_t(args[i]);
    assert(battleCalculateFaceDirectionEvent(event, services()) == 2 &&
           output == 0x123456);
    return result;
  }
};
int main() {
  Fixture f;
  assert(f.run(0) == -1 && f.translated == std::vector<int32_t>({-2, 2}) &&
         f.resolved.size() == 4);
  assert(f.run(0, 2, 1) == -2);
  assert(f.run(4) == 4);
  assert(f.run(12, 2, 1) == 3);
  assert(f.run(8) == 2);
  assert(f.run(8, 2, 1) == 3 &&
         f.byteReads ==
             std::vector<uint32_t>({Fixture::U + 0x189, Fixture::P + 0xbd,
                                    Fixture::U + 0x188, Fixture::P + 0xbc}));
  assert(f.run(0x100) == 1);
  assert(f.run(1, 11) == 1 && f.translated == std::vector<int32_t>{-2});
  assert(f.run(1, 9) == -1);
  assert(f.run(1, 10) == -1);
  assert(f.run(16) == 1);
  f.real(Fixture::V + 0x3c, 0);
  assert(f.run(16) == -1);
  f.real(Fixture::V + 0x3c, 10);
  assert(f.run(16) == -1);
  f.real(Fixture::V + 0x3c, 20);
  assert(f.run(17, 2) == -1);
  assert(f.run(16 | 0x100) == -1);
  f.real(Fixture::P + 0x24, 30);
  assert(f.run(16, 2, 1) == -1);
  f.words[Fixture::P + 0x1ac] = 0x10000000;
  f.real(Fixture::P + 0x18, 0);
  assert(f.run(16, 2, 1) == 1);
  f.words[Fixture::Q + 0x1ac] = 0x10000000;
  f.real(Fixture::Q + 0x18, -10);
  assert(f.run(16, 2, 1, 1) == -1);
  f.real(Fixture::Q + 0x18, std::nanf(""));
  assert(f.run(16, 2, 1, 1) == -2);
  f.words[Fixture::P + 0x1ac] = 0;
  f.real(Fixture::P + 0x24, 0);
  f.real(Fixture::U + 0x3c, 16777216);
  assert(f.run(1, 16777217, 1) == -2);
  f.words[Fixture::A] = uint32_t(-2);
  f.words[Fixture::A + 4] = 257;
  auto s = f.services();
  assert(battleChangeFaceDirectionEvent(f.event, s.positions.unit) == 2 &&
         f.bytes[Fixture::U + 0x189] == 1 && f.bytes[Fixture::U + 0x188] == 2);
}
