#include "effect_deletion.hpp"
#include <cassert>

// Original effect lifecycle: flag word+0,payload pointer+0xc,heap index3.
// Hard delete frees immediately; soft delete flag2 keeps ownership and ORs
// pending-delete flag4. Maya payload release frees its supplied pointer directly.
// Address words are service tokens, not host allocations.
namespace ttyd {
namespace {
uint32_t word(const EffectDeletionServices& s,uint32_t a) {assert(s.memory.word);return s.memory.word(s.memory.user,a);}
void free(const EffectDeletionServices& s,uint32_t p) {assert(s.freeHeap);s.freeHeap(s.user,3,p);}
void release(uint32_t record,const EffectDeletionServices& s) {
    free(s,word(s,record+0xc));assert(s.memory.storeWord);s.memory.storeWord(s.memory.user,record,0);
}
}
void hardDeleteEffect(uint32_t record,const EffectDeletionServices& s) {if(record && word(s,record)!=0)release(record,s);}
void softDeleteEffect(uint32_t record,const EffectDeletionServices& s) {
    if(!record)return;
    const uint32_t flags=word(s,record);if(flags==0)return;
    if(flags&2) {assert(s.memory.storeWord);s.memory.storeWord(s.memory.user,record,flags|4);return;}
    release(record,s); // Original inline release uses retained flags,not a new test.
}
void releaseMayaEffectPayload(uint32_t payload,const EffectDeletionServices& s) {free(s,payload);}
}
