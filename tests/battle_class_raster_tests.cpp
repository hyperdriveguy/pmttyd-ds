#include "battle_class_raster.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <map>
using namespace ttyd;
using F = BattleClassRasterService;
struct Fixture {
  static constexpr uint32_t B = 0x10000, R = 0x20000, W = 0x30000,
                            Cam = 0x40000;
  std::map<uint32_t, uint32_t> words;
  std::vector<BattleClassRasterRequest> calls;
  bool changeEnabledAtWidth = false;
  uint32_t width = 130;
  unsigned identities = 0, rotations = 0;
  void floating(uint32_t a, float f) {
    uint32_t n;
    std::memcpy(&n, &f, 4);
    words[a] = n;
  }
  Fixture() {
    words[0x8041eb90] = B;
    words[B + 0x163f8] = 0x50000;
    words[0x500a0] = 0x51000;
    words[0x51000] = 0x52000;
    words[R + 4] = 1;
    words[R + 8] = 0x12345678;
    words[R + 0xc] = 334;
    std::map<int32_t, float> constants = {
        {-20500, 0},    {-20380, 1.5}, {-20280, 1.3},
        {-20276, .005}, {-20452, 1},   {-20472, .01745329238474369f},
        {-20488, 180},  {-20476, 360}, {-20272, 8},
        {-20400, 4},    {-20268, -4},  {-20264, -3.076923131942749f},
        {-20260, 7.5},  {-20256, .66}, {-20252, -5},
        {-20248, 64},   {-20244, -64}, {-20292, 76},
        {-20240, 80},   {-20236, -67}, {-20468, 20},
        {-20232, 52.5}, {-20480, 90},  {-20228, .9}};
    for (auto [offset, value] : constants)
      floating(0x80427440u + uint32_t(offset), value);
    for (auto offset : {-20544, -20532})
      words[0x80427440u + uint32_t(offset)] = 0xe1e1e1ff;
    for (auto offset : {-20540, -20528, -20524})
      words[0x80427440u + uint32_t(offset)] = 0x000000ff;
    words[0x80427440u + uint32_t(-20536)] = 0x808080ff;
    const float xy[] = {-20, 40, 0,  40, 0,  20,  -20, 20, 20, 40, 20,
                        20,  0,  20, 0,  40, -20, 20,  0,  20, 0,  0,
                        -20, 0,  0,  20, 20, 20,  20,  0,  0,  0};
    const float uv[] = {0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0,
                        0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0};
    for (unsigned i = 0; i < 32; ++i) {
      floating(0x802ef650 + 4 * i, xy[i]);
      floating(0x802ef6d0 + 4 * i, uv[i]);
    }
  }
  static BattleActMatrix identity() {
    BattleActMatrix m{};
    m[0] = m[5] = m[10] = 1;
    return m;
  }
  BattleClassRasterServices services() {
    BattleClassRasterServices s;
    s.user = this;
    s.memory.memory.user = this;
    s.memory.memory.word = [](void *p, uint32_t a) {
      return static_cast<Fixture *>(p)->words[a];
    };
    s.memory.memory.half = [](void *p, uint32_t a) {
      return uint16_t(static_cast<Fixture *>(p)->words[a]);
    };
    s.request =
        [](void *p,
           const BattleClassRasterRequest &r) -> std::optional<uint32_t> {
      auto &f = *static_cast<Fixture *>(p);
      f.calls.push_back(r);
      if (r.function == F::Camera)
        return Cam;
      if (r.function == F::TextureObject) {
        assert(r.integers[0] == 0x52000);
        return 0xabcdef;
      }
      if (r.function == F::FontWidth) {
        assert(r.integers[0] == 0x12345678);
        if (f.changeEnabledAtWidth)
          f.words[R + 4] = 0;
        return f.width;
      }
      return 0;
    };
    s.truncate = [](void *, float v) -> std::optional<int32_t> {
      return int32_t(v);
    };
    s.identity = [](void *p) -> std::optional<BattleActMatrix> {
      ++static_cast<Fixture *>(p)->identities;
      return identity();
    };
    s.math.user = this;
    s.math.translation =
        [](void *,
           const std::array<float, 3> &p) -> std::optional<BattleActMatrix> {
      auto m = identity();
      m[3] = p[0];
      m[7] = p[1];
      m[11] = p[2];
      return m;
    };
    s.math.scale = [](void *, float x, float y,
                      float z) -> std::optional<BattleActMatrix> {
      BattleActMatrix m{};
      m[0] = x;
      m[5] = y;
      m[10] = z;
      return m;
    };
    s.math.rotation = [](void *p, float,
                         uint32_t axis) -> std::optional<BattleActMatrix> {
      assert(axis == 121);
      ++static_cast<Fixture *>(p)->rotations;
      return identity();
    };
    s.math.concat =
        [](void *, const BattleActMatrix &a,
           const BattleActMatrix &b) -> std::optional<BattleActMatrix> {
      BattleActMatrix c{};
      for (unsigned r = 0; r < 3; ++r)
        for (unsigned col = 0; col < 4; ++col) {
          float v = col == 3 ? a[r * 4 + 3] : 0;
          for (unsigned k = 0; k < 3; ++k)
            v += a[r * 4 + k] * b[k * 4 + col];
          c[r * 4 + col] = v;
        }
      return c;
    };
    return s;
  }
  std::vector<BattleClassRasterRequest> requests(F id) const {
    std::vector<BattleClassRasterRequest> result;
    for (const auto &c : calls)
      if (c.function == id)
        result.push_back(c);
    return result;
  }
};
int main() {
  Fixture circle;
  assert(drawPackedBattleClassCircle(Fixture::R, Fixture::identity(),
                                     {10, 20, 30}, 180, circle.services()));
  assert(circle.calls.front().function == F::Camera &&
         circle.calls[1].function == F::Init2D);
  assert(circle.requests(F::Begin).size() == 8 &&
         circle.requests(F::Point).size() == 32);
  const auto points = circle.requests(F::Point);
  assert(points[0].integers == std::vector<uint32_t>({0, 0, 0, 128}));
  assert(points[16].integers == std::vector<uint32_t>({147, 147, 147, 255}));
  const auto icon = circle.requests(F::IconColor);
  assert(icon.size() == 1 &&
         icon[0].integers == std::vector<uint32_t>({16, 334, 0x939393ff}));
  assert(circle.rotations == 2 && circle.identities == 1);
  assert(circle.requests(F::TextureObject)[0].integers ==
         std::vector<uint32_t>({0x52000, 0}));
  Fixture disabled;
  disabled.words[Fixture::R + 4] = 2;
  disabled.words[Fixture::R + 0xc] = 0;
  assert(drawPackedBattleClassCircle(Fixture::R, Fixture::identity(), {0, 0, 0},
                                     0, disabled.services()));
  assert(disabled.requests(F::IconColor)[0].integers ==
         std::vector<uint32_t>({48, 403, 0xc0c0c0ff}));
  Fixture bubble;
  bubble.changeEnabledAtWidth = true;
  assert(drawPackedBattleClassBubble(
      Fixture::W, Fixture::R, Fixture::identity(), 0, bubble.services()));
  const auto quads = bubble.requests(F::Quad);
  assert(quads.size() == 2 &&
         quads[0].reals == std::vector<float>({68, 76, -60, -4}));
  assert(quads[1].reals == std::vector<float>({64, 80, -64, 0}));
  assert(bubble.requests(F::Icon)[0].integers ==
         std::vector<uint32_t>({16, 334}));
  assert(bubble.requests(F::FontColor)[0].integers ==
         std::vector<uint32_t>({0x808080ff}));
  const auto text = bubble.requests(F::FontString)[0];
  assert(text.integers == std::vector<uint32_t>({0x12345678}));
  assert(std::fabs((*text.matrix)[0] - .9f * 90 / 130) < .00001f);
  assert(bubble.calls.back().function == F::FontString);
  Fixture mirrored;
  mirrored.words[Fixture::W + 0x1c] = 1;
  mirrored.width = 0x10000 + 80;
  assert(drawPackedBattleClassBubble(
      Fixture::W, Fixture::R, Fixture::identity(), 360, mirrored.services()));
  assert(mirrored.requests(F::Quad)[1].reals ==
         std::vector<float>({-64, 80, 64, 0}));
  assert(std::fabs((*mirrored.requests(F::FontString)[0].matrix)[0] - .9f) <
         .00001f);
  Fixture invalid;
  assert(!drawPackedBattleClassBubble(
      Fixture::W, Fixture::R, Fixture::identity(), 400, invalid.services()));
  assert(invalid.requests(F::LoadTexture).size() == 1 &&
         invalid.requests(F::Quad).empty());
}
