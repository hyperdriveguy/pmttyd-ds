#include "battle_unit_event.hpp"
#include <cassert>

namespace ttyd {
namespace {
uint32_t word(const BattleUnitEventServices& s,uint32_t a) {assert(s.memory.word);return s.memory.word(s.memory.user,a);}
int32_t sw(uint32_t v) {return v<=0x7fffffff?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}
int32_t resolve(EventEntry& e,uint32_t address,const BattleUnitEventServices& s) {const auto raw=sw(word(s,address));assert(s.values.resolve);return s.values.resolve(s.values.user,e,raw);}
int32_t actor(EventEntry& e,uint32_t args,const BattleUnitEventServices& s) {const int32_t selector=resolve(e,args,s);assert(s.translate);return s.translate(s.user,e,selector);}
uint32_t unit(uint32_t battle,int32_t id,const BattleUnitEventServices& s) {assert(s.unit);return s.unit(s.user,battle,id);}
}
int32_t battleUnitHpEvent(EventEntry& e,bool maximum,const BattleUnitEventServices& s) {
    const uint32_t args=e.arguments,battle=word(s,0x8041eb90);
    const int32_t id=actor(e,args,s);if(id==-1)return 2;
    const uint32_t u=unit(battle,id,s);assert(s.memory.half);const uint16_t raw=s.memory.half(s.memory.user,u+(maximum?0x108:0x10c));
    const int32_t hp=raw<0x8000?int32_t(raw):int32_t(raw)-65536,output=sw(word(s,args+4));
    assert(s.values.store);s.values.store(s.values.user,e,output,hp);return 2;
}
int32_t battleGetUnitWorkEvent(EventEntry& e,const BattleUnitEventServices& s) {
    const uint32_t args=e.arguments;const int32_t id=actor(e,args,s),index=resolve(e,args+4,s);
    const int32_t output=sw(word(s,args+8));const uint32_t battle=word(s,0x8041eb90),u=unit(battle,id,s);
    const int32_t value=sw(word(s,u+0x218+4u*uint32_t(index)));assert(s.values.store);s.values.store(s.values.user,e,output,value);return 2;
}
int32_t battleSetUnitWorkEvent(EventEntry& e,const BattleUnitEventServices& s) {
    const uint32_t args=e.arguments;const int32_t id=actor(e,args,s),index=resolve(e,args+4,s),value=resolve(e,args+8,s);
    const uint32_t battle=word(s,0x8041eb90),u=unit(battle,id,s);assert(s.memory.storeWord);
    s.memory.storeWord(s.memory.user,u+0x218+4u*uint32_t(index),uint32_t(value));return 2;
}
}
