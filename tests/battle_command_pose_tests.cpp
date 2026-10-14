#include "battle_command_pose.hpp"
#include <cassert>
#include <map>
#include <vector>
using namespace ttyd;
struct Fixture {
  static constexpr uint32_t U = 0x1000, P = 0x4000, D = 0x6000, T = 0x7000;
  std::map<uint32_t, uint32_t> w{{U + 0x14, P},   {P + 4, D},
                                 {D, 1},          {P + 0x1ac, 1},
                                 {P + 0x1bc, T},  {T, 67},
                                 {T + 4, 0x9000}, {0x80416ab0, 27},
                                 {0x80416ab4, 2}, {0x80416ab8, 0xffffffff}};
  std::vector<int32_t> calls;
  int32_t active = -1;
  bool copyReady = true;
  unsigned copies = 0;
  BattleCommandPoseServices services() {
    BattleCommandPoseServices s;
    s.user = this;
    s.memory.memory.user = this;
    s.memory.memory.word = [](void *p, uint32_t a) {
      return static_cast<Fixture *>(p)->w[a];
    };
    s.memory.memory.storeWord = [](void *p, uint32_t a, uint32_t v) {
      static_cast<Fixture *>(p)->w[a] = v;
    };
    s.status = [](void *p, uint32_t u, int32_t id) -> std::optional<bool> {
      auto &f = *static_cast<Fixture *>(p);
      assert(u == U);
      f.calls.push_back(id);
      return f.active == id;
    };
    s.copyName = [](void *p, uint32_t part, uint32_t name) {
      auto &f = *static_cast<Fixture *>(p);
      assert(part == P && name == 0x9000);
      ++f.copies;
      f.w[P + 0x204] = 0x40000000;
      return f.copyReady;
    };
    return s;
  }
};
int main() {
  Fixture f;
  assert(packedBattleBodyPartId(0, f.services().memory.memory) == -1);
  assert(packedBattleCommandPose(f.U, f.services()) && f.copies == 1 &&
         f.w[f.P + 0x204] == 0x40000083 &&
         f.calls == std::vector<int32_t>({27, 2}));
  Fixture sick;
  sick.active = 2;
  sick.w[sick.P + 0x204] = 0x400002ff;
  assert(packedBattleCommandPose(sick.U, sick.services()) && !sick.copies &&
         sick.w[sick.P + 0x204] == 0x40000269);
  for (auto flag : {0x10000000u, 0x20000000u}) {
    Fixture alternate;
    alternate.w[alternate.U + 0x138] = flag;
    alternate.w[alternate.P + 0x204] = 0xff;
    assert(packedBattleCommandPose(alternate.U, alternate.services()) &&
           alternate.calls.empty() && !alternate.copies &&
           alternate.w[alternate.P + 0x204] == 0x69);
  }
  Fixture forced;
  forced.w[forced.U + 0x104] = 0x20000;
  assert(packedBattleStatusPoseType(forced.U, forced.services()) == 27 &&
         forced.calls.empty());
  for (auto type : {29, 30, 31, 39, 40, 67}) {
    Fixture a;
    a.w[a.T] = uint32_t(type);
    assert(packedBattleSetAnimationType(a.P, type, a.services()));
    assert(a.w[a.P + 0x204] ==
           (0x40000003u | (type >= 29 && type < 40 ? 8u : 0u) |
            (type >= 29 && type < 31 ? 16u : 0u)));
  }
  Fixture noStatus;
  auto s = noStatus.services();
  s.status = nullptr;
  assert(!packedBattleCommandPose(noStatus.U, s) && !noStatus.copies);
  Fixture missing;
  missing.copyReady = false;
  assert(!packedBattleCommandPose(missing.U, missing.services()));
  Fixture body;
  body.w[body.P + 0x1ac] = 0;
  body.w[body.P] = body.P + 0x500;
  body.w[body.P + 0x500 + 0x1ac] = 1;
  body.w[body.P + 0x500 + 4] = body.D + 0x100;
  body.w[body.D + 0x100] = 9;
  assert(packedBattleBodyPartId(body.U, body.services().memory.memory) == 9);
}
