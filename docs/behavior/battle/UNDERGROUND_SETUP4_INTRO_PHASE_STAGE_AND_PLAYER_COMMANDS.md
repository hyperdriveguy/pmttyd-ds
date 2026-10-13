# Setup4 introduction, first phase and default player commands

Read-only US G8ME01 revision0 static evidence. Follow-up to
[trio setup4](UNDERGROUND_GOOMBA_TRIO_SETUP4_AND_RETURN.md),
[OW-ROAD-01](../../requests/OW-ROAD-01.md) and[AC-01](../../requests/AC-01.md).
Existing complete generic contracts remain authoritative; this binds their
used source domains. No original execution/timing or native equivalence claim.

## Definition importer and fresh source IDs

Each BattleUnitKind record is0xc4bytes; partCOUNT is unsignedbyte+0xb4,
partTABLEpointer+0xb8, initEVENTpointer+0xbc, dataTABLEpointer+0xc0.
All four tik setup4 kindrecords have count1. Their original partrecords are
Goomba0x2a974, Spiked0x30d1c, Paragoomba0x2f724, Frankly0x3daac,
each0x4cbytes. Partrecord +0=partID1,+4namekey,+8modelkey,+cXYZoffset,
+0x18hitbaseXYZ,+0x38defensearray,+0x3cdefattrarray,+0x40attributes,
+0x44counterattrs,+0x48posetable. Do not parse +0xb4 as a wordcount or use
NPC tribe pose roles as a battle part's numericpose table.

With freshly reset source registry and active Goombella, the existing
[setup caller](BATTLE_UNIT_SETUP_ACTORS_AND_REPEATED_IMPORTS.md) enters
systemkind0x803ade74 first, Mariosecond, Goombellathird, then
[group opponents](BATTLE_UNIT_SETUP_OPPONENT_LOOP_AND_EQUIPMENT_MASK.md).
Complete BtlUnit_Entry allocates the first free slot. Thus scoped fresh IDs:

| ID | Owner |
| --- | --- |
| 0 | unit_system |
| 1 | Mario |
| 2 | Goombella |
| 3 | Goomba |
| 4 | SpikedGoomba |
| 5 | Paragoomba |
| 6 | FranklyB4 |

This is conditional on reset/nointervening-spawn/nonzeroGoombella, not a global
fixed-ID guarantee. Frankly publishes its actual ID to battlework+0x424;
BattleTransID(-6) must use that word. Self-2 uses event's bound owner, Mario-3
uses the original Mario registry lookup. Original source IDs and native arena
slots are separate address domains if the native adapter chooses fewer actors.

## Initialization, entry barrier and first phase order

[btlseqInit](BATTLE_INIT_SEQUENCE_DISPATCH_AND_RESOURCE_READINESS.md) requests
source unitsetup, waits all trackedinit eventIDs, then required effectresources,
and enablesstatus before transitioning channel0=1/channel2=0x02000000.
It does not directly call Franklyentry or print092_01.

Scoped btlseqFirstAct0x8011e5c0/0x7f8 normal entrybranch assumes captured
source firstAttackType0 at setup+8. At0x8011e614 it clearsB+0xefc, starts
_default_haikei_set_entry_event0x80416d70(priority10,flags0), then scans64
registry slots ascending. For each nonnull actor with entryscript+0x2b8 it
starts evtEntry(script,10,0), storesIDunit+0x2bc andbinds event+0x160=unitID.
Source NPCbattle flags0x200000 can omit Mario/partykinds; the exact current
flag producer remains the setup/snapshot owner. Mario motion0x1a selects
its datakey0x41 alternate entry; other ordinary actors use installedentry.
The scan launches entryevents together, not one actor then blockingbeforethe
next. It then setschannel2=0x02000001 and returns.

Normalbranch has eFC0; state1 decrements it, fadeEntry(type23,200ms), source
battle-open sound, channel2=0x02000002. State2 polls eventbarrier and fade;
only bothready advancechannel2. Later status/item/audience/stagewall/announcement
barriers precede finalchannel0=2 andchannel3=0x03000000 at0x8011ed78..da0.
Full btlseqFirstAct alternatefirststrike/otherstates and invokedstageproviders
remain distinct dependencies; do not reduce every entry to onefixedtimeout.

Marioentry0x80372bc0 waits40frames thenwalkpose42/movehome count61;
it polls _get_local_frame for0 or10 beforeM_S_1. Goombellaentry0x80393cb0
waits40frames,walkpose42,speed4,sounddata,MovePosition(home,count0,-2,0),
posefromtable1. Franklyentry waits900ms and then source45stepmove; these
concurrent entry clocks must finish through trackedIDs before phase dialogue.

**MovePosition's fifth eventoperand is count; sixth is soundmode.** Frankly
(-2,-60,0,-30,45,-1,0) suppliescount45,soundmode-1,soundtype0,not speed-1.
[Complete movement contract](BATTLE_MOVE_FRAME_AND_POSITION.md): firstcall
speed=horizontalDistance/count and integrates immediately; every invocation
decrements count, returns0 whilepositive, then snapsallXYZ andreturns1
(advance+yield). Ybaseline staysconstant during steps. From(-250,0,-30),
Franklyhorizontal speed190/45unitspercallback is a derivedsourcecase.

btlevtcmd_SetEventPhase0x8010a448/0x5c resolves/translatesactor but stores
**raw scriptoperand1** to unit+0x298. Frankly's phaseevent therefore belongs
to selector0, notselector1's+0x294 fanout. Existing
[phase table](BATTLE_PHASE_TABLES.md) buildsorderedunitpairs, and
[movecontroller](BATTLE_MOVE_CONTROLLER.md) state4 launchesselector1events,
state5waits, state6 launchesoneorderedselector0event and state7waits/conclusion.
Only after allthose finish doesstate8selectactor/state9enteractioncontroller.

btlevtcmd_CheckPhase0x80113708/0x70 compares RAWsecondoperand to
BattleGetSeq(currentglobalB,4), unsignedequality, writesboolean torawoutput,
returns2. Frankly's0x04000001 is exactchannel4word. Hisphase then checks
turn==1. It neither triggers merelywhenbattleStart nor owns menuinput.
The exact39commandcoordinator in the companion contract owns092_01..05,
status/camera/talkpose and returns before firstplayercommand. Bind source
channelword andturncounter through their realwriters, not dialogue-derived
counter increments. Runtime firstphase order remains to be comparedoriginally.

## Original composite stage resources

Selected stage at tikREL0x2619c binds keysbti_01 andstg_00_1. Their actual
map model/texture containers are below. Both b/*.tpl haveone8×8format14image,
zero compressedpayload, andidentical96bytes; their existence does not replace
the mapcontainers' renderassets. Existing mapconverter can consume m/*/d,t;
composite stage placement/animation/object lifecycle belongs to originalstage
services and requires explicit nativebinding.

| ISO resource | Bytes | SHA256 |
| --- | --- | --- |
| b/bti_01.tpl | 96 | `0320d56c8e97016396a5bc8b37d1127b0302e686186b36d4700ef0cb081c4043` |
| b/stg_00_1.tpl | 96 | `0320d56c8e97016396a5bc8b37d1127b0302e686186b36d4700ef0cb081c4043` |
| m/bti_01/d | 181042 | `213c62e758be15bc0731b3fa58558d1b2c565ea1c96e91111ea07b52cf751ca8` |
| m/bti_01/t | 58976 | `18f1efc8f976ca5c82e5ed325c51432c7f2adc43edd25bd8ce86a60365ef7bc3` |
| m/stg_00_1/d | 82534 | `8d749bd8caad7fced1596231b755dc4e372e938816e10add8a8e0d7a69f5a2e1` |
| m/stg_00_1/t | 62784 | `2d676c3cf7f0efbb3aa972a710f0986e82e1720e01144d6a5e346d9bd0256c2e` |

## Default player weapon ownership and power

Source weapon pointers/eventlinks:

| Command | Weapon | Powercallback | Powerwords+0x20..34 | Event |
| --- | --- | --- | --- | --- |
| MarioJump | 0x803538a8 | weaponGetPowerFromMarioJumpLv0x80189cb0 | 1,1,2,2,3,3 | NormalJump0x80355cf0 |
| MarioHammer | 0x80353de8 | weaponGetPowerFromMarioHammerLv0x80189c38 | 1,2,2,4,3,6 | wrapper0x80416c40→core0x8035d6d4 |
| GoombellaHeadbonk | 0x80393ea8 | weaponGetPowerFromPartyAttackLv0x80189b50 | 1,1,2,2,3,3 | NormalAttack0x803941a8 |

[Mario power contract](BATTLE_WEAPON_POWER_CALLBACKS.md) readsstoredJumpLv/
HammerLv andACstatebit1, thenweaponpowertable, thenattackbonusbyte (Jump2ff,
Hammer300). Atlevel1/bonus0, perhitJump1 eitherstate; Hammer1failure/2success.
These are callbackpowerbeforemodifiers, not fixedtotalHPdamage. OrdinaryJump
[fullgraph](BATTLE_NORMAL_JUMP_EVENT_FLOW.md) ownsrepeatedhits/ResultAC/count
andguards; [Hammerbranch](BATTLE_HAMMER_WEAPON_BRANCHES.md) usesAC3,period25,
rawmode0,difficultyweaponbyte+0x6e3beforeunitadjustments.

Goombella callback translatesattackerunitkind→partyID, capturesACbit1 from
battle+0x1cb8, calls pouchGetPartyAttackLv(partyID), computes
index=2*attackLevel+bit, clampsONLYsigned>=6 to5, returnswordweapon+20+4index.
No Marioattackbonusbyte is read. pouchGetPartyAttackLv0x800d3a8c/0x14 reads
signedhalfwordpouch+14*ID+0xa with no localboundscheck. Atlevel0, Headbonk
perhitpower1 eitherbit. Do not substitute partylevel-1 or techmenu's+0xcfield.
Headbonkweapon+0x64=0x01001260,+0x68=0x20001004,+0x74=0x35007.

Headbonkfull227commandstream0x803941a8/0xce0 SHA256
`77d2f5d30bea8bd4ff16b0d09ac2e942e705f4717e9fb233b1ccfb2166efc37c`:
GetSelectEnemyLW3/LW4 at0x803941c0,CommandGetWeaponAddress(self,LW12)
0x803941e8,AcSetDifficulty andSetupAC(self,1,1,0)0x80394294/a4,
StartAC1 at0x803943a4. PreCheck uses0x100firstpass versus0x100100later;
ResultAC0x80394a10/GetResultAC0x80394a30 precede impactbranches.
Failure branch CommandCheckDamage0x100; count<=1 branch0x20000 then increments
localhitcounter andloops; later branch0x20100; StopAC0x80394be8. Source target/
AC/loop/movement services must preserve those phases; no fixedsinglehit or
fixedimpactframe is established by this inventory.

## Source target selection and next bounded requests

[Weapondecision](WEAPON_MENU_SETUP_AND_DECISION.md) rebuildstable andrequests
BattleSamplingEnemy with selectedweaponmask64/68,currentactor/enemybelong.
[Selectedtarget](BATTLE_SELECTED_TARGET_AND_WEAPON.md) reads retainedunit/part
throughpermutation; the attack does not choose the firstenemy itself. Original
trio partflags distinguishwingedParagoomba0x00600809 (hammer/shelllikecannot
 target), Spikedtopcounter1, FranklynotTargetable0x10000/untattleable0x80000.
These constraints and original weaponmasks feedcandidateproducer; visibleenemy
order alone is insufficient. Player can change Mario/Goombella order through
originalcommands; do not forceoneattackorder fromintrobranch.

Still requested through inbox: exact used target-sampler/geometry/stageowners
andspawn/currentphase/turnwriter execution under tik4, source menu/pouchstarting
rankandloadout, fullHeadbonk/Hammer/partnerinit callbackcomposition andpad/impact
clocks, originalrenderedentry/stagecurtain/camera/audio, result/loss/escape
producer/returntrace. Existinggenericcontracts close manylocalservices but
have not established complete nativeGameororiginalruntimecomposition.
No nativecode edits or build/runtime tests in this documentationtask.

## Stage model transform, selective visibility and home follow-up

Additional US read-only recovery for native stage occlusion,2026-10-13.
`_mapLoad`80028f18/0xbe0 reads original scaleconstant
**0x8041f944=10.0**, requests PSMTXScale(10,10,10) at800294dc..ec,
passes it to mapEntry at800294f0..fc and hitEntry8002950c..14.
`mapDisp`80024b08 similarly reconstructs Scale10 at80024b60..80 before
mapReCalcMatrix. There is no recovered battle-specific translation/lift here.
At native enginecoordinate scale0.01, modelvertexscale0.1 is correct.
Ordinary mapconverter's0.1 must not be replaced by an arbitrary smaller stage
root solely to move actor feet above source floor.

Original `m/bti_01/d` information scene root is `bti_S`, collision`bti_A`;
`m/stg_00_1/d` scene root`S`, collision`A`. Both world_root haveidentitySRT.
bti floor node0x25918=`stg_b_yuka` hasidentitySRT andbbox
(-35,0,-10.5)..(35,0,10); its actual horizontalplane is sourceY0.
Side node0x258b0=`stg_b_yoko` bboxY[-5,-1]; edge0x25848=`stg_b_line`Y[-1,0].
The source battleunitY0 and renderedfloorY0 agree; no source data here supports
raising the entire enemy/Frankly lineup. Root scaling, visibility, authored
transforms, alpha/pass/depth/camera and actor modelplacement are separate causes
when the native image occludes grounded actors.

### Initial hide then selected-descendant reveal

BattleStageObjectInit80149bd0/0x8d4 obtains current map root and clearsflag2.
At80149cec..d88, existing A1,A2,B,C each get mapGrpFlagOn(name,1),
thenmapGrpFlagOff(name,2). It hidesescape_wall at80149d8c..94.
For each selectedobject record,80149f28..30 clearsmask1 for that object's
named group; originalgroupflag owner updates subtree flags independently.
A parent hidden request followed by a selected descendant reveal must not be
represented as an immutable inherited parent suppression.

The source selected setup4 stageobjectarray tikREL0x25c08 hasfive0x18byte
records (field+4 specialunit type2, +6 layer0..3, XYZ+8..10):

| Record | Named subtree | Owning map | Source layer | Additional engine XYZ | Native additional XYZ |
| --- | --- | --- | --- | --- | --- |
| 0x25c08 | isi_1 | stg_00_1 | 1 | (-202,0,-17) | (-2.02,0,-0.17) |
| 0x25c20 | isi_2 | stg_00_1 | 1 | (200,0,-30) | (2,0,-0.3) |
| 0x25c38 | A1 | stg_00_1 | 0 | (0,0,-50) | (0,0,-0.5) |
| 0x25c50 | B_mati | stg_00_1 | 2 | (0,0,0) | (0,0,0) |
| 0x25c68 | C | stg_00_1 | 3 | (0,0,0) | (0,0,0) |

`isi_1` and`isi_2` live underA2, so dropping entireA2 would also drop
source-selected stones. `B_mati` lives underB; `B_kabe` remains hidden.
A1/C are directly selected. An equivalent static admission adapter can render
ONLY selected scene_rootsA1,isi_1,isi_2,B_mati,C fromstg_00_1, retaining their
authored ancestor matrices and adding sourceobjecttranslation; it cannot simply
renderallS, orhideA2/B through converter's inheritedsuppression. Dynamicfall/
wall/intro transformations still require object services instead of a staticpack.

bti01 hidden groups areescape_wall, plusif_body/if_wirewhenno type6ironframe.
At8014a458..484 the noironframe branch setsbit1 onif_body/if_wire; setup4's
five selectedobjects have layers1,1,0,2,3 and no layer6. Existing bti_butai,
frame, floor, theatricalmask andcurtains remain source groups; do not hide them
as a substitute for their original camera/material/animation owners.

### Additional transforms preserve authored hierarchy

BattleStageObjectMain80149798/0x438 maps source current position/offset/rotation
to namedgroup services. mapObjTranslate8001e254/0x13c divides inputXYZby10
at8001e324..338, builds translation matrix and concatenates it onto the
object's additional matrix(+0xac) at8001e33c..348, then marksmatrix flags.
It preserves authored jointSRT. With no dynamicobjectoffsets/rotations yet,
selectedobjectXYZ above is an **additional translation**, not a new absolute
modelroot origin. Preserve stg authoredB=(0,0,-8) andC=(0,0,-10) modelunits;
zero objecttranslation must not recenter those props atZ0. The static converter
scene_root selector walks authoredworldhierarchy and so can preserve these
parents; per-object postconversion placement must add sourceXYZ*.01 only once.

Original _set_haikei_entry_scale8011a4d8/0x50c requests A1/A2scale(0,0,1)
onfirstcall, countdown60; it updates actualobjectscale and source objectdata
through _mapobj_data_touch_scale. A1 animation settles atScale1 for elapsed
count>=40; A2 settles for count>=45. Intro scales affect selected descendants,
including A2stones. They do not change complete bti01 root or groundedunitY.
Full six interpolation segments/source intpl_sub and authoredcurtain `dontyo`
playback remain a requested provider rather than a fabricated fixedscale.

### Actual draw admission, pass/depth and ordering boundary

`mapDisp` iterates original mapobjects, tests runtime flags/visibility and
chooses queueclass/camera/floatingdepth; at800252c0..e4 it queues
mapDispMapGrp/Obj with camera fromobjectbyte+5 andclassretainedr25.
This is not one unconditional background-map followed by a foreground-map.
`btlDispMain`801024f0 queues each admittedbodypart in camera4;
opaque bodyparts at80102cdc..cf8 useclass1, whiletranslucent branch
80102cac..cc8 usesclass2, each withsource projecteddepth helper.
Original queue sorting is in[display queue](../display/DISPLAY_QUEUE_AND_DISPATCH.md)
and[sort implementation](../display/DISPLAY_SORT_IMPLEMENTATION.md).
Full used map class/geometry/depth producer composition remains to be traced.

Original animation draws retaindepthtest: default pass0/2 usesGXSetZMode(1,3,1),
alpha compare(6,128,1,0,0); effectivepass3 usesZMode(1,3,0),SRCALPHA/INVSRCALPHA,
as[innerdraw](../display/ANIMATION_INNER_DRAW_STATE_AND_BUFFERS.md) specifies.
Do not defeat source occlusion by unconditionallydrawing allactors withoutdepth.
Opaque/translucent sourcepasses, including stage maps, must not be duplicated
into both nativepasses; namedvisibility and stageobject transforms remain
necessary after passfiltering. This evidence does not establish all original
map material/culling/alpha outputs or a native renderedpixel comparison.

### Original Mario and Goombella homes

Fixed Mario setup0x80416a50 pointsunitdata_Mario and hasXYZ(-100,0,-5).
Goombella initial setup0x80363790 hasplaceholderXYZ(1000,0,1000), then
Btl_UnitSetup overrideshome andlive position(-140,retainedhomeY0,5).
These originalunits give nativeMario(-1,0,-0.05),Goombella(-1.4,0,0.05).
Do not use their field formation, initialplaceholder or stage props'model10
factor for actor enginecoordinates. PartyhomeY is retainedsourcegetterresult,
not globallyforced0 for allpartners. Actual entryevents move into these homes.

Remaining source-provider requests: precise selectedstageflag initialization,
object additional-matrix composition underlatermotion, scene-scaleintro and
curtainclip, map pass/depth producers and original-rendered occlusion trace.
No native edits or new originalruntime observation in this follow-up.
