#include "battle_menu_file_cache.hpp"
#include "battle_event_callbacks.hpp"
#include "event_native_runtime.hpp"
#include <cassert>
#include <cstdio>
#include <fstream>
#include <iterator>
#include <type_traits>
#include <unistd.h>
using namespace ttyd;
namespace {
constexpr uint32_t B=0x84000000;
uint32_t word(EventAddressSpace& m,uint32_t a){uint32_t v=0;assert(m.readWord(a,v));return v;}
uint16_t half(EventAddressSpace& m,uint32_t a){uint16_t v=0;assert(m.readHalf(a,v));return v;}
uint8_t byte(EventAddressSpace& m,uint32_t a){uint8_t v=0;assert(m.readByte(a,v));return v;}
void provision(EventNativeRuntime& vm){
 // Exact original root bytes belong to imported DOL data in the real VM.
 assert(vm.memory.registerRam(BattleMenuFileCache::Root,{'.',0}));
 assert(vm.memory.registerRam(0x8041eb90,{0x84,0,0,0}));
 assert(vm.memory.registerRam(B,std::vector<uint8_t>(0x16400)));
 const std::string format="%s/battle/common/battle_common.tpl";
 std::vector<uint8_t> bytes(format.begin(),format.end());bytes.push_back(0);
 assert(vm.memory.registerRam(0x802ef810,std::move(bytes)));
}
void invalid(const std::string& path){
 auto cache=std::make_shared<BattleMenuFileCache>();
 assert(!cache->load(nullptr) && !cache->ready() && !cache->load("/no/battle/bank"));
 std::ifstream input(path,std::ios::binary);
 std::vector<uint8_t> raw{std::istreambuf_iterator<char>(input),{}};assert(raw.size()==217920);
 const auto original=raw;
 char temporary[]="/tmp/ttyd-menu-bank-XXXXXX";int fd=mkstemp(temporary);assert(fd>=0);close(fd);
 auto write=[&](){std::ofstream out(temporary,std::ios::binary);out.write(reinterpret_cast<const char*>(raw.data()),raw.size());};
 raw[12]=0x7f;write();assert(!cache->load(temporary) && !cache->ready());
 raw=original;
 const uint32_t descriptor=uint32_t(raw[12])<<24 | uint32_t(raw[13])<<16 |
     uint32_t(raw[14])<<8 | raw[15];
 for(unsigned i=0;i<4;++i)raw[20+i]=uint8_t((descriptor+1)>>(24-i*8));
 write();assert(!cache->load(temporary) && !cache->ready());
 raw.resize(12);write();assert(!cache->load(temporary) && !cache->ready());
 assert(unlink(temporary)==0);
 assert(cache->load(path.c_str()) && cache->ready()); // Failed load did not poison owner.
 assert(!cache->load(path.c_str())); // Bound backing can never move on reload.
}
void ownership(const std::string& path){
 auto cache=std::make_shared<BattleMenuFileCache>();assert(cache->load(path.c_str()));
 uint32_t entry=0,payload=0;
 {
  EventNativeRuntime first(0x81000000,{}),second(0x81000000,{});
  assert(cache->bind(first.memory) && cache->bind(second.memory));
  assert(!cache->bind(first.memory)); // Duplicate refusal preserves existing mappings.
  assert(word(first.memory,BattleMenuFileCache::Global)==BattleMenuFileCache::Manager);
  const auto pool=word(first.memory,BattleMenuFileCache::Manager);
  for(unsigned i=0;i<1024;++i){
   assert(!byte(first.memory,pool+i*0xb0));
   assert(word(first.memory,pool+i*0xb0+0xa4)==(i==1023?0:pool+(i+1)*0xb0));
  }
  provision(first);provision(second);
  BattleMenuFileBinding a(cache,nativeBattleActorServices(first)),b(cache,nativeBattleActorServices(second));
  auto sa=a.services(),sb=b.services();BattleMenuResourceCursor ca,cb;
  // Formatting reads the actual VM, rather than returning a canned path.
  assert(first.memory.writeByte(0x802ef810,'!'));
  assert(!initializePackedBattleMenuResources(sa,ca) && ca.phase==2);
  assert(first.memory.writeByte(0x802ef810,'%'));
  assert(initializePackedBattleMenuResources(sa,ca));
  entry=word(first.memory,B+0x163f8);assert(entry==pool);
  const auto holder=word(first.memory,entry+0xa0);payload=word(first.memory,holder);
  assert(payload && word(first.memory,holder+8)==entry);
  assert(half(first.memory,entry+2)==1 && byte(first.memory,entry)==1 && byte(first.memory,entry+1)==4);
  assert(word(first.memory,payload)==0x0020af30 && word(first.memory,payload+4)==97);
  const auto table=word(first.memory,payload+8);assert(table==payload+12);
  for(unsigned i=0;i<97;++i){
   const auto image=word(first.memory,table+i*8),palette=word(first.memory,table+i*8+4);
   if(image){assert(image>=payload && image<payload+217920);assert(byte(first.memory,image+35)==1);assert(word(first.memory,image+8)>=payload && word(first.memory,image+8)<payload+217920);}
   if(palette){assert(palette>=payload && palette<payload+217920);assert(byte(first.memory,palette+2)==1);assert(word(first.memory,palette+8)>=payload && word(first.memory,palette+8)<payload+217920);}
  }
  const auto circle=word(first.memory,table),bubble=word(first.memory,table+8);
  assert(half(first.memory,circle)==40 && half(first.memory,circle+2)==40);
  assert(half(first.memory,bubble)==80 && half(first.memory,bubble+2)==128);
  // Two independent VMs observe the same source descriptor and refcount.
  assert(initializePackedBattleMenuResources(sb,cb));
  assert(word(second.memory,B+0x163f8)==entry && half(first.memory,entry+2)==2);
  assert(endPackedBattleMenuResources(sa,ca) && half(second.memory,entry+2)==1);
  assert(endPackedBattleMenuResources(sb,cb) && !half(first.memory,entry+2) && byte(second.memory,entry)==2);
  const auto savedTable=word(first.memory,payload+8);
  assert(initializePackedBattleMenuResources(sb,cb));
  assert(word(second.memory,B+0x163f8)==entry && half(second.memory,entry+2)==1 && byte(second.memory,entry)==1);
  assert(word(second.memory,payload+8)==savedTable); // No second relocation.
  assert(!cache->acquire("./another.tpl",4) && !cache->acquire("./battle/common/battle_common.tpl",3));
  assert(!cache->release(entry+1) && cache->release(0));
  assert(endPackedBattleMenuResources(sb,cb));
 }
 // Destroying the VMs does not destroy the native process-owned file backing.
 EventNativeRuntime later(0x81000000,{});assert(cache->bind(later.memory));
 assert(word(later.memory,word(later.memory,entry+0xa0))==payload);
 assert(cache->acquire("./battle/common/battle_common.tpl",4)==entry);
 assert(half(later.memory,entry+2)==1 && byte(later.memory,entry)==1);
 assert(cache->release(entry));
}
void rollback(const std::string& path){
 BattleMenuFileCache cache;assert(cache.load(path.c_str()));
 EventNativeRuntime vm(0x81000000,{});
 // Conflict late enough to require undoing earlier successful registrations.
 constexpr uint32_t payload=0x87100000;
 assert(vm.memory.registerRam(payload,{7,8}));
 assert(!cache.bind(vm.memory));uint32_t v=0;
 assert(!vm.memory.readWord(BattleMenuFileCache::Global,v));
 assert(!vm.memory.readWord(BattleMenuFileCache::Manager,v));
 assert(byte(vm.memory,payload)==7);
 assert(vm.memory.unregisterRam(payload));assert(cache.bind(vm.memory));
}
}
int main(int argc,char** argv){
 static_assert(!std::is_copy_constructible<BattleMenuFileCache>::value);
 static_assert(!std::is_move_constructible<BattleMenuFileCache>::value);
 const std::string path=argc>1?argv[1]:"romfs/battle-class-common.tpl";
 invalid(path);ownership(path);rollback(path);
}
