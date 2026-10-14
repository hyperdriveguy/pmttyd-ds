#include "goomba_hand_in.hpp"
namespace ttyd {
bool goombaQuest2Admitted(const FieldActorRecord& actor,const FieldActorTalkState& s,const FieldActorTalkInventory& i) {
    return actor.room==2 && actor.key==0x2b0a9f83 && s.quest1420==2 && s.flag5405==true && i.item95 && *i.item95>=1;
}
bool GoombaHandIn::begin(const FieldActorRecord& actor,ProloguePouch& pouch,FieldActorTalkState& state,
                       FieldActorTalk& talk,ItemSelectionPresentation& selector,const MessageFont& font,uint64_t now,MarioPresentation* mario) {
    if(active())return false;
    if(mario && goombaQuest13Admitted(actor,state,{{},pouch.keyItemCount(107)}))
        return quest13_.begin(actor,pouch,state,talk,selector,font,*mario,now);
    if(active() || talk.message.active() || selector.owner.retained() ||
       !goombaQuest2Admitted(actor,state,{pouch.keyItemCount(95),{}}) || !pouchRevisionAvailable(pouch))return false;
    // Admit both original print resources and the selector before event entry.
    // A failed resource admission does not alter source switches or inventory.
    if(!talk.canPrint(actor,"irai_02_03",font,now) || !talk.canPrint(actor,"irai_02_04",font,now))return false;
    ItemSelectionPresentation probe;if(!probe.begin({95},font))return false;
    if(!talk.print(actor,"irai_02_03",font,now))return false;
    actor_=actor;pouch_=&pouch;switches_=&state;talk_=&talk;selector_=&selector;font_=&font;phase_=Phase::FirstPrint;return true;
}
void GoombaHandIn::tick(uint64_t now) {
    if(quest13_.active()){quest13_.tick(now);return;}
    switch(phase_) {
    case Phase::Idle:return;
    case Phase::FirstPrint:
        if(talk_->message.active())return;
        if(selector_->begin({95},*font_))phase_=Phase::Selecting;
        return;
    case Phase::Selecting: {
        const auto result=selector_->owner.poll();if(!result)return;
        // Read result/index before source deletion. Mode0 does not publish index.
        if(!selector_->owner.release())return;
        if(result==-1){phase_=Phase::Idle;return;}
        phase_=Phase::AcceptedOpen;tick(now);return;
    }
    case Phase::AcceptedOpen:
        actor_.yaw=talk_->actor.yaw;
        if(talk_->print(actor_,"irai_02_04",*font_,now))phase_=Phase::AcceptedPrint;
        return;
    case Phase::AcceptedPrint:
        if(talk_->message.active())return;
        // The event ignores the remove result and sets5406 even on failure.
        if(!removeRevisedKeyItem(*pouch_,95))return; // Native unknown domain never fabricates source completion.
        switches_->flag5406=true;phase_=Phase::Idle;return;
    }
}
}
