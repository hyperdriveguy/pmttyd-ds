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

## Rank0 initializer request completion: maps, flags, null script gates

Follow-up after native incremental commit27779ad; this appendix adds source
side effects beyond the already recovered seven-record projection. Parent
reported source Main/full32 portable logic and original selected5+hidden2
metadata import; actual CPU-map/GPU provider still remains a native boundary.
That projection is valid record state, not proof the following requests ran.
No claim of independent native build/run validation is made by this audit.

Exact used StageObjectInit initial service/storage order for rank0:

1. Capture B, I/S/stage/count/array and memset32 records(B1715c,0,f80).
2. Query pouch rank. Rank1..3 requests mapPlayAnimationLv80029de4 with original
   rank name and(1,0), then writes rank-specific B18110 pointer. Rank0 skips
   that request, writes B18110=0. Query pouch AGAIN; B18114 becomes
   80373e38+e00+rank*78c, hence80374c38 for0. BYTE B182a6=ff;
   WORD B182b8=0; WORD B182bc=0 at80149cc8/cc/cd0.
3. At80149cd4 call mapGetWork8002b438. It returns
   WORD8041e660 + signed32index(WORD8041e664)*2f4, without null/bounds guard.
   Caller dereferences current mapWork+a8→object+8→name pointer, then calls
   mapGrpFlagOff(rootName,2) at80149ce8. The original runtime root owner is
   required; don't substitute a guessed scene-name or treat it as a null-safe
   no-op. Primary map table/current-index lifetime is a separate map provider.
4. In order A1,A2,B,C: mapGetMapObj(name), and only when nonnull,
   mapGrpFlagOn(name,1) then mapGrpFlagOff(name,2).
   Name literals80423028/2c/30/34. Callsites On80149d04/2c/54/7c,
   Off80149d10/38/60/88. These initialize subtree hide then clear displayflag2;
   skip BOTH mutations for missing group.
5. mapGrpFlagOn(escape_wall,1) at80149d94, without the existence test used for
   A1/A2/B/C. Address802f439c is the original name string.
6. Build/reveal selectedfive in source order through mapGrpFlagOff(name,1)
   at80149f30, for each immediately after its fields/cache copies. Used
   categories2 skip unitEntry; update layer counts A1=1,A2=2,B=1,C=1;
   ironframe gate count0. Don't defer all reveals until after the whole loop.
7. Query pouch rank AGAIN at80149fec. Rank0 skips rank>0 decoration loop.
8. Build escape_wall ordinal6; its BattleStageGetPtr80139864 request at
   8014a154 supplies current Wb28 before Ystore at164. Then hide1 before
   OR R68bit1. Build spotlight ordinal7 and hide1 before OR R68bit1, using
   the inline descriptor-list termination already documented above.

After all records, source-owned stage event requests are conditional:

| Caller site / input stage field | Condition and actual source request |
| --- | --- |
| 8014a308 /stage190 | Nonnull→evtEntry(script,priority10,flags0); return discarded |
| 8014a328 /stage1a0 | A1 layercount!=0 AND pointer!=0→same evtEntry; returned event WORD15c→B18100 |
| 8014a354 /stage1a4 | A2 count!=0 AND pointer!=0→same evtEntry; eventWORD15c→B18104 |
| 8014a380 /stage1a8 | B count!=0 AND pointer!=0→same evtEntry; eventWORD15c→B18108 |
| 8014a3ac /stage1ac | C count!=0 AND pointer!=0→same evtEntry; eventWORD15c→B1810c |

Function800374ac is **evtEntry**, not mapPlayAnimation. These fields are
script pointers; they are not clip names or animation resources. Actual
selected tik stage2619c has raw0 at all those five pointer fields, with no
local/import/foreign relocations. Therefore NONE of these requests executes
in this bounded exact setup4 initializer. No invented curtain script or
successful empty event is needed. Separate upstream intro/dontyo source
animation requests are not disproven by this initializer's null pointers.
The actual used layercounts keep missing-layer flags2/4/8 clear. Stage bytes
1b1/1b2/1b3 are also0 and do not OR those overrides. Its ironframe count0
sets BYTE B180f8 mask20, then mapGrpFlagOn(if_body,1) at8014a478 and
mapGrpFlagOn(if_wire,1) at8014a484, in that order. Source string addresses
802f4484 and802f4540, from the retained802f42b8 table+1cc/+288.

The first genuinely unbound map service in native whole-initializer order is
mapGetWork8002b438, after rank0 storage setup; its root lookup followed by
mapGrpFlagOff(rootName,2) is a concrete required provider. The whole seven-
record projection does not make root/subtree flags, map owner/current-index,
subsequent source frame matrix latches or animation services operational.
An equivalent native typed map registry may own those original names/flags
with GPU draw suppression as an explicitly documented renderer adaptation.
Actual setup4's null evtEntry fields can be bound as source branch skips.

Raw stage2619c+190..1b3 (REL2632c/24), all36 byteszero, SHA256
6db65fd59fd356f6729140571b5bcd6bb3b83492a16e1bf0a3884442fc3c8a0e;
relocation census over this range found NONE. Source rank0/root/hide/reveal/
script-null and ironframe service order checked against complete Init listing.
Read-only evidence under ignored artifacts/recovered-temp-evidence/stage5-*.
Documentation-only appendix; no code/shared index/progress/inbox edits or commit.

## Group flag provider: used mask1/2 subtree mutation

Complete mapGrpFlagOn8001edd0/118 and Off8001ecb8/118 capture current mapWork
through the same globalbase/currentindex as mapGetWork. They search loaded
map entries/object records by strcmp original node name and supplied key.
With a found named object, On calls mapFlgOn8001f1fc(object,mask,0) at8001eed0;
Off calls mapFlgOff8001eee8(object,mask,0) at8001edb8.

The used1/2 domain mutates target WORD0 and its entire descendant subtree.
Child pointerE4 and siblingE8 are source object links. Helper arg3=0 means
DO NOT propagate to the TARGET's sibling subtree. Child recursion passes1,
allowing the complete chain of child siblings and their descendants. Source
bodies unroll several levels before recursion; equivalent typed depth-first
traversal may preserve target-before-children ordering. No parent flag read
or inherited immutable-hidden predicate is performed by either setter.

For On(mask1 or2), each visited nodeflags is read, ORmask, then stored. For
Off, each is read, ANDcomplementmask, then stored. Thus parent hide1 followed
by Off(selectedChild,1) makes that child's subtree independently visible even
while parent/sibling WORDs remain1. Renderer filtering must use the actual
source draw flag semantics for geometry owners, rather than checking every
ancestor's currenthide as an overriding visibility rule.

On's generalmask100 domain checks nodeWORD114 and can skip its own OR when
that pointer isnull; traversal still continues. That does not admit any skip
for usedmask1/2. Main visibility synchronization uses mapGetMapObj before
issuing setters, but Init's unconditional hide/reveal requests need real
existing names. Missing target in the public group wrappers takes the original
name-not-found request queue branch (36-byte records803c97b0 indexed by
WORD8041e674, marksWORD+20=0, copies up to31chars, setsWORD8041e670=1 and
updates queuedindex). It is not a recovered silent no-op. Exact queue capacity/
consumer callback and invalid-string domains are separate if such a missing
name is admitted by a native provider.

mapReCalcMatrix removes WORDmask200 after rebuilding WORLD; it does not clear
200000, hide1 or display2 at those latch-consumption sites. Preserve these
WORD masks rather than narrowing native visibility to a bool that loses
transform/type history.

Complete raw DOL SHA256 additions:

| Body | Address/size(hex) | SHA256 |
| --- | --- | --- |
| mapGetWork | 8002b438/14 | ddbc7aaeeb36ae24182217ef14c98d9a241a088126b1f39c314207b07ca13abc |
| mapGrpFlagOn | 8001edd0/118 | 0c197b783322085525b1f43a8d57015f5decb97c4fb0da1413a739df07478e7a |
| mapGrpFlagOff | 8001ecb8/118 | 3f647242377dd37ad4fa51ba0d1fe5a3e2974ba097228ae90275f4d24af75755 |
| mapFlgOn | 8001f1fc/440 | a141d54bf320389afa083b6dc77294c59ce4f727fb49824221432f5bc1da5d61 |
| mapFlgOff | 8001eee8/314 | 63d6a25d7e27a529869cfefca8a933b14ad0201998a763f4458c5b49b074047e |

Used mask1/2 subtree/root-sibling scope and matrix latch writes checked against
complete helpers. Native node registry/map-load order/root selection remains
an implementation/service owner; no invented original current root is supplied.
