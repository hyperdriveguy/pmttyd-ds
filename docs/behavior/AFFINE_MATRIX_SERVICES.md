# Affine matrix services used by original drawing

US G8ME01 revision0,recovered2026-10-12. Evidence B: complete original bodies
PSMTXScale0x8029e2b0,size0x28;PSMTXTransApply0x8029e264,size0x4c;
PSMTXConcat0x8029dd64,size0xcc. These helpers supply the matrices requested by
[string pitch drawing](FONT_STRING_PITCH_DRAWING.md) and
[glyph drawing](FONT_GLYPH_REQUESTS_AND_SETUP.md). No original execution or
matrix/raster equivalence is claimed;no Switch behavior is inferred.

## Representation and instruction interpretation (B/reference)

Matrices contain twelve row-major binary32 words:three rows,four columns,
48 bytes. Entry(i,j) is pointer+16*i+4*j. They represent an affine transform;
the fourth row is implicit rather than stored. None of these functions checks
pointers,alignment,finite values or matrix type,and none allocates or calls
another function. There is no local RNG,GX request or explicit FPSCR/GQR change.

Original opcode4 instructions here are Gekko paired-single arithmetic. The
raw generic LLVM disassembly renders them as newer vector instructions or
unknown words;those printed mnemonics must not be used as the contract. The
inspection tool now applies a [Gekko correction](../GEKKO_DISASSEMBLY.md),and
existing research listings were repaired without changing instruction bytes. Original words identify ps_muls0/1,ps_madds0/1,ps_sum1 and psq_l/st.
All paired loads/stores here have W=0,I=0:two elements through GQR0. A faithful
implementation must establish the runtime GQR0 load/store representation before
assuming these accesses are ordinary float pairs. The finite binary32 formulas
below assume unquantized float loads/stores and the normal floating environment.

For instruction-lane interpretation,the public
[Dolphin paired-single interpreter](https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/Core/PowerPC/Interpreter/Interpreter_Paired.cpp)
was consulted. It models ps_sum1 as retaining C lane0 and adding A lane0 to B
lane1,and scalar-lane multiply/add instructions as selecting C lane0 or lane1.
This is emulator reference evidence,not execution of the supplied game. Original
instruction operands and memory order are independently recovered from the DOL.
The reference is GPL-2.0-or-later;no implementation was copied or linked.

SP denotes the paired-single/scalar single rounding boundary;FMA denotes a
single fused multiply/add boundary. Avoid replacing a chain with one host double
expression or reassociating products. Actual FPSCR rounding,denormal handling,
NaN payloads and exceptions remain unmeasured.

## PSMTXScale: diagonal construction (B)

ABI:output r3;scale x/y/z in f1/f2/f3. Load positive binary32 zero from
0x8042935c. Write,in instruction order:

| Offset | Request |
| --- | --- |
| 0 | Scalar stfs x |
| 4,8 | Paired zeros |
| 12,16 | Paired zeros |
| 20 | Scalar stfs y |
| 24,28 | Paired zeros |
| 32,36 | Paired zeros |
| 40 | Scalar stfs z |
| 44 | Scalar stfs zero |

The result in the ordinary float domain is diag(x,y,z) with zero translation.
Scalar stfs converts the input floating register to binary32. Paired zero stores
still depend on GQR0. Every one of the twelve output words is written;existing
output contents are not read. There is no clamp,identity optimization or stored
fourth row. No meaningful scalar return is established.

## PSMTXTransApply: add translation after source transform (B)

ABI:source r3,destination r4;translations x/y/z in f1/f2/f3. Apply frsp separately
to all three translations. Preserve columns0..2 and add to column3:

`D[i,3]=SP(frsp(t[i])+S[i,3])`, `D[i,j]=S[i,j]` for j0..2.

This adds translation in output coordinates. It does not multiply the supplied
translation by the source rotation/scale. For the string scale matrix,it inserts
pen/vector coordinates directly as the translation column.

Load source pairs at offsets0,8,24,40 before the first destination store.
Then follow this sequence:store pair0;form row0 pair8 using ps_sum1;load source
pair16;store pair8;form row1 pair24;load source pair32;store pair16;form row2
pair40;store pairs24,32,40. ps_sum1 preserves each row's column2 lane and adds
the rounded translation to its column3 lane.

Exact destination==source is safe:each potentially overwritten pair is already
loaded before its corresponding write. Arbitrary partially overlapping buffers
are not generally safe because source pairs16/32 are loaded after writes begin.
Do not infer a temporary whole-matrix copy. No source word is changed when source
and destination are distinct nonoverlapping buffers.

## PSMTXConcat: ordered affine product (B)

ABI:left A r3,right B r4,output C r5. The intended finite-domain transform is
C=A*B. The translation includes A's translation after the transformed B column;
there is no transpose and multiplication order is not commutative.

For each row i and column j,first compute:

1. `p=SP(B[0,j]*A[i,0])` using ps_muls0.
2. `q=FMA(B[1,j],A[i,1],p)` using ps_madds1.
3. `r=FMA(B[2,j],A[i,2],q)` using ps_madds0.
4. For j0..2,store r. For j3,store `FMA(1,A[i,3],r)` using ps_madds1.

Columns0/1 and2/3 are processed as paired lanes. The last operation on pair2/3
uses Unit01 at0x8041e458,whose binary32 words are positive zero and1:
column2 actually requests `FMA(0,A[i,3],r)`,while column3 requests
`FMA(1,A[i,3],r)`. In the finite ordinary domain the column2 expression preserves
the product. For infinity/NaN/sign-of-zero inputs,it must not be silently erased
as an algebraic no-op. The helper has no special-value branch.

All twelve words of A and B,and the Unit01 pair,are loaded before any output
store. Thus C may equal A,B,or both;the pitch caller's C==B does not overwrite
needed input. This also prevents input/output overlap from changing later source
loads within the recovered body,assuming valid memory and no asynchronous
mutation. No identity shortcut or singularity test exists.

Rows0/1 are computed first. Output pair order is offsets0,16,8,24,32,40;
row2 products are interleaved with the row0/1 stores. All inputs are already in
registers at that point. Scalar stfd/lfd saves/restores f14/f15/f31 on a64-byte
stack frame;these are register preservation rather than additional matrix output.
No meaningful scalar return is established.

## Evidence checks and required original comparisons

Three whole-function fingerprints and24 instruction anchors cover constant loads,
paired operations,late source loads and output ordering. Direct read-only checks
pin positive zero and Unit01. Inspection ELFs/disassembly/reports are ignored;
original executable code is not linked into the port or committed.

Execute original helper cases with captured GQR0/FPSCR and returned matrix bits:
scale/translation,noncommuting rotation products,alias outputs,negative zero,
subnormals,overflow and special values. Capture intermediate matrices at font
callers and compare transformed positions and original images. Static anchors
establish byte identity,not runtime rounding or visual equivalence.

Remaining services include projection,GX matrix loading,current-matrix selection,
other matrix/vector helpers and camera producers. String-matrix/center/shake and
message caller coverage,resource lifetimes and draw cadence remain open. These
three local helpers do not establish complete graphics,text,story or game fidelity.
