# Original scanf directive grammar and acceptance bitmaps

US G8ME01 revision0, recovered2026-10-12. Evidence B: scanf's parse_format
at0x8026ce44,size0x548, original40-byte descriptor initializer,52-entry dispatch
table and shared classification bytes. This is distinct from printf's same-named
[directive parser](FORMAT_DIRECTIVE_PARSER.md). See [scan consumers](SCAN_FORMAT_CONSUMERS.md)
and [entry/reader](SCAN_INPUT_RUNTIME.md). No original parser execution or host
scanf equivalence was measured.

## Interface, initialization and continuation (B)

Input r3 points to '%' and r4 to a40-byte output descriptor. The parser does not
validate the starting '%' itself; advance input by one and load/sign-extend the
next byte. Copy the original initializer at0x80303bd8 into local storage:
suppression,explicit-width,length,conversion bytes0..3 are zero; width word+4
is0x7fffffff; bitmap bytes+8..0x27 are zero. It writes all40 descriptor bytes
back on every local exit. It calls no helper, consumes no varargs or input text,
and performs no allocation or RNG operation.

If the immediately following byte is '%', set conversion='%', return the pointer
after that byte and copy the otherwise-default descriptor. This early route
runs before suppression, width or length parsing. A later '%' after those
modifiers is not given that shortcut. The returned pointer normally advances
past the conversion or closing scanset bracket, including invalid directives;
there is no separate parse-status scalar. Conversion byte0xff signals invalid
input to the consumer. Missing/trailing conversion NUL can leave the returned
pointer beyond the format terminator; the consumer's0xff dispatch still exits.
Pointers and output capacity are unchecked locally.

## Suppression and decimal width (B)

At most one initial '*' sets suppression byte0=1 and advances. It is assignment
suppression, not printf's width-argument syntax. Next, bytes whose classification
map has mask0x10 are digits; the pinned map contains ASCII0..9 for that mask.
If at least one digit appears, initialize width0 and accumulate
width=word32(10*width+digit-48), advancing until the first nondigit. There is
no overflow check, width cap, saturation or signed-range validation.

If the final word is zero, set conversion0xff and return one byte beyond the
first nondigit, without parsing further length/conversion rules. Otherwise set
explicit-width byte1=1 and continue. Thus `%0d` is invalid; a long width that
wraps to zero also takes that path, while a width that wraps to a negative signed
word is retained as explicit. No digits leave width0x7fffffff and the indicator0.
The consumer sets c's default to1 only when that indicator is zero.

Width syntax precedes length syntax. There is no precision dot, sign/alternate
flag grammar, positional destination selector or scan-time argument for width.
A byte not admitted by these phases becomes the conversion candidate and is
subject to the dispatch table, rather than a generic format-extension handler.

## Length parsing and conversion validation (B)

One prefix is admitted, with doubled h/l handled specially:

| Prefix | Initial descriptor selector |
| --- | --- |
| None | 0 |
| hh | 1 |
| h | 2 |
| l | 3 |
| ll | 4 |
| L | 6 |

No j/z/t prefix route appears. Repeated/combined prefixes beyond these cases
become invalid conversion bytes. After optional prefix, store the candidate
byte as conversion. Dispatch index=sign_extended(candidate)-69 must be unsigned
<=51; otherwise mark0xff. The52 slots cover ASCII'E'..'x', with these routes:

| Conversion | Resulting validation/normalization |
| --- | --- |
| d,i,o,u,x,X | Keep selector0..4; selector6 (L) marks conversion0xff |
| a,e,E,f,g,G | Reject selectors1,2,4; change selector3 (l) to5; retain0 or6 |
| p | Force selector3 and conversion x, overriding the parsed prefix |
| c | Retain0; change3 (l) to7; other selectors mark0xff |
| s,[ | Retain0; change3 (l) to7; other selectors mark0xff, then still build the bitmap |
| n | Keep descriptor with no extra length validation here |
| All other slots | Mark conversion0xff |

There is no parser normalization of A/F/C/S to lower case: their slots are
invalid. X remains uppercase and is accepted by the consumer's base16 route.
A syntactically parsed L on n retains selector6 even though its consumer has no
store case; do not infer conventional long-double/long-count support from the
prefix alone. p uses word storage even if h/ll/L preceded it. Immediate '%%'
is the only percent route admitted by this parser.

The consumer fetches nonsuppressed destination arguments before dispatch/status
checks, so a descriptor marked invalid does not necessarily imply no varargs
movement. Actual output changes and assignment counts are governed by the
[consumer body](SCAN_FORMAT_CONSUMERS.md), not parser validity alone.

## s bitmap (B)

The s route fills all32 bitmap bytes with0xff, then sets bitmap byte1=0xc1 and
byte4=0xfe. A byte n is accepted when bitmap[n>>3] has bit(n&7). This removes
9..13 and32, matching the six inspected whitespace bytes. Byte0 remains admitted
by the bitmap itself; the string reader terminates on NUL before that membership
can be used. Byte255 is also admitted by the bitmap, but the consumer's signed
byte EOF test stops before accepting it. Classification/membership and actual
reader/consumer behavior must therefore be composed, not conflated.

Even an invalid length selector on s still builds this bitmap, with conversion
remaining0xff. No string length, destination allocation or output terminator
is processed by the parser; those belong to scanning consumers.

## Scanset bracket, complement and ranges (B)

The '[' route begins reading one byte after '['. If first byte is '^', record
complement mode and advance. If the next first byte is ']', include bit93 in the
initially-empty bitmap and advance once; that first ']' is data, not a closing
bracket. Continue until a later ']' or NUL. Each ordinary current byte a is
sign-extended and its low8-based membership bit is set.

If the next byte is '-' and the byte after it is neither NUL nor ']', treat
those three bytes as a range. Let b be the sign-extended endpoint. The current
a bit has already been set; repeatedly increment the signed current value and
set its membership bit while it is <=b. Then advance the format pointer by
three bytes past start/dash/end. There is no escape processing or ascending-range
validation. A descending range retains the start bit and consumes the dash/end
without adding them. Ranges crossing0x7f/0x80 follow signed-byte comparisons,
not unsigned byte ordering or Unicode order; a negative start to positive end
can traverse through numeric zero and set its bitmap bit.

If '-' lacks an eligible endpoint, only the current byte is consumed and the
next iteration processes the dash as a literal byte. A backslash is also an
ordinary bitmap character. '^' is special only at the first position; ']' is
special as an included character only at that initial position. These rules do
not imply modern regex or locale collation behavior.

NUL before a closing ']' marks conversion0xff and skips complement inversion.
Otherwise, if complement mode is set, invert all32 bitmap bytes. Advance one
byte past the closing bracket in the common return path. Invalid length selectors
still traverse/build a scanset but retain conversion0xff. Missing brackets are
not recovered as literal format text.

## Derived examples and validation scope

From these branches: `%f` produces selector0 float storage, `%lf` selector5
double storage, `%Lf` selector6 double storage; `%ls`/`%lc` use selector7 and
its consumer-specific pointer increments. `%hhf` is invalid. `%llp` normalizes
to selector3/x. `%[a-c]` includes a,b,c; `%[c-a]` includes c only; `%[]a]`
includes ']' and a. These are exact branch deductions, not executed samples.

Whole-function fingerprint plus24 instruction anchors pin initializer loading,
width accumulation, selector/dispatch routes, bitmap writes and scanset handling.
Read-only checks pin all40 initializer bytes and all52 jump targets, and verify
the shared classification map's digit set. Original disassembly remains ignored;
no routine implementation is copied or compiled into the native port.

Compare all valid/invalid prefixes, uppercase aliases, explicit0/overflow widths,
returned pointers and complete bitmap bytes against original execution, then
compose with the scan consumer and original icon argument corpus. Integer/
floating/multibyte converters, caller lifetimes and full drawing/layout remain
unverified dependencies. Preserve original IDs/control tokens and story content;
this GameCube contract does not establish Switch parser behavior or full-game
requirements completion.
