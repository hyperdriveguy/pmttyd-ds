#include "save_block_event.hpp"
#include "event_native_runtime.hpp"
#include "event_memory_bootstrap.hpp"
#include "event_script_assets.hpp"
#include <array>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>
namespace ttyd {
namespace {
EventScriptAssets scripts;bool ready=false;
constexpr uint32_t functions[]={0x800d2090,0x800d21fc,0x800d23c4,0x800d284c,0x800e7474,0x8025c0b8,0x8025c188,
                               0x8025c19c,0x8025c2bc,0x8025c2fc,0x8025c380,0x8025c3a4,0x8025c40c,0x8025c454};
int32_t signedWord(uint32_t v){return v<=0x7fffffff?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}
}
bool loadSaveBlockEventAssets(const char* root) {
    if(!root)return false;
    const std::string base=root;
    EventScriptAssets next;if(!next.load((base+"/save-block-events.pmevt").c_str()))return false;
    FILE* f=std::fopen((base+"/save-block-program.bin").c_str(),"rb");if(!f)return false;
    char magic[8];std::array<uint32_t,4> header{};
    bool ok=std::fread(magic,8,1,f)==1 && !std::memcmp(magic,"PMSAVE01",8) && std::fread(header.data(),4,4,f)==4 &&
        header==std::array<uint32_t,4>{0x8030b4ec,0x803bac3c,0x803ba444,14};
    for(unsigned i=0;ok && i<14;++i){std::array<uint32_t,2> row{};ok=std::fread(row.data(),4,2,f)==2 && row[0]==functions[i] && row[1]==i;}
    ok=ok && std::fgetc(f)==EOF;std::fclose(f);uint32_t word=0;
    for(unsigned i=0;ok && i<3;++i)ok=next.readWord(header[i],word);
    if(!ok)return false;
    scripts=std::move(next);ready=true;return true;
}
struct SaveBlockEventRuntime {
    SaveBlockEvent* owner=nullptr;SaveEventServices services;
    EventNativeRuntime vm;
    SaveBlockEventRuntime():vm(0x81000000,platform(this)){}
    static EventNativePlatform platform(SaveBlockEventRuntime* r) {
        EventNativePlatform p;p.user=r;p.clock=[](void*)->uint32_t{return 243000000;};
        p.truncate=[](void*,float v)->int32_t{assert(std::isfinite(v) && double(v)>=-2147483648. && double(v)<2147483648.);return int32_t(v);};
        p.divideWord=[](void*,int32_t a,int32_t b){assert(b);return a/b;};
        p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){};
        p.call=[](void* r,EventEntry& e,uint32_t fn,int32_t first){return static_cast<SaveBlockEventRuntime*>(r)->call(e,fn,first!=0);};return p;
    }
    int32_t operand(EventEntry& e,unsigned n) {
        uint32_t value=0;if(n>=e.argumentCount || !vm.memory.readWord(e.arguments+n*4,value)){owner->failed=true;return 0;}
        return signedWord(value);
    }
    int32_t value(EventEntry& e,unsigned n){return getEventValue(e,operand(e,n),vm.interpreterServices().operands);}
    std::string text(uint32_t address) {
        std::string result;
        for(unsigned i=0;i<256;++i){uint8_t c=0;if(!vm.memory.readByte(address+i,c)){owner->failed=true;return {};}
            if(!c)return result;
            result.push_back(char(c));}
        owner->failed=true;return {};
    }
    int32_t call(EventEntry& e,uint32_t fn,bool first) {
        unsigned tag=0;while(tag<14 && functions[tag]!=fn)++tag;
        if(tag==14 || !services.call || e.argumentCount>4){owner->failed=true;return 0;}
        SaveEventRequest request{};request.kind=SaveEventCall(tag);request.function=fn;request.first=first;
        request.argumentCount=e.argumentCount;
        for(unsigned i=0;i<e.argumentCount;++i){request.originalOperands[i]=operand(e,i);request.values[i]=value(e,i);}
        if(tag<=4 && tag!=1){request.flags=value(e,0);if(tag!=1 && tag!=4)request.text=text(uint32_t(value(e,1)));}
        if(owner->failed)return 0;
        const int32_t status=services.call(services.user,request);
        if(status==2) {
            if(tag==0)setEventValue(e,-30000000,request.result,vm.interpreterServices().operands); // LW0 result.
            if(tag==11 || tag==13)setEventValue(e,operand(e,0),request.result,vm.interpreterServices().operands);
        }
        return status;
    }
    bool initialize(uint64_t now,const SaveBlockSwitches& initial,uint32_t ownerKey) {
        if(!vm.memory.registerRam(0x82000000,std::vector<uint8_t>(1024*1024)) ||
           !vm.memory.registerRam(0x8041f070,std::vector<uint8_t>(16)) ||
           !vm.memory.registerRam(0x8041e428,std::vector<uint8_t>(4)) ||
           !vm.memory.registerRam(0x8041e844,std::vector<uint8_t>(4)) || !vm.memory.importScripts(scripts))return false;
        auto memory=vm.memoryBinding.services();const auto start=eventSdkInitializeAllocator(0x82000000,0x82100000,1,memory);
        if(eventSdkCreateHeap(start,0x82100000,memory)!=0)return false;
        if(!vm.memory.writeWord(0x8041e844,0) || !vm.memory.writeWord(0x81000004,60))return false;
        vm.gp.timestamp=now*eventClockRate(243000000);vm.initializeContexts();const auto op=vm.interpreterServices().operands;
        writeSavedSwitch(SavedSwitchFamily::GlobalByte,0,initial.story,op);
        writeSavedSwitch(SavedSwitchFamily::GlobalByte,4,initial.mode,op);
        writeSavedSwitch(SavedSwitchFamily::GlobalFlag,233,initial.firstHit,op);
        writeSavedSwitch(SavedSwitchFamily::GlobalFlag,236,initial.postStoryHit,op);
        auto creation=vm.creationServices();auto& entry=createTypedRootEvent(vm.schedule,0x8030b4ec,30,0,26,creation);
        uint32_t address=0;
        return vm.entryAddresses.addressOf(&entry,address) && vm.memory.writeWord(address+0x174,ownerKey);
    }
};
bool SaveBlockEvent::begin(uint64_t time,const SaveBlockSwitches& initial,uint32_t owner) {
    if(!ready || active())return false;
    SaveBlockEvent next;next.switches=initial;next.runtime=std::make_shared<SaveBlockEventRuntime>();
    next.runtime->owner=&next;
    if(!next.runtime->initialize(time,initial,owner))return false;
    next.started=true;*this=std::move(next);runtime->owner=this;return true;
}
void SaveBlockEvent::tick(uint64_t time,const SaveEventServices& service) {
    if(!active())return;
    runtime->owner=this;runtime->services=service;runtime->vm.gp.timestamp=time*eventClockRate(243000000);
    runtime->vm.runManager();const auto op=runtime->vm.interpreterServices().operands;
    switches.story=readSavedSwitch(SavedSwitchFamily::GlobalByte,0,op);
    switches.mode=uint8_t(readSavedSwitch(SavedSwitchFamily::GlobalByte,4,op));
    switches.firstHit=readSavedSwitch(SavedSwitchFamily::GlobalFlag,233,op)!=0;
    switches.postStoryHit=readSavedSwitch(SavedSwitchFamily::GlobalFlag,236,op)!=0;
    finished=runtime->vm.runtime.allocationCount==0;
}
}
