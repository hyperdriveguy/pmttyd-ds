#pragma once
#include "event_address_space.hpp"
#include "field_map_source.hpp"
#include <memory>
namespace ttyd {
// Providers own actual process/smart allocations, GX object constructors and
// cache services. current must cover their backing/lifetime through all holds.
// False retains the same source child; a completed callback must include its
// actual effects. No image allocation or GPU completion is inferred here.
struct SourceShadowProcessBackend {
 void* user=nullptr;
 bool (*current)(void*)=nullptr;
 bool (*call)(void*,FieldMapSourceCall&)=nullptr;
};
class SourceShadowProcess {
public:
 static constexpr uint32_t Init=0x8002dcec,Projection=0x8002bc7c;
 static constexpr uint32_t PrimaryPointer=0x804150e0,ProjectionPointer=0x804150e4,DepthPointer=0x804150e8;
 static constexpr uint32_t Primary=0x803c9e38,ProjectionWork=0x803c9d34,DepthWork=0x803c9c30;
 static constexpr uint32_t PrimaryBytes=0x11c,AuxiliaryBytes=0x104;
 static constexpr uint32_t RampObjects=0x8041e738,RampObjectBytes=0x40;
 static constexpr uint32_t StackBytes=32*1024,StackGuardBytes=32;
 // Original initialized pointer words/literals must already exist. This owns
 // only cold process BSS and private native stack, never initialized stand-ins.
 static std::unique_ptr<SourceShadowProcess> bind(EventAddressSpace&,uint32_t stackAddress,const SourceShadowProcessBackend&);
 ~SourceShadowProcess();
 SourceShadowProcess(const SourceShadowProcess&)=delete;
 SourceShadowProcess& operator=(const SourceShadowProcess&)=delete;
 bool current()const;
 bool currentFor(const EventAddressSpace& m)const{return &memory_==&m && current();}
 bool ready()const{return initialized_ && current();}
 bool initialize(unsigned visits=4096);
 bool service(FieldMapSourceCall&,unsigned visits=4096);
 static bool accepts(uint32_t f){return f==Projection;}
 uint32_t pendingInstruction()const{return initializing_.active?initializing_.pc:query_.pc;}
private:
 SourceShadowProcess(EventAddressSpace&,uint32_t,const SourceShadowProcessBackend&);
 FieldMapSourceServices services();bool child(FieldMapSourceCall&);
 EventAddressSpace& memory_;uint32_t stackAddress_;SourceShadowProcessBackend backend_;
 std::array<uint8_t,PrimaryBytes> primary_{};
 std::array<uint8_t,AuxiliaryBytes> projection_{},depth_{};
 std::array<uint8_t,RampObjectBytes> ramps_{};
 std::array<uint8_t,StackBytes> stack_{};
 std::array<uint32_t,5> mappings_{};unsigned mapped_=0;
 bool initialized_=false,invoking_=false;
 FieldMapSourceCursor initializing_,query_;
 const FieldMapSourceCall* call_=nullptr;FieldMapSourceCall operands_;
};
}
