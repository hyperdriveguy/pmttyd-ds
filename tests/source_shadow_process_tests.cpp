#include "source_shadow_process.hpp"
#include "source_gx_texture_object.hpp"
#include "event_native_runtime.hpp"
#include "event_memory_bootstrap.hpp"
#include "event_heap_allocation.hpp"
#include <cassert>
#include <cmath>
#include <vector>
#include <tuple>
#include <string>
using namespace ttyd;
namespace {
using P=SourceShadowProcess;
constexpr uint32_t Heap=0x82000000,HeapEnd=Heap+1024*1024,Stack=0x83000000;
uint32_t word(EventAddressSpace& m,uint32_t a){uint32_t v=0;assert(m.readWord(a,v));return v;}
EventNativePlatform platform(){EventNativePlatform p;p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){};return p;} // Host cache instrumentation; no native GPU receipt.
struct Fixture {
 EventNativeRuntime vm{0x81000000,platform()};
 std::shared_ptr<int> life=std::make_shared<int>(0);
 std::unique_ptr<P> process;
 SourceGxTextureObjectCursor gx;
 bool alive=true,smart=false,hold=false,waiting=false;
 unsigned allocations=0,clears=0,objects=0,lods=0;
 std::vector<uint32_t> payloads,sizes;
 std::vector<std::array<uint32_t,2>> flushed;
 uint32_t pending=0;FieldMapSourceCall* held=nullptr;FieldMapSourceCall operands;
 explicit Fixture(const char* root="romfs"){
  auto& m=vm.memory;EventScriptAssets literals;assert(literals.load((std::string(root)+"/source-shadow-literals.pmevt").c_str()) && m.importScripts(literals));
  assert(m.registerRam(Heap,std::vector<uint8_t>(HeapEnd-Heap)) && m.registerRam(0x8041f070,std::vector<uint8_t>(16)) && m.registerRam(0x8041e428,std::vector<uint8_t>(4)) && m.registerRam(0x8041e844,std::vector<uint8_t>(4)));
  auto services=vm.memoryBinding.services();const auto usable=eventSdkInitializeAllocator(Heap,HeapEnd,1,services);
  assert(eventSdkCreateHeap(usable,HeapEnd,services)==0 && m.writeWord(0x8041e844,0));
  SourceShadowProcessBackend backend;backend.user=this;backend.current=[](void* u){return static_cast<Fixture*>(u)->alive;};backend.call=[](void* u,FieldMapSourceCall& q){return static_cast<Fixture*>(u)->child(q);};
  process=P::bind(m,Stack,backend);assert(process);
 }
 SourceGxTextureObjectServices textureServices(){SourceGxTextureObjectServices s;s.user=this;s.lifetime=life;
  s.current=[](void* u){return static_cast<Fixture*>(u)->alive;};
  s.word=[](void* u,uint32_t a)->std::optional<uint32_t>{uint32_t v;if(!static_cast<Fixture*>(u)->vm.memory.readWord(a,v))return {};return v;};
  s.byte=[](void* u,uint32_t a)->std::optional<uint8_t>{uint8_t v;if(!static_cast<Fixture*>(u)->vm.memory.readByte(a,v))return {};return v;};
  s.store=[](void* u,uint32_t a,uint32_t n,uint32_t v){auto& m=static_cast<Fixture*>(u)->vm.memory;return n==1?m.writeByte(a,uint8_t(v)):n==2?m.writeHalf(a,uint16_t(v)):n==4?m.writeWord(a,v):false;};
  s.clear=[](void* u,uint32_t a)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(u);assert(f.vm.memory.contiguousRam(a,32));return eventFillMemory(a,0,32,f.vm.memoryBinding.services());};
  s.truncate=[](void*,float f)->std::optional<int32_t>{if(!std::isfinite(f) || double(f)<-2147483648.0 || double(f)>=2147483648.0)return {};return int32_t(f);};return s;
 }
 bool child(FieldMapSourceCall& q){pending=q.function;
  assert(!process->initialize()); // Native callback reentry cannot execute prefix twice.
  if(q.function==0x8002f540 && !smart)return false;
  if(hold && !waiting){held=&q;operands=q;waiting=true;return false;}
  if(waiting){assert(held==&q && operands.function==q.function && operands.arguments==q.arguments && operands.scalars==q.scalars);waiting=false;held=nullptr;}
  auto& a=q.arguments;auto& m=vm.memory;
  switch(q.function){
  case 0x800300f0:{assert(a[0]==0);const auto p=eventHeapAllocate(a[0],a[1],vm.memoryBinding.services());assert(p && m.contiguousRam(p,a[1]));payloads.push_back(p);sizes.push_back(a[1]);++allocations;q.result=p;return true;}
  case 0x800050b4:assert(m.contiguousRam(a[0],a[2]));++clears;q.result=eventFillMemory(a[0],int32_t(a[1]),a[2],vm.memoryBinding.services());return true;
  case 0x8002f540:
   // Explicit source-boundary smart allocation provider. It establishes NO
   // genuine smartInit/AutoFree/list state or native renderer residency.
   assert(a[0]==160000 && a[1]==0);q.result=0x84000000;return true;
  case 0x802b72b0:case 0x802b7544:{SourceGxTextureObjectInput input;input.integers=q.arguments;input.reals=q.scalars;
   assert(resumeSourceGxTextureObject(q.function,input,textureServices(),gx));if(q.function==0x802b72b0)++objects;else ++lods;return true;}
  case 0x80296a54:
   assert(m.contiguousRam(a[0],a[1]));flushed.push_back({a[0],a[1]});eventFlushMemory(a[0],a[1],vm.memoryBinding.services());return true;
  default:assert(false);return false;
  }
 }
 void finish(){for(unsigned i=0;i<10000;++i)if(process->initialize(1024))return;assert(false);}
};
void sourceInit(const char* root){
 Fixture f(root);auto& m=f.vm.memory;assert(!f.process->ready() && word(m,P::Primary+0x10c)==0);
 FieldMapSourceCall q;q.function=P::Projection;assert(!f.process->service(q));
 // Genuine first allocation/clear happen once; early gate1 cannot admit query.
 for(unsigned i=0;i<20;++i)assert(!f.process->initialize());
 assert(f.pending==0x8002f540 && f.process->pendingInstruction()==0x8002dda4);
 assert(f.allocations==1 && f.clears==1 && f.sizes[0]==0x1700 && word(m,P::Primary+0x108)==f.payloads[0]);
 assert(word(m,P::Primary+0x10c)==1 && !f.process->ready() && !f.process->service(q));
 assert(m.writeWord(P::Primary+4,0x12345678));f.smart=true;f.hold=true;f.finish();
 assert(f.process->ready() && f.allocations==3 && f.objects==2 && f.lods==2 && f.flushed.size()==2);
 assert(word(m,P::Primary+4)==0x12345678 && word(m,P::Primary+0xfc)==0x84000000 && word(m,P::Primary+0x110)==2 && word(m,P::Primary+0x114)==0x41a00000 && word(m,P::Primary+0x118)==1);
 assert(word(m,P::Primary+0x100)==0x6e && word(m,P::ProjectionWork)==2 && word(m,P::DepthWork)==2);
 assert(word(m,P::ProjectionWork+0x100)==0x40 && word(m,P::DepthWork+0x100)==0x40);
 assert(f.sizes==std::vector<uint32_t>({0x1700,256,131072}));
 for(unsigned i=0;i<0x1700;++i){uint8_t v=1;assert(m.readByte(f.payloads[0]+i,v) && v==0);}
 // Independent GX tile coordinates: I8 tiles8x4, IA8 tiles4x4, BE halfwords.
 for(unsigned y=0;y<16;++y)for(unsigned x=0;x<16;++x){const unsigned at=((y/4)*2+x/8)*32+(y%4)*8+x%8;uint8_t v=0;assert(m.readByte(f.payloads[1]+at,v) && v==x*16+y);}
 for(unsigned y=0;y<256;++y)for(unsigned x=0;x<256;++x){const unsigned at=(((y/4)*64+x/4)*16+(y%4)*4+x%4)*2;uint16_t v=0;assert(m.readHalf(f.payloads[2]+at,v) && v==x*256+y);}
 for(unsigned i=0;i<2;++i){assert((f.flushed[i]==std::array<uint32_t,2>{f.payloads[i+1],f.sizes[i+1]}));const auto obj=P::RampObjects+(i?0:32);assert(word(m,obj+20)==(i?3u:1u));uint8_t complete=0;assert(m.readByte(obj+31,complete) && (complete&2));}
 // Return reads mutable primary gate, preserving raw call identity across yields.
 assert(!f.process->service(q,1));auto foreign=q;assert(!f.process->service(foreign));q.arguments[1]=9;assert(!f.process->service(q));q.arguments[1]=0;
 for(unsigned i=0;i<10 && !q.result;++i)f.process->service(q,1);assert(q.result==1);
 assert(m.writeWord(P::Primary+0x10c,0));FieldMapSourceCall zero;zero.function=P::Projection;assert(f.process->service(zero) && zero.result==0);
 assert(f.process->initialize() && f.allocations==3 && word(m,P::Primary+0x10c)==0);
 // Replacement cannot be torn down by the old owner.
 assert(m.unregisterExternalRam(P::Primary));std::array<uint8_t,P::PrimaryBytes> replacement{};assert(m.registerExternalRam(P::Primary,replacement.data(),replacement.size()));assert(!f.process->current() && !f.process->ready());f.process.reset();assert(m.hasRamBacking(P::Primary,replacement.data(),replacement.size()));
}
void sizes(const char* root){
 Fixture f(root);auto& m=f.vm.memory;FieldMapSourceServices s;s.user=&m;
 s.word=[](void* u,uint32_t a,uint32_t& v){return static_cast<EventAddressSpace*>(u)->readWord(a,v);};
 s.store=[](void* u,uint32_t a,uint32_t v){return static_cast<EventAddressSpace*>(u)->writeWord(a,v);};
 const std::vector<unsigned> tile8x8{0,8,14,32,48},tile8x4{1,2,9,17,34,39,40,41,42,57,58},tile4x4{3,4,5,6,10,19,22,35,43,44,60};
 auto contains=[](const std::vector<unsigned>& a,unsigned v){for(auto n:a)if(n==v)return true;return false;};
 for(unsigned format=0;format<64;++format)for(const auto dimensions:{std::array<uint32_t,2>{0,0},{1,1},{15,33},{256,400},{65535,32767},{0x10010,0x20011}})for(unsigned flag:{0u,1u,2u,257u})for(unsigned levels:{0u,1u,5u}){
  const unsigned tw=contains(tile8x8,format)||contains(tile8x4,format)?8:contains(tile4x4,format)?4:1;
  const unsigned th=contains(tile8x8,format)?8:contains(tile8x4,format)||contains(tile4x4,format)?4:1;
  const unsigned tileBytes=format==6 || format==22?64:32;
  uint32_t w=uint16_t(dimensions[0]),h=uint16_t(dimensions[1]),expected=0;
  auto level=[&]{return ((w+tw-1)/tw)*((h+th-1)/th)*tileBytes;};
  if(uint8_t(flag)!=1)expected=level();else for(unsigned i=0;i<uint8_t(levels);++i){expected+=level();if(w==1 && h==1)break;w=w>1?w/2:1;h=h>1?h/2:1;}
  FieldMapSourceCursor cursor;std::optional<uint32_t> got;
  for(unsigned i=0;i<2000 && !got;++i)got=resumeFieldMapSource(0x802b708c,{dimensions[0],dimensions[1],format,flag,levels},Stack+P::StackBytes-P::StackGuardBytes,s,cursor,17);
  assert(got && *got==expected);
 }
}
void lifetime(const char* root){Fixture f(root);f.alive=false;assert(!f.process->initialize() && !f.process->current() && f.allocations==0);}
}
int main(){sourceInit("romfs");sizes("romfs");lifetime("romfs");}
