# Binary64 normalization, scaling, splitting and integer conversion

US G8ME01 revision0, recovered2026-10-12. B evidence is static original DOL
and constants: frexp0x802720c4,size8c; ldexp0x80272150,size1c4;
modf0x80272314,sizefc; copysign0x80271e80,size28; and
__cvt_dbl_usll0x80264fc4,sizecc. Symbol names are metadata, not definitions of
standard-library behavior. See [binary entry](FORMAT_BINARY_DECIMAL_ENTRY.md)
and [decimal powers](FORMAT_DECIMAL_POWERS.md). No original execution here.

## Stored word convention and floating environment (B/P)

Stored binary64 uses high word H, low word L, raw sign H&0x80000000,
exponent E=(H>>20)&0x7ff, high fraction H&0xfffff. Integer word manipulation
below is32-bit. PowerPC floating multiplication/subtraction is retained where
the original uses it; host floating exception/rounding/denormal behavior is not
established merely by these instruction listings. FPSCR initialization and
runtime changes still require a separate contract and original observations.

## frexp: mantissa and output exponent (B)

Input double f1 and output exponent pointer r3. Store input bits and write
*exponent=0 before any classification. Set absolute high word A=H&0x7fffffff.
If A>=0x7ff00000 or (A|L)==0, return the stored input without further arithmetic.
This covers NaN/infinity and both signed zeros; pointer write still happens.

If A<0x00100000, input is finite nonzero subnormal: multiply by2^54 from
0x804290b8, store the scaled double, refresh H/A and write *exponent=-54.
For this or the normal finite path, replace the stored high exponent with0x3fe
using (H&0x800fffff)|0x3fe00000, preserving sign/fraction and the stored low word.
Write *exponent=(A>>20)+previousOutput-1022; load the modified double and return.
No allocation, count cap, pointer validation or errno write occurs locally.

For finite nonzero binary64 under the original arithmetic environment this
normalizes |mantissa| into[0.5,1) with exponent output in[-1073,1024]. These
bounds follow from bit layout and exact2^54 normalization, not measured gameplay.

## ldexp: initial gates and subnormal normalization (B)

Input double f1, signed32 scale n in r3. Store the original in two local slots.
Inline exponent/fraction classification yields NaN1, infinity2, zero3, normal4,
subnormal5. Classes<=2 return original f1 immediately. Then compare to the
binary64 zero at0x804290c0; equal returns original, preserving signed zero.

For the remaining path, load high/low words from the working slot and set K=E.
If K=0, recheck absolute bits and return if zero. Otherwise multiply working
double by2^54 at0x804290c8, store/refresh high word and set K=scaledE-54.
If signed n<-50000, immediately return tiny*scaledInput, where tiny is the
binary64 literal1e-300 at0x804290d0. This test belongs to the subnormal path.

If K==2047, return workingInput+workingInput. The earlier classification excludes
original NaN/infinity, and ordinary finite subnormal normalization cannot reach
this exponent; retain the local branch without claiming an observed caller.
Otherwise add n to K with32-bit arithmetic and interpret the result signed.

## ldexp: resulting exponent and arithmetic returns (B)

| Signed K after addition | Local path |
| --- | --- |
| K>2046 | Return copysign(huge,workingInput)*huge |
| 0<K<=2046 | Replace exponent bits with K; return stored double |
| K<=-54 and n>50000 | Return copysign(huge,workingInput)*huge |
| K<=-54 and n<=50000 | Return copysign(tiny,workingInput)*tiny |
| -54<K<=0 | Set stored exponent to K+54; multiply loaded result by2^-54 |

huge is the binary64 literal1e300 at0x804290d8; final2^-54 is at0x804290e0.
Exponent replacement preserves stored sign/fraction/low word. The n>50000 branch
handles the local wrapped-exponent case; do not replace the32-bit addition with
unbounded host arithmetic while retaining the same branch predicates.

Overflow/underflow paths perform multiplications, rather than directly storing
infinity/zero bits. Their exact rounded values/exception flags depend on the
floating environment. There is no local errno store or call to an error-report
service. The decimal generator's M scaled by L=1..53 takes the normal exponent
replacement path and forms an exactly representable positive integer.

## copysign: stored sign replacement (B)

Store both doubles. Replace the first stored high word with
(firstHigh&0x7fffffff)|(secondHigh&0x80000000), retaining the first low word.
Load/return the first slot. No arithmetic comparison, magnitude rounding or
NaN classifier; raw stored sign drives ldexp's huge/tiny sign choice.

## modf: integral output and fractional return (B)

Input double f1, integral-output pointer r3. Store bits and set j=E-1023.
Branches cover the entire raw exponent domain, without a separate NaN test:

- j<0: write signed zero to *integral (raw sign, low word0); return original f1.
- 0<=j<20: mask F=0xfffff>>j. If (H&F)|L is zero, store original input into
  *integral and return a zero built from the raw sign. Otherwise write
  integralHigh=H&~F, integralLow=0, then return input-loadedIntegral through fsub.
- 20<=j<=51: mask F=0xffffffff>>(j-20). If L&F is zero, store original input
  and return raw signed zero. Otherwise write H unchanged and L&~F as integral,
  load that double and return input-loadedIntegral through fsub.
- j>51: store original input into *integral and return raw signed zero.

Thus finite magnitudes>=2^52 are integral on this path. Infinity and NaN also
take j>51: the original stored value goes to *integral, while the fraction is
constructed signed zero, including for NaN. Do not assume a host modf's special
value policy matches this implementation. No null-output guard or errno write.
Integral inputs use explicit sign-bit zero construction; nonintegral fractions
use floating subtraction. The generator reads only the integral-output slot.

## __cvt_dbl_usll: observed signed-range word conversion (B)

Store double, load high/low words into return registers r3/r4 and extract E.
If unsigned E<1023, return high0/low0; sign has no effect on that early return.
Otherwise save original high word for sign, replace high with
(H&0xfffff)|0x00100000, and set s=E-1075. This is a53-bit significand pair.

If s<0, shift the pair logically right by -s using PowerPC32-bit shifts and
cross-word ORs. If s>=0 and s<=10, shift the pair logically left by s with
cross-word ORs. Both routes then negate the pair with subtract-from-zero/borrow
if the original raw sign is set. This truncates finite magnitude toward zero
within the admitted range, returning two's-complement bits for negative values.

If s>10, bypass shifting and return0x8000000000000000 for negative raw sign,
or0x7fffffffffffffff for positive raw sign. This includes magnitudes>=2^63,
infinity and NaN. No separate payload or unordered comparison changes this
sign-selected saturation. Despite the metadata name, the observed range policy
is signed64 saturation; do not implement it as a general standard unsigned64
cast with a2^64 limit. No hardware integer-conversion instruction or errno store.

The generator's positive exact integral double is between1 and2^53-1, so its
conversion follows an exact shift route and does not hit saturation. Division/
remainder services used to form integer decimal digits are now covered in
[division](FORMAT_UNSIGNED64_DIVISION.md).

## Finite generator deductions and remaining work (B/P)

For nonzero finite input, binary entry first makes magnitude positive. frexp
returns M in[0.5,1), normalization exponent h in[-1073,1024]. Its trailing-bit procedure gives retained
significand width W in[1,53]. ldexp(M,W) is the positive odd integer V obtained
by removing all trailing binary zeros, and modf stores that integer unchanged.
The converter returns V exactly. Therefore the previously conditional
factorization |input|=V*2^(h-W) now follows from these local contracts.

The raw binary64 representable range and removed-zero count give power p=h-W
in[-1074,1023]. The minimum subnormal has V=1,p=-1074; the normal power2^1023
has V=1,p=1023. These are derived bit-level bounds/examples, not executed traces.
This removes the extreme signed32 exponent-wrap case from the finite generator
power call, although __two_exp's standalone contract still retains it.

Differential obligations: raw signed zeros/special values, exponent-pointer
initial write, subnormal normalization, wrapped scaling predicates, floating
overflow/underflow operations and environment, split masks at j=0/19/20/51/52,
NaN fractional signed zero, conversion shifts/sign/saturation and finite power
endpoints. An independent integer bit-position enumeration of109816 normal/
subnormal exponent and trailing-position classes confirms these three derived
ranges; it is not an execution of original helper instructions.
Remaining: hex-float/wide/memory
services, original caller census and execution comparisons. Full formatting,
story and complete-game equivalence remain unproven.
