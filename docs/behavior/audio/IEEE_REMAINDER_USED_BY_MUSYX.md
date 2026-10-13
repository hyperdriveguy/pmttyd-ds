# Original IEEE remainder used by MusyX

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete __ieee754_fmod8026f6bc/33chex.
Read [mixer preparation and fmod wrapper](MUSYX_VOLUME_COORDINATE_AND_PHASE_PREPARATION.md)
and [mixer output branches](MUSYX_VOLUME_INTERPOLATION_AND_OUTPUT_BRANCHES.md).
Addresses/bit masks hexadecimal; shifts/exponents/counts decimal.
Static bit/arithmetic behavior, not original execution or FPSCR observations.

## Captured double words and exact exceptional gate

Incoming f1=x,f2=y are stored as doubles to separate stack slots. Capture their
high/low32 words. Retain sx=xHigh&80000000; hx=xHigh XOR sx; hy=yHigh&7fffffff;
lx=xLow; ly=yLow. Magnitudes are unsigned IEEE binary64 fields. No caller/output
pointer or global state other than signed-zero constants; no direct/indirect call.

Take the exceptional path if ANY:

- (hy|ly)==0, including either sign of zero divisor;
- hx>=7ff00000, including infinite/NaN dividend;
- (hy | nonzeroBit(ly))>7ff00000, including NaN divisor but permitting infinity.

nonzeroBit uses neg/or/logical sign extraction and equals0/1. On exception,
reload ORIGINAL x/y doubles; DOUBLE fmul P=x*y, then DOUBLE fdiv result=P/P.
Do not substitute a chosen NaN payload, finite-zero result, error code or host
exception handling. NaN propagation, invalid flags/traps and FPSCR remain
runtime requirements; this body does not explicitly set/reset them or errno.

If admitted, compare magnitude pairs lexicographically: high then unsigned low.
|x|<|y| returns ORIGINAL x double unchanged, preserving signed zero. Thus finite
x and infinite y return x. Equal magnitudes load signed-zero table below and
return. Only |x|>|y| enters finite normalization/subtraction; admitted y then
must be finite/nonzero. Divisor sign is ignored in this finite path.

## Exponents and normalized significands

For each admitted magnitude, determine unbiased exponent e:

- High word>=00100000: e=(high arithmetic>>20)-1023.
- Smaller high word nonzero: temp=wrap32(high<<11), e=-1022; while SIGNED temp>0,
  temp=wrap32(temp<<1), decrement e. Stop when the sign bit is reached.
- High word zero: temp=low, e=-1043; perform the same signed-positive shift loop.

The admitted strict-greater path excludes a zero dividend/divisor entering
these normalization loops. For normals (e>=-1022), clear the upper12 high bits,
OR00100000 to restore the hidden bit; keep low word. For subnormals, shift the
high:low pair left by n=-1022-e. If n<=31, use high<<n OR low>>(32-n), low<<n;
if n>31, high=low<<(n-32), low=0. Variable shifts follow PowerPC semantics,
including zero result for a logical shift count32, not a host masked shift.
Each normalized pair contains its53-bit significand with hidden bit at high20.
No floating normalization/division, mantissa rounding or approximate quotient.

## Complete subtract/shift remainder loop

Let X=(xh,xl),Y=(yh,yl) be normalized pairs; retain ex,ey.
Count n=ex-ey is nonnegative under the admitted magnitude comparison.
If n==0 skip directly to final subtraction. Otherwise repeat n times:

1. Candidate low=wrap32(xl-yl); candidate high=wrap32(xh-yh-borrow), where
   borrow1 iff unsigned xl<yl. Interpret candidate high SIGNED for its test.
2. Negative candidate: preserve old X, then shift old X left1, explicitly
   carrying old low.bit31 into high. Nonnegative candidate: if both candidate
   words zero, return signed-zero constant immediately. Otherwise shift the
   candidate pair left1 with low.bit31 carry into high.

After those n iterations, compute the same subtraction once more. If candidate
high nonnegative retain candidate; otherwise preserve X. Zero remainder returns
signed-zero constant. For nonzero remainder, repeatedly shift pair left1 until
high>=00100000, decrementing retained ey on each shift. This uses integer
subtraction and shifts to form a truncation remainder; there is no nearest-even
quotient selection or iterative floating subtraction.

## Exact normal/subnormal reconstruction and zero sign

If final exponent ey>=-1022: subtract hidden00100000 from high; OR
((ey+1023)<<20) and sx into high; store low/high words and load the result double.
No rounding operation before return.

Otherwise n=-1022-ey; source right-shift cases are distinct:

- n<=20: newLow=(oldLow logical>>n) OR wrap32(oldHigh<<(32-n));
  newHigh=oldHigh arithmetic>>n.
- 20<n<=31: same low formula; newHigh=sx.
- n>31: newLow=oldHigh arithmetic>>(n-32); newHigh=sx.

OR sx into newHigh, store words and load the double. Admitted nonzero normalized
high is positive; its arithmetic shifts here preserve that finite significand.
Do not collapse count32 into a host shift masked to0 or invent a minimum-normal
floor. Original instruction/domain evidence, not generic floating casts, governs
these paths. Finite exact remainder is representable without discarded nonzero
bits under the source arithmetic invariants.

All equality/loop/final-zero returns load Zero80303c38: two original doubles,
index offset0 for sx0 and8 for sx80000000. Raw words0000000000000000 and
8000000000000000 are +0/-0. Remainder sign therefore follows x, including exact
negative multiples; y's sign does not choose the zero sign. The function restores
its stack and returns the floating result without a scalar status code.

## Evidence and remaining execution/domains

[Manifest](../../../tools/ieee_remainder_musyx_evidence.json) pins complete body
and16-byte numeric zero table: two spans,94 anchors/zero calls. Twelve finite
bit-pattern vectors cover sign combinations/exact multiples/signed zero,
less-than, subnormal normalization/reconstruction, extreme exponent gap and
nonbinary divisor. Seven exceptional-gate vectors record requested paths without
asserting a NaN payload. A deterministic4096-case finite crosscheck compares
exact integer-rational remainder bits with host math.fmod. These are mathematical
source deductions, not original instruction execution, exception experiments or
FPSCR validation. Full fresh ELF/listing and crosscheck generator stay `/tmp`;
no bodies/binary assets committed.

The local IEEE child now closes the mixer remainder source chain, but runtime
rounding/trap/NaN/denormal behavior, original gain/coordinate/index/output/alias/
IRQ/concurrency domains, DSP command consumers and original mixer/audio traces
remain open. No measured cadence or physical output inferred. EVT-01/full native
binding remain OPEN.
