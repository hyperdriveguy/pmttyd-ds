#pragma once
#include <array>
#include <cstdint>
#include <optional>
namespace ttyd {
namespace FieldLightRegistrySource {
constexpr uint32_t Work=0x8041e650, ActiveGroup=0x8041e664;
constexpr uint32_t Presets=0x803c9630, Vectors=0x802bf870;
constexpr uint32_t Attenuation=0x802bf900, DuplicateFormat=0x802bf930;
constexpr uint32_t Color=0x8041f8b0, Zero=0x8041f8cc, One=0x8041f8d0;
constexpr uint32_t CraftScale=0x8041f8d4, Ten=0x8041f8d8;
constexpr uint32_t Capacity=32, RecordBytes=0x60, DescriptorBytes=8;
constexpr uint32_t Name=2, NameBytes=0x22, Position=0x24, Vector=0x30;
constexpr uint32_t Reference=0x3c, Rgba=0x48, Scalar0=0x4c, Scalar1=0x50;
constexpr uint32_t Coefficients=0x54;
constexpr uint16_t Active=1, Ambient=0x8000, Point=0x1000, Directional=0x2000, Spot=0x4000, Scene=0x80;
}
struct FieldLightEntryArguments {
 uint32_t name=0,type=0,position=0,vector=0,color=0,attenuation=0,flags=0;
 std::array<uint32_t,2> scalars{}; // Entry-captured FLOAT bits f1/f2.
 uint32_t scratch=0; // Stable native mapping for original stack-format buffer.
};
struct FieldLightRegistryServices {
 void* user=nullptr;
 bool (*group)(void*,int32_t&)=nullptr;
 bool (*word)(void*,uint32_t,uint32_t&)=nullptr;
 bool (*half)(void*,uint32_t,uint16_t&)=nullptr;
 bool (*store)(void*,uint32_t,uint32_t)=nullptr;
 bool (*storeHalf)(void*,uint32_t,uint16_t)=nullptr;
 bool (*allocate)(void*,uint32_t heap,uint32_t size,uint32_t&)=nullptr;
 bool (*clear)(void*,uint32_t,uint32_t)=nullptr;
 bool (*copy)(void*,uint32_t,uint32_t)=nullptr;
 bool (*compare)(void*,uint32_t,uint32_t,int32_t&)=nullptr;
 bool (*format)(void*,uint32_t destination,uint32_t format,uint32_t name,int32_t suffix)=nullptr;
 bool (*scale)(void*,uint32_t,std::array<uint32_t,12>&)=nullptr;
 bool (*multiply)(void*,const std::array<uint32_t,12>&,uint32_t source,uint32_t destination)=nullptr;
};
enum class FieldLightOperation:uint8_t {Idle,Initialize,Reset,Enter,Find};
struct FieldLightRegistryCursor {
 FieldLightRegistryServices owner;FieldLightOperation operation=FieldLightOperation::Idle;
 FieldLightEntryArguments arguments;
 uint32_t phase=0,index=0,word=0,descriptor=0,pointer=0,count=0,name=0,suffix=0;
 uint32_t preset=0,step=0,color=0,radius=0,cutoff=0,scale=0;
 uint16_t flags=0;int32_t comparison=0,group=0;
 std::array<uint32_t,6> values{};std::array<uint32_t,3> attenuation{};
 std::array<uint32_t,12> matrix{};
};
// Full original parents, retaining each reached child/store. Unavailable is
// distinct from completion; null allocation and one-past Entry are source
// domains, never invented graceful failures. Providers keep stable identities.
bool initializeFieldLights(const FieldLightRegistryServices&,FieldLightRegistryCursor&);
bool resetFieldLights(const FieldLightRegistryServices&,FieldLightRegistryCursor&);
std::optional<int32_t> enterFieldLight(const FieldLightEntryArguments&,
 const FieldLightRegistryServices&,FieldLightRegistryCursor&);
std::optional<uint32_t> findFieldLight(uint32_t name,
 const FieldLightRegistryServices&,FieldLightRegistryCursor&);
}
