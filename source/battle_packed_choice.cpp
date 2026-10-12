#include "battle_packed_choice.hpp"
#include "battle_packed_status.hpp"
#include <cassert>
#include <cstring>

namespace ttyd {
namespace {
int32_t sw(uint32_t v) {return v<=0x7fffffff?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}
int32_t sb(uint8_t v) {return v<128?v:int32_t(v)-256;}
int32_t sh(uint16_t v) {return v<0x8000?v:int32_t(v)-65536;}
uint32_t word(const BattleActorServices& s,uint32_t a) {assert(s.memory.word);return s.memory.word(s.memory.user,a);}
int32_t half(const BattleActorServices& s,uint32_t a) {assert(s.memory.half);return sh(s.memory.half(s.memory.user,a));}
uint8_t byte(const BattleActorServices& s,uint32_t a) {assert(s.byte);return s.byte(s.user,a);}
float real(const BattleActorServices& s,uint32_t a) {const uint32_t v=word(s,a);float f;std::memcpy(&f,&v,4);return f;}
uint32_t row(int32_t i) {return 4+36u*uint32_t(i);}
int32_t plus(int32_t a,int32_t b) {return sw(uint32_t(a)+uint32_t(b));}
}
BattleCandidateRecords privateBattleCandidateRecords(BattleCandidateBuffer& b) {
    BattleCandidateRecords r;r.bytes.user=&b;r.bytes.memory.user=&b;
    r.bytes.byte=[](void* p,uint32_t o) {auto& b=*static_cast<BattleCandidateBuffer*>(p);assert(o<b.bytes.size());return b.bytes[o];};
    r.bytes.memory.half=[](void* p,uint32_t o) {auto& b=*static_cast<BattleCandidateBuffer*>(p);assert(uint64_t(o)+2<=b.bytes.size());return uint16_t(uint16_t(b.bytes[o])<<8|b.bytes[o+1]);};
    r.bytes.memory.word=[](void* p,uint32_t o) {auto& b=*static_cast<BattleCandidateBuffer*>(p);assert(uint64_t(o)+4<=b.bytes.size());return uint32_t(b.bytes[o])<<24|uint32_t(b.bytes[o+1])<<16|uint32_t(b.bytes[o+2])<<8|b.bytes[o+3];};return r;
}
int32_t packedUnitFp(uint32_t unit,bool maximum,const BattleActorServices& s) {
    const int32_t kind=sw(word(s,unit+8));const uint32_t battle=currentBattleAddress(s),offset=maximum?0x10e:0x112;
    int32_t value=half(s,unit+offset);if(kind>=224 && kind<=230)value=half(s,battleMarioUnit(battle,s)+offset);return value;
}
int32_t packedCanAct(uint32_t unit,const BattleActorServices& s) {
    for(int32_t id:{1,2,9,27})if(checkPackedBattleStatus(unit,id,s))return 0;
    return unit && (word(s,unit+0x138)&1)?0:1;
}
void choosePackedBattleCandidate(const BattleCandidateRecords& b,uint32_t flags,const BattlePackedChoiceServices& s,CandidateChoiceOutputs output) {
    const auto& m=s.position.memory;const auto& bm=b.bytes;const uint32_t base=b.address;
    const uint32_t battle=currentBattleAddress(m);assert(output.write);
    output.write(output.user,true,-1);output.write(output.user,false,0);
    auto count=[&] {return sb(byte(bm,base+0xa6c));};
    auto index=[&](uint32_t rank) {return sb(byte(bm,base+0xa6d+rank));};
    auto emit=[&](int32_t i) {const uint32_t a=base+row(i);output.write(output.user,true,half(bm,a));output.write(output.user,false,half(bm,a+2));};
    if(count()<=0)return;
    const uint32_t actor=battleRegistryUnit(battle,sw(word(bm,base+0xab8)),m);
    emit(index(0));std::array<int32_t,74> weights{};const int32_t bonus=index(0);assert(bonus>=0 && bonus<74);weights[bonus]=10;
    const auto& p=s.position;auto convert=[&](int32_t v) {assert(s.convert);return s.convert(p.numericUser,v);};
    auto multiply=[&](float a,float b) {assert(p.multiply);return p.multiply(p.numericUser,a,b);};
    auto add=[&](float a,float b) {assert(p.add);return p.add(p.numericUser,a,b);};
    auto constant=[&](unsigned n) {return real(m,0x804221f0+4*n);};
    auto update=[&](int32_t& weight,float value) {assert(p.unit.truncateFloat);weight=p.unit.truncateFloat(p.unit.user,value);};
    auto scale=[&](int32_t& weight) {const float coefficient=constant(0),value=convert(weight);update(weight,multiply(coefficient,value));};
    auto status=[&](uint32_t u,int id) {assert(s.status);return s.status(s.user,u,id)!=0;};
    auto itemByte=[&](int32_t item,uint32_t offset) {return byte(m,0x803108a8+40u*uint32_t(item)+offset);};
    for(int32_t rank=0;rank<count();++rank) {
        const int32_t i=index(uint32_t(rank));assert(i>=0 && i<74);
        const uint32_t unit=battleRegistryUnit(battle,half(bm,base+row(i)),m);
        auto& weight=weights[i];weight=plus(weight,100);
        if((flags&1) && sw(word(m,unit+8))==222)scale(weight);
        if(flags&2) {const int32_t kind=sw(word(m,unit+8));if(kind>=224 && kind<=230)scale(weight);}
        if(flags&4) {
            const int32_t distance=count()-rank;const float n=convert(distance),coefficient=constant(2),offset=constant(1);
            const float factor=multiply(coefficient,n),w=convert(weight),sum=add(offset,factor);update(weight,multiply(w,sum));
        }
        if(flags&8) {
            const float r=convert(rank),coefficient=constant(2),offset=constant(3),product=multiply(coefficient,r),sum=add(offset,product),w=convert(weight);update(weight,multiply(sum,w));
        }
        if(flags&0x10) {const int32_t a=sb(byte(m,actor+0xc)),u=sb(byte(m,unit+0xc));if(a==u)scale(weight);}
        if(flags&0x20) {const int32_t a=sb(byte(m,actor+0xc)),u=sb(byte(m,unit+0xc));if(a!=u && u!=2)scale(weight);}
        for(uint32_t flag:{0x100u,0x200u})if(flags&flag) {
            const int32_t hp=half(m,unit+0x10c),maximum=half(m,unit+0x108);const float coefficient=constant(flag==0x100?4:5),h=convert(hp),max=convert(maximum);
            assert(s.divide);const float ratio=s.divide(p.numericUser,h,max);float factor;
            if(flag==0x100) {assert(s.subtract);factor=s.subtract(p.numericUser,coefficient,ratio);}else factor=add(coefficient,ratio);
            const float w=convert(weight);update(weight,multiply(factor,w));
        }
        for(uint32_t flag:{0x400u,0x800u})if(flags&flag) {
            const int32_t hp=half(m,unit+0x10c);const float coefficient=constant(6),h=convert(hp),offset=constant(flag==0x400?0:5);float factor;
            if(flag==0x400) {assert(s.negativeMultiplySubtract);factor=s.negativeMultiplySubtract(p.numericUser,coefficient,h,offset);}else {assert(p.fma);factor=p.fma(p.numericUser,coefficient,h,offset);}
            const float w=convert(weight);update(weight,multiply(factor,w));
        }
        if(flags&0x1000) {const uint32_t kind=word(m,unit+0x10);const int32_t hp=half(m,unit+0x10c);if(hp<=byte(m,kind+0xd))scale(weight);}
        const uint32_t weapon=word(bm,base);if(!weapon)continue;
        if(word(m,unit+0x104)&0x20000)weight=1;
        const int32_t item=sw(word(m,weapon+8));bool override=false;
        switch(item) {
        case 0:continue;
        case 142:assert(s.canAct);override=s.canAct(s.user,unit)==0 || status(unit,10);break;
        case 146:override=status(unit,11);break;
        case 147:override=status(unit,15);break;
        case 143:override=status(unit,14);break;
        case 134:override=status(unit,18);break;
        case 135:override=status(unit,6);break;
        case 136:override=status(unit,7);break;
        case 137:override=status(unit,5);break;
        case 153:override=true;for(int id:{1,2,3,4,5,8,9,11,15,20})if(status(unit,id)) {override=false;break;}break;
        default:
            if(itemByte(item,0x1e)) {
                assert(s.maxFp && s.fp);const int32_t maximum=s.maxFp(s.user,unit),current=s.fp(s.user,unit);
                if(current<maximum) {
                    const int32_t max=s.maxFp(s.user,unit),fp=s.fp(s.user,unit),deficit=sw(uint32_t(max)-uint32_t(fp));
                    const float coefficient=constant(1),d=convert(deficit),offset=constant(5);assert(p.fma);
                    const float factor=p.fma(p.numericUser,coefficient,d,offset),w=convert(weight);update(weight,multiply(w,factor));
                }else {const uint32_t freshWeapon=word(bm,base);const int32_t freshItem=sw(word(m,freshWeapon+8));override=itemByte(freshItem,0x1d)==0 && itemByte(freshItem,0x1f)==0;}
            }
            break;
        }
        if(override)weight=1;
    }
    int32_t selected=0;
    if(!(flags&0x80000000)) {int32_t best=0;for(int32_t i=0;i<74;++i)if(weights[i]>best) {best=weights[i];selected=i;}}
    else {int32_t total=0;for(auto w:weights)total=plus(total,w);assert(s.random);int32_t draw=s.random(s.user,total);for(selected=0;selected<74;++selected) {draw=sw(uint32_t(draw)-uint32_t(weights[selected]));if(draw<=0)break;}}
    emit(selected);
}
}
