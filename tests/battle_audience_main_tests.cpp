#include "battle_audience_main.hpp"
#include "battle_audience_sound.hpp"
#include "game.hpp"
#include <cassert>
#include <map>
#include <vector>
#include <cstring>
using namespace ttyd;
struct Fixture {
    static constexpr uint32_t B=0x10000,A=B+0x2858,R=A+0x133fc+2*0x24;
    std::map<uint32_t,uint8_t> bytes;
    std::vector<uint32_t> requests;
    std::vector<int32_t> bounds,seats;
    int32_t draw=1;bool ready=true,swapOwner=false,success=false;
    uint32_t word(uint32_t a){uint32_t v=0;for(unsigned i=0;i<4;++i)v=v<<8|bytes[a+i];return v;}
    void word(uint32_t a,uint32_t v){for(unsigned i=0;i<4;++i)bytes[a+i]=uint8_t(v>>(24-8*i));}
    void real(uint32_t a,float f){uint32_t v;std::memcpy(&v,&f,4);word(a,v);}
    Fixture(){word(0x8041eb90,B);word(0x803b4dec,0x803b4c04);word(0x803b4c04,121);real(0x804249b4,63);real(0x804249b8,200);}
    uint32_t actor(unsigned i){return A+0x1bc+0x134*i;}
    BattleActorServices actors() {
        BattleActorServices s;s.user=this;s.memory.user=this;
        s.byte=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->bytes[a];};
        s.memory.word=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->word(a);};
        s.memory.storeWord=[](void* p,uint32_t a,uint32_t v){static_cast<Fixture*>(p)->word(a,v);};
        s.memory.storeByte=[](void* p,uint32_t a,uint8_t v){static_cast<Fixture*>(p)->bytes[a]=v;};return s;
    }
    BattleAudienceMainServices main() {
        return {actors(),this,[](void* p,uint32_t fn){auto& f=*static_cast<Fixture*>(p);f.requests.push_back(fn);
            if(fn==0x801a1cdc){f.bytes[f.word(0x8041eb90)+0x2858+0x1375c]=1;return true;}
            if(f.swapOwner){f.word(0x8041eb90,B+0x90000);f.word(A,f.word(A)|0x4000);f.word(B+0xef4,0);}
            return f.ready;}};
    }
    BattleAudienceControlServices control() {
        BattleAudienceControlServices s;s.actors=actors();s.user=this;
        s.actor=[](void* p,uint32_t,int32_t i,bool motion){auto& f=*static_cast<Fixture*>(p);f.seats.push_back(motion?-i-1:i);return f.ready;};
        s.random=[](void* p,int32_t bound){auto& f=*static_cast<Fixture*>(p);f.bounds.push_back(bound);return bound==200?0:f.draw;};
        s.itemOn=[](void* p,int32_t seat)->std::optional<int32_t>{auto& f=*static_cast<Fixture*>(p);f.seats.push_back(seat);
            if(f.success && seat==17){f.word(A+0x137f0,10);return 1;}return packedBattleAudienceEmptyItemOn(seat,f.actors());};return s;
    }
};
int main() {
    using F=Fixture;
    constexpr uint32_t calls[]{0x801a5ca8,0x801a4f0c,0x801a4998,0x801a4570,0x8019f97c};
    Fixture outer;outer.word(F::B+0xef4,0x80);outer.word(F::A,0x40);auto s=outer.main();assert(packedBattleAudienceMain(s));
    assert(outer.requests.size()==6 && outer.requests[0]==0x801a1cdc && outer.bytes[F::A+0x1375c]==1);
    for(unsigned i=0;i<5;++i)assert(outer.requests[i+1]==calls[i]);assert(outer.word(F::A)==0x10040);
    outer.requests.clear();outer.swapOwner=true;assert(packedBattleAudienceMain(s));
    assert(outer.requests.size()==5 && outer.word(F::A)==0x4040 && outer.word(0x8041eb90)==F::B+0x90000);
    Fixture stopped;stopped.ready=false;s=stopped.main();assert(!packedBattleAudienceMain(s) && stopped.requests.size()==1 && stopped.word(F::A)==0);
    Fixture count;count.word(count.actor(0),1);count.word(count.actor(10),1);count.bytes[count.actor(10)+0x19]=0xb;
    count.word(count.actor(11),1);count.bytes[count.actor(11)+0x19]=0xd;count.word(count.actor(12),0x103);
    auto c=count.control();assert(packedBattleAudienceControl(c));assert(count.word(F::A+0x13788)==1 && count.word(F::A+0x1378c)==1 && count.word(F::A+0x13784)==2);
    assert(count.bounds.empty());count.ready=false;assert(!packedBattleAudienceControl(c) && count.seats.back()==0);
    Fixture motion;motion.word(motion.actor(0),0x10);motion.real(motion.actor(0)+0xd0,1);motion.real(motion.actor(0)+0xdc,-2);motion.real(motion.actor(0)+0xe4,-1);
    auto mc=motion.control();assert(packedBattleAudienceControl(mc) && motion.seats.empty() && motion.word(motion.actor(0))==0x10);
    for(const auto offset:{0xd0u,0xdcu,0xe4u})assert(motion.word(motion.actor(0)+offset)==0);
    Fixture throwing;throwing.word(F::B+0xef4,0x80);throwing.word(F::A+0x137f4,1);throwing.draw=0;c=throwing.control();
    assert(packedBattleAudienceControl(c) && throwing.bounds.size()==2001 && throwing.bounds[0]==1800 && throwing.seats.size()==200);
    for(unsigned i=0;i<200;++i)assert(throwing.seats[i]==int32_t(i));assert(throwing.word(F::A+0x137f0)==0);
    throwing.success=true;throwing.bounds.clear();throwing.seats.clear();assert(packedBattleAudienceControl(c) && throwing.word(F::A+0x137f0)==11 && throwing.seats.size()==18);
    Fixture override;override.word(F::B+0x19078,0x20000);c=override.control();assert(packedBattleAudienceControl(c) && override.word(F::A+0x137f4)==10000 && override.bounds.empty());
    override.word(F::B+0xef4,0x80);assert(packedBattleAudienceControl(c) && override.bounds.back()==60);
    Fixture unsupported;unsupported.word(F::B+0xef4,0x80);unsupported.word(F::A+0x137f0,3);unsupported.word(F::A+0x137f4,4);
    c=unsupported.control();assert(!packedBattleAudienceControl(c) && unsupported.bounds.empty());
    Fixture leaf;auto a=leaf.actors();assert(packedBattleAudienceInactiveItems(a) && packedBattleAudienceInactiveApSources(a));
    leaf.word(F::A+0xf25c+99*0x48,1);assert(!packedBattleAudienceInactiveItems(a));leaf.word(F::A,2);assert(!packedBattleAudienceInactiveApSources(a));
    leaf.word(F::A,0);leaf.word(leaf.actor(4),1);assert(!packedBattleAudienceEmptyItemOn(4,a));assert(packedBattleAudienceEmptyItemOn(3,a)==0);
    leaf.word(F::A,0x40000);assert(packedBattleAudienceEmptyItemOn(4,a)==0);
    Fixture noise;BattleAudienceSoundServices ns;ns.actors=noise.actors();ns.user=&noise;
    ns.stop=[](void* p,int32_t){static_cast<Fixture*>(p)->requests.push_back(1);return true;};
    assert(noiseMainBattleAudienceSound(ns));noise.word(F::B+0xef4,0x80);noise.bytes[F::R+0x1e]=25;
    assert(noiseMainBattleAudienceSound(ns) && noise.word(F::R+0x18)==80 && noise.bytes[F::R+0x1f]==25 && noise.bytes[F::R+0x20]==60);
    noise.bytes[F::R+8]=2;assert(noiseMainBattleAudienceSound(ns) && noise.bytes[F::R+8]==0 && noise.requests.size()==1);
    noise.word(F::A,0x12000);noise.word(F::B+0xef4,0);noise.word(noise.actor(0),1);noise.word(F::R,1);
    assert(noiseMainBattleAudienceSound(ns) && noise.bytes[F::R+8]==1 && noise.bytes[F::R+0x1d]==63 && noise.bytes[F::R+0x20]==0);
    Game game(false);const auto registry=game.publicSfx;registry->slots[7].generation=9;registry->slots[7].key=12;
    const auto progress=game.progress;game.restore(progress);assert(game.publicSfx==registry && game.publicSfx->slots[7].generation==9 && game.publicSfx->slots[7].key==12);
}
