#include "game.hpp"
#include "field_npc_registry_owner.hpp"
#include "event_native_runtime.hpp"
#include <cassert>
using namespace ttyd;
void runSharedPoseSessionFixture(){
 assert(loadProfessorStoryAssets("romfs") && loadUndergroundStoryAssets("romfs") && loadTrioBattleAssets("romfs") && loadTrioAttackAssets("romfs") && loadFieldPlayerPoseAssets("romfs"));
 auto game=std::make_unique<Game>();auto& parties=const_cast<FieldPartyRegistry&>(game->fieldPartyState());auto pool=parties.originalPool;assert(pool);
 constexpr const char* defaults[]={"M_Z_1","M_Z_1R","M_S_5","PKR_Z_1"};auto resources=fieldPlayerPoseResources();
 FieldPartyRegistry cold;assert(cold.configureOriginalPool(resources) && !cold.configureOriginalPool(resources));
 for(unsigned i=0;i<3;++i){auto* p=parties.pose(int32_t(i));assert(p && p->original()==pool->pose(int32_t(i)) && p->original()->resource==resources[i]);assert(p->original()->word(0xc)==2 && p->original()->resource->names[0]==defaults[i]);}
 auto vm=std::make_unique<EventNativeRuntime>(0x81000000,EventNativePlatform{});EventScriptAssets data;assert(data.load("romfs/door-data.pmevt") && vm->memory.importScripts(data));
 auto registry=game->initializeOriginalFieldNpcs(vm->memory,0x85000000);assert(registry && game->originalFieldNpcs->poses().sharedPool()==pool);
 auto npcResource=loadBattlePoseResource("romfs/field-native-c_kuri_h.pose-directory.bin","romfs/field-native-c_kuri_h.pmanim");assert(npcResource && game->originalFieldNpcs->poses().install(npcResource));
 std::vector<uint8_t> name(64);for(unsigned i=0;i<64;++i)name[i]=npcResource->draw->byte(4+i);assert(vm->memory.registerRam(0x86000000,std::move(name)));
 uint32_t sourceName=0;assert(vm->memory.readWord(0x90524f10+0x5c,sourceName) && registry->enter(sourceName,0x86000000,{})==0);
 auto actor=*registry->lookup(sourceName);uint32_t handle=0;assert(vm->memory.readWord(actor+0x104,handle) && handle==3 && pool->pose(3)->word(0xc)==0);
 auto* mario=parties.pose(1);mario->materialFlags=0x1800;mario->materialLightFlags=0x40;mario->enteredMilliseconds=987;
 assert(mario->original()->word(0xe8)==0x1800 && mario->original()->word(0xec)==0x40 && mario->original()->timestamp()==987);
 assert(vm->memory.registerExternalRam(0x87000000,mario->original()->state.data(),0x170) && vm->memory.writeWord(0x870000e8,0x123));assert(uint32_t(mario->materialFlags)==0x123);
 assert(pool->selectName(1,"M_W_1R",true,{1000,1000}) && uint64_t(mario->enteredMilliseconds)==1000);
 assert(!parties.configureOriginalPool(resources));int32_t partnerPose=parties.enterPose(0x804213e0,2,{},1111);assert(partnerPose==4 && parties.pose(partnerPose)->original()==pool->pose(4) && pool->pose(4)->resource->names[0]==defaults[3]);
 ProfessorStory scene;assert(scene.beginTrioBattleIntro(0));TrioBattleCommands menu;assert(menu.begin(scene));menu.input(0x100,0);menu.input(0x100,0);
 assert(game->trioAttack.begin(menu,scene,10,{},nullptr,nullptr,&parties));assert(game->trioAttack.battlePosePool()==pool.get());
 assert(pool->pose(5) && pool->pose(6) && pool->pose(7) && pool->pose(8)->resource==resources[3] && pool->pose(9)->resource==npcResource);
 const auto generation=pool->pose(5)->nativeGeneration;assert(pool->release(5));
 auto replacement=game->originalFieldNpcs->poses().enter("c_kuri_h",0,{1200,1200});assert(replacement && *replacement==5 && pool->pose(5)->nativeGeneration!=generation);
 game->trioAttack=TrioAttack{};assert(pool->pose(5) && !pool->pose(6) && !pool->pose(7) && !pool->pose(8) && !pool->pose(9) && pool->pose(0) && pool->pose(3) && pool->pose(4));assert(pool->release(5));
 auto fork=std::make_unique<Game>(*game);assert(fork->fieldPartyState().originalPool!=pool && fork->fieldPartyState().pose(1)->original()!=mario->original());fork->fieldPartyState().pose(1)->materialFlags=0x456;assert(uint32_t(mario->materialFlags)==0x123);
 assert(vm->memory.unregisterExternalRam(0x87000000));registry.reset();vm.reset(); // Facades retire before Game::restore.
 auto* partnerBefore=parties.pose(partnerPose);game->restore(Progress{});assert(game->fieldPartyState().originalPool==pool && game->fieldPartyState().pose(1)==mario && game->fieldPartyState().pose(partnerPose)==partnerBefore && game->originalFieldNpcs);
 assert(uint32_t(mario->materialFlags)==0 && uint32_t(mario->materialLightFlags)==0x42 && uint64_t(mario->enteredMilliseconds)==1000);
}
int main(){runSharedPoseSessionFixture();}
