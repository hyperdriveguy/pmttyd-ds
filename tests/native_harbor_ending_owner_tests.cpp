#ifndef HARBOR_ENDING_IMPLEMENTATION
#define HARBOR_ENDING_IMPLEMENTATION "../source/battle/harbor_tutorial.cpp"
#endif
#include HARBOR_ENDING_IMPLEMENTATION
#include "native_harbor_ending_owner.hpp"
#include "platform.hpp"
#include <cassert>
#include <cstdio>
namespace ttyd {
// TEST-only conclusion and previous-root retirement establishes ending domain.
// Actual counter/source victory event/pouch/model below remain unmodified.
struct NativeHarborRewardBindingFixture {
 static void test(){
  assert(loadHarborTutorialAssets("romfs")&&loadProloguePouchAssets("romfs"));Game game(false);Renderer renderer;Audio audio;NativeHarborEndingOwner owner;
  assert(owner.advance(game,renderer,audio)&&audio.battles==0);
  auto pouch=initializeProloguePouch("Yoshi");assert(game.harborTutorial.beginFromPouch(0,pouch,4));
  auto&r=*game.harborTutorial.runtime_;assert(game.harborTutorial.bindPublicSound(std::make_shared<PositionalSfxRegistry>()));
  assert(owner.advance(game,renderer,audio)&&audio.battles==1&&audio.victories==0);
  assert(r.vm.memory.writeByte(r.packed->unitAddress(2)+0x135,1));
  for(int i=0;i<r.vm.battle.capacity;++i)if(r.vm.battle.entries[i].flags&1)deleteEvent(r.vm.battle.entries[i],r.vm.creationServices());
  auto&t=game.harborTutorial;t.phaseActive=t.attackActive=t.damageActive=false;t.initialized=t.introComplete=true;t.pendingService=0;t.message=MessagePlayback{};r.openingDispatched=true;
  assert(r.vm.memory.writeWord(r.packed->battleAddress()+0xf00,9));
  assert(!owner.advance(game,renderer,audio)&&audio.victories==0); // Actual GPU model unavailable.
  renderer.runtime=t.nativeGameplayLifetime().lock();
  NativeHarborRewardPresentationSource s;s.user=&t;s.lifetime=std::make_shared<int>(1);const auto proof=t.nativeGameplayFacadeProof();
  s.current=[](void*p){return static_cast<HarborTutorial*>(p)->nativeGameplayFacadeProof().current();};
  s.word=[](void*p,uint32_t a)->std::optional<uint32_t>{uint32_t v;if(!static_cast<HarborTutorial*>(p)->readPackedWord(a,v))return {};return v;};
  renderer.model=std::make_shared<NativeHarborRewardPresentation>(s);renderer.available=true;
  game.ticks=1;assert(!owner.advance(game,renderer,audio));assert(audio.victories==0);
  ++game.ticks;assert(owner.advance(game,renderer,audio)&&audio.victories==1);
  bool sawVictory=false;for(;game.ticks<360;++game.ticks){
   t.tick(game.ticks*1000/60,0);assert(owner.advance(game,renderer,audio));sawVictory|=t.actors[1].pose=="M_V_2";
   assert(owner.updateAudio(game,audio));
  }
  uint32_t actualPouch=0;uint16_t exp=0;assert(r.vm.memory.readWord(0x8041eb00,actualPouch)&&r.vm.memory.readHalf(actualPouch+0x96,exp)&&exp==9);
  assert(sawVictory&&audio.battles==1&&audio.victories==1&&t.nativeRewardVictoryEventComplete());
  assert(!renderer.model->iconsActive()&&renderer.model->pointGetFinished()&&t.result==0);
  t=HarborTutorial{};assert(!proof.current());assert(owner.advance(game,renderer,audio)&&audio.retired==1);assert(!owner.updateAudio(game,audio));
 }
};
}
int main(){ttyd::NativeHarborRewardBindingFixture::test();std::puts("Production native ending composition actual counter/victory root/9EXP, GPU/music holds, PointGet and stale-facade retirement pass; backend admissions TEST-only");}
