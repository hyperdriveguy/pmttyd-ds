#include <memory>
#include "battle_weapon_native_numeric.hpp"
#include "fixtures/battle_weapon_source_draw_fixture.hpp"
#include "battle_audience_aux_callback.hpp"
#include "field_camera_events.hpp"
#include <limits>
using namespace ttyd;
namespace {
struct Fixture {
 WeaponSourceFixture source;std::shared_ptr<CameraManager>bank=std::make_shared<CameraManager>();
 std::shared_ptr<const uint8_t>token=std::make_shared<const uint8_t>(0);
 std::shared_ptr<const int>resource=std::make_shared<const int>(1),batch=std::make_shared<const int>(2);
 CameraMainCursor main;CameraDrawCursor draw;DisplayQueue queue;BattleWeaponNativeFrame frame;
 std::unique_ptr<BattleWeaponNativeNumeric>numeric;uint64_t pass=0;unsigned reads=0,checks=0;
 bool live=true,mutate=false,lateInvalidate=false;
 static constexpr uint32_t Base=0x60014000;
 Fixture(){
  bank->initializeBoot(608,480);
  numeric=std::make_unique<BattleWeaponNativeNumeric>(frame,source.actors(),Base,
   [this](uint32_t a)->std::optional<uint32_t>{++reads;if(mutate)++pass;const auto i=source.words.find(a);return i==source.words.end()?std::nullopt:std::optional<uint32_t>(i->second);},[this]{return live;});
  BattleWeaponNativeFrameBinding b;b.allocationOwner=token;b.sourceResourceOwner=resource;
  b.allocation=WeaponSourceFixture::W;b.battle=WeaponSourceFixture::B;b.generation=17;b.cameras=bank;b.actors=source.actors();
  b.addresses={Base,[](uint32_t,uint32_t){return true;}};b.allocationCurrent=[this]{return live;};b.sourceResourceCurrent=[this]{return live;};b.producerCurrent=[this](BattleClassCameraEpoch e){return e.generation==17&&e.pass==pass;};
  BattleWeaponNativeSubmission s;s.user=this;
  s.begin=[](void*p,uint64_t){return static_cast<Fixture*>(p)->check();};
  s.seal=[](void*p,uint64_t n)->std::shared_ptr<const void>{auto&f=*static_cast<Fixture*>(p);assert(f.frame.sourceComplete(n));return f.batch;};
  s.submit=[](void*,uint64_t,const std::shared_ptr<const void>&){return true;};
  s.current=[](void*p,const std::shared_ptr<const void>&b){auto&f=*static_cast<Fixture*>(p);return f.live&&b==f.batch;};
  assert(frame.bind(std::move(b),resource,source.services(),s));
 }
 bool check(){
  ++checks;auto&a=*numeric;const auto d=a.draw();const auto r=a.raster();const auto w=a.weapon();
  assert(w.current(w.user)&&!w.request&&!d.icons&&!d.partyButton&&!d.pinchMark);
  assert(a.camera()==Base+8*0x400&&a.camera(4)==Base+4*0x400&&!a.camera(7));
  const auto before=reads;assert(!a.word(Base)&&!a.word(Base+8*0x400+0x11d)&&reads==before);
  assert(a.word(WeaponSourceFixture::B+0x1c78)==WeaponSourceFixture::W&&!a.word(0x77777777)&&reads==before+2);
  assert(d.center(d.user)&&d.offset(d.user,0));
  for(int m:{0,4})for(int step:{-2,0,3,12,15})assert(d.interpolate(d.user,m,step,12,-20,80)==cameraInterpolation(m,step,12,-20,80));
  assert(!d.interpolate(d.user,99,1,2,0,1));assert(!d.interpolate(d.user,0,1,1,0,std::numeric_limits<float>::infinity()));
  BattleAudienceAuxServices sdk;bindAudienceAuxMathAdapter(sdk);BattleAudienceMatrixRequest q;q.function=0x8029e230;q.xyz={4,5,6};assert(sdk.matrix(nullptr,q));const auto translated=d.translation(d.user,q.xyz);assert(translated==q.output);
  q.function=0x8029dff0;q.radians=.3f;q.axis=120;assert(sdk.matrix(nullptr,q));const auto rotated=d.rotation(d.user,q.radians,q.axis);assert(rotated==q.output&&!d.rotation(d.user,1,999));
  q.function=0x8029e2b0;q.xyz={2,3,4};assert(sdk.matrix(nullptr,q));assert(d.scale(d.user,2,3,4)==q.output);
  q.function=0x8029dd64;q.left=*translated;q.right=*rotated;assert(sdk.matrix(nullptr,q));assert(d.concat(d.user,q.left,q.right)==q.output);
  q.function=0x8029dd04;assert(sdk.matrix(nullptr,q));assert(a.identity()==q.output&&r.identity(r.user)==q.output);
  assert(a.truncate(3.9f)==3&&a.truncate(-3.9f)==-3&&a.truncate(-2147483648.f)==INT32_MIN);
  for(float v:{std::numeric_limits<float>::quiet_NaN(),std::numeric_limits<float>::infinity(),2147483648.f})assert(!a.truncate(v));
  const float x=1.00000011920928955078125f,y=.99999988079071044921875f;assert(a.fma(x,y,-1)==std::fma(x,y,-1));
  assert(!a.fma(std::numeric_limits<float>::max(),2,0));
  BattleClassRasterRequest camera{BattleClassRasterService::Camera,{8},{},{}};const auto at=r.request(r.user,camera);assert(at);
  for(unsigned i=0;i<12;++i)assert(r.memory.memory.word(r.memory.memory.user,*at+0x11c+4*i)==WeaponSourceFixture::bits(bank->camera(8)->matrices.view[i/4][i%4]));
  assert(r.memory.memory.word(r.memory.memory.user,WeaponSourceFixture::B+0x1c78)==WeaponSourceFixture::W);
  camera.integers={4};assert(!r.request(r.user,camera));camera={BattleClassRasterService::Init2D,{},{},{}};assert(!r.request(r.user,camera));
  BattleWeaponNativeNumeric wrong(frame,source.actors(),Base+4,[](uint32_t)->std::optional<uint32_t>{assert(false);return {};},[]{return true;});assert(!wrong.camera()&&!wrong.word(Base+8*0x400+0x11c));
  if(lateInvalidate){mutate=true;assert(!a.word(WeaponSourceFixture::B+0x1c78));assert(!a.camera()&&!a.identity()&&!a.fma(1,2,3));return false;}
  return true;
 }
 bool run(uint32_t identity=0x80121ce8){CameraMainServices m;m.callback=[](void*,uint32_t,uint32_t,CameraState&){return true;};m.lookAt=[](void*,const CameraState&c){return cameraLookAtAdapter(c.eye,c.at,c.up);};assert(bank->main(false,m,main));++pass;
  assert(queue.enter(8,0,identity,WeaponSourceFixture::B,500)==DisplayAdmission::Registered);CameraDrawServices s;s.user=this;s.letterbox=[](void*,DisplayQueue&){return true;};s.request=[](void*,const CameraDrawRequest&){return true;};s.display.user=this;s.display.state=[](void*,const DisplayStateRequest&){return true;};s.display.callback=[](void*p,uint32_t c,uint32_t,uint32_t b){auto&f=*static_cast<Fixture*>(p);return f.frame.resume(f.pass,f.main,f.draw,f.queue,c,b);};return bank->draw(queue,s,draw);}
};
}
int main(){Fixture f;assert(!f.numeric->camera()&&!f.numeric->word(WeaponSourceFixture::B));assert(f.run()&&f.checks==1&&f.frame.publication());assert(!f.numeric->camera()&&!f.numeric->word(Fixture::Base));Fixture stale;stale.lateInvalidate=true;assert(!stale.run()&&!stale.frame.publication());Fixture wrong;assert(!wrong.run(0x8012210c)&&!wrong.checks);return 0;}
