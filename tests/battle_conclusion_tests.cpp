#include "battle_conclusion.hpp"
#include <cassert>
#include <unordered_map>
#include <set>
#include <vector>
#include <iostream>
using namespace ttyd;
namespace {
constexpr uint32_t B=0x10000,M=0x40000,E=0x41000,G=0x42000;
struct Fixture {
 std::unordered_map<uint32_t,uint32_t> words;std::unordered_map<uint32_t,uint16_t> halves;std::unordered_map<uint32_t,uint8_t> bytes;
 std::set<uint32_t> dead,live;std::vector<uint32_t> eventQueries,statusQueries;unsigned afterCalls=0,enemyCalls=0;int after=0;bool mutateOnStatus=false;
 BattleConclusionServices services(){BattleConclusionServices s;s.user=this;s.actors.user=this;s.actors.memory.user=this;
 s.actors.memory.word=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->words[a];};s.actors.memory.half=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->halves[a];};
 s.actors.memory.storeWord=[](void* p,uint32_t a,uint32_t v){static_cast<Fixture*>(p)->words[a]=v;};s.actors.byte=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->bytes[a];};
 s.checkStatus=[](void* p,uint32_t u,int32_t id)->int32_t{auto& f=*static_cast<Fixture*>(p);assert(id==27);f.statusQueries.push_back(u);if(f.mutateOnStatus)f.words[u+0x104]|=0x10000000;return f.dead.count(u)?1:0;};
 s.eventExists=[](void* p,uint32_t id){auto& f=*static_cast<Fixture*>(p);f.eventQueries.push_back(id);return f.live.count(id)!=0;};
 s.afterReaction=[](void* p)->int32_t{auto& f=*static_cast<Fixture*>(p);++f.afterCalls;return f.after;};
 s.enemyBelong=[](void* p,uint32_t u)->int32_t{auto& f=*static_cast<Fixture*>(p);assert(u==M);++f.enemyCalls;return 1;};return s;}
 Fixture(){words[B+0x20]=M;words[B+0x24]=E;words[B+0x28]=G;words[M]=0;words[M+8]=222;words[E]=1;words[E+8]=0xb0;words[G]=2;words[G+8]=0xdd;bytes[E+0xc]=bytes[G+0xc]=1;words[M+0x104]=0x40000;words[G+0x104]=0x10000000;halves[B+8]=2;halves[B+16]=1;}
};
}
int main(){
 Fixture f;auto s=f.services();assert(battleCheckConcluded(B,s)==0);assert(f.words[B+12]==0 && f.words[B+20]==0 && f.words[B+28]==1);
 // Zero HP has no effect; only original status/attributes/group gates qualify.
 f.halves[E+0x10c]=0;assert(battleCheckConcluded(B,s)==0);f.dead.insert(E);assert(battleCheckConcluded(B,s)==1);assert(f.words[B+20]==1);assert(battleInitialEndResult(B,s)==1);
 f.dead.insert(M);battleCheckConcluded(B,s);assert(battleInitialEndResult(B,s)==3); // Own loss wins simultaneous deaths.
 f.words[B+0xef4]=0x70;assert(battleInitialEndResult(B,s)==4);f.words[B+0xef4]=0x30;assert(battleInitialEndResult(B,s)==5);f.words[B+0xef4]=0x10;assert(battleInitialEndResult(B,s)==3);
 Fixture no;auto ns=no.services();battleCheckConcluded(B,ns);assert(!battleInitialEndResult(B,ns));no.words[B+0xef4]=0x10;assert(battleInitialEndResult(B,ns)==4);
 no.halves[B+16]=4;no.words[B+0xef4]=0;assert(battleCheckConcluded(B,ns)==0 && no.words[B+20]==0); // Unrelated nonzero configuration.
 no.words[B+0xef4]=0x20;assert(battleCheckConcluded(B,ns)==1 && no.words[B+20]==0); // Forced predicate still refreshes groups.
 Fixture mut;auto ms=mut.services();mut.mutateOnStatus=true;assert(battleCheckConcluded(B,ms)==1);assert(mut.words[B+12]==1 && mut.words[B+20]==1); // Reload attrs after status provider.
 Fixture barrier;auto bs=barrier.services();barrier.words[M+0x29c]=11;barrier.words[M+0x2a8]=12;barrier.words[E+0x2bc]=13;barrier.live.insert(11);
 assert(!battleUnitEventBarrier(B,bs));assert((barrier.eventQueries==std::vector<uint32_t>{11,12,13}));assert(barrier.words[M+0x29c]==11 && barrier.words[M+0x2a8]==0 && barrier.words[E+0x2bc]==0 && !barrier.afterCalls);
 barrier.live.clear();barrier.after=1;assert(!battleUnitEventBarrier(B,bs) && barrier.afterCalls==1);barrier.after=0;barrier.words[B+0x182b4]=21;barrier.words[B+0x182c4]=22;barrier.words[B+0x180fc]=23;barrier.live.insert(22);barrier.eventQueries.clear();
 assert(!battleAllEventBarrier(B,bs));assert((barrier.eventQueries==std::vector<uint32_t>{21,22}));assert(barrier.words[B+0x182b4]==0 && barrier.words[B+0x180fc]==23);
 barrier.live.clear();assert(battleAllEventBarrier(B,bs)==1 && barrier.words[B+0x180fc]==0);
 std::cout<<"Source conclusion/status gates, end-result priority and ordered event barriers passed\n";
}
