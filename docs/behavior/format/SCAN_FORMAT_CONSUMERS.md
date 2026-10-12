# Original scan formatter consumers and assignment counts

US G8ME01 revision0, recovered2026-10-12. Evidence B: __sformatter at
0x8026c498,size0x9ac, inspected original DOL instructions and classification
bytes. See [entry/string reader](SCAN_INPUT_RUNTIME.md),
[shared varargs](FORMAT_OUTPUT_RUNTIME.md) and [icon callers](../message/FONT_MESSAGE_WIDTH.md).
This covers the complete local consumer body.
[Directive grammar](SCAN_DIRECTIVE_PARSER.md), integer helpers and
[multibyte conversion](MULTIBYTE_CONVERSION.md) are recovered separately;
floating helper internals remain dependencies. No original execution occurred.

## Interface, counters and descriptor (B)

Arguments are readCallback,context,format,varargs. Callback actions0/1/2 are
read/unread/status as defined by the original string reader. Initialize C=0
(consumed-character accounting),A=0(assignments),B=0(successful conversion
accounting). These are separate32-bit words, not interchangeable counts.
Read each format byte through signed-byte extension; NUL ends the main loop.

A '%' calls scanf's parse_format0x8026ce44 with(percentPointer,localDescriptor),
then uses its returned format continuation. The consumed descriptor fields are:

| Offset | Local consumer role |
| --- | --- |
| 0 byte | Suppress destination argument when nonzero |
| 1 byte | Explicit field-width indicator for c |
| 2 byte | Length/storage selector |
| 3 byte | Conversion dispatch byte |
| 4 word | Remaining field width, decremented by character/string loops |
| 8..0x27 | 256-bit acceptance bitmap used by s/[ loops |

The [directive parser](SCAN_DIRECTIVE_PARSER.md) establishes validation, aliases,
default widths and bitmap construction; compose its exact rules with these
consumers before interpreting arbitrary format text.
For nonsuppressed conversions other than '%', request __va_arg class1 and load
a destination pointer from the returned slot **before** checking reader status
or dispatch validity. Suppressed and '%' routes use destination=null. Except
for n, a nonzero status query exits before dispatch; n can write the count after
EOF. Invalid/unsupported dispatch ends scanning without a local error string.

## Classification, whitespace and literals (B)

Classification table0x803bb308 has256 bytes; bit mask0x06 classifies TAB,LF,VT,
FF,CR and ASCII space as whitespace in this binary. Whitespace in the format
consumes its whole run, then reads input while that table mask matches, adds each
skipped byte to C, and unreads the first nonmatching read. Format whitespace
itself is not added to C.

An ordinary literal reads once. Compare the unsigned format byte to the
sign-extended input byte. If equal, increment C and advance the format pointer;
otherwise unread the narrowed/sign-extended input value and exit. A high input
byte cannot equal a positive format-byte value under that comparison. Literal
matches do not increment A or B.

The '%' conversion separately skips input whitespace, counting it in C, then
requires a sign-extended '%' byte. Match adds one to C but increments neither
A nor B; mismatch unreads and exits. This local route does skip whitespace
before literal percent. Do not infer behavior from host scanf specifications.

## Integer and floating dispatch (B)

| Dispatch byte | Requested helper/base |
| --- | --- |
| d | __strtoul or __strtoull, base10 |
| i | Same, base0 |
| o | Same, base8 |
| u | Same, base10 |
| x,X | Same, base16 |
| a,e,E,f,g,G | __strtold |

These are values routed by the consumer tree. Parser normalization may change
which source spellings produce them; direct p/F/A bytes do not have a successful
route in this body. Avoid claiming their format-text behavior without the parser.
Integer selector4 chooses64-bit helper __strtoull0x8026e6dc; all other selectors
choose32-bit __strtoul0x8026eae8. Pass base,width,readCallback,context and pointers
to local consumed count,negative flag and auxiliary status. Consumed count0 exits
without assignment. Otherwise C+=count, apply word/64-bit negation when the flag
is nonzero, and store according to selector if destination is nonnull:

| Selector | Integer storage |
| --- | --- |
| 0,3 | Word at+0 |
| 1 | Low byte at+0 |
| 2 | Low halfword at+0 |
| 4 | Low word at+4, then high word at+0 |

Unknown selectors perform no storage but still increment A if destination is
nonnull. Successful integer routes increment B regardless of destination.
The [unsigned32 helper](SCAN_INTEGER32.md) now establishes its overflow flag,
digit/prefix/whitespace and count behavior. The [unsigned64 helper](SCAN_INTEGER64.md) is recovered separately; the consumer does not directly gate
success on that status word.

Floating routes call __strtold0x8026d6d0(width,readCallback,context,&count,&status).
Count0 exits; otherwise C+=count. Selector0 performs frsp then stfs; selectors5
and6 store the returned f1 double. Other selectors do no local store. Nonnull
destination still increments A, and B increments in every successful route.
Thus default f output is a float destination write, not a promoted double.
See [float entry/special values](SCAN_FLOAT_ENTRY_SPECIAL_VALUES.md) for
recovered special-token callback/count behavior. Finite parsing/rounding,
range exits and executed FPSCR behavior remain open.

## Character conversion c (B)

If explicit-width byte is zero, set width=1. There is no leading-whitespace skip
or NUL terminator. Repeatedly decrement width before deciding whether to read.
With nonnull destination, compare the full callback return to-1. Each real byte
is stored; selector7 calls mbtowc(destination,&localByte,1), ignores its result,
and advances destination by **one byte**. Other selectors store a byte directly.
Stop when width is exhausted or EOF. If at least one byte was stored, C+=count,
A++ and B++; a short read after partial input is therefore counted as success.
Zero stored bytes exits scanning.

With null destination, the loop instead stores/read-sign-extends a byte before
checking-1. Byte0xff is treated as EOF on this route, while the assigned route
accepts callback return255. A nonzero consumed count increments B, but this
suppressed/null c path does **not add its count to C**. This affects a later n;
do not silently repair it to match a modern library. There is no unread after
these c exits, unlike the string/scanset loops.

## String s and acceptance bitmap [ (B)

For s, skip leading whitespace using the table above, add skipped bytes to C,
and unread the first nonwhite byte. Then share the '[' body. '[' itself does
not perform that leading skip. The parser-provided bitmap determines acceptance:
byte n tests descriptor[8+(n>>3)] & (1<<(n&7)). Both assigned and suppressed
routes sign-extend the stored input byte and stop on-1 **before** testing the
bitmap;0xff is therefore treated as EOF even though the string reader returned
255 and advanced. Each attempt decrements width before reading.

Assigned route stores each accepted byte directly, or for selector7 calls
mbtowc(destination,&localByte,1), ignores its return and advances destination
by two bytes. This differs from c's one-byte advance. With zero accepted bytes,
unread the last stored byte and exit without assignment. With positive count,
C+=count, append byte NUL (selector7: halfword NUL), A++, and continue through
the shared exit. There is no destination-capacity check beyond the parsed field
width. `%s` without a width is not thereby safe for a caller's fixed stack buffer.

Suppressed/null route consumes accepted bytes without writes. If count is zero,
unread the last byte and return to the main format loop without incrementing B
or forcing the assigned route's terminal exit. With positive count, C+=count.
At the shared positive-count exit, if remaining width is nonnegative, unread
last byte; if negative, width exhaustion caused no extra read and no unread is
issued. Increment B. These control distinctions are local recovered behavior,
not a recommendation for new input APIs.

## Count conversion and final result (B)

For n, a nonnull destination stores C: selector0/3 word,1 byte,2 halfword,
4 low word at+4 then sign-extended high word at+0. Unknown selectors do not store.
The n route does not increment A or B and consumes no input. Suppressed/null n
simply continues. Destination pointer fetching for nonsuppressed n still occurs.

All terminal routes query callback status. If status is nonzero and B==0,
return-1; otherwise return A. Therefore a successful suppressed conversion can
prevent EOF return despite A==0; literal matches or n alone do not establish B.
EOF pushback can clear the original reader's flag and change this final decision.
Failed later conversions leave earlier destination writes intact. Neither entry
nor consumer initializes failed outputs; game icon callers ignore the returned
assignment count, so incomplete arguments must not be treated as initialized
zero by a fidelity claim.

## Evidence and unresolved dependencies

Whole-body fingerprints and independent anchors check callback routing,
destination acquisition, storage widths, count changes, partial c, acceptance
loop and final return. A complete hash pins classification bytes. These are
static checks, not an executed scanf corpus. Local inspection artifacts remain
ignored; no original routine is compiled into the port.

The [scanf-specific parser](SCAN_DIRECTIVE_PARSER.md) is now recovered. Next
compose the recovered integer and [multibyte](MULTIBYTE_CONVERSION.md) helpers,
then recover __strtold. Compare actual icon argument strings and boundary formats, including
suppression/partial reads, high-byte literals/0xff, EOF unread, n, storage
selectors and wide-pointer increments. Only after those dependencies and original
callers are verified can parsing/layout equivalence be claimed. Original story
messages/tokens remain reused data; no Switch behavior is inferred here.
