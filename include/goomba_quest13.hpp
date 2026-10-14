#pragma once
#include "pouch_revision.hpp"
#include "field_actors.hpp"
#include "item_selection_presentation.hpp"
#include "mario_presentation.hpp"
namespace ttyd {
class ActorAnimationPack;
bool bindGoombaQuest13Poses(const ActorAnimationPack&);
bool goombaQuest13Admitted(const FieldActorRecord&,const FieldActorTalkState&,const FieldActorTalkInventory&);
class GoombaQuest13 {
public:
    bool begin(const FieldActorRecord&,ProloguePouch&,FieldActorTalkState&,FieldActorTalk&,
               ItemSelectionPresentation&,const MessageFont&,MarioPresentation&,uint64_t now);
    void tick(uint64_t now);
    bool active() const {return phase_!=Phase::Idle;}
    uint8_t keyDepth() const {return active()?uint8_t(phase_==Phase::Selecting?2:1):0;}
    const std::vector<int32_t>& table() const {return table_;}
private:
    enum class Phase {Idle,FirstPrint,FirstPose,SecondPose,SecondOpen,SecondPrint,
                      Update,ContinuePrint,MoreOpen,MorePrint,Selecting,Remove,AfterRemove,Finish,FinalPrint};
    Phase phase_=Phase::Idle;
    ProloguePouch* pouch_=nullptr;FieldActorTalkState* switches_=nullptr;
    FieldActorTalk* talk_=nullptr;ItemSelectionPresentation* selector_=nullptr;
    const MessageFont* font_=nullptr;MarioPresentation* mario_=nullptr;
    FieldActorRecord actor_;
    std::vector<int32_t> table_; // One allocation; zero-count update leaves bytes unchanged.
    int32_t count_=0;bool first_=false;
    uint64_t waitUntil_=0;
    bool print(const char* key,uint64_t now);
    bool update();
    bool select();
    void free();
};
}
