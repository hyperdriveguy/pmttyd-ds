#include "battle_hp_gauge.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <map>
#include <vector>
using namespace ttyd;
struct Fixture {
    static constexpr uint32_t U=0x10000;
    std::map<uint32_t,uint8_t> bytes;
    std::vector<int32_t> requests;bool available=true,mutate=false;
    uint32_t word(uint32_t a){uint32_t v=0;for(unsigned i=0;i<4;++i)v=v<<8|bytes[a+i];return v;}
    void word(uint32_t a,uint32_t v){for(unsigned i=0;i<4;++i)bytes[a+i]=uint8_t(v>>(24-8*i));}
    uint16_t half(uint32_t a){return uint16_t(bytes[a])<<8|bytes[a+1];}
    void half(uint32_t a,uint16_t v){bytes[a]=v>>8;bytes[a+1]=v;}
    void real(uint32_t a,float f){uint32_t v;std::memcpy(&v,&f,4);word(a,v);}
    float real(uint32_t a){const auto v=word(a);float f;std::memcpy(&f,&v,4);return f;}
    Fixture(){half(U+0x108,10);half(U+0x10c,8);real(0x804226a4,0);real(0x804226a8,1);}
    BattleHpGaugeServices services() {
        BattleHpGaugeServices s;s.user=this;s.actors.memory.user=this;
        s.actors.memory.word=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->word(a);};
        s.actors.memory.half=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->half(a);};
        s.actors.memory.storeWord=[](void* p,uint32_t a,uint32_t v){static_cast<Fixture*>(p)->word(a,v);};
        s.actors.memory.storeHalf=[](void* p,uint32_t a,uint16_t v){static_cast<Fixture*>(p)->half(a,v);};
        s.interpolate=[](void* p,int32_t mode,int32_t step,int32_t duration,float start,float end)->std::optional<float>{auto& f=*static_cast<Fixture*>(p);f.requests={mode,step,duration};
            if(f.mutate){f.half(U+0x108,20);f.half(U+0x10c,4);}
            if(!f.available)return {};return battleHpGaugeCurve(mode,step,duration,start,end);};return s;
    }
    void init(){initializePackedBattleHpGauge(U,services().actors);}
};
int main() {
    using F=Fixture;
    Fixture f;f.half(F::U+0x1fc,1);f.half(F::U+0x1fe,0xa5);f.init();
    assert(f.half(F::U+0x1fc)==0 && f.half(F::U+0x1fe)==0xa5 && f.half(F::U+0x202)==20 && f.half(F::U+0x206)==50);
    assert(f.word(F::U+0x208)==8 && f.word(F::U+0x20c)==8 && f.real(F::U+0x210)==.8f && f.real(F::U+0x214)==.8f);
    assert(battleHpGaugeCurve(0,10,20,0,1)==.5f && battleHpGaugeCurve(2,10,20,0,1)==.125f);
    assert(battleHpGaugeCurve(2,5,0,7,9)==9 && battleHpGaugeCurve(0,-10,20,0,1)==-.5f);
    auto s=f.services();f.word(F::U+0x208,10);f.half(F::U+0x200,11);f.half(F::U+0x204,1);f.word(F::U+0x1c,0x81);
    assert(packedBattleHpGaugeMain(F::U,s));assert((f.requests==std::vector<int32_t>{0,10,20}));
    assert(std::fabs(f.real(F::U+0x210)-.9f)<1e-6f && f.real(F::U+0x214)==0 && f.word(F::U+0x1c)==0x80 && f.half(F::U+0x1fe)==0xa4);
    f.half(F::U+0x200,1);f.word(F::U+0x208,99);assert(packedBattleHpGaugeMain(F::U,s));
    assert(f.word(F::U+0x208)==8 && f.real(F::U+0x210)==.8f && f.real(F::U+0x214)==.8f);
    Fixture cubic;cubic.init();cubic.half(F::U+0x1fc,1);cubic.word(F::U+0x208,10);cubic.half(F::U+0x200,11);s=cubic.services();
    assert(packedBattleHpGaugeMain(F::U,s) && std::fabs(cubic.real(F::U+0x214)-.825f)<1e-6f && cubic.requests[0]==2);
    // Default captures max before the interpolation callback; mode1 reloads.
    Fixture linear;linear.init();linear.word(F::U+0x208,10);linear.half(F::U+0x200,11);linear.mutate=true;s=linear.services();
    assert(packedBattleHpGaugeMain(F::U,s) && linear.real(F::U+0x210)==.5f);
    cubic.half(F::U+0x200,11);cubic.mutate=true;s=cubic.services();assert(packedBattleHpGaugeMain(F::U,s));
    assert(std::fabs(cubic.real(F::U+0x214)-.8125f)<1e-6f);
    Fixture failed;failed.init();failed.available=false;failed.half(F::U+0x200,11);failed.half(F::U+0x204,8);s=failed.services();
    assert(!packedBattleHpGaugeMain(F::U,s) && failed.half(F::U+0x200)==10 && failed.half(F::U+0x204)==8 && failed.real(F::U+0x214)==0);
    Fixture zero;zero.half(F::U+0x108,0);zero.half(F::U+0x10c,0);zero.init();assert(std::isnan(zero.real(F::U+0x210)));
    s=zero.services();zero.half(F::U+0x200,0xffff);assert(packedBattleHpGaugeMain(F::U,s) && zero.half(F::U+0x200)==0xffff && zero.requests.empty());
}
