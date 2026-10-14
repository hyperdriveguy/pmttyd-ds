#include "battle_audience_window.hpp"
#include "battle_audience_sound_main.hpp"
#include "positional_sfx_registry.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <map>
#include <vector>
using namespace ttyd;
struct Fixture {
    static constexpr uint32_t B=0x10000,A=B+0x2858,V=A+0x1375c,R=A+0x133fc;
    std::map<uint32_t,uint8_t> bytes;
    PositionalSfxRegistry registry;
    std::vector<int32_t> checks;
    std::vector<uint8_t> volumes;
    unsigned noise=0,stops=0;bool mutateCount=false,available=true;
    uint32_t word(uint32_t a){uint32_t v=0;for(unsigned i=0;i<4;++i)v=v<<8|bytes[a+i];return v;}
    void word(uint32_t a,uint32_t v){for(unsigned i=0;i<4;++i)bytes[a+i]=uint8_t(v>>(24-8*i));
        if(mutateCount && a==V+0x18){mutateCount=false;word(A+0x13784,0);}}
    void real(uint32_t a,float f){uint32_t v;std::memcpy(&v,&f,4);word(a,v);}
    float real(uint32_t a){const auto v=word(a);float f;std::memcpy(&f,&v,4);return f;}
    Fixture() {
        word(0x8041eb90,B);real(0x80424988,0);real(0x804249e8,.1f);real(0x80424a2c,-.1f);
        real(0x80424a30,-500);real(0x80424a34,-280);real(0x80424a38,-160);
    }
    BattleActorServices actors() {
        BattleActorServices s;s.user=this;s.memory.user=this;
        s.byte=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->bytes[a];};
        s.memory.word=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->word(a);};
        s.memory.storeWord=[](void* p,uint32_t a,uint32_t v){static_cast<Fixture*>(p)->word(a,v);};
        s.memory.storeByte=[](void* p,uint32_t a,uint8_t v){static_cast<Fixture*>(p)->bytes[a]=v;};return s;
    }
    BattleAudienceSoundMainServices sound() {
        BattleAudienceSoundMainServices s;s.actors=actors();s.user=this;
        s.noise=[](void* p){auto& f=*static_cast<Fixture*>(p);++f.noise;return f.available;};
        s.check=[](void* p,int32_t h){auto& f=*static_cast<Fixture*>(p);f.checks.push_back(h);return f.registry.check(h);};
        s.stop=[](void* p,int32_t h){auto& f=*static_cast<Fixture*>(p);++f.stops;f.word(R,f.word(R)|8);return f.registry.off(h,{});};
        s.setVolume=[](void* p,int32_t h,uint8_t v){auto& f=*static_cast<Fixture*>(p);f.volumes.push_back(v);return f.registry.setVolume(h,v);};
        s.truncate=[](void*,float v)->std::optional<int32_t>{if(!std::isfinite(v) || v>=2147483648.f || v<-2147483648.f)return {};return int32_t(v);};return s;
    }
};
int main() {
    using F=Fixture;
    Fixture window;auto a=window.actors();packedBattleAudienceWindowMain(a);
    assert(window.real(F::V+8)==-500 && window.real(F::V+0xc)==-160 && window.word(F::V+0x10)==0);
    window.word(F::A+0x13784,10);window.bytes[F::V]=1;packedBattleAudienceWindowMain(a);
    assert(window.real(F::V+0x18)==1 && window.word(F::V+0x14)==1);
    assert(window.word(F::V+8)==0xc3ebd27d);
    window.real(F::V+0x18,9.95f);window.word(F::V+0x14,14);packedBattleAudienceWindowMain(a);
    assert(window.real(F::V+0x18)==10 && window.word(F::V+0x14)==15 && window.real(F::V+8)==-280);
    packedBattleAudienceWindowMain(a);assert(window.word(F::V+0x14)==15);
    window.real(F::V+0x18,10.05f);window.bytes[F::V]=0;packedBattleAudienceWindowMain(a);assert(window.real(F::V+0x18)==10);
    window.word(F::V+0x14,31);packedBattleAudienceWindowMain(a);
    assert(window.word(F::V+0x14)==30 && window.real(F::V+8)==-500); // No inactive upper clamp.
    window.bytes[F::V]=1;window.word(F::V+0x14,uint32_t(-2));packedBattleAudienceWindowMain(a);
    assert(window.word(F::V+0x14)==uint32_t(-1) && window.real(F::V+8)<-500); // No active lower clamp.
    window.word(F::V+0x14,0x7fffffff);packedBattleAudienceWindowMain(a);assert(window.word(F::V+0x14)==0x80000000);
    window.bytes[F::V]=0;window.word(F::V+0x14,0x80000000);packedBattleAudienceWindowMain(a);assert(window.word(F::V+0x14)==0x7fffffff);
    window.real(F::V+0x18,0);window.mutateCount=true;packedBattleAudienceWindowMain(a);
    assert(window.real(F::V+0x18)==0); // Clamp reloads count after the amount store.
    Fixture free;for(unsigned i=0;i<24;++i){free.word(F::R+i*0x24,0x81);free.word(F::R+i*0x24+0x18,180);}
    auto sound=free.sound();assert(packedBattleAudienceSoundMain(sound));
    assert(free.noise==1 && free.checks.size()==24 && free.stops==0 && free.volumes.empty());
    for(unsigned i=0;i<24;++i){assert(free.word(F::R+i*0x24)==0x80 && free.word(F::R+i*0x24+0x18)==180);}
    Fixture shared;shared.registry.slots[0].key=55;shared.registry.slots[0].flags=1;
    shared.word(F::R,1);shared.word(F::R+0x24+0xc,5);shared.word(F::R+0x24+0x18,180);
    sound=shared.sound();assert(packedBattleAudienceSoundMain(sound));
    assert(shared.stops==1 && shared.word(F::R)==8 && shared.word(F::R+0x24+0xc)==5 && shared.volumes.empty());
    Fixture fading;for(unsigned i=1;i<24;++i)fading.word(F::R+0x24*i+4,0xffffffff);
    fading.registry.slots[0].key=77;fading.registry.slots[0].flags=1;
    fading.word(F::R+0xc,2);fading.word(F::R+0x10,2);fading.word(F::R+0x18,4);
    fading.bytes[F::R+0x1d]=100;fading.bytes[F::R+0x20]=100;
    sound=fading.sound();assert(packedBattleAudienceSoundMain(sound));
    assert((fading.volumes==std::vector<uint8_t>{25,25}) && fading.word(F::R+0xc)==1);
    assert(packedBattleAudienceSoundMain(sound));assert((fading.volumes==std::vector<uint8_t>{25,25,12,12}));
    assert(fading.word(F::R+0xc)==0 && fading.word(F::R+0x14)==2);
    assert(packedBattleAudienceSoundMain(sound));assert(fading.stops==1 && fading.word(F::R+0x14)==2);
    Fixture endless;for(unsigned i=1;i<24;++i)endless.word(F::R+0x24*i+4,0xffffffff);
    endless.registry.slots[0].key=1;endless.word(F::R+0xc,0xffffffff);endless.word(F::R+0x18,1);
    endless.bytes[F::R+0x1d]=255;endless.bytes[F::R+0x20]=100;sound=endless.sound();
    assert(packedBattleAudienceSoundMain(sound) && endless.word(F::R+0xc)==0xffffffff);
    assert(endless.word(F::R+0x14)==1 && endless.word(F::R+0x18)==0 && endless.volumes.back()==255);
    assert(packedBattleAudienceSoundMain(sound) && endless.volumes.size()==1); // Negative lifetime never decrements.
    Fixture owner;const auto otherR=F::R+0x90000;owner.word(otherR,0x85);sound=owner.sound();
    sound.noise=[](void* p){auto& f=*static_cast<Fixture*>(p);++f.noise;f.word(0x8041eb90,F::B+0x90000);return true;};
    assert(packedBattleAudienceSoundMain(sound) && owner.word(otherR)==0x84 && owner.word(F::R)==0);
    Fixture failure;failure.available=false;sound=failure.sound();assert(!packedBattleAudienceSoundMain(sound) && failure.checks.empty());
    failure.available=true;failure.word(F::R+4,40);assert(!packedBattleAudienceSoundMain(sound));
}
