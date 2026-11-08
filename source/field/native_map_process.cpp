#include "native_map_process.hpp"
#include "native_camera_process.hpp"
#include "native_shadow_process.hpp"
#include "native_ui_startup.hpp"
#include "native_field_animation.hpp"
#include "field_map_source_owner.hpp"
#include "field_map_file_owner.hpp"
#include "field_light_registry_owner.hpp"
#include "field_map_sdk.hpp"
#include "event_heap_allocation.hpp"
#include <algorithm>
#include <cstring>
#include <string>
#include <vector>
namespace ttyd {
namespace {
constexpr uint32_t MapStack=0x8d040000,FileStack=0x8d050000,FileTable=0x8d060000;
constexpr uint32_t LightScratch=0x8d070000,Input=0x8d070200,InputBytes=32;
constexpr uint32_t HeapHandles=0x8041e844,HeapStarts=0x8041e830,HeapEnds=0x8041e81c,SdkRoot=0x8041f070;
constexpr uint32_t PrimaryLength=0x8041e818,DescriptorBytes=12,HeaderBytes=32,NodePrevious=0,NodeNext=4,NodeSize=8;
constexpr uint32_t DrawSync=0x80005ce4,VertexInvalidate=0x802b4df4,CacheFlush=0x80296a54,CacheInvalidate=0x80296a28;
constexpr uint32_t CopyBounded=0x8026d5b8,Format=0x8026a0b8;
constexpr uint32_t LightBankBytes=FieldLightRegistrySource::Capacity*FieldLightRegistrySource::RecordBytes;
constexpr unsigned PathBytes=256;
constexpr uint32_t MapLightLocalsBytes=32,LightScratchBytes=PathBytes+MapLightLocalsBytes;
constexpr uint32_t DescriptorAllocated=8,MapNext=0,MapFreeBytes=4,MapUsed=8;
// This asset also contains a relocated scene used by portable constructor
// fixtures. Actual native loading must acquire/relocate its own original D file.
constexpr uint32_t FixtureScene=0x8a000000,FixtureSceneBytes=975520;
struct Literal {uint32_t address=0;std::vector<uint8_t> bytes;};
struct Guard {bool& busy;explicit Guard(bool& value):busy(value){busy=true;}~Guard(){busy=false;}};
}
struct NativeMapProcess::State {
 std::shared_ptr<NativeCameraProcess> camera;
 NativeMapCpuServices cpu;Estimate estimate;
 struct Heap {uint32_t handle=0,start=0,end=0;};
 std::array<Heap,2> heaps{};uint32_t descriptors=0,pool=0,bytes=0;
 struct Allocation {uint32_t address=0,bytes=0;};
 std::array<Allocation,3> allocations{};unsigned allocated=0;
 std::vector<uint8_t> primary;
 std::vector<Literal> literals;unsigned installed=0;bool poolMapped=false,inputMapped=false;
 std::array<uint8_t,InputBytes> input{};
 std::unique_ptr<FieldMapFileOwner> files;
 std::unique_ptr<FieldLightRegistryOwner> lights;
 std::unique_ptr<FieldMapSourceOwner> maps;
 std::unique_ptr<FieldMapSdk> sdk;
 FieldLightRequest lightInit;FieldMapRequest mapInit,mapLoad;
 bool starting=false,lightsDone=false,initDone=false,loading=false,loadDone=false,sdkActive=false;
 std::optional<FieldMapSourceCall> held;
 Counts counts;
 EventNativeRuntime& vm()const{return camera->shadows()->ui()->fieldProcess()->process().vm();}
 ~State(){maps.reset();sdk.reset();lights.reset();files.reset();auto& m=vm().memory;
  if(inputMapped)m.unregisterExternalRam(Input,input.data());
  // The SDK allocation belongs to the retained process. Return its complete
  // bytes to owned VM RAM when this borrowing owner retires; preserve foreign
  // replacement mappings. Stop all map consumers before destroying this owner.
  if(poolMapped&&m.unregisterExternalRam(pool,primary.data()))m.registerRam(pool,std::move(primary));
  else if(!poolMapped&&!primary.empty()&&m.unmappedRange(pool,bytes))m.registerRam(pool,std::move(primary));
  while(installed){auto& l=literals[--installed];m.unregisterExternalRam(l.address,l.bytes.data());}
 }
 bool heapCurrent(unsigned index)const{
  const auto& h=heaps[index];auto& m=vm().memory;uint32_t value=0,head=0;
  if(!m.readWord(HeapHandles+4*index,value)||value!=h.handle || !m.readWord(HeapStarts+4*index,value)||value!=h.start ||
     !m.readWord(HeapEnds+4*index,value)||value!=h.end || !m.readWord(SdkRoot,value)||value!=descriptors ||
     !m.readWord(descriptors+DescriptorBytes*h.handle,value)||value!=h.end-h.start ||
     !m.readWord(descriptors+DescriptorBytes*h.handle+DescriptorAllocated,head))return false;
  unsigned wanted=index==1?1:0;for(unsigned i=0;index==0&&i<allocated;++i)wanted+=allocations[i].address!=0;
  if(!wanted)return true;
  uint32_t previous=0,mask=0;unsigned found=0,budget=(h.end-h.start)/HeaderBytes;
  while(head&&budget--){uint32_t prev=0,next=0,span=0;
   if(head<h.start || uint64_t(head)+HeaderBytes>h.end || (head&(HeaderBytes-1)) ||
      !m.readWord(head+NodePrevious,prev)||prev!=previous || !m.readWord(head+NodeNext,next) ||
      !m.readWord(head+NodeSize,span)||span<HeaderBytes||uint64_t(head)+span>h.end)return false;
   if(index==1){if(head+HeaderBytes==pool)return span>=bytes+HeaderBytes;}
   else for(unsigned i=0;i<allocated;++i)if(allocations[i].address && head+HeaderBytes==allocations[i].address){
    const uint32_t required=(allocations[i].bytes+2*HeaderBytes-1)&~(HeaderBytes-1);
    if(span<required || (mask&(1u<<i)))return false;
    mask|=1u<<i;++found;
   }
   if(index==0&&found==wanted)return true;
   previous=head;head=next;
  }return false;
 }
 bool baseCurrent()const{
  if(!camera || !camera->ready() || !cpu.current || !cpu.current() ||
     !camera->shadows()->ui()->fieldProcess()->process().heapGlobalsCurrent())return false;
  auto& m=vm().memory;uint32_t actual=0;
  if(!m.readWord(FieldMapSource::PrimaryHeap,actual)||actual!=pool || !m.readWord(PrimaryLength,actual)||actual!=bytes ||
     !heapCurrent(0)||!heapCurrent(1))return false;
  if(poolMapped&&!m.hasRamBacking(pool,primary.data(),bytes))return false;
  if(inputMapped&&!m.hasRamBacking(Input,input.data(),InputBytes))return false;
  for(unsigned i=0;i<installed;++i){const auto& l=literals[i];if(!m.hasRamBacking(l.address,l.bytes.data(),uint32_t(l.bytes.size())))return false;}
  return true;
 }
 bool current()const{
  if(files&&!files->current())return false;
  // The stationary child owners already include this exact backend proof.
  // Avoid recursively validating camera/UI/heap identities again at each layer.
  if(maps)return maps->current();
  if(lightsDone)return lights->ready();
  return baseCurrent();
 }
 bool install(const char* root){
  auto& m=vm().memory;EventScriptAssets inputs;
  const char* packs[]={"field-harbor-map-source.pmevt","field-harbor-loader-literals.pmevt","field-hit-literals.pmevt","field-light-registry.pmevt"};
  if(!inputs.load((std::string(root)+"/"+packs[0]).c_str()))return false;
  for(unsigned i=1;i<sizeof(packs)/sizeof(*packs);++i){EventScriptAssets next;
   if(!next.load((std::string(root)+"/"+packs[i]).c_str())||!inputs.merge(next))return false;
  }
  for(const auto& segment:inputs.segments()){
   if(segment.address==FixtureScene&&segment.size==FixtureSceneBytes)continue;
   constexpr uint32_t OriginalMem1End=0x81800000;
   if(uint64_t(segment.address)+segment.size>OriginalMem1End)return false;
   for(uint32_t i=0;i<segment.size;++i){const auto at=segment.address+i;uint8_t expected=0,actual=0;
    if(!inputs.readByte(at,expected))return false;
    if(m.readByte(at,actual)){if(actual!=expected)return false;continue;}
    if(!m.unmappedRange(at,1))return false;
    if(literals.empty()||uint64_t(literals.back().address)+literals.back().bytes.size()!=at)literals.push_back({at,{}});
    literals.back().bytes.push_back(expected);
   }
  }
  // No fixed fixture heaps or reset archive descriptors: reject occupied private
  // regions before mutating live source storage. Owners validate their globals.
  if(!m.unmappedRange(MapStack,FieldMapSourceOwner::StackBytes) || !m.unmappedRange(FileStack,FieldMapFileOwner::StackBytes) ||
     !m.unmappedRange(FileTable,FieldMapFileOwner::TableBytes) || !m.unmappedRange(LightScratch,LightScratchBytes) ||
     !m.unmappedRange(Input,InputBytes) || !m.unmappedRange(FieldMapSource::Work,FieldMapSource::GlobalsBytes) ||
     !m.unmappedRange(FieldMapSource::NoFogColor,sizeof(uint32_t)) ||
     !m.unmappedRange(FieldLightRegistrySource::Work,2*FieldLightRegistrySource::DescriptorBytes) ||
     !m.unmappedRange(FieldLightRegistrySource::Presets,4*FieldLightRegistrySource::RecordBytes) ||
     !m.unmappedRange(FieldLightRegistrySource::CraftAmbient,FieldLightRegistrySource::AmbientBytes))return false;
  for(auto& l:literals){if(!m.registerExternalRam(l.address,l.bytes.data(),uint32_t(l.bytes.size())))return false;++installed;}
  if(!m.extractRam(pool,bytes,primary) || !m.registerExternalRam(pool,primary.data(),bytes))return false;
  poolMapped=true;
  const char name[]="gor_00";std::memcpy(input.data(),name,sizeof(name));
  if(!m.registerExternalRam(Input,input.data(),InputBytes))return false;
  inputMapped=true;
  files=FieldMapFileOwner::bind(m,FileTable,FileStack,root);
  if(!files)return false;
  FieldLightHeapServices heap;heap.user=this;
  heap.current=[](void* p){return static_cast<State*>(p)->baseCurrent();};
  heap.allocate=[](void* p,uint32_t group,uint32_t size,uint32_t& result){return group==0&&static_cast<State*>(p)->allocate(size,result);};
  heap.flush=[](void* p,uint8_t* backing,uint32_t size){return static_cast<State*>(p)->flush(backing,size);};
  lights=FieldLightRegistryOwner::bind(m,LightScratch,heap);
  return lights && current();
 }
 bool allocate(uint32_t size,uint32_t& result){
  if(!baseCurrent() || allocated==allocations.size() || (size!=LightBankBytes&&size!=FieldMapSource::WorkBytes))return false;
  result=eventHeapAllocate(0,size,vm().memoryBinding.services());
  allocations[allocated++]={result,size};++counts.heap0Allocations;return true;
 }
 bool flush(uint8_t* backing,uint32_t size){
  if(!baseCurrent()||!cpu.flushBacking||!cpu.flushBacking(backing,size)||!baseCurrent())return false;
  ++counts.stableFlushes;return true;
 }
 bool text(uint32_t address,std::string& out)const{
  out.clear();for(unsigned i=0;i<PathBytes;++i){uint8_t value=0;if(uint64_t(address)+i>UINT32_MAX || !vm().memory.readByte(address+i,value))return false;if(!value)return true;out+=char(value);}return false;
 }
 bool writeString(uint32_t destination,const std::string& value){
  auto& m=vm().memory;if(!m.contiguousRam(destination,uint32_t(value.size()+1)))return false;
  for(unsigned i=0;i<=value.size();++i)if(!m.writeByte(destination+i,i<value.size()?uint8_t(value[i]):0))return false;
  return true;
 }
 bool sdkDomain(const FieldMapSourceCall& q)const{
  auto& m=vm().memory;
  // No semantic callbacks inside the synchronous SDK admission. These live
  // constants remain fresh data reads, never cached numerical values.
  if(!m.contiguousRam(animation_sdk::One,8) || !m.contiguousRam(animation_sdk::ConcatTail,8) ||
     !m.contiguousRam(0x80429394,8))return false;
  uint32_t destination=0,size=0;const auto& a=q.arguments;
  switch(q.function){
  case animation_sdk::Identity:case animation_sdk::Translate:case animation_sdk::Scale:case animation_sdk::RotateRadians:destination=a[0];size=animation_sdk::MatrixBytes;break;
  case animation_sdk::Concat:destination=a[2];size=animation_sdk::MatrixBytes;break;
  case animation_sdk::MultiplyPoint:case 0x8029eb80:case 0x8029eba4:case 0x8029eca4:destination=a[2];size=3*sizeof(uint32_t);break;
  case 0x8029ebc8:destination=a[1];size=3*sizeof(uint32_t);break;
  default:return true; // Unknown leaves still remain unavailable in the SDK.
  }
  const auto within=[&](uint32_t start,uint32_t bytes){return destination>=start&&uint64_t(destination-start)+size<=bytes;};
  return within(pool,bytes)||within(MapStack,FieldMapSourceOwner::StackBytes);
 }
 bool advanceSdk(FieldMapSourceCall& q){
  if(!sdk || sdkActive || !sdkDomain(q))return false;
  bool complete=false;
  // One synchronous, raw-only SDK primitive on the owner thread. Its bounded
  // writes cannot alter process/SDK metadata, and no other consumer interleaves.
  // End admission before checking lifetime again, including on every hold.
  {Guard admitted(sdkActive);complete=sdk->advance(q);}
  return current()&&complete;
 }
 std::optional<double> estimateValue(float value){
  const bool admitted=sdkActive;sdkActive=false;
  const auto result=current()?estimate(value):std::nullopt;
  const bool valid=current();sdkActive=admitted&&valid;
  if(!valid)return {};
  if(result)++counts.estimates;
  return result;
 }
 bool child(FieldMapSourceCall& q){
  // Only FieldMapSourceOwner::child invokes this provider, after its complete
  // currentness proof. SDK/native callbacks revalidate before acknowledgement.
  auto& m=vm().memory;const auto& a=q.arguments;bool complete=false;
  if(FieldMapFileOwner::accepts(q.function))complete=files->service(q);
  else switch(q.function){
  case DrawSync:
   if(cpu.waitDrawSync&&cpu.waitDrawSync()&&current()){++counts.drawSyncs;complete=true;}
   break;
  case VertexInvalidate:
   // This owner never acknowledges capture and has no native map GPU cache.
   // CPU-only VM writes are synchronous; no GameCube FIFO command is emitted.
   __sync_synchronize();++counts.vertexInvalidations;complete=true;break;
  case CacheFlush:case CacheInvalidate:
   if(a[0]>=pool && uint64_t(a[0]-pool)+a[1]<=bytes && a[1] && m.hasRamBacking(pool,primary.data(),bytes) &&
      cpu.flushBacking && cpu.flushBacking(primary.data()+a[0]-pool,a[1]) && current()){
    ++counts.cacheRanges;complete=true;
   }break;
  case CopyBounded:{
   std::string value;
   if(a[2]>PathBytes||!text(a[1],value)||!m.contiguousRam(a[0],a[2]))break;
   for(unsigned i=0;i<a[2];++i)if(!m.writeByte(a[0]+i,i<value.size()?uint8_t(value[i]):0))return false;
   q.result=a[0];complete=true;break;
  }
  case Format:{
   std::string format,left,right,result;if(!text(a[1],format)||!text(a[2],left))break;
   if(format=="./m/%s/d")result="./m/"+left+"/d";
   else if(format=="./m/%s/t")result="./m/"+left+"/t";
   else if(format=="./m/%s/c")result="./m/"+left+"/c";
   else if(format=="%s/%s"&&text(a[3],right))result=left+"/"+right;
   else break;
   if(result.size()>=PathBytes||!writeString(a[0],result))break;
   q.result=uint32_t(result.size());complete=true;break;
  }
  default:complete=advanceSdk(q);break;
  }
  if(!complete)held=q;
  return complete;
 }
 bool bindMap(){
  FieldMapHeapOwner heap;heap.user=this;heap.address=pool;heap.bytes=bytes;heap.backing=primary.data();
  heap.lights=lights.get();heap.cameraRoad=camera->roads();heap.shadows=camera->shadows()->shadow();
  heap.current=[](void* p){return static_cast<State*>(p)->baseCurrent();};
  heap.processAllocate=[](void* p,uint32_t size,uint32_t& out){return static_cast<State*>(p)->allocate(size,out);};
  heap.flushProcess=[](void* p,uint8_t* backing,uint32_t size){return static_cast<State*>(p)->flush(backing,size);};
  heap.flushBlock=[](void* p,uint32_t address){auto& s=*static_cast<State*>(p);s.vm().platform.flushBlock(s.vm().platform.user,address);};
  heap.systemCall=[](void* p){auto& s=*static_cast<State*>(p);s.vm().platform.systemCall(s.vm().platform.user);};
  heap.sdk=[](void* p,FieldMapSourceCall& q){return static_cast<State*>(p)->child(q);};
  maps=FieldMapSourceOwner::bind(vm().memory,0,MapStack,heap);
  return bool(maps);
 }
};
NativeMapProcess::NativeMapProcess()=default;NativeMapProcess::~NativeMapProcess()=default;
std::shared_ptr<NativeMapProcess> NativeMapProcess::create(std::shared_ptr<NativeCameraProcess> camera,const char* root,NativeMapCpuServices cpu,Estimate estimate){
 if(!camera||!camera->ready()||camera->busy()||!root||!cpu.current||!cpu.current()||!cpu.waitDrawSync||!cpu.flushBacking||!estimate)return {};
 auto s=std::make_unique<State>();s->camera=std::move(camera);s->cpu=std::move(cpu);s->estimate=std::move(estimate);auto& m=s->vm().memory;
 if(!s->vm().platform.flushBlock||!s->vm().platform.systemCall||!m.readWord(SdkRoot,s->descriptors))return {};
 for(unsigned i=0;i<s->heaps.size();++i){auto& h=s->heaps[i];
  if(!m.readWord(HeapHandles+4*i,h.handle)||!m.readWord(HeapStarts+4*i,h.start)||!m.readWord(HeapEnds+4*i,h.end)||h.end<=h.start+HeaderBytes)return {};
 }
 if(!m.readWord(FieldMapSource::PrimaryHeap,s->pool)||!m.readWord(PrimaryLength,s->bytes)||s->bytes!=s->heaps[1].end-s->heaps[1].start-HeaderBytes||!s->baseCurrent())return {};
 // Cold primary-pool admission is an identity check, not pool initialization.
 uint32_t next=0,free=0;uint16_t used=0;
 if(!m.readWord(s->pool+MapNext,next)||next||!m.readWord(s->pool+MapFreeBytes,free)||free!=s->bytes-HeaderBytes||!m.readHalf(s->pool+MapUsed,used)||used||!s->install(root))return {};
 s->sdk=std::make_unique<FieldMapSdk>(m,[state=s.get()]{return state->sdkActive||state->current();},
  [state=s.get()](float value){return state->estimateValue(value);});
 auto n=std::shared_ptr<NativeMapProcess>(new NativeMapProcess);n->state_=std::move(s);return n;
}
bool NativeMapProcess::current()const{return state_&&state_->current();}
bool NativeMapProcess::initialized()const{
 if(!current()||!state_->initDone||!state_->maps->workAddress()||!state_->lights->bankAddress(0)||!state_->lights->bankAddress(1))return false;
 uint32_t pointer=0;return state_->vm().memory.readWord(FieldMapSource::Work,pointer)&&pointer==state_->maps->workAddress();
}
bool NativeMapProcess::initialize(unsigned visits){
 if(invoking_||!current())return false;
 Guard guard(invoking_);auto& s=*state_;s.starting=true;
 if(s.initDone)return initialized();
 if(!s.lightsDone){if(!s.lights->initialize(s.lightInit))return false;s.lightsDone=true;}
 if(!s.lights->bankAddress(0)||!s.lights->bankAddress(1))return false;
 if(!s.maps&&!s.bindMap())return false;
 if(!s.maps->invoke(s.mapInit,FieldMapSource::Init,{},visits))return false;
 s.initDone=true;return initialized();
}
bool NativeMapProcess::loadHarbor(unsigned visits){
 if(invoking_||!initialized())return false;
 if(state_->loadDone)return loaded();
 state_->loading=true;return resume(visits);
}
bool NativeMapProcess::resume(unsigned visits){
 if(invoking_||!current())return false;
 if(!state_->initDone)return initialize(visits);
 Guard guard(invoking_);auto& s=*state_;
 s.held.reset();
 // The original cooperative DVD worker also retires queued closes after the
 // map parent moves on. This work never advances the simulation clock.
 if(!s.files->pump(visits))return false;
 if(!s.loading)return true;
 if(!s.maps->invoke(s.mapLoad,FieldMapSource::Load,{Input},visits))return false;
 s.loading=false;s.loadDone=true;return loaded();
}
bool NativeMapProcess::busy()const{return state_&&((state_->starting&&!state_->initDone)||state_->loading||(state_->maps&&state_->maps->pending())||state_->lights->pending()||state_->files->pending());}
bool NativeMapProcess::loaded()const{
 if(!initialized()||!state_->loadDone)return false;
 uint32_t count=0;return state_->vm().memory.readWord(state_->maps->workAddress(),count)&&count==1;
}
uint32_t NativeMapProcess::pendingInstruction()const{return state_&&state_->maps?state_->maps->pendingInstruction():0;}
std::optional<FieldMapSourceCall> NativeMapProcess::pendingCall()const{return state_?state_->held:std::nullopt;}
NativeMapProcess::Counts NativeMapProcess::counts()const{return state_?state_->counts:Counts{};}
uint32_t NativeMapProcess::workAddress()const{return state_&&state_->maps?state_->maps->workAddress():0;}
uint32_t NativeMapProcess::poolAddress()const{return state_?state_->pool:0;}
uint32_t NativeMapProcess::poolBytes()const{return state_?state_->bytes:0;}
std::shared_ptr<NativeCameraProcess> NativeMapProcess::cameras()const{return state_?state_->camera:nullptr;}
}
