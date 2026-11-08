#include "native_harbor_reward_flow.hpp"
#include "event_script_assets.hpp"
#include <cassert>
#include <map>
#include <iostream>
using namespace ttyd;
namespace {
constexpr uint32_t Battle=0x84000000,Pouch=0x85000000;
struct Fixture {
 bool alive=true,holdMusic=false,holdEvent=false,eventDone=true;
 unsigned music=0,events=0,writes=0,sounds=0,calls=0,holdAt=0;
 EventScriptAssets assets;
 std::shared_ptr<int> life=std::make_shared<int>(1);
 std::map<uint32_t,uint32_t> memory{{Battle+0xf00,9},{Battle+0x18ff9,0},{Pouch+0x8a,1},{Pouch+0x96,0}};
 bool ready(){return ++calls!=holdAt;}
 NativeHarborRewardCounterServices counterSource(){NativeHarborRewardCounterServices s;
  s.user=this;s.lifetime=life;s.current=[](void*p){return static_cast<Fixture*>(p)->alive;};
  s.word=[](void*p,uint32_t a)->std::optional<uint32_t>{auto&f=*static_cast<Fixture*>(p);if(!f.ready())return {};return f.memory.at(a);};
  s.half=[](void*p,uint32_t a)->std::optional<uint16_t>{auto&f=*static_cast<Fixture*>(p);if(!f.ready())return {};return uint16_t(f.memory.at(a));};
  s.byte=[](void*p,uint32_t a)->std::optional<uint8_t>{auto&f=*static_cast<Fixture*>(p);if(!f.ready())return {};return uint8_t(f.memory.at(a));};
  s.writeHalf=[](void*p,uint32_t a,uint16_t v){auto&f=*static_cast<Fixture*>(p);if(!f.ready())return false;f.memory[a]=v;++f.writes;return true;};
  s.sound=[](void*p){auto&f=*static_cast<Fixture*>(p);if(!f.ready())return false;++f.sounds;return true;};return s;
 }
 NativeHarborRewardPresentationSource presentationSource(){NativeHarborRewardPresentationSource s;
  s.user=this;s.lifetime=life;s.current=[](void*p){return static_cast<Fixture*>(p)->alive;};
  s.word=[](void*p,uint32_t a)->std::optional<uint32_t>{auto&f=*static_cast<Fixture*>(p);uint32_t v;if(!f.ready()||!f.assets.readWord(a,v))return {};return v;};return s;
 }
 NativeHarborRewardFlowServices source(){NativeHarborRewardFlowServices s;
  s.user=this;s.lifetime=life;s.current=[](void*p){return static_cast<Fixture*>(p)->alive;};
  s.victoryMusic=[](void*p){auto&f=*static_cast<Fixture*>(p);if(f.holdMusic){f.holdMusic=false;return false;}++f.music;return true;};
  s.victoryEvent=[](void*p){auto&f=*static_cast<Fixture*>(p);if(f.holdEvent){f.holdEvent=false;return false;}++f.events;return true;};
  s.victoryEventComplete=[](void*p){return static_cast<Fixture*>(p)->eventDone;};return s;
 }
};
void run(bool fast,unsigned holdAt=0){
 Fixture f;assert(f.assets.load("romfs/harbor-tutorial-events.pmevt"));f.holdAt=holdAt;
 auto c=std::make_shared<NativeHarborRewardCounter>(Battle,Pouch,f.counterSource());
 auto p=std::make_shared<NativeHarborRewardPresentation>(f.presentationSource());
 NativeHarborRewardFlow flow(c,p,f.source());unsigned heldVisits=0;uint64_t t=0;
 while(flow.phase()!=NativeHarborRewardFlow::Phase::ReadyForFinishing){
  assert(++t<400);const bool accepted=flow.advance(t,fast?0x100:0);
  if(!accepted)++heldVisits;
  else{const auto writes=f.writes,events=f.events,music=f.music;const auto clock=p->icons()[0].animationTick;
   assert(flow.advance(t,0x300));assert(writes==f.writes&&events==f.events&&music==f.music&&clock==p->icons()[0].animationTick);}
  if(!holdAt&&t==1)assert(flow.wait()==119);
  if(!holdAt&&t==120)assert(flow.phase()==NativeHarborRewardFlow::Phase::Victory&&f.events==0);
  if(!holdAt&&t==121)assert(flow.phase()==NativeHarborRewardFlow::Phase::Counting&&f.events==1&&f.writes==0);
 }
 assert(f.music==1&&f.events==1&&f.writes==9&&f.memory.at(Pouch+0x96)==9&&f.sounds==(fast?1u:4u));
 assert(!p->iconsActive());
 if(!holdAt)assert(t==(fast?146u:267u));
 if(holdAt&&holdAt<=f.calls)assert(heldVisits==1);
 const auto clock=p->icons()[0].animationTick;
 for(unsigned i=0;i<80;++i)assert(flow.advance(++t,0));
 assert(p->pointGetFinished()&&p->icons()[0].animationTick==clock);
 const auto calls=f.calls;f.alive=false;assert(!flow.advance(++t,0)&&f.calls==calls);
}
void branches(){
 for(bool zero:{false,true}){Fixture f;assert(f.assets.load("romfs/harbor-tutorial-events.pmevt"));
  if(zero)f.memory[Pouch+0x8a]=99;else f.memory[Pouch+0x96]=95;
  auto c=std::make_shared<NativeHarborRewardCounter>(Battle,Pouch,f.counterSource());auto p=std::make_shared<NativeHarborRewardPresentation>(f.presentationSource());NativeHarborRewardFlow flow(c,p,f.source());
  if(zero){assert(!flow.advance(1,0)&&flow.phase()==NativeHarborRewardFlow::Phase::ZeroReward&&f.music==0&&f.events==0&&f.writes==0);assert(!flow.advance(2,0));}
  else{uint64_t t=0;while(flow.phase()!=NativeHarborRewardFlow::Phase::LevelUp){assert(++t<300);assert(flow.advance(t,0));}assert(f.memory[Pouch+0x96]==4&&p->iconsActive()&&f.events==1);assert(flow.phase()!=NativeHarborRewardFlow::Phase::ReadyForFinishing);}
 }
}
void serviceHolds(){
 Fixture f;assert(f.assets.load("romfs/harbor-tutorial-events.pmevt"));f.holdMusic=f.holdEvent=true;f.eventDone=false;
 auto c=std::make_shared<NativeHarborRewardCounter>(Battle,Pouch,f.counterSource());auto p=std::make_shared<NativeHarborRewardPresentation>(f.presentationSource());NativeHarborRewardFlow flow(c,p,f.source());
 assert(!flow.advance(1,0)&&flow.phase()==NativeHarborRewardFlow::Phase::Admission&&f.music==0);
 assert(flow.advance(2,0)&&flow.wait()==119&&f.music==1);
 for(uint64_t t=3;t<=121;++t)assert(flow.advance(t,0));
 assert(!flow.advance(122,0)&&f.events==0);assert(flow.advance(123,0)&&f.events==1);
 uint64_t t=123;while(flow.phase()!=NativeHarborRewardFlow::Phase::Cleanup){assert(++t<300);assert(flow.advance(t,0));}
 for(unsigned i=0;i<4;++i)assert(flow.advance(++t,0));
 assert(!flow.advance(++t,0)&&flow.wait()==0);assert(!flow.advance(++t,0)&&flow.wait()==0);
 f.eventDone=true;assert(flow.advance(++t,0)&&flow.phase()==NativeHarborRewardFlow::Phase::ReadyForFinishing&&f.events==1&&f.music==1);
}
}
int main(){run(false);run(true);for(unsigned i=1;i<=100;++i){run(false,i);run(true,i);}branches();serviceHolds();std::cout<<"Native reward flow actual counter/presentation,119 intro, source-row boundaries, physical acceleration, callback holds and stale guard pass\n";}
