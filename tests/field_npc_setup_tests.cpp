#include "field_npc_setup.hpp"
#include "event_manager.hpp"
#include <cassert>
#include <map>
#include <vector>
#include <algorithm>
using namespace ttyd;
struct Fixture {
 std::map<uint32_t,uint32_t> data;std::map<uint32_t,uint32_t> names;
 struct Op {char kind;uint32_t a,b;};std::vector<Op> trace;
 std::map<uint32_t,EventEntry> entries;uint32_t next=100,resolved=0x1000;
 unsigned getters=0,modes=0,battles=0,capacity=999;bool admitted=true,blocked=false,mutate=false;
 uint32_t deleteRow=0,deleteActor=0,deleteScript=0;unsigned created=0;
 Fixture(){data[0x80]=0x1000;}
 void row(uint32_t table,uint32_t name,uint32_t actor,uint32_t init,uint32_t regular) {
  for(unsigned n=0;n<23;++n)data[table+4*n]=0x700+n;
  data[table]=name;data[table+4]=0x80600;data[table+8]=0x80;data[table+12]=init;data[table+16]=regular;
  data[table+0x58]=0;data[table+0x5c]=0;names[name]=actor;
  if(actor){data[actor]=3;data[actor+4]=0x20;data[actor+0x28]=0x8000;data[actor+0x104]=0x9000;}
  data[0x8008]=0x8100;data[0x8010]=0x8200;data[0x8014]=0x8300;
 }
 FieldNpcSetupOperands operands(){return {this,[](void* p,uint32_t a){auto& f=*static_cast<Fixture*>(p);f.trace.push_back({'r',a,0});return f.data[a];},
  [](void* p,EventEntry&,int32_t raw){auto& f=*static_cast<Fixture*>(p);f.trace.push_back({'v',uint32_t(raw),0});return int32_t(f.resolved);}};}
 FieldNpcSetupWorld world(){FieldNpcSetupWorld w;w.user=this;
  w.fieldPointer=[](void* p){auto& f=*static_cast<Fixture*>(p);++f.getters;f.trace.push_back({'g',0,0});return 0xdeadbeefu;};
  w.admit=[](void* p,const EventEntry&,uint32_t table,bool first,const FieldNpcSetupOperands&){auto& f=*static_cast<Fixture*>(p);f.trace.push_back({'A',table,unsigned(first)});return f.admitted;};
  w.npc=[](void* p,uint32_t name,bool asserting,const FieldNpcSetupOperands&){auto& f=*static_cast<Fixture*>(p);f.trace.push_back({asserting?'N':'n',name,0});return f.names.at(name);};
  w.word=[](void* p,uint32_t a){auto& f=*static_cast<Fixture*>(p);f.trace.push_back({'r',a,0});return f.data[a];};
  w.store=[](void* p,uint32_t a,uint32_t v){auto& f=*static_cast<Fixture*>(p);f.trace.push_back({'w',a,v});f.data[a]=v;
   if(f.mutate && a==0x4208){f.data[0x1040]=0xaabbccdd;f.data[0x1044]=0xeeff0011;}
  };
  w.battle=[](void* p,uint32_t actor,uint32_t setup){auto& f=*static_cast<Fixture*>(p);++f.battles;f.trace.push_back({'b',actor,setup});};
  w.copyString=[](void* p,uint32_t to,uint32_t from){static_cast<Fixture*>(p)->trace.push_back({'s',to,from});};
  w.setAnimation=[](void* p,uint32_t pose,uint32_t name,int32_t mode){assert(mode==1);static_cast<Fixture*>(p)->trace.push_back({'a',pose,name});};
  w.mode=[](void* p,int32_t mode){assert(mode==1);auto& f=*static_cast<Fixture*>(p);++f.modes;f.trace.push_back({'m',1,0});};return w;
 }
 FieldNpcSetupEvents events(){return {this,
  [](void* p,uint32_t id){auto& f=*static_cast<Fixture*>(p);f.trace.push_back({'d',id,0});f.entries.erase(id);
   if(f.deleteRow)f.data[f.deleteRow]=f.deleteScript;
   if(f.deleteActor){f.data[f.deleteActor+0x28]=0xbeef;f.data[f.deleteActor+0x124]=f.deleteScript;}
  },
  [](void* p,uint32_t id){auto& f=*static_cast<Fixture*>(p);f.trace.push_back({'?',id,0});return f.entries.count(id)!=0;},
  [](void* p,uint32_t script,uint8_t priority,uint8_t flags)->EventEntry*{auto& f=*static_cast<Fixture*>(p);assert(priority==0);f.trace.push_back({'c',script,flags});
   if(f.blocked || f.entries.size()>=f.capacity)return nullptr;
   auto& e=f.entries[++f.next];e.id=f.next;e.restartScript=script;e.priority=priority;e.flags=flags|1;++f.created;return &e;
  }};}
 int call(EventEntry& e,bool first,FieldNpcSetupCursor& c){return fieldNpcSetupEvent(e,first,c,operands(),world(),events());}
};
int main(){EventEntry e;e.arguments=0x80;e.locals.fill(77);e.waitCount=99;
 // Unavailable first admission retains source first, resolved table and owner.
 Fixture f;f.row(0x1000,0x2000,0x4000,0x5000,0x6000);FieldNpcSetupCursor c;f.admitted=false;
 assert(f.call(e,true,c)==0 && c.sourceFirst && f.getters==1 && f.created==0 && f.data[0x4000]==3);
 EventEntry wrong;wrong.arguments=e.arguments;assert(f.call(wrong,false,c)==0 && f.getters==1);
 Fixture other;assert(fieldNpcSetupEvent(e,false,c,f.operands(),other.world(),f.events())==0 && other.getters==0);
 auto foreignMemory=f.operands();foreignMemory.user=&other;assert(fieldNpcSetupEvent(e,false,c,foreignMemory,f.world(),f.events())==0 && other.getters==0);
 auto foreignEvents=f.events();foreignEvents.user=&other;assert(fieldNpcSetupEvent(e,false,c,f.operands(),f.world(),foreignEvents)==0);
 // Actual deletion precedes allocation admission and the source rereads script.
 f.admitted=true;f.blocked=true;f.resolved=0x90000000;f.data[0x4118]=20;f.entries[20].id=20;
 f.deleteRow=0x100c;f.deleteActor=0x4000;f.deleteScript=0x5050;
 assert(f.call(e,false,c)==0 && !f.entries.count(20) && c.stage==FieldNpcSetupCursor::Stage::InitCreate && c.script==0x5050 && f.getters==1 && f.battles==0);
 f.data[0x100c]=0x9999;f.deleteRow=f.deleteActor=0;f.mutate=true;f.blocked=false;
 assert(f.call(e,false,c)==0 && !c.initialized && f.getters==1 && f.created==1 && f.modes==1 && f.battles==1);
 const auto init=f.data.at(0x4118);assert(init && f.entries.at(init).restartScript==0x5050 && f.entries.at(init).field170==0x4000 && f.entries.at(init).rawBytes[0]==0 && !f.entries.at(init).waitingParent);
 assert(f.data[0x4000]==0x80603 && f.data[0x4004]==0xa0 && f.data[0x4124]==0x6000);
 // Pair reads precede stores, and the second metadata triple is read afresh.
 assert(f.data[0x420c]==0x710 && f.data[0x4218]==0xaabbccdd && f.data[0x421c]==0xeeff0011);
 // Captured descriptor survives an init-deletion mutation to actor+28.
 auto copies=std::vector<Fixture::Op>{};for(const auto& op:f.trace)if(op.kind=='s')copies.push_back(op);
 assert(copies.size()==3 && copies[0].a==0x404c && copies[0].b==0x8200 && copies[1].a==0x406c && copies[1].b==0x8300 && copies[2].a==0x402c && copies[2].b==0x8100);
 assert(e.locals[0]==77 && e.waitCount==99);
 // A genuine source wait does not retain the old table or polling snapshot.
 f.resolved=0x1000;assert(f.call(e,false,c)==0 && !c.initialized && f.getters==2 && f.created==1);
 f.entries.erase(init);f.data[0x4124]=0x6060;f.data[0x411c]=30;f.entries[30].id=30;
 f.blocked=true;f.deleteActor=0x4000;f.deleteScript=0x6161;
 assert(f.call(e,false,c)==0 && c.stage==FieldNpcSetupCursor::Stage::RegularCreate && !f.entries.count(30) && c.script==0x6161);
 f.data[0x4124]=0x9999;f.blocked=false;f.deleteActor=0;
 assert(f.call(e,false,c)==2 && !c.initialized && f.created==2 && f.modes==1);
 const auto regular=f.data.at(0x411c);assert(f.entries.at(regular).restartScript==0x6161 && f.entries.at(regular).flags==0x21 && f.entries.at(regular).field170==0x4000 && f.entries.at(regular).rawBytes[0]==1);
 // Null init/regular scripts preserve the different source old-ID semantics.
 Fixture nulls;nulls.row(0x1000,0x2000,0x4000,0,0);nulls.data[0x4118]=40;nulls.data[0x411c]=41;nulls.entries[40].id=40;nulls.entries[41].id=41;FieldNpcSetupCursor n;
 assert(nulls.call(e,true,n)==0 && nulls.data[0x4118]==0 && nulls.entries.count(40));
 assert(nulls.call(e,false,n)==2 && nulls.entries.count(41) && nulls.data[0x411c]==41 && nulls.created==0);
 // Source first=false resolves a newly selected table after a true return0.
 Fixture changed;changed.row(0x1000,0x2000,0x4000,0x5000,0);changed.row(0x1100,0x2100,0x4400,0,0);FieldNpcSetupCursor fresh;
 assert(changed.call(e,true,fresh)==0);changed.resolved=0x1100;assert(changed.call(e,false,fresh)==2 && changed.created==1);
 // Missing source actor really skips first-phase row; later lookup must be
 // separately admitted, not forged into an initialized actor.
 Fixture missing;missing.row(0x1000,0x2000,0,0x5000,0x6000);FieldNpcSetupCursor absent;
 assert(missing.call(e,true,absent)==0 && missing.created==0 && missing.battles==0 && missing.modes==1);
 missing.admitted=false;assert(missing.call(e,false,absent)==0 && absent.stage==FieldNpcSetupCursor::Stage::Admission);
 // Explicit new first visit restarts installation rather than resume a hold.
 Fixture restart;restart.row(0x1000,0x2000,0x4000,0x5000,0);restart.blocked=true;FieldNpcSetupCursor r;
 assert(restart.call(e,true,r)==0);restart.data[0x100c]=0;restart.blocked=false;assert(restart.call(e,true,r)==0 && restart.created==0 && restart.modes==1 && restart.getters==2);
 Fixture late;late.row(0x1000,0x2000,0x4000,0x5000,0);FieldNpcSetupCursor l;
 assert(fieldNpcSetupEvent(e,true,l,late.operands(),{},late.events())==0);
 assert(late.call(e,false,l)==0 && late.created==1 && late.modes==1);
 // A full pool is not a reason to reject the invocation before the required
 // old-event deletion, which itself frees the replacement's real slot.
 Fixture full;full.row(0x1000,0x2000,0x4000,0x5000,0);full.capacity=1;full.entries[1].id=1;full.data[0x4118]=1;FieldNpcSetupCursor freed;
 assert(full.call(e,true,freed)==0 && full.created==1 && full.entries.size()==1 && !full.entries.count(1));
}
