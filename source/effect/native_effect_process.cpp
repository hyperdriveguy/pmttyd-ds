#include "native_effect_process.hpp"
#include "native_effect_textures.hpp"
#include "native_ui_startup.hpp"
#include "event_heap_allocation.hpp"
#include "event_memory_release.hpp"
#include <array>
#include <string>
namespace ttyd {namespace {
struct Span {uint32_t address,bytes;uint64_t hash;};
// Original cold bytes/BSS. FNV1a64 is an admission fingerprint, not a license
// or runtime generation. Shared DVD root8041f494 is already owned by UI startup.
constexpr Span Spans[]={{0x802c1f20,24,0x03f486a12db9f9a7ULL},{0x80415ee0,4,0x14622da97729ab73ULL},{0x80415ee4,28,0x8de0efae20fc54eeULL},{0x80419a40,4,0x146cb7a97732ea76ULL},{0x8041e908,24,0x81d23fd7003c2305ULL},{0x8041ecd0,8,0xa8c7f832281a39c5ULL},{0x80420180,28,0xf5af84e45fe175a2ULL}};
constexpr uint32_t Root=0x8041f494,Format=0x802c1f20,Init=0x8005ef20,Main=0x8005ea7c,Release=0x8005ecd0;
constexpr uint32_t HeapAllocate=0x800300f0,HeapFree=0x800300c4,Clear=0x800050b4;
constexpr uint32_t Async=0x8018a0e8,Allocate=0x8018a61c,FileFree=0x8018a260,Formatting=0x8026a0b8;
constexpr uint32_t TextureType=4,StringCapacity=1024;
struct Guard {bool& flag;explicit Guard(bool& f):flag(f){flag=true;}~Guard(){flag=false;}};
bool same(const NativeEffectUpdate& a,const NativeEffectUpdate& b){return a.user==b.user && a.current==b.current && a.update==b.update && a.lifetime==b.lifetime && !a.lifetime.owner_before(b.lifetime) && !b.lifetime.owner_before(a.lifetime);}
bool same(const NativeEffectRetirement& a,const NativeEffectRetirement& b){return a.user==b.user && a.quiescent==b.quiescent && a.lifetime==b.lifetime && !a.lifetime.owner_before(b.lifetime) && !b.lifetime.owner_before(a.lifetime);}
}
struct NativeEffectProcess::State {
 std::shared_ptr<NativeUiStartup> ui;std::shared_ptr<NativeFieldAnimation> process;
 std::shared_ptr<NativeFileCache> files;std::unique_ptr<NativeEffectTextures>textures,environment,fade;std::string root;
 // Nonexecuting alias proof over the SAME process registries, not another VM.
 std::unique_ptr<EventAddressSpace> identity;bool alive=true,initialized=false;
 EffectInitCursor init;EffectMainCursor main;EffectReleaseCursor release;
 const void* driverOwner=nullptr;uint64_t visit=0;bool visited=false,visiting=false,releasing=false;
 uint32_t context=0,pending=0;NativeEffectUpdate backend;NativeEffectRetirement retirement;
 enum class FilePhase {Idle,Format,Request};FilePhase filePhase=FilePhase::Idle;
 bool asynchronous=false;std::array<uint32_t,6> fileArgs{};std::string filePath;
 EventNativeRuntime& vm()const{return process->process().vm();}
 bool current()const{
  if(textures && !textures->current())return false;
  if(environment&&!environment->current())return false;
  if(fade&&!fade->current())return false;
  if(!alive || !ui || !ui->current() || ui->fieldProcess()!=process || !process->ready() || !files->aliases(vm().memory))return false;
  if(!identity || !vm().memory.sharesRamWith(*identity,Root,4))return false;
  for(const auto& s:Spans)if(!vm().memory.sharesRamWith(*identity,s.address,s.bytes))return false;
  return true;
 }
 // Finite native C-runtime path adapter for the exact original three-%s
 // callsite. Strings are read from current mapped process memory, separately
 // for the repeated prefix argument. Never substitute a language or host cwd.
 bool text(uint32_t at,std::string& out){
  std::string value;
  for(uint32_t i=0;i<StringCapacity;++i){uint8_t b=0;if(uint64_t(at)+i>UINT32_MAX || !vm().memory.readByte(at+i,b))return false;if(!b){out=std::move(value);return true;}value+=char(b);}
  return false;
 }
 std::optional<uint32_t> file(bool async,const std::array<uint32_t,6>& args){
  if(filePhase!=FilePhase::Idle && (async!=asynchronous || args!=fileArgs))return {};
  if(filePhase==FilePhase::Idle){
   if(args[0]!=TextureType || args[1]!=0 || args[2]!=Format || args[3]!=Root)return {};
   // Resource type4 is not the shared archive-mode field. The original
   // formatted wrappers inherit that field without changing it.
   asynchronous=async;fileArgs=args;filePhase=FilePhase::Format;
  }
  if(filePhase==FilePhase::Format){
   pending=Formatting;std::string format,root,left,right;
   if(!text(args[2],format) || format!="%s/e/%s/effect_%s.tpl" || !text(args[3],root) || !text(args[4],left) || !text(args[5],right))return {};
   auto path=root+"/e/"+left+"/effect_"+right+".tpl";
   if(path.size()>=StringCapacity)return {};
   if(!files->publishFilename(path))return {};
   filePath=std::move(path);filePhase=FilePhase::Request;
  }
  pending=async?Async:Allocate;
  const auto result=async?files->request(filePath,args[0],args[1],true):files->acquire(filePath,args[0],true);
  if(!result)return {};
  filePhase=FilePhase::Idle;filePath.clear();return *result;
 }
};
NativeEffectProcess::NativeEffectProcess()=default;
NativeEffectProcess::~NativeEffectProcess()=default;
std::shared_ptr<NativeEffectProcess> NativeEffectProcess::create(std::shared_ptr<NativeUiStartup> ui,const char* root){
 if(!ui || !root || !ui->current())return {};
 auto process=ui->fieldProcess();if(!process || !process->ready())return {};
 auto files=process->files();if(!files || !files->aliases(process->process().vm().memory) || !files->contains("./e/us/effect_us.tpl",TextureType))return {};
 // Borrow the established source root; equality alone does not authorize a
 // second mapping. Keep the actual UI/process owners alive for every consumer.
 const auto dvd=ui->word(Root);if(!dvd || *dvd!=0x2e000000)return {};
 EventScriptAssets image;if(!image.load((std::string(root)+"/effect-process/memory.pmevt").c_str()) || image.segments().size()!=std::size(Spans))return {};
 auto& memory=process->process().vm().memory;
 for(unsigned i=0;i<std::size(Spans);++i){
  const auto& a=Spans[i];const auto& b=image.segments()[i];if(a.address!=b.address || a.bytes!=b.size || !memory.unmappedRange(a.address,a.bytes))return {};
  uint64_t hash=14695981039346656037ULL;
  for(uint32_t n=0;n<a.bytes;++n){uint8_t v=0;if(!image.readByte(a.address+n,v))return {};hash=(hash^v)*1099511628211ULL;}
  if(hash!=a.hash)return {};
 }
 auto result=std::shared_ptr<NativeEffectProcess>(new NativeEffectProcess);result->state_=std::make_unique<State>();
 auto& s=*result->state_;s.ui=std::move(ui);s.process=std::move(process);s.files=std::move(files);s.root=root;
 if(!memory.importScripts(image))return {};
 auto& vm=s.vm();s.identity=std::make_unique<EventAddressSpace>(vm.entryAddresses,vm.contextAddresses,vm.runtimeAddresses,vm.gpAddresses);
 for(const auto& range:Spans)if(!memory.shareRamTo(*s.identity,range.address,range.bytes))return {};
 if(!memory.shareRamTo(*s.identity,Root,4))return {};
 // Successful cold RAM is permanent source process history. Destruction does
 // not remove it or permit another Init to abandon the old record allocation.
 return result;
}
bool NativeEffectProcess::current()const{return state_ && state_->current();}
bool NativeEffectProcess::poolReady()const{return current() && state_->initialized;}
uint32_t NativeEffectProcess::pending()const{if(state_&&state_->fade&&state_->fade->pending())return state_->fade->pending();if(state_&&state_->environment&&state_->environment->pending())return state_->environment->pending();return state_?(state_->textures&&state_->textures->pending()?state_->textures->pending():state_->pending):Init;}
bool NativeEffectProcess::mainPending()const{return state_ && state_->visiting;}
bool NativeEffectProcess::busy()const{return state_ && (state_->init.active || state_->visiting || state_->releasing || (state_->textures&&state_->textures->busy()) || (state_->environment&&state_->environment->busy()) || (state_->fade&&state_->fade->busy()));}
std::shared_ptr<NativeUiStartup> NativeEffectProcess::ui()const{return state_?state_->ui:nullptr;}
EffectPoolServices NativeEffectProcess::services(){
 EffectPoolServices s;s.memory=state_->vm().memoryBinding.services();s.user=state_.get();
 s.heapAllocate=[](void* p,int32_t heap,uint32_t bytes)->std::optional<uint32_t>{auto& c=*static_cast<State*>(p);c.pending=HeapAllocate;if(!c.current())return {};return c.vm().memoryBinding.allocateHeap(heap,bytes);};
 s.clear=[](void* p,uint32_t at,uint32_t bytes){auto& c=*static_cast<State*>(p);c.pending=Clear;if(!c.current())return false;eventFillMemory(at,0,bytes,c.vm().memoryBinding.services());return true;};
 s.heapFree=[](void* p,int32_t heap,uint32_t at){auto& c=*static_cast<State*>(p);c.pending=HeapFree;if(!c.current())return false;eventHeapFree(heap,at,c.vm().memoryBinding.services());return true;};
 s.update=[](void* p,uint32_t callback,uint32_t effect){auto& c=*static_cast<State*>(p);c.pending=callback;const auto& b=c.backend;return c.current() && b.lifetime && b.current && b.update && b.current(b.user) && b.update(b.user,callback,effect);};
 s.fileFree=[](void* p,uint32_t handle){auto& c=*static_cast<State*>(p);c.pending=FileFree;return c.current() && c.files->release(handle);};
 s.fileAsync=[](void* p,int32_t type,uint32_t completion,uint32_t format,uint32_t root,uint32_t left,uint32_t right)->std::optional<int32_t>{auto& c=*static_cast<State*>(p);if(!c.current())return {};const auto n=c.file(true,{uint32_t(type),completion,format,root,left,right});if(!n)return {};return *n<=0x7fffffff?int32_t(*n):int32_t(int64_t(*n)-0x100000000LL);};
 s.fileAllocate=[](void* p,int32_t type,uint32_t format,uint32_t root,uint32_t left,uint32_t right)->std::optional<uint32_t>{auto& c=*static_cast<State*>(p);if(!c.current())return {};return c.file(false,{uint32_t(type),0,format,root,left,right});};
 return s;
}
EffectPoolServices NativeEffectProcess::constructionServices(){return poolReady()?services():EffectPoolServices{};}
bool NativeEffectProcess::initialize(){
 if(invoking_ || !current())return false;
 Guard guard(invoking_);auto& c=*state_;
 if(c.initialized)return true;
 c.pending=Init;if(!resumeEffectInit(services(),c.init))return false;
 c.initialized=true;c.pending=0;return true;
}
bool NativeEffectProcess::main(const void* owner,uint64_t visit,const NativeEffectUpdate& backend){
 if(invoking_ || !poolReady() || !owner || !visit)return false;
 Guard guard(invoking_);auto& c=*state_;
 if(c.releasing || (c.driverOwner && c.driverOwner!=owner))return false;
 if(c.visiting){if(c.visit!=visit || !same(c.backend,backend))return false;}
 else{
  if(c.visited && visit<=c.visit)return visit==c.visit && same(c.backend,backend);
  c.driverOwner=owner;c.visit=visit;c.visited=c.visiting=true;c.backend=backend;
 }
 c.pending=Main;if(!resumeEffectMain(services(),c.main))return false;
 c.visiting=false;c.pending=0;return true;
}
bool NativeEffectProcess::initializeFade(){
 if(invoking_||!current())return false;Guard guard(invoking_);auto&s=*state_;
 if(s.visiting||s.releasing||s.init.active)return false;
 if(!s.fade){s.fade=NativeSourceTextureStartup::createFade(s.ui,s.root.c_str());if(!s.fade)return false;}
 return s.fade->initializeFade();
}
bool NativeEffectProcess::beginFadeTextures(){
 if(invoking_||!current()||!state_->fade)return false;Guard guard(invoking_);
 if(state_->visiting||state_->releasing)return false;return state_->fade->begin();
}
bool NativeEffectProcess::pumpFadeTextures(uint32_t bytes){
 if(invoking_||!current()||!state_->fade)return false;Guard guard(invoking_);return state_->fade->pump(bytes);
}
bool NativeEffectProcess::fadeTexturesReady()const{return current()&&state_->fade&&state_->fade->ready();}
bool NativeEffectProcess::initializeEnvironment(){
 if(invoking_||!poolReady())return false;Guard guard(invoking_);auto&s=*state_;
 if(s.visiting||s.releasing)return false;
 if(!s.environment){s.environment=NativeSourceTextureStartup::createEnvironment(s.ui,s.root.c_str());if(!s.environment)return false;}
 return s.environment->initializeEnvironment();
}
bool NativeEffectProcess::beginEnvironmentTextures(){
 if(invoking_||!poolReady()||!state_->environment)return false;Guard guard(invoking_);
 if(state_->visiting||state_->releasing)return false;return state_->environment->begin();
}
bool NativeEffectProcess::pumpEnvironmentTextures(uint32_t bytes){
 if(invoking_||!poolReady()||!state_->environment)return false;Guard guard(invoking_);return state_->environment->pump(bytes);
}
bool NativeEffectProcess::environmentTexturesReady()const{return current()&&state_->environment&&state_->environment->ready();}
bool NativeEffectProcess::beginTextures(){
 if(invoking_||!poolReady())return false;Guard guard(invoking_);auto&s=*state_;
 if(s.visiting||s.releasing)return false;
 if(!s.textures){s.textures=NativeEffectTextures::create(s.ui,s.root.c_str());if(!s.textures)return false;}
 return s.textures->begin();
}
bool NativeEffectProcess::pumpTextures(uint32_t bytes){
 if(invoking_||!poolReady()||!state_->textures)return false;Guard guard(invoking_);return state_->textures->pump(bytes);
}
bool NativeEffectProcess::texturesReady()const{return current()&&state_->textures&&state_->textures->ready();}
bool NativeEffectProcess::pumpFiles(uint32_t bytes){
 if(invoking_ || !current())return false;
 Guard guard(invoking_);return state_->files->advanceAsync(bytes);
}
bool NativeEffectProcess::autoRelease(uint32_t context,const NativeEffectRetirement& retirement){
 if(invoking_ || !poolReady())return false;
 Guard guard(invoking_);auto& c=*state_;
 if(c.visiting)return false;
 if(c.releasing){if(c.context!=context || !same(c.retirement,retirement))return false;}
 else{c.context=context;c.retirement=retirement;c.releasing=true;}
 c.pending=Release;
 if(!retirement.lifetime || !retirement.quiescent || !retirement.quiescent(retirement.user) || !current())return false;
 if(!resumeEffectAutoRelease(context,services(),c.release))return false;
 c.releasing=false;c.pending=0;return true;
}
bool NativeEffectProcess::retire(){if(invoking_)return false;
 Guard guard(invoking_);if(state_)state_->alive=false;return true;}
}
