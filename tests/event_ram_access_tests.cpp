#include "event_native_runtime.hpp"
#include <cassert>
#include <type_traits>
#include <string>
using namespace ttyd;
void runEventRamAccessFixture(const char* root){
 static_assert(!std::is_copy_constructible<EventAddressSpace::RawRamAccess>::value,"Borrow must not outlive callback");
 auto vm=std::make_unique<EventNativeRuntime>(0x81000000,EventNativePlatform{});auto& m=vm->memory;
 constexpr uint32_t Base=0x83000000,External=0x87000000;
 std::vector<uint8_t> bytes(1024);for(unsigned i=0;i<bytes.size();++i)bytes[i]=uint8_t(i);
 assert(m.registerRam(Base,bytes));
 std::array<uint8_t,64> aliased{},distinct{};for(unsigned i=0;i<64;++i)aliased[i]=uint8_t(i+19);
 assert(m.registerExternalRam(External,aliased.data(),32) && m.registerExternalRam(External+0x100,aliased.data()+2,32) && m.registerExternalRam(External+0x200,distinct.data(),64));
 assert(m.registerRam(0x85000000,{0x11,0x22}) && m.registerRam(0x85000002,{0x33,0x44}) && m.registerRam(0xfffffffc,{1,2,3,4}));
 EventScriptAssets scripts;assert(scripts.load((std::string(root)+"/animation-boot/memory.pmevt").c_str()));
 bool nested=false;
 assert(m.withRawRamAccess([&](auto& ram){
  assert(!ram.bind(Base,0) && !ram.bind(0x81000038,8) && !ram.bind(0x80415fd0,4));
  // Prefix extension and fixed-capacity overflow affect only acceleration.
  for(unsigned i=1;i<=128;++i)assert(ram.bind(Base,i));
  for(unsigned i=128;i<1024;i+=8)assert(ram.bind(Base+i,8));
  uint32_t value=0;uint8_t byte=0;
  for(unsigned i=0;i<1021;++i){assert(ram.readWord(Base+i,value));assert(value==(uint32_t(bytes[i])<<24|uint32_t(bytes[i+1])<<16|uint32_t(bytes[i+2])<<8|bytes[i+3]));}
  assert(ram.writeWord(Base+1,0x89abcdef) && m.readWord(Base+1,value) && value==0x89abcdef);
  assert(m.writeByte(Base+2,7) && ram.readByte(Base+2,byte) && byte==7);
  assert(ram.writeByte(Base+3,0x55) && m.readWord(Base+1,value) && value==0x890755ef);
  // Semantic state remains live and never enters the raw cache.
  assert(m.writeWord(0x81000038,0x11223344) && ram.readWord(0x81000038,value) && value==0x11223344);
  assert(ram.writeWord(0x8100003c,0x55667788) && vm->gp.timestamp==0x1122334455667788ULL);
  assert(!ram.bind(0x85000000,4) && ram.readWord(0x85000000,value) && value==0x11223344);
  assert(ram.writeWord(0x85000000,0xdeadbeef) && m.readWord(0x85000000,value) && value==0xdeadbeef);
  assert(ram.bind(0xfffffffc,4) && ram.readWord(0xfffffffc,value) && value==0x01020304);
  value=0x98765432;assert(!ram.readWord(0xfffffffd,value) && value==0x98765432 && !ram.writeWord(0xfffffffd,0));
  assert(ram.bind(External,32) && ram.bind(External+0x100,32) && ram.bind(External+0x200,64));
  // Equal and overlapping host storage can have disjoint guest addresses.
  const auto before=aliased;assert(!ram.copy(External+0x100,External,31) && aliased==before);
  assert(!ram.copy(External,External,16) && !ram.copy(Base+2,Base+1,7));
  // Capacity exhaustion retains the ordinary caller copy fallback.
  if(!ram.copy(External+0x200,External,31))for(unsigned i=0;i<31;++i){uint8_t b=0;assert(ram.readByte(External+i,b) && ram.writeByte(External+0x200+i,b));}
  for(unsigned i=0;i<31;++i)assert(distinct[i]==aliased[i]);
  assert(!m.withRawRamAccess([&](auto&){nested=true;return true;}) && !nested);
  std::vector<uint8_t> removed{0xa5};
  assert(!m.registerRam(0x86000000,{1}) && !m.unregisterRam(Base) && !m.extractRam(Base,4,removed) && removed==std::vector<uint8_t>{0xa5});
  assert(!m.registerExternalRam(0x88000000,distinct.data(),64) && !m.unregisterExternalRam(External));
  assert(!m.importScripts(scripts));return true;
 }));
 assert(m.withRawRamAccess([&](auto& ram){
  assert(ram.bind(External,32) && ram.bind(External+0x100,32) && ram.bind(External+0x200,64));
  assert(!ram.copy(External+0x100,External,31));
  assert(ram.copy(External+0x200,External,31));
  for(unsigned i=0;i<31;++i)assert(distinct[i]==aliased[i]);assert(distinct[31]==0);
  // A call returning false still releases the owner for the next visit.
  return false;
 })==false);
 assert(m.registerRam(0x86000000,{1}) && m.unregisterRam(0x86000000));
#if defined(__cpp_exceptions)
 try{m.withRawRamAccess([&](auto& ram)->bool{assert(ram.bind(Base,16));throw 7;});assert(false);}catch(int v){assert(v==7);}
#endif
 assert(m.withRawRamAccess([&](auto& ram){return ram.bind(Base,1024);}));
 // Transparent facade acceleration caches backing, never bytes. Disjoint
 // guest aliases observe the same live host bytes at every width.
 assert(m.withRawRamAccess([&](auto&){
  uint32_t value=0;uint8_t byte=0;
  assert(m.readWord(External,value));
  assert(m.writeWord(External+0x102,0x12345678));
  assert(m.readWord(External+4,value) && value==0x12345678);
  const uint8_t expected[]={0x12,0x34,0x56,0x78};assert(m.matchesBytes(External+4,expected,4));
  assert(m.writeByte(External+5,0xab) && m.readByte(External+0x103,byte) && byte==0xab);
  assert(!m.matchesBytes(External+4,expected,4) && !m.matchesBytes(0xfffffffe,expected,4));
  assert(m.writeHalf(External+6,0xcdef) && m.readWord(External+0x102,value) && value==0x12abcdef);
  assert(m.writeWord(0x81000038,0x87654321) && m.readWord(0x81000038,value) && value==0x87654321);
  assert(m.readWord(0x85000000,value) && value==0xdeadbeef);
  const uint8_t crossing[]={0xde,0xad,0xbe,0xef};assert(m.matchesBytes(0x85000000,crossing,4));
  const uint8_t semantic[]={0x87,0x65,0x43,0x21};assert(m.matchesBytes(0x81000038,semantic,4));
  assert(!m.withRawRamAccess([&](auto&){return true;}) && !m.unregisterExternalRam(External));
  assert(m.unmappedRange(0x89000000,32) && m.unmappedRange(0x89000000,32) && !m.unmappedRange(External,4));
  return true;
 }));
 assert(m.registerRam(0x89000000,std::vector<uint8_t>(32)) && !m.unmappedRange(0x89000000,32));
 // A subsequent remap cannot hit a cached pointer from the ended scope.
 assert(m.unregisterRam(Base) && m.registerRam(Base,std::vector<uint8_t>(1024,0x6a)));
 assert(m.withRawRamAccess([&](auto&){uint32_t v=0;return m.readWord(Base,v) && v==0x6a6a6a6a;}));
 assert(m.importScripts(scripts) && m.unregisterExternalRam(External));
}
int main(){runEventRamAccessFixture("romfs");}
