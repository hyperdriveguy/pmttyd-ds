#include "battle_sequence_binding.hpp"
#include "harbor_source_display_pass.hpp"
#include "game.hpp"
#include "field_npc_registry_owner.hpp"
#include "event_native_runtime.hpp"
#include <cassert>
#include <iostream>
using namespace ttyd;
namespace {
struct Fixture {
 struct Cache {unsigned lines=0,barriers=0;}cache;
 std::unique_ptr<Game>game;
 std::unique_ptr<EventNativeRuntime>vm;
 std::unique_ptr<BattleSequenceBinding>binding;
 std::shared_ptr<CameraManager>cameras;
 HarborBattleCameraReceipt receipt;
 Fixture(){

 EventNativePlatform platform;platform.user=&cache;platform.flushBlock=[](void* p,uint32_t){++static_cast<Cache*>(p)->lines;};platform.systemCall=[](void* p){++static_cast<Cache*>(p)->barriers;};
 game=std::make_unique<Game>(false);vm=std::make_unique<EventNativeRuntime>(0x81000000,platform);
 EventScriptAssets data;assert(data.load("romfs/door-data.pmevt") && vm->memory.importScripts(data));
 auto registry=game->initializeOriginalFieldNpcs(vm->memory,0x85000000);assert(registry);
 auto resource=loadBattlePoseResource("romfs/field-native-c_kuri_h.pose-directory.bin","romfs/field-native-c_kuri_h.pmanim");assert(resource && game->originalFieldNpcs->poses().install(resource));
 std::vector<uint8_t> model(64);for(unsigned i=0;i<64;++i)model[i]=resource->draw->byte(4+i);assert(vm->memory.registerRam(0x86000000,std::move(model)));
 uint32_t name=0;assert(vm->memory.readWord(0x90524f10+0x5c,name) && registry->enter(name,0x86000000,{})==0);
 auto n=*registry->lookup(name);uint32_t gp=0,f=0;assert(vm->memory.readWord(0x80415fd0,gp) && vm->memory.readWord(gp+0x168,f));
 // Explicit caller fixture, not the authored tik_01 encounter bridge.
 assert(vm->memory.writeWord(f+8,n));writeSavedSwitch(SavedSwitchFamily::GlobalByte,0,11,vm->interpreterServices().operands);
 assert(loadProloguePouchAssets("romfs"));game->progress.originalPouch=createProloguePouch();assert(game->progress.originalPouch);
 auto storage=BattleSequenceStorage::cold(game->progress.originalPouch,0x88000000,0x100000,0x89000000);assert(storage);
 assert(!BattleSequenceStorage::cold({},0x88000000,0x100000,0x89000000));
 assert(!BattleSequenceStorage::cold(game->progress.originalPouch,0x88000001,0x100000,0x89000000));
 binding=game->bindOriginalBattleSequence(*vm,"romfs");assert(binding);
 assert(vm->memory.registerRam(0x8041eb90,std::vector<uint8_t>(4)));
 assert(!binding->attachStorage(storage));uint32_t absent=0;assert(!vm->memory.readWord(0x8041e808,absent) && !game->battleSequenceProcess->storage);
 assert(vm->memory.unregisterRam(0x8041eb90) && binding->attachStorage(storage));
 assert(!binding->attachStorage(storage));
 assert(vm->memory.writeHalf(0x89000070,7) && game->progress.originalPouch->half(0x70)==7);

  cameras=game->cameraManager;assert(cameras);
  const auto before=*cameras->camera(4);
  BattleSequenceEntryServices services;services.call=[](void*,BattleSequenceCall& c){return c.function==0x800daefc || c.function==0x800db7a0 || c.function==0x800d76b8 || c.function==0x80005f28;};
  auto&work=game->originalSequence->state.work;assert(binding->callback(0x80073f20,work,services));
  assert(!binding->battleCameraReceipt().lifetime);
  // TEST manual mode equality cannot manufacture a producer receipt.
  cameras->camera(4)->mode=2;assert(!binding->battleCameraReceipt().lifetime);
  game->cameraManager.reset();work[1]=4;
  assert(!binding->callback(0x80073b24,work,services)&&binding->pending()==0x8000e95c);
  assert(!binding->battleCameraReceipt().lifetime&&cache.barriers==0);
  game->cameraManager=cameras;
  // TEST parent source position; the real mode setter executes on this retry.
  assert(!binding->callback(0x80073b24,work,services)&&binding->pending()==0x80041c58);
  receipt=binding->battleCameraReceipt();assert(receipt.lifetime&&receipt.current&&receipt.current(*cameras));
  assert(cameras->camera(4)->mode==2&&cameras->camera(4)->fov==before.fov&&cameras->camera(4)->eye==before.eye&&cameras->camera(4)->matrices.view==before.matrices.view);
  auto copy=std::make_unique<CameraManager>(*cameras);assert(!receipt.current(*copy)&&receipt.current(*cameras));
  assert(!binding->callback(0x80073b24,work,services)&&cache.barriers==1);
  assert(binding->battleCameraReceipt().lifetime==receipt.lifetime); // No repeated producer on child hold.
 }
 void stale(){assert(!receipt.current(*cameras)&&!binding->battleCameraReceipt().lifetime);}
};
void actualProducerAndLifetime(){
 {Fixture f;f.vm->reinitializeContext();assert(f.receipt.current(*f.cameras));}
 {Fixture f;auto copy=std::make_unique<Game>(*f.game);assert(f.receipt.current(*f.cameras));copy.reset();assert(f.receipt.current(*f.cameras));}
 {Fixture f;f.game->cameraManager=std::make_shared<CameraManager>(*f.cameras);f.stale();f.game->cameraManager=f.cameras;f.stale();}
 {Fixture f;f.cameras->camera(4)->mode=0;f.stale();f.cameras->camera(4)->mode=2;f.stale();}
 {Fixture f;f.game->restore(f.game->progress);assert(f.game->cameraManager==f.cameras);f.stale();}
 {Fixture f;f.game.reset();f.stale();f.binding.reset();assert(!f.receipt.current(*f.cameras));}
 {Fixture f;f.vm.reset();f.stale();f.binding.reset();assert(!f.receipt.current(*f.cameras));}
 {Fixture f;f.binding.reset();assert(!f.receipt.current(*f.cameras));}
}
}
int main(){actualProducerAndLifetime();std::cout<<"actual source camera producer receipt tests PASS\n";return 0;}
