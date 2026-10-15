#pragma once
#include "event_callback_registry.hpp"
#include "event_operands.hpp"
namespace ttyd {
struct FieldStoryPositionServices {
    EventOperandServices values;
    void* user=nullptr;
    uint32_t (*mario)(void*)=nullptr;
    float (*subtract)(void*,float,float)=nullptr;
};
// Original evt_mario_get_pos: source pointer captured before mode resolution;
// destination operands stay raw and X/Y/Z writes use evtSetFloat quantization.
int32_t fieldMarioGetPosition(EventEntry&,const FieldStoryPositionServices&);
bool bindFieldMarioPositionCallback(EventCallbackRegistry&,FieldStoryPositionServices&);
// Original harbor predicates read integer locals: LW2 Z<=-100; before
// battle additionally LW0 X>=-25. Do not substitute unquantized root floats.
enum class HarborPositionGate {GoombellaAttack,BeforeBattle};
// Locals must have been populated by the original get-position callback.
// These original integer IF predicates deliberately use evtGetValue, not floats.
bool harborPositionGate(EventEntry&,HarborPositionGate,const EventOperandServices&);
}
