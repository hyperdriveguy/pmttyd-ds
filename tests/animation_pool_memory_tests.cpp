#include "animation_pool_memory.hpp"
#include "animation_tangent_memory.hpp"
#include "animation_initialize.hpp"
#include "battle_sequence_binding.hpp"
#include "event_native_runtime.hpp"
#include "field_npc_registry_owner.hpp"
#include "game.hpp"
#include <cassert>
#include <cstring>
using namespace ttyd;
namespace {
constexpr uint32_t Work=0x803d9470;
struct MemoryWorld {
 std::shared_ptr<BattlePosePool> pool;
 std::unique_ptr<EventNativeRuntime> vm;
 std::unique_ptr<AnimationPoolMemory> view;
 std::unique_ptr<AnimationTangentMemory> tangents;
 uint32_t next=0x8a000000,models=0,textures=0,poses=0;
 explicit MemoryWorld(std::shared_ptr<BattlePosePool> p=std::make_shared<BattlePosePool>()):pool(std::move(p)){
  EventNativePlatform platform;platform.flushBlock=[](void*,uint32_t){};platform.systemCall=[](void*){};
  vm=std::make_unique<EventNativeRuntime>(0x81000000,platform);auto& m=vm->memory;
  assert(m.registerRam(0x80415dc0,std::vector<uint8_t>(4)) && m.writeWord(0x80415dc0,Work));
  assert(m.registerRam(Work,std::vector<uint8_t>(0x110+178*4)) && m.registerRam(0x8041fbb4,std::vector<uint8_t>(4)) && m.writeWord(0x8041fbb4,0x3c8efa33));
  assert(m.registerRam(0x802c1070,std::vector<uint8_t>(8)) && m.writeWord(0x802c1070,0x43300000) && m.writeWord(0x802c1074,0x80000000));
  AnimationInitializeServices s;s.memory.user=this;
  s.memory.word=[](void* p,uint32_t a)->std::optional<uint32_t>{uint32_t v=0;return static_cast<MemoryWorld*>(p)->vm->memory.readWord(a,v)?std::optional<uint32_t>(v):std::nullopt;};
  s.memory.store=[](void* p,uint32_t a,uint32_t v){return static_cast<MemoryWorld*>(p)->vm->memory.writeWord(a,v);};
  s.memory.call=[](void* p,BattleSequenceCall& c){auto& w=*static_cast<MemoryWorld*>(p);
   if(c.function==0x800300f0){assert(c.args[0]==0);c.result=w.next;w.next+=c.args[1]+0x100;return w.vm->memory.registerRam(c.result,std::vector<uint8_t>(c.args[1]));}
   if(c.function==0x8018a61c){c.result=0;return true;} // Explicit fixture null-file branch, not real file startup.
   return c.function==0x80189d38;};
  s.doubleWord=[](void* p,uint32_t a)->std::optional<uint64_t>{uint32_t hi=0,lo=0;auto& m=static_cast<MemoryWorld*>(p)->vm->memory;return m.readWord(a,hi) && m.readWord(a+4,lo)?std::optional<uint64_t>(uint64_t(hi)<<32|lo):std::nullopt;};
  s.tangent=nativeAnimationTangent;AnimationInitializeCursor cursor;assert(initializeAnimation(s,cursor));
  models=word(Work);textures=word(Work+8);poses=word(Work+0x10);
 }
 uint32_t word(uint32_t a){uint32_t v=0;assert(vm->memory.readWord(a,v));return v;}
 void attach(){view=AnimationPoolMemory::bind(pool,vm->memory);assert(view && view->matchesWork());if(!tangents)tangents=AnimationTangentMemory::bind(pool,vm->memory);assert(tangents && tangents->current());}
};
std::shared_ptr<const BattlePoseResource> original(const char* root){auto path=std::string(root)+"/field-player-c_pkr";return loadBattlePoseResource((path+".pose-directory.bin").c_str(),(path+".pmanim").c_str());}
}
void runAnimationPoolMemoryFixture(const char* root){
 auto resource=original(root);assert(resource && loadFieldPlayerPoseAssets(root));
 {MemoryWorld w;w.attach();assert(w.pool->slot(255) && w.word(w.poses+255*0x170)==0);
  auto id=w.pool->enter(resource,2,{});assert(id && *id==0 && w.word(w.models+4)==1 && w.word(w.textures+4)==1 && w.word(w.poses)==1);
  auto other=w.pool->enter(fieldPlayerPoseResources()[0],2,{});assert(other && *other==1);
  for(uint32_t bad:{1u,64u}){assert(w.vm->memory.writeWord(w.poses+0x10,bad) && !w.pool->cacheMetadataReady() && !w.pool->pose(0) && !w.pool->main(0,{}) && !w.pool->selectName(0,"PKR_W_1",true,{}) && !w.pool->evaluate(0,{}));}
  assert(w.vm->memory.writeWord(w.poses+0x10,0) && w.pool->cacheMetadataReady());
  // Cross-row halfword writes and ordinary scalar words alias the actual rows.
  assert(w.vm->memory.writeHalf(w.models+15,0x0203));assert(w.pool->models()[0].texture()==2 && w.pool->models()[1].state[0]==3 && !w.pool->cacheMetadataReady());assert(w.vm->memory.writeHalf(w.models+15,0) && w.pool->cacheMetadataReady());
  assert(w.vm->memory.writeByte(w.models+7,2) && w.pool->models()[0].references()==2);
  assert(w.vm->memory.writeHalf(w.models+6,1) && w.pool->models()[0].references()==1);
  auto* pose=w.pool->pose(0);assert(w.vm->memory.writeWord(w.poses+0xe8,0x1234) && pose->word(0xe8)==0x1234);
  assert(w.vm->memory.writeWord(Work+0xe8,1) && w.pool->outputSelector()==1 && !w.pool->main(0,{}));w.pool->outputSelector(0);assert(w.word(Work+0xe8)==0);
  // Native release holds unsupported signed ref domains without replaying effects.
  assert(w.vm->memory.writeWord(w.models+4,0x80000000) && !w.pool->release(0) && !w.pool->enter(resource,2,{}));assert(w.word(w.poses)==1 && w.word(w.models+4)==0x80000000);
  assert(w.vm->memory.writeWord(w.models+4,1) && w.vm->memory.writeWord(w.models+8,0x12345678) && w.vm->memory.writeWord(w.textures+8,0x87654321));
  assert(w.pool->release(0) && w.word(w.models)==0 && w.word(w.models+8)==0x12345678 && w.word(w.textures+8)==0x87654321);
  auto second=w.pool->enter(resource,2,{});assert(second && *second==0 && w.word(w.models+8)==0 && w.word(w.textures+8)==0); // New native files remain explicitly unbound.
  auto copy=std::make_shared<BattlePosePool>(*w.pool);copy->pose(0)->word(0xe8,0x7777);copy->outputSelector(1);assert(pose->word(0xe8)==0 && w.pool->outputSelector()==0 && copy->models()[0].state.data()!=w.pool->models()[0].state.data());
  assert(!AnimationPoolMemory::bind(w.pool,w.vm->memory));
  assert(w.view->detach() && w.word(w.models+4)==1 && w.word(w.poses)==1);w.view.reset();
  assert(w.word(Work+0x110)!=0);w.attach();assert(w.pool->pose(0)==pose && w.word(w.models+4)==1);
  assert(w.vm->memory.writeWord(Work+0x14,255) && !w.view->matchesWork() && !w.pool->pose(0) && !w.pool->enter(resource,2,{}));
  assert(w.vm->memory.writeWord(Work+0x14,256) && w.pool->pose(0)==pose);
  assert(w.vm->memory.writeWord(0x80415dc0,0) && !w.view->matchesWork() && !w.pool->pose(0));assert(w.vm->memory.writeWord(0x80415dc0,Work));}
 // Mismatched source bytes and late extraction failure must leave both owners intact.
 {MemoryWorld w;assert(w.vm->memory.writeWord(w.models+4,1));assert(!AnimationPoolMemory::bind(w.pool,w.vm->memory) && !w.pool->slot(0) && w.word(w.models+4)==1);}
 {MemoryWorld w;std::vector<uint8_t> backing;assert(w.vm->memory.unregisterRam(w.poses,&backing) && w.vm->memory.registerExternalRam(w.poses,backing.data(),uint32_t(backing.size())));
  assert(!AnimationPoolMemory::bind(w.pool,w.vm->memory) && !w.pool->slot(0) && w.word(Work+4)==64 && w.word(w.models)==0);assert(w.vm->memory.unregisterExternalRam(w.poses) && w.vm->memory.registerRam(w.poses,std::move(backing)));w.attach();}
 // Genuine complete capacity scan and shared Mario/partner/NPC source entry binding.
 {assert(loadFieldPlayerPoseAssets(root));auto game=std::make_unique<Game>(false);auto& parties=const_cast<FieldPartyRegistry&>(game->fieldPartyState());assert(parties.configureOriginalPool(fieldPlayerPoseResources()));
  MemoryWorld w(parties.originalPool);w.attach();auto& mario=const_cast<FieldMarioBattleState&>(game->fieldMarioBattleState());assert(parties.initializeMario(mario,{},0) && parties.enterPose(0x804213e0,2,{},0)==3);
  EventScriptAssets data;assert(data.load((std::string(root)+"/door-data.pmevt").c_str()) && w.vm->memory.importScripts(data));
  auto registry=game->initializeOriginalFieldNpcs(w.vm->memory,0x85000000);assert(registry);auto path=std::string(root)+"/field-native-c_kuri_h";
  auto npc=loadBattlePoseResource((path+".pose-directory.bin").c_str(),(path+".pmanim").c_str());assert(npc && game->originalFieldNpcs->poses().install(npc));
  std::vector<uint8_t> nameBytes(64);for(unsigned i=0;i<64;++i)nameBytes[i]=npc->draw->byte(4+i);assert(w.vm->memory.registerRam(0x86000000,std::move(nameBytes)));
  const auto sourceName=w.word(0x90524f10+0x5c);assert(registry->enter(sourceName,0x86000000,{})==0);auto actor=*registry->lookup(sourceName);assert(w.word(actor+0x104)==4);
  const auto gp=w.word(0x80415fd0),field=w.word(gp+0x168);assert(w.vm->memory.writeWord(field+8,actor));
  // Explicit fixture original MODEL header/file/holder views; not type5 loading
  // or file-manager history. No nonzero file identity is supplied by production.
  for(unsigned i=0;i<64;++i)if(auto r=w.pool->models()[i].resource){const uint32_t file=0x8e000000+0x100*i,holder=0x8e010000+4*i,model=0x8e020000+0x200*i;
   assert(w.vm->memory.registerRam(file,std::vector<uint8_t>(0xa4)) && w.vm->memory.registerRam(holder,std::vector<uint8_t>(4)));
   std::vector<uint8_t> header(0x1b0);for(unsigned n=0;n<header.size();++n)header[n]=r->draw->byte(n);
   assert(w.vm->memory.registerRam(model,std::move(header)) && w.vm->memory.writeWord(file+0xa0,holder) && w.vm->memory.writeWord(holder,model) && w.vm->memory.writeWord(w.models+16*i+8,file));}
  assert(w.view->detach());w.view.reset();writeSavedSwitch(SavedSwitchFamily::GlobalByte,0,11,w.vm->interpreterServices().operands);
  assert(loadProloguePouchAssets(root));game->progress.originalPouch=createProloguePouch();assert(game->progress.originalPouch);
  auto binding=game->bindOriginalBattleSequence(*w.vm,root);assert(binding && binding->attachAnimationPool() && !binding->attachAnimationPool());
  auto storage=BattleSequenceStorage::cold(game->progress.originalPouch,0x88000000,0x100000,0x89000000);assert(storage && binding->attachStorage(storage));
  BattleSequenceEntryServices services;services.call=[](void*,BattleSequenceCall& c){return c.function==0x800daefc || c.function==0x800db7a0 || c.function==0x800d76b8 || c.function==0x80005f28;};
  auto& work=game->originalSequence->state.work;assert(binding->callback(0x80073f20,work,services));work[1]=4;
  assert(!binding->callback(0x80073b24,work,services) && binding->pending()==0x8004ce58 && w.word(Work+0x10c)==1);
  for(unsigned i=0;i<4;++i){auto* p=w.pool->pose(int32_t(i));assert(p && p->word(0xc)==2 && p->word(0x48) && p->word(0x58)==p->word(0x5c) && p->word(0x68)==p->word(0x6c) && p->word(0x3c)==0xffffffff);}
  assert(w.pool->pose(4)->word(0x48)==0);auto first=w.pool->pose(0)->word(0x48);assert(!binding->callback(0x80073b24,work,services) && binding->pending()==0x8004ce58 && w.pool->pose(0)->word(0x48)==first);
  assert(w.pool->selectName(0,"M_W_1",true,{0,0}) && w.pool->evaluate(0,{0,17}) && w.word(w.poses+0x3c)!=0xffffffff);
  binding.reset();assert(w.word(w.models+4)==1 && w.word(w.poses+0x48)==first);
 }
}
int main(){runAnimationPoolMemoryFixture("romfs");}
