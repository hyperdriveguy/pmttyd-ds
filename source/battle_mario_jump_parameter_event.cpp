#include "battle_mario_jump_parameter_event.hpp"
#include <cassert>
#include <cstring>
namespace ttyd {
namespace {
uint32_t word(const EventMemoryServices& s,uint32_t a) {assert(s.word);return s.word(s.user,a);}
int32_t sw(uint32_t v) {return v<=0x7fffffff?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}
float real(const EventMemoryServices& s,uint32_t a) {const uint32_t bits=word(s,a);float value;std::memcpy(&value,&bits,4);return value;}
}
int32_t packedMarioJumpParameterEvent(EventEntry& e,const BattleMoveFrameServices& s) {
    const auto& c=s.common;const auto& p=c.position;const auto& u=p.unit;const auto& m=u.memory;const uint32_t args=e.arguments;
    assert(u.values.resolve && u.translate && u.unit && c.convert && s.distance);
    auto resolve=[&](uint32_t raw) {return u.values.resolve(u.values.user,e,sw(raw));};
    const int32_t actor=resolve(word(m,args)),id=u.translate(u.user,e,actor),integerX=resolve(word(m,args+4));
    const uint32_t rawY=word(m,args+8);const float x=c.convert(p.numericUser,integerX);resolve(rawY);
    const int32_t integerZ=resolve(word(m,args+12));const uint32_t destination=word(m,args+16);const float z=c.convert(p.numericUser,integerZ);
    const int32_t requested=resolve(destination);const uint32_t battle=word(m,0x8041eb90),unit=u.unit(u.user,battle,id);
    std::array<float,3> current;for(unsigned i=0;i<3;++i)current[i]=real(m,unit+0x3c+4*i);
    const float distance=s.distance(s.user,current[0],current[2],x,z);int32_t candidate=requested;
    if(requested==0) {const float speed=real(m,unit+0x170);assert(c.divide && u.truncateFloat);candidate=u.truncateFloat(u.user,c.divide(p.numericUser,distance,speed));}
    const float threshold=real(m,0x804222d4);
    if(distance>threshold) {
        assert(c.subtract && p.multiply && u.truncateFloat);const float difference=c.subtract(p.numericUser,distance,threshold),coefficient=real(m,0x804222d0),extra=p.multiply(p.numericUser,coefficient,difference);
        candidate=sw(uint32_t(candidate)+uint32_t(u.truncateFloat(u.user,extra)));
    }
    const int32_t result=requested==0 || candidate>requested?candidate:requested;
    if(result!=-250000000) {assert(u.values.store);u.values.store(u.values.user,e,sw(destination),result);}
    return 2;
}
}
