# Partner surrounding-wall binding and corrected two-pass admission

US G8ME01 disc0 revision0, ID1, recovered 2026-10-12. Evidence B: complete
local wrapper unk_800c27c0 at0x800c27c0,size0x38;shared marioChkWallAround
at0x800888d0,size0x99c;view leaf and four SDK vector leaves below. Original
instructions were read from the supplied ISO without executing them. Symbol
names aid navigation;the bytes and actual caller establish behavior.

## Actual partner binding

[Stay motion](FIELD_PARTNER_STAY_MOTION.md) calls this wrapper at0x80151bd4
after publishing the [front-wall](FIELD_PARTNER_FRONT_WALL_QUERY.md) X/Z
result and making a SECOND stateful direction/speed query. Its arguments are
P,the local position pointer V,speed in f1,heading in f2,width P+f4 in f3 and
height P+f0 in f4. These are engine units,not host geometry coordinates.

If P+0 has host mask0x00001000,set width f3 to the original6.0 constant at
0x80421518;otherwise preserve f3. Change r3 from P to V,set r4=1 and call
marioChkWallAround. Preserve speed,heading and height. Return its integer r3
unchanged. There is no partner-position store or hit-pointer publication in
the wrapper. Stay ignores this return and copies the resulting local X/Z to
P+58/P+60;its local Y is not published here.

The shared core captures heading,width,height and V/mode. It never uses input
speed f1 for ray length or correction size. Zero speed therefore does not
disable correction. This signature receives a heading angle,not a direction
vector that must first be normalized. P itself is not passed into the core.
The actual stay caller does not replace height with partyGetHeight here;
that helper belongs to the separate front-wall query.

## Shared ray and state ownership

The complete shared two-pass graph is described in
[overworld wall movement](../overworld/OVERWORLD_WALL_MOVEMENT.md#around-correction-two-directional-passes),
with the corrections below. kpaGetStageViewType at0x801feeb8,size0x30 calls
the global Mario getter,loads Mario+14 and returns1 when bit0 is CLEAR,0 when
SET. The core selects eight rays for result1,two for result0. Partner flags
and partner slot do not select this count.

Each pass captures a fixed root and casts,at each sequential angle,the Y+11
ray FIRST,then the Y+(height-1) ray. Both use radius R(.5*width). Start the
angle at heading;advance it with a single-result add of R(360/count) after
each iteration. Thus repeated addition is the literal numeric path,even
though H+k*(360/N) is a useful real-arithmetic description. u(H) is
(sin(H*pi/180),0,-cos(H*pi/180)),from the original sincosf helper.

Each ray independently chooses its callback from GLOBAL Mario state in
this priority order:motion+2e==28 → chkfilterVecVivian0x80091434;otherwise
Mario+0&0x01000000 → chkfilterVecRoll0x80091504;otherwise
chkfilterVec0x800915e4. The selector may obtain Mario again between tests.
This surrounding query does not reuse the partner front-wall Yoshi/Nokotaro
filter choice. Full special callback policies and scene traversal remain
called-service dependencies here;fingerprinting the shared caller does not
close those bodies.

For a nonnull hit,quantize distance using the original fused1000*d plus
ordered-d>=0?+.5:-.5,integer truncation and conversion/division by1000.
Before selecting between hits,call hitGetAttr independently for each candidate
(including null),then obtain global Mario for the ordered classifier:

1. attr&0x00080000 and Mario+0&0x01000000 CLEAR → retain as blocker.
2. Otherwise attr&0x00000080 CLEAR → pass through exactly when
   attr&0x00800000 is SET.
3. Otherwise → pass through exactly when global Mario motion+2e==25.

Discard pass-through candidates. Between two remaining blockers,replace the
first Y+11 candidate with the second only for ordered first-distance>second-
distance. Ties and unordered comparisons retain the first candidate. If only
one remains,use it. The labels lower/upper describe the intended ordinary
height domain;query order remains the rule when the heights coincide or reverse.

For each retained blocker,compute abs(R(radius-quantizedDistance));obtain
u(R(rayAngle+180)) by another sincosf call and accumulate X/Z with single-result
fused multiply-add. Do not substitute arithmetic negation of the previous
direction if numerical equivalence matters. Partial pushes never change ray
origins within a pass. Y accumulators and direction Y start at the pinned
original zero vectors and receive no local Y increment.

No first-pass blocker returns0 without changing V. Otherwise mode1 always
uses R(.125*width) for the initial cap,then replaces it with1.0 only for an
ordered cap<1 comparison. It skips the mode0 global-motion10/11 lookup and
.25 multiplier entirely. With the width6 override,the ordinary finite cap
is1.0 and radius3.0. No additional width/domain sanitization is present.

## Unclamped saved vector and corrected dot branch

At0x80088da4–bc,copy the accumulated FIRST push from stack+60 to stack+54.
This happens BEFORE PSVECMag and optional scaling of stack+60. If ordered
magnitude>cap,scale the working push by R(cap/magnitude). Add that working,
possibly capped push to V using PSVECAdd. The stack+54 copy remains the
UNCLAMPED first push;there is no later overwrite of that saved copy.

The second pass captures V after that addition,restarts at the original
heading and starts with a fresh zero push. No second blocker returns1 with
the first correction already applied. Otherwise PSVECDotProduct receives
the saved UNCLAMPED first push and accumulated second push. At0x800891e0,
fcmpo compares its result with0;cror2,1,2 at0x800891e4 ORs greater and equal;
bf2 at0x800891e8 jumps to half-scaling only if neither was true.

Consequently **ordered dot>=0 skips the second correction**. Negative or
unordered dot takes the scale/add path,assuming enabled floating-point
exceptions permit continuation. Scale the second push by.5 and add it to V;
there is no second magnitude cap. Return1 on either outcome. Return1 does
not request rollback of the first correction.

Earlier OVERWORLD_WALL_MOVEMENT text reversed this branch and described the
saved first push as capped. Both are corrected. A positive finite cap often
makes capped versus uncapped dots have the same sign in real arithmetic;
that is not permission to change the original operand,rounding,overflow or
exception path.

The following finite examples are deductions for an injected-ray test,not
observations of the original running game. Use N8,width20,heading toward+X,
ideal axis directions and only the first ray blocked at distance5 in pass1:
raw first push(-5,0,0),
cap2.5 and corrected X=-2.5. In pass2:

| Injected retained blocker | Second push | Dot with saved first | Final X |
| --- | --- | --- | --- |
| Same +X ray,distance1 | (-9,0,0) | +45 | -2.5;skip |
| Opposite -X ray,distance1 | (+9,0,0) | -45 | +2.0;apply half |
| Retained blocker at radius10 | (0,0,0) | 0 | -2.5;skip |

A NaN dot takes the half-add branch under nontrapping continuation. Test that
branch separately from reachability of NaN scene data;this recovery does not
establish a valid game's numeric domain.

## Complete local SDK leaves

All paired-single operations below are Gekko instructions. Under the required
GQR0 floating-load state,the vector layout is three binary32 values X,Y,Z.
The paired-operation semantic cross-check and environment limitations are
recorded in [SDK distance numerics](FIELD_PARTNER_SDK_DISTANCE_NUMERICS.md).

PSVECDotProduct0x8029ec84,size0x20 first loads paired(Y,Z) from each vector
and multiplies to form(R(Ay*By),R(Az*Bz)). It then loads(X,Y) and performs
paired fused multiply-add;lane0 is F(Ax,Bx,R(Ay*By)). ps_sum0 adds the previous
Z product to that lane0. Returned lane0 is therefore the ordinary positive
dot R(F(Ax,Bx,R(Ay*By))+R(Az*Bz)),not its negation. The other lane is not
returned as the dot. Preserve paired precision/FPSCR behavior for bit parity.

PSVECScale0x8029ebc8,size0x1c loads XY together,scales both by f1 lane0 and
stores XY;then scales the separately loaded Z and stores only Z. In-place
scaling is used by both correction paths. PSVECAdd0x8029eb80,size0x24 loads
both XY pairs,adds/stores them,then loads/adds/stores Z. Source V and output V
alias in the caller;this order supports the actual alias without a temporary
copy of the complete vector. These helpers write Y too;the routine's ordinary
finite Y remains unchanged because its pushes have zero Y.

PSVECMag0x8029ec40,size0x44 loads XY,squares them,loads Z,then paired-madds
Z² onto the X² lane. ps_sum0 produces q=R(F(Z,Z,R(X²))+R(Y²)). Derive zero
from half-half and return q immediately when fcmpu says equal. Otherwise
frsqrte(q),one single-result refinement with half0.5 at0x80429394 and3.0 at
0x80429398,and multiply q by the refined estimate. The refinement is the
same operation sequence described for SDK distance,with this different
squared-sum order. There is no three-double-Newton or ordered-positive domain
classification in this leaf. Native hypot/sqrt alone is not bit-equivalence
evidence.

## Validation and boundaries

tools/verify_field_partner_around.py pins seven complete local body hashes,
selected independently transcribed instructions,constants,zero vectors and
conversion bias;it also checks the actual stay call and the already recovered
lecture callback imports that reach ordinary partner updates. Metadata checks
detect binary/source revision drift;they do not execute original movement.

Keep full hit backend/triangle and scene traversal,filter-specific policies,
global state/view/width/height writers,angle/trig and GQR/FPSCR/frsqrte domains,
floor/fly/homing/fall/jump services and original collision trajectories open.
This closes a used partner binding and shared local correction graph,not the
whole partner or field engine. Complete-game documentation and requirements
remain incomplete.
