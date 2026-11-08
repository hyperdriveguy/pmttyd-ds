#include <memory>
#include "harbor_tutorial_packed.hpp"
#include "event_memory_bootstrap.hpp"
#include "pouch_item_count.hpp"
#include "prologue_pouch.hpp"
#include <cassert>
#include <cmath>
#include <cstdio>
#include <iostream>
using namespace ttyd;
namespace {
constexpr uint32_t SourcePouch=0x8041eb00,LifeShroom=151,CrumpWeapon=0x95544690,JumpWeapon=0x803538a8;
// This fixture executes the actual packed damage/event adapters and original
// asset tables. Its allocator/bootstrap and explicit attack call are TEST
// admission, not proof that the normal opening scheduler reached a battle.
struct F {
 EventNativeRuntime vm{0x81000000,platform()};HarborTutorialPacked packed{vm};
 static EventNativePlatform platform(){EventNativePlatform p;p.clock=[](void*)->uint32_t{return 243000000;};p.truncate=[](void*,float v){return int32_t(v);};p.divideWord=[](void*,int32_t a,int32_t b){return a/b;};p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){};p.fmaFloat=[](void*,float a,float b,float c){return std::fma(a,b,c);};p.addFloat=[](void*,float a,float b){return a+b;};p.multiplyFloat=[](void*,float a,float b){return a*b;};return p;}
 uint32_t w(uint32_t a){uint32_t v;assert(vm.memory.readWord(a,v));return v;}void w(uint32_t a,uint32_t v){assert(vm.memory.writeWord(a,v));}uint16_t h(uint32_t a){uint16_t v;assert(vm.memory.readHalf(a,v));return v;}void h(uint32_t a,uint16_t v){assert(vm.memory.writeHalf(a,v));}
 F(){EventScriptAssets scripts;assert(scripts.load("romfs/harbor-tutorial-events.pmevt"));assert(vm.memory.importScripts(scripts));assert(vm.memory.registerRam(0x82000000,std::vector<uint8_t>(1024*1024))&&vm.memory.registerRam(0x8041f070,std::vector<uint8_t>(16))&&vm.memory.registerRam(0x8041e428,std::vector<uint8_t>(4))&&vm.memory.registerRam(0x8041e844,std::vector<uint8_t>(4)));auto m=vm.memoryBinding.services();const auto start=eventSdkInitializeAllocator(0x82000000,0x82100000,1,m);assert(eventSdkCreateHeap(start,0x82100000,m)==0);w(0x8041e844,0);w(0x81000004,60);vm.initializeContexts();vm.gp.battleContext=1;assert(packed.provision()&&packed.provisionFreshPouch("Yoshi"));
  FILE*f=std::fopen("romfs/harbor-tutorial-program.bin","rb");assert(f);char magic[8];uint32_t roots[12];assert(std::fread(magic,8,1,f)==1&&std::fread(roots,4,12,f)==12);std::fclose(f);
  HarborTutorialActor mario;mario.id=1;mario.kind=222;mario.hp=mario.maxHp=1;assert(packed.registerActor(mario));HarborTutorialActor crump;crump.id=2;crump.sourceKind=roots[6];crump.kind=int32_t(w(roots[6]));crump.hp=crump.maxHp=1;crump.belong=1;assert(packed.registerActor(crump));
 }
 uint32_t u(int id){return w(packed.battleAddress()+0x20+4*uint32_t(id));}
 bool damage(bool mario,uint32_t weapon){const auto args=packed.battleAddress()+0x19000;for(unsigned i=0;i<6;++i)w(args+4*i,std::array<uint32_t,6>{mario?1u:2u,mario?2u:1u,1,weapon,0x100,uint32_t(-30000000)}[i]);auto& e=vm.battle.entries[0];e.flags=1;e.id=1;e.arguments=args;e.argumentCount=6;e.owner=mario?1:2;int32_t result=99;return packed.invoke(e,0x8010a78c,1,result);}
 void relocate(){// Deliberately different allocation addresses, with real registry
  // and part-owner links. Old private slots remain mapped to detect bad writes.
  constexpr uint32_t NewBattle=0x85000000,NewUnits=0x85100000,NewParts=0x85200000;
  auto copy=[&](uint32_t old,uint32_t at,uint32_t n){std::vector<uint8_t>b(n);for(uint32_t i=0;i<n;++i)assert(vm.memory.readByte(old+i,b[i]));assert(vm.memory.registerRam(at,std::move(b)));};
  copy(packed.battleAddress(),NewBattle,0x19400);copy(HarborTutorialPacked::Units,NewUnits,0x4000);copy(HarborTutorialPacked::Parts,NewParts,0x4000);w(0x8041eb90,NewBattle);
  for(unsigned id=1;id<=2;++id){const auto unit=NewUnits+id*0x1000,part=NewParts+id*0x1000;w(NewBattle+0x20+4*id,unit);w(unit+0x14,part);w(part+0x4ec,unit);}
 }
};
void lethalMario(){F f;f.relocate();const auto hp=f.u(1)+0x10c;assert(f.damage(false,CrumpWeapon));assert(f.h(hp)==0&&f.h(HarborTutorialPacked::Units+0x1000+0x10c)==1);}
void lethalEnemy(){F f;f.relocate();assert(f.damage(true,JumpWeapon));assert(f.h(f.u(2)+0x10c)==0&&f.h(HarborTutorialPacked::Units+0x2000+0x10c)==1);}
void revivalHeld(){F f;f.h(f.w(SourcePouch)+0x192+38,LifeShroom);assert(!f.damage(false,CrumpWeapon)&&f.h(f.u(1)+0x10c)==1);f.h(f.w(SourcePouch)+0x192+38,0);assert(f.damage(false,CrumpWeapon)&&!f.h(f.u(1)+0x10c));F enemy;enemy.w(enemy.u(2)+0x308,LifeShroom);assert(!enemy.damage(true,0x803538a8)&&enemy.h(enemy.u(2)+0x10c)==1);}
void sourceDeathLeaves(){F f;f.relocate();auto& e=f.vm.battle.entries[1];e.arguments=f.packed.battleAddress()+0x19040;e.argumentCount=5;e.owner=1;auto call=[&](uint32_t fn,std::initializer_list<uint32_t>values){unsigned i=0;for(auto v:values)f.w(e.arguments+4*i++,v);int32_t result=99;const auto ok=f.packed.invoke(e,fn,1,result);assert(!ok||result==2);return ok;};
 const auto b=f.packed.battleAddress(),u=f.u(1);f.w(b+0x1c70,99);f.w(b+0x1c74,0x7fabcdef);assert(call(0x801141a4,{}));assert(f.w(b+0x1c70)==0&&f.w(b+0x1c74)==0x00abcdef);
 assert(call(0x8010ed8c,{1,uint32_t(-30000000)}));assert(e.locals[0]==222);
 assert(call(0x80105e50,{1,0x1000001b,0x181,0x182,1}));uint8_t status;assert(f.vm.memory.readByte(u+0x135,status)&&status==0x81);
 assert(call(0x80105e50,{1,27,3,4,0}));assert(f.vm.memory.readByte(u+0x135,status)&&status==0);
 assert(!call(0x80105e50,{1,1,3,4,1}));assert(f.vm.memory.readByte(u+0x118,status)&&status==0);
 for(unsigned i=0;i<30;++i)assert(f.vm.memory.writeByte(u+0x118+i,0x55));f.w(u+0xae8+8,0x12345678);assert(call(0x80105df8,{1}));for(unsigned i=0;i<30;++i)assert(f.vm.memory.readByte(u+0x118+i,status)&&status==0);assert(f.w(u+0xae8+8)==0x12345678);
}
void guardEffectsHeld(){for(uint32_t guard:{4u,5u}){F f;f.w(f.packed.battleAddress()+0x1cb0,guard);const auto random=f.packed.random.state;assert(!f.damage(false,CrumpWeapon)&&f.h(f.u(1)+0x10c)==1&&f.packed.random.state==random);}}
void originalCrumpDeathEvent(){
 HarborTutorial scene;auto entry=initializeProloguePouch("Yoshi");assert(scene.beginFromPouch(0,entry,4));BattlePadSample quiet;
 for(unsigned i=0;i<1500&&!scene.introComplete;++i){if(scene.pendingService==0x80106980)scene.animationComplete(1);scene.tick(scene.now+17,i%4==0?0x100:0,&quiet);}
 // TEST damage-boundary setup; it does not replace normal source turn order.
 scene.actors[0].hp=1;assert(scene.openWeaponMenu(0));scene.weaponMenuPresented();BattlePadSample a;a.snapshot.buttons=a.pressed=BattleButtonA;scene.tick(scene.now+17,0,&a);assert(scene.inputWeaponMenu(a)==1);
 for(unsigned i=0;i<30;++i)scene.tick(scene.now+17,0,&quiet);assert(scene.dispatchSelectedAttack());
 for(unsigned i=0;i<300;++i)scene.tick(scene.now+17,0,&quiet);
 uint32_t code=0;assert(!scene.failed&&scene.actors[0].hp==0&&scene.readPackedWord(0x84102278,code)&&(code&0x600)==0x600&&!(code&0x800));
 std::cout<<"Original Crump lethal hit-event code="<<std::hex<<code<<" next service="<<scene.pendingService<<std::dec<<" (not battle-result completion)\n";
}
void originalMarioDeathEvent(){
 assert(loadHarborTutorialAssets("romfs"));HarborTutorial scene;assert(scene.beginFresh(0,1,10,4));uint64_t ticks=0;
 // TEST-only pose acknowledgement: production native completion is owned by
 // presentation. This is a source hit-script test, not normal entry proof.
 for(unsigned i=0;i<1500&&!scene.introComplete;++i){if(scene.pendingService==0x80106980)scene.animationComplete(1);scene.tick((++ticks)*1000/60,i%4==0?0x100:0);}
 assert(scene.introComplete&&!scene.failed&&scene.dispatchInstalledAttack(2));BattlePadSample quiet;
 for(unsigned i=0;i<600&&scene.actors[1].hp==1;++i)scene.tick((++ticks)*1000/60,0,&quiet);
 uint32_t code=0,hit=0;assert(!scene.failed&&scene.actors[1].hp==0&&scene.readPackedWord(0x84101278,code)&&scene.readPackedWord(0x841012b4,hit)&&hit&&(code&0x600)==0x600&&!(code&0x800));
 const auto effects=scene.damageEffects.size();
 for(unsigned i=0;i<200;++i)scene.tick((++ticks)*1000/60,0,&quiet);
 assert(!scene.failed&&scene.actors[1].hp==0&&scene.damageEffects.size()==effects);
 std::cout<<"Original Mario lethal hit-event code="<<std::hex<<code<<" next service="<<scene.pendingService<<std::dec<<" (not battle-result completion)\n";
}
}
int main(){lethalMario();lethalEnemy();revivalHeld();sourceDeathLeaves();guardEffectsHeld();originalMarioDeathEvent();originalCrumpDeathEvent();std::cout<<"actual packed source ordinary lethal/no151, dynamic registry and revival admission tests PASS\n";}
