# Retained original font message width

`source_font_width` owns the whole US `FontGetMessageWidth` wrapper
`80075528/24` and `FontGetMessageWidthLine8007554c/320`. It supplies the genuine
width child of [centered source text](RETAINED_CENTERED_STRING_FONT.md). It does
not measure native glyph quads or substitute atlas-label widths.

## Source control and memory

The loop performs the original separate end-test byte, language-sensitive lead
classification, true text LHZ on the multibyte route, and fresh single-byte map
load. Lead/map/WID scans reuse the exact recovered
[matrix font helpers](RETAINED_MATRIX_FONT_RASTER.md). Actual admitted original
BFN, GP/language, spacing and source memory remain caller-owned. Actor memory
callbacks have the same admitted total-memory contract as those helpers;
optional text reads distinguish unavailable bytes from known NUL. The raw US
BFN in the tests is unchanged. Any genuine startup glyph-zero width mutations
remain the actual shared font producer's responsibility.

Newline compares low16 widths, updates the maximum only when greater, clears
the current width and increments the newline count. Normal glyph addition
masks to16 after actual WID width and fresh language spacing. An unmapped
`ffff` glyph bypasses width. EOF compares low16 again and subtracts fresh
language spacing only for a nonzero maximum. A supplied original nonzero line
pointer receives a real halfword store; the wrapper passes null.

## Commands and source-private ownership

The whole original `msgGetCommand80081aa0` child retains its reads. One stable
optional-byte frame models the original offsets, without a fabricated guest
stack address: icon dimensions08/0a, float scale0c, signed extra10, other scan
outputs14/18, numeric icon1c, string output20, arguments40 and command namec0.
Aliases are preserved, including a string scan reaching its argument input.
Unknown cells are never zero-filled or terminated. Only actual writes merge
into previous command output; a later command without arguments can reuse
actual preceding argument bytes. Unknown output reads hold after an already
completed child instead of replaying it or inventing stack contents.

Typed requests preserve each original function, callsite, format/literal,
private input and output offset. The genuine child owner must retain its own
acknowledged effects while returning unavailable. A returned zero is an actual
return. Required children are:

- `strcmp8026d464` at75604 against actual `80420408` (`icon`).
- `CameraGet8000f2e4` at75610 even though its return is unused.
- `sscanf8026c344` at75650/7567c, with original `%d%f%d%d%d` or
  `%s%f%d%d%d` at802c2bbc/c8; its return is ignored.
- `msgGetIconId80081a1c` at75684, with original negative-result clamp.
- `iconGetSize8001a19c` at756ac with width/height outputs and id low16.
- Actual binary32 FMA and truncation for the width arithmetic.

Icon width and signed extra are captured in source order with actual unsigned
and signed double biases802c2b90/98. Scale is read at the later source point.
The current low16 plus `fma(width,scale,extra)` is rounded as original float
addition, then actual fctiwz returns raw current bits. Later glyphs mask16;
newline/max comparisons consume low16. An `R` command skips the following
newline test; the original signed-byte `'/' == 0x2f72` test cannot match, so
`/r` takes the normal optional-newline-skip path. No normalized markup parser
replaces that behavior. A null command return uses actual null guest byte/half
providers; private offset0 is never substituted for guest null.

## Retention and integration

The cursor pins actual context/text strong control-block identity, borrowed
text object, line pointer and every service identity. All original children,
private outputs and successful source reads survive a hold. Reentry/reset
while invoking is rejected. Current loss latches stale, including loss inside
a child or actor read. Reset is native metadata only. It writes no guest
readiness/queue state.

Bind one stable width cursor and service table alongside each actual centered
font invocation. Its width callback delegates `resumeSourceFontWidthLine` with
the exact received text object and line pointer; keep this context alive through
holds. Actual native source memory must be admitted before total actor reads.
The plain US path needs no camera or tagged-command provider. Command branches
require the true typed children, not a scanner/icon success stub. The executable
test composes the whole centered parent with this whole width owner and original
US BFN; its matrix/glyph callbacks are explicitly source-controller test leaves,
not normal battle GPU or queue admission.

## Validation and limits

`verify_source_font_width.py` pins both entire parent bodies, genuine lead/map/
command helpers,11 linked calls, original literals/biases/spacing and raw US BFN
against the read-only ISO. Focused strict host, ASan/UBSan and GNU ARM syntax
checks pass using one consistent frozen6458 include/archive/RomFS graph plus
new width and frozen centered source files. Tests cover every non-control US
byte, multiline/empty/wrapper behavior, numeric/named icons, all child holds
twice, private read holds, source order and partial outputs, service/text owner
replacement, reentry, stale lifetime, raw16 wrapping, true multibyte LHZ and
fresh final spacing. Tagged child responses are explicitly test models; their
production implementations and actual runtime binding remain external.

No normal Weapon/menu activation, source queue completion, camera readiness,
native raster proof, original PPC framebuffer or physical-device parity is
claimed by this source controller. Root owns full-build registration and gates.
