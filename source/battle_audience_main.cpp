#include "battle_audience_main.hpp"
#include <array>
#include <cassert>
#include <cstring>
#include <utility>
namespace ttyd {
namespace {
uint32_t base(const BattleActorServices& s){return currentBattleAddress(s)+0x2858;}
uint32_t actor(int32_t i,const BattleActorServices& s){return base(s)+0x1bc+uint32_t(i)*0x134;}
bool exists(int32_t i,const BattleActorServices& s){const auto flags=s.memory.word(s.memory.user,actor(i,s));return (flags&1) && !(flags&0x102);}
int32_t integer(uint32_t v){int32_t n;std::memcpy(&n,&v,4);return n;}
float number(uint32_t v){float f;std::memcpy(&f,&v,4);return f;}
uint32_t bits(float f){uint32_t v;std::memcpy(&v,&f,4);return v;}
}
bool packedBattleAudienceMain(const BattleAudienceMainServices& s) {
    const auto& m=s.actors.memory;assert(m.word && m.storeWord && m.storeByte);
    const auto b=currentBattleAddress(s.actors),a=base(s.actors);
    if(!(m.word(m.user,a)&0x10000) && (m.word(m.user,b+0xef4)&0x80))
        if(!s.request || !s.request(s.user,0x801a1cdc))return false;
    for(const auto fn:{0x801a5ca8u,0x801a4f0cu,0x801a4998u,0x801a4570u,0x8019f97cu})
        if(!s.request || !s.request(s.user,fn))return false;
    if(m.word(m.user,b+0xef4)&0x80)m.storeWord(m.user,a,m.word(m.user,a)|0x10000);
    else m.storeWord(m.user,a,m.word(m.user,a)&~0x10000u);
    return true;
}
bool packedBattleAudienceControl(const BattleAudienceControlServices& s) {
    const auto& m=s.actors.memory;assert(m.word && m.storeWord && s.actors.byte);
    const auto b=currentBattleAddress(s.actors),a=base(s.actors);
    for(const auto offset:{0x1378cu,0x13788u,0x13784u})m.storeWord(m.user,a+offset,0);
    for(const bool right:{true,false}) {
        const auto counter=a+(right?0x13788:0x1378c);
        for(unsigned row=0;row<10;++row)for(unsigned col=0;col<10;++col) {
            const auto index=int32_t(20*row+(right?10:0)+col);
            if(exists(index,s.actors)) {
                const auto state=s.actors.byte(s.actors.user,actor(index,s.actors)+0x19);
                if(state!=0xc && state!=0xb)m.storeWord(m.user,counter,m.word(m.user,counter)+1);
            }
        }
    }
    m.storeWord(m.user,a+0x13784,m.word(m.user,a+0x13788)+m.word(m.user,a+0x1378c));
    auto record=actor(0,s.actors);
    for(int32_t i=0;i<200;++i,record+=0x134) {
        const bool motion=(m.word(m.user,record)&0x10)!=0;
        if(motion) {
            volatile float position=number(m.word(m.user,record+0xd0))+number(m.word(m.user,record+0xdc));
            m.storeWord(m.user,record+0xd0,bits(position));
            volatile float velocity=number(m.word(m.user,record+0xdc))+number(m.word(m.user,record+0xe4));
            m.storeWord(m.user,record+0xdc,bits(velocity));
            if(number(m.word(m.user,record+0xd0))<=0)
                for(const auto offset:{0xd0u,0xdcu,0xe4u})m.storeWord(m.user,record+offset,0);
        } else if(exists(i,s.actors) && s.actors.byte(s.actors.user,record+0x1b)<=12)
            if(!s.actor || !s.actor(s.user,record,i,false))return false;
    }
    if(m.word(m.user,b+0x19078)&0x20000)m.storeWord(m.user,a+0x137f4,10000);
    if(!(m.word(m.user,b+0xef4)&0x80))return true;
    const auto counter=integer(m.word(m.user,a+0x137f0));
    if(counter>=integer(m.word(m.user,a+0x137f4)))return true;
    int32_t bound;
    if(m.word(m.user,b+0x19078)&0x20000)bound=60;
    else if(counter==0)bound=1800;else if(counter==1)bound=6000;else if(counter==2)bound=30000;
    else return false; // Source unassigned-bound domain needs a provider.
    if(!s.random)return false;
    if(s.random(s.user,bound)!=0)return true;
    std::array<int32_t,200> index{};for(unsigned i=0;i<200;++i)index[i]=i;
    for(unsigned i=0;i<1000;++i){const auto first=s.random(s.user,200),second=s.random(s.user,200);
        if(first<0 || first>=200 || second<0 || second>=200)return false;
        std::swap(index[first],index[second]);}
    for(const auto seat:index) {
        if(!s.itemOn)return false;
        const auto result=s.itemOn(s.user,seat);if(!result)return false;
        if(*result==1){m.storeWord(m.user,a+0x137f0,m.word(m.user,a+0x137f0)+1);break;}
    }
    return true;
}
bool packedBattleAudienceInactiveItems(const BattleActorServices& s) {
    (void)currentBattleAddress(s);(void)base(s);auto item=base(s)+0xf25c;
    for(unsigned i=0;i<100;++i,item+=0x48)if(s.memory.word(s.memory.user,item)&1)return false;
    return true;
}
bool packedBattleAudienceInactiveApSources(const BattleActorServices& s) {
    return !(s.memory.word(s.memory.user,base(s))&2); // Set-bit graph is a provider.
}
std::optional<int32_t> packedBattleAudienceEmptyItemOn(int32_t index,const BattleActorServices& s) {
    const auto a=base(s),record=actor(index,s);
    if(s.memory.word(s.memory.user,a)&0x40000)return 0;
    if(s.byte(s.user,record+0x1b)!=0)return {};
    const auto table=s.memory.word(s.memory.user,0x803b4de4+8);
    if(s.memory.word(s.memory.user,table)==0)return 0;
    const auto column=index%20;if(column<4 || column>15)return 0;
    if(!exists(index,s))return 0;
    return {};
}
}
