#include "native_ui_startup.hpp"
#include "native_source_resource_path_formatter.hpp"
#include "source_mario_dvd_root.hpp"
#include "event_heap_allocation.hpp"
#include <algorithm>
#include <array>
#include <cstdio>
#include <cstring>
#include <map>
#include <string>
namespace ttyd {namespace {
struct Span {uint32_t address,size;uint64_t hash;};
// Cold source image,address/byte length/FNV1a64 in ascending order:font
// literals,SDK formats,palette-spacing-name pointers,SDK filters,font globals,
// root string,draw scalars,texture scalars,fog and selected color. These hashes
// are admission pins,not licenses or generation IDs; exporter records SHA256 too.
constexpr Span Spans[]={{0x802c2b30,144,0xa7bc9710cf961686ULL},{0x803c4e84,60,0x58a395e7b5c6d4c0ULL},{0x80416010,68,0x0957b20c36b054385ULL},{0x8041e558,8,0x8263db999b8acaebULL},{0x8041e978,68,0xe7d16c6b24f5c8f5ULL},{0x8041f494,4,0x0daffda6dc58ed3bULL},{0x804203e4,96,0x38fa0130cc650a0eULL},{0x80429440,40,0x054efeb1b416e63eULL},{0x80429578,4,0x4d25767f9dce13f5ULL},{0x80429598,4,0x4d25767f9dce13f5ULL}};
struct Image {
 EventScriptAssets source;std::array<std::vector<uint8_t>,10>bytes;
 EventAddressSpace*installed=nullptr;bool retained=false;
 ~Image(){if(installed&&!retained)for(const auto&s:Spans)installed->unregisterRam(s.address);}
 // FNV1a64 offset14695981039346656037/prime1099511628211 over exact bytes.
 // Reject any address/extent/hash mismatch before installing cold source RAM.
 bool load(const char*path){
  if(!source.load(path)||source.segments().size()!=bytes.size())return false;
  for(unsigned i=0;i<bytes.size();++i){const auto&s=Spans[i];const auto&t=source.segments()[i];if(t.address!=s.address||t.size!=s.size)return false;uint64_t hash=14695981039346656037ULL;for(uint32_t n=0;n<s.size;++n){uint8_t v;if(!source.readByte(s.address+n,v))return false;bytes[i].push_back(v);hash=(hash^v)*1099511628211ULL;}if(hash!=s.hash)return false;}
  return true;
 }
 // All ten ranges must be unmapped. Roll back only newly registered ranges
 // on partial failure; successful factory keeps them in the process VM so
 // recreation cannot overwrite live source history.
 bool install(EventAddressSpace&vm){
  for(const auto&s:Spans)if(!vm.unmappedRange(s.address,s.size))return false;
  unsigned done=0;for(;done<bytes.size();++done)if(!vm.registerRam(Spans[done].address,bytes[done]))break;
  if(done!=bytes.size()){while(done)vm.unregisterRam(Spans[--done].address);return false;}installed=&vm;return true;
 }
 bool immutable(uint32_t a,uint32_t n)const{
  if(!n||uint64_t(a)+n>0x100000000ULL)return false;
  return (a>=0x802c2b30&&uint64_t(a)+n<=0x802c2bc0)||(a>=0x8041f494&&uint64_t(a)+n<=0x8041f498);
 }
};
//256 cold slot objects already exist. Require zero flags/resource/generation,
// not null object pointers; held Entry or Release excludes startup admission.
bool coldPrefix(NativeFieldAnimation&process){
 if(process.process().entryPending()||process.releasePending())return false;
 const auto pool=process.pool();if(!pool)return false;
 for(unsigned i=0;i<256;++i){const auto*p=pool->slot(i);if(p&&(p->word(0)||p->resource||p->nativeGeneration))return false;}
 return true;
}
std::string path(const char*root,const char*name){std::string p=root;if(!p.empty()&&p.back()!='/')p+='/';return p+name;}
uint32_t get(const std::array<uint8_t,152>&v,unsigned a,unsigned n){uint32_t out=0;for(unsigned i=0;i<n;++i)out=(out<<8)|v[a+i];return out;}
void set(std::array<uint8_t,152>&v,unsigned a,unsigned n,uint32_t x){for(unsigned i=0;i<n;++i)v[a+i]=uint8_t(x>>(8*(n-i-1)));}
}
struct NativeUiStartup::State {
 std::shared_ptr<NativeFieldAnimation>process;std::unique_ptr<Image>image;
 std::shared_ptr<const NativeSourceFontFile>file;std::string filePath;
 std::shared_ptr<NativeSourceFontMemory>memory=NativeSourceFontMemory::create();
 std::unique_ptr<NativeSourceFontTexture>texture;
 NativeSourceResourcePathFormatter formatter;SourceFontSetupCursor font;
 NativeUiStartupAdmission admission;Counts count;
 uint32_t gp=0,pending=0,phase=0;bool alive=true,invalid=false;
 const void*formatDestination=nullptr;
 // Finite native DVD records use152 bytes (98hex). Synthetic addresses are
 // reserved only after actual unmappedRange checks; they are not ISO pointers.
 struct Entry {uint32_t address=0;std::array<uint8_t,152>bytes{};FILE*file=nullptr;};
 std::map<uint32_t,std::unique_ptr<Entry>>entries;
 struct Heap {uint32_t base=0,bytes=0;std::shared_ptr<std::vector<uint8_t>>backing;bool detached=false,adopted=false;};
 std::optional<Heap>heap;
 struct Read {std::vector<uint32_t>arguments;std::vector<uint8_t>bytes;bool observed=false;};
 std::optional<Read>read;
 EventNativeRuntime&vm()const{return process->process().vm();}
 // Same actual GP pointer must survive. A nonzero global font pointer must
 // equal this captured heap; zero is not itself proof of completed font setup.
 // Observed mismatch poisons this owner instead of silently rebinding it.
 bool current(){
  if(!alive||invalid||!process||!process->ready())return false;
  uint32_t actual=0,fontPointer=0;
  if(!vm().memory.readWord(0x80415fd0,actual)||actual!=gp||!vm().memory.readWord(0x8041e978,fontPointer)||
     (fontPointer&&(!heap||fontPointer!=heap->base))){invalid=true;return false;}
  return true;
 }
 ~State(){
  alive=false;texture.reset();
  for(auto&e:entries){if(e.second->file)std::fclose(e.second->file);vm().memory.unregisterExternalRam(e.first);}
  // Source heap allocation remains source-owned; this native retirement does
  // not invent a guest free/reset or rerun cold loading in a surviving process.
 }
 bool gate(NativeUiStartupBoundary at,uint32_t a,uint32_t n){if(!current())return false;const bool ok=!admission.admit||admission.admit(admission.user,at,a,n);return current()&&ok;}
 std::optional<uint32_t>word(uint32_t a){if(!current())return {};if(memory->owns(a,4))return memory->word(a);uint32_t v;if(!vm().memory.readWord(a,v))return {};return v;}
 BattleActorServices actors(){
  BattleActorServices a;a.user=this;a.memory.user=this;
  a.byte=[](void*p,uint32_t at){auto&s=*static_cast<State*>(p);uint8_t v=0;const bool ok=s.vm().memory.readByte(at,v);if(!ok)s.invalid=true;return v;};
  a.memory.word=[](void*p,uint32_t at){auto&s=*static_cast<State*>(p);uint32_t v=0;if(!s.vm().memory.readWord(at,v))s.invalid=true;return v;};
  a.memory.half=[](void*p,uint32_t at){auto&s=*static_cast<State*>(p);uint16_t v=0;if(!s.vm().memory.readHalf(at,v))s.invalid=true;return v;};
  a.memory.storeWord=[](void*p,uint32_t at,uint32_t v){auto&s=*static_cast<State*>(p);if(!s.vm().memory.writeWord(at,v))s.invalid=true;};
  a.memory.storeByte=[](void*p,uint32_t at,uint8_t v){auto&s=*static_cast<State*>(p);if(!s.vm().memory.writeByte(at,v))s.invalid=true;};return a;
 }
 // Only the original literal/root spans are immutable formatter inputs.
 // Compare live bytes to imported originals and admit only exact256-byte
 // private Font destination; unknown names are not filesystem fallbacks.
 bool bindFormatter(){
  NativeResourcePathFormatterServices s;s.user=this;s.lifetime=process;s.current=[](void*p){return static_cast<State*>(p)->current();};
  s.byte=[](void*p,uint32_t a)->std::optional<uint8_t>{auto&s=*static_cast<State*>(p);uint8_t value;if(!s.image->source.readByte(a,value))return {};return value;};
  s.immutableInput=[](void*p,uint32_t a,uint32_t n){auto&s=*static_cast<State*>(p);if(!s.image->immutable(a,n))return false;for(uint32_t i=0;i<n;++i){uint8_t expected,actual;if(!s.image->source.readByte(a+i,expected)||!s.vm().memory.readByte(a+i,actual)||actual!=expected)return false;}return s.current();};
  s.privateDestination=[](void*p,NativeResourcePathKind kind,const void*output,uint32_t capacity){auto&s=*static_cast<State*>(p);return kind==NativeResourcePathKind::Font&&output&&output==s.formatDestination&&capacity==256&&s.current();};s.stringBudget=256;s.outputCapacity=256;return formatter.bind(std::move(s));
 }
 std::optional<std::string>text(const SourceFontPath*source){if(!source)return {};std::string out;for(unsigned i=0;i<256;++i){auto v=source->byte(i);if(!v)return {};if(!*v)return out;out.push_back(char(*v));}return {};}
 // Only exact source ./font probes may return real catalog-miss0; unknown
 // paths hold. Selected ./f/US opens a separate rb FILE for actual delivery;
 // immutable preflight bytes did not perform this source read.
 std::optional<uint32_t>open(const SourceFontSetupCall&q){
  if(q.integers!=std::vector<uint32_t>{2,0}||q.formatDestination||!q.reals.empty())return {};
  const auto name=text(q.path);if(!name)return {};
  // Actual readonly US FST has no font/ entries. These exact source probes
  // are genuine catalog misses, not a fallback for unknown/missing native data.
  if(*name=="./font/papermarioset_US.bfn"||*name=="./font/papermarioset_JPN.bfn"||*name=="./font/papermarioset_EU.bfn"){++count.opens;return 0;}
  if(*name!="./f/papermarioset_US.bfn")return {};
  uint32_t address=0;for(unsigned i=0;i<256;++i){const uint32_t candidate=0x60030000u+i*152;if(!entries.count(candidate)&&vm().memory.unmappedRange(candidate,152)){address=candidate;break;}}
  if(!address)return {};
  auto e=std::make_unique<Entry>();e->address=address;e->file=std::fopen(filePath.c_str(),"rb");
  if(!e->file){++count.opens;return 0;}
  if(std::fseek(e->file,0,SEEK_END)!=0||std::ftell(e->file)!=long(file->bytes().size())||std::fseek(e->file,0,SEEK_SET)!=0){std::fclose(e->file);return {};}
  if(!vm().memory.registerExternalRam(address,e->bytes.data(),152)){std::fclose(e->file);return {};}
  // Explicit finite native DVD entry producer, not whole DVDMgr/SDK emulation.
  // DVD entry:filename0,length74; flags90 starts8000,half92=2. Read below
  // owns destination7c,length80,offset84 and callback8c; this finite adapter
  // records delivered count88. No whole original worker semantics are implied.
  // Candidate base60030000/stride152/count256 is explicit native token policy.
  const auto n=name->size();std::memcpy(e->bytes.data(),name->c_str(),n+1);set(e->bytes,0x74,4,uint32_t(file->bytes().size()));set(e->bytes,0x90,2,0x8000);set(e->bytes,0x92,2,2);
  entries.emplace(address,std::move(e));++count.opens;return address;
 }
 // Real source heap0 allocates once before transfer admission. Detach only
 // its exact payload into shared backing; source allocator headers stay live.
 // A held transfer must never allocate another block.
 std::optional<uint32_t>allocate(const SourceFontSetupCall&q){
  if(q.integers.size()!=2||q.integers[0]||q.integers[1]!=file->bytes().size())return {};
  if(!heap){
   // Actual source heap0 descriptor/arena already belongs to this very VM.
   // No native backing/token is returned instead of the source allocator.
   Heap h;h.bytes=q.integers[1];h.base=eventHeapAllocate(0,h.bytes,vm().memoryBinding.services());++count.heapAllocations;
   if(!h.base)return 0;
   h.backing=std::make_shared<std::vector<uint8_t>>();heap=std::move(h);
  }
  auto&h=*heap;if(h.adopted)return h.base;
  if(!gate(NativeUiStartupBoundary::HeapTransfer,h.base,h.bytes))return {};
  if(!h.detached){if(!vm().memory.extractRam(h.base,h.bytes,*h.backing))return {};h.detached=true;}
  if(!memory->adoptHeapAllocation(h.base,h.backing->data(),h.bytes,h.bytes,h.backing)){
   // Re-establish the exact observed RAM on failed alias admission. Allocation
   // and its header changes remain captured; the next visit never reallocates.
   if(vm().memory.registerRam(h.base,*h.backing))h.detached=false;
   return {};
  }
  h.adopted=true;++count.heapTransfers;return h.base;
 }
 // Capture exact DVD args,perform one real read and compare every byte to
 // original preflight before publishing source delivery. DvdDelivery can hold
 // after fread; retry reuses observed bytes and coherent entry fields.
 std::optional<uint32_t>deliver(const SourceFontSetupCall&q){
  if(q.integers.size()!=4||!heap||!heap->adopted||q.integers[1]!=heap->base||q.integers[2]!=file->bytes().size()||q.integers[3])return {};
  const auto at=entries.find(q.integers[0]);if(at==entries.end()||!at->second->file)return {};
  auto&e=*at->second;
  if(read){if(read->arguments!=q.integers)return {};}
  else{
   read=Read{};read->arguments=q.integers;
   set(e.bytes,0x7c,4,q.integers[1]);set(e.bytes,0x80,4,q.integers[2]);set(e.bytes,0x84,4,q.integers[3]);
   set(e.bytes,0x90,2,(get(e.bytes,0x90,2)|1u)&~2u);set(e.bytes,0x8c,4,0);set(e.bytes,0x88,4,0);
  }
  if(!read->observed){
   read->bytes.resize(q.integers[2]);if(std::fseek(e.file,0,SEEK_SET)!=0)return {};
   const auto observed=std::fread(read->bytes.data(),1,read->bytes.size(),e.file);++count.reads;
   if(observed!=read->bytes.size()||read->bytes!=file->bytes()){invalid=true;return {};}
   read->observed=true;
  }
  if(!gate(NativeUiStartupBoundary::DvdDelivery,heap->base,uint32_t(read->bytes.size())))return {};
  if(!memory->deliverHeapRead(heap->base,read->bytes))return {};
  // Native completed delivery owns coherent fields; no flag alone authorizes
  // this child. Full actual bytes have already reached the source allocation.
  set(e.bytes,0x7c,4,heap->base+uint32_t(read->bytes.size()));set(e.bytes,0x80,4,0);set(e.bytes,0x84,4,0);set(e.bytes,0x88,4,uint32_t(read->bytes.size()));set(e.bytes,0x90,2,(get(e.bytes,0x90,2)&~1u)|2u);
  const auto result=get(e.bytes,0x74,4);read.reset();return result;
 }
 // Original identities:root07388,formatter26a0b8,DVD open144e88/length
 //144dcc/close144dd4/read144e18,heap300f0 and SDK texture72b0/7544.
 // Close sets flags bit8 but keeps the mapped record until native retirement.
 std::optional<uint32_t>request(const SourceFontSetupCall&q){
  if(!current())return {};
  switch(q.function){
  case 0x80007388:return sourceMarioDvdRoot(OriginalMarioDvdSourceR2);
  case 0x8026a0b8:formatDestination=q.formatDestination;return formatter.request(q);
  case 0x80144e88:return open(q);
  case 0x80144dcc:{if(q.integers.size()!=1)return {};auto e=entries.find(q.integers[0]);if(e==entries.end())return {};return get(e->second->bytes,0x74,4);}
  case 0x80144dd4:{if(q.integers.size()!=1)return {};auto e=entries.find(q.integers[0]);if(e==entries.end())return {};auto&v=*e->second;if(!v.file)return {};set(v.bytes,0x90,2,get(v.bytes,0x90,2)|8u);std::fclose(v.file);v.file=nullptr;++count.closes;return 0;}
  case 0x800300f0:return allocate(q);
  case 0x80144e18:return deliver(q);
  case 0x802b72b0:case 0x802b7544:return texture->request(q);
  default:return {};
  }
 }
};
NativeUiStartup::NativeUiStartup()=default;NativeUiStartup::~NativeUiStartup()=default;
// Cold startup owns the process lifetime and optional admission owner.
// Factory failure rolls back image mappings; success retains cold RAM even
// after this wrapper retires. It does not acknowledge a camera/window/GPU pass.
std::shared_ptr<NativeUiStartup>NativeUiStartup::create(std::shared_ptr<NativeFieldAnimation>process,const char*root,NativeUiStartupAdmission admission){
 if(!process||!root||!process->ready()||!coldPrefix(*process)||(admission.admit&&!admission.lifetime))return {};
 auto state=std::make_shared<State>();state->process=std::move(process);state->admission=std::move(admission);state->image=std::make_unique<Image>();
 if(!state->vm().memory.readWord(0x80415fd0,state->gp)||!state->gp)return {};
 state->filePath=path(root,"source-font/papermarioset_US.bfn");state->file=NativeSourceFontFile::load(state->filePath.c_str());
 if(!state->file||!state->image->load(path(root,"source-ui-startup/memory.pmevt").c_str())||!state->image->install(state->vm().memory))return {};
 state->texture=NativeSourceFontTexture::bind(state->vm(),state->memory,state->process,[s=state.get()]{return s->current();});
 if(!state->texture||!state->bindFormatter())return {};
 auto result=std::shared_ptr<NativeUiStartup>(new NativeUiStartup);state->image->retained=true;result->state_=std::move(state);return result;
}
// Native phases0 US GP store,1 whole FontManager,2 completed texture object.
// Resume2 never repeats cold admission/language store; later language() reads
// actual shared GP history. Keep self/state alive through reentrant providers.
bool NativeUiStartup::resumeFont(){
 if(invoking_||!state_)return false;
 const auto keepAlive=shared_from_this();const auto state=state_;auto&s=*state;if(!s.current())return false;if(s.phase==2)return s.texture->object().has_value();if(!coldPrefix(*s.process))return false;
 invoking_=true;struct Guard{bool&flag;~Guard(){flag=false;}}guard{invoking_};
 if(s.phase==0){
  // Exact actual US marioStInit contribution8006fd0c/10/14/18. It is executed
  // once in the native cold startup prefix, never as a draw/language fallback.
  uint32_t gp=0;s.pending=0x8006fd0c;if(!s.vm().memory.readWord(0x80415fd0,gp)||gp!=s.gp)return false;
  s.pending=0x8006fd18;if(!s.vm().memory.writeWord(gp+0x16c,1))return false;++s.count.gpStores;s.phase=1;
 }
 SourceFontSetupServices services;services.user=&s;services.lifetime=s.process;services.actors=s.actors();services.current=[](void*p){return static_cast<State*>(p)->current();};services.request=[](void*p,const SourceFontSetupCall&q){return static_cast<State*>(p)->request(q);};
 if(!resumeSourceFontManagerSetup(services,s.font)){s.pending=s.texture->pending()?s.texture->pending():s.font.pending();return false;}
 if(!s.current()||!s.texture->object())return false;
 s.phase=2;s.pending=0;return true;
}
uint32_t NativeUiStartup::pending()const{return state_?state_->pending:0;}
bool NativeUiStartup::current()const{return state_&&state_->current()&&(state_->phase!=2||state_->texture->object().has_value());}
std::optional<uint32_t>NativeUiStartup::language()const{return state_?state_->word(state_->gp+0x16c):std::nullopt;}
std::optional<NativeSourceFontTextureDescription>NativeUiStartup::fontObject()const{return state_&&state_->phase==2&&state_->current()?state_->texture->object():std::nullopt;}
std::shared_ptr<NativeSourceFontMemory>NativeUiStartup::fontMemory()const{return fontObject()?state_->memory:nullptr;}
std::shared_ptr<NativeFieldAnimation>NativeUiStartup::fieldProcess()const{return state_?state_->process:nullptr;}
std::shared_ptr<const void>NativeUiStartup::lifetime()const{return state_;}
std::optional<uint32_t>NativeUiStartup::word(uint32_t a)const{return state_?state_->word(a):std::nullopt;}
NativeUiStartup::Counts NativeUiStartup::counts()const{return state_?state_->count:Counts{};}
void NativeUiStartup::retire(){if(state_)state_->alive=false;}
}
