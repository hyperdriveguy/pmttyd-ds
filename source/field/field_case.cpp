#include "field_case.hpp"
#include "event_controls.hpp"
#include "event_lifecycle.hpp"
#include <cassert>
#include <cstddef>
#include <cstring>
namespace ttyd {
// Source ABI: record a0 bytes, local bank +54, event ID +94, priority +9c.
// This semantic native pool retains that layout without emulating heap aliases.
static_assert(sizeof(FieldCaseRecord)==160 && offsetof(FieldCaseRecord,locals)==0x54 &&
 offsetof(FieldCaseRecord,eventId)==0x94 && offsetof(FieldCaseRecord,priority)==0x9c,"case record layout");
std::optional<int32_t> FieldCasePool::enter(const FieldCaseDescriptor& d) {
 if(d.name.size()>=64 || d.name.find('\0')!=std::string::npos)return std::nullopt;
 for(unsigned n=0;n<Capacity;++n)if(!(records_[n].flags&1)) {
  auto& r=records_[n];r={};r.index=int32_t(n);r.type=d.type&~0x8000u;
  r.flags=uint16_t(1|((d.type&0x8000)?4:0));
  std::memcpy(r.name.data(),d.name.c_str(),d.name.size()+1);
  r.argument=d.argument;r.callback=d.callback;r.script=d.script;r.priority=d.priority;
  r.eventId=0;r.locals=d.locals;return int32_t(n);
 }
 return std::nullopt; // Native admission boundary; original overruns allocation.
}
const FieldCaseRecord* FieldCasePool::record(int32_t n) const {return n>=0 && unsigned(n)<Capacity?&records_[unsigned(n)]:nullptr;}
const FieldCaseRecord* FieldCasePool::active(int32_t n) const {const auto* r=record(n);return r && (r->flags&1)?r:nullptr;}
bool FieldCasePool::erase(int32_t n) {if(!record(n))return false;records_[unsigned(n)].flags&=uint16_t(~1u);return true;}
void FieldCasePool::reinitialize(){for(auto& r:records_)r.flags&=uint16_t(~1u);}
bool FieldCasePool::mark(int32_t n){if(!active(n))return false;records_[unsigned(n)].flags|=2;return true;}
// Second pass consumes active+marked records in slot order. Check stale IDs
// before construction; a provider/capacity hold preserves the current mark.
// Child flag20(hex) and field16c carry original case ownership, not execution.
bool FieldCasePool::dispatch(const FieldCaseDispatchServices& s) {
 assert(s.schedule);
 for(auto& r:records_)if((r.flags&3)==3) {
  if(!checkEventId(r.eventId,s.creation))r.eventId=0;
  if(!r.eventId) {
   // Native provider/capacity admission happens before the original constructor.
   // Retain the mark on hold; never publish a fabricated ID or skip NPC stop.
   if(r.priority==20 && !s.stopNpcs)return false;
   auto* gp=s.creation.manager.globals(s.creation.manager.user);
   auto* context=s.creation.manager.context(s.creation.manager.user,gp->battleContext!=0);bool free=false;
   for(int n=0;n<context->capacity;++n)if(!(context->entries[n].flags&1)){free=true;break;}
   if(!free)return false;
   auto& e=createRootEvent(*s.schedule,r.script,r.priority,0x20,s.creation);
   e.field16c=r.index;r.eventId=e.id;e.locals=r.locals;
   if(r.priority==20)s.stopNpcs(s.user);
  }
  r.flags&=uint16_t(~2u);
 }
 return true;
}
// Mode0 deletes a live event before clearing active. Other modes clear only
// the case; erase/reinitialize do not zero the payload or stop existing events.
bool FieldCasePool::eraseEvent(int32_t mode,int32_t n,const EventCreationServices& s) {
 if(!record(n))return false;
 if(mode==0){const auto* r=active(n);if(!r)return false; // Native live-case admission.
  if(checkEventId(r->eventId,s))deleteEventId(r->eventId,s);}
 return erase(n);
}
int32_t fieldCaseNumber(int32_t raw) {
 // Source signed <= comparisons. First three namespaces remain raw; remaining
 // variable families become their index without consulting an event/context.
 if(raw<=-220000000)return raw;
 // EVT variable namespaces are spaced by 20,000,000; subtract the family
 // base (limit-10,000,000) using signed comparisons. This is not evtGetValue.
 constexpr int32_t limits[]={-200000000,-180000000,-160000000,-140000000,
  -120000000,-100000000,-80000000,-60000000,-40000000,-20000000};
 for(const auto limit:limits)if(raw<=limit)return int32_t(int64_t(raw)-int64_t(limit-10000000));
 return raw;
}
// Argument layout: type, flag, name, number-token, script, raw destination.
// Read number via evtGetNumber semantics; do not resolve its variable value.
// Snapshot locals before output, then freshly read destination after admission.
int32_t fieldRunCaseEvent(EventEntry& e,bool bero,const FieldCaseEventServices& s) {
 assert(s.word && s.value && s.setValue && s.text && s.owner);
 const auto args=e.arguments;
 const auto type=s.value(s.user,e,int32_t(s.word(s.user,args)));
 const auto flag=s.value(s.user,e,int32_t(s.word(s.user,args+4)));
 const auto name=s.value(s.user,e,int32_t(s.word(s.user,args+8)));
 const auto argument=fieldCaseNumber(int32_t(s.word(s.user,args+12)));
 const auto script=s.value(s.user,e,int32_t(s.word(s.user,args+16)));
 FieldCaseDescriptor d;d.type=uint16_t(uint32_t(type)|(flag?0x8000:0));
 d.argument=uint32_t(argument);d.script=uint32_t(script);d.priority=bero?20:0;d.locals=e.locals;
 if(!s.text(s.user,uint32_t(name),d.name))return 0;
 auto* pool=s.owner(s.user);assert(pool);const auto index=pool->enter(d);if(!index)return 0;
 const auto destination=int32_t(s.word(s.user,args+20));
 if(destination)s.setValue(s.user,e,destination,*index);
 return 2;
}
}
