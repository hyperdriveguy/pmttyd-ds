#include "battle_phase.hpp"
#include <cassert>
#include <map>
#include <set>
#include <iostream>
using namespace ttyd;
struct Fixture {
    static constexpr uint32_t b=0x1000;
    std::map<uint32_t,uint32_t> words;
    std::map<uint32_t,uint8_t> bytes;
    std::map<uint32_t,float> reals;
    std::set<std::pair<uint32_t,int>> statuses;
    BattlePhaseServices services() {
        BattlePhaseServices s;s.actors.user=s.actors.memory.user=this;
        s.actors.memory.word=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->words[a];};
        s.actors.memory.storeWord=[](void* p,uint32_t a,uint32_t v){static_cast<Fixture*>(p)->words[a]=v;};
        s.actors.memory.storeByte=[](void* p,uint32_t a,uint8_t v){static_cast<Fixture*>(p)->bytes[a]=v;};
        s.actors.byte=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->bytes[a];};
        s.real=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->reals[a];};
        s.status=[](void* p,uint32_t u,int32_t id){return static_cast<Fixture*>(p)->statuses.count({u,id})!=0;};return s;
    }
    uint32_t add(int id,float x,int priority=0,uint32_t phase=0x04000002) {
        const uint32_t u=0x10000+id*0x1000;
        words[b+0x20+4*id]=u;words[u]=id;words[u+0x28]=uint32_t(priority);
        words[u+0x2c]=phase;reals[u+0x30]=x;bytes[u+0x22]=1;return u;
    }
};
int main() {
    constexpr auto b=Fixture::b;
    Fixture f;f.words[b+0xf18]=0x04000002;
    const auto a=f.add(1,-90,0),c=f.add(2,-20,0),d=f.add(3,-20,0);
    const auto high=f.add(4,-120,2),low=f.add(5,-1,-1),slow=f.add(6,4);
    f.bytes[slow+0x20]=10;f.statuses.insert({slow,20});
    const auto excluded=f.add(7,1);f.words[excluded+0x104]=0x80000;
    const auto spent=f.add(8,1);f.bytes[spent+0x20]=10;
    f.add(9,20,0,0x05000009);
    const uint32_t part=0x30000;f.words[a+0x14]=part;f.words[part+0x1c0]=0;
    buildBattlePhaseTables(b,f.services());
    const uint32_t ordered[]={4,6,2,3,1,5};
    for(unsigned i=0;i<6;++i)assert(f.words[b+0x120+4*i]==ordered[i]);
    assert(f.words[b+0x138]==0xffffffff && f.bytes[part+0x4e4]==1);
    assert(f.bytes[excluded+0x311]==255 && f.bytes[spent+0x311]==255);
    // The independent positional table includes other phases and spent actors.
    assert(f.words[b+0x220]==5 && f.words[b+0x228]==2 && f.words[b+0x230]==3);
    assert(!battlePhaseEnded(b,f.services()) && selectBattlePhaseActor(b,f.services())==4);
    f.bytes[high+0x20]=1; // Deferred actors clear state and are skipped once.
    assert(selectBattlePhaseActor(b,f.services())==2 && f.bytes[high+0x20]==0);
    f.statuses.insert({c,27});assert(selectBattlePhaseActor(b,f.services())==4);
    f.bytes[high+0x22]=0;assert(selectBattlePhaseActor(b,f.services())==3);
    f.words[c+0x104]=0x10000;assert(selectBattlePhaseActor(b,f.services())==2);
    // Dead actors cannot keep a phase alive, even when selection permits them.
    for(auto u:{a,d,high,low})f.bytes[u+0x22]=0;
    assert(battlePhaseEnded(b,f.services()));

    Fixture moves;moves.words[b+0xf18]=0x04000002;
    const auto mario=moves.add(1,-80),partner=moves.add(2,-60);
    buildBattlePhaseTables(b,moves.services());
    moves.bytes[mario+0x24]=7;consumeBattleActorMove(b,mario,moves.services());
    assert(moves.bytes[mario+0x20]==10 && moves.bytes[mario+0x22]==0 && moves.bytes[mario+0x24]==0);
    assert(moves.bytes[mario+0x311]==96 && !battlePhaseEnded(b,moves.services()));
    moves.words[partner+0x27c]=0x2000;moves.bytes[partner+0x24]=7;
    consumeBattleActorMove(b,partner,moves.services());
    assert(battlePhaseEnded(b,moves.services()) && moves.bytes[partner+0x24]==7 && moves.words[partner+0x27c]==0);
    moves.bytes[mario+0x20]=0;moves.bytes[mario+0x22]=1;moves.words[mario+0x138]=0x02000000;
    consumeBattleActorMove(b,mario,moves.services());assert(moves.bytes[mario+0x22]==1 && moves.words[mario+0x138]==0);
    for(auto transition:{std::pair<int,int>{5,6},{6,7},{7,10},{8,9},{9,10},{3,0},{4,0}}) {
        moves.bytes[mario+0x20]=transition.first;moves.bytes[mario+0x22]=1;
        consumeBattleActorMove(b,mario,moves.services());assert(moves.bytes[mario+0x20]==transition.second);
        assert(moves.bytes[mario+0x22]==(transition.second==10?0:1));
    }
    moves.bytes[mario+0x20]=0;moves.bytes[mario+0x22]=0;
    consumeBattleActorMove(b,mario,moves.services());assert(moves.bytes[mario+0x22]==255 && moves.bytes[mario+0x20]==10);
    moves.bytes[mario+0x20]=0;moves.bytes[mario+0x22]=3;moves.words[b+0xef4]=0x2000;
    consumeBattleActorMove(b,mario,moves.services());assert(moves.bytes[mario+0x22]==0 && moves.bytes[mario+0x20]==10);
    std::cout<<"Original phase ordering, selection, completion and move consumption passed\n";
}
