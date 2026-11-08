#include <memory>
#include "native_source_icon_memory.hpp"
#include "source_window_texture_setup.hpp"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <unistd.h>
using namespace ttyd;
namespace {
const char*startup(){const auto*p=std::getenv("TTYD_ICON_STARTUP_PACK");return p?p:"romfs/source-icon-startup/startup.bin";}
struct VM {EventEntryAddresses entries;EventContextAddresses contexts{entries};EventRuntimeGlobals globals;EventSchedule schedule;EventRuntimeAddresses runtime{globals,schedule};EventGpAddresses gp;EventAddressSpace memory{entries,contexts,runtime,gp};};
struct Life {bool live=true;std::shared_ptr<const void>pin=std::make_shared<int>(1);static bool check(void*p){return static_cast<Life*>(p)->live;}};
constexpr auto Base=NativeSourceIconMemory::StateBase;
uint32_t w(EventAddressSpace&m,uint32_t at){uint32_t v;assert(m.readWord(at,v));return v;}
void state(){Life process,one,two;auto owner=NativeSourceIconMemory::create(process.pin,&process,Life::check);assert(owner&&owner->current()&&!owner->captured()&&owner->owns(Base)&&!owner->word(Base));assert(!NativeSourceIconMemory::create({},&process,Life::check));assert(!owner->storeWord(Base,0));
 VM a,b;assert(a.memory.registerRam(Base,{0x12,0x34}));assert(a.memory.registerRam(Base+4,{0xab,0xcd,0xef,0x01}));assert(b.memory.registerRam(Base,std::vector<uint8_t>(32,0x77)));
 auto x=owner->bind(a.memory,one.pin,&one,Life::check);assert(x&&x->current()&&owner->captured());assert(!owner->bind(a.memory,one.pin,&one,Life::check));assert(owner->half(Base)==0x1234&&!owner->word(Base));
 auto y=owner->bind(b.memory,two.pin,&two,Life::check);assert(y&&y->current());uint8_t v;assert(!b.memory.readByte(Base+2,v)&&!owner->byte(Base+2));assert(w(b.memory,Base+4)==0xabcdef01);assert(!b.memory.writeWord(Base,0));
 assert(owner->storeWord(Base+8,0x89abcdef));assert(w(a.memory,Base+8)==0x89abcdef&&w(b.memory,Base+8)==0x89abcdef);assert(a.memory.writeHalf(Base+8,0x1234)&&owner->word(Base+8)==0x1234cdef);assert(owner->storeHalf(Base+2,0x5678)&&w(a.memory,Base)==0x12345678);assert(!owner->storeWord(Base-1,0));assert(owner->half(Base)==0x1234);
 y->retire();assert(!y->current()&&w(b.memory,Base)==0x77777777&&!owner->byte(Base+12));x.reset();assert(!a.memory.readByte(Base+2,v)&&!a.memory.readByte(Base+8,v)&&a.memory.readByte(Base,v)&&v==0x12);assert(owner->word(Base)==0x12345678);
 auto again=owner->bind(b.memory,two.pin,&two,Life::check);assert(again&&w(b.memory,Base)==0x12345678);process.live=false;assert(!owner->current()&&!owner->byte(Base));process.live=true;assert(!owner->current());owner->retire();assert(w(b.memory,Base)==0x77777777);
}
void allocations(){Life p,l;VM a,b;auto o=NativeSourceIconMemory::create(p.pin,&p,Life::check);auto x=o->bind(a.memory,l.pin,&l,Life::check),y=o->bind(b.memory,l.pin,&l,Life::check);assert(x&&y);
 auto backing=std::make_shared<std::vector<uint8_t>>(64,0xcc);std::weak_ptr<const void>weak=backing;constexpr uint32_t Heap=0x89000000;
 assert(o->adoptAllocation(Heap,backing->data(),64,backing));assert(!o->adoptAllocation(Heap+16,backing->data()+16,16,backing));assert(!o->adoptAllocation(Heap+4096,backing->data()+16,16,backing));assert(!o->adoptAllocation(Base,backing->data(),16,backing));assert(!o->adoptAllocation(0xfffffff0,backing->data(),32,backing));assert(o->owns(Heap-1,2)&&o->owns(Heap+63)&&!o->word(Heap));
 assert(o->installRead(Heap,{1,2,3,4,5}));assert(!o->installRead(Heap,{9}));assert(w(a.memory,Heap)==0x01020304&&w(b.memory,Heap)==0x01020304);assert(!o->half(Heap+4)&&(*backing)[5]==0xcc);assert(o->storeWord(Heap+8,0x12345678)&&w(a.memory,Heap+8)==0x12345678);assert(!o->word(Heap+5));assert(b.memory.writeByte(Heap+1,0xff)&&o->word(Heap)==0x01ff0304);backing.reset();assert(!weak.expired());x.reset();y.reset();assert(o->word(Heap+8)==0x12345678);
 VM c;auto z=o->bind(c.memory,l.pin,&l,Life::check);assert(z&&w(c.memory,Heap+8)==0x12345678);o->retire();uint8_t v;assert(!c.memory.readByte(Heap,v)&&!z->current()&&!o->word(Heap));z.reset();o.reset();assert(weak.expired());
}
void rejection(){Life p,l;auto o=NativeSourceIconMemory::create(p.pin,&p,Life::check);VM a;std::array<uint8_t,1>external{{0x55}};assert(a.memory.registerRam(Base,{0x12}));assert(a.memory.registerExternalRam(Base+2,external.data(),1));assert(!o->bind(a.memory,l.pin,&l,Life::check)&&!o->captured());uint8_t v;assert(a.memory.readByte(Base,v)&&v==0x12&&a.memory.readByte(Base+2,v)&&v==0x55);
 VM b;auto lease=o->bind(b.memory,l.pin,&l,Life::check);assert(lease);assert(b.memory.registerRam(Base+3,{0x99}));assert(!o->storeWord(Base,0x12345678)&&!o->byte(Base)&&b.memory.readByte(Base+3,v)&&v==0x99);assert(!b.memory.readByte(Base,v));assert(b.memory.unregisterRam(Base+3));assert(o->storeWord(Base,0x12345678));
 auto heap=std::make_shared<std::vector<uint8_t>>(64,0xcc);assert(o->adoptAllocation(0x89000000,heap->data(),64,heap));assert(b.memory.registerRam(0x89000030,{0x88}));assert(!o->installRead(0x89000000,{1,2,3})&&(*heap)[0]==0xcc&&!o->byte(0x89000000));
 l.live=false;assert(!lease->current()&&!o->storeWord(Base,0));l.live=true;assert(!lease->current());lease->retire();assert(!b.memory.readByte(Base,v));
}
void transactionsAndReentry(){
 Life p,l;VM a,b;auto o=NativeSourceIconMemory::create(p.pin,&p,Life::check);auto x=o->bind(a.memory,l.pin,&l,Life::check),y=o->bind(b.memory,l.pin,&l,Life::check);assert(x&&y);uint8_t v;assert(b.memory.registerRam(Base+2,{0x99}));assert(!o->storeWord(Base,0x12345678)&&!a.memory.readByte(Base,v)&&!b.memory.readByte(Base,v)&&!o->byte(Base));assert(b.memory.unregisterRam(Base+2));assert(o->storeWord(Base,0x12345678));
 VM borrowed;assert(borrowed.memory.registerRam(Base,{0xab}));bool attempted=false;assert(borrowed.memory.withRawRamAccess([&](auto&){attempted=true;assert(!o->bind(borrowed.memory,l.pin,&l,Life::check));return true;}));assert(attempted&&borrowed.memory.readByte(Base,v)&&v==0xab);auto z=o->bind(borrowed.memory,l.pin,&l,Life::check);assert(z&&w(borrowed.memory,Base)==0x12345678);
 struct Context{NativeSourceIconMemory*owner=nullptr;NativeSourceIconBinding*binding=nullptr;bool active=false;}context;
 auto check=+[](void*p){auto&c=*static_cast<Context*>(p);if(c.active){assert(!c.owner->current());assert(!c.owner->storeWord(Base,0));c.owner->retire();}return true;};auto owner=NativeSourceIconMemory::create(p.pin,&context,check);assert(owner);context.owner=owner.get();context.active=true;assert(owner->current());context.active=false;auto lease=owner->bind(a.memory,l.pin,&l,Life::check);assert(!lease); // a is already externally owned by the first process.
 VM vm;auto vmcheck=+[](void*p){auto&c=*static_cast<Context*>(p);if(c.active&&c.binding){assert(!c.binding->current());c.binding->retire();c.owner->retire();assert(!c.owner->storeWord(Base,0));}return true;};lease=owner->bind(vm.memory,l.pin,&context,vmcheck);assert(lease);context.binding=lease.get();context.active=true;assert(lease->current());context.active=false;assert(owner->storeWord(Base,0x98765432)&&w(vm.memory,Base)==0x98765432);owner->retire();assert(!vm.memory.readByte(Base,v));
 auto end=std::make_shared<std::vector<uint8_t>>(16,0xaa);assert(o->adoptAllocation(0xfffffff0,end->data(),16,end));assert(o->owns(0xfffffffe,4)&&!o->word(0xfffffffe));
 o->retire();
}
void actualMutableFile(){
 auto assets=NativeSourceIconAssets::load("romfs/source-icons",startup());assert(assets&&assets->bin()->bytes().size()==8174&&assets->tpl()->bytes().size()==677792&&assets->roundedBin().size()==8192);
 assert(assets->word(0x802bf858)==0x25732f69&&assets->owns(0x802bf857,2)&&!assets->word(0x8041e630));
 Life p,l;VM a,b;auto o=NativeSourceIconMemory::create(p.pin,&p,Life::check);auto x=o->bind(a.memory,l.pin,&l,Life::check),y=o->bind(b.memory,l.pin,&l,Life::check);assert(x&&y);
 auto heap=std::make_shared<std::vector<uint8_t>>(assets->tpl()->bytes().size()+32,0xcc);constexpr uint32_t Heap=0x89000000;
 assert(o->adoptAllocation(Heap,heap->data(),uint32_t(heap->size()),heap));assert(o->installRead(Heap,assets->tpl()->bytes()));SourceWindowTextureSetupServices s;s.objects.gx.user=o.get();s.objects.gx.lifetime=o;s.objects.gx.current=[](void*p){return static_cast<NativeSourceIconMemory*>(p)->current();};s.objects.gx.word=[](void*p,uint32_t a){return static_cast<NativeSourceIconMemory*>(p)->word(a);};s.objects.gx.byte=[](void*p,uint32_t a){return static_cast<NativeSourceIconMemory*>(p)->byte(a);};s.objects.gx.store=[](void*p,uint32_t a,uint32_t n,uint32_t v){auto&o=*static_cast<NativeSourceIconMemory*>(p);return n==1?o.storeByte(a,uint8_t(v)):n==2?o.storeHalf(a,uint16_t(v)):n==4?o.storeWord(a,v):false;};SourceWindowTextureSetupCursor c;assert(resumeSourceUnpackTexturePalette(Heap,s,c));assert(o->word(Heap+4)==613&&o->word(Heap+8)==Heap+12&&w(a.memory,Heap+8)==Heap+12&&w(b.memory,Heap+8)==Heap+12);assert(!o->word(0x8041e638)&&!o->word(0x8041e63c));assert(!o->byte(Heap+uint32_t(assets->tpl()->bytes().size()))&&heap->back()==0xcc);
 assert(assets->tpl()->bytes()[8]==0&&assets->tpl()->bytes()[11]==12);
 auto bin=std::make_shared<std::vector<uint8_t>>(8192,0xcc);assert(o->adoptAllocation(0x89200000,bin->data(),8192,bin));assert(o->installRead(0x89200000,assets->roundedBin()));for(unsigned i=0;i<8192;++i)assert(o->byte(0x89200000+i)==assets->roundedBin()[i]);
 o->retire();
 assert(!NativeSourceIconFile::load("romfs/source-icons/icon.tpl",NativeSourceIconFile::Kind::Bin));
 assert(!NativeSourceIconAssets::load("romfs/source-icons","/no/startup"));
 char path[]="/tmp/ttyd-icon-startup-XXXXXX";int fd=mkstemp(path);assert(fd>=0);close(fd);
 std::ifstream input(startup(),std::ios::binary);assert(input);std::vector<uint8_t>raw((std::istreambuf_iterator<char>(input)),{});
 auto save=[&](){FILE*f=std::fopen(path,"wb");assert(f&&std::fwrite(raw.data(),raw.size(),1,f)==1);std::fclose(f);};save();assert(NativeSourceIconAssets::load("romfs/source-icons",path));raw[33]^=1;save();assert(!NativeSourceIconAssets::load("romfs/source-icons",path));raw.pop_back();save();assert(!NativeSourceIconAssets::load("romfs/source-icons",path));assert(!unlink(path));
}
}
int main(){state();allocations();rejection();transactionsAndReentry();actualMutableFile();std::cout<<"native source icon memory tests PASS\n";}
