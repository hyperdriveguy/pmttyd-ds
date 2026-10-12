# Original printf directive parser and normalization

US G8ME01 revision0, recovered2026-10-12. B evidence is static DOL instructions
and dispatch/classification data. This is printf's parse_format at0x8026bcb4,
size504, namespace MSL_C.PPCEABI.bare.H.a printf.c; scanf has a different symbol
with the same local name. See [format consumers](FORMAT_OUTPUT_RUNTIME.md) and
[insertion wrappers](../message/MESSAGE_INSERT_FORMATTING.md). No original execution or
host-libc equivalence was measured; original formats/text remain reused data.

## Inputs, record defaults and continuation (B)

Inputs are(percentPointer,varargsRecord,outputFormatRecord). The caller supplies
a '%' start; this function does not independently enforce that starting byte.
Initialize local record: padding mode1, sign policy0, precision-present0,
alternate form0, length enum0, width0, precision0. Read/sign-extend the next byte.
Record offsets match [consumers](FORMAT_OUTPUT_RUNTIME.md): bytes0..5 are padding,
sign, precision-present, alternate, length and conversion; words8/c are width/
precision. Padding bytes6/7 are not explicitly initialized here, although all
four words are copied to output. Do not compare the entire record as a uniformly
zeroed structure or infer a semantic role for those copied padding bytes.

If the first byte after '%' is '%', immediately set conversion='%', copy record
and return pointer after it. No flags, width, precision, length or value arguments
are consumed on this special path. A later '%' after modifiers takes the normal
conversion validation and is invalid, unlike the immediate `%%` case.

Most exits copy the16-byte record and return one byte past current input byte.
Invalid conversion is byte0xff; the formatter then copies the remaining original
format from this directive's '%' literally and stops, as documented separately.
This parser does not reset already consumed dynamic width/precision arguments
when later validation rejects a directive.

## Ordered repeated flags (B)

The17-entry table at0x803bb828 classifies ASCII space through '0'. Only five
characters consume as flags; repeat until the first other byte:

| Flag | Mutation |
| --- | --- |
| - | padding mode0 (trailing spaces) |
| + | sign policy1 |
| space | sign policy2 unless policy is already1 |
| # | alternate form1 |
| 0 | padding mode2 unless already mode0 |

Minus takes precedence over zero in either order. Plus takes precedence over
space in either order. Other flag-looking bytes are not consumed as flags.
There is no locale normalization or grouping flag support established here.

## Field width and early rejection (B)

If current byte is '*', consume class1 vararg word as signed width. If negative,
negate with word arithmetic and set padding mode0. Consume '*' and move on.
INT_MIN negation wraps locally; no overflow error is computed.

Otherwise consume decimal digits classified by byte-table mask0x10 at0x803bb308.
The read-only256-byte table marks exactly ASCII0..9 with this mask. Accumulate
`width=word(width*10+digit-'0')`, with no arithmetic overflow check.
After either route, if signed width>509, set conversion0xff, copy record and
return pointer after the current byte immediately, before precision/length/
conversion parsing. Width509 is admitted,510 rejected. Negative wrapped widths
are not rejected by this signed upper-bound check. No dynamic argument rollback.

## Precision and length (B)

If current byte is '.', consume it and set precision-present1. A following '*'
consumes class1 vararg word as signed precision; if negative, clear precision-
present but retain that negative word. Then consume '*'. Otherwise consume
ASCII decimal digits with the same wrapped multiply/add route. No digits means
explicit precision0. There is no local precision509 upper bound or overflow
check. Default precision is assigned later by conversion normalization.

Consume at most one length modifier sequence:

| Spelling | Length enum |
| --- | --- |
| absent | 0 |
| hh | 1 |
| h | 2 |
| l | 3 |
| ll | 4 |
| L | 5 |

Repeated h/l beyond those pairs leaves the next byte as the conversion. Other
modern length spellings are not consumed here. The current conversion byte is
stored before validation through the56-entry A..x table at0x803bb748.
Enums can be rewritten below; enum6 is produced by l-string/character handling,
not by an independent length token.

## Complete conversion validation map (B)

| Raw conversion(s) | Local record changes/admission |
| --- | --- |
| d,i,o,u,x,X | L(enum5) invalid. If precision absent, precision word=1. If precision present and mode2, change mode to1; retain precision word |
| f,F | h(enum2) and ll(enum4) invalid; other length enums admitted. Absent precision gives word6 |
| e,E | If precision word==0, set it to1, regardless of precision-present. h/hh/ll enums2/1/4 invalid. If precision absent, set word6 |
| g,G | Same validation/default rule as e/E, at a separate dispatch entry |
| a,A | Absent precision gives word13. h/hh/ll enums2/1/4 invalid; other length enums admitted |
| p | Normalize conversion='x', alternate1, length enum3, precision word8; retain precision-present, padding, sign and width fields |
| c | l(enum3) becomes enum6 and is admitted immediately. Otherwise precision-present or nonzero length makes invalid |
| s | l(enum3) becomes enum6 and is admitted; length0 admitted; any other length invalid. Precision/alternate flags retained |
| n | L(enum5) invalid; other length enums admitted, with no extra width/precision validation here |
| every other conversion | invalid byte0xff |

No uppercase C/S/D/U/O aliases are admitted by the table. Decimal/hex-float
default words are original numeric constants, not modern-library inferred
mantissa precision. Explicit precision0 for e/g becomes1. Dynamic negative
precision clears presence and therefore selects defaults, although its stored
word first passes through conversion-specific operations.

The parser's accepted length does not alone establish argument/output behavior:
for example hh float length1 is admitted by f/F while rejected by e/g/a; all
floating consumers still request a double. lc normalizes to enum6 but the local
c consumer still emits low byte of a class1 word. hhn is admitted here while
the n consumer has no enum1 byte-store branch. These cross-layer behaviors must
not be silently replaced by contemporary printf expectations.

## Alternate flag correction to the consumer record (B)

Byte3 is alternate form, not a string-only field. Numeric helpers receive it;
p sets it explicitly. For s the existing consumer interprets this flag as an
unsigned leading-byte length followed by data, so `%#s` chooses that route.
String-specific use does not remove its numeric role. Whole format text remains
original input; this is not XML/control-tag parsing or message substitution.

## Differential obligations and remaining work (P)

Compare immediate %% versus modified percent, repeated flag precedence,
dynamic negative/INT_MIN width,509/510 boundary, decimal wrap, zero/no-digit/
negative dynamic precision, consumed star args before invalid conversion,
each length/conversion admission combination, p normalization retaining presence,
e/g explicit zero precision, alternate s length prefix, lc/hhn consumer behavior
and literal-tail fallback. These are derived obligations, not executed traces.

Numeric helper stages are now covered in [integer conversion](FORMAT_INTEGER_CONVERSION.md),
[decimal layout](FORMAT_FINITE_DECIMAL_LAYOUT.md) and [hex-float](FORMAT_HEXADECIMAL_FLOAT.md).
Remaining: wide conversion and memory services, actual insertion type/count/format call-site
census and original execution comparisons. The local directive parser and both
dispatch tables are documented; full formatting/full-game equivalence remains
unfinished.
