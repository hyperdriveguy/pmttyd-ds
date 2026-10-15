#include "battle_class_observable_draw.hpp"
#include "battle_act_ui.hpp"
#include "battle_act_draw_continuation.hpp"
#include <limits>
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
         BattleClassObservableDrawCursor& c) {
  return resumeBattleClassObservableDraw(Fixture::W,Fixture::R,stack,s,c);
}
void complete(const std::vector<BattleActDrawSlot>& stack,
              const BattleClassDrawContinuationServices& s,
              BattleClassObservableDrawCursor& c) {
  for(unsigned attempt=0;attempt<8;++attempt)if(run(stack,s,c))return;
  assert(false);
}
}
namespace {
std::vector<uint32_t> iconOrder(const CompositeFixture& f) {
  std::vector<uint32_t> result;
  for(const auto& e:f.acknowledged)
    if(e.kind==uint32_t(F::IconColor) || e.kind==uint32_t(F::Icon))result.push_back(e.integers.at(1));
  return result;
}
void sparse(CompositeFixture& f,std::vector<BattleActDrawSlot>& stack) {
  f.words[Fixture::W+0x28]=0;stack[2].position.reset();
}
void coldEntry() {
  struct Cold {
    CompositeFixture f;
    BattleClassDrawContinuationServices raster;
    BattleClassObservableDrawCursor icons;
    std::vector<std::vector<BattleActDrawSlot>> produced;
  } cold;
  auto& f=cold.f;
  auto memory=f.services().memory;
  memory.memory.storeWord=[](void* p,uint32_t a,uint32_t value){static_cast<Fixture*>(p)->words[a]=value;};
  memory.memory.storeByte=[](void* p,uint32_t a,uint8_t value){static_cast<Fixture*>(p)->words[a]=value;};
  f.words[Fixture::B+0x1c78]=0;f.words[Fixture::B+0x1bb4]=0;
  f.words[0x80415fd0]=0x70000;f.words[0x70004]=60;
  BattleActUiServices init;init.memory=memory;
  init.allocate=[](void*,uint32_t size)->std::optional<uint32_t>{assert(size==0x124);return Fixture::W;};
  assert(initializePackedBattleActUi(Fixture::B,Fixture::B+0x1bb4,Fixture::R,0,init));
  for(unsigned i=0;i<3;++i)assert(f.words[Fixture::W+0x34+4*i]==0 && f.words[Fixture::W+0x20+4*i]==0);
  f.floating(0x80427440u+uint32_t(-20496),12);
  f.floating(0x80427440u+uint32_t(-20492),3.1415927410125732f);
  f.floating(0x80427440u+uint32_t(-20484),270);
  for(unsigned i=0;i<3;++i)f.floating(0x803635d8+4*i,float(240-120*i));
  cold.raster=f.binding();
  auto draw=cold.raster.raster.math;draw.memory=memory;draw.user=&cold;
  draw.camera=[](void*,uint32_t camera){assert(camera==8);return true;};
  // Explicit fixture projection/SDK values; the original parent, not this
  // policy, produces the row's actual optional position/matrix/parameter.
  draw.center=[](void*)->std::optional<std::array<float,3>>{return std::array<float,3>{20,30,0};};
  draw.offset=[](void*,float)->std::optional<std::array<float,2>>{return std::array<float,2>{0,-35};};
  draw.translation=[](void*,const std::array<float,3>& p)->std::optional<BattleActMatrix>{auto m=OriginalFixture::identity();m[3]=p[0];m[7]=p[1];m[11]=p[2];return m;};
  draw.rotation=[](void*,float,uint32_t axis)->std::optional<BattleActMatrix>{assert(axis==120);return OriginalFixture::identity();};
  draw.concat=[](void*,const BattleActMatrix& a,const BattleActMatrix&)->std::optional<BattleActMatrix>{return a;};
  draw.icons=[](void* p,uint32_t w,uint32_t rows,const std::vector<BattleActDrawSlot>& stack){auto& c=*static_cast<Cold*>(p);c.produced.push_back(stack);return resumeBattleClassObservableDraw(w,rows,stack,c.raster,c.icons);};
  draw.partyButton=[](void*){return true;};draw.pinchMark=[](void*){return true;};
  BattleActDrawCursor parent;
  assert(resumePackedBattleActMenuDraw(draw,parent));
  assert(!parent.active() && !cold.icons.active());
  for(const auto& slot:cold.produced.at(0))assert(!slot.position && !slot.matrix && !slot.parameter);
  assert(f.acknowledged.empty());
  assert(f.words[Fixture::W+0x38]==1); // Source delay0 row transitioned, flags still0.
  assert(resumePackedBattleActMenuDraw(draw,parent));
  assert(cold.produced.size()==2 && cold.produced[1][1].position);
  assert(!cold.produced[1][0].position && !cold.produced[1][2].position);
  assert((iconOrder(f)==std::vector<uint32_t>{335}));
}
}
struct MutatingFixture : CompositeFixture {
  decltype(BattleClassRasterServices{}.request) originalRequest=nullptr;
  bool countMutation=false,changed=false;
  BattleClassDrawContinuationServices binding() {
    auto s=CompositeFixture::binding();originalRequest=s.raster.request;
    s.raster.request=[](void* p,const BattleClassRasterRequest& request)->std::optional<uint32_t> {
      auto& f=*static_cast<MutatingFixture*>(static_cast<Fixture*>(p));
      const auto result=f.originalRequest(p,request);
      if(result && !f.changed && (request.function==F::Icon || request.function==F::IconColor)) {
        f.changed=true;
        if(f.countMutation)f.words[B+0x1bb4+8]=2;
        else f.words[W+0x20]=0;
      }
      return result;
    };
    return s;
  }
};
int main() {
  coldEntry();
  // Sparse zero active rows acknowledge no raster effects, not synthetic data.
  CompositeFixture empty;auto zero=slots();
  for(unsigned i=0;i<3;++i){empty.words[Fixture::W+0x20+4*i]=0;zero[i]={};}
  BattleClassObservableDrawCursor ec;
  assert(run(zero,empty.binding(),ec) && !ec.active() && empty.acknowledged.empty());

  CompositeFixture single;auto one=slots();
  single.words[Fixture::W+0x24]=single.words[Fixture::W+0x28]=0;
  one[1]={};one[2]={};BattleClassObservableDrawCursor oc;
  complete(one,single.binding(),oc);
  assert((iconOrder(single)==std::vector<uint32_t>{334}));
  CompositeFixture loneBubble;auto bubble=slots();
  loneBubble.words[Fixture::W+0x20]=loneBubble.words[Fixture::W+0x28]=0;
  bubble[0]={};bubble[2]={};bubble[1].position.reset();
  BattleClassObservableDrawCursor lb;complete(bubble,loneBubble.binding(),lb);
  assert((iconOrder(loneBubble)==std::vector<uint32_t>{335}));
  assert((loneBubble.glyphs==std::vector<unsigned>{0,1,2}));

  CompositeFixture baseline;auto stack=slots();sparse(baseline,stack);
  (*stack[0].position)[1]=10;(*stack[1].position)[1]=30;
  auto bs=baseline.binding();BattleClassObservableDrawCursor bc;complete(stack,bs,bc);
  assert((iconOrder(baseline)==std::vector<uint32_t>{335,334}));
  for(size_t stop=0;stop<baseline.acknowledged.size();++stop) {
    CompositeFixture f;auto borrowed=stack;sparse(f,borrowed);f.stop=int(stop);
    auto s=f.binding();BattleClassObservableDrawCursor c;
    for(unsigned attempt=0;!f.stalled && attempt<8;++attempt)assert(!run(borrowed,s,c));
    assert(f.stalled && c.active());
    const auto prefix=f.acknowledged.size();
    assert(!run(borrowed,s,c) && f.acknowledged.size()==prefix);
    auto copy=borrowed;assert(!run(copy,s,c));
    auto wrong=s;wrong.raster.user=&baseline;assert(!run(borrowed,wrong,c));
    // Retained-boundary visibility/count/acting-owner changes cannot admit
    // another child or replay an already acknowledged prefix.
    const auto flags=f.words[Fixture::W+0x20];
    f.words[Fixture::W+0x20]=0;assert(!run(borrowed,s,c));f.words[Fixture::W+0x20]=flags;
    f.words[Fixture::B+0x1bb4+8]=2;assert(!run(borrowed,s,c));f.words[Fixture::B+0x1bb4+8]=3;
    f.words[0x8041eb90]=0xbad00000;assert(!run(borrowed,s,c));f.words[0x8041eb90]=Fixture::B;
    (*borrowed[0].position)[1]=11;assert(!run(borrowed,s,c));(*borrowed[0].position)[1]=10;
    assert(f.acknowledged.size()==prefix);
    f.released=true;complete(borrowed,s,c);
    assert(f.acknowledged==baseline.acknowledged && f.glyphs==baseline.glyphs);
  }
  for(const auto value:{10.f,std::numeric_limits<float>::quiet_NaN(),std::numeric_limits<float>::infinity()}) {
    CompositeFixture ambiguous;auto a=slots();sparse(ambiguous,a);
    (*a[0].position)[1]=10;(*a[1].position)[1]=value;
    BattleClassObservableDrawCursor c;
    assert(!run(a,ambiguous.binding(),c) && c.active());
    assert(ambiguous.acknowledged.empty());
  }
  // All-known source sort preserves its original non-stable tie behavior:
  // high inactive row swaps A behind equal B, so visible order is B,A.
  CompositeFixture known;known.words[Fixture::W+0x24]=1;known.words[Fixture::W+0x28]=0;
  auto all=slots();(*all[0].position)[1]=1;(*all[1].position)[1]=1;(*all[2].position)[1]=2;
  BattleClassObservableDrawCursor kc;complete(all,known.binding(),kc);
  assert((iconOrder(known)==std::vector<uint32_t>{335,334}));
  CompositeFixture sourceKnown;sourceKnown.words=known.words;
  BattleClassDrawContinuationCursor exact;
  assert(resumeBattleClassDrawIcons(Fixture::W,Fixture::R,all,sourceKnown.binding(),exact));
  assert(sourceKnown.acknowledged==known.acknowledged);
  CompositeFixture heldKnown;heldKnown.words=known.words;heldKnown.stop=int(known.acknowledged.size()-1);
  BattleClassObservableDrawCursor hc;auto hs=heldKnown.binding();
  assert(!run(all,hs,hc));heldKnown.released=true;complete(all,hs,hc);
  assert(heldKnown.acknowledged==known.acknowledged);
  for(bool full:{false,true})for(bool count:{false,true}) {
    MutatingFixture mutation;mutation.countMutation=count;auto input=slots();
    if(!full)sparse(mutation,input);
    auto services=mutation.binding();BattleClassObservableDrawCursor cursor;
    assert(!run(input,services,cursor) && mutation.changed && cursor.active());
    const auto completed=mutation.acknowledged.size();
    mutation.words[Fixture::B+0x1bb4+8]=3;mutation.words[Fixture::W+0x20]=1;
    // This provider already executed a real operation before violating the
    // admission contract. Restoring memory cannot restart/replay that prefix.
    assert(!run(input,services,cursor) && mutation.acknowledged.size()==completed);
  }
  std::cout<<"Observable class cold entry, sparse unique order, exact ties and retained boundaries passed; sparseBoundaries="<<baseline.acknowledged.size()<<"\n";
}
