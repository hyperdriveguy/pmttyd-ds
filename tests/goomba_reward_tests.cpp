#include "goomba_reward.hpp"
#include "prologue_pouch.hpp"
#include <cassert>
#include <vector>
using namespace ttyd;
struct Harness {
 ProloguePouch pouch=initializeProloguePouch("Yoshi");
 FieldActorTalkState* state=nullptr;
 bool ready=true,available=true,ack=false,flag=false;
 std::vector<bool> firstCalls;
};
GoombaRewardServices services(Harness& h) {
 return {&h,[](void* p){return static_cast<Harness*>(p)->ready;},
  [](void* p,int32_t amount,bool first)->std::optional<int32_t>{
   auto& h=*static_cast<Harness*>(p);assert(amount==20 && h.state->quest1420==2 && !h.flag);
   if(!h.available)return {};
   h.firstCalls.push_back(first);
   if(!h.ack)return 0;
   h.pouch.addCoins(amount);return 2;
  },[](void* p,int32_t flag){auto& h=*static_cast<Harness*>(p);assert(flag==5376 && h.state->quest1420==0);h.flag=true;}};
}
int main() {
 MessageFont font;assert(font.load("romfs/font.pmfont") && loadFieldActorCatalog("romfs"));
 const FieldActorRecord* actor=nullptr;
 for(const auto& a:fieldActorCatalog().actors())if(a.room==0 && a.key==0x42ff5f1f)actor=&a;
 assert(actor);
 FieldActorTalkState state;assert(!goombaQuest2RewardAdmitted(*actor,state));
 state.quest1420=2;assert(!goombaQuest2RewardAdmitted(*actor,state));
 state.flag5406=false;assert(!goombaQuest2RewardAdmitted(*actor,state));
 state.flag5406=true;assert(goombaQuest2RewardAdmitted(*actor,state)); //5405 is not read in this source arm.
 auto other=*actor;other.room=2;assert(!goombaQuest2RewardAdmitted(other,state));
 MessageAssets original;assert(original.loadGlobal("romfs/messages/global.txt") && original.loadRoom("romfs/messages/gor_01.txt"));
 FieldActorTalk talk;GoombaReward event;Harness h;h.state=&state;h.pouch.half(0x70,3);h.pouch.half(0x74,1);
 auto s=services(h);h.ready=false;assert(!event.begin(*actor,state,talk,font,s,0));
 h.ready=true;s.coin=nullptr;assert(!event.begin(*actor,state,talk,font,s,0));
 assert(!talk.message.active() && h.pouch.half(0x78)==100 && !h.flag);
 assert(event.begin(*actor,state,talk,font,services(h),0));
 assert(talk.message.id()=="irai_02_02" && talk.message.rawText()==original.find("irai_02_02") && talk.actor.key==actor->key);
 uint64_t tick=0;event.eventMain(0);assert(h.firstCalls.empty());
 GoombaReward snapshot=event;assert(!snapshot.canAdvance());snapshot.eventMain(0);assert(h.firstCalls.empty());
 while(talk.message.active() && tick<3000)talk.message.tick(++tick*1000/60,0x100);
 assert(!talk.message.active());h.available=false;event.eventMain(tick*1000/60);
 assert(h.firstCalls.empty() && state.quest1420==2 && !h.flag);
 h.available=true;event.eventMain(tick*1000/60);event.eventMain(tick*1000/60);
 assert((h.firstCalls==std::vector<bool>{true,false}) && h.pouch.half(0x78)==100 && state.quest1420==2);
 snapshot=event;snapshot.eventMain(0);assert(h.firstCalls.size()==2);
 h.ack=true;event.eventMain(tick*1000/60);
 assert(state.quest1420==0 && h.flag && h.pouch.half(0x78)==120);
 assert(talk.message.id()=="sys_request_finish" && talk.message.rawText()==original.find("sys_request_finish") && talk.actor.key==0);
 assert(event.active() && !event.complete() && state.flag5406==true && !state.flag5405);
 while(talk.message.active() && tick<6000){talk.message.tick(++tick*1000/60,0x100);event.eventMain(tick*1000/60);}
 assert(event.complete() && !event.active());
 event.eventMain(tick*1000/60);assert(h.pouch.half(0x78)==120 && h.firstCalls.size()==3);
 assert(h.pouch.half(0x70)==3 && h.pouch.half(0x74)==1);
 // Source truncation precedes clamp, including malformed/large input cases.
 for(const auto pair:std::vector<std::pair<int32_t,int32_t>>{{980,999},{32760,0},{-30,0},{-10,10}}) {
  h.pouch.half(0x78,int16_t(pair.first));assert(h.pouch.addCoins(20)==pair.second);
 }
 h.pouch.half(0x78,10);assert(h.pouch.addCoins(65536)==10);
 h.pouch.half(0x78,10);assert(h.pouch.addCoins(INT32_MAX)==9);
}
