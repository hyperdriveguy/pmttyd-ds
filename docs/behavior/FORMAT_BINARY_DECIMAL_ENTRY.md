# Binary-to-decimal entry, numeric records and output conversion

US G8ME01 revision0, recovered2026-10-12. B evidence is static original DOL
instructions/constants. __num2dec at0x80266884,size1a4 and __num2dec_internal
at0x80266a28,size3b4 are locally recovered here. See [rounding](FORMAT_DECIMAL_ROUNDING.md)
and [finite layout](FORMAT_FINITE_DECIMAL_LAYOUT.md). Numeric multiplication is
covered in [its contract](FORMAT_DECIMAL_MULTIPLICATION.md), and power generation
in [decimal powers](FORMAT_DECIMAL_POWERS.md). Binary64 helpers and integer-word
conversion are covered in [numeric services](FORMAT_BINARY_NUMERIC_SERVICES.md);
unsigned quotient/remainder are covered in [division](FORMAT_UNSIGNED64_DIVISION.md). This
is not a complete binary-to-decimal equivalence claim or executed-output study.

## Internal record versus public ASCII record (B)

Record byte0=sign, halfword2=exponent, byte4=count, bytes5 onward=digits.
__num2dec_internal produces finite numeric bytes0..9, not ASCII. Its exponent
is used as leading-digit order by its multiplication path. __num2dec subsequently
subtracts count-1 from that exponent and adds ASCII '0' to each numeric digit.
float2str later trims/renormalizes it as documented separately. Treating every
phase's digits/exponent identically changes value and rounding behavior.

Special records use first-byte marker 'I' or 'N', with count1/exponent0; these
are passed through before numeric-to-ASCII conversion. No NUL is appended to
ordinary digit arrays. Count controls their visible extent; unused storage is
not universally initialized.

## __num2dec request handling (B)

Inputs are(controlPointer,outputRecord) and double in f1. Save signed halfword
control+2 as request K before calling __num2dec_internal(output,double). The
control byte0 set by float2str is not read by this wrapper. Other callers and
control meanings outside these local reads remain unproven.

After generation, if unsigned first digit byte>9, return immediately, without
clamping request, rounding, padding, exponent adjustment or ASCII conversion.
For numeric output clamp signed request above36 down to36. Do not clamp a
zero/negative request up to1 or treat it as a canonical-zero request locally.

Round only when signed K>0 and K<unsigned record count. Compare first discarded
numeric digit with5: greater means round up; less means truncate. Exact5 scans
all later numeric bytes and rounds up if any is nonzero. Otherwise inspect the
last retained digit's low bit and round up only when odd. Unlike round_decimal,
this wrapper has a positive-retention admission gate and therefore never uses
the count byte as zero-retention parity.

Write count=low8(K) before carry handling. On round up, walk backward from the
last retained digit: if byte<9 increment it and stop; otherwise write numeric0
and continue left. At the first retained digit, an overflowing9 becomes1 and
exponent increments with halfword storage. Retained count stays K; carried-through
zeros are written explicitly, unlike round_decimal's count-shrinking strategy.

Then, while count<signed K, append numeric0 and increment byte count. Convert
exponent to halfword(oldExponent-(count-1)); add0x30 to each of count numeric
bytes in place. Request K<=0 skips positive rounding/padding but still performs
this exponent/ASCII conversion for the full generated record. Request32 used by
float2str therefore pads shorter records to32 here; its later trailing-zero trim
removes that padding while restoring the exponent scale.

No local output-capacity, pointer, numeric-digit or count-overflow guard. The
36 request cap is not a general validation of every generated/caller record.

## Raw sign, zero and special classification (B)

__num2dec_internal receives(outputRecord) and double f1. Store double bits and
extract raw high-word sign bit before comparisons. Compare the double to binary64
zero at0x80428bc8. Equal (positive or negative zero) writes raw sign, exponent0,
count1 and numeric digit0, then returns. Thus negative zero is preserved in the
record by original code, independent of host comparisons that erase its sign.

For nonzero/unordered input, inspect high exponent bits masked by0x7ff00000
and both fraction words. The local classification is:

| Exponent/fraction | Class |
| --- | --- |
| all-one exponent, nonzero fraction | 1 (NaN route) |
| all-one exponent, zero fraction | 2 (infinity route) |
| zero exponent, zero fraction | 3 (zero, normally handled earlier) |
| nonzero non-all-one exponent | 4 (normal finite) |
| zero exponent, nonzero fraction | 5 (subnormal finite) |

Classes<=2 write raw sign, exponent0/count1, first digit 'N' for class1 or 'I'
otherwise. No NaN payload is copied into digit text. The classification is
recomputed in the special branch; this is not a call to an external classifier.
Sign of NaN later controls the formatter's original -NAN/-nan string route.

## Finite mantissa route and bit count (B)

If raw sign is nonzero, negate f1 and store positive double bits. Call frexp
at0x802720c4 with pointer to a local exponent word; retain returned double M
and that word H. Full frexp normalization/subnormal/edge semantics are now
covered in [numeric services](FORMAT_BINARY_NUMERIC_SERVICES.md); this document
establishes the caller's operations.

Inspect low word of M's stored double. If nonzero, count its trailing zero bits
with iterative low-mask tests/shifts. If low word is zero, inspect high word OR
0x00100000 with the same loop, then add32 to the trailing count. The binary
starts with a16-bit mask and reduces test widths toward1; it does not use a
platform decimal estimate or a rendered string as input.

Let T be that count and L=53-T. Call __two_exp at0x802673e4 with(outputPower,
H-L), creating an intermediate decimal record. Separately call ldexp at0x80272150
with M and L, then modf at0x80272314 with a local integral-double output.
Pass that stored integral double to __cvt_dbl_usll at0x80264fc4, retaining high/
low words as unsigned integer V. Returned fractional result of modf is unused.

Together with the recovered [numeric services](FORMAT_BINARY_NUMERIC_SERVICES.md),
this gives the significand-integer times power-of-two factorization V*2^(H-L),
with positive odd V<=2^53-1 and power H-L in[-1074,1023]. These are derived
binary64 invariants, not executed-original traces. Exact call order, arguments
and stored integral result are established binary behavior here.

## Integer decimal record and multiplication (B)

Create local unsigned numeric decimal record: sign0. If V=0, exponent0,count1,
numeric digit0. Otherwise repeatedly __mod2u(V,10) then __div2u(V,10), append
each remainder byte starting at+5 and increase count. Reverse logical digits
in place using endpoint swaps; exponent=halfword(count-1). Digits remain numeric.
No digit-count-based allocation or output terminator. The recovered nonzero
division contract gives digits0..9 and at most16 digits for this significand;
see [division](FORMAT_UNSIGNED64_DIVISION.md).

Call __timesdec(outputRecord,&integerRecord,&powerRecord) at0x80268b68, then
overwrite outputRecord.sign with the original raw sign. Multiplication count/
precision/rounding is now covered [separately](FORMAT_DECIMAL_MULTIPLICATION.md);
__two_exp recursion/table behavior is covered in [decimal powers](FORMAT_DECIMAL_POWERS.md).
Neither is inferred from its
name. All finite output digits/exponent depend on those services; no
claim of exact shortest decimal, modern dtoa or universal36-digit correctness.

## Differential obligations and remaining work (P)

Compare raw signed zero/NaN sign, special bypass before request clamp, numeric
versus ASCII phases,32 request padding then formatter trim,36 cap, nonpositive
request behavior, tail ties/positive retained parity, explicit carry zeros versus
round_decimal trimming, exponent shifts, low/high mantissa trailing counts,
helper ordering, numeric integer reversal and final sign overwrite. These are
derived obligations, not executed conversion traces.

Remaining: floating environment and executed generator comparisons,
hex-float output and original format/type/count
call-site corpus. Entry/internal local branches are documented; complete binary
decimal generation, formatting/story/full-game equivalence remain unfinished.
