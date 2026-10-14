#pragma once
#include "event_memory_opcodes.hpp"

namespace ttyd {
struct EffectDeletionServices {
    EventMemoryServices memory;
    void* user=nullptr;
    void (*freeHeap)(void*,int32_t heap,uint32_t payload)=nullptr;
};
// Null/inactive records do nothing. Payload free precedes clearing flags;
// a soft-delete request is not proof that its eventual update owner retired it.
void hardDeleteEffect(uint32_t record,const EffectDeletionServices&);
void softDeleteEffect(uint32_t record,const EffectDeletionServices&);
void releaseMayaEffectPayload(uint32_t payload,const EffectDeletionServices&);
}
