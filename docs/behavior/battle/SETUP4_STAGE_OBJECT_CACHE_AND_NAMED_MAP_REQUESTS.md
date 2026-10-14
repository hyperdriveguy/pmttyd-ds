# Setup4 stage object caches and named map requests

2026-10-13 US G8ME01 disc0 revision0 read-only DOL/REL and current native
asset/render audit. Narrow provider for StageObjectMain80149798 after ending,
following [top4 ownership](ENDING_TOP_BANK4_SERVICE_OWNERS_AND_IDLE_DOMAINS.md)
and [stage hierarchy](UNDERGROUND_SETUP4_INTRO_PHASE_STAGE_AND_PLAYER_COMMANDS.md).
No native implementation/commit or whole stage-object initializer closure.

## Original selected5 initialization

StageObjectInit80149bd0 captures B; follows I=B2738→S=I+c→stage=S.word0;
selected count=stage+8,array=stage+c. It memset(B1715c,0,f80),32 records stride7c.
For each selected descriptor D (stride18hex), scans first record with signed
WORD0<=0. Ordinals increment1..5; used selected records are therefore firstfive
while original bank was cleared. At80149de8 R64=D,80149df0 R0=ordinal,
80149df4 R6c=-1. Descriptor is not a direct name pointer: D.word0 points to
name, U16 D4 is unit-category, U16 D6 is layer, floatD8/c/10 XYZ.

All actual setup4 descriptors have unit-category2, so no synthetic battleunit
creation branch (0/1) is admitted by this selected5 phase. Their layers are
1,1,0,2,3 respectively. Source additionalXYZ and owner names:

| R ordinal / source D file offset | Name / map | XYZ | Layer |
| --- | --- | --- | --- |
| 1 /tik25c08 | isi_1 /stg_00_1 | (-202,0,-17) | 1 |
| 2 /tik25c20 | isi_2 /stg_00_1 | (200,0,-30) | 1 |
| 3 /tik25c38 | A1 /stg_00_1 | (0,0,-50) | 0 |
| 4 /tik25c50 | B_mati /stg_00_1 | (0,0,0) | 2 |
| 5 /tik25c68 | C /stg_00_1 | (0,0,0) | 3 |

Used layers copy D.XYZ→R04/08/0c at80149e84..b4. OffsetR10/14/18 and
additionaltranslationR28/2c/30 remain memset0. At80149eb8/c0/c4 it explicitly
stores FLOAT0 (literal80422fd0) to R24/20/1c, which are rotation angles.
This body does NOT initialize these fields toscale1.

It copies12 WORDs from source template802f42b8..42e7 into cachesR34..60,
ordered80149ec8..9f24. Template is twelve FLOAT999.0 (raw4479c000), NOTzero
and NOTcopied currentXYZ. Thus even currentzero vectors compare dirty on the
first original Main call. R68 remains0 in the selected5 phase. It reveals each
named subtree with mapGrpFlagOff(D.word0,1) at80149f30.

Full Init continues: rank0 excludes extra rankdecor but still creates builtin
escape_wall and spotlight descriptors, marking those hiddenR68bit1. Thus
this selected5 table is NOTproof the other27 records remain inactive after
full Init. Whole32 Main binding requires their real descriptors/flags/history;
retain this explicit frontier rather than fabricate onlyfive active records.

## Exact rank0 builtins: escape wall and spotlight, not curtain records

After the five selectedrecords, rank0 bypasses the rank>0 three-decoration
branch. At8014a100 it addresses inline descriptor80417ca0, `escape_wall`:
unit-category4,layer5,rawXYZ(-175,0,0). First free record becomes ordinal6,
R64=descriptor,R6c=-1. It copiesrawXYZ, then requests BattleGetStagePtr80139864
and replaces R08 with current StageWorkW+b28 at8014a164. Original
BattleStageInit supplies Wb28=200 from literal80422b40, making fresh additional
XYZ(-175,200,0). It zeros rotations, copies cachetemplate+60..8f (all999),
hides name through mapGrpFlagOn(name,1) at8014a1dc and ORs R68bit1 at1f4.

The following inline descriptor list starts80417cb8 (not a pointer list):
`spotlight`,unit-category6,layer7,XYZ(0,240,0). It becomes ordinal7,R64 that
address,R6c=-1,rotation0,caches template+90..bf(all999), then hides namedgroup
and ORs R68bit1 at8014a2ec. Index advances by18hex. WORD name0 at80417cd0
terminates the list. Neither used builtin category4/6 requests a synthetic
battleunit entry in this loop. Allremaining records stay bank memsetzero in
this bounded rank0 initialization, apart from other transitive providers.

The `escape_wall` descriptor is NOT a curtain; earlier provisional naming
was corrected by original raw data. Actual stage authored animation requests
occur separately after these records at8014a2f4 onward. Those animation
pointers/consumer readiness remain their own provider. Hidden builtins still
enter Main because their R0 is positive; mapGetMapObj/flags/changed transforms
must have a named owner even when native geometry is suppressed.

Descriptor80417ca0/18 SHA256
0f501e896bed676548881bb474a4523f7c242d5c0605ae384df5dd3b4dbb4100;
80417cb8/18 SHA256
d4befe0079713aa1107c26e38cdac3d0114977948ada34d26632ece27f01abc8.

## Exact Main cache/read order

Main recaptures B, scans32 R=B1715c+i*7c in ordinal order. Signed R0<=0 skips
all fields/services for that record. Positive sets local dirty=0 and processes:

1. Compare R04/08/0c with cacheR34/38/3c using ordered fcmpu equality tests.
   On any mismatch set dirty1; copy RAWWORD X→cache34, Y→38, then Z→3c at
   80149804..1c. Equality does not canonicalize floating bits.
2. Compare offsetR10/14/18 to cacheR40/44/48; same copy order at80149850..68.
3. Compare additionaltranslationR28/2c/30 to cacheR58/5c/60; copy at
   8014989c..b4. This group is NOTrotation.
4. Compare anglesR1c/20/24 to cacheR4c/50/54; copy at801498e8..9900.
5. Read R68. Mask2 forces dirty1, clears ONLY mask2 and storesR68 at80149918.
6. Read descriptorU16 D6. For layer2 ONLY, compare B180e0/e4/e8 to caches
   B180ec/f0/f4. Mismatch sets dirty1 and copiesrawXYZ at80149960..78.
   This global orientation input/cache is shared across layer2 records.
7. Reread D6 for transform dispatch. Services run only when local dirty!=0,
   but cache writes and force-bit consumption already happened BEFORE callbacks.
8. Independently reread R68 and synchronize original group visibility.

No rollback or retry-dirty preservation occurs if a downstream map provider
fails to find a name. Do not move caches after GPU upload or retain mask2 until
native rendering succeeds; that changes original source ordering.

## Used transform ABI and layer-specific request graph

mapObjTranslate8001e254 takes(r3=name,f1=X,f2=Y,f3=Z).
mapObjRotate8001e390 takes(r3=name,f1=angleX,f2=angleY,f3=angleZ), in degrees.
Each service rereads name through current R64→descriptor.word0 before call.
Main never requests mapObjScale. Define single-add total translation:
T = R28xyz + single(R04xyz+R10xyz), preserving two addition steps peraxis.

* Layer0/1 (also6/7 locally): Translate(name,T) at801499f4, then
  Rotate(name,R1c,R20,R24) at80149a0c.
* Layer2 (actual B_mati): Translate(T) at80149a5c;
  Translate(0,-R14,0) at80149a78;
  Rotate(name,B180e0,B180e4,B180e8) at80149a90;
  Translate(0,+R14,0) at80149aa8. R1c/20/24 comparisons still markdirty but
  the layer2 consumer uses global orientation instead.
* Layer3 (actual C): Translate(T) at80149af8;
  Translate(0,-R08,0) at80149b14;
  Rotate(name,R1c,R20,R24) at80149b2c;
  Translate(0,+R08,0) at80149b44.

Ypivot layer2 is offsetY R14, layer3 is positionY R08; do not swap them or
replace both with a fabricated common model pivot. Source zero pivot initial
cases still issue all four calls when dirty, not just one translate.

Both map services select current map owner by global map state and search each
loaded map/object record by strcmp object.node.name to supplied name; missing
name returns without writing. For found objectO, if O.flags mask200 clear,
initialize additional matrixOac toidentity before concatenating; otherwise
retain prior matrix. Translate divides each argumentby10 and computes
PSMTXConcat(Oac,T(local),Oac). Rotate constructs source degree→radian axis
matrices, then concatenates Oac with Rz, Ry, Rx in that order at8001e4a8/b8/c8.
Each service ORs mapobjectflags200000 then200 (separate freshreads/stores).
Thus within a fresh additional-matrix domain the request series gives
T*Rz*Ry*Rx, or T*Ty(-pivot)*Rz*Ry*Rx*Ty(pivot). Authored local/ancestor SRT
remains separate. Actual map-frame producer/flag200 clearing and re-calculation
cadence must be supplied before treating this as an unconditional new absolute
matrix every tick; the local services conditionally preserve existing Oac.

## Actual authored hierarchy composition and per-map latch consumption

Complete mapReCalcMatrix80028130/420 takes(object,parentMatrix,force). For
an object marked200 or inheritedforce, it builds WORLD O1c from
Parent * authoredLOCAL O4c, or Parent * animationLOCAL O7c when flag2000.
If objectflag200000 is set, it then concatenates WORLD*additionalOac into
WORLD at800281b8..c4. It clears ONLY objectflag200 at800281c8..d0. Equivalent
child/sibling branches repeat that composition and latchclear at800283d8..dc
and800284c4..c8. Recalculation propagates parent WORLD to descendant nodes.
This proves additional transform multiplies on the RIGHT of authored/local
hierarchy; WORLD-left additionaltranslation is a different operation.

mapMain80026bb8 calls mapReCalcMatrix at80028100 after its own updates,
using rootpointer(mapentry+a8),parentmatrixScale(10,10,10),force0. The original
outer-loop ordering and same-call point must remain the existing map service
owner: do not imply the renderer resets this latch automatically. After a
completed actual recalculation clears200, the next changed StageObjectMain
series will reset Oac once on its first transform request, then concatenate
subsequent pivot/axis calls because those calls set200. With no new object
changes, Main makes no transform requests, and retained additional remains
part of WORLD via flag200000. Thus no accumulation across updates when the
original latch/recalc owner is supplied, while within one request series
concatenation is required.

mapReCalcMatrix raw80028130/420 SHA256
212353de648b9515c7c9618b7faa4210044a37855eb11ab28fa2d389388d75f1.

## Visibility requests, not an animation player

After transform dispatch, Main tests R68 bit1:

* Set: mapGetMapObj(name)8001f878 at80149b5c; reads returnedO.flags with no
  null guard. If O.flags bit1 clear, mapGrpFlagOn(name,1)8001edd0 at80149b78.
* Clear: mapGetMapObj(name) at80149b88; if O.flags bit1 set,
  mapGrpFlagOff(name,1)8001ecb8 at80149ba4.

These operate named subtree flags, including descendant nodes; selected-child
reveal after hidden parent must remain possible. There is no animationframe,
pose-duration or playback handle gate in Main itself. Authored curtain/intro
animation writers, source scale changes and their force-dirty requests are
upstream providers. The record+68 forcebit2 is not an animation-complete flag.
After all32 records Main calls BattleObjectConfig80149794 (completebodyblr).

## Native resource audit and minimum truthful adapter

Current tools/trio_battle_assets.py converts m/stg_00_1/d with scene_roots
A1,isi_1,isi_2,B_mati,C and scene_offsets A1(0,0,-.5),isi_1(-2.02,0,-.17),
isi_2(2,0,-.3), preserving original hierarchy. It does NOT pass object_bindings.
The original map data therefore has unique named owners, but its current pack
bakes per-root positions into one vertex/batch buffer. Renderer::Room retains
vertices/batches/textures/animation tracks, not a named stageobject registry.
prepareBattleMaps loads stg_00_1 as one Room; drawMap renders that pack. Applying
a five-object request to the whole Room would move unrelated selected props.

map_convert.py already supports object_bindings: PMHOBJ02 payload records retain
pernamedobject tag, affine parent/root matrix, affected vertexindices, local
coordinates and original bakedcoordinates. Existing field bindings show a
reusable converter route; actual stage loader/provider import is missing.
A bounded native adapter may bind original selected descriptors to separately
retained vertexgroups and authored ancestor matrices, explicitly undoing or
accounting for baked scene_offsets once. It must preserve source cache order,
layerpivot request semantics and pernamedvisibility. Keep originalstg parent
B Z=-8/C Z=-10 modelunits; don't recenter already baked hierarchy.
Original coordinates engine*.01 and model*.1 are the established numerical
adapter, not source object units. Include full builtin objects/animation/map
frame providers as separate requests before calling wholeMain source-complete.

## Evidence

Complete original raw DOL SHA256:

| Body | Address/size(hex) | SHA256 |
| --- | --- | --- |
| StageObjectMain | 80149798/438 | f54092d325edbfc3f19bc7571087be134bc24f9b468040665ee79bb3870c79f4 |
| StageObjectInit | 80149bd0/8d4 | e22954ec0bac86bf110c6767fa80cd0298a31d3ff74a5b453ab2d8ee7365e53d |
| mapObjTranslate | 8001e254/13c | 8568e88b40ba87f4d1230cdc767c803b07230e6d9b8d4b62a2dfe1c1371b7a07 |
| mapObjRotate | 8001e390/180 | 57e372ec60ffbbf1c19db9155a33e19257e14109babf0fbd6be1d57032afbbcb |

Template802f42b8/30 SHA256
b6b481ffa97888199fe4f62db633f871f0f44a70a24fc6974df77fc8162b9dfa.
Ignored evidence: artifacts/recovered-temp-evidence/stage5-* and ending-top4-
BattleStageObjectMain.dis. Checked exact selecteddescriptor/default/cache,
transform/visibility call order and native pack limitations. Documentation only;
no native/shared docs edits, build rerun or commit; scoped diff check passed.
