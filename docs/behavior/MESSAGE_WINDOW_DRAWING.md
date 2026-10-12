# General message-window drawing, anchors, scissor and indicator

US G8ME01 revision0, recovered2026-10-12. B evidence is static original DOL:
msgWindow_Disp0x80080260,size6dc; camGetCurPtr0x8000f2e4,size18;
__cvt_fp2unsigned0x80264860,size5c; intplGetValue's mode0 region and original
dispatch/constants. See [window states](MESSAGE_WINDOW_STATES.md),
[lifecycle](MESSAGE_WINDOW_LIFECYCLE.md), [selection drawing](MESSAGE_SELECTION_DRAWING.md)
and [display dispatch](DISPLAY_QUEUE_AND_DISPATCH.md). Inner border/text/pose/
projection services and executed images remain separate obligations.

## Callback setup, current camera and type dispatch (B)

W arrives in r4; incoming r3 is not used as the camera selector. camGetCurPtr
loads index at0x8041e608, indexes four-byte pointer table0x803d95f8 and returns
that pointer, without bounds/null checking. This current-camera pointer C is
cached for scissor coordinates/restoration. dispDraw's camera argument alone
does not establish the current-camera global's value.

Set indicator candidate R=0. Request GXSetFog(type0,floats0/0/0/0,colorPointer)
using a copied word from BSS0x80429580. Runtime color writers are untraced; this
is distinct from selection drawing's BSS color at0x80429584. No fog restoration.
Read unsigned W.word[8] and dispatch0..11 through table0x8030f5a0. Negative
word values and values>11 skip directly to common scissor restoration.

| Types | Local route |
| --- | --- |
| 0,1,6,8,9 | Message border, with optional anchor route |
| 2,4,10 | windowDispGX_Kanban and text, narrow104-high scissor |
| 7 | windowDispGX_Kanban and text,126-high scissor |
| 3 | windowDispGX_System and text, height derived from window height |
| 11 | Text only, same height derivation as3 |
| 5 / outside0..11 | Common restoration only, no local border/text request |

No local state/allocation/handle-null or opacity-zero guard. Main/display queue
admission and borrowed W lifetime remain external. Let x/y/width/height be W
floats+c/+10/+14/+18, a=W.s16[6], alpha=low8(a), F=W.s16[2c]. Geometry may
change during this draw as below. Names/roles do not establish final GPU pixels.

## Interpolation and unsigned clipping conversion (B)

Anchor paths call intplGetValue(0,a,255,start,end). Mode0 uses signed integer
conversion to binary32, then separately rounded fsubs/fmuls/fdivs/fadds:
I(start,end)=start + ((float(a)*(end-start))/255), with those original operation
boundaries. No lower/upper clamp of a here; negative/>255 signed opacity can
extrapolate. The common total0 return-end branch exists but these calls use255.
Mode0 target0x80006d24 is verified independently; other interpolation modes
remain separate contracts.

Define U(v) as the actual __cvt_fp2unsigned used for most scissor words:
return0 for v<0; return0xffffffff when v is not less than2^32, including NaN;
otherwise truncate toward zero using fctiwz. If v>=2^31, first subtract2^31,
convert the reduced value and add0x80000000 with word arithmetic. Constants
0/2^32/2^31 are at0x80303a20/+8/+10. Signed zeros convert to0. No errno or
pointer access beyond local stack. This is a saturating word conversion, not
a general host out-of-range unsigned cast.

The anchored type1 scissor instead uses direct fctiwz of y, loads its signed
word, computes250-truncatedY with32-bit word arithmetic and clamps the signed
result to[0,480]. Its
exception/out-of-range fctiwz behavior still depends on original floating
environment; do not substitute U(y) on this branch.

## Shared message route: anchor selection (B)

For types0/1/6/8/9, if F==0 take the direct route below. If nonzero, obtain a
three-float anchor A. F&0x10 copies W+30/+34/+38 directly; otherwise call
getScreenPoint(&W.positionAt30,&localAnchor)0x8000e7e8. Projection helper body,
camera choice and clipping are not inferred merely from its name.

For types0/6/8/9, compute target centerX=fused(width*0.5+x) and
centerY=fused(y-height*0.5), preserving fmadds/fnmsubs. Then:

- cx=I(A.x,targetCenterX), cy=I(A.y,targetCenterY).
- w=I(10,width), h=I(10,height).
- Request windowDispGX_Message(type,F,alpha,f1=fused(cx-w*0.5),
  f2=fused(cy+h*0.5),f3=w,f4=h,f5=A.x,f6=A.y).

Only when signed a==255, request scissor(C.u16[f4],U(240-(y-36)),
C.u16[f8],100), then msgDisp(W.handle,alpha,x+62,y-36) and retain its return
as R. Other opacities draw the border but skip this text call. Arithmetic
nonfused additions/subtractions here retain binary32 instruction rounding.

## Anchored type1: draw-time position mutation (B)

Write W.x=fused(A.x-width*0.5). If F&8, write W.y=A.y-80; otherwise W.y=A.y+60.
These stored changes precede the four interpolation calls; they are not just
temporary draw coordinates. Compute cx/cy/w/h as above using this new geometry,
then request the same message-border arguments with type1 and F.

Only at signed a==255, request scissor(C.u16[f4],clamp(250-truncSigned(y),0,480),
C.u16[f8],126). Request msgDisp(W.handle,alpha,x+28,y-6), but do not retain its
return. R stays0 on this route, so that call cannot trigger the common indicator
tail locally. Rendering at another opacity still rewrites W.x/y before skipping
text. Repeated draws can therefore expose stored geometry changes to later work.

## Direct message route and remaining type routes (B)

For F==0 shared message types, request windowDispGX_Message(type,0,alpha,
x,y,width,height,0,0). There is no a==255 gate. Scissor y is
U(240-(y-36)-8), height126; x/width always use C.u16[f4]/[f8]. Request
msgDisp(W.handle,alpha,x+62,y-36) and retain R, including for direct type1.
Anchored and direct type1 therefore have different text origin/indicator policy.

Other types ignore F for this local routing:

| Types | Border request | Scissor y | Scissor height | Text origin |
| --- | --- | --- | --- | --- |
| 2,4,10 | windowDispGX_Kanban(type,alpha,x,y,width,height) | U(240-(y-36)-2) | 104 | x+62,y-36 |
| 7 | Same Kanban call | U(240-(y-36)-8) | 126 | x+62,y-36 |
| 3 | windowDispGX_System(type,alpha,x,y,width,height) | U(240-(y-36)-8) | U(height-50) | x+62,y-36 |
| 11 | No local border call | U(240-(y-36)-8) | U(height-50) | x+62,y-36 |

All four rows call text at every opacity and retain R. Types3/11 convert height
before converting scissor y, temporarily retaining that height word; it is
overwritten by the text return before the indicator gate. No local scissor
intersect with prior rect, window width, or visible option bounds. Numerical
request values are established; GXSetScissor's implementation is still separate.

## Common restoration and animated indicator (B)

Every route, including skipped/unknown types and non-full-opacity anchor paths,
requests GXSetScissor(C.u16[f4],C.u16[f6],C.u16[f8],C.u16[fa]) at0x80080888.
This restores the cached current camera's stored rectangle, not an independently
saved previous GX rectangle. Border/text helpers may change other GX state.

If R==0, return. If nonzero, read active halfword at0x8041ea30; if zero set it1.
No local active reset or pulse counter. Load msgw pointer from0x804160b0
(original initializer0x8041ea3c); read its word+20 as pose ID. Call
animPoseMain(poseID), then reload pointer/pose and call animPoseDraw with integer
argument2 and floats(x+width-52,y-height+31,0,180,5), using current W geometry.
No separate opacity/flag/state gate or frame/tick delta in this tail. The actual
meaning of msgDisp's return is now covered in [record admission](MESSAGE_DRAW_RECORD_ADMISSION.md):
matching key ordinals set1; an encountered terminal returns0. Active reset and
animation service cadence remain open. Initialization binds this slot to hoshi_2/HOS_C_1 and
msgDispKeyWait_render, as established in [resource initialization](MESSAGE_RESOURCES_AND_LOOKUP.md);
the render callback is covered there, while pose service bodies remain open.
Do not label R as a proven key-wait enum
just from the visual role or routine names.

## Evidence limits and differential obligations (B/P)

Verified literals are0.5,10,240,36,62,80,60,28,6,8,50,52,31,180,5, plus previously
verified0 and2; type map and unsigned-conversion constants are checked directly
in the ISO. These are original request constants, not provisional native tuning.
No dialogue/message ID/order is changed or inferred by rendering.

Compare type routing, anchor copy versus projection, signed opacity interpolation
without clamp, fused center/edge arithmetic, exactly255 text gates, type1 stored
position mutation and discarded text return, direct text at low opacity, signed
versus unsigned clipping conversion/order, camera rectangle restoration, candidate
R retention, active halfword and per-draw pose update. Normal msgDisp effects
are now covered in [glyph effects](MESSAGE_GLYPH_EFFECTS.md).
Remaining: border/GX/projection/animation services, current-camera and
active producers/pose lifetime, original draw/caller cadence and matched images/story
conditions. Local callback/conversion paths are established; complete dialogue/
rendering/game equivalence remains unproven.
