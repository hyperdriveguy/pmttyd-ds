#include "battle_candidate_sampling.hpp"
#include <cassert>
#include <cmath>
#include <cstring>

namespace ttyd {
namespace {
int32_t signedWord(uint32_t v) {return v<=0x7fffffff?static_cast<int32_t>(v):static_cast<int32_t>(int64_t(v)-0x100000000LL);}
int32_t sb(uint8_t v) {return v<128?v:int32_t(v)-256;}
int32_t sh(int32_t value) {const uint16_t v=value;return v<0x8000?v:int32_t(v)-0x10000;}
uint32_t word(const BattleCandidateBuffer& b,unsigned o) {assert(o+3<b.bytes.size());return uint32_t(b.bytes[o])<<24|uint32_t(b.bytes[o+1])<<16|uint32_t(b.bytes[o+2])<<8|b.bytes[o+3];}
void word(BattleCandidateBuffer& b,unsigned o,uint32_t v) {assert(o+3<b.bytes.size());b.bytes[o]=v>>24;b.bytes[o+1]=v>>16;b.bytes[o+2]=v>>8;b.bytes[o+3]=v;}
int32_t half(const BattleCandidateBuffer& b,unsigned o) {assert(o+1<b.bytes.size());const uint16_t v=uint16_t(b.bytes[o])<<8|b.bytes[o+1];return v<0x8000?v:int32_t(v)-0x10000;}
void half(BattleCandidateBuffer& b,unsigned o,int32_t value) {assert(o+1<b.bytes.size());const uint16_t v=value;b.bytes[o]=v>>8;b.bytes[o+1]=v;}
float add(float a,float b) {return a+b;}
float mul(float a,float b) {return a*b;}
float div(float a,float b) {return a/b;}
unsigned row(int32_t i) {assert(i>=0);return 4+36*i;}
SamplingUnit& unit(const CandidateSamplingServices& s,void* w,int32_t id) {auto* u=s.unit(s.user,w,id);assert(u && u->data);return *u;}
SamplingPart& part(const CandidateSamplingServices& s,const BattleCandidateBuffer& b,int32_t i) {assert(s.part);auto* p=s.part(s.user,half(b,row(i)),half(b,row(i)+2));assert(p && p->position);return *p;}
void remove(BattleCandidateBuffer& b,int32_t index,int32_t& count) {
    assert(count>index && row(count)<=b.bytes.size());
    std::memmove(b.bytes.data()+row(index),b.bytes.data()+row(index+1),36*(count-index-1));--count;
}
}
int32_t samplingForwardLevel(float x,int8_t direction,const CandidateSamplingServices& s) {
    int32_t q=0;if(x>-800.f) {assert(s.truncate);q=s.truncate(s.user,div(add(800.f,x),40.f));}
    if(q>48)q=48;
    return direction<0?48-q:q;
}
int sampleBattleCandidates(BattleCandidateBuffer& b,DamageWeapon* weapon,int32_t actor,int32_t belong,
                          uint32_t admission,uint32_t attributes,int32_t direction,const CandidateSamplingServices& s) {
    b.weapon=weapon;word(b,0xab8,static_cast<uint32_t>(actor));b.bytes[0xabc]=static_cast<uint8_t>(belong);
    word(b,0xac0,admission);word(b,0xac4,attributes);b.bytes[0xac8]=static_cast<uint8_t>(direction);
    assert(s.captureWork && s.unit);void* work=s.captureWork(s.user);
    auto& u=unit(s,work,actor);int result=0;
    if((u.data->statusFlags&0x10) && ((admission&0x300)==0x100 || (admission&0x300)==0x200)) {
        word(b,0xac0,admission^0x300);result=1;
    }
    updateBattleCandidates(b,s);return result;
}
void updateBattleCandidates(BattleCandidateBuffer& b,const CandidateSamplingServices& s) {
    assert(s.captureWork && s.unit);void* work=s.captureWork(s.user);
    if(word(b,0xac0)&0x80000000) {b.bytes[0xa6c]=0xff;return;}
    assert(s.status27 && s.worldPosition && s.truncate);int32_t total=0;
    for(int32_t id=0;id<64;++id) {
        auto* u=s.unit(s.user,work,id);if(!u)continue;assert(u->data);
        if(s.status27(s.user,*u) || (u->data->attributes&0x40))continue;
        assert(u->partCount && u->scale);const unsigned count=*u->partCount;auto* p=u->first;
        for(unsigned j=0;j<count;++j) {
            assert(p && p->position);auto& position=*p->position;
            if(!(position.attributes&0x10000)) {
                const auto world=s.worldPosition(s.user,*p);const float scale=*u->scale;
                float y=world.y;if(!(u->data->attributes&2))y=std::fma(position.offset.y,scale,y);
                const float direction=static_cast<float>(p->direction);const unsigned r=row(total);
                const float hx=std::fma(direction,mul(scale,add(p->samplingOffsets[0].x,p->samplingOffsets[1].x)),world.x);
                const float hy=std::fma(scale,add(p->samplingOffsets[0].y,p->samplingOffsets[1].y),y);
                const float hz=std::fma(scale,add(p->samplingOffsets[0].z,p->samplingOffsets[1].z),world.z);
                half(b,r,id);half(b,r+2,position.id);half(b,r+4,s.truncate(s.user,hx));half(b,r+6,s.truncate(s.user,hy));half(b,r+8,s.truncate(s.user,hz));
                word(b,r+0x18,0);word(b,r+0x20,0xffffffff);
                uint32_t bias=0;
                if((u->data->attributes&1) && !(position.attributes&0x20000))bias=static_cast<uint32_t>(300*sb(b.bytes[0xac8]));
                bias+=static_cast<uint32_t>(int32_t(p->forwardBias)*p->direction);word(b,r+0x18,bias);
                word(b,r+0xc,static_cast<uint32_t>(sh(s.truncate(s.user,add(world.x,static_cast<float>(signedWord(bias)))))));
                word(b,r+0x10,static_cast<uint32_t>(sh(s.truncate(s.user,y))));
                word(b,r+0x14,static_cast<uint32_t>(sh(s.truncate(s.user,add(world.z,5.f)))));
                b.bytes[r+0x1c]=static_cast<uint8_t>(samplingForwardLevel(static_cast<float>(signedWord(word(b,r+0xc))),static_cast<int8_t>(sb(b.bytes[0xac8])),s));
                b.bytes[r+0x1d]=signedWord(word(b,r+0x14))<-30?0:1;++total;
            }
            p=p->next;
        }
    }
    b.bytes[0xab7]=0;b.bytes[0xa6c]=static_cast<uint8_t>(total);int32_t n=sb(b.bytes[0xa6c]);
    for(int32_t i=0;i<n;) {
        auto& u=unit(s,work,half(b,row(i)));auto& p=part(s,b,i);const auto& d=*u.data;
        const uint32_t a=word(b,0xac0),f=p.position->attributes;const int32_t actor=signedWord(word(b,0xab8));const int32_t side=sb(b.bytes[0xabc]);
        bool reject=(a&0x80000000)!=0||(a&0x10000 && d.kind!=222)||(a&0x20000 && (d.kind<216 || d.kind>217))||
            (a&1 && (d.kind==222 || d.kind==223))||(a&2 && d.kind>=224 && d.kind<=230)||
            (a&0x10 && d.kind>=1 && d.kind<=215)||(a&0x40 && d.kind>=218 && d.kind<=221)||
            (a&0x20 && d.kind>=216 && d.kind<=217)||(a&0x1000 && u.id==actor)||
            (a&0x100 && d.belong==side)||(a&0x200 && d.belong!=side && d.belong!=2);
        if(!reject && (a&0x2000) && u.id!=actor)reject=d.kind==unit(s,work,actor).data->kind;
        if(!reject && (a&0x4000))reject=u.id!=actor;
        if(!reject && (a&0x100000))reject=!(f&3)||(f&0x40000);
        if(!reject && (a&0x200000))reject=!(f&7)||(f&0x10)||(f&0x40000);
        if(reject)remove(b,i,n);else ++i;
    }
    for(int32_t i=0;i<n;) {
        auto& u=unit(s,work,half(b,row(i)));auto& p=part(s,b,i);const auto& d=*u.data;const uint32_t c=word(b,0xac4),f=p.position->attributes;
        bool reject=false;
        if(c&1) {
            reject=(f&0x80000)!=0;
            if(!reject) {assert(s.monosiri);const auto* info=s.monosiri(s.user,d.kind);if(!info)reject=true;else {info=s.monosiri(s.user,d.kind);assert(info);reject=info[1]==0;}}
        }
        reject=reject||(c&2 && d.attributes&4)||(c&4 && d.attributes&2)||(c&8 && d.attributes&4)||(c&0x10 && !(d.attributes&6))||
            (c&0x1000 && f&0x100000)||(c&0x2000 && f&0x200000 && !((c&0x100000) && u.id==signedWord(word(b,0xab8))))||
            (c&0x4000 && f&0x400000)||(c&0x8000 && !(f&0x400000))||(c&0x2000000 && d.attributes&0x20)||(c&0x4000000 && f&0x2000000);
        if(reject)remove(b,i,n);else ++i;
    }
    if(word(b,0xac0)&0x100000)for(int32_t i=0;i<n;) {
        auto& p=part(s,b,i);bool reject=false;
        if(p.position->attributes&2)for(int32_t j=0;j<n;++j)if(j!=i && half(b,row(i))==half(b,row(j)) && (part(s,b,j).position->attributes&1)) {reject=true;break;}
        if(reject)remove(b,i,n);else ++i;
    }
    for(int32_t i=0;i<n;) {
        auto& p=part(s,b,i);bool reject=false;
        if((word(b,0xac4)&0x1000000) && !(p.position->attributes&0x40))for(int32_t j=0;j<n;++j) {
            if(j==i || sb(b.bytes[row(i)+0x1d])!=sb(b.bytes[row(j)+0x1d]))continue;
            int32_t x=signedWord(word(b,row(i)+0xc)),other=signedWord(word(b,row(j)+0xc));
            if(sb(b.bytes[0xac8])<0) {x=signedWord(0u-static_cast<uint32_t>(x));other=signedWord(0u-static_cast<uint32_t>(other));}
            if(x<=other)continue;
            auto& peer=part(s,b,j);if(peer.position->attributes&(0x40|0x400000))continue;
            auto& actor=unit(s,work,signedWord(word(b,0xab8)));
            if((actor.data->statusFlags&0x10) && actor.data==unit(s,work,half(b,row(j))).data)continue;
            reject=true;break;
        }
        if(reject)remove(b,i,n);else ++i;
    }
    b.bytes[0xa6c]=static_cast<uint8_t>(n);
    for(int32_t i=0;i<n;++i)b.bytes[0xa6d+i]=static_cast<uint8_t>(i);
    for(int32_t i=0;i<n-1;++i)for(int32_t j=i+1;j<n;++j) {
        const auto left=row(sb(b.bytes[0xa6d+i])),right=row(sb(b.bytes[0xa6d+j]));
        const int32_t lk=signedWord(10u*word(b,left+0x18)+static_cast<uint32_t>(half(b,left+4)));
        const int32_t rk=signedWord(10u*word(b,right+0x18)+static_cast<uint32_t>(half(b,right+4)));
        if(sb(b.bytes[0xac8])>0?lk>rk:lk<=rk) {const auto temp=b.bytes[0xa6d+i];b.bytes[0xa6d+i]=b.bytes[0xa6d+j];b.bytes[0xa6d+j]=temp;}
    }
}
}
