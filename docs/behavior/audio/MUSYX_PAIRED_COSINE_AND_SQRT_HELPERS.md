# MusyX paired cosine and reciprocal-root helpers

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete sndCos8029047c/d4 and
sndSqrt80290428/54hex. Addresses/offsets hexadecimal; shifts/counts decimal.
Read [filter coefficient caller](MUSYX_FILTER_COEFFICIENTS_AND_DSP_PARAMETERS.md).
Static original instruction order, not measured cosine/filter accuracy or
hardware reciprocal-root execution. Neither body calls a child or allocates.

## Original constants and paired operand prerequisites

sqrtConsts80304098 contains binary32(3.0,0.5,0.0). _sinConsts803040a4 contains
four pairs; use c0..c7 below in original order:

| Pair | First | Second |
| --- | --- | --- |
| c0,c1 | -0.1666666716337204 | 0.008333331905305386 |
| c2,c3 | -0.00019840900495182723 | 2.7526000394573202e-06 |
| c4,c5 | 0.6366197466850281 | 1.5707963705062866 |
| c6,c7 | 0.0 | 2.3900000201138027e-08 |

sndCos loads these with psq_l using W0/I0. Required GQR0 floating-load state,
FPSCR rounding/exception/denormal state and paired-lane semantics remain explicit
execution prerequisites; do not reinterpret these loads as quantized integers.
Assembler four-operand order is D,A,C,B, as the repository's
[paired decoder](../../../tools/gekko_disasm.py) records.

Lane cross-check: ps_sum0 produces lane0=A0+B1 and lane1=C1; ps_muls0/1 scales
both A lanes by selected C lane; ps_madds1 fuses each A lane with C1 and B lane.
ps_neg flips both signs. This interpretation was checked against
[Dolphin's paired interpreter](https://raw.githubusercontent.com/dolphin-emu/dolphin/master/Source/Core/Core/PowerPC/Interpreter/Interpreter_Paired.cpp).
Scalar single arithmetic fills both lanes in Dolphin's
[floating interpreter](https://raw.githubusercontent.com/dolphin-emu/dolphin/master/Source/Core/Core/PowerPC/Interpreter/Interpreter_FloatingPoint.cpp).
These references support instruction interpretation; no emulator/original trace
was run and no emulator implementation is asserted as complete hardware evidence.

## Complete cosine phase reduction and retained comparisons

Retain paired c4/c5 and c6/c7. Incoming x is f1 lane0. Initial
ps_sum0 f1,f1,f1,f6 adds c5 to x in lane0 with paired-single rounding;
other lane passes through. Compare this SHIFTED lane0 to c6=0 in CR1;
retain that comparison until final sign handling. Merge c5/c4 into f11.
Load coefficient pairs c0/c1 and c2/c3.

Apply fabs to shifted lane0, SINGLE multiply by c4 to get q. Signed fctiwz
produces word n; store its word to stack. Retain Gray bit
((logicalShiftRight(n,1) XOR n)&1) in CR5 via mcrf after its integer test.
Convert n as SIGNED32 through double43300000/(n XOR80000000), SINGLE subtract
signed bias4503601774854144 at804292a0. SINGLE subtract that converted n from q.
Paired ps_muls1 scales by c5. Test n.lowBit: if set SINGLE subtract c5 from the
reduced phase. Square lane0 with fmuls BEFORE conditional phase negation.
If Gray bit nonzero, ps_neg the reduced pair; zero skips negation.

There is no modulo/libm call, repeated angle wrap, frequency admission gate,
range/finite/NaN check or integer-conversion overflow repair. Initial CR1 compares
shifted x rather than original x; n uses signed fctiwz, not the unsigned helper.
Invalid conversion/FPSCR domains remain original execution work. The lane0
phase formula for finite, ordinary conversion is reproduced in evidence vectors;
that formula does not replace the paired instruction chain under arbitrary state.

## Complete fused cosine polynomial sequence

Let z be reduced/Gray-signed lane0, u=SINGLE z*z (captured before Gray negation),
v=SINGLE u*u. Let R mean single rounding and F mean source fused single
multiply-add. Under ordinary floating GQR0/paired state, the lane0 sequence is:

1. Load/retain paired products a=(R(c0*z),R(c1*z)), b=(R(c2*z),R(c3*z)).
2. b is multiplied by v, then u, with paired rounding at EACH multiplication.
3. t=R(c7*z), then t=R(t*v), then t=R(t*v), using source single/paired operations.
4. p=F(a0,u,z); p=F(v,a1,p) using ps_madds1.
5. p=R(p+b0); p=F(u,b1,p) using ps_madds1.
6. p=F(t,u,p).

Preserve mixed scalar/paired operations and each intermediate rounding/fusion;
do not expand into separately rounded monomials or substitute host cos/sin.
If retained CR1 says shifted x<0, ps_neg final pair. Otherwise return unchanged,
including unordered comparison's non-LT branch if execution continues. No
independently assigned scalar status. Constants are original fitted values;
no claim of exact Taylor coefficients, measured error or all-angle accuracy.

## Complete square-root estimate/refinement order

sndSqrt(x) loads3.0,0.5,0.0; compares incoming x to0.0 in CR0, THEN requests
frsqrte r=x's reciprocal-square-root estimate. The next conditional return
checks the saved equality comparison. Thus +0/-0 returns original x, but the
estimate instruction has already executed and may affect FPSCR/traps.
There is no early gate avoiding that instruction or finite/negative admission.

If not equal (including unordered): repeat EXACTLY three times in order,
all DOUBLE arithmetic:

`t = fmul(r,r)`
`h = fmul(r,0.5)`
`c = fnmsub(x,t,3.0)` meaning fused3.0-x*t
`r = fmul(h,c)`

Finally DOUBLE fmul result=x*r and return. No frsp/single result rounding,
hardware sqrt instruction, libm call or explicit negative/NaN/infinity repair.
Actual estimate bits, fused double behavior/FPSCR/exception/denormal state are
required for bit parity. Synthetic-seed vectors cannot prove original output.
No source status/errno publication or guard controlling caller filter activation.

## Evidence and remaining source/runtime

[Manifest](../../../tools/musyx_cosine_sqrt_helpers_evidence.json) pins two complete
bodies/three numeric spans: five spans,74 anchors/zero calls. Every body word is
anchored, including paired/fused/estimate instructions. Nine ordinary quadrant/
phase, eight integer odd/Gray-bit, five synthetic-seed refinement and five zero-
gate vectors are deductions, not original Gekko execution. Refinement vectors
show formula flow with chosen seeds, without asserting source frsqrte seed or
fused-double bit equivalence. Fresh full ELFs/listings stay `/tmp`; no bodies/
binary assets committed.

Remaining: original GQR0/FPSCR/scalar-paired lane/estimate/rounding/trap/NaN/
denormal execution and valid angle/frequency/coefficient/caller domains,
filter/DSP command consumers and original math/filter/audio traces. Two complete
local helpers answered; full native/runtime binding and EVT-01 remain OPEN.
