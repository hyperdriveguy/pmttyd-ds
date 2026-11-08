#include "native_harbor_gameplay.hpp"
#include "native_harbor_action_prize.hpp"
#include "event_native_runtime.hpp"
#include "harbor_tutorial_animation_selection.hpp"
#include "native_battle_sound.hpp"
#include "prologue_pouch.hpp"
#include <cassert>
#include <cstdio>
#include <type_traits>
static_assert(std::is_copy_constructible<ttyd::NativeHarborGameplay>::value);
static_assert(std::is_move_assignable<ttyd::NativeHarborGameplay>::value);
using namespace ttyd;
namespace {
struct Fixture {
 HarborTutorial scene;NativeHarborGameplay driver;
 std::array<ActorAnimationPack,3> animation;ActorAnimationPack mario;
 std::shared_ptr<PositionalSfxRegistry> sounds=std::make_shared<PositionalSfxRegistry>();
 uint64_t tick=0;bool timedInput=false,timingPress=false;
 Fixture(){
  const char* names[]{"crump","mario","goombella"};
  for(unsigned i=0;i<3;++i)assert(animation[i].load((std::string("romfs/harbor-tutorial-")+names[i]+".pmanim").c_str()));
  assert(mario.load("romfs/trio-attack-mario.pmanim"));
  auto pouch=initializeProloguePouch("Yoshi");pouch.half(0x70,7);
  assert(scene.beginFromPouch(0,pouch,4)&&scene.bindPublicSound(sounds));
 }
 void visit(bool presented=true){
  ++tick;BattlePadSample physical;
  const bool menu=scene.classMenuOpen||scene.weaponMenuOpen||scene.weaponTargetMenuOpen;
  // TEST-only physical input sequence. No action-command success is inferred.
  if(tick%8==0&&(menu||scene.message.active()))physical.snapshot.buttons=physical.pressed=BattleButtonA;
  constexpr uint32_t ManualBattle=0x84000000,TimingReadyOffset=0x1f50;
  uint32_t timing=0;
  if(timedInput&&!timingPress&&scene.readPackedWord(ManualBattle+TimingReadyOffset,timing)&&(timing&1)){
   physical.snapshot.buttons=physical.pressed=BattleButtonA;timingPress=true;
  }
  assert(driver.advance(scene,tick,tick*1000/60,physical.pressed,&physical));
  assert(!scene.failed);
  for(unsigned i=0;i<3;++i){const auto&a=scene.actors[i];const auto selected=selectHarborTutorialAnimation(i,a.pose.c_str(),animation[i],&mario);if(!selected)continue;ActorAnimationSample sample;
   const auto*p=selected->pose;const auto time=float(scene.now-a.poseAt)*.06f;
   // Actual imported original clip evaluation/endpoint, not a timed arbitrary ack.
   if(p&&p->end>p->start&&p->evaluate(time,sample)&&time>=p->end)scene.animationComplete(a.id);
  }
  // Explicit TEST presentation receipt only; native rendering has its own owner.
  if(presented){if(scene.classMenuOpen)scene.classMenuPresented();if(scene.weaponMenuOpen)scene.weaponMenuPresented();}
 }
};
void readiness(){Fixture f;for(unsigned i=0;i<3000&&f.driver.stage()!=NativeHarborGameplay::Stage::Command;++i)f.visit(false);
 assert(f.scene.introComplete&&f.driver.stage()==NativeHarborGameplay::Stage::Command&&f.scene.classMenuOpen);
 const auto hp=f.scene.actors[0].hp;for(unsigned i=0;i<200;++i)f.visit(false);
 assert(f.scene.classMenuOpen&&!f.scene.weaponMenuOpen&&!f.scene.attackActive&&f.scene.actors[0].hp==hp);
 const auto now=f.scene.now;assert(!f.driver.advance(f.scene,f.tick,now,0,nullptr)&&f.scene.now==now);
 HarborTutorial replacement;assert(replacement.beginFresh(now,7,10,4));assert(!f.driver.advance(replacement,f.tick+1,now+17,0,nullptr));
 assert(f.driver.reset()&&!f.driver.conclusion()&&f.driver.stage()==NativeHarborGameplay::Stage::Opening);
 assert(f.driver.advance(replacement,f.tick+1,now+17,0,nullptr));
}
void actualRound(){Fixture f;bool player=false,opponent=false;for(unsigned i=0;i<7000&&f.scene.turn<2;++i){f.visit();player=player||(f.scene.attackActive&&f.driver.stage()==NativeHarborGameplay::Stage::PlayerAttack);opponent=opponent||(f.scene.attackActive&&f.driver.stage()==NativeHarborGameplay::Stage::OpponentAttackWait);
  if(f.scene.pendingService&&f.scene.pendingService!=0x80106980&&f.scene.pendingService!=0x80107708&&f.scene.pendingService!=0x80107694){std::printf("Actual driver pending %08x at stage%d\n",f.scene.pendingService,int(f.driver.stage()));}
 }
 std::printf("round%d stage%d pending%08x command%08x HP%d/%d messages%zu\n",f.scene.turn,int(f.driver.stage()),f.scene.pendingService,f.scene.commandInputPending,f.scene.actors[0].hp,f.scene.actors[1].hp,f.scene.messageOrder.size());
 assert(player&&opponent&&f.scene.turn==2&&f.scene.actors[0].hp==4&&f.scene.actors[1].hp<=7);
 assert(f.scene.messageOrder.front()=="mac_0_b_t1_000"&&!f.driver.conclusion()&&!f.scene.result);
}
}
void prizeMemory(){
 EventNativeRuntime vm(0x81000000,{});constexpr uint32_t Work=0x89000000,Unit=0x89100000,Args=0x89200000,Global=0x8041eb90;
 assert(vm.memory.registerRam(Global,std::vector<uint8_t>(4))&&vm.memory.registerRam(Work,std::vector<uint8_t>(0x100))&&vm.memory.registerRam(Unit,std::vector<uint8_t>(0x300))&&vm.memory.registerRam(Args,std::vector<uint8_t>(12)));
 assert(vm.memory.writeWord(Global,Work)&&vm.memory.writeWord(Work+0x24,Unit)&&vm.memory.writeWord(Unit+0x278,30));
 assert(vm.memory.writeWord(Args,1)&&vm.memory.writeWord(Args+4,0)&&vm.memory.writeWord(Args+8,uint32_t(-30000000)));
 EventEntry event;assert(vm.entryAddresses.registerEntries(0x89300000,&event,1));event.argumentCount=3;event.arguments=Args;event.locals[0]=91;
 assert(nativeHarborActionPrize(vm,event)==2&&event.locals[0]==-1); // Actual early30 branch needs no unknown grade byte.
 assert(vm.memory.writeWord(Unit+0x278,0));event.locals[0]=91;
 assert(!nativeHarborActionPrize(vm,event)&&event.locals[0]==91); // Missing byte must hold, never default0/clamp.
 assert(vm.entryAddresses.unregisterEntries(0x89300000));
 assert(!nativeHarborActionPrize(vm,event));
}
void successfulPrizeFrontier(){Fixture f;f.timedInput=true;
 for(unsigned i=0;i<7000&&f.scene.pendingService!=0x80106fa0;++i)f.visit();
 std::printf("success round%d stage%d pending%08x command%08x HP%d/%d declarations%d\n",f.scene.turn,int(f.driver.stage()),f.scene.pendingService,f.scene.commandInputPending,f.scene.actors[0].hp,f.scene.actors[1].hp,f.scene.successDeclarations);
 assert(f.timingPress&&!f.scene.successDeclarations&&f.scene.turn==1&&f.scene.actors[0].hp==4&&f.scene.pendingService==0x80106fa0&&f.scene.attackActive&&!f.scene.result);
}
int main(){assert(loadHarborTutorialAssets("romfs"));prizeMemory();readiness();actualRound();successfulPrizeFrontier();}
