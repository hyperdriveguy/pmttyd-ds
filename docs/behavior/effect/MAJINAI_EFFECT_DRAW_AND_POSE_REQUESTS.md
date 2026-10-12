# Complete Majinai draw requests and pose scalar/material leaves

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete effMajinaiDisp8025c4c0/12c,camGetPtr8000f2fc/14,
material On80040730/20,Off80040710/20,LoopTimes8004079c/18 and
EvtColor800406b4/1c. Verify:`python3 tools/verify_majinai_effect_update.py`.
Read [Main/cleanup](MAJINAI_EFFECT_UPDATE_STATE_MACHINE.md),
[constructor](MAJINAI_EFFECT_CONSTRUCTION.md) and
[delete/name lifetime](EFFECT_DELETE_CALLERS_AND_LIFETIME.md).
SDK matrix/animation/camera/display implementations are linked identities;
these complete local request bodies do not close those engines.

## Draw callback ABI,guard and ordered matrices

Input r3 is callback camera/index word C,r4 is effect record E. Retain C;read
D=word(E+c),then signed pose ID D1c. EXACT-1 exits without ANY matrix/camera/
pose request. No E/D/flags/phase/alpha/other pose-ID guard. Other invalid IDs
are passed to services. No private payload snapshot or lifetime validation.

For admitted pose,perform:

1. Read D.X4/Y8/Zc and request PSMTXTrans(T,X,Y,Z).
2. Read scale D10 and request PSMTXScale(S,s,s,s).
3. camGetPtr(C);read float camera+114,negate it,then single-multiply by exact
   binary32 factor3c8efa35 at80428b20 (approximately pi/180). Request
   PSMTXRotRad(R,axis byte79='y',that angle).
4. PSMTXConcat(T,R,T),then PSMTXConcat(T,S,T),both output-aliasing first input.

Thus local request matrix order is translation→camera-relative Y rotation→
uniform scale. SDK in-place alias/numeric order and actual meaning/writer of
camera114 are dependencies;do not substitute a new rounded degree constant or
reorder concat based on a host rendering convention. Main depth vector was
captured earlier;draw reads live D coordinates at callback time.

camGetPtr itself returns WORD at803c95f8+wrap32(C<<2). It performs no bounds,
null/current-camera/context validation or fallback. It is a32-bit pointer-table
query,not a camera struct copied into the effect. Actual table initialization,
camera schema/transform state and original render scheduling remain open.

## Pose flags,event colour,animation main and three render passes

After matrices,read current D1c and request material On(ID,840). Load raw colour
templateffffff00 at80428b1c;read WORD D.alpha14 and replace ONLY last byte with
LOW8(alpha),then copy that complete word to a second local RGBA slot. Request
animPoseSetMaterialEvtColor(current D1c,&RGBA). Colour is (255,255,255,low8 alpha),
not a signed/float/clamped alpha conversion. Main ordinarily deletes below0,
but this local callback itself supplies no alpha-range guard.

Next call animPoseMain(current D1c). Then request animPoseDrawMtx THREE TIMES
with freshly read D1c each time,same T pointer,integer pass1,then2,then3,
float arguments+0.0 and10.0 from80428b24/28. No pass is conditional on kind,
phase,alpha,camera ID or a prior returned result. No old material flags/colour
are saved/restored and no return value gates later passes. Pose main/draw can
change data visible to later pose-ID reads;do not cache ID across the sequence.
This callback performs no direct GX/texture/audio/event call and no payload
write or hard-delete request. Renderer and actual image output remain unmeasured.

## Four complete raw pose leaves

All leaves use original pose index I and stride170 hex=368 bytes. Arithmetic
wraps32. Each independently reads animation-manager pointer80415dc0 (r13-7160)
and its pose-array pointer at+10. None supplies index/active/null/sentinel guard,
scene/context selection,resource load or dirty/render callback. They do not
use the effect record's captured context field.

| Service | Exact pose storage operation |
| --- | --- |
| MaterialFlagOn(I,mask) | Read WORD(array+170*I+e8),OR mask,store WORD |
| MaterialFlagOff(I,mask) | Read same WORD,AND complement mask,store WORD |
| GetLoopTimes(I) | Load IEEE binary32 at array+170*I+84;return f1 |
| SetMaterialEvtColor(I,colourPtr) | Read one WORD at supplied colourPtr;store to array+170*I+f0 |

The colour helper captures manager BEFORE reading colour word,and reads its
array+10 AFTER that colour read. Preserve that order/captured manager if memory
providers alias or mutate. On/Off preserve all other flag bits;mask800/840 are
literal requests,not definitions of lighting/render capabilities. LoopTimes is
a raw scalar getter;it does not advance/reset/increment animation progress or
validate ordered range0..1. Whole animation/pose engines and field writers remain
separate source contracts.

The five field-Mario poses and battle part pose affected by Main use these same
leaves. New model assets alone do not satisfy material/pose state requests;
native implementations need original index/address services or an explicitly
faithful adaptation preserving their operations and lifetime.

## Acceptance and remaining resource/platform work

Check exact-1 early exit,raw pointer-table C lookup and captured/live pose IDs;
original coordinate/scale reads,camera114 negation/single factor,Y-axis/concat
order and output alias;On840 before colour/main/draw,RGBA last-byte replacement,
three1/2/3 passes and exact float0/10 requests;all raw leaf offsets/stride/widths,
mask read-modify-write,loop scalar without progress mutation and colour query
order. Include Main soft expiry/no queued draw versus previously queued callback
lifetime as a DRIVER ownership requirement,not a new guard added here.

Original MOBJ_EFF_majinai/A_1 resources,pose async/entry/release/main/draw,
PS matrix math/camera/display depth/queue traversal,effect driver/context/time,
particle and scene ownership,packed addresses/FPSCR/alignment and original
visual/frame traces remain required. Local Main/Disp closure does not claim
original animation/render fidelity or complete game requirements.
