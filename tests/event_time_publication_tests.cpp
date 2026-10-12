#include "event_time_publication.hpp"
#include "event_dispatch.hpp"
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
    std::map<uint32_t,uint16_t> halves;
    std::vector<Op> operations;
    uint64_t sample=110;
    std::function<void(uint32_t)> onRead;
    std::function<void(uint32_t,uint32_t)> onWrite;
    Fixture() {words[0x80415fd0]=0x1000;words[0x800000f8]=243000000;put64(0x1030,100);}
    void put64(uint32_t a,uint64_t v) {words[a]=uint32_t(v>>32);words[a+4]=uint32_t(v);}
    uint64_t get64(uint32_t a) {return uint64_t(words[a])<<32|words[a+4];}
    EventTimeServices services() {
        EventTimeServices s;s.user=this;s.memory.user=this;
        s.upper=[](void* p) {auto& f=*static_cast<Fixture*>(p);f.operations.push_back({'u',0,uint32_t(f.sample>>32)});return uint32_t(f.sample>>32);};
        s.lower=[](void* p) {auto& f=*static_cast<Fixture*>(p);f.operations.push_back({'l',0,uint32_t(f.sample)});return uint32_t(f.sample);};
        s.memory.word=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);if(f.onRead)f.onRead(a);const uint32_t v=f.words[a];f.operations.push_back({'r',a,v});return v;};
        s.memory.storeWord=[](void* p,uint32_t a,uint32_t v) {auto& f=*static_cast<Fixture*>(p);f.operations.push_back({'w',a,v});f.words[a]=v;if(f.onWrite)f.onWrite(a,v);};
        s.memory.half=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);const uint16_t v=f.halves[a];f.operations.push_back({'h',a,v});return v;};return s;
    }
    void publish() {publishEventRetraceTime(99,services());}
};
}
int main() {
    struct Rollover {std::vector<uint32_t> upper{1,2,2,2},lower{0xfffffffe,7};unsigned u=0,l=0;std::vector<char> order;};
    Rollover r;EventTimeServices raw;raw.user=&r;raw.upper=[](void* p) {auto& r=*static_cast<Rollover*>(p);r.order.push_back('u');return r.upper.at(r.u++);};raw.lower=[](void* p) {auto& r=*static_cast<Rollover*>(p);r.order.push_back('l');return r.lower.at(r.l++);};
    assert(readEventRawTime(raw)==0x200000007 && r.order==std::vector<char>({'u','l','u','u','l','u'}));
    Fixture normal;normal.publish();assert(normal.get64(0x1028)==10 && normal.get64(0x1020)==10 && normal.get64(0x1030)==110 && normal.words[0x101c]==99);
    for(uint32_t bank:{0x60u,0x58u,0x50u,0x48u,0x38u,0x40u})assert(normal.get64(0x1000+bank)==10);
    std::vector<uint32_t> writes;for(auto op:normal.operations)if(op.kind=='w')writes.push_back(op.a);
    assert(writes==std::vector<uint32_t>({0x102c,0x1028,0x1024,0x1020,0x1064,0x1060,0x105c,0x1058,0x1054,0x1050,0x104c,0x1048,0x103c,0x1038,0x1044,0x1040,0x1034,0x1030,0x101c}));
    for(uint32_t mask:{1u,2u,4u,8u,15u,0x80000000u}) {
        Fixture f;f.words[0x1018]=mask;f.words[0x1014]=1;f.publish();const uint32_t banks[]{0x48,0x50,0x58,0x60};
        for(unsigned i=0;i<4;++i)assert(f.get64(0x1000+banks[i])==((mask&(1u<<i))?0:10));
        assert(f.get64(0x1038)==10 && f.get64(0x1040)==0);
    }
    Fixture readiness;readiness.words[0x1010]=1;readiness.words[0x8041ec30]=0x8000;readiness.publish();assert(readiness.get64(0x1020)==10 && readiness.get64(0x1038)==0 && readiness.get64(0x1030)==110);
    for(auto op:readiness.operations)assert(op.a!=0x8041ec30); // Short circuit before winCheck.
    for(int32_t state:{0,-1,1,199,200,201})for(uint16_t flag:{uint16_t(0),uint16_t(0x1000)}) {
        Fixture f;f.words[0x8041ec30]=0x8000;f.halves[0x8000]=flag;f.words[0x8020]=uint32_t(state);const bool freeze=flag==0 && state!=0 && state<200;
        assert(eventWindowFreeze(f.services().memory)==(freeze?1:0));f.publish();assert(f.get64(0x1038)==(freeze?0:10) && f.get64(0x1030)==110);
    }
    Fixture negative;negative.sample=95;negative.publish();assert(negative.get64(0x1028)==uint64_t(0)-5 && negative.get64(0x1020)==uint64_t(0)-5 && negative.get64(0x1038)==uint64_t(0)-5);
    const uint64_t limit=0x157529ff;
    Fixture cap;cap.words[0x800000f8]=4;cap.sample=100;cap.put64(0x1020,limit+1);cap.publish();assert(cap.get64(0x1020)==limit && cap.get64(0x1038)==0);
    Fixture equality;equality.words[0x800000f8]=4;equality.sample=100;equality.put64(0x1020,limit);equality.publish();unsigned totalWrites=0;for(auto op:equality.operations)if(op.kind=='w' && op.a==0x1024)++totalWrites;assert(totalWrites==1);
    Fixture carry;carry.put64(0x1060,0xfffffffc);carry.publish();assert(carry.get64(0x1060)==0x100000006);
    // Clock is read BEFORE the cap group's fresh GP pointer. Later groups use
    // current GP/live delta and masks;one group keeps its captured GP through
    // both stores even if the low store changes the global pointer.
    Fixture live;live.words[0x800000f8]=4;live.put64(0x2020,limit+100);live.put64(0x2028,3);live.put64(0x3028,9);live.words[0x3018]=15;live.words[0x3014]=1;
    live.onRead=[&](uint32_t a) {if(a==0x800000f8)live.words[0x80415fd0]=0x2000;};
    live.onWrite=[&](uint32_t a,uint32_t) {if(a==0x2064)live.words[0x80415fd0]=0x3000;if(a==0x3030)live.words[0x80415fd0]=0x4000;};
    live.publish();assert(live.get64(0x1020)==10 && live.get64(0x2020)==limit && live.get64(0x2060)==3 && live.get64(0x3038)==9 && live.get64(0x3040)==0 && live.get64(0x3030)==110 && live.words[0x401c]==99 && live.words[0x301c]==0);
    // Compose admitted raw time with the actual event manager/millisecond wait.
    // Frozen retraces update baseline;normal updates resume without catch-up.
    Fixture timing;timing.put64(0x1030,0);EventEntry e;e.flags=1;e.id=1;e.priority=1;e.timescale=1;e.opcode=10;e.arguments=0x9000;e.nextCommand=0x9008;
    timing.words[0x9000]=3;timing.words[0x9008]=2;EventGlobals g{0,60,0};EventContext c{1,&e,0};EventSchedule schedule;
    struct Bridge {Fixture* f;EventGlobals* g;EventContext* c;};Bridge bridge{&timing,&g,&c};EventManagerServices m;m.user=&bridge;
    m.globals=[](void* p) {auto& b=*static_cast<Bridge*>(p);b.g->timestamp=b.f->get64(0x1038);return b.g;};m.context=[](void* p,bool) {return static_cast<Bridge*>(p)->c;};m.clock=[](void* p) {return static_cast<Bridge*>(p)->f->words[0x800000f8];};
    m.dispatch=[](void* p,EventEntry& e) {auto& b=*static_cast<Bridge*>(p);EventDispatchServices s;s.user=b.f;s.word=b.f->services().memory.word;s.resolve=[](void*,EventEntry&,int32_t v) {return v;};s.clock=[](void* p) {return static_cast<Fixture*>(p)->words[0x800000f8];};s.remove=[](void*,EventEntry& e) {e.flags=0;};return dispatchEvent(e,s);};
    const uint64_t k=eventClockRate(timing.words[0x800000f8]);timing.sample=k;timing.publish();runEventManager(schedule,m);assert(e.waitStart==k && e.opcode==10);
    timing.words[0x1010]=1;timing.sample=2*k;timing.publish();assert(timing.get64(0x1038)==k && timing.get64(0x1030)==2*k); // Readiness barrier admits no main update.
    timing.words[0x1010]=0;timing.sample=3*k;timing.publish();runEventManager(schedule,m);assert(e.lifetime==2*k && e.opcode==10);
    timing.sample=4*k;timing.publish();runEventManager(schedule,m);assert(e.lifetime==3*k && e.opcode==10);
    timing.sample=5*k;timing.publish();runEventManager(schedule,m);assert(e.lifetime==4*k && e.opcode==0 && e.flags==1);
    timing.sample=6*k;timing.publish();runEventManager(schedule,m);assert(e.flags==0);
    std::cout<<"event time publication tests passed\n";
}
