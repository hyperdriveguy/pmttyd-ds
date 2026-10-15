#include "battle_stock_exp_draw.hpp"
#include <cassert>
#include <cstring>
#include <limits>
#include <map>
#include <vector>
using namespace ttyd;
namespace {uint32_t bits(float v){uint32_t n;std::memcpy(&n,&v,4);return n;}}
struct Fixture {
    std::map<uint32_t,uint32_t> words{{0x8041eb90,0x1000},{0x1f04,23}};
    std::vector<uint32_t> reads;std::vector<SourceIconPositionRequest> icons;bool hold=false,mutate=false;
    BattleStockExpDrawServices services(){BattleStockExpDrawServices s;s.user=this;
        s.word=[](void* u,uint32_t a)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(u);f.reads.push_back(a);return f.words[a];};
        s.icon=[](void* u,const SourceIconPositionRequest& r){auto& f=*static_cast<Fixture*>(u);assert(r.icon==404 && r.flags==16 && r.alpha==255 && r.position[2]==-100);
            if(f.hold){assert(r.position[0]==304.5f && r.position[1]==-220);return false;}f.icons.push_back(r);
            if(f.mutate){f.words[0x8041eb90]=0x2000;f.words[0x1f04]=100;f.words[0x8041eb98]=bits(10);f.words[0x8041eb9c]=bits(20);}return true;};return s;}
};
int main(){
    Fixture f;BattleStockExpDrawCursor c;f.words[0x1f04]=0;
    assert(drawBattleStockExp(f.services(),c) && f.reads==std::vector<uint32_t>({0x8041eb90,0x1f04}) && f.icons.empty());
    f.words[0x1f04]=0xffffffff;assert(drawBattleStockExp(f.services(),c) && f.icons.empty());
    f.words[0x1f04]=23;f.hold=true;f.mutate=true;
    assert(!drawBattleStockExp(f.services(),c) && c.pending==0x8001a918 && c.ones==3 && c.tens==2 && c.index==0);
    f.words[0x8041eb98]=bits(99);assert(!drawBattleStockExp(f.services(),c) && c.icon.position[0]==304.5f);
    f.hold=false;assert(drawBattleStockExp(f.services(),c) && f.icons.size()==5 && c.battle==0x1000 && c.count==23);
    assert(f.icons[0].scale==.35f && f.icons[0].position[0]==304.5f);
    assert(f.icons[1].position[0]==301.5f && f.icons[2].position[0]==288.5f);
    assert((f.icons[3].position==std::array<float,3>{312,-184,-100}) && f.icons[3].scale==.5f);
    assert(f.icons[4].position[0]==292 && f.icons[4].scale==.5f);
    Fixture cap;cap.words[0x1f04]=0x7fffffff;assert(drawBattleStockExp(cap.services(),c) && cap.icons.size()==10 && cap.words[0x1f04]==0x7fffffff);
    for(const auto& i:cap.icons)assert(i.scale==.5f);
    Fixture zero;zero.words[0x1f04]=1;zero.words[0x8041eb98]=bits(-304.5f);
    assert(drawBattleStockExp(zero.services(),c) && bits(zero.icons[0].position[0])==0x80000000); // Original fnmsubs negate retains -0.
    auto missing=zero.services();missing.icon=nullptr;assert(!drawBattleStockExp(missing,c) && c.pending==0x8001a918);
}
