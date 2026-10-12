#include "battle_packed_move.hpp"
#include <array>
#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <vector>
using namespace ttyd;
struct Fixture {
    std::array<uint8_t,0x131> bytes{};std::vector<uint32_t> reads;std::vector<std::pair<uint32_t,uint8_t>> writes;
    std::function<void(uint32_t)> onRead;std::function<void(uint32_t)> onStore;
    BattleActorServices services() {
        BattleActorServices s;s.user=this;s.memory.user=this;
        s.byte=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);if(f.onRead)f.onRead(a);return f.bytes.at(a-0x1000);};
        s.memory.storeByte=[](void* p,uint32_t a,uint8_t v) {auto& f=*static_cast<Fixture*>(p);f.writes.push_back({a,v});f.bytes.at(a-0x1000)=v;if(f.onStore)f.onStore(a);};return s;
    }
};
int main() {
    struct Case {uint8_t base,fast,slow,counter,expected,marker;};
    const Case cases[]{{1,0,0,255,1,0},{0,0,0,0,0,10},{128,0,0,0,128,0},{255,0,0,0,255,0},{3,1,1,0,3,0},{3,0,1,0,1,0},{1,0,1,1,0,10},{1,0,1,2,1,0},{0,0,1,3,0,10},{0,0,1,4,1,0},{1,0,1,128,0,10},{64,1,0,0,128,0},{64,1,1,2,1,0},{64,255,0,0,128,0},{255,1,1,4,1,0},{200,0,0,0,200,0}};
    for(auto c:cases) {Fixture f;f.bytes[0x20]=255;f.bytes[0x21]=c.base;f.bytes[0x24]=c.counter;f.bytes[0x12f]=c.fast;f.bytes[0x130]=c.slow;
        resetPackedBattleMoveStatus(0x1000,f.services());assert(f.bytes[0x20]==c.marker && f.bytes[0x21]==c.base && f.bytes[0x22]==c.expected && f.bytes[0x23]==c.expected && f.bytes[0x24]==c.counter && f.bytes[0x12f]==c.fast && f.bytes[0x130]==c.slow);
        assert(f.writes[0]==std::make_pair(0x1020u,uint8_t(0)) && f.writes[1]==std::make_pair(0x1022u,c.base));
        auto fast=std::find(f.reads.begin(),f.reads.end(),0x112f),slow=std::find(f.reads.begin(),f.reads.end(),0x1130);assert(fast<slow && slow!=f.reads.end());
    }
    Fixture alias;alias.bytes[0x21]=2;alias.bytes[0x12f]=1;alias.onRead=[&](uint32_t a) {if(a==0x112f)alias.bytes[0x22]=7;};resetPackedBattleMoveStatus(0x1000,alias.services());assert(alias.bytes[0x22]==14 && alias.bytes[0x23]==14);
    Fixture finalRead;finalRead.bytes[0x21]=2;finalRead.onStore=[&](uint32_t a) {if(a==0x1023)finalRead.bytes[0x23]=0;};resetPackedBattleMoveStatus(0x1000,finalRead.services());assert(finalRead.bytes[0x22]==0 && finalRead.bytes[0x20]==10); // Copied23 is reread before final fallback.
    std::cout<<"packed native move-reset tests passed\n";
}
