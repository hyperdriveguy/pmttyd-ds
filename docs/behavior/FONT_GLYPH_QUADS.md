# Inner glyph width lookup, quad attributes and unconditional RNG

US G8ME01 revision0, recovered2026-10-12. B evidence is original static DOL:
_JUTFont_DrawPos0x80074720,size4dc and kanjiGetWidth0x8007586c,size78.
See [glyph wrappers/setup](FONT_GLYPH_REQUESTS_AND_SETUP.md),
[font state](FONT_DRAW_STATE.md), [message effects](MESSAGE_GLYPH_EFFECTS.md)
and [RNG](RANDOMNESS.md). Font loading/encoding/page ownership, HSV2RGB and
GX/matrix service behavior remain separate. No executed image equivalence here.

## Shared WID1 search and width byte (B)

Both routines use font pointer at0x8041e978 (r13+1a58), read block count at
font+0xc, and scan from font+0x20. Each block's word+0 is compared to0x57494431
('WID1'); word+4 gives the next block displacement. In WID1, unsigned halfwords
+8/+a form an inclusive glyph range. Input glyph is narrowed to low16 bits
for range matching. First matching entry is block+0xc+2*(glyph-rangeStart).
There is no block-length/count/range-order validation or zero-length prevention.

kanjiGetWidth returns unsigned byte entry+1. If no matching block exists,
it sets entry pointer0 and still reads byte+1: no missing-width fallback.
_JUTFont_DrawPos similarly uses null on no match then reads entry bytes, with
no safe return or replacement glyph locally. Actual font data/caller coverage
must establish that required glyphs have width records.

The primitive reads byte0 as unsigned sheet-coordinate offset B, byte1 as
unsigned quad width W. No signed-bearing conversion occurs. Width byte W is
the same value used by layout/half-width callers through kanjiGetWidth.
Neither routine takes a Unicode/text string or translates a message encoding;
those producer/mapping stages remain separate.

## Active glyph descriptor and grid coordinates (B)

Read descriptor pointer at0x8041e99c (r13+1a7c), then unsigned halfwords:
firstGlyph at+8, cellWidth at+c, cellHeight H at+e, columns C at+16, rows N at+18.
Set d=low16(glyph)-firstGlyph as signed word. Through signed divw/mullw/subtract:
globalRow=trunc(d/C), col=d-globalRow*C,
pageQuotient=trunc(globalRow/N), row=globalRow-pageQuotient*N.
No zero-divisor/negative-glyph/descriptor null guard. Valid positive dimensions
and d>=0 give familiar remainder coordinates; retain signed word behavior for
the local algorithm rather than assuming universal unsigned indices.

Convert through original unsigned/signed double biases and binary32 operations:
uFloat=fusedSingle(float(col)*float(cellWidth)+float(B)); quadWidth=float(W);
quadHeight=f32(float(H)-2). Multiplication by original1 is retained in width/
height preparation. The page quotient is used only to reduce the row; this
routine does not switch/load a texture page, call GXLoadTexObj or select a new
descriptor. It relies on active font texture state established elsewhere.

Let truncF mean original fctiwz followed by stored-word load, not a host cast
for nonfinite/out-of-range input. Define low16/low8 narrowing explicitly:

- XL=s16(xOffset), YT=s16(low32(yOffset-2)).
- XR=s16(truncF(f32(float(XL)+quadWidth))).
- YB=s16(truncF(f32(float(YT)-quadHeight))).
- U0=low16(truncF(uFloat)); U1=low16(truncF(f32(uFloat+quadWidth))).
- V0=low16(truncF(f32(2+float(row)*float(H)))).
- V1=low16(truncF(f32(float(V0)+quadHeight))).

The row multiply/add is separate fmuls then fadds; U uses fmadds. V1 starts
from the narrowed V0, not its original wider intermediate. Wrapper matrices
apply scale/current-camera transforms later in GPU processing; the primitive
does not read the shared font scale vector or modify the caller's offsets.

## Quad begin and mandatory noise-coordinate draws (B)

Call GXBegin(0x80,0,4), then irand(64) twice, storing low-byte results A and D
in that order. These calls are unconditional on CPU edge/noise/rainbow/opacity
state. For the recovered nonzero-bound helper each advances the shared rand
state once. They occur even when only one texture generator is active and
the second attribute is visually unused. Do not remove them when noise is off.

Define AR=low8(truncF(f32(float(A)+quadWidth))),
DB=low8(truncF(f32(float(D)-quadHeight))),
DP=low8(truncF(f32(float(D)+quadHeight))). These are attribute bytes, not RNG
calls. There is no fresh random value per vertex.

## Hue requests, FIFO serialization and asymmetric second UV (B)

Read global work word+0x1c. phase=rotateLeft(word,1)&0x1fe, equivalent to
2*(word&0xff). Its producer/time unit remains open; do not label it a measured
frame clock. Set hueBase=low8(XL-phase). Build HSV word from original0x00aaffff,
replacing byte0 for each vertex with the hue below. Call HSV2RGB0x800752fc on
a local copy and write its returned word as four
big-endian RGBA bytes. HSV conversion details remain unrecovered here.

Each vertex writes to FIFO address0xcc008000: three halfwords for position,
four individual color bytes, two halfwords for texture coordinates0, then two
bytes for texture coordinates1. Vertex order and values:

| Vertex | Position | HSV hue byte | UV0 | UV1 |
| --- | --- | --- | --- | --- |
| 1 | (XL,YT,0) | low8(XL+hueBase) | (U0,V0) | (A,D) |
| 2 | (XR,YT,0) | low8(XR+hueBase) | (U1,V0) | (AR,D) |
| 3 | (XR,YB,0) | low8(XR+hueBase) | (U1,V1) | (AR,DB) |
| 4 | (XL,YB,0) | low8(XL+hueBase) | (U0,V1) | (A,DP) |

All position/UV halfwords use low16 bits at the store; position signs are
determined by the established vertex format. The bottom second-coordinate
bytes differ: vertex3 subtracts quadHeight, vertex4 adds it. This asymmetry
is present in original arithmetic and must not be normalized into an assumed
rectangular noise UV mapping. No explicit GXEnd, CPU vertex buffer allocation,
horizontal/scissor clipping, texture-page change or RNG-state restore locally.

## Consequences for message RNG and edge wrappers (B/P)

Each valid inner quad has two explicit irand64 calls. Edge-off wrappers request
one quad; nonzero edge requests five, hence two or ten such calls in those known
paths. msgDisp's shaking adds two earlier irand10000 calls. Standard selected
glyphs start edge-off, draw one quad, enable edge and draw five, then disable it:
twelve inner calls total. Other font/pose/icon/helper consumers still need a
complete call census; these are recovered-path counts, not a full-game budget.

This corrects the earlier incomplete interpretation that only shaking glyphs
consume renderer RNG. Plain/wave/dynamic/history glyphs also reach inner draws,
while skipped glyphs do not. Preserve unused-attribute draws and exact wrapper/
effect order when comparing original/native seed traces. No measured draw rate
or simulation tick is inferred from these counts.

Differential obligations: first matching inclusive WID1, null lookup behavior,
unsigned width/offset bytes, signed grid division, inactive page quotient,
float/truncation/narrowing stages, two unconditional draws, hue phase, four FIFO
vertex/color/UV orders and asymmetric UV1. Remaining: actual font block/glyph/
page data and loading, HSV conversion, FIFO/GX interpretation, word+1c producer,
outer draw cadence and matched original image/seed/story traces. These local
bodies are established; full font/dialogue/complete-game equivalence is unproven.
