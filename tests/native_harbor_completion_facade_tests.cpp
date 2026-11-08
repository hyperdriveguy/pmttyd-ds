// Exact private production implementation, for actual Runtime/VM ownership
// checks only. TEST pre-conclusion deletion/status setup is not gameplay proof.
#ifndef HARBOR_COMPLETION_IMPLEMENTATION
#define HARBOR_COMPLETION_IMPLEMENTATION "../source/battle/harbor_tutorial.cpp"
#endif
#include HARBOR_COMPLETION_IMPLEMENTATION
#include "native_harbor_completion.hpp"
#include "prologue_pouch.hpp"
#include <cassert>
#include <cstdio>
namespace ttyd {
struct NativeHarborCompletionFixture {
 static void test(){
  assert(loadHarborTutorialAssets("romfs")&&loadProloguePouchAssets("romfs"));HarborTutorial t;
  ProloguePouch initial=initializeProloguePouch("Yoshi");initial.half(0x70,8);initial.half(0x74,3);
  assert(t.beginFromPouch(0,initial,4));auto r=t.runtime_;
  assert(!t.beginNativeCompletion(1)); // Original enemy group is not concluded.
  constexpr uint32_t args=0x93000000;assert(r->vm.memory.registerRam(args,std::vector<uint8_t>(16)));
  for(unsigned i=0;i<4;++i)assert(r->vm.memory.writeWord(args+4*i,i?i*10:2));
  EventEntry rotation;rotation.id=99;rotation.currentCommand=args;rotation.arguments=args;rotation.argumentCount=4;
  const auto math=r->vm.platform.fmodDouble;r->vm.platform.fmodDouble=nullptr;
  assert(r->call(rotation,0x80111720,true)==0&&t.pendingService==0x80111720);
  assert((t.actors[0].rotation==std::array<float,3>{0,0,0}));
  r->vm.platform.fmodDouble=math;assert(r->call(rotation,0x80111720,false)==2&&!t.pendingService);
  assert((t.actors[0].rotation==std::array<float,3>{10,20,30}));
  assert(r->call(rotation,0x801115f8,true)==2);
  assert((t.actors[0].rotation==std::array<float,3>{20,40,60}));

  // TEST actual source predicate domain; independent of death/script completion.
  const auto enemy=r->packed->unitAddress(2);assert(enemy);
  assert(r->vm.memory.writeByte(enemy+0x135,1));
  t.pendingService=0x801115f8;assert(!t.beginNativeCompletion(1));
  t.pendingService=0;
  for(int i=0;i<r->vm.battle.capacity;++i)if(r->vm.battle.entries[i].flags&1)deleteEvent(r->vm.battle.entries[i],r->vm.creationServices());
  t.phaseActive=t.attackActive=t.damageActive=false;
  t.initialized=t.introComplete=true;t.message=MessagePlayback{};
  assert(t.actors[0].damage);auto& fatal=r->root(t.actors[0].damage,t.actors[0].id);
  assert(!t.beginNativeCompletion(1)); // Still holds even if U+2b4 is absent.
  deleteEvent(fatal,r->vm.creationServices());
  assert(t.nativeGameplayConclusion()==1);assert(!t.beginNativeCompletion(3));
  const auto owner=t.beginNativeCompletion(1);assert(owner&&t.beginNativeCompletion(1)==owner);
  const auto now=t.now;BattlePadSample pad;pad.snapshot.buttons=0x100;
  t.tick(100,0x100,&pad);assert(t.now==now); // No source simulation replay while retiring.
  struct Hooks{bool alive=true;unsigned visuals=0,music=0;} hooks;
  auto life=std::make_shared<int>(1);NativeHarborCompletionServices s{life,&hooks,[](void* p){return static_cast<Hooks*>(p)->alive;},[](void* p){++static_cast<Hooks*>(p)->visuals;return true;},[](void* p){++static_cast<Hooks*>(p)->music;return true;},{}};
  assert(owner->resume(s));const auto receipt=owner->receipt();assert(receipt&&receipt->result==1&&receipt->pouch.half(0x70)==8&&receipt->pouch.half(0x74)==3);
  assert(hooks.visuals==1&&hooks.music==1);
  // The retained owner pins the old VM, but public facade retirement revokes
  // permission before reading it. Replacement is not the old battle owner.
  t=HarborTutorial{};assert(!owner->resume(s)&&!owner->receipt());
  assert(hooks.visuals==1&&hooks.music==1);
 }
};
}
int main(){ttyd::NativeHarborCompletionFixture::test();std::puts("Actual Harbor Runtime factory, source predicate/held death guard, frozen simulation and facade retirement pass");return 0;}
