# Hexadecimal floating output: raw nibbles, exponent and bounds

US G8ME01 revision0, recovered2026-10-12. B evidence is original static DOL
double2hex at0x8026b410,size338. See [parser](FORMAT_DIRECTIVE_PARSER.md),
[format consumer](FORMAT_OUTPUT_RUNTIME.md), [integer conversion](FORMAT_INTEGER_CONVERSION.md)
and [binary decimal entry](FORMAT_BINARY_DECIMAL_ENTRY.md). This covers all local
paths; it is not a claim that a host printf or Switch output agrees.

## Entry, scratch and special recognition (B)

Inputs are(bufferEnd,formatRecord) plus double f1. Read signed precision P at
record+12. Store original input in stack slots+8 and+24. If P>509, return null
immediately, before numeric generation or output termination. No lower cap.

Otherwise set a local decimal control byte0=0 and signed request halfword32;
call __num2dec with its output record at stack+64. Only that record's first
digit marker at stack+69 is used here: 'I' selects infinity, 'N' selects NaN.
Finite decimal digit/exponent output is not used to generate the hex mantissa.
The numeric generator still runs for finite values, so its helper operations
are part of this call's behavior even though text is built from original bits.

Special paths use strcpy0x8026d5fc with the previously verified string block
at0x80303bb0. Infinity sign is read from the first halfword of the duplicate
input at stack+24; NaN sign is read from the first byte of input at stack+8.
Both are raw stored sign-bit tests. Exact conversion byte 'A' chooses uppercase;
all other bytes choose lowercase:

| Marker/sign | Source offset in string block | Text | New pointer |
| --- | --- | --- | --- |
| I, negative | 1 uppercase / 6 lowercase | -INF / -inf | bufferEnd-5 |
| I, nonnegative | 11 / 15 | INF / inf | bufferEnd-4 |
| N, negative | 19 / 24 | -NAN / -nan | bufferEnd-5 |
| N, nonnegative | 29 / 33 | NAN / nan | bufferEnd-4 |

strcpy includes the NUL, so it ends at bufferEnd-1. Return the new pointer
immediately. Positive special values bypass '+'/'space' sign policy, alternate
form and requested mantissa precision. No payload is printed.

## Raw exponent suffix (B)

For finite input, extract the raw11-bit exponent E from the first halfword of
the original duplicate at stack+24; subtract1023 as a signed word. There is
no frexp normalization here and no zero/subnormal exponent correction.

Build a local16-byte integer format with padding mode1, sign policy1,
precision-present0, alternate0, length0, conversion 'd', width0, precision1.
Its two padding bytes are not individually initialized. Call long2str(E-1023,
bufferEnd,&localFormat), producing signed decimal text with at least one digit
and a NUL at bufferEnd-1. Positive/zero exponents therefore include '+', while
negative exponents include '-'. Unlike scientific decimal formatting, the
exponent does not get a two-digit minimum. Its possible finite values are
-1023..1023. The local call has no null-result branch after long2str.

Prepend 'p' for exact conversion 'a', otherwise 'P'. Keep this suffix pointer
for reverse mantissa storage. The exponent is independent of the mantissa's
requested precision and there is no exponent change for discarded fraction bits.

## Precision loop and exact source addresses (B)

Set loop index i=P and CTR=P. If signed P<1, skip the fraction loop. Otherwise,
for i=P down to1, select the original-stack byte at
stack+8+floor(i/2)+1. For positive i, even chooses its high nibble; odd chooses
its low nibble. Convert numeric nibble0..9 to '0'..'9'; nibble10..15 becomes
'a'..'f' only for exact conversion 'a', otherwise 'A'..'F'. Predecrement output
cursor and store. Decrement i; bdnz repeats exactly P times on this path.

Reverse construction leaves the selected nibbles in increasing i order in the
forward output. For P=1..13 these are the leading P nibbles of the raw52-bit
fraction. No discarded-bit scan, ties rule, rounding carry or trailing-zero trim.
Absent precision defaults to13 in the parser; explicit precision0 stays0.

| Fraction position i | Stack byte read | Established contents |
| --- | --- | --- |
| 1..13 | +9..+15 | Original raw fraction nibbles |
| 14..15 | +16 | Decimal control byte0, explicitly0 |
| 16..17 | +17 | Decimal control padding byte, not initialized locally |
| 18..19 | +18 | Request halfword high byte0 |
| 20..21 | +19 | Request halfword low byte0x20, nibbles2 then0 |
| >=22 | Formula above | Other stack slots, including later records/saved data |

The loop does not cap raw-bit reads at13 or select an explicit padding-zero path.
Precision14/15 happen to read zeros from initialized control storage; larger
precisions can read padding and other live locals. At P=509 the maximal selected
byte is stack+263, outside this128-byte frame. This is a static address calculation,
not an observed original read, supported caller claim or executed overrun.

## Point, fixed leading digit, prefix and sign (B)

After fractions, prepend '.' when P!=0 OR alternate byte3!=0. A negative P skips
the digit loop but still satisfies P!=0; no local lower-bound validation prevents
that record shape. Then store a fixed ASCII '1' one byte before the cursor.
Prepend 'x' for exact conversion 'a', otherwise 'X', moving the cursor by2 so
the previously stored '1' follows it. Prepend ASCII '0'.

Finally, if the raw sign bit from the duplicate original input is set, prepend
'-'. Otherwise policy byte1=1 prepends '+', policy2 prepends a space; other
values add no sign. Return the final pointer. The forward substring therefore
has sign,0x/0X, fixed1, optional point, selected fraction bytes, p/P and signed
decimal raw exponent. Width/padding remain the outer formatter's responsibility.
There is no normalization to a different leading hex digit or special finite
zero rendering. Negative zero uses its raw sign here.

## Derived examples and capacity boundary (B/P)

Examples below assume valid a/A records with no positive sign policy and the
stated precision; they are instruction-derived, not executed output captures:

| Input/record | Derived output |
| --- | --- |
| +0, a, P0 | 0x1p-1023 |
| -0, a, P0 | -0x1p-1023 |
| Minimum positive subnormal, a, P13 | 0x1.0000000000001p-1023 |
| 1, a, P0, alternate | 0x1.p+0 |
| 1.5, a, P1 | 0x1.8p+0 |
| Maximum finite binary64, A, P1 | 0X1.FP+1023 |

Zero/subnormal outputs above do not mathematically represent the input under
ordinary hex-float interpretation; the raw exponent/fixed1 construction is still
the established local algorithm. Reducing P truncates rather than rounding, even
when the discarded bits would cause a rounded mantissa carry.

The P<=509 gate is the only local mantissa admission check. If P>=0, output byte
length is P +3(prefix/leading1) +pointPresent +1(p/P) +signedExponentLength
+optionalValueSign. signedExponentLength is2..5 here. The loop's combined output
length is not rechecked against509; nor is the allocation length supplied. Do
not turn this single precision predicate into a general input/output safety claim.

Differential obligations: numeric generation before raw hex construction,
special sign/case bypass, raw exponent and signed suffix, zero/subnormal fixed1,
precision0/13/14/16/20, raw nibble order, lack of rounding, alternate point,
value sign policy, null cap and full computed displacement. Remaining: original
call-site corpus, executed output/stack-domain comparisons, strcpy/wide/memory
services and floating environment. Full formatting/story/game equivalence remains
unproven; no story message is rewritten to hide formatter differences.
