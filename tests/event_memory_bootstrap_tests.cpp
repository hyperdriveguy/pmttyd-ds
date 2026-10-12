#include "event_memory_bootstrap.hpp"
#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
struct Write {uint32_t a,v;bool operator==(const Write& w) const {return a==w.a && v==w.v;}};
struct Fixture {
    std::map<uint32_t,uint32_t> words;
    std::vector<Write> writes;
    uint32_t flushes=0,barriers=0,byteWrites=0;
    std::function<void(uint32_t,uint32_t)> onWrite;
    uint32_t read(uint32_t a) {auto it=words.find(a);return it==words.end()?0:it->second;}
    EventMemoryServices services() {
        EventMemoryServices s;s.user=this;s.word=[](void* p,uint32_t a) {return static_cast<Fixture*>(p)->read(a);};
        s.storeWord=[](void* p,uint32_t a,uint32_t v) {auto& f=*static_cast<Fixture*>(p);if(f.writes.size()<1000)f.writes.push_back({a,v});if(v)f.words[a]=v;else f.words.erase(a);if(f.onWrite)f.onWrite(a,v);};
        s.storeHalf=[](void* p,uint32_t a,uint16_t v) {auto& f=*static_cast<Fixture*>(p);const uint32_t old=f.read(a&~3u),shift=(2-(a&2))*8;const uint32_t updated=(old&~(0xffffu<<shift))|(uint32_t(v)<<shift);f.words[a&~3u]=updated;};
        s.storeByte=[](void* p,uint32_t a,uint8_t v) {auto& f=*static_cast<Fixture*>(p);++f.byteWrites;const uint32_t old=f.read(a&~3u),shift=(3-(a&3))*8;f.words[a&~3u]=(old&~(0xffu<<shift))|(uint32_t(v)<<shift);};
        s.flushBlock=[](void* p,uint32_t) {++static_cast<Fixture*>(p)->flushes;};s.systemCall=[](void* p) {++static_cast<Fixture*>(p)->barriers;};return s;
    }
    void config(uint32_t low,uint32_t high,const std::vector<std::pair<uint32_t,uint32_t>>& pairs) {
        words[0x8041e430]=low;words[0x8041f080]=high;
        for(uint32_t i=0;i<5;++i) {words[0x804151b8+i*8]=pairs[i].first;words[0x804151bc+i*8]=pairs[i].second;}
    }
};
}
int main() {
    Fixture init;assert(eventSdkInitializeAllocator(0x1003,0x300f,2,init.services())==0x1020);
    const std::vector<Write> expected{{0x8041f070,0x1003},{0x8041f074,2},{0x1003,0xffffffff},{0x100b,0},{0x1007,0},{0x100f,0xffffffff},{0x1017,0},{0x1013,0},{0x8041f07c,0x3000},{0x8041e428,0xffffffff},{0x8041f078,0x1020}};
    assert(init.writes==expected && init.read(0x8041f074)==2);
    Fixture negative;assert(eventSdkInitializeAllocator(0x1000,0x3000,-1,negative.services())==0x1000 && negative.writes.size()==5);
    Fixture live;live.onWrite=[&](uint32_t a,uint32_t v) {if(a==0x1000 && v==0xffffffffu)live.words[0x8041f074]=1;};
    assert(eventSdkInitializeAllocator(0x1000,0x3000,5,live.services())==0x1040 && live.read(0x100c)==0 && live.read(0x8041f074)==1);
    Fixture create;eventSdkInitializeAllocator(0x1000,0x8000,2,create.services());create.writes.clear();assert(eventSdkCreateHeap(0x2003,0x210f,create.services())==0);
    assert(create.writes==std::vector<Write>({{0x1000,0xe0},{0x2020,0},{0x2024,0},{0x2028,0xe0},{0x1004,0x2020},{0x1008,0}}));
    assert(eventSdkCreateHeap(0x3000,0x3100,create.services())==1 && eventSdkCreateHeap(0x4000,0x4100,create.services())==-1);
    create.words[0x8041e844]=0;create.writes.clear();eventSdkDestroyHeap(0,create.services());assert(create.writes==std::vector<Write>({{0x1000,0xffffffff},{0x1008,0},{0x1004,0}}) && create.read(0x8041e844)==0);
    assert(eventSdkCreateHeap(0x5000,0x4ff0,create.services())==0 && create.read(0x1000)==0xffffffe0);
    assert(eventSdkCreateHeap(0x6000,0x6100,create.services())==0); // Negative extent remains available.
    Fixture pools;pools.config(0x100000,0x1000000,{{1,7300},{1,3472},{1,224},{1,128},{0,100}});pools.words[0x8041e80c]=0xdeadbeef;
    initializeEventMemoryPools(pools.services());
    assert(pools.read(0x8041f070)==0x100000 && pools.read(0x8041f074)==5 && pools.read(0x8041f078)==0x100040 && pools.read(0x8041e430)==0x1000000);
    uint32_t p=0x100040;for(uint32_t i=0;i<4;++i) {const uint32_t span=std::vector<uint32_t>{7300,3472,224,128}[i]*1024;assert(pools.read(0x8041e830+4*i)==p && pools.read(0x8041e81c+4*i)==p+span && pools.read(0x8041e844+4*i)==i);p+=span;}
    assert(pools.read(0x8041e840)==p && pools.read(0x8041e82c)==0x1000000 && pools.read(0x8041e854)==4);
    const uint32_t root=pools.read(0x8041e808),mapSpan=3472*1024;
    assert(root==pools.read(0x8041e834)+32 && pools.read(0x8041e818)==mapSpan-32 && pools.read(root)==0 && pools.read(root+4)==mapSpan-64 && pools.read(root+8)==0 && pools.read(0x8041e80c)==0xdeadbeef);
    assert(pools.flushes==(mapSpan-32)/32 && pools.barriers==1);
    const uint32_t oldRoot=root,oldLength=pools.read(0x8041e818),oldHandle=pools.read(0x8041e84c);
    pools.words[0x100000]=0xffffffff;clearEventMemoryPool(2,pools.services());
    assert(pools.read(0x8041e84c)==oldHandle && pools.read(0x100000)==224*1024 && pools.read(0x100018)==0xffffffff && pools.read(0x8041e808)==oldRoot && pools.read(0x8041e818)==oldLength); // Replacement handle ignored.
    // Fixed pool1 is assigned before two percentages,even when percentage0
    // appears first. Both use the same remaining span,not a shrinking remainder.
    Fixture percentages;percentages.config(0x1000,0x9000,{{0,50},{1,1},{0,50},{1,1},{1,1}});initializeEventMemoryPools(percentages.services());
    assert(percentages.read(0x8041e834)==0x1040 && percentages.read(0x8041e83c)==0x1440 && percentages.read(0x8041e840)==0x1840);
    assert(percentages.read(0x8041e830)==0x1c40 && percentages.read(0x8041e81c)==0x5620 && percentages.read(0x8041e838)==0x5620 && percentages.read(0x8041e824)==0x9000);
    // With an earlier available descriptor,the ignored replacement leaves
    // stored handle1 pointing to a destroyed descriptor. Null result is still
    // published and followed by the original map-root header writes at zero.
    Fixture failed;failed.words[0x8041f070]=0x1000;failed.words[0x8041f074]=2;failed.words[0x1000]=0xffffffff;failed.words[0x100c]=1024;failed.words[0x8041e848]=1;failed.words[0x8041e834]=0x2000;failed.words[0x8041e820]=0x2400;
    clearEventMemoryPool(1,failed.services());assert(failed.read(0x8041e848)==1 && failed.read(0x8041e808)==0 && failed.read(0x8041e818)==992 && failed.read(4)==960 && failed.flushes==0 && failed.barriers==0);
    std::cout<<"event memory bootstrap tests passed\n";
}
