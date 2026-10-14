#include "battle_stage_work.hpp"
#include <cassert>
#include <cstring>
#include <map>
#include <vector>
using namespace ttyd;
struct Fixture {
    static constexpr uint32_t B=0x10000,W=B+0x163fc;
    std::map<uint32_t,uint8_t> bytes;
    std::vector<uint32_t> calls;
    bool spot=true,available=true,mutate=false;
    unsigned interpolation=0;
    Fixture() {
        word(0x8041eb90,B);
        real(0x80422b3c,290);real(0x80422b40,200);
        real(0x80422b44,90);real(0x80422b48,-50);
    }
    uint32_t word(uint32_t a){uint32_t v=0;for(unsigned i=0;i<4;++i)v=(v<<8)|bytes[a+i];return v;}
    void word(uint32_t a,uint32_t v){for(unsigned i=0;i<4;++i)bytes[a+i]=uint8_t(v>>(24-8*i));}
    void real(uint32_t a,float f){uint32_t v;std::memcpy(&v,&f,4);word(a,v);}
    float real(uint32_t a){const auto v=word(a);float f;std::memcpy(&f,&v,4);return f;}
    BattleStageWorkServices services() {
        BattleStageWorkServices s;s.user=this;s.actors.user=this;s.actors.memory.user=this;
        s.actors.byte=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->bytes[a];};
        s.actors.memory.word=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->word(a);};
        s.actors.memory.storeWord=[](void* p,uint32_t a,uint32_t v){static_cast<Fixture*>(p)->word(a,v);};
        s.actors.memory.storeByte=[](void* p,uint32_t a,uint8_t v){static_cast<Fixture*>(p)->bytes[a]=v;};
        s.spotlightMain=[](void* p){auto& f=*static_cast<Fixture*>(p);f.calls.push_back(1);if(f.mutate)f.word(0x8041eb90,0x90000);return f.spot;};
        s.activeEntry=[](void* p,uint32_t e){auto& f=*static_cast<Fixture*>(p);f.calls.push_back(e);return f.available;};
        s.freeScratch=[](void* p,uint32_t allocation){auto& f=*static_cast<Fixture*>(p);f.calls.push_back(allocation);
            assert(f.word(W+4+0xe4)==allocation); // Owner is cleared after callback returns.
            if(f.mutate){f.word(0x8041eb90,0x90000);f.word(W+4+0xe4,0xaaaa);}
            return f.available;};
        s.truncate=[](void*,float v)->std::optional<int32_t>{return int32_t(v);};
        return s;
    }
    void init(){initializePackedBattleStageWorkStorage(B,services().actors.memory);}
};
int main() {
    using F=Fixture;
    Fixture init;
    for(unsigned i=0;i<0xb3c;++i)init.bytes[F::W+i]=255;
    init.bytes[F::W-1]=23;init.bytes[F::W+0xb3c]=42;init.init();
    assert(init.bytes[F::W-1]==23 && init.bytes[F::W+0xb3c]==42);
    assert(init.word(F::W+0xae4)==1 && init.word(F::W+0xb04)==1);
    assert(init.bytes[F::W+0xae8]==0 && init.bytes[F::W+0xae9]==0);
    assert(init.bytes[F::W+0xb08]==140 && init.bytes[F::W+0xb09]==140);
    assert(init.real(F::W+0xb24)==290 && init.real(F::W+0xb28)==200 && init.real(F::W+0xb2c)==90);
    assert(init.real(F::W+0xb30)==290 && init.real(F::W+0xb34)==-50 && init.real(F::W+0xb38)==200);
    for(unsigned i=0;i<12;++i){assert(init.word(F::W+4+i*0xe8)==0);assert(init.word(F::W+4+i*0xe8+0xe4)==0);}
    auto s=init.services();assert(packedBattleStageWorkMain(s) && (init.calls==std::vector<uint32_t>{1}));
    Fixture stopped;stopped.init();stopped.spot=false;stopped.word(F::W+4+0xe4,0x1234);s=stopped.services();
    assert(!packedBattleStageWorkMain(s) && stopped.word(F::W+4+0xe4)==0x1234);
    Fixture cleanup;cleanup.init();cleanup.word(F::W+4+0xe4,0x1234);cleanup.mutate=true;s=cleanup.services();
    assert(packedBattleStageWorkMain(s) && cleanup.word(F::W+4+0xe4)==0 && cleanup.word(0x8041eb90)==0x90000);
    Fixture unavailable;unavailable.init();unavailable.word(F::W+4+0xe4,0x1234);unavailable.available=false;s=unavailable.services();
    assert(!packedBattleStageWorkMain(s) && unavailable.word(F::W+4+0xe4)==0x1234);
    Fixture active;active.init();active.word(F::W+4+11*0xe8,3);active.available=false;s=active.services();
    assert(!packedBattleStageWorkMain(s) && active.calls.back()==F::W+4+11*0xe8);
    s.activeEntry=nullptr;assert(!packedBattleStageWorkMain(s));
    Fixture counters;counters.init();
    static constexpr uint32_t count[]={0xaf0,0xaf4,0xb10,0xb14},output[]={0xae8,0xae9,0xb08,0xb09};
    constexpr uint32_t start[]={0xaea,0xaeb,0xb0a,0xb0b},end[]={0xaec,0xaed,0xb0c,0xb0d};
    constexpr uint32_t mode[]={0xb00,0xb01,0xb20,0xb21},duration[]={0xaf8,0xafc,0xb18,0xb1c};
    for(unsigned i=0;i<4;++i) {
        counters.word(F::W+count[i],i+1);counters.word(F::W+duration[i],10);
        counters.bytes[F::W+start[i]]=200+i;counters.bytes[F::W+end[i]]=240+i;counters.bytes[F::W+mode[i]]=128+i;
    }
    s=counters.services();s.interpolate=[](void* p,int32_t m,int32_t elapsed,int32_t d,float a,float b)->std::optional<float>{
        auto& f=*static_cast<Fixture*>(p);const auto i=f.interpolation++;
        assert(m==-128+int32_t(i) && elapsed==10-int32_t(i) && d==10 && a==200+i && b==240+i);
        assert(f.word(F::W+count[i])==i); // Decrement precedes interpolation.
        return i%2?-2.9f:300.9f;
    };
    assert(packedBattleStageWorkMain(s) && counters.interpolation==4);
    for(unsigned i=0;i<4;++i)assert(counters.bytes[F::W+output[i]]==(i%2?254:44));
    Fixture skip;skip.init();skip.word(F::W+0xaf4,0xffffffff);skip.word(F::W+0xb14,0x80000000);s=skip.services();
    assert(packedBattleStageWorkMain(s) && skip.word(F::W+0xaf4)==0xffffffff && skip.word(F::W+0xb14)==0x80000000);
    Fixture missing;missing.init();missing.word(F::W+0xaf0,1);s=missing.services();
    assert(!packedBattleStageWorkMain(s) && missing.word(F::W+0xaf0)==0 && missing.bytes[F::W+0xae8]==0);
    Fixture wrap;wrap.init();wrap.word(F::W+0xaf0,2);wrap.word(F::W+0xaf8,0x80000000);s=wrap.services();
    s.interpolate=[](void*,int32_t,int32_t elapsed,int32_t d,float,float)->std::optional<float>{assert(elapsed==0x7fffffff && d==INT32_MIN);return 0;};
    assert(packedBattleStageWorkMain(s));
    Fixture fresh;fresh.init();fresh.word(F::W+0xaf0,1);fresh.word(F::W+0xaf4,1);s=fresh.services();
    s.interpolate=[](void* p,int32_t,int32_t,int32_t,float a,float)->std::optional<float>{auto& f=*static_cast<Fixture*>(p);if(!f.interpolation++)f.bytes[F::W+0xaeb]=239;else assert(a==239);return 0;};
    assert(packedBattleStageWorkMain(s) && fresh.interpolation==2);
}
