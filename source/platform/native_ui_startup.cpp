#include "native_ui_startup.hpp"
#include "source_archive_layout.hpp"
#include "native_source_resource_path_formatter.hpp"
#include "source_mario_dvd_root.hpp"
#include <cmath>
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
constexpr Span WindowSpans[]={{0x802c0140,72,0xf24d0347b758c182ULL},{0x802c3080,40,0x143f03009da1047dULL},{SourceArchiveLayout::Descriptors,SourceArchiveLayout::TableBytes,0x81b169c331cabfa5ULL},{0x8041ea18,16,0x88201fb960ff6465ULL}};
struct WindowImage {
 EventScriptAssets source;std::array<std::vector<uint8_t>,4>bytes;EventAddressSpace*installed=nullptr;bool retained=false;
 ~WindowImage(){if(installed&&!retained)for(const auto&s:WindowSpans)installed->unregisterRam(s.address);}
 bool load(const char*p){if(!source.load(p)||source.segments().size()!=4)return false;for(unsigned i=0;i<4;++i){const auto&s=WindowSpans[i];const auto&t=source.segments()[i];if(t.address!=s.address||t.size!=s.size)return false;uint64_t h=14695981039346656037ULL;for(uint32_t n=0;n<s.size;++n){uint8_t v;if(!source.readByte(s.address+n,v))return false;bytes[i].push_back(v);h=(h^v)*1099511628211ULL;}if(h!=s.hash)return false;}return true;}
 bool install(EventAddressSpace&vm){for(const auto&s:WindowSpans)if(!vm.unmappedRange(s.address,s.size))return false;unsigned done=0;for(;done<4;++done)if(!vm.registerRam(WindowSpans[done].address,bytes[done]))break;if(done!=4){while(done)vm.unregisterRam(WindowSpans[--done].address);return false;}installed=&vm;return true;}
};
bool coldPrefix(NativeFieldAnimation&process){
 if(process.process().entryPending()||process.releasePending())return false;
 const auto pool=process.pool();if(!pool)return false;
 for(unsigned i=0;i<256;++i){const auto*p=pool->slot(i);if(p&&(p->word(0)||p->resource||p->nativeGeneration))return false;}
 return true;
}
// Independent proof holder, never State/Context -> history -> State cycle.
struct GxProcessProof {
 std::shared_ptr<NativeFieldAnimation>process;uint32_t gp=0;
 std::shared_ptr<const uint8_t>registration;
 bool current()const{return process&&process->ready()&&!process->releasePending()&&process->process().vm().gpAddresses.languageSourceCurrent(gp,registration);}
};
std::string path(const char*root,const char*name){std::string p=root;if(!p.empty()&&p.back()!='/')p+='/';return p+name;}
uint32_t get(const std::array<uint8_t,152>&v,unsigned a,unsigned n){uint32_t out=0;for(unsigned i=0;i<n;++i)out=(out<<8)|v[a+i];return out;}
void set(std::array<uint8_t,152>&v,unsigned a,unsigned n,uint32_t x){for(unsigned i=0;i<n;++i)v[a+i]=uint8_t(x>>(8*(n-i-1)));}
}
struct NativeUiStartup::State:std::enable_shared_from_this<NativeUiStartup::State> {
 std::shared_ptr<NativeFieldAnimation>process;std::unique_ptr<Image>image;
 std::shared_ptr<const NativeSourceFontFile>file;std::string filePath;
 std::shared_ptr<NativeSourceFontMemory>memory=NativeSourceFontMemory::create();
 std::unique_ptr<NativeSourceFontTexture>texture;
 NativeSourceResourcePathFormatter formatter;SourceFontSetupCursor font;
 NativeUiStartupAdmission admission;Counts count;
 std::shared_ptr<GxProcessProof>gxProcess;
 std::shared_ptr<SourceGxInheritedState>gx=std::make_shared<SourceGxInheritedState>();
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
  struct WindowLife {
  std::weak_ptr<State>state;std::shared_ptr<NativeFieldAnimation>process;
  static bool current(void*p){auto s=static_cast<WindowLife*>(p)->state.lock();return s&&s->windowCurrent();}
 };
 std::string rootDirectory;std::shared_ptr<WindowLife>windowLife;
 std::unique_ptr<WindowImage>windowImage;std::shared_ptr<const NativeSourceWindowFile>windowFile;
 std::shared_ptr<NativeSourceWindowMemory>windowMemory;std::unique_ptr<NativeSourceWindowBinding>windowBinding;
 SourceWindowTextureSetupCursor windowSetup,windowCompletion;
 std::shared_ptr<NativeSourceIconProcess>icons;
 uint32_t windowPhase=0,windowHandle=0,windowPending=0,archiveIndex=0;bool archiveHeld=false;
 const void*windowDestination=nullptr;NativeSourceResourcePathFormatter windowFormatter;
 WindowCounts wc;
 std::optional<Heap>windowPalette,windowObjects;
 struct WindowJob {std::array<uint32_t,5>arguments{};std::vector<uint8_t>bytes;bool observed=false,delivered=false,completionEligible=false,callback=false,sourceComplete=false,complete=false;};
 std::optional<WindowJob>windowJob;std::vector<std::array<uint8_t,32>>windowObjectBytes;
 bool entryCurrent(Entry&e){return vm().memory.withRawRamAccess([&](auto&ram){return ram.matchesBacking(e.address,152,e.bytes.data());});}
 bool windowCurrent(){return current()&&phase==2&&(windowPhase==3||coldPrefix(*process))&&texture&&texture->object().has_value();}
 std::optional<uint32_t>windowWord(uint32_t at){if(!windowCurrent())return {};if(windowMemory&&windowMemory->owns(at,4))return windowMemory->word(at);uint32_t v;if(!vm().memory.readWord(at,v))return {};return v;}
 std::optional<uint8_t>windowByte(uint32_t at){if(!windowCurrent())return {};if(windowMemory&&windowMemory->owns(at))return windowMemory->byte(at);uint8_t v;if(!vm().memory.readByte(at,v))return {};return v;}
 bool windowStore(uint32_t at,uint32_t n,uint32_t value){
  if(!windowCurrent())return false;
  if(windowMemory&&windowMemory->owns(at,n))return n==1?windowMemory->storeByte(at,uint8_t(value)):n==2?windowMemory->storeHalf(at,uint16_t(value)):n==4?windowMemory->storeWord(at,value):false;
  return n==1?vm().memory.writeByte(at,uint8_t(value)):n==2?vm().memory.writeHalf(at,uint16_t(value)):n==4?vm().memory.writeWord(at,value):false;
 }
 bool prepareWindow(){
  if(windowMemory)return windowBinding&&windowBinding->current();
  if(!windowCurrent()||!coldPrefix(*process))return false;
  auto file=NativeSourceWindowFile::load(path(rootDirectory.c_str(),"source-window/msgWindow.tpl").c_str());if(!file)return false;
  auto image=std::make_unique<WindowImage>();if(!image->load(path(rootDirectory.c_str(),"source-window-startup/memory.pmevt").c_str())||!image->install(vm().memory))return false;
  auto life=std::make_shared<WindowLife>();life->state=shared_from_this();life->process=process;
  auto memory=NativeSourceWindowMemory::create(life,life.get(),WindowLife::current);if(!memory)return false;
  auto binding=memory->bind(vm().memory,life,life.get(),WindowLife::current);if(!binding)return false;
  NativeResourcePathFormatterServices f;f.user=this;f.lifetime=life;f.current=[](void*p){return static_cast<State*>(p)->windowCurrent();};
  f.byte=[](void*p,uint32_t a)->std::optional<uint8_t>{auto&s=*static_cast<State*>(p);uint8_t v;if(s.windowImage&&s.windowImage->source.readByte(a,v))return v;if(s.image->source.readByte(a,v))return v;return {};};
  f.immutableInput=[](void*p,uint32_t a,uint32_t n){auto&s=*static_cast<State*>(p);if(!((a>=0x802c3080&&uint64_t(a)+n<=0x802c30a8)||(a>=0x8041f494&&uint64_t(a)+n<=0x8041f498)))return false;for(uint32_t i=0;i<n;++i){uint8_t expected,actual;const bool ok=s.windowImage->source.readByte(a+i,expected)||s.image->source.readByte(a+i,expected);if(!ok||!s.vm().memory.readByte(a+i,actual)||actual!=expected)return false;}return s.windowCurrent();};
  f.privateDestination=[](void*p,NativeResourcePathKind kind,const void*out,uint32_t n){auto&s=*static_cast<State*>(p);return kind==NativeResourcePathKind::Window&&out&&out==s.windowDestination&&n==256&&s.windowCurrent();};f.stringBudget=f.outputCapacity=256;
  windowImage=std::move(image);windowMemory=std::move(memory);windowBinding=std::move(binding);windowLife=std::move(life);windowFile=std::move(file);
  if(!windowFormatter.bind(std::move(f))){windowBinding.reset();windowMemory.reset();windowImage.reset();windowLife.reset();windowFile.reset();return false;}
  windowImage->retained=true;windowPhase=1;return true;
 }
 std::optional<uint32_t>windowAllocate(const SourceWindowTextureSetupCall&q){
  if(q.count!=2||q.arguments[0]||(q.callsite!=0x8007fc5c&&q.callsite!=0x8007fd28))return {};
  auto&slot=q.callsite==0x8007fc5c?windowPalette:windowObjects;
  if(!slot){Heap h;h.bytes=q.arguments[1];h.base=eventHeapAllocate(0,h.bytes,vm().memoryBinding.services());++wc.heapAllocations;if(!h.base)return 0;h.backing=std::make_shared<std::vector<uint8_t>>();slot=std::move(h);}
  auto&h=*slot;if(h.bytes!=q.arguments[1])return {};if(h.adopted)return h.base;
  if(!gate(NativeUiStartupBoundary::HeapTransfer,h.base,h.bytes))return {};
  if(!h.detached){if(!vm().memory.extractRam(h.base,h.bytes,*h.backing))return {};h.detached=true;}
  if(!windowMemory->adoptHeapAllocation(h.base,h.backing->data(),h.bytes,h.bytes,h.backing)){
   if(vm().memory.registerRam(h.base,*h.backing))h.detached=false;
   return {};
  }
  h.adopted=true;++wc.heapTransfers;return h.base;
 }
 std::optional<uint32_t>windowOpen(const SourceWindowTextureSetupCall&q){
  if(q.count!=2||q.arguments[0]!=2||q.arguments[1]||!q.path)return {};
  std::string name;for(unsigned i=0;i<256;++i){auto b=q.path->byte(i);if(!b)return {};if(!*b)break;name.push_back(char(*b));if(i==255)return {};}
  if(name!="./f/msgWindow.tpl")return {};
  uint32_t address=0;for(unsigned i=0;i<256;++i){const auto a=0x60030000u+i*152;if(!entries.count(a)&&vm().memory.unmappedRange(a,152)){address=a;break;}}if(!address)return {};
  auto e=std::make_unique<Entry>();e->address=address;e->file=std::fopen(path(rootDirectory.c_str(),"source-window/msgWindow.tpl").c_str(),"rb");
  if(!e->file){++wc.opens;return 0;}
  if(std::fseek(e->file,0,SEEK_END)!=0||std::ftell(e->file)!=long(windowFile->bytes().size())||std::fseek(e->file,0,SEEK_SET)!=0){std::fclose(e->file);return {};}
  if(!vm().memory.registerExternalRam(address,e->bytes.data(),152)){std::fclose(e->file);return {};}
  std::memcpy(e->bytes.data(),name.c_str(),name.size()+1);set(e->bytes,0x74,4,uint32_t(windowFile->bytes().size()));set(e->bytes,0x90,2,0x8000);set(e->bytes,0x92,2,2);entries.emplace(address,std::move(e));windowHandle=address;++wc.opens;return address;
 }
 std::optional<uint32_t>windowRequest(const SourceWindowTextureSetupCall&q){
  if(!windowCurrent())return {};
  switch(q.function){
  case 0x800782e4:{
   if(q.count!=3||q.arguments!=std::array<uint32_t,5>{0x802c3080,0,0,0,0})return {};
   if(archiveHeld){windowPending=0x80078198;return {};}
   if(!gate(NativeUiStartupBoundary::WindowArchive,SourceArchiveLayout::Descriptors,SourceArchiveLayout::TableBytes))return {};
   while(archiveIndex<SourceArchiveLayout::DescriptorCount){uint16_t flags;if(!vm().memory.readHalf(SourceArchiveLayout::Descriptors+SourceArchiveLayout::DescriptorBytes*archiveIndex,flags))return {};if(flags&1){archiveHeld=true;windowPending=0x80078198;return {};}++archiveIndex;++wc.archiveDescriptors;}
   return 0; // Actual whole inactive four-descriptor source exhaustion.
  }
  case 0x80007388:if(q.count)return {};return sourceMarioDvdRoot(OriginalMarioDvdSourceR2);
  case 0x8026a0b8:windowDestination=q.destination;return windowFormatter.request(q);
  case 0x80144e88:return windowOpen(q);
  case 0x80144dcc:{if(q.count!=1||q.arguments[0]!=windowHandle)return {};const auto e=entries.find(windowHandle);if(e==entries.end()||!entryCurrent(*e->second))return {};return get(e->second->bytes,0x74,4);}
  case 0x800300f0:return windowAllocate(q);
  case 0x80144de4:{
   if(q.count!=5||!windowPalette||q.arguments!=std::array<uint32_t,5>{windowHandle,windowPalette->base,windowPalette->bytes,0,0x8007fcf0})return {};
   auto e=entries.find(windowHandle);if(e==entries.end()||!e->second->file)return {};
   uint32_t userdata;if(!vm().memory.readWord(windowHandle+0x6c,userdata)||userdata!=windowHandle)return {};
   if(windowJob)return windowJob->arguments==q.arguments?std::optional<uint32_t>(0):std::nullopt;
   if(!gate(NativeUiStartupBoundary::WindowJob,windowHandle,windowPalette->bytes))return {};
   // Atomic native admission proves the real entry mapping; no callback or
   // yield interleaves the exact source word/half stores. Both flag reloads
   // are real separate reads of the retained native BE entry backing.
   const bool ok=vm().memory.withRawRamAccess([&](auto&ram){auto&b=e->second->bytes;if(!ram.matchesBacking(windowHandle,152,b.data()))return false;
    set(b,0x7c,4,q.arguments[1]);set(b,0x80,4,q.arguments[2]);set(b,0x84,4,q.arguments[3]);auto f=get(b,0x90,2);set(b,0x90,2,f|1);f=get(b,0x90,2);set(b,0x90,2,f&~2u);set(b,0x8c,4,q.arguments[4]);set(b,0x88,4,0);
    WindowJob job;job.arguments=q.arguments;windowJob=std::move(job);return true;});
   if(!ok)return {};
   ++wc.jobs;return 0;
  }
  case 0x80144dd4:{if(q.count!=1||q.arguments[0]!=windowHandle)return {};auto e=entries.find(windowHandle);if(e==entries.end()||!e->second->file)return {};auto&v=*e->second;if(!entryCurrent(v))return {};set(v.bytes,0x90,2,get(v.bytes,0x90,2)|8u);std::fclose(v.file);v.file=nullptr;++wc.closes;return 0;}
  default:return {};
  }
 }
 SourceWindowTextureSetupServices windowServices(){
  SourceWindowTextureSetupServices s;auto&g=s.objects.gx;g.user=this;g.lifetime=windowLife;g.current=[](void*p){return static_cast<State*>(p)->windowCurrent();};
  g.word=[](void*p,uint32_t a){return static_cast<State*>(p)->windowWord(a);};g.byte=[](void*p,uint32_t a){return static_cast<State*>(p)->windowByte(a);};
  g.store=[](void*p,uint32_t a,uint32_t n,uint32_t v){return static_cast<State*>(p)->windowStore(a,n,v);};
  g.clear=[](void*p,uint32_t a)->std::optional<uint32_t>{auto&s=*static_cast<State*>(p);if(!s.windowCurrent()||!s.windowObjects||a<s.windowObjects->base||uint64_t(a)+32>uint64_t(s.windowObjects->base)+s.windowObjects->bytes||(a-s.windowObjects->base)%32)return {};const bool ok=s.vm().memory.withRawRamAccess([&](auto&ram){if(!ram.matchesBacking(a,32,s.windowObjects->backing->data()+a-s.windowObjects->base))return false;auto m=s.vm().memoryBinding.services();eventFillMemory(a,0,32,m);return true;});if(!ok)return {};++s.wc.objectClears;return a;};
  g.truncate=[](void*p,float v)->std::optional<int32_t>{auto&s=*static_cast<State*>(p);if(!s.windowCurrent()||!std::isfinite(v)||double(v)<-2147483648.0||double(v)>=2147483648.0||!s.vm().platform.truncate)return {};const auto out=s.vm().platform.truncate(s.vm().platform.user,v);return s.windowCurrent()?std::optional<int32_t>(out):std::nullopt;};
  s.objects.half=[](void*p,uint32_t a)->std::optional<uint16_t>{auto&s=*static_cast<State*>(p);if(!s.windowCurrent())return {};if(s.windowMemory->owns(a,2))return s.windowMemory->half(a);uint16_t v;if(!s.vm().memory.readHalf(a,v))return {};return v;};
  s.objects.doubleWord=[](void*p,uint32_t a)->std::optional<uint64_t>{auto&s=*static_cast<State*>(p);if(!s.windowCurrent())return {};const auto h=s.windowWord(a),l=s.windowWord(a+4);if(!h||!l)return {};return uint64_t(*h)<<32|*l;};
  s.objects.subtractSingle=[](void*p,uint64_t a,uint64_t b)->std::optional<uint32_t>{auto&s=*static_cast<State*>(p);if(!s.windowCurrent()||!s.vm().platform.subtractDouble||!s.vm().platform.roundDoubleFloat)return {};double x,y;std::memcpy(&x,&a,8);std::memcpy(&y,&b,8);if((a&~0xffULL)!=0x4330000000000000ULL||b!=0x4330000000000000ULL)return {};const auto d=s.vm().platform.subtractDouble(s.vm().platform.user,x,y);if(!s.windowCurrent()||!s.vm().platform.roundDoubleFloat)return {};const auto v=s.vm().platform.roundDoubleFloat(s.vm().platform.user,d);uint32_t out;std::memcpy(&out,&v,4);return s.windowCurrent()?std::optional<uint32_t>(out):std::nullopt;};
  s.request=[](void*p,const SourceWindowTextureSetupCall&q){return static_cast<State*>(p)->windowRequest(q);};return s;
 }
 bool pumpWindow(){
  if(!windowCurrent()||!windowJob||!windowPalette)return false;
  auto&j=*windowJob;if(j.complete)return readyWindow();auto at=entries.find(windowHandle);if(at==entries.end())return false;auto&e=*at->second;if(!entryCurrent(e))return false;
  if(!j.observed){if(!e.file)return false;j.bytes.resize(j.arguments[2]);if(std::fseek(e.file,long(j.arguments[3]),SEEK_SET)!=0)return false;const auto got=std::fread(j.bytes.data(),1,j.bytes.size(),e.file);++wc.reads;if(got!=j.bytes.size()||j.bytes!=windowFile->bytes()){invalid=true;return false;}j.observed=true;}
  if(!j.delivered){
   if(!gate(NativeUiStartupBoundary::DvdDelivery,j.arguments[1],j.arguments[2])||!entryCurrent(e)||!windowMemory->deliverHeapRead(j.arguments[1],j.bytes))return false;
   // Finite successful native delivery of exactly the source request. Preserve
   // original worker's fresh N-based progress/destination/remaining operations.
   auto readWord=[&](unsigned o){return get(e.bytes,o,4);};set(e.bytes,0x80,4,readWord(0x80)-j.arguments[2]);set(e.bytes,0x88,4,readWord(0x88)+j.arguments[2]);set(e.bytes,0x7c,4,readWord(0x7c)+j.arguments[2]);
   j.completionEligible=!readWord(0x80);if(j.completionEligible){auto f=get(e.bytes,0x90,2);set(e.bytes,0x90,2,f&~1u);f=get(e.bytes,0x90,2);set(e.bytes,0x90,2,f|2u);}j.delivered=true;
  }
  if(!j.completionEligible){windowPending=0x80144de4;return false;}
  if(!j.callback){
   windowPending=get(e.bytes,0x8c,4);if(windowPending!=0x8007fcf0||get(e.bytes,0x80,4))return false;
   if(!gate(NativeUiStartupBoundary::WindowCallback,windowHandle+0x40,uint32_t(j.bytes.size()))||!entryCurrent(e))return false;
   j.callback=true;++wc.callbacks;
  }
  if(!j.sourceComplete){auto services=windowServices();if(!resumeSourceWindowTextureCompletion(windowHandle+0x40,services,windowCompletion)){windowPending=windowCompletion.childPending()?windowCompletion.childPending():windowCompletion.pending();return false;}j.sourceComplete=true;}
  if(!windowCurrent()||!windowObjects)return false;
  const auto count=windowWord(windowPalette->base+4);if(!count||uint64_t(*count)*32!=windowObjects->bytes)return false;
  windowObjectBytes.resize(*count);for(uint32_t i=0;i<*count;++i)for(unsigned n=0;n<32;++n){auto v=windowMemory->byte(windowObjects->base+32*i+n);if(!v)return false;windowObjectBytes[i][n]=*v;}
  j.complete=true;windowPhase=3;windowPending=0;return readyWindow();
 }
 std::optional<AnimationTPLImage>windowSourceImage(uint32_t index){
  if(!readyWindow()||index>=windowObjectBytes.size())return {};
  const auto base=windowPalette->base;const uint64_t end=uint64_t(base)+windowPalette->bytes;
  const auto span=[&](uint32_t a,uint64_t n){return a>=base&&uint64_t(a)+n<=end;};
  const auto table=windowMemory->word(base+8);if(!table||!span(*table,uint64_t(windowObjectBytes.size())*8))return {};
  const auto header=windowMemory->word(*table+8*index),palette=windowMemory->word(*table+8*index+4);if(!header||!span(*header,36)||!palette||*palette)return {};
  const auto dimensions=windowMemory->word(*header),format=windowMemory->word(*header+4),data=windowMemory->word(*header+8),ws=windowMemory->word(*header+12),wt=windowMemory->word(*header+16),min=windowMemory->word(*header+20),mag=windowMemory->word(*header+24),bias=windowMemory->word(*header+28),flags=windowMemory->word(*header+32);
  if(!dimensions||!format||(*format!=0&&*format!=14)||!data||!ws||!wt||!min||!mag||!bias||!flags||uint8_t(*flags)!=1)return {};
  AnimationTPLImage image;image.index=index;image.imageHeader=*header;image.paletteHeader=*palette;image.height=uint16_t(*dimensions>>16);image.width=uint16_t(*dimensions);image.data=*data;image.format=*format;image.wrapS=*ws;image.wrapT=*wt;image.minFilter=*min;image.magFilter=*mag;const auto raw=*bias;std::memcpy(&image.bias,&raw,4);image.edge=uint8_t(*flags>>24);image.minLOD=uint8_t(*flags>>16);image.maxLOD=uint8_t(*flags>>8);
  const auto bytes=((uint64_t(image.width)+7)/8)*((uint64_t(image.height)+7)/8)*32;
  if(!image.width||!image.height||!span(image.data,bytes)||image.wrapS>2||image.wrapT>2||!std::isfinite(image.bias)||image.minLOD>image.maxLOD)return {};
  return image;
 }
 bool readyWindow(){
  if(!windowCurrent()||windowPhase!=3||!windowPalette||!windowObjects||!windowMemory||!windowBinding||!windowBinding->current())return false;
  const auto p=windowMemory->word(0x8041ea18),r=windowMemory->word(0x8041ea1c),o=windowMemory->word(0x8041ea24),count=windowMemory->word(windowPalette->base+4);
  if(!p||!r||!o||!count||*p!=windowPalette->base||*r!=1||*o!=windowObjects->base||*count!=windowObjectBytes.size())return false;
  return true;
 }

 ~State(){
  if(icons)icons->retire();
  alive=false;gx->retire();windowBinding.reset();if(windowMemory)windowMemory->retire();texture.reset();
  for(auto&e:entries){if(e.second->file)std::fclose(e.second->file);vm().memory.unregisterExternalRam(e.first);}
  // Source heap allocation remains source-owned; this native retirement does
  // not invent a guest free/reset or rerun cold loading in a surviving process.
 }
 bool gate(NativeUiStartupBoundary at,uint32_t a,uint32_t n){if(!current())return false;const bool ok=!admission.admit||admission.admit(admission.user,at,a,n);return current()&&ok;}
 std::optional<uint32_t>word(uint32_t a){if(!current())return {};if(icons&&icons->owns(a,4))return icons->word(a);if(windowMemory&&windowMemory->owns(a,4))return windowMemory->word(a);if(memory->owns(a,4))return memory->word(a);uint32_t v;if(!vm().memory.readWord(a,v))return {};return v;}
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
 auto state=std::make_shared<State>();state->process=std::move(process);state->admission=std::move(admission);state->rootDirectory=root;state->image=std::make_unique<Image>();
 if(!state->vm().memory.readWord(0x80415fd0,state->gp)||!state->gp)return {};
 state->gxProcess=std::make_shared<GxProcessProof>();state->gxProcess->process=state->process;state->gxProcess->gp=state->gp;
 state->gxProcess->registration=state->vm().gpAddresses.languageRegistration(state->gp);
 SourceGxInheritedBinding gxBinding;gxBinding.process=state->gxProcess;gxBinding.user=state->gxProcess.get();
 gxBinding.current=[](void*p){return static_cast<GxProcessProof*>(p)->current();};
 gxBinding.coldCurrent=[](void*p){auto&s=*static_cast<GxProcessProof*>(p);return s.current()&&coldPrefix(*s.process);};
 if(!state->gx->bind(std::move(gxBinding)))return {};
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
 // Supported exact US cold GX contribution, before Mario/font effects.
 // This does not complete or claim reached DEMOInit/GXInit/Logo.
 s.pending=0x802b3088;if(!s.gx->initializeCold())return false;
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
uint32_t NativeUiStartup::pending()const{return state_?(state_->windowPending?state_->windowPending:state_->pending):0;}
bool NativeUiStartup::beginWindow(){
 if(invoking_||!resumeFont()||!state_)return false;
 const auto keepAlive=shared_from_this();const auto state=state_;auto&s=*state;if(!s.windowCurrent())return false;if(s.windowPhase>=2)return true;
 invoking_=true;struct Guard{bool&b;~Guard(){b=false;}}guard{invoking_};s.windowPending=0x8007fbd8;
 if(!s.prepareWindow())return false;
 auto services=s.windowServices();if(!resumeSourceWindowTextureSetup(services,s.windowSetup)){if(!s.archiveHeld)s.windowPending=s.windowSetup.childPending()?s.windowSetup.childPending():s.windowSetup.pending();return false;}
 if(!s.windowCurrent()||!s.windowJob)return false;
 s.windowPhase=2;s.windowPending=0;return true;
}
bool NativeUiStartup::pumpWindow(){
 if(invoking_||!beginWindow()||!state_)return false;
 const auto keepAlive=shared_from_this();const auto state=state_;auto&s=*state;
 invoking_=true;struct Guard{bool&b;~Guard(){b=false;}}guard{invoking_};
 if(s.windowPhase==3)return s.readyWindow();
 s.windowPending=0x80144de4;return s.pumpWindow();
}
bool NativeUiStartup::resumeWindow(){return pumpWindow();}
bool NativeUiStartup::resumeIcons(){
 if(invoking_||!resumeWindow()||!state_)return false;
 const auto keepAlive=shared_from_this();const auto state=state_;auto&s=*state;
 invoking_=true;struct Guard{bool&b;~Guard(){b=false;}}guard{invoking_};
 if(!s.current())return false;
 if(!s.icons){
  s.icons=NativeSourceIconProcess::create(s.process,path(s.rootDirectory.c_str(),"source-icons").c_str(),path(s.rootDirectory.c_str(),"source-icon-startup/startup.bin").c_str());
  if(!s.icons)return false;
 }
 return s.icons->pump();
}
std::shared_ptr<NativeSourceIconProcess>NativeUiStartup::iconProcess()const{return current()&&state_->icons&&state_->icons->current()?state_->icons:nullptr;}
bool NativeUiStartup::windowReady()const{return state_&&state_->readyWindow();}
std::shared_ptr<NativeSourceWindowMemory>NativeUiStartup::windowMemory()const{return windowReady()?state_->windowMemory:nullptr;}
std::optional<NativeUiStartup::WindowObject>NativeUiStartup::windowObject(uint32_t index)const{
 if(!windowReady()||index>=state_->windowObjectBytes.size())return {};
 for(unsigned n=0;n<32;++n)if(state_->windowMemory->byte(state_->windowObjects->base+32*index+n)!=std::optional<uint8_t>(state_->windowObjectBytes[index][n]))return {};
 const auto image=state_->windowSourceImage(index);if(!image)return {};
 WindowObject o;o.image=*image;o.palette=state_->windowPalette->base;o.address=state_->windowObjects->base+32*index;o.index=index;o.bytes=state_->windowObjectBytes[index];return o;
}
NativeUiStartup::WindowCounts NativeUiStartup::windowCounts()const{return state_?state_->wc:WindowCounts{};}
bool NativeUiStartup::ownsSourceResource(uint32_t a,uint32_t n)const{
 if(!state_||!n||uint64_t(a)+n>0x100000000ULL)return false;
 const auto overlap=[&](uint32_t b,uint32_t size){return uint64_t(a)<uint64_t(b)+size&&uint64_t(b)<uint64_t(a)+n;};
 if(state_->icons&&state_->icons->owns(a,n))return true;
 if(state_->memory->owns(a,n)||(state_->windowMemory&&state_->windowMemory->owns(a,n)))return true;
 for(const auto&s:Spans)if(overlap(s.address,s.size))return true;
 if(state_->windowImage)for(const auto&s:WindowSpans)if(overlap(s.address,s.size))return true;
 return false;
}
std::optional<uint8_t>NativeUiStartup::sourceResourceByte(uint32_t a)const{
 if(!ownsSourceResource(a)||!current())return {};
 auto&s=*state_;if(s.icons&&s.icons->owns(a))return s.icons->byte(a);
 if(s.memory->owns(a))return s.memory->byte(a);
 if(s.windowMemory&&s.windowMemory->owns(a))return s.windowMemory->byte(a);
 uint8_t v;if(!s.vm().memory.readByte(a,v))return {};return v;
}
std::optional<uint16_t>NativeUiStartup::sourceResourceHalf(uint32_t a)const{
 if(uint64_t(a)+2>0x100000000ULL)return {};
 const auto x=sourceResourceByte(a),y=sourceResourceByte(a+1);if(!x||!y)return {};return uint16_t(uint16_t(*x)<<8|*y);
}
std::optional<uint32_t>NativeUiStartup::sourceResourceWord(uint32_t a)const{
 if(uint64_t(a)+4>0x100000000ULL)return {};
 uint32_t v=0;for(unsigned i=0;i<4;++i){const auto b=sourceResourceByte(a+i);if(!b)return {};v=(v<<8)|*b;}return v;
}

bool NativeUiStartup::current()const{return state_&&state_->current()&&(state_->phase!=2||state_->texture->object().has_value());}
std::optional<uint32_t>NativeUiStartup::language()const{return state_?state_->word(state_->gp+0x16c):std::nullopt;}
std::optional<NativeSourceFontTextureDescription>NativeUiStartup::fontObject()const{return state_&&state_->phase==2&&state_->current()?state_->texture->object():std::nullopt;}
std::shared_ptr<NativeSourceFontMemory>NativeUiStartup::fontMemory()const{return fontObject()?state_->memory:nullptr;}
std::shared_ptr<NativeFieldAnimation>NativeUiStartup::fieldProcess()const{return state_?state_->process:nullptr;}
std::shared_ptr<SourceGxInheritedState>NativeUiStartup::gxHistory()const{return state_&&state_->current()&&state_->gx->current()?state_->gx:nullptr;}
std::shared_ptr<const void>NativeUiStartup::lifetime()const{return state_;}
std::optional<uint32_t>NativeUiStartup::word(uint32_t a)const{return state_?state_->word(a):std::nullopt;}
NativeUiStartup::Counts NativeUiStartup::counts()const{return state_?state_->count:Counts{};}
void NativeUiStartup::retire(){if(state_){if(state_->icons)state_->icons->retire();state_->alive=false;state_->gx->retire();if(state_->windowMemory)state_->windowMemory->retire();}}
}
