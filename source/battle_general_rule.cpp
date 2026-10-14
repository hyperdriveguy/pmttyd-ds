#include "battle_general_rule.hpp"
#include "battle_turn_rule.hpp"
#include <array>
#include <cassert>
namespace ttyd {
namespace {
uint32_t word(const BattleActorServices& s,uint32_t a){return s.memory.word(s.memory.user,a);}
uint8_t byte(const BattleActorServices& s,uint32_t a){return s.byte(s.user,a);}
void store(const BattleActorServices& s,uint32_t a,uint8_t v){s.memory.storeByte(s.memory.user,a,v);}
struct Fields {bool use;std::array<uint32_t,4> offsets;};
constexpr Fields scalar[]{
 {false,{0}}, {false,{0x16f38}}, {true,{0x16f38}}, {false,{0x16f39}}, {true,{0x16f39}},
 {false,{0x16f3a,0x16f3b}}, {true,{0x16f3a,0x16f3b}},
 {false,{0x16f3c}}, {true,{0x16f3c}}, {false,{0x16f3d}}, {true,{0x16f3d}},
 {false,{0x16f3c,0x16f3d}}, {true,{0x16f3c,0x16f3d}},
 {false,{0x16f3e}}, {true,{0x16f3e}}, {false,{0x16f3f}}, {true,{0x16f3f}},
 {false,{0x16f3e,0x16f3f}}, {true,{0x16f3e,0x16f3f}}, {true,{0}}, {false,{0}}, {true,{0x16f40}},
 {false,{0x16f41,0x16f42}}, {true,{0x16f41,0x16f42}},
 {false,{0x16f43,0x16f44}}, {true,{0x16f43,0x16f44}},
 {false,{0x16f43,0x16f42,0x16f44,0x16f41}}, {true,{0x16f43,0x16f42,0x16f44,0x16f41}},
 {false,{0x16f45}}, {true,{0x16f45}}, {false,{0x16f46}}, {true,{0x16f46}},
 {false,{0x16f45,0x16f46}}, {true,{0x16f45,0x16f46}},
 {false,{0x16f47}}, {true,{0x16f47}}, {false,{0x16f48}}, {true,{0x16f48}},
 {false,{0x16f49}}, {true,{0x16f49}}, {false,{0x16f4a}}, {true,{0x16f4a}},
 {false,{0x16f49,0x16f4a}}, {true,{0x16f49,0x16f4a}},
 {false,{0x16f4b}}, {true,{0x16f4b}}, {false,{0x16f4d}}, {true,{0x16f4d}},
 {false,{0x16f4b,0x16f4d}}, {true,{0x16f4b,0x16f4d}},
 {false,{0x16f4c}}, {true,{0x16f4c}}, {false,{0x16f4e}}, {true,{0x16f4e}},
 {false,{0x16f4b,0x16f4d}}, {true,{0x16f4b,0x16f4d}},
 {false,{0x16f4c,0x16f4e}}, {true,{0x16f4c,0x16f4e}}
};
static_assert(std::size(scalar)==58);
}
int32_t checkBattleRuleNoUse(int32_t input,const BattleActorServices& s) {
    const auto descriptor=word(s,currentBattleAddress(s)+0x2738);
    if(input>=byte(s,descriptor+0x1b))return 1;
    return input>=byte(s,descriptor+0x1a)?2:6;
}
int32_t checkBattleRuleUse(int32_t input,const BattleActorServices& s) {
    const auto descriptor=word(s,currentBattleAddress(s)+0x2738);
    if(input>=byte(s,descriptor+0x1a))return 6;
    return input>=byte(s,descriptor+0x1b)?2:1;
}
void judgeBattleGeneralRule(const BattleActorServices& s) {
    assert(s.memory.word && s.byte && s.memory.storeByte);
    const auto battle=currentBattleAddress(s),descriptor=word(s,battle+0x2738);
    const auto mario=battleMarioUnit(battle,s);
    battlePartyUnit(battle,s); // Required prefix even for rule0, ignored rules and null Mario.
    const auto rule=byte(s,descriptor+0x18);
    if(rule>67)return;
    if(rule==0){store(s,descriptor+0x19,6);return;}
    if(rule<58) {
        int32_t input=0;
        if(rule==19 || rule==20) {
            if(!mario){store(s,descriptor+0x19,1);return;}
            assert(s.memory.half);input=int16_t(s.memory.half(s.memory.user,mario+0x10c));
        } else for(auto offset:scalar[rule].offsets)if(offset)input+=byte(s,battle+offset);
        const auto result=scalar[rule].use?checkBattleRuleUse(input,s):checkBattleRuleNoUse(input,s);
        store(s,descriptor+0x19,uint8_t(result));return; // Write captured P, not helper's independently captured P2.
    }
    if(rule<60) {
        const auto turn=byte(s,battle+0x16f57),first=byte(s,descriptor+0x1a);
        if(rule==58?turn>first:turn<=first){store(s,descriptor+0x19,6);return;}
        const auto second=byte(s,descriptor+0x1b);
        store(s,descriptor+0x19,(rule==58?turn>second:turn<=second)?2:1);return;
    }
    static constexpr uint32_t zero60[]{0x16f3a,0x16f39,0x16f3b,0x16f41,0x16f42,0x16f45,0x16f4f,0x16f51,0x16f53,0x16f38};
    static constexpr uint32_t zero61[]{0x16f43,0x16f56,0x16f44,0x16f46,0x16f50,0x16f52,0x16f54,0x16f55};
    static constexpr uint32_t zero62[]{0x16f3a,0x16f39,0x16f3b,0x16f41,0x16f42,0x16f45,0x16f4f,0x16f51,0x16f53,0x16f55,0x16f56,0x16f43,0x16f44,0x16f46,0x16f50,0x16f52,0x16f54,0x16f38};
    static constexpr uint32_t zero63[]{0x16f3a,0x16f39,0x16f41,0x16f38};
    static constexpr uint32_t zero64[]{0x16f55,0x16f43};
    static constexpr uint32_t zero65[]{0x16f3a,0x16f39,0x16f41,0x16f55,0x16f43,0x16f38};
    const uint32_t* fields=nullptr;unsigned count=0;
    switch(rule){case 60:fields=zero60;count=10;break;case 61:fields=zero61;count=8;break;case 62:fields=zero62;count=18;break;case 63:fields=zero63;count=4;break;case 64:fields=zero64;count=2;break;case 65:fields=zero65;count=6;break;}
    uint32_t sum=0;
    if(fields) {
        for(unsigned i=0;i<count;++i)sum+=byte(s,battle+fields[i]);
        checkBattleZeroTurnCount(sum,s);return; // Source end/turn helpers have identical bodies; no captured-P copy-back.
    }
    for(auto offset:{0x16f3bu,0x16f3au,0x16f41u,0x16f42u,0x16f45u,0x16f4fu,0x16f51u,0x16f49u,0x16f53u})sum+=byte(s,battle+offset);
    sum+=byte(s,battle+(rule==66?0x16f39:0x16f38));store(s,descriptor+0x19,sum==0?6:1);
}
}
