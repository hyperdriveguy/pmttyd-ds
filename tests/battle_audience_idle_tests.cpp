#include "battle_audience_idle.hpp"
#include "battle_audience_actor_control.hpp"
#include "battle_audience_main.hpp"
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
#include <array>
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

struct Idle : Fixture {
    int32_t draw=359999;unsigned draws=0,targets=0;float delta=0,bias=0;
    bool randomReady=true,targetReady=true;std::vector<std::array<double,3>> fused;
    Idle(){const auto r=member(20);word(r,1);bytes[r+0x1b]=0;half(r+0x1c,0xffff);
        word(r+0x20,0x60000);for(unsigned i=0;i<4;++i)word(0x60000+4*i,0x70000+16*i);
        real(r+0x64,2);real(A+0x13778,100);real(A+0x1377c,100);
        word(B+0x20,0x80000);word(0x80010,222);
    }
    BattleAudienceIdleServices idle(){BattleAudienceIdleServices s;s.actors=services().actors;s.user=this;
        s.random=[](void* p,int32_t n)->std::optional<int32_t>{auto& f=*static_cast<Idle*>(p);assert(n==360000);++f.draws;return f.randomReady?std::optional<int32_t>(f.draw):std::nullopt;};
        s.truncate=[](void*,float v)->std::optional<int32_t>{if(std::isfinite(v) && v>=-2147483648.f && v<2147483648.f)return int32_t(v);return {};};
        s.multiplySingle=[](void*,double x,double y)->std::optional<float>{return float(x*y);};
        s.fusedSingle=[](void* p,double x,double y,double z)->std::optional<float>{static_cast<Idle*>(p)->fused.push_back({x,y,z});return std::fma(float(x),float(y),float(z));};
        s.addTarget=[](void* p,float x,float y){auto& f=*static_cast<Idle*>(p);++f.targets;f.delta=x;f.bias=y;return f.targetReady;};return s;
    }
    void ratio(float v){real(A+0x1377c,v*100);}
    void state(uint8_t type,int32_t timer){bytes[member(20)+0x1b]=type;word(member(20)+0x30,uint32_t(timer));}
};
int main(){
    const auto run=[](Idle& f){return controlPackedBattleAudienceIdle(20,1,f.idle());};
    // Default species share breathing. FPR0 is a threshold, never the scale.
    for(uint8_t type:{0,1,3,5,8,10,11,12}){Idle f;f.state(type,0);f.word(f.member(20),0x21);
        assert(run(f));assert(f.real(f.member(20)+0x64)==std::fma(f.real(0x8042454c),1.f,2.f));
        assert(f.word(f.member(20)+0x30)==1 && f.draws==0);
    }
    for(float ratio:{1.f,.5f,.25f}){Idle f;f.ratio(ratio);f.word(f.member(20),0x21);
        assert(run(f));const float factor=ratio==1?1:ratio;
        assert(f.real(f.member(20)+0x64)==std::fma(f.real(0x8042454c),factor,2.f));
        f.state(0,ratio==1?16:ratio==.5f?31:61);f.real(f.member(20)+0x64,2);
        assert(run(f));assert(f.real(f.member(20)+0x64)==-std::fma(f.real(0x8042454c),factor,-2.f));
    }
    // Every long breathing boundary, including the final timer reset/increment.
    for(auto [timer,sign]:std::vector<std::pair<int,int>>{{180,0},{181,1},{188,1},{189,-1},{196,-1},{197,1},{204,1},{205,-1},{212,-1},{213,1},{220,1},{221,-1},{228,-1},{229,0}}){
        Idle f;f.state(4,timer);f.word(f.member(20),0x21);assert(run(f));
        const auto value=f.real(f.member(20)+0x64);
        if(timer==229){assert(value==1 && f.word(f.member(20)+0x30)==1);}
        else{assert(value==(sign>0?std::fma(f.real(0x8042459c),1.f,2.f):sign<0?-std::fma(f.real(0x8042459c),1.f,-2.f):2.f));assert(f.word(f.member(20)+0x30)==uint32_t(timer+1));}
    }
    // Other species reset at their own period and choose the inclusive midpoint.
    for(auto [type,period,midpoint,coefficient]:std::vector<std::array<int,4>>{{6,36,17,0x45b8},{7,22,10,0x45c0},{9,64,31,0x45cc},{0,32,15,0x454c}}){
        for(int timer:{midpoint,midpoint+1,period}){Idle f;f.state(type,timer);f.word(f.member(20),0x21);assert(run(f));
            const bool plus=timer==midpoint || timer==period;const float initial=timer==period?1.f:2.f;
            const float k=f.real(0x80420000u+uint32_t(coefficient));
            assert(f.real(f.member(20)+0x64)==(plus?std::fma(k,1.f,initial):-std::fma(k,1.f,-initial)));
            assert(f.word(f.member(20)+0x30)==uint32_t(timer==period?1:timer+1));
        }
    }
    Idle unknown;unknown.ratio(.24f);unknown.state(0,4);auto us=unknown.idle();
    assert(!controlPackedBattleAudienceIdle(20,1,us));assert(unknown.word(unknown.member(20)+0x30)==4 && unknown.real(unknown.member(20)+0x64)==2 && unknown.draws==0);
    assert(unknown.word(unknown.member(20)+0x24)==0x70000); // Animation precedes register use.
    us.callerF29=3.25;assert(controlPackedBattleAudienceIdle(20,1,us));assert(unknown.real(unknown.member(20)+0x64)==2 && unknown.word(unknown.member(20)+0x30)==5);
    Idle longUnknown;longUnknown.ratio(.1f);longUnknown.state(4,188);longUnknown.word(longUnknown.member(20),0x21);
    assert(run(longUnknown) && longUnknown.word(longUnknown.member(20)+0x30)==189);
    assert(!run(longUnknown) && longUnknown.word(longUnknown.member(20)+0x30)==189);
    Idle noBreath;noBreath.ratio(0);noBreath.state(2,17);assert(run(noBreath));assert(noBreath.word(noBreath.member(20)+0x30)==17 && noBreath.real(noBreath.member(20)+0x64)==2);
    Idle partial;partial.state(0,32);auto ps=partial.idle();ps.fusedSingle=nullptr;
    assert(!controlPackedBattleAudienceIdle(20,1,ps));assert(partial.word(partial.member(20)+0x30)==0 && partial.real(partial.member(20)+0x64)==1 && partial.draws==0);
    // A marked item suppresses random decisions after the breathing update.
    Idle item;item.half(item.member(20)+0x1c,0);assert(run(item));assert(item.draws==0 && item.word(item.member(20)+0x30)==1);
    // Damage flag wins over suppression, clears both bits and resets status.
    for(int kind:{235,236,239,240}){Idle f;f.word(0x80010,kind);f.word(f.member(20),0x61);assert(run(f));
        assert(f.word(f.member(20))==1 && f.byte(f.member(20)+0x19)==10 && f.word(f.member(20)+0x30)==0 && f.draws==0);
        assert(f.targets==unsigned(kind<236 || kind>=240));
        if(f.targets)assert(f.delta==f.real(0x80424570)*100 && f.bias==0);
    }
    Idle missingTarget;missingTarget.word(missingTarget.member(20),0x41);missingTarget.targetReady=false;
    assert(!run(missingTarget));assert(missingTarget.byte(missingTarget.member(20)+0x19)==10 && missingTarget.targets==1);
    // Actual random cutoffs, break-turn override and type11 exclusion.
    for(auto [draw,state]:std::vector<std::pair<int,int>>{{0,1},{9,1},{10,2},{19,2},{20,4},{29,4},{30,3},{329,3},{330,0}}){Idle f;f.draw=draw;f.word(Fixture::B+0x1617c,1);assert(run(f));assert(f.byte(f.member(20)+0x19)==state && f.draws==1);}
    Idle excluded;excluded.state(11,0);excluded.draw=30;excluded.word(Fixture::B+0x1617c,1);assert(run(excluded));assert(excluded.byte(excluded.member(20)+0x19)==0);
    Idle override;override.draw=30;override.word(Fixture::A+0x13910,1);assert(run(override));assert(override.byte(override.member(20)+0x19)==3);
    for(uint32_t flags:{0u,1u,3u,0x101u}){Idle f;f.ratio(1.25f);f.draw=49;f.word(f.member(0),flags);assert(run(f));assert(f.byte(f.member(20)+0x19)==(flags==1?3:0));}
    for(float ratio:{1.25f,1.5f,1.75f}){Idle f;f.ratio(ratio);const int limit=ratio==1.25f?50:ratio==1.5f?90:160;
        f.draw=limit-1;f.word(f.member(0),1);assert(run(f));assert(f.byte(f.member(20)+0x19)==3);
        Idle boundary;boundary.ratio(ratio);boundary.draw=limit;boundary.word(boundary.member(0),1);assert(run(boundary));assert(boundary.byte(boundary.member(20)+0x19)==0);
    }
    Idle noRandom;noRandom.randomReady=false;assert(!run(noRandom));assert(noRandom.word(noRandom.member(20)+0x30)==1 && noRandom.draws==1);
    Idle badConversion;auto bs=badConversion.idle();bs.truncate=nullptr;assert(!controlPackedBattleAudienceIdle(20,1,bs));assert(badConversion.draws==1);
    Idle helper;auto ha=helper.services().actors;
    assert(packedBattleAudienceOffsetSeat(20,0,1,ha)==0);
    assert(packedBattleAudienceOffsetSeat(0,0,1,ha)==-1);
    assert(packedBattleAudienceOffsetSeat(19,1,0,ha)==-1);
    assert(packedBattleAudienceOffsetSeat(20,-1,0,ha)==-1);
    assert(packedBattleAudienceOffsetSeat(199,0,-1,ha)==-1);
    helper.word(Fixture::B+0x1617c,0xffffffff);assert(packedBattleAudienceBreakTurn(ha)==-1);
    // Original positive population -> whole Control -> member dispatch -> Wait,
    // sharing the actual source RNG across twenty controller ticks.
    Idle crowd;for(unsigned i=0;i<200;++i)crowd.word(crowd.member(i),0);
    crowd.positive(8);crowd.real(Fixture::P+0x84,8);
    assert(initializePackedBattleAudiencePopulation(crowd.services()));
    BattleAudienceControlServices controller;controller.actors=crowd.services().actors;controller.user=&crowd;
    controller.actor=[](void* p,uint32_t,int32_t slot,bool){auto& f=*static_cast<Idle*>(p);
        BattleAudienceActorControlServices actor;actor.actors=f.services().actors;actor.user=p;
        actor.front=[](void*,int32_t)->std::optional<int32_t>{return 1;}; // Explicit test camera facing.
        actor.sine=[](void*,double v)->std::optional<double>{return std::sin(v);};
        actor.state=[](void* p,uint32_t fn,int32_t slot,int32_t front){auto& f=*static_cast<Idle*>(p);
            if(fn!=0x80192260)return controlPackedBattleAudienceIdleReaction(fn,slot,front,f.services().actors);
            auto idle=f.idle();idle.random=[](void* p,int32_t n)->std::optional<int32_t>{auto& f=*static_cast<Idle*>(p);++f.draws;return f.rng.bounded(n);};
            return controlPackedBattleAudienceIdle(slot,front,idle);
        };
        return controlPackedBattleAudienceActor(slot,actor);
    };
    for(unsigned tick=0;tick<20;++tick)assert(packedBattleAudienceControl(controller));
    assert(crowd.word(Fixture::A+0x13784)==8 && crowd.draws>0);
    unsigned active=0,advanced=0;for(unsigned i=0;i<200;++i)if(crowd.word(crowd.member(i))&1){++active;advanced+=crowd.word(crowd.member(i)+0x30)>0;}
    assert(active==8 && advanced>0);
}
