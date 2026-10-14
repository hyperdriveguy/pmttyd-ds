#pragma once
#include <cstdint>
namespace ttyd {
struct EventEntry;
struct FieldPositionOperands {
 void* user=nullptr;
 uint32_t (*word)(void*,uint32_t)=nullptr;
 float (*scalar)(void*,EventEntry&,int32_t)=nullptr;
};
// Captured actor/camera tokens identify live owners. Offsets are original;
// backing memory/layout and owner admission are the caller's responsibility.
struct FieldPositionWorld {
 void* user=nullptr;
 uintptr_t (*mario)(void*)=nullptr;
 int32_t (*currentParty)(void*)=nullptr;
 uintptr_t (*party)(void*,int32_t)=nullptr;
 uintptr_t (*camera)(void*,int32_t)=nullptr;
 uint32_t (*read)(void*,uintptr_t)=nullptr;
 void (*write)(void*,uintptr_t,uint32_t)=nullptr;
 void (*roadReset)(void*)=nullptr;
 void (*shiftReset)(void*)=nullptr;
 uint16_t (*half)(void*,uintptr_t)=nullptr;
};
int32_t fieldMarioSetPositionEvent(EventEntry&,const FieldPositionOperands&,const FieldPositionWorld&);
int32_t fieldPartySetPositionEvent(EventEntry&,const FieldPositionOperands&,const FieldPositionWorld&);
int32_t fieldCameraRoadResetEvent(const FieldPositionWorld&);
}
