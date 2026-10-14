#include "battle_general_rule.hpp"
#include <cassert>
#include <algorithm>
#include <map>
#include <vector>
#include <functional>
using namespace ttyd;
struct Fixture {
    static constexpr uint32_t B=0x2000,P=0x5000,M=0x6000,Party=0x7000;
    std::map<uint32_t,uint32_t> words;
    std::map<uint32_t,uint8_t> bytes;
    std::map<uint32_t,uint16_t> halves;
    std::vector<uint32_t> reads,writes;
    std::function<void(uint32_t)> onRead;
    Fixture(){words[0x8041eb90]=B;words[B+0x2738]=P;words[B+0x20]=M;words[B+0x24]=Party;words[M+8]=222;words[Party+8]=224;}
    BattleActorServices services() {
        BattleActorServices s;s.user=this;s.memory.user=this;
        s.memory.word=[](void* p,uint32_t a){auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);auto value=f.words[a];if(f.onRead)f.onRead(a);return value;};
        s.byte=[](void* p,uint32_t a){auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);auto value=f.bytes[a];if(f.onRead)f.onRead(a);return value;};
        s.memory.half=[](void* p,uint32_t a){auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);auto value=f.halves[a];if(f.onRead)f.onRead(a);return value;};
        s.memory.storeByte=[](void* p,uint32_t a,uint8_t value){auto& f=*static_cast<Fixture*>(p);f.bytes[a]=value;f.writes.push_back(a);};return s;
    }
};
int main() {
    constexpr auto B=Fixture::B,P=Fixture::P,M=Fixture::M,Party=Fixture::Party;
    for(uint8_t rule:{0,68,255}) {
        Fixture f;f.bytes[P+0x18]=rule;f.bytes[P+0x19]=0xa5;judgeBattleGeneralRule(f.services());
        const std::vector<uint32_t> prefix{0x8041eb90,B+0x2738,B+0x20,M+8,M+0xc,B+0x20,M+0x1c,M+8,B+0x24,Party+0x1c,Party+8,Party+0xc,P+0x18};
        assert(f.reads==prefix && f.bytes[P+0x19]==(rule==0?6:0xa5));
    }
    for(int32_t input:{-1,0,5,10,255}) {
        Fixture f;f.bytes[P+0x1a]=5;f.bytes[P+0x1b]=10;
        assert(checkBattleRuleNoUse(input,f.services())==(input>=10?1:input>=5?2:6));
        assert(checkBattleRuleUse(input,f.services())==(input>=5?6:input>=10?2:1));
    }
    Fixture wide;wide.bytes[P+0x18]=5;wide.bytes[P+0x1a]=255;wide.bytes[P+0x1b]=255;
    wide.bytes[B+0x16f3a]=wide.bytes[B+0x16f3b]=255;judgeBattleGeneralRule(wide.services());assert(wide.bytes[P+0x19]==1);
    for(uint8_t rule:{19,20}) {
        Fixture hp;hp.bytes[P+0x18]=rule;hp.halves[M+0x10c]=0xffff;judgeBattleGeneralRule(hp.services());assert(hp.bytes[P+0x19]==(rule==19?1:6));
        Fixture absent;absent.words[B+0x20]=0;absent.bytes[P+0x18]=rule;judgeBattleGeneralRule(absent.services());assert(absent.bytes[P+0x19]==1);
    }
    // Scalar helper recaptures thresholds but writes the caller's captured P.
    Fixture scalar;scalar.bytes[P+0x18]=1;scalar.bytes[P+0x1a]=1;scalar.bytes[P+0x1b]=2;scalar.words[0xa738]=0x9000;
    scalar.onRead=[&](uint32_t a){if(a==P+0x18)scalar.words[0x8041eb90]=0x8000;};
    judgeBattleGeneralRule(scalar.services());assert(scalar.writes==std::vector<uint32_t>{P+0x19} && scalar.bytes[P+0x19]==1);
    for(uint8_t rule:{58,59}) {
        Fixture turn;turn.bytes[P+0x18]=rule;turn.bytes[B+0x16f57]=turn.bytes[P+0x1a]=turn.bytes[P+0x1b]=255;
        judgeBattleGeneralRule(turn.services());assert(turn.bytes[P+0x19]==(rule==58?1:6));
    }
    // The source duplicates 4b/4d for54/55, not the neighbouring count bytes.
    Fixture duplicate;duplicate.bytes[P+0x18]=54;duplicate.bytes[P+0x1a]=1;duplicate.bytes[P+0x1b]=2;duplicate.bytes[B+0x16f4c]=255;
    judgeBattleGeneralRule(duplicate.services());assert(duplicate.bytes[P+0x19]==6);
    duplicate.bytes[B+0x16f4d]=3;judgeBattleGeneralRule(duplicate.services());assert(duplicate.bytes[P+0x19]==1);
    for(uint8_t rule=60;rule<=67;++rule) {
        Fixture zero;zero.bytes[P+0x18]=rule;judgeBattleGeneralRule(zero.services());assert(zero.bytes[P+0x19]==6);
    }
    Fixture end;end.bytes[P+0x18]=64;end.words[0xa738]=0x9000;
    end.onRead=[&](uint32_t a){if(a==B+0x16f43)end.words[0x8041eb90]=0x8000;};
    judgeBattleGeneralRule(end.services());assert(end.bytes[0x9019]==6 && end.bytes[P+0x19]==0);
    for(uint8_t rule:{66,67}) {
        Fixture last;last.bytes[P+0x18]=rule;last.bytes[B+0x16f39]=1;
        judgeBattleGeneralRule(last.services());assert(last.bytes[P+0x19]==(rule==66?1:6));
    }
}
