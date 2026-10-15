#pragma once
#include "event_manager.hpp"
#include <array>
#include <optional>
namespace ttyd {
bool fieldNpcActorEventFunction(uint32_t function);
// False/empty is an unavailable provider with no effects. Successfully read
// arguments and completed outputs survive holds; all stores are synchronous.
struct FieldNpcActorEventServices {
 void* user=nullptr;
 bool (*word)(void*,uint32_t,uint32_t&)=nullptr;
 bool (*value)(void*,EventEntry&,int32_t,int32_t&)=nullptr;
 bool (*real)(void*,EventEntry&,int32_t,float&)=nullptr;
 // Success admits stable full actor reads/stores through this invocation.
 std::optional<uint32_t> (*resolve)(void*,EventEntry&,uint32_t,bool dereference)=nullptr;
 void (*store)(void*,uint32_t,uint32_t)=nullptr;
 bool (*setReal)(void*,EventEntry&,int32_t,float)=nullptr;
 std::optional<float> (*cameraHeading)(void*,uint32_t)=nullptr;
};
// Cursor binds owner, event address+ID, function and argument base. Completed
// argument reads, actor resolution and per-axis outputs survive native holds;
// caller discards it after2 or actual event retirement.
struct FieldNpcActorEventCursor {
 enum class Stage {Arguments,Resolve,Camera,Effects,Output};
 Stage stage=Stage::Arguments;
 void* owner=nullptr;EventEntry* entry=nullptr;
 uint32_t id=0,function=0,args=0,name=0,actor=0;
 unsigned argument=0,output=0;bool initialized=false,outputCaptured=false;
 int32_t selector=0;uint32_t mask=0;
 std::array<int32_t,3> destinations{};
 std::array<float,3> values{};
};
// Source callbacks return2;0 only suspends unavailable native providers.
int32_t fieldNpcActorEvent(uint32_t,EventEntry&,bool first,FieldNpcActorEventCursor&,const FieldNpcActorEventServices&);
}
