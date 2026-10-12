#include "battle_event_callbacks.hpp"
#include "event_memory_bootstrap.hpp"
#include <cassert>
#include <iostream>
using namespace ttyd;
struct Backend {
    EventNativeRuntime* runtime=nullptr;unsigned translations=0,lookups=0,observerCalls=0;
    uint32_t read(uint32_t a) {uint32_t v;assert(runtime->memory.readWord(a,v));return v;}
    BattleDataChildBackend services() {
        BattleDataChildBackend s;s.user=this;
        s.translate=[](void* p,EventEntry& e,int32_t selector) {auto& b=*static_cast<Backend*>(p);assert(selector==-2);++b.translations;(void)b.read(0x8041eb90);return e.owner;};
        s.currentBattle=[](void* p) {return static_cast<Backend*>(p)->read(0x8041eb90);};
        s.unit=[](void* p,uint32_t battle,int32_t actor) {auto& b=*static_cast<Backend*>(p);assert(battle==0x84000000 && actor==7);++b.lookups;b.runtime->field.work[0]=7;b.runtime->field.entries[0].owner=42;return 0x84001000u;};return s;
    }
};
void emit(EventNativeRuntime& r,uint32_t& a,uint8_t op,std::initializer_list<int32_t> args) {assert(r.memory.writeWord(a,uint32_t(args.size())<<16|op));a+=4;for(auto v:args) {assert(r.memory.writeWord(a,uint32_t(v)));a+=4;}}
int main(int argc,char** argv) {
    EventCallbackRegistry registry;EventEntry arbitrary;int32_t result=99;
    assert(!registry.invoke(arbitrary,0x1234,1,result) && result==99 && !registry.bind(0,nullptr,[](void*,EventEntry&,int32_t) {return 2;}) && !registry.bind(0x1234,nullptr,nullptr));
    assert(registry.bind(0x1234,&registry,[](void* p,EventEntry&,int32_t first) {auto& r=*static_cast<EventCallbackRegistry*>(p);assert(r.unbind(0x1234));assert(r.bind(0x5678,nullptr,[](void*,EventEntry&,int32_t) {return 255;}));return first?0:2;}));
    assert(!registry.bind(0x1234,nullptr,[](void*,EventEntry&,int32_t) {return 9;}) && registry.invoke(arbitrary,0x1234,1,result) && result==0 && !registry.contains(0x1234));
    assert(EventCallbackRegistry::dispatch(&registry,arbitrary,0x5678,0)==255 && registry.unbind(0x5678));
    EventNativePlatform platform;platform.user=&registry;platform.call=EventCallbackRegistry::dispatch;
    platform.clock=[](void*)->uint32_t {return 243000000;};platform.flushBlock=[](void*,uint32_t) {};platform.systemCall=[](void*) {};
    EventNativeRuntime runtime(0x81000000,platform);Backend backend{&runtime};
    auto& memory=runtime.memory;
    assert(memory.registerRam(0x82000000,std::vector<uint8_t>(1024*1024)) && memory.registerRam(0x8041f070,std::vector<uint8_t>(16)) && memory.registerRam(0x8041e428,std::vector<uint8_t>(4)) && memory.registerRam(0x8041e844,std::vector<uint8_t>(4)));
    auto m=runtime.memoryBinding.services();const auto start=eventSdkInitializeAllocator(0x82000000,0x82100000,1,m);assert(eventSdkCreateHeap(start,0x82100000,m)==0 && memory.writeWord(0x8041e844,0));runtime.gp.refreshRate=60;runtime.initializeContexts();
    assert(memory.registerRam(0x8041eb90,std::vector<uint8_t>(4)) && memory.writeWord(0x8041eb90,0x84000000));
    assert(memory.registerRam(0x84001000,std::vector<uint8_t>(0x17c)) && memory.registerRam(0x84002000,std::vector<uint8_t>(16)) && memory.writeWord(0x84001018,0x84002000) && memory.writeWord(0x84002000,7));
    assert(memory.registerRam(0x84000000,std::vector<uint8_t>(0x448)) && memory.writeWord(0x84000020+7*4,0x84001000));
    auto childServices=argc==2?nativeBattleDataChildServices(runtime):nativeBattleDataChildServices(runtime,backend.services());assert(bindBattleDataChildCallback(registry,childServices) && !bindBattleDataChildCallback(registry,childServices) && !registry.contains(0x80107788));
    uint32_t childScript=0x80300100;
    if(argc==2) {EventScriptAssets assets;assert(assets.load(argv[1]) && memory.importScripts(assets));childScript=0x80416bd0;}
    else {
        assert(memory.registerRam(childScript,std::vector<uint8_t>(32)));uint32_t a=childScript;emit(runtime,a,91,{int32_t(0x80001200u)});emit(runtime,a,2,{});emit(runtime,a,1,{});assert(memory.writeWord(a,0));
        assert(registry.bind(0x80001200,&backend,[](void* p,EventEntry& e,int32_t first) {auto& b=*static_cast<Backend*>(p);assert(e.owner==7 && e.waitingParent && e.waitingParent->owner==42);++b.observerCalls;return first?0:2;}));
    }
    assert(memory.writeWord(0x84002004,childScript) && memory.registerRam(0x80300000,std::vector<uint8_t>(64)));
    uint32_t a=0x80300000;emit(runtime,a,91,{int32_t(0x8010a010u),-2,argc==2?7:-50000000});emit(runtime,a,2,{});emit(runtime,a,1,{});assert(memory.writeWord(a,0));
    auto& parent=createRootEvent(runtime.schedule,0x80300000,10,0,runtime.creationServices());parent.owner=7;
    auto tick=[&] {runtime.gp.timestamp+=eventClockRate(243000000);runtime.runManager();};tick();
    assert(parent.flags&1);assert(backend.translations==(argc==2?0u:1u) && backend.lookups==(argc==2?0u:1u) && runtime.runtime.nextId==3);
    if(argc==2)assert(parent.waitingChild==nullptr && runtime.runtime.allocationCount==1);
    else assert(parent.waitingChild && parent.waitingChild->owner==7 && backend.observerCalls==1 && runtime.runtime.allocationCount==2);
    tick();assert(parent.flags==0 && runtime.runtime.allocationCount==0);
    // Found key with value0 dispatches handler2 and continues without a child.
    assert(memory.writeWord(0x84002004,0));auto& missing=createRootEvent(runtime.schedule,0x80300000,10,0,runtime.creationServices());missing.owner=7;tick();assert(missing.flags==0 && runtime.runtime.nextId==4 && backend.translations==(argc==2?0u:2u) && backend.lookups==(argc==2?0u:2u));
    assert(bindBattleJumpSettingCallback(registry,runtime) && !bindBattleJumpSettingCallback(registry,runtime));
    assert(memory.registerRam(0x80300200,std::vector<uint8_t>(64)));
    arbitrary.owner=7;arbitrary.arguments=0x80300200;
    assert(memory.writeWord(arbitrary.arguments,uint32_t(-2)) && memory.writeWord(arbitrary.arguments+4,0xfffffffd) && memory.writeWord(arbitrary.arguments+8,0xf24a8280) && memory.writeWord(arbitrary.arguments+12,0xf24a7b4c));
    assert(memory.writeWord(0x84001178,0x12345678));
    assert(registry.invoke(arbitrary,0x8010d6cc,0,result) && result==2);uint32_t word;
    assert(memory.readWord(0x8400116c,word) && word==0xfffffffd && memory.readWord(0x84001170,word) && word==0x40000000 && memory.readWord(0x84001174,word) && word==0x3e4c0000 && memory.readWord(0x84001178,word) && word==0x12345678);
    assert(memory.writeWord(arbitrary.arguments+4,0) && memory.writeWord(arbitrary.arguments+8,0) && memory.writeWord(arbitrary.arguments+12,0));
    assert(registry.invoke(arbitrary,0x8010d6cc,1,result) && result==2 && memory.readWord(0x8400116c,word) && word==0 && memory.readWord(0x84001170,word) && word==0 && memory.readWord(0x84001174,word) && word==0); // No first-call gate or automatic settings.
    if(argc==2) {
        arbitrary.arguments=0x80355cf8;
        assert(registry.invoke(arbitrary,0x8010d6cc,1,result) && result==2 && memory.readWord(0x8400116c,word) && word==20 && memory.readWord(0x84001170,word) && word==0 && memory.readWord(0x84001174,word) && word==0x3f330000); // Actual initial Jump request:0/.69921875.
    }
    assert(registry.unbind(0x8010a010));result=99;assert(!registry.invoke(arbitrary,0x8010a010,0,result) && result==99);
    std::cout<<"native callback registry/data-child execution tests passed\n";
}
