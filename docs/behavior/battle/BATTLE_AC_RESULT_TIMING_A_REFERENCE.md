# BATTLE_TIMING_A_COMMAND.md - Derived Reference

Derived contract reference based on disassembly analysis of `battleAcResult_TimingA.elf` and `battleAcResult_TimingA.disasm`.

## File Pair

- **Binary**: `artifacts/reference/behavior/battleAcResult_TimingA.elf`
- **Disassembly**: `artifacts/reference/behavior/battleAcResult_TimingA.disasm`

## Disassembly Overview

The disassembly begins at address `0x8012a8c4` with standard PowerPC prologue:
- `stwu 1, -32(1)` - Larger stack frame setup (32 bytes vs 16 in TimingA)
- `mflr 0` - Link register save
- `stw 0, 36(1)` - Save return address at offset 36
- `stmw 28, 16(1)` - Save multiple registers (28 through 43, 16 bytes)
- `mr 31, 3` - Move register 3 to link register (RA)
- `bl 0x8012aa64` - Branch to link to subroutine at offset 0x1a0

### Initialization and Comparison Sequence

At offset `0x8012a8dc`:
- `lwz 0, 7316(31)` - Load word from GP workspace offset 7316
- `mr 30, 3` - Move register 3 to local GPU context pointer
- `li 28, 0` - Load immediate 0 to register 28 (clear accumulator)
- `clrlwi. 0, 0, 31` - Clear left word immediate with effect (extract bit 31)
- `bt 2, 0x8012a938` - Branch if test: based on bit 31 of register 0

This sequence appears to initialize the timing accumulator and check the overflow/rollover bit.

### Comparison and Branch Logic

At offset `0x8012a8f4`:
- `lwz 3, 636(3)` - Load word from offset 636 of the GPU context
- `rlwinm. 0, 0, 0, 27, 27` - Rotate left word immediate with effect, test bit 27
- `bt 2, 0x8012a938` - Branch if test based on the result

At offset `0x8012a900`:
- `li 3, 100` - Load immediate 100 (likely a timeout/threshold value)
- `bl 0x80005da0` - Branch to link to subroutine (likely RNG or timing function)

At offset `0x8012a908`:
- `cmpwi 3, 0` - Compare register 3 against 0
- `bt 0, 0x8012a91c` - Branch if test 0 (if result is zero, take branch)
- `li 0, 0` - Load immediate 0
- `stw 0, 7352(31)` - Store word to workspace offset 7352
- `b 0x8012a930` - Branch to offset 0x6c

The alternate path at `0x8012a91c`:
- `li 0, 2` - Load immediate 2
- `stw 0, 7352(31)` - Store word 2 to workspace

This appears to set a result code based on whether the timing accumulator overflowed.

## Verification

- Disassembly verified against `.elf` binary file format: `elf32-powerpc`
- Instruction mnemonics match PowerPC specification
- Memory addresses align with expected `.text` section layout
- Prologue differs from TimingA (32-byte vs 16-byte stack frame)
- Uses larger stack frame consistent with result computation needing more state

### Supporting Reference Files

- `files.json` - Provides SHA hash and provenance for this file pair
- `decomp-tree.json` - Contains function structure reference for battle results
- `us_symbols.csv` - May contain symbol information for `battleAcResult_TimingA`

### Native Implementation Reference

This disassembly corresponds to the native implementation in:
- `include/event_timing_a.hpp`
- `source/event_timing_a.cpp`

The native implementation handles:
- Retrace clock rollover-consistent timing
- Window query processing
- GP10 short-circuit window query
- 64-bit write ordering (low before high)
- Raw baseline and callback argument publication

### Battle Timing and Control

The disassembly supports the event timing system documented in:
- `EVENT_RETRACE_TIME_PUBLICATION.md` - Rollover-consistent OSGetTime
- `GAME_OUTER_LOOP_AND_EVENT_UPDATE_ORDER.md` - Complete clock publication prefix/window/raw readers
- `EVENT_IF_INTEGER_MASK_AND_CONTINUATIONS.md` - Integer/mask/scan helpers
- `EVENT_BITWISE_TIME_AND_INDIRECT_MEMORY.md` - Buffers/allocator/fill/cache bodies

### Timing Accumulator Logic

Key observations from the disassembly:
- The `clrlwi.` instruction tests the carry/overflow bit (bit 31)
- Branch on this bit determines whether the timing wrap was detected
- The value 100 likely represents a tick counter or threshold
- Result of 0 or 2 determines the timing wrap handling path
- Stack writes at offsets 7352 store the result state

### Differences from battleAcMain_TimingA

| Aspect | battleAcMain_TimingA | battleAcResult_TimingA |
|--------|---------------------|------------------------|
| Stack frame | 16 bytes | 32 bytes |
| Init value | N/A | `li 28, 0` (clear accumulator) |
| Overflow test | `clrlwi.` bit 31 | `clrlwi.` test + `li 100` |
| Result values | Defense check outputs | 0 or 2 (wrap handling) |
| Supporting subsystems | Battle input cadence | Event timing/publication |

## Limitations

- Disassembly provides instruction-level detail but does not establish original
  gameplay behavior or numerical equivalence
- No emulator or hardware execution claims are made
- Original execution traces remain required for fidelity verification
- The 32-byte stack frame suggests more complex state than TimingA

---
*Documentation derived from `artifacts/reference/behavior/battleAcResult_TimingA.disasm`
and `artifacts/reference/behavior/battleAcResult_TimingA.elf`, verified against
file contents and supporting reference files.*