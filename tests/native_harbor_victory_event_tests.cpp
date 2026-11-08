// Actual runtime/source-event regression. TEST conclusion and preexisting-root
// deletion below establishes the reward boundary; no gameplay claim is made.
#ifndef HARBOR_VICTORY_IMPLEMENTATION
#define HARBOR_VICTORY_IMPLEMENTATION "../source/battle/harbor_tutorial.cpp"
#endif
#include HARBOR_VICTORY_IMPLEMENTATION
#include <cassert>
#include <cstdio>
namespace ttyd {
struct NativeHarborVictoryEventFixture {
 static void test(){
  assert(loadHarborTutorialAssets("romfs")&&loadProloguePouchAssets("romfs"));HarborTutorial t;
  auto pouch=initializeProloguePouch("Yoshi");assert(t.beginFromPouch(0,pouch,4));assert(!t.startNativeRewardVictoryEvent());
  auto r=t.runtime_;assert(t.bindPublicSound(std::make_shared<PositionalSfxRegistry>()));
  assert(r->vm.memory.writeByte(r->packed->unitAddress(2)+0x135,1));
  for(int i=0;i<r->vm.battle.capacity;++i)if(r->vm.battle.entries[i].flags&1)deleteEvent(r->vm.battle.entries[i],r->vm.creationServices());
  t.phaseActive=t.attackActive=t.damageActive=false;t.initialized=t.introComplete=true;t.pendingService=0;t.message=MessagePlayback{};r->openingDispatched=true;
  assert(r->vm.memory.writeWord(r->packed->battleAddress()+0xf00,9));
  auto counter=t.beginNativeRewardCounter();assert(counter&&counter->admit());
  auto copy=t;assert(!copy.startNativeRewardVictoryEvent()&&!copy.nativeRewardVictoryEventComplete());
  assert(t.startNativeRewardVictoryEvent());const auto root=r->nativeVictoryRoot;assert(root&&r->exists(root));
  uint32_t tracked=0;assert(r->vm.memory.readWord(r->nativeVictoryUnit+0x2a8,tracked)&&tracked==root);
  assert(t.startNativeRewardVictoryEvent()&&r->nativeVictoryRoot==root&&!t.nativeRewardVictoryEventComplete());
  unsigned owned=0;for(int i=0;i<r->vm.battle.capacity;++i)owned+=(r->vm.battle.entries[i].flags&1)&&r->vm.battle.entries[i].id==root;assert(owned==1);
  bool sawPose=false;for(uint64_t tick=1;tick<=90&&!t.nativeRewardVictoryEventComplete();++tick){t.tick(tick*1000/60,0);sawPose|=t.actors[1].pose=="M_V_2";assert(!t.failed);}
  assert(sawPose&&t.nativeRewardVictoryEventComplete());assert(t.now>=1000);
  t=HarborTutorial{};assert(!copy.nativeRewardVictoryEventComplete()&&!copy.startNativeRewardVictoryEvent());
 }
};
}
int main(){ttyd::NativeHarborVictoryEventFixture::test();std::puts("Actual source Mario victory root, owner/trackedID, original pose/timer retirement and facade guard pass");}
