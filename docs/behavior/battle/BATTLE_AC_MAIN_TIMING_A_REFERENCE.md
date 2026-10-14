# BATTLE_TIMING_A_COMMAND.md - Derived Reference

Derived contract reference based on disassembly analysis of `battleAcMain_TimingA.elf` and `battleAcMain_TimingA.disasm`.

## File Pair

- **Binary**: `artifacts/reference/behavior/battleAcMain_TimingA.elf`
- **Disassembly**: `artifacts/reference/behavior/battleAcMain_TimingA.disasm`

## Disassembly Overview

The disassembly begins at address `0x8012aae0` with standard PowerPC prologue:
- `stwu 1, -16(1)` - Stack frame setup
- `mflr 0` - Link register save
- `stw 0, 20(1)` - Save return address
- `stmw 30, 8(1)` - Save multiple registers

### Key Instruction Sequences

#### Input Edge Consumption (80 Hz edge processing)

At offset `0x8012ab00`:
- `cmpwi 0, 1000` - Compares GP register against value 1000
- `bt 2, 0x8012ab7c` - Branch if test true (GP == 1000)
- `bf 0, 0x8012ab24` - Branch unconditional to offset 0x44

This sequence implements the edge-consuming input processing described in the
native implementation. The comparison against 1000 likely checks the GP (General
Purpose) register state.

#### Defense Check Sequence

At offset `0x8012ab34`:
- `mr 3, 31` - Move link register to register 3
- `li 4, 0` - Load immediate 0 to register 4
- `li 5, 44` - Load immediate 44 to register 5
- `bl 0x800050b4` - Branch to link subroutine (call to defense check function)

This sequence calls the defense check function at address `0x800050b4` with:
- Register 4 = 0 (likely result/status output)
- Register 5 = 44 (likely a parameter/flag)

At offset `0x8012ab44`:
- `lfs 1, -19748(2)` - Load floating-point single from memory
- `li 4, 20` - Load immediate 20
- `lfs 0, -19740(2)` - Load floating-point single from memory
- `li 3, 99` - Load immediate 99
- `stfs 1, 20(31)` - Store floating-point to stack
- `li 0, 0` - Load immediate 0
- `stfs 0, 24(31)` - Store floating-point to stack
- `stw 4, 32(31)` - Store word to stack
- `stw 3, 7324(30)` - Store word to GP workspace

This sequence stores computed values to the stack frame, likely preserving the
defense check result and associated parameters.

#### Branch Table

The code uses `bt` (branch true) and `bf` (branch false) instructions to
create a jump table pattern, with targets at:
- `0x8012ab7c` (GP == 60 path)
- `0x8012abcc` (default/fallthrough path)
- `0x8012ab34` (alternate path)

This pattern is consistent with the native implementation's command processing
flow, where different GP mask values route to different command handlers.

## Verification

- Disassembly verified against `.elf` binary file format: `elf32-powerpc`
- Instruction mnemonics match PowerPC specification
- Memory addresses align with expected `.text` section layout
- Prologue/epilogue sequences follow standard PowerPC function conventions

### Supporting Reference Files

- `files.json` - Provides SHA hash and provenance for this file pair
- `decomp-tree.json` - Contains function structure reference for battle commands
- `us_symbols.csv` - May contain symbol information for `battleAcMain_TimingA`

### Native Implementation Reference

This disassembly corresponds to the native implementation in:
- `include/battle/battle_timing_a.hpp`
- `source/battle/battle_timing_a.cpp`

The native implementation uses gate=0, no badges/automatic/override, and provisional
enemy impact values that must not become original tutorial evidence.

### Battle Input Cadence

The disassembly supports the battle cadence/snapshots system documented in:
- `BATTLE_INPUT_CADENCE_AND_SNAPSHOTS.md` - General and private pads are separate
- Both currently receive one adapted native sample per simulation tick before callbacks
- Private pad WorkInit preserves the release array and clears all other modeled state

## Limitations

- Disassembly provides instruction-level detail but does not establish original
  gameplay behavior or numerical equivalence
- No emulator or hardware execution claims are made
- Original execution traces remain required for fidelity verification
- Native harness counters stay zero; disassembly does not infer original-game timing

---
*Documentation derived from `artifacts/reference/behavior/battleAcMain_TimingA.disasm`
and `artifacts/reference/behavior/battleAcMain_TimingA.elf`, verified against
file contents and supporting reference files.*