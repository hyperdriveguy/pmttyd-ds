# Binary64 floor service

US G8ME01 revision0; recovered2026-10-12 from floor at0x80271f7c,
size0x148. Evidence B: original static instructions and constants checked in the
read-only DOL. This is the linked dependency of
[HSV2RGB](../display/HSV_COLOR_CONVERSION.md), not proof that every game's rounding site
uses this helper. No original numeric execution was performed.

## Interface and classification (B)

Input and return are binary64 in f1. The routine stores the input to its local
stack and reads big-endian high word H and low word L. It calls no helper and
writes no caller buffer, global storage or errno locally. Let
j=((H>>20)&0x7ff)-1023. H's sign bit distinguishes negative values; all word
additions below wrap at32 bits. Bitwise transformations preserve the binary64
encoding until the final reload, except the special arithmetic return.

There are three exponent regions. An already-integral input with j>=0 returns
f1 directly without the arithmetic probe. Positive and negative zero instead
enter the j<0 path; the negative-zero encoding is retained.

## Probe before changing fractional inputs (B)

The routine loads binary64 1e300 at0x804290a8 and positive zero at0x804290b0.
Before each fractional finite correction it computes 1e300+input in f1, compares
that result to zero with fcmpo, and proceeds only when ordered greater.
Otherwise it reloads the unchanged H/L words. This is not a clamp of the returned
value to1e300: the probe result is discarded on the ordinary correction path.

In the finite fractional domains below, the mathematical sum is positive.
The arithmetic still matters for floating flags/traps, particularly inexact.
No FPSCR configuration, flag observation or trap execution was recovered here.
Avoid claiming a plain host floor call reproduces these side effects.

## Values below one and high-word fractional region (B)

For j<0, after the positive probe:

- Nonnegative H: replace H=L=0, yielding positive zero.
- Negative H: if (H&0x7fffffff)|L is zero, preserve negative zero.
  Otherwise replace H=0xbff00000,L=0, yielding minus one.

This covers normal/subnormal magnitudes below one; NaNs and infinities take the
large-exponent route instead. For0<=j<20, use F=0x000fffff>>j. If (H&F)|L==0,
return unchanged. Otherwise probe; for negative inputs add0x00100000>>j to H,
then clear H's bits in F and set L=0. The high-word addition can carry into the
exponent. Positive inputs simply clear fractional bits. Negative correction
therefore rounds downward, rather than toward zero.

## Low-word fractional region and special values (B)

For20<=j<=51, use F=0xffffffff>>(j-20). If L&F==0, return unchanged.
Otherwise probe; if negative, increment the retained magnitude before masking:

| Region | Negative correction before clearing L&F |
| --- | --- |
| j=20 | Increment H by1 |
| 21<=j<=51 | Add1<<(52-j) to L; increment H if the new unsigned L is less than the old L |

The j=20 branch avoids a shift by32. Clear L's F bits, store H/L to the local
stack and reload f1. For j>51 finite binary64 values are already integral and
return directly. j=1024 means exponent bits0x7ff: return input+input with fadd.
That operation retains infinities in the usual floating environment and can
quiet a NaN or raise floating status; exact NaN payload/trap behavior must be
established on the original processor. It does not locally canonicalize a NaN
through an integer constant or signal a library error.

## Porting scope and validation

For HSV's nonnegative h=3Hbyte/128<6, these branches produce mathematical
floor(h) in0..5. The hue input is already single precision and is exactly
representable when passed in f1; the helper does not add a frsp step.

Ten independent anchors cover exponent extraction, probe arithmetic, minus-one
encoding, high/low masks, negative carry and special return; the evidence report
fingerprints the complete selected function and checks the two probe constants.
Static checks do not execute PowerPC or verify FPSCR effects. Validate signed
zeros, subnormals, fractional values adjacent to exponent-region boundaries,
negative carry cases, large integral values, infinities and NaN payloads before
claiming standalone numeric-service equivalence. No Switch behavior is asserted.
