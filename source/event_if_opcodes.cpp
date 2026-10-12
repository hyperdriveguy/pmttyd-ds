#include "event_if_opcodes.hpp"
#include <cassert>

namespace ttyd {
namespace {
int32_t signedWord(uint32_t v) {return v<=0x7fffffff?static_cast<int32_t>(v):static_cast<int32_t>(int64_t(v)-0x100000000LL);}
int32_t signedHalf(uint32_t v) {v&=0xffff;return v<0x8000?int32_t(v):int32_t(v)-0x10000;}
uint32_t word(const EventDispatchServices& s,uint32_t a) {assert(s.word);return s.word(s.user,a);}
uint8_t byte(const EventIfOpcodeServices& s,uint32_t a) {assert(s.byte);return s.byte(s.values.user,a);}
int32_t integer(const EventDispatchServices& s,EventEntry& e,uint32_t a) {const int32_t raw=signedWord(word(s,a));assert(s.resolve);return s.resolve(s.user,e,raw);}
float floating(const EventDispatchServices& s,EventEntry& e,uint32_t a) {const int32_t raw=signedWord(word(s,a));assert(s.resolveFloat);return s.resolveFloat(s.user,e,raw);}
bool comparison(unsigned op,int32_t l,int32_t r) {
    switch(op) {case 0:return l==r;case 1:return l!=r;case 2:return l<r;case 3:return l>r;case 4:return l<=r;default:return l>=r;}
}
int32_t byteLoop(uint32_t l,uint32_t r,uint8_t lb,uint8_t rb,const EventIfOpcodeServices& s) {
    for(;;) {
        const int32_t d=int32_t(lb)-int32_t(rb);if(d!=0)return d;if(lb==0)return 0;
        ++l;++r;lb=byte(s,l);rb=byte(s,r);
    }
}
void skip(EventEntry& e,const EventDispatchServices& s,bool stopAtElse) {
    uint32_t p=e.nextCommand;int32_t depth=0;
    for(;;) {
        const uint32_t w=word(s,p),op=w&0xffff;p+=4+static_cast<uint32_t>(4*signedHalf(w>>16));
        if(op>=12 && op<=31)depth=signedWord(static_cast<uint32_t>(depth)+1);
        else if(op==32 && stopAtElse && depth==0)break;
        else if(op==33 || op==1) {depth=signedWord(static_cast<uint32_t>(depth)-1);if(depth<0)break;}
    }
    e.nextCommand=p;
}
}
int32_t eventCompareStrings(uint32_t l,uint32_t r,const EventIfOpcodeServices& s) {
    uint8_t lb=byte(s,l),rb=byte(s,r);const int32_t d=int32_t(lb)-int32_t(rb);if(d!=0)return d;
    const unsigned alignment=l&3;
    if(alignment!=(r&3))return byteLoop(l,r,lb,rb,s);
    if(alignment!=0) {
        if(lb==0)return 0;
        for(unsigned i=0;i<3-alignment;++i) {
            ++l;++r;lb=byte(s,l);rb=byte(s,r);
            const int32_t diff=int32_t(lb)-int32_t(rb);if(diff!=0)return diff;if(lb==0)return 0;
        }
        ++l;++r;
    }
    for(;;) {
        const uint32_t lw=word(s.values,l),rw=word(s.values,r);
        if(((lw-0x01010101u)&0x80808080u)!=0) {
            lb=byte(s,l);rb=byte(s,r);return byteLoop(l,r,lb,rb,s);
        }
        if(lw!=rw)return lw<rw?-1:1;
        l+=4;r+=4;
    }
}
std::optional<int32_t> eventIfOpcode(EventEntry& e,uint8_t op,const EventIfOpcodeServices& services) {
    if(op<12 || op>33)return {};
    const auto& s=services.values;const uint32_t args=e.arguments;
    if(op==33)return 2;
    if(op==32) {skip(e,s,false);return 2;}
    bool accepted;
    if(op<=17) {
        const uint32_t l=static_cast<uint32_t>(integer(s,e,args)),r=static_cast<uint32_t>(integer(s,e,args+4));
        const int32_t result=eventCompareStrings(l==0?0x8041fa2cu:l,r==0?0x8041fa2cu:r,services);
        accepted=comparison(op-12,result,0);
    } else if(op<=23) {
        const float l=floating(s,e,args),r=floating(s,e,args+4);
        // Negated opposite CR relations accept unordered values for20..23.
        switch(op) {case 18:accepted=l==r;break;case 19:accepted=l!=r;break;case 20:accepted=!(l>=r);break;case 21:accepted=!(l<=r);break;case 22:accepted=!(l>r);break;default:accepted=!(l<r);break;}
    } else if(op<=29) {
        const int32_t l=integer(s,e,args),r=integer(s,e,args+4);accepted=comparison(op-24,l,r);
    } else {
        const uint32_t value=static_cast<uint32_t>(integer(s,e,args)),mask=word(s,args+4);
        accepted=op==30?(value&mask)!=0:(value&mask)==0;
    }
    if(!accepted)skip(e,s,true);
    return 2;
}
}
