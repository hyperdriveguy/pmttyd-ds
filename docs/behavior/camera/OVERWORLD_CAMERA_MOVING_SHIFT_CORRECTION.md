# Original moving camera shift correction and cap-release inertia

US G8ME01 revision0, static read-only DOL reconstruction. This closes the
nonzero seventh-argument correction in calcCamRoad8000b6cc for a successful
ordinary main path with record+b4 extraCurveCount0 and a finite nondegenerate
horizontal unit tangent. The19 straight records in
[the bounded path contract](OVERWORLD_CAMERA_STRAIGHT_PATHS.md) satisfy this
geometry domain. Curved/duplicate-path queries remain separate dependencies;
this owner can use a source-contract query provider once those are recovered.

The seventh argument comes from
[camShiftMain's separate state machine](OVERWORLD_CAMERA_COLLISION_TARGET_AND_SHIFT.md).
Its value is not a world-space offset until this owner converts it using lens
and distance. There is no evidence for substituting zero in moving Mario's
ordinary camera path.

## Required inputs and retained work

P is the current divided player targetXZ (original arguments1/3), Q0 the
initial collisionCurve main-path anchorXZ, T its normalized horizontal tangent,
N=(-T.z,T.x), distance the selected road record+58, currentShift the incoming
seventh argument, previousShift=previousState+58. Previous state here is
camRoadMain's work+d8 destination/history, not work+8 displayed state.
PreviousAnchor=previousState+28/+30, previousFlags=previousState+8,
previousTimestamp=previousState+50/+54. The Y used for the final anchor remains
current player targetY or the version101 record20 fixed-Y override.

Work fields initialized by camRoadSetup8000e104:

| Work offset | Source initialization | Consumer |
| --- | --- | --- |
| +19c | .005, word3ba3d70a | movement budget time rate |
| +1a0 | 0 | minimum displacement threshold time rate |
| +1a4 | .004, word3b83126f | tangent-weight increment per successful call |
| +1a8 | .008, word3c03126f | normal-weight increment per successful call |
| +1ac | 1 | tangent weight wT |
| +1b0 | 1 | normal weight wN |

Setup clears the whole work first, then sets these fields at8000e168–190.
The two weight increments are **per calcCamRoad call**, independent of the
getV60FPS timestamp interval. Other recovered source services may mutate work;
this contract describes ordinary setup and this owner, not hypothetical events.

Correction is admitted only when currentShift!=0, a current source-zone record
was selected (r23!=0), and previousState.active!=0 (8000bd24–3c). With shift0,
no current selection, or inactive history, skip correction and retain baseline
Q0. The weight-update tail can still execute after this skip.

## Lens-to-shift conversion and two path queries

Source first computes theta=floatPi*(headerFOV*.5/180) at8000bcf4–bd20,
where **headerFOV is the first float pointed to by container+e8**. This owner
uses that authored value even though cam3dMain does not automatically install
it into CameraEntry's perspective FOV. FloatPi is word40490fd8.

Define the finite real-number shorthand

    K = .01 * 2 * (640 * distance * tan(theta) / 480)

The actual query offsets are **previousShift*K** and **currentShift*K**.
The constant dimensions640/480 are source literals44200000/43f00000, not the
3DS framebuffer. The exact f32 evaluation order is:

    v = distance * sin(theta)
    v = v / cos(theta)
    v = 640 * v
    v = v / 480
    v = 2 * v
    v = shift * v
    v = .01 * v

Source trigonometric outputs are rounded to f32 before these operations. The
previous shift load at8000bd7c is **previousState+58**, not camera distance.

1. Query main collisionCurve(mode0,currentRecord, P+previousShift*K*T), using
   source sentinel input Y1000000 and fresh local output state (8000bd58–be14).
   If it succeeds, A=returnedAnchorXZ; otherwise A=Q0. This computes the
   current-player target with the **prior** shift, not a forward-distance
   lookahead. Its new flags/index/tangent do not replace the initial output.
2. Independently query main collisionCurve(mode0,currentRecord,
   P+currentShift*K*T), again with sentinel Y1000000 (8000bee8–bf9c).
   If it succeeds, B=returnedAnchorXZ and outputFlags=its returned flags,
   replacing the initial main query flags. If it fails, B=Q0 and retain the
   initial flags. The final float segment token and final direction still
   come from the initial query, not this shifted query.

An extra-curve call can follow **each** query when record+b4!=0 at8000be38–a8
and8000bfc4–c038. That is excluded from this main-only contract. It must not be
silently treated as identical to the no-extra branch.

## Cap-release resets

Only when the second query succeeds, compare its flags with previousFlags:

- New flags have bit1 clear and previousFlags bit1 set: set wT=0 at8000c068–70.
- New flags have bit2 clear and previousFlags bit2 set: set wN=0 at8000c090–98.

Here bit1 is integer mask1 (end cap), bit2 is integer mask2 (side cap), not
bit positions1/2. A second query miss performs neither reset. Comparisons are
against previous retained destination flags, not the first query's flags.
Weights are read **after** the resets for this callback's movement budget.

## Finite geometric movement limit

All geometry below is in authored units. Define a=A-PreviousAnchor and
b=B-PreviousAnchor. Source computes magA=PSVECMag(a) at8000c0e8. It also calls
PSVECMag(b), but that return is overwritten before being used in the budgets.
Do not substitute magB for magA.

Let elapsed(rate)=getV60FPS(rate,previousTimestamp,currentGP+40/+44).
The two original calls at8000c11c/148 give:

    travelBase = magA + elapsed(work19c)
    thresholdBase = elapsed(work1a0)
    tangentBudget = travelBase * wT
    normalBudget = travelBase * wN
    tangentThreshold = thresholdBase * wT
    normalThreshold = thresholdBase * wN

Thus the ordinary initialized thresholdBase is0. Source clock conversion is
as described in [transition timing](OVERWORLD_CAMERA_ROAD_TRANSITIONS.md):
each source timestamp is quantized independently to integer milliseconds.
These clocks are GP40/44, distinct from camShift's GP38/3c.

Solve b=alpha*T+beta*N. The source solves the2x2 system by selecting the
larger-magnitude tangent coordinate (8000c178–200), using fused negative
multiply/subtract instructions; it does not call dot products or insert an
epsilon/determinant fallback. In the bounded unit/cardinal domain alpha/beta
satisfy alpha=dot(T,b), beta=dot(N,b), with host arithmetic an explicit numerical
adaptation. Preserve signs alpha<0 and beta<0 separately.

Create tangentVector=alpha*T and normalVector=beta*N. Source uses PSVECMag
on each (8000c268/274). Clamp tangentVector to sign(alpha)*tangentBudget*T
**only if** its magnitude>tangentBudget AND magnitude>tangentThreshold.
Clamp normalVector similarly to sign(beta)*normalBudget*N only if both normal
comparisons are strictly greater (8000c27c–2c0). Equality does not clamp.
Negative budgets/invalid tangents are outside this finite initialized domain;
source does not insert a safe-normalization or invented zero-direction result.

The new anchorXZ is PreviousAnchor+tangentVector+normalVector at8000c2c4–2ec.
This is a displacement limit using prior-shift geometry, not exponential
smoothing of current player position. It can retain the old anchor on the
first cap-release callback because the reset weight makes that budget0.

## Tail writes and next callback

At8000c2f0–348, whether correction ran or was skipped on the successful retained
state path, add work1a4 to wT and work1a8 to wN and clamp **only each upper bound
at1**. A cap-release resets its weight before movement and leaves .004/.008
for the next callback after this tail. Initial weights1 remain1.

Then write the candidate state at8000c354–3dc: active1; current source-selection
word; chosen flags; divided player/look targets; selected/retained record;
corrected anchor; initial query direction/distance/pitch/secondary/index;
currentGP40/44 timestamp; **current seventh argument** at+58; record7c at+5c.
The correction changes anchorXZ and second-query flags, not anchorY or baseline
segment/direction/lens parameters. This candidate becomes retained destination
according to [the transition owner](OVERWORLD_CAMERA_ROAD_TRANSITIONS.md).

If no current selection but previous state is active, the separate fallback
path uses current divided playerXZ with retained previous direction/distance/
pitch/secondary/token, flags0 and selection0. It bypasses the nonzero correction
because r23==0, but still updates weights and candidate timestamp/currentShift.
If no current selection and no previous record, only failed-query target
history work1d4..1e8 is written and the function returns at8000ba14–44; this
path does not run the weight or candidate-write tail. Do not invent an inactive
candidate clear here: existing candidate bytes are retained.

## Verification and open branches

Run `python3 tools/verify_overworld_camera_moving_shift.py`. It pins containing
DOL owners, literal words, both shift operands, cap-release branches, source
clock and output ownership. Finite derived checks cover signed shift input,
retained-shift ordering and cap-release budget-zero behavior. They are source
reconstruction checks, not original gameplay traces or Gekko numerical parity.

A native binding needs a path query provider for both additional queries,
actual retained destination/clock/shift fields, and candidate output history.
Curved and duplicate segment geometry, extra curves, disabled/version-specific
records and exceptional SDK floating-point behavior remain open separately.
