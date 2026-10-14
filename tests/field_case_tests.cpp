#include "field_case.hpp"
#include "event_controls.hpp"
#include "event_lifecycle.hpp"
#include <cassert>
#include <climits>
#include <map>
#include <vector>
using namespace ttyd;
struct Fixture {
 EventGlobals gp;
 std::array<EventEntry,8> entries{};
 EventContext context{8,entries.data(),0};
 EventRuntimeGlobals runtime{0,1,1,0};EventSchedule schedule;
 FieldCasePool pool;unsigned stops=0;
 EventCreationServices creation() {
  EventCreationServices s;s.user=this;s.manager.user=this;
  s.manager.globals=[](void* p){return &static_cast<Fixture*>(p)->gp;};
  s.manager.context=[](void* p,bool){return &static_cast<Fixture*>(p)->context;};
  s.runtime=[](void* p){return &static_cast<Fixture*>(p)->runtime;};
  s.word=[](void*,uint32_t a){assert(a==0x1000 || a==0x1004);return a==0x1000?1u:0u;};
  s.stopAll=[](void*,uint32_t){assert(false);};return s;
 }
 FieldCaseDispatchServices services() {
  return {&schedule,creation(),this,[](void* p){auto& f=*static_cast<Fixture*>(p);++f.stops;
   const auto* r=f.pool.active(1);assert(r && r->eventId && f.entries[1].field16c==1 && f.entries[1].locals==r->locals);}};
 }
};
struct Wrapper {
 FieldCasePool pool;
 std::map<uint32_t,uint32_t> words;
 std::vector<int32_t> resolved;std::vector<uint32_t> reads;
 int32_t written=-1,destination=0;bool lateDestination=false;
 Wrapper(){words={{0x100,9},{0x104,1},{0x108,0x200},{0x10c,uint32_t(-30000005)},{0x110,0x1000},{0x114,uint32_t(-30000015)}};}
 FieldCaseEventServices services() {
  return {this,[](void* p,uint32_t a){auto& w=*static_cast<Wrapper*>(p);w.reads.push_back(a);return w.words.at(a);},
   [](void* p,EventEntry& e,int32_t raw){auto& w=*static_cast<Wrapper*>(p);w.resolved.push_back(raw);
    if(raw==0x1000)e.locals[4]=1234;
    return raw;},
   [](void* p,EventEntry& e,int32_t raw,int32_t value){auto& w=*static_cast<Wrapper*>(p);assert(w.pool.active(value));
    w.destination=raw;w.written=value;e.locals[15]=value;},
   [](void* p,uint32_t a,std::string& name){auto& w=*static_cast<Wrapper*>(p);assert(a==0x200);name="w_bero";
    if(w.lateDestination)w.words[0x114]=uint32_t(-30000014);
    return true;},
   [](void* p){return &static_cast<Wrapper*>(p)->pool;}};
 }
};
int main() {
 for(const auto raw:{INT_MIN,-270000000,-269999999,-250000000,-249999999,-220000000,0,INT_MAX})assert(fieldCaseNumber(raw)==raw);
 for(int32_t limit=-200000000;limit<=-20000000;limit+=20000000) {
  assert(fieldCaseNumber(limit-10000000)==0 && fieldCaseNumber(limit)==10000000);
  assert(fieldCaseNumber(limit-9999999)==1);
 }
 Wrapper w;EventEntry caller;caller.arguments=0x100;caller.locals.fill(-99);caller.locals[15]=37;caller.localFlags.fill(~0u);caller.userWork=0x12345678;
 w.lateDestination=true;assert(fieldRunCaseEvent(caller,true,w.services())==2);
 const auto& r=*w.pool.active(0);assert(r.flags==5 && r.type==9 && r.argument==uint32_t(-5) && r.script==0x1000 && r.priority==20 && !r.callback);
 assert(r.locals[15]==37 && r.locals[4]==1234 && caller.locals[15]==0 && w.destination==-30000014);
 assert((w.resolved==std::vector<int32_t>{9,1,0x200,0x1000}));
 assert((w.reads==std::vector<uint32_t>{0x100,0x104,0x108,0x10c,0x110,0x114}));
 Wrapper ordinary;ordinary.words[0x114]=0;assert(fieldRunCaseEvent(caller,false,ordinary.services())==2 && ordinary.written==-1 && ordinary.pool.active(0)->priority==0);
 FieldCaseDescriptor d;d.type=0x8009;d.name="same";d.script=0x1000;d.locals.fill(123);d.locals[15]=77;
 Fixture f;assert(f.pool.enter(d)==0);d.priority=20;assert(f.pool.enter(d)==1); // No deduplication.
 d.priority=0;d.type=9;assert(f.pool.enter(d)==2);
 assert(f.pool.mark(0) && f.pool.mark(1));auto held=f.services();held.stopNpcs=nullptr;
 assert(!f.pool.dispatch(held));assert(f.pool.active(0)->eventId && !f.pool.active(1)->eventId && (f.pool.active(1)->flags&2));
 assert(f.pool.dispatch(f.services()));assert(f.stops==1 && f.entries[0].id==1 && f.entries[1].id==2);
 assert(f.entries[0].type==0xef && f.entries[0].priority==0 && f.entries[1].priority==20 && f.entries[0].field16c==0);
 assert(f.entries[0].locals[15]==77 && f.entries[0].userWork==0 && f.entries[0].localFlags==std::array<uint32_t,3>{});
 assert(f.pool.mark(0) && f.pool.mark(1) && f.pool.dispatch(f.services()));assert(f.stops==1 && f.runtime.allocationCount==2);
 deleteEventId(1,f.creation());assert(f.pool.mark(0) && f.pool.dispatch(f.services()) && f.pool.active(0)->eventId==3);
 // caseDelete alone preserves ID/fields and leaves the event alive. Reuse clears
 // the entire slot, creating a distinct case lifetime even with the same name.
 assert(f.pool.erase(0) && checkEventId(3,f.creation()) && f.pool.record(0)->eventId==3 && !f.pool.active(0));
 d.locals.fill(-1);assert(f.pool.enter(d)==0 && f.pool.active(0)->eventId==0 && f.pool.active(0)->flags==1);
 assert(f.pool.eraseEvent(0,1,f.creation()) && !checkEventId(2,f.creation()) && f.pool.record(1)->eventId==2);
 assert(f.pool.eraseEvent(7,1,f.creation()) && !f.pool.eraseEvent(0,1,f.creation()));
 f.pool.reinitialize();assert(!f.pool.active(0) && !f.pool.active(2) && f.pool.record(0)->locals[0]==-1 && checkEventId(3,f.creation()));
 FieldCasePool full;d.name=std::string(63,'x');for(int n=0;n<64;++n)assert(full.enter(d)==n);
 assert(!full.enter(d));assert(full.erase(29) && full.enter(d)==29);d.name+= 'x';assert(full.erase(0) && !full.enter(d) && !full.active(0));
 assert(!full.active(-1) && !full.record(64) && !full.erase(INT_MAX) && !full.mark(-1));
 Fixture exhausted;for(auto& e:exhausted.entries){e.flags=1;e.id=55;}
 d.name="case";assert(exhausted.pool.enter(d)==0 && exhausted.pool.mark(0));assert(!exhausted.pool.dispatch(exhausted.services()));
 assert(exhausted.pool.active(0)->eventId==0 && (exhausted.pool.active(0)->flags&2));
}
