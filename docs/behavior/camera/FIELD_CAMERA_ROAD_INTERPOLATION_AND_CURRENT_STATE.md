# Camera road interpolation and current-state construction

US G8ME01 revision0; recovered2026-10-13 for
[OW-CAMERA-01](../../requests/OW-CAMERA-01.md). Complete calcHokanCamRoad8000b038/
238hex (142 instructions) and calcCurrentCamState8000def4/210hex (132), plus
scoped interpolation caller arguments. Addresses/fields hexadecimal; counts
and arithmetic decimal. Static original deductions, not measured framing.
Read [comparison/rate](FIELD_CAMERA_ROAD_COMPARISON_AND_TRANSITION_RATE.md),
[lifecycle/projection](OVERWORLD_CAMERA_LIFECYCLE_AND_PROJECTION.md).

## Interpolation captures and exact weight construction

calcHokanCamRoad retains mode r3/GPR28, first state A/r4/GPR29, second B/r5/GPR30,
output O/r7/GPR31 and supplied progress f1/f29 BEFORE children. Argument r6 is
unused locally. No pointer/extent/active/finite/progress validation.

Three cos80271ea8 requests, at8000b084/90/ac: first cos(source0), second
cos(source0), then cos(phase). Round each return to single. Let P,Q,C be those
three rounded returns. The phase uses single multiply supplied progress by
source word40490fd8 (3.141592025756836), then single FMA(phase,1,0) before call.
This is NOT the different positive-pi word40490fdb used in angle wrapping.
Preserve all three calls; constant folding their results assumes child behavior.

In original single operation order:

1. D=add(1,P); N=subtract(1,Q).
2. H=divide(2,D); V=FMA(0.5,N,C).
3. V=multiply(H,V); V=add(1,V); V=subtract(2,V).
4. W=multiply(source0.5000010132789612,V), word3f000011.
5. If ordered W>1 replace with1; otherwise retain W, including unordered.

There is NO lower weight clamp or incoming progress clamp here. With supplied
P=Q=1 the real-number shape reduces to multiplier*(1-C), but separate rounded
operations remain authoritative. Do not substitute exact .5, exact mathematical
pi or a generic linear curve. The caller owns progress/timestamp admission;
this function does not read time or count frames.

## Points, nearest-angle direction and scalar writes

Mode nonzero: for fields28,2c,30, individually read A then B, single(B-A),
singleFMA(W,difference,A), store O, in that field order. Mode0 copies the three
B floats directly. Other interpolation always continues, regardless of mode.

Read A34/A3c into f1/f2; request atan2802725a0 at8000b158, round result to single
angle U. Read B34/B3c; same request at8000b16c, round angle V. Capture these
returns before any direction output store. Then:

- While ordered single(U-V)>source positive pi40490fdb, subtract single twoPi
  word40c90fdb from U and repeat.
- Then while ordered single(U-V)<source negative pic0490fdb, add twoPi to U.
- Inclusive ±pi boundaries stay as supplied; unordered comparisons exit loops.
- Single difference(V-U), FMA(W,difference,U) yields blended angle J.
- sin80272410(J) at8000b1b8, round single/store O34; then cos(J) at8000b1c8,
  round single/store O3c. O38 is not part of that sine/cosine pair.

There is no iteration cap, result-domain/null guard, final angle normalization
or interpolation of direction components independently. Child atan2/sin/cos and
FP domains own their actual outputs; nonconverging exceptional inputs must not
be silently replaced by a native timeout claim.

Finally interpolate fields38,40,44,48,5c with the same separate single(B-A)/FMA
formula, fresh reads per field and original ordered stores. No writes to state
words0/4/8/24, progress4c, or other fields. Output alias/overlap can affect later
source reads; the body does not snapshot both whole states or use memmove.
ABI80-byte frame/GPR28..31/f29..31/LR and paired-single state restored. Seven
local direct calls; no independent scalar result inferred.

## Scoped original caller arguments

camRoadMain region8000aef8..14 requests interpolation with mode=G.wordcc,
A=G+138, B=G+d8, r6=G+68(unused by the child), O=G+08, progress=G.floatd0.
G is the freshly loaded road-work pointer [80414fa8] at8000aef8. These eight
owner/argument/call words pin the child interface; full transition/timestamp/copy
admission remains the caller's separate contract. Do not redirect output to
r6 or label r6 as a third blended state merely from its supplied pointer.

## Current-state prefix and inactive branch

calcCurrentCamState retains output pointer O/r3/GPR31. Read G=[80414fa8], then
G.word0 pointer H, then H.worde8 pointer V. Copy V.float0/4/8 to O0/4/8 in order.
This happens BEFORE the active test; no pointer validation or early null return.
Then reload G FRESH. If G.word8==0:

O0c=0, O10=100, O14=100, O18=0, O1c=0, O20=0, ordered single stores; exit.
The100 literal bits42c80000 live8041f698. Prefix copies still occur. No matrix
or trigonometric call on this inactive branch; other output fields unchanged.

## Active construction captures geometry before library calls

For G.word8 nonzero, compute single alpha=multiply(source phase40490fd8,G4c),
then divide by source180. Capture G30/34/38 as base X/Y/Z, G3c as dx direction,
G44 as dz direction, G48 as magnitude L, G50 as rotation operand. All these
values are in saved FPRs before the first cos request; later G mutation does
not cause geometry rereads. Output prefix came from the earlier G snapshot.

Request cos(alpha) at8000dfdc; round to single C1. Single multiply(directionX,L),
then multiply by C1 gives displacement DX. Request sin(alpha) at8000dff0,
round S; single multiply(L,S) gives DY. Request cos(alpha) AGAIN at8000e000,
round C2; multiply(directionZ,L), then multiply by C2 gives DZ. Keep both cos
requests/roundings; no reuse of the first result or FMA in these products.

Store O0c=single(baseX+DX), O10=single(baseY+DY), O14=single(baseZ+DZ).
Build stack axis=(-directionZ,0,directionX). Single multiply original rotation
operand by source3c8efa35 (about0.01745329238) supplies f1 for
PSMTXRotAxisRad8029e1c0(matrix stack+2c,axis stack+08) at8000e04c.
Then build stack vector(-DX,-DY,-DZ) and request
PSMTXMultVec8029e6a4(matrix stack+2c,vector stack+20,result stack+14)
at8000e074. Finally read O0c/10/14 FRESH and single-add resultXYZ, storing to
O18/1c/20 in order. No normalization/zero-axis/child-error/result check locally.

These component formulas pin local construction; matrix/math children own actual
numeric transformations. Final reads observe O after children, so they must not
be replaced by the earlier base/displacement registers under unproved alias
assumptions. No projection/view matrix publication or room clamp occurs here.
ABI256-byte frame/GPR31/f23..31/LR and paired-single state restored. Five direct
calls; no independent scalar return/IRQ/wait or measured lens behavior.

## Source constants and validation

[Manifest](../../../tools/camera_road_interpolation_evidence.json):20 spans/
294 anchors (all274 body words/twelve constants/eight caller words),twelve direct
calls, zero indirect sites. Constants also pin0,1,2,0.5,twoPi,±pi,180,100 and
rotation conversion; source addresses/bits are recorded without rounded-value
replacement. Three supplied-cos weight/six field/five angle mathematical vectors,
exact body/field/write/capture/child/caller sites and ISO pins checked independently.
Vectors are bounded deductions, not original cos/atan2/matrix execution traces.
Raw ELFs/listings/generator remain `/tmp`; no binary assets/raw bodies committed.

OW-CAMERA-01 gains these two complete local services. The
[complete local manager/time helper](FIELD_CAMERA_ROAD_MANAGER_COPY_AND_TIMEBASE.md)
now supplies timestamp/copy/transition composition. Remaining: calcCamRoad/
collisionCurve authored geometry/selection, pointer/container/profile/angle/
alias/lifetime/FP domains, math/matrix implementation and original road/room/
control/pause/cadence/visual binding. Native framing/full-game remain OPEN.
