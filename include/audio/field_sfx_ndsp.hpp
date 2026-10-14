#pragma once
#include "positional_sfx_registry.hpp"
namespace ttyd {
// Adapter for already-owned native effect voices; not an On allocator, macro
// interpreter, or source emitter. Registry Off retains original slot semantics.
class FieldSfxNdsp {
public:
 bool attach(int32_t sourceHandle,int32_t sourceKey,uint32_t sourceDescriptor,int32_t ndspChannel);
 bool attachPending(int32_t sourceHandle,int32_t sourceKey,uint32_t sourceDescriptor);
 PositionalSfxRegistry& registry(){return registry_;}
 const PositionalSfxRegistry& registry() const{return registry_;}
 bool canOff(int32_t sourceHandle,bool backendAvailable) const;
 PositionalSfxStopServices services();
 bool clear();
private:
 PositionalSfxRegistry registry_;
 std::array<uint32_t,40> descriptors_{};
 bool install(int32_t,int32_t,uint32_t,int32_t,bool);
};
}
