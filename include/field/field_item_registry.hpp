#pragma once
#include "field_item_get.hpp"
#include "actor_animation.hpp"
#include <array>
#include <string>
namespace ttyd {
struct FieldItemIcon {
 uint16_t flags=0,id=0;std::string name;
 ItemGetPoint position;float scale=1;uint32_t color=0xffffffff;
 uint32_t camera() const {return flags&0x10?8:flags&0x100?5:flags&0x200?7:4;}
 bool visible(bool buffersReady) const{return buffersReady && (flags&1) && !(flags&2);}
};
struct FieldItemIconDraw {
 ActorMatrix model;
 std::array<ActorVector,4> vertices{};
 std::array<std::array<float,2>,4> uv{{{{0,0}},{{1,0}},{{1,1}},{{0,1}}}};
 uint32_t color=0xffffffff;
 bool depthWrite=true,blend=false;
};
// Finite ordinary item95 icon307, no mapflag40/cull/animated/full-size domain.
// False refuses an unbound material instead of returning invented geometry.
bool item95IconDraw(const FieldItemIcon&,float cameraYaw,uint32_t width,uint32_t height,FieldItemIconDraw&);
struct FieldItemEntry {
 uint16_t flags=0,mode=0,sequence=0,substate=0;
 int32_t item=0,secondary=0;uint32_t callback=0,status=0;
 std::string name;ItemGetPoint position;float scale=1;uint8_t alpha=255;
 uint32_t counter78=0,counter7c=0;
};
// One original ordinary field domain: separate fixed32 item/icon pools. Native
// full-pool/name admission is explicit; source eviction/overflow is unbound.
class FieldItemRegistry {
public:
 static constexpr unsigned Capacity=32;
 FieldItemEntry* enter95(const char* name,ItemGetPoint initial,bool iconBuffersReady=true,bool mapBlend=false);
 FieldItemEntry* find(const char* name); // Source scans names, including inactive.
 const FieldItemEntry* find(const char* name) const;
 FieldItemIcon* findIcon(const char* name); // Source first ACTIVE matching icon.
 void flagOn(const char* name,uint16_t flags);
 bool forceGet(const char* name); // Original writes only; no receive/UI call.
 int32_t pollGet(const char* name) const; // Source wrapper's subsequent-call result.
 void deleteNamed(const char* name); // All equal item names, repeated first-active icon deletion.
 void synchronizeIcon(const FieldItemEntry&);
 void synchronizeIcons(); // After item owner; skips entries deleted by that call.
 std::array<FieldItemEntry,Capacity>& items(){return items_;}
 const std::array<FieldItemEntry,Capacity>& items() const{return items_;}
 const std::array<FieldItemIcon,Capacity>& icons() const{return icons_;}
private:
 std::array<FieldItemEntry,Capacity> items_{};
 std::array<FieldItemIcon,Capacity> icons_{};
};
}
