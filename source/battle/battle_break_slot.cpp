#include "battle_break_slot.hpp"
#include <array>
#include <cassert>
namespace ttyd {
namespace {
uint32_t controller(const BattleActorServices& s){return currentBattleAddress(s)+0x1616c;}
uint32_t reel(unsigned index,const BattleActorServices& s){return controller(s)+0x18+index*0x5c;}
}
bool initializePackedBattleBreakSlot(const BattleBreakSlotServices& s) {
    const auto& m=s.actors.memory;assert(m.word && m.storeWord && m.storeByte);
    const auto c=controller(s.actors);
    std::array<std::array<uint32_t,8>,3> words{};
    constexpr std::array<uint32_t,3> addresses{0x802fe040,0x802fe060,0x802fe080};
    constexpr std::array<unsigned,3> counts{8,8,5};
    for(unsigned i=0;i<3;++i)for(unsigned j=0;j<counts[i];++j)words[i][j]=m.word(m.user,addresses[i]+4*j);
    eventFillMemory(c,0,0x134,m);m.storeWord(m.user,c+4,0);
    auto r=reel(0,s.actors);
    for(unsigned i=0;i<3;++i,r+=0x5c){m.storeWord(m.user,r,i);m.storeWord(m.user,r+8,0);}
    for(unsigned i=0;i<3;++i) {
        r=reel(i,s.actors);m.storeWord(m.user,r+0x18,counts[i]);
        if(!s.allocate)return false;
        const auto allocation=s.allocate(s.user,4*counts[i]);if(!allocation)return false;
        m.storeWord(m.user,r+0x14,*allocation);
        if(!*allocation)return false; // Source memcpy through null is an unresolved domain.
        for(unsigned j=0;j<counts[i];++j)m.storeWord(m.user,*allocation+4*j,words[i][j]);
    }
    return true;
}
bool packedBattleBreakSlotIdleMain(const BattleActorServices& s) {
    const auto& m=s.memory;assert(m.word && m.storeWord && m.storeByte);
    const auto b=currentBattleAddress(s);(void)currentBattleAddress(s);
    const auto c=controller(s);
    (void)battleMarioUnit(b,s);(void)battlePartyUnit(b,s);
    if(m.word(m.user,c+4)!=0)return false;
    auto r=reel(0,s);unsigned stopped=0;
    for(unsigned i=0;i<3;++i)if(m.word(m.user,r+i*0x5c+8)==2)++stopped;
    if(stopped==2 && m.word(m.user,r+0x5c+0x10)!=m.word(m.user,r+0x10)) {
        m.storeWord(m.user,c+4,1);m.storeWord(m.user,c+8,0);
    }
    r=reel(0,s);
    for(unsigned i=0;i<3;++i,r+=0x5c) {
        if(m.word(m.user,r+8)!=0)return false;
        m.storeWord(m.user,r+4,m.word(m.user,r+4)&~1u);m.storeByte(m.user,r+0x58,255);
    }
    return true;
}
bool endPackedBattleBreakSlot(const BattleBreakSlotServices& s) {
    for(unsigned i=0;i<3;++i) {
        const auto r=reel(i,s.actors),p=s.actors.memory.word(s.actors.memory.user,r+0x14);
        if(!s.free || !s.free(s.user,p))return false;
    }
    return true;
}
}
