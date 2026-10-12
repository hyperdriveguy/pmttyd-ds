# Message draw prefix, controls, history admission and embedded objects

US G8ME01 revision0, recovered2026-10-12. B evidence is static original DOL:
msgDisp0x800833e0,sizeb54; msgDispKeyWait_render0x80083360,size80;
animGetPtr0x80042f30,size8. This document recovers msgDisp's iteration/return,
control dispatch, history visibility and pose/icon paths, plus the indicator
render callback. Normal glyph effect branches after admission remain a separate
recovery step, now covered in [glyph effects](MESSAGE_GLYPH_EFFECTS.md). Together
these establish local msgDisp branches, not complete font/raster behavior. See
[parser](MESSAGE_PARSER_RECORDS.md), [objects](MESSAGE_EMBEDDED_OBJECTS.md),
[playback](MESSAGE_PLAYBACK.md) and [window drawing](MESSAGE_WINDOW_DRAWING.md).

## Exposed prefix and result ownership (B)

Inputs are(smartHandle,alphaWord) and origin x/y in f1/f2. Cache D=*handle with
no null guard. Call FontDrawStart_alpha(alphaWord) before any count test. Set
record pointer D+0x3c, index0 and return candidate K=0. Iterate while signed
index<D.word[0x10], reloading that exposed playback cursor on every loop test;
advance record pointer by24. D.parsedCount[0x38] and record schedules+0xc are
not consulted for this loop's admission. Negative/zero exposed count draws no
records but still performs the font-start call. Count/buffer bounds are unchecked.

Most records continue through the common increment. Normal loop completion
returns K, exactly0 or1. Key control below can set it1; a terminal control
returns0 immediately even if earlier exposed keys set it1. This is a record-
ordinal test, not a fresh button read, wait-mode enum or general success result.
Window drawing may retain or discard it as already documented.

## Reserved control dispatch before history filtering (B)

Unsigned halfword code R+4 selects the12-entry0xfff4..0xffff table at0x8030f600:

| Code | Target | Local action |
| --- | --- | --- |
| fff4 | 800834c8 | Embedded pose path below |
| fff5 | 80083ee0 | Continue, no local draw effect |
| fff6 | 800834ac | If payload R.word[0]!=0 call FontDrawNoise; else FontDrawNoiseOff |
| fff7 | 80083498 | Copy payload color word; call FontDrawColor(pointer) |
| fff8 | 800835c4 | History admission, then icon path |
| fff9..fffd | 80083ee0 | Continue, no local draw effect |
| fffe | 80083480 | If D.u16[8]==R.s16[6], set K=1; continue |
| ffff | 80083478 | Return0 immediately |

The key comparison preserves unsigned D ordinal versus sign-extended record
ordinal. Equal low16 bit patterns above0x7fff do not automatically compare equal.
Controls except icon execute before scroll/history/ordinary glyph clipping, so
offscreen color/noise records can affect later font state. No schedule or input
poll occurs in this control dispatch. Other codes take drawable admission.

## Drawable coordinates and history state (B)

For icon/ordinary glyph paths, establish base coordinates as binary32 additions:
drawX=x+float(R.s16[6]); drawY=y+float(D.s32[0x28])+float(R.s16[8]). Integer
conversion uses the verified signed-word double bias, then fsubs. Initialize
historyEmphasis=0 and fadeByte=255 for each drawable record.

Only when D.flags&0x40, apply the following history rules. Let S=D.s32[28],
Y=R.s16[8], i=D.s32[f03c], n=D.s32[f040], B[j]=D.s32[f044+4*j]. Indices,
counts and word sums/differences are unchecked; signed tests preserve original
wrapping arithmetic. No lower clamping of a corrupted history cursor.

| Scroll relation | First condition | Otherwise |
| --- | --- | --- |
| S==B[i] | Y+B[i+1]>0: emphasis1; else skip | No fade branch |
| S<B[i] | Y+B[i]>0: emphasis1 | Compute d=min(B[i]-S,100), fade=low8(trunc((100-d)*255/100)) |
| S>B[i] | Y+B[i+1]>0: emphasis1 | Compute d=min(S-B[i],100), fade=low8(trunc(d*255/100)) |

For the S<B[i] fade branch: if i!=n-1, require Y+B[i+1]>0, then set emphasis1;
otherwise skip. At i==n-1, preserve emphasis0 and request a faded current color
through GXSetTevColor(1,&color). Its alpha is trunc(oldColorAlpha*fadeByte/255).
For S>B[i]: if i!=n-2, require Y+B[i+2]>0, then set emphasis1; otherwise skip.
At i==n-2, preserve emphasis0 and make the same TEV faded-color request.

The divisions use signed reciprocal-multiply/shift/correction sequences, not
floating fade interpolation. Valid positive differences capped at100 give the
stated truncation. There is no explicit lower bound for wrapped differences.
Strict >0 admits emphasis; equality0 takes the skip/fade route. These rules act
before icon-specific and ordinary glyph clipping; pose/control paths bypass them.
Only two fade-boundary cases set the TEV color here; emphasis uses later glyph
color setup, not a universal whole-message alpha multiplier.

## Embedded pose route (B)

fff4 saves FontGetDrawColor's returned word. Build local identity matrix.
If ordered R.float[14] lies in inclusive[90,270], request scale(1,1,-1) on it;
otherwise keep identity. Request translation with
(x+float(R.s16[6]),y+float(S)+float(R.s16[8])-40,0), preserving separate binary32
add/subtract steps. Matrix helper composition/paired-single arithmetic remain
service contracts; the exact local request order is identity, optional scale,
translation.

Call animPoseMain(R.word[0]), then animPoseDrawMtx(poseID,matrix,2,
f1=R.float[14],f2=R.float[10]). Reissue FontDrawStart_alpha(original alphaWord),
then FontDrawColor(savedWordPointer), and continue. No local visibility/history/
window-opacity gate, fade multiplication, key-result mutation or pose free.
Font/animation helpers may mutate shared state; these calls restore requested
font alpha/color, not a proven full GX/font-state snapshot. The same pose can
be advanced on repeated draws because animPoseMain is inside the draw path.

## Icon clipping, request and color restoration (B)

After common history admission, fff8 fetches/saves current font color. If signed
word S+R.s16[8]+R.s16[a]-40>0, skip the icon; equality0 remains admitted. Build
position(x+float(R.s16[6])+20,y+float(S)+float(R.s16[8])-40+float(R.s16[a]),0).
Request iconDispGxAlpha(position,16,low16(R.word[0]),combinedAlpha,
f1=R.float[10]), where combinedAlpha is low8(trunc(fadeByte*low8(alphaWord)/255)).
No icon-coordinate geometry scaling or use of R.float[14] locally.

Then reissue FontDrawStart_alpha(original alphaWord), restore saved color via
FontDrawColor, and continue. Skipped icons bypass those final restore calls,
although the initial FontGetDrawColor already ran. Icon IDs/camera argument16,
asset bindings and actual glyph alignment need the icon service/data contract.

## Ordinary glyph admission and history emphasis (B)

For other drawable codes, let vertical=S+R.s16[8] with word arithmetic. Convert
to binary32 and compare with fused(32*R.float[10]-10). If ordered vertical is
greater than or equal to this threshold, skip. Unordered does not satisfy that
combined greater/equal branch. No horizontal/right/bottom clipping test locally
at this gate; window scissor and inner font renderer are separate.

If historyEmphasis!=0, request GXSetTevColor(1,color) with RGB0x6e6e6e when
W.type (W from D.f24c) is3/8/9, otherwise0x505050; set alpha=fadeByte. This
request is not explicitly multiplied by alphaWord here. For record flag0x10000,
call FontDrawScaleVec with(R.float[10],1,1); otherwise FontDrawScale(R.float[10]).
Call FontDrawCode(code,drawX,drawY), then FontDrawColor_(), then continue.

When historyEmphasis==0, remaining dynamic/shake/wave/selection/normal glyph
branches execute at0x800839f4 onward, specified in [glyph effects](MESSAGE_GLYPH_EFFECTS.md).
They include draw-time RNG, clock-based motion, matrix/font state and a stored
factor mutation. The admission/return contract alone is not full glyph presentation behavior.

## Indicator render callback (B)

msgDispKeyWait_render preserves incoming word r3, then calls animGetPtr, which
loads the global pointer at0x80415dc0 without validation. Request, in order:
GXSetZMode(1,7,0); GXSetBlendMode(0,4,5,0); GXSetZCompLoc(0);
GXSetAlphaCompare(6,128,1,0,0). Store the incoming word into animation work+0xe0.
No active-halfword/opacity/state test, alpha clamp, pose advancement, mesh draw
or saved-GX restoration locally. Incoming-word meaning and animation renderer's
consumption of field+e0 remain caller/service questions, not inferred pose ID.

Initialization installs this callback on hoshi_2/HOS_C_1 as covered in
[lookup](MESSAGE_RESOURCES_AND_LOOKUP.md). Embedded anim parsing also installs
it on that global work pose, not on the newly created record pose. The window
tail's active flag, pose update/draw request and this callback's GX/work mutation
are distinct layers; callback name alone does not establish input semantics.

Differential obligations: exposed live prefix versus parsed count/schedules,
terminal cancellation, signed/unsigned key match, pre-history control effects,
history boundary/strictness/fade bytes, pose/history bypass and draw-time updates,
icon clip equality/composed alpha/color restore, glyph threshold/emphasis and
indicator GX/work stores. Remaining: font/icon/matrix/pose
services, callback field meaning, actual draw cadence and original image/story
caller comparisons. Full message rendering/dialogue/game equivalence remains
unproven; no dialogue text or story sequencing is changed here.
