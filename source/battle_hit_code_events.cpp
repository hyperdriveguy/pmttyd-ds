#include "battle_hit_code_events.hpp"
#include <cassert>
namespace ttyd {
namespace {
int32_t signedWord(uint32_t value){return value<=0x7fffffffu?int32_t(value):int32_t(int64_t(value)-0x100000000LL);}
int32_t actor(EventEntry& e,uint32_t args,const BattleUnitEventServices& s){
    assert(s.memory.word && s.values.resolve && s.translate);
    const auto selector=s.values.resolve(s.values.user,e,signedWord(s.memory.word(s.memory.user,args)));
    return s.translate(s.user,e,selector);
}
}
int32_t battleCheckHitCodeEvent(EventEntry& e,const BattleUnitEventServices& s){
    const auto args=e.arguments;const auto id=actor(e,args,s);
    const auto mask=s.memory.word(s.memory.user,args+4);
    const auto output=signedWord(s.memory.word(s.memory.user,args+8));
    const auto battle=s.memory.word(s.memory.user,0x8041eb90);assert(s.unit && s.values.store);
    const auto unit=s.unit(s.user,battle,id);
    s.values.store(s.values.user,e,output,(s.memory.word(s.memory.user,unit+0x278)&mask)?1:0);return 2;
}
int32_t battleCheckHitPatternEvent(EventEntry& e,const BattleUnitEventServices& s){
    const auto args=e.arguments;const auto id=actor(e,args,s);
    const auto pattern=s.memory.word(s.memory.user,args+4);
    const auto output=signedWord(s.memory.word(s.memory.user,args+8));
    const auto battle=s.memory.word(s.memory.user,0x8041eb90);assert(s.unit && s.values.store);
    const auto unit=s.unit(s.user,battle,id);
    s.values.store(s.values.user,e,output,s.memory.word(s.memory.user,unit+0x274)==pattern?1:0);return 2;
}
int32_t battleGetHitCodeEvent(EventEntry& e,const BattleUnitEventServices& s){
    const auto args=e.arguments;const auto id=actor(e,args,s);
    const auto output=signedWord(s.memory.word(s.memory.user,args+4));
    const auto battle=s.memory.word(s.memory.user,0x8041eb90);assert(s.unit && s.values.store);
    const auto unit=s.unit(s.user,battle,id);
    s.values.store(s.values.user,e,output,int32_t(s.memory.word(s.memory.user,unit+0x278)&255));return 2;
}
}
