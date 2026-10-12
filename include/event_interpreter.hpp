#pragma once
#include "event_data_opcodes.hpp"
#include "event_control_opcodes.hpp"
#include "event_memory_opcodes.hpp"
#include "event_tail_opcodes.hpp"

namespace ttyd {
struct EventInterpreterServices {
    EventSchedule* schedule=nullptr;
    EventCreationServices creation;
    EventOperandServices operands;
    EventMemoryServices memory;
    void* user=nullptr;
    int32_t (*call)(void*,EventEntry&,uint32_t function,int32_t firstCall)=nullptr;
    void (*format)(void*,const EventDiagnosticCall&)=nullptr;
    void* numericUser=nullptr;
    int32_t (*divideWord)(void*,int32_t,int32_t)=nullptr;
};
// Combines every recovered opcode0..118 with actual operand/lifecycle services.
// Original-address memory,USER_FUNC,clock,FPSCR and formatter owners are required
// at their call boundaries. No synthetic success or blanket command yield.
int32_t dispatchEventInterpreter(EventEntry&,const EventInterpreterServices&);
void runEventInterpreterManager(const EventInterpreterServices&);
}
