#include "animation_startup_process.hpp"
#include "event_memory_bootstrap.hpp"
#include <algorithm>
#include <cassert>
namespace ttyd {
namespace {
constexpr uint32_t Work=0x803d9470;
struct Span{uint32_t address,size;};
// Exact source data/BSS image layout; no executable segments or arena buffers.
constexpr Span ImageSpans[]={{0x802c1070,8},{0x802c11d8,16},{0x802c11e8,16},
 {0x80305900,0xc00},{Work,0x110},{0x803d9580,178*4},{0x804151b8,40},
 {0x80415da0,16},{0x80415db4,4},{0x80415dc0,4},{0x8041e428,12},
 {0x8041e808,80},{0x8041f070,20},{0x8041fbb4,4}};
// Build-time extraction pins every span hash; runtime checks layout and critical fields.
bool admittedImage(const EventScriptAssets& image){
 if(image.segments().size()!=sizeof(ImageSpans)/sizeof(*ImageSpans))return false;
 for(unsigned i=0;i<image.segments().size();++i)if(image.segments()[i].address!=ImageSpans[i].address || image.segments()[i].size!=ImageSpans[i].size)return false;
 auto word=[&](uint32_t a,uint32_t expected){uint32_t v=0;return image.readWord(a,v) && v==expected;};
 if(!word(0x80415dc0,Work) || !word(0x80415db4,0x80305900) || !word(0x8041fbb4,0x3c8efa33) || !word(0x802c1070,0x43300000) || !word(0x802c1074,0x80000000))return false;
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
std::unique_ptr<AnimationStartupProcess> AnimationStartupProcess::cold(std::shared_ptr<NativeFileCache> files,std::shared_ptr<BattlePosePool> pool,const EventNativePlatform& platform,const char* path,uint32_t gp,uint32_t arena,uint32_t bytes){
 // Explicit native arena policy: aligned, nonwrapping, room for all four source
 // fixed pools, descriptor reservation and a valid final free-list header.
 constexpr uint32_t minimum=11124*1024+128;
 if(!files || !pool || !platform.flushBlock || !platform.systemCall || !gp || uint64_t(gp)+EventGpAddresses::size>0x100000000ULL || (gp<=0x80415fd0 && uint64_t(gp)+EventGpAddresses::size>0x80415fd0) || !arena || (arena&31) || (bytes&31) || bytes<minimum || uint64_t(arena)+bytes>0xffffffffULL)return {};
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
 case 0x800300f0:
  if(c.args[0]!=0)return false;
  c.result=eventHeapAllocate(0,c.args[1],vm_->memoryBinding.services());return true;
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
bool AnimationStartupProcess::advance(){
 if(aliases_)return aliases_->matchesWork();
 if(!initialized_){auto s=services();if(!initializeAnimation(s,cursor_))return false;initialized_=true;}
 // Do not rerun the completed source parent if alias admission is unavailable.
 aliases_=AnimationPoolMemory::bind(pool_,vm_->memory);return bool(aliases_);
}
}
