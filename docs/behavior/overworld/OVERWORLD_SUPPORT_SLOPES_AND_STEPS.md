# Ordinary support, slope-vector reduction and upstairs ownership

US G8ME01 disc0 revision0, recovered2026-10-12 from read-only local ISO.
This is static source control/data flow, not gameplay observation or numerical
comparison. Verify11 containing bodies,111 instructions and22 data spans with
`python3 tools/verify_overworld_support_and_steps.py`. Large-body fingerprints
pin containing evidence and do not establish all special-mode branches.

The supported slice is ordinary Mario motions0/1/2/12, normal input/poses,
scale1, no roll/Paper/external movement, no moving platforms/special floor
attributes/Jabara/Vivian/Koopa or motion locks. Direction gain is separately
specified in [the rate contract](OVERWORLD_DIRECTION_MOVEMENT_RATE.md).
Full directional wall movement and ray/filter ownership are in
[the wall contract](OVERWORLD_WALL_MOVEMENT.md). No partner function was included.

## Independent owner state

| Mario field | Role in these bodies |
| --- | --- |
| +7c/+80/+84/+88 | Retained vertical tuple; +7c is input to support query even in ground states |
| +90 | Actual rootY, separate from query outputs |
| +180/+1a4 | Retained base horizontal speed and travel heading |
| +19c | Camera-owned heading offset; not floor slope |
| +1c4 | Selected support height rounded additionally to.01 original units |
| +1c8 | Stored upstairs targetY |
| +1e8 | Current support node token |
| +218 | Overall inclination angle in degrees |
| +21c/+220/+224 | Signed projected Z-tilt, explicit0, signed projected X-tilt angles |
| +50 | Signed counter; ordinary ground/upstairs domain0 |
| +44/+48 | Motion phase and countdown; upstairs entry0/3 |
| Main flags0x00040000/0x80 | Upstairs active and current-input drift suppression |
| Main flag0x00010000 | Jump gate also checked by grounded support-loss tail |

The four-angle slope record is NOT an XYZ normal, nor solely the normal of the
highest accepted support. Both SearchGround and FrontStep clear it on entry;
queries overwrite it in their source order before vertical candidate acceptance.
Current root and node, support metadata height, forward target and slope record
must remain independently owned.

## Movement and query order

Normal marioMain first dispatches motion, then ordinary ground path calls
marioMove80059a78 (0x8005b0cc), overhead8008c904 (0x8005b0d0), then
marioChkGnd8008f248 (0x8005b0d4). marioMove calls marioMoveMain800594a8,
which uses the PREVIOUS stored slope record before the wall solver.

After full horizontal movement, marioChkGnd:

1. Clears support node+1e8 at8008f500, performs its separate overhead/availability
   queries and ordinary eligibility gates, then calls SearchGround with depth11,
   current vertical tuple+7c and three output destinations at8008f838–84c.
2. If a support node is returned, writes+1e8 and computes support metadata+1c4
   as `float(fctiwz(fma(100, returnedHeight, .5)))/100` (8008f858–898).
   This uses fixed+.5 even for negative height. It does not replace the query's
   returned height or actual rootY yet. Special surface handlers precede stairs.
3. With signed counter+50==0, calls FrontStep8008926c at8008f9e8 while rootY
   STILL has its pre-support value. This call can replace the slope record made
   by the five-foot query. It runs even if the five-foot query found no node.
4. Accepts upstairs when forward node exists, rise=forwardHeight-currentRootY
   lies inclusively5..11 original units, overall+218<=30 and retained base+180
   is nonzero. Store target+1c8 first; request motion12 at8008fa58, then return
   without floor snap (8008fa34–a5c). Base0 can still store a candidate target
   but does not enter motion12.
5. Otherwise write ACTUAL rootY from the SearchGround returned-height output
   at8008fae8. This remains the query's.001-unit rounded pointY, not +1c4's
   extra.01-unit metadata rounding. If node is null, evaluate support-loss tail.

The original ordinary path has no XZ endpoint rejection based on a broad center
floor rise>=5 before this query composition. Walls and these foot/front services
own displacement and stair admission. A native coarse tall-ledge guard is a
provisional additional rule, not established by these source bodies.

## Normal five-foot SearchGround service

`marioSearchGround`8008e694 clears slope record, initializes outputHeight=rootY,
outputDelta=-1, and queries five rotated center/cardinal probes in source order.
Ordinary nonzero base uses revise360(travel+1a4-headingOffset+19c); zero base
uses converted facing+1ac. Radius=.375*baseWidth (normally7.5 original units).
For unit(uX,uZ), ordered offsets are(0,0),r*(uX,uZ),r*(-uZ,uX),
r*(uZ,-uX),r*(-uX,-uZ); keep this order for equal-height ownership.

For depthD and vertical inputV, upward origin offset is5+abs(V), downward ray
length is that offset+abs(D)+.1. For D11,V0 this scans rootY+5 down through
rootY-11.1. Each ray first selects nearest admitted solid source triangle via
normal chkfilterVec/face policy, then locally rejects inclination>=50 in this
plain domain. It does not search a second lower surface after nearest slope
rejection. Query point and normal components are rounded separately to.001
original units using sign-aware Q1000; slope angles derive from that normal.

Let F=that ray's quantized pointY,R=captured rootY,d=R-F. Plain candidate flow:

- If F<R and d>=2, then d>abs(V) writes outputDelta=d and skips admitting this
  ray as support. If d<=abs(V), the ray can join normal highest-height selection.
- Other vertical cases (including drops strictly<2) require abs(F-R)<=11.
- Admitted F must be strictly higher than the current winning height; equal
  heights retain the earlier accepted probe. Admitting sets outputHeight=F,
  outputDelta=0, outputAngle=angleABf(rootXZ,pointXZ) and winning node.
- A later non-admitted lower ray can overwrite outputDelta even if an earlier
  node remains the winner. Outputs therefore need ordered independent writes;
  no local all-probe sum or nearest-center fallback is equivalent.

Initial best-height sentinel=-10000 original units (80420968), not an invented
host negative infinity. Output node0 does not necessarily mean no ray hit:
height/slope/distance rejection can leave a positive delta and angle-record
writes. The geometric service/output aliasing and other states' conditional
bounds remain separate full-service recovery requirements.

With V0, a0.01-native drop (1 engine unit) is admissible support; a0.03-native
lower hit (3 engine units) is a positive-delta fall candidate. After stairs V
can be positive; do not force it to0 merely because the actor is ground state.
These are static branch examples, not measured original traversal.

## Forward-step query and slope record

`marioChkFrontStep`8008926c clears slope record, initializes outputHeight=rootY
and outputDelta=-1. Ordinary probe uses travel+1a4 if base!=0, converted facing
otherwise. It rounds heading to whole degrees using fixed+.5 and fctiwz, then
revise360 (80089324–354). It does not subtract+19c in this ordinary nonzero-base
path. Forward distance=.265*baseWidth, normally5.2999997 original units.
Origin is forwardXZ, rootY+.5*bodyHeight; downward length=.5*bodyHeight+11+.1.
Normal bodyHeight37 gives origin+18.5 and reach to rootY-11.1.

A successful ray point/normal is quantized Q1000. It computes projected and
overall slope angles, then supplies forward height/delta/angle outputs. Raw
FrontStep has no ordinary50-degree cone rejection; its caller's30-degree stair
gate is separate. A steep successful forward hit can still update stored slope
angles even though it is not a staircase. For a hit below root by>=4 it writes
positive outputDelta rather than a raised floor candidate; the ordinary stairs
caller still tests returned height minus root and cannot accept a downward rise.

For normal components(nx,ny,nz), define A(a,b)=original
angleABf(0,0,a,b), and Q100plus(x)=float(fctiwz(fma(100,x,.5)))/100.
The local normal-to-record map is:

```
Ztilt(+21c) = Q100plus(A(single(100*nz),single(100*ny))-180)
zero(+220)  = 0
Xtilt(+224) = Q100plus(A(single(100*nx),single(100*ny))-180)
incl(+218)  = A(original_sqrt(single(nx*nx + nz*nz)), -ny)
```

The source uses separate products/adds/FMA and original square-root construction;
this displayed map identifies dependencies, not host numerical equivalence.
Signed projected angles use fixed+.5; negative values can round toward0 differently
from symmetric rounding. incl is the angle helper's float, not Q100plus.
SearchGround writes the projected fields+21c/+220/+224 before its50-degree
cone rejection (8008ebe4/8008ebe8/8008ec4c precede8008ed34–8008ed48).
It writes overall+218 only AFTER surviving that cone, at8008ef20, before
vertical admission. A steep rejected ray can therefore overwrite projected
fields while leaving overall+218 from an earlier survivor or the initial0; it can leave the record from a later rejected/nonwinning
ray. FrontStep then resets/overwrites that record on the normal ground path.
Do not simply attach winning-floor normals to these shared fields at pass end.

## Returned floor-normal orientation

The downward ray owner returns a normalized normal facing against the ray,
including double-sided cull3 triangles hit from their back side. It does not
unconditionally return the source winding normal. HitQuery+24 is the hit point;
+30 is the returned normal. SearchGround uses those outputs separately.

In hitCheckVecFilter800158a4's optimized direction(0,-1,0) path,
80015b34–80015b54 computes signed plane distance
`p=dot(windingNormal,origin-triangleOrigin)` in f31. The double-sided path
permits either winding. At80015d20/80015d4c/80015d50, ordered p>=0 takes the
copy at80015d54–80015d68; negative p instead calls PSVECScale(normal,-1,
query+30) at80015d70–80015d7c. Thus a valid downward back-side hit flips a
downward winding normal upward before slope construction. Final80016528–80016568
normalizes the saved nearest-hit normal with PSVECNormalize8029ebe4 and returns
it in+30. General oblique rays use the same signed-plane-distance orientation
at80016460–80016494. Keep face admission and returned-normal orientation as
separate operations; cull1 downward rays reject normalY<=0 at80015b20–80015b30.

This is a source-derived orientation contract, not a claim that a host cross
product, intersection routine, or normalization matches Gekko arithmetic.

## Slope-vector reduction before the direction-rate service

In marioMoveMain, combine retained travel and current-input drift first. Unless
mainflags&0x02080000, apply these independent vector-axis gates:

```
c = single(original_cos(single(single(2*pi*incl)/360)))
if Xtilt(+224) != 0: combinedX = single(combinedX*c)
if Ztilt(+21c) != 0: combinedZ = single(combinedZ*c)
```

The source queries cosine separately for each nonzero gate (800597ac–804),
then adds external deltas (excluded here), computes heading/norm, applies air
cap, applies [direction gain](OVERWORLD_DIRECTION_MOVEMENT_RATE.md), then calls
wall movement. Overall inclination is used for both enabled axes, not each
projected tilt's own cosine. An inclined plane with normalX nonzero/normalZ0
reduces X only; the orthogonal Z component stays intact. A plane with both
components nonzero reduces both enabled components by cos(overall).

Slope state can be stale from the previous query pass, deliberately. Pending
upstairs keeps the front-query slope record through its active ticks because
ordinary ground queries are bypassed while its main0x00040000 flag is present.
It is not a new query against the destination point before each XZ displacement.

## Missing support distinguishes fall10 from fall11

When no node is retained and ordinary jump/roll gates permit the tail:
outputDelta<0 requests10+SetFallPara at8008ff18–f30;
outputDelta>0 requests11+SetFallPara at8008ff34–f50;
zero does not request either. The request is late in marioMain; there is no
same-call second marioMotion invocation. The following manager call enters fall.

All probes miss leaves delta-1 and selects10. A recognized lower surface>=2
engine units below root with V0 can leave positive delta and selects11.
Both table rows use mot_fall, but motion11 entry sets mainflag0x80 at8009b008–010;
motion10 does not. This flag suppresses current-stick drift in marioMoveMain
(80059634–63c). Falling toward a known lower floor and falling through unknown
support therefore have distinct horizontal input policy. Both retain base/travel;
variant10 additionally has a sound-cleanup post callback, variant11 has none.
Collapsing both into a boolean falling/motion10 discards original behavior.

## Upstairs entry, three rises and completion dispatch

The source table row803100c0 resolves motion12 to mot_upstairs8009b158.
Accepted-step request occurs AFTER the current call's XZ movement; the next
manager call begins entry. It consumes pending bit0, sets mainflags0x00040000
and0x80, calls MarioClearJumpPara, sets countdown3, computes
velocity+7c=(target+1c8-actualRootY)/3, and phase0. It retains base+180,
travel+1a4 and support node+1e8; it does not clear support to a fabricated null.

Same entry invocation adds velocity to actual root and decrements3→2.
Next invocation adds velocity and decrements2→1. Both use retained base/travel
for subsequent horizontal movement; current stick is queried globally but the
upstairs callback does not walk/dash-select it, and flag0x80 blocks input drift.

Third invocation adds the final velocity, decrements1→0, clears completion
flags, and calls marioChgMotAuto800a6bbc at8009b268. The dispatcher then processes
the new idle/walk/dash entry using CURRENT input before horizontal movement.
A newly admitted A trigger on this third call can launch through the plain
[ground arbitration paths](OVERWORLD_MOTION_ARBITRATION.md). Earlier upstairs
A triggers were not sampled/buffered by this leaf. Ground overhead/support checks
can resume in that same completed invocation; do not use a blanket wasStepping
flag to skip them after completion.

Upstairs DOES NOT clear velocity+7c on completion. Auto plus walk/dash entry also
do not locally clear the tuple; idle entry does. Thus current nonzero ground input
after stairs can reach SearchGround with retained positive V. Preserve vertical
owner state independently of step countdown/active state. Complete external
vertical writers and platform/pose services remain dependencies.

## Native acceptance targets and remaining scope

Native bindings should retain separate source support/query/target/slope outputs,
Q1000 point/normal versus Q100 metadata, previous-angle axis reduction, current
direction gain, and pending motion transitions. Useful source-derived fixtures:

- Flat support with X-only/Z-only/diagonal direction gain and zero tilt gates.
- Inclined X plane reduces X while Z stays intact; projected-angle0 gates.
- Foot support below2 versus recognized larger drop versus all-ray miss;
  distinct10/11 drift policy and no invented landing/node fallback.
- Five-foot selection before forward target, root snap deferred on accepted step,
  thresholds5/11 and30 inclusive, base0 does not start an upstairs sequence.
- Three manager rises: first two retain travel, third reclassifies input and
  resumes ground queries, third-call A admitted, retained velocity after completion.

Original math/intersection/traversal/aliasing, full signed-delta exceptional cases,
special attributes/modes, moving platforms/root matrices, hazard/pose/sound,
actual SDK/camera input owners and source runtime comparison remain unfinished.
These local contracts support concrete player-visible fixes; they do not close
all field physics or prove the prototype faithfully reproduces original geometry
and timing on hardware.
