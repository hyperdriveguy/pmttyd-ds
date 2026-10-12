# BATTLE_STICK_KEEP_LEFT.md - Derived Reference

Derived contract reference based on disassembly analysis of `battleAcMain_StickKeepLeft.elf` and `battleAcMain_StickKeepLeft.disasm`.

## File Pair

- **Binary**: `artifacts/reference/behavior/battleAcMain_StickKeepLeft.elf`
- **Disassembly**: `artifacts/reference/behavior/battleAcMain_StickKeepLeft.disasm`

## Disassembly Overview

The disassembly begins at address `0x8012a17c` with a standard PowerPC prologue, featuring a larger stack frame of 48 bytes (compared to the 16-byte frame in `battleAcMain_TimingA`):
- `stwu 1, -48(1)` - Stack frame setup
- `mflr 0` - Link register save
- `stw 0, 52(1)` - Save return address at offset 52
- `stmw 23, 12(1)` - Save multiple registers (23 through 37, 16 bytes)
- `mr 24, 3` - Move register 3 to register 24 (likely saving a saved register)
- `li 26, 0` - Load immediate 0 to register 26
- `addi 29, 24, 8012` - Set up a base address at `0x8012` from register 24
- `addi 28, 24, 7968` - Set up another base at `0x1F40` (`0x1F40` = 8000 decimal) from register 24
- `lwz 3, 7312(3)` - Load word from offset 7312 of the GP register context

### Key Initialization Sequence

At offset `0x8012a19c`:
- `lwz 3, 7312(3)` - Load GP-relative word, likely retrieving a game state pointer or command index from the workspace

This is followed by more sequences that set up the stick keep-left battle action processing.

### Stick Keep-Left Input Processing

The function processes the left stick keep input, which is part of the battle input cadence system. Key observations:

1. **Larger Stack Frame (48 bytes vs 16 bytes)**: Suggests more state needs to be preserved, likely because this function handles both input processing and result computation

2. **Register 24 Save**: `mr 24, 3` saves the third register (likely containing the GP pointer or command data) to register 24 for later use

3. **Base Address Setup**: The `addi` instructions set up two constants:
   - `0x8012` (32770 decimal) - likely an code/data section base
   - `8000` (0x1F40) - possibly a constant or threshold value

4. **GP-Relative Load**: `lwz 3, 7312(3)` loads from offset 7312 of the GP context, which is a substantial offset indicating deeply nested state access

### Supporting Reference Files

- `files.json` - Provides SHA hash and provenance for this file pair
- `decomp-tree.json` - Contains function structure reference for stick keep-left processing
- `us_symbols.csv` - May contain symbol information for `battleAcMain_StickKeepLeft`

### Native Implementation Reference

This disassembly corresponds to the native implementation in:
- `include/battle_timing_a.hpp` (related timing/stick processing)
- `source/battle_timing_a.cpp` (related timing/stick processing)

The native implementation handles:
- Battle pad input with stick keep-left detection
- One adapted native sample per simulation tick before callbacks
- Private pad WorkInit preserving the release array and clearing all other modeled state
- General and private pads being separate

### Battle Input Cadence Context

This function is part of the battle input system documented in:
- `BATTLE_INPUT_CADENCE_AND_SNAPSHOTS.md` - General and private pads are separate
- Both currently receive one adapted native sample per simulation tick before callbacks
- Private pad WorkInit preserves the release array and clears all other modeled state after every defense check
- Native guard result is evaluated at impact, not first press

### Differences from battleAcMain_TimingA

| Aspect | battleAcMain_TimingA | battleAcMain_StickKeepLeft |
|--------|---------------------|---------------------------|
| Stack frame | 16 bytes | 48 bytes |
| Prologue pattern | Standard 16-byte frame | Extended 48-byte frame |
| Focus | Timing/overflow detection | Stick input processing and battle action |
| Key operations | GP null check, comparison | Stick input, command dispatch |
| Related subsystems | Event timing, battle cadence | Battle input, pad handling |

### Limitations

- Disassembly provides instruction-level detail but does not establish original gameplay behavior or numerical equivalence
- No emulator or hardware execution claims are made
- Original execution traces remain required for fidelity verification
- The larger stack frame suggests more complex state than TimingA

---
*Documentation derived from `artifacts/reference/behavior/battleAcMain_StickKeepLeft.disasm` and `artifacts/reference/behavior/battleAcMain_StickKeepLeft.elf`, verified against file contents and supporting reference files.*