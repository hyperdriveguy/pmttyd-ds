# Shared font draw state, alpha composition and GX effect requests

US G8ME01 revision0, recovered2026-10-12. B evidence is static original DOL
and original initializers. The14 local font state helpers below are recovered;
JUTFont_DrawStart and FontDrawCode/Mtx local requests are now covered in
[glyph/setup requests](FONT_GLYPH_REQUESTS_AND_SETUP.md); inner GX/font raster
services remain separate. See [draw admission](MESSAGE_DRAW_RECORD_ADMISSION.md),
[glyph effects](MESSAGE_GLYPH_EFFECTS.md) and [window manager](MESSAGE_WINDOW_MANAGER.md).
No original glyph/texture assets or implementation bodies are copied into source.

## Shared storage and scale setters (B)

| Address | Direct state role |
| --- | --- |
| 0x8041e9a4 (r13+1a84) | Word edge flag |
| 0x8041e9a8 (r13+1a88) | Byte draw alpha |
| 0x8041e9ac (r13+1a8c) | Raw RGBA color word |
| 0x8041e9b0..b8 (r13+1a90..98) | Three scale words/floats |
| 0x80416010 | Three-entry original palette, followed by charspace data |

These are global shared fields, not per-message records. FontDrawScaleVec
(0x8007773c,size1c) copies exactly three words from its pointer to scale storage,
preserving input bit patterns without floating computation. FontDrawScale
(0x80077758,size14) stores f1 as single precision into all three scale fields.
No pointer/range/finite validation or draw/GX call in either helper.
FontGetDrawColor(0x8007776c,size8) returns the raw color word by value.

## Color scaling and caller-buffer mutation (B)

For color alpha A and global draw-alpha byte G, effective alpha is
floor(A*G/255), computed through the signed0x80808081 reciprocal sequence,
arithmetic shift7 and sign correction. Both inputs are bytes; RGB is preserved.
All color request helpers below call GXSetTevColor(1,&localEffectiveColor).

FontDrawColor(pointer),0x800777e0,size64: read the supplied fourth byte A and
full word. Store that full word to raw global color first. Compute effective
alpha, then write it back to pointer+3, read the modified word into a local
copy and call GXSetTevColor. The caller buffer is mutable even though its value
is also retained globally. Ordinary nonaliasing raw storage keeps unscaled A;
if the supplied pointer aliases raw global color, the subsequent alpha write
also changes that global word. No alias/null guard.

FontDrawColor_(),0x80077774,size6c: read raw global color, compute effective
alpha in a local copy, redundantly store the same raw word globally and request
the effective color. There is no external color-pointer write. This restores
the requested effective form of current raw color, not a saved earlier palette.

FontDrawColorIDX(index),0x80077844,size70: index the word table at0x80416010 with
low32(index<<2), store the selected raw word globally, scale only a local copy
and request it. Original initializers are index0=0x1e1414ff,
index1=0xdcdcdcff,index2=0xb47814ff. No three-entry bounds check; larger/negative
indices can read neighboring storage. Palette writers/lifetime remain untraced.

Derived examples, not original execution: A=255,G=128 gives effective128 while
raw color remains alpha255 for a nonaliasing pointer. With A=128,G=128, the
first call writes caller alpha64; reusing that mutated pointer writes32 on the
next call and stores raw alpha64 before doing so. Saving FontGetDrawColor's
word and later passing a fresh local copy avoids that particular pointer reuse,
but does not restore every other font/GX field.

## Draw-start reset and delegated setup (B)

FontDrawStart_alpha(alphaWord),0x80077ac0,size8c, stores low8(alphaWord) as G,
clears edge word, restores scale words from original(1,1,1) at0x802c2b6c and
sets raw color to current palette index0. Compose its effective alpha as above
in a local copy, then call JUTFont_DrawStart0x80074bfc(pointerToEffectiveColor).
No full-message state save, allocation or local validation.

FontDrawStart(),0x80077b4c,size90, performs the same direct state resets with
G=255 and a separately generated local palette color, then calls that same
JUTFont service. The local entry bodies do not themselves call NoiseOff or
RainbowColorOff. Delegated JUT initialization is now covered in
[glyph/setup requests](FONT_GLYPH_REQUESTS_AND_SETUP.md): active texture-generator/
TEV-stage counts reset to1; saved prior state is not restored. Inner GX effects
remain open. Scale/color/edge stores
above are established independently of those service effects.

msgDisp's initial start and pose/icon restart calls therefore reset raw color,
edge and scale directly; later restoration of a saved raw color changes color
only. Previous draw scale is not restored by that saved word. This is shared
font state across record types, not an isolated glyph-state transaction.

## Edge and rainbow helpers (B)

FontDrawEdge0x80077ab4,sizec writes edge word1; FontDrawEdgeOff0x80077aa8,sizec
writes0. No immediate GX request, rendering, prior-value stack or alpha change.
Their consumption by glyph rendering remains a separate contract.

FontDrawRainbowColor0x80077a08,sizea0 makes these calls in order:

| GX helper | Exact integer arguments |
| --- | --- |
| GXSetNumTevStages | (2) |
| GXSetTevOrder | (1,255,255,4) |
| GXSetTevColorOp | (1,0,0,0,1,0) |
| GXSetTevAlphaOp | (1,0,0,0,1,0) |
| GXSetTevColorIn | (1,15,0,10,15) |
| GXSetTevAlphaIn | (1,7,0,5,7) |

FontDrawRainbowColorOff0x800779e4,size24 requests only GXSetNumTevStages(1).
No direct CPU rainbow boolean, saved stage configuration or color restoration.
Final rainbow/color interpolation is not proved solely by the helper names.

## Noise helper and original window texture request (B)

FontDrawNoise0x800778e0,size104 ignores incoming arguments. Ordered requests:

| GX helper | Exact arguments |
| --- | --- |
| GXSetNumTexGens | (2) |
| GXSetNumTevStages | (2) |
| GXSetTevKAlphaSel | (1,0) |
| GXSetTevOrder | (1,1,2,255) |
| GXSetTevColorOp | (1,0,0,0,1,0) |
| GXSetTevAlphaOp | (1,0,0,0,1,0) |
| GXSetTevColorIn | (1,15,15,15,0) |
| GXSetTevAlphaIn | (1,7,0,4,7) |
| GXLoadTexObj | (globalWindowObjects+0x640,2) |

The texture address is window palette object index50 at32-byte stride, not a
new procedural noise asset. There is no readiness/count/null guard here; setup
is covered by the window manager, descriptor/image binding still needs recovery.
Next request PSMTXScale(localMatrix,1/64,1/64,1), then
GXLoadTexMtxImm(matrix,33,1), then GXSetTexCoordGen2(1,1,5,33,0,125).
The1/64 and1 bits are original constants0x80420438/0x804203e4.
No RNG or per-call time input in this helper; final texture/GX meaning remains
service/data work.

FontDrawNoiseOff0x800778b4,size2c requests GXSetNumTexGens(1) followed by
GXSetNumTevStages(1). It does not restore all stage/texture-matrix bindings or
alter CPU raw color/alpha/scale/edge locally. RainbowOff and NoiseOff therefore
have different direct reset scopes; neither is a saved-state pop.

## Validation obligations and open rendering work (B/P)

Compare raw versus effective color, caller mutation/aliasing/repeated scaling,
index0 palette restoration, byte alpha narrowing, start's scale/edge reset,
saved-color-only restore after embedded objects, effect call order, different
off-helper scope and index50 readiness. The reciprocal interpretation was
independently checked over all65536 byte pairs, not by executing original code.

Remaining: inner glyph geometry/page/vertex consumption, font glyph/width/
texture tables, edge/color/alpha/scale raster effects, GX/matrix service bodies,
palette/current-state writers, descriptor50 binding, outer cadence and matched
original images/story callers. These14 helper bodies are locally established;
full font rendering/dialogue/complete-game equivalence remains unproven.
