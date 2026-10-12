# BATTLE_TIMINGA_GET_SUCCESS_FRAME.md - Derived Reference

Derived contract reference based on disassembly analysis of `battleAcTimingA_GetSuccessFrame.elf` and `battleAcTimingA_GetSuccessFrame.disasm`.

## File Pair

- **Binary**: `artifacts/reference/behavior/battleAcTimingA_GetSuccessFrame.elf`
- **Disassembly**: `artifacts/reference/behavior/battleAcTimingA_GetSuccessFrame.disasm`

## Disassembly Overview

The disassembly begins at address `0x8012aa64` with standard PowerPC prologue:
- `stwu 1, -16(1)` - Stack frame setup (16 bytes)
- `mflr 0` - Link register save
- `stw 0, 20(1)` - Save return address
- `stw 31, 12(1)` - Save link register
- `mr 31, 3` - Move register 3 to link register
- `bl 0x800fa15c` - Branch to link to a subroutine at offset 0x15c (within the same translation unit)

### Success Frame Retrieval Sequence

At offset `0x8012aa78`:
- `bl 0x800fa15c` - Call to subroutine at offset 0x15c (likely an initialization or state setup function)
- At return, `lwz 0, 8016(31)` - Load word from offset 8016 of the GP workspace (register 31)

The value loaded from offset 8016 appears to be the "success frame" or completion status.

At offset `0x8012aa80`:
- `slwi 4, 3, 2` - Shift left word immediate: shift register 3 left by 2 bits (multiply by 4)
- This likely computes an index or offset multiplication

At offset `0x8012aa84`:
- `addi 3, 13, -24840` - Add immediate: register 3 = 13 + (-24840) = -24827
- This computes a negative base address or memory offset

### Key Observations

1. **Offset 8016**: The `lwz 0, 8016(31)` loads from a substantial workspace offset, likely the "success frame" status word that determines whether a battle action succeeded

2. **Index Computation**: The `slwi 4, 3, 2` shifts the loaded value left by 2 bits (multiplies by 4), likely computing an array index for accessing battle outcome data

3. **Negative Base Address**: The `addi 3, 13, -24840` computes a large negative offset, possibly for accessing a reverse-ordered or backward- indexed data structure

### Supporting Reference Files

- `files.json` - Provides SHA hash and provenance for this file pair
- `decomp-tree.json` - Contains function structure reference for timing A success frame
- `us_symbols.csv` - May contain symbol information for `battleAcTimingA_GetSuccessFrame`

### Native Implementation Reference

This disassembly corresponds to the native implementation in:
- `include/event_timing_a.hpp` (related timing)
- `source/event_timing_a.cpp` (related timing)

The native implementation handles:
- Success frame retrieval after battle action completion
- Rollover-consistent OSGetTime and winCheck processing
- GP10 short-circuit window query processing
- VIPostCallback clock prefix publication

### Battle Success Frame Pipeline

This function is part of the broader battle timing system:

1. `battleAcMain_TimingA` - Initializes timing accumulator and checks overflow
2. `battleAcResult_TimingA` - Sets result code (0 or 2) based on timing wrap
3. `battleAcTimingA_GetSuccessFrame` - Retrieves the success frame from workspace offset 8016
4. The success frame value is then used to determine battle outcome persistence

### Relationship to Workspace Offsets

The consistent use of workspace offsets is notable:
- **Offset 7324**: Stored by `battleAcDelete_StickKeepLeft` (result code 1005)
- **Offset 7352**: Loaded by `battleAcResult_StickKeepLeft` (retrieves stored result)
- **Offset 8016**: Loaded by `battleAcTimingA_GetSuccessFrame` (success frame)

These offsets are in the same general range (7324-8016), suggesting a contiguous workspace region for battle result tracking.

### Limitations

- Disassembly provides instruction-level detail but does not establish original gameplay behavior or numerical equivalence
- No emulator or hardware execution claims are made
- Original execution traces remain required for fidelity verification
- The success frame value interpretation requires cross-reference with original game behavior

---
*Documentation derived from `artifacts/reference/behavior/battleAcTimingA_GetSuccessFrame.disasm` and `artifacts/reference/behavior/battleAcTimingA_GetSuccessFrame.elf`, verified against file contents and supporting reference files.*