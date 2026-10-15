#pragma once
#include "battle_display_footer.hpp"
#include "event_address_space.hpp"
namespace ttyd {
// Attach the process-owned original color cycle to an already imported VM.
// Replaces only the four imported DATA bytes; the BSS stage must be unmapped.
// The palette and address space must remain alive together: no detach/reset is
// implied by a scene or display pass. Failure preserves existing mappings.
bool bindBattleCommandPaletteMemory(EventAddressSpace&, BattleCommandPalette&);
}
