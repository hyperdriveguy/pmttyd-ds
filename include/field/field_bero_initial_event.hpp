#pragma once
#include "field_bero_position.hpp"
#include "field_position_event.hpp"
#include <memory>
namespace ttyd {
class FieldBeroInfo;
struct FieldBeroInitialRuntime;
// Retained original initial_pos child, admitted only for normal kind0.
// Immutable info snapshot and private guest VM survive provider holds. Services
// are refreshed on tick; locals publish only after the original event retires.
class FieldBeroInitialEvent {
public:
 FieldBeroInitialEvent()=default;
 FieldBeroInitialEvent(const FieldBeroInitialEvent&)=delete;
 FieldBeroInitialEvent& operator=(const FieldBeroInitialEvent&)=delete;
 FieldBeroInitialEvent(FieldBeroInitialEvent&&)=delete;
 FieldBeroInitialEvent& operator=(FieldBeroInitialEvent&&)=delete;
 bool begin(const FieldBeroInfo&,const std::array<int32_t,16>&,uint64_t milliseconds);
 void tick(uint64_t milliseconds,const FieldBeroPositionServices&,const FieldPositionWorld&);
 bool readGlobal(uint32_t address,uint32_t&) const;
 bool started=false,finished=false,failed=false;
 // pendingFunction is the last original callback token, not a native function
 // pointer. Nonzero during a hold is diagnostic, not proof of a failed event.
 uint32_t pendingFunction=0,eventId=0;
 std::optional<std::array<int32_t,16>> locals;
 std::optional<FieldBeroPosition> position;
private:
 std::shared_ptr<FieldBeroInitialRuntime> runtime_;
};
}
