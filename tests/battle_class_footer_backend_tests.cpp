#include "battle_class_footer_backend.hpp"
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

struct Gate {
  Fixture f;
  BattleClassFooterServices raw = f.services();
  unsigned accepted = 0, target = ~0u, waits = 0;
  bool pause() {
    if (accepted == target && waits++ < 2)
      return true;
    ++accepted;
    return false;
  }
  BattleClassFooterServices services() {
    auto s = raw;
    s.user = this;
    s.raster.user = this;
    s.raster.math.user = this;
    s.project = [](void *p, uint32_t u) -> std::optional<std::array<float, 3>> {
      auto &g = *static_cast<Gate *>(p);
      if (g.pause())
        return {};
      return g.raw.project(g.raw.user, u);
    };
    s.audienceItem = [](void *p, uint32_t u) -> std::optional<int32_t> {
      auto &g = *static_cast<Gate *>(p);
      if (g.pause())
        return {};
      return g.raw.audienceItem(g.raw.user, u);
    };
    s.changePosition = [](void *p, uint32_t u) -> std::optional<bool> {
      auto &g = *static_cast<Gate *>(p);
      if (g.pause())
        return {};
      return g.raw.changePosition(g.raw.user, u);
    };
    s.sound = [](void *p, uint32_t u) {
      auto &g = *static_cast<Gate *>(p);
      return !g.pause() && g.raw.sound(g.raw.user, u);
    };
    s.button = [](void *p, int32_t t, float x, float y, float a) {
      auto &g = *static_cast<Gate *>(p);
      return !g.pause() && g.raw.button(g.raw.user, t, x, y, a);
    };
    s.raster.request =
        [](void *p,
           const BattleClassRasterRequest &q) -> std::optional<uint32_t> {
      auto &g = *static_cast<Gate *>(p);
      if (g.pause())
        return {};
      return g.raw.raster.request(g.raw.raster.user, q);
    };
    s.raster.math.translation =
        [](void *p,
           const std::array<float, 3> &v) -> std::optional<BattleActMatrix> {
      auto &g = *static_cast<Gate *>(p);
      if (g.pause())
        return {};
      return g.raw.raster.math.translation(g.raw.raster.math.user, v);
    };
    s.raster.math.scale = [](void *p, float x, float y,
                             float z) -> std::optional<BattleActMatrix> {
      auto &g = *static_cast<Gate *>(p);
      if (g.pause())
        return {};
      return g.raw.raster.math.scale(g.raw.raster.math.user, x, y, z);
    };
    s.raster.math.rotation =
        [](void *p, float a, uint32_t axis) -> std::optional<BattleActMatrix> {
      auto &g = *static_cast<Gate *>(p);
      if (g.pause())
        return {};
      return g.raw.raster.math.rotation(g.raw.raster.math.user, a, axis);
    };
    s.raster.math.concat =
        [](void *p, const BattleActMatrix &a,
           const BattleActMatrix &b) -> std::optional<BattleActMatrix> {
      auto &g = *static_cast<Gate *>(p);
      if (g.pause())
        return {};
      return g.raw.raster.math.concat(g.raw.raster.math.user, a, b);
    };
    return s;
  }
};
void equal(const Fixture &a, const Fixture &b) {
  assert(a.words == b.words && a.halves == b.halves &&
         a.projections == b.projections && a.buttonArgs == b.buttonArgs &&
         a.sounds == b.sounds && a.calls.size() == b.calls.size());
  for (unsigned i = 0; i < a.calls.size(); ++i) {
    const auto &x = a.calls[i];
    const auto &y = b.calls[i];
    assert(x.function == y.function && x.integers == y.integers &&
           x.reals == y.reals && x.matrix == y.matrix);
  }
}
void setup(Fixture &f, unsigned scenario) {
  if (scenario & 1)
    f.party();
  if (scenario & 2)
    f.item = 4;
  f.floating(Fixture::W + 0x120, scenario & 4 ? 2 : 90);
  f.halves[Fixture::U + 0x10c] = scenario & 8 ? 1 : 3;
  if (scenario & 1)
    f.halves[Fixture::P + 0x10c] = scenario & 8 ? 2 : 1;
  f.words[Fixture::G + 0x1c] = 25;
  f.floating(Fixture::U + 0x3c, scenario & 4 ? -140 : 0);
}
int main(int argc, char **argv) {
  if (argc == 2) {
    BattleClassFooterAssets a;
    assert(!a.load(nullptr) && a.load(argv[1]));
    assert(!a.surface(3) && !a.surface(17));
    assert(a.surface(4)->width == 80 && a.surface(11)->height == 56);
    assert(!a.load("/no/footer/catalog") && a.surface(5));
  }

  for (unsigned op = 0; op < 3; ++op)
    for (unsigned scenario = 0; scenario < 16; ++scenario) {
      Fixture expected;
      setup(expected, scenario);
      auto sync = expected.services();
      assert(op == 0   ? packedBattleClassPartyButton(sync)
             : op == 1 ? packedBattleClassPinchMarks(sync)
                       : packedBattleClassButton(5, 10, 20, 90, sync));
      Gate baseline;
      setup(baseline.f, scenario);
      BattleClassFooterCursor c;
      auto run = [&](Gate &g, BattleClassFooterCursor &cursor) {
        auto s = g.services();
        return op == 0 ? resumeBattleClassPartyFooter(s, cursor)
               : op == 1
                   ? resumeBattleClassPinchFooter(s, cursor)
                   : resumeBattleClassFooterButton(5, 10, 20, 90, s, cursor);
      };
      assert(run(baseline, c));
      equal(expected, baseline.f);
      for (unsigned target = 0; target < baseline.accepted; ++target) {
        Gate held;
        setup(held.f, scenario);
        held.target = target;
        BattleClassFooterCursor cursor;
        assert(!run(held, cursor));
        const auto words = held.f.words;
        auto wrong = held.services();
        wrong.user = &baseline;
        assert(!(op == 0   ? resumeBattleClassPartyFooter(wrong, cursor)
                 : op == 1 ? resumeBattleClassPinchFooter(wrong, cursor)
                           : resumeBattleClassFooterButton(5, 10, 20, 90, wrong,
                                                           cursor)));
        assert(held.f.words == words);
        assert(!run(held, cursor));
        assert(run(held, cursor));
        assert(!cursor.operation && held.accepted == baseline.accepted);
        equal(expected, held.f);
      }
    }
}
