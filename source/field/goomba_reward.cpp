#include "goomba_reward.hpp"
namespace ttyd {
bool goombaQuest2RewardAdmitted(const FieldActorRecord& actor,const FieldActorTalkState& state) {
 return actor.room==0 && actor.key==0x42ff5f1f && state.quest1420==2 && state.flag5406==true;
}
bool GoombaReward::begin(const FieldActorRecord& actor,FieldActorTalkState& state,
 FieldActorTalk& talk,const MessageFont& font,const GoombaRewardServices& services,uint64_t now) {
 if(active() || talk.message.active() || !goombaQuest2RewardAdmitted(actor,state) ||
    !services.ready || !services.coin || !services.setFlag || !services.ready(services.user) ||
    !talk.canPrint(actor,"irai_02_02",font,now) || !talk.canPrint(actor,"sys_request_finish",font,now))return false;
 if(!talk.print(actor,"irai_02_02",font,now))return false;
 owner_=this;switches_=&state;talk_=&talk;font_=&font;actor_=actor;services_=services;
 firstCoinCall_=true;phase_=Phase::Printing;return true;
}
void GoombaReward::eventMain(uint64_t now) {
 if(!canAdvance())return;
 if(phase_==Phase::Printing) {
  if(talk_->message.active())return;
  phase_=Phase::Coin;
 }
 if(phase_==Phase::Coin) {
  const auto result=services_.coin(services_.user,20,firstCoinCall_);
  if(!result)return; // Missing native provider is not a source completion.
  firstCoinCall_=false;
  if(*result!=2)return;
  switches_->quest1420=0;
  services_.setFlag(services_.user,5376);
  phase_=Phase::FinishOpen;
 }
 if(phase_==Phase::FinishOpen) {
  auto speaker=actor_;speaker.key=0; // Original final print's fourth operand is0, not me.
  if(!talk_->print(speaker,"sys_request_finish",*font_,now))return;
  phase_=Phase::FinishPrint;
 }
 if(phase_==Phase::FinishPrint && !talk_->message.active())phase_=Phase::Complete;
}
}
