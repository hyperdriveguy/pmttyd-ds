#include "trouble_board_presentation.hpp"
namespace ttyd {
namespace {
// Opaque port handle, scoped to this owner, never a host pointer or GC address.
constexpr int32_t MenuHandle=1;
uint64_t pendingKey(const TroubleEventRequest& r){return uint64_t(r.eventId)<<32 | r.function;}
}
bool TroubleBoardPresentation::bind(TroubleBoardEvent& event,const MessageAssets& assets,const MessageFont& font,
 const char* room,const TroubleBoardPresentationServices& services) {
 if(event_ || !room || !*room)return false;
 event_=&event;assets_=&assets;font_=&font;room_=room;services_=services;return true;
}
bool TroubleBoardPresentation::visit(uint64_t visit,uint64_t now,const TroubleBoardInput& input) {
 return eventMain(visit,now,input) && presentationMain(input);
}
bool TroubleBoardPresentation::eventMain(uint64_t visit,uint64_t now,const TroubleBoardInput& input) {
 if(!event_ || presentationPending_ || (visited_ && visit<=lastVisit_))return false;
 visited_=true;lastVisit_=visit;now_=now;input_=input;presentationPending_=true;
 event_->tick(now,{this,[](void* p,TroubleEventRequest& r){return static_cast<TroubleBoardPresentation*>(p)->call(r);}});
 return true;
}
bool TroubleBoardPresentation::presentationMain(const TroubleBoardInput& input) {
 if(!presentationPending_)return false;
 presentationPending_=false;
 if(menu_)menu_->iconMain();
 if(message.active())message.tick(now_,input.messageTriggers);
 if(selection.active())selection.tick(input.selection);
 if(services_.selectionSound) {
  for(auto id:selection.sounds)services_.selectionSound(services_.soundUser,id);
  selection.sounds.clear();
 }
 return true;
}
int32_t TroubleBoardPresentation::call(TroubleEventRequest& r) {
 const auto key=pendingKey(r);
 // first can already be false after an earlier missing resource/provider. The
 // retained operation starts exactly once when its dependencies become ready.
 auto opened=[&]{return opened_.find(key)!=opened_.end();};
 auto done=[&]{opened_.erase(key);return 2;};
 switch(r.kind) {
 case TroubleEventCall::Toge:
  toge_[r.eventId]={r.values[0],r.values[1],r.values[2],r.values[3]};return 2;
 case TroubleEventCall::Print:
  if(!opened()) {
   const auto* text=assets_->find(r.text.c_str());
   if(!text || message.pending() || !message.open(r.text.c_str(),text,*font_,now_))return 0;
   const auto t=toge_[r.eventId];
   anchor_={t.type==2,float(t.x),float(t.y),float(t.z)};
   message.placementOverride=t.type==2?7:0;
   toge_.erase(r.eventId);opened_[key]=true;
  }
  return message.pending()?0:done();
 case TroubleEventCall::Append:
  if(!opened()) {
   const auto* text=assets_->find(r.text.c_str());
   if(!text || !message.append(text,*font_,now_))return 0;
   opened_[key]=true;
  }
  return message.pending()?0:done();
 case TroubleEventCall::Continue:
  if(!opened()) {if(!message.resume(now_))return 0;opened_[key]=true;}
  return message.pending()?0:done();
 case TroubleEventCall::Select:
  if(!opened()) {
   const auto* text=assets_->find(r.text.c_str());
   if(!services_.selectionSound || !text || selection.active() || !selection.open(r.text.c_str(),text,*font_,now_))return 0;
   opened_[key]=true;
  }
  if(!selection.complete())return 0;
  r.result=selection.current;return done();
 case TroubleEventCall::MenuInit: {
  if(menu_ || !services_.menuSound)return 0;
  auto next=std::make_unique<TroubleQuestPresentation>();
  activeQuest_=r.activeQuest;
  TroubleQuestStateServices s{this,[](void*){return true;},
   [](void* p){return std::optional<int32_t>(static_cast<TroubleBoardPresentation*>(p)->activeQuest_);},
   [](void* p,int32_t f){return troubleQuestFlag(static_cast<TroubleBoardPresentation*>(p)->event_->switches,f);},
   [](void* p,int32_t row){auto& a=*static_cast<TroubleBoardPresentation*>(p);a.services_.menuSound(a.services_.soundUser,row);}};
  if(!next->begin(r.story,*font_,room_.c_str(),s))return 0;
  menu_=std::move(next);r.result=MenuHandle;return 2;
 }
 case TroubleEventCall::MenuMain: {
  if(!menu_ || r.work!=MenuHandle)return 0;
  activeQuest_=r.activeQuest;
  const auto status=menu_->invoke(input_.menu);
  if(!status || *status!=2 || !menu_->result())return 0;
  r.result=*menu_->result();return 2;
 }
 case TroubleEventCall::MenuEnd:
  if(!menu_ || r.work!=MenuHandle)return 0;
  menu_.reset();return 2;
 default:
  return services_.world.call?services_.world.call(services_.world.user,r):0;
 }
}
}
