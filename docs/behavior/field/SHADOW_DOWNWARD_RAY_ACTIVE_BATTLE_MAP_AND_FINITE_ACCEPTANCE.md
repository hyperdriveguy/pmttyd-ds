# Shadow downward ray: active battle map and finite acceptance

US G8ME01 disc0 revision0, read-only ISO recovery 2026-10-14, from EVT-01's
independent shadow-preparation provider frontier. This contracts the exact
finite direction(0,-1,0), positive-length branch of hitCheckVecFilter800158a4,
shadow filter8002d380 and source map/context producers. It does not establish
whole arbitrary-ray numeric equivalence or the current original game's live
state. Ignored full listings: artifacts/reference/display-class-check/.

Related: [shadow captures/stores](../display/SHADOW_COLLISION_PREPARATION_CAPTURE_AND_STORE_ORDER.md),
[active HitObj registry](DYNAMIC_OBJECT_HIT_REGISTRY_AND_RAY_ORDER.md),
[NULL downward geometric branch](../overworld/OVERWORLD_ROAD_NULL_DOWNWARD_RAY.md),
[source outer-loop order](../platform/GAME_OUTER_LOOP_AND_EVENT_UPDATE_ORDER.md),
[setup4 stage identity](../battle/UNDERGROUND_SETUP4_INTRO_PHASE_STAGE_AND_PLAYER_COMMANDS.md).
No Dolphin, emulator, hardware or original runtime observation is claimed.

## Context is the published HitObj chain, not a floor snapshot

hitCheckVecFilter calls mapGetWork8002b438 once at800158e4, but does not retain
or subsequently read its return value. The actual traversal starts from head
8041e628 (r13+1708), captured at80015940 after initial query math. It follows
each selected object's fresh next pointer+e0. There is no per-ray GP14 test,
native scene selection, packed battle-floor lookup or source stage-name lookup.
The chain must already have been published by its real producer.

mapGetWork returns global base8041e660 plus signed current index8041e664*2f4.
The source has independently selected field/battle map contexts; this index is
not inferred from GP14 inside the ray. Fresh bmapLoad8002892c/17c:

1. sysWaitDrawSync; capture map base; publish index0 at80028950. Its field
   buffer-preservation/free work runs against this context.
2. Publish index1 at80028a44. Clear battle MapWork(base+2f4,0,2f4).
3. _mapLoad(battleWork,mapIndex0,primaryName) at80028a68; publish count1.
4. If captured secondaryName nonnull, _mapLoad(battleWork,mapIndex1,secondaryName)
   at80028a88; publish count2. Leave selected context index1.

seq_battleMain's rank0 state3 calls bmapLoad at80073d48 with literal primary
name802c2ac4=bti_01 and selected stage.word4 as secondaryName. Original setup4
stage selects stg_00_1, as pinned by the linked setup4 contract. GP14's battle
publication is a separate earlier battle_init operation; it is not the map
selector. _mapLoad creates each collision hierarchy through hitEntry with
the same source Scale10 parent used for mapEntry/rendered geometry.

marioStMain calls mapMain80026bb8 at8006f94c, hitMain80018264 at8006f950,
then evtmgrMain. These calls occur in the global outer loop, including battle;
they are not restricted by the caller to a field-only root. hitMain calls
mapGetWork twice, captures the first result for later binding work and uses
the second result for active-chain rebuilding. It clears head then scans
selected context's map entries in ascending order, within each its HitObj
slots in ascending order. Geometry part countnode+5c must be nonzero and
runtime FLAGS HALF0&81 must equal0. Eligible objects append through+e0;
last.next becomesNULL. Map0 MOBJ reserve slots precede map1 static nodes.
Runtime binding/animation transforms are then updated by the remaining
hitMain body before EVT and the later deferred draw.

An original loaded battle map can therefore supply battle-stage geometry to
the shadow ray. A native engine that has not executed/imported that selected
context, registry publication and live transforms cannot certify the chain
merely because its battle renderer or packed floor service exists. Conversely
GP14 alone cannot invalidate an explicitly retained already-published chain.

## Complete shadow filter and tagged MOBJ dependency

filter8002d380 captures HitObj argumentr4, ignores queryr3, calls hitGetAttr
80014310. That leaf returns HitObj.word4 or0 forNULL. If attr80000000 is clear,
return1. If set, call mobjHitObjPtrToPtr8005f7c4 with captured HitObj, then read
returned object.word0; return0 only if flags800 is set, else1.

mobjHitObjPtrToPtr freshly reads HitObj.word4; if attr80000000 set returns
HitObj.wordd0, elseNULL. The caller has no null guard before object.word0.
Thus a tagged owner needs its actual MOBJ pointer/lifetime/flags; unavailable
mapping is a provider hold, not permission to treat the object as accepted or
rejected. The second attribute read must remain fresh across a held/mutating
provider. Ordinary source stage collision attributes0 avoid this branch.
The filter does not reject attr1/4, map sensor labels, steep normals, hidden
draw groups or Mario/partner-specific inclusion masks.

## Finite downward query traversal and source writes

Entry captures query and filter. Call PSVECSquareMag(direction); zero returns
NULL before writing query outputs. For the admitted unit direction, capture
initial halfLength=f32(.5*length), build fixed midpoint using PSVECScale then
PSVECAdd. Load active head; exact directionX/Z==0 and Y==-1 selects downward
region80015aa0..80015e00 directly. The horizontal/other-direction bodies are
outside this contract; do not infer this region by a generic floor-ray API.

For each object in active-chain order:

1. Invoke supplied shadow filter(query,object) BEFORE broad-phase. Return0
   skips object. A held filter retains current token; completed later reads
   of center/radius/cull/triangle pointers are fresh.
2. PSVECDistance(fixed midpoint,object.center+c0). Reject only distance>
   f32(initialHalfLength+object.radius+cc); equality proceeds. Midpoint and
   halfLength never shrink when a nearer candidate changes query.length.
3. Read object.node8→draw58→BYTE1. Exactly cull1 sets query.word0=1;
   every other cull sets it0. This source query write occurs even on a later
   triangle miss. Capture triangle buffer+ac; index0. Fresh signed HALF+a8
   count at each loop test, stride54. Fresh next+e0 after object scan.

Runtime triangle is V0/+0,V1/+c,V2/+18,E0/+24,E1/+30,E2/+3c,N/+48.
hitCalcVtxPosition uses strip triples with opposite winding to rendered GX
triangles: ordinal i takes(v[i],v[i+2-(i%2)],v[i+1+(i%2)]).
Then E0=V2-V0,E1=V0-V1,E2=V1-V2,N=cross(E0,E1), from actual current
world-transformed source vertices. Reversing render triangles blindly changes
one-sided admission; normal used for lighting is not the collision normal.

Let Di=P-Vi, Q=dot(N,D0), Ki=Di.z*Ei.x-Di.x*Ei.z. In finite ordinary data:

| Cull branch | Ordered admission |
| --- | --- |
| cull1 | Require Ny>0; reject Q<0; reject any Ki<0 |
| other | Require f32(Ny*Q)>0; reject any f32(Q*Ki)<0 |

Edge equality proceeds. Cull1 permits coplanar Q0; two-sided rejects it.
The Ki arithmetic is source separate fmuls followed by fmsubs: first
f32(Di.x*Ei.z), then fused single-result Di.z*Ei.x minus that product.
Q/vector helpers use their pinned paired-single order, not arbitrary host
double dot/cross products. Numerical adapters must state their normalization,
FMA/rounding and exceptional-input limits.

Candidate T is source fdivs(-Q,-Ny). With query.length>=0, T>=currentLength
rejects; the far endpoint is excluded. Source writes query.length=T, then
point=(P.x,f32(P.y-T),P.z), then N when Q>=0 or PSVECScale(N,-1) otherwise.
It updates the retained winner only for initial bestLength<0 or strictly
bestLength>T. Ties preserve earlier object/triangle. Accepted candidate
writes shrink the length for subsequent triangle tests. On a winner, tail
PSVECNormalize normalizes the SAVED winner normal, restores saved winning
length/point/normal to query and explicitly returns winning HitObj token
in r3 at80016538. An older generic traversal document's claim of an unstaged
return value is superseded for this complete pinned tail. No hit returnsNULL;
query scratch cull/outputs can reflect earlier work, so a native caller must
not mistake any partially written scratch for a completed returned hit.

## Exact setup4 static asset domain and conditional first result

Fresh ISO FST reads matched existing private conversion input byte-for-byte:

| Asset | Size | SHA256 |
| --- | --- | --- |
| m/bti_01/d | 181042 | 213c62e758be15bc0731b3fa58558d1b2c565ea1c96e91111ea07b52cf751ca8 |
| m/stg_00_1/d | 82534 | 8d749bd8caad7fced1596231b755dc4e372e938816e10add8a8e0d7a69f5a2e1 |

Collision root bti_A/node254a0 is hierarchy-only. Its children, in order:

| Node (offset relative map-data base) | Mesh | Initial engine geometry |
| --- | --- | --- |
| 25508 | A_bti_1_00 | Four strip triangles, X[-300,300],Z[-100,100],Y0 |
| 25570 | A_bti_1_01 | Two strip triangles, X[-350,350],Z[100.0000095,600],Y-50 |

Both source draw cull1, attrs0, local identitySRT. Secondary collision root
A/df80 is hierarchy-only; pPlane2_q00000/dfe8 has source local translation
Y-2, 160 strip triangles, X[-50,50],Z[-30,50],engineY-20. Its cull1/attrs0
were freshly read. The source parent's Scale10 yields these engine units;
converted scale.1 represents engine*.01, not authored*.01.

For pinned finishing ray P=(-100,11,-5),direction(0,-1,0),L150:
initialHalfLength75, midpoint(-100,-64,-5), endY-139. In the initialized,
enabled/untransformed static registry domain, A_bti_1_00 triangle ordinal1
admits the ray at(-100,0,-5),T11, ideal normalized normal(0,1,0).
Triangle0 fails its edge domain; later Y0 ties cannot replace winner.
The secondary plane's X bounds exclude this particular ray; the rear bti
plane's Z bounds exclude it. This is a conditional geometric calculation,
not a source execution trace or proof of present runtime flags, active-chain
history, generated MOBJ slots or original normalization's exact result bits.
Other finishing records require their own independent traversal.

### Initial matrices, spheres and mutation boundaries

Complete _hitEnt8001896c/48c clears new HitObj FLAGS HALF0 at800189cc,
publishes original node pointer+8 and selected mapIndex HALFaa. It constructs
local=T*Rz*Ry*Rx*S, then world=parent*local, and copies attributes from
node.draw.word8 (or0 when no draw pointer). This initializer does not establish
that later runtime flag/binding writers have not executed.

For mesh nodes, compute localCenter=PSVECScale(PSVECAdd(originalBBoxMin,
originalBBoxMax),.5), store+9c; transform with world to center+c0. This uses
the original bounds BEFORE expansion. Then mutate original node bbox min
XYZ by subtracting single float.1 and max XYZ by adding that same float,
in XYZ order. Epsilon address8041f83c, bits3dcccccd. Transform the expanded
max corner through the current world matrix; radiuscc=PSVECDistance(that
point,worldCenter). Neither a flat-plane radius nor radius from all triangle
vertices replaces this producer. Hierarchy-only nodes use local translation
for center but do not become active geometry candidates.

bti collision node SRTs and their parent are identity below Scale10;
pPlane2_q00000 adds authored localY-2 before that scale. Child traversal
propagates computed world, sibling traversal propagates parent's matrix.
The original node bbox is writable: rebuilding a source-equivalent node
without importing its current expanded bounds is a distinct history question.
A native one-time constructor adapter should document its unexpanded source
input and avoid silently applying expansion twice.

Known independent live mutation families include hitReCalcMatrix/2,
hitBindMapObj/hitBindUpdate, hitMain's named map-animation binding/recalculation,
hitFlgOn/Off and hitGrpFlagOn/Off, HitObj attribute writers, hitDelete,
hitEntryMOBJ and selected map load/unload. Source stage map visibility requests
do not themselves prove a collision mutation nor prove that no other writer
ran. The current native trio stage/render pipeline does not recover the full
original writer census and lifetimes. Therefore an explicit initialized
battle-registry snapshot is a bounded adapter; unsupported subsequent
flags/attributes/bindings/transforms/MOBJ/load history must invalidate its
relevant owner and hold the ray until restored. It cannot declare the
collision registry permanently static because native rendering looks static.

## Existing native helpers are evidence, not this provider

Game groundAt/groundWithin currently use the field-room global pack and
spatial index; choose highest floor before slope rejection; permit barycentric
epsilon1e-4 and denominator threshold1e-8; apply Mario/partner attribute and
walkable-slope rules; can supply Y0 for an empty legacy pack; combine static
and native object overlays with separately adapted ordering. These differ
from the source active battle chain, sphere gate, callback, strict edge/far
endpoint and raw query normal. Calling groundAt or the packed battle floor
snapshot would therefore fabricate this provider's ownership/semantics.

Smallest honest implementation is a separate source-order HitObj snapshot
adapter for the loaded battle map: node/runtime flags, ordered current
triangles with source winding, center/radius/cull, next chain and tagged MOBJ
state. Import original metadata/geometry, retain live mutations, perform
independent query and declare numeric helper adaptation. Unknown registry,
flags/transforms, tagged owners or geometry is unavailable, not a miss.
Do not mark prepared2 until that genuine completed query returns a hit.

## Evidence limits

Read-only US complete bodies inspected: hitCheckVecFilter800158a4/d04,
shadow filter8002d380/58, hitGetAttr80014310/18,
mobjHitObjPtrToPtr8005f7c4/1c, hitMain80018264/24c,
bmapLoad8002892c/17c and selected sequence map-load call sites.
Direct DOL write census found selected-index stores only in bmapLoad,
bmapUnLoad,mapLoad,mapUnLoad,mapInit; active-head stores in hitMain/hitDelete.
This census excludes indirect/REL/generated stores. Complete numeric helpers,
all battle entry/exit/animation/mobj producers and original runtime import
remain separate work. No code/shared index/progress changes in this task.

| Complete US body | SHA256 |
| --- | --- |
| hitCheckVecFilter800158a4/d04 | b15fb470ab90907c5637b54cf7421c5a049182abc5f58e6473370e24d2e0904c |
| shadow filter8002d380/58 | 89930715288a406ec7c4ad6ebed8f3367050e18f277e70a89ae24effbdc46b57 |
| hitMain80018264/24c | 2665e4e790f6a8545871c7710902792a86b86e1edcf3e2ae1e39a75cac123cc5 |
| bmapLoad8002892c/17c | db1c37ba1723eb44e26223fdc78d595381d71abe136fa9c7987691c0024e55d7 |
| _hitEnt8001896c/48c | 972ae5ae81e2207327f34c7d1a39f6d708bc740e03b6afa2c9e375b2e2f2e7b0 |
