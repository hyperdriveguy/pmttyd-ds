#include "battle_packed_status.hpp"
#include <array>
#include <cassert>
#include <functional>
#include <iostream>
#include <vector>
using namespace ttyd;
struct Fixture {
    std::array<uint8_t,0x400> bytes{};std::vector<uint32_t> reads;std::vector<uint32_t> writes;
    std::function<void(uint32_t)> onStore;
    BattleActorServices services() {
        BattleActorServices s;s.user=this;s.memory.user=this;
        s.byte=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);return f.bytes.at(a-0x1000);};
        s.memory.storeByte=[](void* p,uint32_t a,uint8_t v) {auto& f=*static_cast<Fixture*>(p);f.writes.push_back(a);f.bytes.at(a-0x1000)=v;if(f.onStore)f.onStore(a);};return s;
    }
};
int main() {
    const std::vector<std::pair<int,uint32_t>> single{{0,0x12a},{1,0x118},{2,0x11a},{3,0x11b},{5,0x11e},{6,0x11f},{7,0x120},{8,0x121},{9,0x122},{16,0x129},{17,0x12b},{18,0x12c},{19,0x12f},{20,0x130},{21,0x12d},{22,0x12e},{27,0x135}};
    for(auto [id,offset]:single) {Fixture f;f.bytes[offset]=255;auto value=getPackedBattleStatus(0x1000,id,f.services());assert(value.duration==255 && value.strength==255 && f.reads==std::vector<uint32_t>{0x1000+offset} && checkPackedBattleStatus(0x1000,id,f.services()));}
    for(auto [id,offset]:std::vector<std::pair<int,uint32_t>>{{4,0x11c},{23,0x131},{24,0x133}}) {Fixture f;f.bytes[offset]=7;f.bytes[offset+1]=9;auto value=getPackedBattleStatus(0x1000,id,f.services());assert(value.duration==7 && value.strength==9 && f.reads==std::vector<uint32_t>({0x1000+offset,0x1000+offset+1}));}
    for(int id=10;id<=15;++id)for(uint8_t strength:{0,1,127,128,255}) {Fixture f;const uint32_t offset=0x123+uint32_t((id-10)/2)*2;f.bytes[offset]=4;f.bytes[offset+1]=strength;auto value=getPackedBattleStatus(0x1000,id,f.services());const bool admitted=id%2==0?(strength>0 && strength<128):strength>=128;assert(value.duration==(admitted?4:0) && value.strength==(admitted?strength:0) && f.reads.front()==0x1000+offset+1 && f.reads.size()==(admitted?2u:1u));}
    for(int id:{-1,25,26,28,127,128,255}) {Fixture f;auto value=getPackedBattleStatus(0,id,f.services());assert(value.duration==0 && value.strength==0 && f.reads.empty());}
    Fixture narrowed;narrowed.bytes[0x118]=2;assert(getPackedBattleStatus(0x1000,257,narrowed.services()).duration==2);
    Fixture zap;zap.bytes[0x2f7]=255;assert(checkPackedBattleStatus(0x1000,6,zap.services()) && zap.reads==std::vector<uint32_t>({0x111f,0x12f7}));zap.reads.clear();zap.bytes[0x11f]=255;assert(checkPackedBattleStatus(0x1000,262,zap.services()) && zap.reads==std::vector<uint32_t>{0x111f});zap.reads.clear();assert(!checkPackedBattleStatus(0,6,zap.services()) && zap.reads.empty());
    Fixture alias;alias.bytes[0x11c]=7;alias.bytes[0x11d]=9;alias.onStore=[&](uint32_t a) {if(a==0x111d)alias.bytes[0x11d]=99;};writePackedBattleStatus(0x1000,4,0x111d,0x1120,alias.services());assert(alias.bytes[0x120]==9 && alias.writes==std::vector<uint32_t>({0x111d,0x1120}));
    Fixture same;same.bytes[0x11c]=7;same.bytes[0x11d]=9;writePackedBattleStatus(0x1000,4,0x1300,0x1300,same.services());assert(same.bytes[0x300]==9 && same.writes.size()==2);
    Fixture optional;optional.bytes[0x12f]=1;writePackedBattleStatus(0x1000,19,0,0,optional.services());assert(optional.reads==std::vector<uint32_t>{0x112f} && optional.writes.empty());
    std::cout<<"packed status getter/presence/output alias tests passed\n";
}
