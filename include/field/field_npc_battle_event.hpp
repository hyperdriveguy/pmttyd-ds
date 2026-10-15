#pragma once
#include "battle_sequence_entry.hpp"
namespace ttyd {
// Complete source USER_FUNC parents. Child calls retain their original address
// and arguments. A false provider result means no completed effects; its own
// continuation must survive retries. Native suspension is distinct from source
// return0, which completes this invocation and starts a fresh resumed visit.
struct FieldNpcBattleEventCursor {
 bool active=false,queued=false,first=false;
 uint32_t function=0,entry=0,phase=0,pending=0,args=0,raw=0,name=0,field=0,npc=0,party=0,value=0;
 void* owner=nullptr;
 BattleSequenceCall child;
};
bool fieldNpcBattleEventFunction(uint32_t);
// true publishes the original return (0/2, or1 for get_reglid). false retains
// the invocation; caller must not replace its original first flag on retry.
bool fieldNpcBattleEvent(uint32_t function,uint32_t entry,bool first,
 const BattleSequenceEntryServices&,FieldNpcBattleEventCursor&,int32_t& result);
class EventNativeRuntime;struct EventEntry;
// Actual caller VM supplies argument/value/output operations. External services
// own field/NPC/sequence/Mario/party/audio children and keep their identities
// stationary until completion or actual caller retirement. No fixture fallback.
class NativeFieldNpcBattleScriptCall {
public:
 bool step(uint32_t,EventEntry&,bool,EventNativeRuntime&,
           const BattleSequenceEntryServices&,int32_t& result);
 uint32_t pending() const{return cursor_.pending?cursor_.pending:function_;}
 bool releaseForRetirement() const{return !bound_ || !cursor_.active;}
private:
 EventNativeRuntime* vm_=nullptr;EventEntry* entry_=nullptr;
 uint32_t id_=0,function_=0,address_=0,args_=0;bool first_=false,bound_=false;
 BattleSequenceEntryServices binding_;FieldNpcBattleEventCursor cursor_;
 BattleSequenceEntryServices services();
};
}
