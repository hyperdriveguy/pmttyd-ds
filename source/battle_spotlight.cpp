#include "battle_spotlight.hpp"
#include <cassert>
#include <cstring>
namespace ttyd {
namespace {
constexpr uint32_t Table=0x802f37a8;
float number(uint32_t v){float f;std::memcpy(&f,&v,4);return f;}
uint32_t bits(float f){uint32_t v;std::memcpy(&v,&f,4);return v;}
void triple(uint32_t from,uint32_t to,const EventMemoryServices& m) {
    const auto x=m.word(m.user,from),y=m.word(m.user,from+4);
    m.storeWord(m.user,to,x);m.storeWord(m.user,to+4,y);
    m.storeWord(m.user,to+8,m.word(m.user,from+8));
}
}
uint32_t packedBattleSpotlightEntry(const BattleSpotlightServices& s) {
    const auto& m=s.memory;assert(m.word && m.half && m.storeWord && m.storeHalf);
    const auto w=m.word(m.user,0x804175a0),count=m.word(m.user,w);
    auto record=m.word(m.user,w+4);
    if(count && count<0x80000000)
        for(uint32_t i=0;i<count;++i) {if(!(m.half(m.user,record)&1))break;record+=0x2c;}
    triple(Table+0x54,record+4,m);
    triple(Table+0x60,record+0x1c,m);
    triple(Table+0x6c,record+0x10,m);
    m.storeWord(m.user,record+0x28,m.word(m.user,0x804295d4));
    m.storeHalf(m.user,record,m.half(m.user,record)|1);
    return record;
}
bool initializePackedBattleSpotlight(const BattleSpotlightServices& s) {
    const auto& m=s.memory;assert(m.word && m.storeWord && s.fma);
    auto owner=[&](){return m.word(m.user,0x804175a0);};
    m.storeWord(m.user,owner(),5);
    const auto count=m.word(m.user,owner());
    if(!s.allocate)return false;
    const auto allocation=s.allocate(s.user,count*0x2c);if(!allocation)return false;
    m.storeWord(m.user,owner()+4,*allocation);
    const auto w=owner(),extent=m.word(m.user,w),bank=m.word(m.user,w+4);
    eventFillMemory(bank,0,extent*0x2c,m);
    const auto step=number(m.word(m.user,0x80422b1c)),offset=number(m.word(m.user,0x80422b18));
    for(unsigned i=0;i<5;++i) {
        const auto r=packedBattleSpotlightEntry(s);
        // Original position loads all three before any position override store.
        const auto x=m.word(m.user,Table+0x30),y=m.word(m.user,Table+0x34),z=m.word(m.user,Table+0x38);
        (void)x;
        m.storeWord(m.user,r+4,bits(s.fma(s.user,step,float(i),offset)));
        m.storeWord(m.user,r+8,y);m.storeWord(m.user,r+0xc,z);
        triple(Table+0x3c,r+0x10,m);triple(Table+0x48,r+0x1c,m);
        m.storeWord(m.user,r+0x28,m.word(m.user,0x80422ad4));
    }
    return true;
}
bool packedBattleSpotlightMain(const BattleSpotlightServices& s) {
    const auto& m=s.memory;assert(m.word);
    const auto w=m.word(m.user,0x804175a0);
    (void)m.word(m.user,w); // Source positive-count loop has no observable work.
    const auto gate=m.word(m.user,w+8);
    if(!gate || gate>=0x80000000)return true;
    const float priority=number(m.word(m.user,0x80422b14));
    return s.display && s.display(s.user,6,0,0x80137d20,0,priority);
}
bool freePackedBattleSpotlight(const BattleSpotlightServices& s) {
    const auto& m=s.memory;assert(m.word);
    const auto w=m.word(m.user,0x804175a0),allocation=m.word(m.user,w+4);
    return s.free && s.free(s.user,allocation); // Source retains all Work fields.
}
}
