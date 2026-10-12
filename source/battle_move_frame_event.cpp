#include "battle_move_frame_event.hpp"
#include <cassert>
#include <cstring>
namespace ttyd {
namespace {
uint32_t word(const EventMemoryServices& s,uint32_t a) {assert(s.word);return s.word(s.user,a);}
int32_t sw(uint32_t v) {return v<=0x7fffffff?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}
float real(const EventMemoryServices& s,uint32_t a) {const uint32_t bits=word(s,a);float value;std::memcpy(&value,&bits,4);return value;}
}
int32_t packedMoveFrameEvent(EventEntry& e,const BattleMoveFrameServices& s) {
    const auto& c=s.common;const auto& p=c.position;const auto& u=p.unit;const auto& m=u.memory;const uint32_t args=e.arguments;
    assert(u.values.resolve && u.translate && u.values.resolveFloat && u.unit && c.convert);
    const int32_t actor=u.values.resolve(u.values.user,e,sw(word(m,args))),id=u.translate(u.user,e,actor);
    const int32_t integerX=u.values.resolve(u.values.user,e,sw(word(m,args+4)));
    const int32_t rawY=sw(word(m,args+8));const float x=c.convert(p.numericUser,integerX);u.values.resolve(u.values.user,e,rawY); // Original prefetch before X conversion;Y value discarded.
    const int32_t integerZ=u.values.resolve(u.values.user,e,sw(word(m,args+12)));
    const int32_t rawSpeed=sw(word(m,args+16));const float z=c.convert(p.numericUser,integerZ);float speed=u.values.resolveFloat(u.values.user,e,rawSpeed);
    const uint32_t output=word(m,args+20),battle=word(m,0x8041eb90),unit=u.unit(u.user,battle,id);
    std::array<float,3> current;for(unsigned i=0;i<3;++i)current[i]=real(m,unit+0x3c+4*i);
    assert(s.distance);const float distance=s.distance(s.user,current[0],current[2],x,z);
    const float firstZero=real(m,0x80422280);
    if(speed==firstZero) {speed=real(m,unit+0x170);if(speed==firstZero)speed=real(m,0x8042228c);}
    assert(c.divide && u.truncateFloat && c.negativeMultiplySubtract);
    const float quotient=c.divide(p.numericUser,distance,speed),secondZero=real(m,0x80422280);
    int32_t count=u.truncateFloat(u.user,quotient);const float converted=c.convert(p.numericUser,count);
    const float residual=c.negativeMultiplySubtract(p.numericUser,converted,speed,distance);
    if(residual>secondZero)count=sw(uint32_t(count)+1);
    if(count==0)count=1;
    if(output!=0xf1194d80) {assert(u.values.store);u.values.store(u.values.user,e,sw(output),count);}
    return 2;
}
}
