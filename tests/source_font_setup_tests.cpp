#include <memory>
#include "source_font_setup.hpp"
#include "native_source_font_memory.hpp"
#include "event_native_runtime.hpp"
#include <cassert>
#include <cstring>
#include <fstream>
#include <functional>
using namespace ttyd;
namespace {
uint32_t be32(const std::vector<uint8_t>&v,size_t i){assert(i+4<=v.size());return uint32_t(v[i])<<24|uint32_t(v[i+1])<<16|uint32_t(v[i+2])<<8|v[i+3];}
struct ColdSource {
 std::vector<uint8_t>dol,bfn;uint32_t bss=0,bssSize=0;
 ColdSource(){std::ifstream iso("/home/carson/games/roms/G8ME01.iso",std::ios::binary);assert(iso);iso.seekg(0x420);std::vector<uint8_t>offset(4);iso.read(reinterpret_cast<char*>(offset.data()),4);auto base=be32(offset,0);iso.seekg(base);dol.resize(256);iso.read(reinterpret_cast<char*>(dol.data()),256);uint32_t size=256;for(unsigned i=0;i<18;++i)size=std::max(size,be32(dol,4*i)+be32(dol,0x90+4*i));dol.resize(size);iso.seekg(base);iso.read(reinterpret_cast<char*>(dol.data()),size);assert(iso);bss=be32(dol,0xd8);bssSize=be32(dol,0xdc);
  std::ifstream meta("romfs/font.json");std::string json((std::istreambuf_iterator<char>(meta)),{});auto pos=json.find("source_offset");assert(pos!=std::string::npos);pos=json.find(':',pos)+1;auto bfnoffset=std::stoull(json.substr(pos));bfn.resize(66560);iso.seekg(std::streamoff(bfnoffset));iso.read(reinterpret_cast<char*>(bfn.data()),bfn.size());assert(iso&&be32(bfn,0)==0x464f4e54);
 }
 uint8_t byte(uint32_t a)const{for(unsigned i=0;i<18;++i){auto base=be32(dol,0x48+4*i),size=be32(dol,0x90+4*i);if(a>=base&&uint64_t(a)<uint64_t(base)+size)return dol[be32(dol,4*i)+a-base];}assert(a>=bss&&uint64_t(a)<uint64_t(bss)+bssSize);return 0;}
 uint32_t word(uint32_t a)const{uint32_t v=0;for(unsigned i=0;i<4;++i)v=v<<8|byte(a+i);return v;}
 void provision(EventAddressSpace&vm)const{for(auto span:NativeSourceFontMemory::StateSpans){std::vector<uint8_t>v;for(uint32_t i=0;i<span.second;++i)v.push_back(byte(span.first+i));assert(vm.registerRam(span.first,std::move(v)));}}
 std::string string(uint32_t a)const{std::string result;while(auto b=byte(a++))result+=char(b);return result;}
};
struct Event{uint32_t fn,site;std::vector<uint32_t>args;bool operator==(const Event&o)const{return fn==o.fn&&site==o.site&&args==o.args;}};
struct Fixture {
 static constexpr uint32_t Heap=0x89000000,Root=0x8041f494;
 ColdSource source;EventNativeRuntime vm{0x81000000,{}};
 std::shared_ptr<NativeSourceFontMemory>memory=NativeSourceFontMemory::create();std::unique_ptr<NativeSourceFontBinding>binding;
 std::shared_ptr<std::vector<uint8_t>>heap;std::shared_ptr<const void>lifetime=std::make_shared<int>(1);
 SourceFontSetupCursor cursor;std::vector<Event>trace,ops;std::optional<Event>pending;
 std::vector<std::pair<uint32_t,uint8_t>>byteStores;
 std::map<uint32_t,uint32_t>extra{{0x80415fd0,0x82000000},{0x8200016c,1}};
 std::vector<uint32_t>probeLengths;unsigned open=0,failures=0;int hold=-1;bool alive=true,reenter=false;
 std::function<void(Fixture&,const Event&)>onOp;uint32_t selector=1;
 Fixture(){source.provision(vm.memory);binding=memory->bind(vm.memory);assert(binding);}
 uint32_t word(uint32_t at){std::optional<uint32_t> v;if(memory->owns(at,4))v=memory->word(at);else if(extra.count(at))v=extra[at];else v=source.word(at);assert(v);trace.push_back({1,at,{*v}});return *v;}
 uint16_t half(uint32_t at){auto v=memory->half(at);assert(v);trace.push_back({2,at,{*v}});return *v;}
 BattleActorServices actors(){BattleActorServices a;a.memory.user=this;a.memory.word=[](void*p,uint32_t at){return static_cast<Fixture*>(p)->word(at);};a.memory.half=[](void*p,uint32_t at){return static_cast<Fixture*>(p)->half(at);};a.memory.storeWord=[](void*p,uint32_t at,uint32_t value){auto&f=*static_cast<Fixture*>(p);assert(f.memory->storeWord(at,value));f.trace.push_back({3,at,{value}});};a.memory.storeByte=[](void*p,uint32_t at,uint8_t value){auto&f=*static_cast<Fixture*>(p);assert(f.memory->storeByte(at,value));f.trace.push_back({4,at,{value}});f.byteStores.push_back({at,value});};return a;}
 std::string path(const SourceFontPath&v){std::string out;for(uint32_t i=0;;++i){auto b=v.byte(i);assert(b);if(!*b)return out;out+=char(*b);}}
 bool gate(const SourceFontSetupCall&q){Event e{q.function,q.callsite,q.integers};for(float f:q.reals){uint32_t n;std::memcpy(&n,&f,4);e.args.push_back(n);}if(!pending){pending=e;ops.push_back(e);trace.push_back(e);if(onOp)onOp(*this,e);}else assert(*pending==e);if(reenter)assert(!cursor.reset()&&!run());if(int(ops.size()-1)==hold&&failures++<2)return false;pending.reset();return true;}
 SourceFontSetupServices services(){SourceFontSetupServices s;s.actors=actors();s.user=this;s.lifetime=lifetime;s.current=[](void*p){return static_cast<Fixture*>(p)->alive;};s.request=[](void*p,const SourceFontSetupCall&q)->std::optional<uint32_t>{auto&f=*static_cast<Fixture*>(p);if(!f.gate(q))return {};
  switch(q.function){
  case 0x80007388:return Root;
  case 0x8026a0b8:{assert(q.formatDestination&&q.integers.size()==3);auto format=f.source.string(q.integers[0]),root=f.source.string(q.integers[1]),name=f.source.string(q.integers[2]);auto at=format.find("%s");assert(at!=std::string::npos);format.replace(at,2,root);at=format.find("%s");assert(at!=std::string::npos);format.replace(at,2,name);for(unsigned i=0;i<format.size();++i)q.formatDestination->cells[i]=uint8_t(format[i]);q.formatDestination->cells[uint32_t(format.size())]=0;return uint32_t(format.size());}
  case 0x80144e88:{assert(q.path&&(q.integers==std::vector<uint32_t>{2,0}));auto path=f.path(*q.path);auto index=f.open++;if(!index){assert(path=="./f/papermarioset_US.bfn");return 1;}assert(path.find("./font/")==0);return index<=f.probeLengths.size()?index+1:0;}
  case 0x80144dcc:if(q.integers[0]==1)return uint32_t(f.source.bfn.size());else{assert(q.integers[0]>=2&&q.integers[0]-2<f.probeLengths.size());return f.probeLengths[q.integers[0]-2];}
  case 0x80144dd4:return 0;
  case 0x800300f0:assert(q.integers[0]==0);f.heap=std::make_shared<std::vector<uint8_t>>(q.integers[1],0xcc);assert(f.memory->adoptAllocation(Heap,f.heap->data(),q.integers[1],f.heap));return Heap;
  case 0x80144e18:assert((q.integers==std::vector<uint32_t>{1,Heap,uint32_t(f.source.bfn.size()),0}));assert(f.memory->word(0x8041e978)==Heap&&!f.memory->word(Heap));assert(f.memory->installRead(Heap,f.source.bfn));return 0; // Source ignores real read return.
  case 0x802b72b0:assert((q.integers==std::vector<uint32_t>{0x8041e97c,Heap+992+32,512,256,0,0,0,0}));return 0; // Explicit test-only GX child; no native object/raster claim.
  case 0x802b7544:assert((q.integers==std::vector<uint32_t>{0x8041e97c,1,1,0,0,0})&&(q.reals==std::vector<float>{0,0,0}));return 0;
  default:assert(false);return {};
  }
 };return s;}
 bool run(){return resumeSourceFontSetup(selector,services(),cursor);}
};
}
int main(){
 Fixture base;assert(base.run()&&!base.cursor.active());assert(base.source.bfn[77]==1&&base.memory->byte(Fixture::Heap+77)==9);assert(base.open==8&&base.byteStores.size()==2&&base.byteStores[0].second==7&&base.byteStores[1].second==9&&base.byteStores[0].first==base.byteStores[1].first&&base.memory->byte(base.byteStores[0].first)==9);assert(base.memory->word(0x8041e9a0)==Fixture::Heap+32&&base.memory->word(0x8041e99c)==Fixture::Heap+992);
 for(unsigned gate=0;gate<base.ops.size();++gate){Fixture f;f.hold=int(gate);f.reenter=true;assert(!f.run());auto trace=f.trace;assert(!f.run()&&f.trace==trace);assert(f.run()&&f.trace==base.trace&&f.byteStores==base.byteStores);}
 // Original block loop reloads its live header count once per condition.
 {bool scan=false;unsigned counts=0;for(const auto&e:base.trace){if(e.fn==0x80144dd4&&e.site==0x80075108)scan=true;if(e.fn==0x802b72b0)scan=false;if(scan&&e.fn==1&&e.site==Fixture::Heap+12)++counts;}assert(counts==12);}
 // The actual patched original BFN and source globals share a second VM.
 {EventNativeRuntime second(0x83000000,{});base.source.provision(second.memory);auto binding=base.memory->bind(second.memory);assert(binding);uint32_t font;uint8_t width;assert(second.memory.readWord(0x8041e978,font)&&font==Fixture::Heap&&second.memory.readByte(Fixture::Heap+77,width)&&width==9);assert(second.memory.writeByte(Fixture::Heap+77,7)&&base.memory->byte(Fixture::Heap+77)==7);assert(second.memory.writeByte(Fixture::Heap+77,9));}
 // The original published-before-read guard is not native readiness/admission.
 {Fixture f;for(unsigned i=0;i<base.ops.size();++i)if(base.ops[i].fn==0x80144e18)f.hold=int(i);assert(f.hold>=0&&!f.run()&&f.memory->word(0x8041e978)==Fixture::Heap&&f.memory->owns(Fixture::Heap)&&!f.memory->word(Fixture::Heap)&&f.byteStores.empty());SourceFontSetupCursor other;assert(resumeSourceFontSetup(1,f.services(),other)&&!f.memory->word(Fixture::Heap));}
 // One-time guard cannot restart a held first load; a new invocation skips all source children.
 {auto count=base.ops.size();base.selector=0xffffffff;assert(base.run()&&base.ops.size()==count);}
 // Actual manager uses GP low16 once, not caller language guesses.
 {Fixture f;f.extra[0x8200016c]=0x10001;assert(resumeSourceFontManagerSetup(f.services(),f.cursor)&&f.byteStores.size()==2);}
 // All seven probes execute; signed maximum and selected read length stay distinct.
 {Fixture f;f.probeLengths={uint32_t(f.source.bfn.size()+64),0xffffffff,0,12,40,1,4};assert(f.run()&&f.heap->size()==f.source.bfn.size()+64&&f.open==8);assert(f.memory->owns(Fixture::Heap+uint32_t(f.source.bfn.size()))&&!f.memory->byte(Fixture::Heap+uint32_t(f.source.bfn.size())));}
 // Missing actual GX provider holds before either source width patch.
 {Fixture f;f.onOp=[&](auto&,const Event&e){if(e.fn==0x802b72b0)f.alive=false;};assert(!f.run()&&f.cursor.stale()&&f.byteStores.empty());f.alive=true;assert(!f.run());}
 {Fixture f;auto s=f.services();s.request=nullptr;assert(!resumeSourceFontSetup(1,s,f.cursor)&&f.cursor.pending()==0x80007388);}
}
