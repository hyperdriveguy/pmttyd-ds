#include "battle_hp_gauge.hpp"
#include <cassert>
#include <cstring>
namespace ttyd {
namespace {
int16_t integer(uint16_t v){int16_t n;std::memcpy(&n,&v,2);return n;}
int32_t integer(uint32_t v){int32_t n;std::memcpy(&n,&v,4);return n;}
float number(uint32_t v){float f;std::memcpy(&f,&v,4);return f;}
uint32_t bits(float f){uint32_t v;std::memcpy(&v,&f,4);return v;}
float rounded(float f){volatile float value=f;return value;}
}
void initializePackedBattleHpGauge(uint32_t u,const BattleActorServices& s) {
    const auto& m=s.memory;assert(m.half && m.word && m.storeHalf && m.storeWord);
    m.storeHalf(m.user,u+0x200,0);m.storeHalf(m.user,u+0x204,0);m.storeHalf(m.user,u+0x1fc,0);
    m.storeWord(m.user,u+0x208,uint32_t(integer(m.half(m.user,u+0x10c))));
    m.storeWord(m.user,u+0x20c,uint32_t(integer(m.half(m.user,u+0x10c))));
    const auto ratio=rounded(float(integer(m.half(m.user,u+0x10c)))/float(integer(m.half(m.user,u+0x108))));
    m.storeWord(m.user,u+0x210,bits(ratio));m.storeWord(m.user,u+0x214,bits(ratio));
    m.storeHalf(m.user,u+0x202,m.half(m.user,u+0x1fc)==1?40:20);
    m.storeHalf(m.user,u+0x206,uint16_t(int32_t(integer(m.half(m.user,u+0x202)))+30));
}
float battleHpGaugeCurve(int32_t mode,int32_t step,int32_t duration,float start,float end) {
    assert(mode==0 || mode==2);if(!duration)return end;
    const auto delta=rounded(end-start),n=float(duration),t=float(step);
    float numerator,denominator;
    if(mode==2){const auto squareT=rounded(t*t),squareN=rounded(n*n);numerator=rounded(rounded(t*squareT)*delta);denominator=rounded(n*squareN);}
    else {numerator=rounded(t*delta);denominator=n;}
    return rounded(start+rounded(numerator/denominator));
}
bool packedBattleHpGaugeMain(uint32_t u,const BattleHpGaugeServices& s) {
    const auto& m=s.actors.memory;assert(m.half && m.word && m.storeHalf && m.storeWord);
    auto half=[&](unsigned o){return integer(m.half(m.user,u+o));};
    auto store=[&](unsigned o,float f){m.storeWord(m.user,u+o,bits(f));};
    if(half(0x200)>0) {
        m.storeHalf(m.user,u+0x200,uint16_t(half(0x200)-1));
        if(half(0x200)<=0) {
            m.storeWord(m.user,u+0x208,m.word(m.user,u+0x20c));
            const auto ratio=rounded(float(half(0x10c))/float(half(0x108)));
            store(0x214,ratio);store(0x210,ratio);
        } else {
            const bool modeOne=m.half(m.user,u+0x1fc)==1;
            int16_t retainedMax=0;
            if(modeOne)store(0x210,rounded(float(half(0x10c))/float(half(0x108))));
            else {store(0x214,0);retainedMax=half(0x108);}
            if(!s.interpolate)return false;
            const auto curve=s.interpolate(s.user,modeOne?2:0,half(0x200),half(0x202),number(m.word(m.user,0x804226a4)),number(m.word(m.user,0x804226a8)));
            if(!curve)return false;
            const auto difference=integer(m.word(m.user,u+0x208)-m.word(m.user,u+0x20c));
            const auto curveValue=*curve,product=rounded(float(difference)*curveValue);
            if(modeOne)store(0x214,rounded(number(m.word(m.user,u+0x210))+rounded(product/float(half(0x108)))));
            else {
                const auto current=half(0x10c);
                const auto contribution=rounded(product/float(retainedMax));
                store(0x210,rounded(rounded(float(current)/float(retainedMax))+contribution));
            }
        }
    }
    if(half(0x204)>0)m.storeHalf(m.user,u+0x204,uint16_t(half(0x204)-1));
    if((m.word(m.user,u+0x1c)&1) && half(0x204)<=0 && (m.half(m.user,u+0x1fe)&1)) {
        m.storeWord(m.user,u+0x1c,m.word(m.user,u+0x1c)&~1u);
        m.storeHalf(m.user,u+0x1fe,m.half(m.user,u+0x1fe)&~1u);
    }
    return true;
}
}
