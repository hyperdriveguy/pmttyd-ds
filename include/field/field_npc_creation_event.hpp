#pragma once
#include "event_manager.hpp"
#include <optional>
namespace ttyd {
bool fieldNpcCreationEventFunction(uint32_t);
struct FieldNpcCreationEventServices {
 void* user=nullptr;
 bool (*word)(void*,uint32_t,uint32_t&)=nullptr;
 bool (*half)(void*,uint32_t,uint16_t&)=nullptr;
 bool (*value)(void*,EventEntry&,int32_t,int32_t&)=nullptr;
 std::optional<uint32_t> (*resolve)(void*,EventEntry&,uint32_t)=nullptr;
 std::optional<uint32_t> (*tribe)(void*,uint32_t)=nullptr;
 std::optional<uint32_t> (*async)(void*,uint32_t name,uint32_t mode)=nullptr;
 std::optional<int32_t> (*enter)(void*,uint32_t name,uint32_t model)=nullptr;
 bool (*copy)(void*,uint32_t destination,uint32_t source)=nullptr;
 bool (*animation)(void*,uint32_t handle,uint32_t name,uint32_t force)=nullptr;
 void (*store)(void*,uint32_t,uint32_t)=nullptr;
 void (*storeHalf)(void*,uint32_t,uint16_t)=nullptr;
};
struct FieldNpcCreationEventCursor {
 FieldNpcCreationEventServices owner;EventEntry* event=nullptr;uint32_t id=0,function=0,phase=0,args=0;
 uint32_t raw=0,name=0,modelOrTribe=0,gp=0,mode=0,actor=0,descriptor=0,pointer=0,handle=0,metadata=0,zero=0;
 uint16_t half=0;
};
// Complete original evt_npc_entry800536e4 and evt_npc_set_tribe8004e5c0.
// Empty means a retained native child. Engaged0 completes one Entry poll;
// engaged1 completes SetTribe and yields; engaged2 completes Entry/continues.
// No firstCall gate. Keep identical event/function/services during a hold.
// Memory stores are synchronous admitted effects; source invalid dereference
// domains are not converted into successful ignored setters.
std::optional<int32_t> fieldNpcCreationEvent(uint32_t,EventEntry&,
 const FieldNpcCreationEventServices&,FieldNpcCreationEventCursor&);
}
