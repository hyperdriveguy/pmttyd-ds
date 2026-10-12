#include "battle_actor.hpp"
#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <vector>
using namespace ttyd;
struct Fixture {
    std::map<uint32_t,uint32_t> words;std::map<uint32_t,uint8_t> bytes;
    std::vector<uint32_t> reads;std::function<void(uint32_t)> onRead;
    BattleActorServices services() {
        BattleActorServices s;s.user=this;s.memory.user=this;
        s.memory.word=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);if(f.onRead)f.onRead(a);return f.words[a];};
        s.memory.storeWord=[](void* p,uint32_t a,uint32_t v) {static_cast<Fixture*>(p)->words[a]=v;};
        s.byte=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);return f.bytes[a];};return s;
    }
    void unit(uint32_t b,int slot,uint32_t u,int32_t kind,uint8_t belong,uint32_t flags=0) {words[b+0x20+slot*4]=u;words[u+8]=uint32_t(kind);bytes[u+0xc]=belong;words[u+0x1c]=flags;}
};
int main() {
    constexpr uint32_t b=0x1000,other=0x2000,u=0x4000,v=0x5000;
    Fixture f;f.words[0x8041eb90]=b;EventEntry e;e.owner=-17;
    for(int32_t selector:{0,63,64,2147483647,-1,-2,-7,int32_t(-2147483647-1)}) {
        f.reads.clear();assert(translateBattleActor(selector==-2?&e:nullptr,selector,f.services())==(selector==-2?-17:selector));assert(f.reads==std::vector<uint32_t>{0x8041eb90});
    }
    f.words[b+0x420]=99;f.words[b+0x424]=0xffffffff;assert(translateBattleActor(nullptr,-6,f.services())==-1);
    for(int id:{-1,-2147483647-1,64,2147483647}) {f.reads.clear();assert(battleRegistryUnit(b,id,f.services())==0 && f.reads.empty());}
    setBattleRegistryUnit(b,-1,u,f.services());assert(f.words[b+0x1c]==u && battleRegistryUnit(b,-1,f.services())==0);setBattleRegistryUnit(b,64,v,f.services());assert(f.words[b+0x120]==0);
    f.unit(b,1,u,222,1);f.unit(b,2,v,222,0,8);f.words[v]=77;assert(battleMarioUnit(b,f.services())==v && translateBattleActor(nullptr,-3,f.services())==77);
    f.unit(b,0,0x6000,222,0);assert(battleMarioUnit(b,f.services())==0x6000); // Lowest admitted slot,not stored identity.
    Fixture party;party.unit(b,0,u,224,0,8);party.unit(b,1,v,230,0,0x100);party.words[v]=0xfffffffd;
    assert(battlePartyUnit(b,party.services())==v && party.reads==std::vector<uint32_t>({b+0x20,u+0x1c,b+0x24,v+0x1c,v+8,v+0xc}));
    party.words[0x8041eb90]=b;assert(translateBattleActor(nullptr,-4,party.services())==-3);
    for(int32_t kind:{223,231,-1}) {party.words[v+8]=uint32_t(kind);assert(battlePartyUnit(b,party.services())==0);}
    party.words[v+8]=224;party.bytes[v+0xc]=0xff;assert(battlePartyUnit(b,party.services())==0);party.bytes[v+0xc]=0;assert(battlePartyUnit(b,party.services())==v);
    Fixture system;system.unit(b,0,u,219,0);system.unit(b,1,v,220,3,8);system.words[v]=123;system.words[v+0x10]=0xffffffff;
    assert(battleSystemUnit(b,system.services())==v && system.reads==std::vector<uint32_t>({b+0x20,u+8,b+0x24,v+8}));system.words[0x8041eb90]=b;assert(translateBattleActor(nullptr,-5,system.services())==123);
    system.words[u+8]=220;assert(battleSystemUnit(b,system.services())==u);system.words[u+8]=221;system.words[v+8]=221;assert(battleSystemUnit(b,system.services())==0);
    Fixture absent;for(auto query:{battleMarioUnit,battlePartyUnit,battleSystemUnit}) {absent.reads.clear();assert(query(b,absent.services())==0 && absent.reads.size()==64 && absent.reads.front()==b+0x20 && absent.reads.back()==b+0x11c);}
    absent.words[0x8041eb90]=b;absent.words[0]=0x87654321;assert(translateBattleActor(nullptr,-4,absent.services())==-1);assert(uint32_t(translateBattleActor(nullptr,-3,absent.services()))==0x87654321 && absent.reads.back()==0);assert(uint32_t(translateBattleActor(nullptr,-5,absent.services()))==0x87654321 && absent.reads.back()==0); // Original dereferences,not missing-unit fallback.
    f.onRead=[&](uint32_t a) {if(a==b+0x20)f.words[0x8041eb90]=other;};f.words[0x6000]=55;
    assert(translateBattleActor(nullptr,-3,f.services())==55 && currentBattleAddress(f.services())==other); // Explicit captured B scan;later recapture independent.
    std::cout<<"battle actor translation and registry query tests passed\n";
}
