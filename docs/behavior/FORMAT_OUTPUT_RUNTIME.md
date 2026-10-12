# Original format output, varargs and formatter consumers

US G8ME01 revision0, recovered2026-10-12. B evidence is static original DOL
instructions/data. No original formatting execution or host-libc equivalence
was measured. See [insert formatting](MESSAGE_INSERT_FORMATTING.md). This
document covers local output/ABI/consumer behavior; format grammar and numeric
conversion helpers remain separate obligations. Original format text is reused.

## sprintf/vsprintf entry and termination (B)

sprintf at0x8026a0b8,sizee0 receives destination r3, format r4 and varargs.
Save r3..r10 into a register-save area. Save f1..f8 doubles only if condition
register bit6 is set; insertion callbacks explicitly clear it. Build a12-byte
varargs record with word0=0x02000000, +4=caller overflow-area pointer and
+8=register-save base. The initial general index is2, skipping destination/
format, while floating index is0. Argument slots are not inferred from format
text by the wrapper itself.

Create writer context {destination,limit=0xffffffff,copied=0}, then call
__pformatter(__StringWrite,&context,format,&varargs). Retain its return word R.
If destination is nonnull, write NUL at destination+index where index=R when
unsigned(R)<0xffffffff, otherwise index=0xfffffffe. Thus R=-1 terminates at
destination-2 using original word addition, not at copied-length or destination0.
Return R unchanged. The insertion wrappers ignore R.

vsprintf at0x8026a198,size78 takes caller-supplied varargs record and otherwise
uses the same writer context, formatter and termination rule. The destination
null test occurs after formatting; it does not prevent __StringWrite memcpy
calls to addresses derived from a null base. No safe size-query API is established.

## String sink counts and clipping (B)

__StringWrite at0x8026a390,size6c receives(context,source,requestedCount).
Read unsigned copied C and limit L. Compute word sum S=C+requestedCount.
If unsigned S>L, accepted=L-C with word arithmetic; otherwise accepted=requested.
memcpy(destination+C,source,accepted), then copied+=accepted. Return1 always,
including zero/clipped writes. No null/capacity/overflow error is reported locally;
word wrap can change the admission compare. It does not append NUL.

The formatter separately counts requested bytes. A context with a smaller limit
can therefore return a larger formatted count than physically copied bytes.
sprintf's limit is the maximum word, not the caller's actual allocation size;
this does not bound insertion's2560-byte stack area.

## __va_arg slot and overflow handling (B)

__va_arg at0x80264750,sizec8 takes(varargsRecord,class). General index is signed
byte0; floating index is signed byte1. Defaults use general index, size4/stride4,
one slot and register threshold8. Class3 uses floating index, size8/stride8,
register-base offset32 and threshold8. Class2 uses general index, size8, two
slots and threshold7; if general index is odd, skip one slot before selection.

Compare the original signed index to threshold before applying that class2
skip. If below, return saveBase+baseOffset+alignedIndex*stride and advance index
by slot count as a byte. Otherwise set selected index byte=8, align overflow
pointer upward to size4/8 with word arithmetic, return aligned address and
advance overflow pointer by size. Class0 additionally dereferences the selected
slot and returns that word as pointer; other classes return the slot address.
There is no local validity check, available-argument count or byte-index clamp.

Integer/string/character consumers request class1, 64-bit integer consumers
class2, floating consumers class3. Four insertion words in general slots cannot
be treated as automatically promoted double values for floating formats.

## Formatter stream and internal format record (B)

__pformatter at0x8026a454,size774 takes(writeCallback,context,format,varargs).
Initialize total count0. Find '%' with strchr; emit preceding literal bytes
through callback and count them. If no '%' remains, strlen and emit remaining
literal bytes, return total. Zero callback result returns-1; any nonzero result
is success, without an accepted-byte-count contract.

For a directive, call printf's parse_format at0x8026bcb4 with(percentPointer,
varargs,formatRecord), retaining returned continuation pointer. The16-byte
record offsets observed by consumers are:

| Offset | Consumer role |
| --- | --- |
| 0 byte | alignment/padding mode:0 trailing spaces,1 leading spaces,2 leading zeros |
| 1 byte | numeric sign policy, interpreted by conversion helpers |
| 2 byte | precision-present flag |
| 3 byte | alternate form; string consumer uses it for length-prefix handling |
| 4 byte | length/argument enum |
| 5 byte | normalized conversion byte |
| 8 word | field width |
| c word | precision |

The grammar, aliases and enum spellings are now recovered in
[directive parser](FORMAT_DIRECTIVE_PARSER.md). Do not equate this record with
modern libc by field names alone.

## Conversion consumers (B)

| Normalized conversion | Established local route |
| --- | --- |
| d/i | Obtain class1 word, except length enum4 gets class2 high/low; enum2 sign-extends low16 and enum1 sign-extends low8; long2str or longlong2str |
| o/u/x/X | Same argument routes; enum2 zero-extends low16 and enum1 zero-extends low8; same numeric helpers |
| e/E/f/F/g/G | Class3 double, including length enum5; float2str |
| a/A | Class3 double, including length enum5; double2hex |
| s | String path below |
| c | Class1 word; place low byte in local buffer, length1 |
| % | Local literal percent byte, length1, no value argument |
| n | Store current total through class1 pointer as described below; emit no bytes |

Numeric helpers return a start pointer into a local buffer; length
is endPointer-start. Null helper result takes fallback, not a universal empty
numeric output. Numeric digit/precision/rounding behavior remains unrecovered here.
Unknown/invalid normalized conversions and helper failure take fallback below;
raw pointer formats may be normalized upstream, so this dispatch alone does not
establish whether a raw format is accepted.

For n: length enum0 or3 stores a word; enum2 stores low halfword; enum4 stores
sign-extended total as high/low words. Other enums consume the pointer but perform
no local store. No pointer null/capacity guard. Do not invent a byte-store path
for enum1 from a modern printf assumption.

## String bytes, precision and null values (B)

For ordinary s, read class1 pointer. Null is replaced by NUL-only original data
at0x80303bb0: empty output, not a fabricated null label. If formatRecord.byte3
is nonzero, first byte is unsigned length and data starts at pointer+1; optional
precision clamps that length by signed compare. Embedded NUL bytes within this
length do not automatically truncate this path.

Otherwise precision absent uses strlen; precision present uses memchr(data,0,
precision) and emits the bytes before NUL if found, or precision bytes otherwise.
No decoding, UTF-8 conversion or message token interpretation occurs here.

Length enum6 takes class1 pointer as wide source, replaces null with pointer
r13+1498, and wcstombs into a512-byte local buffer. Negative conversion result
takes fallback; other results proceed through the same string-length path.
Wide sentinel contents, encoding/error/conversion termination and range behavior
remain wcstombs/runtime-data obligations, not inferred from the narrow empty case.

## Padding order, sign emission and fallback (B)

Let len be conversion byte count and emittedWidth=len. For mode1/2, choose space
or zero. When zero-padding and first content byte is '+', '-' or space, emit that
one byte first, advance data pointer and reduce len by1, keeping emittedWidth at
the original length. Then emit one pad byte per callback while signed emittedWidth
<fieldWidth. Emit remaining conversion bytes. For mode0 instead emit content,
then one trailing space per callback until emittedWidth reaches fieldWidth.
Add emittedWidth to total with word arithmetic. Any zero callback result returns-1.

Fallback strlen-counts the entire remaining format beginning at the original
percent pointer, emits it literally, then returns accumulated total immediately.
It does not resume formatting later directives. This is also the numeric/wide
conversion failure path, so remaining arguments can stay unconsumed. Successful
conversion paths continue at parse_format's returned pointer until input NUL.

## Differential obligations and remaining work (P)

Compare register-save versus overflow alignment, integer versus double slots,
null narrow s, length-prefixed embedded NUL, precision-bounded bytes, character
low-byte narrowing, n enum stores/no-store cases, sign-before-zero padding,
literal-tail fallback/early termination, callback zero/nonzero semantics,
clipped sink count versus formatter total, final NUL placement and failure R=-1.
These are static-derived obligations, not executed library comparisons.

Remaining: parse_format grammar/width/precision consumption, integer and decimal/
hex-float helper algorithms, wcstombs and memory primitives, actual insertion
format/type/count call sites and original execution comparisons. Output/varargs/
consumer local contracts are documented; full formatting/full-game equivalence
remain unfinished.
Integer helper stages are now specified in
[integer conversion](FORMAT_INTEGER_CONVERSION.md); floating/wide helpers and
executed call-site comparisons remain open.
