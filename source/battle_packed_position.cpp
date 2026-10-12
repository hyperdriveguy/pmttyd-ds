#include "battle_packed_position.hpp"
#include <cassert>
#include <cstring>

namespace ttyd {
namespace {
uint32_t word(const EventMemoryServices& s,uint32_t a) {assert(s.word);return s.word(s.user,a);}
int32_t sw(uint32_t v) {return v<=0x7fffffff?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}
}
void storePackedMovementFloat(uint32_t unit,bool acceleration,float value,const EventMemoryServices& s) {
    uint32_t bits;std::memcpy(&bits,&value,4);assert(s.storeWord);s.storeWord(s.user,unit+(acceleration?0x174:0x170),bits);
}
int32_t packedMovementSetterEvent(EventEntry& e,bool acceleration,const BattleUnitEventServices& s) {
    const uint32_t args=e.arguments;assert(s.values.resolve && s.translate && s.values.resolveFloat && s.unit);
    const int32_t actor=s.values.resolve(s.values.user,e,sw(word(s.memory,args)));
    const int32_t id=s.translate(s.user,e,actor);
    const float value=s.values.resolveFloat(s.values.user,e,sw(word(s.memory,args+4)));
    const uint32_t battle=word(s.memory,0x8041eb90),unit=s.unit(s.user,battle,id);
    storePackedMovementFloat(unit,acceleration,value,s.memory);return 2;
}
void copyPackedUnitPosition(uint32_t unit,bool home,const std::array<uint32_t,3>& outputs,const EventMemoryServices& s) {
    assert(s.storeWord);const uint32_t offset=home?0x30:0x3c;
    for(unsigned i=0;i<3;++i)s.storeWord(s.user,outputs[i],word(s,unit+offset+4*i));
}
int32_t packedSetUnitPositionEvent(EventEntry& e,const BattleUnitEventServices& s) {
    const uint32_t args=e.arguments;assert(s.values.resolve && s.translate && s.unit && s.memory.storeWord);
    const int32_t actor=s.values.resolve(s.values.user,e,sw(word(s.memory,args))),id=s.translate(s.user,e,actor);
    std::array<uint32_t,3> raw,coordinates;
    for(unsigned i=0;i<3;++i) {raw[i]=word(s.memory,args+4+4*i);const int32_t integer=s.values.resolve(s.values.user,e,sw(raw[i]));const float value=float(integer);std::memcpy(&coordinates[i],&value,4);}
    const uint32_t battle=word(s.memory,0x8041eb90),unit=s.unit(s.user,battle,id);
    const std::array<uint32_t,3> previous{word(s.memory,unit+0x3c),word(s.memory,unit+0x40),word(s.memory,unit+0x44)};
    for(unsigned i=0;i<3;++i)if(raw[i]==0xf1194d80)coordinates[i]=previous[i];
    for(unsigned i=0;i<3;++i)s.memory.storeWord(s.memory.user,unit+0x3c+4*i,coordinates[i]);
    return 2;
}
int32_t packedUnitPositionEvent(EventEntry& e,bool home,const BattleUnitEventServices& s) {
    const uint32_t args=e.arguments;assert(s.values.resolve && s.translate && s.unit && s.values.store);
    const int32_t actor=s.values.resolve(s.values.user,e,sw(word(s.memory,args)));
    const int32_t id=s.translate(s.user,e,actor);
    const uint32_t battle=word(s.memory,0x8041eb90),unit=s.unit(s.user,battle,id);
    const std::array<uint32_t,3> outputs{word(s.memory,args+4),word(s.memory,args+8),word(s.memory,args+12)};
    const uint32_t offset=home?0x30:0x3c;
    std::array<float,3> coordinates;
    for(unsigned i=0;i<3;++i) {const uint32_t bits=word(s.memory,unit+offset+4*i);std::memcpy(&coordinates[i],&bits,4);}
    for(unsigned i=0;i<3;++i) {
        if(!home && outputs[i]==0xf1194d80)continue;
        assert(s.truncateFloat);const int32_t value=s.truncateFloat(s.user,coordinates[i]);
        s.values.store(s.values.user,e,sw(outputs[i]),value);
    }
    return 2;
}
}
