# NPC map-side movement and Goomba3 zero vertical path

US G8ME01 disc0 revision0; read-only DOL recovery2026-10-13 for
[plaza Goomba3](GOOMBA3_SOURCE_SETUP_AND_TALK.md). Complements
[shared wander](FIELD_NPC_WANDER_DRIVER_AND_GOOMBA4.md),
[NPC main side ordering](FIELD_NPC_MARIO_SIDE_RESPONSE_NUMERICS.md) and
[scalar ray wrapper](FIELD_SCALAR_RAY_WRAPPER.md).
This is source-static behavior, not an original runtime measurement.

## Complete finite ordinary npcHitCheckSide body

npcHitCheckSide80048418/918 receives NPC,r4outX,r5outZ,r6direction pointer,
f1deltaX,f2deltaZ. It first overwrites both outputs with NPCcurrentX/Z.
Both delta components exactly0 returnsnull immediately, with no ray call or
direction write. Nonzero components call angleABf(0,0,deltaX,deltaZ), then
sincosf(angle,&rayX,&rayZ). angleABf8000703c/1b4 returns field heading **degrees** through original
lookup803041e0: +X90,+Z180,-X270,-Z0. sincosf80005e8c/78 converts
single(degrees*pi)/180, calls original sin80272410 then cos80271ea8,
publishes single sin through firstpointer and negative singlecos through
secondpointer. Thus rayX=sin(angle),rayZ=-cos(angle); the middle slope
angle is also degrees. These are original helper providers; substituting
normalize(delta) or ordinary atan2 radians is not instruction-equivalent. Let width=NPC+14c,
height=NPC+150,rootXYZ=NPC+8c/90/94 and movement length L. L derives from
single fma(deltaX,deltaX,single(deltaZ*deltaZ)), then the source frsqrte+
three double refinements and final single conversion. Source NaN/classification
branches remain the helper's full domain; finite positive motion uses L.

Three ordered ray probes share direction(rayX,0,rayZ), originrootX/rootZ,
and in/out extent singleFMA(.75,width,L):

| Probe | OriginY offset | Admission after nonnull ray result |
| --- | --- | --- |
| 1,80048594..48650 | min(single(.2*height),10) | Immediately slide and return that contact |
| 2,80048804..488b0 | single(.4*height) | Slide/return only when angleABf(0,0,sqrt(nx²+nz²),-ny)>=45 |
| 3,80048b6c..48c14 | single(.75*height) | Immediately slide and return that contact |

Reset extent from L/width before each probe. A middle contact failing its
angle predicate continues to probe3; it does not itself stop movement. A
third miss adds full delta to outputs and returnsnull, even if a rejected
middle contact existed. A probe's returned hit position and shortened extent
are not used by this slide calculation. Returned normalNX/NY/NZ is.

Each probe selects hitCheckAttr(mask4,...) if NPCmainflags2000 set, otherwise
hitCheckFilter(null callback,...). Ordinary Goomba3 flags80603/00480603 lack2000,
so all three pass null callback. This skips the optional owner callback; it
does not invoke ordinary Mario wall filters, pass-through attributes or named
owner rules. See the scalar-wrapper contract for the actual null semantics. Original triangle/map-object
registry, authored transforms, nearest/tie traversal, hit attributes and query
filters belong to the ray provider, including mutable extent semantics.

On an admitted hit the exact single-operation slide shape is:

```
dot = singleFMA(deltaX,nx,single(deltaZ*nz))
slideX = single(.5 * singleFNMSUB(dot,nx,deltaX))
slideZ = single(.5 * singleFNMSUB(dot,nz,deltaZ))
outX = single(outX + slideX)
outZ = single(outZ + slideZ)
```

singleFNMSUB(a,b,c) is c-a*b with one rounding. This **halves** tangent motion;
it neither clamps movement to returned ray distance nor removes normal motion
then applies the full tangent. Preserve fused operations where supported.

Only NPCmainflags10000 enables heading reflection. Ordinary Goomba3 lacks it,
so its direction pointer is unchanged by this body. The separate reflection
branch calls reviseAngle(direction+180), computes normal heading through
angleABf(0,0,100*nx,100*nz)/reviseAngle, then wrapped differences/sum and
integer truncation before publishing direction. It is outside this finite
Goomba3 domain and must not silently enable a bounce for source flags80603.

### Horizontal null-callback backend admission

Fresh hitCheckVecFilter800158a4 horizontal-direction region80015938..15a74
proves the optional callback is skipped when null. It traverses shared active
head8041e628/next+e0; checks owner midpoint/radius broadphase; reads source
node+58 draw-mode byte+1 for one-sided culling; traverses triangles+ac with
stride54/signed count+a8. One-sided normal dot direction>=0 is rejected.
No attribute/name rejection occurs independently in this horizontal region.
Nearest ties retain earlier owner/triangle. Original geometry query and
registry lifetimes still belong to providers. A native all-source-culled-
geometry ray is justified in this finite domain; adding Mario-only wall
attribute/name restrictions would invent the absent callback. See also
[shared registry/order](DYNAMIC_OBJECT_HIT_REGISTRY_AND_RAY_ORDER.md).

## npcMain calls the map-side body twice on contact

With active state10 and positive dt, caller retains earlier documented
special half-displacement and mainflag08000000 Z suppression. Goomba3 lacks
40000000, so source80049f00..9f24 calls the body with temporary outputXZ and
pointerNPC+1ac; publishes its **first** return into NPCcontact+2fc.

If first return isnull, copy temporaryXZ to currentXZ. If nonnull, source
80049f34..9f5c computes newdelta=tempXZ-currentXZ and calls the same body
again, this time outputs point directly to NPCcurrentX/Z. It ignores this
second return; contact+2fc continues to identify the first contact. Therefore
second-pass hit or miss must not overwrite contact+2fc. Original currentXYZ
is unchanged before the second call, except any independently owned writes;
output initialization now aliases currentXZ and preserves the source order.

Then clear NPCmovementX+1b4/Z+1bc and invoke npcMoveG80048d9c. The known
Mario side response follows, then automatic pose selection uses actual
current-minus-previous movement. Never infer walk speed from requested delta
before these map/actor responses. dt<=0 skips horizontal and npcMoveG and
retains components, per the existing caller contract.

## Goomba3 does not need a speculative gravity simulation

Fresh npcEntry clears vertical+1b8 and other work. Source Goomba3 init is
RETURN/END; its shared regular writer emits horizontal components only.
Fresh80603 and wander00480603 lack both gravitymask20000 and mainflag10.
Complete npcMoveG captures/clears vertical+1b8 before gates, leaves gravity
contribution0 because20000 absent, and tests netvertical0 with10 clear.
It exits before rootY/contact300/state/ground-ray writes. This is identical
finite zero-vertical deduction to Goomba4, independently of40000000.

Therefore source Goomba3 remains at authoredY0 in this lifecycle without
foreign vertical/flag writers; no floor snap, downward support query or added
gravity belongs to this finite slice. The remaining obstacle to authored
wandering is the actual map-side ray provider and its ordered response,
plus retained NPC/Mario collision/scheduler/RNG owners. General gravity,
nonzero vertical writers, foreign events and all-story lifetimes remain open.

## Constants and validation

Read-only source single constants (r2=80427440):0 at8041fc7c, .5fc80,
10fc98,180fccc,.75fcd4,.2fcec(3e4ccccd),100fcf0,
.4fcf4(3ecccccd),45fcf8. Source upper offset reuses.75 exactly.
Fresh named inspect_dol_function.py extracts retained in ignored
artifacts/reference/goomba3-services. npcHitCheckSide code SHA256
5166abc07dc098033804bb34aec364b100484d883bff4ddb4cdb58e1ce42c0b3;
npcMoveG9e112539a029748a486e2f692b41cf3fd6884beb3dc9c0ea3a36673c092602b6.
No source/native code, shared docs,index,emulator profiles or saves changed.
Documentation only: no build, GPU, original execution or hardware parity claim.
