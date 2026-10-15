#pragma once
// Shared host/ARM development fixture. Real tutorial VM, source UI allocation,
// original TPL/font/icons and whole source draw calls. Camera4 explicitly adapts
// Renderer::worldTutorial's native fallback lens/view; this is not the original
// battle camera driver or normal BattleSequenceManager scheduling.
#include "harbor_tutorial.hpp"
#include "battle_class_frame_pipeline.hpp"
#include "battle_class_footer_backend.hpp"
#include "battle_act_ring_position.hpp"
#include "battle_screen_point.hpp"
#include "battle_audience_aux_callback.hpp"
#include "battle_audience_item_on.hpp"
#include "battle_act_menu.hpp"
#include "battle_packed_status.hpp"
#include "battle_timing_a.hpp"
#include "prologue_pouch.hpp"
#include "field_camera_events.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <map>
struct HarborClassFrameFixture {
 ttyd::HarborTutorial scene;
 ttyd::BattleClassAssets assets;ttyd::MessageFont font;ttyd::BattleClassBackend backend;
 ttyd::BattleClassFramePipeline pipeline;ttyd::HarborClassMenuDrawBinding binding;
 ttyd::BattleActorServices source,memory;ttyd::BattleClassRasterServices raster;
 ttyd::BattleClassFooterServices footer;ttyd::BattleClassFooterCursor party,pinch,button;
 // Optional actual retained footer raster owner. Install before starting a
 // pass; retain its context/resource generation through all source waits.
 void* footerRasterUser=nullptr;
 std::optional<uint32_t> (*footerRaster)(void*,const ttyd::BattleClassRasterRequest&)=nullptr;
 ttyd::BattleRandom random{1};unsigned rngCalls=0;uint64_t sequence=0;
 std::map<uint32_t,uint32_t> camera;std::array<uint32_t,4> scratch{};
 static constexpr uint32_t Cam4=0x87500400,Cam8=0x87500800,Stack=0x87600000;
 static uint32_t bits(float f){uint32_t n;std::memcpy(&n,&f,4);return n;}
 static float scalar(uint32_t n){float f;std::memcpy(&f,&n,4);return f;}
 void cameraFixture(){
  const float length=std::sqrt(180.f*180+590.f*590),uy=590/length,uz=-180/length;
  const float v[]{1,0,0,0,0,uy,uz,-uy*280-uz*550,0,-uz,uy,uz*280-uy*550};
  for(unsigned i=0;i<12;++i){camera[Cam4+0x11c+i*4]=bits(v[i]);camera[Cam8+0x11c+i*4]=bits(i==0 || i==5 || i==10?1.f:0.f);}
  const float y=1/std::tan(42.f*3.14159265358979323846f/360),x=y/(608.f/480);
  const float p[]{x,0,0,0,0,y,0,0,0,0,-8000.f/7990,-80000.f/7990,0,0,-1,0};
  for(unsigned i=0;i<16;++i)camera[Cam4+0x15c+i*4]=bits(p[i]);
 }
 uint32_t word(uint32_t a)const{
  const auto found=camera.find(a);if(found!=camera.end())return found->second;
  if(a>=Stack && a<Stack+16){assert(!(a&3));return scratch[(a-Stack)/4];}
  return source.memory.word(source.memory.user,a);
 }
 void word(uint32_t a,uint32_t n){
  if(a>=Stack && a<Stack+16){assert(!(a&3));scratch[(a-Stack)/4]=n;return;}
  assert(!camera.count(a));source.memory.storeWord(source.memory.user,a,n);
 }
 std::optional<ttyd::BattleActMatrix> matrix(ttyd::BattleAudienceMatrixRequest q){
  ttyd::BattleAudienceAuxServices s;ttyd::bindAudienceAuxMathAdapter(s);
  return s.matrix(nullptr,q)?std::optional<ttyd::BattleActMatrix>(q.output):std::nullopt;
 }
 std::optional<std::array<float,3>> project(uint32_t position){
  ttyd::BattleScreenPointServices s;s.memory=memory.memory;s.user=this;
  s.camera=[](void*,uint32_t id)->std::optional<uint32_t>{return id==4?std::optional<uint32_t>(Cam4):std::nullopt;};
  s.view=[](void* u,uint32_t a,const ttyd::BattleScreenPoint& in,ttyd::BattleScreenPoint& out){
   auto& f=*static_cast<HarborClassFrameFixture*>(u);ttyd::BattleScreenPoint v;
   for(unsigned i=0;i<3;++i){v[i]=scalar(f.word(a+i*16))*in[0]+scalar(f.word(a+i*16+4))*in[1]+scalar(f.word(a+i*16+8))*in[2]+scalar(f.word(a+i*16+12));}
   out=v;return true;
  };
  s.project=[](void* u,uint32_t a,const ttyd::BattleScreenPoint& in,ttyd::BattleScreenPoint& out){
   auto& f=*static_cast<HarborClassFrameFixture*>(u);std::array<float,4> v;
   for(unsigned i=0;i<4;++i)v[i]=scalar(f.word(a+i*16))*in[0]+scalar(f.word(a+i*16+4))*in[1]+scalar(f.word(a+i*16+8))*in[2]+scalar(f.word(a+i*16+12));
   if(!v[3])return false;
   for(unsigned i=0;i<3;++i){out[i]=v[i]/v[3];}
   return true;
  };
  ttyd::BattleScreenPoint out;return ttyd::packedBattleScreenPoint(position,out,s)?std::optional<ttyd::BattleScreenPoint>(out):std::nullopt;
 }
 ttyd::BattleActRingPositionServices ring(){
  ttyd::BattleActRingPositionServices s;s.memory=memory;s.user=this;
  s.project=[](void* u,uint32_t a){return static_cast<HarborClassFrameFixture*>(u)->project(a);};
  s.sine=[](void*,double a)->std::optional<double>{return std::sin(a);};s.cosine=[](void*,double a)->std::optional<double>{return std::cos(a);};return s;
 }
 bool prepare(const char* root,const ttyd::BattleClassAssets* admitted=nullptr){
  const std::string base=root;
  if(!font.load((base+"/font.pmfont").c_str()))return false;
  if(!admitted){if(!assets.load((base+"/battle-class-surfaces.bin").c_str(),(base+"/battle-class-icons.bin").c_str()))return false;admitted=&assets;}
  // Match actual field entry: carry the original new-game pouch (including
  // its original boots/hammer key items) through the existing import owner.
  // Bare beginFresh intentionally lacks that prologue equipment context.
  const auto pouch=ttyd::initializeProloguePouch("Yoshi");
  if(!scene.beginFromPouch(0,pouch,4))return false;
  for(unsigned n=0;n<1500 && !scene.introComplete;++n){if(scene.pendingService==0x80106980)scene.animationComplete(1);scene.tick((n+1)*1000/60,n%4==0?0x100:0);}
  if(!scene.introComplete || !scene.initializeClassMenuResources() || !scene.openClassMenu())return false;
  auto initial=scene.classMenuDrawBinding({});if(!initial)return false;source=initial->display.draw.memory;memory=source;cameraFixture();
  memory.memory.user=this;memory.memory.word=[](void* u,uint32_t a){return static_cast<HarborClassFrameFixture*>(u)->word(a);};
  memory.memory.storeWord=[](void* u,uint32_t a,uint32_t n){static_cast<HarborClassFrameFixture*>(u)->word(a,n);};
  memory.memory.half=[](void* u,uint32_t a){const auto& s=static_cast<HarborClassFrameFixture*>(u)->source.memory;return s.half(s.user,a);};
  memory.memory.storeHalf=[](void* u,uint32_t a,uint16_t n){const auto& s=static_cast<HarborClassFrameFixture*>(u)->source.memory;s.storeHalf(s.user,a,n);};
  memory.memory.storeByte=[](void* u,uint32_t a,uint8_t n){const auto& s=static_cast<HarborClassFrameFixture*>(u)->source.memory;s.storeByte(s.user,a,n);};
  memory.memory.flushBlock=[](void* u,uint32_t a){const auto& s=static_cast<HarborClassFrameFixture*>(u)->source.memory;assert(s.flushBlock);s.flushBlock(s.user,a);};
  memory.memory.systemCall=[](void* u){const auto& s=static_cast<HarborClassFrameFixture*>(u)->source.memory;assert(s.systemCall);s.systemCall(s.user);};
  backend.assets=admitted;backend.font=&font;
  backend.label=[this](uint32_t a)->const char*{for(const auto& row:scene.classRows)if(row.nameAddress==a)return row.label.c_str();return nullptr;};
  backend.random=[this](uint32_t bound){++rngCalls;return uint32_t(random.bounded(int32_t(bound)));};
  ttyd::BattleActDrawServices linked;linked.memory=memory;linked.user=this;
  linked.camera=[](void* u,uint32_t id){return id==8 && static_cast<HarborClassFrameFixture*>(u)->camera.count(Cam8+0x11c)!=0;};
  linked.center=[](void* u)->std::optional<std::array<float,3>>{auto& f=*static_cast<HarborClassFrameFixture*>(u);std::array<float,3> out;return ttyd::packedBattleActRingCenter(out,f.ring())?std::optional<std::array<float,3>>(out):std::nullopt;};
  linked.offset=[](void* u,float a)->std::optional<std::array<float,2>>{auto& f=*static_cast<HarborClassFrameFixture*>(u);std::array<float,2> out;return ttyd::packedBattleActRingOffset(a,out,f.ring())?std::optional<std::array<float,2>>(out):std::nullopt;};
  linked.interpolate=[](void*,int32_t m,int32_t i,int32_t n,float a,float b){return ttyd::cameraInterpolation(m,i,n,a,b);};
  linked.translation=[](void* u,const std::array<float,3>& xyz){ttyd::BattleAudienceMatrixRequest q;q.function=0x8029e230;q.xyz=xyz;return static_cast<HarborClassFrameFixture*>(u)->matrix(q);};
  linked.rotation=[](void* u,float a,uint32_t axis){ttyd::BattleAudienceMatrixRequest q;q.function=0x8029dff0;q.radians=a;q.axis=axis;return static_cast<HarborClassFrameFixture*>(u)->matrix(q);};
  linked.scale=[](void* u,float x,float y,float z){ttyd::BattleAudienceMatrixRequest q;q.function=0x8029e2b0;q.xyz={x,y,z};return static_cast<HarborClassFrameFixture*>(u)->matrix(q);};
  linked.concat=[](void* u,const ttyd::BattleActMatrix& a,const ttyd::BattleActMatrix& b){ttyd::BattleAudienceMatrixRequest q;q.function=0x8029dd64;q.left=a;q.right=b;return static_cast<HarborClassFrameFixture*>(u)->matrix(q);};
  linked.partyButton=[](void* u){auto& f=*static_cast<HarborClassFrameFixture*>(u);return ttyd::resumeBattleClassPartyFooter(f.footer,f.party);};
  linked.pinchMark=[](void* u){auto& f=*static_cast<HarborClassFrameFixture*>(u);return ttyd::resumeBattleClassPinchFooter(f.footer,f.pinch);};
  raster.memory=memory;raster.math=linked;raster.user=this;
  raster.request=[](void* u,const ttyd::BattleClassRasterRequest& q)->std::optional<uint32_t>{
   auto& f=*static_cast<HarborClassFrameFixture*>(u);
   if(f.footerRaster)return f.footerRaster(f.footerRasterUser,q);
   if(q.function==ttyd::BattleClassRasterService::Camera)return q.integers==std::vector<uint32_t>{8}?std::optional<uint32_t>(Cam8):std::nullopt;
   return f.backend.request(q);
  };
  raster.truncate=[](void*,float x)->std::optional<int32_t>{if(!std::isfinite(x) || double(x)<-2147483648. || double(x)>=2147483648.)return {};return int32_t(x);};
  raster.identity=[](void*)->std::optional<ttyd::BattleActMatrix>{ttyd::BattleActMatrix m{};m[0]=m[5]=m[10]=1;return m;};
  footer.raster=raster;footer.user=this;
  footer.project=[](void* u,uint32_t a){return static_cast<HarborClassFrameFixture*>(u)->project(a);};
  footer.audienceItem=[](void* u,uint32_t)->std::optional<int32_t>{auto& f=*static_cast<HarborClassFrameFixture*>(u);ttyd::packedBattleAudienceItemOn({Stack,Stack+4,Stack+8,Stack+12,0},f.memory);return int32_t(f.scratch[0]);};
  footer.changePosition=[](void* u,uint32_t b)->std::optional<bool>{auto& f=*static_cast<HarborClassFrameFixture*>(u);ttyd::BattleActMenuServices s;s.memory=f.memory;s.user=&f;
   s.request=[](void* v,const ttyd::BattleActRequest& q)->std::optional<uint32_t>{auto& f=*static_cast<HarborClassFrameFixture*>(v);using F=ttyd::BattleActService;if(q.function==F::Mario)return ttyd::battleMarioUnit(q.arguments[0],f.memory);if(q.function==F::Party)return ttyd::battlePartyUnit(q.arguments[0],f.memory);if(q.function==F::Status)return ttyd::checkPackedBattleStatus(q.arguments[0],int32_t(q.arguments[1]),f.memory)?1:0;return {};};
   const auto allowed=ttyd::checkPackedBattleChangePosition(b,s);return allowed?std::optional<bool>(*allowed!=0):std::nullopt;};
  footer.button=[](void* u,int32_t t,float x,float y,float a){auto& f=*static_cast<HarborClassFrameFixture*>(u);return ttyd::resumeBattleClassFooterButton(t,x,y,a,f.footer,f.button);};
  // No audio or linked TexturePlane2 owner in this fixture: a branch requiring
  // them holds honestly. Healthy solo/no-item source data needs neither.
  const auto actual=scene.classMenuDrawBinding(linked);if(!actual)return false;binding=*actual;binding.display.draw.memory=memory;
  return pipeline.bind(binding.owner,binding.allocation,binding.display,raster,backend,binding.current);
 }
 bool advance(){++sequence;return pipeline.resume({binding.owner,binding.allocation,sequence},8,0x84000000);}
};
