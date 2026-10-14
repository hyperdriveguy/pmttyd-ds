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
int32_t battleMarkUnit1feEvent(EventEntry& e,const BattleUnitEventServices& s){
    const auto args=e.arguments,battle=s.memory.word(s.memory.user,0x8041eb90);
    const auto id=actor(e,args,s);assert(s.unit);const auto unit=s.unit(s.user,battle,id);
    if(unit){assert(s.memory.half && s.memory.storeHalf);s.memory.storeHalf(s.memory.user,unit+0x1fe,uint16_t(s.memory.half(s.memory.user,unit+0x1fe)|1));}
    return 2;
}
int32_t battleGetHitDamageEvent(EventEntry& e,const BattleUnitEventServices& s,const BattleActorServices& bytes){
    const auto args=e.arguments;const auto id=actor(e,args,s);
    const auto battle=s.memory.word(s.memory.user,0x8041eb90);assert(s.unit);
    const auto unit=s.unit(s.user,battle,id);
    const auto output=signedWord(s.memory.word(s.memory.user,args+4));
    if(unit){assert(bytes.byte && s.values.store);const auto raw=bytes.byte(bytes.user,unit+0x270);
        s.values.store(s.values.user,e,output,raw<128?int32_t(raw):int32_t(raw)-256);}
    return 2;
}
int32_t battleUnitFlagEvent(EventEntry& e,bool on,const BattleUnitEventServices& s){
    const auto args=e.arguments;const auto id=actor(e,args,s);
    const auto mask=s.memory.word(s.memory.user,args+4);
    const auto battle=s.memory.word(s.memory.user,0x8041eb90);assert(s.unit && s.memory.storeWord);
    const auto unit=s.unit(s.user,battle,id),flags=s.memory.word(s.memory.user,unit+0x1c);
    s.memory.storeWord(s.memory.user,unit+0x1c,on?flags|mask:flags&~mask);return 2;
}
int32_t battleResetUnitFacingEvent(EventEntry& e,const BattleUnitEventServices& s,const BattleActorServices& bytes){
    assert(s.memory.word && s.memory.storeByte && bytes.byte && s.unit);
    const auto args=e.arguments,battle=s.memory.word(s.memory.user,0x8041eb90);
    const auto id=actor(e,args,s);const auto unit=s.unit(s.user,battle,id);
    const auto raw=bytes.byte(bytes.user,unit+0xc);
    const auto belong=raw<128?int32_t(raw):int32_t(raw)-256;
    const auto direction=bytes.byte(bytes.user,battle+uint32_t(belong*8)+0xa);
    s.memory.storeByte(s.memory.user,unit+0x189,direction);return 2;
}
int32_t battleCheckUnitAttributeEvent(EventEntry& e,const BattleUnitEventServices& s){
    const auto args=e.arguments;const auto id=actor(e,args,s);
    const auto mask=s.memory.word(s.memory.user,args+4);
    const auto output=signedWord(s.memory.word(s.memory.user,args+8));
    const auto battle=s.memory.word(s.memory.user,0x8041eb90);assert(s.unit && s.values.store);
    const auto unit=s.unit(s.user,battle,id);
    s.values.store(s.values.user,e,output,(s.memory.word(s.memory.user,unit+0x104)&mask)?1:0);return 2;
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
