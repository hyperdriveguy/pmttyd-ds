# BATTLE_STICK_KEEP_LEFT_RESULT.md - Minimal Function Reference

Derived reference based on disassembly analysis of `battleAcResult_StickKeepLeft.elf` and `battleAcResult_StickKeepLeft.disasm`.

## File Pair

- **Binary**: `artifacts/reference/behavior/battleAcResult_StickKeepLeft.elf`
- **Disassembly**: `artifacts/reference/behavior/battleAcResult_StickKeepLeft.disasm`

## Disassembly Overview

This is a minimal function containing only 2 instructions at address `0x8012a174`:

### Instruction Sequence

1. `lwz 3, 7352(3)` - Load word from offset 7352 of the GP register context (register 3)
   - Loads a value from the game state workspace at a high offset (7352)
   - This is the same offset used in `battleAcResult_TimingA.disasm` for timing result storage

2. `blr` - Branch to Link Register
   - Return from the function, branching to the link register address

### Function Analysis

This is a minimal result retrieval function that:
- Loads a stored result value from the GP workspace at offset 7352
- Returns that value in register 3

The offset 7352 is the same as used in `battleAcResult_TimingA.disasm`, suggesting this function retrieves the timing/result that was previously stored.

### Supporting Reference Files

- `files.json` - Provides SHA hash and provenance for this file pair
- `decomp-tree.json` - Contains function structure reference for battle result retrieval
- `us_symbols.csv` - May contain symbol information for `battleAcResult_StickKeepLeft`

### Relationship to Other Battle Functions

This function forms part of the stick keep-left battle pipeline:

1. `battleAcMain_StickKeepLeft` - Initializes/processes stick input
2. `battleAcDelete_StickKeepLeft` - Stores result code (1005) to workspace offset 7324
3. `battleAcResult_StickKeepLeft` - Retrieves stored result from workspace offset 7352

The pipeline flows: input → processing → result storage → result retrieval.

### Native Implementation Reference

This disassembly corresponds to the native implementation in the battle result retrieval path, related to:
- Battle pad input result retrieval
- Integration with `BATTLE_INPUT_CADENCE_AND_SNAPSHOTS.md` battle cadence system
- The consistent use of workspace offsets 7324 and 7352 for result storage and retrieval

### Limitations

- Disassembly provides minimal instruction-level detail (2 instructions)
- Does not establish original gameplay behavior or numerical equivalence beyond the observed pattern
- No emulator or hardware execution claims are made
- The observed offset 7352 connection to `battleAcResult_TimingA` is notable but requires further verification

---
*Documentation derived from `artifacts/reference/behavior/battleAcResult_StickKeepLeft.disasm` and `artifacts/reference/behavior/battleAcResult_StickKeepLeft.elf`, verified against file contents.*