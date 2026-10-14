#include "battle_main.hpp"
#include <cassert>
#include <map>
#include <vector>
using namespace ttyd;
struct Fixture {
    static constexpr uint32_t B=0x1000,C=0x3000,G=0x5000;
    std::map<uint32_t,uint32_t> words{{0x8041eb90,B},{0x80415fd0,G}};
    std::vector<uint32_t> calls,reads,writes;
    uint32_t unavailable=0,replaceOwner=0;
    bool changeBankAfterFirstRead=false;
    unsigned bankReads=0;
    BattleMainServices services() {
        BattleMainServices s;s.user=this;s.actors.memory.user=this;
        s.actors.memory.word=[](void* p,uint32_t a){auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);
            if(a==B+4 && f.changeBankAfterFirstRead)return ++f.bankReads==1?1u:2u;
            return f.words[a];};
        s.actors.memory.storeWord=[](void* p,uint32_t a,uint32_t v){auto& f=*static_cast<Fixture*>(p);f.writes.push_back(a);f.words[a]=v;};
        s.request=[](void* p,uint32_t fn,uint32_t b){auto& f=*static_cast<Fixture*>(p);assert(b==B);f.calls.push_back(fn);
            if(f.replaceOwner)f.words[0x8041eb90]=f.replaceOwner;
            if(fn==0x801024f0)f.words[B+0xef4]|=0x400;
            return fn!=f.unavailable;};return s;
    }
};
int main() {
    const std::vector<uint32_t> expected(battleMainRequests.begin(),battleMainRequests.end());
    Fixture ended;ended.words[Fixture::B+4]=4;ended.words[Fixture::G]=0x3000;
    auto s=ended.services();assert(packedBattleMainWrapper(s));
    assert(ended.calls==expected && ended.words[Fixture::B+0xef4]==0x80000400);
    assert(packedBattleSequenceEndCheck(s.actors));
    for(auto a:ended.reads)assert(a!=0x80415fd0);
    Fixture missing;missing.words[Fixture::B+4]=4;missing.unavailable=0x80149798;s=missing.services();
    assert(!packedBattleMainWrapper(s) && missing.calls.size()==5 && missing.words[Fixture::B+0xef4]==0);
    assert(!packedBattleSequenceEndCheck(s.actors));
    Fixture owner;owner.words[Fixture::B+4]=4;owner.words[Fixture::C+4]=2;owner.replaceOwner=Fixture::C;s=owner.services();
    assert(packedBattleMainWrapper(s) && owner.words[Fixture::B+0xef4]==0x80000400 && owner.words[Fixture::C+0xef4]==0);
    for(uint32_t flags:{0u,0x1000u,0x2000u,0x3000u}) {
        Fixture f;f.words[Fixture::B+4]=2;f.words[Fixture::G]=flags;f.words[Fixture::B+0xef4]=0x80000001;s=f.services();
        assert(packedBattleMainWrapper(s) && f.calls==expected);
        assert(f.words[Fixture::B+0xef4]==(flags==0x3000?0x80000461u:0x80000401u));
        assert(f.words[Fixture::B+4]==(flags==0x3000?3u:2u));
        if(flags==0x3000)assert(f.words[Fixture::B+0xf24]==0x07000000);
    }
    Fixture fresh;fresh.words[Fixture::G]=0x3000;fresh.changeBankAfterFirstRead=true;s=fresh.services();
    assert(packedBattleMain(s)==1 && fresh.bankReads==2 && fresh.words[Fixture::B+4]==3);
    Fixture null;null.words[0x8041eb90]=0;s=null.services();assert(packedBattleMainWrapper(s) && null.calls.empty() && null.writes.empty());
}
