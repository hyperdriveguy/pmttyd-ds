#include "battle_attack_registration.hpp"
#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
struct Fixture {
    std::array<EventEntry,4> field{},battleEntries{};
    EventContext fieldContext{4,field.data(),0},battleContext{4,battleEntries.data(),0};EventGlobals gp;
    EventRuntimeGlobals runtime{0,1,1,0};EventSchedule schedule;
    std::map<uint32_t,uint32_t> memory;
    std::map<int32_t,int32_t> statuses;
    std::vector<int32_t> statusQueries,actorQueries;
    std::vector<uint32_t> scripts;
    std::vector<int32_t> draws;
    unsigned randomIndex=0,windowCalls=0,advances=0;
    bool spy=false,nullCreation=false;
    std::function<void()> onWindow,onCreate;
    std::function<void(uint32_t,uint32_t)> onWrite;
    Fixture() {memory[0x10000+0x420]=10;memory[0x20000]=7;memory[0x20000+0x2a0]=0x9000;memory[0x20000+0x2a4]=0xa000;memory[0x9000]=1;memory[0xa000]=1;memory[0x8034f234]=1;memory[0x8034e0cc]=1;}
    EventCreationServices creation() {
        EventCreationServices s;s.user=this;s.manager.user=this;
        s.manager.globals=[](void* p) {return &static_cast<Fixture*>(p)->gp;};s.manager.context=[](void* p,bool b) {auto& f=*static_cast<Fixture*>(p);return b?&f.battleContext:&f.fieldContext;};
        s.runtime=[](void* p) {return &static_cast<Fixture*>(p)->runtime;};s.word=[](void* p,uint32_t a) {return static_cast<Fixture*>(p)->memory[a];};s.stopAll=[](void*,uint32_t) {};return s;
    }
    BattleAttackRegistrationServices services() {
        BattleAttackRegistrationServices s;s.user=this;s.memory.user=this;s.events=creation();s.schedule=&schedule;
        s.memory.word=s.events.word;s.memory.storeWord=[](void* p,uint32_t a,uint32_t v) {auto& f=*static_cast<Fixture*>(p);f.memory[a]=v;if(f.onWrite)f.onWrite(a,v);};
        s.unit=[](void* p,uint32_t b,int32_t id) {assert(b==0x10000);auto& f=*static_cast<Fixture*>(p);f.actorQueries.push_back(id);return id==10?0x20000u:0x30000u;};
        s.status=[](void* p,uint32_t u,int32_t id) {assert(u==0x20000 || u==0x30000);auto& f=*static_cast<Fixture*>(p);f.statusQueries.push_back(id);return f.statuses[id];};
        s.irand=[](void* p,int32_t bound) {assert(bound==100);auto& f=*static_cast<Fixture*>(p);return f.draws.at(f.randomIndex++);};
        s.windowOff=[](void* p) {auto& f=*static_cast<Fixture*>(p);++f.windowCalls;if(f.onWindow)f.onWindow();};
        s.advance=[](void* p,uint32_t b,int32_t c) {assert(b==0x10000 && c==6);++static_cast<Fixture*>(p)->advances;};
        if(spy)s.create=[](void* p,uint32_t script,int32_t priority,uint32_t flags)->EventEntry* {assert(priority==10 && flags==0);auto& f=*static_cast<Fixture*>(p);f.scripts.push_back(script);if(f.onCreate)f.onCreate();return f.nullCreation?nullptr:&f.field[0];};
        return s;
    }
    void run() {registerBattleAttackState6(0x10000,services());}
};
}
int main() {
    Fixture actual;actual.field[1].flags=1;actual.field[1].id=99;actual.runtime.allocationCount=1;actual.memory[0x20290]=99;actual.schedule.managerActive=true;actual.schedule.count=2;actual.runtime.speed=.5f;
    actual.onWrite=[&](uint32_t a,uint32_t v) {if(a==0x202a8) {assert(v==1 && actual.field[0].owner==-1 && actual.field[1].flags==1);actual.memory[0x20000]=47;}if(a==0x20290)assert(actual.field[0].owner==47 && actual.field[1].flags==0 && v==0);};actual.run();
    auto& e=actual.field[0];assert(e.flags==1 && e.priority==10 && e.type==0xef && e.owner==47 && e.id==1 && e.timescale==.5f && e.credit==0 && e.lifetime==0);
    assert(actual.runtime.allocationCount==1 && actual.schedule.count==2 && actual.memory[0x20290]==0 && actual.advances==1 && actual.actorQueries==std::vector<int32_t>({10,10}));
    Fixture reread;reread.onWindow=[&] {reread.memory[0x10420]=20;reread.memory[0x30000]=88;reread.memory[0x302a0]=0x9000;reread.gp.battleContext=1;};reread.run();assert(reread.actorQueries==std::vector<int32_t>({10,20}) && reread.battleEntries[0].owner==88 && reread.field[0].flags==0 && reread.advances==1);
    for(int32_t suppressed:{1,2,9,27}) {Fixture f;f.spy=true;f.statuses[suppressed]=1;f.run();assert(f.scripts.empty() && f.advances==1);assert(f.statusQueries.back()==suppressed);}
    Fixture exempt;exempt.spy=true;exempt.statuses[27]=1;exempt.memory[0x20104]=0x10000;exempt.run();assert(exempt.scripts==std::vector<uint32_t>({0x9000}));
    Fixture gravity;gravity.spy=true;gravity.memory[0x10ef4]=0x40000000;gravity.run();assert(gravity.scripts==std::vector<uint32_t>({0x8034f234}) && gravity.statusQueries.empty());
    for(uint32_t category:{0x10u,2u}) {Fixture f;f.spy=true;f.memory[0x202ac]=category;f.memory[0x202a0]=0;f.run();assert(f.scripts.size()==(category==2?1:0) && f.statusQueries.empty() && f.advances==1);if(category==2)assert(f.scripts[0]==0);}
    Fixture electric;electric.spy=true;electric.statuses[6]=1;electric.draws={0};electric.run();assert(electric.randomIndex==1 && electric.scripts[0]==0x9000 && electric.statusQueries.back()==5);
    Fixture negative;negative.spy=true;negative.statuses[6]=1;negative.draws={-1};negative.run();assert(negative.scripts[0]==0x8034e0cc && negative.statusQueries.back()==6); // Native RNG-domain fixture,not observed original reachability.
    for(int32_t draw:{49,50}) {Fixture f;f.spy=true;f.statuses[5]=1;f.draws={draw};f.memory[0x2027c]=0x80000000;f.run();assert(f.scripts[0]==(draw<50?0xa000u:0x9000u) && f.memory[0x2027c]==(draw<50?0x80000010u:0x80000000u));}
    Fixture failure;failure.spy=true;failure.nullCreation=true;failure.memory[0x20290]=77;failure.run();assert(failure.memory[0x20290]==77 && failure.memory[0x202a8]==0 && failure.advances==1);
    Fixture skippedWindow;skippedWindow.spy=true;skippedWindow.memory[0x202ac]=1;skippedWindow.run();assert(skippedWindow.windowCalls==0 && skippedWindow.actorQueries.size()==2);
    std::cout<<"battle attack registration tests passed\n";
}
