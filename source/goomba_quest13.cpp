#include "goomba_quest13.hpp"
#include "actor_animation.hpp"
#include <algorithm>
namespace ttyd {
namespace {bool posesReady=false;}
bool bindGoombaQuest13Poses(const ActorAnimationPack& pack) {
    posesReady=pack.pose("M_N_5B") && pack.pose("M_N_C") && pack.pose("M_S_1");return posesReady;
}
bool goombaQuest13Admitted(const FieldActorRecord& actor,const FieldActorTalkState& s,const FieldActorTalkInventory& i) {
    return posesReady && actor.room==2 && actor.key==0x2b0a9f83 && s.quest1420==13 &&
        s.flag5406==true && s.flag5407.has_value() && i.item107 && *i.item107>0;
}
bool GoombaQuest13::begin(const FieldActorRecord& actor,ProloguePouch& pouch,FieldActorTalkState& state,FieldActorTalk& talk,
                        ItemSelectionPresentation& selector,const MessageFont& font,MarioPresentation& mario,uint64_t now) {
    const auto count=pouch.keyItemCount(107);
    if(active() || talk.message.active() || selector.owner.retained() ||
       !goombaQuest13Admitted(actor,state,{{},count}) || !pouchRevisionAvailable(pouch))return false;
    for(const char* id:{"irai_13_001","irai_13_002","irai_13_003","irai_13_004","irai_13_005"})if(!talk.canPrint(actor,id,font,now))return false;
    ItemSelectionPresentation probe;if(!probe.begin(std::vector<uint16_t>(unsigned(count),107),font))return false;
    actor_=actor;pouch_=&pouch;switches_=&state;talk_=&talk;selector_=&selector;font_=&font;mario_=&mario;
    table_=std::vector<int32_t>(unsigned(count)+1);count_=count;first_=!*state.flag5407;
    if(first_) {if(!print("irai_13_001",now)){free();return false;}phase_=Phase::FirstPrint;}
    else {phase_=Phase::Update;tick(now);}
    return true;
}
bool GoombaQuest13::print(const char* key,uint64_t now) {
    actor_.yaw=talk_->actor.key==actor_.key?talk_->actor.yaw:actor_.yaw;
    // Numeric speaker0 inherits the Goomba4 type6 talk-event owner.
    return talk_->print(actor_,key,*font_,now);
}
bool GoombaQuest13::update() {
    count_=pouch_->keyItemCount(107);
    if(count_>0) {
        if(unsigned(count_)>=table_.size())return false; // Source alloc has fixed capacity; no native overflow/resize.
        std::fill_n(table_.begin(),count_,107);table_[unsigned(count_)]=-1;
    }
    return true;
}
bool GoombaQuest13::select() {
    if(!selector_->begin(std::vector<uint16_t>(unsigned(count_),107),*font_))return false;
    phase_=Phase::Selecting;return true;
}
void GoombaQuest13::free() {std::vector<int32_t>().swap(table_);phase_=Phase::Idle;}
void GoombaQuest13::tick(uint64_t now) {
    // Continue nonblocking source statements in order; only messages, selectors
    // and the authored500ms waits yield. No rendering-frame timing is used.
    while(active())switch(phase_) {
    case Phase::Idle:return;
    case Phase::FirstPrint:
        if(talk_->message.active())return;
        mario_->requestPose("M_N_5B");waitUntil_=now+500;phase_=Phase::FirstPose;return;
    case Phase::FirstPose:
        if(now<waitUntil_)return;
        mario_->requestPose("M_N_C");waitUntil_=now+500;phase_=Phase::SecondPose;return;
    case Phase::SecondPose:
        if(now<waitUntil_)return;
        mario_->requestPose("M_S_1");phase_=Phase::SecondOpen;continue;
    case Phase::SecondOpen:
        if(!print("irai_13_002",now))return;
        phase_=Phase::SecondPrint;return;
    case Phase::SecondPrint:
        if(talk_->message.active())return;
        if(!update())return;
        select();return;
    case Phase::Update:
        if(!update())return;
        if(!count_){phase_=Phase::Finish;continue;}
        if(!print("irai_13_005",now))return;
        phase_=Phase::ContinuePrint;return;
    case Phase::ContinuePrint:
        if(talk_->message.active())return;
        select();return;
    case Phase::MoreOpen:
        if(!print("irai_13_003",now))return;
        phase_=Phase::MorePrint;return;
    case Phase::MorePrint:
        if(talk_->message.active())return;
        select();return;
    case Phase::Selecting: {
        const auto result=selector_->owner.poll();if(!result || !selector_->owner.release())return;
        phase_=result==107?Phase::Remove:Phase::Finish;continue;
    }
    case Phase::Remove:
        if(!removeRevisedKeyItem(*pouch_,107))return;
        phase_=first_?Phase::AfterRemove:Phase::Update;continue;
    case Phase::AfterRemove:
        if(!update())return;
        phase_=count_?Phase::MoreOpen:Phase::Finish;continue;
    case Phase::Finish:
        if(first_)switches_->flag5407=true;
        if(pouch_->keyItemCount(107)==0) {
            if(!print("irai_13_004",now))return;
            phase_=Phase::FinalPrint;return;
        }
        free();return;
    case Phase::FinalPrint:
        if(talk_->message.active())return;
        free();return;
    }
}
}
