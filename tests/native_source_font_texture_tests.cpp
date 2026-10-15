#include <memory>
#include "native_source_font_texture.hpp"
#include "event_native_runtime.hpp"
#include <cassert>
#include <fstream>
#include <cstring>
#include <iostream>
using namespace ttyd;
namespace {
uint32_t be(const std::vector<uint8_t>&v,unsigned at){assert(at+4<=v.size());return uint32_t(v[at])<<24|uint32_t(v[at+1])<<16|uint32_t(v[at+2])<<8|v[at+3];}
struct Original {
 std::vector<uint8_t>dol,bfn;uint32_t bss=0,bssSize=0;
 Original(){std::ifstream iso("/home/carson/games/roms/G8ME01.iso",std::ios::binary);assert(iso);std::vector<uint8_t>head(0x428);iso.read(reinterpret_cast<char*>(head.data()),head.size());assert(iso&&std::string(head.begin(),head.begin()+6)=="G8ME01");const auto at=be(head,0x420);dol.resize(256);iso.seekg(at);iso.read(reinterpret_cast<char*>(dol.data()),dol.size());assert(iso);unsigned size=256;for(unsigned i=0;i<18;++i)size=std::max(size,be(dol,4*i)+be(dol,0x90+4*i));dol.resize(size);iso.seekg(at);iso.read(reinterpret_cast<char*>(dol.data()),dol.size());assert(iso);bss=be(dol,0xd8);bssSize=be(dol,0xdc);bfn.resize(66560);iso.seekg(948050384);iso.read(reinterpret_cast<char*>(bfn.data()),bfn.size());assert(iso&&be(bfn,0)==0x464f4e54&&bfn[77]==1);}
 uint8_t byte(uint32_t at)const{for(unsigned i=0;i<18;++i){auto base=be(dol,0x48+4*i),size=be(dol,0x90+4*i);if(at>=base&&uint64_t(at)<uint64_t(base)+size)return dol[be(dol,4*i)+at-base];}assert(at>=bss&&uint64_t(at)<uint64_t(bss)+bssSize);return 0;}
 std::vector<uint8_t>span(uint32_t at,unsigned n)const{std::vector<uint8_t>v;for(unsigned i=0;i<n;++i)v.push_back(byte(at+i));return v;}
 uint32_t word(uint32_t at)const{return be(span(at,4),0);}
 std::string string(uint32_t at)const{std::string text;while(auto b=byte(at++))text+=char(b);return text;}
 void globals(EventAddressSpace&vm)const{for(auto span:NativeSourceFontMemory::StateSpans)assert(vm.registerRam(span.first,this->span(span.first,span.second)));}
};
struct Fixture {
 static constexpr uint32_t Heap=0x89000000,Object=0x8041e97c;
 Original source;std::shared_ptr<EventNativeRuntime>vm=std::make_shared<EventNativeRuntime>(0x81000000,EventNativePlatform{});
 std::shared_ptr<NativeSourceFontMemory>memory=NativeSourceFontMemory::create();
 std::shared_ptr<std::vector<uint8_t>>heap=std::make_shared<std::vector<uint8_t>>(source.bfn.size(),0xcc);
 bool alive=true;unsigned truncations=0;bool reenter=false,retireNumeric=false;std::unique_ptr<NativeSourceFontTexture>owner;
 explicit Fixture(bool prepare=true){source.globals(vm->memory);for(auto s:{std::pair<uint32_t,unsigned>{0x803c4e84,60},{0x8041e558,8},{0x80429440,40},{0x804203f8,4}})assert(vm->memory.registerRam(s.first,source.span(s.first,s.second)));owner=NativeSourceFontTexture::bind(*vm,memory,vm,[this]{return alive;});assert(owner);assert(memory->adoptAllocation(Heap,heap->data(),heap->size(),heap));if(prepare){assert(memory->installRead(Heap,source.bfn));assert(vm->memory.writeWord(0x8041e978,Heap)&&vm->memory.writeWord(0x8041e99c,Heap+992)&&vm->memory.writeWord(0x8041e9a0,Heap+32));}numeric();}
 SourceFontSetupCall init()const{SourceFontSetupCall q;q.function=0x802b72b0;q.callsite=0x800751b8;q.integers={Object,Heap+992+32,512,256,0,0,0,0};return q;}
 SourceFontSetupCall lod()const{SourceFontSetupCall q;q.function=0x802b7544;q.callsite=0x800751e0;q.integers={Object,1,1,0,0,0};q.reals={0,0,0};return q;}
 void numeric(){vm->platform.user=this;vm->platform.truncate=[](void*p,float v)->int32_t{auto&f=*static_cast<Fixture*>(p);++f.truncations;if(f.reenter){assert(!f.owner->request(f.lod()));assert(!f.owner->object());}if(f.retireNumeric)f.owner->retire();return int32_t(v);};}
 uint32_t word(uint32_t at){uint32_t v;assert(vm->memory.readWord(at,v));return v;}
};
void genuineChildren(){Fixture f;assert(!f.owner->object()&&!f.owner->request(f.lod()));assert(f.owner->request(f.init())==std::optional<uint32_t>(0));assert(f.word(Fixture::Object)==0x90&&!f.owner->object());
 EventNativeRuntime second(0x82000000,{});f.source.globals(second.memory);auto binding=f.memory->bind(second.memory);assert(binding);
 f.reenter=true;assert(f.owner->request(f.lod())==std::optional<uint32_t>(0)&&f.truncations==3);auto object=f.owner->object();assert(object&&object->object==Fixture::Object&&object->font==Fixture::Heap&&object->glyphs==Fixture::Heap+992&&object->image==Fixture::Heap+1024&&object->width==512&&object->height==256);assert(f.word(Fixture::Object)==0x190&&f.word(Fixture::Object+8)==0x3fdff&&f.word(Fixture::Object+12)==((object->image>>5)&0x1fffff));uint16_t count;assert(second.memory.readHalf(Fixture::Object+28,count)&&count==2048&&f.memory->half(Fixture::Object+28)==2048);for(unsigned i=0;i<32;++i)assert(f.memory->byte(Fixture::Object+i)==object->objectBytes[i]);assert(f.memory->byte(Fixture::Heap+77)==1); // no fabricated whole Setup7/9 completion.
 assert(!f.owner->request(f.lod())&&!f.owner->request(f.init()));
 assert(f.vm->memory.writeByte(Fixture::Object+31,7)&&!f.owner->object()&&f.owner->stale());assert(f.vm->memory.writeByte(Fixture::Object+31,object->objectBytes[31])&&!f.owner->object());}
void heldRealProviders(){Fixture f;assert(f.owner->request(f.init()));f.vm->platform.truncate=nullptr;assert(!f.owner->request(f.lod())&&f.owner->pending()==0x802b7580&&!f.owner->object());auto altered=f.lod();altered.reals[0]=1;assert(!f.owner->request(altered));assert(f.vm->memory.writeWord(Fixture::Object,0xdeadbeef));f.numeric();assert(f.owner->request(f.lod())&&f.word(Fixture::Object)==0x190&&f.owner->object());
 Fixture g;assert(g.owner->request(g.init()));std::vector<uint8_t>constant;assert(g.vm->memory.unregisterRam(0x80429440,&constant));assert(!g.owner->request(g.lod())&&g.owner->pending()==0x802b7548&&g.truncations==0);assert(g.vm->memory.registerRam(0x80429440,std::move(constant)));assert(g.owner->request(g.lod())&&g.owner->object());
 Fixture h;assert(h.vm->memory.unregisterRam(0x803c4e84));assert(!h.owner->request(h.init())&&h.owner->pending()==0x802b7424);assert(h.word(Fixture::Object)==0x90);assert(h.vm->memory.writeWord(0x8041e978,Fixture::Heap+32));assert(!h.owner->request(h.init())&&h.owner->stale());assert(h.vm->memory.writeWord(0x8041e978,Fixture::Heap));assert(!h.owner->request(h.init()));}
void domainAndRetirement(){Fixture f;auto bad=f.init();bad.callsite++;assert(!f.owner->request(bad)&&f.word(Fixture::Object)==0);bad=f.init();bad.integers[1]+=32;assert(!f.owner->request(bad));bad=f.init();bad.integers.push_back(0);assert(!f.owner->request(bad));assert(!NativeSourceFontTexture::bind(*f.vm,f.memory,f.vm,[]{return true;}));assert(f.owner->request(f.init()));assert(f.owner->request(f.lod()));f.alive=false;assert(!f.owner->object()&&f.owner->stale());f.alive=true;assert(!f.owner->object());f.owner->retire();assert(f.owner->stale()&&!f.owner->request(f.init())&&f.word(0x8041e978)==0);assert(f.memory->word(Fixture::Object)==0x190); // shared CPU history survives VM detachment.
 Fixture g;assert(g.owner->request(g.init()));g.retireNumeric=true;assert(!g.owner->request(g.lod())&&g.owner->stale()&&!g.owner->object());assert(g.word(0x8041e978)==0);
 EventNativeRuntime vm(0x83000000,{});auto memory=NativeSourceFontMemory::create();auto life=std::make_shared<int>(1);assert(!NativeSourceFontTexture::bind(vm,memory,life,[]{return true;})&&!memory->captured());assert(!NativeSourceFontTexture::bind(vm,memory,{},[]{return true;}));}
void actualVmLifetimePin(){
 Fixture f;assert(f.owner->request(f.init())&&f.owner->request(f.lod()));std::weak_ptr<EventNativeRuntime> actual=f.vm;f.vm.reset();assert(!actual.expired()&&f.owner->object());f.owner.reset();assert(actual.expired()); // detach binding BEFORE dropping sole actual VM owner.
}
void wholeSetupSdkRoute(){
 Fixture f(false);
 // Actual cold globals come from the original DOL/BSS. Setup owns all
 // pointer writes; actual BFN bytes are installed only by the declared test
 // DVD Read below. This does not claim native DVD/catalog/heap readiness.
 SourceFontSetupServices s;s.user=&f;s.lifetime=f.vm;s.current=[](void*p){return static_cast<Fixture*>(p)->alive;};s.actors.memory.user=&f;
 s.actors.memory.word=[](void*p,uint32_t at){auto&f=*static_cast<Fixture*>(p);if(f.memory->owns(at,4)){auto v=f.memory->word(at);assert(v);return *v;}return f.source.word(at);};
 s.actors.memory.half=[](void*p,uint32_t at){auto&f=*static_cast<Fixture*>(p);auto v=f.memory->half(at);assert(v);return *v;};
 s.actors.memory.storeWord=[](void*p,uint32_t at,uint32_t v){assert(static_cast<Fixture*>(p)->vm->memory.writeWord(at,v));};
 s.actors.memory.storeByte=[](void*p,uint32_t at,uint8_t v){assert(static_cast<Fixture*>(p)->vm->memory.writeByte(at,v));};
 unsigned opens=0;s.request=[](void*,const SourceFontSetupCall&)->std::optional<uint32_t>{return {};};
 // A typed wrapper context preserves genuine SDK q by identity and actual
 // complete child execution; only explicitly named file/path fixtures below
 // replace missing OS/catalog providers for this isolated source composition.
 struct Context {Fixture*f;unsigned*opens;}context{&f,&opens};s.user=&context;s.current=[](void*p){return static_cast<Context*>(p)->f->alive;};
 s.request=[](void*p,const SourceFontSetupCall&q)->std::optional<uint32_t>{auto&c=*static_cast<Context*>(p);auto&f=*c.f;switch(q.function){case 0x80007388:return 0x8041f494;case 0x8026a0b8:{assert(q.formatDestination);auto text=f.source.string(q.integers[0]);for(unsigned i=1;i<3;++i){auto at=text.find("%s");assert(at!=std::string::npos);text.replace(at,2,f.source.string(q.integers[i]));}for(unsigned i=0;i<text.size();++i)q.formatDestination->cells[i]=uint8_t(text[i]);q.formatDestination->cells[uint32_t(text.size())]=0;return uint32_t(text.size());}case 0x80144e88:return (*c.opens)++?0:1;case 0x80144dcc:return uint32_t(f.source.bfn.size());case 0x80144dd4:return 0;case 0x800300f0:return Fixture::Heap;case 0x80144e18:assert(q.integers[1]==Fixture::Heap);assert(f.memory->installRead(Fixture::Heap,f.source.bfn));return uint32_t(f.source.bfn.size());case 0x802b72b0:case 0x802b7544:return f.owner->request(q);default:assert(false);return {};}};
 SourceFontSetupCursor cursor;assert(resumeSourceFontSetup(1,s,cursor));assert(opens==8&&f.owner->object()&&f.memory->byte(Fixture::Heap+77)==9&&f.truncations==3);
}
}
int main(){genuineChildren();heldRealProviders();domainAndRetirement();actualVmLifetimePin();wholeSetupSdkRoute();std::cout<<"actual native source font SDK texture tests PASS\n";}
