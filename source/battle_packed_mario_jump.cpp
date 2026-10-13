#include "battle_packed_mario_jump.hpp"
#include "battle_packed_status.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {
uint32_t word(const EventMemoryServices& m,uint32_t a) {assert(m.word);return m.word(m.user,a);}
void word(const EventMemoryServices& m,uint32_t a,uint32_t v) {assert(m.storeWord);m.storeWord(m.user,a,v);}
int32_t sw(uint32_t v) {return v<=0x7fffffff?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}
float real(const EventMemoryServices& m,uint32_t a) {const uint32_t bits=word(m,a);float v;std::memcpy(&v,&bits,4);return v;}
double wide(const EventMemoryServices& m,uint32_t a) {const uint64_t hi=word(m,a),lo=word(m,a+4),bits=hi<<32|lo;double v;std::memcpy(&v,&bits,8);return v;}
void store(const EventMemoryServices& m,uint32_t a,float v) {uint32_t bits;std::memcpy(&bits,&v,4);word(m,a,bits);}
float negate(float v) {uint32_t bits;std::memcpy(&bits,&v,4);bits^=0x80000000;std::memcpy(&v,&bits,4);return v;}
std::array<float,3> vector(const EventMemoryServices& m,uint32_t a) {std::array<float,3> values;for(unsigned i=0;i<3;++i)values[i]=real(m,a+4*i);return values;}
void visible(const EventMemoryServices& m,uint32_t unit,const std::array<float,3>& values) {for(unsigned i=0;i<3;++i)store(m,unit+0x3c+4*i,values[i]);}
struct Ops {
    const BattlePackedMarioJumpServices& s;const EventMemoryServices& m;void* user;
    float add(float a,float b) const {const auto& p=s.motion.query.common.position;assert(p.add);return p.add(user,a,b);}
    float sub(float a,float b) const {const auto& c=s.motion.query.common;assert(c.subtract);return c.subtract(user,a,b);}
    float mul(float a,float b) const {const auto& p=s.motion.query.common.position;assert(p.multiply);return p.multiply(user,a,b);}
    float div(float a,float b) const {const auto& c=s.motion.query.common;assert(c.divide);return c.divide(user,a,b);}
    float cvt(int32_t n) const {const auto& c=s.motion.query.common;assert(c.convert);return c.convert(user,n);}
    float round(double v) const {assert(s.motion.roundDouble);return s.motion.roundDouble(user,v);}
    float sine(float rad) const {assert(s.motion.sine);return round(s.motion.sine(user,double(rad)));}
    float fused(float a,float b,float c) const {const auto& p=s.motion.query.common.position;assert(p.fma);return p.fma(user,a,b,c);}
    float subtractProduct(float a,float b,float c) const {const auto& p=s.motion.query.common;assert(p.negativeMultiplySubtract);return p.negativeMultiplySubtract(user,a,b,c);}
    int32_t divideWord(int32_t a,int32_t b) const {assert(s.divideWord);return s.divideWord(user,a,b);}
    float radians(uint32_t unit) const {const float tau=real(m,0x804222c0),theta=real(m,unit+0x1b4),degrees=real(m,0x804222bc);return div(mul(tau,theta),degrees);}
    void advance(uint32_t unit,int32_t mode,bool initial) const {
        float rad;
        if(initial) {const float zero=real(m,0x80422280),tau=real(m,0x804222c0);store(m,unit+0x1bc,zero);const float degrees=real(m,0x804222bc);store(m,unit+0x1bc,zero);const float theta=real(m,unit+0x1b4);rad=div(mul(tau,theta),degrees);}else rad=radians(unit);
        const float q=sine(rad);const double coefficient=wide(m,mode<3?0x802ee718:0x802ee728),one=wide(m,0x802ee710);const float g=real(m,unit+0x174);assert(s.fma);
        const double factor=s.fma(user,coefficient,double(q),one);const float phi=real(m,unit+0x178);store(m,unit+0x178,round(s.fma(user,double(g),factor,double(phi))));
    }
    void theta(uint32_t unit) const {const float theta=real(m,unit+0x1b4),step=real(m,unit+0x1b8);store(m,unit+0x1b4,add(theta,step));store(m,unit+0x1b4,revisePackedJumpAngle(real(m,unit+0x1b4),s));}
};
}
int32_t packedACPossibility(uint32_t unit,const BattleActorServices& s) {assert(s.byte);if(s.byte(s.user,unit+0xc)!=0 || word(s.memory,unit+8)==223)return 0;for(int id:{1,2,9,27})if(checkPackedBattleStatus(unit,id,s))return 0;return unit && (word(s.memory,unit+0x138)&1)?0:1;}
float revisePackedJumpAngle(float input,const BattlePackedMarioJumpServices& s) {const auto& p=s.motion.query.common.position;const auto& m=p.unit.memory;assert(s.fmod && s.motion.roundDouble);const double degrees=wide(m,0x802bf240);float result=s.motion.roundDouble(p.numericUser,s.fmod(p.numericUser,double(input),degrees));if(result!=result)result=real(m,0x8041f448);const float zero=real(m,0x8041f448);if(result<zero) {const float degreesFloat=real(m,0x8041f45c);assert(p.add);result=p.add(p.numericUser,result,degreesFloat);if(result>=degreesFloat)result=zero;}return result;}
int32_t packedMarioJumpPositionEvent(EventEntry& e,int32_t first,const BattlePackedMarioJumpServices& s) {
    const auto& motion=s.motion;const auto& p=motion.query.common.position;const auto& u=p.unit;const auto& m=u.memory;const uint32_t args=e.arguments;Ops op{s,m,p.numericUser};assert(u.values.resolve && u.translate && u.unit);
    auto resolve=[&](uint32_t raw) {return u.values.resolve(u.values.user,e,sw(raw));};
    const int32_t actor=resolve(word(m,args)),id=u.translate(u.user,e,actor),x=resolve(word(m,args+4));const uint32_t ry=word(m,args+8);const float fx=op.cvt(x);
    const int32_t y=resolve(ry);const uint32_t rz=word(m,args+12);const float fy=op.cvt(y);
    const int32_t z=resolve(rz);const uint32_t rn=word(m,args+16);const float fz=op.cvt(z);
    const int32_t count=resolve(rn),mode=resolve(word(m,args+20)),sound=resolve(word(m,args+24));const uint32_t battle=word(m,0x8041eb90),unit=u.unit(u.user,battle,id);
    if(first) {
        e.initialized=1;if(mode==1)e.initialized=2;
        else {
            setPackedMovePosition(unit,true,{fx,fy,fz},m);const auto current=vector(m,unit+0x3c);setPackedMovePosition(unit,false,current,m);
            assert(motion.heading && motion.query.distance);store(m,unit+0x17c,motion.heading(motion.query.user,current[0],current[2],fx,fz));store(m,unit+0x180,motion.query.distance(motion.query.user,current[0],current[2],fx,fz));
            const float zero=real(m,0x80422280),dx=op.sub(fx,current[0]);if(dx>zero || dx<zero) {assert(m.storeByte);m.storeByte(m.user,unit+0x188,dx>zero?1:255);}word(m,unit+0x16c,uint32_t(count));
            const int32_t n=sw(word(m,unit+0x16c));if(n==0) {const float d=real(m,unit+0x180),speed=real(m,unit+0x170);assert(u.truncateFloat);word(m,unit+0x16c,uint32_t(u.truncateFloat(u.user,op.div(d,speed))));}else {const float d=real(m,unit+0x180),nf=op.cvt(n);store(m,unit+0x170,op.div(d,nf));}
            const int32_t nr=sw(word(m,unit+0x16c));const float speed=real(m,unit+0x170),distance=real(m,unit+0x180),nf=op.cvt(nr),residual=op.subtractProduct(nf,speed,distance);
            const int32_t setupN=sw(word(m,unit+0x16c));if(setupN==0)return 2;
            float zeroCurve=0,pi=0;
            for(unsigned i=0;i<3;++i) {const int32_t n= i==0?setupN:sw(word(m,unit+0x16c));const float target=real(m,unit+0x160+4*i),base=real(m,unit+0x154+4*i),diff=op.sub(target,base),divisor=op.cvt(n);if(i==0) {zeroCurve=real(m,0x80422280);pi=real(m,0x804222c4);}store(m,unit+0x1a8+4*i,op.div(diff,divisor));}
            store(m,unit+0x178,zeroCurve);store(m,unit+0x174,op.div(pi,op.cvt(sw(word(m,unit+0x16c)))));
            const int32_t adjustN=sw(word(m,unit+0x16c));const float old=real(m,unit+0x170),divisor=op.cvt(adjustN);store(m,unit+0x170,op.add(old,op.div(residual,divisor)));
            const float ninety=real(m,0x804222c8);float degrees=0;if(mode<3)degrees=real(m,0x804222bc);store(m,unit+0x1b4,ninety);
            const double twenty=wide(m,0x802ee6f0),six=wide(m,0x802ee6f8),fortySeven=wide(m,0x802ee700);const int32_t angularN=sw(word(m,unit+0x16c));store(m,unit+0x1b8,mode<3?op.div(degrees,op.cvt(angularN)):op.cvt(op.divideWord(360,angularN)));
            assert(s.subtract && s.divide && s.add);float height=op.round(s.subtract(op.user,double(real(m,unit+0x180)),twenty));height=op.round(s.divide(op.user,double(height),six));height=op.round(s.add(op.user,double(height),fortySeven));store(m,unit+0x184,height);
            if(mode==2 || mode==4) {const float h=real(m,unit+0x184);const double multiplier=wide(m,mode==2?0x802ee708:0x802ee720);assert(s.multiply);store(m,unit+0x184,op.round(s.multiply(op.user,double(h),multiplier)));}
            op.advance(unit,mode,true);loadPackedUnitSoundMode(2,sound,unit+0x1c4,unit+0x18c,m);const uint32_t key=word(m,unit+0x18c);
            if(key) {for(unsigned i=0;i<3;++i)word(m,0x802ee470+4*i);const auto baseline=vector(m,unit+0x154);assert(s.play);s.play(s.user,key,baseline);}
        }
    }
    const int32_t phase=e.initialized<128?e.initialized:int32_t(e.initialized)-256;
    if(phase==1) {
        for(unsigned i=0;i<3;++i) {const float base=real(m,unit+0x154+4*i),delta=real(m,unit+0x1a8+4*i);store(m,unit+0x154+4*i,op.add(base,delta));}
        auto v=vector(m,unit+0x3c);store(m,unit+0x1bc,v[1]);v[0]=real(m,unit+0x154);const float q=op.sine(real(m,unit+0x178)),height=real(m,unit+0x184),baseY=real(m,unit+0x158);v[1]=op.fused(height,q,baseY);v[2]=real(m,unit+0x15c);visible(m,unit,v);
        const float targetY=real(m,unit+0x164);if(targetY>v[1] && sw(word(m,unit+0x16c))<3) {v[1]=targetY;visible(m,unit,v);}store(m,unit+0x1bc,v[1]);op.advance(unit,mode,false);op.theta(unit);word(m,unit+0x16c,word(m,unit+0x16c)-1);if(word(m,unit+0x16c)!=0)return 0;
        v[1]=real(m,unit+0x164);visible(m,unit,v);const float last=real(m,unit+0x1bc),g=real(m,0x804222cc);store(m,unit+0x178,negate(op.sub(last,last)));store(m,unit+0x174,g);return 1;
    }
    if(phase==2) {
        assert(s.eligible);if(!s.eligible(s.user,unit))return 2;
        const float last=real(m,unit+0x1bc),g=real(m,0x804222cc),difference=op.sub(last,last),ninety=real(m,0x804222c8);const double divisor=wide(m,0x802ee730);
        store(m,unit+0x178,negate(difference));store(m,unit+0x174,g);word(m,unit+0x16c,1);store(m,unit+0x1b4,ninety);store(m,unit+0x1b8,op.cvt(op.divideWord(360,sw(word(m,unit+0x16c)))));
        const float current=real(m,unit+0x1bc),diff=op.sub(current,current);assert(s.divide);store(m,unit+0x184,op.round(s.divide(op.user,double(std::fabs(diff)),divisor)));
        for(unsigned i=0;i<3;++i)store(m,unit+0x154+4*i,real(m,unit+0x3c+4*i));
        e.initialized=3;return 0;
    }
    if(phase==3) {
        const float q1=op.sine(op.radians(unit)),height=real(m,unit+0x184);const double divisor=wide(m,0x802ee738);const float x=real(m,unit+0x154),product=op.mul(height,q1);assert(s.divide && s.add);
        const float tau=real(m,0x804222c0),degrees=real(m,0x804222bc);const double fraction=s.divide(op.user,double(product),divisor),sum=s.add(op.user,double(x),fraction);store(m,unit+0x154,op.round(sum));
        const float theta=real(m,unit+0x1b4),rad=op.div(op.mul(tau,theta),degrees),q2=op.sine(rad),h=real(m,unit+0x184),y=real(m,unit+0x158);store(m,unit+0x158,op.subtractProduct(h,q2,y));op.theta(unit);const auto baseline=vector(m,unit+0x154);visible(m,unit,baseline);word(m,unit+0x16c,word(m,unit+0x16c)-1);return word(m,unit+0x16c)==0?1:0;
    }
    return 0;
}
}
