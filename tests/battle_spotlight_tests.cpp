#include "battle_spotlight.hpp"
#include "battle_action_help.hpp"
#include "event_memory_release.hpp"
#include "game.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <map>
#include <vector>
using namespace ttyd;
struct Fixture {
    static constexpr uint32_t W=0x10000,Bank=0x20020,Table=0x802f37a8;
    std::map<uint32_t,uint8_t> bytes;
    std::vector<std::pair<uint32_t,uint32_t>> writes;
    unsigned allocations=0,frees=0,displays=0;
    bool mutate=false;
    uint32_t word(uint32_t a){uint32_t v=0;for(unsigned i=0;i<4;++i)v=(v<<8)|bytes[a+i];return v;}
    void word(uint32_t a,uint32_t v){writes.emplace_back(a,v);for(unsigned i=0;i<4;++i)bytes[a+i]=v>>(24-8*i);}
    uint16_t half(uint32_t a){return uint16_t(bytes[a])<<8|bytes[a+1];}
    void half(uint32_t a,uint16_t v){bytes[a]=v>>8;bytes[a+1]=v;}
    void real(uint32_t a,float f){uint32_t v;std::memcpy(&v,&f,4);word(a,v);}
    float real(uint32_t a){const auto v=word(a);float f;std::memcpy(&f,&v,4);return f;}
    Fixture() {
        word(0x804175a0,W);word(W+8,0xffffffff);
        word(0x804295d4,0);word(0x80422ad4,0xffffffff);
        real(0x80422b1c,50);real(0x80422b18,-100);real(0x80422b14,-1000);
        for(unsigned i=0;i<9;++i)real(Table+0x54+4*i,100+i);
        for(unsigned i=0;i<3;++i){real(Table+0x30+4*i,i==2?-50:0);real(Table+0x3c+4*i,0);real(Table+0x48+4*i,i==2?150:20);}
        word(0x20000,0);word(0x20004,0x1000);half(0x20008,0);
    }
    BattleSpotlightServices services() {
        BattleSpotlightServices s;s.user=this;s.memory.user=this;
        s.memory.word=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->word(a);};
        s.memory.half=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->half(a);};
        s.memory.storeWord=[](void* p,uint32_t a,uint32_t v){static_cast<Fixture*>(p)->word(a,v);};
        s.memory.storeHalf=[](void* p,uint32_t a,uint16_t v){static_cast<Fixture*>(p)->half(a,v);};
        s.memory.storeByte=[](void* p,uint32_t a,uint8_t v){static_cast<Fixture*>(p)->bytes[a]=v;};
        s.memory.flushBlock=[](void*,uint32_t){};s.memory.systemCall=[](void*){};
        s.allocate=[](void* p,uint32_t size)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(p);assert(size==0xdc && f.word(W)==5);++f.allocations;
            const auto a=eventMapAllocate(0x20000,size,f.services().memory);
            if(f.mutate){f.word(0x804175a0,W+0x100);f.word(W+0x100,5);f.word(W+0x108,7);}
            return a;};
        s.free=[](void* p,uint32_t a){auto& f=*static_cast<Fixture*>(p);++f.frees;eventMapFree(0x20000,a,f.services().memory);return true;};
        s.fma=[](void*,float a,float b,float c){return std::fma(a,b,c);};
        s.display=[](void* p,int32_t camera,int32_t cls,uint32_t callback,uint32_t arg,float priority){auto& f=*static_cast<Fixture*>(p);++f.displays;
            assert(camera==6 && cls==0 && callback==0x80137d20 && arg==0 && priority==-1000);return true;};
        return s;
    }
};
int main() {
    Fixture f;auto s=f.services();assert(initializePackedBattleSpotlight(s));
    assert(f.allocations==1 && f.word(Fixture::W)==5 && f.word(Fixture::W+4)==Fixture::Bank && f.word(Fixture::W+8)==0xffffffff);
    for(unsigned i=0;i<5;++i) {
        const auto r=Fixture::Bank+0x2c*i;
        assert(f.half(r)==1 && f.half(r+2)==0);
        assert(f.real(r+4)==-100+50*int(i) && f.real(r+8)==0 && f.real(r+12)==-50);
        assert(f.real(r+0x10)==0 && f.real(r+0x14)==0 && f.real(r+0x18)==0);
        assert(f.real(r+0x1c)==20 && f.real(r+0x20)==20 && f.real(r+0x24)==150 && f.word(r+0x28)==0xffffffff);
        std::vector<uint32_t> colors;for(const auto& write:f.writes)if(write.first==r+0x28)colors.push_back(write.second);
        assert(colors.size()>=2 && colors[colors.size()-2]==0 && colors.back()==0xffffffff);
    }
    assert(packedBattleSpotlightMain(s) && f.displays==0);
    f.word(Fixture::W+8,0);assert(packedBattleSpotlightMain(s) && f.displays==0);
    f.word(Fixture::W+8,1);assert(packedBattleSpotlightMain(s) && f.displays==1);
    s.display=nullptr;assert(!packedBattleSpotlightMain(s));
    f.word(Fixture::W,0x80000000);s=f.services();assert(packedBattleSpotlightMain(s) && f.displays==2); // Count is not the display gate.
    f.word(Fixture::W,5);assert(freePackedBattleSpotlight(s) && f.frees==1);
    assert(f.word(Fixture::W+4)==Fixture::Bank && f.word(Fixture::W+8)==1 && f.word(Fixture::W)==5);
    assert(f.half(0x20008)==0 && f.word(0x20004)==0x1000);
    assert(initializePackedBattleSpotlight(s) && f.allocations==2 && f.word(Fixture::W+8)==1);
    f.half(Fixture::Bank+0x2c,0x80);f.word(0x804295d4,0x12345678);
    assert(packedBattleSpotlightEntry(s)==Fixture::Bank+0x2c && f.half(Fixture::Bank+0x2c)==0x81);
    assert(f.real(Fixture::Bank+0x2c+4)==100 && f.real(Fixture::Bank+0x2c+0x1c)==103 && f.real(Fixture::Bank+0x2c+0x10)==106);
    assert(f.word(Fixture::Bank+0x2c+0x28)==0x12345678);
    Fixture recaptured;recaptured.mutate=true;s=recaptured.services();assert(initializePackedBattleSpotlight(s));
    assert(recaptured.word(Fixture::W+4)==0 && recaptured.word(Fixture::W+0x104)==Fixture::Bank && recaptured.word(Fixture::W+0x108)==7);
    Fixture failure;s=failure.services();s.allocate=nullptr;assert(!initializePackedBattleSpotlight(s) && failure.word(Fixture::W)==5 && failure.word(Fixture::W+4)==0);
    s=failure.services();s.free=nullptr;assert(!freePackedBattleSpotlight(s));
    Game game;auto owner=game.battleSpotlight;owner->work={5,0x1234,1};owner->entryColor=0x12345678;
    const auto saved=game.progress;game.restore(saved);
    assert(game.battleSpotlight==owner && owner->work[2]==1 && owner->entryColor==0x12345678);
    Game independent;assert(independent.battleSpotlight!=owner && independent.battleSpotlight->work[2]==0);
    Fixture help;help.word(0x8041eb90,0x30000);help.real(0x804272b8,498);
    BattleActionHelpServices h;h.actors.memory=help.services().memory;h.user=&help;
    h.display=[](void* p,int32_t camera,int32_t cls,uint32_t cb,uint32_t arg,float priority){++static_cast<Fixture*>(p)->displays;assert(camera==8 && cls==1 && cb==0x80216728 && arg==0 && priority==498);return true;};
    assert(packedBattleActionHelpMain(h) && help.displays==0);
    help.word(0x30000+0x19008,123);assert(packedBattleActionHelpMain(h) && help.displays==0);
    help.word(0x30000+0x1900c,0xffffffff);assert(packedBattleActionHelpMain(h) && help.displays==1);
    help.word(0x30000+0xef4,2);assert(packedBattleActionHelpMain(h) && help.displays==1);
    help.word(0x30000+0xef4,0);h.display=nullptr;assert(!packedBattleActionHelpMain(h));
}
