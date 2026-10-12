#include "battle_packed_takeoff.hpp"
#include <cassert>
#include <cmath>
#include <cstring>

namespace ttyd {
namespace {
uint32_t word(const EventMemoryServices& s,uint32_t address) {assert(s.word);return s.word(s.user,address);}
int32_t signedWord(uint32_t value) {return value<=0x7fffffff?int32_t(value):int32_t(int64_t(value)-0x100000000LL);}
int32_t signedByte(uint32_t value) {value&=255;return value<128?int32_t(value):int32_t(value)-256;}
float real(const EventMemoryServices& s,uint32_t address) {const uint32_t bits=word(s,address);float value;std::memcpy(&value,&bits,4);return value;}
}
int32_t packedEnemyBelong(uint32_t unit,const BattleActorServices& s) {assert(s.byte);const int32_t belong=signedByte(s.byte(s.user,unit+0xc));return belong==0?1:belong==1?0:belong;}
int32_t packedTakeoffPositionEvent(EventEntry& e,const BattlePackedTakeoffServices& s) {
    const auto& p=s.position;const auto& u=p.unit;const auto& m=u.memory;const uint32_t args=e.arguments;
    const uint32_t battle=word(m,0x8041eb90);
    assert(u.values.resolve && u.translate && u.unit && u.values.store && s.core.sample && s.core.choose);
    const int32_t actorRaw=u.values.resolve(u.values.user,e,signedWord(word(m,args)));
    const int32_t targetRaw=u.values.resolve(u.values.user,e,signedWord(word(m,args+4)));
    const int32_t partId=u.values.resolve(u.values.user,e,signedWord(word(m,args+8)));
    const std::array<uint32_t,3> outputs{word(m,args+12),word(m,args+16),word(m,args+20)};
    const int32_t actorId=u.translate(u.user,e,actorRaw);const uint32_t actor=u.unit(u.user,battle,actorId);
    float currentX=real(m,actor+0x3c),ignored=real(m,actor+0x40);ignored=real(m,actor+0x44);
    const int32_t targetId=u.translate(u.user,e,targetRaw);const uint32_t target=u.unit(u.user,battle,targetId);
    const uint32_t requested=packedUnitPart(target,partId,m);
    std::array<float,3> position;for(unsigned i=0;i<3;++i)position[i]=real(m,actor+0x30+4*i);
    packedHitPosition(target,requested,{&position[0],&ignored,&position[2]},p);
    const int32_t belong=signedByte(uint32_t(packedEnemyBelong(actor,p.memory)));
    const uint32_t sideAddress=battle+uint32_t(belong)*8+0xa;assert(p.memory.byte);
    const uint8_t side=p.memory.byte(p.memory.user,sideAddress);
    BattleCandidateBuffer buffer; // Stack-owned semantic storage; providers initialize all readable fields.
    const int32_t sampled=s.core.sample(s.core.user,buffer,0,actorId,belong,0x01001240,0x01000000,signedByte(0u-side));
    int32_t chosenUnit,chosenPart;s.core.choose(s.core.user,buffer,4,chosenUnit,chosenPart);
    int32_t magnitude=60;
    if(signedByte(buffer.bytes[0xa6c])>0) {
        chosenUnit=u.translate(u.user,e,chosenUnit);const uint32_t candidate=u.unit(u.user,battle,chosenUnit);
        const uint32_t part=packedUnitPart(candidate,chosenPart,m);float chosenX;
        packedHitPosition(candidate,part,{&chosenX,&ignored,&ignored},p);
        assert(s.subtract);
        const float originalDistance=s.subtract(p.numericUser,currentX,position[0]),chosenDistance=s.subtract(p.numericUser,currentX,chosenX);
        if(std::fabs(originalDistance)>std::fabs(chosenDistance))position[0]=chosenX;
        magnitude=requested==part?60:30;if(sampled!=0)magnitude=-magnitude;
    }
    const int32_t factor=signedByte(p.memory.byte(p.memory.user,sideAddress));assert(p.add);
    position[0]=p.add(p.numericUser,position[0],float(magnitude*factor));
    for(unsigned i=0;i<3;++i) {assert(u.truncateFloat);const int32_t value=u.truncateFloat(u.user,position[i]);u.values.store(u.values.user,e,signedWord(outputs[i]),value);}
    return 2;
}
}
