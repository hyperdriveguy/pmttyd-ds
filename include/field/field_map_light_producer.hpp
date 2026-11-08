#pragma once
#include "field_light_registry.hpp"
namespace ttyd {
namespace FieldMapLightSource {
constexpr uint32_t TableField=0x98, PositionScale=0x8041f944;
constexpr uint32_t AmbientName=0x802bfc88, PointName=0x802bfc98;
constexpr uint32_t DirectionalName=0x802bfca4, SpotName=0x802bfcb8;
constexpr uint32_t Kind=4, Position=8, Vector=0x14, Color=0x2c;
constexpr uint32_t Scalar0=0x30,Scalar1=0x34,Attenuation=0x38,Flags=0x3c,Magic=0x40;
constexpr uint32_t SupportedFlagsMagic=0x012d60d8;
}
struct FieldMapLightServices {
 void* user=nullptr;
 bool (*word)(void*,uint32_t,uint32_t&)=nullptr;
 bool (*store)(void*,uint32_t,uint32_t)=nullptr;
 bool (*compare)(void*,uint32_t,uint32_t,int32_t&)=nullptr;
 bool (*scale)(void*,uint32_t source,uint32_t scale,uint32_t destination)=nullptr;
 bool (*entry)(void*,const FieldLightEntryArguments&,int32_t&)=nullptr;
};
struct FieldMapLightCursor {
 FieldMapLightServices owner;bool active=false;
 uint32_t phase=0,map=0,positionTemp=0,colorTemp=0,formatTemp=0;
 uint32_t index=0,offset=0,table=0,count=0,record=0,scale=0,kind=0,magic=0,color=0;
 unsigned kindIndex=0;int32_t comparison=0,result=0;
 FieldLightEntryArguments arguments;
};
// Complete reached _mapLoad8002951c..8002960c light loop. This is not the full
// map loader. Inputs must be actual relocated map-entry/table/record storage.
// Capture only reached fields; prior rows survive later unavailable children.
bool loadFieldMapLights(uint32_t mapEntry,uint32_t positionTemp,uint32_t colorTemp,
 uint32_t formatTemp,const FieldMapLightServices&,FieldMapLightCursor&);
}
