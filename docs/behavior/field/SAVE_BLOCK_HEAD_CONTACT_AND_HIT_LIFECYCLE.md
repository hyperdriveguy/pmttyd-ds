# Save-block head-contact producer, collision box and hit lifecycle

US G8ME01 disc0 revision0;2026-10-13. Static read-only ISO/DOL evidence;
no original execution or native full-fidelity claim. Complements
[source spawn/save event](ROGUEPORT_SAVE_BLOCK_SOURCE_BINDING.md) and
[UV/material binding](../display/SAVE_BLOCK_ANIMATED_UV_AND_MATERIAL_BINDING.md).
Scoped delivery for [OW-INTERACT-01](../../requests/OW-INTERACT-01.md) and
[SAVE-01](../../requests/SAVE-01.md); full card persistence remains open.

## Collision owner and original dimensions

`mobjEntry`80060b9c creates the pose, then calls
`mobjHitEntry(object,0)` at80060d4c..54. The save-block constructor does not
replace that collision type. `mobjHitEntry`80060d78..800612ec requests the
model base via animPoseGetAnimBaseDataPtr80039c84, reads original model bbox
floats+0xd0..+0xe4, constructs sizes and original hit-node templates. The save
model bbox is min(-1.2699999809265137,0,-1.2699999809265137),
max(1.2699999809265137,2.500000238418579,1.2699999809265137).
No invented proximity radius or render-quad bounds participate.

Let R denote a binary32 rounding. Source fields:

- object+0x184 = R(10*R(scaleX*abs(R(minX−maxX)))).
- object+0x188 = R(10*R(scaleY*abs(maxY))). This is maxY, not maxY−minY.
- object+0x18c = R(10*R(scaleZ*abs(R(minZ−maxZ)))).
- object+0x190..198 starts from bbox midpoint, scaled by.5 then10, but
  object+0x194 (the Y component) is explicitly set0 afterward at80060eac.

The model-name prefix HOBJ_ special depth override80060f14..4c does not
match `MOBJ_SaveBlock`, so its dimensions remain source bbox-derived.
For default scale1 the dimensions are
(25.399999618530273,25.000001907348633,25.399999618530273), and center-offset
is(0,0,0). These are derived float values, not observations.

Mode0 creates a world matrix from position+centerOffset, object rotationZ/Y/X
in that request order, then Scale(dimensions). It copies0x68 bytes from original
node template8030b0dc into object+0x90, creates name `HOBJ_`+objectName at
object+0x78, puts that name pointer into node+0, and calls
hitEntryMOBJ(node,matrix)800185e8. Store returned hit pointer atobject+0x74;
write object pointer into hit+0xd0 at80061074..78. Attribute source80415f28
contains0x80000000, enabling original map-object owner identification;
`mobjHitObjPtrToPtr`8005f7c4 returns hit+0xd0 only when hit flags+4 contain
that mask. For the original room name the resulting hit name is
`HOBJ_mobj_save`. Mode1/2 and special depth overrides are separate domains.

## Original box vertex/strip source

Node8030b0dc references draw-mode/attribute block80415f28 atnode+0x58.
Its first bytes are00/01/00/00, so source cull byte+1=1 (one-sided);
attribute word+8=0x80000000. Do not use default double-sided cull3 for this
dynamic box. See [ray cull gate](../overworld/OVERWORLD_ROAD_NULL_DOWNWARD_RAY.md).

Node8030b0dc references mesh8030b090 (node+0x64). The mesh has six strip
pointers at+0x10..+0x24 and VCD80309cf0 at+0xc. VCD+0 references position
array80309a78; count8, then these binary32 triples:

| Index | Unit XYZ |
| --- | --- |
| 0 | (-.5,0,.5) |
| 1 | (.5,0,.5) |
| 2 | (-.5,1,.5) |
| 3 | (.5,1,.5) |
| 4 | (-.5,1,-.5) |
| 5 | (.5,1,-.5) |
| 6 | (-.5,0,-.5) |
| 7 | (.5,0,-.5) |

Each original strip contains count4, then24-byte vertex records with signed
position index at+4 of its count-prefixed record. Strip addresses and indices
in source mesh order:

| Address | Position indices |
| --- | --- |
| 80309d58 | 0,6,2,4 |
| 8030a08c | 7,1,5,3 |
| 8030a3c0 | 7,6,1,0 (bottom) |
| 8030a6f4 | 5,4,7,6 |
| 8030aa28 | 3,2,5,4 (top) |
| 8030ad5c | 1,0,3,2 |

Complete `hitCalcVtxPosition`800165a8..80016a58 processes the ordinary
unpacked mesh path: triangle j's indices are(j,j+2,j+1) for even j and
(j,j+1,j+2) for odd j, then requests the source node matrix transform for each
vertex. This alternating order is **not fan triangulation**. The original
box yields12 triangles with outward normals; its bottom has normal(0,-1,0).
Keep source strip/triangle order when adding dynamic collision nodes.

At room spawn(185,60,-55),zero rotation and unit object scale, bottomY=60,
topY rounds85, X faces round172.3000030517578/197.6999969482422 and Z faces
round-67.69999694824219/-42.29999923706055. Preserve the float transform
rather than rounded decimal literals when generating these vertices. A_1
node animation does not redefine this bbox collision primitive. Original
mobjMain separately transforms existing hit nodes with object position/rotation/
scale; recovering animation meshes is not a substitute for that hit owner.

## New-head-contact admission and flag8 producer

`marioChkOverhead` publishes actual head hit pointer atMario+0x1f8. The
[ordinary head contract](../overworld/OVERWORLD_AIRBORNE_AND_LANDING.md)
recovers five probes, source ray/normal/contact admission and upward jump
response. Registering the above dynamic object triangles into that query is
the source-derived contact route; proximity+A is not its replacement.

`mobjMain`8005f968..80060988 captures Mario inr28, manager inr22,
and initializes newHeadCandidate r24=NULL at8005f9e0. In field modeGP+0x14=0:

1. Read current Mario+0x1f8. Null clears cached manager+0xc and leaves
   candidateNULL.
2. If nonnull and cached previous head nonnull, call hitGetName on each and
   strcmp their names. Equal names retain the cache and candidateNULL even
   when raw pointers differ.
3. Otherwise update manager+0xc=currentHead and candidate=currentHead
   (8005fa90..98). The candidate is a new contact by NAME across all objects,
   not a persistent held collision or a per-object distance test.

For the save-block's active object flag1 and default non-Koopa Mario,
flag0x400000 enables the branch8006031c..38. When its flag8 is clear,
Mario character byte+0x3c!=2 and candidate==object+0x74, write
object+0x1e0=0x400000 and admit the shared flag8 write800605f8..608.
The source also has other attack/partner producers under other object flags;
the save constructor's flags0x400085 do not enable those alternatives.
`mobjMain`'s outer sequence may skip update processing in field sequence
states beyond1 except8 unless objectflag0x400 is set. The current save-block
does not set0x400. Full field-sequence/cadence ownership remains separate.

Before callback dispatch the producer has three suppressions:
objectflag0x100 clears8; nonzero marioStGetSystemLevel8006f7bc (GP+0x18)
clears8; Mario motion31 OR32 clears8. These occur8006060c..658. Then the
object+0x1d0 callback runs at80060670. An already running save lifecycle
continues independently of flag8 because stages1..4 do not recheck the hit.

This new-contact cache must survive ordinary frames and clear on no head hit.
Repeated polling of the same contact must not retrigger saves. Native dynamic
node lifetime/query merge order, original field sequence, transformed Mario
and all other object producers need broader integration/trace validation.

## Complete local save-block callback stages

`mobj_save_blk`80066f58..800670e0 switches on signed stateobject+0x1dc;
only0..4 have handlers, other values no-op. Every valid stage returns0; it
does not request object deletion. No handler falls through into the next stage
in the same invocation.

| State | Original ordered requests/publication |
| --- | --- |
| 0 | If flag8clear:return. Otherwise marioKeyOff, marioStSystemLevel(1), psndSFXOn_3D(431,&objectXYZ), animPoseSetAnim(pose,`A_1`,restart1), camGetPtr(8) halfword flags OR0x200, increment state1. |
| 1 | Read animPoseGetLoopTimes(pose). Ordered ratio<1 waits. Otherwise animPoseSetAnim(`S_1`,restart1); if optional event+0x1d4 nonnull, call mobjRunEvent and increment2; if null, write3. |
| 2 | evtCheckID(object+0x1cc)==1 waits. Any other result writes eventID0 and increments3. |
| 3 | mobjRunEvent(object,systemEvent+0x1d8), then increment4. The helper return is not checked. |
| 4 | evtCheckID(eventID)==1 waits. Any other result calls marioStSystemLevel(0), clears camera8 flag0x200, clears objectflag8, marioKeyOn, writes state0. It does not explicitly clear eventID here. |

A_1 literal is80420208. The original clip has selector0,start0,end15,
16 keys0..15,75 node/14 texture updates, no position/normal/visibility updates.
It is nonlooping. The query8004079c..800407b0 only returns floatpose+0x84;
it does not freshly evaluate animation. Compare threshold is binary32
1.0 at80420250. The
[animation Main time contract](../display/ANIMATION_MAIN_ENTRY_AND_TIME.md)
publishes elapsed ratio there even after nonloop selected time clamps at15.
Nominal15 animation units at source rate1 corresponds250ms, but callback
completion is a query of the published ratio, not a hard-coded render-frame
counter or a promise about exact observed field-frame timing. NaN ratio is
not ordered<1 and therefore proceeds through state1.

Important scheduling: mobjMain callback80060670 precedes its same-iteration
animPoseMain80060770. State1 reads a previous publication. A native owner that
samples A_1 before checking completion has changed this ordering. Stage0's
animation restart and system-level requests also precede that later Main.

`mobjRunEvent`8005f744..8005f7c0 returns0 if event pointer null or
existing eventID is running (evtCheckID!=0). Otherwise writeeventID0, call
`evtEntryType(event,priority30,flags0,type26)`80037220; write new context
userdata+0x174=object and object+0x1cc=contextID+0x15c. Save callback
state1/3 does not test this helper result. No local allocation failure guard.
Its optional-event wait and subsequent system-event wait are separate stages.
The original room's optional pointer0 selects0→1→3→4→0.

Mario keys use a nested unsigned-byte counter atMario+0x39: KeyOff sets
mainflag8 and increments it modulo256; KeyOn decrements and clears mainflag8
only when resulting signed byte==0. The save system event has additional key
OFF/ON calls, so preserve nested ownership rather than a single bool. System
level1 stores mode1 and changes GP+0x18 low nibble to1, requesting original
event stop mask1; mode0 clears low nibble and requests event start mask239.
These are source service requests; full event pause/category behavior remains
owned by the event manager contract. Save callback camera8 lock is independent.

## Validation and remaining work

Fresh named `inspect_dol_function.py` read-only listings cover mobjMain,
mobjHitEntry, mobj_save_blk, mobjRunEvent, animPoseGetLoopTimes,
hitEntryMOBJ, hitCalcVtxPosition, marioChkOverhead, marioStSystemLevel,
Mario key leaves and owner/name leaves. Exact original DOL data reads recover
unit vertices, six strip lists, attribute block and literals/constants.
Original model/S_1 identity remains pinned in the linked display contract;
A_1 was parsed from that same exact ISO resource. Listings and any extracted
payloads remain /tmp, not source history. Original Nintendo/Intelligent Systems
rights remain; no redistribution is implied. Docs-only: no make/build or
Dolphin/hardware correctness claim.

Remaining owners: dynamic hit registration/allocation/map query and ordering,
all field/transform/partner/contact producers, pause/service/category lifetime,
original audio macro431, common save child event/card flow, first-hit/story
switch persistence, matched original observations and native regression captures.
