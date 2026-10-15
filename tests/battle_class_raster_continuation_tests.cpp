#include "battle_class_raster_continuation.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <map>
using namespace ttyd;
using F = BattleClassRasterService;
struct OriginalFixture {
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
  OriginalFixture() {
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
      return static_cast<OriginalFixture *>(p)->words[a];
    };
    s.memory.memory.half = [](void *p, uint32_t a) {
      return uint16_t(static_cast<OriginalFixture *>(p)->words[a]);
    };
    s.request =
        [](void *p,
           const BattleClassRasterRequest &r) -> std::optional<uint32_t> {
      auto &f = *static_cast<OriginalFixture *>(p);
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
      ++static_cast<OriginalFixture *>(p)->identities;
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
      ++static_cast<OriginalFixture *>(p)->rotations;
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
namespace {
struct Event {
  uint32_t kind = 0;
  std::vector<uint32_t> integers;
  std::vector<float> reals;
  std::optional<BattleActMatrix> matrix;
  bool operator==(const Event &e) const {
    return kind == e.kind && integers == e.integers && reals == e.reals &&
           matrix == e.matrix;
  }
};
struct Fixture : OriginalFixture {
  BattleClassRasterServices original;
  std::vector<Event> acknowledged, attempted;
  std::vector<std::array<uint32_t, 3>> reads;
  int stop = -1;
  bool released = false;
  Fixture() {
    original = OriginalFixture::services();
    const auto view = identity();
    for (unsigned i = 0; i < 12; ++i)
      floating(Cam + 0x11c + 4 * i, view[i]);
  }
  bool event(const Event &e) {
    attempted.push_back(e);
    if (int(acknowledged.size()) == stop && !released)
      return false;
    acknowledged.push_back(e);
    return true;
  }
  BattleClassRasterServices services() {
    auto s = original;
    s.user = this;
    s.memory.memory.user = this;
    s.math.user = this;
    s.memory.memory.word = [](void *p, uint32_t a) {
      auto &f = *static_cast<Fixture *>(p);
      f.reads.push_back({0, a, f.words[a]});
      return f.words[a];
    };
    s.memory.memory.half = [](void *p, uint32_t a) {
      auto &f = *static_cast<Fixture *>(p);
      const auto n = uint16_t(f.words[a]);
      f.reads.push_back({1, a, n});
      return n;
    };
    s.request =
        [](void *p,
           const BattleClassRasterRequest &r) -> std::optional<uint32_t> {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.event({uint32_t(r.function), r.integers, r.reals, r.matrix}))
        return {};
      return f.original.request(&f, r);
    };
    s.truncate = [](void *p, float v) -> std::optional<int32_t> {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.event({1, {}, {v}, {}}))
        return {};
      return f.original.truncate(&f, v);
    };
    s.identity = [](void *p) -> std::optional<BattleActMatrix> {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.event({2, {}, {}, {}}))
        return {};
      return f.original.identity(&f);
    };
    s.math.scale = [](void *p, float x, float y,
                      float z) -> std::optional<BattleActMatrix> {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.event({3, {}, {x, y, z}, {}}))
        return {};
      return f.original.math.scale(&f, x, y, z);
    };
    s.math.translation =
        [](void *p,
           const std::array<float, 3> &v) -> std::optional<BattleActMatrix> {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.event({4, {}, {v[0], v[1], v[2]}, {}}))
        return {};
      return f.original.math.translation(&f, v);
    };
    s.math.rotation = [](void *p, float angle,
                         uint32_t axis) -> std::optional<BattleActMatrix> {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.event({5, {axis}, {angle}, {}}))
        return {};
      return f.original.math.rotation(&f, angle, axis);
    };
    s.math.concat =
        [](void *p, const BattleActMatrix &a,
           const BattleActMatrix &b) -> std::optional<BattleActMatrix> {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.event({6, {}, std::vector<float>(b.begin(), b.end()), a}))
        return {};
      return f.original.math.concat(&f, a, b);
    };
    return s;
  }
};
const BattleActMatrix Base{1, 0, 0, 12, 0, 1, 0, 23, 0, 0, 1, 34};
const std::array<float, 3> Position{10, 20, 30};
bool run(bool bubble, const BattleClassRasterServices &s,
         BattleClassRasterCursor *cursor = nullptr, float angle = 180) {
  if (cursor)
    return bubble ? resumePackedBattleClassBubble(Fixture::W, Fixture::R, Base,
                                                  angle, s, *cursor)
                  : resumePackedBattleClassCircle(Fixture::R, Base, Position,
                                                  angle, s, *cursor);
  return bubble ? drawPackedBattleClassBubble(Fixture::W, Fixture::R, Base,
                                              angle, s)
                : drawPackedBattleClassCircle(Fixture::R, Base, Position, angle,
                                              s);
}
void equivalent(bool bubble, float angle, bool enabled, bool mirrored,
                uint32_t width) {
  Fixture old, fresh;
  for (auto *f : {&old, &fresh}) {
    f->words[Fixture::R + 4] = enabled;
    f->words[Fixture::W + 0x1c] = mirrored;
    f->width = width;
    f->changeEnabledAtWidth = true;
  }
  assert(run(bubble, old.services(), nullptr, angle));
  BattleClassRasterCursor cursor;
  assert(run(bubble, fresh.services(), &cursor, angle));
  assert(!cursor.active());
  assert(old.acknowledged == fresh.acknowledged);
  assert(old.reads == fresh.reads);
  assert(old.words == fresh.words);
  for (size_t stop = 0; stop < fresh.acknowledged.size(); ++stop) {
    Fixture held;
    held.words[Fixture::R + 4] = enabled;
    held.words[Fixture::W + 0x1c] = mirrored;
    held.width = width;
    held.changeEnabledAtWidth = true;
    held.stop = int(stop);
    auto s = held.services();
    BattleClassRasterCursor c;
    assert(!run(bubble, s, &c, angle));
    assert(c.active());
    const auto reads = held.reads;
    const auto request = held.attempted.back();
    assert(!run(bubble, s, &c, angle));
    assert(held.reads == reads && held.attempted.back() == request);
    auto wrong = s;
    wrong.user = &fresh;
    assert(!run(bubble, wrong, &c, angle));
    wrong = s;
    wrong.math.user = &fresh;
    assert(!run(bubble, wrong, &c, angle));
    wrong = s;
    wrong.memory.memory.user = &fresh;
    assert(!run(bubble, wrong, &c, angle));
    auto changed = Base;
    changed[3] += 1;
    assert(!(bubble ? resumePackedBattleClassBubble(Fixture::W, Fixture::R,
                                                    changed, angle, s, c)
                    : resumePackedBattleClassCircle(Fixture::R, changed,
                                                    Position, angle, s, c)));
    assert(held.reads == reads);
    held.released = true;
    assert(run(bubble, s, &c, angle));
    assert(!c.active());
    assert(held.acknowledged == fresh.acknowledged);
    assert(held.reads == fresh.reads);
    assert(held.words == fresh.words);
  }
}
void nullPrefixes() {
  for (bool bubble : {false, true})
    for (unsigned missing = 0; missing < 7; ++missing) {
      Fixture old, fresh;
      auto a = old.services(), b = fresh.services();
      const auto remove = [&](BattleClassRasterServices &s) {
        switch (missing) {
        case 0:
          s.request = nullptr;
          break;
        case 1:
          s.truncate = nullptr;
          break;
        case 2:
          s.identity = nullptr;
          break;
        case 3:
          s.math.scale = nullptr;
          break;
        case 4:
          s.math.translation = nullptr;
          break;
        case 5:
          s.math.rotation = nullptr;
          break;
        case 6:
          s.math.concat = nullptr;
          break;
        }
      };
      remove(a);
      remove(b);
      const auto expected = run(bubble, a);
      BattleClassRasterCursor c;
      assert(run(bubble, b, &c) == expected);
      assert(c.active() != expected);
      assert(old.acknowledged == fresh.acknowledged);
      assert(old.reads == fresh.reads);
      if (!expected) {
        const auto reads = fresh.reads;
        const auto events = fresh.acknowledged;
        assert(!run(bubble, b, &c));
        assert(reads == fresh.reads && events == fresh.acknowledged);
      }
    }
}
size_t first(const Fixture &f, uint32_t kind) {
  for (size_t i = 0; i < f.acknowledged.size(); ++i)
    if (f.acknowledged[i].kind == kind)
      return i;
  assert(false);
  return 0;
}
std::vector<Event> events(const Fixture &f, uint32_t kind) {
  std::vector<Event> result;
  for (const auto &e : f.acknowledged)
    if (e.kind == kind)
      result.push_back(e);
  return result;
}
void mutations() {
  Fixture baseline;
  assert(run(false, baseline.services()));
  Fixture circle;
  circle.stop = int(first(baseline, uint32_t(F::Begin)));
  auto cs = circle.services();
  BattleClassRasterCursor cc;
  assert(!run(false, cs, &cc));
  const auto reads = circle.reads;
  for (unsigned i = 0; i < 32; ++i) {
    circle.floating(0x802ef650 + 4 * i, 999);
    circle.floating(0x802ef6d0 + 4 * i, .5f);
  }
  for (unsigned i = 0; i < 12; ++i)
    circle.floating(Fixture::Cam + 0x11c + 4 * i, 999);
  circle.floating(0x80427440u + uint32_t(-20264), 999);
  assert(!run(false, cs, &cc));
  assert(circle.reads == reads);
  circle.released = true;
  assert(run(false, cs, &cc));
  assert(events(circle, uint32_t(F::Point)) ==
         events(baseline, uint32_t(F::Point)));
  assert(events(circle, uint32_t(F::LoadMatrix)) ==
         events(baseline, uint32_t(F::LoadMatrix)));

  // B belongs to the invocation before the unavailable camera request.
  Fixture cameraWait;
  cameraWait.stop = 0;
  auto cameraServices = cameraWait.services();
  BattleClassRasterCursor cameraCursor;
  assert(!run(false, cameraServices, &cameraCursor));
  cameraWait.words[0x8041eb90] = 0x91000;
  cameraWait.released = true;
  assert(run(false, cameraServices, &cameraCursor));
  assert(events(cameraWait, uint32_t(F::TextureObject))[0].integers[0] ==
         0x52000);

  // Camera matrix elements are fresh only when that later concat is prepared.
  for (bool isBubble : {false, true}) {
    Fixture reference;
    assert(run(isBubble, reference.services()));
    Fixture cameraFresh;
    cameraFresh.stop = int(first(reference, 6));
    auto s = cameraFresh.services();
    BattleClassRasterCursor cursor;
    assert(!run(isBubble, s, &cursor));
    cameraFresh.floating(Fixture::Cam + 0x11c + 3 * 4, 444);
    cameraFresh.released = true;
    assert(run(isBubble, s, &cursor));
    const auto load = events(cameraFresh, uint32_t(F::LoadMatrix));
    assert(load.size() == 1 && load[0].matrix && (*load[0].matrix)[3] == 456);
  }

  Fixture object;
  object.stop = int(first(baseline, uint32_t(F::TextureObject)));
  auto os = object.services();
  BattleClassRasterCursor oc;
  assert(!run(false, os, &oc));
  const auto objectRequest = object.attempted.back();
  object.words[0x8041eb90] = 0x91000;
  object.words[Fixture::B + 0x163f8] = 0x92000;
  object.words[0x500a0] = 0x93000;
  object.words[0x51000] = 0x94000;
  assert(!run(false, os, &oc));
  assert(object.attempted.back() == objectRequest);
  object.released = true;
  assert(run(false, os, &oc));
  assert(events(object, uint32_t(F::TextureObject)) ==
         events(baseline, uint32_t(F::TextureObject)));

  Fixture bubbleBaseline;
  assert(run(true, bubbleBaseline.services()));
  Fixture bubble;
  bubble.stop = int(first(bubbleBaseline, uint32_t(F::FontWidth)));
  auto bs = bubble.services();
  BattleClassRasterCursor bc;
  assert(!run(true, bs, &bc));
  const auto widthRequest = bubble.attempted.back();
  bubble.words[Fixture::R + 8] = 0x99887766;
  bubble.words[Fixture::R + 4] = 0;
  bubble.words[Fixture::R + 0xc] = 343;
  bubble.words[Fixture::W + 0x1c] = 1;
  bubble.floating(0x80427440u + uint32_t(-20500),
                  123); // Entry zero is retained.
  assert(!run(true, bs, &bc));
  assert(bubble.attempted.back() == widthRequest);
  bubble.released = true;
  assert(run(true, bs, &bc));
  assert(events(bubble, uint32_t(F::FontString))[0].integers ==
         std::vector<uint32_t>{0x12345678});
  assert(events(bubble, uint32_t(F::FontColor))[0].integers ==
         std::vector<uint32_t>{0x808080ff});
  assert(events(bubble, uint32_t(F::Quad)) ==
         events(bubbleBaseline, uint32_t(F::Quad)));
  assert(events(bubble, uint32_t(F::Icon)) ==
         events(bubbleBaseline, uint32_t(F::Icon)));
  assert(events(bubble, uint32_t(F::FontString))[0].matrix ==
         events(bubbleBaseline, uint32_t(F::FontString))[0].matrix);

  Fixture start;
  start.stop = int(first(bubbleBaseline, uint32_t(F::FontStart)));
  auto ss = start.services();
  BattleClassRasterCursor sc;
  assert(!run(true, ss, &sc));
  start.words[Fixture::R + 4] = 0;
  start.released = true;
  assert(run(true, ss, &sc));
  assert(events(start, uint32_t(F::FontColor))[0].integers ==
         std::vector<uint32_t>{0x808080ff});

  Fixture color;
  color.stop = int(first(bubbleBaseline, uint32_t(F::FontColor)));
  auto fs = color.services();
  BattleClassRasterCursor fc;
  assert(!run(true, fs, &fc));
  const auto capturedColor = color.attempted.back();
  color.words[Fixture::R + 4] = 0;
  color.words[0x80427440u + uint32_t(-20540)] = 123;
  color.words[Fixture::R + 8] = 456;
  color.released = true;
  assert(run(true, fs, &fc));
  assert(events(color, uint32_t(F::FontColor))[0] == capturedColor);
  assert(events(color, uint32_t(F::FontString))[0].integers ==
         std::vector<uint32_t>{0x12345678});
}
void unavailableAngles() {
  for (bool bubble : {false, true})
    for (float angle : {-1.f, 361.f}) {
      Fixture old, fresh;
      assert(!run(bubble, old.services(), nullptr, angle));
      BattleClassRasterCursor c;
      auto s = fresh.services();
      assert(!run(bubble, s, &c, angle));
      assert(old.reads == fresh.reads &&
             old.acknowledged == fresh.acknowledged);
      const auto reads = fresh.reads;
      const auto calls = fresh.acknowledged;
      assert(!run(bubble, s, &c, angle));
      assert(fresh.reads == reads && fresh.acknowledged == calls);
    }
}
} // namespace
int main() {
  mutations();
  unavailableAngles();
  nullPrefixes();
  for (bool bubble : {false, true})
    for (float angle : {0.f, 90.f, 180.f, 270.f, 360.f})
      equivalent(bubble, angle, true, false, 130);
  equivalent(true, 180, false, true, 30);
  equivalent(true, 90, true, true, 0x12340082);
  equivalent(false, 270, false, false, 0);
}
