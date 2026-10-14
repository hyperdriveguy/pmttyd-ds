#include "battle_return_sequence.hpp"
#include <cassert>
#include <map>
#include <vector>
#include <array>
using namespace ttyd;
struct Fixture {
    static constexpr uint32_t B=0x1000,G=0x5000,H=0x7000,W=0x9000;
    std::map<uint32_t,uint32_t> words{{0x8041eb90,B},{0x80415fd0,G}};
    std::vector<std::array<uint32_t,3>> transitions,fades;
    unsigned tails=0,events=0;
    bool mutateFade=false;
    EventMemoryServices memory() {
        EventMemoryServices m;m.user=this;
        m.word=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->words[a];};
        m.storeWord=[](void* p,uint32_t a,uint32_t v){static_cast<Fixture*>(p)->words[a]=v;};return m;
    }
    BattleReturnSequenceServices sequence() {
        BattleReturnSequenceServices s;s.actors.memory=memory();s.user=this;
        s.tail=[](void* p){++static_cast<Fixture*>(p)->tails;};
        s.setSequence=[](void* p,int32_t seq,int32_t param,uint32_t data){auto& f=*static_cast<Fixture*>(p);assert(f.words[W+0x10]==31);f.transitions.push_back({uint32_t(seq),uint32_t(param),data});};return s;
    }
    FieldSequenceFadeServices fade() {
        FieldSequenceFadeServices s;s.memory=memory();s.user=this;
        s.fade=[](void* p,uint32_t t,uint32_t ms,uint32_t rgba){auto& f=*static_cast<Fixture*>(p);f.fades.push_back({t,ms,rgba});if(f.mutateFade)f.words[W+4]=6;};
        s.event=[](void* p,uint32_t script,int32_t type,int32_t priority,uint32_t flags){auto& f=*static_cast<Fixture*>(p);assert(script==0x80414f28 && type==0 && priority==0 && flags==0 && f.fades.size()==1);++f.events;f.words[0x80415fd0]=H;f.words[H]=0x81f;};return s;
    }
};
int main() {
    using F=Fixture;
    Fixture f;f.words[F::W+4]=5;f.words[F::W+0x10]=77;auto s=f.sequence();
    assert(packedBattleReturnSequence(F::W,s) && f.words[F::W+4]==5 && f.words[F::W+0x10]==77 && f.tails==1);
    f.words[F::B+0xef4]=0x80000000;
    assert(packedBattleReturnSequence(F::W,s) && f.words[F::W+4]==6 && f.words[F::W+0x10]==0 && f.tails==2);
    for(unsigned i=1;i<=30;++i)assert(packedBattleReturnSequence(F::W,s) && f.words[F::W+0x10]==i && f.transitions.empty() && f.tails==2);
    assert(packedBattleReturnSequence(F::W,s) && (f.transitions==std::vector<std::array<uint32_t,3>>{{2,1,0}}));
    Fixture alternate;alternate.words[F::W+4]=5;alternate.words[F::G+0xc]=1;alternate.words[F::B+0xef4]=0x80000000;s=alternate.sequence();
    assert(packedBattleReturnSequence(F::W,s) && alternate.words[F::W+4]==11 && alternate.words[F::W+0x10]==0 && alternate.tails==1);
    assert(!packedBattleReturnSequence(F::W,s) && alternate.tails==1);
    for(uint32_t flags:{0u,0x1000u,0x2000u,0x3000u}) {
        Fixture a;a.words[F::W+4]=6;a.words[F::W+0x10]=30;a.words[F::G]=flags;s=a.sequence();
        assert(packedBattleReturnSequence(F::W,s) && a.transitions[0][0]==(flags==0x3000?5u:2u) && a.tails==0);
    }
    for(uint32_t old:{0x7fffffffu,0xffffffffu}) {
        Fixture a;a.words[F::W+4]=6;a.words[F::W+0x10]=old;s=a.sequence();
        assert(packedBattleReturnSequence(F::W,s) && a.words[F::W+0x10]==old+uint32_t(1) && a.transitions.empty());
    }
    for(uint32_t select:{0u,1u}) {
        Fixture a;a.words[F::G+0x118]=select;a.words[F::G+0xf8]=8;a.words[F::G+0xfc]=201;a.words[F::G+0x108]=12;a.words[F::G+0x10c]=450;
        auto fs=a.fade();assert(packedFieldSequenceIncomingFade(F::W,fs));
        assert((a.fades[0]==std::array<uint32_t,3>{select?12u:8u,select?450u:201u,0xff}));
        assert(a.words[F::W+4]==1 && a.words[F::G+0xf8]==9 && a.words[F::G+0xfc]==300 && a.words[F::G+0x108]==9 && a.words[F::G+0x10c]==300);
        assert(!packedFieldSequenceIncomingFade(F::W,fs) && a.fades.size()==1 && a.events==0);
    }
    Fixture special;special.words[F::G]=0x10;special.mutateFade=true;auto fs=special.fade();
    assert(packedFieldSequenceIncomingFade(F::W,fs) && (special.fades[0]==std::array<uint32_t,3>{18,900,0xff}));
    assert(special.events==1 && special.words[F::G]==0x10 && special.words[F::H]==0x80f && special.words[F::W+4]==7);
    assert(special.words[F::H+0xf8]==9 && special.words[F::G+0xf8]==0 && special.words[F::H+0x10c]==300);
}
