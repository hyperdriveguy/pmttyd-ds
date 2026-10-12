# Selection draw callback, geometry and cursor smoothing

US G8ME01 revision0, recovered2026-10-12. B evidence is static original DOL:
selectWindow_Disp at0x8007fdf4,size148, constants at0x804205fc..0x80420618 and
integer-conversion bias at0x802c3340. See [selection state/input](MESSAGE_SELECTION.md),
[window manager](MESSAGE_WINDOW_MANAGER.md) and [lifecycle](MESSAGE_WINDOW_LIFECYCLE.md).
This covers the local callback; inner border/text/icon services and display
dispatch cadence remain open; local dispatch is now covered in
[display queue](DISPLAY_QUEUE_AND_DISPATCH.md). No original draw output is captured here.

## Callback arguments and admission (B)

Display callback receives its window pointer W in r4; incoming r3 is not used
locally. Load handle=W.word[0x28], cache D=*handle before drawing. No allocation,
state/flags, null handle, opacity-zero, option-count or readiness guard. Eligibility
was decided by main/display registration; queue lifetime still needs recovery.
Text drawing later reloads W.handle, while cursor drawing retains this cached D.

## Fog, border and text calls (B)

First call GXSetFog with type0, all four floating arguments0.0, and a pointer
to a copied color word from0x80429584. That word is in DOL BSS, not a constant
stored in the ISO; runtime writers remain untraced. The callback does not locally
restore the previous fog state at return. GX service behavior is a dependency.

Construct border color from original RGBA word0xffffff00 at0x804205fc, replacing
its low alpha byte with low8(W.s16[6]). Call windowDispGX_Waku_col at0x8007b44c
with integer type0, color pointer, floats(W.x,W.y,W.width,W.height,30.0), using
geometry at W+c/+10/+14/+18. Opacity is narrowed to a byte without clamping;
whole signed halfword is not passed as a separate border alpha argument.

Then call msgDisp(W.handle,low8(W.s16[6]),f1=f32(W.x+40),f2=f32(W.y-25)).
This draws message records through the normal text service; local callback does
not copy option text, modify parser/playback indices or apply its own text clip.
Record highlighting and glyph/embedded-object drawing require msgDisp recovery.
Local order is fog, border, text, cursor update, cursor icon.

## Cursor target and stored draw-time mutation (B)

Selection main changes signed-byte target index D.f243. Constructor supplies
initial cursor ordinate at D.float[f248]. Draw callback reads those fields and
performs the following binary32 sequence:

1. Sign extend D.byte[f243] to integer t; compute31*t+58 with word arithmetic.
   Convert through the original0x4330000080000000 double bias, using fsubs.
2. Let target=f32(W.y-f32(31*t+58)). Compute delta=f32(target-oldY), where
   oldY=D.float[f248].
3. Store newY=singlePrecisionFusedMultiplyAdd(k,delta,oldY), with k from
   0x80420614, bits0x3e99999a (approximately0.3). Reload the stored float.

The binary uses fmadds for step3. A host expression that separately rounds
the multiplication and addition is not the same established operation. There
is no local snapping threshold, clamp, elapsed-time scaling or delta-time input.
Signed target arithmetic admits all byte values; no option-range normalization
is inserted here. Larger layout/inherited-state validity is a caller obligation.

This writes D.f248 during rendering, not selectionMain's navigation update.
Even opacity0 takes this path if callback is invoked. Repeated draws without
navigation move the stored cursor repeatedly; skipped draws preserve its old
ordinate. Static-derived constant-target error after n draws is approximately
0.7^n times initial error, subject to the original binary32/fused rounding.
It is not a measured rate per simulation tick or rendered frame.

## Cursor icon request and remaining services (B/P)

Build stack position(float W.x+2,newY,0). Call iconDispGxAlpha at0x8001a93c with
that position pointer, integer arguments20 and0x1f8, alpha low8(W.s16[6]), and
f1=1.0. These are concrete request values; camera/icon enum meaning and original
texture/animation binding still need the icon service/data contract. No separate
cursor visibility gate or high/low opacity normalization locally.

Verified constants: zero0x80420600, border30 at+4, text40 at+8/text25 at+c,
cursor2 at+10, smoothing at+14, icon scalar1 at+18, plus the RGBA word and
double bias. These are original bits, not provisional native design constants.
The callback does not locally free the handle/icon, acknowledge selection result,
change W.state/flags or update target index. Direct state stores in this callback
update the cursor ordinate; called-service mutations remain separate dependencies.

Differential obligations: exact call order, BSS fog-color source, low8 opacity,
border/text/cursor geometry, signed target index, fused stored smoothing and
draw-count dependence, cached D versus freshly loaded handle, opacity-zero
behavior and icon request values. Remaining: border/text/icon rendering bodies,
sort implementation/ties and actual invocation count, input/render ordering,
matched original images/traces and original selection/story callers. Local draw
contract is established; full UI/dialogue/full-game equivalence remains unproven.
