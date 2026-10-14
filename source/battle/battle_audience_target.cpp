#include "battle_audience_target.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {
float number(uint32_t v){float f;std::memcpy(&f,&v,4);return f;}
uint32_t bits(float f){uint32_t v;std::memcpy(&v,&f,4);return v;}
int32_t signedWord(uint32_t v){int32_t n;std::memcpy(&n,&v,4);return n;}
}
bool packedBattleAudienceAddTargetNumSub(float delta,const BattleActorServices& s) {
    const auto& m=s.memory;assert(m.word && m.storeWord);
    const auto a=currentBattleAddress(s)+0x2858;
    auto real=[&](uint32_t x){return number(m.word(m.user,x));};
    const auto zero=real(0x80424988);if(delta==zero)return true;
    if(!std::isfinite(delta))return false;
    const auto one=real(0x80424990),negativeOne=real(0x804249d4),step=real(0x804249ac);
    do {
        float chunk=zero;
        if(delta>zero){chunk=delta<one?delta:one;delta=delta-chunk;if(delta<zero)delta=zero;}
        else if(delta<zero){chunk=delta>negativeOne?delta:negativeOne;delta=delta-chunk;if(delta>zero)delta=zero;}
        if(chunk!=zero) {
            const auto desired=real(a+0x1377c),baseline=real(a+0x13778);
            const float ratio=desired/baseline,difference=ratio-one,q=difference/step;
            if(!std::isfinite(q) || q>=2147483648.f || q<-2147483648.f)return false;
            const auto integer=int32_t(q);float result;
            if(chunk>zero && integer>=1) {
                const float denominator=float(signedWord(uint32_t(integer)+1));
                const float change=chunk/denominator;result=desired+change;
            } else if(chunk<zero && integer<=-1) {
                const float denominator=float(signedWord(uint32_t(integer)-1));
                const float change=chunk/denominator;result=desired-change;
            } else result=desired+chunk;
            m.storeWord(m.user,a+0x1377c,bits(result));
        }
    } while(delta!=zero);
    return true;
}
bool packedBattleAudienceAddTargetNum(float delta,float bias,const BattleActorServices& s) {
    const auto& m=s.memory;assert(m.word && m.storeWord);
    const auto a=currentBattleAddress(s)+0x2858;
    auto real=[&](uint32_t x){return number(m.word(m.user,x));};
    auto store=[&](uint32_t x,float v){m.storeWord(m.user,x,bits(v));};
    const auto zero=real(0x80424988);float offset=real(a+0x13780)+bias;store(a+0x13780,offset);
    if(offset>zero && delta<zero){offset=offset+delta;store(a+0x13780,offset);if(offset<zero){delta=offset;store(a+0x13780,zero);}else delta=zero;}
    else if(offset<zero && delta>zero){offset=offset+delta;store(a+0x13780,offset);if(offset>zero){delta=offset;store(a+0x13780,zero);}else delta=zero;}
    if(!packedBattleAudienceAddTargetNumSub(delta,s))return false;
    auto target=real(a+0x1377c);if(target<zero)target=zero;
    const auto capacity=float(signedWord(m.word(m.user,a+0x13790)));if(target>capacity)target=capacity;store(a+0x1377c,target);
    offset=real(a+0x13780);const auto minimum=real(0x804249d8),maximum=real(0x804249dc);
    if(offset<minimum)offset=minimum;
    if(offset>maximum)offset=maximum;
    store(a+0x13780,offset);return true;
}
}
