#include "battle_stage_floor.hpp"
#include <cassert>
#include <cstring>
namespace ttyd {
namespace {
float real(uint32_t address,const EventMemoryServices& m){const auto bits=m.word(m.user,address);float value;std::memcpy(&value,&bits,4);return value;}
}
// Deliberately recapture global B: z <= stage+0xb2c uses original constant
// 0x80422170, otherwise stage+0xb34. Passed B/X/Y do not affect this step.
// Equality takes the first branch; unordered (NaN) z takes the second.
float packedBattleStageFloorHeight(uint32_t,float,float,float z,const EventMemoryServices& m){
    assert(m.word);const auto stage=m.word(m.user,0x8041eb90)+0x163fc;
    return z<=real(stage+0xb2c,m)?real(0x80422170,m):real(stage+0xb34,m);
}
// These absolute offsets alias stage+0xb2c/+0xb34. Copy source words from
// 0x80422b44/48; do not initialize the remaining stage or its children here.
void initializePackedBattleStageFloorFields(uint32_t battle,const EventMemoryServices& m){
    assert(m.word && m.storeWord);
    m.storeWord(m.user,battle+0x16f28,m.word(m.user,0x80422b44));
    m.storeWord(m.user,battle+0x16f30,m.word(m.user,0x80422b48));
}
// Resolve/convert X, read raw output, then resolve/convert Z before capturing
// B. The floor getter then resolves B again. Truncate through the admitted
// numeric service and return EVT completion 2, without an output-skip sentinel.
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
