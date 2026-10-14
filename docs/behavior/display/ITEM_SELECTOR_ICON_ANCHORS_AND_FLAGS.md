# Original item selector icon anchors and flags

US G8ME01 revision0; read-only DOL recovery2026-10-13. Source functions:
winIconSet8017b8d8/2f4, iconDispGx8001a918/24,
iconDispGxAlpha8001a93c/2b4 and iconGX800197f8/9a4. Supplemental to
[selector requests](../message/ITEM_TABLE_SELECTION_DRAW_REQUESTS.md).
Original quad/matrix service requests are established; complete material/GX/PICA
raster and numeric parity remain separate. No source code or asset conversions
changed during this recovery.

## Two distinct icon services

Rows and conditional stripe447 call **winIconSet**, not iconDispGx. Their anchor
is centered horizontally **and vertically**. Cursor504 and arrows445/446 call
**iconDispGx**, whose ordinary original quad is centered X and anchored bottom Y.
A universal message-icon bottom anchor is incorrect for the selector row service.
Do not globally change other battle/message icons when binding this window path.

Also, iconDispGx's second argument16/20 is a packed flags word, **not a camera
index**. The source copies low16(argument) into temporary icon record+0.
General iconDispGxAlpha gets the current camera separately; winIconSet explicitly
uses camera8. Earlier selector tables labelling16/20 as camera are superseded
by this direct service recovery.

## Window service: centered normalized quad

winIconSet receives(iconID,positionPointer,scaleVectorPointer,colorPointer).
It remaps284 to283 locally, narrows ID to low16 and calls iconGetTexObj.
This supplies original icon descriptor's selected TPL texture; iconID is not
an image index. Read GXGetTexObjHeight and Width as U16, convert to float.
Form scaleX=float(width)*suppliedScaleX,
scaleY=float(height)*suppliedScaleY,scaleZ=suppliedScaleZ.

Matrix requests are:

1. PSMTXTrans(T,positionX,positionY,positionZ).
2. PSMTXScale(S,scaleX,scaleY,scaleZ).
3. PSMTXConcat(T,S,localMatrix).
4. camGetPtr(8), then PSMTXConcat(camera8+11c,localMatrix,localMatrix).
5. GXLoadPosMtxImm(localMatrix,0), GXSetCurrentMtx(0).

No camera-facing Y rotation or flags-based mirror. GXBegin(80hex,0,4) emits:

| Order | Position XYZ | UV |
| --- | --- | --- |
| 1 | (-.5,-.5,0) | (0,1) |
| 2 | (-.5,+.5,0) | (0,0) |
| 3 | (+.5,+.5,0) | (1,0) |
| 4 | (+.5,-.5,0) | (1,1) |

Source constants are exactly -.5,+.5,0,1. Color is copied into TEV color1.
For alpha255 the local requests disable blending/depth writes and use alpha
compare greater-or-equal128; non255 uses source-alpha blending and always alpha
compare. Exact GPU state translation remains independent of geometric anchors.
Texture LOD request uses min/mag linear1/1, all LOD/bias floats0, remaining
options0. No resizing or original texture crop occurs in this leaf.

Consequently row40x40 texture with scale(.5,.5,.5) occupies source20x20 centered
at (listX+35,listY-44-24*i+D): top=anchorY+10,bottom=anchorY-10.
The row source scissor beginning at listY-34 therefore admits the whole first
row. Its text anchor remains the independent original listY-32; do not lower
or raise the row to compensate for the wrong general-icon anchor.
Conditional stripe24x24 at scale.15 similarly occupies3.6x3.6 centered in Y.
The actual quest second argument0 suppresses the stripe branch entirely.

## General service and flags

iconDispGx is a thin wrapper setting alpha255 then calling iconDispGxAlpha.
Alpha admission requires both icon.bin/icon.tpl retained buffers ready.
It gets current camera, forms T(position), uniform S(requestedScale), and
Ry(-camera.word114 angle times original degree-to-radian constant), then
requests concatenations T*Ry*S and currentCamera+11c times that matrix.
The original helper loads descriptor width/height/frame data into a temporary
icon record and selects animated frame using GP.word1c modulo total delays.
It then calls iconGX(matrix,&temporaryRecord).

iconGX applies flags through an additional scale before loading its matrix:

- flags4: X scale-1; otherwise+1.
- flags8: Y scale-1; otherwise+1.
- flags16: if current camera differs from camGetPtr(8), multiply one additional
  Y scale-1. This camera-dependent adjustment does not occur in winIconSet.

Flags16 also skips the ordinary iconGX geometry reduction factor. For the used
cursor/arrows flags16/20, descriptor dimensions I,H yield these local quads:
left=-I*.5,right=left+I,top=H,bottom=H+(-H)=0,Z=0.
Vertex order is top-left,top-right,bottom-right,bottom-left, with
UV(0,0),(1,0),(1,1),(0,1). Uniform suppliedScale applies through the preceding
matrix. Source float addition for right/bottom remains an explicit operation;
this is not an independently centered-height quad.

The absent-flags16 path applies .6 to these dimensions/positions, with
additional icon403/405 exceptions; that is outside the selector's used flags.
See [EXP draw anchor](EXP_ICON_CLOCK_AND_TYPE0_WINDOW_FRAME.md#2026-10-13-used404-draw-anchor-follow-up)
for the separately bound battle icon404 path. Do not transplant its reduction
into flags16 selector calls.

Cursor request20decimal is flags14hex: **X mirrored**, flags16 full geometry,
no initial flags8 Y mirror. Arrow requests16decimal are flags10hex: full
geometry, no initial X/Y mirror. Their source texture sizes40x40 and48x48,
respectively, combine with requested scales1/.6. They remain bottom anchored;
only the row/stripe window service is centered vertically.

## Evidence and limits

Fresh complete named spans were extracted from the read-only US ISO into
/tmp/selector-icon-{window,general,wrapper,inner-full}.elf and disassembled
there. Constant words were mapped through original DOL sections; calls were
matched against the original public symbol CSV. No user emulator profile,
save, source ISO or shared implementation modified. Relative documentation
links were checked. Original texture selection/animation/GX material and native
PICA filtering remain separate owners; these findings establish source anchoring,
flags and matrix/vertex requests, not matched original pixels.
