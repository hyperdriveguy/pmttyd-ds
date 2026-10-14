#include "retained_icon_frame.hpp"
#include "battle_scene_views.hpp"
#include <cassert>
#include <cmath>
#include <limits>
using namespace ttyd;
namespace {
CameraProjectionMatrix identity(){CameraProjectionMatrix m{};for(unsigned i=0;i<4;++i)m[i][i]=1;return m;}
void close(float a,float b){assert(std::abs(a-b)<.0001f);}
std::array<float,4> point(const CameraProjectionMatrix& m,std::array<float,4> p){
  std::array<float,4> r{};for(unsigned i=0;i<4;++i)for(unsigned j=0;j<4;++j)r[i]+=m[i][j]*p[j];return r;
}
RetainedIconPacket packet(uint32_t image,uint32_t camera=4){
  RetainedIconPacket p;p.image=image;p.camera=camera;p.transform=identity();
  p.viewport=p.scissor={0,0,240,400};p.pipeline.color=true;return p;
}
}
int main(){
  RetainedIconFrames frames;SourceIconPass pass{std::make_shared<const uint8_t>(0),1,false};
  auto q=packet(7);assert(!frames.append(pass,q));frames.observe(pass);
  RetainedIconCamera c;c.projection=identity();c.view={{{0,1,0,17},{-1,0,0,-9},{0,0,1,5}}};
  c.sourceAspect=608.f/480.f;c.viewport=q.viewport;assert(frames.camera(pass,c));
  q.quad.positions[0]={13,22,31};q.material.tint=0x12345678;
  assert(frames.append(pass,q));q.quad.positions[0][0]=99;q.material.tint=0;
  assert(frames.retains(7) && frames.published().empty());
  pass.complete=true;frames.observe(pass);assert(frames.published().size()==1);
  assert(frames.published()[0].quad.positions[0][0]==13 && frames.published()[0].material.tint==0x12345678);
  assert(!frames.append(pass,q));assert(frames.sceneCamera()->view==c.view);
  frames.observe(pass);assert(frames.published().size()==1); // completion idempotent
  auto stale=pass;pass.sequence=2;pass.complete=false;frames.observe(pass);
  assert(!frames.append(pass,packet(10)));assert(frames.camera(pass,c));
  for(unsigned i=0;i<200;++i)assert(frames.append(pass,packet(i+10,i<100?4:8)));
  assert(frames.published().size()==1 && frames.retains(7) && frames.retains(209));
  frames.observe(stale);assert(!frames.append(stale,q));assert(frames.retains(209));
  auto bad=packet(700);bad.camera=3;assert(!frames.append(pass,bad));bad=packet(700);
  bad.transform[0][0]=std::numeric_limits<float>::quiet_NaN();assert(!frames.append(pass,bad));
  bad=packet(700);bad.scissor={230,0,20,400};assert(!frames.append(pass,bad));
  bad=packet(700);bad.pipeline.beforeAlpha=bad.pipeline.alphaReject=bad.pipeline.depth=bad.pipeline.depthWrite=true;
  assert(!frames.append(pass,bad));assert(!frames.retains(700));
  pass.complete=true;frames.observe(pass);assert(frames.published().size()==200 && !frames.retains(7));
  assert(frames.sceneCamera());
  ++pass.sequence;pass.complete=false;frames.observe(pass);assert(frames.published().size()==200);
  pass.complete=true;frames.observe(pass);assert(frames.published().empty() && !frames.retains(10));
  pass.owner=std::make_shared<const uint8_t>(0);pass.sequence=1;pass.complete=false;
  frames.observe(pass);assert(!frames.append(pass,packet(99)));assert(frames.camera(pass,c));assert(frames.append(pass,packet(99)));assert(frames.retains(99));
  frames.observe({});assert(!frames.retains(99));
  // Actual published rolled/translated view agrees for source icon positions
  // and independently scaled native world vertices, including partial viewport.
  c.viewport={30,40,180,300};
  const auto source=iconViewportTransform(iconSceneTransform(nativeCameraTransform(c.projection,c.view),c),c.viewport);
  const auto world=iconSceneWorldTransform(c);
  const auto a=point(source,{40,70,-80,1}),b=point(world,{.4f,.7f,-.8f,1});
  for(unsigned i=0;i<4;++i)close(a[i],b[i]);
  const auto lower=lowerBattleSceneMatrix(source);const auto x=point(lower,{40,70,-80,1});
  const auto expected=lowerBattleSceneClip(a);for(unsigned i=0;i<4;++i)close(x[i],expected[i]);
  auto viewport=iconViewportTransform(identity(),{30,40,180,300});
  auto lo=point(viewport,{-1,-1,0,1}),hi=point(viewport,{1,1,0,1});
  close(lo[0],-.75f);close(hi[0],.75f);close(lo[1],-.8f);close(hi[1],.7f);
  c.viewport={0,0,240,400};auto aspect=iconSceneTransform(identity(),c);
  close(aspect[0][0],1);close(aspect[1][1],(608.f/480.f)/(400.f/240.f));
}
