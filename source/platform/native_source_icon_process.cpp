#include "native_source_icon_process.hpp"
#include "native_source_resource_path_formatter.hpp"
#include "source_mario_dvd_root.hpp"
#include "event_heap_allocation.hpp"
#include "event_memory_opcodes.hpp"
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <map>
#include <string>
namespace ttyd {namespace {
// Source DVDMgr entry layout; its embedded DVDFileInfo starts at0x40.
constexpr uint32_t DvdRecordSize=0x98,DvdFileInfo=0x40;
constexpr uint32_t DvdLogicalLength=0x74,DvdDestination=0x7c,DvdRemaining=0x80;
constexpr uint32_t DvdOffset=0x84,DvdProgress=0x88,DvdCallback=0x8c,DvdFlags=0x90,DvdMode=0x92;
constexpr uint32_t DvdActive=1,DvdComplete=2,DvdCloseRequested=8,DvdOccupied=0x8000;
// Explicit checked native view policy, never an original allocator address.
constexpr uint32_t NativeDvdBase=0x60040000,NativeDvdSlots=256;
constexpr uint32_t ColdPoseSlots=256,PrivatePathCapacity=128;
constexpr uint32_t Created=1,JobsAdmitted=2,CallbacksComplete=3;
struct Guard{bool&v;explicit Guard(bool&b):v(b){v=true;}~Guard(){v=false;}};
// Native one-contribution admission per actual process owner, not a source
// readiness flag. Weak tokens retain no process and cannot revive at address ABA.
std::vector<std::weak_ptr<NativeFieldAnimation>>&admitted(){static std::vector<std::weak_ptr<NativeFieldAnimation>>tokens;return tokens;}
bool sameOwner(const std::shared_ptr<NativeFieldAnimation>&a,const std::shared_ptr<NativeFieldAnimation>&b){return a==b&&!a.owner_before(b)&&!b.owner_before(a);}
bool cold(NativeFieldAnimation&p){if(p.process().entryPending()||p.releasePending())return false;auto pool=p.pool();if(!pool)return false;for(unsigned i=0;i<ColdPoseSlots;++i){const auto*s=pool->slot(i);if(s&&(s->word(0)||s->resource||s->nativeGeneration))return false;}return true;}
uint32_t get(const std::array<uint8_t,DvdRecordSize>&b,unsigned a,unsigned n){uint32_t v=0;for(unsigned i=0;i<n;++i)v=v<<8|b[a+i];return v;}
void put(std::array<uint8_t,DvdRecordSize>&b,unsigned a,unsigned n,uint32_t v){for(unsigned i=0;i<n;++i)b[a+i]=uint8_t(v>>(8*(n-i-1)));}
}
struct NativeSourceIconProcess::State:std::enable_shared_from_this<State>{
 std::shared_ptr<NativeFieldAnimation>process;std::shared_ptr<const NativeSourceIconAssets>asset;
 std::shared_ptr<NativeSourceIconMemory>memory;std::unique_ptr<NativeSourceIconBinding>binding;
 NativeSourceIconAdmission admission;NativeSourceResourcePathFormatter formatter;
 SourceIconSetupCursor init,tplCompletion,binCompletion;Counts count;
 uint32_t gp=0,phase=0,pending=0;std::shared_ptr<const uint8_t>registration;
 bool alive=true,invalid=false;mutable bool checking=false;const void*destination=nullptr;
 std::string directory;
 struct Life{std::weak_ptr<State>state;std::shared_ptr<NativeFieldAnimation>process;static bool current(void*p){auto s=static_cast<Life*>(p)->state.lock();return s&&s->current();}};
 std::shared_ptr<Life>life;
 struct Heap{uint32_t base=0,size=0;std::shared_ptr<std::vector<uint8_t>>bytes;bool detached=false,adopted=false;};
 std::map<uint32_t,Heap>heaps;
 struct Entry {std::array<uint8_t,DvdRecordSize>bytes{};uint32_t address=0;bool tpl=false;FILE*file=nullptr;~Entry(){if(file)std::fclose(file);}};
 std::map<uint32_t,std::unique_ptr<Entry>>entries;
 struct Job {std::array<uint32_t,5>args{};std::vector<uint8_t>bytes;bool observed=false,delivered=false,callback=false,done=false;};
 std::map<uint32_t,Job>jobs;
 EventNativeRuntime&vm()const{return process->process().vm();}
 bool current(){if(!alive||invalid||checking||!process||!process->ready())return false;Guard guard(checking);
  if(!vm().gpAddresses.languageSourceCurrent(gp,registration)||(phase<CallbacksComplete&&!cold(*process)))invalid=true;
  return !invalid;
 }
 bool gate(NativeSourceIconBoundary at,uint32_t a,uint32_t n){if(!current())return false;const bool ok=!admission.admit||admission.admit(at,a,n);return current()&&ok;}
 bool entryCurrent(Entry&e){return current()&&vm().memory.withRawRamAccess([&](auto&ram){return ram.matchesBacking(e.address,DvdRecordSize,e.bytes.data());});}
 std::optional<uint8_t>byte(uint32_t a){if(!current())return {};if(memory->owns(a))return memory->byte(a);if(asset->owns(a))return asset->byte(a);uint8_t v;if(!vm().memory.readByte(a,v))return {};return v;}
 std::optional<uint32_t>word(uint32_t a){if(!current())return {};if(memory->owns(a,4))return memory->word(a);if(asset->owns(a,4))return asset->word(a);uint32_t v;if(!vm().memory.readWord(a,v))return {};return v;}
 bool store(uint32_t a,uint32_t n,uint32_t v){if(!current()||asset->owns(a,n))return false;if(memory->owns(a,n))return n==1?memory->storeByte(a,uint8_t(v)):n==2?memory->storeHalf(a,uint16_t(v)):n==4?memory->storeWord(a,v):false;
  return n==1?vm().memory.writeByte(a,uint8_t(v)):n==2?vm().memory.writeHalf(a,uint16_t(v)):n==4?vm().memory.writeWord(a,v):false;
 }
 bool prepareFormatter(){NativeResourcePathFormatterServices s;s.user=life.get();s.lifetime=life;
  s.current=Life::current;
  s.byte=[](void*p,uint32_t a)->std::optional<uint8_t>{auto s=static_cast<Life*>(p)->state.lock();return s?s->byte(a):std::nullopt;};
  s.immutableInput=[](void*p,uint32_t a,uint32_t n){auto s=static_cast<Life*>(p)->state.lock();if(!s||!s->current()||!n)return false;
   if(a>=0x802bf858&&uint64_t(a)+n<=0x802bf870)return true;
   if(a<0x8041f494||uint64_t(a)+n>0x8041f498)return false;
   // Actual source-root bytes must exist in the real startup VM. Whole leaf
   // addi r3,r2,0x8054 supplies address; this does not invent a runtime root.
   for(unsigned i=0;i<n;++i){uint8_t v;if(!s->vm().memory.readByte(a+i,v)||v!=(a+i==0x8041f494?uint8_t('.'):0))return false;}return s->current();};
  s.privateDestination=[](void*p,NativeResourcePathKind kind,const void*out,uint32_t capacity){auto s=static_cast<Life*>(p)->state.lock();return s&&s->current()&&kind==NativeResourcePathKind::Icon&&out&&out==s->destination&&capacity==PrivatePathCapacity;};
  s.stringBudget=s.outputCapacity=PrivatePathCapacity;return formatter.bind(std::move(s));
 }
 std::optional<uint32_t>allocate(const SourceIconSetupCall&q){if(q.count!=2||q.arguments[0])return {};
  auto at=heaps.find(q.callsite);if(at==heaps.end()){Heap h;h.size=q.arguments[1];h.base=vm().memoryBinding.allocateHeap(0,h.size);++count.allocations;if(!h.base)return 0;h.bytes=std::make_shared<std::vector<uint8_t>>();at=heaps.emplace(q.callsite,std::move(h)).first;}
  auto&h=at->second;if(h.size!=q.arguments[1])return {};if(h.adopted)return h.base;
  if(!gate(NativeSourceIconBoundary::HeapTransfer,h.base,h.size))return {};
  if(!h.detached){if(!vm().memory.extractRam(h.base,h.size,*h.bytes))return {};h.detached=true;}
  if(!memory->adoptHeapAllocation(h.base,h.bytes->data(),h.size,h.size,h.bytes)){if(vm().memory.registerRam(h.base,*h.bytes))h.detached=false;return {};}
  h.adopted=true;++count.transfers;return h.base;
 }
 std::optional<uint32_t>open(const SourceIconSetupCall&q){if(q.count!=2||q.arguments[0]!=2||q.arguments[1]||!q.path)return {};
  std::string name;bool terminated=false;for(unsigned i=0;i<PrivatePathCapacity;++i){auto v=q.path->byte(i);if(!v)return {};if(!*v){terminated=true;break;}name.push_back(char(*v));}if(!terminated||(name!="./icon.tpl"&&name!="./icon.bin"))return {};
  const bool tpl=name=="./icon.tpl";uint32_t address=0;for(unsigned i=0;i<NativeDvdSlots;++i){const uint32_t a=NativeDvdBase+DvdRecordSize*i;if(!entries.count(a)&&vm().memory.unmappedRange(a,DvdRecordSize)){address=a;break;}}if(!address)return {};
  auto e=std::make_unique<Entry>();e->address=address;e->tpl=tpl;e->file=std::fopen((directory+(tpl?"/icon.tpl":"/icon.bin")).c_str(),"rb");if(!e->file){++count.opens;return 0;}
  const auto&file=tpl?asset->tpl():asset->bin();if(std::fseek(e->file,0,SEEK_END)||std::ftell(e->file)!=long(file->bytes().size())||std::fseek(e->file,0,SEEK_SET))return {};
  if(!vm().memory.registerExternalRam(address,e->bytes.data(),DvdRecordSize))return {};
  std::memcpy(e->bytes.data(),name.c_str(),name.size()+1);put(e->bytes,DvdLogicalLength,4,uint32_t(file->bytes().size()));put(e->bytes,DvdFlags,2,DvdOccupied);put(e->bytes,DvdMode,2,2);entries.emplace(address,std::move(e));++count.opens;return address;
 }
 std::optional<uint32_t>request(const SourceIconSetupCall&q){if(!current())return {};
  switch(q.function){
  case 0x80007388:if(q.count)return {};return sourceMarioDvdRoot(OriginalMarioDvdSourceR2);
  case 0x8026a0b8:destination=q.destination;return formatter.request(q);
  case 0x800300f0:return allocate(q);
  case 0x800050b4:{if(q.count!=3||q.arguments[1])return {};auto at=std::find_if(heaps.begin(),heaps.end(),[&](const auto&v){return v.second.base==q.arguments[0]&&v.second.adopted&&q.arguments[2]<=v.second.size;});if(at==heaps.end())return {};
   uint32_t result=0;const uint32_t address=q.arguments[0],bytes=q.arguments[2];
   const auto&backing=*at->second.bytes;
   const bool cleared=vm().memory.withRawRamAccess([&](auto&ram){
    if(bytes&&!ram.matchesBacking(address,bytes,backing.data()))return false;
    result=eventFillMemory(address,0,bytes,vm().memoryBinding.services());
    return std::all_of(backing.begin(),backing.begin()+bytes,[](uint8_t value){return value==0;});
   });
   if(!cleared||!current())return {};
   ++count.clears;return result;}
  case 0x80144e88:return open(q);
  case 0x80144dcc:{if(q.count!=1)return {};auto e=entries.find(q.arguments[0]);if(e==entries.end()||!entryCurrent(*e->second))return {};return get(e->second->bytes,DvdLogicalLength,4);}
  case 0x80144de4:{if(q.count!=5||q.arguments[3])return {};auto e=entries.find(q.arguments[0]);if(e==entries.end()||!entryCurrent(*e->second))return {};auto&entry=*e->second;
   const auto expected=entry.tpl?0x8001b83cu:0x8001b810u;const auto n=entry.tpl?asset->tpl()->bytes().size():asset->roundedBin().size();if(q.arguments[2]!=n||q.arguments[4]!=expected)return {};
   const auto heap=heaps.find(entry.tpl?0x8001b760:0x8001b7d0);if(heap==heaps.end()||!heap->second.adopted||q.arguments[1]!=heap->second.base||q.arguments[2]!=heap->second.size)return {};
   auto j=jobs.find(q.arguments[0]);if(j!=jobs.end())return j->second.args==q.arguments?std::optional<uint32_t>(0):std::nullopt;
   Job job;job.args=q.arguments;jobs.emplace(q.arguments[0],std::move(job));put(entry.bytes,DvdDestination,4,q.arguments[1]);put(entry.bytes,DvdRemaining,4,q.arguments[2]);put(entry.bytes,DvdOffset,4,0);put(entry.bytes,DvdFlags,2,(get(entry.bytes,DvdFlags,2)|DvdActive)&~DvdComplete);put(entry.bytes,DvdCallback,4,q.arguments[4]);put(entry.bytes,DvdProgress,4,0);++count.jobs;return 0;}
  case 0x80144dd4:{if(q.count!=1)return {};auto e=entries.find(q.arguments[0]);if(e==entries.end()||!entryCurrent(*e->second))return {};auto&entry=*e->second;if(!entry.file)return {};put(entry.bytes,DvdFlags,2,get(entry.bytes,DvdFlags,2)|DvdCloseRequested);std::fclose(entry.file);entry.file=nullptr;++count.closes;return 0;}
  default:return {};
  }
 }
 SourceIconSetupServices services(){SourceIconSetupServices s;auto&g=s.common.objects.gx;g.user=life.get();g.lifetime=life;g.current=Life::current;
  g.word=[](void*p,uint32_t a)->std::optional<uint32_t>{auto s=static_cast<Life*>(p)->state.lock();return s?s->word(a):std::nullopt;};
  g.byte=[](void*p,uint32_t a)->std::optional<uint8_t>{auto s=static_cast<Life*>(p)->state.lock();return s?s->byte(a):std::nullopt;};
  g.store=[](void*p,uint32_t a,uint32_t n,uint32_t v){auto s=static_cast<Life*>(p)->state.lock();return s&&s->store(a,n,v);};
  s.request=[](void*p,const SourceIconSetupCall&q)->std::optional<uint32_t>{auto s=static_cast<Life*>(p)->state.lock();return s?s->request(q):std::nullopt;};return s;
 }
 bool pump(){if(!current()||phase<JobsAdmitted)return false;auto source=services();for(auto&entry:entries){auto& e=*entry.second;auto at=jobs.find(entry.first);if(at==jobs.end())continue;auto&j=at->second;if(j.done)continue;if(!entryCurrent(e))return false;
   if(!j.observed){if(!e.file)return false;const auto&logical=e.tpl?asset->tpl()->bytes():asset->bin()->bytes();j.bytes.resize(logical.size());if(std::fseek(e.file,0,SEEK_SET))return false;const auto n=std::fread(j.bytes.data(),1,j.bytes.size(),e.file);++count.reads;if(n!=logical.size()||j.bytes!=logical||std::fgetc(e.file)!=EOF||std::ferror(e.file)){invalid=true;return false;}
    if(!e.tpl){j.bytes=asset->roundedBin();}
    if(j.bytes.size()!=j.args[2])return false;
    j.observed=true;}
   if(!j.delivered){if(!gate(NativeSourceIconBoundary::Delivery,j.args[1],j.args[2])||!entryCurrent(e)||!memory->deliverHeapRead(j.args[1],j.bytes))return false;
    put(e.bytes,DvdRemaining,4,get(e.bytes,DvdRemaining,4)-j.args[2]);put(e.bytes,DvdProgress,4,get(e.bytes,DvdProgress,4)+j.args[2]);put(e.bytes,DvdDestination,4,get(e.bytes,DvdDestination,4)+j.args[2]);if(!get(e.bytes,DvdRemaining,4)){put(e.bytes,DvdFlags,2,get(e.bytes,DvdFlags,2)&~DvdActive);put(e.bytes,DvdFlags,2,get(e.bytes,DvdFlags,2)|DvdComplete);}j.delivered=true;++count.deliveries;}
   if(get(e.bytes,DvdRemaining,4)||get(e.bytes,DvdCallback,4)!=j.args[4])return false;
   if(!j.callback){if(!gate(NativeSourceIconBoundary::Callback,e.address+DvdFileInfo,j.args[2]))return false;j.callback=true;++count.callbacks;}
   pending=j.args[4];auto&cursor=e.tpl?tplCompletion:binCompletion;
   const bool done=e.tpl?resumeSourceIconTplCompletion(e.address+DvdFileInfo,source,cursor):resumeSourceIconBinCompletion(e.address+DvdFileInfo,source,cursor);
   if(!done){pending=cursor.childPending()?cursor.childPending():cursor.pending();return false;}j.done=true;
  }
  phase=CallbacksComplete;pending=0;return ready();
 }
 bool ready(){if(!current()||phase!=CallbacksComplete||jobs.size()!=2||heaps.size()!=4)return false;for(const auto&j:jobs)if(!j.second.done)return false;
  const auto t=memory->word(0x8041e630),b=memory->word(0x8041e634),tr=memory->word(0x8041e638),br=memory->word(0x8041e63c);
  return t&&b&&tr&&br&&*tr==1&&*br==1&&*t==heaps.at(0x8001b760).base&&*b==heaps.at(0x8001b7d0).base;
 }
 void retire(){alive=false;init.reset();tplCompletion.reset();binCompletion.reset();formatter.retire();binding.reset();if(memory)memory->retire();for(auto&e:entries)vm().memory.unregisterExternalRam(e.first);entries.clear();}
 ~State(){retire();}
};
NativeSourceIconProcess::NativeSourceIconProcess()=default;
NativeSourceIconProcess::~NativeSourceIconProcess(){retire();}
std::shared_ptr<NativeSourceIconProcess>NativeSourceIconProcess::create(std::shared_ptr<NativeFieldAnimation>process,const char*directory,const char*pack,NativeSourceIconAdmission admission){
 if(!process||!process->ready()||!cold(*process)||!directory||!pack||(admission.admit&&!admission.lifetime))return {};
 auto s=std::make_shared<State>();s->process=std::move(process);s->directory=directory;s->admission=std::move(admission);if(!s->vm().memory.readWord(0x80415fd0,s->gp))return {};s->registration=s->vm().gpAddresses.languageRegistration(s->gp);if(!s->registration||!s->current())return {};
 for(const auto&w:admitted())if(auto token=w.lock())if(sameOwner(token,s->process))return {};
 s->asset=NativeSourceIconAssets::load(directory,pack);if(!s->asset)return {};
 // Cold-only owner, never adopt a previous IconInit/ready flag as completion.
 // SourceInit will itself write all8 words; preexisting state indicates another
 // owner or source execution and is deliberately outside this admission domain.
 if(!s->vm().memory.unmappedRange(NativeSourceIconMemory::StateBase,NativeSourceIconMemory::StateSize))return {};
 s->life=std::make_shared<State::Life>();s->life->state=s;s->life->process=s->process;
 s->memory=NativeSourceIconMemory::create(s->life,s->life.get(),State::Life::current);if(!s->memory)return {};
 s->binding=s->memory->bind(s->vm().memory,s->life,s->life.get(),State::Life::current);if(!s->binding||!s->prepareFormatter())return {};
 admitted().push_back(s->process);s->phase=Created;auto result=std::shared_ptr<NativeSourceIconProcess>(new NativeSourceIconProcess);result->state_=std::move(s);return result;
}
bool NativeSourceIconProcess::begin(){if(invoking_||!state_)return false;Guard guard(invoking_);auto&s=*state_;if(!s.current())return false;if(s.phase>=JobsAdmitted)return true;auto services=s.services();if(!resumeSourceIconSetup(services,s.init)){s.pending=s.init.childPending()?s.init.childPending():s.init.pending();return false;}s.phase=JobsAdmitted;s.pending=0;return true;}
bool NativeSourceIconProcess::pump(){if(invoking_||!state_)return false;if(!begin())return false;Guard guard(invoking_);return state_->pump();}
bool NativeSourceIconProcess::current()const{if(invoking_||!state_)return false;Guard guard(invoking_);return state_->current();}
bool NativeSourceIconProcess::ready()const{if(invoking_||!state_)return false;Guard guard(invoking_);return state_->ready();}
uint32_t NativeSourceIconProcess::pending()const{return state_?state_->pending:0;}
bool NativeSourceIconProcess::owns(uint32_t a,uint32_t n)const{return state_&&((state_->memory&&state_->memory->owns(a,n))||(state_->asset&&state_->asset->owns(a,n)));}
std::optional<uint8_t>NativeSourceIconProcess::byte(uint32_t a)const{if(invoking_||!state_)return {};Guard guard(invoking_);return state_->byte(a);}
std::optional<uint16_t>NativeSourceIconProcess::half(uint32_t a)const{if(invoking_||!state_||a==UINT32_MAX)return {};Guard guard(invoking_);auto x=state_->byte(a),y=state_->byte(a+1);return x&&y?std::optional<uint16_t>(uint16_t(*x)<<8|*y):std::nullopt;}
std::optional<uint32_t>NativeSourceIconProcess::word(uint32_t a)const{if(invoking_||!state_)return {};Guard guard(invoking_);return state_->word(a);}
std::shared_ptr<NativeSourceIconMemory>NativeSourceIconProcess::memory()const{return current()?state_->memory:nullptr;}
std::shared_ptr<const NativeSourceIconAssets>NativeSourceIconProcess::assets()const{return current()?state_->asset:nullptr;}
NativeSourceIconProcess::Counts NativeSourceIconProcess::counts()const{return state_?state_->count:Counts{};}
void NativeSourceIconProcess::retire(){if(invoking_||!state_)return;Guard guard(invoking_);state_->retire();}
}
