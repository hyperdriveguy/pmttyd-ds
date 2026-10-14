#include "battle_audience_init.hpp"
#include "battle_audience_resources.hpp"
#include "battle_audience_sound.hpp"
#include "battle_audience_target.hpp"
#include "battle_timing_a.hpp"
#include "event_script_assets.hpp"
#include "game.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <map>
#include <vector>
using namespace ttyd;
struct Fixture {
    static constexpr uint32_t B=0x10000,A=B+0x2858,P=0x40000,S=0x50000,C=0x51000;
    std::map<uint32_t,uint8_t> bytes;
    BattleRandom rng{1};std::vector<int32_t> bounds;
    std::vector<unsigned> order;bool available=true;bool mutation=false;
    uint32_t word(uint32_t a){uint32_t v=0;for(unsigned i=0;i<4;++i)v=v<<8|bytes[a+i];return v;}
    void word(uint32_t a,uint32_t v){for(unsigned i=0;i<4;++i)bytes[a+i]=v>>(24-8*i);}
    uint16_t half(uint32_t a){return uint16_t(bytes[a])<<8|bytes[a+1];}
    void half(uint32_t a,uint16_t v){bytes[a]=v>>8;bytes[a+1]=v;}
    void real(uint32_t a,float f){uint32_t v;std::memcpy(&v,&f,4);word(a,v);}
    float real(uint32_t a){const auto v=word(a);float f;std::memcpy(&f,&v,4);return f;}
    Fixture() {
        word(0x8041eb90,B);word(0x8041eb00,P);word(B+0x2738,S-0x100);word(S-0x100+0xc,S);half(P+0x8a,1);
        EventScriptAssets asset;assert(asset.load("romfs/trio-attack-events.pmevt"));
        for(const auto span:{std::pair<uint32_t,unsigned>{0x802f9eac,65},{0x802f9fb0,11},{0x803b4de4,96},{0x80424988,22}})
            for(unsigned i=0;i<span.second;++i){uint32_t v;assert(asset.readWord(span.first+4*i,v));word(span.first+4*i,v);}
        for(unsigned i=0;i<16;++i){bytes[C+0x20+2*i]=bytes[C+0x21+2*i]=i==10?5:i>=13?1:0;}
    }
    BattleAudienceInitServices services() {
        BattleAudienceInitServices s;s.user=this;s.actors.user=this;s.actors.memory.user=this;
        s.actors.byte=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->bytes[a];};
        s.actors.memory.word=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->word(a);};
        s.actors.memory.half=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->half(a);};
        s.actors.memory.storeWord=[](void* p,uint32_t a,uint32_t v){static_cast<Fixture*>(p)->word(a,v);};
        s.actors.memory.storeByte=[](void* p,uint32_t a,uint8_t v){static_cast<Fixture*>(p)->bytes[a]=v;};
        s.random=[](void* p,int32_t bound){auto& f=*static_cast<Fixture*>(p);f.bounds.push_back(bound);return f.rng.bounded(bound);};
        s.fma=[](void*,float a,float b,float c){return std::fma(a,b,c);};
        s.setting=[](void* p){auto& f=*static_cast<Fixture*>(p);f.order.push_back(1);if(!f.available)return false;return initializePackedSetup4ZeroAudience(f.services());};
        s.texture=[](void* p,const char* path)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(p);const bool normal=std::strcmp(path,"battle/audience/audience_normal.tpl")==0;f.order.push_back(normal?2:3);return normal?1u:2u;};
        s.noise=[](void* p){auto& f=*static_cast<Fixture*>(p);f.order.push_back(4);BattleAudienceSoundServices sound;sound.actors=f.services().actors;sound.user=p;
            sound.play=[](void*,uint32_t,bool assigned,int32_t right,int32_t left)->std::optional<int32_t>{assert(!assigned && !right && !left);return -1;};
            return noiseAlwaysBattleAudienceSound(sound);};
        s.target=[](void* p){auto& f=*static_cast<Fixture*>(p);f.order.push_back(5);return updatePackedBattleAudienceZeroTarget(f.services().actors);};return s;
    }
};
int main() {
    Fixture f;auto s=f.services();assert(initializePackedSetup4AudienceParameters(Fixture::C,Fixture::S,s));
    assert(f.bounds.size()==16);for(auto b:f.bounds)assert(b==1);
    const auto inherited=f.rng.state;f.bytes[Fixture::A-1]=77;f.bytes[Fixture::A+0x13914]=88;
    for(unsigned i=0;i<0x13914;++i)f.bytes[Fixture::A+i]=255;
    assert(initializePackedBattleAudience(s));
    assert((f.order==std::vector<unsigned>{1,2,3,4,5}));
    assert(f.bytes[Fixture::A-1]==77 && f.bytes[Fixture::A+0x13914]==88);
    assert(f.bounds.size()==424);
    assert(f.word(Fixture::A+0xc)==1 && f.word(Fixture::A+0x10)==2 && f.bytes[Fixture::A+0x18]==12);
    assert(f.word(Fixture::A+0x13790)==0 && f.real(Fixture::A+0x13778)==8);
    assert(f.word(Fixture::A+0x13784)==0 && f.real(Fixture::A+0x1375c+0x18)==0);
    for(unsigned i=0;i<200*0x134;++i)assert(f.bytes[Fixture::A+0x1bc+i]==0);
    for(unsigned i=0;i<12;++i)assert(i==10?f.word(Fixture::A+0x13794+4*i)>=5 && f.word(Fixture::A+0x13794+4*i)<=50:f.word(Fixture::A+0x13794+4*i)==0);
    const auto sound=Fixture::A+0x133fc+2*0x24;
    assert(f.word(sound)==0 && f.word(sound+4)==0xffffffff && f.word(sound+0x18)==180 && f.bytes[sound+0x20]==60);
    BattleRandom expected(inherited);for(unsigned i=0;i<108;++i)expected.next();assert(expected.state==f.rng.state);
    unsigned zero=0,ties=0,permutation=0;for(auto b:f.bounds){zero+=b==0;ties+=b==2;permutation+=b==12;}
    assert(zero==300 && ties==55 && permutation==48);
    assert(f.word(Fixture::A+0x137f4)<=3 && f.word(Fixture::A+0x137f0)==0 && f.word(Fixture::A+0x137f8)==0);
    // All cardinality selections/permutations still admit the sole nonzero type.
    for(uint32_t seed=1;seed<=80;++seed) {
        Fixture varied;varied.rng.state=seed;s=varied.services();assert(initializePackedSetup4AudienceParameters(Fixture::C,Fixture::S,s));
        assert(initializePackedBattleAudience(s));assert(varied.word(Fixture::A+0x13794+40)>0 && varied.bounds.size()==424);
    }
    Fixture failed;s=failed.services();assert(initializePackedSetup4AudienceParameters(Fixture::C,Fixture::S,s));failed.available=false;
    assert(!initializePackedBattleAudience(s) && (failed.order==std::vector<unsigned>{1}));
    Fixture positive;s=positive.services();assert(initializePackedSetup4AudienceParameters(Fixture::C,Fixture::S,s));positive.half(Fixture::P+0x8c,100);
    assert(!initializePackedBattleAudience(s) && positive.bounds.size()==16); // No fabricated population.
    BattleAudienceResources resources;assert(!resources.handle("battle/audience/audience_normal.tpl"));assert(resources.load("romfs"));
    assert(resources.payload(1)->size()==428896 && resources.payload(2)->size()==39776 && !resources.payload(3));
    assert(resources.handle("battle/audience/audience_normal.tpl")==1 && resources.handle("battle/audience/audience_luigi.tpl")==2);
    assert(!resources.handle("audience_normal.tpl") && !resources.load("/nonexistent-audience-bank"));
    Game game;const auto owner=game.trioRandom;owner->bounded(100);const auto state=owner->state;const auto saved=game.progress;game.restore(saved);
    assert(game.trioRandom==owner && game.trioRandom->state==state);Game separate;assert(separate.trioRandom!=owner && separate.trioRandom->state==1);
    Fixture target;auto actors=target.services().actors;
    target.real(Fixture::A+0x13778,8);target.real(Fixture::A+0x1377c,8);
    assert(packedBattleAudienceAddTargetNumSub(.5f,actors) && target.real(Fixture::A+0x1377c)==8.5f);
    target.real(Fixture::A+0x1377c,16);
    assert(packedBattleAudienceAddTargetNumSub(1,actors) && std::fabs(target.real(Fixture::A+0x1377c)-16.2f)<1e-5f);
    target.real(Fixture::A+0x1377c,4);
    assert(packedBattleAudienceAddTargetNumSub(-.75f,actors) && target.real(Fixture::A+0x1377c)==3.75f);
    target.real(Fixture::A+0x1377c,8);target.real(Fixture::A+0x13780,0);target.word(Fixture::A+0x13790,0);
    assert(packedBattleAudienceAddTargetNum(2.5f,0,actors) && target.real(Fixture::A+0x1377c)==0);
    target.word(Fixture::A+0x13790,50);target.real(Fixture::A+0x1377c,8);target.real(Fixture::A+0x13780,2);
    assert(packedBattleAudienceAddTargetNum(-1,0,actors) && target.real(Fixture::A+0x1377c)==8 && target.real(Fixture::A+0x13780)==1);
    target.real(Fixture::A+0x13778,0);target.real(Fixture::A+0x1377c,8);
    assert(!packedBattleAudienceAddTargetNumSub(1,actors)); // Explicit unresolved conversion domain.
}
