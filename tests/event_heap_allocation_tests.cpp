#include "event_heap_allocation.hpp"
#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
struct Operation {char kind;uint32_t address,value;bool operator==(const Operation& o) const {return kind==o.kind && address==o.address && value==o.value;}};
struct Fixture {
    std::map<uint32_t,uint8_t> bytes;
    std::vector<Operation> operations;
    std::function<void(uint32_t)> onRead;
    Fixture() {put(0x8041f070,0x1000);put(0x8041e844,0);put(0x1000,0x1234);put(0x1004,0x2000);put(0x1008,0x5000);node(0x5000,0,0,64);}
    void put(uint32_t a,uint32_t v) {for(unsigned i=0;i<4;++i)bytes[a+i]=uint8_t(v>>(24-8*i));}
    uint32_t peek(uint32_t a) {uint32_t v=0;for(unsigned i=0;i<4;++i)v=v<<8|bytes[a+i];return v;}
    void node(uint32_t a,uint32_t previous,uint32_t next,uint32_t size) {put(a,previous);put(a+4,next);put(a+8,size);}
    EventMemoryServices services() {
        EventMemoryServices s;s.user=this;
        s.word=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);if(f.onRead)f.onRead(a);const uint32_t v=f.peek(a);f.operations.push_back({'r',a,v});return v;};
        s.storeWord=[](void* p,uint32_t a,uint32_t v) {auto& f=*static_cast<Fixture*>(p);f.operations.push_back({'w',a,v});f.put(a,v);};
        s.storeByte=[](void* p,uint32_t a,uint8_t v) {auto& f=*static_cast<Fixture*>(p);f.operations.push_back({'b',a,v});f.bytes[a]=v;};
        s.flushBlock=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);f.operations.push_back({'f',a,0});};
        s.systemCall=[](void* p) {auto& f=*static_cast<Fixture*>(p);f.operations.push_back({'c',0,0});};return s;
    }
};
}
int main() {
    Fixture split;split.node(0x2000,0,0x3000,128);split.node(0x3000,0x2000,0,64);
    assert(eventSdkAllocate(0,1,split.services())==0x2020); // First fit,not smallest64 node.
    assert(split.peek(0x2008)==64 && split.peek(0x2048)==64 && split.peek(0x2040)==0 && split.peek(0x2044)==0x3000 && split.peek(0x3000)==0x2040 && split.peek(0x1004)==0x2040);
    assert(split.peek(0x2000)==0 && split.peek(0x2004)==0x5000 && split.peek(0x5000)==0x2000 && split.peek(0x1008)==0x2000 && split.peek(0x1000)==0x1234);
    std::vector<Operation> writes;for(auto op:split.operations)if(op.kind=='w')writes.push_back(op);
    assert(writes==std::vector<Operation>({{'w',0x2008,64},{'w',0x2048,64},{'w',0x2040,0},{'w',0x2044,0x3000},{'w',0x3000,0x2040},{'w',0x1004,0x2040},{'w',0x2004,0x5000},{'w',0x2000,0},{'w',0x5000,0x2000},{'w',0x1008,0x2000}}));
    for(auto op:split.operations)assert(op.kind=='w' || op.kind=='r'); // SDK alone does not fill/flush.
    Fixture absorb;absorb.node(0x2000,0,0x3000,96);absorb.node(0x3000,0x2000,0,128);
    assert(eventSdkAllocate(0,1,absorb.services())==0x2020 && absorb.peek(0x2008)==96 && absorb.peek(0x3000)==0 && absorb.peek(0x1004)==0x3000);
    Fixture middle;middle.node(0x2000,0,0x3000,32);middle.node(0x3000,0x2000,0x4000,64);middle.node(0x4000,0x3000,0,128);
    assert(eventSdkAllocate(0,1,middle.services())==0x3020 && middle.peek(0x2004)==0x4000 && middle.peek(0x4000)==0x2000 && middle.peek(0x1004)==0x2000 && middle.peek(0x3008)==64);
    Fixture middleSplit;middleSplit.node(0x2000,0,0x3000,32);middleSplit.node(0x3000,0x2000,0x4000,160);middleSplit.node(0x4000,0x3000,0,128);
    assert(eventSdkAllocate(0,1,middleSplit.services())==0x3020 && middleSplit.peek(0x2004)==0x3040 && middleSplit.peek(0x3040)==0x2000 && middleSplit.peek(0x3044)==0x4000 && middleSplit.peek(0x4000)==0x3040 && middleSplit.peek(0x1004)==0x2000);
    Fixture signedSize;signedSize.node(0x2000,0,0x3000,0x80000000);signedSize.node(0x3000,0x2000,0,64);assert(eventSdkAllocate(0,1,signedSize.services())==0x3020);
    Fixture empty;empty.put(0x1004,0);assert(eventSdkAllocate(0,1,empty.services())==0);for(auto op:empty.operations)assert(op.kind=='r');
    Fixture noFit;noFit.node(0x2000,0,0,32);assert(eventHeapAllocate(0,1,noFit.services())==0);for(auto op:noFit.operations)assert(op.kind=='r');
    Fixture secondHandle;secondHandle.put(0x8041e848,1);secondHandle.put(0x1010,0x3000);secondHandle.put(0x1014,0);secondHandle.node(0x3000,0,0,96);
    assert(eventHeapAllocate(1,1,secondHandle.services())==0x3020 && secondHandle.peek(0x1014)==0x3000 && secondHandle.peek(0x1004)==0x2000);
    Fixture wrapper;wrapper.node(0x2000,0,0,96);for(unsigned i=0;i<64;++i)wrapper.bytes[0x2020+i]=0xaa;
    assert(eventHeapAllocate(0,1,wrapper.services())==0x2020 && wrapper.bytes[0x2020]==0 && wrapper.bytes[0x2021]==0xaa && wrapper.peek(0x2008)==96);
    assert(wrapper.operations[wrapper.operations.size()-3]==(Operation{'b',0x2020,0}) && wrapper.operations[wrapper.operations.size()-2]==(Operation{'f',0x2020,0}) && wrapper.operations.back()==(Operation{'c',0,0}));
    Fixture zero;zero.node(0x2000,0,0,32);assert(eventHeapAllocate(0,0,zero.services())==0x2020);for(auto op:zero.operations)assert(op.kind=='r' || op.kind=='w');
    // A caller's heap table handle is read before descriptor access;the size is
    // reread after fit selection as in the original body,not snapshotted once.
    Fixture reread;reread.node(0x2000,0,0,128);unsigned sizeReads=0;
    reread.onRead=[&](uint32_t a) {if(a==0x2008 && ++sizeReads==2)reread.put(a,96);};
    assert(eventSdkAllocate(0,1,reread.services())==0x2020 && sizeReads==2 && reread.peek(0x2008)==96 && reread.peek(0x1004)==0);
    // Both original context requests fit a normal SDK heap;each wrapper clears
    // only its requested payload,then flushes every original32-byte block.
    Fixture arrays;arrays.put(0x1008,0);arrays.node(0x2000,0,0,0x36080);
    const uint32_t first=eventHeapAllocate(0,0x1b000,arrays.services()),second=eventHeapAllocate(0,0x1b000,arrays.services());
    assert(first==0x2020 && second==0x1d040 && first!=second && arrays.peek(0x2008)==0x1b020 && arrays.peek(0x1d028)==0x1b020);
    unsigned flushes=0,barriers=0;for(auto op:arrays.operations) {if(op.kind=='f')++flushes;if(op.kind=='c')++barriers;}
    assert(flushes==2*(0x1b000/32) && barriers==2);
    std::cout<<"event SDK heap allocation tests passed\n";
}
