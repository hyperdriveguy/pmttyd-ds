#include "battle_class_frame_pipeline.hpp"
#include "battle_act_ui.hpp"
#include "field_camera_events.hpp"
#include <iostream>
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
struct FrameFixture : Fixture {
  BattleClassAssets assets;
  MessageFont font;
  BattleClassBackend backend;
  std::vector<std::string> completed,attempted;
  std::string hold;
  bool released=false;
  unsigned lookups=0,randoms=0,generation=1;
  BattleActorServices memory;
  BattleClassFramePipeline* nested=nullptr;
  BattleClassPublicationKey nestedKey;
  bool retireInCamera=false;
  unsigned nestedAttempts=0;
  FrameFixture() {
    assert(assets.load("romfs/battle-class-surfaces.bin","romfs/battle-class-icons.bin"));
    assert(font.load("romfs/font.pmfont"));
    backend.assets=&assets;backend.font=&font;
    backend.label=[](uint32_t address)->const char* {assert(address==0x12345678);return "Jump";};
    backend.random=[this](uint32_t bound){assert(bound);++randoms;return 0;};
    memory=Fixture::services().memory;
    memory.memory.storeWord=[](void* p,uint32_t a,uint32_t value){static_cast<Fixture*>(p)->words[a]=value;};
    memory.memory.storeByte=[](void* p,uint32_t a,uint8_t value){static_cast<Fixture*>(p)->words[a]=value;};
    words[B+0x1c78]=0;words[B+0x1bb4]=0;words[B+0x1bb4+8]=3;
    words[R+40+4]=1;words[R+40+8]=0x12345678;words[R+40+0xc]=336;
    words[W]=B+0x1bb4;words[W+4]=R;words[0x80415fd0]=0x70000;words[0x70004]=60;
    floating(0x80427440u+uint32_t(-20496),12);
    floating(0x80427440u+uint32_t(-20492),3.1415927410125732f);
    floating(0x80427440u+uint32_t(-20484),270);
    floating(0x80427440u+uint32_t(-20464),-60);
    floating(0x80427440u+uint32_t(-20460),55);
    floating(0x80427440u+uint32_t(-20456),.1f);
    for(unsigned i=0;i<3;++i)floating(0x803635d8+4*i,float(240-120*i));
    initialize(0);
  }
  void initialize(uint32_t flag) {
    BattleActUiServices s;s.memory=memory;
    s.allocate=[](void*,uint32_t size)->std::optional<uint32_t>{assert(size==0x124);return W;};
    s.release=[](void*,uint32_t address){assert(address==W);return true;};
    assert(initializePackedBattleActUi(B,B+0x1bb4,R,flag,s));
  }
  bool call(const char* name) {
    attempted.emplace_back(name);
    if(hold==name && !released)return false;
    completed.emplace_back(name);return true;
  }
  BattleClassRasterServices raster() {
    auto s=Fixture::services();s.memory=memory;
    s.request=[](void* p,const BattleClassRasterRequest& r)->std::optional<uint32_t> {
      auto& f=*static_cast<FrameFixture*>(static_cast<Fixture*>(p));
      assert(r.function==F::Camera && r.integers==std::vector<uint32_t>{8});
      if(!f.call("rasterCamera"))return {};
      return Cam;
    };
    return s;
  }
  BattleClassDisplayServices display() {
    BattleClassDisplayServices s;s.user=this;s.draw.user=this;s.draw.memory=memory;
    s.cursorLookup=[](void* p,uint32_t menu,uint32_t type,std::optional<uint32_t>& out) {
      auto& f=*static_cast<FrameFixture*>(p);assert(menu==B+0x171c && type==14);
      if(!f.call("lookup"))return false;
      ++f.lookups;out=B+0x1bb4;return true;
    };
    s.draw.camera=[](void* p,uint32_t id) {
      assert(id==8);auto& f=*static_cast<FrameFixture*>(p);
      if(f.nested) {
        ++f.nestedAttempts;assert(!f.nested->resume(f.nestedKey,8,B));
        if(f.retireInCamera)f.nested->retire();
      }
      return f.call("camera");
    };
    s.draw.center=[](void* p)->std::optional<std::array<float,3>> {
      if(!static_cast<FrameFixture*>(p)->call("center"))return {};
      return std::array<float,3>{-90,60,0};
    };
    s.draw.offset=[](void* p,float angle)->std::optional<std::array<float,2>> {
      if(!static_cast<FrameFixture*>(p)->call("offset"))return {};
      return std::array<float,2>{80*std::cos(angle),35*std::sin(angle)};
    };
    s.draw.interpolate=[](void* p,int32_t mode,int32_t step,int32_t count,float a,float b) {
      if(!static_cast<FrameFixture*>(p)->call("interpolate"))return std::optional<float>{};
      return cameraInterpolation(mode,step,count,a,b);
    };
    s.draw.translation=[](void* p,const std::array<float,3>& xyz)->std::optional<BattleActMatrix> {
      if(!static_cast<FrameFixture*>(p)->call("translation"))return {};
      auto m=identity();m[3]=xyz[0];m[7]=xyz[1];m[11]=xyz[2];return m;
    };
    s.draw.rotation=[](void* p,float angle,uint32_t axis)->std::optional<BattleActMatrix> {
      if(!static_cast<FrameFixture*>(p)->call("rotation"))return {};
      assert(axis==120);
      const float sn=std::sin(angle),cs=std::cos(angle);return BattleActMatrix{1,0,0,0,0,cs,-sn,0,0,sn,cs,0};
    };
    s.draw.scale=[](void* p,float x,float y,float z)->std::optional<BattleActMatrix> {
      if(!static_cast<FrameFixture*>(p)->call("scale"))return {};
      return BattleActMatrix{x,0,0,0,0,y,0,0,0,0,z,0};
    };
    s.draw.concat=[](void* p,const BattleActMatrix& a,const BattleActMatrix& b)->std::optional<BattleActMatrix> {
      if(!static_cast<FrameFixture*>(p)->call("concat"))return {};
      BattleActMatrix out{};
      for(unsigned row=0;row<3;++row)for(unsigned col=0;col<4;++col) {
        float value=col==3?a[4*row+3]:0;
        for(unsigned k=0;k<3;++k)value+=a[4*row+k]*b[4*k+col];
        out[4*row+col]=value;
      }
      return out;
    };
    // Explicit test providers for retained whole source footer owners. The
    // pipeline must call these and must not acknowledge a missing owner.
    s.draw.partyButton=[](void* p){return static_cast<FrameFixture*>(p)->call("party");};
    s.draw.pinchMark=[](void* p){return static_cast<FrameFixture*>(p)->call("pinch");};
    return s;
  }
  bool bind(BattleClassFramePipeline& pipeline,std::shared_ptr<const uint8_t> token) {
    const auto expected=generation;
    return pipeline.bind(token,W,display(),raster(),backend,[this,expected] {
      return generation==expected && words[B+0x1c78]==W;
    });
  }
};
int main() {
  FrameFixture cold;BattleClassFramePipeline pipeline;auto token=std::make_shared<const uint8_t>(0);
  assert(cold.bind(pipeline,token));
  BattleClassPublicationKey key{token,Fixture::W,1};
  assert(pipeline.resume(key,8,Fixture::B));
  assert(!pipeline.active() && pipeline.publication().batch());
  assert(pipeline.publication().batch()->quads.empty() && cold.backend.quads.empty());
  assert(cold.words[Fixture::W+0x38]==1 && cold.words[Fixture::W+0x14]==0);
  const auto completed=cold.completed.size();
  assert(pipeline.resume(key,8,Fixture::B) && cold.completed.size()==completed && cold.lookups==1);
  key.sequence=2;cold.hold="party";
  assert(!pipeline.resume(key,8,Fixture::B) && pipeline.active());
  assert(cold.backend.quads.size()>0 && pipeline.publication().key().sequence==1);
  assert(pipeline.publication().batch()->quads.empty());
  const auto partial=cold.backend.quads.size(),prefix=cold.completed.size();
  assert(!pipeline.resume(key,8,Fixture::B));
  assert(cold.backend.quads.size()==partial && cold.completed.size()==prefix);
  auto replacement=key;++replacement.sequence;assert(!pipeline.resume(replacement,8,Fixture::B));
  replacement=key;replacement.owner=std::make_shared<const uint8_t>(0);assert(!pipeline.resume(replacement,8,Fixture::B));
  cold.released=true;assert(pipeline.resume(key,8,Fixture::B));
  assert(pipeline.publication().key().sequence==2 && pipeline.publication().batch()->quads.size()==partial);
  assert(cold.lookups==2 && pipeline.admit(key) && pipeline.consumeAdmission());
  assert(!pipeline.consumeAdmission());
  key.sequence=1;assert(!pipeline.resume(key,8,Fixture::B));

  // Every whole callback wait keeps the completed queue/math/raster prefix,
  // including actual backend geometry and its font RNG consumption.
  for(const auto* hold:{"lookup","camera","center","offset","interpolate","translation","rotation","scale","concat","rasterCamera","party","pinch"}) {
    FrameFixture f;f.initialize(1);f.hold=hold;
    BattleClassFramePipeline p;auto owner=std::make_shared<const uint8_t>(0);assert(f.bind(p,owner));
    BattleClassPublicationKey pass{owner,Fixture::W,5};
    bool done=p.resume(pass,8,Fixture::B);
    if(done) {assert(std::string(hold)=="rotation");continue;} // Resume states10/11 use no spin.
    assert(p.active() && !p.publication().batch());
    const auto calls=f.completed.size(),quads=f.backend.quads.size();
    const auto rng=f.randoms;
    assert(!p.resume(pass,8,Fixture::B));
    assert(f.completed.size()==calls && f.backend.quads.size()==quads && f.randoms==rng);
    f.released=true;assert(p.resume(pass,8,Fixture::B));
    assert(!p.active() && p.publication().batch() && f.lookups==1);
    const auto finished=f.completed.size();assert(p.resume(pass,8,Fixture::B) && f.completed.size()==finished);
  }
  // Real cold entry's state1 spin path also retains its SDK operation.
  FrameFixture spin;BattleClassFramePipeline sp;auto st=std::make_shared<const uint8_t>(0);
  assert(spin.bind(sp,st));assert(sp.resume({st,Fixture::W,1},8,Fixture::B));
  spin.hold="rotation";BattleClassPublicationKey sk{st,Fixture::W,2};
  assert(!sp.resume(sk,8,Fixture::B));const auto spinPrefix=spin.completed.size();
  assert(!sp.resume(sk,8,Fixture::B) && spin.completed.size()==spinPrefix);
  spin.released=true;assert(sp.resume(sk,8,Fixture::B) && spin.lookups==2);

  // Missing footer owners cannot produce a published success.
  FrameFixture missing;auto ds=missing.display();ds.draw.partyButton=nullptr;
  BattleClassFramePipeline m;auto mt=std::make_shared<const uint8_t>(0);
  assert(m.bind(mt,Fixture::W,ds,missing.raster(),missing.backend,[]{return true;}));
  assert(!m.resume({mt,Fixture::W,1},8,Fixture::B) && !m.publication().batch());
  // Allocation lifetime cannot be proved by manually retaining an old key.
  FrameFixture lifetime;BattleClassFramePipeline life;auto first=std::make_shared<const uint8_t>(0);
  assert(lifetime.bind(life,first));lifetime.hold="party";
  BattleClassPublicationKey old{first,Fixture::W,1};assert(!life.resume(old,8,Fixture::B));
  const auto before=lifetime.completed.size();++lifetime.generation;lifetime.released=true;
  assert(!life.resume(old,8,Fixture::B) && lifetime.completed.size()==before);
  auto fresh=std::make_shared<const uint8_t>(0);assert(!lifetime.bind(life,fresh));
  const auto guest=lifetime.words;life.retire();assert(lifetime.words==guest && !life.publication().batch());
  lifetime.initialize(1);assert(lifetime.bind(life,fresh));
  assert(life.resume({fresh,Fixture::W,1},8,Fixture::B));
  BattleClassPublicationKey latest{fresh,Fixture::W,1};
  assert(life.publication().batch() && life.admit(latest));
  ++lifetime.generation; // Actual source owner ended; guest W deliberately unchanged.
  const auto staleGuest=lifetime.words;
  assert(!life.publication().batch() && !life.admit(latest) && !life.consumeAdmission());
  assert(lifetime.words==staleGuest);
  BattleClassFramePipeline unavailable;
  assert(!unavailable.bind(fresh,Fixture::W,lifetime.display(),lifetime.raster(),lifetime.backend,{}));
  assert(!unavailable.resume({fresh,Fixture::W,1},8,Fixture::B));
  for(bool retire:{false,true}) {
    FrameFixture recursive;BattleClassFramePipeline guarded;auto rt=std::make_shared<const uint8_t>(0);
    assert(recursive.bind(guarded,rt));recursive.nested=&guarded;
    recursive.nestedKey={rt,Fixture::W,1};recursive.retireInCamera=retire;
    const auto beforeGuest=recursive.words;
    const bool ready=guarded.resume(recursive.nestedKey,8,Fixture::B);
    assert(recursive.nestedAttempts==1 && ready!=retire);
    if(retire) {
      assert(!guarded.active() && !guarded.publication().batch() && recursive.words==beforeGuest);
      recursive.nested=nullptr;++recursive.generation;recursive.initialize(1);
      auto nt=std::make_shared<const uint8_t>(0);assert(recursive.bind(guarded,nt));
      assert(guarded.resume({nt,Fixture::W,1},8,Fixture::B));
    }
  }
  std::cout<<"Class frame pipeline cold progression, retained source/backend/footer waits and allocation lifetime passed\n";
}
