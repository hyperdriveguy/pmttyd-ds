# First underground Goomba trio: setup4 and return ownership

Read-only US G8ME01 disc0 revision0 static REL/DOL evidence, scoped to
[OW-ROAD-01](../../requests/OW-ROAD-01.md) and
[AC-01](../../requests/AC-01.md). No original execution or native gameplay
parity claim. See [field scene](../story/UNDERGROUND_FIRST_ARRIVAL_AND_GOOMBA_TRIO.md)
for crossing and pre/postbattle messages, and existing
[field battle entry/return](../field/FIELD_BATTLE_ENTRY_AND_RETURN.md) for the
complete common callback/data-provider contract. This document adds the actual
used tik encounter, not a replacement harness.

## Setup identity, group and units

The field script sets NPC`クリボー１` battleinfo4 at tikREL0x140e0, then
battle_start0x140f0 and wait_battle_end0x140fc. Numeric setup is module-owned:
`npcSetBattleInfo`0x80043d80 selects current module through map prefix, uses
index stride0x44 and stores source-selected NPC battle data. Numeric4 alone
cannot identify an encounter across gor andtik modules.

`btl_tik_setup_no_tbl`REL0x28b38 binds BTLNO_TIK_01_01→4. Setup tablebase
0x28340, selected record0x28450, size0x44, name`tik01クリボー３人衆`,
map`tik_01`, musicBGM_ZAKO_BATTLE1. Raw record+0xc=-1,+0x18=0x20000010;
flag consumers remain separately owned, so do not invent their meaning.
Record+0x14→weightedtable0x27fb0, whose weight50 entry→group0x26460 and
stage0x2619c. Weighted entry+0xc onward is zero terminator. Keep source
RNG consumption even though only one group has positive weight.
Stage record source strings are`bti_01`,`stg_00_1`, count5 and objectpointer
0x25c08; complete stageobject rendering/services remain open.

Group0x26460 has **count4**, unitarray0x263a0, each0x30bytes. Group raw+8/+c0,
+0x10=100,+0x14→battle_heart_drop_param_default0x803702a0,
+0x18→battle_flower_drop_param_default0x80370f20,+0x1c=5.
Enemy group includes tutorial Frankly as a coordinator; he is not an ordinary
fourth foe. All four setup bytes+4=1, word+8=0x04000004 and zero unitWork.
Source-record order and setup positions:

| Array record | Kind pointer/type | XYZ | Item table |
| --- | --- | --- | --- |
| 0x263a0 | unit_kuriboo0x2a7d8/type1 | (30,0,-10) | battle_item_kurikuri0x80417900 |
| 0x263d0 | unit_togekuri0x30c30/type3 | (70,0,0) | battle_item_togekuri0x8036f0b8 |
| 0x26400 | unit_patakuri0x2f578/type2 | (110,40,10) | battle_item_patakuri0x8036f108 |
| 0x26430 | unit_lecture_frankli_kuriboo0x3d990/type0xb4 | (150,0,20) | null |

Frankly init immediately hides him, and entry event later places him on Mario's
side. Group placement is not final visible scene placement.

## Stats, body flags and weapons

`BtlUnit_Entry` DOL0x80128fe0 reads signedkind+8 into maxHP andcurrentHP at
0x80129248..264; kind+0xa supplies FP at0x80129268..280. Source ordinary
bodies have part1, modelc_kuribo; Frankly modelc_kuri_h. Every used defense
and defenseattribute array is fivezero bytes. These are five elemental slots,
not five separate body parts.

| Kind | HP/FP | Danger/peril/level/bonus bytes | Unit attribute+0xac | Part attribute+0x40 | Part counter+0x44 |
| --- | --- | --- | --- | --- | --- |
| Goomba | 2/0 | 1/1/7/0 | 0x00040000 | 0x00000009 | 0 |
| Paragoomba | 2/0 | 1/1/7/0 | 0x00040004 | 0x00600809 | 0 |
| SpikedGoomba | 2/0 | 1/1/8/0 | 0x00040000 | 0x00000009 | 1 |
| FranklyB4 | 10/0 | 1/1/1/0 | 0x12000000 | 0x00890009 | 0 |

Existing decomp enum names identify Paragoomba unitbit4=floating, part0x800
winged,0x200000 hammerlikecannotTarget,0x400000 shellTosslikecannotTarget;
Spiked partcounter1=topspiky. Franklypart0x10000 notTargetable and0x80000
untattleable. Unnamed bits retain raw identity; their complete runtime consumers
are not implied by these labels. Defenses do not make spike Jump safe, and
raw airpart targetability cannot be replaced by a generic ground foe.

| Weapon record | HPdamage function/work0 | Special+0x74 | Raw+0x68 | Raw+0x7c |
| --- | --- | --- | --- | --- |
| Goomba0x2a9c0 | weaponGetPowerDefault0x80189d30 /1 | 0x5087 | 0x20001000 | 0x80002004 |
| Paragoomba0x2f664 | same /1 | 0x5087 | 0x20000000 | 0x80002000 |
| Spiked0x30d68 | same /2 | 0x5087 | 0x20001000 | 0x80002004 |

All three weaponrecords0xc0bytes, no FPdamage function, no attackevt at+0xb0;
the unitattackevent explicitly supplies this weapon. Allraw+0x64=0x01101260,
+0x6c=0x00000302,+0xb4=100. Base damage is a source function parameter,
not a guaranteed postguard/status result. Exact original PreCheckDamage and
CheckDamage arguments are self-2,targetLW3,partLW4,weapon,0x100,resultLW5.
Player Jump/Hammer/Goombella attack commands and guard input remain their
separate source providers; do not reuse fixed Crump tutorial outcomes.

Paragoomba datatable0x2f808 tag0xe→dmg_fall_event0x2f914. It clears
unitfloating4 andpartwinged0x800,1000ms, changes pose/data/parts to grounded
Goomba versions, setsunitflag4, falls toY0, changeshome,500ms, ChangeKind1,
child groundedinit and resumespose. A surviving damaged flying unit must lose
wings through this source branch rather than retaining air-target restrictions.
Spiked datatable0x30ec0 tag0x25→dmg_avoid_counter_thorn_head_event0x30f80;
see conditional instruction messages below. Activation is owned by the common
damageevent dispatcher, not a new input proximity rule.

## Frankly entry and first-turn lesson

Unlike the optional gor setup3 lesson, tik4 is the mandatory trio encounter
and its separate coordinator is tutorialtype0xb4. Franklyinit binds
wait/entry/phase/damage/confusion scripts, publishes unitID to battlework+0x424,
unitWork0=0, positions(0,-1000,0), faces1, setsmaxmovecount0 and startswait.
`lecture_init` RELtext0x7e18 is a trivial return2; the inventory's speculative
ASCII identifier`8`` comes from code bytes and must not replace the symbol.

Entry waits900ms, positions(-250,0,-30), requestsR_1, moves
(-60,0,-30,45,-1,0), requestsS_1. Phase first checks0x04000001 then turn==1;
only that branch stops Mario(-3)wait, setsMarioM_S_1, FranklytalkT_1/stayS_1,
statusoff, and prints092_01 explicit battle speaker-2(selfFrankly).
It pans to spike(71,49,213;target71,35,0;30frames,type11), waits30frames,
msg_toge(1,0,0,0),092_02. It pans to airborne(111,85,213;111,71,0;30,11),
waits30frames,msg_toge,092_03. Franklyfaces-1,camera mode0,60frames,092_04;
faces1,1500ms,faces-1,mode3,camera(-60,82,340;-60,60,13;30,11),30frames,
092_05,faces1,statuson,camera0. Later turns skip this first-turn instruction.

Spiked counter-reaction waits30frames, sourceeffecttoge_flush, resolves special
unit-6(FranklyB4), requires validunit andhisunitWork0==0. get_attacker examines
battlework+0x420 attacker's currentKind: Mario0xde→0,Goombella0xe0→1,
other→2. Other attackerreturns immediately. Otherwise2500ms,statusoff,
Franklyfaces-1, Mario branch092_06 orGoombella092_07, writesFranklywork0=1,
faces1,statuson. Thus the first spike mistake has original conditional Frankly
instructions exactly once per battle; these are not alternate arbitrary prose.

## Battle start and return protocol

The existing field-battle contract is directly reused: initial battle_start
stores NPCowner, marioFBattlePrepare, Mario/activeparty controlsoff,
BattleInformation result0, psndSetFlag0x10, seqSetSeq(4,null,null), returns0.
Resumed firstCall0 returns2. The next-command-header extra operandread is
present here as in Crump and discarded by fbatGetPointer; do not invent a
second eventparameter.

wait_battle_end0x8004eed8 returns0 until **sequence2 AND result!=0**.
Only then MarioCtrlOn,activepartyCtrlOn,L_psndBGM_stop,marioFBattlePost and
return2. No local victory-only test or result clearing exists. Loss/escape
sequence routing and resultproducer meanings remain required dependencies;
this predicate alone does not authorize defeated-player field continuation.
After actualreturn the original161command fieldscene owns93/94/94_01,
escapeanimations andstory12. A native setup4pending state must preserve that
scene event and battledata owner through entry, player turns and return.

## Original instructional message metadata

All seven used IDs below live in msg/US/tik_01.txt and use explicit battle
Frankly attribution, not print_party suffixes. Retain source bytes/controltokens;
no story text is copied into this contract. Tokens listed are lexical census.

| Key | Gate | Bytes | SHA256 | Unique controls |
| --- | --- | --- | --- | --- |
| mac_0_092_01 | firstturn | 91 | `1a0561fa1bea9fe7ed8a00ec3e02222d533e5bb504baf4f2094389e08b9c1933` | `<k>`, `<wait 100>` |
| mac_0_092_02 | firstturn | 193 | `5b20b318c864ab1320e3c361021616767d65cdc441391ce3f202c9f6a3ed1cfa` | `<k>`, `<p>`, `<wait 250>` |
| mac_0_092_03 | firstturn | 98 | `cb1815e3f059de7700dd95743e0568baefafc0aa4bd0d08b0e052026843b2b46` | `<k>`, `<wait 300>` |
| mac_0_092_04 | firstturn | 94 | `cb1c7778813fbe4d3a7f1cf3c1c54cd716ce082c6e77769e4b649cab5b6075ec` | `<k>`, `<wait 250>` |
| mac_0_092_05 | firstturn | 265 | `d436258dd9719788a79296a4cc99be1bcb921527f86ef399c91435f44ad974cc` | `<icon PAD_Y_ON 0.5 -20 35 -10>`, `<k>`, `<p>`, `<wait 250>` |
| mac_0_092_06 | firstMario spikecounter | 110 | `72684d316d074c4246ed55769fd43f3d61cc9d7e90f1dbcbfdfb85c083379bb5` | `<k>`, `<wait 250>`, `<wait 300>` |
| mac_0_092_07 | firstGoombella spikecounter | 105 | `6d6c9ed281094a156a61730ac046784526052023bfb473de16cafde814069df2` | `<k>`, `<wait 250>`, `<wait 300>` |

## Unit event inventories

Offsets are original RELfile offsets. Hex retains variable tokens/raw masks.
Strings and localcode are distinguished by annotated symbol identity.

### init_event@unit_kuriboo.o

REL0x2aa80/a8, 10commands, SHA256`bc6586b11b7f987f3fcb147a98d6a4628e4b5c20df39b2ad74a51a8c56914986`.

| Offset | Opcode | Ordered operands |
| --- | --- | --- |
| 0x2aa80 | 91 | `btlevtcmd_SetEventWait` · `0xfffffffe` · `wait_event` |
| 0x2aa90 | 91 | `btlevtcmd_SetEventAttack` · `0xfffffffe` · `attack_event` |
| 0x2aaa0 | 91 | `btlevtcmd_SetEventDamage` · `0xfffffffe` · `damage_event` |
| 0x2aab0 | 91 | `btlevtcmd_SetEventConfusion` · `0xfffffffe` · `attack_event` |
| 0x2aac0 | 91 | `btlevtcmd_SetRunSound` · `0xfffffffe` · `SFX_ENM_KURI_MOVE1` · `SFX_ENM_KURI_MOVE2` · `0x00000000` · `0x00000006` · `0x00000006` |
| 0x2aae0 | 91 | `btlevtcmd_SetWalkSound` · `0xfffffffe` · `SFX_ENM_KURI_MOVE1` · `SFX_ENM_KURI_MOVE2` · `0x00000000` · `0x00000006` · `0x00000006` |
| 0x2ab00 | 91 | `btlevtcmd_SetJumpSound` · `0xfffffffe` · `SFX_ENM_KURI_JUMP1` · `SFX_ENM_KURI_LANDING1` |
| 0x2ab14 | 91 | `btlevtcmd_StartWaitEvent` · `0xfffffffe` |
| 0x2ab20 | 2 |  |
| 0x2ab24 | 1 |  |
### init_event_patakuri@unit_patakuri.o

REL0x2f8ac/68, 8commands, SHA256`ec9b8f14f84966793e7e1441cbcd68470ac26a963419901a6070942af02d7413`.

| Offset | Opcode | Ordered operands |
| --- | --- | --- |
| 0x2f8ac | 91 | `btlevtcmd_SetEventWait` · `0xfffffffe` · `wait_event_patakuri` |
| 0x2f8bc | 91 | `btlevtcmd_SetEventAttack` · `0xfffffffe` · `attack_event_patakuri` |
| 0x2f8cc | 91 | `btlevtcmd_SetEventDamage` · `0xfffffffe` · `damage_event` |
| 0x2f8dc | 91 | `btlevtcmd_SetEventConfusion` · `0xfffffffe` · `attack_event_patakuri` |
| 0x2f8ec | 91 | `btlevtcmd_AnimeSetPoseSoundTable` · `0xfffffffe` · `0x00000001` · `pose_sound_timing_table` |
| 0x2f900 | 91 | `btlevtcmd_StartWaitEvent` · `0xfffffffe` |
| 0x2f90c | 2 |  |
| 0x2f910 | 1 |  |
### init_event@unit_togekuri.o

REL0x30ed8/a8, 10commands, SHA256`bc6586b11b7f987f3fcb147a98d6a4628e4b5c20df39b2ad74a51a8c56914986`.

| Offset | Opcode | Ordered operands |
| --- | --- | --- |
| 0x30ed8 | 91 | `btlevtcmd_SetEventWait` · `0xfffffffe` · `wait_event` |
| 0x30ee8 | 91 | `btlevtcmd_SetEventAttack` · `0xfffffffe` · `attack_event` |
| 0x30ef8 | 91 | `btlevtcmd_SetEventDamage` · `0xfffffffe` · `damage_event` |
| 0x30f08 | 91 | `btlevtcmd_SetEventConfusion` · `0xfffffffe` · `attack_event` |
| 0x30f18 | 91 | `btlevtcmd_SetRunSound` · `0xfffffffe` · `SFX_ENM_TOGEKURI_MOVE1` · `SFX_ENM_TOGEKURI_MOVE2` · `0x00000000` · `0x00000006` · `0x00000006` |
| 0x30f38 | 91 | `btlevtcmd_SetWalkSound` · `0xfffffffe` · `SFX_ENM_TOGEKURI_MOVE1` · `SFX_ENM_TOGEKURI_MOVE2` · `0x00000000` · `0x00000006` · `0x00000006` |
| 0x30f58 | 91 | `btlevtcmd_SetJumpSound` · `0xfffffffe` · `SFX_ENM_KURI_JUMP1` · `SFX_ENM_KURI_LANDING1` |
| 0x30f6c | 91 | `btlevtcmd_StartWaitEvent` · `0xfffffffe` |
| 0x30f78 | 2 |  |
| 0x30f7c | 1 |  |
### init_event@unit_lecture_frankli_kuriboo.o

REL0x3daf8/d4, 15commands, SHA256`6106887abe25a16049800f8e1b773b000178cbd277ce71ee7fca201e1cc46522`.

| Offset | Opcode | Ordered operands |
| --- | --- | --- |
| 0x3daf8 | 91 | `lecture_init` |
| 0x3db00 | 91 | `btlevtcmd_SetEventWait` · `0xfffffffe` · `wait_event` |
| 0x3db10 | 91 | `btlevtcmd_SetEventEntry` · `0xfffffffe` · `entry_event` |
| 0x3db20 | 91 | `btlevtcmd_SetEventPhase` · `0xfffffffe` · `phase_event` |
| 0x3db30 | 91 | `btlevtcmd_SetEventDamage` · `0xfffffffe` · `damage_event` |
| 0x3db40 | 91 | `btlevtcmd_SetEventConfusion` · `0xfffffffe` · `btldefaultevt_Confuse` |
| 0x3db50 | 91 | `btlevtcmd_GetUnitId` · `0xfffffffe` · `0xfe363c80` |
| 0x3db60 | 91 | `_battle_set_lecture_unit_id` · `0xfe363c80` |
| 0x3db6c | 91 | `btlevtcmd_SetUnitWork` · `0xfffffffe` · `0x00000000` · `0x00000000` |
| 0x3db80 | 91 | `btlevtcmd_SetPos` · `0xfffffffe` · `0x00000000` · `0xfffffc18` · `0x00000000` |
| 0x3db98 | 91 | `btlevtcmd_ChangeFaceDirection` · `0xfffffffe` · `0x00000001` |
| 0x3dba8 | 91 | `btlevtcmd_SetMaxMoveCount` · `0xfffffffe` · `0x00000000` |
| 0x3dbb8 | 91 | `btlevtcmd_StartWaitEvent` · `0xfffffffe` |
| 0x3dbc4 | 2 |  |
| 0x3dbc8 | 1 |  |
### entry_event@unit_lecture_frankli_kuriboo.o

REL0x3dbcc/74, 7commands, SHA256`a8fb267d3a525ebefdf5c294eb9116910ac6b2af582eb0d95a89d73b0276c746`.

| Offset | Opcode | Ordered operands |
| --- | --- | --- |
| 0x3dbcc | 10 | `0x00000384` |
| 0x3dbd4 | 91 | `btlevtcmd_SetPos` · `0xfffffffe` · `0xffffff06` · `0x00000000` · `0xffffffe2` |
| 0x3dbec | 91 | `btlevtcmd_AnimeChangePose` · `0xfffffffe` · `0x00000001` · `R_1` |
| 0x3dc00 | 91 | `btlevtcmd_MovePosition` · `0xfffffffe` · `0xffffffc4` · `0x00000000` · `0xffffffe2` · `0x0000002d` · `0xffffffff` · `0x00000000` |
| 0x3dc24 | 91 | `btlevtcmd_AnimeChangePose` · `0xfffffffe` · `0x00000001` · `S_1` |
| 0x3dc38 | 2 |  |
| 0x3dc3c | 1 |  |
### phase_event@unit_lecture_frankli_kuriboo.o

REL0x3dc40/280, 39commands, SHA256`3501ee925659cc4c41187c638bab2c779e31dc51a90a36da1cfeb0668cc2b83a`.

| Offset | Opcode | Ordered operands |
| --- | --- | --- |
| 0x3dc40 | 91 | `btlevtcmd_CheckPhase` · `0xfe363c80` · `0x04000001` |
| 0x3dc50 | 25 | `0xfe363c80` · `0x00000000` |
| 0x3dc5c | 91 | `btlevtcmd_get_turn` · `0xfe363c80` |
| 0x3dc68 | 24 | `0xfe363c80` · `0x00000001` |
| 0x3dc74 | 91 | `btlevtcmd_StopWaitEvent` · `0xfffffffd` |
| 0x3dc80 | 91 | `btlevtcmd_AnimeChangePose` · `0xfffffffd` · `0x00000001` · `M_S_1` |
| 0x3dc94 | 91 | `btlevtcmd_SetTalkPose` · `0xfffffffe` · `T_1` |
| 0x3dca4 | 91 | `btlevtcmd_SetStayPose` · `0xfffffffe` · `S_1` |
| 0x3dcb4 | 91 | `btlevtcmd_StatusWindowOnOff` · `0x00000000` |
| 0x3dcc0 | 91 | `evt_msg_print` · `0x00000002` · `mac_0_092_01` · `0x00000000` · `0xfffffffe` |
| 0x3dcd8 | 91 | `evt_btl_camera_set_mode` · `0x00000000` · `0x00000003` |
| 0x3dce8 | 91 | `evt_btl_camera_set_moveto` · `0x00000000` · `0x00000047` · `0x00000031` · `0x000000d5` · `0x00000047` · `0x00000023` · `0x00000000` · `0x0000001e` · `0x0000000b` |
| 0x3dd14 | 9 | `0x0000001e` |
| 0x3dd1c | 91 | `evt_msg_toge` · `0x00000001` · `0x00000000` · `0x00000000` · `0x00000000` |
| 0x3dd34 | 91 | `evt_msg_print` · `0x00000002` · `mac_0_092_02` · `0x00000000` · `0xfffffffe` |
| 0x3dd4c | 91 | `evt_btl_camera_set_moveto` · `0x00000000` · `0x0000006f` · `0x00000055` · `0x000000d5` · `0x0000006f` · `0x00000047` · `0x00000000` · `0x0000001e` · `0x0000000b` |
| 0x3dd78 | 9 | `0x0000001e` |
| 0x3dd80 | 91 | `evt_msg_toge` · `0x00000001` · `0x00000000` · `0x00000000` · `0x00000000` |
| 0x3dd98 | 91 | `evt_msg_print` · `0x00000002` · `mac_0_092_03` · `0x00000000` · `0xfffffffe` |
| 0x3ddb0 | 91 | `btlevtcmd_ChangeFaceDirection` · `0xfffffffe` · `0xffffffff` |
| 0x3ddc0 | 91 | `evt_btl_camera_set_mode` · `0x00000000` · `0x00000000` |
| 0x3ddd0 | 9 | `0x0000003c` |
| 0x3ddd8 | 91 | `evt_msg_print` · `0x00000002` · `mac_0_092_04` · `0x00000000` · `0xfffffffe` |
| 0x3ddf0 | 91 | `btlevtcmd_ChangeFaceDirection` · `0xfffffffe` · `0x00000001` |
| 0x3de00 | 10 | `0x000005dc` |
| 0x3de08 | 91 | `btlevtcmd_ChangeFaceDirection` · `0xfffffffe` · `0xffffffff` |
| 0x3de18 | 91 | `evt_btl_camera_set_mode` · `0x00000000` · `0x00000003` |
| 0x3de28 | 91 | `evt_btl_camera_set_moveto` · `0x00000000` · `0xffffffc4` · `0x00000052` · `0x00000154` · `0xffffffc4` · `0x0000003c` · `0x0000000d` · `0x0000001e` · `0x0000000b` |
| 0x3de54 | 9 | `0x0000001e` |
| 0x3de5c | 91 | `evt_msg_print` · `0x00000002` · `mac_0_092_05` · `0x00000000` · `0xfffffffe` |
| 0x3de74 | 91 | `btlevtcmd_ChangeFaceDirection` · `0xfffffffe` · `0x00000001` |
| 0x3de84 | 91 | `btlevtcmd_StatusWindowOnOff` · `0x00000001` |
| 0x3de90 | 91 | `evt_btl_camera_set_mode` · `0x00000000` · `0x00000000` |
| 0x3dea0 | 4 | `0x00000063` |
| 0x3dea8 | 33 |  |
| 0x3deac | 33 |  |
| 0x3deb0 | 3 | `0x00000063` |
| 0x3deb8 | 2 |  |
| 0x3debc | 1 |  |
### damage_event@unit_lecture_frankli_kuriboo.o

REL0x3dec0/28, 5commands, SHA256`f59a6bfb7c00767c28083d3812e89e8a081a3c9e96ec8e55de2e2537ee1a04ba`.

| Offset | Opcode | Ordered operands |
| --- | --- | --- |
| 0x3dec0 | 50 | `0xfe363c8a` · `0xfffffffe` |
| 0x3decc | 50 | `0xfe363c8b` · `0x00000001` |
| 0x3ded8 | 94 | `btldefaultevt_Damage` |
| 0x3dee0 | 2 |  |
| 0x3dee4 | 1 |  |
### wait_event@unit_lecture_frankli_kuriboo.o

REL0x3dee8/18, 3commands, SHA256`14bb860655a0fa243d510f0c64ef87508cce5f9fac9524eada619e21c22af21e`.

| Offset | Opcode | Ordered operands |
| --- | --- | --- |
| 0x3dee8 | 91 | `btlevtcmd_AnimeChangePoseFromTable` · `0xfffffffe` · `0x00000001` |
| 0x3def8 | 2 |  |
| 0x3defc | 1 |  |
### dmg_fall_event@unit_patakuri.o

REL0x2f914/1ac, 26commands, SHA256`0a767bb88cb548d455c9619528f49eb32f29f595cde619445dadc5ad01b9b83f`.

| Offset | Opcode | Ordered operands |
| --- | --- | --- |
| 0x2f914 | 50 | `0xfe363c8a` · `0xfffffffe` |
| 0x2f920 | 50 | `0xfe363c8b` · `0x00000001` |
| 0x2f92c | 91 | `btlevtcmd_OffAttribute` · `0xfe363c8a` · `0x00000004` |
| 0x2f93c | 91 | `btlevtcmd_OffPartsAttribute` · `0xfe363c8a` · `0xfe363c8b` · `0x00000800` |
| 0x2f950 | 91 | `btlevtcmd_AnimeChangePoseType` · `0xfe363c8a` · `0xfe363c8b` · `0xfe363c8e` |
| 0x2f964 | 10 | `0x000003e8` |
| 0x2f96c | 91 | `btlevtcmd_AnimeSetPoseTable` · `0xfe363c8a` · `0xfe363c8b` · `pose_table` |
| 0x2f980 | 91 | `btlevtcmd_ChangeDataTable` · `0xfe363c8a` · `data_table` |
| 0x2f990 | 91 | `btlevtcmd_ReplaceParts` · `0xfe363c8a` · `0xfe363c8b` · `parts` · `0x00000001` |
| 0x2f9a8 | 91 | `btlevtcmd_OnUnitFlag` · `0xfe363c8a` · `0x00000004` |
| 0x2f9b8 | 50 | `0xfe363c8b` · `0x00000001` |
| 0x2f9c4 | 91 | `btlevtcmd_AnimeChangePoseType` · `0xfe363c8a` · `0xfe363c8b` · `0x00000027` |
| 0x2f9d8 | 91 | `btlevtcmd_snd_se` · `0xfffffffe` · `SFX_ENM_FALL1` · `0xf1194d80` · `0x00000000` · `0xf1194d80` |
| 0x2f9f4 | 91 | `btlevtcmd_GetPos` · `0xfe363c8a` · `0xfe363c80` · `0xfe363c81` · `0xfe363c82` |
| 0x2fa0c | 50 | `0xfe363c81` · `0x00000000` |
| 0x2fa18 | 91 | `btlevtcmd_SetFallAccel` · `0xfe363c8a` · `0xf24a7e80` |
| 0x2fa28 | 91 | `btlevtcmd_JumpPosition` · `0xfe363c8a` · `0xfe363c80` · `0xfe363c81` · `0xfe363c82` · `0x0000000a` · `0xffffffff` |
| 0x2fa48 | 91 | `btlevtcmd_SetHomePos` · `0xfe363c8a` · `0xfe363c80` · `0xfe363c81` · `0xfe363c82` |
| 0x2fa60 | 91 | `btlevtcmd_snd_se` · `0xfffffffe` · `SFX_ENM_KURI_MOVE1` · `0xf1194d80` · `0x00000000` · `0xf1194d80` |
| 0x2fa7c | 10 | `0x000001f4` |
| 0x2fa84 | 91 | `btlevtcmd_ChangeKind` · `0xfe363c8a` · `0x00000001` |
| 0x2fa94 | 94 | `init_event` |
| 0x2fa9c | 9 | `0x00000001` |
| 0x2faa4 | 91 | `btlevtcmd_AnimeChangePoseType` · `0xfe363c8a` · `0xfe363c8b` · `0x00000027` |
| 0x2fab8 | 2 |  |
| 0x2fabc | 1 |  |
### dmg_avoid_counter_thorn_head_event@unit_togekuri.o

REL0x30f80/1fc, 40commands, SHA256`85aeba836c1a07e3cdc860a5b744794daf97a8a7f364e8629a86d74b059b9ab1`.

| Offset | Opcode | Ordered operands |
| --- | --- | --- |
| 0x30f80 | 9 | `0x0000001e` |
| 0x30f88 | 91 | `btlevtcmd_GetPos` · `0xfffffffe` · `0xfe363c80` · `0xfe363c81` · `0xfe363c82` |
| 0x30fa0 | 91 | `btlevtcmd_CheckStatus` · `0xfffffffe` · `0x0000000a` · `0xfe363c83` |
| 0x30fb4 | 25 | `0xfe363c83` · `0x00000000` |
| 0x30fc0 | 53 | `0xfe363c81` · `0x00000032` |
| 0x30fcc | 4 | `0x0000005a` |
| 0x30fd4 | 33 |  |
| 0x30fd8 | 91 | `btlevtcmd_CheckStatus` · `0xfffffffe` · `0x0000000b` · `0xfe363c83` |
| 0x30fec | 25 | `0xfe363c83` · `0x00000000` |
| 0x30ff8 | 53 | `0xfe363c81` · `0x0000001e` |
| 0x31004 | 4 | `0x0000005a` |
| 0x3100c | 33 |  |
| 0x31010 | 53 | `0xfe363c81` · `0x00000028` |
| 0x3101c | 4 | `0x0000005a` |
| 0x31024 | 3 | `0x0000005a` |
| 0x3102c | 91 | `evt_eff` · `zero_tik_0000cb6c` · `toge_flush` · `0x00000000` · `0xfe363c80` · `0xfe363c81` · `0xfe363c82` · `0x0000003c` · `0x00000000` · `0x00000000` · `0x00000000` · `0x00000000` · `0x00000000` · `0x00000000` · `0x00000000` |
| 0x3106c | 91 | `btlevtcmd_GetUnitId` · `0xfffffffa` · `0xfe363c80` |
| 0x3107c | 25 | `0xfe363c80` · `0xffffffff` |
| 0x31088 | 91 | `btlevtcmd_GetUnitWork` · `0xfffffffa` · `0x00000000` · `0xfe363c80` |
| 0x3109c | 24 | `0xfe363c80` · `0x00000000` |
| 0x310a8 | 91 | `get_attacker` · `0xfe363c80` |
| 0x310b4 | 24 | `0xfe363c80` · `0x00000002` |
| 0x310c0 | 2 |  |
| 0x310c4 | 33 |  |
| 0x310c8 | 10 | `0x000009c4` |
| 0x310d0 | 91 | `btlevtcmd_StatusWindowOnOff` · `0x00000000` |
| 0x310dc | 91 | `btlevtcmd_ChangeFaceDirection` · `0xfffffffa` · `0xffffffff` |
| 0x310ec | 91 | `get_attacker` · `0xfe363c80` |
| 0x310f8 | 24 | `0xfe363c80` · `0x00000000` |
| 0x31104 | 91 | `evt_msg_print` · `0x00000002` · `mac_0_092_06` · `0x00000000` · `0xfffffffa` |
| 0x3111c | 32 |  |
| 0x31120 | 91 | `evt_msg_print` · `0x00000002` · `mac_0_092_07` · `0x00000000` · `0xfffffffa` |
| 0x31138 | 33 |  |
| 0x3113c | 91 | `btlevtcmd_SetUnitWork` · `0xfffffffa` · `0x00000000` · `0x00000001` |
| 0x31150 | 91 | `btlevtcmd_ChangeFaceDirection` · `0xfffffffa` · `0x00000001` |
| 0x31160 | 91 | `btlevtcmd_StatusWindowOnOff` · `0x00000001` |
| 0x3116c | 33 |  |
| 0x31170 | 33 |  |
| 0x31174 | 2 |  |
| 0x31178 | 1 |  |

## Full attack graphs and remaining implementation dependencies

`attack_event@unit_kuriboo.o` REL0x2ab58/7b0, 131commands, SHA256`e96304a79fc32b4e8f80cbb6e5c34338e3d90046cafebb1ccf1bc680cbf42551`. Callback census: `btlevtcmd_AnimeChangePoseType`, `btlevtcmd_AttackDeclare`, `btlevtcmd_CalculateFaceDirection`, `btlevtcmd_ChangeFaceDirection`, `btlevtcmd_CheckDamage`, `btlevtcmd_CheckToken`, `btlevtcmd_ChoiceSamplingEnemy`, `btlevtcmd_EnemyItemUseCheck`, `btlevtcmd_FaceDirectionAdd`, `btlevtcmd_FaceDirectionSub`, `btlevtcmd_GetEnemyBelong`, `btlevtcmd_GetFirstAttackTarget`, `btlevtcmd_GetHitPos`, `btlevtcmd_GetHomePos`, `btlevtcmd_GetPos`, `btlevtcmd_GetPosFloat`, `btlevtcmd_JumpContinue`, `btlevtcmd_JumpPosition`, `btlevtcmd_JumpSetting`, `btlevtcmd_MoveDirectionAdd`, `btlevtcmd_MovePosition`, `btlevtcmd_PayWeaponCost`, `btlevtcmd_PreCheckDamage`, `btlevtcmd_ResetFaceDirection`, `btlevtcmd_ResultACDefence`, `btlevtcmd_SamplingEnemy`, `btlevtcmd_SetMoveSoundDataWork`, `btlevtcmd_SetMoveSpeed`, `btlevtcmd_SetRotate`, `btlevtcmd_StartAvoid`, `btlevtcmd_StartWaitEvent`, `btlevtcmd_WaitGuardMove`, `btlevtcmd_WeaponAftereffect`, `btlevtcmd_check_battleflag`, `btlevtcmd_snd_se`, `evt_btl_camera_add_zoom`, `evt_btl_camera_nomove_y_onoff`, `evt_btl_camera_set_homing_unit`, `evt_btl_camera_set_mode`, `evt_btl_camera_set_moveSpeedLv`, `evt_btl_camera_set_zoom`, `evt_btl_camera_shake_h`, `krb_get_dir`.

`attack_event_patakuri@unit_patakuri.o` REL0x2fb0c/61c, 99commands, SHA256`bb16a121b9a2a3d05075edb9ba65473d718ed15072974f6cf4b552220ee29f1a`. Callback census: `btlevtcmd_AnimeChangePoseType`, `btlevtcmd_AttackDeclare`, `btlevtcmd_CalculateFaceDirection`, `btlevtcmd_ChangeFaceDirection`, `btlevtcmd_CheckDamage`, `btlevtcmd_CheckToken`, `btlevtcmd_ChoiceSamplingEnemy`, `btlevtcmd_DivePosition`, `btlevtcmd_EnemyItemUseCheck`, `btlevtcmd_FaceDirectionAdd`, `btlevtcmd_FaceDirectionSub`, `btlevtcmd_GetEnemyBelong`, `btlevtcmd_GetHitPos`, `btlevtcmd_GetHomePos`, `btlevtcmd_PayWeaponCost`, `btlevtcmd_PreCheckDamage`, `btlevtcmd_ResetFaceDirection`, `btlevtcmd_ResultACDefence`, `btlevtcmd_SamplingEnemy`, `btlevtcmd_SetMoveSoundDataWork`, `btlevtcmd_SetMoveSpeed`, `btlevtcmd_StartAvoid`, `btlevtcmd_StartWaitEvent`, `btlevtcmd_WaitGuardMove`, `btlevtcmd_WeaponAftereffect`, `btlevtcmd_snd_se`, `evt_btl_camera_add_zoom`, `evt_btl_camera_nomove_y_onoff`, `evt_btl_camera_set_homing_unit`, `evt_btl_camera_set_mode`, `evt_btl_camera_set_moveSpeedLv`, `evt_btl_camera_set_zoom`, `evt_btl_camera_shake_h`.

`attack_event@unit_togekuri.o` REL0x31310/954, 162commands, SHA256`7ffecd92c5b3f826c170e9b2bc461f0dc98172fd30ddc8973544b285c7a92657`. Callback census: `btlevtcmd_AddRotate`, `btlevtcmd_AddScale`, `btlevtcmd_AnimeChangePose`, `btlevtcmd_AnimeChangePoseType`, `btlevtcmd_AttackDeclare`, `btlevtcmd_CalculateFaceDirection`, `btlevtcmd_ChangeFaceDirection`, `btlevtcmd_CheckDamage`, `btlevtcmd_CheckToken`, `btlevtcmd_ChoiceSamplingEnemy`, `btlevtcmd_EnemyItemUseCheck`, `btlevtcmd_FaceDirectionAdd`, `btlevtcmd_FaceDirectionSub`, `btlevtcmd_GetEnemyBelong`, `btlevtcmd_GetHitPos`, `btlevtcmd_GetHomePos`, `btlevtcmd_GetPos`, `btlevtcmd_GetPosFloat`, `btlevtcmd_GetRotate`, `btlevtcmd_JumpContinue`, `btlevtcmd_JumpPosition`, `btlevtcmd_JumpSetting`, `btlevtcmd_MoveDirectionAdd`, `btlevtcmd_MovePosition`, `btlevtcmd_PayWeaponCost`, `btlevtcmd_PreCheckDamage`, `btlevtcmd_ResetFaceDirection`, `btlevtcmd_ResultACDefence`, `btlevtcmd_SamplingEnemy`, `btlevtcmd_SetDispOffset`, `btlevtcmd_SetMoveSoundDataWork`, `btlevtcmd_SetMoveSpeed`, `btlevtcmd_SetRotate`, `btlevtcmd_SetRotateOffset`, `btlevtcmd_SetScale`, `btlevtcmd_StartAvoid`, `btlevtcmd_StartWaitEvent`, `btlevtcmd_WaitGuardMove`, `btlevtcmd_WeaponAftereffect`, `btlevtcmd_snd_se`, `evt_btl_camera_add_zoom`, `evt_btl_camera_nomove_y_onoff`, `evt_btl_camera_set_homing_unit`, `evt_btl_camera_set_mode`, `evt_btl_camera_set_moveSpeedLv`, `evt_btl_camera_set_zoom`, `evt_btl_camera_shake_h`, `krb_get_dir`.

The static attack inventories are reproducible with rel_event_inventory and
are not compressed into invented impact constants. Complete unitattack
movement/animation clocks, guard/action-command pads and result, effect/camera/
message services, playercommand menus and Goombella scripts, stage/audience/
items/rewards, damageengine counterdispatch and battle result/sequence teardown
still need native composition and original runtime checks. These requested
services remain documentation inbox dependencies; recovering data does not
close setup4 gameplay fidelity. No nativecode edits, build or runtime tests were
performed in this documentation task.
