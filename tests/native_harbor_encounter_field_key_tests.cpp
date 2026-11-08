#define main nativeGetPositionSailorFixtureMain
#include "native_harbor_sailors_tests.cpp"
#undef main
#include "native_harbor_encounter_field.hpp"
#include "native_field_mario_get_position.hpp"
#include "native_environment_sound.hpp"
#include "native_field_mario_key.hpp"
#include "field_party.hpp"
#include "harbor_encounter.hpp"
#include <cstring>
namespace {
struct FreshHost {
 HarborEncounter scene;NativeFacadeRegistration registration;
 std::unique_ptr<NativeFieldMarioGetPosition> getPosition;
 std::unique_ptr<NativeFieldMarioKey>key;FieldMarioBattleState mario;
 NativeFacadeRegistration audioHost;std::shared_ptr<EventScriptAssets>audioSource;
 std::shared_ptr<NativeEnvironmentSound>environment;std::optional<NativeEnvironmentColdProof>cold;
 std::unique_ptr<NativeFieldEnvironmentOff>environmentOff;
 unsigned borrows=0;uint32_t firstFunction=0,firstCommand=0;bool actualRootSeen=false;
};
void freshFacade(const char*tags){
 assert(loadHarborEncounterAssets("romfs"));World resources(tags);FreshHost h;NativeHarborEncounterFieldConfig c;
 auto assets=std::make_shared<NativeHarborEncounterFieldAssets>();assets->source=resources.source;assert(assets->tags.load(tags));
 EventScriptAssets battle;assert(battle.load("romfs/harbor-tutorial-events.pmevt"));std::array<uint8_t,8> table{};for(unsigned i=0;i<table.size();++i)assert(battle.readByte(0x80417920+i,table[i]));assets->originalHeldItemTable=table;
 for(const auto*name:{"c_pkr","c_kanbu_ibt","c_zako_n","c_gara","c_suifu_b"}){const auto stem=std::string("romfs/field-native-")+name;auto pose=loadBattlePoseResource((stem+".pose-directory.bin").c_str(),(stem+".pmanim").c_str());assert(pose);assets->poses.push_back(pose);}
 c.assets=assets;c.animation=NativeFieldAnimation::create("romfs",NativeFileCache::create(),World::platform());assert(c.animation);c.host=h.registration.proof();c.serviceLifetime=std::make_shared<int>(1);c.user=&h;
 c.current=[](void*p,const EventNativeRuntime&vm,const HarborEncounter*f){auto&h=*static_cast<FreshHost*>(p);++h.borrows;return f==&h.scene&&h.scene.nativeFieldVmIs(vm);};
 // Explicit TEST native Audio startup: same retained owner for all field calls,
 // actual two-store contribution only, not whole psndInit/SoundInit completion.
 h.audioSource=std::make_shared<EventScriptAssets>();assert(h.audioSource->load("romfs/harbor-tutorial-events.pmevt"));
 NativeEnvironmentSoundBinding sound;sound.lifetime=h.audioSource;sound.host=h.audioHost.proof();sound.user=&h;sound.current=[](void*){return true;};
 sound.word=[](void*p,uint32_t address)->std::optional<uint32_t>{auto&host=*static_cast<FreshHost*>(p);uint32_t word=0;if(!host.audioSource->readWord(address,word))return {};return word;};
 h.environment=NativeEnvironmentSound::prepare(sound);assert(h.environment);h.cold=h.environment->initializeNativeCold();assert(h.cold);
 c.call=[](void*p,EventNativeRuntime&vm,EventEntry&e,uint32_t fn,bool first)->std::optional<int32_t>{auto&h=*static_cast<FreshHost*>(p);
  if(fn==NativeFieldMarioKeyFunction){
   if(!h.key){NativeFieldMarioKeyBinding b;b.lifetime=std::make_shared<int>(1);b.host=h.registration.proof();b.user=&h;b.current=[](void*p,const EventNativeRuntime&v){return static_cast<FreshHost*>(p)->scene.nativeFieldVmIs(v);};
    auto&w=b.world;w.storage.user=&h;w.admit=[](void*,uint32_t f){return f==NativeFieldMarioKeyFunction;};
    w.storage.mario=[](void*)->uintptr_t{return FieldMarioOwnerAddress;};
    w.storage.read=[](void*p,uintptr_t a){assert(a==FieldMarioOwnerAddress);return static_cast<FreshHost*>(p)->mario.flags;};
    w.storage.write=[](void*p,uintptr_t a,uint32_t v){assert(a==FieldMarioOwnerAddress);static_cast<FreshHost*>(p)->mario.flags=v;};
    w.byte=[](void*p,uintptr_t a){assert(a==FieldMarioOwnerAddress+0x39);return static_cast<FreshHost*>(p)->mario.keyDepth;};
    w.storeByte=[](void*p,uintptr_t a,uint8_t v){assert(a==FieldMarioOwnerAddress+0x39);static_cast<FreshHost*>(p)->mario.keyDepth=v;};
    h.key=NativeFieldMarioKey::create(vm,b);if(!h.key)return {};
   }return h.key->invoke(fn,e,first);
  }
  if(fn==NativeEnvironmentOffEventFunction){
   if(!h.environmentOff){h.environmentOff=NativeFieldEnvironmentOff::create(vm,h.environment,*h.cold);if(!h.environmentOff)return {};}
   return h.environmentOff->invoke(fn,e,first);
  }
  if(fn==NativeFieldMarioGetPositionFunction){
   if(!h.getPosition){NativeFieldMarioGetPositionBinding b;b.lifetime=std::make_shared<int>(1);b.host=h.registration.proof();b.user=&h;
    b.current=[](void*p,const EventNativeRuntime&v){auto&host=*static_cast<FreshHost*>(p);return host.scene.nativeFieldVmIs(v);};
    b.position=[](void*,unsigned axis)->std::optional<uint32_t>{const std::array<float,3>actualTestXYZ{-20,15,-110};uint32_t word;std::memcpy(&word,&actualTestXYZ[axis],4);return word;};
    h.getPosition=NativeFieldMarioGetPosition::create(vm,b);if(!h.getPosition)return {};
   }return h.getPosition->invoke(fn,e,first);
  }
if(e.currentCommand>=0x90506314&&e.currentCommand<0x90506dfc&&!h.actualRootSeen){h.actualRootSeen=true;h.firstFunction=fn;h.firstCommand=e.currentCommand;}return {};};
 assert(h.scene.beginNativeField(0,{0,15,0},10,false,c));
 for(uint64_t t=17;t<120000&&!h.actualRootSeen;t+=17)h.scene.tick(t,0,15);
 assert(!h.scene.failed&&h.actualRootSeen&&h.getPosition&&h.key&&h.mario.keyDepth==1&&(h.mario.flags&8)&&!h.scene.nativeBattleRequest());
 std::printf("Fresh original beforebattle after real GetPos/ENVOff/Key held: function=%08x command=%08x\n",unsigned(h.firstFunction),unsigned(h.firstCommand));
 const auto before=h.borrows;auto copied=h.scene;copied.tick(120001,0,15);assert(h.borrows==before&&!copied.nativeBattleRequest());
 h.registration.retire();h.scene.tick(120001,0,15);assert(h.borrows==before);
}
}
int main(int argc,char**argv){assert(argc==2);freshFacade(argv[1]);return 0;}
