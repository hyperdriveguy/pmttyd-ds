#include "battle_data_child.hpp"
#include <cassert>

namespace ttyd {
namespace {
int32_t sw(uint32_t v) {return v<=0x7fffffff?static_cast<int32_t>(v):static_cast<int32_t>(int64_t(v)-0x100000000LL);}
uint32_t word(const EventMemoryServices& s,uint32_t a) {assert(s.word);return s.word(s.user,a);}
int32_t resolve(const EventDispatchServices& s,EventEntry& e,uint32_t a) {assert(s.word && s.resolve);const int32_t raw=sw(s.word(s.user,a));return s.resolve(s.user,e,raw);}
}
uint32_t battleUnitData(uint32_t unit,int32_t key,const EventMemoryServices& s) {
    uint32_t p=word(s,unit+0x18);if(p==0)return 0;
    for(;;p+=8) {const uint32_t candidate=word(s,p);if(candidate==0)return 0;if(candidate==static_cast<uint32_t>(key))return word(s,p+4);}
}
int32_t runBattleDataChild(EventEntry& e,const BattleDataChildServices& s) {
    const uint32_t args=e.arguments;const int32_t selector=resolve(s.values,e,args);
    assert(s.translate && s.currentBattle && s.unit);
    const int32_t translated=s.translate(s.user,e,selector);const uint32_t battle=s.currentBattle(s.user),unit=s.unit(s.user,battle,translated);
    const int32_t key=resolve(s.values,e,args+4);const uint32_t script=battleUnitData(unit,key,s.memory);
    if(script==0)return 2;
    assert(s.schedule);auto& child=createChildEvent(*s.schedule,e,script,0,s.creation);
    e.opcode=0;child.owner=translated;return 255;
}
}
