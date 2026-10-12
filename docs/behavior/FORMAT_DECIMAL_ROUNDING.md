# Original decimal record preparation and rounding

US G8ME01 revision0, recovered2026-10-12. B evidence is static DOL instructions/
data, not executed numeric output. round_decimal at0x8026b2e4,size12c and the
entry/preparation stages of float2str at0x8026abc8,size71c are inspected here.
See [format consumers](FORMAT_OUTPUT_RUNTIME.md), [directive grammar](FORMAT_DIRECTIVE_PARSER.md)
and [integer conversion](FORMAT_INTEGER_CONVERSION.md). Full decimal/hex-float
layout and binary-to-decimal generation remain open; no host-libc equivalence
or complete floating-format claim is made.

## Decimal record fields and ownership (B)

round_decimal receives(decimalRecord,retainedCount K). Consumer offsets are:

| Offset | Observed role |
| --- | --- |
| byte0 | sign marker; this rounding helper does not modify it |
| byte1 | not assigned here |
| signed halfword2 | decimal exponent used by caller |
| unsigned byte4 | logical digit count D |
| byte5 onward | ASCII digit sequence |

The helper mutates the original record in place and returns no new string.
There is no capacity, null-pointer or digit-validity check. Logical digit count
is authoritative; stale bytes beyond it are not cleared and no NUL is appended.
This record is distinct from the16-byte printf format record.

## Entry gates and canonical zero (B)

If signed K<0, set exponent0, count1 and first digit '0', then return.
Preserve sign byte and other storage. This is truncation to canonical zero,
not signed zero elimination or an error code.

Otherwise read unsigned D and compare signed K with it. If K>=D, return without
mutation, including without trimming existing trailing zeros. If K<D, the
first discarded digit pointer P=record+5+K is in the digit sequence for a
valid record; inspect byte at P and subtract ASCII '0' with signed-byte narrowing.
Counts/digit contents outside normal decimal records are not locally validated.

## Discarded tail and tie admission (B)

For a first discarded digit other than5, initial carry is1 when that digit is
greater than5, otherwise0, for valid ASCII digits. For first discarded digit5,
scan from the last logical digit backward toward P, skipping ASCII zeros:

- If any later discarded digit is nonzero, initial carry1.
- If all later digits are zero, carry is low bit of byte P-1.

For K>0 this is the last retained ASCII digit's parity, implementing exact-tie
rounding toward an even retained digit. For K=0, P-1 is record.byte4 (the digit
count), not a virtual leading zero. Thus exact ties with zero retention use
count parity literally. Do not replace that memory read with a mathematically
conventional zero-retention tie rule without original execution comparison.

The tail scan uses unsigned pointer comparisons and does not treat NUL as a
terminator. Only logical count and ASCII-zero checks determine its stopping point.

## Carry propagation and trimming (B)

Starting immediately before P, while K is nonzero:

1. Move pointer back one byte; compute signed-byte digit=(oldByte+carry)-'0'.
2. Set carry1 if digit>9, otherwise0 (valid digit input).
3. If carry1 or digit=0, decrement K and continue without writing that byte.
4. Otherwise write digit+'0' at this position and stop the loop.

This trims zeros from the retained suffix even when initial carry0. It also
discards overflowing9 positions by shrinking logical count instead of writing
zero bytes to every carried-through position. Those stale bytes remain present
but outside the final count. The implementation's signed-byte intermediate and
word/byte storage should not be changed to arbitrary-precision strings.

If carry remains nonzero when propagation finishes, increment signed exponent
by1 with halfword storage, set count1 and first digit '1', then return.
If carry0 and K=0, use canonical zero above (exponent0/count1/digit0).
Otherwise set count=low8(K) and retain the current exponent. No digit shifting,
new buffer, sign update or final terminator is performed.

## Derived record comparison cases (P)

These are instruction-derived expectations for ASCII records, not executed
original tests; exponent remains E unless noted. Caller normalization can make
some raw record shapes unreachable from float2str.

| Original digits | K | Result logical digits | Exponent |
| --- | --- | --- | --- |
| 12500 | 2 | 12 | E |
| 13500 | 2 | 14 | E |
| 12501 | 2 | 13 | E |
| 12000 | 3 | 12 | E |
| 99900 | 2 | 1 | halfword(E+1) |
| 5 | 0 | 1 | halfword(E+1), count parity1 |
| 50 | 0 | 0 | 0, count parity0 |
| any valid digits | negative | 0 | 0 |

Also compare K>=D no-mutation, sign retention, halfword exponent wrap, all-zero
retained prefixes, dirty discarded bytes, and stale bytes outside the final count.

## float2str preparation and special values (B)

float2str receives bufferEnd, formatRecord and double in f1. If signed precision
word>509, return null before decimal generation. No local lower-bound check.
Set decimal-generation control byte0=0 and signed-halfword+2=32, then call
__num2dec at0x80266884 with(control,decimalRecord) and the double. Generator
algorithms, exact32-digit request semantics and record invariants remain open.

Trim trailing ASCII zeros from the generated decimal while count>1. Each trimmed
digit decrements byte count and increments exponent with halfword storage.
Then inspect first digit: '0' forces exponent0; 'I' takes infinity; 'N' takes
NaN; other records continue to ordinary decimal layout. This phase precedes the
later exponent normalization to exponent+count-1 and rounding/layout selection.

For infinity, choose sign by comparing the original double to binary64 zero at
0x80428c00. For NaN, choose sign from decimalRecord.byte0. Case is determined by
ctype mask0x80 indexed by the format conversion byte; the read-only table marks
exactly ASCII A..Z. Original contiguous strings at0x80303bb0 are empty sentinel,
-INF/-inf/INF/inf/-NAN/-nan/NAN/nan, each NUL-terminated. Negative variants copy
to bufferEnd-5, positive variants bufferEnd-4 and return that pointer. These
branches bypass ordinary sign-policy prefix and do not infer payload text.

Ordinary finite formatting later invokes round_decimal in three local routes:
g/G retention equals requested precision; e/E retention equals precision+1;
fixed formatting computes retention from the digit/exponent/fraction budget.
Their precise mode selection, zero placement/capacity and final output construction
need the next detailed contract. The exponent meaning before/after normalization
must not be conflated or treated as a universal fixed-point scale.

## Remaining obligations (P)

Recover full float2str finite layout and __num2dec generation, hex-float output,
runtime numeric conversions and actual message format/type/count callers. Compare
original special-value sign/case, generated record normalization, exact ties and
zero-retention count parity before claiming a port's decimal output equivalent.
Local rounding and preparation contracts are documented; full formatting/story/
complete-game behavior remains unfinished.
