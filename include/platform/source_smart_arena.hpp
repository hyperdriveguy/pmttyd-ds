#pragma once
#include "event_address_space.hpp"
#include "field_map_source.hpp"
#include <memory>
namespace ttyd {
struct SourceSmartArenaBackend {
 void* user=nullptr;
 // Pure shared SDK heap/platform/file lifetime admission; no mapping mutation.
 bool (*current)(void*)=nullptr;
 // Real allocation, memory/cache, draw-sync, file collection/movement/thread
 // providers. False retains the actual source child and its partial effects.
 bool (*call)(void*,FieldMapSourceCall&)=nullptr;
};
class SourceSmartArena {
public:
 static constexpr uint32_t Init=0x8002fbe0,Allocate=0x8002f540,Free=0x8002f878,AutoFree=0x8002f9cc,Compact=0x8002f378,TextureObject=0x8002f33c;
 static constexpr uint32_t Pointer=0x804151e0,Work=0x803ca410,WorkBytes=0xe01c,FirstAllocation=0x8041e810;
 static constexpr uint32_t HeaderBytes=0x1c,HeaderCount=2048,LeadingGap=0xe004,UsedHead=0xe008,UsedTail=0xe00c,AvailableHead=0xe010,AvailableTail=0xe014,ReleaseCounter=0xe018;
 static constexpr uint32_t StackBytes=32*1024,StackGuardBytes=32;
 // Bind actual cold process BSS. Initialized wp and real shared heap4 globals/
 // SDK backing belong to their existing owners; never import an arena image.
 static std::unique_ptr<SourceSmartArena> bind(EventAddressSpace&,uint32_t stackAddress,const SourceSmartArenaBackend&);
 ~SourceSmartArena();
 SourceSmartArena(const SourceSmartArena&)=delete;SourceSmartArena& operator=(const SourceSmartArena&)=delete;
 bool current()const;
 bool currentFor(const EventAddressSpace& m)const{return &memory_==&m && current();}
 bool ready()const{return initialized_ && current();}
 bool initialize(unsigned visits=4096);
 bool service(FieldMapSourceCall&,unsigned visits=4096);
 static bool accepts(uint32_t f){return f==Allocate || f==Free || f==AutoFree || f==Compact || f==TextureObject;}
 uint32_t pendingInstruction()const{return cursor_.pc;}
private:
 SourceSmartArena(EventAddressSpace&,uint32_t,const SourceSmartArenaBackend&);
 FieldMapSourceServices services();bool child(FieldMapSourceCall&);
 EventAddressSpace& memory_;uint32_t stackAddress_;SourceSmartArenaBackend backend_;
 std::array<uint8_t,WorkBytes> work_{};
 std::array<uint8_t,4> first_{};
 std::array<uint8_t,StackBytes> stack_{};
 std::array<uint32_t,3> mappings_{};unsigned mapped_=0;
 bool initialized_=false,invoking_=false;
 FieldMapSourceCursor cursor_;
 const FieldMapSourceCall* call_=nullptr;FieldMapSourceCall operands_;
};
}
