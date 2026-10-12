# NULL-filter Bero downward ray acceptance

US G8ME01 revision0; source-only recovered2026-10-12. Verify
`python3 tools/verify_overworld_road_null_ray.py`. This pins complete owners and
literal acceptance/order sites, not original execution or native numeric parity.
Read [Bero placement](OVERWORLD_ROAD_POSITION_AND_CONTACT.md) and
[wall ray policy](OVERWORLD_WALL_MOVEMENT.md).

## NULL skips a callback; the active runtime domain still applies

bero_get_position_normal uses hitCheckFilter(NULL,pointOutputs,lengthOutputs,
normalOutputs,originP,dir(0,-1,0)) with length1,000,000 original units. The scalar
wrapper8001577c builds the query and invokes hitCheckVecFilter800158a4. A null
hit returns without changing caller output values; Bero keeps its centerY.

hitCheckVecFilter starts from the runtime active HitObj chain at r13+1708,
following each object's +e0. It does not scan all source map triangles directly.
`hitMain`80018264 rebuilds this chain before EVT/Mario in the field manager:

- Source map node+5c (geometry part count) must be nonzero (800182a4–ac).
- Runtime **HitObj halfword+0 flags &0x81 must equal0** (800182b0–b8).
  Thus runtime bits1 and80 exclude an object before any NULL callback decision.
- Eligible objects append in map-entry order and then HitObj-array order;
  final eligible object's +e0 is setnull (800182c4–8001830c).

These are runtime flags, **not** source draw attributes. `_hitEnt` initializes
HitObj flags0; event/map runtime services may change them later. Disabled runtime
nodes cannot be inferred from attr1/4 alone. Part-free hierarchy nodes never
supply triangle candidates, even if they have a center or attribute value.

At80015aa0–c0 the downward query calls the supplied callback only if nonnull.
For NULL it proceeds directly to broad-phase. There is **no default attribute
predicate** for source draw attr1, attr4, collision sensor labels, Mario masks,
or road-registration attr00800000 in this branch. Importing chkfilterVec or a
Mario support filter would add a rule absent from this NULL query. Conversely
scanning disabled objects merely because their triangles are packed would omit
the active-chain rule.

## Sphere, cull and triangle rules

The bounded domain here is finite ordinary source geometry, positive lengthL,
exact unit downward direction. Source computes fixed halfLength=.5*L and
segment midpoint M=P+(0,-1,0)*halfLength. For each active object it rejects only
when PSVECDistance(M,HitObj.worldCenter+c0) > halfLength+HitObj.radius+cc
(80015ac4–dc). This broad-phase is a sphere, not an axis-aligned bbox test.
Equality proceeds. M and halfLength are computed from the initial input length;
nearest-hit updates do not recompute them for each later object.

`_hitEnt` stores local bbox midpoint before expanding source bbox min/max by
.1 original units. Its world sphere radius is distance(world-transformed expanded
bboxMax,worldCenter), at80018b70–88. Runtime transform/binding owners can update
center/radius/triangle buffers. Packed static geometry alone does not recover
all event-driven object flags, matrices or sphere state.

The node draw cull byte exactly1 enables one-sided behavior; every other value
takes two-sided behavior in this query. The source scans signed16 triangle
count+A8, buffer+AC, stride0x54 in order. Let verticesV0/V1/V2 be runtime triangle
fields+0/+c/+18, edgesE0/E1/E2 fields+24/+30/+3c, and N field+48. Source
hitCalcVtxPosition constructs E0=V2-V0, E1=V0-V1, E2=V1-V2 and
N=cross(E0,E1), with the actual packed vertex/strip decode and world matrix.
For each i define Di=P-Vi and Ki=Di.z*Ei.x-Di.x*Ei.z; let Q=dot(N,D0).

For one-sided downward candidates (80015b14–c10):

1. Reject N.y<=0.
2. Reject Q<0.
3. Reject any Ki<0; edge equality is admitted.

For two-sided candidates (80015c14–cdc):

1. Reject N.y*Q<=0. In particular coplanar Q==0 is rejected, unlike a valid
   upward one-sided plane at Q==0.
2. Reject any Q*Ki<0; edge equality is admitted after the plane test.

Remaining candidate distance is T=Q/N.y, equivalently -Q/(-N.y). For a
nonnegative current query length, reject **T>=currentLength** (80015ce0–d14).
The positive Bero length therefore admits [0,L) in its one-sided domain, and
positiveT<L in the two-sided domain. It does not admit a hit exactly at the
far endpoint or use a native barycentric epsilon. Source then writes query
length=T and point=(P.x,P.y-T,P.z). Returned normal is N when Q>=0 and -N
otherwise; the winner normal is normalized at80016528–30 before returning.
No Q1000 rounding occurs in this backend/scalar caller.

The winning object updates only for the first admitted hit or a strictly
smaller distance (80015d8c–da4). Equal-distance ties retain the earlier source
object/triangle. Traversal does not require an upward normal for all cull modes,
reject source attr1 by default, or discard steep surfaces using a walkable cone.
The geometric finite-domain tests above preserve source admission; instruction
rounding/FMA/paired-single math remain numerical adaptation boundaries.

## Station-return debugging boundary

An arrivalY below the road marker is not itself proof of a missing NULL attribute
filter: the source ray intentionally samples below the adjusted marker point and
can return a surface that a later Mario support filter treats differently. A
native result must first be checked against the exact source center/direction,
source-unit scale, active node flags, current transforms, cull/edge/length tests
and source-order winner. The preceding placement contract retains markerY on a
miss; it does not clamp a returned floor to a convenient road height.

## Authored-map units become engine units under a ×10 parent matrix

`_mapLoad`80028f18 builds the initial parent matrix with source scale10 on all
three axes. At800294dc it reads r2+signed8504, address8041f944, IEEE32 word
41200000 (10.0), passes that value in f1/f2/f3, and calls PSMTXScale8029e2b0
at800294ec into stack+90. It then passes this **same** parent matrix to
mapEntry at800294fc and hitEntry at80029514. hitEntry passes its matrix argument
to hitEntrySub; `_hitEnt` concatenates parent*local at80018a84–90, propagating
that world matrix through the hierarchy and transformed center/triangle owner.
Thus source authored-map hierarchy coordinates must be multiplied by10 to
obtain the initial runtime engine coordinates, before later event transforms.

The native converter's map scale.1 therefore corresponds to authoredMap*10
engine units*.01 native units. These are two distinct conversion stages; using
.01 directly on authored-map data would omit the runtime parent10. This closes
the ordinary initial rendered-map/collision root scale, not every event-driven
map-object/animation transform or numerical rounding equivalence.

For gor_03/n_bero, a static original hierarchy/bbox calculation gives an authored
center approximately(0,4,-60); with the source parent10 the initial runtime
center is approximately(0,40,-600) engine units. Ordinary arrival mode1 adjusts
its start by **30 engine units**, so approximate P=(0,40,-630), corresponding
to native(0,.4,-6.3) at engine scale.01. Do not subtract30 authored-map units
or omit the parent10. This calculation identifies the unit mapping, not an
original game trace or proof of the enabled floor/winner for a particular
station-return runtime state. Original single-precision matrix math and live
object overlays remain the numeric/state boundary.
