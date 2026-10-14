#include "battle_audience_sound_main.hpp"
#include <cassert>
#include <cstring>
namespace ttyd {
namespace {
int32_t integer(uint32_t v){int32_t n;std::memcpy(&n,&v,4);return n;}
float rounded(float f){volatile float value=f;return value;}
float interpolate(int32_t step,int32_t duration,uint8_t start,uint8_t end) {
    if(!duration)return float(end);
    const auto delta=rounded(float(end)-float(start));
    const auto product=rounded(float(step)*delta);
    return rounded(float(start)+rounded(product/float(duration)));
}
}
bool packedBattleAudienceSoundMain(const BattleAudienceSoundMainServices& s) {
    const auto& m=s.actors.memory;assert(m.word && m.storeWord && m.storeByte && s.actors.byte);
    (void)currentBattleAddress(s.actors); // Initial BaseGetPtr before NoiseMain.
    if(!s.noise || !s.noise(s.user))return false;
    auto r=currentBattleAddress(s.actors)+0x2858+0x133fc;
    for(unsigned i=0;i<24;++i,r+=0x24) {
        auto word=[&](unsigned o){return m.word(m.user,r+o);};
        auto byte=[&](unsigned o){return s.actors.byte(s.actors.user,r+o);};
        auto store=[&](unsigned o,uint32_t v){m.storeWord(m.user,r+o,v);};
        auto clear=[&](){store(0,word(0)&~1u);};
        if(!s.check)return false;
        const auto valid=s.check(s.user,integer(word(4)));if(!valid)return false;
        if(*valid==-1){clear();continue;}
        if(integer(word(0xc))==0) {
            if(!s.stop || !s.stop(s.user,integer(word(4))))return false;
            clear();continue;
        }
        auto volume=[&](int32_t step,int32_t duration)->bool {
            if(!s.truncate)return false;
            const auto value=s.truncate(s.user,interpolate(step,duration,byte(0x1f),byte(0x20)));
            if(!value)return false;
            m.storeByte(m.user,r+0x1e,uint8_t(*value));
            const auto product=uint32_t(byte(0x1d))*uint32_t(byte(0x1e));
            return s.setVolume && s.setVolume(s.user,integer(word(4)),uint8_t(integer(product)/100));
        };
        if(integer(word(0x18))>0) {
            store(0x14,word(0x14)+1);
            if(!volume(integer(word(0x14)),integer(word(0x18))))return false;
            if(integer(word(0x14))>=integer(word(0x18)))store(0x18,0);
        }
        if(integer(word(0xc))>0) {
            if(word(0xc)==word(0x10)) {
                m.storeByte(m.user,r+0x1f,byte(0x1e));m.storeByte(m.user,r+0x20,0);
            }
            if(integer(word(0xc))<=integer(word(0x10)) &&
                !volume(integer(word(0x10)-word(0xc)),integer(word(0x10))))return false;
            store(0xc,word(0xc)-1);
        }
    }
    return true;
}
}
