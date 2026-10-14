#include "battle_break_slot.hpp"
#include "event_script_assets.hpp"
#include <cassert>
#include <map>
#include <vector>
using namespace ttyd;
struct Fixture {
    static constexpr uint32_t B=0x10000,C=B+0x1616c;
    std::map<uint32_t,uint8_t> bytes;
    std::vector<uint32_t> allocations,frees;
    bool available=true,swap=false;uint32_t next=0x70000;
    uint32_t word(uint32_t a){uint32_t v=0;for(unsigned i=0;i<4;++i)v=v<<8|bytes[a+i];return v;}
    void word(uint32_t a,uint32_t v){for(unsigned i=0;i<4;++i)bytes[a+i]=uint8_t(v>>(24-8*i));}
    uint32_t reel(unsigned i){return C+0x18+i*0x5c;}
    Fixture() {
        word(0x8041eb90,B);EventScriptAssets source;assert(source.load("romfs/trio-attack-events.pmevt"));
        for(unsigned i=0;i<21;++i){uint32_t v;assert(source.readWord(0x802fe040+4*i,v));word(0x802fe040+4*i,v);}
    }
    BattleBreakSlotServices services() {
        BattleBreakSlotServices s;s.user=this;s.actors.user=this;s.actors.memory.user=this;
        s.actors.byte=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->bytes[a];};
        s.actors.memory.word=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->word(a);};
        s.actors.memory.storeWord=[](void* p,uint32_t a,uint32_t v){static_cast<Fixture*>(p)->word(a,v);};
        s.actors.memory.storeByte=[](void* p,uint32_t a,uint8_t v){static_cast<Fixture*>(p)->bytes[a]=v;};
        s.allocate=[](void* p,uint32_t size)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(p);f.allocations.push_back(size);
            if(!f.available)return {};const auto address=f.next;f.next+=size;
            if(f.swap)f.word(0x8041eb90,B+0x90000);return address;};
        s.free=[](void* p,uint32_t address){auto& f=*static_cast<Fixture*>(p);f.frees.push_back(address);if(f.swap)f.word(0x8041eb90,B+0x90000);return f.available;};return s;
    }
};
int main() {
    using F=Fixture;
    Fixture f;for(unsigned i=0;i<0x134;++i)f.bytes[F::C+i]=255;f.bytes[F::C-1]=77;f.bytes[F::C+0x134]=88;
    auto s=f.services();assert(initializePackedBattleBreakSlot(s));assert((f.allocations==std::vector<uint32_t>{32,32,20}));
    assert(f.bytes[F::C-1]==77 && f.bytes[F::C+0x134]==88 && f.word(F::C+4)==0);
    for(unsigned i=0;i<3;++i) {
        const auto r=f.reel(i),p=f.word(r+0x14);const unsigned count=i==2?5:8;
        assert(f.word(r)==i && f.word(r+8)==0 && f.word(r+0x18)==count);
        for(unsigned j=0;j<count;++j)assert(f.word(p+4*j)==f.word(0x802fe040+32*i+4*j));
        f.word(r+4,0x81);
    }
    const auto first=f.word(f.reel(0)+0x14);assert(packedBattleBreakSlotIdleMain(s.actors));
    for(unsigned i=0;i<3;++i){assert(f.word(f.reel(i)+4)==0x80 && f.bytes[f.reel(i)+0x58]==255);}
    assert(f.allocations.size()==3 && f.frees.empty() && f.word(f.reel(0)+0x14)==first);
    assert(endPackedBattleBreakSlot(s) && (f.frees==std::vector<uint32_t>{0x70000,0x70020,0x70040}));
    assert(f.word(f.reel(0)+0x14)==first && f.word(F::C+4)==0); // End does not null/reset owners.
    Fixture failing;failing.available=false;s=failing.services();assert(!initializePackedBattleBreakSlot(s) && failing.allocations.size()==1);
    Fixture changed;changed.swap=true;s=changed.services();assert(initializePackedBattleBreakSlot(s));
    assert(changed.word(changed.reel(0)+0x14)==0x70000 && changed.word(changed.reel(1)+0x14)==0);
    assert(changed.word(changed.reel(1)+0x90000+0x14)==0x70020); // Fresh getter, retained reel across allocation.
    Fixture states;s=states.services();assert(initializePackedBattleBreakSlot(s));states.word(states.reel(0)+4,1);states.word(states.reel(1)+8,2);
    assert(!packedBattleBreakSlotIdleMain(s.actors) && states.word(states.reel(0)+4)==0 && states.bytes[states.reel(0)+0x58]==255);
    states.word(states.reel(0)+8,2);states.word(states.reel(1)+0x10,7);states.word(F::C+8,99);
    assert(!packedBattleBreakSlotIdleMain(s.actors) && states.word(F::C+4)==1 && states.word(F::C+8)==0);
    Fixture ending;s=ending.services();ending.word(ending.reel(0)+0x14,123);ending.word(ending.reel(1)+0x90000+0x14,456);ending.word(ending.reel(2)+0x90000+0x14,789);ending.swap=true;
    assert(endPackedBattleBreakSlot(s) && (ending.frees==std::vector<uint32_t>{123,456,789}));
}
