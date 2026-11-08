#ifndef HARBOR_FINISHING_IMPLEMENTATION
#define HARBOR_FINISHING_IMPLEMENTATION "../source/battle/harbor_tutorial.cpp"
#endif
#include HARBOR_FINISHING_IMPLEMENTATION
#include <cassert>
#include <cstdio>
namespace ttyd {
struct NativeHarborFinishingBindingFixture {
 static void test(){
  assert(loadHarborTutorialAssets("romfs")&&loadProloguePouchAssets("romfs"));HarborTutorial t;
  const auto pouch=initializeProloguePouch("Yoshi");assert(t.beginFromPouch(0,pouch,4));assert(!t.nativeRewardBattleAddress());
  auto&r=*t.runtime_;assert(t.bindPublicSound(std::make_shared<PositionalSfxRegistry>()));
  // TEST-only conclusion/old-root deletion admits the ending domain.
  assert(r.vm.memory.writeByte(r.packed->unitAddress(2)+0x135,1));
  for(int i=0;i<r.vm.battle.capacity;++i)if(r.vm.battle.entries[i].flags&1)deleteEvent(r.vm.battle.entries[i],r.vm.creationServices());
  t.phaseActive=t.attackActive=t.damageActive=false;t.initialized=t.introComplete=true;t.pendingService=0;t.message=MessagePlayback{};r.openingDispatched=true;
  assert(r.vm.memory.writeWord(r.packed->battleAddress()+0xf00,9));auto counter=t.beginNativeRewardCounter();assert(counter&&counter->admit());
  assert(t.startNativeRewardVictoryEvent()&&counter->start());
  for(uint64_t tick=1;tick<=141;++tick){t.tick(tick*1000/60,0);assert(counter->advance(tick,0));}
  assert(counter->phase()==NativeHarborRewardCounter::Phase::Done&&t.nativeRewardVictoryEventComplete());
  const auto battle=t.nativeRewardBattleAddress();assert(battle&&*battle==r.packed->battleAddress());
  NativeHarborFinishingRequest q;q.function=0x800f87a8;q.count=1;q.arguments[0]=*battle;
  const auto mario=t.nativeRewardFinishingChild(q);assert(mario&&*mario==r.packed->unitAddress(1));
  auto copy=t;assert(!copy.nativeRewardBattleAddress()&&!copy.nativeRewardFinishingChild(q));
  q.arguments[0]^=4;assert(!t.nativeRewardFinishingChild(q));q.arguments[0]=*battle;q.function=0x800f8728;assert(t.nativeRewardFinishingChild(q)==0);
  uint8_t actual=0;assert(r.vm.memory.readByte(*mario+0x138,actual)&&t.nativeRewardReadByte(*mario+0x138)==actual);
  q.function=0x801275f4;q.count=2;q.arguments={*mario,0x80427298,0,0};assert(t.nativeRewardFinishingChild(q)&&t.actors[1].pose=="M_V_1");
  const auto epoch=t.actors[1].poseAt;t.now+=16;assert(t.nativeRewardFinishingChild(q)&&t.actors[1].poseAt==epoch);
  q.function=0x80127640;q.arguments[1]=42;assert(t.nativeRewardFinishingChild(q)&&t.actors[1].pose=="M_R_1");
  q.arguments[1]=43;assert(!t.nativeRewardFinishingChild(q)&&t.actors[1].pose=="M_R_1");
  q.arguments={*mario,0x802feafc,0xf1194d80,0};q.function=0x8012619c;q.count=4;assert(!t.nativeRewardFinishingChild(q));
  SourceBattleVoiceCursor voice;q.voice=&voice;const auto sound=t.nativeRewardFinishingChild(q);assert(sound&&t.nativeRewardFinishingChild(q)==sound);
  t=HarborTutorial{};assert(!copy.nativeRewardBattleAddress()&&!copy.nativeRewardReadByte(*mario+0x138)&&!copy.nativeRewardFinishingChild(q));
 }
};
}
int main(){ttyd::NativeHarborFinishingBindingFixture::test();std::puts("Actual finishing registry/body/type42/source-voice binding, force0 epoch, counter/root gates and copy/stale rejection pass; TEST ending admission only");}
