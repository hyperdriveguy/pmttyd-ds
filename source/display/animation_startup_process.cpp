#include "animation_startup_process.hpp"
#include "event_memory_bootstrap.hpp"
#include "field_npc_clock.hpp"
#include <algorithm>
#include <cassert>
namespace ttyd {
namespace {
// US cold W at803d9470 spans110hex bytes,followed by 178 binary32 tangents
// at803d9580. This is bootstrap source storage,not an existing live allocation.
constexpr uint32_t Work=0x803d9470;
struct Span{uint32_t address,size;};
// Exact source data/BSS image layout; no executable segments or arena buffers.
// Ordered address/byte-size allowlist: render seeds/formats/warning/sort names,
// normal Mario tokens,initialized Vivian destination,Joint/Transform tables,
// W/tangents and roots,heap config/arena globals,Entry/facing/Main literals,
// party token and SDK ONE/ZERO. Initialized DOL bytes override broad BSS zeros.
// Keep synchronized with tools/animation_boot_image.py; hashes belong to that
// read-only extractor,while this runtime verifies layout and critical words.
constexpr Span ImageSpans[]={{0x802c103c,12},{0x802c1060,8},{0x802c1068,8},{0x802c1070,8},{0x802c1078,76},{0x802c10c4,220},{0x802c11a8,48},{0x802c11d8,16},{0x802c11e8,16},{0x802c18a0,28},
 {0x80305900,0xc00},{0x80306678,32},{0x80306698,0x40},{0x803066d8,0x20},{Work,0x110},{0x803d9580,178*4},{0x804151b8,40},
 {0x80415da0,16},{0x80415db4,4},{0x80415dc0,4},{0x80415dc4,4},{0x80415dc8,16},{0x80415eb0,12},{0x8041e428,12},{0x8041e458,8},
 {0x8041e808,80},{0x8041f070,20},{0x8041fb0c,8},{0x8041fb24,8},{0x8041fb2c,4},{0x8041fb34,12},{0x8041fb40,4},{0x8041fb44,4},{0x8041fb64,4},{0x8041fb68,4},{0x8041fb6c,0x2c},{0x8041fb98,4},{0x8041fba4,8},{0x8041fbac,4},{0x8041fbb0,4},{0x8041fbb4,4},{0x804213e0,8},{0x80429358,8}};
// Build-time extraction pins every span hash; runtime checks layout and critical fields.
bool admittedImage(const EventScriptAssets& image){
 if(image.segments().size()!=sizeof(ImageSpans)/sizeof(*ImageSpans))return false;
 for(unsigned i=0;i<image.segments().size();++i)if(image.segments()[i].address!=ImageSpans[i].address || image.segments()[i].size!=ImageSpans[i].size)return false;
 auto word=[&](uint32_t a,uint32_t expected){uint32_t v=0;return image.readWord(a,v) && v==expected;};
 // Critical raw float words:2,-2,3,1/16,60,1000,draw degrees->radians,
 // W/Vivian roots,init factor3c8efa33 (distinct from draw3c8efa35),signed
 // integer bias4330000080000000,and frame->ms16.666666 atfb40.
 if(!word(0x802c1060,0x43300000) || !word(0x802c1064,0) || !word(0x8041fb44,0x3f000000) || !word(0x8041fb68,0xbf000000) || !word(0x80306678,0x8003e178) || !word(0x80306694,0x8003e1f8))return false;
 if(!word(0x8041fb64,0x40000000) || !word(0x8041fb98,0xc0000000) || !word(0x8041fbb0,0x40400000) || !word(0x8041fbac,0x3d800000) || !word(0x8041fba4,0x42700000) || !word(0x8041fba8,0x447a0000) || !word(0x8041fb2c,0x3c8efa35) || !word(0x80415dc0,Work) || !word(0x80415db4,0x80305900) || !word(0x8041fbb4,0x3c8efa33) || !word(0x802c1070,0x43300000) || !word(0x802c1074,0x80000000) || !word(0x8041fb40,0x41855555))return false;
 // Five(enabled,sizeKiB) heap rows at804151b8. First four enabled; last
 // 100KiB row disabled. Work+tangent cold zero check excludes initialized Vivian.
 const uint32_t values[]={7300,3472,224,128,100};
 for(unsigned i=0;i<5;++i)if(!word(0x804151b8+8*i,i==4?0:1) || !word(0x804151bc+8*i,values[i]))return false;
 for(uint32_t a=Work;a<Work+0x110+178*4;a+=4)if(!word(a,0))return false;
 for(auto pair:{std::make_pair(0x802c11d8u,"a/vivian.bin"),std::make_pair(0x802c11e8u,"a/ag2tg.bin")}){
  for(unsigned i=0;;++i){uint8_t v=0;if(!image.readByte(pair.first+i,v) || v!=uint8_t(pair.second[i]))return false;if(!v)break;}
 }
 return true;
}
}
AnimationStartupProcess::AnimationStartupProcess(std::shared_ptr<NativeFileCache> files,std::shared_ptr<BattlePosePool> pool,const EventNativePlatform& platform,uint32_t gp):pool_(std::move(pool)),files_(std::move(files)),vm_(std::make_unique<EventNativeRuntime>(gp,platform)){}
AnimationStartupProcess::~AnimationStartupProcess(){selection_.reset();presentation_.reset();main_.reset();if(scratch_)scratch_->retire();}
std::unique_ptr<AnimationStartupProcess> AnimationStartupProcess::cold(std::shared_ptr<NativeFileCache> files,std::shared_ptr<BattlePosePool> pool,const EventNativePlatform& platform,const char* path,uint32_t gp,uint32_t arena,uint32_t bytes){
 // Explicit native arena policy: aligned, nonwrapping, room for all four source
 // fixed pools, descriptor reservation and a valid final free-list header.
 // Native minimum: enabled7300+3472+224+128KiB=11124KiB,plus 128 bytes
 // descriptor/final-free-header allowance. 32-byte alignment is allocator policy;
 // these are admission bounds,not measured hardware memory consumption.
 constexpr uint32_t minimum=11124*1024+128;
 if(!files || !pool || !platform.flushBlock || !platform.systemCall || !gp || uint64_t(gp)+EventGpAddresses::size>0x100000000ULL || (gp<=0x80415fd0 && uint64_t(gp)+EventGpAddresses::size>0x80415fd0) || !arena || (arena&31) || (bytes&31) || bytes<minimum || uint64_t(arena)+bytes>0xffffffffULL)return {};
 // Cold-only admission rejects ANY preexisting source/typed state. Never
 // erase a live shared pool or re-import the image over edited runtime storage.
 if(pool->workEstablished_ || pool->sourceGuard_ || !pool->cacheMetadataReady() || std::any_of(pool->work_.begin(),pool->work_.end(),[](uint8_t b){return b!=0;}))return {};
 for(const auto& m:pool->models_)if(std::any_of(m.state.begin(),m.state.end(),[](uint8_t b){return b!=0;}))return {};
 for(const auto& t:pool->textures_)if(std::any_of(t.state.begin(),t.state.end(),[](uint8_t b){return b!=0;}))return {};
 for(const auto& p:pool->poses_)if(p && std::any_of(p->state.begin(),p->state.end(),[](uint8_t b){return b!=0;}))return {};
 if(!files->contains("a/vivian.bin",0) || !files->contains("a/ag2tg.bin",0))return {};
 EventScriptAssets image;if(!image.load(path) || !admittedImage(image))return {};
 auto result=std::unique_ptr<AnimationStartupProcess>(new AnimationStartupProcess(std::move(files),std::move(pool),platform,gp));auto& vm=*result->vm_;
 if(!vm.memory.importScripts(image) || !vm.memory.registerRam(arena,std::vector<uint8_t>(bytes)) || !result->files_->bind(vm.memory))return {};
 // These are caller/native runtime bounds, deliberately replacing source's
 // pre-OS initialization sentinels before the recovered bootstrap reads them.
 const bool bounds=vm.memory.writeWord(0x8041e430,arena) && vm.memory.writeWord(0x8041f080,arena+bytes);assert(bounds);(void)bounds;
 initializeEventMemoryPools(vm.memoryBinding.services());return result;
}
AnimationInitializeServices AnimationStartupProcess::services(){
 AnimationInitializeServices s;s.memory.user=this;
 s.memory.word=[](void* p,uint32_t a)->std::optional<uint32_t>{uint32_t v=0;return static_cast<AnimationStartupProcess*>(p)->vm_->memory.readWord(a,v)?std::optional<uint32_t>(v):std::nullopt;};
 s.memory.store=[](void* p,uint32_t a,uint32_t v){return static_cast<AnimationStartupProcess*>(p)->vm_->memory.writeWord(a,v);};
 s.memory.call=[](void* p,BattleSequenceCall& c){return static_cast<AnimationStartupProcess*>(p)->child(c);};
 s.doubleWord=[](void* p,uint32_t a)->std::optional<uint64_t>{auto& m=static_cast<AnimationStartupProcess*>(p)->vm_->memory;uint32_t hi=0,lo=0;return m.readWord(a,hi) && m.readWord(a+4,lo)?std::optional<uint64_t>(uint64_t(hi)<<32|lo):std::nullopt;};
 s.tangent=nativeAnimationTangent;return s;
}
bool AnimationStartupProcess::child(BattleSequenceCall& c){
 switch(c.function){
 // __memAlloc(heap0,size) uses the SAME event SDK heap owner/headers,
 // including real zero-fill/cache services; no invented bump-pointer result.
 case 0x800300f0:
  if(c.args[0]!=0)return false;
  c.result=vm_->memoryBinding.allocateHeap(0,c.args[1]);return true;
 case 0x8018a61c:{
  // The two cold literal-only vsprintf requests need no varargs conversion.
  // The cache exposes the actual source shared filename at803e6e78. Generic
  // formatted/interleaved wrappers remain outside this admitted cold domain.
  if(c.args[0]!=0 || (c.args[1]!=0x802c11d8 && c.args[1]!=0x802c11e8))return false;
  if(!formatted_){
   std::string path;bool terminated=false;
   for(unsigned i=0;i<16;++i){uint8_t v=0;if(!vm_->memory.readByte(c.args[1]+i,v) || v=='%')return false;if(!v){terminated=true;break;}path+=char(v);}
   if(!terminated)return false;
   filename_=std::move(path);formatAddress_=c.args[1];formatted_=true;
  }
  if(formatAddress_!=c.args[1])return false;
  auto h=files_->acquire(filename_,0);if(!h)return false;
  c.result=*h;formatted_=false;filename_.clear();return true;
 }
 case 0x8018a260:return files_->release(c.args[0]);
 case 0x80189d38:return files_->archiveType(c.args[0]);
 case 0x80005634:{
  const auto dst=c.args[0],src=c.args[1],n=c.args[2];
  if(!((dst==0x80305900 && n==0xc00) || (dst==0x80415da0 && n==16)) || (src&3) || !files_->ownsPayload(src,n))return false;
  // Exact positive memcpy_as4 domain: sequential word read/store pairs. No
  // callback wait or prefix replay; all ranges have stable owned backing.
  for(uint32_t o=0;o<n;o+=4){uint32_t v=0;const bool ok=vm_->memory.readWord(src+o,v) && vm_->memory.writeWord(dst+o,v);assert(ok);(void)ok;}return true;
 }
 default:return false;
 }
}
// Source initialization completes once,then pool/tangent/scratch aliasing can
// retry separately. A later attachment hold must not replay allocations/files.
bool AnimationStartupProcess::advance(){
 if(producerPending())return false;
 if(!initialized_){auto s=services();if(!initializeAnimation(s,cursor_))return false;initialized_=true;}
 // Completed source effects never replay while either alias admission waits.
 if(!aliases_)aliases_=AnimationPoolMemory::bind(pool_,vm_->memory);
 if(!aliases_ || !aliases_->matchesWork())return false;
 if(!tangentMemory_)tangentMemory_=AnimationTangentMemory::bind(pool_,vm_->memory);
 if(tangentMemory_ && tangentMemory_->current() && !scratch_)scratch_=AnimationScratchMemory::bind(pool_,files_,vm_->memory,*aliases_);
 return ready() && scratch_ && scratch_->current();
}
bool AnimationStartupProcess::text(uint32_t address,std::string& out) const{
 // Native filename/string admission. Core Entry itself has no such length cap.
 std::string s;for(unsigned i=0;i<256;++i){uint8_t b=0;if(!vm_->memory.readByte(address+i,b))return false;if(!b){out=std::move(s);return true;}s+=char(b);}return false;
}
bool AnimationStartupProcess::entryChild(AnimationEntryCall& c){
 switch(c.function){
 case 0x80189d38:return files_->archiveType(c.args[0]);
 case 0x8018a260:return files_->release(c.args[0]);
 // strcmp admits semantic stack arguments only in lanes0/1(mask3).
 // Filename formatting below is bounded native adaptation,not generic varargs.
 case 0x8026d464:{
  std::string a,b;
  if(c.stackArguments&~3u)return false;
  if(c.stackArguments&1)a=c.text;else if(!text(c.args[0],a))return false;
  if(c.stackArguments&2)b=c.text;else if(!text(c.args[1],b))return false;
  c.result=a==b?0:a<b?0xffffffff:1;return true;
 }
 case 0x8026a0b8:{
  std::string format,prefix,name;
  if(c.stackArguments!=1 || c.args[1]!=0x802c1068 || !text(c.args[1],format) || format!="%s/%s-" || !text(c.args[2],prefix) || !text(c.args[3],name))return false;
  c.text=prefix+"/"+name+"-";return c.text.size()<256;
 }
 // Capture completed MODEL(type5) or TPL(type4) filename before acquire/Yield.
 // Held file work never re-reads/reformats a changed source name on retry.
 case 0x8018a61c:
 case 0x8018a5f8:{
  const bool formatted=c.function==0x8018a61c;
  if(!entryFormatted_){
   if(formatted){
    std::string format,prefix,name;
    if(c.stackArguments || c.args[0]!=5 || c.args[1]!=0x8041fb34 || !text(c.args[1],format) || format!="%s/%s" || !text(c.args[2],prefix) || !text(c.args[3],name))return false;
    entryFilename_=prefix+"/"+name;
   }else{if(c.stackArguments!=1 || c.args[1]!=4)return false;entryFilename_=c.text;}
   entryFormatted_=true;
  }
  auto h=files_->acquire(entryFilename_,formatted?5:4);if(!h)return false;
  c.result=*h;entryFormatted_=false;entryFilename_.clear();return true;
 }
 // __div2i ABI is HIGH/LOW numerator and zero HIGH divisor; preserve raw
 // signed64 quotient HIGH in result,LOW separately. Divisor0 remains a hold.
 case 0x80264ad8:{
  uint64_t result=0;
  if(c.args[2] || !nativeFieldNpcClockDivide(nullptr,uint64_t(c.args[0])<<32|c.args[1],c.args[3],result))return false;
  c.result=uint32_t(result>>32);c.low=uint32_t(result);return true;
 }
 case 0x80041ee0:{auto s=services();return runAnimationBufferFunction(c.function,c.args[0],s.memory,entryBuffers_);}
 case 0x8029dd04:
  // Native identity leaf admits constant zero/one and stable pose RAM.
  // Preserve the source paired-store order as sequential word pairs; there
  // is no callback hold between these stores or mutable SDK constant owner.
  {constexpr uint32_t offsets[]={8,12,24,28,32,36,16,20,0,4,40,44};
   constexpr uint32_t values[]={0,0,0,0,0,0,0,0x3f800000,0x3f800000,0,0x3f800000,0};
   for(unsigned i=0;i<12;++i){const bool ok=vm_->memory.writeWord(c.args[0]+offsets[i],values[i]);assert(ok);(void)ok;}}
  return true;
 default:return false;
 }
}
// Post-source native admission only:256 pose/64 model/64 texture slots,
// positive signed refs,actual file names/counts and descriptor0 CPU geometry.
// Preparing sidecars is not a transaction/rollback of source Entry. Source BE
// state stays untouched; an adoption hold does not replay the completed parent.
bool AnimationStartupProcess::adoptEntry(){
 auto& m=vm_->memory;const auto h=entry_.result;
 if(h>=256 || !pool_->sourceArraysCurrent())return false;
 auto* p=pool_->poses_[h].get();if(!p || !p->word(0))return false;
 const auto mi=p->word(0x10);if(mi>=64)return false;
 auto& model=pool_->models_[mi];const auto ti=model.texture();if(ti>=64)return false;
 auto& texture=pool_->textures_[ti];auto resource=entryResource_;
 if(!resource || !resource->draw || !resource->textures || !resource->modelFlags || resource->names.empty() || !resource->bank || resource->discrete.size()!=resource->names.size() || !model.flags() || !model.references() || model.references()>0x7fffffff || !texture.flags() || !texture.references() || texture.references()>0x7fffffff || !model.file() || !texture.file() || (model.resource && model.resource!=resource) || (texture.resource && texture.name!=resource->textureName))return false;
 uint32_t holder=0,data=0,v=0;std::string name;
 if(!m.readWord(model.file()+0xa0,holder) || !m.readWord(holder,data) || !data || !text(data+4,name) || name!=resource->modelName || !text(data+0x44,name) || name!=resource->textureName || !text(texture.file()+0x20,name) || name!="a/"+resource->textureName+"-")return false;
 // Counts/flags stay invariant under original type5 relocation. Compare the
 // actual loaded MODEL with the admitted original converted resource metadata.
 for(auto offset:{0xc4u,0xf0u,0xf8u,0x12cu,0x13cu,0x140u,0x144u,0x148u})if(!m.readWord(data+offset,v) || v!=resource->draw->word(offset))return false;
 const auto* initial=resource->bank->pose(resource->names.front().c_str());ActorAnimationSample shape;ActorAnimationCache cache;
 if(!initial || !initial->evaluate(0,shape,pool_->tangents_.get()) || shape.positions.empty() || shape.nodes.empty() || shape.world.empty())return false;
 allocateActorAnimationCache(*initial,cache);
 // Sidecars only: never call typed enter/restart or overwrite source BE rows,
 // timestamp, refs, file identities, scratch pointers or untouched stale words.
 model.resource=resource;if(!texture.resource){texture.name=resource->textureName;texture.resource=resource;}
 // Native generation/archive metadata and presentation/cache sidecars follow
 // CPU preparation. Scratch-view admission below can still hold after these
 // native writes;do not describe adoption as atomic whole-sidecar publication.
 p->resource=std::move(resource);p->shape=std::move(shape);p->cache=std::move(cache);p->nativeGeneration=pool_->nextGeneration_++;p->archiveType=entry_.mode==1?2:entry_.mode==0?1:0;pool_->archiveType_=p->archiveType;
 if(!scratch_ || !scratch_->current())return false;
 p->sourceBuffers=scratch_->pose(h,p->nativeGeneration);
 return bool(p->sourceBuffers);
}
std::optional<int32_t> AnimationStartupProcess::enter(uint32_t name,uint32_t mode,std::shared_ptr<const BattlePoseResource> resource){
 if(!ready() || producerPending())return {};
 if(!entryResource_){
  std::string requested;
  if(mode>2 || !pool_->cacheMetadataReady() || !resource || !resource->draw || !resource->textures || !resource->bank || resource->names.empty() || !text(name,requested) || requested!=resource->modelName || !files_->contains("a/"+resource->modelName,5) || !files_->contains("a/"+resource->textureName+"-",4))return {};
  entryResource_=std::move(resource);entryComplete_=false;entry_={};entryBuffers_={};
 }else if(entryResource_!=resource || entry_.name!=name || entry_.mode!=mode)return {};
 if(!entryComplete_){
  auto cold=services();AnimationEntryServices s;s.memory=cold.memory;
  s.call=[](void* p,AnimationEntryCall& c){return static_cast<AnimationStartupProcess*>(p)->entryChild(c);};
  if(!enterAnimationPose(name,mode,s,entry_))return {};
  entryComplete_=true;
 }
 // Completed source-2 is a real model failure. Empty optional means hold;
 // source completion remains retained while native metadata adoption waits.
 if(entry_.result==0xfffffffe){entryResource_.reset();return -2;}
 if(!adoptEntry())return {};
 const auto result=int32_t(entry_.result);entryResource_.reset();return result;
}
}
