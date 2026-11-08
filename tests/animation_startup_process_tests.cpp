#include "animation_startup_process.hpp"
#include "animation_tangent.hpp"
#include "event_memory_release.hpp"
#include "event_heap_allocation.hpp"
#include <cassert>
#include <cstdio>
#include <cstring>
using namespace ttyd;
namespace {
constexpr uint32_t Arena=0x82000000,Bytes=12*1024*1024,Work=0x803d9470;
uint32_t word(EventAddressSpace& m,uint32_t a){uint32_t v=0;assert(m.readWord(a,v));return v;}
uint16_t half(EventAddressSpace& m,uint32_t a){uint16_t v=0;assert(m.readHalf(a,v));return v;}
uint8_t byte(EventAddressSpace& m,uint32_t a){uint8_t v=0;assert(m.readByte(a,v));return v;}
std::vector<uint8_t> load(const std::string& path){FILE* f=std::fopen(path.c_str(),"rb");assert(f && !std::fseek(f,0,SEEK_END));auto n=std::ftell(f);assert(n>0);std::rewind(f);std::vector<uint8_t> b(size_t(n),0);assert(std::fread(b.data(),1,b.size(),f)==b.size());std::fclose(f);return b;}
struct Platform {
 uint32_t flushes=0,barriers=0,last=0;bool yield=false;
 EventNativePlatform services(){EventNativePlatform p;p.user=this;p.flushBlock=[](void* u,uint32_t a){auto& p=*static_cast<Platform*>(u);++p.flushes;p.last=a;};p.systemCall=[](void* u){++static_cast<Platform*>(u)->barriers;};return p;}
};
}
void runAnimationStartupProcessFixture(const char* root){
 Platform platform;auto p=platform.services();auto pool=std::make_shared<BattlePosePool>();auto files=NativeFileCache::create();assert(files && stagePrologueAnimationFiles(*files,root));
 assert(files->yieldProvider(&platform,[](void* u){return static_cast<Platform*>(u)->yield;}));
 const auto image=std::string(root)+"/animation-boot/memory.pmevt";
 auto make=[&](const EventNativePlatform& services,uint32_t address,uint32_t bytes){return AnimationStartupProcess::cold(files,pool,services,image.c_str(),0x81000000,address,bytes);};
 assert(!make({},Arena,Bytes) && !make(p,Arena+1,Bytes) && !make(p,Arena,1024) && !make(p,0xffffffe0,Bytes));
 assert(!AnimationStartupProcess::cold(files,pool,p,"/missing/image",0x81000000,Arena,Bytes));
 pool->outputSelector(1);assert(!make(p,Arena,Bytes));pool->outputSelector(0);
 auto absent=NativeFileCache::create();assert(!AnimationStartupProcess::cold(absent,pool,p,image.c_str(),0x81000000,Arena,Bytes));assert(absent->stage("still-open",0,{1}));
 // Real warm source file state exercises state2 and pending Yield on the cold
 // parent's next acquisition; no invented numeric file/holder is supplied.
 auto viv=files->acquire("a/vivian.bin",0);assert(viv && files->release(*viv));
 auto process=make(p,Arena,Bytes);assert(process && !process->ready());auto& vm=process->vm();auto& m=vm.memory;
 assert(word(m,0x8041e430)==Arena+Bytes && word(m,0x8041f080)==Arena+Bytes && word(m,0x8041f070)==Arena && word(m,0x8041f074)==5);
 const uint32_t spans[]={7300,3472,224,128};uint32_t start=Arena+64;
 for(unsigned i=0;i<4;++i){assert(word(m,0x8041e830+i*4)==start && word(m,0x8041e844+i*4)==i);start+=spans[i]*1024;assert(word(m,0x8041e81c+i*4)==start);}
 assert(word(m,0x8041e840)==start && word(m,0x8041e82c)==Arena+Bytes && word(m,0x8041e854)==4);
 const auto map=word(m,0x8041e808);assert(map==word(m,0x8041e834)+32 && word(m,map+4)==3472*1024-64);
 assert(platform.flushes==(3472*1024-32)/32 && platform.barriers==1 && word(m,0x80415dc0)==Work && word(m,Work)==0);
 assert(word(m,0x802c1060)==0x43300000 && word(m,0x802c1064)==0 && word(m,0x8041fb44)==0x3f000000 && word(m,0x8041fb68)==0xbf000000 && word(m,0x80306694)==0x8003e1f8);
 EventScriptAssets boot;assert(boot.load(image.c_str()));for(unsigned i=0;i<0xc00;++i){uint8_t b=0;assert(boot.readByte(0x80305900+i,b) && byte(m,0x80305900+i)==b);}
 vm.gp.timestamp=0x1122334455667788ULL;vm.initializeContexts();uint32_t field=0,battle=0;assert(vm.entryAddresses.addressOf(vm.field.entries,field) && vm.entryAddresses.addressOf(vm.battle.entries,battle));
 assert(field==Arena+96 && battle==field+0x1b020 && vm.field.capacity==256 && vm.battle.capacity==256 && vm.field.previousTimestamp==vm.gp.timestamp);
 assert(m.writeWord(0x80305900,0xdeadbeef) && m.writeWord(*viv+0xac,1));
 pool->outputSelector(1); // Deliberate late native mismatch: alias admission must hold, never reset it.
 assert(!process->advance() && process->pending()==0x8018a61c);
 const auto scratch=word(m,Work+0x104),head=word(m,Arena+8),flushes=platform.flushes,barriers=platform.barriers;
 assert(scratch>Arena && scratch+0x180000<word(m,0x8041e81c) && head==scratch-32 && word(m,scratch-24)==0x180020 && word(m,Work)==battle+0x1b020);
 assert(half(m,*viv+2)==0 && byte(m,*viv)==2 && word(m,Work+0x100)==0);
 // Formatting is already complete; changing its input across Yield must not replay it.
 assert(m.writeByte(0x802c11d8,'X'));
 assert(m.writeWord(*viv+0xac,0) && !process->advance() && process->pending()==0x8018a61c && word(m,Arena+8)==head && platform.flushes==flushes); // Yield still unavailable.
 platform.yield=true;assert(!process->advance() && !process->ready() && !process->pending()); // Full source parent done; e8 mismatch holds aliasing.
 assert(m.writeByte(0x802c11d8,'a'));
 const auto ag=word(m,Work+0x100);assert(ag && half(m,ag+2)==1 && half(m,*viv+2)==0 && byte(m,*viv)==2 && word(m,Arena+8)==head && platform.barriers==barriers);
 auto raw=load(std::string(root)+"/animation-startup/a/vivian.bin");for(unsigned i=0;i<0xc00;++i)assert(byte(m,0x80305900+i)==raw[i]);for(unsigned i=0;i<16;++i)assert(byte(m,0x80415da0+i)==raw[0xc00+i]);
 const std::string filename="a/ag2tg.bin";for(unsigned i=0;i<=filename.size();++i)assert(byte(m,0x803e6e78+i)==(i<filename.size()?uint8_t(filename[i]):0));
 pool->outputSelector(0);assert(process->advance() && process->ready() && process->animationMemory()->aliases(*pool,m));
 assert(word(m,Work+4)==64 && word(m,Work+0xc)==64 && word(m,Work+0x14)==256 && pool->slot(255) && !pool->slot(255)->word(0));
 assert(word(m,process->animationMemory()->poseAddress(255))==0 && word(m,Work+0x108)==scratch);
 const auto agHolder=word(m,ag+0xa0),agPayload=word(m,agHolder);assert(agPayload && word(m,agHolder+8)==ag && files->ownsPayload(agPayload,41984));
 assert(process->advance() && word(m,Arena+8)==head && half(m,ag+2)==1 && platform.flushes==flushes && platform.barriers==barriers);
 const auto memory=vm.memoryBinding.services();const auto temp=eventHeapAllocate(0,96,memory);assert(temp && m.writeWord(temp,0x12345678));eventHeapFree(0,temp,memory);assert(eventHeapAllocate(0,96,memory)==temp && word(m,temp)==0);eventHeapFree(0,temp,memory);
 assert(word(m,Arena+8)==head && word(m,0x8041e808)==map && half(m,ag+2)==1);
 assert(m.writeWord(0x80415dc0,0) && !process->ready() && !process->advance());assert(m.writeWord(0x80415dc0,Work) && process->ready() && process->advance());
 assert(!make(p,Arena,Bytes)); // Existing established/aliased pool cannot be cold-reset.
}
int main(){runAnimationStartupProcessFixture("romfs");}
