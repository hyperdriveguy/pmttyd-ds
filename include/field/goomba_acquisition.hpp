#pragma once
#include "field_actors.hpp"
#include "field_item_registry.hpp"
#include "prologue_pouch.hpp"
namespace ttyd {
bool goombaQuest2AcquisitionAdmitted(const FieldActorRecord&,const FieldActorTalkState&);
// Original Goomba3 quest2 print -> entry/force -> blocking poll ->5405.
// eventMain and itemMain are independent scheduler visits. Presentation services
// remain injected; this owner supplies actual pouch receive and named deletion.
class GoombaAcquisition {
public:
 bool begin(const FieldActorRecord&,ProloguePouch&,FieldActorTalkState&,
            FieldActorTalk&,const MessageFont&,FieldItemRegistry&,const FieldItemGetServices&,
            uint64_t now,bool mapBlend=false);
 void eventMain();
 void itemMain(uint32_t sourceTriggers);
 bool active() const{return phase_!=Phase::Idle && phase_!=Phase::Complete;}
 bool complete() const{return phase_==Phase::Complete;}
 const FieldItem95Get& get() const{return get_;}
private:
 enum class Phase {Idle,Printing,EntryPending,Getting,Complete};
 Phase phase_=Phase::Idle;
 FieldActorTalk* talk_=nullptr;FieldActorTalkState* switches_=nullptr;
 ProloguePouch* pouch_=nullptr;FieldItemRegistry* registry_=nullptr;
 FieldItemEntry* entry_=nullptr;FieldItem95Get get_;
 FieldItemGetServices presentation_{};bool mapBlend_=false;
 FieldItemGetServices services();
 void publishEntry();
};
}
