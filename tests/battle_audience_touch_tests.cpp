#include "battle_audience_touch.hpp"
#include <cassert>
#include <limits>
using namespace ttyd;
namespace {
RetainedIconCamera camera(){
  RetainedIconCamera c;
  c.projection={{{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}}};
  c.view={{{1,0,0,0},{0,1,0,0},{0,0,1,0}}};
  c.viewport={0,0,240,400};c.sourceAspect=400.f/240;
  return c;
}
BattleAudienceTouchQuad quad(float x=0){
  return {{{x-.2f,-2.2f,0},{x+.2f,-2.2f,0},{x+.2f,-1.8f,0},{x-.2f,-1.8f,0}}};
}
}
int main(){
  auto owner=std::make_shared<const uint8_t>(0);
  SourceIconPass building{owner,1,false},displayed{owner,1,true};
  BattleAudienceTouchTarget t{5,6,7,8};
  std::vector<BattleAudienceTouchCandidate> live{{t,true,true,true}};
  BattleAudienceTouchFrame frame;
  assert(!frame.hit(displayed,160,120,live));
  assert(frame.begin(building,camera()));
  assert(frame.append(building,t,quad()));
  assert(!frame.hit(displayed,160,120,live));
  assert(!frame.finish(building));
  assert(frame.finish(displayed));
  assert(frame.hit(displayed,160,120,live)==t);
  assert(!frame.append(building,t,quad()));
  assert(!frame.hit(displayed,210,120,live));
  assert(!frame.hit(displayed,160,145,live));
  assert(!frame.hit(displayed,320,120,live));
  assert(!frame.hit(displayed,160,240,live));
  assert(!frame.hit(displayed,-1,120,live));
  assert(!frame.hit(displayed,std::numeric_limits<float>::quiet_NaN(),120,live));
  live[0].dangerous=false;assert(!frame.hit(displayed,160,120,live));live[0].dangerous=true;
  live[0].visible=false;assert(!frame.hit(displayed,160,120,live));live[0].visible=true;
  live[0].interruptible=false;assert(!frame.hit(displayed,160,120,live));live[0].interruptible=true;
  live[0].target.itemGeneration++;assert(!frame.hit(displayed,160,120,live));live[0].target=t;
  live[0].target.actorGeneration++;assert(!frame.hit(displayed,160,120,live));live[0].target=t;
  live.push_back(live[0]);assert(!frame.hit(displayed,160,120,live));live.pop_back();
  auto otherOwner=std::make_shared<const uint8_t>(0);
  assert(!frame.hit({otherOwner,1,true},160,120,live));
  assert(!frame.hit({owner,2,true},160,120,live));
  assert(!frame.hit({owner,1,false},160,120,live));
  assert(!frame.begin(building,camera())); // Cannot replay same pass.
  building.sequence=displayed.sequence=2;
  assert(frame.begin(building,camera()));
  assert(!frame.hit({owner,1,true},160,120,live));
  assert(!frame.append({owner,1,false},t,quad())); // Stale cannot poison new build.
  assert(frame.append(building,t,quad()));
  const BattleAudienceTouchTarget second{9,10,11,12};
  assert(frame.append(building,second,quad()));
  const BattleAudienceTouchTarget unrelated{12,13,14,15};
  assert(frame.append(building,unrelated,quad(.65f)));
  assert(frame.finish(displayed));
  // Missing source evidence for overlapping geometry blocks selection.
  assert(!frame.hit(displayed,160,120,live));
  live.push_back({second,true,true,true});
  assert(!frame.hit(displayed,160,120,live)); // No guessed front actor.
  live.back().dangerous=false;assert(!frame.hit(displayed,160,120,live));
  live.back().visible=false;assert(frame.hit(displayed,160,120,live)==t);
  // Explicitly invisible only applies to the exact retained incarnation.
  live.back().target.itemGeneration++;
  assert(!frame.hit(displayed,160,120,live));
  live.back().target=second;live.back().target.actorGeneration++;
  assert(!frame.hit(displayed,160,120,live));
  live.back().target=second;
  live.push_back(live.back());
  assert(!frame.hit(displayed,160,120,live)); // Duplicated hidden identity is unknown.
  live.pop_back();
  // The absent unrelated candidate does not overlap this point.
  assert(frame.hit(displayed,160,120,live)==t);
  assert(!frame.hit(displayed,290,120,live));
  live.pop_back();
  building.sequence=displayed.sequence=3;
  auto moved=camera();moved.view[0][3]=.5f;
  assert(frame.begin(building,moved));
  assert(frame.append(building,t,quad()));assert(frame.finish(displayed));
  assert(!frame.hit(displayed,160,120,live));
  assert(frame.hit(displayed,260,120,live)==t); // Actual camera translation.
  building.sequence=displayed.sequence=4;
  assert(frame.begin(building,camera()));
  auto invalid=quad();invalid[0][0]=std::numeric_limits<float>::infinity();
  assert(!frame.append(building,t,invalid));assert(!frame.finish(displayed));
  building.sequence=displayed.sequence=5;
  assert(frame.begin(building,camera()));assert(frame.append(building,t,quad()));
  assert(!frame.append(building,t,quad()));assert(!frame.finish(displayed));
  building.sequence=displayed.sequence=6;
  auto behind=camera();behind.projection[3][3]=-1;
  assert(frame.begin(building,behind));assert(!frame.append(building,t,quad()));
  assert(!frame.finish(displayed));
  building.sequence=displayed.sequence=7;
  auto bad=camera();bad.viewport.width=0;assert(!frame.begin(building,bad));
  assert(!frame.hit(displayed,160,120,live));
  assert(frame.begin({otherOwner,1,false},camera()));
  assert(frame.finish({otherOwner,1,true})); // Empty completed crowd selects none.
  assert(!frame.hit({otherOwner,1,true},160,120,live));
  frame.reset();assert(!frame.hit(displayed,160,120,live));
}
