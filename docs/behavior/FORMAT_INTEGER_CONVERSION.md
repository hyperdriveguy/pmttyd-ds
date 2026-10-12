# Original 32-bit and 64-bit integer text conversion

US G8ME01 revision0, recovered2026-10-12. B evidence is static original DOL
instructions/dispatch tables, not executed output. long2str at0x8026ba5c,size258
and longlong2str at0x8026b748,size314 have matching local conversion stages.
See [directive parser](FORMAT_DIRECTIVE_PARSER.md), [format consumers](FORMAT_OUTPUT_RUNTIME.md)
and [insert formatting](MESSAGE_INSERT_FORMATTING.md). No host-libc equivalence
or Switch contract is inferred; original format/data bytes remain reused assets.

## ABI, reverse storage and early empty output (B)

long2str receives(valueWord,bufferEnd,formatRecord). longlong2str receives
(valueHigh,valueLow,bufferEnd,formatRecord). The caller supplies a local16-byte
copy of the parsed format record, so converter mutations below affect that copy,
not the formatter's original record. Signed/unsigned narrowing for h/hh occurred
in the formatter before this call, and 64-bit values are high/low words.

Set C=bufferEnd-1 and store NUL there before any admission decision. Initialize
digit count0 and negative marker0. If value is zero and precision word is zero,
return C immediately unless alternate form is set AND conversion byte is 'o'.
This bypasses base/sign/zero-padding/prefix handling and capacity checks.
Zero with explicit precision0 therefore produces an empty substring for decimal,
unsigned and hex, while alternate octal takes the normal zero-digit path.

Successful conversion prepends bytes before C and returns its final pointer.
The formatter computes length `(bufferEnd-1)-returnedPointer`; no heap allocation
or forward strcat occurs. A later capacity failure returns null but leaves the
already written terminator/digits in the local buffer.

## Valid conversion base and sign policy (B)

Each routine uses a33-entry X..x table:0x803bb6c4 for32-bit,0x803bb640 for64-bit.
The established valid cases are:

| Conversion | Base | Value/sign handling |
| --- | --- | --- |
| d/i | 10 | If signed value<0, modulo-width negate and mark negative; retain sign policy |
| o | 8 | Interpret value bits unsigned; write sign policy byte1=0 |
| u | 10 | Interpret value bits unsigned; write sign policy byte1=0 |
| x/X | 16 | Interpret value bits unsigned; write sign policy byte1=0 |

The signed minimum retains its high bit after modulo-width negation and is
subsequently divided as unsigned magnitude; it is not a signed abs operation
that requires a wider host integer. Unsupported converter bytes reach a default
label without establishing a supported base; no local invalid-conversion fallback
is computed here. The directive/formatter layer admits valid types first.

## Digit generation and alternate octal (B)

Do at least one division step after the early-empty gate. For32-bit use divwu
for quotient/remainder. For64-bit call __mod2u(0x80264c10) first, then __div2u
(0x802649ec) on the same value/base; retain remainder low word and quotient
high/low. These helpers' complete arithmetic edge contracts remain separate.

Remainder0..9 maps to ASCII '0'+remainder. Larger remainders use 'a'..'f' only
for exact conversion 'x'; otherwise 'A'..'F'. Prepend byte, increment digit
count and repeat while unsigned quotient is nonzero.

For base8 with alternate flag, if first digit is not '0', prepend '0' and
increment digit count. This occurs before precision adjustment/capacity checking.
Zero itself already starts with '0', so it is not doubled by alternate octal.
That prefix counts as a digit for subsequent precision-zero fill.

## Zero padding becomes precision and local capacity gate (B)

If padding mode=2, replace precision word P with field width W. Then subtract1
from P if negative marker is set or retained sign policy is nonzero. If base16
and alternate flag is nonzero, subtract2 more from P. All mutations use word
arithmetic. For unsigned cases sign policy was already cleared before this step.
Other padding modes retain the existing precision word.

Read the resulting signed P. Compute word `P+(bufferEnd-C)` and signed-compare
to509. Greater returns null; equality is admitted. This is not a direct final-
output-length<=509 check: it includes the current pointer displacement (including
NUL and any octal digit prefix) plus P. It can conservatively reject a precision
that would otherwise fit by maximum-of-digit-count reasoning. There is no lower
bound, no supplied allocation length and no arithmetic overflow protection.

If signed digitCount<P, prepend P-digitCount ASCII zeros. The binary batches
eight stores then a remainder loop; byte result is the same for valid bounded
counts. Precision-present was handled by the parser, which switches integer
mode2 to mode1 for explicit precision. Converter zero-padding behavior must
therefore be interpreted with that preceding normalization.

## Hex prefix and final sign order (B)

After precision fill, if base16 and alternate flag is set, prepend conversion
byte 'x'/'X', then '0'. This is unconditional for values reaching this stage,
including zero when precision is nonzero. Do not import a modern library's
nonzero-value-only alternate-hex condition. Pointer normalization to x/alternate/
precision8 therefore uses this same stage.

Then prepend '-' when negative marker is set. Otherwise retained sign policy1
prepends '+', policy2 prepends space, other values prepend nothing. Return C.
Order in the forward substring is sign, optional hex prefix, precision zeros,
digits. Octal's zero prefix was already included with digits before filling.
Outer formatter space/zero padding still follows its own record and sign logic.

## Derived comparison cases and remaining work (P)

Compare zero/nonzero at precision0, alternate octal versus hex, signed minima,
all-one unsigned words, uppercase/lowercase hex, octal precision counting,
unsigned sign-policy clearing, mode2 width adjustment for sign/prefix, the exact
`P+end-C` gate at509/510 and wrapping values, null helper result's literal-tail
fallback, and caller-copy mutation versus the original format record. These
are derived obligations, not measured formatting output.

For example, original instructions imply alternate x with zero/nonzero precision
can differ from a host formatter's zero-prefix policy; that difference still needs
executed comparison before port equivalence is claimed. No resource format is
rewritten to avoid it.

Remaining: unsigned64 helper details, decimal/hex-float conversion and rounding,
wide/memory services, actual message format/type/count call-site census, replacement
callbacks and execution validation. Both integer converters' local stages and
tables are documented; full formatting/full-game equivalence remain unfinished.
