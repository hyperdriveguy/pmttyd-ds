#include "field_party_lifetime.hpp"
#include <cassert>
#include <functional>
#include <map>
#include <vector>
#include <iostream>
using namespace ttyd;
namespace {
constexpr uint32_t T=0x8041ead8,D=0x8041ead4,B=0x803143e8;
constexpr uint32_t P=0x81000000,Q=0x81000200,R=0x81000400,M=0x82000000,N=0x82001000;
constexpr uint32_t K=0x800be69c,K2=0x800be594,H=0x80001000,GET=0x8005e0c0;
struct Op {
    char kind;uint32_t a,b,c;
    bool operator==(const Op& o)const{return kind==o.kind && a==o.a && b==o.b && c==o.c;}
};
struct Fixture {
    std::map<uint32_t,uint32_t> words;
    std::map<uint32_t,uint8_t> bytes;
    std::vector<Op> trace;
    std::function<void(Fixture&,const Op&)> after;
    std::function<bool(Fixture&,BattleSequenceCall&)> child;
    size_t attempts=0,failAt=~size_t(0);bool failed=false;
    bool admit(){const bool hold=!failed && attempts==failAt;++attempts;if(hold)failed=true;return !hold;}
    void record(Op o){trace.push_back(o);if(after)after(*this,o);}
    BattleSequenceEntryServices services(){
        BattleSequenceEntryServices s;s.user=this;
        s.word=[](void* u,uint32_t a)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(u);if(!f.admit())return {};auto i=f.words.find(a);assert(i!=f.words.end());const auto v=i->second;f.record({'w',a,v,0});return v;};
        s.byte=[](void* u,uint32_t a)->std::optional<uint8_t>{auto& f=*static_cast<Fixture*>(u);if(!f.admit())return {};auto i=f.bytes.find(a);assert(i!=f.bytes.end());const auto v=i->second;f.record({'b',a,v,0});return v;};
        s.store=[](void* u,uint32_t a,uint32_t v){auto& f=*static_cast<Fixture*>(u);if(!f.admit())return false;f.words[a]=v;f.record({'s',a,v,0});return true;};
        s.storeByte=[](void* u,uint32_t a,uint8_t v){auto& f=*static_cast<Fixture*>(u);if(!f.admit())return false;f.bytes[a]=v;f.record({'t',a,v,0});return true;};
        s.call=[](void* u,BattleSequenceCall& c){auto& f=*static_cast<Fixture*>(u);if(!f.admit())return false;if(f.child && !f.child(f,c))return false;f.record({'c',c.function,c.args[0],c.args[1]});return true;};return s;
    }
    Fixture(){words={{T,P},{T+4,Q},{P+0x160,M},{P+0xc,17},{Q+0x160,M},{Q+0xc,18},{B+0x28+0x24,H},{B+13*0x28+0x24,0}};
        bytes={{M+0x245,0},{M+0x246,1},{M+0x247,99},{M+0x248,99},{P+0x31,1},{Q+0x31,13},{P+0x34,0},{Q+0x34,0}};
        child=[](Fixture&,BattleSequenceCall& c){if(c.function==GET)c.result=M;return true;};}
};
std::vector<Op> killTrace(bool forget=false){
    std::vector<Op> v={{'w',T,P,0},{'w',P+0x160,M,0},{'b',M+0x245,0,0},{'w',T,P,0},{'b',P+0x31,1,0},
        {'s',D,B+0x28,0},{'w',B+0x28+0x24,H,0},{'b',P+0x34,0,0},{'c',H,P,0}};
    if(!forget){std::vector<Op> post={{'b',M+0x245,0,0},{'w',T,P,0},{'w',T,P,0},{'b',P+0x31,1,0}};v.insert(v.end(),post.begin(),post.end());}
    std::vector<Op> tail={{'t',M+0x247,forget?0u:1u,0},{'t',M+0x245,255,0},{'s',T,0,0},
        {'w',P+0xc,17,0},{'c',0x8003b2d0,17,0},{'c',0x800300c4,0,P}};
    v.insert(v.end(),tail.begin(),tail.end());return v;
}
void ordinaryAndEveryPrimitiveHold(){
    for(const bool forget:{false,true}){
        const auto expected=killTrace(forget);
        for(size_t hold=0;hold<=expected.size();++hold){
            Fixture f;if(hold<expected.size())f.failAt=hold;
            auto s=f.services();FieldPartyRemovalCursor c;
            bool done=removeFieldParty(0,forget,s,c);
            if(hold<expected.size()){
                assert(!done && c.active);const auto prefix=f.trace;
                assert(!removeFieldParty(1,forget,s,c));assert(f.trace==prefix);
                assert(!removeFieldParty(0,!forget,s,c));assert(f.trace==prefix);
                auto wrong=s;wrong.storeByte=nullptr;assert(!removeFieldParty(0,forget,wrong,c));assert(f.trace==prefix);
                done=removeFieldParty(0,forget,s,c);
            }
            assert(done && !c.active && f.trace==expected);
            assert(f.words[T]==0 && f.words[T+4]==Q && f.words[D]==B+0x28);
        }
    }
}
void negativeAndNull(){
    for(const int32_t i:std::array<int32_t,3>{{-1,-128,INT32_MIN}}){Fixture f;FieldPartyRemovalCursor c;assert(removeFieldParty(i,false,f.services(),c));assert(f.trace.empty());}
    Fixture f;f.words[T]=0;FieldPartyRemovalCursor c;assert(removeFieldParty(0,true,f.services(),c));assert((f.trace==std::vector<Op>{{'w',T,0,0}}));
}
void hookAndFreshOwners(){
    Fixture f;f.child=[](Fixture& x,BattleSequenceCall& call){if(call.function==H){
        x.bytes[M+0x245]=1;x.words[P+0x160]=N;x.words[P+0xc]=0xffffffff;x.words[D]=0x12345678;
    }return true;};
    // First post-hook table read returns Q, second returns R. Definition uses
    // pre-hook P.kind; saved kind is raw R.kind (no sign extension on store).
    f.after=[](Fixture& x,const Op& o){if(o.kind=='w' && o.a==T+4)x.words[T+4]=R;};f.bytes[R+0x31]=0x82;
    FieldPartyRemovalCursor c;assert(removeFieldParty(0,false,f.services(),c));
    assert(f.bytes[M+0x247]==0x82 && f.bytes[M+0x245]==255 && f.words[T]==0 && f.words[T+4]==R);
    assert(!f.bytes.count(N+0x247) && f.words[D]==0x12345678);
    assert((f.trace[f.trace.size()-2]==Op{'c',0x8003b2d0,0xffffffff,0}));
    assert((f.trace.back()==Op{'c',0x800300c4,0,P}));
    for(const bool nullAfter:{false,true}){Fixture g;g.child=[nullAfter](Fixture& x,BattleSequenceCall& call){if(call.function==H){if(nullAfter)x.words[T]=0;else x.bytes[M+0x245]=255;}return true;};
        FieldPartyRemovalCursor d;assert(removeFieldParty(0,false,g.services(),d));assert(g.bytes[M+0x247]==0);}
}
void nullHookMode11AndSignedKind(){
    for(const bool noHook:{false,true}){Fixture f;if(noHook)f.words[B+0x28+0x24]=0;else f.bytes[P+0x34]=11;
        FieldPartyRemovalCursor c;assert(removeFieldParty(0,false,f.services(),c));
        for(const auto& o:f.trace){assert(!(o.kind=='c' && o.a==H));if(noHook)assert(!(o.kind=='b' && o.a==P+0x34));}}
    Fixture f;f.bytes[P+0x31]=255;f.words[B-0x28+0x24]=0;FieldPartyRemovalCursor c;
    assert(removeFieldParty(0,false,f.services(),c));assert(f.words[D]==B-0x28 && f.bytes[M+0x247]==255);
    // Original negative linked-index path still reads address31. This fixture
    // maps it deliberately; the controller must not invent an early success.
    Fixture g;g.bytes[M+0x245]=128;g.bytes[0x31]=13;FieldPartyRemovalCursor d;
    assert(removeFieldParty(0,true,g.services(),d));assert(g.words[D]==B+13*0x28);
}
void childProgressAndPinnedArguments(){
    Fixture f;unsigned hook=0,release=0,free=0;
    f.child=[&](Fixture& x,BattleSequenceCall& call){
        if(call.function==H){assert(call.args[0]==P);if(hook++==0){x.words[B+0x28+0x24]=0x1234;return false;}}
        if(call.function==0x8003b2d0){assert(call.args[0]==17);assert(x.words[T]==0 && x.bytes[M+0x245]==255);if(release++==0){x.words[P+0xc]=42;return false;}}
        if(call.function==0x800300c4){assert(call.args[0]==0 && call.args[1]==P);if(free++==0)return false;}
        return true;
    };
    FieldPartyRemovalCursor c;const auto s=f.services();unsigned holds=0;
    while(!removeFieldParty(0,false,s,c)){assert(c.active);assert(++holds<=3);assert(c.pending==H || c.pending==0x8003b2d0 || c.pending==0x800300c4);}
    assert(holds==3 && hook==2 && release==2 && free==2 && f.trace==killTrace());
}
void rawEvents(){
    constexpr uint32_t E=0x83000000,A=0x83001000;
    for(const bool clear:{false,true})for(const uint32_t slot:{0u,1u,7u,0xffffffffu})for(const uint8_t index:{uint8_t(3),uint8_t(255)}){
        const std::vector<Op> prefix={{'c',GET,0,0},{'w',E+0x18,A,0},{'w',A,slot,0},{'b',M+slot+0x245,index,0}};
        auto expected=prefix;if(index<128)expected.push_back({'c',slot && clear?K2:K,slot?1u:3u,0});
        for(size_t hold=0;hold<=expected.size();++hold){Fixture f;f.words[E+0x18]=A;f.words[A]=slot;f.bytes[M+slot+0x245]=index;
            if(hold<expected.size())f.failAt=hold;auto s=f.services();FieldPartyRemovalEventCursor c;int32_t result=99;
            bool done=removeFieldPartyEvent(E,clear,s,c,result);
            if(hold<expected.size()){assert(!done && result==99);const auto before=f.trace;
                assert(!removeFieldPartyEvent(E+4,clear,s,c,result));assert(!removeFieldPartyEvent(E,!clear,s,c,result));assert(f.trace==before);
                done=removeFieldPartyEvent(E,clear,s,c,result);}
            assert(done && result==2 && !c.active && f.trace==expected);
        }
    }
    Fixture f;f.words[E+0x18]=A;f.words[A]=1;f.bytes[M+0x246]=4;unsigned calls=0;
    f.child=[&](Fixture& x,BattleSequenceCall& call){if(call.function==GET)call.result=M;else{assert(call.function==K2 && call.args[0]==1);if(calls++==0){x.words[A]=0;x.bytes[M+0x246]=255;return false;}}return true;};
    FieldPartyRemovalEventCursor c;int32_t result=99;assert(!removeFieldPartyEvent(E,true,f.services(),c,result));assert(result==99 && c.pending==K2);
    assert(removeFieldPartyEvent(E,true,f.services(),c,result) && result==2 && calls==2);
}
void marioOwnerQueries(){
    for(const uint8_t primary:{uint8_t(2),uint8_t(255)})for(const uint8_t aux:{uint8_t(7),uint8_t(128)}){
        std::vector<Op> expected={{'c',GET,0,0},{'b',M+0x245,primary,0}};
        if(primary<128){expected.push_back({'c',K,primary,0});expected.push_back({'t',M+0x245,255,0});}
        expected.push_back({'c',GET,0,0});expected.push_back({'b',N+0x246,aux,0});
        if(aux<128){expected.push_back({'c',K,aux,0});expected.push_back({'t',N+0x246,255,0});}
        for(size_t hold=0;hold<=expected.size();++hold){Fixture f;f.bytes[M+0x245]=primary;f.bytes[N+0x246]=aux;unsigned queries=0;
            f.child=[&](Fixture&,BattleSequenceCall& call){if(call.function==GET)call.result=queries++?N:M;return true;};
            if(hold<expected.size())f.failAt=hold;FieldMarioPartyRemovalCursor c;const auto s=f.services();bool done=removeMarioParties(s,c);
            if(hold<expected.size()){assert(!done);auto wrong=s;wrong.user=nullptr;assert(!removeMarioParties(wrong,c));done=removeMarioParties(s,c);}
            assert(done && !c.active && f.trace==expected && queries==2);
        }
    }
    // First child mutates a different Mario owner. The second getter, and not
    // a joint initial snapshot, determines the auxiliary index/ff destination.
    Fixture f;unsigned queries=0,kills=0;f.bytes[M+0x246]=255;f.bytes[N+0x246]=1;
    f.child=[&](Fixture& x,BattleSequenceCall& call){if(call.function==GET)call.result=queries++?N:M;
        else if(call.function==K){if(kills++==0){x.bytes[N+0x246]=9;return false;}if(kills==2)assert(call.args[0]==0);else assert(call.args[0]==9);}return true;};
    FieldMarioPartyRemovalCursor c;assert(!removeMarioParties(f.services(),c));assert(queries==1 && c.pending==K);
    assert(removeMarioParties(f.services(),c));assert(queries==2 && kills==3 && f.bytes[N+0x246]==255);
}
}
int main(){ordinaryAndEveryPrimitiveHold();negativeAndNull();hookAndFreshOwners();nullHookMode11AndSignedKind();childProgressAndPinnedArguments();rawEvents();marioOwnerQueries();std::cout<<"field party lifetime passed\n";}
