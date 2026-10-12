# Ordinary Mario wall movement and directional sliding

US G8ME01 disc0 revision0, recovered2026-10-12 from read-only local ISO.
Static local behavior, not original execution, measured movement or numerical
comparison. Verify with `python3 tools/verify_overworld_wall_movement.py`:
nine containing bodies,48 instruction anchors,14 scalar constants and three
original filter strings. Fingerprints of hitCheckVecFilter and initialization
pin containing evidence, not a complete recovery of those large bodies.

## Semantic interface and scope

The ordinary local solver consumes mutable root position P, intended nonnegative
horizontal speed S, heading H in original degrees, current width W=Mario+1c0,
body height T=+1bc, signed motion counter C=+50, motion ID, Mario flags+0,
and view flag word+14. Retain independent contact tokens and counters; these
must not be replaced by a single blocked boolean. Ordinary initialization sets
base/current width20 and body height37; callers reload current dimensions.

Provide a directional 3D ray query taking origin, unit direction and maximum
length, returning optional node token, distance and original normal. Collision
attributes, original node names, cull selector and source query iteration order
belong to that provider. Return null for no admitted hit, rather than fabricating
a zero-distance wall. The local movement solver operates on whole intended XZ
vectors; splitting X then Z changes the documented response on oblique walls.

The plain domain excludes Vivian status2, transformations/motion26/28/25,
roll flag0x01000000, map-specific movement bypass, dynamic NPC hidden-block
ownership, balloons and the dou10_yoko_yari hazards. Special branches are explicit
service dependencies below. Ordinary jump/fall remain supported; C!=0 adds one
forward height and falling motions affect the around-pass correction cap.

## Ray provider and filter boundary

Original HitQuery input origin is record+0xc, direction+0x18, maximum length+0x3c;
output hit point+0x24 and normal+0x30 accompany rewritten distance+0x3c.
ID1 corrected these previously reversed labels2026-10-12:the backend computes
origin plus scaled direction into+0x24 at0x80016454/58,and copies/normalizes
the triangle normal into+0x30 at0x80016474..80/0x80016530..68.
[Partner ray contract](FIELD_PARTNER_FRONT_WALL_QUERY.md) pins these anchors.
`hitCheckVecFilter`0x800158a4 returns the retained node token. It calls the filter
on each candidate node; return0 rejects, nonzero admits (0x80016190–198).
The general oblique ray path performs triangle geometry checks and strict nearer
replacement. With cull selector byte1, dot(original triangle normal,direction)>=0
rejects (0x800161c4–16210; `cror2,1,2` at0x8001620c). Other cull selectors do not take this rejection path.
Keep the original hitMesh normal convention: blindly reversing normals changes
both culling and source ownership. Rendering cull alone is not evidence that a
host ray implementation reproduces original numerics.

Ordinary `chkfilterVec`0x800915e4 applies ordered attribute rules:

1. If attr&0x00080080, admit immediately.
2. If attr&0x00800000 is clear and attr&4 is set, reject.
3. If attr&0x80000000 is clear, admit.
4. Otherwise resolve the NPC owner and reject these exact owner-name strings:
   MOBJ_HiddenHatenaBlock, MOBJ_HiddenBadgeBlock, MOBJ_Hidden10CountBlock.
   Other owner names admit. Owner/NPC resolution is a real dependency.

Motion28 chooses chkfilterVecVivian; roll flag0x01000000 chooses chkfilterVecRoll;
ordinary selects chkfilterVec. No ordinary local rule limits queries to triangles
with nearly vertical normals. Legacy `.walls` conversion keeps only
abs(normalY)/norm<=.2 and loses cull metadata, so it is insufficient as the full
ray-query source. A complete source triangle pack with attributes/cull and a host
3D intersection implementation is an explicit numerical/domain adaptation until
compared against the original triangle-query service and iteration order.

## Quantization

Each successful local ray distance is rounded to0.001 original units by:
`Q(x)=float(trunc(fma(1000,x,x>=0 ? .5 : -.5)))/1000`.
The source uses fctiwz, signed integer conversion back to float, then division;
exceptional conversion domains remain original numerical-service dependencies.
Forward hits also quantize all three returned normal components separately using
Q before storing/selecting the normal. The local solver does not renormalize this
quantized normal. Root position itself is not rounded by these local wrappers.

Constants are pinned at0x804208a8=1000,804208cc=.5,804208d0=-.5,
804208bc=45,804208d4=180,804208c8=360,804208d8=.125,804208dc=.25,
804208b4=11,804208ec=.1,8042092c=26.6400013. Original scale0.01 requires performing
source quantization before scaling; quantizing native distance directly to.001
would make its world step100 times too coarse.

## Forward search and slide

`searchFrontWall`0x8008a2f0 receives mutable local P, float output destination,
S,H,W. Ordinary radius r=single(.5*W), forward maximum L=single(r+S).
Heights in array order are[26.6400013,11] when C==0, otherwise
[26.6400013,11,.1]. Each origin is(P.x,P.y+height,P.z) along unit heading H.
Do not substitute body height37 for these heights.

Query all forward heights. On a hit quantize distance and normal, then retain
first hit or replace only if Qdistance is strictly less than the retained
Qdistance (0x8008a800–850). Equal/unordered distances retain the earlier hit.
A balloon classifier is called for each successful forward query; true stores
that node into Mario+210 even if it is not the closest retained normal.

If a forward hit exists, form intended d=S*u(H), compute
`dot=single(d.z*n.z)` then fused `dot=fma(d.x,n.x,dot)`, and compute
`slide.x=.5*fnmsub(dot,n.x,d.x)`, `slide.z=.5*fnmsub(dot,n.z,d.z)`
(0x8008a86c–89c). This is half the tangential component of the whole displacement;
Qdistance does not move P to the wall before sliding. Keep original FMA/rounding
services explicit when claiming numerical equivalence.

Then perform side guards from the ORIGINAL P, radius length r and the same
height array, heading H+45 first (0x8008a8b0): stop this pass at its first hit.
If that pass found any hit, perform H-45 (0x8008aa48). The second side pass queries
EVERY height and overwrites its retained hit token on EVERY iteration, including
null (0x8008ab80–b90). Therefore its final-height hit/null alone controls the final
side guard. Do not rewrite this as an any-hit OR or nearest-hit aggregation.

Final tail (0x8008ab94–bfc):

| Forward token | Final side token | Output float | Position effect | Return |
| --- | --- | --- | --- | --- |
| Nonzero | Null | Last side distance temporary | P.xz += slide | Forward token |
| Nonzero | Nonzero | 0 | Retain P | Forward token |
| Null | Null | Untouched | P.xz += full d, using fused axis adds | Null |
| Null | Nonzero | Untouched | Retain P | Null |

The successful-slide output float is the side-query distance temporary, not
forward Qdistance or slide length. If a failed side query leaves distance
unchanged, that unchanged radius is the output. Provider writes on failed queries
must preserve original contract. A returned forward hit and zero slide differ
from no hit; keep those observations separate.

## Wrapper ownership and pass-through classification

`marioMoveMain` ordinary movement calls `marioCheckWall`0x8008b790 with computed
norm and combined heading at0x800599e4 (other explicit flag routes also call it).
The wrapper initializes Mario+214=1000 and maintains independent direction
fallback metadata: with S!=0 and C==0, a new larger S stores S/H into+1cc/+1d0
and countdown+1d4=14; otherwise countdown decrements, clamps at0 and eventually
replaces stored S/H. It copies retained+1d0 to+1a0. Other paths reset+1cc=0.
These metadata writes do not change the current S/H displacement.

For S!=0, copy current position into local P, clear balloon token+210, call
searchFrontWall(P,&temporary,S,H,currentW). If a forward token exists, contact+1e4
is balloon+210 if nonzero, otherwise forward token. Read forward node attributes
and apply this separate ordered pass-through classifier:

```
if ((attr & 0x00080000) && !(MarioFlags & 0x01000000)) pass = false;
else if (!(attr & 0x80)) pass = (attr & 0x00800000) != 0;
else pass = (motion == 25);
```

When pass is true, ignore local P and move ACTUAL root by full original S/H;
store the node in+1f0/+1f4 when node!=null. For a blocking hit store temporary
into+214, contact node+1f4, update signed-byte wall counter+3f (threshold10,
or20 for source node-name prefix checked by the wrapper), then copy local P
back to actual root. No-hit also copies local P. Additional conditional contact
+1f0 writes use source wall attributes/motion25; they do not grant ordinary
movement through a blocker.

After either moving branch, call `marioChkWallAround` at0x8008ba04 with ACTUAL
root, mode0, original S/H,currentW and currentT. Wrapper ignores its return.
Then call dou10_yoko_yari at0x8008ba0c with &Mario+214. This map/hazard service
must remain a dependency, not be silently treated as recovered generic physics.

For S==0 the wrapper instead calls `marioSearchFrontWall_staying`0x8008b4d8,
then dou10_yoko_yari, and skips Around. Staying clears wall counter+3f, uses
probe speed=.5*baseWidth (Paper mode zero), calls searchFrontWall on a disposable
position using travel-heading minus+19c; if no hit it queries converted facing.
It does not copy disposable position to actual root. A map/Vivian-specific third
query/contact stage remains separate. Thus zero input can refresh contacts but
must not slide or apply the ordinary moving-around correction.

## Around correction: two directional passes

`marioChkWallAround`0x800888d0 ignores its float S argument for correction sizing.
Let N=8 when Mario+14 bit0 is clear, else N=2, as established by complete
kpaGetStageViewType0x801feeb8. Bind the view flag owner; its later writers are not
recovered by this contract. Directions are H+k*(360/N),k=0..N-1 in that order.
Radius is r=.5*W. At each angle cast two rays from the same captured root XZ:
first Y+11, then Y+(T-1), both length r. Quantize successful distances with Q.
Apply the wrapper's ordered pass-through classifier independently to each hit,
and discard pass-through hits before choosing the nearest of the two distances.
If both blockers exist, upper replaces lower only if its Qdistance is strictly
less; ties retain lower.

Start correction V at zero. For each retained blocker add
`abs(r-Qdistance)*u(rayAngle+180)` to V using fused axis additions. The ray
origin stays fixed across all angles in this pass; do not query from each partial
correction. If no angle retained a blocker, return0 and do not move root.

Otherwise save the UNCLAMPED first correction, then cap its norm to K=max(1,.125*W). With mode0 and
motion10 or11, K=max(1,.25*W); mode!=0 retains .125 regardless of falling motion.
Scale V when norm>K, then add first capped V to actual root. The separate saved
first V remains unclamped (copy at0x80088db4–bc,scale at0x80088dd8).
Query a SECOND identical directional pass, captured from
this newly corrected root, with a fresh zero correction accumulator.

If no second blockers exist, return1. Otherwise compute dot(savedFirstV,secondV).
If ordered dot>=0 return1 without second correction. Otherwise (negative or
unordered,assuming floating-point exceptions permit continuation),scale second
V by.5,add it to actual root,return1 (0x800891d8–214). The comparison is
`fcmpo; cror 2,1,2; bf 2`,which tests greater OR equal before skipping.
There is no second magnitude cap. Earlier versions reversed this predicate
and incorrectly described the saved first V as capped;see the
[partner binding and numeric contract](FIELD_PARTNER_SURROUNDING_WALL.md).
Do not mistake return1 for a request to reject/revert the already applied root.

## Remaining boundaries

Full numerical hit-driver triangle/scene traversal, dynamic NPC identity,
filter-specific special modes, balloon/contact field consumers, view-flag writers,
map bypass, stationary map branch and hazard helper execution remain dependencies.
No original runtime collision traces have been recorded here. Plain native
geometry/ray adaptation should verify oblique sliding, head-on rejection,
ordered side-tail behavior, quantization near boundaries, cull/attribute rules,
forward vs around heights,8/2 directions, fall cap and two-pass position ownership.
Keep legacy endpoint circles for unrelated eligibility queries only if their
provisional behavior is clearly separated from original player displacement.

## Original heading convention

The called `sincosf`0x80005e8c outputs X=sin(H*pi/180) first, then
Z=-cos(H*pi/180): sine request0x80005ec4/store0x80005ed0, cosine request
0x80005ed4, explicit negation0x80005edc/store0x80005ee0. Thus rotating an
existing XZ unit direction by positive a for H+a gives
`(x*cos(a)-z*sin(a), x*sin(a)+z*cos(a))`. Use this convention for side guards,
around directions and opposite rays. It is not the positive-cosine convention.
Original trigonometric services still remain numerical-equivalence dependencies.
