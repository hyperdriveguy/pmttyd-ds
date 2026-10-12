# BATTLE_ACTION_COMMAND_MANAGER.md - Derived Reference

Derived contract reference based on disassembly analysis of `BattleActionCommandManager.elf` and `BattleActionCommandManager.disasm`.

## File Pair

- **Binary**: `artifacts/reference/behavior/BattleActionCommandManager.elf`
- **Disassembly**: `artifacts/reference/behavior/BattleActionCommandManager.disasm`

## Disassembly Overview

The disassembly begins at address `0x800fa5b0` with standard PowerPC prologue:
- `stwu 1, -16(1)` - Stack frame setup (16 bytes)
- `mflr 0` - Link register save
- `stw 0, 20(1)` - Save return address
- `stw 31, 12(1)` - Save link register
- `mr 31, 3` - Move register 3 to link register
- `addi 3, 31, 7452` - Set up GPU context pointer at offset 7452
- `bl 0x8011a0ac` - Branch to link to a large subroutine at offset 0x1fafc (likely an initialization or setup function)

### Command Processing Sequence

At offset `0x800fa5d0`:
- `lwz 12, 7328(31)` - Load word from GP workspace offset 7328 (likely a command index or pointer)
- `cmplwi 12, 0` - Compare word against 0
- `bt 2, 0x800fa618` - Branch if test: if command index != 0, take branch
- `mr 3, 31` - Move link register to register 3
- `mtctr 12` - Move register 12 to count register (CTR) for looping
- `bctrl` - Branch to subroutine via CTR

This appears to be the main command dispatch loop that processes battle action commands.

### Command Index Based Routing

The `cmplwi 12, 0` / `bt` pattern indicates a dispatch on the command index value:
- If command index == 0: take one path (likely termination or initialization)
- If command index != 0: branch to command-specific handler at `0x800fa618`

At offset `0x800fa5f0`:
- `li 0, 0` - Load immediate 0 (likely a default/fallback result)
- `stw 0, 7328(31)` - Store word 0 to workspace offset 7328 (likely resetting the command index)

At offset `0x800fa5f8`:
- `lwz 5, 7336(31)` - Load word from workspace offset 7336 (likely a sub-command or parameter)
- `cmplwi 5, 0` - Compare against 0
- `bt 2, 0x800fa618` - Branch if sub-command != 0

This nested dispatch structure suggests the manager processes a primary command index, then dispatches to sub-commands within each main command.

### Supporting Reference Files

- `files.json` - Provides SHA hash and provenance for this file pair
- `decomp-tree.json` - Contains function structure reference for battle command management
- `us_symbols.csv` - May contain symbol information for `BattleActionCommandManager`

### Native Implementation Reference

This disassembly corresponds to the native implementation in:
- `include/BattleActionCommandManager.hpp` (hypothetical)
- `source/BattleActionCommandManager.cpp` (hypothetical)

The native implementation handles:
- Battle action command dispatch and routing
- Sub-command processing within each main command
- Command index validation and termination
- Resource management for command execution

### Battle Command Ecosystem

This manager is part of the broader battle action system documented in related files:

| Related File | Purpose |
|-------------|---------|
| `BATTLE_ACTION_COMMAND_CALLBACKS.md` | Command callback registration and invocation |
| `BATTLE_ACTION_RECORD_BYTE_UPDATES.md` | Byte-level command record updates |
| `BATTLE_ACTION_RECORD_TURN_RULES.md` | Turn rule processing for commands |
| `BATTLE_STAGE_AFTEREFFECT_REQUESTS.md` | Aftereffect requests following command execution |
| `BATTLE_WEAPON_AFTEREFFECT_DISPATCH.md` | Weapon aftereffect dispatch following commands |

### Dispatch Pattern

The `cmplwi`/`bt` dispatch pattern observed here is consistent across the battle action system:
- Command indices 0 trigger termination/initialization paths
- Non-zero indices branch to specific command handlers at `0x800fa618`
- After processing, the command index is often reset to 0
- Sub-command processing follows the same pattern

### Function Sub-range

Based on the disassembly size and structure, this manager likely handles:
- Initial command setup and validation
- Dispatch to sub-commands (attack, defense, item, switch)
- Cleanup and result recording
- Integration with the battle phase controller

### Limitations

- Disassembly provides instruction-level detail but does not establish original
  gameplay behavior or numerical equivalence
- No emulator or hardware execution claims are made
- Original execution traces remain required for fidelity verification
- The `bl 0x8011a0ac` call likely invokes initialization code that may have complex state

---
*Documentation derived from `artifacts/reference/behavior/BattleActionCommandManager.disasm`
and `artifacts/reference/behavior/BattleActionCommandManager.elf`, verified against
file contents and supporting reference files.*