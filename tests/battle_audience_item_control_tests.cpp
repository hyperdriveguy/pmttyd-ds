#include "battle_audience_actor_control.hpp"
#include "battle_audience_item_control.hpp"
#include "battle_audience_sound_start.hpp"
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

struct Items : Fixture {
    unsigned targets=0,throws=0,sounds=0;float delta=0;
    bool targetReady=true,throwReady=true,soundReady=true;uint32_t key=0,position=0;
    std::vector<uint8_t> states;
    Items(){const auto r=member(20);word(r,1);bytes[r+0x19]=7;half(r+0x1c,3);word(r+0x20,0x60000);
        for(unsigned i=0;i<10;++i)word(0x60000+4*i,0x70000+16*i);
        real(r+0x64,1);real(A+0x13778,100);word(item(3),1);word(item(3)+4,1);word(item(3)+8,2);
    }
    uint32_t item(int32_t n){return A+0xf25c+uint32_t(n)*0x48;}
    void ended(unsigned animation){const auto r=member(20);word(r+0x24,0x70000+16*animation);word(r+0x28,0x71000);word(0x71000,0xffffffff);word(r+0x2c,0);}
    BattleAudienceItemControlServices control(){BattleAudienceItemControlServices s;s.actors=services().actors;s.user=this;
        s.actors.memory.storeByte=[](void* p,uint32_t a,uint8_t v){auto& f=*static_cast<Items*>(p);f.bytes[a]=v;if(a==f.member(20)+0x19)f.states.push_back(v);};
        s.randomRaw=services().randomRaw;
        s.addTarget=[](void* p,float x,float y){auto& f=*static_cast<Items*>(p);assert(y==0);++f.targets;f.delta=x;return f.targetReady;};
        s.throwSound=[](void* p,int32_t kind,int32_t slot){auto& f=*static_cast<Items*>(p);assert(kind==0 && slot==20 && f.byte(f.member(20)+0x19)==8);++f.throws;return f.throwReady;};
        s.soundAt=[](void* p,uint32_t key,uint32_t at){auto& f=*static_cast<Items*>(p);++f.sounds;f.key=key;f.position=at;return f.soundReady;};return s;
    }
};
int main(){
    const auto run=[](Items& f,uint32_t fn,int front=0){return controlPackedBattleAudienceItemState(fn,20,front,f.control());};
    Items addresses;auto a=addresses.services().actors;
    assert(packedBattleAudienceItemAddress(0,a)==addresses.item(0));assert(packedBattleAudienceItemAddress(99,a)==addresses.item(99));
    assert(packedBattleAudienceItemAddress(-1,a)==addresses.item(-1)); // Source arithmetic has no clamping.
    // Animation choice precedes all hold/countdown guards.
    for(int front:{0,1,0x101}){Items f;assert(run(f,0x80191a48,front));assert(f.word(f.member(20)+0x24)==(uint8_t(front)==1?0x70000u:0x70050u));assert(f.word(f.item(3)+8)==2 && f.throws==0);}
    for(uint32_t flags:{0x21u,1u}){Items f;f.word(Items::B+0xef4,0x80);f.word(f.member(20),flags);f.word(Items::A,flags==1?0x40000:0);
        assert(run(f,0x80191a48));assert(f.word(f.item(3)+8)==2 && f.throws==0);}
    Items countdown;const auto cr=countdown.member(20);countdown.word(Items::B+0xef4,0x80);
    assert(run(countdown,0x80191a48));assert(countdown.word(countdown.item(3)+8)==1 && countdown.byte(cr+0x19)==7);
    assert(run(countdown,0x80191a48));assert(countdown.word(countdown.item(3)+8)==0 && countdown.byte(cr+0x19)==8 && countdown.throws==1);
    Items inactive;inactive.word(Items::B+0xef4,0x80);inactive.word(inactive.item(3),0);inactive.word(inactive.item(3)+8,0);
    assert(run(inactive,0x80191a48));assert(inactive.word(inactive.item(3)+8)==0xffffffff && inactive.throws==0);
    Items otherState;otherState.word(Items::B+0xef4,0x80);otherState.word(otherState.item(3)+4,2);otherState.word(otherState.item(3)+8,1);
    assert(run(otherState,0x80191a48) && otherState.word(otherState.item(3)+8)==0 && otherState.throws==0);
    Items wrap;wrap.word(Items::B+0xef4,0x80);wrap.word(wrap.item(3)+8,0x80000000);
    assert(run(wrap,0x80191a48) && wrap.word(wrap.item(3)+8)==0x7fffffff && wrap.throws==0);
    Items missingThrow;missingThrow.word(Items::B+0xef4,0x80);missingThrow.word(missingThrow.item(3)+8,1);missingThrow.throwReady=false;
    assert(!run(missingThrow,0x80191a48) && missingThrow.byte(missingThrow.member(20)+0x19)==8 && missingThrow.throws==1);
    // Interrupt destroys the held item but penalty classification reads item0.
    for(int kind:{235,236,239,240}){Items f;const auto r=f.member(20);f.word(r,0x61);f.word(f.item(0)+0x10,kind);f.word(f.item(3)+0x10,kind==236?235:236);
        assert(run(f,0x80191a48));assert(f.word(r)==1 && f.half(r+0x1c)==0xffff && f.word(f.item(3))==0 && f.byte(r+0x19)==10);
        assert(f.targets==unsigned(kind<236 || kind>=240) && f.throws==0);
        if(f.targets)assert(f.delta==f.real(0x80424570)*100);
    }
    Items targetHold;targetHold.word(targetHold.member(20),0x41);targetHold.targetReady=false;
    assert(!run(targetHold,0x80191a48));assert(targetHold.byte(targetHold.member(20)+0x19)==10 && targetHold.word(targetHold.item(3))==0);
    Items finished;finished.ended(6);finished.bytes[finished.member(20)+0x19]=8;
    assert(run(finished,0x80191980) && finished.states==std::vector<uint8_t>{0});
    Items interrupted;interrupted.ended(6);interrupted.word(interrupted.member(20),0x61);
    assert(run(interrupted,0x80191980));assert(interrupted.states==std::vector<uint8_t>({0,10}) && interrupted.targets==1);
    // Complete escape eligibility, including the fresh SysCtrl/existence gates.
    for(unsigned state=0;state<256;++state){Items f;f.bytes[f.member(20)+0x19]=uint8_t(state);
        const bool blocked=state==12 || state==13 || state==14 || state==15 || state==17 || state==18 || state==21 || state==22;
        assert(packedBattleAudienceEscapeAllowed(20,f.services().actors)==!blocked);}
    for(uint32_t flags:{0u,3u,0x101u,0x11u,0x201u}){Items f;f.word(f.member(20),flags);assert(!packedBattleAudienceEscapeAllowed(20,f.services().actors));}
    Items locked;locked.word(Items::A,0x4000);assert(!packedBattleAudienceEscapeAllowed(20,locked.services().actors));
    Items damage;const auto dr=damage.member(20);damage.bytes[dr+0x19]=10;
    assert(run(damage,0x8019173c));assert(damage.word(dr+4)==1 && damage.word(dr+0x30)==1 && damage.real(dr+0xdc)==8 && damage.real(dr+0xe4)==-1 && damage.real(dr+0xe8)==270);
    assert(damage.real(dr+0x64)>.89f && damage.real(dr+0x64)<.91f);
    for(unsigned tick=1;tick<8;++tick)assert(run(damage,0x8019173c));assert(damage.word(dr+0x30)==8);
    assert(run(damage,0x8019173c) && damage.word(dr+0x30)==1 && damage.real(dr+0x64)>.89f);
    damage.ended(9);assert(run(damage,0x8019173c));assert(damage.byte(dr+0x19)==12 && damage.sounds==1 && damage.key==0x802f9698 && damage.position==dr+0x48 && damage.real(dr+0x64)==1);
    Items cannotEscape;const auto nr=cannotEscape.member(20);cannotEscape.word(nr+4,1);cannotEscape.word(Items::A,0x4000);cannotEscape.ended(9);
    assert(run(cannotEscape,0x8019173c) && cannotEscape.byte(nr+0x19)==0 && cannotEscape.sounds==0);
    Items missingSound;missingSound.word(missingSound.member(20)+4,1);missingSound.ended(9);missingSound.soundReady=false;
    assert(!run(missingSound,0x8019173c) && missingSound.byte(missingSound.member(20)+0x19)==12);
    // Ordinary retreat launch has no same-call phase1/2 fallthrough.
    for(int raw:{0,32767}){Items f;const auto r=f.member(20);f.forcedRaw=raw;f.real(r+0x48,120);f.real(r+0x50,600);
        assert(run(f,0x80191290));assert(f.word(r+4)==1 && f.word(r)&1 && f.rawCalls==1 && f.sounds==1 && f.key==0x802f9684);
        assert(f.real(r+0x78)==(raw==0?-2.f:0.f) && f.real(r+0x80)==8 && f.real(r+0xdc)==6 && f.real(r+0xe4)==-.5f);
        f.real(r+0xe4,0);assert(run(f,0x80191290) && f.word(r+4)==2 && f.word(r)&1);
        assert(run(f,0x80191290) && f.word(r)==0);
    }
    Items rawHold;rawHold.rawAvailable=false;assert(!run(rawHold,0x80191290));assert(rawHold.word(rawHold.member(20)+8)==0xffffffff && rawHold.word(rawHold.member(20)+4)==0);
    Items sinking;const auto sr=sinking.member(20);sinking.bytes[sr+0x1b]=11;sinking.word(sr+8,2);sinking.real(sr+0x4c,-100);
    assert(run(sinking,0x80191290) && sinking.word(sr+4)==0 && sinking.word(sr+8)==1);
    assert(run(sinking,0x80191290) && sinking.word(sr+4)==1 && sinking.real(sr+0x7c)==-1 && sinking.word(sr)&1);
    assert(run(sinking,0x80191290) && sinking.word(sr)==0 && sinking.word(sr+4)==2 && sinking.rawCalls==0 && sinking.sounds==0);
    // Actual member motion drives both escape branches to deletion.
    for(uint8_t type:{0,11}){Items f;const auto r=f.member(20);f.bytes[r+0x1b]=type;f.bytes[r+0x19]=12;f.word(r+8,0);f.forcedRaw=0;
        BattleAudienceActorControlServices actor;actor.actors=f.control().actors;actor.user=&f;
        actor.front=[](void*,int32_t)->std::optional<int32_t>{return 0;};
        actor.state=[](void* p,uint32_t fn,int32_t slot,int32_t front){return controlPackedBattleAudienceItemState(fn,slot,front,static_cast<Items*>(p)->control());};
        unsigned ticks=0;while((f.word(r)&1) && ticks<200){assert(controlPackedBattleAudienceActor(20,actor));++ticks;}
        assert(!(f.word(r)&1) && ticks<200 && f.rawCalls==(type==11?0u:1u));
    }
    // Whole throw-sound helper captures lane before stopping the previous sound,
    // but obtains fresh member position for Sound2 afterwards.
    Items audio;const auto ar=audio.member(20);audio.real(ar+0x48,-1);audio.real(ar+0x4c,2);audio.real(ar+0x50,3);
    auto start=BattleAudienceSoundStartServices{};start.actors=audio.services().actors;start.user=&audio;
    start.on=[](void* p,uint32_t key,const std::array<float,3>& position)->std::optional<int32_t>{auto& f=*static_cast<Items*>(p);++f.sounds;assert((key==0x802fa43c && position==std::array<float,3>({50,2,3})));return 7;};
    BattleAudienceSoundServices sound;sound.actors=start.actors;sound.user=&audio;
    const auto lane=Items::A+0x133fc+10*0x24;audio.word(lane,0x21);audio.word(lane+4,9);
    sound.stop=[](void* p,int32_t handle){auto& f=*static_cast<Items*>(p);assert(handle==9);f.real(f.member(20)+0x48,50);return true;};
    sound.volume=[](void*,int32_t handle)->std::optional<uint8_t>{assert(handle==7);return 87;};
    assert(playPackedBattleAudienceThrowSound(0,20,start,sound));assert(audio.word(lane)==0x21 && audio.word(lane+4)==7 && audio.word(lane+0xc)==0xffffffff && audio.word(lane+0x10)==0);
    assert(audio.byte(lane+0x1c)==87 && audio.byte(lane+0x1d)==87 && audio.byte(lane+0x1e)==100 && audio.word(lane+0x14)==0 && audio.word(lane+0x18)==0);
    Items rejected;rejected.real(rejected.member(20)+0x48,1);start.actors=rejected.services().actors;start.user=&rejected;sound.actors=start.actors;sound.user=&rejected;
    start.on=[](void*,uint32_t key,const std::array<float,3>&)->std::optional<int32_t>{assert(key==0x802fa458);return -1;};
    const auto right=Items::A+0x133fc+11*0x24;rejected.word(right+0xc,123);
    assert(playPackedBattleAudienceThrowSound(1,20,start,sound) && rejected.word(right+4)==0xffffffff && rejected.word(right)==0 && rejected.word(right+0xc)==123);
    Items partialAudio;start.actors=partialAudio.services().actors;start.user=&partialAudio;sound.actors=start.actors;sound.user=&partialAudio;
    start.on=[](void*,uint32_t,const std::array<float,3>&)->std::optional<int32_t>{return 7;};sound.volume=nullptr;
    partialAudio.bytes[lane+0x1c]=19;
    assert(!playPackedBattleAudienceThrowSound(0,20,start,sound));
    assert(partialAudio.word(lane)&1 && partialAudio.word(lane+4)==7 && partialAudio.word(lane+0xc)==0xffffffff && partialAudio.byte(lane+0x1c)==19);
    for(float x:{-1.f,0.f,1.f,std::numeric_limits<float>::quiet_NaN()}){Items f;f.real(f.member(20)+0x48,x);
        start.actors=f.services().actors;start.user=&f;sound.actors=start.actors;sound.user=&f;
        start.on=[](void*,uint32_t,const std::array<float,3>&)->std::optional<int32_t>{return -1;};
        assert(playPackedBattleAudienceThrowSound(0,20,start,sound));
        const auto expected=Items::A+0x133fc+(x<=0?10:11)*0x24;
        assert(f.word(expected+4)==0xffffffff);
    }
    Items unsupported;unsupported.writes.clear();assert(!run(unsupported,0x80191470) && unsupported.writes.empty());
}
