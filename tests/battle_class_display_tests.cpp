#include "battle_class_display.hpp"
#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace ttyd;
namespace {
struct Fixture {
    static constexpr uint32_t Passed=0x10000,B=0x30000,W=0x40000,C=0x50000;
    std::map<uint32_t,uint32_t> words{{0x8041eb90,0x20000},{B+0x1c78,W},{W,C},{W+4,0x60000},{W+0xc,10},{C+8,0}};
    std::vector<std::string> attempts,completed;
    std::vector<uint32_t> reads;
    std::vector<std::array<uint32_t,2>> stores;
    std::string hold;
    bool released=false,writeLookup=true;
    unsigned lookupAttempts=0;
    bool call(const char* name){attempts.emplace_back(name);if(hold==name && !released)return false;completed.emplace_back(name);return true;}
    BattleClassDisplayServices services(){
        BattleClassDisplayServices s;s.user=this;s.draw.user=this;s.draw.memory.memory.user=this;
        s.draw.memory.memory.word=[](void* p,uint32_t address){auto& f=*static_cast<Fixture*>(p);f.reads.push_back(address);return f.words.at(address);};
        s.draw.memory.memory.storeWord=[](void* p,uint32_t at,uint32_t value){auto& f=*static_cast<Fixture*>(p);f.stores.push_back({at,value});f.words[at]=value;};
        s.draw.memory.memory.storeByte=[](void* p,uint32_t at,uint8_t value){auto& f=*static_cast<Fixture*>(p);f.stores.push_back({at,value});f.words[at]=value;};
        s.cursorLookup=[](void* p,uint32_t menu,uint32_t type,std::optional<uint32_t>& output){auto& f=*static_cast<Fixture*>(p);assert(menu==Passed+0x171c && type==14);assert(f.reads.empty());if(f.lookupAttempts++)assert(!f.writeLookup || output==0xabcdef);else assert(!output);if(f.writeLookup)output=0xabcdef;f.words[0x8041eb90]=B;return f.call("lookup");};
        s.draw.camera=[](void* p,uint32_t id){auto& f=*static_cast<Fixture*>(p);assert(id==8);assert(f.reads.size()==3);f.words[0x8041eb90]=0xbad00000;return f.call("camera");};
        s.draw.icons=[](void* p,uint32_t work,uint32_t rows,const std::vector<BattleActDrawSlot>& slots){auto& f=*static_cast<Fixture*>(p);assert(work==W && rows==0x60000 && slots.empty());return f.call("icons");};
        s.draw.partyButton=[](void* p){return static_cast<Fixture*>(p)->call("party");};
        s.draw.pinchMark=[](void* p){return static_cast<Fixture*>(p)->call("pinch");};
        return s;
    }
};
void finish(Fixture& f){
    assert((f.completed==std::vector<std::string>{"lookup","camera","icons","party","pinch"}));
    assert((f.stores==std::vector<std::array<uint32_t,2>>{{Fixture::W+0xc,1},{Fixture::W+0x14,1},{Fixture::W+0x1c,0}}));
}
}
int main(){
    Fixture ordinary;BattleClassDisplayCursor cursor;
    assert(resumePackedBattleClassQueuedDisplay(0,Fixture::Passed,ordinary.services(),cursor));
    assert(!cursor.active && !cursor.draw.active() && !cursor.lookupOutput);finish(ordinary);
    // Camera argument is ignored, including across suspension. Each service
    // frontier retains the completed prefix, passed-B lookup and child writes.
    for(const auto* hold:{"lookup","camera","icons","party","pinch"}){
        Fixture f;f.hold=hold;BattleClassDisplayCursor c;auto s=f.services();
        assert(!resumePackedBattleClassQueuedDisplay(0xffffffff,Fixture::Passed,s,c));
        assert(c.active);
        const auto reads=f.reads.size(),stores=f.stores.size(),completed=f.completed.size();
        assert(!resumePackedBattleClassQueuedDisplay(7,Fixture::Passed,s,c));
        assert(f.reads.size()==reads && f.stores.size()==stores && f.completed.size()==completed);
        // A different passed-B invocation cannot steal the retained lookup.
        assert(!resumePackedBattleClassQueuedDisplay(7,Fixture::Passed+4,s,c));
        auto wrong=s;wrong.user=&ordinary;
        assert(!resumePackedBattleClassQueuedDisplay(7,Fixture::Passed,wrong,c));
        wrong=s;wrong.draw.pinchMark=nullptr;
        assert(!resumePackedBattleClassQueuedDisplay(7,Fixture::Passed,wrong,c));
        assert(f.reads.size()==reads && f.stores.size()==stores && f.completed.size()==completed);
        f.released=true;
        assert(resumePackedBattleClassQueuedDisplay(7,Fixture::Passed,s,c));
        finish(f);assert(!c.active && !c.draw.active());
    }
    // An acknowledged cursor lookup may leave its unused stack output unwritten.
    Fixture unwritten;unwritten.writeLookup=false;BattleClassDisplayCursor unused;
    assert(resumePackedBattleClassQueuedDisplay(4,Fixture::Passed,unwritten.services(),unused));finish(unwritten);
    // Missing provider holds before any guest draw capture; binding a different
    // table to this pending invocation is rejected, not silently restarted.
    Fixture missing;auto ms=missing.services();ms.cursorLookup=nullptr;BattleClassDisplayCursor mc;
    assert(!resumePackedBattleClassQueuedDisplay(4,Fixture::Passed,ms,mc));
    assert(missing.reads.empty() && missing.stores.empty());
    assert(!resumePackedBattleClassQueuedDisplay(4,Fixture::Passed,missing.services(),mc));
    // Direct wrapper ignores its passed battle value and performs no lookup.
    Fixture direct;direct.words[0x8041eb90]=Fixture::B;auto ds=direct.services();BattleActDrawCursor dc;
    assert(resumePackedBattleClassDisplay(0xdeadbeef,ds.draw,dc));
    assert((direct.completed==std::vector<std::string>{"camera","icons","party","pinch"}));
    std::cout<<"Original queued class lookup/wrapper and retained child failure prefixes passed\n";
}
