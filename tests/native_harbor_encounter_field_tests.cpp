#define main nativeHarborSailorFixtureMain
#include "native_harbor_sailors_tests.cpp"
#undef main
#include "native_harbor_encounter_field.hpp"
#include "harbor_encounter.hpp"
#include "field_mario_battle_transfer.hpp"
#include "battle_timing_a.hpp"
namespace {
struct Route {
 World&world;HarborEncounter original,copied;
 NativeFacadeRegistration host,facade;
 std::unique_ptr<NativeHarborEncounterField> field;
 unsigned borrowed=0;bool alive=true;
 bool admitInformation=false;BattleRandom random{7};std::vector<int32_t>draws;
 explicit Route(World&w):world(w){
  // Existing fixture establishes the actual VM/registry/resource owners. Drop
  // its unused constructor facades before attaching the single production one.
  w.creation.reset();w.sailors.reset();w.setup.reset();
  auto assets=std::make_shared<NativeHarborEncounterFieldAssets>();assets->source=w.source;
  // Source records remain unchanged; no synthetic tag or pose table is made.
  assert(assets->tags.load(tagPath));
  EventScriptAssets battle;assert(battle.load("romfs/harbor-tutorial-events.pmevt"));std::array<uint8_t,8> table{};for(unsigned i=0;i<table.size();++i)assert(battle.readByte(0x80417920+i,table[i]));assets->originalHeldItemTable=table;
  NativeHarborEncounterFieldConfig c;c.animation=w.animation;c.registry=w.owner;c.registryBinding=w.registry.get();c.assets=assets;c.host=host.proof();c.serviceLifetime=std::make_shared<int>(1);c.user=this;
  c.current=[](void*p,const EventNativeRuntime&vm,const HarborEncounter*facade){auto&r=*static_cast<Route*>(p);++r.borrowed;return r.alive&&&vm==&r.world.vm&&facade==&r.original;};
  c.setup.user=this;
  c.setup.admitBattle=[](void*p,EventAddressSpace&m,uint32_t actor,int32_t index){auto&r=*static_cast<Route*>(p);assert(&m==&r.world.vm.memory&&r.world.owner->ownsFieldActor(actor)&&index==2);return r.admitInformation;};
  c.setup.random=[](void*p,int32_t bound){auto&r=*static_cast<Route*>(p);r.draws.push_back(bound);return r.random.bounded(bound);};
  field=NativeHarborEncounterField::prepare(w.vm,&original,facade.proof(),std::move(c));assert(field);
  w.vm.platform.user=this;
  w.vm.platform.call=[](void*p,EventEntry&e,uint32_t fn,int32_t first){auto&r=*static_cast<Route*>(p);const auto result=r.field->invoke(&r.original,fn,e,first!=0);return result?*result:0;};
  w.vm.platform.retiring=[](void*p,const EventEntry&e){auto&r=*static_cast<Route*>(p);r.field->retire(&r.original,e);};
 }
 static const char*tagPath;
};
const char*Route::tagPath=nullptr;
void constructorAndCopies(const char*tags){World w(tags);Route::tagPath=tags;Route r(w);assert(!r.field->initialized()&&!r.field->request(&r.original));
 // Both actual Game/Encounter owners are still alive. Shared runtime/callback
 // state is not permission for another facade to borrow the original host.
 r.copied=r.original;const auto before=r.borrowed;assert(!r.field->current(&r.copied));assert(!r.field->advance(&r.copied)&&r.borrowed==before);
 for(unsigned i=0;i<30000&&!r.field->initialized();++i){if(!r.field->advance(&r.original))continue;w.vm.gp.timestamp=uint64_t(i)*17*eventClockRate(243000000);w.vm.runManager();}
 assert(r.field->initialized());const auto actors=w.registry->activeActors();assert(actors&&actors->size()==6);
 assert(w.word(0x80305364+8)==0x90540da8&&w.word(0x80305364+12)==0x90540f84&&w.word(0x8100012c)==0x676f7200);
 for(auto name:{0x90401028u,0x90401040u,0x9040105cu,0x9040106cu,0x9040041cu,0x90400f50u}){const auto actor=r.field->actor(&r.original,name);assert(actor&&*actor);}
 auto&e=createRootEvent(w.vm.schedule,0x90506314,10,0,w.vm.creationServices());
 bool information=false;for(uint32_t a=0x90506314;a<0x90506b44;a+=4)if(w.word(a)==0x8004f090){e.arguments=a+4;information=true;break;}assert(information);
 const auto crump=r.field->actor(&r.original,0x90401040);assert(crump&&*crump);std::array<uint32_t,50> beforeInfo{};for(unsigned i=0;i<50;++i)beforeInfo[i]=w.word(*crump+0x230+4*i);
 assert(!r.field->invoke(&r.original,0x8004f090,e,true)&&r.field->pending()==0x80043d80);assert(!r.field->invoke(&r.original,0x8004f090,e,false)&&r.draws.empty());for(unsigned i=0;i<50;++i)assert(w.word(*crump+0x230+4*i)==beforeInfo[i]);
 r.admitInformation=true;assert(r.field->invoke(&r.original,0x8004f090,e,false)==2);assert(r.draws.size()==17&&r.draws.back()==50&&w.word(*crump+0x230)==0x90540870&&w.word(*crump+0x248)==0x90540c08&&w.word(*crump+0x234)==0x20000010);
 bool found=false;for(uint32_t a=0x90506314;a<0x90506b44;a+=4)if(w.word(a)==0x8004ef54){e.arguments=a+4;found=true;break;}assert(found);
 // Actual source Start prefix remains held at the genuine absent backend.
 assert(!r.field->invoke(&r.original,0x8004ef54,e,true));assert(r.field->pending()==MarioFieldBattlePrepare&&!r.field->request(&r.original));
 const auto prefix=r.borrowed;assert(!r.field->invoke(&r.copied,0x8004ef54,e,false)&&r.borrowed==prefix);
 r.host.retire();assert(!r.field->current(&r.original)&&r.borrowed==prefix);
 // Retire context before VM, as required by its Runtime member order.
 w.vm.platform.retiring=nullptr;w.vm.platform.call=nullptr;r.field.reset();
}
struct FreshHost {
 HarborEncounter scene;NativeFacadeRegistration registration;
 unsigned borrows=0;uint32_t firstFunction=0,firstCommand=0;bool actualRootSeen=false;
};
void freshFacade(const char*tags){
 assert(loadHarborEncounterAssets("romfs"));World resources(tags);FreshHost h;NativeHarborEncounterFieldConfig c;
 auto assets=std::make_shared<NativeHarborEncounterFieldAssets>();assets->source=resources.source;assert(assets->tags.load(tags));
 EventScriptAssets battle;assert(battle.load("romfs/harbor-tutorial-events.pmevt"));std::array<uint8_t,8> table{};for(unsigned i=0;i<table.size();++i)assert(battle.readByte(0x80417920+i,table[i]));assets->originalHeldItemTable=table;
 for(const auto*name:{"c_pkr","c_kanbu_ibt","c_zako_n","c_gara","c_suifu_b"}){const auto stem=std::string("romfs/field-native-")+name;auto pose=loadBattlePoseResource((stem+".pose-directory.bin").c_str(),(stem+".pmanim").c_str());assert(pose);assets->poses.push_back(pose);}
 c.assets=assets;c.animation=NativeFieldAnimation::create("romfs",NativeFileCache::create(),World::platform());assert(c.animation);c.host=h.registration.proof();c.serviceLifetime=std::make_shared<int>(1);c.user=&h;
 c.current=[](void*p,const EventNativeRuntime&vm,const HarborEncounter*f){auto&h=*static_cast<FreshHost*>(p);++h.borrows;return f==&h.scene&&h.scene.nativeFieldVmIs(vm);};
 c.call=[](void*p,EventNativeRuntime&,EventEntry&e,uint32_t fn,bool)->std::optional<int32_t>{auto&h=*static_cast<FreshHost*>(p);if(e.currentCommand>=0x90506314&&e.currentCommand<0x90506dfc&&!h.actualRootSeen){h.actualRootSeen=true;h.firstFunction=fn;h.firstCommand=e.currentCommand;}return {};};
 assert(h.scene.beginNativeField(0,{0,15,0},10,false,c));
 for(uint64_t t=17;t<120000&&!h.actualRootSeen;t+=17)h.scene.tick(t,0,15);
 assert(!h.scene.failed&&h.actualRootSeen&&!h.scene.nativeBattleRequest());
 std::printf("Fresh original beforebattle held: function=%08x command=%08x\n",unsigned(h.firstFunction),unsigned(h.firstCommand));
 const auto before=h.borrows;auto copied=h.scene;copied.tick(120001,0,15);assert(h.borrows==before&&!copied.nativeBattleRequest());
 h.registration.retire();h.scene.tick(120001,0,15);assert(h.borrows==before);
}
}
int main(int argc,char**argv){assert(argc==2);constructorAndCopies(argv[1]);freshFacade(argv[1]);std::puts("Actual six-NPC constructors, original placement roots, copied-facade rejection before host borrow and real held Start pass");return 0;}
