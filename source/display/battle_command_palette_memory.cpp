#include "battle_command_palette_memory.hpp"
namespace ttyd {
bool bindBattleCommandPaletteMemory(EventAddressSpace& memory, BattleCommandPalette& palette) {
    constexpr uint32_t Stage=0x8041eba0, Color=0x80416d20;
    // Install the unmapped bootstrap BSS byte first. In particular, do not
    // replace an existing live stage owner or silently reset its history.
    if(!memory.registerExternalRam(Stage,reinterpret_cast<uint8_t*>(&palette.state),1))return false;
    std::vector<uint8_t> original;
    if(!memory.extractRam(Color,4,original)) {
        memory.unregisterExternalRam(Stage);return false;
    }
    if(!memory.registerExternalRam(Color,palette.rgba.data(),4)) {
        memory.registerRam(Color,std::move(original));
        memory.unregisterExternalRam(Stage);return false;
    }
    return true;
}
}
