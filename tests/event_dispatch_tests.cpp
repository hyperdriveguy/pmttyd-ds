#include "event_dispatch.hpp"
#include <cassert>
#include <map>
#include <vector>
#include <limits>
#include <iostream>
using namespace ttyd;
namespace {
uint32_t command(int count,int opcode) {return uint32_t(uint16_t(count))<<16|uint32_t(opcode);}
struct Fixture {
    EventEntry entry;
    std::map<uint32_t,uint32_t> memory;
    std::vector<int32_t> results,firstCalls,arguments,resolved,otherResults;
    int calls=0,others=0,deletes=0,clockReads=0;uint32_t clock=4000;
    bool mutateLifetime=false;
    Fixture() {entry.nextCommand=0x1000;entry.flags=1;}
    EventDispatchServices services() {
        EventDispatchServices s;s.user=this;
        s.word=[](void* p,uint32_t address) {auto& f=*static_cast<Fixture*>(p);assert(f.memory.count(address));return f.memory[address];};
        s.resolve=[](void* p,EventEntry& e,int32_t raw)->int32_t {auto& f=*static_cast<Fixture*>(p);f.resolved.push_back(raw);if(f.mutateLifetime)e.lifetime=999;return raw;};
        s.clock=[](void* p) {auto& f=*static_cast<Fixture*>(p);++f.clockReads;return f.clock;};
        s.call=[](void* p,EventEntry& e,uint32_t function,int32_t first)->int32_t {
            auto& f=*static_cast<Fixture*>(p);assert(function==0x12345678);f.firstCalls.push_back(first);
            assert(e.initialized!=0 && e.arguments==0x1008 && e.argumentCount==1);
            f.arguments.push_back(f.memory[e.arguments]);assert(f.calls<int(f.results.size()));return f.results[f.calls++];
        };
        s.remove=[](void* p,EventEntry& e) {auto& f=*static_cast<Fixture*>(p);++f.deletes;e={};};
        s.handler=[](void* p,EventEntry&,uint8_t opcode)->int32_t {auto& f=*static_cast<Fixture*>(p);assert(opcode==3);assert(f.others<int(f.otherResults.size()));return f.otherResults[f.others++];};return s;
    }
    int run() {return dispatchEvent(entry,services());}
    void frame(int count) {memory[0x1000]=command(1,9);memory[0x1004]=uint32_t(count);memory[0x1008]=command(0,2);}
    void user() {memory[0x1000]=command(2,91);memory[0x1004]=0x12345678;memory[0x1008]=5;memory[0x100c]=command(0,2);}
};
}
int main() {
    Fixture zero;zero.frame(0);assert(zero.run()==-1 && zero.deletes==1 && zero.resolved==std::vector<int32_t>({0}));
    Fixture one;one.frame(1);assert(one.run()==0 && one.entry.opcode==0 && one.entry.waitCount==0 && one.entry.initialized==1);
    assert(one.run()==-1 && one.deletes==1);
    Fixture three;three.frame(3);assert(three.run()==0 && three.entry.opcode==9 && three.entry.waitCount==2);
    three.memory[0x1004]=0;assert(three.run()==0 && three.entry.waitCount==1);
    assert(three.run()==0 && three.entry.opcode==0 && three.resolved==std::vector<int32_t>({3}));
    assert(three.run()==-1);
    Fixture negative;negative.frame(-1);assert(negative.run()==0 && negative.entry.waitCount==-2 && negative.entry.opcode==9);
    negative.entry.waitCount=std::numeric_limits<int32_t>::min();negative.run();assert(negative.entry.waitCount==std::numeric_limits<int32_t>::max());
    Fixture user;user.user();user.results={0,1};
    assert(user.run()==0 && user.entry.opcode==91 && user.entry.currentCommand==0x1000 && user.entry.nextCommand==0x100c);
    user.memory[0x1004]=0xffffffff;user.memory[0x1008]=9;
    user.entry.initialized=3; // Callback-owned motion phase still means resumed USER_FUNC.
    assert(user.run()==0 && user.entry.opcode==0 && user.firstCalls==std::vector<int32_t>({1,0}) && user.arguments==std::vector<int32_t>({5,9}));
    assert(user.resolved==std::vector<int32_t>({0x12345678}) && user.run()==-1);
    Fixture immediate;immediate.user();immediate.results={2};assert(immediate.run()==-1 && immediate.deletes==1);
    for(int result:{-1,-2,std::numeric_limits<int32_t>::min()}) {
        Fixture abort;abort.user();abort.results={result};assert(abort.run()==1 && abort.entry.opcode==91 && abort.deletes==0);
    }
    Fixture stop;stop.user();stop.results={255};assert(stop.run()==-1 && stop.deletes==0 && stop.entry.opcode==91);
    for(int result:{3,4}) {
        Fixture repeat;repeat.user();repeat.results={result,2};assert(repeat.run()==-1 && repeat.firstCalls==std::vector<int32_t>({1,0}));
    }
    Fixture unknown;unknown.memory[0x1000]=command(0,250);unknown.memory[0x1004]=command(0,2);
    assert(unknown.run()==-1 && unknown.others==0);
    Fixture signedCount;signedCount.memory[0x1000]=command(-1,3);signedCount.otherResults={0};
    assert(signedCount.run()==0 && signedCount.entry.nextCommand==0x1000 && signedCount.entry.arguments==0x1004 && signedCount.entry.argumentCount==255);
    Fixture marker;marker.user();marker.memory[0x100c]=command(1,9);marker.memory[0x1010]=1;marker.memory[0x1014]=command(0,2);marker.results={2};
    assert(marker.run()==0 && marker.entry.waitCount==0 && marker.entry.opcode==0);
    assert(marker.resolved==std::vector<int32_t>({0x12345678,1}));
    Fixture ms;ms.entry.arguments=0x1004;ms.entry.lifetime=123;ms.memory[0x1004]=10;ms.mutateLifetime=true;
    assert(eventWaitMilliseconds(ms.entry,ms.services())==0 && ms.entry.waitStart==123 && ms.entry.lifetime==999);
    ms.mutateLifetime=false;ms.entry.lifetime=132;assert(eventWaitMilliseconds(ms.entry,ms.services())==0);
    ms.entry.lifetime=133;assert(eventWaitMilliseconds(ms.entry,ms.services())==1 && ms.resolved.size()==1);
    Fixture msZero;msZero.entry.arguments=0x1004;msZero.memory[0x1004]=0;auto noClock=msZero.services();noClock.clock=nullptr;
    assert(eventWaitMilliseconds(msZero.entry,noClock)==2 && msZero.clockReads==0);
    Fixture low;low.entry.initialized=1;low.entry.waitCount=32;low.entry.waitStart=0xfffffff0;
    low.entry.lifetime=0x100000010;assert(eventWaitMilliseconds(low.entry,low.services())==1);
    low.entry.waitStart=0x77777777fffffff0ULL;assert(eventWaitMilliseconds(low.entry,low.services())==1); // Start high ignored.
    Fixture negativeElapsed;negativeElapsed.entry.initialized=1;negativeElapsed.entry.waitCount=3;
    negativeElapsed.entry.waitStart=5;negativeElapsed.entry.lifetime=4;
    assert(eventWaitMilliseconds(negativeElapsed.entry,negativeElapsed.services())==0);
    negativeElapsed.clock=12000;assert(eventWaitMilliseconds(negativeElapsed.entry,negativeElapsed.services())==1); // Unsigned64 division, low quotient.
    negativeElapsed.entry.waitCount=-1;negativeElapsed.entry.waitStart=4;assert(eventWaitMilliseconds(negativeElapsed.entry,negativeElapsed.services())==1);
    // Real manager credits drive fetch/wait/user continuation against one sample.
    struct Integration {
        Fixture script;EventGlobals gp;EventContext context;EventSchedule schedule;
        std::vector<int> seen;int inputSample=77;
        Integration():context{1,&script.entry,0} {gp.refreshRate=60;gp.timestamp=100;script.frame(1);script.entry.id=1;script.entry.timescale=2;}
    } integration;
    EventManagerServices manager;manager.user=&integration;
    manager.globals=[](void* p) {return &static_cast<Integration*>(p)->gp;};
    manager.context=[](void* p,bool) {return &static_cast<Integration*>(p)->context;};
    manager.clock=[](void*)->uint32_t {return 4000;};
    manager.dispatch=[](void* p,EventEntry& entry)->int32_t {auto& i=*static_cast<Integration*>(p);i.seen.push_back(i.inputSample);return dispatchEvent(entry,i.script.services());};
    runEventManager(integration.schedule,manager);
    assert(integration.seen==std::vector<int>({77,77}) && integration.script.deletes==1 && !integration.schedule.managerActive);
    Integration delayed;delayed.script.memory[0x1000]=command(1,10);delayed.script.memory[0x1004]=1;delayed.script.entry.timescale=2;
    delayed.gp.timestamp=1;manager.user=&delayed;runEventManager(delayed.schedule,manager);
    assert(delayed.script.entry.opcode==10 && delayed.script.entry.waitStart==1 && delayed.script.entry.lifetime==1 && delayed.script.clockReads==2);
    delayed.gp.timestamp=2;runEventManager(delayed.schedule,manager);assert(delayed.script.deletes==1);
    std::cout<<"event dispatch tests passed\n";
}
