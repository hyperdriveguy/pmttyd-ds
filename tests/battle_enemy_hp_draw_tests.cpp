#include "battle_enemy_hp_draw.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
uint32_t bits(float f){uint32_t v;std::memcpy(&v,&f,4);return v;}
struct Fixture {
    std::map<uint32_t,uint32_t> words;std::map<uint32_t,uint16_t> halves;
    std::vector<uint32_t> reads,units,colors;std::vector<int32_t> switches,numbers;
    unsigned cameras=0,screens=0,numberCalls=0;bool switchHold=false,numberHold=false,mutate=false;
    static constexpr uint32_t B=0x100000,G=0x200000,U=0x300000,V=0x310000,W=0x320000;
    BattleMonosiriServices mono(){BattleMonosiriServices s;s.user=this;
        s.word=[](void* p,uint32_t a)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);return f.words[a];};
        s.switchFlag=[](void* p,int32_t k)->std::optional<int32_t>{auto& f=*static_cast<Fixture*>(p);f.switches.push_back(k);if(f.switchHold)return {};return 0;};return s;}
    BattleEnemyHpDrawServices draw(){BattleEnemyHpDrawServices s;s.user=this;
        s.word=mono().word;
        s.half=[](void* p,uint32_t a)->std::optional<uint16_t>{auto& f=*static_cast<Fixture*>(p);auto v=f.halves[a];if(f.mutate && a==V+0x10c)f.words[B+0x20]=W;return v;};
        s.camera=[](void* p,uint32_t i){assert(i==8);++static_cast<Fixture*>(p)->cameras;return true;};
        s.status=[](void* p,uint32_t u,int32_t i)->std::optional<int32_t>{auto& f=*static_cast<Fixture*>(p);assert(i==27);f.units.push_back(u);if(f.mutate && u==U)f.words[B+0x20]=V;return u==W?1:0;};
        s.monosiri=[](void* p,uint32_t u)->std::optional<int32_t>{auto& f=*static_cast<Fixture*>(p);f.units.push_back(u);return 1;};
        s.screenPoint=[](void* p,const std::array<float,3>& in,std::array<float,3>& out){auto& f=*static_cast<Fixture*>(p);++f.screens;out=in;return true;};
        s.interpolate=[](void*,int32_t mode,int32_t elapsed,int32_t duration,float from,float to)->std::optional<float>{assert(mode==0 && elapsed==5 && duration==20 && from==0 && to==255);return 63.75f;};
        s.truncate=[](void*,float value)->std::optional<int32_t>{return int32_t(std::trunc(value));};
        s.number=[](void* p,const CameraViewMatrix& m,int32_t n,uint32_t mode,uint32_t rgba){auto& f=*static_cast<Fixture*>(p);++f.numberCalls;assert(mode==1 && rgba==0xffffffff && m[2][3]==0);if(f.mutate){assert(m[0][3]==19 && m[1][3]==-19 && n==-1);}if(f.numberHold)return false;f.numbers.push_back(n);return true;};
        s.bar=[](void* p,const std::array<float,3>& point,uint32_t u,uint32_t color){auto& f=*static_cast<Fixture*>(p);if(f.mutate){assert(u==U && point[0]==7 && point[1]==13);}f.colors.push_back(color);return true;};return s;
    }
};
}
int main(){
    CameraDepthSnapshot camera{};for(unsigned i=0;i<3;++i)camera.view[i][i]=1;for(unsigned i=0;i<4;++i)camera.projection[i][i]=1;
    const auto point=battleEnemyHpScreenPoint(camera,{2,-3,4});assert((point==CameraDepthPoint{608,-720,0}));
    camera.projection[3][3]=0;const auto infinite=battleEnemyHpScreenPoint(camera,{2,-3,4});assert(std::isinf(infinite[0]) && std::isinf(infinite[1]) && infinite[2]==0);
    // Global B and kind remain captured across a real switch-provider wait.
    Fixture f;f.words[0x8041eb90]=Fixture::G;f.words[Fixture::U+8]=33;f.words[Fixture::U+4]=65;f.words[Fixture::U+0x1c]=4;
    f.words[Fixture::G+0x163d4+8]=2;f.switchHold=true;BattleMonosiriCursor m;
    assert(!checkBattleMonosiri(Fixture::U,f.mono(),m) && m.pending==0x800858b4);
    f.words[0x8041eb90]=Fixture::B;f.words[Fixture::U+8]=0;f.switchHold=false;
    assert(checkBattleMonosiri(Fixture::V,f.mono(),m)==1 && f.switches==std::vector<int32_t>({4507,4507}));
    assert(m.battle==Fixture::G && m.unit==Fixture::U && m.pending==0);
    // Policy bypass performs no switch/unit-flag/bank read. Negative remainder uses PPC slw, not host UB.
    f.reads.clear();f.switches.clear();f.words[Fixture::B+0x163f4]=2;
    assert(checkBattleMonosiri(Fixture::U,f.mono(),m)==1 && f.switches.empty() && f.reads.size()==3);
    f.words[Fixture::B+0x163f4]=0;f.words[Fixture::U+8]=0xffffffff;f.words[Fixture::U+0x1c]=0;f.words[Fixture::B+0x163d4]=0xffffffff;
    assert(checkBattleMonosiri(Fixture::U,f.mono(),m)==0 && f.switches.back()==4473);
    // Genuine empty walk still requests camera8 and reads all64 passed-B slots.
    Fixture empty;BattleEnemyHpDrawCursor c;assert(drawBattleEnemyHp(Fixture::B,empty.draw(),c));assert(empty.cameras==1 && empty.reads.size()==64 && c.slot==64);
    // All status-gated units avoid Tattle and drawing without inventing an ending gate.
    empty.words[Fixture::B+0x20]=Fixture::W;empty.units.clear();assert(drawBattleEnemyHp(Fixture::B,empty.draw(),c));assert(empty.units==std::vector<uint32_t>{Fixture::W});
    // Registry mutation is visible to checker/number but retained first U feeds flags/world/bar.
    Fixture a;a.mutate=true;a.numberHold=true;a.words[Fixture::B+0x20]=Fixture::U;a.words[Fixture::U+0x1c]=1;
    a.words[Fixture::U+0x3c]=bits(5);a.words[Fixture::U+0x40]=bits(10);a.words[Fixture::U+0x44]=bits(9);
    a.halves[Fixture::U+0xd4]=2;a.halves[Fixture::U+0xd6]=3;a.halves[Fixture::U+0x1fe]=1;a.halves[Fixture::U+0x204]=5;
    a.halves[Fixture::V+0x10c]=100;a.halves[Fixture::W+0x10c]=0xffff;
    assert(!drawBattleEnemyHp(Fixture::B,a.draw(),c) && c.pending==0x80019120 && a.screens==1 && a.colors.empty());
    a.words[Fixture::B+0x20]=0;a.words[Fixture::U+0x3c]=bits(999);a.numberHold=false;
    assert(drawBattleEnemyHp(0,a.draw(),c) && a.screens==1 && a.numberCalls==2 && a.colors==std::vector<uint32_t>{0xffffff3f});
    assert(a.units==std::vector<uint32_t>({Fixture::U,Fixture::V}) && c.battle==Fixture::B);
    // Fade color carries to the next admitted unit; number colors always reset to white.
    Fixture carry;carry.words[Fixture::B+0x20]=Fixture::U;carry.words[Fixture::B+0x24]=Fixture::V;
    carry.words[Fixture::B+0x1720]=0x01000000;carry.words[Fixture::U+0x1c]=1;
    carry.halves[Fixture::U+0x1fe]=1;carry.halves[Fixture::U+0x204]=5;
    assert(drawBattleEnemyHp(Fixture::B,carry.draw(),c) && carry.colors==std::vector<uint32_t>({0xffffff3f,0xffffff3f}));
    // Active source path holds at its actual missing child; it cannot be treated as empty.
    auto services=carry.draw();services.screenPoint=nullptr;assert(!drawBattleEnemyHp(Fixture::B,services,c) && c.pending==0x801009b4);
}
