#include "event_native_runtime.hpp"
#include "event_memory_bootstrap.hpp"
#include <cassert>
#include <cmath>
#include <iostream>
using namespace ttyd;
struct Platform {
    EventNativeRuntime* runtime=nullptr;
    unsigned flushes=0,barriers=0,calls=0,truncations=0,divisions=0;
    std::vector<int32_t> firstCalls;std::vector<uint32_t> retired;
    EventNativePlatform services() {
        EventNativePlatform p;p.user=this;
        p.clock=[](void*)->uint32_t {return 243000000;};
        p.truncate=[](void* p,float v) {++static_cast<Platform*>(p)->truncations;assert(std::isfinite(v) && double(v)>=-2147483648. && double(v)<2147483648.);return int32_t(v);};
        p.divideWord=[](void* p,int32_t a,int32_t b) {++static_cast<Platform*>(p)->divisions;assert(b!=0);return a/b;};
        p.call=[](void* p,EventEntry& e,uint32_t function,int32_t first) {auto& f=*static_cast<Platform*>(p);assert(function==0x80001200);++f.calls;f.firstCalls.push_back(first);assert(e.locals[0]==7);return first?0:2;};
        p.retiring=[](void* p,const EventEntry& e){static_cast<Platform*>(p)->retired.push_back(e.id);};
        p.flushBlock=[](void* p,uint32_t) {++static_cast<Platform*>(p)->flushes;};p.systemCall=[](void* p) {++static_cast<Platform*>(p)->barriers;};return p;
    }
};
void emit(EventNativeRuntime& r,uint32_t& a,uint8_t op,std::initializer_list<int32_t> args) {
    assert(r.memory.writeWord(a,uint32_t(args.size())<<16|op));a+=4;
    for(auto v:args) {assert(r.memory.writeWord(a,uint32_t(v)));a+=4;}
}
int main(int argc,char** argv) {
    Platform platform;EventNativeRuntime r(0x81000000,platform.services());platform.runtime=&r;
    // Authored native arena input;SDK descriptors and handle table are shared.
    assert(r.memory.registerRam(0x82000000,std::vector<uint8_t>(1024*1024)) && r.memory.registerRam(0x8041f070,std::vector<uint8_t>(16)) && r.memory.registerRam(0x8041e428,std::vector<uint8_t>(4)) && r.memory.registerRam(0x8041e844,std::vector<uint8_t>(4)));
    auto m=r.memoryBinding.services();const auto start=eventSdkInitializeAllocator(0x82000000,0x82100000,1,m);assert(eventSdkCreateHeap(start,0x82100000,m)==0 && r.memory.writeWord(0x8041e844,0));
    assert(r.memory.writeWord(0x81000004,60));r.initializeContexts();assert(platform.barriers==2 && r.field.capacity==256 && r.battle.capacity==256);
    auto creation=r.creationServices();
    if(argc==2) {
        EventScriptAssets assets;assert(assets.load(argv[1]) && r.memory.importScripts(assets));
        auto& e=createRootEvent(r.schedule,0x80416bd0,10,0,creation);r.gp.timestamp=eventClockRate(243000000);r.runManager();assert(e.flags==0 && r.runtime.allocationCount==0);
        std::cout<<"native owner original child execution passed\n";return 0;
    }
    assert(r.memory.registerRam(0x80300000,std::vector<uint8_t>(256)));uint32_t a=0x80300000;
    emit(r,a,50,{-30000000,7});emit(r,a,50,{-50000000,9});emit(r,a,91,{int32_t(0x80001200u)});emit(r,a,9,{1});emit(r,a,2,{});emit(r,a,1,{});assert(r.memory.writeWord(a,0));
    auto& field=createRootEvent(r.schedule,0x80300000,10,0,creation);assert(field.id==1);
    r.gp.battleContext=-1;auto& battle=createRootEvent(r.schedule,0x80300000,11,0,creation);assert(battle.id==2 && &battle==r.battle.entries && &field==r.field.entries);
    auto tick=[&](int32_t context) {r.gp.battleContext=context;r.gp.timestamp+=eventClockRate(243000000);r.runManager();};
    tick(0);assert(field.opcode==91 && field.initialized==1 && battle.opcode==0 && r.field.work[0]==9 && r.battle.work[0]==0);
    tick(1);assert(battle.opcode==91 && r.battle.work[0]==9 && platform.firstCalls==std::vector<int32_t>({1,1}));
    tick(1);assert(battle.flags==1 && battle.opcode==0 && battle.waitCount==0);tick(1);assert(battle.flags==0 && field.flags==1);
    tick(0);tick(0);assert(field.flags==0 && r.runtime.allocationCount==0 && platform.firstCalls==std::vector<int32_t>({1,1,0,0}));
    assert(platform.retired==std::vector<uint32_t>({2,1}));
    // Numeric and namespace services retain independent platform owners and
    // reload current GP for EACH query,including a swapped GP root.
    auto services=r.interpreterServices();assert(services.operands.truncate(services.operands.user,3.9f)==3 && services.divideWord(services.numericUser,7,2)==3 && platform.truncations==1 && platform.divisions==1);
    EventGlobals alternate{1,60,r.gp.timestamp};assert(r.gpAddresses.registerGp(0x83000000,alternate) && r.memory.writeWord(0x80415fd0,0x83000000));
    assert(services.operands.contextAddress(services.operands.user)==0x803d8dd0 && services.operands.globalAddress(services.operands.user)==0x83000000);
    setEventValue(r.field.entries[0],-50000000,77,services.operands);assert(r.battle.work[0]==77 && r.field.work[0]==9);
    writeSavedSwitch(SavedSwitchFamily::GlobalByte,0,123,services.operands);uint32_t w;assert(r.memory.readWord(0x83000174,w) && w==123 && r.memory.readWord(0x81000174,w) && w==0);
    r.schedule.count=17;r.runtime.nextId=99;r.reinitializeContext();assert(r.battle.work[0]==77 && r.schedule.count==17 && r.runtime.nextId==99);
    assert(r.memory.readWord(0x804151e8,w) && w==99 && r.memory.readWord(0x8041e858,w) && w==0);
    std::cout<<"native owner context/continuation/shared memory tests passed\n";
}
