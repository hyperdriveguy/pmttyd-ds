# BATTLE_MAIN.md - Derived Reference

Derived contract reference based on disassembly analysis of `battleMain.elf` and `battleMain.disasm`.

## File Pair

- **Binary**: `artifacts/reference/behavior/battleMain.elf`
- **Disassembly**: `artifacts/reference/behavior/battleMain.disasm`

## Disassembly Overview

The disassembly begins at address `0x800f904c` with standard PowerPC prologue:
- `stwu 1, -16(1)` - Stack frame setup (16 bytes)
- `mflr 0` - Link register save
- `stw 0, 20(1)` - Save return address
- `stw 31, 12(1)` - Save link register
- `lwz 31, 7280(13)` - Load GP pointer from offset 7280 of frame pointer register 13
- `cmplwi 31, 0` - Compare GP against 0 (null check)

### Initialization and Null Check

At offset `0x800f9060`:
- `cmpwi 31, 0` - Compare GP register against 0
- `bt 2, 0x800f9080` - Branch if test: if GP != 0, take branch to offset 0x80
- If GP == 0: `bl 0x800f8e28` - Branch to link to some initialization/fallback function
- Then `cmpwi 3, 0` - Compare register 3 against 0

This appears to be a null check on the GP (General Purpose) pointer at initialization. If GP is null, the code branches to a fallback path at `0x800f8e28`.

### Main Battle Entry Point

The function at `0x800f904c` appears to be the main battle entry point, setting up the game state and branching based on whether the GP pointer is valid.

### Supporting Reference Files

- `files.json` - Provides SHA hash and provenance for this file pair
- `decomp-tree.json` - Contains function structure reference for battle main
- `us_symbols.csv` - May contain symbol information for `battleMain`

### Native Implementation Reference

This disassembly corresponds to the native implementation in:
- `include/battle_timing_a.hpp` (related timing)
- `source/battle_timing_a.cpp` (related timing)
- The battle main entry likely interfaces with `Game::battle` in `source/game.cpp`

### Battle Initialization Sequence

Key observations from the disassembly:
1. **GP Null Check**: The `cmplwi 31, 0` / `bt` pattern is a standard null check on the game pointer
2. **Fallback Path**: If GP is null (0), branches to `0x800f8e28` for fallback initialization
3. **GP Pointer Loading**: `lwz 31, 7280(13)` loads the GP from the stack frame, preserved from the prologue's `stwu 1, -16(1)`
4. **Comparison Against Zero**: The immediate comparison `cmpwi 31, 0` is the first substantive check in the function

### Function Sub-range and Purpose

Based on the disassembly structure, `battleMain` likely:
- Initializes the battle system state
- Performs null safety checks on the game state pointer
- Branches to appropriate initialization or error handling based on GP validity
- Serves as the entry point for battle phase processing

### Limitations

- Disassembly provides instruction-level detail but does not establish original gameplay behavior or numerical equivalence
- No emulator or hardware execution claims are made
- Original execution traces remain required for fidelity verification
- The null check pattern is standard but the specific fallback behavior requires original execution observation

### Related Disassembly Files

Other related battle disassemblies in the reference directory:
- `battleAcMain_TimingA.disasm` - Timing A command processing (documented in `BATTLE_AC_COMMAND_MD.md`)
- `battleAcResult_TimingA.disasm` - Result timing A (documented in `BATTLE_RESULT_TIMING_A_MD.md`)
- `BattleActionCommandManager.disasm` - Action command manager (documented in `BATTLE_ACTION_COMMAND_MANAGER.md`)

---
*Documentation derived from `artifacts/reference/behavior/battleMain.disasm` and `artifacts/reference/behavior/battleMain.elf`, verified against file contents and supporting reference files.*