#include "battle_act_draw.hpp"
#include <cassert>
#include <cstring>
#include <map>
#include <string>
using namespace ttyd;
struct Fixture {
  static constexpr uint32_t B = 0x10000, W = 0x20000, C = 0x30000, G = 0x40000;
  std::map<uint32_t, uint32_t> words;
  std::map<uint32_t, uint8_t> bytes;
  std::vector<BattleActDrawSlot> slots;
  std::vector<std::string> calls;
  bool cameraReady = true;
  float centerX = 100;
  unsigned rotations = 0;
  void floating(uint32_t a, float f) {
    uint32_t n;
    std::memcpy(&n, &f, 4);
    words[a] = n;
  }
  Fixture() {
    words[0x8041eb90] = B;
    words[B + 0x1c78] = W;
    words[W] = C;
    words[W + 4] = 0x50000;
    words[C + 8] = 3;
    words[0x80415fd0] = G;
    words[G + 4] = 60;
    const float constants[] = {0,
                               12,
                               3.1415927410125732f,
                               180,
                               270,
                               90,
                               360,
                               .01745329238474369f,
                               20,
                               -60,
                               55,
                               .1f,
                               1};
    for (unsigned i = 0; i < 13; ++i)
      floating(0x8042242c + 4 * i, constants[i]);
    const float angles[] = {240, 120, 0,   0,   0,   270, 180, 90,
                            0,   0,   270, 203, 157, 90,  0};
    for (unsigned i = 0; i < 15; ++i)
      floating(0x803635d8 + 4 * i, angles[i]);
  }
  BattleActDrawServices services() {
    BattleActDrawServices s;
    s.user = this;
    s.memory.memory.user = this;
    s.memory.memory.word = [](void *p, uint32_t a) {
      return static_cast<Fixture *>(p)->words[a];
    };
    s.memory.memory.storeWord = [](void *p, uint32_t a, uint32_t n) {
      static_cast<Fixture *>(p)->words[a] = n;
    };
    s.memory.memory.storeByte = [](void *p, uint32_t a, uint8_t n) {
      static_cast<Fixture *>(p)->bytes[a] = n;
    };
    s.camera = [](void *p, uint32_t n) {
      auto &f = *static_cast<Fixture *>(p);
      assert(n == 8);
      f.calls.push_back("camera");
      return f.cameraReady;
    };
    s.center = [](void *p) -> std::optional<std::array<float, 3>> {
      return std::array<float, 3>{static_cast<Fixture *>(p)->centerX, 50, 99};
    };
    s.offset = [](void *, float a) -> std::optional<std::array<float, 2>> {
      return std::array<float, 2>{a, 2};
    };
    s.translation =
        [](void *,
           const std::array<float, 3> &p) -> std::optional<BattleActMatrix> {
      BattleActMatrix m{};
      m[3] = p[0];
      m[7] = p[1];
      m[11] = p[2];
      return m;
    };
    s.rotation = [](void *p, float,
                    uint32_t axis) -> std::optional<BattleActMatrix> {
      assert(axis == 120);
      ++static_cast<Fixture *>(p)->rotations;
      return BattleActMatrix{};
    };
    s.scale = [](void *, float x, float y,
                 float z) -> std::optional<BattleActMatrix> {
      BattleActMatrix m{};
      m[0] = x;
      m[5] = y;
      m[10] = z;
      return m;
    };
    s.concat = [](void *, const BattleActMatrix &a,
                  const BattleActMatrix &) -> std::optional<BattleActMatrix> {
      return a;
    };
    s.interpolate = [](void *, int32_t mode, int32_t step, int32_t duration,
                       float from, float to) -> std::optional<float> {
      assert(mode == 0 || mode == 4);
      return from + (to - from) * float(step) / duration;
    };
    s.icons = [](void *p, uint32_t w, uint32_t rows,
                 const std::vector<BattleActDrawSlot> &slots) {
      auto &f = *static_cast<Fixture *>(p);
      assert(w == W && rows == 0x50000);
      f.slots = slots;
      f.calls.push_back("icons");
      return true;
    };
    s.partyButton = [](void *p) {
      static_cast<Fixture *>(p)->calls.push_back("party");
      return true;
    };
    s.pinchMark = [](void *p) {
      static_cast<Fixture *>(p)->calls.push_back("pinch");
      return true;
    };
    return s;
  }
};
int main() {
  constexpr auto W = Fixture::W, C = Fixture::C;
  Fixture f;
  // currentBattleAddress reads8041eb70 (verified existing actor contract).
  f.words[0x8041eb90] = Fixture::B;
  f.words[W + 0x48] = 1;
  f.words[W + 0x4c] = 2;
  f.words[W + 0x50] = 3;
  auto s = f.services();
  assert(drawPackedBattleActMenu(s));
  assert(f.words[W + 0x34] == 1 && f.words[W + 0x48] == 0 &&
         f.words[W + 0x4c] == 1);
  assert(!f.slots[0].position && f.bytes[W + 0x14] == 0);
  assert((f.calls ==
          std::vector<std::string>{"camera", "icons", "party", "pinch"}));
  for (unsigned i = 0; i < 60; ++i)
    assert(drawPackedBattleActMenu(s));
  assert(f.words[W + 0xc] == 1 && f.bytes[W + 0x14] == 1 && f.rotations > 0);
  assert(f.slots[0].position && (*f.slots[0].position)[2] == 2 &&
         f.words[W + 0x20] == 3);
  f.words[C] = 1;
  assert(drawPackedBattleActMenu(s));
  assert(f.words[W + 0xc] == 2 && f.bytes[W + 0x14] == 0 &&
         f.words[W + 0x18] == 1);
  assert(f.words[W + 0x70] == 0 && f.words[W + 0x84] == 2 &&
         !(f.words[W + 0x20] & 2));
  for (unsigned i = 0; i < 40; ++i)
    assert(drawPackedBattleActMenu(s));
  assert(f.words[W + 0xc] == 1 && f.bytes[W + 0x14] == 1);
  f.words[C] = 0;
  assert(drawPackedBattleActMenu(s));
  assert(f.words[W + 0x34] == 5);
  Fixture resume;
  resume.words[0x8041eb90] = Fixture::B;
  resume.words[W + 0xc] = 10;
  resume.words[W + 0x34] = 10;
  resume.words[W + 0x38] = 11;
  resume.words[W + 0x3c] = 11;
  auto rs = resume.services();
  assert(drawPackedBattleActMenu(rs));
  assert(resume.words[W + 0xc] == 1 && resume.bytes[W + 0x14] == 1 &&
         resume.words[W + 0x48] == 1);
  assert(resume.slots[0].matrix && resume.slots[0].parameter &&
         *resume.slots[0].parameter == 360);
  for (unsigned i = 0; i < 14; ++i)
    assert(drawPackedBattleActMenu(rs));
  assert(resume.words[W + 0x34] == 3 && resume.words[W + 0x38] == 3);
  Fixture empty;
  empty.words[0x8041eb90] = Fixture::B;
  empty.words[C + 8] = 0;
  assert(drawPackedBattleActMenu(empty.services()));
  assert(empty.slots.empty() && empty.words[W + 0xc] == 1);
  Fixture blocked;
  blocked.words[0x8041eb90] = Fixture::B;
  blocked.cameraReady = false;
  assert(!drawPackedBattleActMenu(blocked.services()));
  assert(blocked.calls.size() == 1 && blocked.words[W + 0x1c] == 0);
  Fixture unsupported;
  unsupported.words[0x8041eb90] = Fixture::B;
  unsupported.words[W + 0x34] = 99;
  assert(drawPackedBattleActMenu(unsupported.services()));
  assert(!unsupported.slots[0].matrix && unsupported.words[W + 0x34] == 99);
  // The original wrapper selects Mario's class cursor and draws descending Y.
  Fixture sorted;
  sorted.words[Fixture::B + 0x420] = 1;
  sorted.words[Fixture::B + 0x24] = 0x60000;
  sorted.words[0x60008] = 222;
  sorted.words[Fixture::B + 0x1bbc] = 3;
  for (unsigned i = 0; i < 3; ++i)
    sorted.words[W + 0x20 + i * 4] = 1;
  sorted.words[W + 0x24] = 3;
  std::vector<BattleActDrawSlot> slots(3);
  for (unsigned i = 0; i < 3; ++i) {
    slots[i].position = std::array<float, 3>{0, float(i == 1 ? 9 : 2), 0};
    slots[i].matrix = BattleActMatrix{};
    slots[i].parameter = 0;
  }
  BattleActIconDrawServices draw;
  draw.memory = sorted.services().memory;
  draw.user = &sorted;
  draw.bubble = [](void *p, uint32_t w, uint32_t row, const BattleActMatrix &,
                   float) {
    assert(w == W && row == 0x50014);
    static_cast<Fixture *>(p)->calls.push_back("bubble1");
    return true;
  };
  draw.circle = [](void *p, uint32_t, uint32_t row, const BattleActMatrix &,
                   const std::array<float, 3> &, float) {
    static_cast<Fixture *>(p)->calls.push_back(row == 0x50000 ? "circle0"
                                                              : "circle2");
    return true;
  };
  assert(drawPackedBattleActIcons(W, 0x50000, slots, draw));
  assert((sorted.calls ==
          std::vector<std::string>{"bubble1", "circle0", "circle2"}));
  sorted.calls.clear();
  sorted.words[W + 0x20] = 0;
  assert(drawPackedBattleActIcons(W, 0x50000, slots, draw));
  assert(sorted.calls.size() == 2);
  slots[1].position.reset();
  assert(!drawPackedBattleActIcons(W, 0x50000, slots, draw));
}
