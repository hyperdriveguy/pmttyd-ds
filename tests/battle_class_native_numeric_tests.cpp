#include "battle_class_native_numeric.hpp"
#include "battle_audience_aux_callback.hpp"
#include "field_camera_events.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <limits>
#include <map>
using namespace ttyd;
namespace {
uint32_t bits(float v){uint32_t n;std::memcpy(&n,&v,4);return n;}
struct Fixture {
  static constexpr uint32_t B=0x10000,W=0x30000,C=0x40000,Base=0x60010000;
  std::map<uint32_t,uint32_t> words{{0x8041eb90,B},{B+0x1c78,W},{W,C},{W+4,0x50000},{W+0xc,10},{C+8,0},
    {B+0x420,1},{B+0x24,0x60000},{0x60008,222},{B+0x1bb4+8,0}};
  std::shared_ptr<CameraManager> bank=std::make_shared<CameraManager>();
  std::shared_ptr<const uint8_t> token=std::make_shared<const uint8_t>(0);
  BattleClassAssets assets;MessageFont font;BattleClassBackend backend;
  CameraMainCursor main;CameraDrawCursor drawCursor;DisplayQueue queue;
  BattleClassNativeFrame frame;std::unique_ptr<BattleClassNativeNumeric> numeric;
  uint64_t pass=0;bool live=true,mutate=false,invalidateAtEnd=false,tested=false;unsigned sourceReads=0;
  BattleActorServices actors(){BattleActorServices a;a.memory.user=this;
    a.memory.word=[](void* p,uint32_t at){return static_cast<Fixture*>(p)->words.at(at);};
    a.memory.half=[](void* p,uint32_t at){return uint16_t(static_cast<Fixture*>(p)->words.at(at));};
    a.memory.storeWord=[](void* p,uint32_t at,uint32_t v){static_cast<Fixture*>(p)->words[at]=v;};
    a.memory.storeByte=[](void* p,uint32_t at,uint8_t v){static_cast<Fixture*>(p)->words[at]=v;};return a;}
  Fixture(){bank->initializeBoot(608,480);
    words[0x60000+0x3c]=bits(100);words[0x60000+0x40]=bits(50);words[0x60000+0x44]=bits(0);
    for(const auto [offset,v]:std::map<int32_t,float>{{-20220,22.5},{-20216,105},{-20500,0},{-20212,-180},{-20224,4.71238899230957f},{-20240,80},{-20284,35}})words[0x80427440u+uint32_t(offset)]=bits(v);
    words[0x80422244]=bits(304);words[0x80422248]=bits(240);words[0x80422240]=bits(0);
    assert(assets.load("romfs/battle-class-surfaces.bin","romfs/battle-class-icons.bin"));assert(font.load("romfs/font.pmfont"));
    backend.assets=&assets;backend.font=&font;backend.label=[](uint32_t)->const char*{assert(false);return nullptr;};backend.random=[](uint32_t)->uint32_t{assert(false);return 0;};
    BattleClassNativeNumericFooters footers;footers.user=this;footers.party=[](void* p){auto& f=*static_cast<Fixture*>(p);return f.check(*f.numeric);};footers.pinch=[](void*){return true;};
    numeric=std::make_unique<BattleClassNativeNumeric>(frame,actors(),Base,[this](uint32_t at)->std::optional<uint32_t>{++sourceReads;if(mutate)++pass;const auto i=words.find(at);return i==words.end()?std::nullopt:std::optional<uint32_t>(i->second);},[this]{return live;},footers);
    BattleClassDisplayServices display;display.draw=numeric->draw();display.user=this;
    display.cursorLookup=[](void*,uint32_t,uint32_t,std::optional<uint32_t>&){return true;};
    BattleClassNativeFrameBinding binding;binding.allocationOwner=token;binding.allocation=W;binding.generation=7;binding.cameras=bank;binding.actors=actors();
    binding.addresses={Base,[](uint32_t,uint32_t){return true;}};binding.allocationCurrent=[this]{return live && words.at(B+0x1c78)==W;};
    binding.producerCurrent=[this](BattleClassCameraEpoch e){return e.generation==7 && e.pass==pass;};
    assert(frame.bind(std::move(binding),token,display,numeric->raster(),backend));
  }
  bool check(BattleClassNativeNumeric& a){
    const auto d=a.draw();const auto r=a.raster();assert(a.camera()==Base+8*0x400 && a.camera(4)==Base+4*0x400 && !a.camera(7));
    BattleClassNativeNumeric wrong(frame,actors(),Base+4,[this](uint32_t)->std::optional<uint32_t>{++sourceReads;return 123;},[]{return true;});
    const auto wrongBefore=sourceReads;assert(!wrong.camera() && !wrong.word(Base+8*0x400+0x11c) && sourceReads==wrongBefore);
    const auto before=sourceReads;assert(!a.word(Base) && !a.word(Base+8*0x400+0x11d));assert(sourceReads==before);
    assert(a.word(B+0x1c78)==W && !a.word(0x77777777));assert(sourceReads==before+2);
    assert(d.center(d.user) && d.offset(d.user,0) && a.project(0x60000+0x3c));
    for(int mode:{0,4})for(int step:{-2,0,3,12,15})assert(d.interpolate(d.user,mode,step,12,-20,80)==cameraInterpolation(mode,step,12,-20,80));
    assert(d.interpolate(d.user,99,1,0,1,2)==2);assert(!d.interpolate(d.user,99,1,2,1,2));
    assert(!d.interpolate(d.user,0,1,1,0,std::numeric_limits<float>::infinity()));
    BattleAudienceAuxServices sdk;bindAudienceAuxMathAdapter(sdk);
    BattleAudienceMatrixRequest q;q.function=0x8029e230;q.xyz={4,5,6};assert(sdk.matrix(nullptr,q));const auto translated=d.translation(d.user,q.xyz);assert(translated==q.output);
    q.function=0x8029dff0;q.radians=.3f;q.axis=120;assert(sdk.matrix(nullptr,q));const auto rotated=d.rotation(d.user,q.radians,q.axis);assert(rotated==q.output);assert(!d.rotation(d.user,1,999));
    q.function=0x8029e2b0;q.xyz={2,3,4};assert(sdk.matrix(nullptr,q));assert(d.scale(d.user,2,3,4)==q.output);
    q.function=0x8029dd64;q.left=*translated;q.right=*rotated;assert(sdk.matrix(nullptr,q));assert(d.concat(d.user,q.left,q.right)==q.output);
    q.function=0x8029dd04;assert(sdk.matrix(nullptr,q));assert(r.identity(r.user)==q.output);
    assert(r.truncate(r.user,3.9f)==3 && r.truncate(r.user,-3.9f)==-3 && r.truncate(r.user,-2147483648.f)==INT32_MIN);
    for(float v:{std::numeric_limits<float>::quiet_NaN(),std::numeric_limits<float>::infinity(),2147483648.f})assert(!r.truncate(r.user,v));
    BattleClassRasterRequest request{BattleClassRasterService::Camera,{8},{},{}};const auto camera=r.request(r.user,request);assert(camera);
    for(unsigned i=0;i<12;++i)assert(r.memory.memory.word(r.memory.memory.user,*camera+0x11c+4*i)==bits(bank->camera(8)->matrices.view[i/4][i%4]));
    assert(r.memory.memory.word(r.memory.memory.user,B+0x1c78)==W);
    assert(r.memory.memory.half(r.memory.memory.user,B+0x1c78)==uint16_t(W));
    request.integers={4};assert(!r.request(r.user,request));request.function=BattleClassRasterService::Init2D;request.integers={};assert(!r.request(r.user,request));
    tested=true;
    if(invalidateAtEnd){mutate=true;const auto old=pass;assert(!a.word(B+0x1c78));assert(pass==old+1);assert(!d.translation(d.user,{1,2,3}) && !a.camera() && !a.project(0x60000+0x3c));return false;}
    return true;
  }
  bool run(){CameraMainServices m;m.callback=[](void*,uint32_t,uint32_t,CameraState&){return true;};m.lookAt=[](void*,const CameraState& c){return cameraLookAtAdapter(c.eye,c.at,c.up);};assert(bank->main(false,m,main));++pass;
    assert(queue.enter(8,0,0x8012210c,B,500)==DisplayAdmission::Registered);CameraDrawServices s;s.user=this;s.letterbox=[](void*,DisplayQueue&){return true;};s.request=[](void*,const CameraDrawRequest&){return true;};s.display.user=this;s.display.state=[](void*,const DisplayStateRequest&){return true;};
    s.display.callback=[](void* p,uint32_t cam,uint32_t,uint32_t b){auto& f=*static_cast<Fixture*>(p);return f.frame.resume(f.pass,f.main,f.drawCursor,f.queue,cam,b);};return bank->draw(queue,s,drawCursor);}
};
}
int main(){Fixture f;assert(!f.numeric->camera() && !f.numeric->word(Fixture::B));assert(f.run() && f.tested);assert(!f.numeric->camera() && !f.numeric->word(Fixture::Base+8*0x400+0x11c));
 Fixture changed;changed.invalidateAtEnd=true;assert(!changed.run() && changed.tested && !changed.frame.publication().batch());return 0;}
