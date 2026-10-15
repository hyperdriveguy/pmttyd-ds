#pragma once
#include "event_creation.hpp"
namespace ttyd {
// Direct evt_door_param selectors. Bound memory is required through each ordered
// read; geometry-derived selectors4/5/10/11 retain an explicit provider boundary.
struct FieldDoorParameterServices {
 void* user=nullptr;
 uint32_t (*word)(void*,uint32_t)=nullptr;
 int32_t (*value)(void*,EventEntry&,int32_t)=nullptr;
 bool (*admit)(void*,uint32_t record,uint32_t selector)=nullptr; // Native bound-memory preflight.
 void (*setValue)(void*,EventEntry&,int32_t,int32_t)=nullptr;
};
// EVT0 retains USER_FUNC, 2 consumes it. Native admit preflight does not
// replace the ordered original memory reads with invented default values.
int32_t fieldDoorParameterEvent(EventEntry&,const FieldDoorParameterServices&);
}
