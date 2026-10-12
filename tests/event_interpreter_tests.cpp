#include "event_interpreter.hpp"
#include "event_entry_addresses.hpp"
#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
constexpr int32_t lw=-30000000;
int32_t sw(uint32_t v) {return v<=0x7fffffff?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}
struct Call {uint32_t function;int32_t first;};
struct Fixture {
    std::array<EventEntry,256> entries{};
    EventEntryAddresses addresses;
    EventContext context{256,entries.data(),0};
    EventGlobals gp{0,60,0};
    EventRuntimeGlobals runtime{0,1,1,0};
    EventSchedule schedule;
    std::map<uint32_t,uint8_t> bytes;
    std::vector<Call> calls;
    std::vector<EventDiagnosticCall> diagnostics;
    std::vector<uint32_t> flushes;
    unsigned barriers=0,divisions=0;
    int32_t observedInteger=0;float observedFloat=0;
    static constexpr uint32_t clock=243000000;
    Fixture() {assert(addresses.registerEntries(0x10000,entries.data(),256));}
    uint32_t read(uint32_t a) {
        uint32_t mapped;if(addresses.readWord(a,mapped))return mapped;
        uint32_t v=0;for(unsigned i=0;i<4;++i)v=v<<8|bytes[a+i];return v;
    }
    void write(uint32_t a,uint32_t v) {
        if(addresses.writeWord(a,v))return;
        for(unsigned i=0;i<4;++i)bytes[a+i]=uint8_t(v>>(24-8*i));
    }
    EventCreationServices creation() {
        EventCreationServices s;s.user=this;s.manager.user=this;
        s.manager.globals=[](void* p) {return &static_cast<Fixture*>(p)->gp;};
        s.manager.context=[](void* p,bool battle) {assert(!battle);return &static_cast<Fixture*>(p)->context;};
        s.manager.clock=[](void*)->uint32_t {return clock;};
        s.runtime=[](void* p) {return &static_cast<Fixture*>(p)->runtime;};
        s.word=[](void* p,uint32_t a) {return static_cast<Fixture*>(p)->read(a);};
        s.stopAll=[](void* p,uint32_t mask) {auto& f=*static_cast<Fixture*>(p);stopAllEvents(mask,f.creation());};return s;
    }
    EventInterpreterServices services() {
        EventInterpreterServices s;s.schedule=&schedule;s.creation=creation();s.user=this;s.numericUser=&divisions;
        s.operands.user=this;s.operands.contextAddress=[](void*)->uint32_t {return 0x8000;};
        s.operands.entryAddress=[](void* p,EventEntry& e)->uint32_t {uint32_t a;assert(static_cast<Fixture*>(p)->addresses.addressOf(&e,a));return a;};
        s.operands.globalAddress=[](void*)->uint32_t {return 0x3000;};
        s.operands.readWord=s.creation.word;
        s.operands.writeWord=[](void* p,uint32_t a,uint32_t v) {static_cast<Fixture*>(p)->write(a,v);};
        s.operands.readByte=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);uint8_t v;if(f.addresses.readByte(a,v))return v;return f.bytes[a];};
        s.operands.writeByte=[](void* p,uint32_t a,uint8_t v) {auto& f=*static_cast<Fixture*>(p);if(!f.addresses.writeByte(a,v))f.bytes[a]=v;};
        s.operands.truncate=[](void*,float v) {assert(std::isfinite(v) && double(v)>=-2147483648. && double(v)<2147483648.);return int32_t(v);};
        s.memory.user=this;s.memory.word=s.creation.word;s.memory.storeWord=s.operands.writeWord;s.memory.storeByte=s.operands.writeByte;
        s.memory.half=[](void* p,uint32_t a)->uint16_t {auto& b=static_cast<Fixture*>(p)->bytes;return uint16_t(b[a])<<8|b[a+1];};
        s.memory.storeHalf=[](void* p,uint32_t a,uint16_t v) {auto& b=static_cast<Fixture*>(p)->bytes;b[a]=uint8_t(v>>8);b[a+1]=uint8_t(v);};
        s.memory.flushBlock=[](void* p,uint32_t a) {static_cast<Fixture*>(p)->flushes.push_back(a);};
        s.memory.systemCall=[](void* p) {++static_cast<Fixture*>(p)->barriers;};
        s.divideWord=[](void* p,int32_t a,int32_t b) {++*static_cast<unsigned*>(p);assert(b!=0 && !(a==std::numeric_limits<int32_t>::min() && b==-1));return a/b;};
        s.format=[](void* p,const EventDiagnosticCall& d) {static_cast<Fixture*>(p)->diagnostics.push_back(d);};
        s.call=[](void* p,EventEntry& e,uint32_t function,int32_t first) {auto& f=*static_cast<Fixture*>(p);f.calls.push_back({function,first});
            if(function==0x80001300)return first?0:2;
            assert(function==0x80001200);const auto s=f.services();f.observedInteger=getEventValue(e,sw(f.read(e.arguments)),s.operands);f.observedFloat=getEventFloat(e,lw+2,s.operands);return 2;
        };return s;
    }
    void emit(uint32_t& a,int op,std::initializer_list<int32_t> args) {write(a,uint32_t(args.size())<<16|uint32_t(op));a+=4;for(auto v:args) {write(a,uint32_t(v));a+=4;}}
    void end(uint32_t& a) {emit(a,1,{});write(a,0);} // Label scan reads terminal operand unconditionally.
    void tick() {gp.timestamp+=eventClockRate(clock);runEventInterpreterManager(services());}
};
}
int main() {
    Fixture f;uint32_t a=0x90000000;
    f.emit(a,50,{lw,3});f.emit(a,5,{lw});f.emit(a,53,{lw+1,1});f.emit(a,6,{});
    f.emit(a,50,{lw+7,9});f.emit(a,56,{lw+7,2});f.emit(a,57,{lw+7,2});
    f.emit(a,24,{lw+1,3});f.emit(a,34,{lw+1});f.emit(a,36,{3});f.emit(a,52,{lw+2,-229998464});f.emit(a,58,{lw+2,2});f.emit(a,49,{});
    f.emit(a,32,{});f.emit(a,50,{lw+1,99});f.emit(a,33,{});
    f.emit(a,62,{sw(0x90005000)});f.emit(a,63,{lw+3});f.emit(a,83,{lw+1,sw(0x90006000)});f.emit(a,85,{lw+4,sw(0x90006000)});
    f.emit(a,76,{8,lw+5});f.emit(a,74,{lw+5});f.emit(a,51,{-190000000,42});f.emit(a,94,{sw(0x90002000)});
    f.emit(a,118,{});f.emit(a,115,{lw+2});f.emit(a,91,{sw(0x80001200),lw+1});f.emit(a,2,{});f.end(a);
    a=0x90002000;f.emit(a,53,{lw+1,4});f.emit(a,50,{-70000000,1});f.emit(a,50,{-170000000,11});f.emit(a,91,{sw(0x80001300)});f.emit(a,2,{});f.end(a);
    f.write(0x90005000,uint32_t(lw+7));f.write(0x8041e808,0x600000);f.write(0x600000,0);f.write(0x600004,128);
    auto& parent=createRootEvent(f.schedule,0x90000000,10,0,f.creation());assert(f.runtime.allocationCount==1 && parent.id==1);
    f.tick();auto& child=f.entries[1];
    assert(parent.flags&0x10);assert(parent.waitingChild==&child && child.waitingParent==&parent && child.priority==11);
    assert(parent.locals[0]==0 && parent.locals[1]==3 && child.locals[1]==7 && parent.locals[3]==lw+7 && parent.locals[4]==3);
    assert(parent.locals[7]==0 && f.divisions==2); // Distinct divw and fctiwz service owners.
    assert(parent.userWork==0x600020 && child.userWork==parent.userWork && f.read(0x600020)==42 && f.read(0x3174)==11);
    assert(f.runtime.allocationCount==2 && f.schedule.count==2 && f.calls.size()==1 && f.calls[0].first==1 && f.calls[0].function==0x80001300);
    assert(f.flushes==std::vector<uint32_t>({0x600020}) && f.barriers==1);
    f.tick();assert(parent.flags==1 && parent.waitingChild==nullptr && child.flags==0 && parent.locals[1]==7 && parent.localFlags[0]==1);
    assert(f.runtime.allocationCount==1 && f.calls.size()==2 && f.calls[1].first==0 && parent.field150==0x90005004);
    // The higher-priority child finishes before the parent's live eligibility
    // check,so parent resumes and yields at118 in this same rebuilt pass.
    assert(parent.opcode==0 && f.diagnostics.empty() && f.calls.size()==2);
    f.tick();assert(parent.flags==0 && f.runtime.allocationCount==0 && f.observedInteger==7 && f.observedFloat==3.5f);
    assert(f.diagnostics.size()==1 && f.diagnostics[0].format==0x802c010c && f.diagnostics[0].floating==3.5f && f.diagnostics[0].saveFloatArguments);
    assert(f.calls.size()==3 && f.calls[2].first==1 && f.calls[2].function==0x80001200 && !f.schedule.managerActive);
    // A fractional speed accumulates manager credits;WAIT_WHILE is re-resolved
    // only on eligible dispatches,then real deletion releases the allocation.
    Fixture polling;a=0x90000000;polling.emit(a,11,{-90000000});polling.emit(a,2,{});polling.end(a);
    auto& slow=createRootEvent(polling.schedule,0x90000000,1,0,polling.creation());slow.timescale=.5f;polling.write(0x8084,1);
    polling.tick();assert(slow.opcode==0 && slow.credit==.5f);polling.tick();assert(slow.opcode==11 && slow.credit==0);
    polling.write(0x8084,0);polling.tick();assert(slow.opcode==11 && slow.credit==.5f);polling.tick();assert(slow.flags==0 && polling.runtime.allocationCount==0);
    // Independent root93 is not appended in the active pass;ID output uses the
    // actual local namespace. It runs from the next priority-table rebuild.
    Fixture roots;a=0x90000000;roots.emit(a,93,{sw(0x90002000),lw});roots.emit(a,9,{1});roots.emit(a,2,{});roots.end(a);
    a=0x90002000;roots.emit(a,50,{-170000000,77});roots.emit(a,2,{});roots.end(a);
    auto& root=createRootEvent(roots.schedule,0x90000000,10,0,roots.creation());roots.tick();assert(root.locals[0]==2 && roots.entries[1].flags==1 && roots.read(0x3174)==0 && roots.schedule.count==1);
    roots.tick();assert(roots.runtime.allocationCount==0 && roots.read(0x3174)==77);
    std::cout<<"combined event interpreter tests passed\n";
}
