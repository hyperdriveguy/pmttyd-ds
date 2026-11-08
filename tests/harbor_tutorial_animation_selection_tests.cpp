#include "harbor_tutorial_animation_selection.hpp"
#include <cassert>
#include <cmath>
#include <cstdio>
using namespace ttyd;
namespace {
void samples(const ActorAnimationPose& pose) {
  ActorAnimationSample sample;
  for(float time : {pose.start,(pose.start+pose.end)*.5f,pose.end}) {
    assert(pose.evaluate(time,sample));
    assert(!sample.vertices.empty() && sample.vertices.size()<=12000);
    assert(sample.colors.empty() || sample.colors.size()==sample.vertices.size());
    for(const auto& vertex:sample.vertices)
      assert(std::isfinite(vertex.x) && std::isfinite(vertex.y) && std::isfinite(vertex.z) &&
             std::isfinite(vertex.u) && std::isfinite(vertex.v));
  }
}
}
int main() {
  ActorAnimationPack mario,combat,crump,helper;
  assert(mario.load("romfs/harbor-tutorial-mario.pmanim"));
  assert(combat.load("romfs/trio-attack-mario.pmanim"));
  assert(crump.load("romfs/harbor-tutorial-crump.pmanim"));
  assert(helper.load("romfs/harbor-tutorial-goombella.pmanim"));
  for(const char* name:{"M_S_1","M_N_2","M_D_1","M_C_3"}) {
    const auto selection=selectHarborTutorialAnimation(1,name,mario,&combat);
    assert(selection && selection->owner==HarborTutorialAnimationOwner::Dedicated);
    assert(selection->pose==mario.pose(name));samples(*selection->pose);
  }
  // Direct original Jump requests and types42/40, plus the existing guard and
  // victory clips. These remain paired with the ordinary bank's own atlas.
  for(const char* name:{"M_J_1B","M_J_1D","M_A_5","M_D_2","M_W_1","M_R_1","M_B_1","M_V_1","M_V_2"}) {
    assert(!mario.pose(name));
    const auto upper=selectHarborTutorialAnimation(1,name,mario,&combat);
    const auto lower=selectHarborTutorialAnimation(1,name,mario,&combat);
    assert(upper && lower && upper->owner==HarborTutorialAnimationOwner::OrdinaryMario);
    assert(upper->pose==combat.pose(name) && lower->pose==upper->pose);
    assert(lower->owner==upper->owner);samples(*upper->pose);
    assert(!selectHarborTutorialAnimation(1,name,mario,nullptr));
    assert(!selectHarborTutorialAnimation(0,name,crump,&combat));
    assert(!selectHarborTutorialAnimation(2,name,helper,&combat));
  }
  for(const char* name:{"S_1","R_2","R_1","A_1A","A_1B","A_1C","D_1"}) {
    auto selection=selectHarborTutorialAnimation(0,name,crump,&combat);
    assert(selection && selection->owner==HarborTutorialAnimationOwner::Dedicated);samples(*selection->pose);
  }
  for(const char* name:{"PKR_S_1","PKR_T_1"}) {
    auto selection=selectHarborTutorialAnimation(2,name,helper,&combat);
    assert(selection && selection->owner==HarborTutorialAnimationOwner::Dedicated);samples(*selection->pose);
  }
  assert(!selectHarborTutorialAnimation(1,"missing-original-clip",mario,&combat));
  assert(!selectHarborTutorialAnimation(1,nullptr,mario,&combat));
  const auto intro=selectHarborTutorialAnimation(1,"M_N_2",mario,&combat);
  assert(intro && intro->pose->end==18 && !intro->pose->loop);
  assert(299*.06f<intro->pose->end && 300*.06f>=intro->pose->end);
  std::puts("Actual original tutorial/ordinary-bank selections and all required clip samples pass");
}
