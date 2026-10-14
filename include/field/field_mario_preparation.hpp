#pragma once
#include "field_position_event.hpp"
namespace ttyd {
struct FieldMarioPreparationOperands {
 void* user=nullptr;
 uint32_t (*word)(void*,uint32_t)=nullptr;
 int32_t (*value)(void*,EventEntry&,int32_t)=nullptr;
};
// Admission must establish all addressed owners before any original side effect.
// The backing domain may differ per callback; unimplemented domains must refuse.
struct FieldMarioPreparationWorld {
 FieldPositionWorld storage;
 bool (*admit)(void*,uint32_t function)=nullptr;
 uint8_t (*byte)(void*,uintptr_t)=nullptr;
 void (*storeByte)(void*,uintptr_t,uint8_t)=nullptr;
 void (*storeHalf)(void*,uintptr_t,uint16_t)=nullptr;
};
int32_t fieldMarioKeyOff(const FieldMarioPreparationWorld&);
int32_t fieldMarioKeyOn(const FieldMarioPreparationWorld&);
int32_t fieldMarioKeyEvent(EventEntry&,const FieldMarioPreparationOperands&,const FieldMarioPreparationWorld&);
int32_t fieldMarioBackgroundEvent(bool enabled,const FieldMarioPreparationWorld&);
int32_t fieldMarioCameraYOffEvent(const FieldMarioPreparationWorld&);
}
