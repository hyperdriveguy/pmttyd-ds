#include "trouble_quest_presentation.hpp"
#include <cstring>
namespace ttyd {
namespace {
constexpr uint32_t Ids[]={504,524,524,134,136};
}
TroubleQuestIcon* TroubleQuestPresentation::find(const char* name) {
 for(auto& icon:icons_)if((icon.flags&1) && icon.name && !std::strcmp(icon.name,name))return &icon;
 return nullptr;
}
bool TroubleQuestPresentation::begin(int32_t story,const MessageFont& font,const char* path,const TroubleQuestStateServices& state) {
 if(owner_ || !state.ready || !state.activeQuest || !state.flag || !state.sound || !state.ready(state.user))return false;
 for(uint32_t id:Ids) {
  const auto* resource=messageIcon(id);if(!resource || resource->frames.empty())return false;
  if(id==504){if(resource->loop!=1 || resource->frames.size()!=10)return false;
   for(unsigned n=0;n<10;++n)if(resource->frames[n].delay!=(n==9?28u:2u) || resource->frames[n].width!=40 || resource->frames[n].height!=40)return false;
  } else {
   const auto& frame=resource->frames[0];const uint32_t width=id==524?24:60,height=id==524?32:48;
   if(resource->loop || resource->frames.size()!=1 || frame.delay || frame.width!=width || frame.height!=height)return false;
  }
 }
 if(!cache_.prepare(path,font))return false;
 services_=state;
 TroubleQuestMenuServices s;s.user=this;
 s.ready=[](void* p){auto& a=*static_cast<TroubleQuestPresentation*>(p);return a.services_.ready(a.services_.user);};
 s.activeQuest=[](void* p){auto& a=*static_cast<TroubleQuestPresentation*>(p);return a.services_.activeQuest(a.services_.user);};
 s.flag=[](void* p,int32_t f){auto& a=*static_cast<TroubleQuestPresentation*>(p);return a.services_.flag(a.services_.user,f);};
 s.iconEnter=[](void* p,const char* name,int32_t id){auto& a=*static_cast<TroubleQuestPresentation*>(p);
  for(auto& icon:a.icons_)if(!(icon.flags&1)) {
   const auto* resource=messageIcon(id);icon={};icon.name=name;icon.resource=resource;
   icon.flags=uint16_t(0x11 | (resource->loop?0x1000:0));icon.delay=uint16_t(resource->frames[0].delay);break;
  }
 };
 s.iconFlags=[](void* p,const char* n,uint32_t on,uint32_t off){if(auto* i=static_cast<TroubleQuestPresentation*>(p)->find(n))i->flags=uint16_t((i->flags|on)&~off);};
 s.iconScale=[](void* p,const char* n,float scale){if(auto* i=static_cast<TroubleQuestPresentation*>(p)->find(n))i->scale=scale;};
 s.iconPosition=[](void* p,const char* n,float x,float y,float z){if(auto* i=static_cast<TroubleQuestPresentation*>(p)->find(n)){i->x=x;i->y=y;i->z=z;}};
 s.iconDelete=[](void* p,const char* n){if(auto* i=static_cast<TroubleQuestPresentation*>(p)->find(n))i->flags&=~uint16_t(1);};
 s.sound=[](void* p,int32_t id){auto& a=*static_cast<TroubleQuestPresentation*>(p);a.services_.sound(a.services_.user,id);};
 s.publish=[](void* p,int32_t id){static_cast<TroubleQuestPresentation*>(p)->result_=id;};
 s.display=[](void* p,TroubleQuestMenuWork& work){static_cast<TroubleQuestPresentation*>(p)->displayWork_=&work;};
 if(!menu_.begin(story,s))return false;
 owner_=this;return true;
}
std::optional<int32_t> TroubleQuestPresentation::invoke(TroubleQuestMenuInput input) {
 if(!canAdvance())return {};
 displayWork_=nullptr;result_.reset();return menu_.invoke(input);
}
void TroubleQuestPresentation::iconMain(bool ready) {
 if(!canAdvance() || !ready)return;
 for(auto& icon:icons_)if((icon.flags&0x1001)==0x1001) {
  if(icon.delay)--icon.delay;
  if(!icon.delay){icon.frame=uint16_t(icon.frame+1);if(icon.frame>=icon.resource->frames.size())icon.frame=0;
   icon.delay=uint16_t(icon.resource->frames[icon.frame].delay);}
 }
}
}
