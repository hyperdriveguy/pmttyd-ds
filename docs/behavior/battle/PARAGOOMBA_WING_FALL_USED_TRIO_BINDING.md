# Paragoomba wing fall: used underground trio binding

2026-10-13 documentation-only recovery from the read-only US ISO. This closes
the original kind2 wing-fall path for the underground setup4 encounter; kind3
is Spiked Goomba. Native execution and measured presentation are separate.
The [trio contract](UNDERGROUND_GOOMBA_TRIO_SETUP4_AND_RETURN.md) retains the
complete original event inventories and generic damage bookkeeping.

Evidence: tik REL SHA256
`ca30b06f0b7176dbe364349f239ef3ddfe66650e50ea1246921bd2a368a9dac7`,
`dmg_fall_event@unit_patakuri.o` file2f914/1ac,26 commands, SHA256
`0a767bb88cb548d455c9619528f49eb32f29f595cde619445dadc5ad01b9b83f`;
grounded `init_event` file3033c/a8,10 commands, SHA256
`bc6586b11b7f987f3fcb147a98d6a4628e4b5c20df39b2ad74a51a8c56914986`.
Fresh disassemblies/inventories remain `/tmp/trio-para-offattr.dis`,
`trio-para-offpartattr.dis`, `trio-para-ground-init.json`; prior source leaf
listings are `/tmp/trio-changekind.dis`, `trio-changedata.dis`,
`trio-replaceparts.dis`. Relocated addresses below use the existing native
pack scheme section5 base93500000, file section base da30.

## Admission and original reaction owner

Ordinary Jump weapon W74=35807 and Headbonk W74=35007 each have mask4000.
The [damage orchestrator](BATTLE_DAMAGE_ORCHESTRATION.md) admits part reaction
when this weapon mask and victim P1ac800 are set. It adds reaction code400000
**only when per-hit flags100 are present**. A fresh failed H1 hit reducing
Paragoomba HP2→1 uses full code410117 (400000+10000+100+23). First successful
Jump hit flags20000 does not itself get400000; do not select wing fall from
damage or HP loss alone. The successful follow-up flags20100 retain their
original fall/death graph, including possible lethal damage.

The installed flying damage wrapper is `damage_event@unit_patakuri.o`,
file303e4, relocated935229b4. Like the ordinary Goomba wrapper, it sets
LW10=-2/LW11=1 and runs child generic defaultDamage8034cc60. Normal reaction23
sets LW14=39/LW15=-1. Generic subsetevt_damage80350588 checks full-code400000,
runs data event decimal14 (hex0e), then returns. Flying data table93521dd8
maps this key to fall93521ee4. The tracked victim hit event U2b4 owns the
generic parent and its children; preserve the source owner/locals/part/code
and child completion barrier rather than launching an unrelated animation.

## Ordered used fall stream

All coordinates are source engine coordinates; the native map/render scale
adapter must not change simulation operands.

1. Set LW10=-2, LW11=1. Clear unit attribute4 and part attribute800.
2. Select pose type LW14 (fresh ordinary reaction39 gives PTK_D_1), then
   WAIT_MSEC1000. Do not immediately substitute the grounded pose.
3. Set pose table93522728, data table935227c8, then ReplaceParts with
   table93522800 and new partID1. Set unit flag4; set LW11=1.
4. Select pose39, now KUR_D_1. Play original SFX_ENM_FALL1 with the source
   snd_se operands (-2,key,sentinel,0,sentinel).
5. GetPos into LW0..2, set LW1=0; SetFallAccel eventfloatf24a7e80 (=1);
   JumpPosition(actor,LW0,LW1,LW2,10,-1). SetHomePos to those coordinates.
6. Play SFX_ENM_KURI_MOVE1; WAIT_MSEC500. ChangeKind(actor,1).
7. RUN_CHILD grounded init9352290c; WAIT_FRAME1; select pose39; RETURN/END.

Replacement and pose recreation occur before descent and before ChangeKind.
The original stream does not reset HP, move count, actor registry identity,
original kind definition, reward metadata or unit home X/Z.

## Data, original art and installed grounded events

| Record | Flying source / relocated | Grounded source / relocated |
| --- | --- | --- |
| Kind definition | 2f578 /93521b48, kind2 | No replacement of U10 |
| Part | 2f724 /93521cf4 | 30230 /93522800 |
| Pose table | 2f770 /93521d40 | 30158 /93522728 |
| Data table | 2f808 /93521dd8 | 301f8 /935227c8 |
| Init event | 2f8ac /93521e7c | 3033c /9352290c |
| Damage event | 303e4 /935229b4 | Same wrapper |
| Grounded attack / wait | — | 30414 /935229e4;30bc4 /93523194 |

Both part records have resource bank key `c_kuribo` at RELfilec86c. Their
part names change `btl_un_patakuri` (c7f8)→`btl_un_kuriboo` (c9c4).
This is a real original part/animation transition within the same resource
bank, not evidence for a different invented character bank.

| Pose type | Flying key | Grounded key |
| --- | --- | --- |
| 28,69 | PTK_S_1 | KUR_S_1 |
| 39 | PTK_D_1 | KUR_D_1 |
| 42 | PTK_R_1 | KUR_R_1 |
| 41 | PTK_R_2 | KUR_R_2 |
| 40 | PTK_W_1 | KUR_W_1 |

Both part definitions use local position(0,0,0), hit(0,22,0), sampling(0,30,0).
Flying attributes00600809 first become00600009 after OffPartsAttribute800;
ReplaceParts then installs attributes9, removing air-target restrictions as
well as wing bit. Grounded defense table30208 and defense-attribute30210
replace flying2f63c/2f644; counter attributes remain0. Do not retain the old
floating/air part admission after the real replacement.

Ground init installs wait93523194, attack935229e4, damage935229b4 and confusion
**the same grounded attack935229e4**. It sets run and walk sounds
(KURI_MOVE1 c918,KURI_MOVE2 c9d4,0,6,6), jump sounds (KURI_JUMP1 c9e8,
KURI_LANDING c9fc), then StartWaitEvent. These original key strings and poses
must be included in locally extracted packs. Native renderer currently uses
the combined underground animation pack for these actors; actual scene pose
and current part resource/table must drive the drawing after replacement.
An actor-slot PTK prefix or changing only currentkind cannot reproduce this.

### Original missing PTK_R_2 and descriptor0 geometry

The authored flying pose type41 requests PTK_R_2, but original US `a/c_kuribo`
contains no exact name PTK_R_2. The bank has45 name rows; header1ac points to
directory6034 whose row0 is **KUR_Z_2**, animation descriptor6b74. Original
bank size125524, SHA256
`9b934dee83eb489f5151d8a497ac59611e402b7db786c9e82b4809d7d7a475e4`;
local provenance `local-assets/field-actors/c_kuribo.provenance.json` identifies
read-only ISO path `a/c_kuribo`, source offset886507548.

Battle btlDispMain80102a2c/40 calls the same animPoseSetAnim80041170 used by
ordinary field actors. Its exact strcmp search over64-byte rows selects
index0 on exhaustion. Force0 plus same selected index preserves animation
clock/cache state; this is not a missing-name failure or retry. See
[selector/restart](../display/ANIMATION_SELECTOR_AND_RESTART.md) and the
[battle pose setter](UNDERGROUND_GOOMBA_TRIO_SETUP4_AND_RETURN.md#used-explicit-named-battle-pose-setter).
Thus missing PTK_R_2 must preserve original KUR_Z_2 fallback, not KUR_S_1,
PTK_R_1 or an invented PTK_R_2. Resource subsets must retain source descriptor0
as their fallback despite a different packing order for other animations.

KUR_Z_2 is nonlooping, start0/end20, with only one sparse key at0. It has
three visibility updates and zero position, normal, texture or node updates.
Its geometry is therefore static over this interval and visible. Existing
host `character_animation_data.animation/evaluate` plus
`character_bake.geometry` yields135 triangles with texture indices0,2,6,8,9
at source times0,10,20. Source-coordinate bounds (host numeric matrix adapter,
not observed original draw) are X[-5.348030,5.831209],
Y[-0.708966,12.955863], Z[-0.120490,0.174001].
KUR_R_2 exists separately: looping0..10, eight keys0,4,5.5,6,7,7.5,8,10;
458 position updates,40 node updates,3 visibility updates. Import it for the
grounded type41 request, while retaining KUR_Z_2 for the original missing
flying name. Do not render the missing animation as an invisible actor.

## Small leaf ABI and preserved packed fields

| Callback | Operand treatment and used effect |
| --- | --- |
| OffAttribute8010e368/64 | Evaluate/translate actor; RAW maskarg1; U104 &= ~mask; return2 |
| OffPartsAttribute8010e1b8/84 | Evaluate/translate actor, evaluate part; RAW maskarg2; P1ac &= ~mask; return2 |
| OnUnitFlag80105b40/60 | Evaluate/translate actor; RAW mask; BtlUnit_OnUnitFlag801277bc/10 ORs U1c (not U104 or U108) |
| AnimeSetPoseTable80106468/70 | Evaluate actor/part; RAW table→P1bc |
| ChangeDataTable8010dae0/5c | Evaluate actor; RAW table→U18 |
| ChangeKind8010dc44/6c | Evaluate actor/newkind; write ONLY U8 |
| SetHomePos801123d0/128 | Evaluate actor/X/Y/Z with evtGetValue in order; translate actor; signed32 XYZ→float; leaf801289a0/10 writes U30/34/38 only |
| ReplaceParts8011319c/168 | Evaluate actor/oldpart/newpartID; RAW newtable; replace matching source record, then btlDispEntAnime(unit) |

The attribute wrappers have no first-call branch or output, and do not
evaluate their mask operands. ChangeKind preserves U4 original kind and U10
kind definition; reloading kind1 wholesale would erase original retained
state. ReplaceParts sets P4=newrecord/P8=record4, P1ac/1b0=record40/44,
P1b4/1b8=record38/3c, positionP24/28/2c=recordc/10/14; resets part rotation
and pivot; writes P170/174=record18/1c, P17c/180=0, P1a0=0. It preserves
P178 hitZ, local scale/base angles/sampling offsets and already-written P1bc
in this wrapper. It always calls btlDispEntAnime for all part pose IDs.
The used first record has ID1; source mismatch loops without advancing.
Native preallocated pose/atlas ownership can be an explicit allocation
adapter while honoring this recreation and current pose/resource selection.

SetHomePos has no coordinate sentinel checks, first-call path or output.
It does not call evtGetFloat, set current position U3c/40/44, apply offsets
or perform a floor query. The original wrapper converts the signed integer
results using xoris/4330 subtraction before storing single-precision home
coordinates. Its used GetPos-produced LW0..2 remain the integer source
destination. Source listings `/tmp/trio-para-sethome.dis` and
`trio-para-sethome-leaf.dis` close this newly reached native fall boundary.

The grounded init installs through these exact immediate return2 wrappers.
Every actor argument is evaluated and translated. Script operands are RAW;
the sound keys are evaluated through evtGetValue, unlike those scripts.

| Callback | Remaining operands and packed writes |
| --- | --- |
| SetEventWait8010a500/5c | RAW script→U28c |
| SetEventAttack8010a388/64 | RAW script→U2a0; U2ac=0 |
| SetEventDamage8010a55c/5c | RAW script→U2b0 |
| SetEventConfusion8010a32c/5c | RAW script→U2a4 |
| SetRunSound80104f64/bc | Evaluate keys arg1/2→U1cc/1d0; evaluate numeric arg3/4/5→low16 U1d4/1d6/1d8 |
| SetWalkSound80104ddc/bc | Evaluate keys arg1/2→U1dc/1e0; evaluate numeric arg3/4/5→low16 U1e4/1e6/1e8 |
| SetJumpSound80104b08/80 | Evaluate keys arg1/2→U1c4/1c8 |

These wrappers do not start/cancel events, load sound mode, stop voices or
clear other movement counters. StartWaitEvent is a separate final command.
Source disassemblies are `/tmp/trio-para-btlevtcmd_SetEvent*.dis`,
`trio-para-btlevtcmd_Set*Sound.dis` and `trio-para-onunitflag.elf`.

## Recovery, conclusion and field return boundaries

### Grounded attack local angle helper

Grounded attack935229e4 uses callback **93105a14**, which original symbols
identify as `krb_get_dir@unit_patakuri.o`, not a distance helper. File5ad8,
source loaded805c0478,size118, raw SHA256
`6b29c916c6ccddb8362c36dcc8ce3b75d2a0690d75ac5277789f586251862ac9`.
Original command file306f4 has operands LW5,LW6,LW9,LW10,LW8:
oldX,oldY,newX,newY,raw angle-output token. Grounded attack full131-command
inventory `/tmp/trio-para-ground-attack.json` SHA256
`e96304a79fc32b4e8f80cbb6e5c34338e3d90046cafebb1ccf1bc680cbf42551`.

First four operands use evtGetFloat80030840; fifth uses evtGetValue80031260
to retain its previous integer angle. Compute dx=newX-oldX/dy=newY-oldY
with single fsubs. Both compare equal to float0: write the prior integer
angle back to the raw fifth output token. Otherwise systemangleABf8000703c
(dx,dy,0,0), fctiwz and evtSetValue80030cfc to that token. Return2 in both
branches. There is no unit translation, sqrt, pow, distance calculation or
evtSetFloat output. This is verified semantic reuse of the already recovered
Spiked93105b2c and Headbonk80189910 angle helper, not a names-only alias.

REL24 sites5b14/24/34/44→evtGetFloat;5b54→evtGetValue;
5b84/5bb4→evtSetValue;5b9c→systemangleABf. ADDR16_HA/LO
5b5a/62 and5b92/96 resolve section4+4bc4,fileca2c,bytes00000000 float0.
Inspection-only listing/ELF `/tmp/trio-para-krbdir.dis/.elf` plus original
relocation stream pin these bindings; raw branch placeholders in an unrelocated
ELF alone are not import targets.

The generic damage parent remains responsible for nonlethal recovery or the
subsequent original death graph. WaitAttackEnd/reaction readiness uses the
original tracked event fields U29c/U2a8/U2b4/U2bc; wait U290 is excluded.
See [waits/conclusion](BATTLE_WAITS_AND_CONCLUSION.md). Registry deletion and
status27 cannot be synthesized from HP0 alone.

Enemy group conclusion excludes units with original masks20000/10000000;
Frankly attributes12000000 remain excluded. Original btlseqEnd80215234
assigns result1 when the enemy side concludes, subject to its higher-priority
flag/own-side branches. Full end-controller/reward/pouch/return ownership
remains a distinct native dependency, not permission to return after fall
or after summing live HP. See [target/turn/victory](UNDERGROUND_TRIO_TARGETS_TURNS_AND_VICTORY.md).

The next concrete owner after last-enemy death/barrier is end bank7,
state07000000 in btlseqEnd80215234, not field wait_battle_end. After its
result1 setter at8021532c, it reads result and fbatGetPointer8004829c.
FieldBattleInfo+3c nonnull selects end07000003 (which later launches that
original event). Null selects `_check_audience_battle_end_exec`8021290c,
end07000005 and `_BattleMarioKouraDelete`80212844, then allocates2ac bytes
through BattleAlloc800f8934→Bf28 and calls btlseqEnd_DispInit802151b4.
Thus the first genuine unresolved used leaf after result selection is the
audience-end admission helper on the null callback branch; the field callback
branch needs its actual FieldBattleInfo+3c event instead. Source listing is
`/tmp/trio-para-end.dis`. These dependencies remain requests, not return2
stubs or evidence that the full victory graph is implemented.

Field wait_battle_end8004eed8 completes only when seqGetSeq==2 and original
result is nonzero; it restores field control and calls original postbattle
services. The original underground161-command scene then owns dialogue
mac_0_093/094/094_01, departing actors and story12. Preserve that source
continuation; see [field entry/return](../field/FIELD_BATTLE_ENTRY_AND_RETURN.md)
and [first arrival](../story/UNDERGROUND_FIRST_ARRIVAL_AND_GOOMBA_TRIO.md).
Original nonempty audience, audio backend, visual parity, complete battle-end
rewards and field return must remain explicit boundaries until implemented.

Validation: read-only source disassembly and existing event/table inventories;
`git diff --check`. Documentation only, no native edits, build rerun or commit.

## Native used-path binding (2026-10-13)

Fresh Headbonk on flying kind2/bodypart1 now admits the original damage path.
The weapon4000/part800/per-hit100 gate adds400000 before shared calculators and
direct application, producing410117/HP1. The installed original damage root
owns fall child completion; no standalone fall timer is invented.
Native OffAttribute/OffPartsAttribute, raw pose/data tables, scoped ReplaceParts,
ChangeKind, integer SetHomePos and original event/sound setters preserve the
listed packed writes. ReplaceParts reuses the preloaded c_kuribo resource/pose
IDs, an explicit one-part native resource adapter; following source pose39
resets animation. Full multiparts/resource allocation/release remains unbound.
Original fall/descent/home/wait/grounded-init ordering now completes. Current
kind becomes1 while original definition93521b48 and HP1 remain. Grounded attack
and wait execute through original scripts, source angle-table krb_get_dir alias
93105a14 and default weapon power1; generic jump numerical services remain
native adapters. Existing no-input player damage/recovery then reaches nextmenu.

Command refresh imports live part attributes and current kind. The source
Goomba name replaces Paragoomba, and the grounded part becomes a Hammer target.
Original KUR_R_2 and descriptor0 KUR_Z_2 are packaged; absent PTK_R_2 retains
shared source fallback rather than invented art. Unit identity/definition, level,
HP/moves and reward metadata are not reset by the transform.

Host integration verifies410117, HP1, kind1/original definition retained, exact
replacement/pose/data/event pointers, floor/homeY0, grounded default attack,
source menu name/target admission and original final10717 death/deferred removal.
With all opponents removed the runtime retains the precise ordinary ending
owner80215234/07000000. Victory/pouch/export/end presentation and original field
return remain required; story11/result0/control lock are preserved at that
boundary. Successful Jump/Headbonk follow-ups, Spike counter/warning, nonfresh
statuses/crowd/effects and original/hardware timing parity remain incomplete.
