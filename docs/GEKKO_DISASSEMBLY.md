# Correct GameCube paired-single disassembly

The research inspection tool now corrects LLVM's generic PowerPC output through
`tools/gekko_disasm.py`. LLVM23.1.1 misdecodes GameCube instruction encodings as
newer vector operations even with `--mcpu=750` and vector features disabled.
A CPU-selection flag alone did not fix the observed output.

## Future inspection

Existing commands continue to work:

```sh
python3 tools/inspect_dol_function.py PSMTXScale --symbols artifacts/reference/us_symbols.csv --output artifacts/reference/behavior/PSMTXScale.elf > artifacts/reference/behavior/PSMTXScale.disasm
```

The ELF contains the original bytes. LLVM decodes ordinary instructions;the
Gekko decoder replaces affected instruction text using the displayed big-endian
word. No original instruction is executed. Original listings and ELFs remain
ignored rather than distributed or added to source history.

## Existing listing repair

```sh
python3 tools/gekko_disasm.py --check artifacts/reference
python3 tools/gekko_disasm.py --in-place artifacts/reference
```

`--check` never writes and returns1 if corrections are needed. `--in-place`
repairs files;rerunning is idempotent. A single file without either flag prints
corrected text to stdout. Counts go to stderr. Directory mode recursively scans
`.disasm`,`.annotated` and `.txt` files with the `elf32-powerpc` listing header.
Explicit files also support GNU-style compact instruction words. Binary files
are rejected. Unrelated reference text and empty failed-inspection outputs are
not rewritten. Directories supplied here must contain GameCube PowerPC listings;
this decoder is not intended for modern PowerPC binaries.

Addresses,byte formatting,line endings and human `#`/`;` comments are preserved.
Ordinary instruction lines are unchanged. Unrecognized/reserved encodings in the
Gekko opcode space print `.long 0x...` rather than a misleading modern mnemonic;
the report counts them. A clean check means no text correction is needed,not
that all instructions in an arbitrary file are valid or fully understood.

## Encoding scope and operands

The decoder handles primary opcode4 paired arithmetic/comparison/move/merge,
indexed quantized loads/stores and `dcbz_l`;primary opcodes56/57/60/61 are the
immediate quantized load/store variants. It preserves arithmetic Rc as a dotted
mnemonic and checks relevant unused/reserved operand bits.

Floating registers use `fN`,general registers `rN`,comparison fields `crN`.
Four-operand paired arithmetic uses assembler order **D,A,C,B**,including
`ps_sum0/1`,not the physical bit-field order D,A,B,C. Quantized memory syntax
ends in **W,I**. Immediate offsets are signed12-bit;indexed variants decode W/I
from different positions and distinguish update forms. I names GQR0..7 by
number. Mnemonics establish operand encoding;actual GQR/FPSCR state and runtime
numeric semantics still require separate evidence.

References consulted2026-10-12:

- [GNU PowerPC opcode definitions](https://github.com/RTEMS/sourceware-mirror-binutils-gdb/blob/master/opcodes/ppc-opc.c),PPCPS entries:encoding masks and standard assembler operand order. Upstream GPL-3.0-or-later.
- [Dolphin Gekko disassembler](https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/Common/GekkoDisassembler.cpp) and [instruction tables](https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/Core/PowerPC/PPCTables.cpp):GameCube opcode selection and quantized fields. Upstream license notices apply to those sources.

This is an independently authored field decoder;no upstream implementation was
copied or linked. Instruction encoding facts were cross-checked against those
primary references and synthetic tests. It is not a complete replacement for
LLVM or a GameCube emulator.

## Repair validation on the local research collection

On2026-10-12,the scan covered388 nonempty PowerPC listings and61,863 instruction
rows. It repaired354 paired instructions in47 files with zero unrecognized
paired encodings. All386 existing inspection ELF hashes were unchanged. Every
changed line retained its complete address/byte prefix and line ending;all
unrelated lines were identical. The repeated check reported zero changes.
`artifacts/reference/behavior/gekko-repair-report.json` records changed paths
and the unchanged-ELF count;it remains ignored.

Ten decoder regression tests cover arithmetic forms/record bits,comparison and
merge/move/cache forms,all quantized W/I combinations,signed displacement bounds,
indexed/update fields,reserved cases,known mislabels,preservation/idempotence and
CLI read-only checks/binary rejection. Fresh original matrix-function inspection
also exercises the integrated LLVM correction path. These checks validate text
interpretation and preservation,not executed game behavior.
