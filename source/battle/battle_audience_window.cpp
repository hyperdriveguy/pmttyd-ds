#include "battle_audience_window.hpp"
#include <cassert>
#include <cstring>
namespace ttyd {
namespace {
float number(uint32_t v){float f;std::memcpy(&f,&v,4);return f;}
uint32_t bits(float f){uint32_t v;std::memcpy(&v,&f,4);return v;}
int32_t integer(uint32_t v){int32_t n;std::memcpy(&n,&v,4);return n;}
// Explicit stores retain the PPC single rounding boundaries even on hosts
// that contract arithmetic or evaluate intermediates at wider precision.
float rounded(float f){volatile float value=f;return value;}
}
void packedBattleAudienceWindowMain(const BattleActorServices& s) {
    const auto& m=s.memory;assert(m.word && m.storeWord && s.byte);
    const auto a=currentBattleAddress(s)+0x2858;
    const auto v=currentBattleAddress(s)+0x2858+0x1375c;
    auto real=[&](uint32_t x){return number(m.word(m.user,x));};
    auto store=[&](uint32_t x,float f){m.storeWord(m.user,x,bits(f));};
    const auto count=rounded(float(integer(m.word(m.user,a+0x13784))));
    const auto difference=rounded(count-real(v+0x18));
    const auto zero=real(0x80424988),tenth=real(0x804249e8);
    if(difference>zero) {
        const auto scaled=rounded(tenth*difference),change=scaled<tenth?tenth:scaled;
        store(v+0x18,rounded(real(v+0x18)+change));
        const auto fresh=rounded(float(integer(m.word(m.user,a+0x13784))));
        if(real(v+0x18)>fresh)store(v+0x18,fresh);
    } else if(difference<zero) {
        const auto scaled=rounded(real(0x80424a2c)*difference),change=scaled<tenth?tenth:scaled;
        store(v+0x18,rounded(real(v+0x18)-change));
        const auto fresh=rounded(float(integer(m.word(m.user,a+0x13784))));
        if(real(v+0x18)<fresh)store(v+0x18,fresh);
    }
    if(s.byte(s.user,v)) {
        m.storeWord(m.user,v+0x14,m.word(m.user,v+0x14)+1);
        if(integer(m.word(m.user,v+0x14))>15)m.storeWord(m.user,v+0x14,15);
    } else {
        m.storeWord(m.user,v+0x14,m.word(m.user,v+0x14)-1);
        if(integer(m.word(m.user,v+0x14))<0)m.storeWord(m.user,v+0x14,0);
    }
    const auto step=rounded(float(integer(m.word(m.user,v+0x14))));
    const auto start=real(0x80424a30),end=real(0x80424a34);
    const auto remainder=rounded(15.f-step),delta=rounded(end-start);
    const auto squaredDuration=rounded(15.f*15.f),squaredRemainder=rounded(remainder*remainder);
    const auto total=rounded(start+delta),product=rounded(squaredRemainder*delta);
    store(v+8,rounded(total-rounded(product/squaredDuration)));
    store(v+0xc,real(0x80424a38));store(v+0x10,zero);
}
}
