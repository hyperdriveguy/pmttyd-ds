#include "battle_packed_sampling.hpp"
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
void word(const BattleActorServices& s,uint32_t a,uint32_t v) {assert(s.memory.storeWord);s.memory.storeWord(s.memory.user,a,v);}
void half(const BattleActorServices& s,uint32_t a,int32_t v) {assert(s.memory.storeHalf);s.memory.storeHalf(s.memory.user,a,uint16_t(v));}
void byte(const BattleActorServices& s,uint32_t a,uint8_t v) {assert(s.memory.storeByte);s.memory.storeByte(s.memory.user,a,v);}
float real(const BattleActorServices& s,uint32_t a) {const uint32_t bits=word(s,a);float value;std::memcpy(&value,&bits,4);return value;}
uint32_t row(uint32_t base,int32_t i) {return base+4+36u*uint32_t(i);}
void remove(const BattleActorServices& m,uint32_t base,int32_t index,int32_t& n) {
    for(int32_t j=index;j<n-1;++j) {
        const uint32_t dest=row(base,j),src=dest+36;
        for(unsigned pair=0;pair<4;++pair) {const uint32_t a=word(m,src+8*pair),b=word(m,src+8*pair+4);word(m,dest+8*pair,a);word(m,dest+8*pair+4,b);}
        word(m,dest+32,word(m,src+32));
    }
    --n;
}
}
int32_t packedSamplingForwardLevel(float x,int32_t direction,const BattlePackedSamplingServices& s) {
    const auto& c=s.common;const auto& p=c.position;const auto& m=p.memory;int32_t q=0;
    const float minimum=real(m,0x80422668);
    if(x>minimum) {const float offset=real(m,0x8042266c),denominator=real(m,0x80422670);assert(p.add && c.divide && p.unit.truncateFloat);const float sum=p.add(p.numericUser,offset,x),value=c.divide(p.numericUser,sum,denominator);q=p.unit.truncateFloat(p.unit.user,value);if(q>48)q=48;}
    return direction<0?sw(48u-uint32_t(q)):q;
}
int32_t samplePackedBattleCandidates(const BattleCandidateRecords& b,uint32_t weapon,int32_t actor,int32_t belong,uint32_t admission,uint32_t attributes,int32_t direction,const BattlePackedSamplingServices& s) {
    const auto& bm=b.bytes;const uint32_t a=b.address;
    word(bm,a,weapon);word(bm,a+0xab8,uint32_t(actor));byte(bm,a+0xabc,uint8_t(belong));word(bm,a+0xac0,admission);word(bm,a+0xac4,attributes);byte(bm,a+0xac8,uint8_t(direction));
    const auto& m=s.common.position.memory;const uint32_t battle=currentBattleAddress(m),unit=battleRegistryUnit(battle,actor,m);int32_t result=0;
    if(word(m,unit+0x27c)&0x10) {
        const uint32_t mask=word(bm,a+0xac0);
        if((mask&0x300)==0x100) {word(bm,a+0xac0,mask&~0x100u);word(bm,a+0xac0,word(bm,a+0xac0)|0x200);result=1;}
        else if((mask&0x300)==0x200) {word(bm,a+0xac0,word(bm,a+0xac0)|0x100);word(bm,a+0xac0,word(bm,a+0xac0)&~0x200u);result=1;}
    }
    updatePackedBattleCandidates(b,s);return result;
}
void updatePackedBattleCandidates(const BattleCandidateRecords& b,const BattlePackedSamplingServices& s) {
    const auto& c=s.common;const auto& p=c.position;const auto& m=p.memory;const auto& bm=b.bytes;const uint32_t a=b.address;
    const uint32_t initialAdmission=word(bm,a+0xac0),battle=currentBattleAddress(m);
    if(initialAdmission&0x80000000) {byte(bm,a+0xa6c,255);return;}
    auto truncate=[&](float v) {assert(p.unit.truncateFloat);return p.unit.truncateFloat(p.unit.user,v);};
    auto convert=[&](int32_t v) {assert(c.convert);return c.convert(p.numericUser,v);};
    auto add=[&](float x,float y) {assert(p.add);return p.add(p.numericUser,x,y);};
    auto multiply=[&](float x,float y) {assert(p.multiply);return p.multiply(p.numericUser,x,y);};
    auto fma=[&](float x,float y,float z) {assert(p.fma);return p.fma(p.numericUser,x,y,z);};
    auto unitAt=[&](uint32_t r) {return battleRegistryUnit(battle,half(bm,r),m);};
    auto partAt=[&](uint32_t r) {const uint32_t u=unitAt(r);return packedUnitPart(u,half(bm,r+2),m.memory);};
    int32_t total=0;
    for(int32_t id=0;id<64;++id) {
        const uint32_t unit=battleRegistryUnit(battle,id,m);if(!unit)continue;
        assert(c.status);if(c.status(c.user,unit,27)!=0 || (word(m,unit+0x104)&0x40))continue;
        const uint32_t kind=word(m,unit+0x10);uint32_t part=word(m,unit+0x14);const uint8_t count=byte(m,kind+0xb4);
        for(unsigned j=0;j<count;++j) {
            if(word(m,part+0x1ac)&0x10000) {part=word(m,part);continue;}
            const int32_t facing=sb(byte(m,part+0xbd));const uint32_t definition=word(m,part+4);
            const auto world=packedPartWorldPosition(part,p);float baseY=world[1];
            if(!(word(m,unit+0x104)&2)) {const float offset=real(m,part+0x28),scale=real(m,unit+0x114);baseY=fma(offset,scale,baseY);}
            const float offX=real(m,part+0x188),offX2=real(m,part+0x194),sumX=add(offX,offX2),scaleX=real(m,unit+0x114),productX=multiply(scaleX,sumX),sign=convert(facing),zOffset=real(m,0x8042220c);
            const float hitX=fma(sign,productX,world[0]),sortZ=add(zOffset,world[2]);
            const float offY=real(m,part+0x18c),offY2=real(m,part+0x198),scaleY=real(m,unit+0x114),sumY=add(offY,offY2),hitY=fma(scaleY,sumY,baseY);
            const float offZ=real(m,part+0x190),offZ2=real(m,part+0x19c),scaleZ=real(m,unit+0x114),sumZ=add(offZ,offZ2),hitZ=fma(scaleZ,sumZ,world[2]);
            const uint32_t r=row(a,total);half(bm,r,id);half(bm,r+2,sw(word(m,definition)));half(bm,r+4,truncate(hitX));half(bm,r+6,truncate(hitY));half(bm,r+8,truncate(hitZ));word(bm,r+0x18,0);word(bm,r+0x20,0xffffffff);
            if((word(m,unit+0x104)&1) && !(word(m,part+0x1ac)&0x20000)) {const int32_t direction=sb(byte(bm,a+0xac8));word(bm,r+0x18,word(bm,r+0x18)+uint32_t(300*direction));}
            const int32_t forward=half(m,part+0x1a0),sortY=sh(uint16_t(truncate(baseY)));const uint32_t bias=word(bm,r+0x18)+uint32_t(forward*facing);const int32_t z=sh(uint16_t(truncate(sortZ)));word(bm,r+0x18,bias);
            const float biasFloat=convert(sw(word(bm,r+0x18)));const int32_t x=sh(uint16_t(truncate(add(world[0],biasFloat))));word(bm,r+0xc,uint32_t(x));word(bm,r+0x10,uint32_t(sortY));word(bm,r+0x14,uint32_t(z));
            const int32_t sortX=sw(word(bm,r+0xc)),direction=sb(byte(bm,a+0xac8));byte(bm,r+0x1c,uint8_t(packedSamplingForwardLevel(convert(sortX),direction,s)));byte(bm,r+0x1d,sw(word(bm,r+0x14))<-30?0:1);
            part=word(m,part);++total;
        }
    }
    byte(bm,a+0xab7,0);byte(bm,a+0xa6c,uint8_t(total));int32_t n=sb(byte(bm,a+0xa6c));
    for(int32_t i=0;i<n;) {
        const uint32_t r=row(a,i),unit=unitAt(r),part=packedUnitPart(unit,half(bm,r+2),m.memory),mask=word(bm,a+0xac0);
        auto kind=[&] {return sw(word(m,unit+8));};auto range=[&](int32_t low,int32_t high) {const int32_t k=kind();return k>=low && k<=high;};
        bool reject=(mask&0x80000000)!=0;
        if(!reject && (mask&0x10000))reject=kind()!=222;
        if(!reject && (mask&0x20000))reject=!range(216,217);
        if(!reject && (mask&1))reject=range(222,223);
        if(!reject && (mask&2))reject=range(224,230);
        if(!reject && (mask&0x10))reject=range(1,215);
        if(!reject && (mask&0x40))reject=range(218,221);
        if(!reject && (mask&0x20))reject=range(216,217);
        if(!reject && (mask&0x1000)) {const uint32_t actor=word(bm,a+0xab8);reject=actor==word(m,unit);}
        if(!reject && (mask&0x100)) {const int32_t side=sb(byte(bm,a+0xabc)),belong=sb(byte(m,unit+0xc));reject=side==belong;}
        if(!reject && (mask&0x200)) {const int32_t side=sb(byte(bm,a+0xabc)),belong=sb(byte(m,unit+0xc));reject=side!=belong && belong!=2;}
        if(!reject && (mask&0x2000)) {const int32_t actor=sw(word(bm,a+0xab8));if(uint32_t(actor)!=word(m,unit)) {const uint32_t u=battleRegistryUnit(battle,actor,m);const int32_t actorKind=sw(word(m,u+8));reject=kind()==actorKind;}}
        if(!reject) {const uint32_t late=word(bm,a+0xac0);
            if(late&0x4000) {const uint32_t actor=word(bm,a+0xab8);reject=actor!=word(m,unit);}
            if(!reject && (late&0x100000)) {const uint32_t f=word(m,part+0x1ac);reject=!(f&3)||(f&0x40000);}
            if(!reject && (late&0x200000)) {const uint32_t f=word(m,part+0x1ac);reject=!(f&7)||(f&0x10)||(f&0x40000);}
        }
        if(reject)remove(bm,a,i,n);else ++i;
    }
    for(int32_t i=0;i<n;) {
        const uint32_t r=row(a,i),unit=unitAt(r),part=packedUnitPart(unit,half(bm,r+2),m.memory);bool reject=false;
        if(word(bm,a+0xac4)&1) {
            if(word(m,part+0x1ac)&0x80000)reject=true;
            else {assert(s.monosiri);const uint32_t info=s.monosiri(s.user,sw(word(m,unit+8)));if(!info)reject=true;else {const uint32_t again=s.monosiri(s.user,sw(word(m,unit+8)));reject=word(m,again+4)==0;}}
        }
        if(!reject) {const uint32_t mask=word(bm,a+0xac4);
            for(auto test:std::array<std::array<uint32_t,2>,3>{{{{2,4}},{{4,2}},{{8,4}}}})if(!reject && (mask&test[0]))reject=(word(m,unit+0x104)&test[1])!=0;
            if(!reject && (mask&0x10))reject=!(word(m,unit+0x104)&6);
            if(!reject && (mask&0x1000))reject=(word(m,part+0x1ac)&0x100000)!=0;
            if(!reject && (mask&0x2000) && (word(m,part+0x1ac)&0x200000)) {if(mask&0x100000) {const uint32_t actor=word(bm,a+0xab8);reject=actor!=word(m,unit);}else reject=true;}
            if(!reject && (mask&0x4000))reject=(word(m,part+0x1ac)&0x400000)!=0;
            if(!reject && (mask&0x8000))reject=!(word(m,part+0x1ac)&0x400000);
            if(!reject && (mask&0x2000000))reject=(word(m,unit+0x104)&0x20)!=0;
            if(!reject && (mask&0x4000000))reject=(word(m,part+0x1ac)&0x2000000)!=0;
        }
        if(reject)remove(bm,a,i,n);else ++i;
    }
    if(word(bm,a+0xac0)&0x100000)for(int32_t i=0;i<n;) {
        const uint32_t r=row(a,i),part=partAt(r);bool reject=false;
        if(word(m,part+0x1ac)&2)for(int32_t j=0;j<n;++j) {const uint32_t peer=row(a,j);if(peer!=r && half(bm,r)==half(bm,peer)) {const uint32_t other=partAt(peer);if(word(m,other+0x1ac)&1) {reject=true;break;}}}
        if(reject)remove(bm,a,i,n);else ++i;
    }
    for(int32_t i=0;i<n;) {
        const uint32_t r=row(a,i),part=partAt(r);bool reject=false;
        if((word(bm,a+0xac4)&0x1000000) && !(word(m,part+0x1ac)&0x40))for(int32_t j=0;j<n;++j) {
            const uint32_t peer=row(a,j);if(peer==r)continue;const int32_t lane=sb(byte(bm,r+0x1d)),otherLane=sb(byte(bm,peer+0x1d));if(lane!=otherLane)continue;
            const int32_t direction=sb(byte(bm,a+0xac8));int32_t x=sw(word(bm,r+0xc)),other=sw(word(bm,peer+0xc));if(direction<0) {x=sw(0u-uint32_t(x));other=sw(0u-uint32_t(other));}if(x<=other)continue;
            const int32_t peerId=half(bm,peer),peerPart=half(bm,peer+2);const uint32_t blocker=packedRegistryPart(peerId,peerPart,m);if(word(m,blocker+0x1ac)&(0x40|0x400000))continue;
            const uint32_t actor=battleRegistryUnit(battle,sw(word(bm,a+0xab8)),m),unit=battleRegistryUnit(battle,half(bm,peer),m);
            if((word(m,actor+0x27c)&0x10) && actor==unit)continue;
            reject=true;break;
        }
        if(reject)remove(bm,a,i,n);else ++i;
    }
    byte(bm,a+0xa6c,uint8_t(n));const int32_t published=sb(byte(bm,a+0xa6c));for(int32_t i=0;i<published;++i)byte(bm,a+0xa6d+uint32_t(i),uint8_t(i));
    const int32_t direction=sb(byte(bm,a+0xac8));
    for(int32_t i=0;i<published-1;++i)for(int32_t j=i+1;j<published;++j) {
        const uint8_t li=byte(bm,a+0xa6d+uint32_t(i)),ri=byte(bm,a+0xa6d+uint32_t(j));const uint32_t left=row(a,sb(li)),right=row(a,sb(ri));
        const uint32_t lb=word(bm,left+0x18);const int32_t lx=half(bm,left+4);const uint32_t rb=word(bm,right+0x18);const int32_t rx=half(bm,right+4);
        const int32_t l=sw(10u*lb+uint32_t(lx)),r=sw(10u*rb+uint32_t(rx));if(direction>0?l>r:l<=r) {byte(bm,a+0xa6d+uint32_t(i),ri);byte(bm,a+0xa6d+uint32_t(j),li);}
    }
}
}
