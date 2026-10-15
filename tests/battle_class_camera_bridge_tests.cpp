#include "battle_class_camera_bridge.hpp"
#include <algorithm>
#include <cassert>
#include <cstring>
#include <iostream>
#include <limits>
#include <map>
using namespace ttyd;
namespace {
uint32_t bits(float f){uint32_t n;std::memcpy(&n,&f,4);return n;}
struct Fixture {
 static constexpr uint32_t B=0x10000,U=0x20000,Base=0x60000000;
 std::shared_ptr<CameraManager> bank=std::make_shared<CameraManager>();
 CameraMainCursor main;CameraDrawCursor draw;DisplayQueue queue;
 BattleClassCameraBridge bridge;
 std::map<uint32_t,uint32_t> words;
 unsigned reads=0,requests=0,callbacks=0;uint32_t invalidateAt=0;
 uint64_t generation=7,producerPass=0;
 bool freeRange=true,complete=false,admit=true;
 Fixture(){
  bank->initializeBoot(608,480);
  words={{0x8041eb90,B},{B+0x420,2},{B+0x28,U}};
  words[U+0x3c]=bits(100);words[U+0x40]=bits(50);words[U+0x44]=bits(0);
  for(const auto entry:std::map<int32_t,float>{{-20220,22.5f},{-20216,105},{-20500,0},{-20212,-180},
       {-20224,4.71238899230957f},{-20240,80},{-20284,35}})
    words[0x80427440u+uint32_t(entry.first)]=bits(entry.second);
  words[0x80422244]=bits(304);words[0x80422248]=bits(240);words[0x80422240]=bits(0);
 }
 BattleActorServices memory(){BattleActorServices s;s.memory.user=this;
  s.memory.word=[](void* u,uint32_t a){auto& f=*static_cast<Fixture*>(u);++f.reads;if(a==f.invalidateAt)++f.producerPass;return f.words.at(a);};return s;}
 bool bind(){return bridge.bind(bank,generation,memory(),{Base,[this](uint32_t b,uint32_t n){assert(b==Base && n==13*0x400);return freeRange;}},
  [this](BattleClassCameraEpoch e){return admit && e.generation==generation && e.pass==producerPass;});}
 BattleClassCameraEpoch epoch()const{return {generation,producerPass};}
 void produce(){
  CameraMainServices s;
  // Test callbacks are explicit providers; the bridge itself neither installs
  // a camera callback nor invents any camera coordinates.
  s.callback=[](void*,uint32_t,uint32_t,CameraState&){return true;};
  s.lookAt=[](void*,const CameraState& c){return cameraLookAtAdapter(c.eye,c.at,c.up);};
  assert(bank->main(false,s,main));++producerPass;
 }
 bool dispatch(bool enqueue=true){
  if(enqueue)assert(queue.enter(8,0,0x8012210c,B,500)==DisplayAdmission::Registered);
  CameraDrawServices s;s.user=this;
  s.letterbox=[](void*,DisplayQueue&){return true;};
  s.request=[](void* u,const CameraDrawRequest&){++static_cast<Fixture*>(u)->requests;return true;};
  s.display.user=this;s.display.state=[](void*,const DisplayStateRequest&){return true;};
  s.display.callback=[](void* u,uint32_t camera,uint32_t callback,uint32_t battle){
   auto& f=*static_cast<Fixture*>(u);++f.callbacks;
   assert(camera==8 && callback==0x8012210c && battle==B);
   assert(f.bridge.begin(f.epoch(),f.main,f.draw,f.queue,battle));
   return f.complete && f.bridge.finish(f.epoch());
  };
  return bank->draw(queue,s,draw);
 }
 void ready(){produce();assert(bind());assert(!dispatch());assert(bridge.available());}
};
}
int main(){
 Fixture f;
 assert(f.bind());assert(!f.bridge.available());assert(!f.bridge.camera(8));
 assert(f.bridge.owns(Fixture::Base) && !f.bridge.owns(Fixture::Base-1));
 assert(!f.bridge.begin({7,1},f.main,f.draw,f.queue,Fixture::B)); // No admitted successful producer.
 f.produce();assert(!f.dispatch());
 const auto cam4=f.bridge.camera(4),cam8=f.bridge.camera(8);
 assert(cam4==Fixture::Base+4*0x400 && cam8==Fixture::Base+8*0x400 && f.bridge.camera()==cam8);
 assert(!f.bridge.camera(9));assert(!f.bridge.word(Fixture::Base));
 for(unsigned camera:{4u,8u}){
  const auto* c=f.bank->camera(camera);const auto base=Fixture::Base+camera*0x400;
  for(unsigned i=0;i<12;++i)assert(f.bridge.word(base+0x11c+4*i)==bits(c->matrices.view[i/4][i%4]));
  for(unsigned i=0;i<16;++i)assert(f.bridge.word(base+0x15c+4*i)==bits(c->matrices.projection[i/4][i%4]));
  assert(f.bridge.word(base+0x114)==bits(c->yaw));assert(!f.bridge.word(base+0x11d));
 }
 // Source units are read unchanged, including both SDK operations and the
 // original304/240 final scales. No scene fixture *100 or identity camera8.
 const auto projected=f.bridge.screenPoint(Fixture::U+0x3c);assert(projected);
 auto expected=cameraDepthProject(f.bank->camera(4)->matrices,
   cameraDepthView(f.bank->camera(4)->matrices,{100,50,0}));
 assert((*projected==BattleScreenPoint{304*expected[0],240*expected[1],0}));
 const auto center=f.bridge.ringCenter();assert(center);
 assert((*center==BattleScreenPoint{std::max(-180.f,(*projected)[0]+22.5f),(*projected)[1]+105,0}));
 const auto offset=f.bridge.ringOffset(4.71238899230957f);assert(offset && (*offset==std::array<float,2>{80,0}));
 BattleAudienceMatrixRequest copy;copy.function=0x8029dd30;copy.left={1,0,0,42,0,1,0,20,0,0,1,10};
 assert(f.bridge.matrix(copy) && copy.output==copy.left);
 const auto reads=f.reads,requests=f.requests;
 assert(f.bridge.begin(f.epoch(),f.main,f.draw,f.queue,Fixture::B));
 assert(!f.bridge.begin({7,2},f.main,f.draw,f.queue,Fixture::B));
 assert(!f.bridge.begin(f.epoch(),f.main,f.draw,f.queue,Fixture::B+4));
 assert(f.reads==reads && f.requests==requests);
 f.complete=true;assert(f.dispatch(false));assert(!f.bridge.available());
 assert(!f.bridge.begin(f.epoch(),f.main,f.draw,f.queue,Fixture::B));
 f.produce();assert(f.dispatch());assert(f.callbacks==3);
 // Each retained ownership/publication violation poisons this binding even if
 // the original scalar is restored. Explicit retire/rebind is the only reset.
 for(unsigned mutation=0;mutation<8;++mutation){
  Fixture held;held.ready();const auto before=held.reads;
  if(mutation==0)held.generation++;
  if(mutation==1)held.producerPass++;
  if(mutation==2)held.freeRange=false;
  if(mutation==3)held.draw.display.userdata++;
  if(mutation==4)held.draw.stage=9;
  if(mutation==5)held.main.active=true;
  if(mutation==6)held.bank->camera(4)->matrices.view[0][3]=1;
  if(mutation==7)held.bank->camera(8)->viewPublished=false;
  assert(!held.bridge.available());
  if(mutation==0)--held.generation;
  if(mutation==1)--held.producerPass;
  if(mutation==2)held.freeRange=true;
  if(mutation==3)--held.draw.display.userdata;
  if(mutation==4)held.draw.stage=8;
  if(mutation==5)held.main.active=false;
  if(mutation==6)held.bank->camera(4)->matrices.view[0][3]=0;
  if(mutation==7)held.bank->camera(8)->viewPublished=true;
  assert(!held.bridge.available());assert(!held.bridge.screenPoint(Fixture::U+0x3c));
  assert(held.reads==before && held.bridge.owns(Fixture::Base));
  assert(!held.bridge.finish(held.epoch()));
  held.bridge.retire();assert(!held.bridge.owns(Fixture::Base));
 }
 Fixture rejected;rejected.freeRange=false;assert(!rejected.bind());rejected.freeRange=true;
 auto missing=std::function<bool(BattleClassCameraEpoch)>{};
 assert(!rejected.bridge.bind(rejected.bank,7,rejected.memory(),{Fixture::Base,[](uint32_t,uint32_t){return true;}},missing));
 // A published boot projection is insufficient without a successful Main view.
 Fixture boot;assert(boot.bind());boot.producerPass=1;
 CameraDrawServices s;s.letterbox=[](void*,DisplayQueue&){return true;};s.request=[](void*,const CameraDrawRequest&){return true;};
 s.display.user=&boot;s.display.state=[](void*,const DisplayStateRequest&){return true;};
 s.display.callback=[](void* u,uint32_t,uint32_t,uint32_t){auto& f=*static_cast<Fixture*>(u);assert(!f.bridge.begin(f.epoch(),f.main,f.draw,f.queue,Fixture::B));return false;};
 assert(boot.queue.enter(8,0,0x8012210c,Fixture::B,500)==DisplayAdmission::Registered);
 assert(!boot.bank->draw(boot.queue,s,boot.draw));assert(!boot.bridge.available());
 Fixture finite;finite.ready();finite.words[Fixture::U+0x3c]=bits(std::numeric_limits<float>::infinity());
 assert(!finite.bridge.screenPoint(Fixture::U+0x3c));
 Fixture late;late.ready();late.invalidateAt=0x80422248;
 assert(!late.bridge.screenPoint(Fixture::U+0x3c));assert(!late.bridge.available());
 // Real camera objects stay alive while borrowed, and explicit retirement
 // permits address/context reuse with a newly validated generation.
 Fixture rebound;rebound.ready();std::weak_ptr<CameraManager> lifetime=rebound.bank;
 rebound.bank.reset();assert(!lifetime.expired());
 rebound.bridge.retire();assert(lifetime.expired());
 rebound.bank=std::make_shared<CameraManager>();rebound.bank->initializeBoot(608,480);
 ++rebound.generation;rebound.main={};rebound.draw={};rebound.queue.reinitialize();
 rebound.produce();assert(rebound.bind());assert(!rebound.dispatch());
 assert(rebound.bridge.available());
 std::cout<<"Actual CameraManager Main/Draw class bridge, source word views, projection/ring and epoch guards passed\n";
}
