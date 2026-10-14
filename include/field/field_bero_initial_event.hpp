#pragma once
#include "field_bero_position.hpp"
#include "field_position_event.hpp"
#include <memory>
namespace ttyd {
class FieldBeroInfo;
struct FieldBeroInitialRuntime;
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
 uint32_t pendingFunction=0,eventId=0;
 std::optional<std::array<int32_t,16>> locals;
 std::optional<FieldBeroPosition> position;
private:
 std::shared_ptr<FieldBeroInitialRuntime> runtime_;
};
}
