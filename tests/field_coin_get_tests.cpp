#include "field_coin_get.hpp"
#include "prologue_pouch.hpp"
#include "item_get_text.hpp"
#include "goomba_reward.hpp"
#include "item_get_windows.hpp"
#include "field_camera_events.hpp"
#include <cassert>
#include <cmath>
#include <string>
#include <vector>
using namespace ttyd;
struct Harness {
 ProloguePouch pouch;
 FieldCoinClock clock{9,0xffffff00,4000}; // One raw tick/ms makes exact/wrap boundaries explicit.
 std::vector<std::string> calls;
 const int32_t* parameter=nullptr;
 unsigned positionReads=0;
 std::vector<int> readOrder;
 int32_t action=2;
 bool ready=true;
};
FieldCoinGetServices services(Harness& h) {
 FieldCoinGetServices s;s.user=&h;
 s.ready=[](void* p){return static_cast<Harness*>(p)->ready;};
 s.mario=[](void* p){auto& h=*static_cast<Harness*>(p);++h.positionReads;h.readOrder.push_back(0);return ItemGetPoint{3,20,-7};};
 s.heading=[](void* p){static_cast<Harness*>(p)->readOrder.push_back(1);return 0.f;};s.clock=[](void* p){return static_cast<Harness*>(p)->clock;};
 s.effect=[](void* p,int32_t type,ItemGetPoint xyz){assert(type==7 && xyz.y==82 && xyz.z<-16.99f);static_cast<Harness*>(p)->calls.push_back("effect7");return uintptr_t(42);};
 s.effectPayload=[](void* p,uintptr_t effect,int32_t kind){assert(effect==42 && kind==5);static_cast<Harness*>(p)->calls.push_back("payload5");};
 s.softDelete=[](void* p,uintptr_t effect){assert(effect==42);static_cast<Harness*>(p)->calls.push_back("softDelete");};
 s.iconEnter=[](void* p,const char* name,uint32_t descriptor,ItemGetPoint xyz){assert(std::string(name)=="iraiCoin" && descriptor==403 && xyz.y==72);static_cast<Harness*>(p)->calls.push_back("icon403");};
 s.iconFlags=[](void* p,const char* name,uint16_t on,uint16_t off){assert(std::string(name)=="iraiCoin");auto& h=*static_cast<Harness*>(p);
  if(on==0x102 && !off)h.calls.push_back("flags102");else {assert(!on && off==2);h.calls.push_back("clear2");}};
 s.iconDelete=[](void* p,const char* name){assert(std::string(name)=="iraiCoin");static_cast<Harness*>(p)->calls.push_back("iconDelete");};
 s.request=[](void* p,FieldCoinRequest r){auto& h=*static_cast<Harness*>(p);
  const char* names[]={"GetMotion","Off3800","Fanfare201","StayMotion","ResumeA0","Off201"};h.calls.push_back(names[int(r)]);};
 s.window=[](void* p,uint32_t definition,const int32_t* parameter){auto& h=*static_cast<Harness*>(p);assert(definition==0x80415e84);h.parameter=parameter;h.calls.push_back("window");return 4;};
 s.windowClose=[](void* p,int32_t window){assert(window==4);auto& h=*static_cast<Harness*>(p);assert(h.parameter[1]==1);h.calls.push_back("close");};
 s.windowAction=[](void* p,int32_t window){assert(window==4);auto& h=*static_cast<Harness*>(p);h.calls.push_back("action");return h.action;};
 s.addCoins=[](void* p,int32_t amount){auto& h=*static_cast<Harness*>(p);h.calls.push_back("grant");return h.pouch.addCoins(amount);};
 return s;
}
int main() {
 Harness h;h.pouch.half(0x78,100);FieldCoinGet coin;
 auto s=services(h);h.ready=false;assert(!coin.bind(s));h.ready=true;
 s.effectPayload=nullptr;assert(!coin.bind(s));assert(!coin.invoke(20,true,0));
 assert(coin.bind(services(h)) && coin.invoke(20,true,0x100)==0);
 assert((h.calls==std::vector<std::string>{"effect7","icon403","flags102","payload5","GetMotion","window","Off3800","Fanfare201"}));
 assert(coin.state()==1 && coin.retained() && h.parameter==coin.parameter().data());
 assert(h.parameter[0]==20 && !h.parameter[1] && h.pouch.half(0x78)==100);
 assert(!coin.bind(services(h)) && !coin.invoke(20,true,0));
 FieldCoinGet snapshot=coin;assert(!snapshot.canAdvance() && !snapshot.invoke(20,false,0x100));
 h.clock.high=0;h.clock.low+=200;assert(coin.invoke(20,false,0x100)==0 && coin.state()==1); // High ignored, strict low/wrap comparison.
 h.clock.low+=1;assert(coin.invoke(20,false,0x100)==0 && coin.state()==2 && h.parameter[1]==1);
 h.clock.low+=500;assert(coin.invoke(20,false,0x100)==0 && coin.state()==2);
 h.clock.low+=1;assert(coin.invoke(20,false,0x100)==0 && coin.state()==10 && h.pouch.half(0x78)==100);
 assert(coin.invoke(20,false,0x200)==0 && coin.state()==10); // B is not acknowledgment.
 assert(coin.invoke(20,false,0x100)==0 && coin.state()==100 && h.pouch.half(0x78)==100);
 h.calls.clear();assert(coin.invoke(7,false,0)==2 && coin.state()==101); // Current resolved amount, not displayed snapshot.
 assert((h.calls==std::vector<std::string>{"grant","softDelete","iconDelete","StayMotion","ResumeA0","Off201","close"}));
 assert(h.pouch.half(0x78)==107 && !h.parameter[1] && coin.retained());
 assert(coin.invoke(20,false,0)==0 && coin.retained()); // Explicit cleanup call, not automatic VM continuation.
 h.action=0;assert(coin.invoke(20,false,0)==2 && !coin.retained() && h.pouch.half(0x78)==107);
 assert(!coin.invoke(20,false,0));
 // Native unbound clock is distinct from an elapsed source wait.
 assert(coin.bind(services(h)));assert(coin.invoke(20,true,0)==0);h.clock.hardwareFrequency=0;
 assert(!coin.invoke(20,false,0) && coin.state()==1 && h.pouch.half(0x78)==107);
 // Source signed elapsed comparison rejects a quotient with its sign bit set.
 h.clock.hardwareFrequency=4000;h.clock.low+=0x80000000u;
 assert(coin.invoke(20,false,0)==0 && coin.state()==1);
 assert(h.readOrder.size()==2*h.positionReads);
 for(unsigned i=0;i<h.readOrder.size();++i)assert(h.readOrder[i]==int(i%2));
 // Placement retains original offsets, periodic angle normalization and finite NaN handling.
 const auto a=fieldCoinPlacement({0,0,0},0),b=fieldCoinPlacement({0,0,0},360);
 assert(a.effect.y==62 && a.icon.y==52 && std::fabs(a.effect.x-b.effect.x)<.001f && std::fabs(a.effect.z-b.effect.z)<.001f);
 const auto nan=fieldCoinPlacement({0,0,0},NAN);assert(std::isfinite(nan.effect.x) && std::isfinite(nan.effect.z));
 const std::array<std::array<float,3>,4> cardinal{{{{89.95f,10,0}},{{179.95f,0,10}},{{269.95f,-10,0}},{{359.95f,0,-10}}}};
 for(const auto& point:cardinal) {
  const auto p=fieldCoinPlacement({0,0,0},point[0]);
  assert(std::fabs(p.effect.x-point[1])<.02f && std::fabs(p.effect.z-point[2])<.02f);
  for(float side:{-.001f,.001f}) {
   const auto q=fieldCoinPlacement({0,0,0},point[0]+side);
   assert(std::fabs(q.effect.x-p.effect.x)<.02f && std::fabs(q.effect.z-p.effect.z)<.02f);
  }
 }
 assert(expandUsCoinGetText("You got <col c00000ff><NUM></col> coin<S>!",20)=="You got <col c00000ff>20</col> coins!");
 assert(expandUsCoinGetText("<NUM> coin<S> <NUM> <S>",1)=="1 coin <NUM> <S>");
 assert(expandUsCoinGetText("<NUM> coin<S>",0)=="0 coins");
 assert(expandUsCoinGetText("<NUM> coin<S>",-1)=="-1 coins");
 // Compose the actual source callback with the original reward caller. Story
 // completion follows return2 at100, while the borrowed window parameter lives.
 MessageFont font;assert(font.load("romfs/font.pmfont") && loadFieldActorCatalog("romfs"));
 const FieldActorRecord* actor=nullptr;
 for(const auto& a:fieldActorCatalog().actors())if(a.room==0 && a.key==0x42ff5f1f)actor=&a;
 assert(actor);
 Harness live;live.pouch.half(0x78,100);FieldCoinGet actual;assert(actual.bind(services(live)));
 FieldActorTalkState state;state.quest1420=2;state.flag5406=true;
 struct Bridge {FieldCoinGet* coin;FieldActorTalkState* state;uint32_t triggers=0;bool finished=false;} bridge{&actual,&state};
 GoombaRewardServices rewardServices{&bridge,[](void*){return true;},
  [](void* p,int32_t amount,bool first){auto& b=*static_cast<Bridge*>(p);return b.coin->invoke(amount,first,b.triggers);},
  [](void* p,int32_t flag){auto& b=*static_cast<Bridge*>(p);assert(flag==5376 && b.state->quest1420==0);b.finished=true;}};
 FieldActorTalk talk;GoombaReward reward;assert(reward.begin(*actor,state,talk,font,rewardServices,0));
 uint64_t tick=0;
 while(talk.message.active() && tick<3000)talk.message.tick(++tick*1000/60,0x100);
 reward.eventMain(tick*1000/60);assert(actual.state()==1 && state.quest1420==2);
 live.clock.low+=201;reward.eventMain(tick*1000/60);assert(actual.state()==2);
 live.clock.low+=501;bridge.triggers=0x100;reward.eventMain(tick*1000/60);assert(actual.state()==10);
 reward.eventMain(tick*1000/60);assert(actual.state()==100 && live.pouch.half(0x78)==100 && !bridge.finished);
 bridge.triggers=0;reward.eventMain(tick*1000/60);
 assert(actual.state()==101 && actual.retained() && live.pouch.half(0x78)==120 && bridge.finished && state.quest1420==0);
 assert(talk.message.id()=="sys_request_finish" && talk.actor.key==0 && state.flag5406==true);
 assert(!live.parameter[1] && live.action==2); // Closing window did not delay quest writes.
 while(talk.message.active() && tick<6000){talk.message.tick(++tick*1000/60,0x100);reward.eventMain(tick*1000/60);}
 assert(reward.complete() && actual.retained());live.action=0;
 assert(actual.invoke(20,false,0)==2 && !actual.retained() && live.pouch.half(0x78)==120);
 // Native lifetime retirement uses actual manager deletion, not a fabricated
 // action0 or another invocation of the completed source callback.
 struct WindowHarness:Harness {CoinGetWindow window;unsigned observations=0;} native;
 assert(loadItemGetWindowAssets("romfs") && native.window.prepare(font));
 auto ns=services(native);
 ns.window=[](void* p,uint32_t definition,const int32_t* parameter){
  auto& h=*static_cast<WindowHarness*>(p);h.parameter=parameter;return h.window.enter(definition,parameter);};
 ns.windowClose=[](void* p,int32_t id){static_cast<WindowHarness*>(p)->window.closeAutoDelete(id);};
 ns.windowRetained=[](void* p,int32_t id){auto& h=*static_cast<WindowHarness*>(p);assert(id==0);++h.observations;return h.window.retained();};
 FieldCoinGet owned;assert(owned.bind(ns));
 assert(!owned.retireDeletedWindow() && native.observations==0);
 assert(owned.invoke(20,true,0)==0 && !owned.retireDeletedWindow());
 int32_t rate=60;ItemSelectionSequenceServices sequence{&rate,[](void* p){return *static_cast<int32_t*>(p);},
  [](void*,int32_t mode,int32_t step,int32_t count,float a,float b){return cameraInterpolation(mode,step,count,a,b).value_or(a);}};
 for(unsigned i=0;i<20;++i)native.window.managerTick(sequence);
 native.clock.low+=201;assert(owned.invoke(20,false,0)==0);
 native.clock.low+=501;assert(owned.invoke(20,false,0)==0);
 assert(owned.invoke(20,false,0x100)==0 && owned.invoke(20,false,0)==2);
 assert(owned.retained() && !owned.retireDeletedWindow() && !owned.bind(ns));
 FieldCoinGet borrowedCopy=owned;assert(!borrowedCopy.retireDeletedWindow());
 const unsigned reads=native.positionReads;
 for(unsigned i=0;i<9;++i)native.window.managerTick(sequence);
 assert(!native.window.retained() && native.window.action(0)==1);
 assert(owned.retireDeletedWindow() && !owned.retained());
 assert(native.positionReads==reads && native.pouch.half(0x78)==20);
 assert(!owned.retireDeletedWindow() && owned.bind(ns));
 assert(owned.invoke(7,true,0)==0 && native.window.retained() && native.parameter[0]==7);
}
