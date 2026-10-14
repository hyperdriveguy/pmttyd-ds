#include "goomba_acquisition.hpp"
namespace ttyd {
namespace {constexpr const char* Name="irai_item";}
bool goombaAcquisitionPresentationReady(const FieldItemGetServices& p) {
 return p.ready && p.mario && p.request && p.effect && p.window && p.windowAction &&
        p.bgmSilent && p.bgmCheck && p.ready(p.user);
}
bool goombaQuest2AcquisitionAdmitted(const FieldActorRecord& a,const FieldActorTalkState& s) {
 return a.room==0 && a.key==0x42ff5f1f && s.quest1420==2 && s.flag5406==false && s.flag5405==false;
}
bool GoombaAcquisition::begin(const FieldActorRecord& actor,ProloguePouch& pouch,FieldActorTalkState& state,
 FieldActorTalk& talk,const MessageFont& font,FieldItemRegistry& registry,const FieldItemGetServices& p,uint64_t now,bool blend) {
 if(active() || talk.message.active() || !goombaQuest2AcquisitionAdmitted(actor,state) ||
    !goombaAcquisitionPresentationReady(p) ||
    !talk.canPrint(actor,"irai_02_00",font,now))return false;
 if(!talk.print(actor,"irai_02_00",font,now))return false;
 talk_=&talk;switches_=&state;pouch_=&pouch;registry_=&registry;presentation_=p;mapBlend_=blend;
 owner_=this;stopped_=false;entry_=nullptr;get_=FieldItem95Get{};phase_=Phase::Printing;return true;
}
void GoombaAcquisition::publishEntry() {
 if(!entry_ || !(entry_->flags&1))return;
 entry_->flags=get_.flags();entry_->substate=get_.substate();entry_->status=get_.status();entry_->position=get_.itemPosition();
}
FieldItemGetServices GoombaAcquisition::services() {
 FieldItemGetServices s;s.user=this;
 s.ready=[](void* p){auto& a=*static_cast<GoombaAcquisition*>(p);return a.presentation_.ready(a.presentation_.user);};
 s.mario=[](void* p){auto& a=*static_cast<GoombaAcquisition*>(p);a.publishEntry();return a.presentation_.mario(a.presentation_.user);};
 s.request=[](void* p,ItemGetRequest r){auto& a=*static_cast<GoombaAcquisition*>(p);a.publishEntry();a.presentation_.request(a.presentation_.user,r);};
 s.effect=[](void* p,int32_t type,ItemGetPoint xyz){auto& a=*static_cast<GoombaAcquisition*>(p);a.publishEntry();return a.presentation_.effect(a.presentation_.user,type,xyz);};
 s.window=[](void* p,uint32_t definition,const char* name){auto& a=*static_cast<GoombaAcquisition*>(p);a.publishEntry();return a.presentation_.window(a.presentation_.user,definition,name);};
 s.windowAction=[](void* p,int32_t slot){auto& a=*static_cast<GoombaAcquisition*>(p);a.publishEntry();return a.presentation_.windowAction(a.presentation_.user,slot);};
 s.bgmSilent=[](void* p){auto& a=*static_cast<GoombaAcquisition*>(p);a.publishEntry();return a.presentation_.bgmSilent(a.presentation_.user);};
 s.bgmCheck=[](void* p,int32_t group){auto& a=*static_cast<GoombaAcquisition*>(p);a.publishEntry();return a.presentation_.bgmCheck(a.presentation_.user,group);};
 s.receive=[](void* p,int32_t item){auto& a=*static_cast<GoombaAcquisition*>(p);a.publishEntry();return a.pouch_->insertKeyItem(item);};
 s.removeNamedEntries=[](void* p,const char* name){auto& a=*static_cast<GoombaAcquisition*>(p);a.publishEntry();a.registry_->deleteNamed(name);};
 if(presentation_.discardSelection)s.discardSelection=[](void* p,int32_t kind,int32_t item,int32_t mode){auto& a=*static_cast<GoombaAcquisition*>(p);a.publishEntry();return a.presentation_.discardSelection(a.presentation_.user,kind,item,mode);};
 return s;
}
void GoombaAcquisition::eventMain() {
 if(!canAdvance() || stopped_)return;
 if(phase_==Phase::Printing){if(talk_->message.active())return;phase_=Phase::EntryPending;}
 if(phase_==Phase::EntryPending) {
  if(!registry_->enter95(Name,{0,-1000,0},true,mapBlend_))return; // Native available-slot frontier, no story write.
  registry_->flagOn(Name,2);registry_->forceGet(Name);entry_=registry_->find(Name);
  phase_=Phase::Getting;return; // Original wrapper firstCall returns0, not a grant or poll.
 }
 if(phase_==Phase::Getting && registry_->pollGet(Name)==2){switches_->flag5405=true;phase_=Phase::Complete;}
}
void GoombaAcquisition::itemMain(uint32_t triggers) {
 if(!canAdvance())return;
 if(phase_!=Phase::Getting || !entry_ || !(entry_->flags&1) || entry_->sequence!=2)return;
 if(get_.state()==ItemGetStep::Idle) {
  // Finite original forced-get profile; unsupported live flag mutations remain a frontier.
  if(entry_->item!=95 || entry_->mode!=16 || entry_->flags!=0x1003 || entry_->substate ||
     entry_->secondary!=-1 || entry_->callback || entry_->status || !get_.begin(Name,services(),entry_->position))return;
 }
 get_.tick(triggers);publishEntry();
 // Original second active check prevents deleted records from resubmitting icons.
 registry_->synchronizeIcon(*entry_);
}
}
