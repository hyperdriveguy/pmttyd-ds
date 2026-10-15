#include "battle_class_draw_adapter.hpp"
#include <cassert>
#include <cstring>
#include <map>
using namespace ttyd;
using F = BattleClassRasterService;
struct Fixture {
  static constexpr uint32_t B = 0x10000, R = 0x20000, W = 0x30000,
                            Cam = 0x40000;
  std::map<uint32_t, uint32_t> words;
  std::vector<BattleClassRasterRequest> calls;
  bool cameraReady = true;
  uint32_t width = 130;
  unsigned identities = 0, rotations = 0;
  void floating(uint32_t a, float f) {
    uint32_t n;
    std::memcpy(&n, &f, 4);
    words[a] = n;
  }
  Fixture() {
    words[0x8041eb90] = B;
    words[B + 0x420] = 1;
    words[B + 0x24] = 0x60000;
    words[0x60008] = 222;
    words[B + 0x1bb4 + 8] = 2;
    words[W + 0x20] = 1; // Circle.
    words[W + 0x24] = 3; // Selected bubble.
    words[R + 20 + 4] = 1;
    words[R + 20 + 8] = 0x12345678;
    words[R + 20 + 0xc] = 335;
    for(unsigned i=0;i<12;++i)floating(Cam + 0x11c + 4*i, identity()[i]);
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
    s.request = [](void* p, const BattleClassRasterRequest& r)
        -> std::optional<uint32_t> {
      auto& f = *static_cast<Fixture*>(p);
      f.calls.push_back(r);
      assert(r.function == F::Camera && r.integers == std::vector<uint32_t>{8});
      return f.cameraReady ? std::optional<uint32_t>{Cam} : std::nullopt;
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
  Fixture f;
  BattleClassAssets assets;
  MessageFont font;
  assert(assets.load("romfs/battle-class-surfaces.bin", "romfs/battle-class-icons.bin"));
  assert(font.load("romfs/font.pmfont"));
  BattleClassBackend backend;
  backend.assets = &assets;
  backend.font = &font;
  unsigned labelCalls = 0, randomCalls = 0;
  bool labelReady = true;
  backend.label = [&](uint32_t address) -> const char* {
    ++labelCalls;
    assert(address == 0x12345678);
    return labelReady ? "Jump" : nullptr; // Explicit fixture label, no production fallback.
  };
  backend.random = [&](uint32_t bound) {
    ++randomCalls;
    assert(bound == 64);
    return 0u; // Explicit deterministic fixture RNG owner.
  };
  std::vector<BattleActDrawSlot> slots(2);
  slots[0] = {{std::array<float,3>{10, 20, 30}}, Fixture::identity(), 180};
  slots[1] = {{std::array<float,3>{40, 50, 0}}, Fixture::identity(), 0};
  assert(drawBattleClassBackendIcons(Fixture::W, Fixture::R, slots, f.services(), backend));
  // Depth-sort draws the higher-Y bubble first (2 surfaces + icon +4glyphs),
  // followed by the lower-Y circle (8 surfaces + icon).
  assert(backend.complete() && backend.quads.size() == 16);
  assert(backend.quads[0].texture == BattleClassTexture::Surfaces);
  assert(backend.quads[2].texture == BattleClassTexture::ClassIcons);
  assert(backend.quads[3].texture == BattleClassTexture::Font);
  assert(backend.quads[7].texture == BattleClassTexture::Surfaces);
  assert(backend.quads[15].texture == BattleClassTexture::ClassIcons);
  assert(labelCalls == 2 && randomCalls == 8);
  assert(f.calls.size() == 2 && f.identities == 1 && f.rotations == 5);
  // Caller batch is appended, never automatically cleared.
  auto retained = backend.quads.size();
  assert(drawBattleClassBackendIcons(Fixture::W, Fixture::R, slots, f.services(), backend));
  assert(backend.quads.size() == retained * 2 && randomCalls == 16);
  f.cameraReady = false;
  assert(!drawBattleClassBackendIcons(Fixture::W, Fixture::R, slots, f.services(), backend));
  assert(backend.quads.size() == retained * 2 && randomCalls == 16);
  f.cameraReady = true;
  backend.begin();
  backend.random = {};
  assert(!drawBattleClassBackendIcons(Fixture::W, Fixture::R, slots, f.services(), backend));
  assert(randomCalls == 16 && !backend.quads.empty()); // Failure is not rollback.
  backend.begin();
  labelReady = false;
  assert(!drawBattleClassBackendIcons(Fixture::W, Fixture::R, slots, f.services(), backend));
  backend.begin();
  labelReady = true;
  slots[0].position.reset();
  assert(!drawBattleClassBackendIcons(Fixture::W, Fixture::R, slots, f.services(), backend));
  assert(backend.quads.empty());
}
