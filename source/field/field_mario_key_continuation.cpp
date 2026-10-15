#include "field_mario_key_continuation.hpp"
namespace ttyd {
namespace {
bool same(const FieldMarioKeyServices&a,const FieldMarioKeyServices&b) {
    return a.user==b.user&&a.current==b.current&&a.query==b.query&&a.party==b.party&&
        a.partyCurrent==b.partyCurrent&&a.marioByte==b.marioByte&&a.partyWord==b.partyWord&&a.partyByte==b.partyByte;
}
bool current(const FieldMarioKeyServices&s) {return s.current&&s.current(s.user);}
struct Invoke {bool& b;explicit Invoke(bool&v):b(v){b=true;}~Invoke(){b=false;}};
}
bool FieldMarioKeyCursor::reset() {
    if(invoking_)return false;
    *this=FieldMarioKeyCursor{};return true;
}
std::optional<int32_t> originalMarioChkCtrl(const FieldMarioKeyServices&s) {
    if(!current(s)||!s.marioByte)return {};
    const auto value=s.marioByte(s.user,0x38);
    if(!current(s)||!value)return {};
    return *value==0?1:0;
}
std::optional<int32_t> resumeOriginalMarioChkKey(const FieldMarioKeyServices&s,FieldMarioKeyCursor&c) {
    if(c.invoking_||c.poisoned_)return {};
    if(!c.bound_){c.services_=s;c.bound_=true;}else if(!same(s,c.services_))return {};
    Invoke invoke(c.invoking_);
    if(!current(s)){c.poisoned_=true;return {};}
    auto alive=[&](){
        if(c.poisoned_)return false;
        if(!current(s)||(c.party_.registry&&(!s.partyCurrent||!s.partyCurrent(s.user,c.party_)))){c.poisoned_=true;return false;}
        return true;
    };
    auto finish=[&](int32_t value)->std::optional<int32_t>{c.pending_=0;c.result_=value;return value;};
    while(alive()) {
        if(c.result_)return c.result_;
        if(c.step_==1) {
            c.pending_=0x800bf1b4;
            if(!s.party)return {};
            const auto p=s.party(s.user,c.id_);
            if(!alive()||!p)return {};
            if(!p->registry||(p->actor&&!p->incarnation)||!s.partyCurrent||!s.partyCurrent(s.user,*p))return {};
            c.party_=*p;++c.step_;continue;
        }
        if(c.step_==3) {
            c.pending_=0x8005c4ac;
            if(!s.marioByte)return {};
            const auto value=s.marioByte(s.user,0x39);
            if(!alive()||!value)return {};
            if(*value!=0)return finish(0);
            ++c.step_;continue;
        }
        if(c.step_==8) {if(!c.party_.actor)return finish(1);++c.step_;continue;}
        if(c.step_==12) {
            c.pending_=0x8005c558;
            if(!s.partyWord)return {};
            const auto value=s.partyWord(s.user,c.party_,0);
            if(!alive()||!value)return {};
            c.step_=(*value&0x100)?13:14;continue;
        }
        if(c.step_==13) {
            c.pending_=0x8005c564;
            if(!s.partyByte)return {};
            const auto value=s.partyByte(s.user,c.party_,0x31);
            if(!alive()||!value)return {};
            if(*value==7||*value==5||*value==6)return finish(0);
            ++c.step_;continue;
        }
        FieldMarioKeyRequest r;
        switch(c.step_) {
        case 0:r={0x80141050,0x8005c48c,0};break;
        case 2:r={0x8002e210,0x8005c498,0};break;
        case 4:r={0x800a4224,0x8005c4c4,0};break;
        case 5:r={0x800a4224,0x8005c4d0,0};break;
        case 6:r={0x800a16dc,0x8005c4e4,0};break;
        case 7:r={0x800a68e8,0x8005c4f8,0};break;
        case 9:r={0x800c5744,0x8005c51c,0};break;
        case 10:r={0x800c0068,0x8005c530,0};break;
        case 11:r={0x800b6e94,0x8005c544,0};break;
        case 14:r={0x800d0874,0x8005c58c,0};break;
        default:return {};
        }
        c.pending_=r.function;
        if(!s.query)return {};
        const auto value=s.query(s.user,r);
        if(!alive()||!value)return {};
        switch(c.step_) {
        case 0:c.id_=*value;break;
        case 2:if(*value!=2)return finish(0);break;
        case 4:if(*value==1)return finish(0);break;
        case 5:if(*value==2)return finish(0);break;
        case 6:case 7:if(*value==0)return finish(0);break;
        case 9:if(*value==1)return finish(0);break;
        case 10:case 11:if(*value!=0)return finish(0);break;
        case 14:return finish(*value==0?1:0);
        default:break;
        }
        ++c.step_;
    }
    return {};
}
}
