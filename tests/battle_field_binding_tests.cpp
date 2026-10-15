#include "battle_sequence_binding.hpp"
#include "game.hpp"
#include "field_npc_registry_owner.hpp"
#include "event_native_runtime.hpp"
#include "event_memory_release.hpp"
#include <cassert>
using namespace ttyd;
void runFieldBindingFixture(){
 struct Cache {unsigned lines=0,barriers=0;} cache;
 EventNativePlatform platform;platform.user=&cache;platform.flushBlock=[](void* p,uint32_t){++static_cast<Cache*>(p)->lines;};platform.systemCall=[](void* p){++static_cast<Cache*>(p)->barriers;};
 auto game=std::make_unique<Game>(false);auto vm=std::make_unique<EventNativeRuntime>(0x81000000,platform);
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
 auto binding=game->bindOriginalBattleSequence(*vm,"romfs");assert(binding);
 assert(vm->memory.registerRam(0x8041eb90,std::vector<uint8_t>(4)));
 assert(!binding->attachStorage(storage));uint32_t absent=0;assert(!vm->memory.readWord(0x8041e808,absent) && !game->battleSequenceProcess->storage);
 assert(vm->memory.unregisterRam(0x8041eb90) && binding->attachStorage(storage));
 assert(!binding->attachStorage(storage));
 assert(vm->memory.writeHalf(0x89000070,7) && game->progress.originalPouch->half(0x70)==7);
 const auto before=*game->cameraManager->camera(4);
 BattleSequenceEntryServices services;services.call=[](void*,BattleSequenceCall& c){return c.function==0x800daefc || c.function==0x800db7a0 || c.function==0x800d76b8 || c.function==0x80005f28;};
 auto& work=game->originalSequence->state.work;
 assert(vm->memory.registerRam(0x86100000,std::vector<uint8_t>(0x580)) && vm->memory.writeWord(0x86100008,n) && vm->memory.writeWord(gp+0x168,0x86100000));
 assert(!binding->callback(0x80073f20,work,services) && game->soundFlags->value()==0);
 assert(vm->memory.writeWord(gp+0x168,f) && binding->callback(0x80073f20,work,services));
 // Exercise Main's genuine state4 child using a test-owned dispatcher position.
 // No source work advancement/authored resource load is claimed by this fixture.
 work[1]=4;assert(!binding->callback(0x80073b24,work,services) && binding->pending()==0x80041c58 && work[1]==4);
 uint32_t pool=0,value=0;uint16_t used=0;
 assert(vm->memory.readWord(0x8041e80c,pool) && pool==0x88000020 && vm->memory.readWord(pool+4,value) && value==0x63fe0);
 assert(cache.lines==0x64000/32 && cache.barriers==1);
 assert(game->cameraManager->camera(4)->mode==2 && game->cameraManager->camera(4)->fov==before.fov && game->cameraManager->camera(4)->eye==before.eye && game->cameraManager->camera(4)->matrices.view==before.matrices.view);
 assert(vm->memory.readWord(gp+0x14,value) && value==0); // Real animation child has not completed.
 assert(!binding->callback(0x80073b24,work,services) && cache.barriers==1); // No repeated allocation.
 assert(vm->memory.readWord(0x8041eb90,value) && value==0);
 // Actual nested allocations and frees share this arena, without copied B.
 auto memory=vm->memoryBinding.services();assert(storage->canAllocate(pool,0x19098,vm->memory));
 const auto b=eventMapAllocate(pool,0x19098,memory),unit=eventMapAllocate(pool,0x100,memory);assert(b==pool+32 && unit>b);
 assert(vm->memory.writeWord(b,0x12345678));eventMapFree(pool,unit,memory);eventMapFree(pool,b,memory);
 assert(vm->memory.readWord(pool+4,value) && value==0x63fe0 && vm->memory.readHalf(pool+8,used) && used==0);
 freeEventBattleMapPool(memory);assert(vm->memory.readWord(0x8041e80c,value) && value==0);
 assert(vm->memory.readWord(0x88000004,value) && value==0xfffe0);
 binding.reset();assert(!vm->memory.readWord(0x8041eb00,value));
 uint32_t color=0;assert(vm->memory.readWord(0x804203b0,color) && vm->memory.writeWord(0x804203b0,color^1));assert(!game->bindOriginalBattleSequence(*vm,"romfs"));assert(vm->memory.writeWord(0x804203b0,color));
 auto rebound=game->bindOriginalBattleSequence(*vm,"romfs");assert(rebound && vm->memory.readWord(0x88000004,value) && value==0xfffe0 && game->progress.originalPouch->half(0x70)==7);
 // Malformed allocator links are rejected before any source allocator effects.
 assert(vm->memory.writeWord(0x88000000,0x88000000));assert(!storage->canAllocate(0x88000000,0x64000,vm->memory));
}
int main(){runFieldBindingFixture();}
