#include "battle_stage_floor.hpp"
#include <cassert>
#include <cstring>
namespace ttyd {
namespace {
float real(uint32_t address,const EventMemoryServices& m){const auto bits=m.word(m.user,address);float value;std::memcpy(&value,&bits,4);return value;}
}
float packedBattleStageFloorHeight(uint32_t,float,float,float z,const EventMemoryServices& m){
    assert(m.word);const auto stage=m.word(m.user,0x8041eb90)+0x163fc;
    return z<=real(stage+0xb2c,m)?real(0x80422170,m):real(stage+0xb34,m);
}
void initializePackedBattleStageFloorFields(uint32_t battle,const EventMemoryServices& m){
    assert(m.word && m.storeWord);
    m.storeWord(m.user,battle+0x16f28,m.word(m.user,0x80422b44));
    m.storeWord(m.user,battle+0x16f30,m.word(m.user,0x80422b48));
}
int32_t battleStageFloorPositionEvent(EventEntry& e,const BattleUnitEventServices& s,
    void* numericUser,float (*convert)(void*,int32_t)){
    assert(s.memory.word && s.values.resolve && s.values.store && s.truncateFloat && convert);
    const auto args=e.arguments;
    const auto x=convert(numericUser,s.values.resolve(s.values.user,e,int32_t(s.memory.word(s.memory.user,args))));
    const auto destination=int32_t(s.memory.word(s.memory.user,args+4));
    const auto z=convert(numericUser,s.values.resolve(s.values.user,e,int32_t(s.memory.word(s.memory.user,args+8))));
    const auto battle=s.memory.word(s.memory.user,0x8041eb90);
    const auto floor=packedBattleStageFloorHeight(battle,x,real(0x80422170,s.memory),z,s.memory);
    s.values.store(s.values.user,e,destination,s.truncateFloat(s.user,floor));return 2;
}
}
