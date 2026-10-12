#include "battle_event_callbacks.hpp"
#include "event_memory_bootstrap.hpp"
#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <vector>
using namespace ttyd;
struct Fixture {
    std::map<uint32_t,uint32_t> words;std::vector<uint32_t> reads,writes,frees;
    std::function<void()> onFree;
    Fixture() {for(uint32_t i=0;i<0x28;i+=4)words[0x1000+i]=0xabc00000+i;words[0x100c]=0x9000;}
    EffectDeletionServices services() {
        EffectDeletionServices s;s.user=this;s.memory.user=this;
        s.memory.word=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);return f.words[a];};
        s.memory.storeWord=[](void* p,uint32_t a,uint32_t v) {auto& f=*static_cast<Fixture*>(p);f.writes.push_back(a);f.words[a]=v;};
        s.freeHeap=[](void* p,int32_t heap,uint32_t payload) {auto& f=*static_cast<Fixture*>(p);assert(heap==3);assert(f.writes.empty());f.frees.push_back(payload);if(f.onFree)f.onFree();};return s;
    }
};
int main() {
    for(auto deletion:{hardDeleteEffect,softDeleteEffect}) {
        Fixture null;deletion(0,null.services());assert(null.reads.empty() && null.writes.empty() && null.frees.empty());
        Fixture zero;zero.words[0x1000]=0;deletion(0x1000,zero.services());assert(zero.reads==std::vector<uint32_t>{0x1000} && zero.writes.empty() && zero.frees.empty());
    }
    for(uint32_t flags:{1u,2u,3u,4u,6u,7u,0x80000000u,0x80000002u}) {
        Fixture hard;hard.words[0x1000]=flags;const auto prior=hard.words;hard.onFree=[&] {assert(hard.words[0x1000]==flags);hard.words[0x1000]=99;};hardDeleteEffect(0x1000,hard.services());
        assert(hard.words[0x1000]==0 && hard.reads==std::vector<uint32_t>({0x1000,0x100c}) && hard.frees==std::vector<uint32_t>{0x9000});for(auto [a,v]:prior)if(a!=0x1000)assert(hard.words[a]==v);
        Fixture soft;soft.words[0x1000]=flags;softDeleteEffect(0x1000,soft.services());
        if(flags&2) {assert(soft.words[0x1000]==(flags|4) && soft.reads==std::vector<uint32_t>{0x1000} && soft.frees.empty());softDeleteEffect(0x1000,soft.services());assert(soft.writes==std::vector<uint32_t>({0x1000,0x1000}));}
        else assert(soft.words[0x1000]==0 && soft.reads==std::vector<uint32_t>({0x1000,0x100c}) && soft.frees==std::vector<uint32_t>{0x9000});
    }
    Fixture zeroPayload;zeroPayload.words[0x1000]=4;zeroPayload.words[0x100c]=0;softDeleteEffect(0x1000,zeroPayload.services());assert(zeroPayload.frees==std::vector<uint32_t>{0});
    Fixture maya;const auto prior=maya.words;releaseMayaEffectPayload(0,maya.services());releaseMayaEffectPayload(0x1000,maya.services());assert(maya.frees==std::vector<uint32_t>({0,0x1000}) && maya.reads.empty() && maya.words==prior);
    // Native fog expiry defers flags02 records,then actual hard deletion releases
    // SDK heap3 payloads. All arena/addresses/cache services are authored inputs.
    EventNativePlatform platform;platform.flushBlock=[](void*,uint32_t) {};platform.systemCall=[](void*) {};
    EventNativeRuntime r(0x81000000,platform);auto& ram=r.memory;
    assert(ram.registerRam(0x90000000,std::vector<uint8_t>(4096)) && ram.registerRam(0x8041f070,std::vector<uint8_t>(16)) && ram.registerRam(0x8041e428,std::vector<uint8_t>(4)) && ram.registerRam(0x8041e850,std::vector<uint8_t>(4)));
    auto memory=r.memoryBinding.services();auto start=eventSdkInitializeAllocator(0x90000000,0x90001000,1,memory);assert(eventSdkCreateHeap(start,0x90001000,memory)==0 && ram.writeWord(0x8041e850,0));
    auto payload=eventHeapAllocate(3,64,memory);assert(payload!=0);
    assert(ram.registerRam(0x84000000,std::vector<uint8_t>(0x19000)) && ram.registerRam(0x8041eb90,std::vector<uint8_t>(4)) && ram.writeWord(0x8041eb90,0x84000000) && ram.registerRam(0x88000000,std::vector<uint8_t>(0x28)) && ram.writeWord(0x88000000,3) && ram.writeWord(0x8800000c,payload));
    assert(ram.writeWord(0x840182b8,1) && ram.writeWord(0x840182bc,0x88000000));BattleTurnFirstBackend backend{&r};assert(battleFogEndCheck(nativeBattleTurnFirstServices(backend))==1);
    uint32_t word;assert(ram.readWord(0x88000000,word) && word==7 && ram.readWord(0x840182bc,word) && word==0);
    hardDeleteEffect(0x88000000,nativeEffectDeletionServices(r));assert(ram.readWord(0x88000000,word) && word==0 && ram.readWord(0x8800000c,word) && word==payload);
    assert(eventHeapAllocate(3,64,memory)==payload);assert(ram.writeWord(0x88000000,8));softDeleteEffect(0x88000000,nativeEffectDeletionServices(r));assert(ram.readWord(0x88000000,word) && word==0 && eventHeapAllocate(3,64,memory)==payload);releaseMayaEffectPayload(payload,nativeEffectDeletionServices(r));
    std::cout<<"effect deletion/fog/SDK heap3 tests passed\n";
}
