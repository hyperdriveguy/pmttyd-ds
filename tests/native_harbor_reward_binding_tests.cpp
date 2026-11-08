// Actual private runtime binding. TEST conclusion/root retirement setup below
// establishes the domain; it is not a gameplay or story-completion receipt.
#ifndef HARBOR_REWARD_IMPLEMENTATION
#define HARBOR_REWARD_IMPLEMENTATION "../source/battle/harbor_tutorial.cpp"
#endif
#include HARBOR_REWARD_IMPLEMENTATION
#include "native_harbor_reward_counter.hpp"
#include <cassert>
#include <cstdio>
namespace ttyd {
struct NativeHarborRewardBindingFixture {
 static void test(){
  assert(loadHarborTutorialAssets("romfs")&&loadProloguePouchAssets("romfs"));HarborTutorial t;
  auto pouch=initializeProloguePouch("Yoshi");pouch.half(0x96,0);assert(t.beginFromPouch(0,pouch,4));
  assert(t.bindPublicSound(std::make_shared<PositionalSfxRegistry>()));
  assert(!t.beginNativeRewardCounter());auto r=t.runtime_;
  const auto enemy=r->packed->unitAddress(2);assert(enemy&&r->vm.memory.writeByte(enemy+0x135,1));
  for(int i=0;i<r->vm.battle.capacity;++i)if(r->vm.battle.entries[i].flags&1)deleteEvent(r->vm.battle.entries[i],r->vm.creationServices());
  t.phaseActive=t.attackActive=t.damageActive=false;t.initialized=t.introComplete=true;t.pendingService=0;t.message=MessagePlayback{};
  assert(r->vm.memory.writeWord(r->packed->battleAddress()+0xf00,9));
  auto copy=t;assert(!copy.beginNativeRewardCounter());
  r->scene=&copy;assert(!t.beginNativeRewardCounter()&&!copy.beginNativeRewardCounter());r->scene=&t;
  auto owner=t.beginNativeRewardCounter();assert(owner&&owner==t.beginNativeRewardCounter());assert(owner->admit()&&owner->total()==9);
  assert(owner->start());for(unsigned tick=1;tick<=4;++tick)assert(owner->advance(tick,0));
  uint32_t p=0;uint16_t exp=0;assert(r->vm.memory.readWord(0x8041eb00,p)&&r->vm.memory.readHalf(p+0x96,exp)&&exp==1);
  assert(owner->advance(4,0)&&r->vm.memory.readHalf(p+0x96,exp)&&exp==1);
  // Actual source VM and pouch are retained by owner, but facade retirement
  // revokes further reads/writes first, while the old runtime remains alive.
  t=HarborTutorial{};assert(!copy.beginNativeRewardCounter());
  assert(!owner->admit()&&!owner->start()&&!owner->advance(5,0));
  assert(r->vm.memory.readHalf(p+0x96,exp)&&exp==1);
  HarborTutorial canceled;assert(canceled.beginFromPouch(0,pouch,4));auto cr=canceled.runtime_;
  assert(canceled.bindPublicSound(std::make_shared<PositionalSfxRegistry>()));
  assert(cr->vm.memory.writeByte(cr->packed->unitAddress(2)+0x135,1));
  for(int i=0;i<cr->vm.battle.capacity;++i)if(cr->vm.battle.entries[i].flags&1)deleteEvent(cr->vm.battle.entries[i],cr->vm.creationServices());
  canceled.phaseActive=canceled.attackActive=canceled.damageActive=false;canceled.initialized=canceled.introComplete=true;canceled.pendingService=0;canceled.message=MessagePlayback{};
  auto abandoned=canceled.beginNativeRewardCounter();assert(abandoned);abandoned.reset();
  assert(!canceled.beginNativeRewardCounter()); // No re-mint/re-award after cancellation.
 }
};
}
int main(){ttyd::NativeHarborRewardBindingFixture::test();std::puts("Actual Harbor reward binding, origin/copy/current domain and cancellation latch pass");}
