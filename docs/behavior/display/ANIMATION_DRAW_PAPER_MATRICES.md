# Paper draw matrices:prepass outputs and parent transform overrides

US G8ME01 disc0 revision0;ID1,2026-10-12. Evidence B:complete
animPoseDrawMtx0x8003b4d8. Matrix operations below are REQUEST ORDER to SDK;
PSMTXCopy/Concat/RotRad/Scale/Trans numeric/alias behavior remains separate.
Notation from [wrapper](ANIMATION_DRAW_WRAPPER_AND_ADMISSION.md). No execution.

## Optional linked prepass

Initial parent link !=−1 AND P+0 has ANY mask0xc0 (0x40 or0x80) admits prepass
BEFORE linked resource/pass filter. Capture parent's resource Dp;E starts as
incoming M. If P+0 mask8 CLEAR AND incoming A!=0 via fcmpu:request
RotRad(Y,round32(A*0x3c8efa35)) then Concat(M,rotation,scratch);E=scratch.
Mask8 SET skips this angle transform. Then if incoming S!=1 via fcmpu:
Scale(S,S,S),Concat(E,scale,sameScratch);E=scratch. NaN is not equal0/1.
Second concat can alias its FIRST input/output after preceding rotation.

Set modeling level at0x8041e89c=0,modeling pointer0x8041e8a0=0x803d8e70;
Copy(E,modelingBase). Publish CURRENT W+18=parent P and CURRENT W+20=Dp.
Call evalProc(parentIndex=−1,groupIndex=word[Dp+144]−1) at8003b5f8.
[Group traversal](ANIMATION_GROUP_TRAVERSAL_AND_REQUESTS.md) specifies cache
requests and hierarchy order. This pass does not emit geometry via renderProc.
It can update requested per-group position/matrix stores before later admission
fails. It does not assign its E/scratch as the later wrapper M by default.

## Common two-dimensional override

For admitted linked draws,publish wrapper parameters/link material as in
[wrapper](ANIMATION_DRAW_WRAPPER_AND_ADMISSION.md). Both parent-mask2 branches
begin C=Scale(P+10c,P+110,1),then T=Trans(P+104,P+108,0) and
Concat(C,T,C). These fields are P fields,not global W fields.

## Parent P+0 mask2 SET

Copy C to CURRENT W+3c;reload W and set W+cc=address CURRENT W+3c.
Clear CURRENT W+d0,+d4 individually. Original code includes a null test of the
STACK ADDRESS holding C;ordinary valid stack domain takes the nonnull copy path.
It is not a test for zero scales or a nullable input matrix.

Separately S3=Scale(P+f8,P+fc,P+f8). If CURRENT word[W+10c]!=0,
Y=round32(2*P+100);otherwise Y=P+100. Request T3=Trans(0,Y,0),
Concat(T3,S3,S3),then Concat(incoming M,S3,globalMatrix0x8041e86c).
Replace eventual M pointer with that global matrix. Global mode W+10c differs
from P+10c scale;do not merge similarly numbered fields. Original angle/scale
floats A,S are still forwarded to the inner service.

## Parent P+0 mask2 CLEAR

Retain incoming M pointer. Extend common C in order:

1. Concat(C,Trans(−0.5,+1,0),C).
2. Concat(C,Scale(single1/P+f8,single1/P+fc,1),C).
3. Concat(C,Trans(+0.5,−1,0),C).

Copy this C to CURRENT W+3c,publish W+cc=address CURRENT W+3c,clear d0/d4.
Reciprocals have no zero/nonfinite guard. This override is consumed separately
by group/matrix/material services;do not multiply it directly into M as a
substitute. Concat output aliases FIRST input on all these steps.

## Lifetime and evidence

[Inner draw](ANIMATION_INNER_DRAW_STATE_AND_BUFFERS.md) receives final M plus
unchanged A,S,V. Linked wrapper clears cc/d0/d4 only after that request;it does
not restore previous overrides. Copy/prepass/global matrix/current-pose fields
also survive until other writers. Rejection before preparation does not run
cleanup. Distinct flags on parent and linked record govern different routes.
`verify_animation_draw.py` checks literal transform operands/order/constants.
Original SDK alias/FPSCR,model-stack capacities,override consumers/pool lifetime,
geometry/material/GX and original paper visuals remain open.

ID1 matrix SDK dependency update (2026-10-12):
[affine/stack](ANIMATION_MATRIX_STACK_AND_AFFINE_NUMERICS.md) and
[rotation](ANIMATION_SDK_AXIS_ROTATION.md) now specify complete local called
Copy/Concat/Scale/Trans/RotRad bodies,including alias snapshots/store order.
Override consumers/source/lifetime,GQR0/FPSCR/libm/render/material/GX/original
paper visuals remain open;numeric equivalence is not established by host matrices.
