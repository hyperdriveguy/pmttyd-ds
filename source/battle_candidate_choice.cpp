#include "battle_candidate_choice.hpp"
#include <cassert>
#include <cmath>

namespace ttyd {
namespace {
int32_t signedWord(uint32_t v) {return v<=0x7fffffff?static_cast<int32_t>(v):static_cast<int32_t>(int64_t(v)-0x100000000LL);}
int32_t add(int32_t a,int32_t b) {return signedWord(static_cast<uint32_t>(a)+static_cast<uint32_t>(b));}
int32_t byte(const BattleCandidateBuffer& b,unsigned offset) {assert(offset<b.bytes.size());const uint8_t v=b.bytes[offset];return v<128?v:int32_t(v)-256;}
int32_t half(const BattleCandidateBuffer& b,unsigned offset) {assert(offset+1<b.bytes.size());const uint16_t v=uint16_t(b.bytes[offset])<<8|b.bytes[offset+1];return v<0x8000?v:int32_t(v)-0x10000;}
int32_t word(const BattleCandidateBuffer& b,unsigned offset) {assert(offset+3<b.bytes.size());return signedWord(uint32_t(b.bytes[offset])<<24|uint32_t(b.bytes[offset+1])<<16|uint32_t(b.bytes[offset+2])<<8|b.bytes[offset+3]);}
unsigned row(int32_t index) {assert(index>=0 && index<=74);return 4+36*index;}
float mul(float a,float b) {return a*b;}
float plus(float a,float b) {return a+b;}
float minus(float a,float b) {return a-b;}
float divide(float a,float b) {return a/b;}
bool status(const CandidateChoiceServices& s,DamageUnit& u,int id) {assert(s.status);return s.status(s.user,u,id);}
uint8_t itemByte(const CandidateChoiceServices& s,int32_t item,unsigned offset) {assert(s.itemByte);return s.itemByte(s.user,0x803108a8u+40u*static_cast<uint32_t>(item)+offset);}
void emit(BattleCandidateBuffer& b,int32_t index,CandidateChoiceOutputs output) {
    const unsigned offset=row(index);output.write(output.user,true,half(b,offset));output.write(output.user,false,half(b,offset+2));
}
}
void chooseBattleCandidate(BattleCandidateBuffer& b,uint32_t flags,const CandidateChoiceServices& s) {
    chooseBattleCandidate(b,flags,s,{s.user,s.output});
}
void chooseBattleCandidate(BattleCandidateBuffer& b,uint32_t flags,const CandidateChoiceServices& s,CandidateChoiceOutputs output) {
    assert(s.captureWork && output.write);void* work=s.captureWork(s.user);
    output.write(output.user,true,-1);output.write(output.user,false,0);
    if(byte(b,0xa6c)<=0)return;
    assert(s.unit);auto* actor=s.unit(s.user,work,word(b,0xab8));assert(actor);
    const int32_t first=byte(b,0xa6d);emit(b,first,output);
    std::array<int32_t,74> weights{};assert(first>=0 && first<74);weights[first]=10;
    auto update=[&](int32_t& weight,float value) {assert(s.truncate);weight=s.truncate(s.user,value);};
    for(int32_t rank=0;rank<byte(b,0xa6c);++rank) {
        const int32_t index=byte(b,0xa6d+rank);assert(index>=0 && index<74);
        auto* u=s.unit(s.user,work,half(b,row(index)));assert(u);
        auto& weight=weights[index];weight=add(weight,100);
        if((flags&1) && u->kind==222)update(weight,mul(1.5f,static_cast<float>(weight)));
        if((flags&2) && u->kind>=224 && u->kind<=230)update(weight,mul(1.5f,static_cast<float>(weight)));
        if(flags&4)update(weight,mul(static_cast<float>(weight),plus(.4000000059604645f,mul(.10000000149011612f,static_cast<float>(byte(b,0xa6c)-rank)))));
        if(flags&8)update(weight,mul(plus(.5f,mul(.10000000149011612f,static_cast<float>(rank))),static_cast<float>(weight)));
        if((flags&0x10) && actor->belong==u->belong)update(weight,mul(1.5f,static_cast<float>(weight)));
        if((flags&0x20) && actor->belong!=u->belong && u->belong!=2)update(weight,mul(1.5f,static_cast<float>(weight)));
        if(flags&0x100)update(weight,mul(minus(2.f,divide(static_cast<float>(u->hp),static_cast<float>(u->maxHp))),static_cast<float>(weight)));
        if(flags&0x200)update(weight,mul(plus(1.f,divide(static_cast<float>(u->hp),static_cast<float>(u->maxHp))),static_cast<float>(weight)));
        if(flags&0x400)update(weight,mul(std::fma(-.05000000074505806f,static_cast<float>(u->hp),1.5f),static_cast<float>(weight)));
        if(flags&0x800)update(weight,mul(std::fma(.05000000074505806f,static_cast<float>(u->hp),1.f),static_cast<float>(weight)));
        if(flags&0x1000) {assert(s.danger);if(u->hp<=s.danger(s.user,*u))update(weight,mul(1.5f,static_cast<float>(weight)));}
        if(!b.weapon)continue;
        if(u->attributes&0x20000)weight=1;
        const int32_t item=b.weapon->itemId;bool override=false;
        switch(item) {
        case 0:continue;
        case 142:assert(s.canRecover);override=!s.canRecover(s.user,*u) || status(s,*u,10);break;
        case 146:override=status(s,*u,11);break;
        case 147:override=status(s,*u,15);break;
        case 143:override=status(s,*u,14);break;
        case 134:override=status(s,*u,18);break;
        case 135:override=status(s,*u,6);break;
        case 136:override=status(s,*u,7);break;
        case 137:override=status(s,*u,5);break;
        case 153:
            override=true;for(int id:{1,2,3,4,5,8,9,11,15,20})if(status(s,*u,id)) {override=false;break;}break;
        default:
            if(itemByte(s,item,0x1e)) {
                assert(s.maxFp && s.fp);
                const int32_t maximum=s.maxFp(s.user,*u);const int32_t current=s.fp(s.user,*u);
                if(current<maximum) {
                    const int32_t rereadMax=s.maxFp(s.user,*u);const int32_t rereadFp=s.fp(s.user,*u);
                    const int32_t deficit=add(rereadMax,signedWord(0u-static_cast<uint32_t>(rereadFp)));
                    update(weight,mul(static_cast<float>(weight),std::fma(.10000000149011612f,static_cast<float>(deficit),1.f)));
                } else {
                    assert(b.weapon);const int32_t rereadItem=b.weapon->itemId;
                    override=itemByte(s,rereadItem,0x1d)==0 && itemByte(s,rereadItem,0x1f)==0;
                }
            }
            break;
        }
        if(override)weight=1;
    }
    int32_t selected=0;
    if(!(flags&0x80000000)) {
        int32_t best=0;for(int32_t i=0;i<74;++i)if(weights[i]>best) {selected=i;best=weights[i];}
    } else {
        int32_t total=0;for(auto weight:weights)total=add(total,weight);
        assert(s.random);int32_t draw=s.random(s.user,total);
        for(selected=0;selected<74;++selected) {
            draw=add(draw,signedWord(0u-static_cast<uint32_t>(weights[selected])));if(draw<=0)break;
        }
    }
    emit(b,selected,output);
}
}
