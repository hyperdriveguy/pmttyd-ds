#include "event_address_space.hpp"
#include "event_context_initialization.hpp"
#include <cassert>
#include <cstdio>
#include <iostream>
#include <memory>
#include <unistd.h>
using namespace ttyd;
struct Fixture {
    EventEntryAddresses entries;
    EventContextAddresses contexts{entries};
    EventRuntimeGlobals runtime{0,1,1,0};EventSchedule schedule;
    EventRuntimeAddresses globals{runtime,schedule};EventGpAddresses gp;
    EventAddressSpace memory{entries,contexts,globals,gp};
    EventGlobals state{0,60,123};EventContext field,battle;
    std::array<EventEntry,256> fieldEntries{},battleEntries{};unsigned allocations=0;
    Fixture() {assert(contexts.registerContext(0x803d8d30,field) && contexts.registerContext(0x803d8dd0,battle));assert(gp.registerGp(0x81000000,state) && memory.writeWord(0x80415fd0,0x81000000));}
    EventManagerServices manager() {EventManagerServices s;s.user=this;s.globals=[](void* p) {return static_cast<Fixture*>(p)->gp.currentGlobals();};s.context=[](void* p,bool b) {auto& f=*static_cast<Fixture*>(p);return b?&f.battle:&f.field;};return s;}
    void initialize() {
        EventContextInitializationServices s;s.user=this;s.manager=manager();s.field=&field;s.battle=&battle;s.runtime=&runtime;s.schedule=&schedule;
        s.allocate=[](void* p,int32_t heap,uint32_t bytes) {auto& f=*static_cast<Fixture*>(p);assert(heap==0 && bytes==0x1b000);auto* e=f.allocations==0?f.fieldEntries.data():f.battleEntries.data();assert(f.entries.registerEntries(0x82000000+f.allocations*0x20000,e,256));++f.allocations;return e;};
        s.clearEntries=[](void*,EventEntry* e,uint32_t bytes) {assert(bytes==0x1b000);for(unsigned i=0;i<256;++i)e[i]={};};
        s.clearBank=[](void*,EventContext& c,bool flags) {if(flags)c.flags.fill(0);else c.work.fill(0);};
        s.timestampHalf=[](void*,EventContext& c,bool high,uint32_t value) {if(high)c.previousTimestamp=(uint64_t(value)<<32)|(c.previousTimestamp&0xffffffffu);else c.previousTimestamp=(c.previousTimestamp&0xffffffff00000000ULL)|value;};
        initializeEventContexts(s);
    }
};
void word(std::vector<uint8_t>& b,unsigned at,uint32_t value) {for(unsigned i=0;i<4;++i)b[at+i]=uint8_t(value>>(24-8*i));}
int main(int argc,char** argv) {
    // Compare registration against the original readable-byte admission rule,
    // including unaligned edges, runtime holes, RAM and the32-bit ceiling.
    auto probe=std::make_unique<Fixture>();probe->initialize();
    assert(probe->memory.registerRam(0x83000000,std::vector<uint8_t>(1024*1024)));
    for(const auto center:{0x803d8530u,0x803d8d30u,0x804151e8u,0x80415fd0u,0x8041e858u,0x8041e940u,0x81000000u,0x810013d8u,0x82000000u,0x8201b000u,0x83000000u,0x83100000u,0xfffffff8u})
        for(int offset=-5;offset<=5;++offset)for(unsigned count:{1u,2u,4u,9u}) {
            const uint32_t base=center+offset;bool expected=uint64_t(base)+count<=0x100000000ULL;
            for(unsigned i=0;expected && i<count;++i){uint8_t value;if(probe->memory.readByte(base+i,value))expected=false;}
            const bool admitted=probe->memory.registerRam(base,std::vector<uint8_t>(count));assert(admitted==expected);
            if(admitted)assert(probe->memory.unregisterRam(base));
        }
    // A live context can contain an unreadable unresolved host pointer. Its
    // conservative interval probe must not turn the old sparse rule into a ban.
    EventEntry unmapped;probe->field.entries=&unmapped;
    uint8_t missing=0;assert(!probe->memory.readByte(0x803d8dc0,missing));
    assert(probe->memory.registerRam(0x803d8dc0,{1,2,3,4}));
    assert(!probe->memory.registerRam(0x803d8dbf,{1,2}));
    assert(probe->memory.unregisterRam(0x803d8dc0));probe->field.entries=probe->fieldEntries.data();
    assert(probe->memory.registerRam(0x83200000,{1,2,3,4,5,6,7,8}));
    assert(probe->memory.registerRam(0x83200008,{9,10,11,12,13,14,15,16}));
    assert(probe->memory.registerRam(0xfffffffc,{1,2,3,4}));
    // Exercise direct RAM, adjacent-region fallback, holes and overflow against
    // byte-visible state. Failed stores must leave every existing byte intact.
    for(const auto base:{0x83200000u,0x83200008u,0xfffffffcu,0x810013d8u,0x803d8dc0u})
        for(unsigned offset=0;offset<13;++offset)for(unsigned size:{2u,4u}) {
            const uint32_t at=base+offset;bool readable=uint64_t(at)+size<=0x100000000ULL;uint32_t expected=0;
            std::array<uint8_t,4> prior{};std::array<bool,4> present{};
            for(unsigned i=0;i<size;++i){present[i]=uint64_t(at)+i<=0xffffffffULL && probe->memory.readByte(at+i,prior[i]);readable=readable && present[i];expected=expected<<8|prior[i];}
            uint32_t actual=0xfeedbeef;uint16_t half=0xbeef;
            const bool loaded=size==4?probe->memory.readWord(at,actual):probe->memory.readHalf(at,half);
            assert(loaded==readable);if(loaded)assert((size==4?actual:half)==expected);
            const uint32_t value=0x89abcdef;
            const bool stored=size==4?probe->memory.writeWord(at,value):probe->memory.writeHalf(at,uint16_t(value));
            assert(stored==readable);
            for(unsigned i=0;i<size;++i)if(present[i]){uint8_t byte;assert(probe->memory.readByte(at+i,byte));assert(byte==(stored?uint8_t(value>>(8*(size-1-i))):prior[i]));}
        }
    probe.reset();
    Fixture f;f.initialize();uint32_t w=0;uint16_t half=0;
    assert(f.memory.readWord(0x803d8dc0,w) && w==0x82000000 && f.memory.readWord(0x803d8e60,w) && w==0x82020000);
    assert(f.memory.writeWord(0x8200009c,0x11223344) && f.fieldEntries[0].locals[0]==0x11223344);
    assert(f.memory.writeHalf(0x8200009d,0xaabb) && f.fieldEntries[0].locals[0]==0x11aabb44 && f.memory.readHalf(0x8200009d,half) && half==0xaabb);
    assert(f.memory.registerRam(0x810013d8,{0x12,0x34,0x56,0x78}));
    assert(f.memory.writeWord(0x810013d6,0xabcdef01) && f.memory.readWord(0x810013d6,w) && w==0xabcdef01); // GP/RAM boundary.
    assert(!f.memory.writeWord(0x810013da,0xffffffff) && f.memory.readWord(0x810013d8,w) && w==0xef015678);
    w=99;assert(!f.memory.readWord(0xffffffff,w) && w==99 && !f.memory.registerRam(0xffffffff,{1,2}));
    assert(!f.memory.registerRam(0x80415fd0,{1}) && !f.memory.registerRam(0x82000000,{1}) && !f.memory.registerRam(0x803d8d30,{1}) && !f.memory.registerRam(0x8041e940,{1}));
    // Retired entry bytes remain accessible without retaining native objects.
    std::vector<uint8_t> retired;f.contexts.detachEntryArrays(0x82000000,256);assert(f.entries.unregisterEntries(0x82000000,&retired));assert(f.memory.registerRam(0x82000000,std::move(retired)));
    assert(f.memory.readWord(0x8200009c,w) && w==0x11aabb44 && f.memory.readWord(0x803d8dc0,w) && w==0x82000000 && f.field.entriesUnresolved);
    assert(f.memory.unregisterRam(0x82000000,&retired));assert(f.entries.registerEntries(0x82000000,f.fieldEntries.data(),256));f.contexts.resolveEntryArrays();assert(f.field.entries==f.fieldEntries.data());
    EventScriptAssets assets;
    if(argc==2) {assert(assets.load(argv[1]) && f.memory.importScripts(assets));for(const auto& segment:assets.segments())for(uint32_t i=0;i<segment.size;++i) {uint8_t a,b;assert(assets.readByte(segment.address+i,a) && f.memory.readByte(segment.address+i,b) && a==b);}std::cout<<"original script import passed\n";return 0;}
    // Authored pack verifies mutable runtime copies and all-or-none import.
    char path[]="/tmp/pm-address-space-XXXXXX";int fd=mkstemp(path);assert(fd>=0);close(fd);
    std::vector<uint8_t> pack(52);for(unsigned i=0;i<8;++i)pack[i]="PMEVT001"[i];word(pack,8,2);word(pack,12,0x80300000);word(pack,16,12);word(pack,20,36);word(pack,24,0x8030000c);word(pack,28,4);word(pack,32,48);word(pack,36,2);word(pack,40,1);word(pack,44,0);word(pack,48,0x12345678);
    FILE* file=std::fopen(path,"wb");assert(file && std::fwrite(pack.data(),1,pack.size(),file)==pack.size());std::fclose(file);assert(assets.load(path));unlink(path);
    assert(f.memory.registerRam(0x8030000c,{9,8,7,6}) && !f.memory.importScripts(assets) && !f.memory.readWord(0x80300000,w));assert(f.memory.unregisterRam(0x8030000c) && f.memory.importScripts(assets));
    EventCreationServices creation;creation.user=&f;creation.manager=f.manager();creation.runtime=[](void* p) {return &static_cast<Fixture*>(p)->runtime;};creation.word=[](void* p,uint32_t a) {uint32_t v;assert(static_cast<Fixture*>(p)->memory.readWord(a,v));return v;};
    auto& root=createRootEvent(f.schedule,0x80300000,10,0,creation);assert(root.id==1 && f.runtime.nextId==2);
    assert(f.memory.writeWord(0x8030000a,0xdeadbeef) && f.memory.readWord(0x8030000a,w) && w==0xdeadbeef && assets.readWord(0x8030000c,w) && w==0x12345678);
    // Two independent VM facades alias one process lifetime owner, with original byte order.
    std::array<uint8_t,8> process{};Fixture second;
    assert(f.memory.registerExternalRam(0x8041eb98,process.data(),8));
    assert(second.memory.registerExternalRam(0x8041eb98,process.data(),8));
    assert(!f.memory.registerRam(0x8041eb9c,{0}) && !f.memory.registerExternalRam(0x8041eb98,process.data(),8));
    assert(!f.memory.registerExternalRam(0xffffffff,process.data(),8) && !f.memory.registerExternalRam(0x8041eb00,nullptr,8));
    assert(f.memory.writeWord(0x8041eb98,0x3f800000) && second.memory.readWord(0x8041eb98,w) && w==0x3f800000);
    assert(second.memory.writeHalf(0x8041eb9a,0x1234) && process[2]==0x12 && f.memory.readWord(0x8041eb98,w) && w==0x3f801234);
    const auto before=process;assert(!f.memory.writeWord(0x8041eb9e,0) && process==before);
    assert(f.memory.unregisterExternalRam(0x8041eb98) && !f.memory.readWord(0x8041eb98,w));
    assert(second.memory.readWord(0x8041eb98,w) && w==0x3f801234);
    std::cout<<"event address space tests passed\n";
}
