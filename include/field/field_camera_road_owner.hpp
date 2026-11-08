#pragma once
#include "field_map_source.hpp"
#include "event_address_space.hpp"
#include <memory>
namespace ttyd {
// Borrow the actual existing camera4 table slot and 0x260-byte process entry.
// camInit/camEntryPersp own those allocations; road loading never creates one.
struct FieldCameraEntryBorrow {
 uint8_t* slotBacking=nullptr;
 uint32_t entryAddress=0;
 uint8_t* entryBacking=nullptr;
};
class FieldCameraRoadOwner {
public:
 static constexpr uint32_t RoadPointer=0x80414fa8,RoadWork=0x803c93e8,RoadBytes=0x210;
 static constexpr uint32_t ShiftPointer=0x804169c8,ShiftWork=0x803e1b30,ShiftBytes=0x38;
 static constexpr uint32_t CameraTable=0x803c95f8,FieldCamera=4,CameraSlot=CameraTable+FieldCamera*sizeof(uint32_t),CameraBytes=0x260;
 static constexpr uint32_t StackBytes=32*1024,StackGuardBytes=32;
 static std::unique_ptr<FieldCameraRoadOwner> bind(EventAddressSpace&,uint32_t stackAddress,const FieldCameraEntryBorrow&);
 ~FieldCameraRoadOwner();
 FieldCameraRoadOwner(const FieldCameraRoadOwner&)=delete;FieldCameraRoadOwner& operator=(const FieldCameraRoadOwner&)=delete;
 bool current() const;
 bool currentFor(const EventAddressSpace& m) const{return &memory_==&m && current();}
 static bool accepts(uint32_t f){return f==FieldMapSource::CameraLoad || f==FieldMapSource::CameraUnload || f==FieldMapSource::CameraPerspective;}
 // Same source child services/map heap/file worker as the suspended parent.
 // Cursor/stack and raw call object/providers stay stationary through holds.
 bool service(FieldMapSourceCall&,const FieldMapSourceServices&,unsigned visits=4096);
 bool pending() const{return call_!=nullptr;}
 uint32_t pendingInstruction() const{return cursor_.pc;}
private:
 FieldCameraRoadOwner(EventAddressSpace&,uint32_t,const FieldCameraEntryBorrow&);
 EventAddressSpace& memory_;uint32_t stackAddress_;FieldCameraEntryBorrow camera_;
 std::array<uint8_t,RoadBytes> road_{};
 std::array<uint8_t,ShiftBytes> shift_{};
 std::array<uint8_t,StackBytes> stack_{};
 std::array<uint32_t,3> mappings_{};unsigned mapped_=0;
 FieldMapSourceCursor cursor_;const FieldMapSourceCall* call_=nullptr;
 FieldMapSourceCall operands_;
};
}
