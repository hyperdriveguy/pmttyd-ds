# Original multibyte length check and halfword conversion

US G8ME01 revision0, recovered2026-10-12. Evidence B: mbtowc at0x80269a5c,
size0x120 and is_utf8_complete at0x80269b7c,size0xec, original DOL instructions.
Names are symbol annotations; their behavior is established by the inspected
bytes. See [scan formatter consumers](SCAN_FORMAT_CONSUMERS.md) and
[font code mapping](../message/FONT_CODE_MAPPING.md). This runtime decoder is distinct from
the game's Japanese lead-byte/glyph path. No original execution or modern
UTF-8 library equivalence was measured.

## Length/completeness helper (B)

is_utf8_complete(source,available) takes r3/r4 and returns a signed word. If
unsigned available==0, return-1 without reading source. Otherwise read byte b0;
source is unchecked. The complete local decision table is:

| Lead/input | Return and additional access |
| --- | --- |
| b0==0 | 0 |
| 1<=b0<=0x7f | 1 |
| 0xc0<=b0<=0xdf, available<2 | -2, no trail read |
| 0xc0<=b0<=0xdf, available>=2 | 2 if b1&0x80 is nonzero, otherwise-1 |
| 0xe0<=b0<=0xef, available>=3 | 3 if both b1&0x80 and b2&0x80 are nonzero, otherwise-1 |
| 0xe0<=b0<=0xef, available==2 | -2 if b1&0x80 is nonzero, otherwise-1 |
| 0xe0<=b0<=0xef, available==1 | -2 without a trail read |
| Any other lead | -1 |

On the complete three-byte route, a bad b1 exits before reading b2. The
continuation condition checks only bit7, admitting0x80..0xff rather than just
0x80..0xbf. Four-byte leads0xf0..0xff are rejected, as are isolated0x80..0xbf.
The helper does not decode a scalar, reject overlong encodings, check surrogate
ranges, inspect locale/encoding configuration, maintain state or write output.
It reports incomplete forms as-2, distinct from malformed/zero-capacity-1.
Size comparisons are unsigned; a huge size admits the local reads without
checking actual allocation length. It does not advance a caller cursor.

## mbtowc entry and error handling (B)

mbtowc(destination,source,available) takes r3/r4/r5. If source is null, return0
immediately without writing destination or resetting any global shift state.
This null-source route precedes available==0. For nonnull source and available0,
return-1 without a source read. Otherwise call the helper above. Any negative
helper result becomes-1; incomplete and malformed input are not distinguished
in mbtowc's return. Destination is not written on these exits. There is no local
errno write, retained partial-input buffer or locale dispatch.

For helper length0,1,2,3, compute a candidate halfword U as below, then compare
the candidate's required byte-length classification against that helper result.
No additional available-length checks occur after the helper. The normal
1/2/3 routes read exactly those source bytes; the length0 route reads no further
byte and begins with U=0.

## Exact decode and canonical-length test (B)

All loads are unsigned bytes; the original shifts/masks yield:

| Helper length | Candidate U |
| --- | --- |
| 0 | 0 |
| 1 | b0&0x7f |
| 2 | low16(((b0&0x3f)<<6) \| (b1&0x7f)) |
| 3 | low16((((b0&0x0f)<<6) \| (b1&0x3f))<<6 \| (b2&0x7f)) |

The final byte contributes **seven bits**, while the intermediate byte on the
three-byte route contributes six. This is an OR, not addition: a final bit6
can overlap a bit already present in the accumulated value. Preserve the masks
and overlap rather than substituting a standard UTF-8 scalar decoder.
The first three-byte stage masks to0x03c0 after shifting the lead, and the
second masks to0xffc0 after shifting the combined value; the table expresses
the same result for the helper's admitted lead ranges.

Classify U: length0 when U==0, length1 when0<U<0x80, length2 when
0x80<=U<0x800, otherwise length3. If this classification differs from the helper
length, return-1 and leave destination untouched. This rejects overlong forms
according to the **computed candidate**, not full modern UTF-8 validity.
If equal, store U as a halfword at destination when destination is nonnull,
then return helper length. Null destination still performs validation/decoding.
There is no U+10ffff maximum, surrogate rejection, four-byte support, replacement
character or multiple-halfword emission in this routine.

## Derived boundaries and scan-consumer composition

The following cases are deductions from the original branches and masks, not
executed results:

| Bytes / available | is_utf8_complete | mbtowc outcome |
| --- | --- | --- |
| 00 / 1 | 0 | Store halfword0,return0 |
| 41 / 1 | 1 | Store0x0041,return1 |
| c2 / 1 | -2 | Return-1,no store |
| c0 80 / 2 | 2 | Candidate0 has length0;return-1,no store |
| c2 c0 / 2 | 2 | Candidate0x00c0 has length2;store/return2 despite nonstandard trail |
| ed a0 80 / 3 | 3 | Candidate0xd800;store/return3 without surrogate rejection |
| f0 90 80 80 / 4 | -1 | Return-1,no store |

The scan parser maps l on c/s/[ to selector7. Those consumers call mbtowc with
a pointer to **one local byte and available1** for each accepted input byte;
they ignore its return. Thus ASCII bytes write halfwords, but any non-ASCII byte
returns-1 without an output store. A multibyte sequence cannot be assembled
across those calls because this converter is stateless and each call has size1.
Consumer assignment/accepted-byte counts can still advance despite no decoder
write. No placeholder or initialized destination value should be invented.

For c the consumer advances the destination by one byte after each call, so
successful halfword writes can overlap. For s/[ it advances by two and appends
a halfword NUL. Suppressed/null destination routes do not call mbtowc at all.
The byte reader and consumer's own0xff EOF narrowing are separate from these
helper checks. See the [consumer contract](SCAN_FORMAT_CONSUMERS.md) for exact
partial/EOF/accounting behavior before claiming end-to-end scanning equivalence.

This runtime is not a reason to reinterpret original US message bytes as UTF-8.
The [message lead predicate](../message/MESSAGE_RESOURCES_AND_LOOKUP.md) and BFN glyph
mapping use their own configuration/byte domains, and story/control tokens must
be retained unchanged.

## Evidence and remaining obligations

Two complete-function fingerprints and twenty-four independent anchors pin
null/size/error routes, lead and continuation masks, incomplete results,
decode masks, canonical-length thresholds and final halfword write. These are
read-only binary checks; no original routine is executed or compiled into the
native port. Original inspection ELF/disassembly/report outputs remain ignored,
with no asset conversion or license change.

Compare source access order, all byte/length boundaries, permissive trail cases,
overlong candidates, surrogate values, null pointers and destination writes
against original execution. Establish actual wide-format call sites before
claiming those paths are exercised in original gameplay. Floating scanning and
numeric conversion helpers, full text/drawing callers, localization/resource
lifetimes and executed layout/images remain open. Full-game documentation and
requirements are incomplete; no Switch decoder behavior is asserted.
