#include "event_switch_opcodes.hpp"
#include "event_data_opcodes.hpp"
#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
uint32_t command(int n,int op) {return uint32_t(uint16_t(n))<<16|uint32_t(op);}
struct Fixture {
    EventEntry e;
    std::map<uint32_t,uint32_t> memory;
    std::vector<int32_t> queries;
    std::vector<uint32_t> reads;
    std::function<void(int32_t)> onQuery;
    std::function<void(uint32_t)> onRead;
    Fixture() {e.arguments=0x1000;e.nextCommand=0x2000;e.switchIndex=0;e.switchStates[0]=1;e.switchValues[0]=7;memory[0x1000]=7;memory[0x1004]=9;memory[0x2000]=command(0,36);memory[0x2004]=command(0,49);}
    EventDispatchServices services() {
        EventDispatchServices s;s.user=this;
        s.word=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);if(f.onRead)f.onRead(a);return f.memory.at(a);};
        s.resolve=[](void* p,EventEntry& e,int32_t op) {auto& f=*static_cast<Fixture*>(p);f.queries.push_back(op);if(f.onQuery)f.onQuery(op);return op>=-30000000 && op<-29999984?e.locals[op+30000000]:op;};
        s.store=[](void*,EventEntry& e,int32_t d,int32_t v) {assert(d>=-30000000 && d<-29999984);e.locals[d+30000000]=v;};
        return s;
    }
    void run(uint8_t op) {assert(eventSwitchOpcode(e,op,services())==2);}
};
bool relation(uint8_t op,int v,int k) {switch(op) {case 36:return v==k;case 37:return v!=k;case 38:return v<k;case 39:return v>k;case 40:return v<=k;default:return v>=k;}}
}
int main() {
    Fixture unsupported;assert(!eventSwitchOpcode(unsupported.e,33,unsupported.services()));assert(!eventSwitchOpcode(unsupported.e,50,unsupported.services()));assert(unsupported.reads.empty());
    Fixture push;push.e.switchIndex=255;push.memory[0x1000]=uint32_t(-30000000);push.e.locals[0]=42;push.run(34);
    assert(push.e.switchIndex==0 && push.e.switchValues[0]==42 && push.e.switchStates[0]==1 && push.queries==std::vector<int32_t>({-30000000}));
    push.e.switchIndex=255;push.queries.clear();push.run(35);assert(push.e.switchValues[0]==-30000000 && push.queries.empty());
    Fixture lateIndex;lateIndex.e.switchIndex=255;lateIndex.onQuery=[&](int32_t) {lateIndex.e.switchIndex=0;};lateIndex.run(34);assert(lateIndex.e.switchIndex==1 && lateIndex.e.switchValues[1]==7 && lateIndex.e.switchStates[1]==1);
    Fixture earlyIndex;earlyIndex.e.switchIndex=255;earlyIndex.onRead=[&](uint32_t a) {if(a==0x1000)earlyIndex.e.switchIndex=0;};earlyIndex.run(35);assert(earlyIndex.e.switchIndex==0 && earlyIndex.e.switchValues[0]==7);
    push.run(49);assert(push.e.switchIndex==255 && push.e.switchStates[0]==0 && push.e.switchValues[0]==-30000000);
    for(uint8_t op=36;op<=41;++op)for(auto pair:std::vector<std::pair<int,int>>{{-7,2},{2,-7},{2,2}}) {
        Fixture f;f.e.switchValues[0]=pair.first;f.memory[0x1000]=uint32_t(pair.second);f.e.nextCommand=0x1ffc;f.memory[0x1ffc]=command(0,53);f.run(op);
        const bool accepted=relation(op,pair.first,pair.second);assert(f.e.switchStates[0]==(accepted?0:1) && f.e.nextCommand==(accepted?0x1ffcu:0x2000u));
        assert(f.queries==std::vector<int32_t>({pair.second}));
    }
    Fixture retained;retained.e.switchValues[0]=99;retained.onQuery=[&](int32_t) {retained.e.switchIndex=1;retained.e.switchValues[0]=7;retained.e.switchStates[1]=1;};retained.run(36);
    assert(retained.e.switchStates[0]==0 && retained.e.switchStates[1]==1 && retained.e.switchIndex==1 && retained.e.nextCommand==0x2000);
    for(uint8_t state:{uint8_t(0),uint8_t(255),uint8_t(254),uint8_t(128)}) {
        Fixture f;f.e.switchStates[0]=state;f.run(36);assert(f.queries.size()==1 && f.e.switchStates[0]==state && f.e.nextCommand==0x2004);
        Fixture d;d.e.switchStates[0]=state;d.run(42);assert(d.queries.empty() && d.e.nextCommand==0x2004);
    }
    Fixture defaultCase;defaultCase.run(42);assert(defaultCase.e.switchStates[0]==0 && defaultCase.reads.empty() && defaultCase.queries.empty());
    // End scan counts resolved SWITCH34 and terminal1 but ignores35/DELETE2.
    Fixture endScan;endScan.memory[0x2000]=command(0,35);endScan.memory[0x2004]=command(0,34);endScan.memory[0x2008]=command(0,49);endScan.memory[0x200c]=command(0,1);endScan.memory[0x2010]=command(0,2);endScan.memory[0x2014]=command(0,49);endScan.memory[0x2018]=command(0,49);
    endScan.run(48);assert(endScan.e.nextCommand==0x2018 && endScan.e.switchIndex==0 && endScan.e.switchStates[0]==1);
    endScan.run(49);assert(endScan.e.switchIndex==255 && endScan.e.switchStates[0]==0);
    Fixture nextScan;nextScan.memory[0x1000]=8;nextScan.memory[0x2000]=command(0,34);nextScan.memory[0x2004]=command(0,36);nextScan.memory[0x2008]=command(0,49);nextScan.memory[0x200c]=command(0,35);nextScan.memory[0x2010]=command(0,48);nextScan.memory[0x2014]=command(0,46);
    nextScan.run(36);assert(nextScan.e.nextCommand==0x2014 && nextScan.e.switchStates[0]==1);
    Fixture stride;stride.e.nextCommand=0x2000;stride.memory[0x2000]=command(-2,53);stride.memory[0x1ffc]=command(0,49);stride.run(48);assert(stride.e.nextCommand==0x1ffc);
    for(uint8_t state:{uint8_t(0),uint8_t(1),uint8_t(255),uint8_t(254),uint8_t(128)})for(int candidate:{7,8}) {
        Fixture f;f.e.switchStates[0]=state;f.memory[0x1000]=uint32_t(candidate);f.e.nextCommand=0x1ffc;f.memory[0x1ffc]=command(0,53);f.run(43);
        const uint8_t expected=state==0?0:candidate==7?255:state;
        const uint32_t next=state==0?0x2004:candidate==7 || state==255?0x1ffc:0x2000;
        assert(f.e.switchStates[0]==expected && f.e.nextCommand==next && f.queries.size()==1);
        Fixture g;g.e.switchStates[0]=state;g.memory[0x1000]=uint32_t(candidate);g.e.nextCommand=0x1ffc;g.memory[0x1ffc]=command(0,53);g.run(44);
        const uint8_t andState=state==0?0:state==254?254:candidate==7?255:254;
        const uint32_t andNext=state==0?0x2004:state==254 || candidate!=7?0x2000:0x1ffc;
        assert(g.e.switchStates[0]==andState && g.e.nextCommand==andNext && g.queries.size()==1);
    }
    for(uint8_t state:{uint8_t(0),uint8_t(1),uint8_t(255),uint8_t(254),uint8_t(128)}) {
        Fixture f;f.e.switchStates[0]=state;f.run(46);assert(f.e.switchStates[0]==(state==0 || state==255?0:1) && f.e.nextCommand==(state==0 || state==255?0x2004u:0x2000u) && f.queries.empty());
    }
    for(auto pair:std::vector<std::pair<uint32_t,uint32_t>>{{2,3},{0x80000000,0x80000000},{7,0},{1,uint32_t(-30000000)}}) {
        Fixture f;f.e.switchValues[0]=int32_t(pair.first);f.memory[0x1000]=pair.second;f.e.nextCommand=0x1ffc;f.memory[0x1ffc]=command(0,53);f.run(45);
        const bool accepted=(pair.first&pair.second)!=0;assert(f.queries.empty() && f.e.switchStates[0]==(accepted?0:1) && f.e.nextCommand==(accepted?0x1ffcu:0x2000u));
    }
    for(auto range:std::vector<std::pair<int,int>>{{7,9},{5,7},{7,7},{9,5},{-9,-1}}) {
        Fixture f;f.memory[0x1000]=uint32_t(range.first);f.memory[0x1004]=uint32_t(range.second);f.e.nextCommand=0x1ffc;f.memory[0x1ffc]=command(0,53);f.run(47);
        const bool accepted=range.first<=7 && 7<=range.second;assert(f.e.switchStates[0]==(accepted?0:1) && f.e.nextCommand==(accepted?0x1ffcu:0x2000u) && f.queries.size()==2);
    }
    Fixture between;between.memory[0x1000]=5;between.memory[0x1004]=1;between.e.switchStates[0]=0;
    between.onQuery=[&](int32_t op) {if(op==5) {between.memory[0x1004]=9;between.e.arguments=0x9999;between.e.switchIndex=1;}};
    between.run(47);assert(between.queries==std::vector<int32_t>({5,9}) && between.e.nextCommand==0x2004 && between.e.switchIndex==1);
    // Execute real OR/AND groups through scan/refetch,assignment and END_SWITCH.
    for(int mode:{0,1,2}) {
        Fixture program;program.e.switchIndex=255;uint32_t a=0x3000;
        auto emit=[&](int op,std::initializer_list<int32_t> args) {program.memory[a]=command(args.size(),op);a+=4;for(int32_t v:args) {program.memory[a]=uint32_t(v);a+=4;}};
        emit(34,{7});const int group=mode==0?43:44;
        emit(group,{mode==0?8:7});emit(group,{mode==2?8:7});emit(50,{-30000000,10});emit(46,{});emit(42,{});emit(50,{-30000000,20});emit(49,{});emit(9,{1});
        program.e.nextCommand=0x3000;auto dispatcher=program.services();dispatcher.handler=[](void* p,EventEntry& e,uint8_t op) {auto& f=*static_cast<Fixture*>(p);if(auto r=eventSwitchOpcode(e,op,f.services()))return *r;EventDataOpcodeServices s;s.values=f.services();auto r=eventDataOpcode(e,op,s);assert(r);return *r;};
        assert(dispatchEvent(program.e,dispatcher)==0 && program.e.locals[0]==(mode==2?20:10) && program.e.switchIndex==255 && program.e.switchStates[0]==0 && program.e.switchValues[0]==7);
    }
    std::cout<<"event SWITCH opcode tests passed\n";
}
