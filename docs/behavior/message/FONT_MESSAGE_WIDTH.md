# Message width scanning and line-break counting

US G8ME01 revision0; recovered2026-10-12. Evidence B: original DOL
FontGetMessageWidthLine0x8007554c,size0x320 and
FontGetMessageWidth0x80075528,size0x24. See [font mapping](FONT_CODE_MAPPING.md),
[mutable widths](FONT_RESOURCE_SETUP.md), [command split/lead predicate](MESSAGE_RESOURCES_AND_LOOKUP.md)
and [icon dimensions](ICON_DIMENSIONS.md). Static recovery only; no original
execution or native layout equivalence was established.

## Interface and scan state (B)

Input r3 is a NUL-terminated byte stream; r4 is an optional pointer to an output
halfword. Initialize maximum M=0,current width W=0,LF counter L=0. Process until
the current byte is zero. At each iteration initialize glyph G=0xffff, then
call _ismbblead on the signed current byte. If true, read a big-endian halfword
at the current pointer, map it with JUTFont_CodeToGlyph, and advance two bytes.
There is no trail-byte check or terminal-byte guard on that halfword access.
Otherwise dispatch the current single byte: LF, '<', or ordinary glyph.

The lead predicate returns false for every byte when configuration word+0x16c
is nonzero; with word0 it recognizes0x81..0x9f and0xe0..0xfc. This scanner uses
that shared configuration, not INF1's encoding field or a Unicode decoder.
Ordinary bytes, including CR and high bytes outside the lead path, are mapped
as unsigned byte codes and advance the input by one.

An ordinary LF updates M if low16(W)>low16(M), sets W=0, increments L as a
32-bit word, and advances one byte. L counts processed LF bytes, not visual
lines: empty input gives0, a single nonempty line gives0, and a trailing LF adds
one. No wrap width, screen size, inserted soft break or drawing callback is used.

## Glyph advance and trailing spacing (B)

If low16(G)==0xffff, skip the width lookup. Otherwise scan original WID1 blocks
for the first inclusive glyph range and read the entry's unsigned width byte.
A failed lookup yields null and is still dereferenced; no replacement width
appears here. Set W=low16(W+width+spacing), where spacing is the word selected
from table0x8041601c using configuration word+0x16c times4. The original table
has seven US entries4,2,2,2,2,2,2. The lookup has no index bound. The pinned
Japanese source declares only two entries, so its array length must not be
substituted for this US binary table.

The current in-memory width table includes [setup's patches](FONT_RESOURCE_SETUP.md).
This helper never multiplies ordinary glyph widths by fontScale or changes
style based on tags. After input ends, compare low16(W) with low16(M) and retain
the larger. If low16(M) is nonzero, return low16(M-spacing), using the current
configuration again. If maximum is zero, leave zero unchanged. Thus the final
subtraction is applied even when the widest contribution consists of an icon,
not an ordinary trailing glyph. Unsigned comparison/wrapping are material:
large streams can wrap instead of saturating; a positive maximum below spacing
can underflow the final halfword.

If r4 is nonnull, store low16(L) through it. FontGetMessageWidth forces r4=null
and invokes this same routine; it does not choose a different scanner or width
policy. Input bytes and font widths are not written by these routines.

## Command handling and LF suppression (B)

On '<', call msgGetCommand(input+1,nameStack,argStack); replace the input pointer
with its returned pointer without checking null. The splitter processes at most
128 bytes, does not stop at NUL, and does not initialize an absent argument.
Consequently malformed/truncated markup is not a safe literal-text fallback.
Compare the full name to original string `icon` at0x80420408.

For other names, inspect only the name's first byte. If it is uppercase R,
continue immediately, allowing a following LF to be processed normally on the
next iteration. Otherwise the code tests first byte '/' and then compares that
same byte value to0x2f72. A byte cannot equal0x2f72, so the latter branch is
unreachable under the inspected instructions. It is not an established '/r'
name exception. All remaining commands consume one LF immediately following
the parsed end pointer without updating M, W or L. No other command effect is
applied here: style, scale, color, waits, selection and message events are not
interpreted as in the [display parser](MESSAGE_PARSER_RECORDS.md).

This distinction makes a generic remove-all-markup-and-split-on-LF routine
inadequate: R-prefixed names and other names have different LF treatment.
Preserve original control tokens; do not rewrite story dialogue to approximate
this scanner's results.

## Icon width contribution (B)

For exact `icon`, call camGetCurPtr and discard its returned pointer locally.
If signed argument byte0 is ASCII0..9, request sscanf with `%d%f%d%d%d`.
Otherwise request `%s%f%d%d%d`; resolve the resulting name with msgIconStr2ID
and replace a negative result with ID0. A leading '+' or '-' therefore takes the
name route. The five outputs are ID/name,scale Q,horizontal offset J,auxiliary
integer Z,advance N. Conversion count is ignored; fields have no local defaults.
The name output starts at a32-byte stack region and `%s` has no width bound.
See [scan entry](../format/SCAN_INPUT_RUNTIME.md) and
[consumer behavior](../format/SCAN_FORMAT_CONSUMERS.md) for ABI, destination writes and
failed conversion counts. Scan grammar/numeric dependencies remain open; the
printf contracts do not establish them.

Pass low16(ID) to iconGetWidthHight with local halfword width/height outputs.
Only width I, Q and N affect this routine's advance; height, J and Z are ignored.
The arithmetic is:

```
t = fused_single(float_unsigned16(I) * Q + float_signed32(N))
u = SP(float_unsigned16(low16(W)) + t)
W = signed32_truncate_toward_zero(u)
```

The fused operation is fmadds0x80075708, followed by fadds then fctiwz. W retains
the full integer result until a later low16 use; a subsequent glyph advance
explicitly narrows it. There is no extra glyph spacing added at this icon step,
but the routine's final spacing subtraction still applies. There is no special
HM glyph branch here, unlike the [record parser](MESSAGE_EMBEDDED_OBJECTS.md).
After the icon, normal post-command LF suppression runs and G remains0xffff.
No local clamp of Q, N, integer overflow or malformed icon arguments occurs.
Floating status/conversion edge behavior depends on the actual original FPSCR.

## Evidence and reimplementation obligations

Fingerprints pin both complete selected functions; independent anchors check
lead/command routing, LF/max changes, icon fused arithmetic, sentinel, width
accumulation and return. Checks also pin the seven spacing words, icon string and
two scanf format strings. Static validation does not execute the scanner or
verify runtime configuration/font/icon state.

Compare empty/trailing/multiple LF, R-prefixed and slash commands, post-command
LF, two-byte boundaries, glyph0xffff, unmapped codes and mutable space widths.
Include icon-only lines, mixed icons/glyphs, numeric/named arguments and halfword
wrap cases; establish scanf and icon resource lifetimes first. Recover drawing
string/line consumers and actual callers before claiming full layout or story
fidelity. Switch behavior is not inferred from this GameCube routine.
