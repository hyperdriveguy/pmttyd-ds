#include "event_system_level.hpp"
#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
struct Write {uint32_t a,v;bool operator==(const Write& w) const {return a==w.a && v==w.v;}};
struct Fixture {
    std::array<EventEntry,4> entries{};
    EventContext context{4,entries.data(),0};EventGlobals globals;
    EventRuntimeGlobals runtime;
    std::map<uint32_t,uint32_t> words;
    std::vector<Write> writes;
    std::vector<uint32_t> reads;
    std::function<void(uint32_t,uint32_t)> onWrite;
    Fixture() {words[0x80415fd0]=0x1000;const uint8_t types[]{1,2,0x10,0xef};for(unsigned i=0;i<4;++i) {entries[i].flags=1;entries[i].type=types[i];entries[i].lifetime=123;entries[i].credit=.5f;}}
    EventMemoryServices memory() {
        EventMemoryServices s;s.user=this;
        s.word=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);return f.words[a];};
        s.storeWord=[](void* p,uint32_t a,uint32_t v) {auto& f=*static_cast<Fixture*>(p);f.writes.push_back({a,v});f.words[a]=v;if(f.onWrite)f.onWrite(a,v);};return s;
    }
    EventCreationServices creation() {
        EventCreationServices s;s.user=this;s.manager.user=this;
        s.manager.globals=[](void* p) {return &static_cast<Fixture*>(p)->globals;};
        s.manager.context=[](void* p,bool) {return &static_cast<Fixture*>(p)->context;};
        s.runtime=[](void* p) {auto& f=*static_cast<Fixture*>(p);f.runtime.systemLevel=getEventSystemLevel(f.memory());return &f.runtime;};
        s.stopAll=[](void* p,uint32_t mask) {auto& f=*static_cast<Fixture*>(p);stopAllEvents(mask,f.creation());};return s;
    }
    void set(int32_t r) {setEventSystemLevel(r,memory(),creation());}
};
}
int main() {
    for(int32_t request:{-1,5,99}) {Fixture f;f.words[0x1018]=0x12345678;f.set(request);assert(f.writes.empty() && f.reads.empty() && f.words[0x1018]==0x12345678);for(auto& e:f.entries)assert(e.flags==1);}
    for(int32_t request=0;request<=4;++request) {
        Fixture f;f.words[0x1018]=0x8000ffff;for(auto& e:f.entries)if(request==0)e.flags=3;f.set(request);
        const uint32_t nibble=std::vector<uint32_t>{0,1,3,7,15}[request];assert(f.words[0x1018]==(0x8000fff0|nibble) && f.words[0x8041e940]==uint32_t(request));
        assert(uint32_t(getEventSystemLevel(f.memory()))==(0x8000fff0|nibble));
        assert(f.writes.front()==(Write{0x8041e940,uint32_t(request)}));
        assert(f.writes.size()==(request==0 || request==4?2:3));
        const uint8_t affected=request==1?1:request==2?2:request==3?0x10:0xef;
        for(auto& e:f.entries) {const bool match=(e.type&affected)!=0;assert(e.flags==(request==0?(match?1:3):(match?3:1)));assert(e.lifetime==123 && e.credit==.5f);}
    }
    Fixture transitions;transitions.set(2);assert(transitions.entries[0].flags==1 && transitions.entries[1].flags==3);transitions.set(1);assert(transitions.words[0x1018]==1 && transitions.entries[0].flags==3 && transitions.entries[1].flags==3);
    transitions.set(0);assert(transitions.entries[0].flags==1 && transitions.entries[1].flags==1);transitions.set(3);assert(transitions.entries[2].flags==3);transitions.set(0);assert(transitions.entries[2].flags==3); // EF excludes type10.
    Fixture distinct;distinct.set(2);assert(distinct.words[0x8041e940]==2 && getEventSystemLevel(distinct.memory())==3);
    Fixture live;live.words[0x1018]=15;live.words[0x2018]=0x40000004;
    live.onWrite=[&](uint32_t a,uint32_t) {if(a==0x1018)live.words[0x80415fd0]=0x2000;};live.set(2);
    assert(live.writes==std::vector<Write>({{0x8041e940,2},{0x1018,3},{0x2018,0x40000007}}) && getEventSystemLevel(live.memory())==0x40000007);
    // Constructor/restart run-check reads the whole mask,not requested enum:
    // request2 produces mask3,whose original run-check branch stops type10.
    Fixture runCheck;runCheck.set(2);checkEventEntryRun(runCheck.creation());assert(runCheck.entries[2].flags==3);
    runCheck.words[0x1018]|=0x80000000;runCheck.entries[2].flags=1;checkEventEntryRun(runCheck.creation());assert(runCheck.entries[2].flags==1); // Whole-word compare is not low-nibble enum lookup.
    std::cout<<"event system-level tests passed\n";
}
