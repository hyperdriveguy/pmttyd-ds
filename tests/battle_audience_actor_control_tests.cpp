#include "battle_audience_actor_control.hpp"
#include "battle_audience_population.hpp"
#include "battle_timing_a.hpp"
#include "event_script_assets.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <cstdio>
#include <map>
#include <vector>
#include <limits>
using namespace ttyd;
struct Fixture {
    static constexpr uint32_t B=0x10000,A=B+0x2858,P=0x40000,S=0x50000;
    EventScriptAssets assets;std::map<uint32_t,uint8_t> bytes;BattleRandom rng{1};
    std::vector<int32_t> bounds;std::vector<uint32_t> writes;unsigned rawCalls=0;
    std::optional<int32_t> forcedRaw;bool mutateRaw=false,rawAvailable=true;
    Fixture(){assert(assets.load("romfs/trio-attack-events.pmevt"));word(0x8041eb90,B);word(0x8041eb00,P);
        word(B+0x2738,S-0x100);word(S-0x100+0xc,S);half(P+0x8a,1);real(B+0x163fc+0xb34,17);
        for(unsigned i=0;i<16;++i)bytes[S+0xb0+i]=i==10?5:i>=13?1:0;
    }
    uint32_t word(uint32_t a){
        if(bytes.count(a)||bytes.count(a+1)||bytes.count(a+2)||bytes.count(a+3)){
            uint32_t v=0;for(unsigned i=0;i<4;++i)v=v<<8|bytes[a+i];return v;
        }
        uint32_t v=0;if(a>=0x80000000){if(!assets.readWord(a,v)){std::fprintf(stderr,"Missing original word %08x\n",a);assert(false);}}return v;
    }
    uint8_t byte(uint32_t a){if(bytes.count(a) || a<0x80000000)return bytes[a];return word(a&~3u)>>(24-8*(a&3u));}
    void word(uint32_t a,uint32_t v){writes.push_back(a);for(unsigned i=0;i<4;++i)bytes[a+i]=v>>(24-8*i);}
    uint16_t half(uint32_t a){return uint16_t(byte(a))<<8|byte(a+1);}
    void half(uint32_t a,uint16_t v){bytes[a]=v>>8;bytes[a+1]=v;}
    void real(uint32_t a,float v){uint32_t b;std::memcpy(&b,&v,4);word(a,b);}
    float real(uint32_t a){uint32_t b=word(a);float v;std::memcpy(&v,&b,4);return v;}
    uint32_t member(unsigned i){return A+0x1bc+i*0x134;}
    BattleAudiencePopulationServices services(){
        BattleAudiencePopulationServices s;s.user=this;s.actors.user=this;s.actors.memory.user=this;
        s.actors.byte=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->byte(a);};
        auto& m=s.actors.memory;m.word=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->word(a);};
        m.half=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->half(a);};
        m.storeWord=[](void* p,uint32_t a,uint32_t v){static_cast<Fixture*>(p)->word(a,v);};
        m.storeHalf=[](void* p,uint32_t a,uint16_t v){static_cast<Fixture*>(p)->half(a,v);};
        m.storeByte=[](void* p,uint32_t a,uint8_t v){static_cast<Fixture*>(p)->bytes[a]=v;};
        s.random=[](void* p,int32_t bound){auto& f=*static_cast<Fixture*>(p);f.bounds.push_back(bound);return f.rng.bounded(bound);};
        s.randomRaw=[](void* p)->std::optional<int32_t>{auto& f=*static_cast<Fixture*>(p);++f.rawCalls;
            if(f.mutateRaw)f.word(0x8041eb90,0x20000);if(!f.rawAvailable)return {};return f.forcedRaw?*f.forcedRaw:f.rng.next();};
        s.truncate=[](void*,float v)->std::optional<int32_t>{if(std::isfinite(v) && v>=-2147483648.f && v<2147483648.f)return int32_t(v);return {};};
        s.fma=[](void*,float a,float b,float c){return std::fma(a,b,c);};return s;
    }
    void positive(int32_t count,unsigned mode=0){half(P+0x8c,1);word(B+0x19078,0x10000);word(B+0x19084,count);word(S+0xac,mode);}
};

int main(){
    Fixture f;auto a=f.services().actors;const auto r=f.member(0);
    f.word(r,1);f.word(r+0x20,0x803b4e00); // Stable animation table storage below.
    f.word(r+0x20,0x60000);for(unsigned i=0;i<4;++i)f.word(0x60000+4*i,0x70000+16*i);
    f.word(r+0x24,0x70000);f.real(r+0x6c,0);
    // Whole inclusive tilt phases return to zero, then ChangeStatus clears timer.
    for(unsigned tick=0;tick<=61;++tick){
        assert(controlPackedBattleAudienceIdleReaction(0x80192054,0,0x101,a));
        if(tick==10)assert(f.real(r+0x6c)==15);
        if(tick==40)assert(f.real(r+0x6c)==-16.5f);
        if(tick==60)assert(f.real(r+0x6c)==0);
    }
    assert(f.byte(r+0x19)==0 && f.word(r+0x30)==0);
    f.word(r+0x30,uint32_t(-1));assert(controlPackedBattleAudienceIdleReaction(0x80192164,0,0,a));
    assert(f.real(r+0xa0)==-12 && f.word(r+0x30)==0);
    for(unsigned tick=0;tick<=46;++tick){
        assert(controlPackedBattleAudienceIdleReaction(0x80192164,0,1,a));
        if(tick==15)assert(f.real(r+0xa0)==180);
        if(tick==30)assert(f.real(r+0xa0)==180);
        if(tick==45)assert(f.real(r+0xa0)==0);
    }
    assert(f.word(r+0x30)==0 && f.real(r+0xa0)==0);
    // Jump falls through phases0/1/2 in one call. Three jumps for type7.
    f.bytes[r+0x1b]=7;
    assert(controlPackedBattleAudienceIdleReaction(0x80191f20,0,1,a));
    assert(f.word(r+4)==2 && f.word(r+8)==3 && f.real(r+0xdc)==5 && f.real(r+0xe4)==-1);
    f.real(r+0xe4,0);assert(controlPackedBattleAudienceIdleReaction(0x80191f20,0,1,a));
    assert(f.word(r+4)==1 && f.word(r+8)==2);
    assert(controlPackedBattleAudienceIdleReaction(0x80191f20,0,1,a));assert(f.word(r+4)==2 && f.real(r+0xe4)==-1);
    f.word(r+8,1);f.real(r+0xe4,0);assert(controlPackedBattleAudienceIdleReaction(0x80191f20,0,1,a));
    assert(f.word(r+4)==0 && f.word(r+8)==0 && f.byte(r+0x19)==0);
    f.writes.clear();assert(!controlPackedBattleAudienceIdleReaction(0x80192260,0,0,a));assert(f.writes.empty());
    // Dispatcher forwards the full front result and all21 original target addresses.
    BattleAudienceActorControlServices s;s.actors=a;s.user=&f;
    s.front=[](void*,int32_t slot)->std::optional<int32_t>{assert(slot==0);return 0x101;};
    s.state=[](void* p,uint32_t fn,int32_t slot,int32_t front){auto& f=*static_cast<Fixture*>(p);
        assert(slot==0 && front==0x101);f.word(0x60080,fn);return false;};
    const uint32_t funcs[]{0x80192260,0x80192164,0x80192054,0x80191f20,0x80191e18,0x80191cec,0x80191bb0,
        0x80191a48,0x80191980,0x80191884,0x8019173c,0x80191470,0x80191290,0x80191058,
        0x80190d68,0x801906bc,0x80190444,0x8019024c,0x8019014c,0x8018ff4c,0x8018f9a8};
    f.real(r+0x48,10);f.real(r+0x78,3);
    for(unsigned state=0;state<21;++state){f.bytes[r+0x19]=state;assert(!controlPackedBattleAudienceActor(0,s));
        assert(f.word(0x60080)==funcs[state] && f.real(r+0x48)==10);}
    // Out-of-range byte skips state but still requires GetFront and runs motion.
    f.bytes[r+0x19]=255;f.bytes[r+0x1b]=0;f.real(r+0x84,-4);f.real(r+0xd0,-1);f.real(r+0xdc,3);f.real(r+0xe4,-1);
    assert(controlPackedBattleAudienceActor(0,s));assert(f.real(r+0x48)==13 && f.real(r+0x78)==-1);
    assert(f.real(r+0xe8)==270 && f.real(r+0xd0)==0 && f.real(r+0xdc)==0 && f.real(r+0xe4)==0);
    f.bytes[r+0x1b]=11;f.real(r+0xd0,20);f.real(r+0xdc,2);f.real(r+0xe4,1);
    assert(controlPackedBattleAudienceActor(0,s));assert(f.real(r+0xd0)==0 && f.real(r+0xdc)==0 && f.real(r+0xe4)==0);
    // Type2 wraps only once, publishes the narrowed SDK sine and final offsets.
    f.bytes[r+0x1b]=2;const float period=f.real(0x804245d0);f.real(r+0x12c,2*period);
    f.real(r+0x4c,20);f.real(r+0x7c,2);f.real(r+0xd0,3);f.real(r+0xdc,4);f.real(r+0xe4,-1);
    f.real(r+0xcc,7);f.real(r+0xd4,9);f.real(r+0xc4,-999);
    assert(!controlPackedBattleAudienceActor(0,s));assert(f.real(r+0x12c)>period && f.real(r+0xc4)==-999);
    assert(f.real(r+0xd0)==7 && f.real(r+0xdc)==3); // Stores preceding unavailable sine retained.
    f.real(r+0x12c,period);s.sine=[](void*,double phase)->std::optional<double>{assert(phase>0 && phase<0.051);return 0.25;};
    assert(controlPackedBattleAudienceActor(0,s));assert(f.real(r+0x130)==1.25f);
    assert(f.real(r+0xc4)==f.real(r+0x4c)+f.real(r+0xd0)+1.25f);
    assert(f.real(r+0xc0)==f.real(r+0x48)+7 && f.real(r+0xc8)==f.real(r+0x50)+9);
    // NaN velocity preserves prior facing; non-type2 preserves its hover storage.
    f.bytes[r+0x1b]=0;f.real(r+0x78,std::numeric_limits<float>::quiet_NaN());f.real(r+0xe8,123);
    assert(controlPackedBattleAudienceActor(0,s));assert(f.real(r+0xe8)==123 && f.real(r+0x130)==1.25f);
    f.writes.clear();s.front=nullptr;assert(!controlPackedBattleAudienceActor(0,s));assert(f.writes.empty());
    // Run the real jump state and dispatcher together through all three landings.
    Fixture cycle;const auto cr=cycle.member(0);cycle.word(cr,1);cycle.bytes[cr+0x19]=3;cycle.bytes[cr+0x1b]=7;
    cycle.word(cr+0x20,0x60000);for(unsigned i=0;i<4;++i)cycle.word(0x60000+4*i,0x70000+16*i);
    BattleAudienceActorControlServices cs;cs.actors=cycle.services().actors;cs.user=&cycle;
    cs.front=[](void*,int32_t)->std::optional<int32_t>{return 1;};
    cs.state=[](void* p,uint32_t fn,int32_t slot,int32_t front){return controlPackedBattleAudienceIdleReaction(fn,slot,front,static_cast<Fixture*>(p)->services().actors);};
    unsigned jumps=0,ticks=0;
    while(cycle.byte(cr+0x19)==3 && ticks<100){
        const auto phase=cycle.word(cr+4);
        assert(controlPackedBattleAudienceActor(0,cs));
        if(phase<2 && cycle.word(cr+4)==2)++jumps;
        ++ticks;
    }
    assert(jumps==3 && ticks<100 && cycle.byte(cr+0x19)==0);
    assert(cycle.real(cr+0xd0)==0 && cycle.real(cr+0xdc)==0 && cycle.real(cr+0xe4)==0);
}
