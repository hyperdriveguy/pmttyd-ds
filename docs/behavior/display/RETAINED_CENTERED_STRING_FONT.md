# Retained original centered FontDrawString

`source_string_font` implements the complete US body80076a10..80076cd8
(716 bytes), separately from StringMatrix80076cdc and ScalarFont800773d0.
It is required by original weapon MainIcon speech-bubble labels. It does not
substitute either sibling, an estimated label width or a new menu style.

## Original operation order

The source fetches actual current camera8000f2e4, calls
FontGetMessageWidthLine8007554c(text,0), masks its result to16 bits and centers
its pen using the original unsigned-double conversion and binary32 multiply
by negative one-half. It concatenates actual camera+11c view with the caller's
matrix once, then loads that matrix at GX index0. It captures the signed-double
conversion constant once after loading the matrix. The input matrix is read
when concat is reached, after camera/width children, and retained through a hold.
No standalone projection, second view transform or native screen offset is added.

The drawing loop reads each byte, tests the source language-sensitive lead
predicate and maps the actual single byte or source LHZ to a glyph. It does
**not** parse commands, skip '<...>', implement newline layout, or skip glyphffff.
Those behaviors cannot be borrowed from StringMatrix. Native glyph-profile
admission can refuse unsupported glyphs before RNG; that is a native limitation,
not a source skip. The initial loop-test byte and subsequent single-byte source
load remain separate reads. The multibyte route performs one actual LHZ.

When source outline is enabled, the source saves the current color, installs
actual palette0 with current global alpha, and emits four glyphs at x-2/x+2 and
y-2/y+2. Two distinct fctiwz calls are retained before the two pairs. The source
then restores its saved color with freshly reloaded alpha. The main glyph has
its own fctiwz call. Shadow coordinates explicitly wrap through signed16; the
main call retains its full source integer operand for the actual JUT child.

After each glyph, source scans actual WID1 and reloads actual GP language and
spacing. It performs signed-word conversion of spacing+width followed by a
binary32 addition to its pen. It does not infer spacing from a native font
measurement. The native original US glyph owner still consumes exactly two
original RNG calls per admitted glyph; rejection and retained waits emit no
GPU prefix and do not replay earlier glyphs.

## Ownership and unavailable children

`SourceStringFontCursor` retains source position, base/text identities, captured
matrix/numeric/child operands and successful prefix. Changed services, streams,
borrowed matrix identity or captured matrix bytes refuse; current loss is sticky.
Request/reset reentry is refused. Total actor-memory callbacks must represent
actual admitted source storage, as in the matrix/scalar owners. Unknown private
text bytes use optional reads; neither a missing byte nor an unwritten suffix
becomes NUL. Source memory, font/global state, camera and native resources must
remain owned by the genuine invocation.

The width callback is specifically original8007554c(text,0). It receives the
same stable typed source stream and genuine null line-count destination. It is
not a generic native string-width callback. This batch does not implement that
whole child: its command/sscanf/icon branches remain an independent dependency.
Tests explicitly supply a width-child fixture return to verify the parent's
low16 centering and hold protocol; this is not production width completion.
Actual source parent completion is unavailable until a genuine provider admits
that child. Width, actual SDK matrix, numeric conversion, effective TEV and JUT
raster providers remain mandatory at their reached boundaries. No guest stack
address or source readiness flag is manufactured.

## Evidence and limits

`tools/verify_source_string_font.py` pins the whole original parent, all14 direct
calls, actual lead/map/JUT reference bodies, three literal/table spans and original
US BFN from the read-only ISO. Static pins are not executed-PPC evidence.
Focused tests use real source constants/BFN/atlas metadata, the original 7→9
Setup glyph0 mutation, actual finite glyph RNG/quad admission, an explicit
width fixture, and retained child waits. They check raw '<'/newline mapping,
multibyte LHZ, low16 width, source view/base capture, late spacing/alpha and stale
owners. Native startup/whole width/MainIcon/normal battle/hardware/PPC raster
parity are not established by these tests. This batch changes no story dialogue,
original assets, simulation timing or controls.
