#include "event_memory_opcodes.hpp"
#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
constexpr int32_t lw=-30000000;
struct Access {char type;uint32_t address,value;bool operator==(const Access& a) const {return type==a.type && address==a.address && value==a.value;}};
struct Fixture {
    EventEntry e;
    std::map<uint32_t,uint8_t> bytes;
    std::vector<Access> accesses;
    std::vector<int32_t> queries,outputs,destinations;
    std::vector<float> floatOutputs;
    std::map<int32_t,float> floats;
    std::function<void(int32_t)> onQuery;
    std::function<void()> onStore;
    Fixture() {e.arguments=0x100;e.field150=0x150;e.userFlags=0x158;put(0x100,uint32_t(lw));put(0x104,2);}
    void put(uint32_t a,uint32_t v) {for(unsigned i=0;i<4;++i)bytes[a+i]=uint8_t(v>>(24-8*i));}
    uint32_t peek(uint32_t a) {uint32_t v=0;for(unsigned i=0;i<4;++i)v=v<<8|bytes[a+i];return v;}
    void header(uint32_t a,uint32_t next,uint32_t size,uint16_t used=0) {put(a,next);put(a+4,size);bytes[a+8]=uint8_t(used>>8);bytes[a+9]=uint8_t(used);}
    EventMemoryServices memory() {
        EventMemoryServices s;s.user=this;
        s.word=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);const uint32_t v=f.peek(a);f.accesses.push_back({'r',a,v});return v;};
        s.half=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);const uint16_t v=uint16_t(f.bytes[a])<<8|f.bytes[a+1];f.accesses.push_back({'h',a,v});return v;};
        s.storeWord=[](void* p,uint32_t a,uint32_t v) {auto& f=*static_cast<Fixture*>(p);f.accesses.push_back({'w',a,v});f.put(a,v);};
        s.storeHalf=[](void* p,uint32_t a,uint16_t v) {auto& f=*static_cast<Fixture*>(p);f.accesses.push_back({'s',a,v});f.bytes[a]=uint8_t(v>>8);f.bytes[a+1]=uint8_t(v);};
        s.storeByte=[](void* p,uint32_t a,uint8_t v) {auto& f=*static_cast<Fixture*>(p);f.accesses.push_back({'b',a,v});f.bytes[a]=v;};
        s.flushBlock=[](void* p,uint32_t a) {static_cast<Fixture*>(p)->accesses.push_back({'f',a,0});};
        s.systemCall=[](void* p) {static_cast<Fixture*>(p)->accesses.push_back({'c',0,0});};return s;
    }
    EventMemoryOpcodeServices services() {
        EventMemoryOpcodeServices s;s.memory=memory();s.values.user=this;s.values.word=s.memory.word;
        s.values.resolve=[](void* p,EventEntry& e,int32_t op) {auto& f=*static_cast<Fixture*>(p);f.queries.push_back(op);if(f.onQuery)f.onQuery(op);return op>=lw && op<lw+16?e.locals[op-lw]:op;};
        s.values.resolveFloat=[](void* p,EventEntry&,int32_t op) {auto& f=*static_cast<Fixture*>(p);f.queries.push_back(op);if(f.onQuery)f.onQuery(op);return f.floats.at(op);};
        s.values.store=[](void* p,EventEntry& e,int32_t d,int32_t v) {auto& f=*static_cast<Fixture*>(p);f.destinations.push_back(d);f.outputs.push_back(v);if(d>=lw && d<lw+16)e.locals[d-lw]=v;if(f.onStore)f.onStore();};
        s.values.storeFloat=[](void* p,EventEntry&,int32_t d,float v) {auto& f=*static_cast<Fixture*>(p);f.destinations.push_back(d);f.floatOutputs.push_back(v);if(f.onStore)f.onStore();};return s;
    }
    void run(uint8_t op) {assert(eventMemoryOpcode(e,op,services())==2);}
};
}
int main() {
    Fixture unsupported;assert(!eventMemoryOpcode(unsupported.e,73,unsupported.services()));assert(!eventMemoryOpcode(unsupported.e,91,unsupported.services()));assert(unsupported.accesses.empty());
    for(unsigned align=0;align<4;++align)for(uint32_t length:{0u,1u,31u,32u,33u,77u}) {
        Fixture f;const uint32_t address=0x1000+align;f.bytes[address-1]=0x55;f.bytes[address+length]=0x66;
        assert(eventFillMemory(address,0x1234,length,f.memory())==address);
        assert(f.bytes[address-1]==0x55 && f.bytes[address+length]==0x66);
        for(uint32_t i=0;i<length;++i)assert(f.bytes[address+i]==0x34);
        uint32_t next=address;for(auto a:f.accesses) {assert(a.address==next);if(a.type=='w') {assert((a.address&3)==0 && a.value==0x34343434);next+=4;}else {assert(a.type=='b' && a.value==0x34);++next;}}
        assert(next==address+length);
        if(length<32)for(auto a:f.accesses)assert(a.type=='b');
    }
    Fixture flush;eventFlushMemory(0x1021,0,flush.memory());assert(flush.accesses.empty());eventFlushMemory(0x1021,64,flush.memory());
    assert(flush.accesses==std::vector<Access>({{'f',0x1021,0},{'f',0x1041,0},{'f',0x1061,0},{'c',0,0}}));
    Fixture zero;assert(eventMapAllocate(0,0,zero.memory())==0 && eventMapAllocate(0,0xffffffe1,zero.memory())==0 && zero.accesses.empty());
    Fixture best;best.header(0x1000,0x2000,128);best.header(0x2000,0x3000,96);best.header(0x3000,0,96);
    assert(eventMapAllocate(0x1000,1,best.memory())==0x2020);
    assert(best.peek(0x1000)==0x2000 && best.peek(0x1004)==128 && best.peek(0x2000)==0x2040 && best.peek(0x2004)==32 && best.bytes[0x2009]==1);
    assert(best.peek(0x2040)==0x3000 && best.peek(0x2044)==32 && best.bytes[0x2049]==0);
    for(unsigned i=0;i<32;++i)assert(best.bytes[0x2020+i]==0);
    std::vector<Access> mutations;for(auto a:best.accesses)if(a.type=='w' || a.type=='s' || a.type=='b' || a.type=='f' || a.type=='c')mutations.push_back(a);
    const std::vector<Access> prefix{{'w',0x2000,0x2040},{'w',0x2004,32},{'s',0x2008,1},{'w',0x2040,0x3000},{'w',0x2044,32},{'s',0x2048,0}};
    assert(std::vector<Access>(mutations.begin(),mutations.begin()+6)==prefix && mutations[6]==(Access{'w',0x2020,0}) && mutations[14]==(Access{'f',0x2020,0}) && mutations[15]==(Access{'c',0,0}));
    Fixture exact;exact.header(0x1000,0,64);assert(eventMapAllocate(0x1000,32,exact.memory())==0x1020);
    assert(exact.peek(0x1000)==0x1040 && exact.peek(0x1004)==32 && exact.peek(0x1040)==0 && exact.peek(0x1044)==0 && exact.bytes[0x1049]==0);
    Fixture failed;failed.header(0x1000,0x2000,128,1);failed.header(0x2000,0,32);assert(eventMapAllocate(0x1000,1,failed.memory())==0);
    for(auto a:failed.accesses)assert(a.type=='r' || a.type=='h');
    Fixture pointers;pointers.put(0x100,uint32_t(lw));pointers.e.locals[0]=0x5000;pointers.run(74);assert(pointers.e.userWork==0x5000 && pointers.e.userFlags==0x158 && pointers.e.field150==0x150);pointers.run(75);assert(pointers.e.userFlags==0x5000 && pointers.e.field150==0x150);
    Fixture allocate;allocate.header(0x1000,0,192);allocate.put(0x8041e808,0x9999);allocate.put(0x100,8);allocate.put(0x104,99);allocate.e.userWork=0x7777;
    allocate.onQuery=[&](int32_t op) {assert(op==8);allocate.put(0x104,uint32_t(lw));allocate.put(0x8041e808,0x1000);allocate.e.arguments=0x9999;};
    allocate.onStore=[&] {assert(uint32_t(allocate.outputs.back())==allocate.e.userWork && allocate.e.userFlags==0x158 && allocate.e.field150==0x150);};
    allocate.run(76);assert(allocate.e.userWork==0x1020 && allocate.destinations[0]==lw && allocate.e.locals[0]==0x1020);
    allocate.e.arguments=0x100;allocate.run(76);assert(allocate.e.userWork==0x1060 && allocate.bytes[0x1009]==1 && allocate.bytes[0x1049]==1); // No release of prior payload.
    Fixture wrap;wrap.put(0x100,0x40000000);wrap.put(0x104,uint32_t(lw));wrap.put(0x8041e808,0x5555);wrap.e.userWork=123;wrap.run(76);assert(wrap.e.userWork==0 && wrap.outputs[0]==0);
    for(auto a:wrap.accesses)assert(a.type=='r' && (a.address==0x100 || a.address==0x104 || a.address==0x8041e808));
    for(uint8_t op=77;op<=80;++op) {
        Fixture f;f.put(0x104,uint32_t(lw+1));f.e.locals[0]=-1;f.e.locals[1]=2;f.run(op);
        const uint32_t rhs=op==78 || op==80?uint32_t(lw+1):2;
        assert(uint32_t(f.outputs[0])==(op<=78?0xffffffffu&rhs:0xffffffffu|rhs));
        assert(f.queries==(op==78 || op==80?std::vector<int32_t>({lw}):std::vector<int32_t>({lw+1,lw})));
    }
    Fixture rawCapture;rawCapture.e.locals[0]=1;rawCapture.put(0x104,2);rawCapture.onQuery=[&](int32_t) {rawCapture.put(0x100,uint32_t(lw+1));rawCapture.put(0x104,99);};rawCapture.run(80);assert(rawCapture.outputs[0]==3 && rawCapture.destinations[0]==lw);
    Fixture rhsFirst;rhsFirst.e.locals[0]=0;rhsFirst.onQuery=[&](int32_t op) {if(op==2)rhsFirst.e.locals[0]=1;};rhsFirst.run(79);assert(rhsFirst.outputs[0]==3 && rhsFirst.queries==std::vector<int32_t>({2,lw}));
    struct Time {uint8_t op;int32_t input,result;};
    for(auto t:std::vector<Time>{{81,17,1},{81,-17,-1},{81,16,0},{81,-16,0},{81,40000000,-1894967},{81,2147483647,0},{82,1,16},{82,-1,-16},{82,3,50},{82,-3,-50},{82,3000000,-21582788}}) {Fixture f;f.put(0x104,uint32_t(t.input));f.run(t.op);assert(f.outputs[0]==t.result && f.queries==std::vector<int32_t>({t.input}));}
    for(uint8_t op:{uint8_t(83),uint8_t(84)}) {
        Fixture f;f.put(0x100,7);f.put(0x104,0x4000);f.floats[7]=1.5f;f.onQuery=[&](int32_t) {f.put(0x104,0x5000);f.e.arguments=0x9999;};f.run(op);
        assert(f.peek(0x5000)==(op==83?7u:0x3fc00000u) && f.peek(0x4000)==0 && f.queries==std::vector<int32_t>({7}) && f.outputs.empty());
    }
    for(uint8_t op:{uint8_t(85),uint8_t(86)}) {
        Fixture f;f.put(0x104,0x4000);f.put(0x4000,op==85?uint32_t(lw+1):0x3fc00000u);f.run(op);
        assert(f.queries.empty() && f.destinations[0]==lw && f.accesses[0].address==0x104 && f.accesses[1].address==0x100 && f.accesses[2].address==0x4000);
        if(op==85)assert(f.outputs[0]==lw+1);else assert(f.floatOutputs[0]==1.5f);
    }
    for(uint8_t op:{uint8_t(87),uint8_t(88)}) {
        Fixture f;f.put(0x100,uint32_t(lw+1));f.e.locals[1]=lw;f.floats[3]=2.5f;
        f.onQuery=[&](int32_t q) {if(q==lw+1) {f.put(0x104,3);f.e.arguments=0x9999;}};f.run(op);
        assert(f.queries==std::vector<int32_t>({lw+1,3}) && f.destinations[0]==lw);
        if(op==87)assert(f.e.locals[0]==3);else assert(f.floatOutputs[0]==2.5f);
    }
    for(uint8_t op:{uint8_t(89),uint8_t(90)}) {
        Fixture f;f.put(0x100,uint32_t(lw+1));f.e.locals[1]=lw+2;f.e.locals[2]=7;f.floats[lw+2]=2.5f;
        f.onQuery=[&](int32_t q) {if(q==lw+2) {f.put(0x104,uint32_t(lw));f.e.arguments=0x9999;}};f.run(op);
        assert(f.queries==std::vector<int32_t>({lw+1,lw+2}) && f.destinations[0]==lw);
        if(op==89)assert(f.e.locals[0]==7);else assert(f.floatOutputs[0]==2.5f);
    }
    Fixture literal;literal.put(0x100,5);literal.put(0x104,uint32_t(lw));literal.run(89);assert(literal.e.locals[0]==5 && literal.queries==std::vector<int32_t>({5,5}));
    std::cout<<"event memory opcode tests passed\n";
}
