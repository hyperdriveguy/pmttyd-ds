#include "battle_action_services.hpp"
#include "battle_timing_a.hpp"
#include "battle_hammer.hpp"
#include <cassert>
#include <limits>
#include <vector>
#include <iostream>
using namespace ttyd;
struct Fixture {
    int32_t state=99,count=4;
    uint32_t result=1,timing=0x80,flags=0x1234,actorFlags=0;
    std::array<int32_t,8> parameters{{1,2,3,4,5,6,7,8}};
    std::array<int32_t,8> resolved{{11,-250000000,13,14,-250000000,16,17,18}};
    std::vector<int> order;
    int calls=0,outputDestination=0;int32_t outputValue=0,callbackReturn=99;
    bool callbackClears=false;
    ActionCommandView view() {
        ActionCommandView w;w.state=&state;w.result=&result;w.count=&count;
        w.timingOptions=&timing;w.setupFlags=&flags;w.parameters=&parameters;
        w.actorFlags=&actorFlags;w.user=this;
        w.evaluate=[](void* p) {auto& f=*static_cast<Fixture*>(p);f.order.push_back(20);++f.calls;if(f.callbackClears)f.result=2;return f.callbackReturn;};
        w.remove=[](void* p) {auto& f=*static_cast<Fixture*>(p);f.order.push_back(21);f.state=1000;};
        w.help=[](void* p,int32_t type) {auto& f=*static_cast<Fixture*>(p);f.order.push_back(30+type);};
        w.successWindow=[](void* p) {auto& f=*static_cast<Fixture*>(p);f.order.push_back(22);return 8;};
        return w;
    }
    ActionValueServices values() {
        ActionValueServices s;s.user=this;
        s.resolve=[](void* p,int32_t raw) {auto& f=*static_cast<Fixture*>(p);f.order.push_back(raw);assert(raw>=0 && raw<8);
            if(raw==1)assert(f.parameters[0]==11);return f.resolved[raw];};
        s.store=[](void* p,int32_t raw,int32_t value) {auto& f=*static_cast<Fixture*>(p);f.order.push_back(40);f.outputDestination=raw;f.outputValue=value;};
        return s;
    }
};
int main() {
    Fixture f;auto w=f.view();auto values=f.values();
    const std::array<int32_t,8> operands{{0,1,2,3,4,5,6,7}};
    assert(setActionParameters(w,operands,values)==2);
    assert(f.parameters==std::array<int32_t,8>({11,2,13,14,5,16,17,18}));
    assert(f.order==std::vector<int>({0,1,2,3,4,5,6,7}));
    f.order.clear();assert(setActionFlags(w,0xf1194d80)==2);
    assert(f.flags==0xf1194d80 && f.timing==0x80 && f.order.empty());
    assert(setTimingOptions(w,0,1,values)==2 && f.timing==0x81 && f.flags==0xf1194d80);
    f.resolved[0]=0;assert(setTimingOptions(w,0,1,values)==2 && f.timing==0x80);
    f.resolved[0]=1;f.order.clear();assert(startActionCommand(w,0,values)==2);
    assert(f.state==100 && f.order==std::vector<int>({0,32}));
    f.actorFlags=0x10;f.order.clear();startActionCommand(w,0,values);
    assert(f.order==std::vector<int>({0}));
    f.resolved[0]=0;w.actorFlags=nullptr;f.order.clear();startActionCommand(w,0,values);
    assert(f.order==std::vector<int>({0}));
    f.order.clear();f.callbackReturn=2; // Return2 alone cannot unblock pending1.
    assert(resultActionCommandEvent(w)==0 && f.order==std::vector<int>({20}));
    f.callbackReturn=0;f.callbackClears=true;f.order.clear();
    assert(resultActionCommandEvent(w)==2 && f.order==std::vector<int>({20,30}));
    w.evaluate=nullptr;f.result=0x80000003;f.order.clear();
    assert(evaluateActionCommand(w)==2 && f.order.empty());
    assert(readActionResult(w,-123456,values)==2 && f.outputDestination==-123456 && f.outputValue==-2147483645);
    assert(f.order==std::vector<int>({40}) && f.calls==2 && f.result==0x80000003);
    f.order.clear();readActionCount(w,777,values);assert(f.outputValue==4 && f.count==4);
    assert(f.order==std::vector<int>({40}));
    f.order.clear();readTimingSuccessWindow(w,888,values);
    assert(f.outputDestination==888 && f.outputValue==8 && f.order==std::vector<int>({22,40}));
    f.order.clear();stopActionCommandEvent(w);
    assert(f.state==1000 && f.result==0x80000003 && f.count==4 && f.order==std::vector<int>({21,30}));
    w.remove=nullptr;f.order.clear();stopActionCommandEvent(w);assert(f.order==std::vector<int>({30}));

    // Repeated stop requests restart TimingA finish; result and count persist.
    TimingACommand timing;timing.state=100;timing.result=2;timing.count=2;
    int helpType=-1;
    struct Context {TimingACommand* command;int* help;};Context context{&timing,&helpType};
    ActionCommandView bound;bound.state=&timing.state;bound.result=&timing.result;bound.count=&timing.count;bound.user=&context;
    bound.remove=[](void* p) {static_cast<Context*>(p)->command->state=1000;};
    bound.help=[](void* p,int32_t type) {*static_cast<Context*>(p)->help=type;};
    stopActionCommandEvent(bound);assert(timing.callbacksLive && helpType==0 && timing.count==2);
    timing.tick();assert(timing.finishCountdown==59);
    stopActionCommandEvent(bound);timing.tick();assert(timing.finishCountdown==59);
    for(int n=0;n<59;++n)timing.tick();assert(timing.state==1002 && timing.callbacksLive);
    timing.tick();assert(!timing.callbacksLive && timing.result==2 && timing.count==2);

    struct PrizeFixture {
        std::vector<int> order;ActionPrizeActor actor;uint8_t grade=0;int byteReads=0;
        int32_t actorId=7,input=0,dest=0,value=0;int captured=1,lookup=2;
    } p;
    ActionPrizeServices prize;prize.user=&p;
    prize.captureWork=[](void* u)->void* {auto& p=*static_cast<PrizeFixture*>(u);p.order.push_back(1);return &p.captured;};
    prize.resolve=[](void* u,int32_t raw) {auto& p=*static_cast<PrizeFixture*>(u);p.order.push_back(raw);return raw==10?p.actorId:p.input;};
    prize.translate=[](void* u,int32_t id) {auto& p=*static_cast<PrizeFixture*>(u);p.order.push_back(3);return id+1;};
    prize.unit=[](void* u,int32_t id) {auto& p=*static_cast<PrizeFixture*>(u);p.order.push_back(4);assert(id==8);p.lookup=3;return &p.actor;};
    prize.prizeByte=[](void* u,void* captured) {auto& p=*static_cast<PrizeFixture*>(u);p.order.push_back(5);assert(captured==&p.captured);++p.byteReads;return p.grade;};
    prize.store=[](void* u,int32_t raw,int32_t value) {auto& p=*static_cast<PrizeFixture*>(u);p.order.push_back(6);p.dest=raw;p.value=value;};
    assert(readActionPrize({{10,11,int32_t(-30000000+6)}},prize)==2);
    assert(p.dest==-30000000+6 && p.value==0 && p.order==std::vector<int>({1,10,3,4,11,5,6}));
    for(auto byte:{0,1,7,255}) {
        p.grade=byte;assert(actionPrizeLevel(&p.captured,nullptr,0,prize)==(byte<2?0:6));
    }
    p.grade=255;assert(actionPrizeLevel(&p.captured,nullptr,std::numeric_limits<int32_t>::max(),prize)==0);
    p.actor.lastHitCode=0x1234001e;const int reads=p.byteReads;
    assert(actionPrizeLevel(&p.captured,&p.actor,99,prize)==-1 && p.byteReads==reads);
    p.actor.lastHitCode=0x1234011e;assert(actionPrizeLevel(&p.captured,&p.actor,0,prize)==-1);
    for(int id:{-1,-5}) {
        p.actorId=id;p.order.clear();readActionPrize({{10,11,22}},prize);
        assert(p.value==6 && p.order==std::vector<int>({1,10,11,5,6}));
    }
    std::cout<<"action command service tests passed\n";
}
