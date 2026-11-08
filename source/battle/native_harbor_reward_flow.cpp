#include "native_harbor_reward_flow.hpp"
#include <utility>
namespace ttyd {
namespace {
constexpr int32_t IntroVisits=120,OrdinaryCleanupVisits=5;
struct Guard{bool&flag;explicit Guard(bool&f):flag(f){flag=true;}~Guard(){flag=false;}};
}
NativeHarborRewardFlow::NativeHarborRewardFlow(
 std::shared_ptr<NativeHarborRewardCounter>c,
 std::shared_ptr<NativeHarborRewardPresentation>p,NativeHarborRewardFlowServices s)
 :counter_(std::move(c)),presentation_(std::move(p)),services_(std::move(s)){}
bool NativeHarborRewardFlow::current()const{
 if(checking_||!services_.lifetime||!services_.current||!counter_||!presentation_||
    !services_.victoryMusic||!services_.victoryEvent||!services_.victoryEventComplete)return false;
 Guard guard(checking_);return services_.current(services_.user)&&presentation_->current();
}
bool NativeHarborRewardFlow::advance(uint64_t tick,uint32_t held){
 if(invoking_||!current())return false;
 Guard guard(invoking_);
 if(hasCompletedOuter_&&tick<completedOuter_)return false;
 if(hasCompletedOuter_&&tick==completedOuter_)return true;
 if(hasTick_&&tick<tick_)return false;
 if(hasTick_&&done_&&tick==tick_)return true;
 if(!hasTick_||done_){tick_=tick;held_=held;hasTick_=true;done_=false;step_=0;}
 // A future outer tick never replaces a held counter's original tick/input.
 if(!visit())return false;
 if(!presentation_->update(counter_->remaining(),tick_))return false;
 done_=true;hasCompletedOuter_=true;completedOuter_=tick;return true;
}
bool NativeHarborRewardFlow::visit(){
 // Each state transition is retained before the final presentation borrow.
 // No later source row runs in the same visit, except original08->09.
 if(step_==100)return true;
 switch(phase_){
 case Phase::Admission:
  if(step_==0){if(!counter_->admit())return false;step_=1;}
  if(counter_->phase()==NativeHarborRewardCounter::Phase::ZeroReward){phase_=Phase::ZeroReward;return false;}
  if(step_==1){if(!presentation_->admit(counter_->total()))return false;step_=2;}
  if(step_==2){if(!current()||!services_.victoryMusic(services_.user))return false;step_=3;}
  // Original state08 falls through09 and decrements120 in this same visit.
  wait_=IntroVisits-1;phase_=Phase::Intro;break;
 case Phase::Intro:
  if(--wait_<=0)phase_=Phase::Victory;
  break;
 case Phase::Victory:
  if(step_==0){if(!current()||!services_.victoryEvent(services_.user))return false;step_=1;}
  if(step_==1){if(!presentation_->enterPointGet())return false;step_=2;}
  if(step_==2){if(!counter_->start())return false;step_=3;}
  phase_=Phase::Counting;break;
 case Phase::Counting:
  if(step_==0){if(!counter_->advance(tick_,held_))return false;step_=1;}
  if(counter_->phase()==NativeHarborRewardCounter::Phase::LevelUp){phase_=Phase::LevelUp;break;}
  if(counter_->phase()==NativeHarborRewardCounter::Phase::Done){
   if(step_==1){if(!presentation_->hideIcons())return false;step_=2;}
   if(step_==2){if(!presentation_->softDeletePointGet())return false;step_=3;}
   wait_=OrdinaryCleanupVisits;phase_=Phase::Cleanup;
  }
  break;
 case Phase::Cleanup:
  if(step_==0){if(wait_>0)--wait_;step_=1;}
  // Native resource guard in addition to the original five-visit delay: never
  // certify finishing while the actual admitted source victory root is live.
  if(wait_<=0){if(!current()||!services_.victoryEventComplete(services_.user))return false;phase_=Phase::ReadyForFinishing;}
  break;
 case Phase::ReadyForFinishing: break; // PointGet keeps its own leave clock.
 case Phase::ZeroReward:return false;
 case Phase::LevelUp:break;
 }
 step_=100;return true;
}
}
