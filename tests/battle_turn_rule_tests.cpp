#include "battle_turn_rule.hpp"
#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <vector>
using namespace ttyd;
struct Fixture {
    std::map<uint32_t,uint32_t> words;std::map<uint32_t,uint8_t> bytes;
    std::vector<uint32_t> reads;std::vector<uint8_t> statuses;
    std::function<void(uint32_t)> onRead;std::function<void(uint8_t)> onStore;
    Fixture() {words[0x8041eb90]=0x2000;words[0x4738]=0x3000;}
    BattleActorServices services() {
        BattleActorServices s;s.user=this;s.memory.user=this;
        s.memory.word=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);auto v=f.words[a];if(f.onRead)f.onRead(a);return v;};
        s.byte=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);auto v=f.bytes[a];if(f.onRead)f.onRead(a);return v;};
        s.memory.storeByte=[](void* p,uint32_t a,uint8_t v) {auto& f=*static_cast<Fixture*>(p);f.bytes[a]=v;f.statuses.push_back(v);if(f.onStore)f.onStore(v);};return s;
    }
};
int main() {
    for(uint8_t rule:{0,59,66,255}) {Fixture f;f.bytes[0x3018]=rule;judgeBattleTurnRule(f.services());assert(f.reads==std::vector<uint32_t>({0x8041eb90,0x4738,0x3018}) && f.statuses.empty());}
    for(uint32_t sum:{0u,1u,0xffffffffu}) {
        Fixture f;f.bytes[0x18f57]=5;f.bytes[0x301a]=5;f.bytes[0x301b]=5;
        assert(checkBattleZeroTurnCount(sum,f.services())==(sum==0?6:1));assert(f.statuses==std::vector<uint8_t>(sum==0?std::initializer_list<uint8_t>{2,6}:std::initializer_list<uint8_t>{1,1}));
        auto reads=std::vector<uint32_t>{0x8041eb90,0x4738,0x18f57,0x301b,0x18f57,0x301a};if(sum!=0)reads.push_back(0x3019);reads.push_back(0x3019);assert(f.reads==reads);
    }
    for(uint8_t status:{2,6,4,0x80}) {Fixture f;f.bytes[0x18f57]=10;f.bytes[0x301b]=9;f.bytes[0x301a]=255;f.bytes[0x3019]=status;assert(checkBattleZeroTurnCount(1,f.services())==((status&2)?status:1));assert(f.statuses.size()==((status&2)?0u:1u));}
    Fixture excluded;excluded.bytes[0x18f57]=10;excluded.bytes[0x301a]=9;excluded.bytes[0x301b]=9;excluded.bytes[0x3019]=255;assert(checkBattleZeroTurnCount(0,excluded.services())==255 && excluded.statuses.empty());
    Fixture reload;reload.onStore=[&](uint8_t) {reload.bytes[0x18f57]=9;reload.words[0x8041eb90]=0x9000;};assert(checkBattleZeroTurnCount(0,reload.services())==2 && reload.statuses==std::vector<uint8_t>{2});
    Fixture final;final.onStore=[&](uint8_t) {final.bytes[0x3019]=255;};assert(checkBattleZeroTurnCount(0,final.services())==255);
    // Contract-derived counter selectors and load order,independent of status
    // outcome tests. Every used field can make the sum nonzero;unused fields do not.
    const std::vector<std::vector<uint32_t>> fields{
        {0x16f3a,0x16f39,0x16f3b,0x16f41,0x16f42,0x16f45,0x16f4f,0x16f51,0x16f53,0x16f38},
        {0x16f43,0x16f56,0x16f44,0x16f46,0x16f50,0x16f52,0x16f54,0x16f55},
        {0x16f3a,0x16f39,0x16f3b,0x16f41,0x16f42,0x16f45,0x16f4f,0x16f51,0x16f53,0x16f55,0x16f56,0x16f43,0x16f44,0x16f46,0x16f50,0x16f52,0x16f54,0x16f38},
        {0x16f3a,0x16f39,0x16f41,0x16f38},{0x16f55,0x16f43},{0x16f3a,0x16f39,0x16f41,0x16f55,0x16f43,0x16f38}};
    for(unsigned index=0;index<fields.size();++index) {
        Fixture f;f.bytes[0x3018]=60+index;judgeBattleTurnRule(f.services());assert(f.bytes[0x3019]==6);
        for(unsigned i=0;i<fields[index].size();++i)assert(f.reads[3+i]==0x2000+fields[index][i]);assert(f.reads[3+fields[index].size()]==0x8041eb90);
        for(auto field:fields[index]) {Fixture nonzero;nonzero.bytes[0x3018]=60+index;nonzero.bytes[0x2000+field]=255;judgeBattleTurnRule(nonzero.services());assert(nonzero.bytes[0x3019]==1);}
    }
    Fixture recapture;recapture.bytes[0x3018]=64;recapture.words[0x7738]=0x8000;recapture.onRead=[&](uint32_t a) {if(a==0x18f43)recapture.words[0x8041eb90]=0x5000;};judgeBattleTurnRule(recapture.services());assert(recapture.bytes[0x8019]==6 && recapture.bytes[0x3019]==0);
    std::cout<<"turn-rule selector/two-phase status tests passed\n";
}
