#include <memory>
#include "battle_command_pose_native.hpp"
#include "battle_event_callbacks.hpp"
#include "battle_command_pose.hpp"
#include "battle_packed_pose_lookup.hpp"
#include "event_native_runtime.hpp"
#include "harbor_tutorial_packed.hpp"
#include <cassert>
#include <string>
using namespace ttyd;
int main(){
 EventScriptAssets scripts;assert(scripts.load("romfs/harbor-tutorial-events.pmevt"));
 auto vm=std::make_unique<EventNativeRuntime>(0x81000000,EventNativePlatform{});auto packed=std::make_unique<HarborTutorialPacked>(*vm);
 assert(vm->memory.importScripts(scripts));assert(packed->provision());
 HarborTutorialActor mario;mario.id=1;mario.kind=222;mario.hp=mario.maxHp=10;
 assert(packed->registerActor(mario));
 const auto unit=HarborTutorialPacked::Units+0x1000,part=HarborTutorialPacked::Parts+0x1000;
 auto read=[&](uint32_t a){uint32_t n=0;assert(vm->memory.readWord(a,n));return n;};
 const auto name=packedBattlePoseName(part,67,nativeBattleActorServices(*vm).memory);assert(name);
 std::string original;for(unsigned i=0;i<64;++i){uint8_t b=0;assert(vm->memory.readByte(name+i,b));if(!b)break;original+=char(b);}
 assert(original=="M_C_3"); // Actual original type67 table lookup, no substitute input string.
 assert(vm->memory.writeWord(part+0x204,0x400002ff));
 assert(nativeBattleCommandPose(*vm,unit));assert(read(part+0x204)==0x400000e7);
 for(unsigned i=0;i<=original.size();++i){uint8_t b=0;assert(vm->memory.readByte(part+0x1c4+i,b));assert(b==(i==original.size()?0:uint8_t(original[i])));}
 // Source flag branches are complete without GPU/animation acknowledgment.
 assert(vm->memory.writeWord(unit+0x138,0x10000000));assert(vm->memory.writeWord(part+0x204,0xffffffff));
 assert(nativeBattleCommandPose(*vm,unit));assert(read(part+0x204)==0xffffff69);
 // Actual source-name admission failure occurs before copy or flags.
 assert(vm->memory.writeWord(unit+0x138,0));const auto table=read(part+0x1bc);uint32_t row=table;
 for(;read(row)!=67&&read(row)!=0xffffffff;row+=8){}
 assert(read(row)==67);assert(vm->memory.writeWord(row+4,0x5fffffff));
 assert(vm->memory.writeWord(part+0x204,0x12345678));
 assert(!nativeBattleCommandPose(*vm,unit)&&read(part+0x204)==0x12345678);
 return 0;
}
