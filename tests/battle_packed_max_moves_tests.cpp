#include "harbor_tutorial_packed.hpp"
#include <cassert>
#include <iostream>
using namespace ttyd;
int main(){EventNativeRuntime vm{0x81000000,{}};// Fixture supplies the verified original stage-floor constants used by provision.
 assert(vm.memory.registerRam(0x80422b44,{0x42,0xb4,0,0,0xc2,0x48,0,0}));HarborTutorialPacked packed(vm);assert(packed.provision());assert(vm.memory.registerRam(0x82000000,std::vector<uint8_t>(8)));auto w=[&](uint32_t a,uint32_t v){assert(vm.memory.writeWord(a,v));};auto byte=[&](uint32_t a,uint8_t v){assert(vm.memory.writeByte(a,v));};auto read=[&](uint32_t a){uint8_t v;assert(vm.memory.readByte(a,v));return v;};const auto b=HarborTutorialPacked::Battle,u=HarborTutorialPacked::Units;w(b+0x20,u);w(u+8,220);byte(u+0x21,1);byte(u+0x22,1);byte(u+0x23,77);w(0x82000000,0xfffffffe);w(0x82000004,0);EventEntry event;event.owner=0;event.arguments=0x82000000;event.argumentCount=2;int32_t result=-1;assert(packed.invoke(event,0x8010fce0,1,result) && result==2 && read(u+0x21)==0 && read(u+0x22)==0 && read(u+0x23)==77);w(0x82000004,0xffffffff);assert(packed.invoke(event,0x8010fce0,1,result) && read(u+0x21)==255 && read(u+0x22)==255);const auto other=u+0x1000;w(b+0x20+63*4,other);byte(other+0x21,1);byte(other+0x22,1);w(0x82000004,2);event.owner=63;assert(packed.invoke(event,0x8010fce0,1,result) && read(other+0x21)==2 && read(other+0x22)==1);
 std::cout<<"Original packed max-move event resolves system and nonvisual owner63 with signed clamp, independent of visual slots\n";
}
