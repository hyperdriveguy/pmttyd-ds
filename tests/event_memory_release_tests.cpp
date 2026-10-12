#include "event_memory_release.hpp"
#include "event_heap_allocation.hpp"
#include "event_memory_bootstrap.hpp"
#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
struct Op {char kind;uint32_t a,v;bool operator==(const Op& o) const {return kind==o.kind && a==o.a && v==o.v;}};
struct Fixture {
    std::map<uint32_t,uint32_t> words;
    std::vector<Op> operations;
    unsigned flushes=0,barriers=0;
    std::function<void(uint32_t,uint32_t)> onWrite;
    uint32_t read(uint32_t a) {auto i=words.find(a);return i==words.end()?0:i->second;}
    void map(uint32_t h,uint32_t next,uint32_t size,uint16_t used) {words[h]=next;words[h+4]=size;words[h+8]=uint32_t(used)<<16;}
    void sdk(uint32_t h,uint32_t prev,uint32_t next,uint32_t size) {words[h]=prev;words[h+4]=next;words[h+8]=size;}
    EventMemoryServices services() {
        EventMemoryServices s;s.user=this;
        s.word=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);const auto v=f.read(a);f.operations.push_back({'r',a,v});return v;};
        s.half=[](void* p,uint32_t a)->uint16_t {auto& f=*static_cast<Fixture*>(p);const uint16_t v=uint16_t(f.read(a&~3u)>>(16-8*(a&2)));f.operations.push_back({'h',a,v});return v;};
        s.storeWord=[](void* p,uint32_t a,uint32_t v) {auto& f=*static_cast<Fixture*>(p);if(f.operations.size()<1000)f.operations.push_back({'w',a,v});if(v)f.words[a]=v;else f.words.erase(a);if(f.onWrite)f.onWrite(a,v);};
        s.storeHalf=[](void* p,uint32_t a,uint16_t v) {auto& f=*static_cast<Fixture*>(p);f.operations.push_back({'s',a,v});const unsigned shift=16-8*(a&2);f.words[a&~3u]=(f.read(a&~3u)&~(0xffffu<<shift))|(uint32_t(v)<<shift);};
        s.storeByte=[](void* p,uint32_t a,uint8_t v) {auto& f=*static_cast<Fixture*>(p);const unsigned shift=24-8*(a&3);f.words[a&~3u]=(f.read(a&~3u)&~(0xffu<<shift))|(uint32_t(v)<<shift);};
        s.flushBlock=[](void* p,uint32_t) {++static_cast<Fixture*>(p)->flushes;};s.systemCall=[](void* p) {++static_cast<Fixture*>(p)->barriers;};return s;
    }
};
}
int main() {
    Fixture null;eventMapFree(0,0,null.services());assert(null.operations.empty());null.map(0x1000,0,32,0);eventMapFree(0,0x1020,null.services());assert(null.operations==std::vector<Op>({{'h',0x1008,0}}));
    Fixture both;both.map(0x1000,0x1040,32,0);both.map(0x1040,0x1080,32,7);both.map(0x1080,0x10c0,32,0);both.map(0x10c0,0,32,0);
    eventMapFree(0x1000,0x1060,both.services());assert(both.read(0x1000)==0x10c0 && both.read(0x1004)==160 && both.read(0x1008)==0 && both.read(0x1048)==0x70000); // One forward,then backward;removed header stays used.
    assert(both.operations[both.operations.size()-3]==(Op{'w',0x1000,0x10c0}) && both.operations[both.operations.size()-2]==(Op{'w',0x1004,160}) && both.operations.back()==(Op{'s',0x1008,0}) && both.flushes==0);
    Fixture usedPrev;usedPrev.map(0x1000,0x1040,32,1);usedPrev.map(0x1040,0,32,1);eventMapFree(0x1000,0x1060,usedPrev.services());assert(usedPrev.read(0x1048)==0 && usedPrev.read(0x1008)==0x10000);
    Fixture noPrev;noPrev.map(0x2000,0,64,0);noPrev.map(0x1000,0,32,1);eventMapFree(0x2000,0x1020,noPrev.services());assert(noPrev.read(0x1008)==0 && noPrev.read(0x1004)==32);
    Fixture allocation;allocation.map(0x1000,0,128,0);const uint32_t payload=eventMapAllocate(0x1000,32,allocation.services());assert(payload==0x1020);eventMapFree(0x1000,payload,allocation.services());assert(allocation.read(0x1000)==0 && allocation.read(0x1004)==128 && allocation.read(0x1008)==0);
    Fixture sdkBoth;sdkBoth.sdk(0x1000,0,0x10c0,64);sdkBoth.sdk(0x1040,99,99,128);sdkBoth.sdk(0x10c0,0x1000,0x1100,64);sdkBoth.sdk(0x1100,0x10c0,0,64);
    assert(eventSdkInsertFree(0x1000,0x1040,sdkBoth.services())==0x1000 && sdkBoth.read(0x1008)==256 && sdkBoth.read(0x1004)==0x1100 && sdkBoth.read(0x1100)==0x1000 && sdkBoth.read(0x1048)==192);
    Fixture sdkHead;sdkHead.sdk(0x1080,0,0,64);sdkHead.sdk(0x1000,99,99,64);assert(eventSdkInsertFree(0x1080,0x1000,sdkHead.services())==0x1000 && sdkHead.read(0x1004)==0x1080 && sdkHead.read(0x1008)==64 && sdkHead.read(0x1080)==0x1000);
    Fixture empty;empty.sdk(0x1000,99,99,64);assert(eventSdkInsertFree(0,0x1000,empty.services())==0x1000 && empty.read(0x1000)==0 && empty.read(0x1004)==0);
    Fixture release;release.words[0x8041f070]=0x8000;release.words[0x8008]=0x2000;release.words[0x8004]=0x1000;release.sdk(0x1000,0,0x1040,64);release.sdk(0x1040,0x1000,0,64);release.sdk(0x2000,0,0x2080,64);release.sdk(0x2080,0x2000,0,64);
    eventSdkFree(0,0x2020,release.services());assert(release.read(0x8008)==0x2080 && release.read(0x2080)==0 && release.read(0x8004)==0x1000 && release.read(0x1044)==0x2000 && release.read(0x2000)==0x1040 && release.read(0x2004)==0);
    eventSdkFree(0,0x20a0,release.services());assert(release.read(0x8008)==0 && release.read(0x2008)==64); // Nonadjacent nodes do not merge.
    Fixture sdkRound;sdkRound.words[0x8041f070]=0x8000;sdkRound.words[0x8041e844]=0;sdkRound.words[0x8004]=0x1000;sdkRound.sdk(0x1000,0,0,256);
    auto one=eventHeapAllocate(0,1,sdkRound.services()),two=eventHeapAllocate(0,1,sdkRound.services());assert(one==0x1020 && two==0x1060);const auto before=sdkRound.flushes;eventHeapFree(0,one,sdkRound.services());eventHeapFree(0,two,sdkRound.services());assert(sdkRound.read(0x8004)==0x1000 && sdkRound.read(0x1008)==256 && sdkRound.read(0x8008)==0 && sdkRound.flushes==before);
    Fixture sdkNull;sdkNull.words[0x8041f070]=0x8000;eventHeapFree(0,0,sdkNull.services());assert(sdkNull.operations.size()>1 && sdkNull.operations[1].a==0x8041f070 && sdkNull.operations[2].a==0xffffffe4 && sdkNull.read(0x8004)==0xffffffe0); // No inferred SDK null guard.
    Fixture battle;battle.words[0x8041e808]=0x1000;battle.map(0x1000,0,0x80000,0);allocateEventBattleMapPool(battle.services());const auto root=battle.read(0x8041e80c);
    assert(root==0x1020 && battle.read(0x8041e814)==0x64000 && battle.read(root)==0 && battle.read(root+4)==0x63fe0 && battle.read(root+8)==0 && battle.flushes==0x64000/32 && battle.barriers==1);
    const auto inner=eventMapAllocate(root,32,battle.services());assert(inner==root+32);freeEventBattleMapPool(battle.services());assert(battle.read(0x8041e80c)==0 && battle.read(0x8041e814)==0x64000 && battle.read(0x1004)==0x80000 && battle.read(root+8)==0x10000); // Inner state is not walked/cleared.
    Fixture failed;failed.words[0x8041e808]=0x1000;failed.map(0x1000,0,32,0);allocateEventBattleMapPool(failed.services());assert(failed.read(0x8041e80c)==0 && failed.read(4)==0x63fe0 && failed.flushes==0);freeEventBattleMapPool(failed.services());assert(failed.read(0x8041e80c)==0 && failed.read(0x8041e814)==0x64000);
    // Compose original bootstrap,SDK context payloads,nested battle allocation
    // and both release families. Each returns to its own initial free extent.
    Fixture cycle;cycle.words[0x8041e430]=0x100000;cycle.words[0x8041f080]=0x1000000;
    const std::vector<uint32_t> sizes{7300,3472,224,128,100};
    for(uint32_t i=0;i<5;++i) {cycle.words[0x804151b8+8*i]=i==4?0:1;cycle.words[0x804151bc+8*i]=sizes[i];}
    initializeEventMemoryPools(cycle.services());const uint32_t primary=cycle.read(0x8041e808),primarySize=cycle.read(primary+4);
    const uint32_t array0=eventHeapAllocate(0,0x1b000,cycle.services()),array1=eventHeapAllocate(0,0x1b000,cycle.services());assert(array0!=0 && array1!=0 && array0!=array1);
    allocateEventBattleMapPool(cycle.services());const uint32_t nested=cycle.read(0x8041e80c),block=eventMapAllocate(nested,32,cycle.services());eventMapFree(nested,block,cycle.services());freeEventBattleMapPool(cycle.services());
    assert(cycle.read(primary+4)==primarySize && cycle.read(0x8041e80c)==0);
    eventHeapFree(0,array0,cycle.services());eventHeapFree(0,array1,cycle.services());
    const uint32_t descriptor=cycle.read(0x8041f070),pool0=cycle.read(0x8041e830);
    assert(cycle.read(descriptor+4)==pool0 && cycle.read(pool0+8)==7300*1024 && cycle.read(descriptor+8)==0 && cycle.read(descriptor+20)!=0); // Pool1 SDK reservation remains owned.
    std::cout<<"event memory release tests passed\n";
}
