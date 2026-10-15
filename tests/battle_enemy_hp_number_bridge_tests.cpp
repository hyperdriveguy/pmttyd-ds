#include <memory>
#include "battle_enemy_hp_camera_bridge.hpp"
#include "source_icon_number.hpp"
#include "source_icon_resources.hpp"
#include "event_script_assets.hpp"
#include <cassert>
#include <cstring>
#include <iostream>
using namespace ttyd;
namespace {
// Numeric/provider integration fixture, not the normal BattleMain scheduler.
// CameraManager really publishes its boot bank through Main and dispatches the
// HP queue record through Draw. Original source icon assets and linked Number,
// Matrix and GX bodies run whole; the host sink records actual GPU requests.
struct Fixture {
 std::shared_ptr<CameraManager> cameras=std::make_shared<CameraManager>();
 CameraMainCursor main;CameraDrawCursor draw;DisplayQueue queue;
 BattleEnemyHpCameraBridge bridge;SourceIconResources resources;EventScriptAssets source;
 SourceIconNumberCursor number;SourceIconGXServices gx;SourceIconNumberServices services;
 CameraViewMatrix base{{{1,0,0,23},{0,1,0,47},{0,0,1,0}}};
 uint32_t rgba=0xffffffff;int32_t width=-1;uint64_t producer=0;
 bool current=true,hold=true,unavailable=false;unsigned viewReads=0,bindings=0,callbacks=0;
 std::vector<SourceIconGXCall> calls;std::vector<SourceIconGXQuad> quads;
 static constexpr uint32_t B=0x12340000,Base=0x60000000;
 std::optional<uint32_t> word(uint32_t a){
  if(bridge.owns(a)){++viewReads;return bridge.word(a);}
  if(auto v=resources.word(a))return v;
  uint32_t v;return source.readWord(a,v)?std::optional<uint32_t>(v):std::nullopt;
 }
 Fixture(){
  assert(resources.load("romfs/source-icons") && source.load("romfs/trio-attack-events.pmevt"));
  cameras->initializeBoot(608,480);
  BattleActorServices actors;actors.memory.user=this;
  actors.memory.word=[](void* u,uint32_t a){uint32_t v;assert(static_cast<Fixture*>(u)->source.readWord(a,v));return v;};
  assert(bridge.bind(cameras,17,actors,{Base,[](uint32_t a,uint32_t bytes){return a==Base && bytes==13*0x400;}},
      [this](BattleClassCameraEpoch e){return current && e.generation==17 && e.pass==producer;}));
  auto& i=services.icons;i.user=this;
  i.word=[](void* u,uint32_t a){return static_cast<Fixture*>(u)->word(a);};
  i.half=[](void* u,uint32_t a)->std::optional<uint16_t>{auto& f=*static_cast<Fixture*>(u);
    if(f.bridge.owns(a))return {};
    if(auto v=f.resources.half(a))return v;
    uint32_t word;if(!f.source.readWord(a&~3u,word))return {};
    return uint16_t(word>>((a&2)?0:16));};
  i.currentCamera=[](void* u){return static_cast<Fixture*>(u)->bridge.camera();};
  i.matrix=[](void* u,BattleAudienceMatrixRequest& q){return static_cast<Fixture*>(u)->bridge.matrix(q);};
  gx.user=this;gx.word=i.word;gx.half=i.half;gx.matrix=i.matrix;
  gx.call=[](void* u,const SourceIconGXCall& q)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(u);
    if(!f.bridge.available())return {};
    if(q.function==0x8000f2fc)return f.bridge.camera(q.integers[0]);
    if(q.function==0x8000f2e4)return f.bridge.camera();
    if(q.function==0x80037a80 || q.function==0x802b7544 || q.function==0x802b7880 || q.function==0x802b76c4 || q.function==0x802b76d4){
      const auto value=f.resources.request(q,&f,[](void* u,uint32_t index){auto& f=*static_cast<Fixture*>(u);const auto image=f.resources.image(index);assert(image && image->width && image->height);++f.bindings;return true;});
      if(value)f.calls.push_back(q);
      return value;
    }
    f.calls.push_back(q);return 0; // Recorded host GPU state operation.
  };
  gx.quad=[](void* u,const SourceIconGXQuad& q){auto& f=*static_cast<Fixture*>(u);
    if(!f.bridge.available() || f.hold)return false;
    f.quads.push_back(q);return true;};
  i.gx=&gx;
 }
 void produce(){
  CameraMainServices s;s.callback=[](void*,uint32_t,uint32_t,CameraState&){return true;};
  s.lookAt=[](void*,const CameraState& c){return cameraLookAtAdapter(c.eye,c.at,c.up);};
  assert(cameras->main(false,s,main));++producer;
 }
 bool run(bool enter=false){
  if(enter)assert(queue.enter(8,0,0x8011fde0,B,902)==DisplayAdmission::Registered);
  CameraDrawServices s;s.user=this;
  s.letterbox=[](void*,DisplayQueue&){return true;};
  s.request=[](void*,const CameraDrawRequest&){return true;};
  s.display.state=[](void*,const DisplayStateRequest&){return true;};
  s.display.user=this;s.display.callback=[](void* u,uint32_t camera,uint32_t callback,uint32_t battle){auto& f=*static_cast<Fixture*>(u);++f.callbacks;
    assert(camera==8 && callback==0x8011fde0 && battle==B);
    if(!f.bridge.begin({17,f.producer},f.main,f.draw,f.queue,battle)){f.unavailable=true;return false;}
    if(!resumeSourceIconNumber(f.base,59,1,f.rgba,f.services,f.number,f.width))return false;
    return f.bridge.finish({17,f.producer});};
  return cameras->draw(queue,s,draw);
 }
};
}
int main(){
 Fixture f;f.produce();assert(!f.run(true));
 assert(f.viewReads==12 && f.bindings==1 && f.quads.empty());
 const auto effects=f.calls.size();const auto reads=f.viewReads;
 assert(!f.run() && f.calls.size()==effects && f.viewReads==reads);
 f.hold=false;assert(f.run() && f.width==32 && f.quads.size()==2 && f.bindings==2 && f.viewReads==24);
 assert(!f.bridge.available() && !f.bridge.word(Fixture::Base+8*0x400+0x11c));
 unsigned loads=0;for(const auto& q:f.calls)if(q.function==0x802ba2fc){
   ++loads;const auto& view=f.cameras->camera(8)->matrices.view;
   // Source Number applies digit translation, then source Matrix concatenates
   // actual camera8 view. In this boot-bank fixture its affine basis is identity.
   assert(q.matrix[3]==view[0][0]*(loads==1?23:7)+view[0][1]*47+view[0][3]);
 }
 assert(loads==2);
 Fixture stale;stale.produce();assert(!stale.run(true));
 const auto prefix=stale.calls.size();stale.current=false;stale.hold=false;
 assert(!stale.run() && stale.unavailable && stale.calls.size()==prefix && stale.quads.empty());
 Fixture reset;reset.produce();assert(!reset.run(true));
 const auto saved=reset.calls.size();reset.queue.reinitialize();
 assert(reset.queue.enter(8,0,0x8011fde0,Fixture::B,902)==DisplayAdmission::Registered);
 reset.hold=false;assert(!reset.run() && reset.unavailable && reset.calls.size()==saved);
 std::cout<<"Actual camera publication/view words -> original Number/Matrix/GX/assets, retained quad hold and stale leases passed\n";
}
