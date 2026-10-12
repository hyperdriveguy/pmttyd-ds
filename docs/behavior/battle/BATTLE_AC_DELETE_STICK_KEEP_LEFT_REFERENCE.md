# BATTLE_STICK_KEEP_LEFT_DELETE.md - Minimal Function Reference

Derived reference based on disassembly analysis of `battleAcDelete_StickKeepLeft.elf` and `battleAcDelete_StickKeepLeft.disasm`.

## File Pair

- **Binary**: `artifacts/reference/behavior/battleAcDelete_StickKeepLeft.elf`
- **Disassembly**: `artifacts/reference/behavior/battleAcDelete_StickKeepLeft.disasm`

## Disassembly Overview

This is a minimal function containing only 3 instructions at address `0x80129fc0`:

### Instruction Sequence

1. `li 0, 1005` - Load immediate 1005 (decimal) into register 0
   - This is likely a battle ID or return code, specifically 1005

2. `stw 0, 7324(3)` - Store word to offset 7324 of the GP register context (register 3)
   - Stores the value 1005 to the GP workspace at offset 7324
   - This likely records the battle result or cleanup marker

3. `blr` - Branch to Link Register
   - Return from the function, branching to the link register address

### Function Analysis

This is a minimal teardown/cleanup function that:
- Records a battle result code of 1005 to the GP workspace
- Returns control to the caller

The value 1005 may correspond to a specific battle outcome or deletion marker in the battle system.

### Supporting Reference Files

- `files.json` - Provides SHA hash and provenance for this file pair
- `decomp-tree.json` - Contains function structure reference for battle deletion
- `us_symbols.csv` - May contain symbol information for `battleAcDelete_StickKeepLeft`

### Native Implementation Reference

This disassembly corresponds to the native implementation in the battle action cleanup path, related to:
- Battle action command deletion/cleanup
- Stick keep-left battle result recording
- Integration with `BATTLE_INPUT_CADENCE_AND_SNAPSHOTS.md` battle pad update order

### Relationship to battleAcMain_StickKeepLeft

This function appears to be the cleanup/teardown counterpart to `battleAcMain_StickKeepLeft`:
- `battleAcMain_StickKeepLeft`: Initializes/processes stick keep-left input
- `battleAcDelete_StickKeepLeft`: Cleans up/stores result after stick keep-left processing

The main function processes/initializes, and this function records/stores the result.

### Limitations

- Disassembly provides minimal instruction-level detail (3 instructions)
- Does not establish original gameplay behavior or numerical equivalence beyond the observed pattern
- No emulator or hardware execution claims are made
- The value 1005 is observed but its full semantics require original execution context

---
*Documentation derived from `artifacts/reference/behavior/battleAcDelete_StickKeepLeft.disasm` and `artifacts/reference/behavior/battleAcDelete_StickKeepLeft.elf`, verified against file contents.*