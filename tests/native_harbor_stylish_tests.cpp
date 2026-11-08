#include "native_harbor_stylish.hpp"
#include "event_native_runtime.hpp"
#include <cassert>
using namespace ttyd;
struct Fixture{
 static constexpr uint32_t Battle=0x89000000,Unit=0x89100000,Args=0x89200000;
 EventNativeRuntime vm{0x81000000,{}};EventEntry e;NativeHarborStylishCursor cursor;
 Fixture(){assert(vm.memory.registerRam(0x8041eb90,std::vector<uint8_t>(4))&&vm.memory.registerRam(Battle,std::vector<uint8_t>(0x17000))&&vm.memory.registerRam(Unit,std::vector<uint8_t>(0x400))&&vm.memory.registerRam(Args,std::vector<uint8_t>(20)));
  assert(vm.memory.writeWord(0x8041eb90,Battle)&&vm.memory.writeWord(Battle+0x24,Unit));
  assert(vm.entryAddresses.registerEntries(0x89300000,&e,1));
  e.flags=1;e.id=17;e.currentCommand=0x95510000;e.arguments=Args;e.argumentCount=5;
  const uint32_t v[]={1,2,4,6,0};for(unsigned i=0;i<5;++i)assert(vm.memory.writeWord(Args+4*i,v[i]));
 }
 uint32_t read(uint32_t a){uint32_t v;assert(vm.memory.readWord(a,v));return v;}
 void press(unsigned history=0){assert(vm.memory.writeWord(Battle+0xf2c+0x18+4*history,0x100));}
 std::optional<int32_t>step(bool physical=true){return nativeHarborStylishStart(vm,e,!cursor.started,physical,cursor);}
};
int main(){
 {Fixture f;assert(!f.step(false)&&f.read(Fixture::Battle+0x271c)==0&&!f.cursor.started);for(unsigned n=1;n<6;++n){assert(f.step()==0&&f.read(Fixture::Battle+0x271c)==n);}assert(f.step()==1&&f.read(Fixture::Battle+0x2730)==1);}
 {Fixture f;f.press();assert(f.step()==1&&int32_t(f.read(Fixture::Battle+0x2730))==-1);}
 {Fixture f;assert(f.step()==0&&f.step()==0);f.press();assert(f.step()==1&&f.read(Fixture::Battle+0x2730)==2);}
 {Fixture f;for(unsigned n=0;n<4;++n)assert(f.step()==0);f.press();assert(f.step()==1&&int32_t(f.read(Fixture::Battle+0x2730))==-2);}
 {Fixture f;assert(f.vm.memory.writeWord(Fixture::Args+16,2));f.press(1);assert(f.step()==1&&int32_t(f.read(Fixture::Battle+0x2730))==-1);}
 {Fixture f;assert(f.vm.memory.writeByte(Fixture::Unit+0x307,1));assert(f.step()==0&&f.step()==0&&f.step()==1&&f.read(Fixture::Battle+0x2730)==2);}
 {Fixture f;assert(f.vm.memory.writeWord(Fixture::Unit+0x27c,0x10));assert(f.step(false)==1&&f.read(Fixture::Battle+0x2730)==1);}
 {Fixture f;assert(f.vm.memory.writeWord(Fixture::Battle+0x24,0));assert(f.step(false)==1&&f.read(Fixture::Battle+0x2730)==1);}
 {Fixture f;assert(f.step()==0&&f.step()==0);assert(f.vm.memory.writeWord(Fixture::Battle+0x163f4,4));assert(!f.step()&&f.cursor.pending==0x80010bc4&&f.read(Fixture::Battle+0x271c)==2);assert(f.vm.memory.writeWord(Fixture::Battle+0x163f4,0));assert(f.step()==0&&f.read(Fixture::Battle+0x271c)==3);}
 {Fixture f;assert(f.vm.memory.writeWord(Fixture::Args+16,31));assert(!f.step()&&!f.cursor.started&&f.cursor.pending==0x8011a028);}
 {Fixture f;assert(f.step()==0);++f.e.id;assert(!f.step()&&f.read(Fixture::Battle+0x271c)==1);}
 {Fixture f;f.press();assert(f.step()==1);assert(f.vm.memory.writeWord(Fixture::Args,uint32_t(-30000000))&&f.vm.memory.writeWord(Fixture::Args+4,uint32_t(-29999999)));f.e.argumentCount=2;assert(nativeHarborStylishResult(f.vm,f.e)==2&&f.e.locals[0]==-1&&f.e.locals[1]==1);}
 {Fixture f;assert(!nativeHarborPointIncrementZeroDomain(f.vm));assert(f.vm.memory.registerRam(0x8041eb00,std::vector<uint8_t>(4))&&f.vm.memory.registerRam(0x89400000,std::vector<uint8_t>(0x100)));assert(f.vm.memory.writeWord(0x8041eb00,0x89400000));assert(nativeHarborPointIncrementZeroDomain(f.vm));assert(f.vm.memory.writeHalf(0x8940008c,1));assert(!nativeHarborPointIncrementZeroDomain(f.vm));}

}
