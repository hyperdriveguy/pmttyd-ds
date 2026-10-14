#pragma once
#include "field_actors.hpp"
#include <optional>
namespace ttyd {
// Original Goomba3 quest2 reward calls the blocking coin service; it never
// replaces that presentation with a direct coin write.
struct GoombaRewardServices {
 void* user=nullptr;
 bool (*ready)(void*)=nullptr;
 std::optional<int32_t> (*coin)(void*,int32_t amount,bool firstCall)=nullptr;
 void (*setFlag)(void*,int32_t flag)=nullptr;
};
bool goombaQuest2RewardAdmitted(const FieldActorRecord&,const FieldActorTalkState&);
class GoombaReward {
public:
 bool begin(const FieldActorRecord&,FieldActorTalkState&,FieldActorTalk&,
            const MessageFont&,const GoombaRewardServices&,uint64_t now);
 void eventMain(uint64_t now);
 bool active() const{return phase_!=Phase::Idle && phase_!=Phase::Complete;}
 bool complete() const{return phase_==Phase::Complete;}
 bool canAdvance() const{return owner_==this;}
private:
 enum class Phase {Idle,Printing,Coin,FinishOpen,FinishPrint,Complete};
 Phase phase_=Phase::Idle;
 const GoombaReward* owner_=nullptr;
 FieldActorTalkState* switches_=nullptr;
 FieldActorTalk* talk_=nullptr;
 const MessageFont* font_=nullptr;
 FieldActorRecord actor_;
 GoombaRewardServices services_;
 bool firstCoinCall_=true;
};
}
