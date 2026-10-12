#include "event_data_opcodes.hpp"
#include "event_operands.hpp"
#include <cassert>
#include <cmath>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
constexpr int32_t lw=-30000000;
struct Fixture {
    EventEntry e;
    std::map<uint32_t,uint32_t> memory;
    std::vector<int32_t> queries,destinations,integers;
    std::vector<float> floats,conversions;
    std::vector<uint32_t> reads;
    std::function<void(uint32_t)> onRead;
    std::function<void(int32_t)> onQuery;
    std::function<void()> onStore,onConvert;
    unsigned stores=0;
    Fixture() {e.arguments=0x3000;e.field150=0x4000;memory[0x3000]=uint32_t(lw);memory[0x3004]=2;}
    uint32_t read(uint32_t a) {
        reads.push_back(a);if(onRead)onRead(a);
        if(a>=0x109c && a<0x10dc)return uint32_t(e.locals[(a-0x109c)/4]);
        return memory.at(a);
    }
    EventOperandServices operands() {
        EventOperandServices s;s.user=this;
        s.contextAddress=[](void*)->uint32_t {return 0x2000;};
        s.entryAddress=[](void*,EventEntry&)->uint32_t {return 0x1000;};
        s.readWord=[](void* p,uint32_t a) {return static_cast<Fixture*>(p)->read(a);};
        s.writeWord=[](void* p,uint32_t a,uint32_t v) {auto& f=*static_cast<Fixture*>(p);assert(a>=0x109c && a<0x10dc);f.e.locals[(a-0x109c)/4]=int32_t(v);};
        s.truncate=[](void*,float v) {assert(std::isfinite(v) && double(v)>=-2147483648. && double(v)<2147483648.);return int32_t(v);};
        return s;
    }
    EventDataOpcodeServices services() {
        EventDataOpcodeServices s;s.values.user=this;s.numericUser=this;
        s.values.word=[](void* p,uint32_t a) {return static_cast<Fixture*>(p)->read(a);};
        s.values.resolve=[](void* p,EventEntry& e,int32_t op) {auto& f=*static_cast<Fixture*>(p);f.queries.push_back(op);if(f.onQuery)f.onQuery(op);return getEventValue(e,op,f.operands());};
        s.values.resolveFloat=[](void* p,EventEntry& e,int32_t op) {auto& f=*static_cast<Fixture*>(p);f.queries.push_back(op);if(f.onQuery)f.onQuery(op);return getEventFloat(e,op,f.operands());};
        s.values.store=[](void* p,EventEntry& e,int32_t d,int32_t v) {auto& f=*static_cast<Fixture*>(p);f.destinations.push_back(d);f.integers.push_back(v);setEventValue(e,d,v,f.operands());++f.stores;if(f.onStore)f.onStore();};
        s.values.storeFloat=[](void* p,EventEntry& e,int32_t d,float v) {auto& f=*static_cast<Fixture*>(p);f.destinations.push_back(d);f.floats.push_back(v);setEventFloat(e,d,v,f.operands());++f.stores;if(f.onStore)f.onStore();};
        s.divideWord=[](void*,int32_t a,int32_t b) {assert(b!=0 && !(a==std::numeric_limits<int32_t>::min() && b==-1));return a/b;};
        s.truncate=[](void* p,float v) {auto& f=*static_cast<Fixture*>(p);f.conversions.push_back(v);if(f.onConvert)f.onConvert();return int32_t(v);};
        return s;
    }
    void run(uint8_t op) {assert(eventDataOpcode(e,op,services())==2);}
};
uint32_t command(int n,int op) {return uint32_t(n)<<16|uint32_t(op);}
}
int main() {
    Fixture unsupported;assert(!eventDataOpcode(unsupported.e,49,unsupported.services()));assert(!eventDataOpcode(unsupported.e,74,unsupported.services()));assert(unsupported.reads.empty());
    Fixture assignment;assignment.memory[0x3004]=uint32_t(lw+1);assignment.e.locals[1]=8;
    assignment.run(50);assert(assignment.e.locals[0]==8 && assignment.queries==std::vector<int32_t>({lw+1}));
    assignment.queries.clear();assignment.run(51);assert(assignment.e.locals[0]==lw+1 && assignment.queries.empty());
    assignment.memory[0x3004]=uint32_t(-229998464);assignment.run(52);assert(assignment.floats.back()==1.5f && assignment.e.locals[0]==-229998464);
    // Captured destination survives RHS mutation of the argument words; lhs is live.
    Fixture ordering;ordering.e.locals[0]=7;ordering.memory[0x3004]=5;
    ordering.onQuery=[&](int32_t op) {if(op==5) {ordering.e.locals[0]=20;ordering.memory[0x3000]=uint32_t(lw+1);ordering.e.arguments=0x9999;}};
    ordering.run(53);assert(ordering.queries==std::vector<int32_t>({5,lw}) && ordering.e.locals[0]==25 && ordering.destinations[0]==lw);
    struct Arithmetic {uint8_t op;int32_t lhs,rhs,result;};
    for(auto c:std::vector<Arithmetic>{{53,2147483647,1,int32_t(-2147483647-1)},{54,int32_t(-2147483647-1),1,2147483647},{55,0x40000001,4,4},{56,-7,2,-3},{56,7,-2,-3},{57,-3,2,0},{57,3,2,1}}) {
        Fixture f;f.e.locals[0]=c.lhs;f.memory[0x3004]=uint32_t(c.rhs);f.run(c.op);assert(f.e.locals[0]==c.result);
        assert(f.queries==std::vector<int32_t>({c.rhs,lw}));
        if(c.op==57)assert(f.conversions==std::vector<float>({float(c.rhs)+.5f,float(c.lhs)+.5f}));
    }
    Fixture mod;mod.e.locals[0]=99;mod.memory[0x3004]=2;
    mod.onConvert=[&] {if(mod.conversions.size()==1)mod.e.locals[0]=-3;};
    mod.run(57);assert(mod.e.locals[0]==0 && mod.conversions==std::vector<float>({2.5f,-2.5f}));
    for(auto c:std::vector<Arithmetic>{{58,7,2,9},{59,7,2,5},{60,7,2,14},{61,7,2,0}}) {
        Fixture f;f.e.locals[0]=c.lhs;f.memory[0x3004]=uint32_t(c.rhs);f.run(c.op);
        const float expected=c.op==61?3.5f:float(c.result);
        assert(f.floats[0]==expected && getEventFloat(f.e,lw,f.operands())==expected);
        assert(f.queries==std::vector<int32_t>({c.rhs,lw}));
    }
    Fixture rounding;rounding.e.locals[0]=16777216;rounding.memory[0x3004]=1;
    // Use a captured output to check binary32 arithmetic without encoding overflow.
    auto roundServices=rounding.services();roundServices.values.storeFloat=[](void* p,EventEntry&,int32_t,float v) {static_cast<Fixture*>(p)->floats.push_back(v);};
    assert(eventDataOpcode(rounding.e,58,roundServices)==2 && rounding.floats[0]==16777216.f);
    Fixture quantized;quantized.e.locals[0]=1;quantized.memory[0x3004]=3;quantized.run(61);
    assert(quantized.floats[0]==1.f/3.f && getEventFloat(quantized.e,lw,quantized.operands())==341.f/1024.f);
    for(uint8_t op:{uint8_t(62),uint8_t(68)}) {Fixture f;f.memory[0x3000]=uint32_t(lw+1);f.e.locals[1]=0x4500;f.run(op);assert(f.e.field150==0x4500 && f.queries==std::vector<int32_t>({lw+1}));}
    for(uint8_t op=63;op<=66;++op) {
        Fixture f;const unsigned n=op-62;
        for(unsigned i=0;i<n;++i) {f.memory[0x3000+4*i]=uint32_t(lw+i);f.memory[0x4000+4*i]=i==0?uint32_t(lw+5):10+i;}
        f.onRead=[&](uint32_t a) {if(a>=0x4000 && a<0x4010)assert(f.e.field150==a+4);};
        f.run(op);assert(f.e.field150==0x4000+4*n && f.queries.empty() && f.e.locals[0]==lw+5);
        for(unsigned i=1;i<n;++i)assert(f.e.locals[i]==int32_t(10+i));
    }
    Fixture lanes;lanes.memory[0x4000]=1;lanes.memory[0x5000]=99;lanes.memory[0x3004]=uint32_t(lw+1);
    lanes.onStore=[&] {if(lanes.stores==1) {lanes.e.field150=0x5000;lanes.e.arguments=0x9999;lanes.memory[0x3004]=uint32_t(lw);}};
    lanes.run(64);assert(lanes.e.locals[0]==99 && lanes.e.locals[1]==0 && lanes.e.field150==0x5004);
    for(uint8_t op=69;op<=72;++op) {
        Fixture f;const unsigned n=op-68;
        for(unsigned i=0;i<n;++i) {f.memory[0x3000+4*i]=uint32_t(lw+i);f.memory[0x4000+4*i]=0x3f800000;}
        f.onRead=[&](uint32_t a) {if(a>=0x4000 && a<0x4010)assert(f.e.field150==a+4);};
        f.run(op);assert(f.floats==std::vector<float>(n,1.f) && f.e.field150==0x4000+4*n && f.queries.empty());
    }
    Fixture singleFloat;singleFloat.memory[0x5000]=0x3f800000;
    singleFloat.onRead=[&](uint32_t a) {if(a==0x3000)singleFloat.e.field150=0x5000;};
    singleFloat.run(69);assert(singleFloat.e.field150==0x5004 && singleFloat.floats[0]==1.f);
    Fixture floatLanes;floatLanes.memory[0x4000]=0x3f800000;floatLanes.memory[0x5000]=0x40000000;floatLanes.memory[0x3004]=uint32_t(lw+1);
    floatLanes.onStore=[&] {if(floatLanes.stores==1) {floatLanes.e.field150=0x5000;floatLanes.e.arguments=0x9999;floatLanes.memory[0x3004]=uint32_t(lw);}};
    floatLanes.run(70);assert(floatLanes.floats==std::vector<float>({1,2}) && getEventFloat(floatLanes.e,lw,floatLanes.operands())==2.f && floatLanes.e.field150==0x5004);
    for(uint8_t op:{uint8_t(67),uint8_t(73)}) {
        Fixture f;f.memory[0x3004]=uint32_t(-1);f.memory[0x4ffc]=0xffffffff;
        f.onQuery=[&](int32_t i) {assert(i==-1);f.e.field150=0x5000;f.memory[0x3000]=uint32_t(lw+1);};
        f.run(op);assert(f.e.field150==0x5000 && f.destinations[0]==lw && f.e.locals[1]==0);
        if(op==67)assert(f.e.locals[0]==-1);else assert(f.floats[0]==-1.f);
    }
    Fixture indexed;indexed.memory[0x3004]=0;indexed.memory[0x4000]=0x3f800000;
    auto indexServices=indexed.services();indexServices.values.storeFloat=roundServices.values.storeFloat;
    assert(eventDataOpcode(indexed.e,73,indexServices)==2 && indexed.floats[0]==1065353216.f && indexed.e.field150==0x4000);
    // Real dispatch executes assignment, arithmetic and raw cursor reads before
    // its frame wait yields. This uses actual integer/float operand providers.
    Fixture program;uint32_t a=0x6000;
    auto emit=[&](int op,std::initializer_list<int32_t> args) {program.memory[a]=command(args.size(),op);a+=4;for(int32_t v:args) {program.memory[a]=uint32_t(v);a+=4;}};
    emit(50,{lw,7});emit(53,{lw,2});emit(57,{lw,4});emit(62,{0x4000});emit(63,{lw+1});emit(52,{lw+2,-229998464});emit(58,{lw+2,2});emit(9,{1});
    program.memory[0x4000]=uint32_t(lw+5);program.e.nextCommand=0x6000;
    auto dispatch=program.services().values;
    dispatch.handler=[](void* p,EventEntry& e,uint8_t op) {auto& f=*static_cast<Fixture*>(p);auto r=eventDataOpcode(e,op,f.services());assert(r);return *r;};
    assert(dispatchEvent(program.e,dispatch)==0 && program.e.opcode==0 && program.e.waitCount==0);
    assert(program.e.locals[0]==1 && program.e.locals[1]==lw+5 && getEventFloat(program.e,lw+2,program.operands())==3.5f);
    std::cout<<"event data opcode tests passed\n";
}
