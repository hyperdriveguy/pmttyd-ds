#pragma once
#include "pouch_revision.hpp"
#include "field_actors.hpp"
#include "item_selection_presentation.hpp"
namespace ttyd {
// Original Goomba4 quest2 event owner. Simulation ticks/messages advance this
// graph; rendering never advances it. Other quests retain their own graphs.
class GoombaHandIn {
public:
    bool begin(const FieldActorRecord&,ProloguePouch&,FieldActorTalkState&,
               FieldActorTalk&,ItemSelectionPresentation&,const MessageFont&,uint64_t now);
    void tick(uint64_t now);
    bool active() const {return phase_!=Phase::Idle;}
    uint8_t keyDepth() const {return active()?uint8_t(phase_==Phase::Selecting?2:1):0;}
private:
    enum class Phase {Idle,FirstPrint,Selecting,AcceptedOpen,AcceptedPrint};
    Phase phase_=Phase::Idle;
    ProloguePouch* pouch_=nullptr;
    FieldActorTalkState* switches_=nullptr;
    FieldActorTalk* talk_=nullptr;
    ItemSelectionPresentation* selector_=nullptr;
    const MessageFont* font_=nullptr;
    FieldActorRecord actor_;
};
bool goombaQuest2Admitted(const FieldActorRecord&,const FieldActorTalkState&,
                         const FieldActorTalkInventory&);
}
