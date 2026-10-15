#include "battle_class_draw_continuation.hpp"
#include <iostream>
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
  unsigned stalled=0;
  Fixture() {
    original = OriginalFixture::services();
    const auto view = identity();
    for (unsigned i = 0; i < 12; ++i)
      floating(Cam + 0x11c + 4 * i, view[i]);
  }
  bool event(const Event &e) {
    attempted.push_back(e);
    if (int(acknowledged.size()) == stop && !released) {
      ++stalled;return false;
    }
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
      assert(r.function!=F::FontString);
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
struct CompositeFixture : Fixture {
  unsigned fontStage=0;
  std::vector<unsigned> glyphs;
  std::optional<Event> fontRequest;
  CompositeFixture() {
    words[B+0x420]=1;words[B+0x24]=0x60000;words[0x60008]=222;
    words[B+0x1bb4+8]=3;
    for(unsigned i=0;i<3;++i) {
      words[W+0x20+4*i]=i==1?3:1;
      words[R+20*i+4]=1;words[R+20*i+8]=0x12345678;words[R+20*i+0xc]=334+i;
    }
  }
  BattleClassDrawContinuationServices binding(bool font=true) {
    BattleClassDrawContinuationServices s;s.raster=Fixture::services();s.fontUser=this;
    if(font)s.retainedFontString=[](void* p,const BattleClassRasterRequest& r)->std::optional<uint32_t> {
      auto& f=*static_cast<CompositeFixture*>(p);
      const Event request{uint32_t(r.function),r.integers,r.reals,r.matrix};
      assert(r.function==F::FontString);
      if(!f.fontStage) {
        if(!f.event(request))return {};
        f.fontRequest=request;f.glyphs.push_back(0);f.fontStage=1;
        return {}; // First real glyph emitted; original FontString still pending.
      }
      assert(f.fontRequest==request);
      f.glyphs.push_back(1);f.glyphs.push_back(2);f.fontStage=0;f.fontRequest.reset();
      return f.original.request(&f,r);
    };
    return s;
  }
};
std::vector<BattleActDrawSlot> slots() {
  const BattleActMatrix matrix{1,0,0,12,0,1,0,23,0,0,1,34};
  std::vector<BattleActDrawSlot> result(3);
  for(unsigned i=0;i<3;++i) {
    result[i].position=std::array<float,3>{10,float(30-10*i),30};
    result[i].matrix=matrix;result[i].parameter=180;
  }
  return result;
}
bool run(const std::vector<BattleActDrawSlot>& stack,
         const BattleClassDrawContinuationServices& s,
         BattleClassDrawContinuationCursor& c) {
  return resumeBattleClassDrawIcons(Fixture::W,Fixture::R,stack,s,c);
}
void complete(const std::vector<BattleActDrawSlot>& stack,
              const BattleClassDrawContinuationServices& s,
              BattleClassDrawContinuationCursor& c) {
  for(unsigned attempt=0;attempt<8;++attempt)if(run(stack,s,c))return;
  assert(false);
}
}
int main() {
  const auto stack=slots();CompositeFixture baseline;auto bs=baseline.binding();
  BattleClassDrawContinuationCursor bc;
  complete(stack,bs,bc);assert(!bc.active());
  assert((baseline.glyphs==std::vector<unsigned>{0,1,2}));
  assert(!baseline.acknowledged.empty());
  std::vector<uint32_t> iconOrder;
  for(const auto& call:baseline.acknowledged)
    if(call.kind==uint32_t(F::Icon) || call.kind==uint32_t(F::IconColor))
      iconOrder.push_back(call.integers.at(1));
  assert((iconOrder==std::vector<uint32_t>{334,335,336})); // Circle, bubble, circle.
  assert(baseline.requests(F::FontString).size()==1);
  for(size_t stop=0;stop<baseline.acknowledged.size();++stop) {
    CompositeFixture f;f.stop=int(stop);auto s=f.binding();BattleClassDrawContinuationCursor c;
    for(unsigned attempt=0;!f.stalled && attempt<8;++attempt)assert(!run(stack,s,c));
    assert(f.stalled && c.active());
    const auto prefix=f.acknowledged.size(),reads=f.reads.size();
    const auto glyphs=f.glyphs;
    assert(!run(stack,s,c));
    assert(f.acknowledged.size()==prefix && f.reads.size()==reads && f.glyphs==glyphs);
    auto copy=stack;
    assert(!run(copy,s,c)); // Borrowed vector identity, not equal contents.
    assert(!resumeBattleClassDrawIcons(Fixture::W+4,Fixture::R,stack,s,c));
    assert(!resumeBattleClassDrawIcons(Fixture::W,Fixture::R+20,stack,s,c));
    auto wrong=s;wrong.fontUser=&baseline;assert(!run(stack,wrong,c));
    wrong=s;wrong.raster.math.user=&baseline;assert(!run(stack,wrong,c));
    wrong=s;wrong.raster.identity=nullptr;assert(!run(stack,wrong,c));
    wrong=s;wrong.retainedFontString=nullptr;assert(!run(stack,wrong,c));
    assert(f.acknowledged.size()==prefix && f.reads.size()==reads && f.glyphs==glyphs);
    f.released=true;complete(stack,s,c);assert(!c.active());
    assert(f.acknowledged==baseline.acknowledged);
    assert(f.reads==baseline.reads && f.words==baseline.words);
    assert(f.calls.size()==baseline.calls.size());
    assert(f.identities==baseline.identities && f.rotations==baseline.rotations);
    assert(f.glyphs==baseline.glyphs);
  }
  // No fallback to an unsafe raw FontString request, even though all other
  // linked services and earlier circle/bubble operations have succeeded.
  CompositeFixture missing;auto ms=missing.binding(false);BattleClassDrawContinuationCursor mc;
  assert(!run(stack,ms,mc) && mc.active());
  const auto reads=missing.reads.size(),prefix=missing.acknowledged.size();
  assert(!run(stack,ms,mc));
  assert(missing.reads.size()==reads && missing.acknowledged.size()==prefix);
  assert(missing.glyphs.empty());
  // Unwritten source stack elements remain an unavailable source frontier.
  CompositeFixture unwritten;auto us=unwritten.binding();auto absent=slots();absent[0].position.reset();
  BattleClassDrawContinuationCursor uc;assert(!run(absent,us,uc));
  assert(unwritten.acknowledged.empty() && unwritten.glyphs.empty());
  std::cout<<"Retained class outer/raster composition and partial FontString ownership passed; boundaries="<<baseline.acknowledged.size()<<"\n";
}
