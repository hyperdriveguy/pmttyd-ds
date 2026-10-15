#pragma once
#include "event_memory_opcodes.hpp"
#include <array>
namespace ttyd {
// Native view of parsed source stack inputs, not an original allocated struct.
// All scalar/vector fields are guest addresses; ordinal and area are captured.
struct PackedMapNpcInput {
 uint32_t ordinal=0,area=0,name=0,tribe=0,ai=0,territory=0,battle=0;
 uint32_t position=0,direction=0,territoryBase=0,loiter=0,homing=0;
 std::array<uint32_t,4> ranges{};
};
struct PackedMapNpcServices {
 EventMemoryServices memory;
 void* user=nullptr;
 int32_t (*compare)(void*,uint32_t,uint32_t)=nullptr;
 void (*copyString)(void*,uint32_t,uint32_t)=nullptr;
 void (*entry)(void*,uint32_t name,uint32_t model)=nullptr;
 uint32_t (*lookup)(void*,uint32_t name)=nullptr;
 uint32_t (*area)(void*,uint32_t name)=nullptr;
 void (*battle)(void*,uint32_t actor,int32_t index)=nullptr;
 void (*animation)(void*,int32_t pose,uint32_t name,int32_t mode)=nullptr;
};
uint32_t packedMapNpcTribe(uint32_t name,const PackedMapNpcServices&);
int32_t packedMapNpcDefinition(uint32_t table,uint32_t name,const PackedMapNpcServices&);
// Exact admitted setupDataBase85334..8555c. Caller owns earlier parser/history
// gate, stable inputs/all synchronous services. Returns post-Entry named actor;
// duplicates can therefore populate an earlier slot, not the newly created one.
uint32_t registerPackedMapNpc(const PackedMapNpcInput&,const PackedMapNpcServices&);
}
