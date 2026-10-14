#include "field_previous_party.hpp"
#include "field_area_event.hpp"
#include "animation_group_prefetch.hpp"
#include "event_lifecycle.hpp"
#include "game.hpp"
#include <cassert>
#include <map>
#include <cstring>
using namespace ttyd;
struct Fixture {
 uintptr_t current=0x1000;uint32_t selector=0;std::map<uintptr_t,uint8_t> bytes;
 std::vector<std::pair<char,uintptr_t>> trace;std::optional<int32_t> result;std::optional<uintptr_t> pointer=uintptr_t(0);
 unsigned enters=0;int32_t kind=0,index=0;bool second=false,changeOwner=false,changeStored=false;
 static uint32_t word(void* p,uint32_t address){auto& f=*static_cast<Fixture*>(p);assert(address==0x80);f.trace.push_back({'a',address});return f.selector;}
 FieldPreviousPartyWorld world(){FieldPreviousPartyWorld w;w.user=this;
  w.mario=[](void* p){auto& f=*static_cast<Fixture*>(p);f.trace.push_back({'m',f.current});return f.current;};
  w.byte=[](void* p,uintptr_t a){auto& f=*static_cast<Fixture*>(p);f.trace.push_back({'b',a});return f.bytes.at(a);};
  w.storeByte=[](void* p,uintptr_t a,uint8_t v){auto& f=*static_cast<Fixture*>(p);f.trace.push_back({'B',a});f.bytes[a]=f.changeStored?uint8_t(1):v;};
  w.enter=[](void* p,int32_t kind,bool second)->std::optional<int32_t>{auto& f=*static_cast<Fixture*>(p);++f.enters;f.kind=kind;f.second=second;f.trace.push_back({'e',uintptr_t(second)});if(f.changeOwner)f.current=0x2000;return f.result;};
  w.party=[](void* p,int32_t index)->std::optional<uintptr_t>{auto& f=*static_cast<Fixture*>(p);f.index=index;f.trace.push_back({'p',uintptr_t(uint32_t(index))});return f.pointer;};return w;
 }
};
int main(){EventEntry e;e.arguments=0x80;e.locals[0]=99;e.waitCount=100;
 // Source early returns do not touch the registry, including a positive byte
 // outside the admitted native pool. Each raw selector owns distinct bytes.
 for(unsigned selector:{0,1})for(unsigned index:{0,1,127}){Fixture f;f.selector=selector;f.bytes[0x1245+selector]=index;FieldPreviousPartyCursor c;
  assert(fieldPreviousPartyEvent(e,true,c,&f,Fixture::word,f.world())==2 && f.enters==0);
  assert((f.trace==std::vector<std::pair<char,uintptr_t>>{{'m',0x1000},{'a',0x80},{'b',0x1245+selector}}));}
 for(unsigned selector:{0,1}){Fixture f;f.selector=selector;f.bytes[0x1245+selector]=255;f.bytes[0x1247+selector]=0;FieldPreviousPartyCursor c;
  assert(fieldPreviousPartyEvent(e,true,c,&f,Fixture::word,f.world())==2 && f.enters==0 && f.trace.size()==4);}
 // Missing first-phase resources retain actual captured owner/kind/selector;
 // subsequent VM first=false must never bypass the required constructor.
 for(unsigned selector:{0,1}){Fixture f;f.selector=selector;f.bytes[0x1245+selector]=255;f.bytes[0x1247+selector]=1;FieldPreviousPartyCursor c;auto w=f.world();w.enter=nullptr;
  assert(fieldPreviousPartyEvent(e,true,c,&f,Fixture::word,w)==0 && c.stage==FieldPreviousPartyCursor::Stage::Constructor && f.enters==0);
  Fixture other;other.result=0;assert(fieldPreviousPartyEvent(e,false,c,&f,Fixture::word,other.world())==0 && other.enters==0);
  f.current=0x2000;f.selector=1-selector;f.bytes[0x1247+selector]=7;
  f.result=256;f.changeStored=true;f.pointer=0xabc;f.changeOwner=true;
  assert(fieldPreviousPartyEvent(e,false,c,&f,Fixture::word,f.world())==2 && f.kind==1 && f.second==(selector!=0) && f.index==1 && f.bytes.at(0x1245+selector)==1);
  assert(f.trace[0]==std::make_pair('m',uintptr_t(0x1000)) && f.enters==1 && !f.bytes.count(0x2245+selector));
  // Completed calls have no replay; area discards this cursor after completion.
  assert(fieldPreviousPartyEvent(e,false,c,&f,Fixture::word,f.world())==2 && f.enters==1);
 }
 {Fixture f;f.bytes={{0x1245,255},{0x1247,1}};f.result=-9;FieldPreviousPartyCursor c;assert(fieldPreviousPartyEvent(e,true,c,&f,Fixture::word,f.world())==2 && f.bytes[0x1245]==255 && f.trace.back().first=='e');}
 {Fixture f;f.bytes={{0x1245,0}};f.pointer={};FieldPreviousPartyCursor c;
  assert(fieldPreviousPartyEvent(e,false,c,&f,Fixture::word,f.world())==0 && c.stage==FieldPreviousPartyCursor::Stage::Lookup);
  f.bytes[0x1245]=1;f.pointer=uintptr_t(0);assert(fieldPreviousPartyEvent(e,false,c,&f,Fixture::word,f.world())==2 && f.enters==0 && f.index==0);}
 {Fixture f;f.bytes={{0x1245,255},{0x1247,1}};f.result=0;FieldPreviousPartyCursor c;auto w=f.world();w.party=nullptr;
  assert(fieldPreviousPartyEvent(e,true,c,&f,Fixture::word,w)==0 && c.stage==FieldPreviousPartyCursor::Stage::Lookup && f.enters==1);
  f.bytes[0x1245]=1;assert(fieldPreviousPartyEvent(e,false,c,&f,Fixture::word,f.world())==2 && f.index==0 && f.enters==1);}
 {Fixture f;f.bytes={{0x1245,255},{0x1247,1}};FieldPreviousPartyCursor c;
  assert(fieldPreviousPartyEvent(e,true,c,&f,Fixture::word,f.world())==0);f.bytes[0x1247]=0;
  assert(fieldPreviousPartyEvent(e,true,c,&f,Fixture::word,f.world())==2); // Actual restarted first phase.
 }
 {Fixture f;f.selector=2;FieldPreviousPartyCursor c;assert(fieldPreviousPartyEvent(e,true,c,&f,Fixture::word,f.world())==0 && f.trace.size()==2);}
 {Fixture f;f.bytes={{0x1245,255},{0x1247,0}};FieldPreviousPartyCursor c;
  assert(fieldPreviousPartyEvent(e,true,c,&f,Fixture::word,{})==0);assert(fieldPreviousPartyEvent(e,false,c,&f,Fixture::word,f.world())==2 && f.enters==0);}
 assert(e.locals[0]==99 && e.waitCount==100);
 // Actual constructor bytes and real negative/null lookup, without fake Entry.
 Game game(false);auto native=game.fieldPreviousPartyWorld();
 for(unsigned selector:{0,1}){Fixture operands;operands.selector=selector;FieldPreviousPartyCursor c;
  assert(fieldPreviousPartyEvent(e,true,c,&operands,Fixture::word,native)==2);assert(game.fieldMarioBattleState().partyIndices[selector]==255);}
 auto& state=const_cast<FieldMarioBattleState&>(game.fieldMarioBattleState());state.savedPartyKinds[0]=1;
 Fixture operands;FieldPreviousPartyCursor held;assert(fieldPreviousPartyEvent(e,true,held,&operands,Fixture::word,native)==0 && game.fieldPartyState().pointer(0)==nullptr);
 state.savedPartyKinds[0]=0;assert(fieldPreviousPartyEvent(e,false,held,&operands,Fixture::word,native)==0); // Captured nonzero request cannot be erased by later bytes.
 state.partyIndices[0]=127;FieldPreviousPartyCursor early;assert(fieldPreviousPartyEvent(e,true,early,&operands,Fixture::word,native)==2);
 FieldPreviousPartyCursor resumed;assert(fieldPreviousPartyEvent(e,false,resumed,&operands,Fixture::word,native)==0);state.partyIndices[0]=255;
 // Delayed typed-provider installation in the real area still starts the source
 // first phase. The attached initial_pos is a third live event, never a new VM.
 assert(loadFieldAreaEventAssets("romfs") && loadFieldMapFlagAssets("romfs") && loadFieldBeroInfoAssets("romfs"));
 FieldAreaEvent area;assert(area.begin(2,0));FieldAreaGpState image{};std::memcpy(image.bytes.data()+0x11c-FieldAreaGpState::Begin,"w_bero",7);
 image.bytes[0x176-FieldAreaGpState::Begin]=1;image.bytes[0x177-FieldAreaGpState::Begin]=146;const auto root=area.appendAreaInit(image);assert(root);
 FieldMapFlags map;map.synchronize(2);FieldBeroInfo info;info.synchronize(2);FieldBeroExec exec;AnimationGroupPrefetch cache;assert(cache.open("romfs/field-prefetch"));
 struct Trace {bool normal=false;uint32_t child=0,parent=0;EventCreationServices creation;EventEntry* bero=nullptr;} trace;
 FieldAreaEventServices s;s.map=&map;s.beroInfo=&info;s.beroExec=&exec;s.prefetch=&cache;s.mario=game.fieldMarioPreparationWorld();s.user=&trace;
 s.call=[](void* p,FieldAreaNativeCall& r){auto& t=*static_cast<Trace*>(p);
  if(r.function==0x800e3058){t.bero=&r.event;t.creation=r.creation;}
  if(r.function==0x80079b74){t.normal=true;t.child=r.event.id;assert(r.event.waitingParent && r.event.waitingParent->waitingParent);t.parent=r.event.waitingParent->id;assert(r.event.waitingParent->waitingChild==&r.event && r.event.locals[10]==-230000000+30*1024 && uint32_t(r.event.locals[3])==0x8100011c);t.creation=r.creation;}
  return 0;};
 for(unsigned tick=1;tick<40 && area.pendingFunction!=0x800e3058;++tick)area.tick(tick*17,s);
 assert(area.pendingFunction==0x800e3058 && area.liveEventCount()==2 && !trace.normal);
 assert(trace.bero);state.savedPartyKinds[0]=1;s.previousParty=game.fieldPreviousPartyWorld();
 area.tick(680,s);assert(!trace.normal && area.pendingFunction==0x800e3058 && area.liveEventCount()==2);
 // Real source restart preserves the event ID but must create a fresh first
 // phase. Clear source busy1 explicitly for this controlled restart fixture.
 state.savedPartyKinds[0]=0;exec.onOff(1,1);const auto restartedId=trace.bero->id;restartEvent(*trace.bero,trace.creation);
 for(unsigned tick=41;tick<52 && !trace.normal;++tick)area.tick(tick*17,s);
 assert(trace.parent==restartedId);
 assert(!area.failed && trace.normal && area.pendingFunction==0x80079b74 && area.liveEventCount()==3 && area.eventAlive(*root) && area.eventAlive(trace.parent) && area.eventAlive(trace.child));
 assert(game.fieldMarioBattleState().keyDepth==2 && game.fieldKeysOff && game.fieldPartyState().pointer(0)==nullptr && game.fieldPartyState().pointer(1)==nullptr);
 deleteEventId(*root,trace.creation);area.tick(1000,s);assert(!area.failed && area.liveEventCount()==0 && area.completed().size()==1 && !area.completed()[0].returned && game.fieldKeysOff);
}
