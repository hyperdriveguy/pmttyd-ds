# Camera road comparison and transition rate

US G8ME01 revision0; recovered2026-10-13 for
[OW-CAMERA-01](../../requests/OW-CAMERA-01.md). Complete compareCamRoad8000b270/
45chex (279 instructions), plus two scoped camRoadMain caller regions. Addresses,
masks and fields hexadecimal; arithmetic/counts decimal. Static deductions,
not observed room selection, camera motion or measured transition duration.
Read [lifecycle/projection](OVERWORLD_CAMERA_LIFECYCLE_AND_PROJECTION.md) and
[rate/reset owners](FIELD_MARIO_CAMERA_RATE_ID_AND_RESET_OWNERS.md).

## Arguments and early integer progress rejection

Retain first state A/r3 into GPR27, second state B/r4 into GPR28, output pointer
O/r5 into GPR29; mask K/GPR30 starts0. No pointer/extent/alias validation.
B.word0==0 returns0 immediately WITHOUT writing O. A.word0 is not tested.

If freshly read A.word24==B.word24, unless A.word4==1/B.word4==0 OR
A.word4==0/B.word8==1, apply this integer test:

1. Read B.float4c then A.float4c; fctiwz each separately, capture signed word
   results. Delta=wrap32(Ainteger-Binteger), not truncate(Afloat-Bfloat).
2. Compute word absolute by sign/xor/subtract; signed absolute<=1 returns0.
   INT_MIN absolute remains INT_MIN, so its signed comparison also rejects.
3. Fresh A.word24 as R; load R.word40 then R.word44; D=wrap32(R44-R40).
   Signed absolute>=signed(wrap32(D-1)) returns0.
4. Otherwise signed absolute>=signed(wrap32(D-2)) sets MASK20; smaller leaves K.

No original interpretation of R40/44 as a valid count/range is assumed here.
Pointer equality does not validate R; conversions/FPSCR/exception domains are
separate. Tests are signed after word arithmetic. All these returns leave O
untouched. Special cases bypass integer rejection; they are asymmetric A/B tests.

## Transition-mask accumulation uses fresh fields

After that gate, reread A24/B24: unequal sets MASK1. Read A.word4 once:1 with
fresh B.word4==0 sets MASK2;0 with fresh B.word8==1 sets MASK4. Other combinations
leave those bits alone. No generic enum-domain or symmetric state-change rule.

Read A.float4c then B.float4c, single subtract, fabs. If ordered absolute<=
DOUBLE1.0 at802bf570, leave MASK8 clear; otherwise, including unordered, set8.
This floating test is separate from the earlier individually truncated integer
progress test. Mask20 already set survives. K does not itself cause an early
exit: the following metrics may still reject the comparison and return0.

## Four ordered metric candidates

All operations below retain original single precision and load order. “Choose
larger” means replace only on an ordered candidate>current comparison; it is
not a host max with an invented NaN policy. Profile pointer G=[80414fa8] is
loaded FRESH at each indicated weight access. The five weight fields are not
fixed constants proved by this body.

First candidate:

- Single multiply A3c*B3c, then single FMA(A34,B34,product). If ordered result>1,
  replace with original float1.0. There is NO lower clamp at-1.
- Request acos80272560 at8000b410; round return to single, multiply by source
  radians-to-degrees word42652ee1 (about57.29578018), fabs. If ordered angle>180,
  replace with single(360-angle). No repeated wrap or normalization of input.
- Single sequence: P=twoPi*A40; P*=angle; P/=360; P*=fresh G.float1b4.
  This becomes metric S. twoPi is40c90fdb (about6.28318548).

Second candidate: single(A44-B44), fabs; ordered>180 replaces with360-absolute.
Single sequence(twoPi*A40)*folded/360*fresh G1b8; choose larger versus S.
Third uses A48-B48 and G1bc with the same separate operations and fold. This
third block captures A40 in f4 for the next candidate as well.
Fourth: single(captured A40-B40), fabs, multiply fresh G1c0; choose larger.
No camera lens/angle/length/physical-unit labels are assigned to opaque source
fields solely from these formulas. The A/B pointers remain captured across
acos; later fields and G can nevertheless change. No child-result validity gate.

Source float constants:1=8041f638, twoPi=8041f654, radians-to-degrees=8041f660,
360=8041f664,180=8041f668. acos/library/FP implementation owns actual values and
exceptions; real-number algebra alone does not prove bitwise original numerics.

## Point distance and complete square-root branches

Read raw point floats A28/B28,A2c/B2c,A30/B30. Separate single differences,
separate squared multiplies, then single(Xsquare+Ysquare), then add Zsquare:
Q. No fused multiply-add in this sum.

- fcmpu Q==source float0 returns that loaded0 as distance L.
- Ordered Q>0: frsqrte estimate R, then THREE double refinement iterations
  R=(0.5*R)*(3-Q*(R*R)), with original fnmsub at the bracket, followed by
  double Q*R and frsp to single L. The third iteration's factor multiplication
  and final product use the explicit source register sequence. Estimate/FPSCR
  fidelity must not be replaced by a claim that host sqrt was measured equal.
- Otherwise compare Q against double0. Ordered Q<0 loads original source NaN
  bits7fffffff at8041e3c8 as L.
- Remaining unordered/nonpositive case: store Q to single scratch, classify
  exponent mask7f800000/mantissa007fffff. Classification1(NaN) loads that same
  source NaN. Classes2(infinity),3(zero),4(normal),5(subnormal) leave Q as L.
  Positive infinity reaches the positive branch first; this classifier is not
  an unconditional early infinity bypass.

Double refinement constants0.5/3.0/0.0 at802bf578/580/588; source zero float
8041f62c. The branch/classification description closes the local code only.
Actual finite/exception/underflow/alias domains remain provider-owned.

## Joint deadband, bounded score and output publication

If ordered S<source .01 (bits3c23d70a/8041f66c) AND ordered L<source .3
(bits3e99999a/8041f670), return0 WITHOUT writing O, regardless of K.
Equality to either threshold does not take that paired rejection; unordered
comparison also does not establish either ordered less-than predicate.

Otherwise compute single L*fresh G1c4 and replace S only if ordered product>S.
If ordered S>90, replace with90 (42b40000/8041f674); then if ordered S<20,
replace with20 (41a00000/8041f678). Finally single divide source1.0/S, store to
O.word0 as float at8000b6a8. ONLY after the store, if K==0 set MASK10. Return K.
No output store on ANY earlier rejection, no output-pointer null guard,
no saturation of unordered S, and no state-field update in this function.
Caller must distinguish unchanged output from a new reciprocal.

ABI64-byte frame/GPR27..31/LR saved/restored. Exactly one direct call(acos),
no indirect callbacks, IRQ/wait/timestamp/frame counter. Returned bitmask and
reciprocal are local results; no duration in measured frames or seconds inferred.

## Both original caller directions are pinned

Scoped camRoadMain8000ab48 regions show fresh global G=[80414fa8]:

| Call site | A | B | O |
| --- | --- | --- | --- |
| 8000abd4 | G+d8 | G+68 | G+d4 |
| 8000ad54 | G+68 | G+d8 | G+d4 |

Those branches can reverse comparison direction; do not universally label A
as old/current or B as new. Full caller admission, copying/transition state and
rate consumption remain camRoadMain's separate contract. These eight pinned
argument/call words do not claim the complete caller recovered.

## Validation and remaining road work

[Manifest](../../../tools/camera_road_compare_evidence.json):18 spans/306 anchors
(all279 compare words/19 constant words/eight caller words),one local direct
call, no indirect sites. Eight integer/nine postmetric mathematical vectors,
integer wrapping/early returns, exact masks/calls/constant bits/output store/
caller directions checked independently against read-only ISO evidence.
Postmetric vectors begin with supplied S/L/weight; they are not original acos/
frsqrte observations. Generic pin verification does not prove semantic prose.
Raw ELFs/listings/generator stay `/tmp`; no binary assets/raw bodies committed.

Remaining: authored geometry/contact/selection via calcCamRoad/collisionCurve,
transition composition calcHokanCamRoad/calcCurrentCamState/camRoadMain,
actual road-state layouts/profile weights/pointer/value/alias/lifetime/FP domains,
room limits and original camera/control/pause/cadence/visual execution. The 2DS XL
port now has this local comparison service; OW-CAMERA-01/native/full-game remain
OPEN until the surrounding service and original binding are established.
