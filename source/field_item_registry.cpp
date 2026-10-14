#include "field_item_registry.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
bool item95IconDraw(const FieldItemIcon& icon,float yaw,uint32_t width,uint32_t height,FieldItemIconDraw& out) {
 if(icon.id!=307 || !icon.visible(true) || (icon.flags&~uint16_t(1|0x80|0x100)) || width!=40 || height!=40)return false;
 FieldItemIconDraw draw;const float radians=-(yaw*.01745329238474369f),s=std::sin(radians),c=std::cos(radians);
 ActorMatrix rotation;rotation.v[0]=rotation.v[10]=c;rotation.v[2]=s;rotation.v[8]=-s;
 const float xyz[]={icon.position.x,icon.position.y,icon.position.z},scale[]={icon.scale,icon.scale,icon.scale};
 draw.model=ActorMatrix::translation(xyz)*rotation*ActorMatrix::scale(scale);
 const float factor=.6000000238418579f,w=float(width),h=float(height);
 const float left=(-w*.5f)*factor,extent=w*factor,top=h*factor,bottom=top+(-h)*factor;
 draw.vertices={{{{left,top,0}},{{left+extent,top,0}},{{left+extent,bottom,0}},{{left,bottom,0}}}};
 draw.color=icon.color;draw.depthWrite=(icon.color&255)==255;draw.blend=!draw.depthWrite;
 out=draw;return true;
}
FieldItemEntry* FieldItemRegistry::find(const char* name) {
 if(!name)return nullptr;
 for(auto& e:items_)if(e.name==name)return &e;
 return nullptr;
}
const FieldItemEntry* FieldItemRegistry::find(const char* name) const {
 if(!name)return nullptr;
 for(const auto& e:items_)if(e.name==name)return &e;
 return nullptr;
}
FieldItemIcon* FieldItemRegistry::findIcon(const char* name) {
 if(!name)return nullptr;
 for(auto& i:icons_)if((i.flags&1) && i.name==name)return &i;
 return nullptr;
}
FieldItemEntry* FieldItemRegistry::enter95(const char* name,ItemGetPoint position,bool ready,bool mapBlend) {
 // Native admission for the finite explicit-name/available-slot source path.
 if(!name || !*name || std::strlen(name)>15)return nullptr;
 FieldItemEntry* item=nullptr;FieldItemIcon* icon=nullptr;
 for(auto& e:items_)if(!(e.flags&1)){item=&e;break;}
 for(auto& i:icons_)if(!(i.flags&1)){icon=&i;break;}
 if(!item || !icon)return nullptr;
 *item=FieldItemEntry{};item->name=name;item->flags=1;item->mode=16;item->item=95;item->secondary=-1;item->callback=0;
 item->position=position;item->scale=1;item->alpha=255;
 *icon=FieldItemIcon{};icon->flags=ready?1:0x2001;if(mapBlend)icon->flags|=0x40;icon->id=307;icon->name=name;icon->scale=1;icon->color=0xffffffff;
 // Original setters resolve first active name, not necessarily the new icon.
 auto* target=findIcon(name);target->position=position;target->scale=1;target->color=(target->color&0xffffff00u)|255;
 item->sequence=7;item->substate=0;return item;
}
void FieldItemRegistry::flagOn(const char* name,uint16_t flags) {if(auto* e=find(name))e->flags|=flags;}
bool FieldItemRegistry::forceGet(const char* name) {
 auto* e=find(name);if(!e)return false; // Native safety refusal for source null dereference.
 e->counter78=e->counter7c=0;e->sequence=2;e->substate=0;e->flags|=0x1000;e->status&=~0x10000u;return true;
}
int32_t FieldItemRegistry::pollGet(const char* name) const {
 const auto* e=find(name);return !e || !(e->flags&1) || (e->status&(2|0x10000))?2:0;
}
void FieldItemRegistry::deleteNamed(const char* name) {
 if(!name)return;
 for(auto& e:items_)if(e.name==name){if(auto* icon=findIcon(name))icon->flags&=~1u;e.flags&=~1u;}
}
void FieldItemRegistry::synchronizeIcon(const FieldItemEntry& e) {
 if(e.flags&1)if(auto* i=findIcon(e.name.c_str())) {
  i->position=e.position;i->scale=e.scale;i->color=(i->color&0xffffff00u)|e.alpha;
 }
}
void FieldItemRegistry::synchronizeIcons() {for(const auto& e:items_)synchronizeIcon(e);}
}
