# Inner draw facing,angle smoothing and matrix request order

US G8ME01 disc0 revision0;ID1,2026-10-12. Evidence B:complete
_animPoseDrawMtx,local8003be8c..8003c210. Entry/notation from
[inner draw](ANIMATION_INNER_DRAW_STATE_AND_BUFFERS.md). No execution.
All comparisons ordered except explicit `fcmpu` equality. Original SDK/libm/
FPSCR/alias/source domains remain separate;A is input angle,S input scale.

## Camera heading and target selection when P+0 mask8 SET

Compute single H=A−90,store P+70. Query camGetPtr(4),compute camera X18−Xc
and Z20−Z14 with fsubs;request atan2(−deltaX,−deltaZ),frsp return,then single
multiply57.29579162597656 (word42652ee4). Call this raw heading K.
Normalize first by repeated +360 while<−360,−360 while>=360,then one+360
if<0. Let N(K) denote that exact sequence,not a generic wrapping helper.

D=N(K)−CURRENT P+70. Wrap D with repeated +360 while<−360 and−360 while>=360;
NO final negative-to-positive add. If D in(90,270] OR[−270,−90),select rear;
otherwise front. Boundaries90/−90 front,270/−270 rear;NaN front.

BOTH branches query camGetPtr(4)/atan2 AGAIN and normalize a FRESH heading K2.
Front stores N(K2) to P+7c. Rear adds180 to N(K2),wraps with repeated ±360
at−360/360 boundaries (no last negative add),then stores P+7c. Do not retain
first camera sample as the target or infer a shortest-arc choice from one call.

## Unwrap retained angle and interpolate per invocation

While CURRENT target P+7c−CURRENT retained P+74>180,add360 to P+74 and store.
Then while target−retained<−180,subtract360 from P+74 and store. No finite
validation/cap;NaN exits ordered loops,infinity can fail to terminate.
Reload P+0 mask0x100:SET snaps P+74=target;CLEAR stores
fmadds(0.10000000149011612,single(target−retained),retained).
This advances once per admitted INNER invocation,without a simulation-tick or
frame-count guard. Do not multiply smoothing by elapsed time.

Reload angle P+74;if !=0 via fcmpu,RotRad(Y,single angle*0x3c8efa35),
Concat(M,rotation,scratch),M=scratch. Then if P+78 EXACTLY equals float−1,
Scale(1,1,−1),Concat(M,reflection,sameScratch),M=scratch. This stored sign
comes from [Main's earlier facing tail](ANIMATION_MAIN_CAMERA_FACING.md);
no fresh Main call precedes this test locally. Alias FIRST input/output if M
already scratch. No reclassification from smoothed angle here.

## Mask8 CLEAR and common uniform scale

If mask8 CLEAR,skip P+70/+74/+7c updates/camera/sign reflection. Instead rotate
Y by incoming A iff A!=0 via fcmpu,using the same factor/concat scratch behavior.
For EITHER branch,if incoming S!=1 via fcmpu,Scale(S,S,S),Concat(M,scale,scratch),
M=scratch. Rotation/reflection therefore precede uniform scale. NaN angle/scale
is not normalized or rejected.

Only after these matrices does inner check P+3c==−1 and possibly call Main.
That Main can rewrite P+78 AFTER the current reflection decision;do not move it
before matrix construction to simplify pose evaluation. Matrix/resource/current/
next/fraction captures remain retained through that call. Current wrapper parent
flag8 and inner linked flag8 are different flags and can choose different routes.
`verify_animation_draw.py` pins full local graph/constants. Original numeric,
flag/angle writers,camera cadence,pool/alias and visible paper facing remain open.
