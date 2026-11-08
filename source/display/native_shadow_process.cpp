#include "native_shadow_process.hpp"
#include "native_ui_startup.hpp"
#include "native_field_animation.hpp"
#include "source_gx_texture_object.hpp"
#include "event_heap_allocation.hpp"
#include <cmath>
#include <string>
namespace ttyd {
namespace {
// Private native frame addresses, outside animation/SDK/file owners.
constexpr uint32_t SmartStack=0x8d000000,ShadowStack=0x8d010000;
struct Literal {uint32_t address;std::vector<uint8_t> bytes;};
}
struct NativeShadowProcess::State {
 std::shared_ptr<NativeUiStartup> ui;
 std::vector<Literal> literals;
 unsigned installed=0;bool alive=true,smartDone=false,shadowDone=false;
 uint32_t heapStart=0,heapEnd=0,heapHandle=0,descriptors=0,arenaPayload=0;
 Counts counts;
 std::unique_ptr<SourceSmartArena> smart;
 std::unique_ptr<SourceShadowProcess> shadows;
 SourceGxTextureObjectCursor gx;
 EventNativeRuntime& vm()const{return ui->fieldProcess()->process().vm();}
 ~State(){shadows.reset();smart.reset();while(installed){const auto& l=literals[--installed];vm().memory.unregisterExternalRam(l.address,l.bytes.data());}}
 bool word(uint32_t a,uint32_t& v)const{return vm().memory.readWord(a,v);}
 bool current()const{
  if(!alive || !ui || !ui->current() || !ui->fieldProcess() || !ui->fieldProcess()->process().heapGlobalsCurrent())return false;
  auto& m=vm().memory;
  for(unsigned i=0;i<installed;++i)if(!m.hasRamBacking(literals[i].address,literals[i].bytes.data(),literals[i].bytes.size()))return false;
  uint32_t a=0,b=0,c=0,d=0;
  if(!word(0x8041e840,a) || a!=heapStart || !word(0x8041e82c,b) || b!=heapEnd || !word(0x8041e854,c) || c!=heapHandle || !word(0x8041f070,d) || d!=descriptors)return false;
  // Full heap4 allocation remains the actual SDK allocated node. Do not
  // acknowledge a clear/recreated SDK heap beneath the retained smart owner.
  if(smartDone){const uint32_t descriptor=descriptors+12*heapHandle;uint32_t allocated=0,free=0,size=0;
   if(!word(descriptor+4,free) || free || !word(descriptor+8,allocated) || allocated!=arenaPayload-32 || !word(allocated+8,size) || size!=heapEnd-heapStart)return false;
  }
  return true;
 }
 bool install(const char* root){
  EventScriptAssets source,shadow;
  if(!source.load((std::string(root)+"/source-smart-literals.pmevt").c_str()) || !shadow.load((std::string(root)+"/source-shadow-literals.pmevt").c_str()) || !source.merge(shadow))return false;
  // Preflight the complete union. Reuse matching UI/SDK bytes; prepare only
  // genuinely absent fragments. Never import over live initialized inputs.
  for(const auto& segment:source.segments())for(uint32_t i=0;i<segment.size;++i){const uint32_t at=segment.address+i;uint8_t expected=0,actual=0;
   if(!source.readByte(at,expected))return false;
   if(vm().memory.readByte(at,actual)){if(actual!=expected)return false;continue;}
   if(!vm().memory.unmappedRange(at,1))return false;
   if(literals.empty() || uint64_t(literals.back().address)+literals.back().bytes.size()!=at)literals.push_back({at,{}});
   literals.back().bytes.push_back(expected);
  }
  for(auto& l:literals){if(!vm().memory.registerExternalRam(l.address,l.bytes.data(),l.bytes.size()))return false;++installed;}
  return true;
 }
 SourceGxTextureObjectServices sdk(){SourceGxTextureObjectServices s;s.user=this;s.lifetime=ui->fieldProcess();
  s.current=[](void* p){return static_cast<State*>(p)->current();};
  s.word=[](void* p,uint32_t a)->std::optional<uint32_t>{uint32_t v;if(!static_cast<State*>(p)->word(a,v))return {};return v;};
  s.byte=[](void* p,uint32_t a)->std::optional<uint8_t>{uint8_t v;if(!static_cast<State*>(p)->vm().memory.readByte(a,v))return {};return v;};
  s.store=[](void* p,uint32_t a,uint32_t n,uint32_t v){auto& m=static_cast<State*>(p)->vm().memory;return n==1?m.writeByte(a,uint8_t(v)):n==2?m.writeHalf(a,uint16_t(v)):n==4?m.writeWord(a,v):false;};
  s.clear=[](void* p,uint32_t a)->std::optional<uint32_t>{auto& s=*static_cast<State*>(p);if(!s.current() || !s.vm().memory.contiguousRam(a,32))return {};return eventFillMemory(a,0,32,s.vm().memoryBinding.services());};
  s.truncate=[](void* p,float f)->std::optional<int32_t>{auto& s=*static_cast<State*>(p);if(!s.current() || !s.vm().platform.truncate || !std::isfinite(f) || double(f)<-2147483648.0 || double(f)>=2147483648.0)return {};return s.vm().platform.truncate(s.vm().platform.user,f);};return s;
 }
 bool child(FieldMapSourceCall& q){if(!current())return false;const auto& a=q.arguments;auto& m=vm().memory;
  switch(q.function){
  case 0x802964ac:if(a[0]!=heapHandle || a[1]!=heapEnd-heapStart-32)return false;q.result=eventSdkAllocate(int32_t(a[0]),a[1],vm().memoryBinding.services());++counts.heap4Allocations;return current();
  case 0x800300f0:if(a[0]!=0)return false;q.result=eventHeapAllocate(0,a[1],vm().memoryBinding.services());++counts.heap0Allocations;return current();
  case 0x800050b4:if(!m.contiguousRam(a[0],a[2]))return false;q.result=eventFillMemory(a[0],int32_t(a[1]),a[2],vm().memoryBinding.services());++counts.fills;return current();
  case 0x80296a54:if(!m.contiguousRam(a[0],a[1]))return false;eventFlushMemory(a[0],a[1],vm().memoryBinding.services());++counts.flushes;return current();
  case 0x802b72b0:case 0x802b7544:{SourceGxTextureObjectInput input;input.integers=q.arguments;input.reals=q.scalars;
   if(!resumeSourceGxTextureObject(q.function,input,sdk(),gx) || !current())return false;
   if(q.function==0x802b72b0)++counts.objects;else ++counts.lods;return true;}
  // Actual GPU synchronization and moving/pending file ownership are separate
  // providers. Reached pressure/scene operations remain held here.
  default:return false;
  }
 }
};
NativeShadowProcess::NativeShadowProcess()=default;NativeShadowProcess::~NativeShadowProcess()=default;
std::shared_ptr<NativeShadowProcess> NativeShadowProcess::create(std::shared_ptr<NativeUiStartup> ui,const char* root){
 if(!ui || !root || !ui->current() || !ui->fieldProcess() || !ui->fieldProcess()->ready() || ui->fieldProcess()->entryPending() || ui->fieldProcess()->producerPending())return {};
 auto s=std::make_shared<State>();s->ui=std::move(ui);
 if(!s->word(0x8041e840,s->heapStart) || !s->word(0x8041e82c,s->heapEnd) || !s->word(0x8041e854,s->heapHandle) || !s->word(0x8041f070,s->descriptors) || s->heapEnd<=s->heapStart+32 || !s->install(root) || !s->current())return {};
 SourceSmartArenaBackend b;b.user=s.get();b.current=[](void* p){return static_cast<State*>(p)->current();};b.call=[](void* p,FieldMapSourceCall& q){return static_cast<State*>(p)->child(q);};
 s->smart=s->ui->fieldProcess()->process().bindSmartArena(SmartStack,b);if(!s->smart)return {};
 SourceShadowProcessBackend shadow;shadow.user=s.get();shadow.current=b.current;shadow.call=b.call;shadow.smartArena=s->smart.get();
 s->shadows=SourceShadowProcess::bind(s->vm().memory,ShadowStack,shadow);if(!s->shadows)return {};
 auto n=std::shared_ptr<NativeShadowProcess>(new NativeShadowProcess);n->state_=std::move(s);return n;
}
bool NativeShadowProcess::initialize(unsigned visits){
 if(invoking_ || !current())return false;
 invoking_=true;struct Guard{bool& f;~Guard(){f=false;}}guard{invoking_};auto& s=*state_;
 if(!s.smartDone){if(!s.smart->initialize(visits))return false;if(!s.word(SourceSmartArena::Work,s.arenaPayload))return false;s.smartDone=true;if(!s.current())return false;}
 if(!s.shadowDone){if(!s.shadows->initialize(visits))return false;s.shadowDone=true;}
 return ready();
}
bool NativeShadowProcess::current()const{return state_&&state_->current()&&state_->smart&&state_->smart->current()&&state_->shadows&&state_->shadows->current();}
bool NativeShadowProcess::ready()const{return current()&&state_->smartDone&&state_->shadowDone&&state_->smart->ready()&&state_->shadows->ready();}
bool NativeShadowProcess::busy()const{return invoking_||(state_&&((state_->smart&&state_->smart->pendingInstruction())||(state_->shadows&&state_->shadows->pendingInstruction())));}
uint32_t NativeShadowProcess::pending()const{return !state_?0:state_->gx.active()?state_->gx.pending():state_->smart&&!state_->smartDone?state_->smart->pendingInstruction():state_->shadows?state_->shadows->pendingInstruction():0;}
NativeShadowProcess::Counts NativeShadowProcess::counts()const{return state_?state_->counts:Counts{};}
std::shared_ptr<NativeUiStartup> NativeShadowProcess::ui()const{return state_?state_->ui:nullptr;}
SourceSmartArena* NativeShadowProcess::smartArena()const{return ready()?state_->smart.get():nullptr;}
SourceShadowProcess* NativeShadowProcess::shadow()const{return ready()?state_->shadows.get():nullptr;}
}
