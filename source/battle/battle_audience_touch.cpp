#include "battle_audience_touch.hpp"
#include "battle_scene_views.hpp"
#include <cmath>
namespace ttyd {
namespace {
bool same(const SourceIconPass& a,const SourceIconPass& b) {
  return a.owner && a.owner==b.owner && a.sequence && a.sequence==b.sequence;
}
bool valid(const BattleAudienceTouchTarget& t) {
  return t.slot<200 && t.item<100 && t.actorGeneration && t.itemGeneration;
}
float cross(const std::array<float,2>& a,const std::array<float,2>& b,float x,float y) {
  return (b[0]-a[0])*(y-a[1])-(b[1]-a[1])*(x-a[0]);
}
bool triangle(const std::array<float,2>& a,const std::array<float,2>& b,
              const std::array<float,2>& c,float x,float y) {
  const float area=cross(a,b,c[0],c[1]);
  if(!std::isfinite(area) || area==0)return false;
  const float ab=cross(a,b,x,y),bc=cross(b,c,x,y),ca=cross(c,a,x,y);
  return area>0?(ab>=0 && bc>=0 && ca>=0):(ab<=0 && bc<=0 && ca<=0);
}
}
bool BattleAudienceTouchTarget::operator==(const BattleAudienceTouchTarget& t)const {
  return slot==t.slot && item==t.item && actorGeneration==t.actorGeneration && itemGeneration==t.itemGeneration;
}
void BattleAudienceTouchFrame::reset(){pass_={};quads_.clear();complete_=false;failed_=false;lower_={};}
bool BattleAudienceTouchFrame::begin(const SourceIconPass& p,const RetainedIconCamera& camera) {
  // A stale pass cannot replace a newer pass from the same lifetime.
  if(!p.owner || !p.sequence || p.complete ||
     (pass_.owner==p.owner && p.sequence<=pass_.sequence))return false;
  reset();
  const auto& r=camera.viewport;
  if(!r.width || !r.height || r.x>240 || r.y>400 || r.width>240-r.x || r.height>400-r.y ||
     !std::isfinite(camera.sourceAspect) || camera.sourceAspect<=0)return false;
  for(const auto& row:camera.projection)for(float v:row)if(!std::isfinite(v))return false;
  for(const auto& row:camera.view)for(float v:row)if(!std::isfinite(v))return false;
  lower_=lowerBattleSceneMatrix(iconSceneWorldTransform(camera));
  for(const auto& row:lower_)for(float v:row)if(!std::isfinite(v)){reset();return false;}
  pass_=p;return true;
}
bool BattleAudienceTouchFrame::append(const SourceIconPass& p,const BattleAudienceTouchTarget& t,
                                    const BattleAudienceTouchQuad& world) {
  if(!same(pass_,p) || p.complete || complete_)return false;
  const auto reject=[&](){failed_=true;return false;};
  if(failed_ || !valid(t))return reject();
  for(const auto& q:quads_)if(q.target.slot==t.slot)return reject();
  Quad q;q.target=t;
  for(unsigned i=0;i<4;++i){
    std::array<float,4> clip{};
    for(unsigned row=0;row<4;++row){
      clip[row]=lower_[row][3];
      for(unsigned col=0;col<3;++col){
        if(!std::isfinite(world[i][col]))return reject();
        clip[row]+=lower_[row][col]*(world[i][col]*.01f);
      }
      if(!std::isfinite(clip[row]))return reject();
    }
    // Conservative admission at the eye/clip boundary. The producer owns
    // actual raster visibility/occlusion; this helper invents no depth policy.
    if(clip[3]<=0)return reject();
    q.pixels[i]={(1-clip[1]/clip[3])*160,(1-clip[0]/clip[3])*120};
    for(float v:q.pixels[i])if(!std::isfinite(v))return reject();
  }
  quads_.push_back(q);return true;
}
bool BattleAudienceTouchFrame::finish(const SourceIconPass& p) {
  if(!same(pass_,p) || !p.complete || complete_ || failed_)return false;
  complete_=true;return true;
}
std::optional<BattleAudienceTouchTarget> BattleAudienceTouchFrame::hit(
    const SourceIconPass& p,float x,float y,const std::vector<BattleAudienceTouchCandidate>& live)const {
  if(!complete_ || !p.complete || !same(pass_,p) || !std::isfinite(x) || !std::isfinite(y) ||
     x<0 || x>=320 || y<0 || y>=240)return {};
  std::optional<BattleAudienceTouchTarget> result;
  for(const auto& q:quads_){
    if(!triangle(q.pixels[0],q.pixels[1],q.pixels[2],x,y) &&
       !triangle(q.pixels[0],q.pixels[2],q.pixels[3],x,y))continue;
    // Uncertain source identity cannot establish that a published occupant is
    // hidden. Only one exact, explicitly invisible incarnation permits a tap
    // through its geometry. Unrelated geometry never blocks this point.
    const BattleAudienceTouchCandidate* current=nullptr;
    unsigned matches=0;
    for(const auto& c:live)if(c.target.slot==q.target.slot){
      ++matches;current=&c;
    }
    if(matches!=1 || !(current->target==q.target))return {};
    if(!current->visible)continue;
    const bool eligible=current->dangerous && current->interruptible;
    // Without retained per-pixel depth/alpha, a second visible actor is
    // ambiguous even when that actor carries a harmless item.
    if(!eligible || result)return {};
    result=q.target;
  }
  return result;
}
} // namespace ttyd
