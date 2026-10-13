# Original item-selector window frame and decoration

US G8ME01 revision0; read-only static recovery2026-10-13 for
[OW-INTERACT-01](../../requests/OW-INTERACT-01.md). Scoped to the negative
explicit item table's three source windows, definition decoration0/1 and
windowDispGX2_Waku_col. See [selector contract](../message/ITEM_TABLE_SELECTION_WINDOW.md)
and [related untransformed type0 frame](EXP_ICON_CLOCK_AND_TYPE0_WINDOW_FRAME.md).
Original execution, raster equivalence and New2DS performance remain unmeasured.

## Manager draw order, colors and transforms

winMgrDisp8023f288/484 receives the retained manager record in its second
argument. Read rotation binary32 at+14, multiply by source degree-to-radian
constant0.01745329238474369 (80428060), call PSMTXRotRad with axis'z'. Read
scale binary32 at+10 and construct uniform XYZ scale. PSMTXConcat at8023f2d4
forms Rz(rotation)*S(scale). No translation is included in that manager matrix.

The base call at8023f38c passes that matrix, type0, RGBA, and source rectangle
X/Y/width/height from signed manager WORDs+18/+1c/+20/+24 converted to binary32,
with border20 (80428064). RGB bytes are read from definition+1c/+1d/+1e;
alpha is the low8 of manager WORD+c. Definition alpha+1f is not copied here.
Template0's three definitions supply whiteRGB. The frame draws before the
additional decoration and before the definition's contents callback.

Definition decoration WORD+4 dispatches after base draw. Decoration0 and2
add no extra frame. Used decoration1 takes the branch8023f3c8..450 and issues
one further type0 frame with the same Rz*S and same low8 manager alpha:

| Extra tab property | Exact source expression/value |
| --- | --- |
| RGB | 2b,a4,ff (WORD2ba4ff00 at80427fa0 with alpha replaced) |
| X | signed managerX + truncTowardZero((managerWidth-150)/2) |
| Y | signed managerY+14 |
| Width / height | 150 /28 |
| Border | 8 |

Integer subtract/divide/add happens before binary32 conversion. Other
manager decorations3..5 have different geometry and are outside this used path.
The contents callback is loaded from definition+24 at8023f6e0 and receives
manager pointer after both frames; it does not inherit a newly invented local
origin from this manager routine.

windowDispGX2_Waku_col8007ad68/6e4 obtains camGetCurPtr before checking
completion global r13+1afc. A zero completion word exits without GX drawing.
The used type is0. Its local normalization changes a type with nonzero low16
to0; callers here pass literal0, so no broader type-domain inference is needed.

The frame's **own rectangle center** is its transform origin. Compute
negativeHalfWidth=(-width)*0.5, halfHeight=height*0.5, then create translation
T(x-negativeHalfWidth,y-halfHeight,0). PSMTXConcat8007afc4 combines this with
the supplied matrix, producing T(center)*Rz*S. At8007afd4 camera+11c is
concatenated on the left. Load that result as position matrix0; set current
matrix0. Thus the base and extra tab each rotate/scale around their separate
rectangle centers. Scaling absolute screenX/Y about zero or treating the tab
as part of one parent-centered rectangle gives different transition geometry.

## Centered nine-slice FIFO

The used type0 loads texture-object global r13+1b04, index0*32, TEXMAP0.
GXBegin8007b00c uses QUADS80hex, vertex format0 and36 vertices. Grid:

- X columns: negativeHalfWidth, negativeHalfWidth+border,
  (negativeHalfWidth+width)-border, negativeHalfWidth+width.
- Y rows: halfHeight, halfHeight-border,
  border+(halfHeight-height), halfHeight-height.
- U and V: 0,0.3333333432674408,0.6666666865348816,1.

Nine quads are emitted row-major, each top-left,top-right,bottom-right,
bottom-left, Z0, with matching UV grid. The original center is drawn as the
ninth-cell grid's middle cell, not omitted. Source FIFO8007b018..3f0 has
exactly180 float writes,36*(XYZ+UV). Source constants: zero80420538,
half8042053c, one-third80420540,two-thirds80420544,one80420548.
Each indicated subtraction/addition/multiplication is binary32. Retaining
source arithmetic order matters independently of equivalent real-number math.

The centered grid plus T(center) reproduces ordinary source rectangle
coordinates when rotation0/scale1. For example base list(-24,116,300,72)
places center(126,80); its decoration1 is(51,130,150,28), center(126,116).
These are source coordinate deductions for one row's source list-height rule,
not a 3DS layout or a measured pixel comparison.

## Material and texture source

The complete local GX program establishes:

| Operation | Used parameters/effect |
| --- | --- |
| Cull | NONE0 |
| Z compare location | before texture,1 |
| Alpha compare | ALWAYS7,ref0,AND0,ALWAYS7,ref0 |
| Blend | BLEND1,SRCALPHA4,INVSRCALPHA5,logic7 |
| Z mode | enable1,ALWAYS7,update0 |
| Fog | NONE0,all supplied float distances0 |
| Attributes | direct position9 and TEX0d; XYZ/ST float4,format0 |
| Texture generation | one; coordinate0,MTX2x4 type1,TEX0 source4,identity3c,normalize0,postidentity7d |
| Channels / stages | zero channels,one texgen,one TEV stage |
| TEV order | stage0,TEXCOORD0,TEXMAP0,COLORNULLff |
| TEV color inputs | ZERO15,C0=2,TEXC8,ZERO15 |
| TEV alpha inputs | ZERO7,A0=1,TEXA4,ZERO7 |
| TEV operations | add0,bias0,scale0,clamp1,outPREV0 |
| TEV swap | stage0,ras0,tex0 |
| TEV color register | REG0 enum1 receives supplied RGBA |

Consequently texturedRGB multiplies suppliedRGB; textureAlpha multiplies
suppliedAlpha; source-alpha blending composes over previous drawing. No generated
solid rounded rectangle, baked blue replacement pixels or texture-as-opaque
quad follows this program. GX/SDK enum names annotate literal recovered arguments;
backend translation must preserve their effective state and vertex ordering.

[Window palette lifecycle](../message/MESSAGE_WINDOW_MANAGER.md) owns loading
font/msgWindow.tpl from archive or `%s/f/msgWindow.tpl` from DVD, texture-object
array creation and the separate completion-word policy. The actual ISO resource
is f/msgWindow.tpl, size321984, FST offset947396944. Descriptor0 is72x72,
GX14(CMPR), and supplies the original nine-slice texture. Manager decoration1
uses this **same descriptor0** with a different supplied color and geometry;
there is no second tab-texture binding in the recovered used branch. Preserve
source texture pixels/filter metadata through original TPL conversion. Camera,
projection/viewport, exact filtering and hardware raster rounding remain providers.

## Evidence and remaining bindings

Fresh complete ISO spans: winMgrDisp8023f288/484 SHA256
536032aa8c248aa975678bb8d8af1c962188d7e6696733f80fe9bc6a60b54e7c;
windowDispGX2_Waku_col8007ad68/6e4 SHA256
120268e5add1567256774f46d474fe10364ba7c047890901fa7398417d08c5b5.
Fresh symbolic/numeric FIFO check extracted all180 writes and independently
compared all36 XYZ/UV tuples with the declared nine-cell grid at width300,
height72,border20. This is an instruction/data deduction, not source execution.
Inspection-only ELF/listings, checker and metadata remain
/tmp/item-frame-manager.*, /tmp/item-frame-draw.*,
/tmp/item-frame-fifo.py and /tmp/item-frame-fifo-evidence.json.
ISO opened read-only; no original binary/image/prose or code committed.

Native binding still needs transformed citro3d vertex batching, original palette
readiness/lifetime, correct camera8/reflow, list/header/help content draws and
scissor restoration, original font layout, manager sequence-to-display timing,
texture filtering equivalence, exact SDK PSMTX/sine/cosine/intermediate
rounding providers, GX backend state parity, matched Dolphin captures and
New2DS profiling.
This recovery pins the frame/tab geometry and material only; it does not close
OW-INTERACT-01 or establish complete selector pixels. No shared index/PROGRESS/
ASSETS, build, emulator/profile/save or native source changed by this worker.
