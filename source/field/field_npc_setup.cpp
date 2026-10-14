#include "field_npc_setup.hpp"
#include "event_manager.hpp"
#include <cassert>
namespace ttyd {
namespace {
int32_t signedWord(uint32_t v){return v<=INT32_MAX?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}
}
int32_t fieldNpcSetupEvent(EventEntry& e,bool first,FieldNpcSetupCursor& c,
 const FieldNpcSetupOperands& o,const FieldNpcSetupWorld& w,const FieldNpcSetupEvents& events) {
 if(first)c={};
 c.firstVisit(first);
 if(c.stage!=FieldNpcSetupCursor::Stage::Start && (c.owner!=w.user || c.memoryOwner!=o.user || c.eventOwner!=events.user || c.entry!=&e || c.eventId!=e.id))return 0;
 if(c.stage==FieldNpcSetupCursor::Stage::Start) {
  if(!o.word || !o.value || !w.fieldPointer)return 0;
  const auto args=e.arguments;c.table=uint32_t(o.value(o.user,e,signedWord(o.word(o.user,args))));
  c.owner=w.user;c.memoryOwner=o.user;c.eventOwner=events.user;c.entry=&e;c.eventId=e.id;w.fieldPointer(w.user);c.stage=FieldNpcSetupCursor::Stage::Admission;
 }
 if(!o.word || !w.admit || !w.npc || !w.word || !w.store || !w.battle || !w.copyString || !w.setAnimation || !w.mode || !events.create || !events.erase || !events.alive)return 0;
 if(c.stage==FieldNpcSetupCursor::Stage::Admission) {
  if(!w.admit(w.user,e,c.table,c.sourceFirst,o))return 0;
  c.row=c.table;c.stage=c.sourceFirst?FieldNpcSetupCursor::Stage::FirstRow:FieldNpcSetupCursor::Stage::PollRow;
 }
 for(;;) {
  if(c.stage==FieldNpcSetupCursor::Stage::FirstRow) {
   const auto name=o.word(o.user,c.row);
   if(!name){w.mode(w.user,1);c={};return 0;} // Genuine first-phase return, not a resource hold.
   c.actor=w.npc(w.user,name,false,o);
   if(!c.actor){c.row+=0x5c;continue;}
   const auto flags=w.word(w.user,c.actor),mask=o.word(o.user,c.row+4);
   c.descriptor=w.word(w.user,c.actor+0x28);w.store(w.user,c.actor,flags|mask);
   const auto secondary=w.word(w.user,c.actor+4),secondaryMask=o.word(o.user,c.row+8);w.store(w.user,c.actor+4,secondary|secondaryMask);
   const auto init=o.word(o.user,c.row+0xc);
   if(init) {
    const auto old=w.word(w.user,c.actor+0x118);if(old)events.erase(events.user,old);
    c.script=o.word(o.user,c.row+0xc);c.stage=FieldNpcSetupCursor::Stage::InitCreate;
   } else {w.store(w.user,c.actor+0x118,0);c.stage=FieldNpcSetupCursor::Stage::Metadata;}
  }
  if(c.stage==FieldNpcSetupCursor::Stage::InitCreate) {
   auto* child=events.create(events.user,c.script,0,0);if(!child)return 0;
   child->rawBytes[0]=0;child->field170=c.actor;w.store(w.user,c.actor+0x118,child->id);c.stage=FieldNpcSetupCursor::Stage::Metadata;
  }
  if(c.stage==FieldNpcSetupCursor::Stage::Metadata) {
   const auto copy=[&](uint32_t from,uint32_t to){w.store(w.user,c.actor+to,o.word(o.user,c.row+from));};
   const auto pair=[&](uint32_t from,uint32_t to){const auto a=o.word(o.user,c.row+from),b=o.word(o.user,c.row+from+4);w.store(w.user,c.actor+to,a);w.store(w.user,c.actor+to+4,b);};
   copy(0x14,0x128);copy(0x18,0x12c);copy(0x10,0x124);
   for(unsigned n=0;n<4;++n)copy(0x1c+4*n,0x130+4*n);
   copy(0x2c,0x1f8);pair(0x30,0x1fc);copy(0x38,0x204);
   pair(0x3c,0x208);copy(0x44,0x210);pair(0x3c,0x214);copy(0x44,0x21c);
   // Admitted finite source float words retain their IEEE representation.
   for(unsigned n=0;n<4;++n)copy(0x48+4*n,0x220+4*n);
   w.battle(w.user,c.actor,o.word(o.user,c.row+0x58));
   auto pose=w.word(w.user,c.descriptor+0x10);if(pose)w.copyString(w.user,c.actor+0x4c,pose);
   pose=w.word(w.user,c.descriptor+0x14);if(pose)w.copyString(w.user,c.actor+0x6c,pose);
   pose=w.word(w.user,c.descriptor+8);
   if(pose){w.copyString(w.user,c.actor+0x2c,pose);w.setAnimation(w.user,w.word(w.user,c.actor+0x104),c.actor+0x2c,1);}
   c.row+=0x5c;c.stage=FieldNpcSetupCursor::Stage::FirstRow;
  }
  if(c.stage==FieldNpcSetupCursor::Stage::PollRow) {
   const auto name=o.word(o.user,c.row);
   if(!name){c.row=c.table;c.stage=FieldNpcSetupCursor::Stage::RegularRow;continue;}
   const auto actor=w.npc(w.user,name,true,o);assert(actor);
   const auto init=w.word(w.user,actor+0x118);
   if(init && events.alive(events.user,init)){c={};return 0;} // Genuine poll ends this invocation.
   c.row+=0x5c;continue;
  }
  if(c.stage==FieldNpcSetupCursor::Stage::RegularRow) {
   const auto name=o.word(o.user,c.row);if(!name){c={};return 2;}
   c.actor=w.npc(w.user,name,true,o);assert(c.actor);
   const auto regular=w.word(w.user,c.actor+0x124);
   if(!regular){c.row+=0x5c;continue;}
   const auto old=w.word(w.user,c.actor+0x11c);if(old)events.erase(events.user,old);
   c.script=w.word(w.user,c.actor+0x124);c.stage=FieldNpcSetupCursor::Stage::RegularCreate;
  }
  if(c.stage==FieldNpcSetupCursor::Stage::RegularCreate) {
   auto* regular=events.create(events.user,c.script,0,0x20);if(!regular)return 0;
   regular->rawBytes[0]=1;regular->field170=c.actor;w.store(w.user,c.actor+0x11c,regular->id);
   c.row+=0x5c;c.stage=FieldNpcSetupCursor::Stage::RegularRow;
  }
 }
}
}
