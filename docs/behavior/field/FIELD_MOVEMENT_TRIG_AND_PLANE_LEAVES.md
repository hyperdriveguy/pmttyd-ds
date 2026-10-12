# Used movement leaves: degree conversion,trig output and plane direction

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete local
movePos0x80005de4(size0xa8),sincosf0x80005e8c(size0x78),
marioPlaneGetFlyingDir0x8009e774(size0x4c),marioGetPtr0x8005e0c0(size8).
[General partner placement](../partner/FIELD_PARTNER_GENERAL_PLACEMENT.md) requests these;
placement3/4 and ordinary field movement also depend on these math interfaces.
Only these local wrappers/leaves are closed here,not the underlying libm or
plane-state lifecycle. R denotes the original single-result rounding operation.

## movePos: conversion and two ordered fused stores

Inputs are X address in r3,Z address in r4,distance D in f1,degree angle A in
f2. Retain both addresses and D. Load the single constant0x40c90fdb at
0x8041f458(6.2831854820251465) and360 at0x8041f45c. Compute
T=R(R(constant*A)/360),using fmuls then fdivs. T is retained as the rounded
single result;libm receives its exactly representable widened double value.

Call sin0x80272410(T),then frsp its returned value and retain sine S. Call
cos0x80271ea8(T),then frsp its result to cosine C. Both libm calls happen
BEFORE output loads/stores. Now:

1. Load CURRENT *X. Compute fmadds(D,S,*X),single fused result,and store *X.
2. Load CURRENT *Z AFTER the X store. Compute fnmsubs(D,C,*Z),the negated
   fused multiply-subtract single result,and store *Z. Algebraically this is
   Z-D*C,but preserve the actual fnmsubs operation,rounding and signed-zero
   behavior rather than selecting a different host FMA identity.

No Y write,angle wrapping,width clipping,distance limit,pointer guard or value
return contract is imposed here. If X and Z alias,the second load observes the
first write. If aliases involve state changed by libm callbacks/exception behavior,
that also remains an external domain. Negative distance reverses displacement;
the caller decides its sign. Splitting these fused operations into separate
multiply/add operations can change numeric results.

## sincosf: a different conversion and intervening store

Inputs are degree angle A in f1,sine output address r3,negative-cosine output
address r4. Load single0x40490fdb at0x8041f460(3.1415927410125732) and180 at
0x8041f464. Compute U=R(R(constant*A)/180),with fmuls then fdivs. Do not
replace this interface with movePos's constant/divisor pair without proving
numeric equivalence in every required input/FPSCR domain.

Call sin0x80272410(U),frsp its result and store it to the FIRST output.
Only THEN call cos0x80271ea8(U),frsp its result,negate using fneg and store
to the SECOND output. The second result is negative cosine,not positive cosine.
There is no angle wrap or input/output guard. With aliased output addresses,the
second store replaces the first;the sine store still happens before cos is called.
This ordering differs from movePos's two-libm-calls-before-any-output policy.

The local functions establish degree conversion,requested libm addresses,
rounding/fused instructions and memory order. They do not establish host
sin/cos bit equality,libm argument reduction/polynomials,exception behavior,
Gekko FPSCR/denormal/GQR state or original measured trajectories. Paired-single
register saves/restores in these bodies are decoded as psq_st/psq_l by the
[corrected Gekko tooling](../../GEKKO_DISASSEMBLY.md),not newer PowerPC vector ops.

## Plane direction: global object differs from captured owner

marioGetPtr loads the global pointer from SDA r13+0x19e0 and returns it.
marioPlaneGetFlyingDir calls that leaf,then reads GLOBAL Mario G+2e:

- Unsigned motion!=24 returns0 without reading G+294.
- Motion==24 loads plane-state pointer G+294,then its word+0. Bit0 set returns1;
  bit0 clear returns-1. No state-pointer null guard exists on this path.

This helper does not use partner P or a supplied owner pointer. General placement
first tests its CAPTURED owner M's motion24,then calls this GLOBAL query and
compares only result1. If G!=M or their motions/state change,0 and-1 both take
the same B=180,F=0 heading route. Do not substitute a captured-owner plane flag
unless pointer/lifecycle equivalence is established. No plane position,speed,
flight physics or resource lifecycle is implemented by this small query.

The general-placement verifier pins these four complete local bodies,the two
constant/divisor pairs,original sin/cos call targets,fused/store ordering and
all plane query return branches. Original execution and broader domains remain
open after those static checks pass.
