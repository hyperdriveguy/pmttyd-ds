#include "battle_event_utilities.hpp"
#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <vector>
using namespace ttyd;
struct Fixture {
    std::map<uint32_t,uint32_t> words;std::vector<uint32_t> reads;std::function<void(uint32_t)> onRead;
    EventEntry e;int32_t output=0,result=0;unsigned resolves=0;
    std::function<void()> onResolve;
    Fixture() {e.arguments=0x1000;words[0x8041eb90]=0x2000;words[0x1000]=uint32_t(-30000000);words[0x1004]=2;words[0x2ef4]=2;}
    BattleEventUtilityServices services() {
        BattleEventUtilityServices s;s.memory.user=this;s.values.user=this;
        s.memory.word=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);auto v=f.words[a];if(f.onRead)f.onRead(a);return v;};
        s.values.resolve=[](void* p,EventEntry&,int32_t raw) {auto& f=*static_cast<Fixture*>(p);++f.resolves;if(f.onResolve)f.onResolve();return raw;};
        s.values.store=[](void* p,EventEntry&,int32_t output,int32_t value) {auto& f=*static_cast<Fixture*>(p);f.output=output;f.result=value;};return s;
    }
};
int main() {
    Fixture flag;assert(battleFlagQueryEvent(flag.e,flag.services())==2 && flag.output==-30000000 && flag.result==1 && flag.resolves==0 && flag.reads==std::vector<uint32_t>({0x8041eb90,0x1004,0x2ef4,0x1000}));
    flag.words[0x1004]=0;assert(battleFlagQueryEvent(flag.e,flag.services())==2 && flag.result==0);
    flag.words[0x1004]=uint32_t(-30000000);flag.words[0x2ef4]=0x80000000;assert(battleFlagQueryEvent(flag.e,flag.services())==2 && flag.result==1 && flag.resolves==0); // Mask remains raw token bits.
    Fixture capture;capture.onRead=[&](uint32_t a) {if(a==0x8041eb90) {capture.words[a]=0x3000;capture.e.arguments=0x5000;}if(a==0x2ef4)capture.words[0x1000]=uint32_t(-30000001);};
    assert(battleFlagQueryEvent(capture.e,capture.services())==2 && capture.result==1 && capture.output==-30000001); // Old args/battle;late raw output.
    const std::vector<std::pair<int32_t,int32_t>> cases{{29,483},{200,3333},{90,1500},{80,1333},{5,83},{-29,-483},{0,0},{2147483647,-16},{-2147483647-1,0}};
    for(auto [input,expected]:cases) {Fixture time;time.words[0x1000]=uint32_t(input);time.words[0x1004]=uint32_t(-30000000);assert(battleFramesToMillisecondsEvent(time.e,time.services())==2 && time.result==expected && time.output==-30000000 && time.resolves==1 && time.reads==std::vector<uint32_t>({0x1000,0x1004}));}
    Fixture late;late.words[0x1000]=29;late.onResolve=[&] {late.e.arguments=0x9000;late.words[0x1004]=uint32_t(-30000002);};assert(battleFramesToMillisecondsEvent(late.e,late.services())==2 && late.result==483 && late.output==-30000002);
    std::cout<<"battle flag and wrapped frame conversion tests passed\n";
}
