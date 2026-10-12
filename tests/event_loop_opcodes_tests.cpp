#include "event_loop_opcodes.hpp"
#include "event_data_opcodes.hpp"
#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
constexpr int32_t lw=-30000000;
uint32_t command(int n,int op) {return uint32_t(uint16_t(n))<<16|uint32_t(op);}
struct Fixture {
    EventEntry e;
    std::map<uint32_t,uint32_t> memory;
    std::vector<int32_t> queries,destinations,outputs;
    std::function<void(uint32_t)> onRead;
    std::function<void(int32_t)> onQuery;
    std::function<void()> onStore;
    Fixture() {e.loopIndex=255;e.arguments=0x1000;e.nextCommand=0x2000;e.labelIds.fill(255);memory[0x1000]=1;}
    EventDispatchServices services() {
        EventDispatchServices s;s.user=this;
        s.word=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);if(f.onRead)f.onRead(a);return f.memory.at(a);};
        s.resolve=[](void* p,EventEntry& e,int32_t op) {auto& f=*static_cast<Fixture*>(p);f.queries.push_back(op);if(f.onQuery)f.onQuery(op);return op>=lw && op<lw+16?e.locals[op-lw]:op;};
        s.store=[](void* p,EventEntry& e,int32_t d,int32_t v) {auto& f=*static_cast<Fixture*>(p);f.destinations.push_back(d);f.outputs.push_back(v);if(d>=lw && d<lw+16)e.locals[d-lw]=v;if(f.onStore)f.onStore();};
        return s;
    }
    void run(uint8_t op) {assert(eventLoopOpcode(e,op,services())==2);}
};
}
int main() {
    Fixture unsupported;assert(!eventLoopOpcode(unsupported.e,3,unsupported.services()));assert(!eventLoopOpcode(unsupported.e,9,unsupported.services()));
    Fixture labels;labels.e.labelIds[0]=7;labels.e.labelIds[1]=7;labels.e.labelIds[2]=128;labels.e.labelAddresses[0]=100;labels.e.labelAddresses[1]=200;labels.e.labelAddresses[2]=300;
    labels.memory[0x1000]=7;labels.run(4);assert(labels.e.nextCommand==100);
    labels.memory[0x1000]=uint32_t(-128);labels.run(4);assert(labels.e.nextCommand==300);
    labels.memory[0x1000]=128;labels.run(4);assert(labels.e.nextCommand==0x1000);
    labels.memory[0x1000]=uint32_t(-1);labels.e.labelAddresses[3]=400;labels.run(4);assert(labels.e.nextCommand==400); // Empty ff records compare as signed-1.
    labels.memory[0x1000]=uint32_t(-270000000);labels.run(4);assert(labels.e.nextCommand==0x1000);
    labels.memory[0x1000]=uint32_t(-270000001);labels.run(4);assert(labels.e.nextCommand==uint32_t(-270000001));
    Fixture fallback;fallback.memory[0x1000]=99;fallback.onQuery=[&](int32_t) {fallback.e.arguments=0x8888;};fallback.run(4);assert(fallback.e.nextCommand==0x8888);
    Fixture pushed;pushed.memory[0x1000]=uint32_t(lw);pushed.e.locals[0]=0;
    pushed.onRead=[&](uint32_t a) {if(a==0x1000)assert(pushed.e.loopIndex==0 && pushed.e.loopBodies[0]==0x1004);};
    pushed.run(5);assert(pushed.e.loopCounts[0]==lw && pushed.e.nextCommand==0x2000 && pushed.queries.empty());
    pushed.run(6);assert(pushed.e.locals[0]==-1 && pushed.e.loopCounts[0]==lw && pushed.e.loopIndex==0 && pushed.e.nextCommand==0x1004);
    pushed.e.locals[0]=1;pushed.e.nextCommand=0x7777;pushed.run(6);assert(pushed.e.locals[0]==0 && pushed.e.loopIndex==255 && pushed.e.nextCommand==0x7777);
    Fixture literal;literal.e.loopIndex=0;literal.e.loopBodies[0]=0x1234;
    literal.run(6);assert(literal.e.nextCommand==0x1234 && literal.e.loopCounts[0]==0 && literal.e.loopIndex==0 && literal.queries.empty());
    literal.e.loopCounts[0]=1;literal.e.nextCommand=0x7777;literal.run(6);assert(literal.e.loopCounts[0]==0 && literal.e.loopIndex==255 && literal.e.nextCommand==0x7777);
    literal.e.loopIndex=0;literal.e.loopCounts[0]=-2;literal.run(6);assert(literal.e.loopCounts[0]==-3 && literal.e.loopIndex==0);
    literal.e.loopCounts[0]=-10000000;literal.run(6);assert(literal.e.loopCounts[0]==-10000001 && literal.queries.empty());
    literal.run(6);assert(literal.e.loopCounts[0]==-10000001 && literal.queries==std::vector<int32_t>({-10000001}) && literal.outputs.back()==-10000002);
    Fixture retained;retained.e.loopIndex=0;retained.e.loopCounts[0]=lw;retained.e.loopBodies[0]=0x1234;retained.e.locals[0]=2;
    retained.onStore=[&] {retained.e.loopIndex=1;retained.e.loopBodies[0]=0x4321;};retained.run(6);
    assert(retained.e.nextCommand==0x4321 && retained.e.loopIndex==1 && retained.e.loopCounts[0]==lw);
    retained.e.loopIndex=0;retained.e.locals[0]=1;retained.e.nextCommand=0x9999;retained.run(6);
    assert(retained.e.loopIndex==0 && retained.e.nextCommand==0x9999); // Pop rereads mutated index1.
    for(uint8_t op:{uint8_t(7),uint8_t(8)}) {
        Fixture f;f.e.loopIndex=1;f.e.loopCounts[1]=lw;
        f.memory[0x2000]=command(1,5);f.memory[0x2008]=command(0,2);f.memory[0x200c]=command(0,6);
        f.memory[0x2010]=command(1,53);f.memory[0x2018]=command(2,6);
        f.run(op);assert(f.e.nextCommand==(op==7?0x2024u:0x2018u) && f.e.loopIndex==(op==7?0:1));
        assert(f.e.loopCounts[1]==lw && f.queries.empty() && f.outputs.empty());
    }
    Fixture terminal;terminal.e.loopIndex=0;terminal.memory[0x2000]=command(2,1);terminal.run(8);assert(terminal.e.nextCommand==0x2000 && terminal.e.loopIndex==0);
    terminal.run(7);assert(terminal.e.nextCommand==0x200c && terminal.e.loopIndex==255);
    Fixture signedStride;signedStride.e.loopIndex=0;signedStride.memory[0x2000]=command(-2,53);signedStride.memory[0x1ffc]=command(0,6);signedStride.run(8);assert(signedStride.e.nextCommand==0x1ffc);
    Fixture program;uint32_t a=0x3000;
    auto emit=[&](int op,std::initializer_list<int32_t> args) {program.memory[a]=command(args.size(),op);a+=4;for(int32_t v:args) {program.memory[a]=uint32_t(v);a+=4;}};
    emit(5,{lw+1});emit(53,{lw,1});emit(6,{});emit(9,{1});program.e.locals[1]=3;program.e.nextCommand=0x3000;
    auto dispatcher=program.services();dispatcher.handler=[](void* p,EventEntry& e,uint8_t op) {auto& f=*static_cast<Fixture*>(p);if(auto r=eventLoopOpcode(e,op,f.services()))return *r;EventDataOpcodeServices s;s.values=f.services();auto r=eventDataOpcode(e,op,s);assert(r);return *r;};
    assert(dispatchEvent(program.e,dispatcher)==0 && program.e.loopIndex==255 && program.e.locals[0]==3 && program.e.locals[1]==0);
    assert(program.e.loopCounts[0]==lw+1 && program.e.opcode==0 && program.e.waitCount==0);
    std::cout<<"event loop opcode tests passed\n";
}
