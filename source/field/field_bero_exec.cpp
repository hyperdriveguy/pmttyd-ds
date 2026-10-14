#include "field_bero_exec.hpp"
#include "event_manager.hpp"
#include <cassert>
namespace ttyd {
int32_t fieldBeroExecOnOffEvent(EventEntry& e,const FieldBeroExecEventServices& s){
    assert(s.word && s.value && s.owner);const auto args=e.arguments;
    const auto operation=s.value(s.user,e,int32_t(s.word(s.user,args)));
    const auto mask=s.value(s.user,e,int32_t(s.word(s.user,args+4)));
    auto* owner=s.owner(s.user);assert(owner);owner->onOff(operation,uint32_t(mask));return 2;
}
}
