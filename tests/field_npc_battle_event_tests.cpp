#include "field_npc_battle_event.hpp"
#include "harbor_encounter.hpp"
#include "event_native_runtime.hpp"
#include <cassert>
#include <algorithm>
#include <map>
#include <vector>
#include <set>
#include <cstdio>
using namespace ttyd;
namespace {
struct World {
 std::map<uint32_t,uint32_t> words{{0x10018,0x20000},{0x20000,0x1234},{0x20004,0x1005b},{0x5011c,0x6789}};
 std::vector<uint32_t> calls,reads;std::vector<BattleSequenceCall> records;
 std::set<uint32_t> held;uint32_t sequence=4,result=0,party=0x60000;
 bool hold=false,holdStore=false;unsigned stores=0;
 BattleSequenceEntryServices services(){BattleSequenceEntryServices s;s.user=this;
  s.word=[](void* p,uint32_t a)->std::optional<uint32_t>{auto& w=*static_cast<World*>(p);w.reads.push_back(a);auto n=w.words.find(a);return n==w.words.end()?std::nullopt:std::optional<uint32_t>(n->second);};
  s.store=[](void* p,uint32_t a,uint32_t v){auto& w=*static_cast<World*>(p);if(w.holdStore){w.holdStore=false;return false;}w.words[a]=v;++w.stores;return true;};
  s.call=[](void* p,BattleSequenceCall& c){auto& w=*static_cast<World*>(p);
   if(w.hold && w.held.insert(c.function).second)return false;
   w.calls.push_back(c.function);w.records.push_back(c);
   switch(c.function){
    case 0x80031260:c.result=c.args[1];break;
    case 0x8004829c:c.result=0x40000;break;
    case 0x8004f5d8:c.result=0x50000;break;
    case 0x80141050:c.result=7;break;
    case 0x800bf1b4:assert(c.args[0]==7);c.result=w.party;break;
    case 0x80114508:assert(c.args[0]==0x40020 && c.args[1]==0);w.result=0;break;
    case 0x8002e218:assert(c.args[0]==4 && !c.args[1] && !c.args[2]);w.sequence=4;break;
    case 0x8002e210:c.result=w.sequence;break;
    case 0x80114500:assert(c.args[0]==0x40020);c.result=w.result;break;
    case 0x80030cfc:assert(c.args[0]==0x10000);w.words[c.args[1]]=c.args[2];break;
    case 0x800bed44:case 0x800bed0c:assert(c.args[0]==w.party);break;
   }return true;
  };return s;
 }
};
int32_t run(World& w,uint32_t f,bool first){FieldNpcBattleEventCursor c;int32_t result=-99;
 for(unsigned n=0;n<100;++n){if(fieldNpcBattleEvent(f,0x10000,n?false:first,w.services(),c,result)){assert(!c.active && !c.pending);return result;}assert(result==-99 && c.active);}
 assert(false);return -99;
}
void parents(){
 const std::vector<uint32_t> start={0x80031260,0x80031260,0x8004829c,0x8004f5d8,0x8005c3cc,0x8005c2c4,0x80141050,0x800bf1b4,0x800bed44,0x80114508,0x800dc110,0x8002e218};
 for(bool hold:{false,true}){World w;w.hold=hold;w.holdStore=hold;assert(run(w,0x8004ef54,true)==0);assert(w.calls==start && w.stores==1 && w.words[0x40008]==0x50000);
  assert(w.records[1].args[1]==0x1005b); // Original next-command header, not a second NPC operand.
  w.calls.clear();assert(run(w,0x8004ef54,false)==2);
  assert((w.calls==std::vector<uint32_t>{0x80031260,0x80031260,0x8004829c,0x8004f5d8}));assert(w.stores==1);
 }
 for(auto sequence:{2u,4u})for(auto result:{0u,1u,5u,0xffffffffu}){World w;w.sequence=sequence;w.result=result;w.hold=true;
  assert(run(w,0x8004eed8,true)==(sequence==2 && result?2:0));
  const std::vector<uint32_t> wait={0x8004829c,0x8002e210,0x80114500,0x8005c290,0x80141050,0x800bf1b4,0x800bed0c,0x800dbcc4,0x8005c30c};
  assert(w.calls==std::vector<uint32_t>(wait.begin(),wait.begin()+(sequence!=2?2:result?9:3)));
 }
 World absent;absent.party=0;assert(run(absent,0x8004ef54,true)==0);
 assert(std::find(absent.calls.begin(),absent.calls.end(),0x800bed44)==absent.calls.end());
 absent.calls.clear();absent.sequence=2;absent.result=3;assert(run(absent,0x8004eed8,false)==2);
 assert(std::find(absent.calls.begin(),absent.calls.end(),0x800bed0c)==absent.calls.end());
}
void registration(){
 World w;w.hold=true;FieldNpcBattleEventCursor c;int32_t result=-99;
 assert(!fieldNpcBattleEvent(0x8004f894,0x10000,true,w.services(),c,result));
 // Lookup is held after name capture; destination and registration ID are late.
 assert(c.pending==0x80031260);w.words[0x20000]=0x9999;
 for(unsigned n=0;n<20 && c.phase!=33;++n)fieldNpcBattleEvent(0x8004f894,0x10000,false,w.services(),c,result);
 assert(c.phase==33 && c.queued);w.words[0x5011c]=99;w.words[0x20004]=0x70000;
 assert(fieldNpcBattleEvent(0x8004f894,0x10000,false,w.services(),c,result));assert(result==1);
 assert(w.records[0].args[1]==0x1234 && w.words[0x1005b]==0x6789 && !w.words.count(0x70000));
 assert((w.calls==std::vector<uint32_t>{0x80031260,0x8004f5d8,0x80030cfc}));
 // A different native owner cannot resume a held invocation.
 World a,b;a.hold=true;FieldNpcBattleEventCursor pinned;result=-99;
 assert(!fieldNpcBattleEvent(0x8004ef54,0x10000,true,a.services(),pinned,result));
 assert(!fieldNpcBattleEvent(0x8004ef54,0x10000,false,b.services(),pinned,result));assert(b.calls.empty());
}
void caller(){
 EventNativePlatform platform;platform.flushBlock=[](void*,uint32_t){};platform.systemCall=[](void*){};
 EventNativeRuntime vm(0x81000000,platform);EventEntry e;e.id=37;e.arguments=0x20000;
 assert(vm.entryAddresses.registerEntries(0x10000,&e,1));
 for(uint32_t a:std::array<uint32_t,2>{{0x20000,0x30000}})assert(vm.memory.registerRam(a,std::vector<uint8_t>(16)));
 assert(vm.memory.writeWord(0x20000,0x1111) && vm.memory.writeWord(0x20004,0x1005b));
 assert(vm.memory.writeWord(0x30000,0x2222) && vm.memory.writeWord(0x30004,0x1005b));
 NativeFieldNpcBattleScriptCall c;int32_t result=-99;World w;w.hold=true;
 assert(!c.step(0x8004ef54,e,true,vm,{},result) && c.releaseForRetirement());
 e.arguments=0x30000; // E18 is read when admitted, not at prebinding suspension.
 assert(!c.step(0x8004ef54,e,false,vm,w.services(),result) && !c.releaseForRetirement());
 assert(c.pending()==0x8004829c);e.arguments=0x20000; // Captured A remains30000.
 for(unsigned n=0;n<30 && result==-99;++n)c.step(0x8004ef54,e,false,vm,w.services(),result);
 assert(result==0 && w.stores==1 && c.releaseForRetirement());
 auto lookup=std::find_if(w.records.begin(),w.records.end(),[](const BattleSequenceCall& q){return q.function==0x8004f5d8;});
 assert(lookup!=w.records.end() && lookup->args[0]==0x10000 && lookup->args[1]==0x2222);
 // Actual caller's local output, not a similarly addressed external VM/record.
 e.arguments=0x20000;assert(vm.memory.writeWord(0x20004,uint32_t(-30000000)));
 NativeFieldNpcBattleScriptCall query;w.hold=false;result=-99;
 assert(query.step(0x8004f894,e,true,vm,w.services(),result) && result==1 && e.locals[0]==0x6789);
}
void original(const char* root){
 assert(loadHarborEncounterAssets(root));HarborEncounter e;assert(e.begin(0,{0,15,-100},2,false));
 unsigned tick=1;for(;tick<3000 && !e.battleRequested;++tick)e.tick(tick*17,tick%4==0?0x100:0,15);
 assert(!e.failed && e.pendingService==0x8004ef54 && e.story==4);
 for(unsigned n=0;n<3;++n)e.tick((tick++)*17,0,15); // Missing binding must retain original first=true.
 World w;e.battleServices=w.services();
 for(unsigned n=0;n<100 && std::find(w.calls.begin(),w.calls.end(),0x8002e210)==w.calls.end();++n){w.hold=true;e.tick((tick++)*17,0,15);}
 assert(!e.failed && w.stores==1 && e.story==4 && w.sequence==4);
 assert(std::count(w.calls.begin(),w.calls.end(),0x8005c3cc)==1);
 // The real original start advances to its distinct wait callback. No source
 // outcome/field return is fabricated; this fixture keeps sequence4/result0.
 for(unsigned n=0;n<10;++n)e.tick((tick++)*17,0,15);
 assert(!e.failed && !e.finished && e.story==4 && e.messageOrder.size()==3);
 assert(std::count(w.calls.begin(),w.calls.end(),0x8002e210)>0);
}
}
int main(int argc,char** argv){parents();registration();caller();if(argc>1)original(argv[1]);std::puts("retained source battle start/wait/registration and original harbor barrier passed");}
