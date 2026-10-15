#include "battle_main_continuation.hpp"
#include "battle_sequence_startup.hpp"
#include <cassert>
#include <map>
#include <vector>
using namespace ttyd;
struct Fixture {
    static constexpr uint32_t B=0x1000,C=0x3000,G=0x5000;
    std::map<uint32_t,uint32_t> words{{0x8041eb90,B},{0x80415fd0,G}};
    std::vector<std::array<uint32_t,3>> effects;
    std::vector<uint32_t> reads,attempts;
    uint32_t blocked=0,replaceOwner=0;
    bool splitOuter=false,reenter=false;
    unsigned globalReads=0;
    BattleMainCursor* mainCursor=nullptr;
    BattleSequenceDispatchCursor* sequenceCursor=nullptr;
    BattleActorServices actors(){
        BattleActorServices a;a.memory.user=this;
        a.memory.word=[](void* u,uint32_t at){auto& f=*static_cast<Fixture*>(u);f.reads.push_back(at);
            if(at==0x8041eb90 && f.splitOuter)return ++f.globalReads==1?B:C;
            return f.words[at];};
        a.memory.storeWord=[](void* u,uint32_t at,uint32_t n){auto& f=*static_cast<Fixture*>(u);f.words[at]=n;f.effects.push_back({1,at,n});};
        return a;
    }
    BattleMainServices main(){
        BattleMainServices s;s.actors=actors();s.user=this;
        s.request=[](void* u,uint32_t fn,uint32_t b){auto& f=*static_cast<Fixture*>(u);f.attempts.push_back(fn);
            if(f.reenter){assert(f.mainCursor);assert(!resumePackedBattleMainWrapper(f.main(),*f.mainCursor));assert(!f.mainCursor->reset());}
            if(fn==f.blocked)return false;
            f.effects.push_back({0,fn,b});
            if(f.replaceOwner)f.words[0x8041eb90]=f.replaceOwner;
            if(fn==0x801024f0)f.words[b+0xef4]|=0x400;
            return true;};return s;
    }
    BattleSequenceDispatchServices sequence(){
        BattleSequenceDispatchServices s;s.actors=actors();s.user=this;
        s.request=[](void* u,const BattleSequenceDispatchRequest& q){auto& f=*static_cast<Fixture*>(u);f.attempts.push_back(q.function);
            if(f.reenter){assert(f.sequenceCursor);assert(!resumePackedBattleSequenceManager(f.sequence(),*f.sequenceCursor));assert(!f.sequenceCursor->reset());}
            if(q.function==f.blocked)return false;
            f.effects.push_back({q.function,q.battle,uint32_t(q.mode)});
            f.words[B+4]=4;f.words[0x8041eb90]=C;return true;};return s;
    }
};
void mainHolds(){
    for(uint32_t bank:{2u,4u})for(uint32_t flags:{0u,0x3000u})for(uint32_t fn:battleMainRequests){
        Fixture oracle;oracle.words[Fixture::B+4]=bank;oracle.words[Fixture::G]=flags;
        assert(packedBattleMainWrapper(oracle.main()));
        Fixture f;f.words[Fixture::B+4]=bank;f.words[Fixture::G]=flags;f.blocked=fn;
        BattleMainCursor c;assert(!resumePackedBattleMainWrapper(f.main(),c));assert(c.active() && c.pending()==fn);
        const auto prefix=f.effects;const auto reads=f.reads;
        for(unsigned i=0;i<2;++i){assert(!resumePackedBattleMainWrapper(f.main(),c));assert(f.effects==prefix && f.reads==reads);}
        auto changed=f.main();changed.user=&oracle;
        assert(!resumePackedBattleMainWrapper(changed,c));assert(f.effects==prefix && f.reads==reads);
        changed=f.main();changed.actors.audienceLifecycle.user=&oracle;
        assert(!resumePackedBattleMainWrapper(changed,c));assert(f.effects==prefix && f.reads==reads);
        assert(!resumePackedBattleMain(f.main(),c)); // Different entry cannot replace held wrapper.
        f.blocked=0;assert(resumePackedBattleMainWrapper(f.main(),c));
        assert(!c.active() && !c.pending() && f.effects==oracle.effects && f.words==oracle.words);
    }
    Fixture split;split.splitOuter=true;split.words[Fixture::C+4]=4;split.words[Fixture::B+0xef4]=0x11;
    BattleMainCursor c;split.blocked=battleMainRequests[6];assert(!resumePackedBattleMainWrapper(split.main(),c));
    split.words[Fixture::C+0xef4]=0x80;split.blocked=0;assert(resumePackedBattleMainWrapper(split.main(),c));
    assert(split.globalReads==2 && split.words[Fixture::B+0xef4]==0x80000011 && split.words[Fixture::C+0xef4]==0x480);
    for(const auto& e:split.effects)if(e[0]==0)assert(e[2]==Fixture::C);
    Fixture null;null.words[0x8041eb90]=0;auto empty=null.main();empty.request=nullptr;empty.actors.memory.storeWord=nullptr;
    assert(resumePackedBattleMainWrapper(empty,c) && !c.active() && null.effects.empty());
    Fixture held;held.words[Fixture::B+4]=4;held.blocked=battleMainRequests[3];
    assert(!resumePackedBattleMain(held.main(),c));held.words[0x8041eb90]=Fixture::C;held.blocked=0;
    assert(resumePackedBattleMain(held.main(),c)==0 && !(held.words[Fixture::B+0xef4]&0x80000000));
    Fixture reentry;reentry.words[Fixture::B+4]=4;reentry.reenter=true;reentry.mainCursor=&c;
    assert(resumePackedBattleMainWrapper(reentry.main(),c) && !c.active());
}
void sequenceHolds(){
    const std::vector<std::vector<std::array<uint32_t,3>>> expected{
        {{0x8011ee40,Fixture::B,0},{0x8011efd8,Fixture::B,1}},
        {{0x8011efd8,Fixture::B,0},{0x8011e5c0,Fixture::B,0}},
        {{0x8011efd8,Fixture::B,0},{0x8011c510,Fixture::B,0}},
        {{0x80215234,Fixture::B,0}}};
    for(unsigned bank=0;bank<4;++bank)for(const auto& hold:expected[bank]){
        Fixture f;f.words[Fixture::B+4]=bank;f.blocked=hold[0];BattleSequenceDispatchCursor c;
        assert(!resumePackedBattleSequenceManager(f.sequence(),c));assert(c.active() && c.pending()==hold[0]);
        const auto prefix=f.effects;const auto reads=f.reads;
        for(unsigned i=0;i<2;++i){assert(!resumePackedBattleSequenceManager(f.sequence(),c));assert(f.effects==prefix && f.reads==reads);}
        f.words[Fixture::B+4]=3;f.words[0x8041eb90]=Fixture::C;
        f.blocked=0;assert(resumePackedBattleSequenceManager(f.sequence(),c));assert(!c.active() && !c.pending());
        assert(f.effects==expected[bank]);
    }
    for(uint32_t bank:{4u,5u,0xffffffffu}){Fixture f;f.words[Fixture::B+4]=bank;auto s=f.sequence();s.request=nullptr;BattleSequenceDispatchCursor c;
        assert(resumePackedBattleSequenceManager(s,c) && f.effects.empty() && !c.active());}
    Fixture f;f.words[Fixture::B+4]=1;f.reenter=true;BattleSequenceDispatchCursor c;f.sequenceCursor=&c;
    assert(resumePackedBattleSequenceManager(f.sequence(),c) && f.effects==expected[1]);
}
int main(){mainHolds();sequenceHolds();return 0;}
