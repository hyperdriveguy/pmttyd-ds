#include "battle_packed_hit.hpp"
#include <cassert>
#include <cstring>

namespace ttyd {
namespace {
uint32_t word(const EventMemoryServices& s,uint32_t address) {assert(s.word);return s.word(s.user,address);}
int32_t signedWord(uint32_t value) {return value<=0x7fffffff?int32_t(value):int32_t(int64_t(value)-0x100000000LL);}
int32_t signedByte(uint8_t value) {return value<128?int32_t(value):int32_t(value)-256;}
float real(const EventMemoryServices& s,uint32_t address) {const uint32_t bits=word(s,address);float value;std::memcpy(&value,&bits,4);return value;}
void store(const EventMemoryServices& s,uint32_t address,float value) {uint32_t bits;std::memcpy(&bits,&value,4);assert(s.storeWord);s.storeWord(s.user,address,bits);}
template<class World,class Read,class Write>
void hit(uint32_t unit,uint32_t part,const BattlePartPositionServices& s,World world,Read read,Write write) {
    const auto& m=s.memory.memory;assert(s.memory.byte);
    const int32_t belong=signedByte(s.memory.byte(s.memory.user,unit+0xc));
    const uint32_t battle=currentBattleAddress(s.memory);
    const float factor=float(signedByte(s.memory.byte(s.memory.user,battle+uint32_t(belong)*8+0xa)));
    world();
    assert(s.multiply && s.fma);
    float yFactor=0;
    for(unsigned layer=0;layer<2;++layer)for(unsigned axis=0;axis<3;++axis) {
        const float offset=real(m,part+0x170+12*layer+4*axis),scale=real(m,unit+0x114);
        float current,product=0;
        // The first X multiplication precedes its output read. The other X/Y
        // multiplications follow the output read; Z has no rounded product.
        if(layer==0 && axis==0) {
            product=s.multiply(s.numericUser,offset,scale);current=read(axis);
            yFactor=real(m,0x804226a8);
        } else {
            current=read(axis);
            if(axis!=2)product=s.multiply(s.numericUser,offset,scale);
        }
        const float value=axis==2?s.fma(s.numericUser,offset,scale,current):s.fma(s.numericUser,axis==0?factor:yFactor,product,current);
        write(axis,value);
    }
}
}
std::array<float,3> packedHitPosition(uint32_t unit,uint32_t part,const BattlePartPositionServices& s) {
    std::array<float,3> values{};
    hit(unit,part,s,[&] {values=packedPartWorldPosition(part,s);},[&](unsigned i) {return values[i];},[&](unsigned i,float v) {values[i]=v;});return values;
}
void writePackedHitPosition(uint32_t unit,uint32_t part,const std::array<uint32_t,3>& outputs,const BattlePartPositionServices& s) {
    const auto& m=s.memory.memory;
    hit(unit,part,s,[&] {writePackedPartWorldPosition(part,outputs,s);},[&](unsigned i) {return real(m,outputs[i]);},[&](unsigned i,float v) {store(m,outputs[i],v);});
}
int32_t packedHitPositionEvent(EventEntry& e,const BattlePartPositionServices& s) {
    const uint32_t args=e.arguments;const auto& u=s.unit;assert(u.values.resolve && u.translate && u.unit && u.values.store);
    const int32_t actor=u.values.resolve(u.values.user,e,signedWord(word(u.memory,args))),id=u.translate(u.user,e,actor);
    const int32_t partId=u.values.resolve(u.values.user,e,signedWord(word(u.memory,args+4)));
    const uint32_t firstBattle=word(u.memory,0x8041eb90),unit=u.unit(u.user,firstBattle,id);
    const uint32_t part=packedRegistryPart(id,partId,s.memory);
    const std::array<uint32_t,3> outputs{word(u.memory,args+8),word(u.memory,args+12),word(u.memory,args+16)};
    const auto coordinates=packedHitPosition(unit,part,s);
    for(unsigned i=0;i<3;++i)if(outputs[i]!=0xf1194d80) {assert(u.truncateFloat);const int32_t value=u.truncateFloat(u.user,coordinates[i]);u.values.store(u.values.user,e,signedWord(outputs[i]),value);}
    return 2;
}
}
