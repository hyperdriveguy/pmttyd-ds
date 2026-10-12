# Font glyph matrix/edge requests and draw-start GX setup

US G8ME01 revision0, recovered2026-10-12. B evidence is static original DOL:
FontDrawCode0x80077594,size1a8; FontDrawCodeMtx0x80077438,size15c;
JUTFont_DrawStart0x80074bfc,size294. See [font state](FONT_DRAW_STATE.md),
[glyph effects](MESSAGE_GLYPH_EFFECTS.md) and [window drawing](MESSAGE_WINDOW_DRAWING.md).
This establishes all local request paths; inner _JUTFont_DrawPos, glyph/texture
tables and GX/matrix numerical/raster services remain separate dependencies.
No original execution or complete font rendering equivalence is claimed.

## Ordinary glyph's scale and camera matrix (B)

FontDrawCode receives glyph word in r3 and x/y in f1/f2. Save glyph and x/y;
load the three shared scale floats and request PSMTXScale(localMatrix,sx,sy,sz).
Then PSMTXTransApply(localMatrix,localMatrix,x,y,0). Obtain camGetCurPtr() and
request PSMTXConcat(camera+0x11c,localMatrix,localMatrix). Load that resulting
matrix with GXLoadPosMtxImm(matrix,0), then GXSetCurrentMtx(0).

No camera argument, horizontal/vertical/opacity clip, glyph-validity test or
per-call draw-start setup. Current-camera/scale state is inherited from callers.
Scale and translation apply through matrix requests, not by modifying glyph
layout records. Exact helper operation/alias/numerical behavior is still open;
do not freely reverse the argument order while implementing these requests.

## Caller-matrix variant and visible mutation (B)

FontDrawCodeMtx receives(matrixPointer,glyphWord). Obtain current camera and
request PSMTXConcat(camera+0x11c,matrixPointer,matrixPointer). Both second input
and output point to the caller's matrix; there is no copy to local storage or
restore request after drawing. Then GXLoadPosMtxImm(matrixPointer,0) and
GXSetCurrentMtx(0). It does not read the shared scale vector or x/y inputs.

The in-place output exposes matrix-service writes to the caller. A persistent
matrix passed repeatedly is not guaranteed to retain its initial contents;
msgDisp's dynamic path builds a fresh matrix before this call. Inner concat
alias handling/numerics and actual persistent-matrix callers still need recovery.
This variant is not an immutable-matrix wrapper around ordinary FontDrawCode.

## Shared edge branch, raw color and five requests (B)

After matrix loading, both functions read the shared edge word0x8041e9a4.
If zero, skip directly to _JUTFont_DrawPos(glyphWord,0,0). No color reissue on
that path: the center request inherits the current GX color established by
font/color/history services, including direct TEV overrides.

If any nonzero edge word:

1. Read current palette index0 and save raw global color0x8041e9ac to a local
   word. Store palette0 as raw global color; compute palette effective alpha
   using the current draw-alpha byte and request GXSetTevColor(1,effectiveColor).
2. Request _JUTFont_DrawPos0x80074720 four times in order with
   (glyph,-2,0),(glyph,2,0),(glyph,0,-2),(glyph,0,2).
3. Restore the saved raw word globally. Reread draw alpha, compose that raw
   color's effective alpha and request GXSetTevColor(1,effectiveColor).
4. Request the center _JUTFont_DrawPos(glyph,0,0).

Alpha uses the byte-product/255 sequence established in font state. These local
color copies do not mutate an external caller color buffer. The draw-alpha byte
is read again at restoration, not necessarily the same cached value if inner
services/callbacks change it. Edge flag is not cleared afterward. No prior GPU
color snapshot, matrix/current-slot restore, or color restoration to a direct
history TEV override; the center is reissued from saved raw CPU color.

Offsets are integer request coordinates, not measured screen-pixel outlines;
matrix scaling and inner glyph geometry determine final displacement. The full
glyph word is forwarded; narrowing/fallback behavior belongs to _JUTFont_DrawPos.
Return behavior of that service is not used for branch decisions.

## JUT draw-start common setup and order (B)

JUTFont_DrawStart(colorPointer) caches current camera, then makes the following
ordered requests. Arguments are exact original integer values; API names are
symbol annotations, not a claim of fully recovered register/hardware behavior.

| Helper | Arguments |
| --- | --- |
| __GXSetIndirectMask | (0) |
| GXSetCullMode | (0) |
| GXSetZCompLoc | (1) |
| GXSetAlphaCompare | (7,0,0,7,0) |
| GXSetBlendMode | (1,4,5,7) |
| GXSetZMode | (1,7,0) |
| GXClearVtxDesc | () |
| GXSetVtxDesc, four calls | (9,1),(11,1),(13,1),(14,1) |
| GXSetVtxAttrFmt, four calls | (0,9,1,3,0),(0,11,1,5,0),(0,13,1,2,0),(0,14,1,0,0) |
| GXSetChanCtrl | (4,0,1,1,0,0,2) |
| GXSetNumChans | (1) |
| GXSetNumTexGens | (1) |
| GXSetTevColor | (1,copy of *colorPointer) |
| GXSetNumTevStages | (1) |
| GXSetTevOrder | (0,0,0,255) |
| GXSetTevColorOp | (0,0,0,0,1,0) |
| GXSetTevAlphaOp | (0,0,0,0,1,0) |
| GXSetTevColorIn | (0,15,2,12,15) |
| GXSetTevAlphaIn | (0,7,1,4,7) |
| GXLoadTexObj | (address0x8041e97c,0) |

The supplied color pointer is read into a local copy; JUT setup does not locally
scale or write its alpha. That already occurred in FontDrawStart/_alpha. The
texture argument is a static GX object address, not a loaded pointer; its image/
format/setup and current font page ownership remain data/service dependencies.

## Texture-matrix mode, current camera and fog (B)

Next read global work word+0x16c. If zero, request PSMTXScale(localMatrix,
1/512,1/1024,1); otherwise use(1/512,1/256,1). Any nonzero word selects the
second mode. Its producer/meaning is not established here as language, font
family or framebuffer size. Original bits at0x804203ec/f0/f4/e4 are verified.

Then GXLoadTexMtxImm(matrix,30,1), GXSetTexCoordGen2(0,1,4,30,0,125),
GXLoadPosMtxImm(cachedCamera+0x11c,0), GXSetCurrentMtx(0). Finally GXSetFog with
type0, floats0/0/0/0 and a copied word from BSS0x80429578. No previous fog state
restore or supplied-color-pointer mutation. No local allocation/failure/texture-
readiness guard, glyph drawing or CPU scale/edge/color store in this routine.

This resolves the delegated start's active-stage reset scope: it requests one
texture generator and one TEV stage, removing active stage1 noise/rainbow paths
from that setup. Other stage1/texture-matrix parameters are not restored to
saved prior values. Actual GX service/hardware effects remain unverified; there
is no universal state pop. FontDrawStart's direct CPU reset is documented
separately and precedes these delegated requests.

Differential obligations: ordinary versus caller-owned matrix, concat order/
in-place output, inherited no-edge color, four edge-offset order plus center,
raw-color versus GPU override restoration, repeated alpha reads, complete setup
call order, vertex attributes, static texture object, word+16c branch and fog
color source. Remaining: _JUTFont_DrawPos geometry/page lookup/vertices, font
loading/width/glyph data, GX/matrix bodies, state producers/cadence and matched
original images/story callers. These three local bodies are established; full
font rendering/dialogue/complete-game equivalence remains unproven.
