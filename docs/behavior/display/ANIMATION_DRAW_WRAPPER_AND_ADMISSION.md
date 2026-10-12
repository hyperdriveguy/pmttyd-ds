# Animation DrawMtx wrapper:admission and linked request order

US G8ME01 disc0 revision0;ID1,2026-10-12. Evidence B:complete
animPoseDrawMtx0x8003b4d8,size0x790. Inputs ID r3,matrix pointer M r4,
original integer pass V r5,angle A f1,scale S f2. No original execution.
P=word[W+10]+0x170*ID,W loaded through0x80415dc0. No local ID/active/null
validation. Loaded resource lookup used here and by the inner service:
J=word[W+0];E=word[J+16*poseResourceIndex+8];H=word[E+a0];D=word[H].
The code captures/reloads pointers at distinct points;live-pool/alias domains
remain separate.

## Unlinked and linked admission

Initial P+90==−1 goes directly to ordinary resource admission below;otherwise
perform [optional evaluation prepass](ANIMATION_DRAW_PAPER_MATRICES.md),then
reload current P+90 and current W/pool to capture linked record Q. Resolve Q's
resource Dq. Filter ORIGINAL pass V using Dq+c4. Unlinked route resolves P's
resource Dp instead. For either resource:

- V==2 AND resource+c4 mask1 SET:admit immediately.
- Otherwise compute literal PowerPC `slw(1,V)` and test resource+c4 against it;
  zero intersection returns to epilogue. No bounds or enum validation for V.

For normal0..31 pass values the second rule tests1<<V. Do not replace mode2's
special mask1 admission with an ordinary mask4 test. Higher/negative shifts
retain original instruction semantics;invalid inputs are not clamped.

A linked prepass may already have changed globals before this filter returns.
Rejection does not restore those globals. Initial unlinked route performs no
paper/group/GX cleanup or prepass. The original Mario a/a_mario resource has
mask+c4=1,so its ordinary mode2 is admitted by the SPECIAL rule while mode1
fails the normal mask2 test. That is a serialized-data example,not a runtime
observation or proof that all actor resources share the mask.

## Linked preparation and final request

If CURRENT captured Q+4 mask2 SET,perform
[shared ring and paper selection/timestamps](ANIMATION_DRAW_SHARED_BUFFERS_AND_PAPER_SELECTION.md).
CLEAR skips those steps. Both cases then:

1. Capture parent P+98 andP+9c. Publish W+30=P+a0 iff first word nonzero,
   else null;W+34=P+c0 iff second nonzero,else null.
2. Publish W+38=0,W+24=captured98,W+28=captured9c,W+2c=0,
   W+d8=1,W+dc=1. Reload W between stores;do not clear the entire work object.
3. Copy raw parent words e8,ec,f0,f4 into captured Q at the same offsets,in order.
   Material parameters are distinct from GLOBAL ring fields with similar offsets.
4. Prepare [parent paper matrix requests](ANIMATION_DRAW_PAPER_MATRICES.md).

At final dispatch8003bbd4,reload parent P+90. If !=−1,call
_animPoseDrawMtx(captured Q,effective M,V,A,S)0x8003bc68. After return,clear
CURRENT W+cc,+d0,+d4 individually. Then reread captured Q+4;mask2 SET clears
CURRENT W+e8,otherwise leaves it untouched. No cursor reset or pointer release.
If parent P+90==−1 at final dispatch,call inner on P instead and skip this linked
cleanup. That recheck does not recapture Q or unwind preceding side effects.

Unlinked admitted path also calls inner(P,M,V,A,S),without wrapper cleanup.
The inner ABI consumes a RECORD POINTER,whereas this wrapper consumes an ID.
Incoming angle/scale remain passed even if wrapper used them in a prepass;
prepass scratch does not automatically replace M in the eventual call.

## Used binding and evidence

[Partner draw](../partner/FIELD_PARTNER_DISPLAY_MATRIX_AND_PASSES.md) calls
wrapper at800b7a48 and800b7a60 (pass2 then1,A0,S1). Mario equivalents are
80057054/800570a0;Majinai sites are8025c5a0,8025c5b8,8025c5d0.
These are concrete binary bindings,not counts of original frames or scenes.

`python3 tools/verify_animation_draw.py` verifies five complete local bodies,
1396 words,20 constants,five data spans,two US lecture imports and148 direct
DOL sites,including135 wrapper references. A hash/metadata-checked Mario
resource pins pass mask42-group/root metadata. Source/mode/link/ring/material/
SDK/render/GX lifetime and original images remain open despite local closure.
