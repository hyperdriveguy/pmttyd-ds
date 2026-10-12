# Decimal powers of two: seeds, recursion and precision

US G8ME01 revision0, recovered2026-10-12. B evidence is static original DOL
__two_exp at0x802673e4,size1784, dispatch at0x803bb1e0 and seed strings below.
See [binary entry](FORMAT_BINARY_DECIMAL_ENTRY.md) and
[multiplication](FORMAT_DECIMAL_MULTIPLICATION.md). This establishes the local
power routine, including its seed-copy rounding distinction. Numeric runtime
helpers and executed output comparisons still remain.

## Dispatch and seed ownership (B)

Inputs are(outputRecord,signed32 power e). Compute low32(e+64), compare unsigned
with72. If<=72, use it as a four-byte index into the73-entry jump table at
0x803bb1e0. Twenty-one powers select dedicated seed bodies; the other52 table
entries select the same recursive path used outside the table range.

Each seed body writes sign0, a literal leading-digit exponent, count and numeric
digit bytes. Source text is ASCII decimal without punctuation. String offsets
below are relative to0x80303ac8; the seed-only block starts at+37 and occupies184
bytes, including NUL separators, ending after the power8 seed. The preceding37
bytes hold another numeric literal outside this routine's seed set.

| Power | Seed body | Leading exponent | String offset | Source digits |
| --- | --- | --- | --- | --- |
| -64 | 0x80267424 | -20 | 37 | 45 |
| -53 | 0x80267520 | -16 | 83 | 38 |
| -32 | 0x8026761c | -10 | 122 | 23 |
| -16 | 0x80267718 | -5 | 146 | 12 |
| -8 | 0x80267814 | -3 | 159 | 6 |
| -7 | 0x80267910 | -3 | 166 | 5 |
| -6 | 0x80267a0c | -2 | 172 | 5 |
| -5 | 0x80267b08 | -2 | 178 | 4 |
| -4 | 0x80267c04 | -2 | 183 | 3 |
| -3 | 0x80267d00 | -1 | 187 | 3 |
| -2 | 0x80267dfc | -1 | 191 | 2 |
| -1 | 0x80267ef8 | -1 | 194 | 1 |
| 0 | 0x80267ff4 | 0 | 196 | 1 |
| 1 | 0x802680ec | 0 | 198 | 1 |
| 2 | 0x802681e4 | 0 | 200 | 1 |
| 3 | 0x802682dc | 0 | 202 | 1 |
| 4 | 0x802683d4 | 1 | 204 | 2 |
| 5 | 0x802684d0 | 1 | 207 | 2 |
| 6 | 0x802685cc | 1 | 210 | 2 |
| 7 | 0x802686c8 | 2 | 213 | 3 |
| 8 | 0x802687c4 | 2 | 217 | 3 |

Direct read-only comparison establishes that each positive/zero source is the
integer decimal digits of2^e; each negative source is the integer decimal digits
of5^(-e), with its table exponent supplying the leading decimal scale. These
are source identities, not a claim that the returned records always equal the
unrounded mathematical value. Only powers-64 and-53 exceed the36-digit budget.

## Seed copy and original comparison domain (B)

Copy while count<36 and signed source byte!=0. Store each source byte minus0x30
as a numeric output digit; increment source pointer/count. Store count. Then
read the current source byte, sign extend, and:

- If zero, return.
- If less than numeric5, return.
- If greater than numeric5, round up.
- If equal to numeric5, scan following signed bytes until NUL; any byte unequal
  to ASCII '0' (0x30) rounds up. Otherwise use the last retained numeric digit's
  low bit to decide the tie.

The comparison is immediate5, not0x35 and not sourceByte-0x30. All actual seed
digits are ASCII0x30..0x39, therefore any remaining non-NUL seed digit takes
the greater-than5 route. The local tie branch exists but cannot be reached by
the verified seed strings. All21 seed-copy/round bodies were compared as220-byte
instruction sequences after normalizing branches to their common epilogue;
they have identical bodies, including this comparison domain.

Consequently both long seeds copy36 numeric digits and increment the retained
record. The first discarded source byte for-64 is0x37 ('7'); for-53 it is0x32
('2'). The latter also increments, unlike numeric nearest rounding of its exact
source. This is a static derived consequence, not an executed-original trace.
Do not silently substitute a modern decimal library's nearest seed generation.

Round up walks backward through output digits. Digit<9 increments and stops;
otherwise write0 and continue unless at the first digit, which becomes1 and
increments the signed halfword exponent. Count stays unchanged, including on
all9 carry; no trimming, NUL or canonical-zero conversion.

## Recursive power construction (B)

Fallback at0x802688c0 computes h by adding the raw sign bit to e, then arithmetic
right shifting by1. For signed32 inputs this is division by2 truncated toward
zero, including negative odd inputs. Recurse into a local record at stack+52:
__two_exp(local,h). Then call __timesdec(output,local,local), squaring that
record through the separately established36-digit multiplication/rounding path.

If raw low bit of original e is zero, return the squared record. If odd:

1. Copy42 bytes from output to a second local record at stack+8, using ten word
   loads/stores and one halfword. This copies unused digits/padding as well as
   the logical record; no fresh-tail initialization is inserted.
2. If e>0, rebuild the stack+52 factor with sign0/exponent0 and ASCII seed '2'
   at0x80303b8e. Otherwise rebuild sign0/exponent-1 with seed '5' at0x80303b8a.
   Both use the same36-byte ASCII-to-numeric copy/round structure. Their verified
   strings have one digit and terminate, so the rounding branch is not taken.
3. Call __timesdec(output,copiedSquare,factor), then return.

Thus negative odd powers square the truncation-toward-zero half-power then
multiply by the numeric record for0.5; positive odd powers multiply by2. Every
recursive square and odd correction can round at36 digits. A single final
rounding of an exact arbitrary-precision power is not the same established
algorithm. No reciprocal operation or floating power call occurs here.

## Derived record invariants and remaining evidence (B/P)

For all signed32 power arguments, recursive absolute magnitude decreases until
a dedicated seed is reached; zero is a seed. All actual seed/factor records
have sign0, nonzero count<=36 and numeric digits0..9. Multiplication preserves
those count/digit bounds for these records, so induction establishes that power
records have count1..36 and numeric digits0..9. This resolves the generator's
normal multiplication scratch budget: integer record<=20 digits plus power
record<=36, giving at most56 scratch digits. It does not validate arbitrary
records passed directly to __timesdec.

The literal seed exponents and each multiplication exponent are halfword
stores. Extreme signed32 powers may wrap the exponent; there is no local range
error, saturation or caller validation. The original finite-double caller's
power range still depends on frexp/ldexp/mantissa helper contracts. Induction
on digit/count bounds is not a proof of exact power accuracy or shortest output.

Differential obligations: sparse dispatch and holes, seed identities, ASCII5
comparison, both truncated seeds, signed odd halving, square-before-correction,
rounding at each multiplication,42-byte copy and halfword exponent storage.
Outstanding: binary numeric services and finite-input power range, original
execution/caller corpus, hex-float and remaining formatting/runtime contracts.
Full formatting, story and complete-game equivalence are still unproven.
