#include "field_item_get.hpp"
namespace ttyd {
bool FieldItem95Get::begin(const char* name,const FieldItemGetServices& s) {
 if(state_==ItemGetStep::Running || state_==ItemGetStep::DiscardPending || !name || !*name ||
    !s.ready || !s.mario || !s.request || !s.effect || !s.window || !s.windowAction ||
    !s.bgmSilent || !s.bgmCheck || !s.receive || !s.removeNamedEntries || !s.ready(s.user))return false;
 services_=s;name_=name;state_=ItemGetStep::Running;substate_=0;flags_=0x1003;
 status_=0;position_={0,-1000,0};effect_=0;discard_=0;for(auto& w:windows_)w=-1;return true;
}
ItemGetStep FieldItem95Get::tick(uint32_t buttons) {
 if(state_!=ItemGetStep::Running)return state_;
 if(substate_==0) {
  status_&=~0x10000u;substate_=1;
  request(ItemGetRequest::Camera8FlagOn);request(ItemGetRequest::KeyOff);
  request(ItemGetRequest::SystemLevel2);request(ItemGetRequest::SoundFlag80On);request(ItemGetRequest::ClearMarioMotion);
  // Source substate0 falls through to1; forced1000 bypasses motion polling.
  substate_=2;request(ItemGetRequest::GetItemMotion);
  const auto mario=services_.mario(services_.user);position_=mario;position_.y+=50.f;
  auto effectPosition=mario;effectPosition.y+=62.f;
  effect_=services_.effect(services_.user,6,effectPosition);request(ItemGetRequest::EffectPayload5);
  request(ItemGetRequest::IconFlag100On);
  if(services_.bgmSilent(services_.user))flags_|=8;
  if(!(flags_&8))request(ItemGetRequest::BgmOff3800);
  request(ItemGetRequest::KeyItemFanfare);
  flags_&=~0x2000u;
  windows_[0]=services_.window(services_.user,0x80313dc8,name_.c_str());
  windows_[1]=services_.window(services_.user,0x80313df0,name_.c_str());
  if(services_.receive(services_.user,95))status_|=1;
  else windows_[2]=services_.window(services_.user,0x80313e18,name_.c_str());
  request(ItemGetRequest::IconFlag80On);return state_;
 }
 if(substate_==2) {
  request(ItemGetRequest::ControlOff);status_|=0x01000000u;substate_=3;
 }
 if(substate_==3) {
  if(!(flags_&0x2000) && services_.bgmCheck(services_.user,1)!=0) {
   flags_|=0x2000;request(ItemGetRequest::BgmOff201);
   if(!(flags_&8))request(ItemGetRequest::BgmResumeA0);
  }
  if(services_.windowAction(services_.user,windows_[0])!=0 || !(buttons&0x300))return state_;
  request(ItemGetRequest::WindowCloseMain);request(ItemGetRequest::WindowCloseSecondary);
  if(!(flags_&0x2000)) {request(ItemGetRequest::BgmOff201);if(!(flags_&8))request(ItemGetRequest::BgmResumeA0);}
  request(ItemGetRequest::EffectSoftDelete);request(ItemGetRequest::Camera4);
  request(ItemGetRequest::IconFlag100Off);request(ItemGetRequest::IconFlag80Off);
  if(!(status_&1)) {
   substate_=5;request(ItemGetRequest::WindowCloseFailure);request(ItemGetRequest::StayMotion);request(ItemGetRequest::IconFlag2On);
   if(services_.discardSelection)discard_=services_.discardSelection(services_.user,1,95,1);
   state_=ItemGetStep::DiscardPending;return state_;
  }
  request(ItemGetRequest::SystemLevel0);request(ItemGetRequest::SoundFlag80Off);
  status_&=~0x01000000u;request(ItemGetRequest::ControlOn);request(ItemGetRequest::KeyOn);
  request(ItemGetRequest::StayMotion);request(ItemGetRequest::Camera8FlagOff);
  services_.removeNamedEntries(services_.user,name_.c_str());flags_&=~1u;state_=ItemGetStep::Complete;
 }
 return state_;
}
}
