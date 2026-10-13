# DMG-02 — Direct application service internals and native bindings

**Status: OPEN (partially answered).** Part of the [request inbox](../DOCUMENTATION_REQUESTS.md); validated deliveries and locks are registered in the [delivery log](DELIVERY_LOG.md).
Request text below is preserved verbatim from the inbox; answers and implementation breadcrumbs are append-only history. Updated 2026-10-12.

## Open todos

- [ ] Complete `BodyPartsId`/`GetPartsPtr`, `GetHitPos`/`GetPos`/`GetHeight`/
  `GetBelong` plus the `field114`/`pointer144+10` schemas and lifetimes; the
  original `belong` getter needs closure.
- [ ] Concrete boss/Mario/partner part attributes, kind `9c/a0/a4/a8` sound
  pointers and the original effect/macro resources.
- [ ] Action-record `AddCount`/`AddPoint` saturation and storage, audience
  helper state/RNG effects, hit-event dispatch inputs and all reachable
  death/revival/recovery scripts.
- [ ] Shared FP owner lookup/update reconstruction and pouch/item151
  consumption (no invented revival).
- [ ] Exact numerical/rounding and position-query/service request order with
  original traces (suppression, zero/absorption, simultaneous HP/FP, Pity at
  full FP, wakeup on lethal/Instant Kill, pattern25 gates).
- [x] Unit/part position queries and movement sound core — delivered
  ([positions](../behavior/battle/BATTLE_UNIT_PART_POSITION_QUERIES.md),
  [movement sound](../behavior/battle/BATTLE_MOVEMENT_SOUND_CONTROL.md)).

## Request text (verbatim)

The local [direct application](../behavior/battle/BATTLE_DAMAGE_APPLICATION.md) is now
implemented and tested,including shared partner FP,Pity Flower/Sleep RNG order,
sound/effect request precedence,narrowing/suppression/accounting,lethal-code
marking and final hit-event request. The native adapter records latest-hit
requests and uses the resource mutations;original effect/audio/VM execution is
pending. No replacement effects/samples or revival scenes were invented.

Please recover complete BodyPartsId/GetPartsPtr,GetHitPos/GetPos/GetHeight/
GetBelong and field114/pointer144+10 schemas/lifetimes;concrete boss/Mario/partner
part attributes,kind9c/a0/a4/a8 sound pointers and original effect/macro resources.
Pin action-record AddCount/AddPoint saturation/storage and audience helper state/
RNG effects,hit-event dispatch inputs and all reachable death/revival/recovery
scripts,shared FP owner lookup/update reconstruction and pouch/item151 consumption.
Confirm exact numerical/rounding and position-query/service request order with
original traces,including suppression,zero/absorption,simultaneous HP/FP,Pity at
full FP,wakeup on lethal/Instant Kill and pattern25 gates. The current interface
treats belong as resolved semantic unit state;its original getter needs closure.
Use native data/requests as test fixtures only;do not treat them as original
scene/stat/position/audio/turn behavior. Link concrete data/service answers here.

## Answers and delivery notes (append-only history)

Partial ID2 answer:[unit/part position queries](../behavior/battle/BATTLE_UNIT_PART_POSITION_QUERIES.md)
close home,part offset/world and scaled hit leaves,BattleGetUnitPartsPtr and the
event HitPos wrapper. Absolute part flag10000000 differs from owner-position
plus offset;relative X uses owner189,while scaled hit X uses captured side-table
factor. Two offset triplets scale through unit114 with ordered fused operations;
event outputs truncate to signed integer with raw destination sentinel checks.
No source writer/actual original scene coordinate is inferred from the leaf.
[Movement sound core](../behavior/battle/BATTLE_MOVEMENT_SOUND_CONTROL.md) closes retained
voice/periodic alternation/stop and identifies only record18 as retained handle;
keys0/4 are playback inputs. Original kind sounds,source writers,field144 schema,
height/belong/action-record/audience/revival/VM/resource helpers and execution
traces remain open. POS/MOVE-SE requirements provide scoped implementation cases.

### 2026-10-13 delivery: ordinary Hammer used underground trio binding

New [battle contract](../behavior/battle/ORDINARY_HAMMER_USED_TRIO_BINDING.md)
pins originalW80353de8→wrapper80416c40→387cmdcore8035d6d4; freshmask2clear
N4/T25/difficulty3,approach/precheck,level1M_H_1A/B/C,actualResultAC→WAIT7
impact and100/20100branches. ExistingHammerCommand/ActionCommandView/shared
VM/damage APIs mapped; packedID3main/result/delete lifecycle/heldgeneralpad/
sharedforcedcounter/levelP99/Hammerpower80189c38 remain concreteintegration
requirements. No fabricatedfixedimpact schedule orautoselectedHammer;
originalJumpSpikedFranklywarningremainsconditionalsourcegraph. Display/audio/
nonfresh/stylish/firstattackproviders remainexplicitrequests. Documentation-only,
no nativeedits/commit;git diff --check validation only.


### 2026-10-13 delivery: original Paragoomba wing-fall used binding

[Wing-fall contract](../behavior/battle/PARAGOOMBA_WING_FALL_USED_TRIO_BINDING.md)
pins W74/part800/F100 admission, actual code410117 fresh failed H1,
tracked generic damage→data0e→fall93521ee4 child and full26-command order.
Grounded pose/data/parts93522728/27c8/2800 recreate original c_kuribo KUR poses;
ChangeKind writes only U8, preserves original U10/HP/actor identity. ReplaceParts
reset/preservation fields, OffAttribute/OffParts RAW masks, OnUnitFlag U1c,
all grounded event-install and evaluated sound-key packed writes are closed.
Ground init9352290c exact wait/attack/damage/confusion/sound/StartWait sequence
included. Air admission becomes partattrs9 at replacement; no invented hide-wing
or HP-based scene trigger. Original last-enemy result selection enters end7
07000000 then FieldBattleInfo+3c callback or audience-end admission8021290c;
full end/reward/field return remains a concrete dependency. Docs-only/no native
edits/commit; read-only source recovery and git diff --check validation.


### 2026-10-13 delivery: wing-fall SetHomePos801123d0

[Wing-fall ABI](../behavior/battle/PARAGOOMBA_WING_FALL_USED_TRIO_BINDING.md)
closes reached SetHomePos801123d0/128: four evtGetValue operands in order,
actor translated, signed integer XYZ converted to float; leaf801289a0/10
writes U30/34/38 only. No sentinel, evtGetFloat, output, first-call/current
position/offset/floor query. Native must preserve original integer GetPos
locals before storing home, not invent fractional inputs. Read-only source
listings /tmp/trio-para-sethome.dis and leaf listing. Docs-only/no native
edits/commit; git diff --check passed.


### 2026-10-13 delivery: grounded Paragoomba attack93105a14 angle helper

[Wing-fall contract](../behavior/battle/PARAGOOMBA_WING_FALL_USED_TRIO_BINDING.md)
pins reached93105a14 to original krb_get_dir@unit_patakuri.o, not distance.
Four float inputs old/newXY and evaluated prior integer output; dx/dy bothzero
writespriorangle, else systemangleABf→fctiwz→integer output. Exact REL imports,
zero constant, raw helper hash and used groundattack306f4 five locals recorded.
Semantic provider reuse with Spike/Headbonk validated; no invented sqrt/pow or
names-only alias. Docs-only/no native edits/commit; git diff --check passed.


### 2026-10-13 delivery: battle missing PTK_R_2 source fallback art

[Wing-fall resource contract](../behavior/battle/PARAGOOMBA_WING_FALL_USED_TRIO_BINDING.md)
pins shared battle animPoseSetAnim exact-search/exhaustion→descriptor0 policy.
Original a/c_kuribo row0 KUR_Z_2, descriptor6b74, nonloop0..20/one key0,
three visibility updates only, visible static135-triangle geometry. PTK_R_2 is
absent; KUR_R_2 exists and is a separate original10-frame run. Bank/hash/source
provenance and host evaluated geometry bounds recorded; numerical host adapter
not claimed as original raster observation. Native subset fallback must retain
source KUR_Z_2 rather than current packfirst KUR_S_1. Docs-only/no native edits
or commit; git diff --check passed.


### 2026-10-13 delivery/request: original trio ending controller

[Ending contract](../behavior/battle/UNDERGROUND_TRIO_VICTORY_CONTROLLER_AND_FIELD_RETURN.md)
pins exact32-entry end-bank jump table, distinct F/I/NPC setup owners, fresh
constructor/preserved F3c path and alternate actual callback/resume branch.
Closed00/05/06 order, source audience Great/Fast bonus+complete AddTargetNum/
Sub float ABI/clamps, Koura originalchild/freshU10420clear leaf, resetcolorU311ff,
ordered per-ID SetStatus0..24 with Charge retained/19,20move recalculation,
owned-event barriers/remaining all-unit3f keys, DropMaterial owner and source
EXP/reward/export/fade/publication/field continuation dependencies. Original
MarioM_V_2/GoombellaPKR_Y_1 victory scripts identified. No result/pouch/story
completion fabricated from nativeHP0. Next bounded requests: generalF3c producer
outsidefreshscope, exactDropMaterial RNG/equipment/fielddrop consumers, remaining
end display/effect/audio/EXP/rank/level services and actual field reconstruction.
Docs-only/read-onlyISO, no native edits/commit; git diff --check passed.


### 2026-10-13 delivery/request: complete trio DropMaterial and pouch prerequisites

[Drop/entry-pouch contract](../behavior/battle/UNDERGROUND_TRIO_DROP_MATERIAL_AND_ENTRY_POUCH.md)
closes full801141bc HP→FP→item pass, source current unit ratios/upper-only cap,
actual five used rows, exact draw order and314 badge queries/fallback25 words.
SourceI14 coin versusS14 flower owner corrected in ending contract. Original
setup item producers drawSpike/Para229, selected/retained/back-item slots,
fieldgetter masks/311,312 badge RNG/countcoin hierarchy and generic death
consumer/actualtrio separate admission pinned. Sourcestg0_00 callback grants
6/0x32/9, source keyinsert/tier writes and freshRevise/import/partnerFP facts
closed. Actualordinarytitle/newgame/boots6 lifecycle remains bounded request:
onlyseq_e3Main directstage-table query callers were found, not production
newgame proof. Native providedentrypouch is explicit persistent-owner adapter,
not authorization to reset liveHPFP or invent save ownership. Sourcehashes,
listings/fieldinventories recorded; no native edits/commit; git diff --check.


### 2026-10-13 delivery/request: trio EXP source presentation/counting

[EXP contract](../behavior/battle/UNDERGROUND_TRIO_EXP_PRESENTATION_AND_COUNTING.md)
pins08FogForceStop/StockExp1..100/fortune/level gates, samecall09timer119,
0a actualvictorychildren/PointGet,0b originalheldbattlepad300 counting/RNGfree
pouch96 progression. CompleteDispMain has noE0=Bf24 write; sourceWhistle E0
mustnotberepaired. All19staricon fields/init/Main/end and original404 four
40pxframes418..421 delay6, sourceframewindowrequest pinned. PointGet phase11
cos easing/hold/softdelete and originalUSphrase+digit texture124..131/US6..13
sourceart inspected. MarioM_V_1/2 b_mario andGoombellaPKR_Y_1 c_pkr verified;
type43actualcategory69→PKR_S_1 clarified, notfirstrow/directoryfallback.
Remaining exact innericon clock/window/GXquad matrices/GPU viewport/parity,
MSLcos numeric provider, completeeffectaudio and laterEXP/level/export/field
providers remain boundedrequests. Docs-only/no native edits/commit;
git diff --check validation.

### 2026-10-13 delivery: fresh ten-point no-level-up ending tail

[Ordinary EXP ending tail](../behavior/battle/UNDERGROUND_TRIO_NO_LEVEL_UP_ENDING_TAIL.md)
pins per-invocation0c..1f, ordinary skip0f, five-call effect pointer release
independent of PointGet leave/deallocation, exact voice/bodytype42 gates,
source fade10 (200ms, strict ratio>1, GP4 refresh, selected-slot flags2),
ordered actualunit→pouch export and field-party reconstruction. Rule0 writes
I19=6, not invented return2. Normal ending does not heal HP/FP or directly
unlock/story-advance; original battle/sequence/field wait remains required.
Other level-up, zeroEXP, fortune-end, fade/GX/audio/field constructor services
remain bounded dependencies. Read-only original bodies/constants; docs-only,
no native edits/build/commit; git diff --check.

### 2026-10-13 delivery: original EXP icon creation clock/window frame

[Icon clock and frame](../behavior/display/EXP_ICON_CLOCK_AND_TYPE0_WINDOW_FRAME.md)
pins iconEntry2D memset/localframe0/currentimage418/delay6, per-record Main
countdown/wrap/reload and hiddenclock advancement. Original outerupdate
creates in battleMain then decrements in iconMain samepass. Windowtype0
uses original f/msgWindow.tpl descriptor0 (72x72CMPR), exactnine-slice36vertices
UV0/thirds/1, color*texture alpha andsourcecamera. Native raster/palette/camera
services remain explicit; no globalclock or substitute rectangle needed.
Readonly bodies/descriptor/TPL header and180floatFIFO symbolic census;
docs-only/no native edits/build/commit; git diff --check.

### 2026-10-13 delivery: complete used US PointGet layout/lifecycle

[PointGet](../behavior/effect/POINT_GET_US_LAYOUT_AND_LIFECYCLE.md) pins original
36-byte payload, creation/update same-outer-pass ownership, pre-update depth
versus live draw XYZ,45enter/softdeletehold/60leave. CompleteUS10 widthL322/
scale200/322, two ordered passes/black128shadow offsets4,-4 versuswhiteFG,
FOREGROUNDdimensions forbothpasses, exactphrase/digit cursor/quad/UV matrices
and originaldigit1.42857146 scale. No generatedlayout or extraeffectcompletion
barrier. Displaycallback inherits blend/cull/depth fromoriginaldispatcher;
sourcecamera/GXsampling/numeric/raster/provider lifetimes remainbounded.
Read-only whole localbodies/constants;docs-only/no native edits/build/commit;
git diff --check.

### 2026-10-13 delivery: independent EXP count/pad/icon-end audit

[Count follow-up](../behavior/battle/UNDERGROUND_TRIO_EXP_PRESENTATION_AND_COUNTING.md#2026-10-13-independent0a0b0c-audit)
pins0a noE4reset/nofallthrough; sourceheldB1034&300; onceonly1000fastlatch;
onepointpercall, originalSFX_STAR_POINT_GET1 onlytransferscounterlow3zero;
thresholdcheckevenno-transfer, actualP96reset/E0c/P8a+1>=99. Lasttransfer
remains0b;nextcallsets0c delay. Freshnormal10delay100/fiveSFX;firstheld
delay10/oneSFX. ExpIconEnd clearsE30mask2, iconDelete onlyactive1bit, retains
all19E pointers/recordbytes; nofree/wholememset. Docs-only/read-only/no native
edits/build/commit; git diff --check.

### 2026-10-13 delivery/request: ordinary trio field-return handshake

[Field handshake](../behavior/battle/UNDERGROUND_TRIO_FIELD_RETURN_HANDSHAKE.md)
pins source sequence5→6/counter>30/GP alternatives, seq-game originalretained
fade presets/constructor9/300, BattleExit ordered originalresource/context/event
restoration and actualwaitseq2&&I10nonzero. Authoredgor_01 christine_nakama
374b0 evt_pouch_party_join(1)→P0eOR1 belongsjoininghistory, not1f/newgame
heal. marioCtrlOn byte38nestedcount, globalpartyCtrlOnBOTHslots20000000,
Postoriginalappearance/flags andunchanged161-commandmessage/story12tail.
Nextboundednativeproviders: BattleEnd800f8da4/transitivecontextrelease/field
rootpause-resume; actualpartyEntry/HelloMain/partyGetAppearPos800c6c9c;
originalGPpresets/sequenceexit-init ordering and Progresspouchjoin/saveowner.
Read-onlycompletebodies/tables/joinrecord/nativeAPIcheck; docs-only/no native
edit/build/commit; git diff --check.

### 2026-10-13 delivery: original used fade10 texture/NPC namespace

[Fade10 consumer](../behavior/display/BATTLE_END_FADE10_TEXTURE_CONSUMER.md)
pins completeused disp_texture: embedded1x1I4FF80415020, NO externalTPL/palette
lookup, identityUV/matrix, alphaByte=low8trunc(single255*slotratio), blackRGB,
sourcealpha blend, camera8projection/view,608-squarequad±304 andprojection
restore. Type10doesnotcompletedflag2skip; solidblackprimitivecanmatchthis
usedwhite-texture*blackmaterialunderdeclaredcamera/coverageadapter. Actual
GX/camera/clip/rasterparity andotherfadeart remainbounded. Ending1c loadsF8
thenNPC140OR100 at8021654c..54, separatefromNPC0/visibility. Readonlywhole
body/table/constants/texel; docs-only/no nativeedits/build/commit;diffcheckpass.

### 2026-10-13 delivery/request: ending mapfree/fieldparty fresh constructor

[BattleFree/End](../behavior/battle/ENDING_HEAP_FREE_AND_REMAINING_UNIT_TEARDOWN.md)
closesBattleFree(pointer):nullskip,actualmapheapbase8041e80c/_mapFree,notSDK
__memFree; header32/live8/size4/next0 orderedneighborcoalescing, noEpayload
clear. LaterBattleEnd64slotliveDelete thenicon/menu/stage/audience/breakslot,
optionalB1c78free/Bfree/globalB0.
[Constructorcomposition](../behavior/partner/FIELD_PARTY_POSITION_CONSTRUCTORS.md#2026-10-13-battle1f-ordinary-goombella-constructor-composition)
resolveskind1c_pkr/PKRS1versusHP<=5PKRD2,f4=26,length56,partyEntryposition
helper2twiceviafreshHelloMain10,not16-countsubstate0jump. ExistingMain/general
appearance/fallbackcontractsauthority; GetAppearPos2_800c6718 remainingfull
typedray/classification/caller contract/nativeproviderrequested, plusactual
registry/allocator/model/context. Readonlywholebodies;docs-only/no nativeedit
build/commit; git diff --check.

### 2026-10-13 delivery: source E BattleAlloc/mapheap bootstrap pairing

[Mapallocationbinding](../behavior/memory/BATTLE_END_MAP_ALLOCATION_BINDING.md)
pinsending05actualcall802154e0→BattleAlloc800f8934(2ac),not88d8; wrapper
usesroot8041e80c/_mapAlloc8002ff58. Complete32-byte roundedbestfit/firsttie/
mandatorysplit/zero/DCflush andE2ac→2c0 payload/2e0 span, nestedsourcearena
64000/root63fe0. Existing eventmapallocation/memorybootstrap/subpoolcontracts
areauthority; parentprimaryrootandSDKheaders remainseparate. Correctfreepair
usesinnerroot; no12-byteSDKheader, noE-onlyarenaoriginalhistoryclaim or
staledouble-free/idempotenceassumption. Docs-only/read-only/no nativecode
build/commit; git diff --check.

### 2026-10-13 delivery/request: preservedtrio postwait callbacks/nativegaps

[Returnaudit](../behavior/story/UNDERGROUND_FIRST_ARRIVAL_AND_GOOMBA_TRIO.md#2026-10-13-preserved-native-return-callback-audit)
correctstag61fieldwait/62prev-partyrestore(no62triousage); orderedpost093/
094_kur/094_01extparty/story12sourcegraph;fieldNPCtribePTK persistsbattle
wingfall anddamageforce1. Concretecurrentnativegaps27/29partyRunStopdefault
success,54a_recnotapplied,70BGMon_fwithoutactualfadeowner; originalfield
MarioWord4/currentindices/partyconstructor/controlcounter/pouchjoinand
separatefieldVMpausedchildren ownershiprequired. Noforcedunlock orstorywrite.
Read-onlysource/nativeaudit; docs-only/no nativecode/build/commit;diffcheckpass.
