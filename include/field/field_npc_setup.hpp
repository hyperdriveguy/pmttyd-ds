#pragma once
#include <cstdint>
namespace ttyd {
struct EventEntry;
struct FieldNpcSetupOperands {
 void* user=nullptr;
 uint32_t (*word)(void*,uint32_t)=nullptr;
 int32_t (*value)(void*,EventEntry&,int32_t)=nullptr;
};
struct FieldNpcSetupEvents {
 void* user=nullptr;
 void (*erase)(void*,uint32_t id)=nullptr;
 bool (*alive)(void*,uint32_t id)=nullptr;
 // Null is native allocation admission with NO constructor effects. Deletion
 // has already occurred; retain the freshly captured script across this wait.
 EventEntry* (*create)(void*,uint32_t script,uint8_t priority,uint8_t flags)=nullptr;
};
struct FieldNpcSetupWorld {
 void* user=nullptr;
 uint32_t (*fieldPointer)(void*)=nullptr; // Source getter runs even though result is ignored.
 // Before row effects, admit readable immutable/lifetime-stable owners and all
 // synchronous world services below. Does NOT require event allocation capacity.
 // No effects on false. Providers remain infallible across an allocation hold;
 // retire the event before replacing their backing storage.
 bool (*admit)(void*,const EventEntry&,uint32_t table,bool sourceFirst,const FieldNpcSetupOperands&)=nullptr;
 uint32_t (*npc)(void*,uint32_t name,bool asserting,const FieldNpcSetupOperands&)=nullptr;
 uint32_t (*word)(void*,uint32_t address)=nullptr;
 void (*store)(void*,uint32_t address,uint32_t value)=nullptr;
 void (*battle)(void*,uint32_t actor,uint32_t setup)=nullptr;
 void (*copyString)(void*,uint32_t destination,uint32_t source)=nullptr;
 void (*setAnimation)(void*,uint32_t pose,uint32_t name,int32_t mode)=nullptr;
 void (*mode)(void*,int32_t)=nullptr; // Fresh mode owner; not the earlier getter's cached pointer.
};
struct FieldNpcSetupCursor {
 enum class Stage {Start,Admission,FirstRow,InitCreate,Metadata,PollRow,RegularRow,RegularCreate};
 Stage stage=Stage::Start;
 bool initialized=false,sourceFirst=false;
 void* owner=nullptr;
 void* memoryOwner=nullptr;
 void* eventOwner=nullptr;
 EventEntry* entry=nullptr;
 uint32_t eventId=0;
 uint32_t table=0,row=0,actor=0,descriptor=0,script=0;
 void firstVisit(bool first){if(!initialized){initialized=true;sourceFirst=first;}}
};
// Native suspension preserves one source invocation. A genuine source return0
// resets the cursor so the next invocation freshly resolves its table and polls.
int32_t fieldNpcSetupEvent(EventEntry&,bool first,FieldNpcSetupCursor&,
 const FieldNpcSetupOperands&,const FieldNpcSetupWorld&,const FieldNpcSetupEvents&);
}
