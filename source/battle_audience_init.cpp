#include "battle_audience_init.hpp"
#include "battle_audience_sound.hpp"
#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {
float number(uint32_t v){float f;std::memcpy(&f,&v,4);return f;}
uint32_t bits(float f){uint32_t v;std::memcpy(&v,&f,4);return v;}
int32_t signedWord(uint32_t v){int32_t n;std::memcpy(&n,&v,4);return n;}
uint32_t base(const BattleActorServices& a){return currentBattleAddress(a)+0x2858;}
void countHalf(bool right,uint32_t retained,const BattleActorServices& a) {
    const auto& m=a.memory;const auto counter=retained+(right?0x13788:0x1378c);
    for(unsigned row=0;row<10;++row)for(unsigned col=0;col<10;++col) {
        const auto e=base(a)+0x1bc+0x134*(20*row+(right?10:0)+col),flags=m.word(m.user,e);
        if((flags&1) && !(flags&0x102)) {const auto current=base(a)+0x1bc+0x134*(20*row+(right?10:0)+col);const auto state=a.byte(a.user,current+0x19);if(state!=12 && state!=11)m.storeWord(m.user,counter,m.word(m.user,counter)+1);}
    }
}
}
bool initializePackedSetup4AudienceParameters(uint32_t config,uint32_t setup,const BattleAudienceInitServices& s) {
    const auto& m=s.actors.memory;assert(m.word && m.storeWord && m.storeByte && s.actors.byte && s.random);
    if(m.word(m.user,config+0x1c)!=0)return false;
    for(unsigned i=0;i<16;++i) {
        const auto expected=i==10?5:i>=13?1:0;
        if(s.actors.byte(s.actors.user,config+0x20+2*i)!=expected || s.actors.byte(s.actors.user,config+0x21+2*i)!=expected)return false;
    }
    m.storeWord(m.user,setup+0xac,m.word(m.user,config+0x1c));
    for(unsigned i=0;i<16;++i) {
        const auto first=s.actors.byte(s.actors.user,config+0x20+2*i),last=s.actors.byte(s.actors.user,config+0x21+2*i);
        m.storeByte(m.user,setup+0xb0+i,uint8_t(s.random(s.user,int32_t(last)-first+1)+first));
    }
    return true;
}
bool initializePackedSetup4ZeroAudience(const BattleAudienceInitServices& s) {
    const auto& m=s.actors.memory;assert(m.word && m.half && m.storeWord && s.actors.byte && s.random && s.fma);
    const auto b=currentBattleAddress(s.actors),a=base(s.actors),setup=m.word(m.user,m.word(m.user,b+0x2738)+0xc),t=setup+0xac,p=m.word(m.user,0x8041eb00);
    const auto rank=int16_t(m.half(m.user,p+0x88)),level=int16_t(m.half(m.user,p+0x8a));
    if(rank<0 || rank>3 || level<1 || m.half(m.user,p+0x8c)!=0 || m.word(m.user,t)!=0 ||
       (m.word(m.user,b+0x19078)&0x10000) || m.word(m.user,b+0x19088)!=0)return false;
    for(unsigned i=0;i<16;++i)if(s.actors.byte(s.actors.user,t+4+i)!=(i==10?5:i>=13?1:0))return false;
    // Original copies the entire level table before selecting its row.
    std::array<uint32_t,65> levels;for(unsigned i=0;i<65;++i)levels[i]=m.word(m.user,0x802f9eac+4*i);
    m.storeWord(m.user,a+0x13790,50u*(rank+1));
    if(m.half(m.user,m.word(m.user,0x8041eb00)+0x8c)!=0)return false;
    m.storeWord(m.user,a+0x13790,0);
    const auto baseline=number(levels[std::min<int32_t>(level-1,64)]);m.storeWord(m.user,a+0x13778,bits(baseline));
    const auto q0=number(m.word(m.user,m.word(m.user,0x8041eb00)+0x84));
    const auto first=s.random(s.user,10),second=s.random(s.user,10);
    const auto q1=number(m.word(m.user,m.word(m.user,0x8041eb00)+0x84));
    const float delta=q1-baseline,product=delta*float(first+second-13);
    const auto desired=s.fma(s.user,number(m.word(m.user,0x80424998)),product,q0);
    m.storeWord(m.user,a+0x1377c,bits(desired));m.storeWord(m.user,a+0x13780,0);
    if(!std::isfinite(desired))return false;
    std::array<int32_t,11> thresholds;for(unsigned i=0;i<11;++i)thresholds[i]=signedWord(m.word(m.user,0x802f9fb0+4*i));
    const auto draw=s.random(s.user,100);int32_t cardinality=12,sum=0;
    for(unsigned i=0;i<11;++i){sum+=thresholds[i];if(draw<sum){cardinality=i+1;break;}}
    std::array<int32_t,12> index{},weight{};
    for(unsigned i=0;i<12;++i)index[i]=i;
    for(unsigned i=0;i<24;++i){const auto first=s.random(s.user,12),second=s.random(s.user,12);std::swap(index[first],index[second]);}
    int32_t admitted=0;
    for(const auto type:index) {
        const auto byte=s.actors.byte(s.actors.user,t+4+type);
        const int32_t setupWeight=byte<128?byte:int32_t(byte)-256;
        if(admitted<cardinality && setupWeight) {
            weight[type]=setupWeight*(s.random(s.user,10)+1);
            if(s.actors.byte(s.actors.user,0x803b4de4+32*type)&1)weight[type]*=2;
            ++admitted;
        }
    }
    // In this admitted source domain N clamps to0 and sum is positive. The
    // 13 allocation quotients are all0, residual/guest/Entry bodies skip.
    int32_t total=0;for(auto value:weight)total+=value;assert(total>0);
    for(unsigned i=0;i<12;++i)index[i]=i;
    for(unsigned i=0;i<11;++i)for(unsigned j=i+1;j<12;++j)
        if(weight[i]==weight[j]?s.random(s.user,2)==0:weight[i]<weight[j])std::swap(index[i],index[j]);
    for(unsigned i=0;i<150;++i){(void)s.random(s.user,0);(void)s.random(s.user,0);}
    for(unsigned i=0;i<12;++i)m.storeWord(m.user,a+0x13794+4*i,uint32_t(weight[i]));
    return true;
}
bool initializePackedBattleAudience(const BattleAudienceInitServices& s) {
    const auto& m=s.actors.memory;assert(m.word && m.storeWord && m.storeByte && s.actors.byte && s.random);
    const auto a=base(s.actors),window=base(s.actors)+0x1375c;
    (void)m.word(m.user,0x8041eb00);
    eventFillMemory(a,0,0x13914,m);
    if(!s.setting || !s.setting(s.user))return false;
    if(!s.texture)return false;
    const auto normal=s.texture(s.user,"battle/audience/audience_normal.tpl");if(!normal)return false;
    m.storeWord(m.user,a+0xc,*normal);
    // GuestTPLRead has its own current base capture before requesting the file.
    const auto guest=base(s.actors);
    const auto luigi=s.texture(s.user,"battle/audience/audience_luigi.tpl");if(!luigi)return false;
    m.storeWord(m.user,guest+0x10,*luigi);m.storeByte(m.user,guest+0x18,12);
    const auto throwing=base(s.actors);const auto draw=s.random(s.user,100);
    m.storeWord(m.user,throwing+0x137f4,draw<80?0:draw<90?1:draw<95?2:3);
    m.storeWord(m.user,throwing+0x137f0,0);m.storeWord(m.user,throwing+0x137f8,0);
    m.storeWord(m.user,a+0x13908,0);
    for(const auto offset:{0x1378cu,0x13788u,0x13784u})m.storeWord(m.user,a+offset,0);
    countHalf(true,a,s.actors);countHalf(false,a,s.actors);
    m.storeWord(m.user,a+0x13784,m.word(m.user,a+0x13788)+m.word(m.user,a+0x1378c));
    m.storeWord(m.user,window+0x18,bits(float(signedWord(m.word(m.user,a+0x13784)))));
    if(!s.noise || !s.noise(s.user))return false;
    setBattleAudienceVolume(2,60,180,s.actors);
    m.storeWord(m.user,a+0x13910,0);
    return s.target && s.target(s.user);
}
bool updatePackedBattleAudienceZeroTarget(const BattleActorServices& a) {
    const auto& m=a.memory;assert(m.word && m.half && a.byte);
    const auto owner=base(a);if(m.word(m.user,owner)&0x40000)return true;
    const float amount=number(m.word(m.user,owner+0x1377c))+number(m.word(m.user,owner+0x13780));
    if(!std::isfinite(amount) || amount>=2147483648.f || amount<-2147483648.f)return false;
    int32_t target=int32_t(amount);if(target<0)target=0;
    const auto capacity=signedWord(m.word(m.user,owner+0x13790));if(target>capacity)target=capacity;
    const auto currentOwner=base(a);uint32_t count=0;
    for(unsigned i=0;i<200;++i) {
        const auto r=base(a)+0x1bc+0x134*i,flags=m.word(m.user,r);
        if((flags&1) && !(flags&0x102) && a.byte(a.user,currentOwner+0x1bc+0x134*i+0x19)!=12)++count;
    }
    const auto delta=uint32_t(target)-count;
    const auto b=currentBattleAddress(a);(void)base(a);const auto p=m.word(m.user,0x8041eb00);
    if(!m.half(m.user,p+0x8c) || !delta || (m.word(m.user,b+0xef4)&0x100))return true;
    return false;
}
}
