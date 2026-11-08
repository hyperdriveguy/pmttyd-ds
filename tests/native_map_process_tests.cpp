#include "native_map_process.hpp"
#include "native_camera_process.hpp"
#include "native_shadow_process.hpp"
#include "native_ui_startup.hpp"
#include "native_field_animation.hpp"
#include "field_map_file_owner.hpp"
#include "field_light_registry_owner.hpp"
#include "fixtures/harbor_light_golden.hpp"
#include "game.hpp"
#include "event_ram_access.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <iostream>
using namespace ttyd;
namespace {
constexpr uint32_t SdkDescriptors=0x8041f070,HeapHandles=0x8041e844,HeapLength=0x8041e818;
constexpr uint32_t DescriptorBytes=12,AllocatedHead=8,HeaderBytes=32,Archives=0x803dbff0,ArchiveBytes=4*0x28;
constexpr uint32_t GxBegin=0x802b9e10,CapturePc=0x8002a360,CaptureBytes=0x10000;
uint32_t word(EventAddressSpace& m,uint32_t address){uint32_t value=0;assert(m.readWord(address,value));return value;}
uint16_t half(EventAddressSpace& m,uint32_t address){uint16_t value=0;assert(m.readHalf(address,value));return value;}
std::vector<uint8_t> bytes(EventAddressSpace& m,uint32_t address,uint32_t count){std::vector<uint8_t> out(count);for(uint32_t i=0;i<count;++i)assert(m.readByte(address+i,out[i]));return out;}
EventNativePlatform platform(){EventNativePlatform p;p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){};
 p.truncate=[](void*,float f){return int32_t(f);};p.subtractDouble=[](void*,double a,double b){volatile double v=a-b;return v;};p.roundDoubleFloat=[](void*,double d){volatile float v=float(d);return v;};return p;}
struct Fixture {
 std::unique_ptr<Game> game=std::make_unique<Game>(false);
 bool alive=true,fence=false,flush=true;unsigned fenceAttempts=0;
 NativeMapCpuServices cpu(){NativeMapCpuServices p;p.current=[this]{return alive;};
  // Explicit host queue fixture: no renderer/GPU work is submitted here. ARM
  // validation uses the production Renderer service rather than this receipt.
  p.waitDrawSync=[this]{++fenceAttempts;return fence;};p.flushBacking=[this](uint8_t* p,uint32_t n){assert(p&&n);return flush;};return p;
 }
 Fixture(){assert(game->initializeNativeUiFieldAnimation("romfs",NativeFileCache::create(),platform()));}
 EventAddressSpace& memory(){return game->nativeUiStartup()->fieldProcess()->process().vm().memory;}
};
void coldInitAndRetention(){
 std::cerr<<"map phase cold fixture\n";
 Fixture f;auto& g=*f.game;auto& m=f.memory();const auto camera=g.nativeCameraProcess();const auto shadow=g.nativeShadowProcess();
 const auto root=word(m,FieldMapSource::PrimaryHeap),length=word(m,HeapLength);const auto archive=bytes(m,Archives,ArchiveBytes);
 // Owner creation does not allocate detached bank tokens or reset the pool.
 std::cerr<<"map phase cold initialize\n";assert(g.initializeNativeMapProcess("romfs",f.cpu()));auto map=g.nativeMapProcess();assert(map&&map->initialized()&&!map->loaded()&&!map->busy());
 assert(map->cameras()==camera && camera->shadows()==shadow && map->poolAddress()==root && map->poolBytes()==length);
 assert(map->counts().heap0Allocations==3 && map->counts().stableFlushes==3 && map->counts().drawSyncs==0);
 const auto work=map->workAddress();assert(work && word(m,FieldMapSource::Work)==work && word(m,work)==0);
 assert(word(m,FieldLightRegistrySource::Work)==FieldLightRegistrySource::Capacity);
 const auto bank0=word(m,FieldLightRegistrySource::Work+4),bank1=word(m,FieldLightRegistrySource::Work+12);
 assert(bank0&&bank1&&bank0!=bank1 && work!=bank0 && work!=bank1 && word(m,work-HeaderBytes+8)>=FieldMapSource::WorkBytes+HeaderBytes);
 assert(bytes(m,Archives,ArchiveBytes)==archive && m.unmappedRange(0x8a000000,975520)); // Fixture scene was not imported.
 assert(!NativeMapProcess::create(camera,"romfs",f.cpu(),nativeCameraLibmNumerics().reciprocalSquareRoot));
 assert(map->current()&&map->initialize()&&map->counts().heap0Allocations==3);
 std::cerr<<"map phase cold restore\n";auto saved=g.progress;g.restore(saved);assert(g.nativeMapProcess()==map && g.nativeCameraProcess()==camera);
 auto candidate=std::make_unique<Game>(g);assert(!candidate->nativeMapProcess());assert(candidate->retainNativeFieldAnimation(g));assert(candidate->nativeMapProcess()==map);
 auto checkpoint=std::make_unique<Game>(false);checkpoint->restore(saved);assert(checkpoint->retainNativeFieldAnimation(g,true));
 assert(checkpoint->nativeMapProcess()==map && !g.nativeMapProcess());
 f.alive=false;assert(!map->current()&&!checkpoint->nativeMapProcess());f.alive=true;assert(map->initialized());
 // Both real SDK allocation lists participate in owner currentness.
 for(unsigned index=0;index<2;++index){const auto d=word(m,SdkDescriptors)+DescriptorBytes*word(m,HeapHandles+4*index),head=word(m,d+AllocatedHead);
  assert(m.writeWord(d+AllocatedHead,0)&&!map->current());assert(m.writeWord(d+AllocatedHead,head)&&map->current());
  // Existing outer borrows must retain exact, fresh SDK-list reads. A cached
  // success cannot authorize a later failed proof, nor vice versa.
  assert(m.withRawRamAccess([&](auto&){
   if(!map->current()||!m.writeWord(d+AllocatedHead,0)||map->current())return false;
   return m.writeWord(d+AllocatedHead,head)&&map->current();
  }));
 }
 assert(bytes(m,Archives,ArchiveBytes)==archive);
}
void actualHarborCaptureHold(){
 std::cerr<<"map phase harbor fixture\n";
 Fixture f;auto& g=*f.game;auto& m=f.memory();assert(g.initializeNativeMapProcess("romfs",f.cpu()));auto map=g.nativeMapProcess();
 const auto work=map->workAddress(),root=map->poolAddress();const auto archive=bytes(m,Archives,ArchiveBytes);
 const auto projection=word(m,SourceShadowProcess::Primary+0x10c);
 constexpr uint32_t MarkerOffset=0x120,Marker=0x12345678;assert(m.writeWord(work+MarkerOffset,Marker));
 std::cerr<<"map phase harbor fence\n";assert(!g.beginNativeHarborMapLoad()&&map->busy()&&map->pendingCall()->function==0x80005ce4);
 assert(word(m,work+MarkerOffset)==Marker&&word(m,work)==0);
 const auto ticks=g.ticks;BattlePoseClock before,after;assert(g.fieldPartyState().nativeAnimation->clock(before));auto saved=g.progress;
 g.tick({});assert(g.fieldPartyState().nativeAnimation->clock(after));
 assert(g.ticks==ticks&&before.modeZero==after.modeZero&&before.modeNonzero==after.modeNonzero&&word(m,work+MarkerOffset)==Marker);
 g.restore(saved);assert(g.nativeMapProcess()==map&&map->busy());
 auto candidate=std::make_unique<Game>(g);assert(!candidate->retainNativeFieldAnimation(g));
 std::cerr<<"map phase harbor load\n";f.fence=true;constexpr unsigned VisitBudget=65536,ResumeLimit=10000;
 for(unsigned i=0;i<ResumeLimit;++i){assert(!map->resume(VisitBudget));const auto call=map->pendingCall();if(call&&call->function==GxBegin)break;
  if(call&&!FieldMapFileOwner::accepts(call->function)){
   std::cerr<<"unexpected native map child "<<std::hex<<call->function<<" at "<<map->pendingInstruction()<<" args "<<call->arguments[0]<<" "<<call->arguments[1]<<"\n";assert(false);
  }assert(i+1<ResumeLimit);}
 std::cerr<<"map phase harbor captured hold\n";const auto call=map->pendingCall();assert(call&&call->function==GxBegin&&call->arguments[1]==CaptureBytes);
 assert(map->pendingInstruction()==CapturePc&&map->initialized()&&!map->loaded()&&word(m,work)==0);
 assert(call->arguments[0]>=root&&uint64_t(call->arguments[0]-root)+CaptureBytes<=map->poolBytes());
 assert(word(m,work+FieldMapSource::ObjectCount)==540&&word(m,work+FieldMapSource::HitCount)==98);
 assert(map->counts().heap0Allocations==3&&map->counts().estimates==98&&map->counts().drawSyncs==1&&map->counts().vertexInvalidations==1&&map->counts().cacheRanges>=1);
 const auto d=word(m,work+0x7c),t=word(m,work+0x84);assert(d>=root&&t>=root&&word(m,d)==1039618&&word(m,work+0x88)==525792);
 assert(word(m,FieldMapSource::NoFogColor)==0&&bytes(m,Archives,ArchiveBytes)==archive);
 const auto bank=word(m,FieldLightRegistrySource::Work+4);
 for(unsigned i=0;i<harbor_light_fixture::Lights.size();++i){const auto at=bank+i*FieldLightRegistrySource::RecordBytes;
  assert(half(m,at)==(i?0x1001:0x8001)&&word(m,at+FieldLightRegistrySource::Rgba)==harbor_light_fixture::Lights[i].color);
  for(unsigned axis=0;axis<3;++axis)assert(word(m,at+FieldLightRegistrySource::Position+4*axis)==harbor_light_fixture::Lights[i].position[axis]);
 }
 assert(word(m,FieldCameraRoadOwner::RoadWork+0x1d0)==101);
 FieldMapSourceCall getter;getter.function=SourceShadowProcess::Projection;
 assert(g.nativeShadowProcess()->shadow()->service(getter)&&getter.result==projection&&word(m,SourceShadowProcess::Primary+0x10c)==projection);
 const auto object=word(m,work+FieldMapSource::ObjectArray);assert(m.writeWord(object+FieldMapSource::World,Marker));
 const auto count=map->counts();
 for(unsigned i=0;i<3;++i){assert(!map->resume(VisitBudget)&&map->pendingInstruction()==CapturePc&&word(m,work)==0&&word(m,object+FieldMapSource::World)==Marker);}
 assert(map->counts().heap0Allocations==count.heap0Allocations&&map->counts().estimates==count.estimates&&map->counts().cacheRanges==count.cacheRanges);
 // Queued original Close requests retire through the actual source DVD worker,
 // even though the map parent remains held at its unsupported capture child.
 const auto table=word(m,FieldMapFileOwner::TablePointer);
 for(unsigned i=0;i<FieldMapFileOwner::Slots;++i)assert(!half(m,table+i*FieldMapFileOwner::SlotBytes+0x90));
 f.alive=false;assert(!map->resume()&&!map->current()&&map->busy());f.alive=true;assert(map->current()&&!map->loaded());
 // Equal bytes in a foreign allocation do not satisfy the primary pool borrow.
 const auto same=bytes(m,root,map->poolBytes());assert(m.unregisterExternalRam(root)&&m.registerRam(root,same));
 assert(!map->current()&&!map->resume());
}
}

void numericalCallbackBoundary(){
 Fixture f;f.fence=true;auto& m=f.memory();bool available=false,retire=false;unsigned attempts=0;
 auto map=NativeMapProcess::create(f.game->nativeCameraProcess(),"romfs",f.cpu(),[&](float squared)->std::optional<double>{
  ++attempts;if(!available)return {};if(retire){f.alive=false;retire=false;}return 1./std::sqrt(double(squared));
 });assert(map&&map->initialize());
 constexpr uint32_t Distance=0x8029eeb4;constexpr unsigned VisitBudget=65536,ResumeLimit=10000;
 assert(!map->loadHarbor(VisitBudget));
 for(unsigned i=0;i<ResumeLimit;++i){const auto call=map->pendingCall();if(call&&call->function==Distance)break;assert(!map->resume(VisitBudget));assert(i+1<ResumeLimit);}
 assert(attempts==1&&map->counts().estimates==0&&map->pendingCall()->function==Distance);
 const auto pc=map->pendingInstruction();available=retire=true;
 assert(!map->resume(VisitBudget)&&!map->current()&&map->pendingInstruction()==pc&&attempts==2&&map->counts().estimates==0);
 f.alive=true;
 for(unsigned i=0;i<ResumeLimit;++i){assert(!map->resume(VisitBudget));const auto call=map->pendingCall();if(call&&call->function==GxBegin)break;assert(i+1<ResumeLimit);}
 assert(map->pendingCall()->function==GxBegin&&map->counts().estimates==98&&attempts==100&&map->counts().heap0Allocations==3&&!map->loaded());
}

int main(){assert(loadFieldPlayerPoseAssets("romfs"));coldInitAndRetention();actualHarborCaptureHold();numericalCallbackBoundary();std::cout<<"shared native map initialization/capture hold PASS\n";}
