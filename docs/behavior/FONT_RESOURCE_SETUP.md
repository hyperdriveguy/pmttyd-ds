# Font loading, descriptor selection and width patches

US G8ME01 revision0, recovered2026-10-12. Evidence B: JUTFontSetup
0x80075004,size0x2f8, original DOL constants and three read-only ISO BFN
resources. The pinned public Japanese font manager has a partial setup routine;
it is a structural aid, not proof of this US implementation. See
[code mapping](FONT_CODE_MAPPING.md), [glyph quads](FONT_GLYPH_QUADS.md) and
[GX draw setup](FONT_GLYPH_REQUESTS_AND_SETUP.md). No original loading/rendering
execution was performed.

## One-time guard and resource requests (B)

The routine receives a selector in r3, saves it, and returns immediately if
font pointer0x8041e978 is nonzero. It does not compare the new selector against
an existing font or reload after a language change. No local reset/free path
exists. On first setup, index=(selector<<2)&0x0003fffc selects a word pointer in
the name table0x80416038: this uses the low16 bits of the selector, with no
seven-entry bounds check.

| Table index | Resource basename |
| --- | --- |
| 0 | papermarioset_JPN |
| 1 | papermarioset_US |
| 2,3,4,5 | papermarioset_EU |
| 6 | papermarioset_US |

These are table indices, not a recovered user-facing language enum. The initial
path is formatted into local stack storage using `%s/f/%s.bfn`, the current
getMarioStDvdRoot result and the indexed name. It calls DVDMgrOpen(path,2,0),
then DVDMgrGetLength on that result without a null check. The chosen handle and
length remain live while a second loop probes all seven table entries using
`%s/font/%s.bfn`, again Open(path,2,0). For each nonnull probe handle, update the
maximum length using a signed comparison and close that handle; null probes
are skipped. Duplicate names are still probed separately.

Allocate on heap0 with size=(maximum_length+31)&0xffffffe0. Publish the returned
pointer to0x8041e978 before reading. DVDMgrRead receives the selected handle,
that buffer, (selected_length+31)&0xffffffe0, and offset0; its result is ignored.
Close the selected handle. No allocation/read failure recovery, zero fill,
header/version validation or rollback appears locally. Publishing the pointer
is not evidence of asynchronous concurrency safety.

The ISO inventory contains the three `f/` BFN paths below and no `font/` BFN
paths. This does not establish how DVDMgrOpen resolves runtime-root paths or
whether its service remaps/falls back; those service bodies and actual loading
callers remain open. Do not claim the maximum is the Japanese file size merely
because that resource exists under a different path.

## Block scan and texture requests (B)

Read the font's word+0xc as block count; begin at font+0x20 and advance by each
block's word+4. Matching INF1 stores its address at0x8041e9a0; matching GLY1
stores its address at0x8041e99c. Both are overwritten for each match, so the
last matching block wins. MAP1/WID1/other tags cause no descriptor write.
There are no local count/size/file-end checks, required-block checks or fallback
initializations. Later consumers dereference the resulting pointers.

Texture object0x8041e97c is initialized using the selected GLY1:

| GXInitTexObj argument | Request |
| --- | --- |
| Image pointer | GLY1+0x20 |
| Width / height | unsigned halfwords GLY1+0x1a / +0x1c |
| Format | 0 (I4 in the pinned GX enum), hardcoded rather than GLY1+0x14 |
| S/T wrap | 0 / 0 (clamp) |
| Mipmap | 0 |

GXInitTexObjLOD then receives min/mag filter1/1 (linear), min/max LOD and bias
all positive float zero, bias-clamp0, edge-LOD0 and anisotropy0. These are
service requests; decoded GX object fields/raster behavior are separate work.
The routine does not use GLY1's sheet-size word to switch image pages. The draw
setup's [normalization matrix](FONT_GLYPH_REQUESTS_AND_SETUP.md) and quad grid
must be interpreted together with these physical dimensions.

## Ordered mutations of original width data (B)

After texture setup, map code0x20 through JUTFont_CodeToGlyph. Search WID1 blocks
in file order for the first inclusive glyph range containing that result.
The entry is block+0xc+2*(glyph-first). Store7 into entry+1, preserving its offset
byte. Repeat the mapping/search for code0x8140 and store9 into its entry+1.
Neither failed WID1 lookup is checked: the null-result path still writes byte1.
The width patches occur after publishing/loading/scanning/texture requests.

In US/EU resources, INF1 replacement glyph is0; code0x20 maps to0 through a
type2 MAP1, while0x8140 falls outside every map and returns replacement0.
Both writes therefore target glyph0's width, whose on-disc value is1: first7,
then9. Its final in-memory width is9 under this setup path. This is a static
code-plus-data derivation, not an executed observation or proof that every
runtime uses this font/selector. In the Japanese resource0x8140 maps to0x5d,
so these two patches target different glyphs. Preserve ordering and aliasing
when reusing original width data; an immutable on-disc width table changes the
behavior of recovered width/layout consumers.

## Original resource shape (B)

| ISO path | File size | Block count | WID1 glyph range | GLY1 cell / grid / dimensions |
| --- | --- | --- | --- | --- |
| f/papermarioset_US.bfn | 66560 | 11 | 0..161 | 24x24 / 21 columns,8 rows / 512x256 |
| f/papermarioset_EU.bfn | 66560 | 11 | 0..161 | 24x24 / 21 columns,8 rows / 512x256 |
| f/papermarioset_JPN.bfn | 264896 | 11 | 0..489 | 24x24 / 21 columns,24 rows / 512x1024 |

Each file consists of INF1, WID1, eight MAP1 blocks and GLY1, whose end exactly
matches file size. US/EU offsets are INF1=32,WID1=64,GLY1=992; Japanese offsets
are32,64,2720. GLY1's format halfword is0 in these files, and its payload size
is65536 or262144 bytes respectively. The descriptor grid uses+0x16 as horizontal
column count and+0x18 as vertical row count in the recovered draw arithmetic;
the public struct's row/column naming should not reverse those actual uses.
The Japanese vertical grid covers576 pixels of its1024-pixel texture; do not
infer additional live glyphs merely from unused texture area.

The independent verifier hashes all three complete BFN files, checks the seven
name pointers and five name/path strings, and checks setup/mapping instruction
anchors. Original payloads and a local descriptor report stay ignored under
artifacts/reference/behavior; no converted font or copied executable is committed.
Original asset rights remain unchanged; see [provenance](../ASSETS.md).

## Remaining work and port obligations

Preserve lazy setup, actual selector/caller conditions, resource identity,
last-descriptor selection, first-width selection and ordered mutable widths.
Establish DVD/root/allocation/read lifetime/error semantics, text byte decoding,
message layout/string wrappers, actual loaded texture state and executed glyph
coverage before claiming complete font/dialogue equivalence. Reflow must retain
original message IDs/tokens and story content. No Switch font behavior is inferred.
