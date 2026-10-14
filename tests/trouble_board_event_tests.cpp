#include "trouble_board_event.hpp"
#include "trouble_quest_state.hpp"
#include "message_assets.hpp"
#include <cassert>
#include <algorithm>
#include <type_traits>
#include <vector>
using namespace ttyd;
static_assert(!std::is_copy_constructible<TroubleBoardEvent>::value,"One event runtime owner");
struct Backend {
 MessageAssets resources;
 Backend(){assert(resources.loadGlobal("romfs/messages/global.txt") && resources.loadRoom("romfs/messages/gor_02.txt"));}
 std::vector<TroubleEventCall> calls;
 std::vector<std::string> messages,cases;
 std::vector<int32_t> choices,quests;
 unsigned selection=0,menu=0;
 int keyDepth=0,busy=0;
 bool blockNpc=false;
 std::string queuedNpc;unsigned appliedNpc=0;
 int coins=30,item95=2,item107=1;
 TroubleEventServices services() {
  return {this,[](void* p,TroubleEventRequest& r)->int32_t{
   auto& b=*static_cast<Backend*>(p);
   if(r.first) {
    b.calls.push_back(r.kind);
    if(r.kind==TroubleEventCall::Key)b.keyDepth+=r.values[0]?-1:1;
    if(r.kind==TroubleEventCall::Print || r.kind==TroubleEventCall::Append || r.kind==TroubleEventCall::Select) {assert(b.resources.find(r.text.c_str()));b.messages.push_back(r.text);}
    if(r.kind==TroubleEventCall::Select)++b.selection;
    if(r.kind==TroubleEventCall::MenuMain)++b.menu;
   }
   if(r.kind==TroubleEventCall::NpcPosition) {
    if(r.first)b.queuedNpc=r.text;
    assert(!b.queuedNpc.empty());
    if(b.blockNpc)return 0;
    ++b.appliedNpc;
   }
   if(r.kind==TroubleEventCall::RegisterCase) {
    assert(r.argumentCount==6 && r.values[0]==9 && r.values[1]==1 && r.values[3]==0 && r.values[5]==0);
    assert(r.values[4]==int32_t(b.cases.empty()?0x90546018:0x90546210));
    assert(std::all_of(r.locals.begin(),r.locals.end(),[](int32_t v){return v==0;}));
    b.cases.push_back(r.text);
   }
   if(r.kind==TroubleEventCall::CoinRead)r.result=b.coins;
   if(r.kind==TroubleEventCall::CoinAdd)b.coins+=r.values[0];
   if(r.kind==TroubleEventCall::Cancel) {
    struct Context {Backend* backend;const TroubleQuestServices* switches;} context{&b,&r.switches};
    TroubleQuestServices s;s.user=&context;
    s.clearFlag=[](void* p,int32_t f){auto& c=*static_cast<Context*>(p);c.switches->clearFlag(c.switches->user,f);};
    s.setByte=[](void* p,int32_t byte,uint8_t v){auto& c=*static_cast<Context*>(p);c.switches->setByte(c.switches->user,byte,v);};
    s.countItem=[](void* p,int32_t item){auto& b=*static_cast<Context*>(p)->backend;return item==95?b.item95:b.item107;};
    s.removeItem=[](void* p,int32_t item){auto& b=*static_cast<Context*>(p)->backend;--(item==95?b.item95:b.item107);return 1;};
    assert(r.cancellationItems && r.cancellationItemsAddress);
    return cancelTroubleQuestItems(*r.cancellationItems,s).value_or(0);
   }
   if(r.kind==TroubleEventCall::MenuInit){assert(r.argumentCount==0);r.result=0x70001000;}
   if(r.kind==TroubleEventCall::MenuMain){assert(r.work==0x70001000);r.result=b.quests.empty()?-1:b.quests[std::min(size_t(b.menu-1),b.quests.size()-1)];}
   if(r.kind==TroubleEventCall::Select)r.result=b.choices.empty()?0:b.choices[std::min(size_t(b.selection-1),b.choices.size()-1)];
   if(r.kind==TroubleEventCall::MenuEnd)assert(r.work==0x70001000);
   if(r.kind==TroubleEventCall::Print && r.first){b.busy=1;return 0;}
   if(r.kind==TroubleEventCall::Print && b.busy-- >0)return 0;
   return 2;
  }};
 }
};
FieldActorTalkState initial(bool lecture=false) {
 FieldActorTalkState state;initializeTroubleQuestState(state);setTroubleQuestFlag(state,5374,lecture);return state;
}
void run(TroubleBoardEvent& event,Backend& backend,uint64_t start=0) {
 for(unsigned n=0;n<500 && event.active();++n)event.tick(start+n*1000/60,backend.services());
 assert(!event.failed && event.finished && backend.keyDepth==0);
}
int main() {
 assert(loadTroubleBoardEventAssets("romfs"));
 TroubleBoardEvent installer;Backend registration;assert(installer.begin(0,23,initial(),TroubleBoardRoot::Installer));run(installer,registration);
 assert((registration.cases==std::vector<std::string>{"A_i04_keizi","A_i04_kunter"}));
 TroubleBoardEvent unknown;FieldActorTalkState legacy;
 assert(!unknown.begin(0,23,legacy));
 auto missing=initial();missing.trouble.known&=~uint64_t(1);assert(!unknown.begin(0,23,missing));
 TroubleBoardEvent no;Backend refused;refused.choices={1};assert(no.begin(0,23,initial()));run(no,refused);
 assert(!*troubleQuestFlag(no.switches,5374) && *no.switches.quest1420==0);
 assert((refused.messages==std::vector<std::string>{"irai_00","irai_01","irai_02"}));assert(refused.menu==0);
 TroubleBoardEvent yes;Backend lecture;assert(yes.begin(0,23,initial()));run(yes,lecture);
 assert(*troubleQuestFlag(yes.switches,5374));assert(lecture.menu==0);
 assert((lecture.messages==std::vector<std::string>{"irai_00","irai_01","irai_03"}));
 for(int quest:{2,13,30}) {
  auto state=initial(true);for(int f=5405;f<=5421;++f)setTroubleQuestFlag(state,f,true);
  for(int b=1469;b<=1474;++b)setTroubleQuestByte(state,b,9);
  TroubleBoardEvent accepted;Backend b;b.quests={quest};assert(accepted.begin(0,401,state));run(accepted,b);
  assert(*accepted.switches.quest1420==quest && *troubleQuestFlag(accepted.switches,5374));
  assert(accepted.diagnosticCount==1);
  for(int f=5405;f<=5421;++f)assert(!*troubleQuestFlag(accepted.switches,f));
  for(int byte=1469;byte<=1474;++byte)assert(*troubleQuestByte(accepted.switches,byte)==0);
  const std::string suffix=quest<10?"0"+std::to_string(quest):std::to_string(quest);
  assert((b.messages==std::vector<std::string>{"irai_help_"+suffix,"irai_"+std::to_string(quest+42),"irai_01","irai_73"}));
 }
 TroubleBoardEvent declined;Backend b;b.quests={2,-1};b.choices={1};assert(declined.begin(0,23,initial(true)));run(declined,b);
 assert(*declined.switches.quest1420==0 && b.menu==2);
 assert(std::count(b.calls.begin(),b.calls.end(),TroubleEventCall::MenuInit)==1);
 assert(std::count(b.calls.begin(),b.calls.end(),TroubleEventCall::Continue)==1);
 TroubleBoardEvent pending;Backend npc;npc.quests={1};npc.blockNpc=true;assert(pending.begin(0,23,initial(true)));
 for(int n=0;n<20;++n)pending.tick(n*17,npc.services());
 assert(pending.active() && !pending.rootAlive && !pending.failed && *pending.switches.quest1420==1 && pending.pendingFunction);
 npc.blockNpc=false;run(pending,npc,400);assert(npc.appliedNpc==1);
 TroubleBoardEvent counter;Backend empty;assert(counter.begin(0,23,initial(true),TroubleBoardRoot::Counter));run(counter,empty);
 assert((empty.messages==std::vector<std::string>{"irai_06"}));
 auto complete=initial(true);for(int f=5375;f<=5404;++f)setTroubleQuestFlag(complete,f,true);
 TroubleBoardEvent all;Backend full;assert(all.begin(0,401,complete,TroubleBoardRoot::Counter));run(all,full);
 assert((full.messages==std::vector<std::string>{"irai_74"}));
 auto assigned=initial(true);assigned.quest1420=2;
 for(int f=5405;f<=5421;++f)setTroubleQuestFlag(assigned,f,true);
 TroubleBoardEvent cancelled;Backend cancel;cancel.choices={1,0};assert(cancelled.begin(0,23,assigned,TroubleBoardRoot::Counter));run(cancelled,cancel);
 assert(*cancelled.switches.quest1420==0 && cancel.coins==0 && cancel.item95==0);
 for(int f=5405;f<=5421;++f)assert(!*troubleQuestFlag(cancelled.switches,f));
 TroubleBoardEvent poor;Backend low;low.coins=29;low.choices={1,0};assert(poor.begin(0,23,assigned,TroubleBoardRoot::Counter));run(poor,low);
 assert(*poor.switches.quest1420==2 && low.coins==29 && low.item95==2 && *troubleQuestFlag(poor.switches,5405));
 assert(low.messages.back()=="irai_11");
 TroubleBoardEvent unbound;assert(unbound.begin(0,23,initial()));unbound.tick(0,{});
 assert(unbound.active() && !unbound.failed && unbound.waitingForProvider && !*troubleQuestFlag(unbound.switches,5374));
 unbound.tick(17,{});Backend later;run(unbound,later,34);
 assert(!unbound.waitingForProvider && *troubleQuestFlag(unbound.switches,5374));
 assert((later.messages==std::vector<std::string>{"irai_00","irai_01","irai_03"}));
 // Retained nonempty children cannot exhaust the finite entry pool through
 // source-case root reentry. Admission stops before constructor assertions.
 TroubleBoardEvent pool;Backend held;held.quests={1};held.blockNpc=true;
 assert(pool.begin(0,23,initial(true)));
 unsigned admitted=1;
 for(unsigned visit=0;visit<10000 && admitted<300;++visit) {
  pool.tick(visit*17,held.services());assert(!pool.failed);
  if(!pool.rootAlive){if(!pool.appendRoot(TroubleBoardRoot::Board))break;++admitted;}
 }
 assert(admitted>100 && admitted<300 && pool.active() && !pool.rootAlive);
 assert(!pool.appendRoot(TroubleBoardRoot::Board));

}
