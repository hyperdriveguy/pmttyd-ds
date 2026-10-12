# Finite decimal copy and initial binary64 approximation

US G8ME01 revision0,recovered2026-10-12. Evidence B: __dec2num finite preparation
0x802659e8..0x80265d34 within the containing0x1080-byte routine,original power
constants and selected instructions. See [entry shortcuts](DECIMAL_TO_BINARY_SPECIAL_VALUES.md),
[scan decimal records](SCAN_FLOAT_DECIMAL.md) and [ldexp](FORMAT_BINARY_NUMERIC_SERVICES.md).
**Scope:** local preparation,initial approximation and classification only.
Boundary selection/refinement/final rounding remain open;no original execution
or correctly-rounded conversion claim is established.

## Private record copy and digit conversion (B)

The finite route is entered only when count is nonzero and byte+5 is neither
'0','I' nor'N'. Copy all42 caller-record bytes to stack+0x1a0 using word loads/
stores plus a final halfword. This read is independent of the active digit count.
Caller record is not written;later preparation changes the private copy.
Let D be its unsigned byte+4,E its signed halfword+2,and digits start at+5.
Convert D bytes in-place to numeric bytes by low8(byte-48),with unrolled groups
of8 and a remainder loop. No digit-range validation,capacity bound,NUL test or
replacement character appears. The original finite input sign byte is retained
in the copy,but no negative application happens in this stage.

The copied record provides37 bytes after its header. The local loop trusts
D0..255;counts beyond those37 bytes address outside the copied record and can
continue beyond the local frame. Such malformed record behavior is not verified
as a normal caller domain. The scanner's retained count<=20 avoids that count
edge,but this does not validate all public/runtime callers.

Set local record exponent to signed16(E+D-1),and retain that value Q as a
sign-extended register. This changes from a decimal integer scale to leading-
digit position for later comparisons. Initialize approximation P to the first
numeric byte as an exact unsigned-byte-to-double conversion using bias
0x4330000000000000. Set cursor to the second digit,end to digits+D. The source
record remains untouched throughout.

## Chunk sizes, integer accumulation and floating boundaries (B)

While cursor<end,choose k=(remaining modulo8),using the original signed remainder
sequence;when that remainder is0,use8. The first chunk can therefore have1..8
bytes,with subsequent full chunks8 until the end. For valid D and cursor
arithmetic,remaining is positive. Accumulate U in a32-bit word,initially0:
U=word32(10*U+numericByte) for exactly k bytes,advancing cursor. For valid digits,
k<=8 means U<=99999999;every chunk integer is exactly representable as binary64.
Malformed numeric bytes still use wrapping word arithmetic.

Original table0x803bb1a0 supplies binary64 powers10^1..10^8. Preserve separate
operations rather than replacing them with a fused multiply-add:

```
product = binary64_multiply(P, powers[k-1])
chunk = exact_unsigned32_to_double(U)
sum = binary64_add(product, chunk)
if U != 0 and unordered_compare_equal(product, sum):
    break
P = sum
Q = word32(Q-k)
```

The equality gate is the original fcmpu equality branch,not a decimal precision
threshold or an estimate of remaining error. When it fires,the cursor has
already advanced across the chunk,but P and Q are **not updated**: the enlarged
product and sum are discarded. Further chunks are not processed. Zero chunks
do not take that early exit;they store product and decrement Q normally.
FPSCR/rounding/NaN effects of the arithmetic remain original execution questions.

Thus this is an initial approximation of the retained record,not proof that
all caller digits affect P. The later refinement body must decide the final
binary64;do not describe this stage alone as the converter's rounding algorithm.

## Scaling by five and two (B)

The next constants are binary64 five0x4014000000000000 at0x80428be0,and signed
integer conversion bias0x4330000080000000 at0x80428bf0. Q is converted to double
through xoris/bias with a double fsub,no single-precision narrowing.

If signed Q<0,call pow0x80272620 with base5 and exponent -Q,then divide P by
that returned value. Otherwise call pow with base5 and exponent Q,then multiply
P by its return. Both multiplication/division are binary64 operations. Save this
intermediate,then call ldexp0x80272150 with it in f1 and Q in r3,and save the
returned candidate C.

For exact real arithmetic this composes P*5^Q*2^Q=P*10^Q. That identity does
not establish floating equivalence to one host pow(10,Q),a decimal parser or a
fused operation. Intermediate overflow/underflow and pow/ldexp rounding can
affect the candidate. The [ldexp body](FORMAT_BINARY_NUMERIC_SERVICES.md) is
already recovered;pow and its internal implementation remain dependencies.
Q starts as signed16 but chunk subtraction is32-bit,so do not truncate it back
to16 merely because the record exponent occupies a halfword.

## Candidate classification (B)

Store C as double high/low words. Extract exponent with high&0x7ff00000 and
fraction with (high&0x000fffff)|low;sign is ignored for this classification.
Assign a local class before the unrecovered refinement dispatch:

| Class | Candidate bits |
| --- | --- |
| 1 | Exponent0x7ff,fraction nonzero (NaN encoding) |
| 2 | Exponent0x7ff,fraction zero (infinity encoding) |
| 3 | Exponent0,fraction zero (zero encoding) |
| 4 | Other finite exponent (normal encoding) |
| 5 | Exponent0,fraction nonzero (subnormal encoding) |

This is raw-bit classification,not proof of mathematical magnitude or a normal
input result. Infinity class routes into a boundary-record path beginning
0x80265d3c;other classes branch later at0x80265f54. Those branches,exact
comparisons,successive representable adjustments,ties and final sign/return
still require recovery. No candidate is claimed final at this boundary.

## Evidence, examples and remaining work

For valid record digits12300,D5,E0: private exponent becomes4,first P1;
remaining chunk2300 yields P12300,Q0,then the five/two scaling stage requests
pow(5,0),ldexp(...,0). This is a request-sequence deduction,not a measured final
conversion or helper-accuracy claim. Caller scan normalization may instead hand
off digits123,E2;that must be composed with this private-record contract.

Twenty additional anchors pin copy/digit conversion,exponent preparation,chunk
selection/accumulation,separate multiply/add,early non-update exit,scaling calls
and classification. Direct checks pin all eight decimal powers and three
five/unsigned/signed conversion constants. Prior whole-function identity covers
the containing routine;original inspection/report artifacts remain ignored.
No original execution,executable linking,asset conversion or story rewriting
occurs by this research step.

Next: pow dependency and __dec2num boundary records/refinement/rounding/final
return. Compare private records,candidates,helper requests,FPSCR and final bits
against original execution for actual scanner inputs and numeric boundaries.
Complete scanner/layout/story/game coverage and requirements remain incomplete;
no Switch behavior is inferred.
