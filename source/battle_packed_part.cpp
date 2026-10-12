#include "battle_packed_part.hpp"
#include <cassert>
#include <cstring>

namespace ttyd {
namespace {
uint32_t word(const EventMemoryServices& s,uint32_t a) {assert(s.word);return s.word(s.user,a);}
int32_t sw(uint32_t v) {return v<=0x7fffffff?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}
float real(const EventMemoryServices& s,uint32_t a) {const uint32_t bits=word(s,a);float value;std::memcpy(&value,&bits,4);return value;}
void store(const EventMemoryServices& s,uint32_t a,float value) {uint32_t bits;std::memcpy(&bits,&value,4);assert(s.storeWord);s.storeWord(s.user,a,bits);}
struct WorldInputs {bool absolute;uint32_t part,owner;std::array<float,3> base,offset;float direction;};
WorldInputs inputs(uint32_t part,const BattlePartPositionServices& s) {
    const auto& m=s.memory.memory;WorldInputs data{};data.part=part;
    const uint32_t flags=word(m,part+0x1ac);data.owner=word(m,part+0x4ec);data.absolute=(flags&0x10000000)!=0;
    if(data.absolute)return data;
    for(unsigned i=0;i<3;++i)data.base[i]=real(m,data.owner+0x3c+4*i);
    for(unsigned i=0;i<3;++i)data.offset[i]=real(m,part+0x24+4*i);
    assert(s.memory.byte);const uint8_t raw=s.memory.byte(s.memory.user,data.owner+0x189);
    data.direction=float(raw<128?int32_t(raw):int32_t(raw)-256);return data;
}
float coordinate(unsigned i,const WorldInputs& data,const BattlePartPositionServices& s) {
    if(data.absolute)return real(s.memory.memory,data.part+0x18+4*i);
    if(i==0) {assert(s.fma);return s.fma(s.numericUser,data.offset[0],data.direction,data.base[0]);}
    assert(s.add);return s.add(s.numericUser,data.base[i],data.offset[i]);
}
}
uint32_t packedUnitPart(uint32_t unit,int32_t id,const EventMemoryServices& s) {
    uint32_t part=word(s,unit+0x14);if(id<0)return part;
    if(word(s,part)==0)return part; // Original unchecked head dereference,single-part shortcut.
    while(part) {const uint32_t definition=word(s,part+4);if(word(s,definition)==uint32_t(id))return part;part=word(s,part);}
    return 0;
}
uint32_t packedRegistryPart(int32_t id,int32_t part,const BattleActorServices& s) {const uint32_t battle=currentBattleAddress(s),unit=battleRegistryUnit(battle,id,s);return packedUnitPart(unit,part,s.memory);}
void copyPackedPartPosition(uint32_t part,bool absolute,const std::array<uint32_t,3>& outputs,const EventMemoryServices& s) {
    assert(s.storeWord);const uint32_t offset=absolute?0x18:0x24;
    for(unsigned i=0;i<3;++i)s.storeWord(s.user,outputs[i],word(s,part+offset+4*i));
}
void packedPartWorldPosition(uint32_t part,const std::array<float*,3>& outputs,const BattlePartPositionServices& s) {const auto data=inputs(part,s);for(unsigned i=0;i<3;++i)*outputs[i]=coordinate(i,data,s);}
std::array<float,3> packedPartWorldPosition(uint32_t part,const BattlePartPositionServices& s) {const auto data=inputs(part,s);std::array<float,3> values;for(unsigned i=0;i<3;++i)values[i]=coordinate(i,data,s);return values;}
void writePackedPartWorldPosition(uint32_t part,const std::array<uint32_t,3>& outputs,const BattlePartPositionServices& s) {
    const auto data=inputs(part,s);
    if(data.absolute) {copyPackedPartPosition(part,true,outputs,s.memory.memory);return;}
    for(unsigned i=0;i<3;++i)store(s.memory.memory,outputs[i],coordinate(i,data,s));
}
int32_t packedPartPositionEvent(EventEntry& e,const BattlePartPositionServices& s) {
    const uint32_t args=e.arguments;const auto& u=s.unit;assert(u.values.resolve && u.translate && u.unit && u.values.store);
    const int32_t actor=u.values.resolve(u.values.user,e,sw(word(u.memory,args))),id=u.translate(u.user,e,actor);
    const int32_t partId=u.values.resolve(u.values.user,e,sw(word(u.memory,args+4)));
    const uint32_t firstBattle=word(u.memory,0x8041eb90);u.unit(u.user,firstBattle,id); // Original first lookup result is discarded.
    const uint32_t part=packedRegistryPart(id,partId,s.memory);
    const std::array<uint32_t,3> outputs{word(u.memory,args+8),word(u.memory,args+12),word(u.memory,args+16)};
    const auto coordinates=packedPartWorldPosition(part,s);
    for(unsigned i=0;i<3;++i)if(outputs[i]!=0xf1194d80) {assert(u.truncateFloat);const int32_t value=u.truncateFloat(u.user,coordinates[i]);u.values.store(u.values.user,e,sw(outputs[i]),value);}
    return 2;
}
}
