#include "battle_packed_move_position.hpp"
#include <cassert>
#include <cstring>
namespace ttyd {
namespace {
uint32_t word(const EventMemoryServices& s,uint32_t a) {assert(s.word);return s.word(s.user,a);}
void word(const EventMemoryServices& s,uint32_t a,uint32_t v) {assert(s.storeWord);s.storeWord(s.user,a,v);}
int32_t sw(uint32_t v) {return v<=0x7fffffff?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}
float real(const EventMemoryServices& s,uint32_t a) {const uint32_t bits=word(s,a);float v;std::memcpy(&v,&bits,4);return v;}
void store(const EventMemoryServices& s,uint32_t a,float v) {uint32_t bits;std::memcpy(&bits,&v,4);word(s,a,bits);}
float negate(float v) {uint32_t bits;std::memcpy(&bits,&v,4);bits^=0x80000000;std::memcpy(&v,&bits,4);return v;}
std::array<float,3> vector(const EventMemoryServices& s,uint32_t a) {std::array<float,3> values;for(unsigned i=0;i<3;++i)values[i]=real(s,a+4*i);return values;}
void visible(uint32_t unit,const std::array<float,3>& values,const EventMemoryServices& s) {for(unsigned i=0;i<3;++i)store(s,unit+0x3c+4*i,values[i]);}
}
void setPackedMovePosition(uint32_t unit,bool target,const std::array<float,3>& input,const EventMemoryServices& s) {const auto values=input;const uint32_t offset=target?0x160:0x154;for(unsigned i=0;i<3;++i)if(values[i]!=real(s,0x804226c4))store(s,unit+offset+4*i,values[i]);}
void integratePackedMoveHorizontal(uint32_t x,uint32_t z,float speed,float heading,const BattlePackedMovePositionServices& s) {
    const auto& c=s.query.common;const auto& p=c.position;const auto& m=p.unit.memory;assert(p.multiply && p.add && c.divide && s.sine && s.cosine && s.roundDouble);
    const float tau=real(m,0x80422690),degrees=real(m,0x80422694),product=p.multiply(p.numericUser,tau,heading),rad=c.divide(p.numericUser,product,degrees);
    const double sinValue=s.sine(p.numericUser,double(rad));const float sine=s.roundDouble(p.numericUser,sinValue);const double cosValue=s.cosine(p.numericUser,double(rad));
    const float xStep=p.multiply(p.numericUser,speed,sine),oldX=real(m,x),cosine=s.roundDouble(p.numericUser,cosValue),negative=negate(speed),newX=p.add(p.numericUser,oldX,xStep),zStep=p.multiply(p.numericUser,negative,cosine);
    store(m,x,newX);const float oldZ=real(m,z);store(m,z,p.add(p.numericUser,oldZ,zStep));
}
int32_t packedMovePositionEvent(EventEntry& e,int32_t firstCall,const BattlePackedMovePositionServices& s) {
    const auto& c=s.query.common;const auto& p=c.position;const auto& u=p.unit;const auto& m=u.memory;const uint32_t args=e.arguments;
    assert(u.values.resolve && u.translate && u.unit && c.convert);
    auto resolve=[&](uint32_t raw) {return u.values.resolve(u.values.user,e,sw(raw));};auto convert=[&](int32_t v) {return c.convert(p.numericUser,v);};
    const int32_t actor=resolve(word(m,args)),id=u.translate(u.user,e,actor);
    const int32_t x=resolve(word(m,args+4));const uint32_t rawY=word(m,args+8);const float targetX=convert(x);
    const int32_t y=resolve(rawY);const uint32_t rawZ=word(m,args+12);const float targetY=convert(y);
    const int32_t z=resolve(rawZ);const uint32_t rawCount=word(m,args+16);const float targetZ=convert(z);
    const int32_t count=resolve(rawCount),soundMode=resolve(word(m,args+20)),soundType=resolve(word(m,args+24));
    const uint32_t battle=word(m,0x8041eb90),unit=u.unit(u.user,battle,id);
    if(firstCall!=0) {
        e.initialized=1;word(m,unit+0x16c,uint32_t(count));const auto current=vector(m,unit+0x3c);
        setPackedMovePosition(unit,false,current,m);setPackedMovePosition(unit,true,{targetX,targetY,targetZ},m);
        assert(s.heading && s.query.distance);const float heading=s.heading(s.query.user,current[0],current[2],targetX,targetZ);store(m,unit+0x17c,heading);
        const float distance=s.query.distance(s.query.user,current[0],current[2],targetX,targetZ);store(m,unit+0x180,distance);
        const float zero=real(m,0x80422280);assert(c.subtract);const float dx=c.subtract(p.numericUser,targetX,current[0]);
        if(dx>zero || dx<zero) {assert(m.storeByte);m.storeByte(m.user,unit+0x188,dx>zero?1:255);}
        const uint32_t storedCount=word(m,unit+0x16c);assert(c.divide && p.add);
        if(storedCount==0) {
            const float d=real(m,unit+0x180),old=real(m,unit+0x170),quotient=c.divide(p.numericUser,d,old);assert(u.truncateFloat);word(m,unit+0x16c,uint32_t(u.truncateFloat(u.user,quotient)));
            if(word(m,unit+0x16c)==0)word(m,unit+0x16c,1);
            const int32_t n=sw(word(m,unit+0x16c));const float speed=real(m,unit+0x170),factor=convert(n),freshDistance=real(m,unit+0x180);assert(c.negativeMultiplySubtract);
            const float residual=c.negativeMultiplySubtract(p.numericUser,factor,speed,freshDistance),denominator=convert(n),adjustment=c.divide(p.numericUser,residual,denominator);store(m,unit+0x170,p.add(p.numericUser,speed,adjustment));
        }else {const float d=real(m,unit+0x180),n=convert(sw(storedCount));store(m,unit+0x170,c.divide(p.numericUser,d,n));}
        loadPackedUnitSoundMode(soundType,soundMode,unit+0x1c4,unit+0x18c,m);
    }
    const float speed=real(m,unit+0x170),heading=real(m,unit+0x17c);integratePackedMoveHorizontal(unit+0x154,unit+0x15c,speed,heading,s);
    const auto baseline=vector(m,unit+0x154);visible(unit,baseline,m);word(m,unit+0x16c,word(m,unit+0x16c)-1);
    if(sw(word(m,unit+0x16c))>0) {controlPackedUnitMovementSound(unit,s.sound);return 0;}
    const auto target=vector(m,unit+0x160);visible(unit,target,m);endPackedUnitMovementSound(unit,s.sound);return 1;
}
}
