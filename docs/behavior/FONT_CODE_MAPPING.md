# Font code-to-glyph mapping

US G8ME01 revision0, recovered2026-10-12 from JUTFont_CodeToGlyph
0x80074e90,size0x174; kanjiSearch0x800758e4,size0x20; and
hankakuSearch0x80075904,size0x24. Evidence B: static original instructions and
original BFN descriptors. See [resource setup](FONT_RESOURCE_SETUP.md) and
[glyph widths/quads](FONT_GLYPH_QUADS.md). No original execution was performed.

## Input narrowing, default and first-match search (B)

JUTFont_CodeToGlyph takes code in r3. Range/search operations use its low16 bits.
The initial result is unsigned halfword INF1+0x12, from the active info pointer
0x8041e9a0. Load font pointer0x8041e978 and word+0xc block count; scan from+0x20
in file order, advancing by block word+4. Only MAP1 tags are considered.
The candidate's low16 code must be in the inclusive unsigned range
MAP1+0xa..+0xc. On the first in-range MAP1, apply the mapping type at+8 and
return immediately, including an unknown type or a failed type3 search.
There is no retry in a later overlapping MAP1. If no range matches, return the
initial replacement glyph. Missing font/info pointers, malformed sizes/ranges
and mapping-table bounds are unchecked locally.

kanjiSearch forwards its argument unchanged into this helper. hankakuSearch
first masks it to an unsigned byte. Neither is a text decoder or a width getter;
both return a glyph index. No helper calls, allocation, texture state writes or
RNG operations occur inside JUTFont_CodeToGlyph.

## Mapping types (B)

All returned glyphs are unsigned halfwords. Four type routes are recognized:

| Type | Lookup |
| --- | --- |
| 0 | low16(code-firstchar) |
| 1 | Byte-row arithmetic described below |
| 2 | u16 at MAP1+0x10+2*(low16(code)-firstchar) |
| 3 | Binary search of four-byte code/glyph pairs at MAP1+0x10 |

Type0's subtraction uses incoming r3 rather than the separately masked register;
its final low16 mask makes high input bits irrelevant. It does not add a glyph
base from the payload. Type2 does not test a sentinel or substitute if the table
halfword is0xffff; it returns that halfword directly.

For type1, store the input halfword and split it as high byte hi and low byte lo.
Set t=lo-64 if lo<128, otherwise t=lo-65. If halfword MAP1+0xe is zero, return
low16(188*(hi-136)+702+t). Otherwise return
low16(188*(hi-136)-94+u16(MAP1+0x10)+t).
The range gate is the only input restriction here; there is no local validation
of legal multibyte lead/trail values. The conditional halfword is used as a gate,
not a loop count in this mapping route. Ordinary host character encoding APIs
are not established substitutes for this byte arithmetic.

## Type3 binary search and boundaries (B)

Initialize signed low=0 and high=u16(MAP1+0xe), **without subtracting one**.
While high>=low, compute mid=trunc_toward_zero((low+high)/2), using the original
signed sum/sign-adjust/arithmetic-shift sequence. Read the pair code at
MAP1+0x10+4*mid. On equality, return its glyph halfword at+2. Otherwise an
unsigned input-code comparison sets high=mid-1 for a smaller code or low=mid+1
for a larger code. Exhaustion returns the original replacement glyph immediately.

This route expects sorted pairs but does not check sorting or the block boundary.
The inclusive initial high can address one pair beyond a conventional count.
US/EU files have28 sorted pairs; the next four bytes are the following GLY1 tag,
whose halfwords are0x474c/0x5931. The Japanese file has210 sorted pairs and a
zero padding pair after them. In each inspected file, the last real pair code
also equals MAP1's upper range endpoint, so a code that passed the range gate
cannot exceed that final code. Under this data ordering, a missing code below
that endpoint exhausts before reading the extra pair; the last code matches a
real entry. This reachability deduction does not justify silently changing the
recovered bound for other/corrupted resources.

## Concrete resource coverage (B)

Each of the three original fonts has seven type2 MAP1 blocks and one type3;
no type0/type1 block is present in this local inventory. Those routes are binary
contracts, not demonstrated normal gameplay cases. For US/EU, type2 ranges in
file order are0xde..0xe2,0xd6..0xdc,0xe7..0xee,0xba..0xc1,0xc7..0xd4,
0x20..0x3b,0x3f..0x7e; the final type3 range is0x3d..0xfc.
Ranges overlap, making first-match behavior material. Type3 itself covers a
sparse set of28 sorted code/glyph pairs, not every code in its range.

The original on-disc INF1 replacement is0 for all three fonts. ASCII space
maps to glyph0; US/EU code0x8140 has no matching range and returns0. Japanese
code0x8140 is a type3 pair mapping to glyph0x5d. This distinction explains the
[setup width mutations](FONT_RESOURCE_SETUP.md); do not convert an unmapped
code to an invented question-mark glyph as a fidelity assumption.

## Validation and remaining scope

Whole-function fingerprints and independent anchors pin default loading,
range routing, mapping arithmetic, table access and search bounds; full BFN
hashes pin the reused data. Static inspection establishes no text-decoder,
Unicode, localization or runtime-font-selection equivalence. Validate mapping
results for every original input code, overlapping ranges, absent pairs,
replacement behavior and width patches against original execution. Then recover
byte-stream token/encoding consumers and glyph/string layout callers. Adapted
screen reflow must preserve message content and event conditions; the GameCube
mapping alone does not establish Switch behavior.
