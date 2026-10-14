# Original kanban/type4 window and Trouble Center lock notice

US G8ME01 disc0 revision0; read-only recovery2026-10-14 for
[ITEM-01](../../requests/ITEM-01.md) and
[OW-INTERACT-01](../../requests/OW-INTERACT-01.md). Complete source helper
windowDispGX_Kanban8007e9f0/ffchex, used type4 branch and its original TPL
binding. Supplements [message parser](../message/MESSAGE_PARSER_RECORDS.md),
[window drawing](../message/MESSAGE_WINDOW_DRAWING.md),
[window states](../message/MESSAGE_WINDOW_STATES.md) and
[door notice caller](../field/TROUBLE_CENTER_DOOR_CASE9_GEOMETRY_AND_LIFETIME.md).
Static original requests/asset headers, not executed GX/PICA raster parity.

## Exact notice resource and parser/placement

Original gor_02_035 is93 bytes excluding NUL in msg/US/gor_02.txt,
keyoffset90266,textoffset90277,SHA256
709f213fa2a18dcb315bb8eeac23fe0b1d4dbae8bd11d2eec5f50efc12bae2c7.
It begins `<kanban>` followed by LF and contains original `<col c00000ff>`,
`</col>`,`<k>` controls. Preserve bytes/spacing/LF/control order; no ordinary
bubble fallback or replacement development text is source-equivalent.

Parser kanban writes W.type4 and sets localV=0,D.f23c=0. It does not itself
change window geometry, original message identity or the source dialogue text.
The door event requests toge(1,0,0,0),then print(0,gor_02_035,0,0).
Selector1 selects placementmode0; the case-created event has actual typeef,
so numeric speaker0 does not derive a named NPC owner. Type4 draw routing also
ignores the placement flags locally. No invented doorkeeper speech/tail belongs
in this sign notice. Source color tokenc00000ff remains the parser/record
consumer's request, separate from the sign frame's white texture tint.

## Type4 dispatch and frame transform

Complete helper's jump table8030f480 has11 pointers. Both type2 and type4
select8007ec70. Type7 selects8007f838,10 selects8007f254; other entries skip
its drawing branch after shared state setup. This contract establishes used4,
not all sign/diary/broadcast types by visual resemblance.

Input r3=type,r4=alpha,f1/f2=x/y,f3/f4=width/height. Cache current camera.
Build T(x,y,0),then S(width/560,height/176,1),concat T*S,then
camera.view(+11c)*(T*S),load position matrix0 and select current matrix0.
The four source quads below are canonical560x176 local geometry transformed
by that same matrix. Do not make cap widths constant when width changes;
this helper uniformly scales the whole sign, unlike a general nine-slice.

| Draw order | TPL index | Local top-left | Geometry width,height | Actual texture width,height |
| --- | --- | --- | --- | --- |
| 1,left | 29 | 0,0 | 72,176 | 72,176 |
| 2,upper middle | 31 | 72,0 | 416,136 | 416,40 |
| 3,right | 30 | 488,0 | 72,176 | 72,176 |
| 4,lower middle | 32 | 72,-136 | 416,40 | 416,40 |

For each draw, load TEXMAP0 from original window texture-object array+index*32.
Read actual U16 texture height then width. Build texture matrix
S(abs(geometryWidth)/textureWidth,abs(geometryHeight)/textureHeight,1),
load at texture matrix30 with2x4 mode, and select it for TEX0 through
GXSetTexCoordGen2. Emit GXBegin(80hex,vertexformat0,4). Quad order is
upper-left,upper-right,lower-right,lower-left with Z0 and untransformed
UV(0,0),(1,0),(1,1),(0,1). Source geometry is Y-up/downward extent.
Full branch8007ec70..8007f250 emits4quads/16vertices/80float FIFO values.

**Upper middle is not a stretched40-pixel image.** Its texture matrixY scale
is136/40=3.4. Original wrapT=0(CLAMP) causes the top40 source units to consume
image31, then remaining96 units sample its bottom edge. A lossless local frame
composition should preserve that first40-pixel band and extend its bottom
edge; resizing the entire416x40 texture into416x136 changes the original art.
Other three used texture scales are1. No shadow prepass, message-bubble cap
indices9/11/13/15 or speech-tail texture is requested in this sign helper.

## Original texture setup and material

Original f/msgWindow.tpl ISOoffset947396944,size321984,SHA256
2ef0cdc818d2402de5bfb95394f67d78017e74bc1b41c38b79780cb0b870dc67.
It contains51 descriptors. Used header/data offsets:

| Index | Header offset | CMPR data offset |
| --- | --- | --- |
| 29 | 5b8 | 37200 |
| 31 | 600 | 3a380 |
| 30 | 5dc | 38ac0 |
| 32 | 624 | 3c400 |

All four use original format14/CMPR,wrapS/T0,min/magfilter1,zeroLOD bias,
minLOD/maxLOD0. Complete TEXGetGXTexObjFromPalette80037a80/dc forwards
header wrapS/T directly into GXInitTexObj and filter/LOD fields into
GXInitTexObjLOD. windowTexSetup creates32-byte texture objects for every
original descriptor through that helper; Kanban does not override wrap modes.
Thus upper-middle clamp behavior follows actual source setup, not merely an
inference from TPL headers or a chosen native sampler.

Shared Kanban material requests before dispatch:
no culling; one TEV stage; alpha compare ALWAYS/AND/ALWAYS; blend ADD with
SRCALPHA/INVSRCALPHA; depth compare enabledALWAYS,depth updatefalse.
Fogtype0 with zero parameters/white copied fog color. Direct vertex attributes
POSITIONXYZ/F32 and TEX0ST/F32; TEXMAP0/TEXCOORD0,null color channel.
Set color register1 from sourceffffff00 with alpha replaced by low8(inputr4).
TEV color input(0,ZERO,C1,TEXC,ZERO) and alpha(0,ZERO,A1,TEXA,ZERO),
ADD/clamp operations: RGB=TEXC,alpha=callerAlpha*TEXA in the ordinary material
interpretation. No arbitrary panel tint/shadow or font color is baked into art.
Shader/numeric/sample-edge/CMPR decompression and actual blending parity remain
native backend obligations.

## Outer frame/text geometry and opacity

msgWindow_Disp dispatches type4 with
windowDispGX_Kanban(type4,low8(W.s16alpha),W.x,W.y,W.width,W.height),
then GXSetScissor(currentCamera.u16f4,U(240-(y-36)-2),
currentCamera.u16f8,104),msgDisp(handle,alpha,x+62,y-36).
U is the recovered unsigned float conversion, not arbitrary rounded native
pixels. Text is requested at **every** opacity, with its return retained for
common indicator processing. There is no ordinary anchored-bubble alpha255
text gate, anchor growth or tail route on type4.

Default geometry from the original constructor is(-280,225,560,176).
With those finite values, textorigin=(-218,189),clipY49,height104.
Explicit source wpos could change the geometry; this notice contains no such
control. Restore the cached current camera scissor in the common tail, then
run the original key indicator branch when msgDisp's return is nonzero.
Source letterbox/projection/pixel placement still belongs to the camera/backend.

General window state machine uses type4 opening/closing sound rows32/33,
requests psndSFXOn(row OR20000hex) unless windowflags8 suppresses sound.
Opening/closing time-based alpha and deferred deletion remain the linked state
contract. Parser localV=0 is not a license to skip the source window lifecycle
or replace its keywait with an unrelated UI acknowledgment.

## Evidence and implementation boundary

Complete original helper span SHA256
befbf6b1ea453907df42fad9b4c8bae5ce6e51c6ea039feca5eb43e69d5b55b3.
Fresh source inspection/report and annotation script stay under ignored project
artifacts: trouble-kanban.elf/.txt,trouble-kanban-annotated.txt,
trouble-kanban-annotate.py,trouble-windowTexSetup.elf/.txt,
trouble-TPLBind.elf/.txt,trouble-msgWindow.tpl. No original binary/art/message
payload, generated art, runtime code or shared index/progress/inbox is committed
by this delivery. Resources remain Nintendo/Intelligent Systems; local inspection
or conversion does not confer redistribution rights.

Remaining: native parser acceptance and type4 original art/material renderer,
exact frame/text clip/state/sounds/key-indicator lifecycle; original screenshot/
execution comparison and New2DSXL performance/hardware validation. Type4 must
remain unavailable if these required providers are missing; an ordinary bubble
is not correctness evidence for the original sign notice.
