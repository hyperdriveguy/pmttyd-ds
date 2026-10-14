#include "battle_audience_actor_control.hpp"
#include "battle_audience_idle.hpp"
#include "field_camera_events.hpp"
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

struct Reaction : Fixture {
    int32_t bounded=0;bool boundedReady=true,curveReady=true;
    struct Curve {int32_t mode,step,duration;float start,end;};std::vector<Curve> curves;
    Reaction(){const auto r=member(0);word(r,1);word(r+0x20,0x60000);
        for(unsigned i=0;i<5;++i)word(0x60000+4*i,0x70000+16*i);
        real(A+0x13778,100);real(A+0x1377c,100);
    }
    BattleAudienceReactionServices reaction(){BattleAudienceReactionServices s;s.actors=services().actors;s.user=this;
        s.random=[](void* p,int32_t n)->std::optional<int32_t>{auto& f=*static_cast<Reaction*>(p);assert(n==2);f.bounds.push_back(n);return f.boundedReady?std::optional<int32_t>(f.bounded):std::nullopt;};
        s.randomRaw=services().randomRaw;
        s.interpolate=[](void* p,int32_t mode,int32_t step,int32_t duration,float start,float end)->std::optional<float>{auto& f=*static_cast<Reaction*>(p);f.curves.push_back({mode,step,duration,start,end});return f.curveReady?cameraInterpolation(mode,step,duration,start,end):std::nullopt;};return s;
    }
};
int main(){
    const auto run=[](Reaction& f,uint32_t fn,int front=0){return controlPackedBattleAudienceReaction(fn,0,front,f.reaction());};
    // Cheer animation is selected only for low-byte0; Booing always selects it.
    for(uint32_t fn:{0x80191e18u,0x80191884u})for(int front:{0,1,0x100,0x101}){
        Reaction f;const auto r=f.member(0);f.word(r+0x24,0x76543);f.word(r+4,2);
        assert(run(f,fn,front));assert(f.word(r+0x24)==((fn==0x80191884 || uint8_t(front)==0)?0x70040u:0x76543u));
        assert(f.bounds.empty() && f.curves.empty());
    }
    // One source RNG call at phase0; real unclamped mode4 curve spans0..30.
    for(int draw:{0,1}){Reaction f;f.bounded=draw;const auto r=f.member(0);f.bytes[r+0x19]=4;
        for(unsigned step=0;step<=30;++step){assert(run(f,0x80191e18));
            const auto c=f.curves.back();assert(c.mode==4 && c.step==int(step) && c.duration==15 && c.start==0 && c.end==10);
            if(step==0)assert(f.real(r+0x70)==0 && f.word(r+4)==1 && f.word(r+8)==1 && f.real(r+0xe8)==(draw?90:270));
            if(step==15)assert(f.real(r+0x70)==10);
            if(step==29)assert(f.byte(r+0x19)==4 && f.word(r+8)==30);
        }
        assert(f.byte(r+0x19)==0 && f.word(r+4)==0 && f.word(r+8)==0 && f.real(r+0x70)==0);
        assert(f.bounds==std::vector<int32_t>{2});
    }
    Reaction repeats;const auto rr=repeats.member(0);repeats.real(rr+0xc,2.5f);repeats.bytes[rr+0x19]=9;
    for(unsigned i=0;i<93;++i)assert(run(repeats,0x80191884,1));
    assert(repeats.byte(rr+0x19)==0 && repeats.curves.size()==93 && repeats.bounds.size()==1);
    Reaction unordered;const auto ur=unordered.member(0);unordered.word(ur+4,1);unordered.word(ur+8,30);
    unordered.real(ur+0xc,std::numeric_limits<float>::quiet_NaN());unordered.bytes[ur+0x19]=4;
    assert(run(unordered,0x80191e18));assert(unordered.byte(ur+0x19)==4 && unordered.word(ur+8)==0 && std::isnan(unordered.real(ur+0xc)));
    Reaction missing;missing.boundedReady=false;const auto mr=missing.member(0);
    assert(!run(missing,0x80191e18));assert(missing.word(mr+0x24)==0x70040 && missing.word(mr+4)==0 && missing.curves.empty());
    missing.boundedReady=true;missing.curveReady=false;
    assert(!run(missing,0x80191e18));assert(missing.word(mr+4)==1 && missing.word(mr+8)==0 && missing.real(mr+0xe8)==270);
    const auto calls=missing.bounds.size();missing.curveReady=true;assert(run(missing,0x80191e18));assert(missing.bounds.size()==calls && missing.word(mr+8)==1);
    Reaction wrap;const auto wr=wrap.member(0);wrap.word(wr+4,1);wrap.word(wr+8,0x7fffffff);
    assert(run(wrap,0x80191e18));assert(wrap.curves.back().step==0x7fffffff && wrap.word(wr+8)==0x80000000);
    // Raw rand endpoint is included. Conversion/division/add are separate singles.
    for(uint32_t fn:{0x80191cecu,0x80191bb0u})for(int raw:{0,32767})for(int front:{0,1,0x101}){
        Reaction f;f.forcedRaw=raw;const auto r=f.member(0);assert(run(f,fn,front));
        assert(f.word(r+0x24)==(uint8_t(front)==1?0x70020u:0x70030u));
        assert(f.word(r+4)==2 && f.word(r+8)==(fn==0x80191cec?1u:2u));
        const float start=fn==0x80191cec?f.real(0x8042457c):4.f;
        const float fraction=float(raw)/f.real(0x80424554);assert(f.real(r+0xdc)==start+fraction && f.real(r+0xe4)==f.real(0x80424578));
        assert(f.rawCalls==1 && f.real(r+0xa0)==(fn==0x80191bb0?20.f:0.f));
        assert(run(f,fn,front));assert(f.rawCalls==1); // Phase2 never redraws velocity.
    }
    Reaction rawMissing;rawMissing.rawAvailable=false;const auto rm=rawMissing.member(0);
    assert(!run(rawMissing,0x80191bb0));assert(rawMissing.word(rm+4)==1 && rawMissing.word(rm+8)==2 && rawMissing.real(rm+0xa0)==0);
    rawMissing.rawAvailable=true;rawMissing.forcedRaw=32767;assert(run(rawMissing,0x80191bb0));assert(rawMissing.word(rm+4)==2 && rawMissing.real(rm+0xdc)==5 && rawMissing.real(rm+0xa0)==20);
    Reaction turn;const auto tr=turn.member(0);turn.word(tr+4,2);turn.real(tr+0xa0,350);turn.real(tr+0xe4,-.2f);
    assert(run(turn,0x80191bb0));assert(turn.real(tr+0xa0)==370 && turn.rawCalls==0);
    // Invalid phases still perform their original animation call, without RNG.
    for(int phase:{-1,3}){Reaction f;f.word(f.member(0)+4,uint32_t(phase));assert(run(f,0x80191bb0,1));assert(f.rawCalls==0 && f.real(f.member(0)+0xa0)==0);}
    // Run jump bodies through the actual ordered member motion tail and landings.
    for(uint32_t fn:{0x80191cecu,0x80191bb0u}){Reaction f;const auto r=f.member(0);f.forcedRaw=32767;f.bytes[r+0x19]=fn==0x80191cec?5:6;
        BattleAudienceActorControlServices actor;actor.actors=f.services().actors;actor.user=&f;
        actor.front=[](void*,int32_t)->std::optional<int32_t>{return 1;};
        actor.state=[](void* p,uint32_t fn,int32_t slot,int32_t front){return controlPackedBattleAudienceReaction(fn,slot,front,static_cast<Reaction*>(p)->reaction());};
        unsigned ticks=0;while(f.byte(r+0x19)!=0 && ticks<200){assert(controlPackedBattleAudienceActor(0,actor));++ticks;}
        assert(ticks<200 && f.rawCalls==(fn==0x80191cec?1u:2u));
        assert(f.real(r+0xd0)==0 && f.real(r+0xdc)==0 && f.real(r+0xe4)==0 && f.real(r+0xa0)==0);
    }
    // An actual Idle state4 decision now proceeds through cheering back to idle.
    Reaction decision;const auto dr=decision.member(0);decision.half(dr+0x1c,0xffff);
    // Supplied test seed yields the source state4 choice; seed ownership is
    // explicit and is not a claim about original new-game seed production.
    decision.rng.state=27120;BattleRandom oracle=decision.rng;
    assert(oracle.bounded(360000)==20);const auto facing=oracle.bounded(2);
    BattleAudienceActorControlServices chain;chain.actors=decision.services().actors;chain.user=&decision;
    chain.front=[](void*,int32_t)->std::optional<int32_t>{return 0;};
    chain.state=[](void* p,uint32_t fn,int32_t slot,int32_t front){auto& f=*static_cast<Reaction*>(p);
        if(fn!=0x80192260){auto reaction=f.reaction();
            reaction.random=[](void* p,int32_t n)->std::optional<int32_t>{auto& f=*static_cast<Reaction*>(p);f.bounds.push_back(n);return f.rng.bounded(n);};
            return controlPackedBattleAudienceReaction(fn,slot,front,reaction);
        }
        BattleAudienceIdleServices idle;idle.actors=f.services().actors;idle.user=p;
        idle.random=[](void* p,int32_t n)->std::optional<int32_t>{assert(n==360000);auto& f=*static_cast<Reaction*>(p);f.bounds.push_back(n);return f.rng.bounded(n);};
        idle.truncate=[](void*,float v)->std::optional<int32_t>{return int32_t(v);};
        idle.multiplySingle=[](void*,double a,double b)->std::optional<float>{return float(a*b);};
        idle.fusedSingle=[](void*,double a,double b,double c)->std::optional<float>{return std::fma(float(a),float(b),float(c));};
        return controlPackedBattleAudienceIdle(slot,front,idle);
    };
    assert(controlPackedBattleAudienceActor(0,chain));assert(decision.byte(dr+0x19)==4);
    for(unsigned tick=0;tick<31;++tick){assert(controlPackedBattleAudienceActor(0,chain));if(tick==0)assert(decision.real(dr+0xe8)==(facing?90:270));}
    assert(decision.byte(dr+0x19)==0 && decision.bounds==std::vector<int32_t>({360000,2}));
    assert(decision.rng.state==oracle.state);
    Reaction unsupported;unsupported.writes.clear();assert(!run(unsupported,0x8019173c));assert(unsupported.writes.empty());
}
