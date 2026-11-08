#include "native_harbor_gameplay.hpp"
namespace ttyd {namespace {
// Original channel4 ordering: opening1, Mario2, tutorial feedback3,
// opponent4, after-opponent5, then turn increment/reset at6.
constexpr uint32_t PlayerPhase=0x04000002,FeedbackPhase=0x04000003;
constexpr uint32_t OpponentPhase=0x04000004,ClosingPhase=0x04000005;
struct Guard{bool&v;explicit Guard(bool&x):v(x){v=true;}~Guard(){v=false;}};
}
bool NativeHarborGameplay::reset(){
 if(invoking_)return false;
 runtime_.reset();scene_=nullptr;tick_=time_=0;sampled_=false;
 stage_=Stage::Opening;round_=1;conclusion_.reset();pending_=0;return true;
}
bool NativeHarborGameplay::advance(HarborTutorial& s,uint64_t tick,uint64_t ms,uint32_t keys,const BattlePadSample* physical){
 if(invoking_||!s.running()||s.failed)return false;
 const auto owner=s.nativeGameplayLifetime().lock();if(!owner)return false;
 if(runtime_){if(scene_!=&s||runtime_!=owner)return false;}
 else{runtime_=owner;scene_=&s;}
 if(sampled_&&(tick<=tick_||ms<time_))return false;
 Guard guard(invoking_);sampled_=true;tick_=tick;time_=ms;pending_=0;
 const bool dialogueBefore=s.message.active();
 s.tick(ms,keys,physical);pending_=s.pendingService;if(s.failed)return false;
 if(stage_==Stage::Conclusion)return true;
 if(s.result){conclusion_=s.result;stage_=Stage::Conclusion;return true;}
 // No held event/child is acknowledged here. Tick owns its continuation.
 if(s.attackActive||s.damageActive||s.phaseActive||dialogueBefore||s.message.active())return true;
 auto concluded=[&](){const auto result=s.nativeGameplayConclusion();if(result){conclusion_=*result;stage_=Stage::Conclusion;return true;}return false;};
 auto phase=[&](uint32_t value,Stage next){if(s.dispatchPhase(round_,value))stage_=next;};
 switch(stage_){
 case Stage::Opening:
  if(s.introComplete){round_=s.turn;stage_=Stage::PlayerPhase;}break;
 case Stage::PlayerPhase:
  // This bounded adapter supports the actual setup2 tags, not arbitrary battles.
  if(s.actors[1].movePhase!=PlayerPhase||s.actors[0].movePhase!=OpponentPhase)return false;
  phase(PlayerPhase,Stage::PlayerPhaseWait);break;
 case Stage::PlayerPhaseWait:stage_=Stage::CommandOpen;break;
 case Stage::CommandOpen:
  if(s.openClassMenu())stage_=Stage::Command;
  else pending_=s.commandInputPending?s.commandInputPending:s.pendingService;
  break;
 case Stage::Command:{
  if(!physical)return true;
  const auto input=s.inputCommand(*physical);
  if(!input){pending_=s.commandInputPending;break;}
  if(*input==1&&s.dispatchSelectedAttack())stage_=Stage::PlayerAttack;
  break;}
 case Stage::PlayerAttack:
  if(!concluded())stage_=Stage::Feedback;
  break;
 case Stage::Feedback:phase(FeedbackPhase,Stage::FeedbackWait);break;
 case Stage::FeedbackWait:stage_=Stage::OpponentPhase;break;
 case Stage::OpponentPhase:phase(OpponentPhase,Stage::OpponentPhaseWait);break;
 case Stage::OpponentPhaseWait:stage_=Stage::OpponentAttack;break;
 case Stage::OpponentAttack:
  if(!concluded()&&s.dispatchInstalledAttack(HarborTutorial::CrumpId))stage_=Stage::OpponentAttackWait;
  break;
 case Stage::OpponentAttackWait:
  if(!concluded())stage_=Stage::ClosingPhase;
  break;
 case Stage::ClosingPhase:phase(ClosingPhase,Stage::ClosingPhaseWait);break;
 case Stage::ClosingPhaseWait:
  if(s.nativeGameplayNextTurn()){round_=s.turn;stage_=Stage::PlayerPhase;}
  break;
 case Stage::Conclusion:break;
 }
 return true;
}
}
