#include "battle_audience_item_on.hpp"
#include "battle_class_footer.hpp"
#include <cassert>
#include <cstring>
#include <map>
using namespace ttyd;
using F = BattleClassRasterService;
struct Fixture {
  static constexpr uint32_t B = 0x10000, U = 0x30000, P = 0x40000, W = 0x50000,
                            G = 0x60000;
  std::map<uint32_t, uint32_t> words;
  std::map<uint32_t, uint16_t> halves;
  std::vector<BattleClassRasterRequest> calls;
  std::vector<int> projections;
  std::vector<float> buttonArgs;
  unsigned sounds = 0;
  int32_t item = -1;
  bool position = true;
  void floating(uint32_t a, float f) {
    uint32_t n;
    std::memcpy(&n, &f, 4);
    words[a] = n;
  }
  float real(uint32_t a) {
    const auto n = words[a];
    float f;
    std::memcpy(&f, &n, 4);
    return f;
  }
  Fixture() {
    words[0x8041eb90] = B;
    words[B + 0x24] = U;
    words[U + 8] = 222;
    words[B + 0x1c78] = W;
    halves[U + 0x10c] = 7;
    halves[U + 0x108] = 10;
    words[0x80415fd0] = G;
    words[G + 0x16c] = 1;
    words[B + 0x163f8] = 0x70000;
    words[0x700a0] = 0x71000;
    words[0x71000] = 0x72000;
    for (auto [off, value] :
         std::map<int32_t, float>{{-20500, 0},
                                  {-20480, 90},
                                  {-20368, 5},
                                  {-20468, 20},
                                  {-20364, .5},
                                  {-20388, -120},
                                  {-20384, 60},
                                  {-20436, 10},
                                  {-20380, 1.5},
                                  {-20376, -28},
                                  {-20496, 12},
                                  {-20396, 28},
                                  {-20372, -12},
                                  {-20472, .01745329238474369f}})
      floating(0x80427440u + uint32_t(off), value);
    halves[0x80416d26] = 7;
    halves[0x80416d32] = 12;
    words[0x80422424] = 0xffffffff;
    floating(0x804249cc, 10);
  }
  void party() {
    words[B + 0x28] = P;
    words[P + 8] = 224;
    halves[P + 0x10c] = 6;
    halves[P + 0x108] = 10;
  }
  BattleClassFooterServices services() {
    BattleClassFooterServices s;
    s.user = this;
    s.raster.user = this;
    s.raster.memory.memory.user = this;
    s.raster.memory.user = this;
    s.raster.memory.memory.word = [](void *p, uint32_t a) {
      return static_cast<Fixture *>(p)->words[a];
    };
    s.raster.memory.memory.half = [](void *p, uint32_t a) {
      return static_cast<Fixture *>(p)->halves[a];
    };
    s.raster.memory.memory.storeWord = [](void *p, uint32_t a, uint32_t n) {
      static_cast<Fixture *>(p)->words[a] = n;
    };
    s.raster.memory.byte = [](void *, uint32_t) { return uint8_t(0); };
    s.raster.request =
        [](void *p,
           const BattleClassRasterRequest &r) -> std::optional<uint32_t> {
      auto &f = *static_cast<Fixture *>(p);
      f.calls.push_back(r);
      return r.function == F::Camera ? 0x80000u : 0u;
    };
    s.project = [](void *p, uint32_t a) -> std::optional<std::array<float, 3>> {
      auto &f = *static_cast<Fixture *>(p);
      f.projections.push_back(a);
      return std::array<float, 3>{a == U + 0x3c ? 100.f : 200.f, 50, 0};
    };
    s.audienceItem = [](void *p, uint32_t extra) -> std::optional<int32_t> {
      assert(!extra);
      return static_cast<Fixture *>(p)->item;
    };
    s.changePosition = [](void *p, uint32_t b) -> std::optional<bool> {
      assert(b == B);
      return static_cast<Fixture *>(p)->position;
    };
    s.sound = [](void *p, uint32_t key) {
      assert(key == 0x802ef7a4);
      ++static_cast<Fixture *>(p)->sounds;
      return true;
    };
    s.button = [](void *p, int32_t type, float x, float y, float angle) {
      static_cast<Fixture *>(p)->buttonArgs = {float(type), x, y, angle};
      return true;
    };
    s.raster.math.translation =
        [](void *,
           const std::array<float, 3> &p) -> std::optional<BattleActMatrix> {
      BattleActMatrix m{};
      m[0] = m[5] = m[10] = 1;
      m[3] = p[0];
      m[7] = p[1];
      m[11] = p[2];
      return m;
    };
    s.raster.math.concat =
        [](void *, const BattleActMatrix &,
           const BattleActMatrix &b) -> std::optional<BattleActMatrix> {
      return b;
    };
    s.raster.math.scale = [](void *, float x, float y,
                             float z) -> std::optional<BattleActMatrix> {
      BattleActMatrix m{};
      m[0] = x;
      m[5] = y;
      m[10] = z;
      return m;
    };
    s.raster.math.rotation =
        [](void *, float, uint32_t axis) -> std::optional<BattleActMatrix> {
      assert(axis == 121);
      BattleActMatrix m{};
      m[0] = m[5] = m[10] = 1;
      return m;
    };
    return s;
  }
};
int main() {
  Fixture f;
  assert(packedBattleClassPartyButton(f.services()));
  assert(f.real(Fixture::W + 0x120) == 90 && f.buttonArgs.empty());
  f.item = 3;
  assert(packedBattleClassPartyButton(f.services()));
  assert((f.buttonArgs == std::vector<float>{5, 100, 30, 85}));
  assert(f.sounds == 0);
  f.party();
  f.item = -1;
  assert(packedBattleClassPartyButton(f.services()));
  assert((f.buttonArgs == std::vector<float>{4, 150, 30, 0}));
  f.item = 4;
  assert(packedBattleClassPartyButton(f.services()));
  assert(f.sounds == 1 && f.buttonArgs[3] == 85);
  assert(packedBattleClassPartyButton(f.services()) && f.sounds == 1);
  f.floating(Fixture::W + 0x120, 2);
  assert(packedBattleClassPartyButton(f.services()));
  assert(f.buttonArgs[3] == -3);
  assert(packedBattleClassPartyButton(f.services()));
  assert(f.buttonArgs[3] == 0);
  f.calls.clear();
  assert(packedBattleClassPinchMarks(f.services()));
  assert(f.calls.size() == 3 &&
         f.calls[0].function ==
             F::Init2D); // Actual healthy early exits, not stubbed draws.
  f.halves[Fixture::U + 0x10c] = 1;
  f.words[Fixture::G + 0x1c] = 25;
  assert(packedBattleClassMarioPinch(f.services()));
  assert(f.calls.back().function == F::Quad &&
         f.calls.back().integers == std::vector<uint32_t>({233, 82, 47, 255}));
  bool critical = false;
  for (const auto &r : f.calls)
    if (r.function == F::TextureObject && r.integers[1] == 12)
      critical = true;
  assert(critical);
  assert(packedBattleClassButton(5, 10, 20, 90, f.services()));
  assert(f.calls.back().function == F::TexturePlane2 &&
         f.calls.back().integers[0] == 5 && (*f.calls.back().matrix)[3] == 10);
  const auto base = Fixture::B + 0x2858, member = base + 0x1bc, out = 0x90000u;
  f.words[out + 4] = 77;
  packedBattleAudienceItemOn({out, out + 4, 0, 0, 0},
                             f.services().raster.memory);
  assert(f.words[out] == uint32_t(-1) && f.words[out + 4] == 77);
  f.words[member] = 1;
  f.halves[member + 0x1c] = 0;
  f.words[base + 0xf260] = 4;
  f.floating(member + 0x48, 1);
  f.floating(member + 0x4c, 2);
  f.floating(member + 0x50, 3);
  f.words[base + 0xf26c] = 123;
  packedBattleAudienceItemOn({out, out + 4, out + 8, out + 12, out + 16},
                             f.services().raster.memory);
  assert(f.words[out] == 0 && f.real(out + 4) == 1 && f.real(out + 8) == 2 &&
         f.real(out + 12) == 13 && f.words[out + 16] == 123);
  f.words[base + 0xf260] = 5;
  packedBattleAudienceItemOn({out, 0, 0, 0, 0}, f.services().raster.memory);
  assert(f.words[out] == uint32_t(-1));
  f.words[base + 0xf260] = 7;
  packedBattleAudienceItemOn({out, 0, 0, 0, 0}, f.services().raster.memory);
  assert(f.words[out] == 0);
}
