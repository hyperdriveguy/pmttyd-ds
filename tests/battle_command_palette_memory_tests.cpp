#include "battle_command_palette_memory.hpp"
#include "event_native_runtime.hpp"
#include <cassert>
using namespace ttyd;
int main(){
    EventNativeRuntime first(0x81000000,{}),second(0x81000000,{});
    BattleCommandPalette palette;
    // Imported DATA neighbors survive replacement of the exact four-byte span.
    assert(first.memory.registerRam(0x80416d1f,{0x12,0xff,0xc0,0xc0,0xcf,0x34}));
    assert(second.memory.registerRam(0x80416d20,{0xff,0xc0,0xc0,0xcf}));
    assert(bindBattleCommandPaletteMemory(first.memory,palette));
    palette.step();uint32_t color=0;uint8_t byte=0;
    assert(first.memory.readWord(0x80416d20,color) && color==0xffc1c0cf);
    assert(first.memory.readByte(0x80416d1f,byte) && byte==0x12);
    assert(first.memory.readByte(0x80416d24,byte) && byte==0x34);
    // A second battle/VM sees current process history, not imported boot bytes.
    assert(bindBattleCommandPaletteMemory(second.memory,palette));
    assert(second.memory.readWord(0x80416d20,color) && color==0xffc1c0cf);
    assert(second.memory.writeByte(0x80416d23,0x7e) && palette.rgba[3]==0x7e);
    assert(first.memory.readByte(0x80416d23,byte) && byte==0x7e);
    assert(first.memory.writeByte(0x8041eba0,0xff) && palette.state==-1);
    palette.step();assert(second.memory.readWord(0x80416d20,color) && color==0xffc1c07e);
    // Rebinding cannot replace an existing stage or erase the shared history.
    BattleCommandPalette other;
    assert(!bindBattleCommandPaletteMemory(first.memory,other));
    assert(palette.state==-1 && palette.rgba[3]==0x7e);
    EventNativeRuntime missing(0x81000000,{});
    assert(!bindBattleCommandPaletteMemory(missing.memory,other));
    assert(!missing.memory.readByte(0x8041eba0,byte));
    assert(missing.memory.registerRam(0x80416d20,{1,2,3,4}));
    assert(bindBattleCommandPaletteMemory(missing.memory,other));
}
