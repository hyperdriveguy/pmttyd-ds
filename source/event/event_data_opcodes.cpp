#include "event_data_opcodes.hpp"
#include <cassert>
#include <cstring>

namespace ttyd {
namespace {
int32_t signedWord(uint32_t v) {return v<=0x7fffffff?static_cast<int32_t>(v):static_cast<int32_t>(int64_t(v)-0x100000000LL);}
uint32_t word(const EventDispatchServices& s,uint32_t a) {assert(s.word);return s.word(s.user,a);}
int32_t raw(const EventDispatchServices& s,uint32_t a) {return signedWord(word(s,a));}
int32_t integer(const EventDispatchServices& s,EventEntry& e,int32_t op) {assert(s.resolve);return s.resolve(s.user,e,op);}
float floating(const EventDispatchServices& s,EventEntry& e,int32_t op) {assert(s.resolveFloat);return s.resolveFloat(s.user,e,op);}
void store(const EventDispatchServices& s,EventEntry& e,int32_t d,int32_t v) {assert(s.store);s.store(s.user,e,d,v);}
void storeFloat(const EventDispatchServices& s,EventEntry& e,int32_t d,float v) {assert(s.storeFloat);s.storeFloat(s.user,e,d,v);}
float ieee(uint32_t v) {float f;static_assert(sizeof(f)==sizeof(v),"binary32 provider required");std::memcpy(&f,&v,sizeof(f));return f;}
}
std::optional<int32_t> eventDataOpcode(EventEntry& e,uint8_t op,const EventDataOpcodeServices& services) {
    if(op<50 || op>73)return {};
    const auto& s=services.values;const uint32_t args=e.arguments;
    if(op==62 || op==68) {e.field150=static_cast<uint32_t>(integer(s,e,raw(s,args)));return 2;}
    if(op>=63) {
        if(op==67 || op==73) {
            const int32_t d=raw(s,args),i=integer(s,e,raw(s,args+4));
            const int32_t v=raw(s,e.field150+4u*static_cast<uint32_t>(i));
            if(op==67)store(s,e,d,v);else storeFloat(s,e,d,static_cast<float>(v));
            return 2;
        }
        const bool isFloat=op>=69;
        const unsigned lanes=isFloat?op-68:op-62;
        for(unsigned i=0;i<lanes;++i) {
            // Single float69 captures its raw destination before the cursor.
            int32_t d=0;if(op==69)d=raw(s,args);
            const uint32_t cursor=e.field150;
            if(op!=69)d=raw(s,args+4*i);
            e.field150=cursor+4;
            const uint32_t v=word(s,cursor);
            if(isFloat)storeFloat(s,e,d,ieee(v));else store(s,e,d,signedWord(v));
        }
        return 2;
    }
    const int32_t d=raw(s,args);
    if(op==50 || op==51) {
        const int32_t rhs=raw(s,args+4);store(s,e,d,op==50?integer(s,e,rhs):rhs);return 2;
    }
    if(op==52) {storeFloat(s,e,d,floating(s,e,raw(s,args+4)));return 2;}
    if(op>=58) {
        const float rhs=floating(s,e,raw(s,args+4)),lhs=floating(s,e,d);
        float result;
        if(op==58)result=lhs+rhs;
        else if(op==59)result=lhs-rhs;
        else if(op==60)result=rhs*lhs;
        else result=lhs/rhs;
        storeFloat(s,e,d,result);return 2;
    }
    int32_t rhs=integer(s,e,raw(s,args+4));
    if(op==57) {assert(services.truncate);const float rounded=static_cast<float>(rhs)+.5f;rhs=services.truncate(services.numericUser,rounded);}
    int32_t lhs=integer(s,e,d);
    if(op==57) {const float rounded=static_cast<float>(lhs)+.5f;lhs=services.truncate(services.numericUser,rounded);}
    uint32_t result;
    if(op==53)result=static_cast<uint32_t>(lhs)+static_cast<uint32_t>(rhs);
    else if(op==54)result=static_cast<uint32_t>(lhs)-static_cast<uint32_t>(rhs);
    else if(op==55)result=static_cast<uint32_t>(lhs)*static_cast<uint32_t>(rhs);
    else {
        assert(services.divideWord);const int32_t q=services.divideWord(services.numericUser,lhs,rhs);
        result=op==56?static_cast<uint32_t>(q):static_cast<uint32_t>(lhs)-static_cast<uint32_t>(q)*static_cast<uint32_t>(rhs);
    }
    store(s,e,d,signedWord(result));return 2;
}
}
