#include "battle_audience_population.hpp"
#include "battle_audience_init.hpp"
#include "battle_audience_resources.hpp"
#include "battle_audience_sound.hpp"
#include "battle_display_footer.hpp"
#include "battle_timing_a.hpp"
#include "event_script_assets.hpp"
#include "trio_attack.hpp"
#include "game.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <cstdio>
#include <map>
#include <vector>
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
    // Empty setup4 agrees with the previously verified whole used graph and RNG.
    for(uint32_t seed=1;seed<=20;++seed){
        Fixture original,expanded;original.rng.state=expanded.rng.state=seed;
        auto os=original.services();BattleAudienceInitServices bounded;bounded.actors=os.actors;bounded.user=&original;bounded.random=os.random;bounded.fma=os.fma;
        assert(initializePackedSetup4ZeroAudience(bounded));assert(initializePackedBattleAudiencePopulation(expanded.services()));
        assert(original.rng.state==expanded.rng.state && original.bounds==expanded.bounds && expanded.rawCalls==0);
        for(unsigned i=0;i<12;++i)assert(original.word(Fixture::A+0x13794+4*i)==expanded.word(Fixture::A+0x13794+4*i));
        assert(expanded.word(Fixture::A+0x13790)==0 && expanded.word(expanded.member(0))==0);
    }
    // Authentic supplied positive input produces actual Goomba records and source animation pointers.
    BattleAudienceResources resource;assert(resource.loadFrames("romfs"));
    Fixture crowd;crowd.positive(8);crowd.word(Fixture::B+0x19088,2); // Original override2=>type10.
    assert(initializePackedBattleAudiencePopulation(crowd.services()));
    unsigned count=0;for(unsigned i=0;i<200;++i){const auto r=crowd.member(i);if(!(crowd.word(r)&1))continue;++count;
        assert(crowd.byte(r+0x1b)==10 || crowd.byte(r+0x1b)==12);
        assert(crowd.word(r+0x24)==crowd.word(crowd.word(r+0x20)+4));
        assert(crowd.word(r+0x24)==crowd.word(r+0x28));assert(crowd.real(r+0x4c)==17);
        assert(crowd.half(r+0x1c)==0xffff && crowd.byte(r+0x44)==255);
    }assert(count==8 && crowd.rawCalls>=16);
    BattleAudienceAnimationServices animations;animations.actors=crowd.services().actors;animations.user=&crowd;
    animations.scriptWord=[](void* p,uint32_t a)->std::optional<uint32_t>{return static_cast<Fixture*>(p)->word(a);};
    BattleAudienceAnimationCursor cursor;
    for(unsigned tick=0;tick<30;++tick){assert(processPackedBattleAudienceAnimations(crowd.member(0),animations,cursor));
        for(unsigned i=0;i<200;++i){const auto r=crowd.member(i);if(!(crowd.word(r)&1))continue;
            assert(resource.frame(crowd.byte(r+0x1b)==12?2:1,crowd.byte(r+0x1a)));}
    }
    // Entry rejection consumes no RNG or initialization and preserves other bytes.
    Fixture entry;entry.forcedRaw=32767;auto es=entry.services();const auto r=entry.member(4);
    assert(packedBattleAudienceEntry(4,10,11,es)==1 && entry.real(r+0x48)==-125 && entry.real(r+0xb4)==-125);
    assert(entry.real(r+0x4c)==-2000 && entry.real(r+0xb8)==17 && (entry.word(r)&8));
    assert(entry.byte(r+0x1b)==10 && entry.word(r+0x20)==entry.word(0x803b4de8+10*32));
    auto draws=entry.rawCalls;assert(packedBattleAudienceEntry(4,10,0,es)==0 && entry.rawCalls==draws);
    entry.word(entry.member(5),0x100);assert(packedBattleAudienceEntry(5,10,0,es)==0 && entry.rawCalls==draws);
    // Paired type9 reserves its neighbor, keeps original placement and routes status through it.
    assert((entry.byte(0x803b4de4+9*32)&1)!=0);
    assert(packedBattleAudienceEntry(7,9,0,es)==0 && entry.rawCalls==draws);
    assert(packedBattleAudienceEntry(6,9,0,es)==1 && entry.word(entry.member(7))==0x101);
    assert(entry.real(entry.member(6)+0x48)==-62.5f);
    packedBattleAudienceChangeStatus(7,13,es.actors);assert(entry.byte(entry.member(6)+0x19)==13 && entry.byte(entry.member(7)+0x19)==0);
    const auto untouched=entry.word(entry.member(6)+0x20);packedBattleAudienceDelete(6,es.actors);
    assert(entry.word(entry.member(6))==0 && entry.word(entry.member(7))==0 && entry.word(entry.member(6)+0x20)==untouched);
    // Animation reset is forced only when requested; frame byte is not reset by SetAnim.
    entry.word(r+0x2c,23);entry.bytes[r+0x1a]=99;packedBattleAudienceSetAnimation(4,1,0,es.actors);
    assert(entry.word(r+0x2c)==23 && entry.byte(r+0x1a)==99);
    packedBattleAudienceSetAnimation(4,1,1,es.actors);assert(entry.word(r+0x2c)==0 && entry.byte(r+0x1a)==99);
    const uint32_t end=0x70000;entry.word(end,~0u);entry.word(r+0x28,end);
    assert(packedBattleAudienceAnimationEnded(4,es.actors)==1);entry.word(r+0x2c,1);assert(packedBattleAudienceAnimationEnded(4,es.actors)==0);
    entry.word(r+0x2c,0x80000000);assert(packedBattleAudienceAnimationEnded(4,es.actors)==1); // Actual cntlzw/rotate result.
    // Source captures Entry's record before rand, while its Sub requeries the new battle.
    Fixture changed;changed.mutateRaw=true;changed.forcedRaw=0;
    assert(packedBattleAudienceEntry(2,10,0,changed.services())==1);
    assert(changed.word(changed.member(2))==5 && changed.real(changed.member(2)+0x48)==-185);
    assert(changed.byte(0x20000+0x2858+0x1bc+2*0x134+0x1b)==10);
    // Full seat scan performs raw rand even with no candidates, and retains exclusive endpoint.
    Fixture seats;seats.forcedRaw=32767;
    assert(packedBattleAudienceFreeSeat(true,seats.services())==-1 && seats.rawCalls==1);
    seats.forcedRaw=0;const auto first=packedBattleAudienceFreeSeat(true,seats.services());assert(first && *first>=4 && *first<=15);
    for(unsigned i=0;i<200;++i)seats.word(seats.member(i),1);
    assert(packedBattleAudienceFreeFrontThenBack(seats.services())==-1 && seats.rawCalls==4);
    const auto drawsBefore=seats.bounds.size();assert(packedBattleAudienceFreeSeatUniform(seats.services())==-1 && seats.bounds.size()==drawsBefore);
    // Missing raw service keeps Entry's already published flags; no successful stub.
    Fixture missing;missing.rawAvailable=false;assert(!packedBattleAudienceEntry(0,10,0,missing.services()));
    assert(missing.word(missing.member(0))==5 && missing.word(missing.member(0)+0x20)==0);
    // All original finite setup modes and override choices produce their own capacity/weights.
    for(unsigned mode=0;mode<=6;++mode){Fixture f;f.positive(7,mode);assert(initializePackedBattleAudiencePopulation(f.services()));
        if(mode==2){unsigned used=0;for(unsigned i=0;i<200;++i)used+=(f.word(f.member(i))&1)!=0;assert(used==50);}
        if(mode==5)assert(f.word(Fixture::A+0x13794+4)==1);
        if(mode==6)assert(f.word(Fixture::A+0x13794+9*4)==2);
    }
    for(unsigned override=1;override<=12;++override){Fixture f;f.positive(6);f.word(Fixture::B+0x19088,override);
        assert(initializePackedBattleAudiencePopulation(f.services()));unsigned nonzero=0;
        for(unsigned type=0;type<12;++type)nonzero+=f.word(Fixture::A+0x13794+4*type)!=0;assert(nonzero==1);
    }
    // Whole original Init reaches its real known-key audio boundary after publishing the crowd.
    Fixture full;full.positive(8);BattleAudienceInitServices initialization;auto population=full.services();
    initialization.actors=population.actors;initialization.user=&full;initialization.random=population.random;
    initialization.setting=[](void* p){return initializePackedBattleAudiencePopulation(static_cast<Fixture*>(p)->services());};
    initialization.texture=[](void*,const char* name)->std::optional<uint32_t>{return std::strstr(name,"normal")?1u:2u;};
    initialization.noise=[](void* p){auto& f=*static_cast<Fixture*>(p);BattleAudienceSoundServices sound;sound.actors=f.services().actors;sound.user=p;
        sound.play=[](void*,uint32_t key,bool assigned,int32_t right,int32_t left)->std::optional<int32_t>{assert(key==0x802fa574 && assigned && right+left==8);return {};};
        return noiseAlwaysBattleAudienceSound(sound);};
    assert(!initializePackedBattleAudience(initialization));
    assert(full.word(Fixture::A+0x13784)==8 && full.real(Fixture::A+0x1375c+0x18)==8 && full.word(Fixture::A+0xc)==1);
    // Native supplied positive pouch preserves the existing unavailable audio frontier.
    // Do not substitute a -1 known-key sound result to make positive startup succeed.
    assert(loadProfessorStoryAssets("romfs") && loadUndergroundStoryAssets("romfs") && loadTrioBattleAssets("romfs") && loadTrioAttackAssets("romfs"));
    ProfessorStory scene;assert(scene.beginTrioBattleIntro(0));
    TrioBattleCommands menu;assert(menu.begin(scene,1));menu.input(0x100,0);menu.input(0x100,0);
    assert(loadProloguePouchAssets("romfs"));auto pouch=createProloguePouch();assert(pouch);pouch->half(0x8c,1);
    pouch->bytes[0x84]=0x41;pouch->bytes[0x85]=pouch->bytes[0x86]=pouch->bytes[0x87]=0;
    auto rng=std::make_shared<BattleRandom>(1);TrioAttack native;
    assert(!native.begin(menu,scene,10,pouch,nullptr,nullptr,nullptr,nullptr,{},rng));assert(rng->state!=1);
}
