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


## Original used hit, death and variant lifecycle (2026-10-13 follow-up)

Evidence: original US DOL inventories `/tmp/trio-default-damage.json` and
`/tmp/trio-dead-events.json`, original tik ADDR32 inventory
`/tmp/trio-hit-wait.json`, and inspected wrappers in `/tmp/trio-*.dis`.
These are local evidence artifacts, not committed assets or executable code.

| Script | Original address/REL file offset | Size | Commands | SHA256 |
| --- | --- | --- | --- | --- |
| btldefaultevt_Damage | DOL8034cc60 | 11b4 | 403 | af10cde9557883a3876e84df7cc1d37616d18e80db154a230fc4ccf034767beb |
| subsetevt_damage | DOL80350550 | f4c | 330 | d14883ed2b076bb278ff5ef7e5f02bab5ceb2a14bb7eb076a00c73646a942b05 |
| subsetevt_dead | DOL8034f558 | 990 | 176 | f31b840dc35e6fdefb3823536b75995fda93b6f28a1f68c3cdfa76aa261a1ba2 |
| subsetevt_dead_core | DOL8034fee8 | 304 | 51 | 6a75b8adcf9bb4bcf3ad521f9185b4e57cfba92f507e2c0bd0af8348fb697f35 |
| subsetevt_dead_core_nospin_norotate | DOL803501ec | 1f4 | 31 | 01d4c4367d38a57a02be712577ff918ae94bd02c6df70f3a032aa25b11f0f023 |

The three installed damage wrappers are tik REL2ab28 Goomba,303e4
Paragoomba,3117c Spiked Goomba. Each is28 bytes/five commands and identical
raw SHA256 f59a6bfb7c00767c28083d3812e89e8a081a3c9e96ec8e55de2e2537ee1a04ba:
SET LW10=-2; SET LW11=1; RUN_CHILD btldefaultevt_Damage; RETURN; END.
The resolved generic child is DOL8034cc60. Their wait wrappers respectively
REL2b308,30bc4,31c64 are18 bytes/three commands: AnimeChangePoseFromTable(-2,1),
RETURN,END; identical raw SHA256
14bb860655a0fa243d510f0c64ef87508cce5f9fac9524eada619e21c22af21e.
Ownership/cancellation of these events follows
[BATTLE_EVENT_DISPATCH.md](BATTLE_EVENT_DISPATCH.md), especially U278 fullcode,
U2b4 hit event and U290 wait event. Do not replace these children with a
standalone HP-change animation.

### Fresh ordinary Goomba hit, alive after application

[Damage calculation](BATTLE_DAMAGE_CALCULATION.md) assigns normal element0
lowcode23 by OR with flags. Direct application starts the installed hit event;
[direct application](BATTLE_DAMAGE_APPLICATION.md) does not set status27.
In genericDamage the fresh absent status/reaction-pattern branches pass through.
Lowcode23 selects the CASE_OR group30,27,26,23 at8034d43c..8034d454:
OnUnitFlag(LW10,1), SET LW14=39,LW15=-1; only damagepattern1 replacesLW14
with38. Spawn brother RunDataEventChild(LW10,10), then RUN_CHILD
subsetevt_damage at8034d4cc. A missing data key must obey the original lookup
service, rather than interpreting the key as an event address.

subsetevt_damage first checks code200000/customkey13, then400000/customkey14;
then the explicit special reaction patterns/custom keys. Fresh ground Goomba
has none. GetDamage >=1 reaches the ordinary fallback at80351084:
AnimeChangePoseType(LW10,LW11,LW14); WAIT_MSEC1000. If LW15 !=-1, set that pose
then WAIT_FRAME30; fresh LW15=-1 skips it. Child returns at803510d4.
Zero damage has a separate lowcode23 branch at80351124 which still selectsLW14
and waits1000ms. Other zero-damage codes instead shake display offsets; do not
use zero damage as a general no-reaction shortcut.

GenericDamage label10 then checks lethal400,revival200,backfire4000 and guard1000.
With those clear, if code100 is clear call WaitAttackEnd at8034db98. Continue
home movement only if currentXYZ differs from homeXYZ. Fresh ordinary Goomba
standing at home skips movement to label102, ResetFaceDirection(LW10),
StartWaitEvent(LW10), RETURN. The damage pose table maps type39 to KUR_D_1
(REL2a89c record); table type1 is KUR_N_1 (not the automatic no-status wait choice). Spiked equivalents TGK_D_1/
TGK_N_1; flying Paragoomba PTK_D_1/PTK_N_1. Ordinary home movement type40
is corresponding *_W_1 and speed is source event float4.

WaitAttackEnd8010a154/74 reads current attacker ID from battle+420, then its
tracked attack eventID U2a8. Missing attacker or zero ID returns2. If evtCheckID
80036604 says active return0 (block). Otherwise clear attackerU2a8 and return2.
This is an attacker-event barrier, not a fixed wait or victim hit-event poll.
Highcode100 skips this barrier; success20000 alone does not.

### Lethal event-owned status27 and deferred removal

When direct application sets lethalcode400, genericDamage first completes its
reaction child, then at8034da98 checks400; WAIT_FRAME30 at8034dac0;
RUN_CHILD subsetevt_dead at8034dac8; RETURN. subsetevt_dead distinguishesMario
kind222 and partner kinds224..230 from the default enemy branch. Ordinary enemy
jumps directly to label1 at8034f6b0; the preceding home-return movement is
player/partner-specific. At8034f6b8 the script calls
OnOffStatus(LW10,27,1,1,1). This is the original status27 producer. It occurs
after damage reaction and30-frame wait, not when HP first reaches zero.

The fresh enemy branch at8034fb30 tests emergency-revival800. With it clear,
check data key49(decimal,hex31); if present ClearAllStatus and child that event,
otherwise RUN_CHILD subsetevt_dead_core at8034fecc. Generic fallback core:

1. Check optional data key50(hex32), run child if present.
2. SetLW0=48; OnUnitFlag(LW10,08000000); soundSFX_BTL_ENEMY_DIE1_1.
3. Repeat48 times: WAIT_FRAME1; AddRotate(LW10,0,-15,0); decrementcounter.
4. SetRotate(LW10,0,0,0); soundSFX_BTL_ENEMY_DIE1_2. GetPos; Z+=10;
   evt_eff kemuri_test type4 with original event-float scalar.
5. GetCoin→StoreCoin; GetExp→StoreExp; GetPos,Z+=10,Y+=30;
   _add_star_point_disp_offset; evt_eff star_point using returned exp.
   Source contains its explicit LOOP exp branch; preserve source scheduling.
6. SetRotateOffset(0,0,0); counter30. Repeat30 times WAIT_FRAME1 then
   AddRotate(LW10,3,0,0); decrementcounter.
7. GetBackItem; ClearAllStatus; CheckDamageCode400. Nonzero calls
   KillUnit(LW10,0), otherwise KillUnit(LW10,2); RETURN.

KillUnit80113304/b8 resolves owner and returns2 if absent. With unit present,
set halfwordU10c HP=0; ClearStatus80127b58. Raw second operand is a bitmask,
not an evaluated event variable in this wrapper. If mode&2, ORunitU104 attribute
20000 and return2. Otherwise BtlUnit_SetStatus80127bac(unit,27,1,1). If
!(mode&1), ORunitU1c flag80000000. Return2. Hence mode0 reassertsstatus27 and
marks deferred deletion, mode1 reassertsstatus27 withoutmarking deletion,
mode2 marks attribute20000 withoutreassertion. BattleCheckUnitBroken800f7b28/5c
later scans64 registered units and BtlUnit_Delete80128e08 when U1c has80000000.
Native must preserve status/event/registry ordering; HP==0 alone cannot stand
in for these producers or conclusion barriers.

OnOffStatus80105e50/bc evaluates unit and duration/strength/on arguments,
uses signed raw lowbyte status operand; on calls SetStatus with signed-lowbyte
duration/strength, off calls SetStatus(status,0,0). Status27 getter reads byteU135
as documented in [status storage](BATTLE_STATUS_STORAGE.md).

### Paragoomba fall and Spiked counter dispatch

The complete tik variant child streams and message speaker keys are already
listed above. Their admission is now tied to the generic damage script:
subsetevt_damage at80350588 checks code400000, calls datakey14(decimal,hex0e)
then RETURN. Flying tableREL2f808 bindskey0e→dmg_fall_eventREL2f914. The
[orchestrator](BATTLE_DAMAGE_ORCHESTRATION.md) sets partReaction for
weaponW74&4000 and partP1ac&800; it ORsF400000 only when F100 is present.
Do not assume every successful Jump immediately enters this child without
preserving original per-hit flags and the original follow-up graph.

Fall child immediately clears unitattribute4 and partattribute800; selects
LW14 and waits1000ms; replaces pose table30158,data table301f8,part record30230
then sets unitflag4. Selectpose39 now KUR_D_1; soundSFX_ENM_FALL1; read current
position,setY0,fallaccel1,JumpPosition(count10,mode-1); set home tothat destination;
soundSFX_ENM_KURI_MOVE1; wait500ms; ChangeKind1; RUN_CHILDgroundinit3033c;
WAIT_FRAME1; selectpose39; return. It is a complete original actor/part/event
data transition, not just aY teleport or hidden-wing draw override.

Fresh Spiked counter dispatch installs fullcode31(hex1f) on the defender:
genericDamage at8034cfac RunDataEventChild(LW10,37decimal,hex25), then GOTO
label10. Spiked data tableREL30ec0 key25→REL30f80 warning child. That child waits
30frames, emits toge_flush at sourceY offset huge50/tiny30/default40, verifies
Franklyunit-6 exists and work0==0. get_attacker reads current attacker:
Mario222→0,Goombella224→1,other→2; other returns withoutwarning. Wait2500ms,
hide statuswindow, Frankly faces-1, recomputeattacker; original US message
mac_0_092_06 forMario,mac_0_092_07 forGoombella, evt_msg_print(2,key,0,-6).
Set Franklywork0=1,face+1,statuswindowon. Original once-per-coordinator flag,
speaker ownership and timing must be retained. This is not precheckresult4;
fresh spike precheck returns1 and laterCheckDamage counter branch returns-1,
as established in [target/counter contract](UNDERGROUND_TRIO_TARGETS_TURNS_AND_VICTORY.md).

### Used explicit named battle pose setter

btlevtcmd_AnimeChangePose80106bb8/88 resolves unit/part/name then calls
BtlUnit_SetAnim80127688/24. SetAnim explicitly passes force0 to
btlDispChangeAnime80101168/94. ChangeAnime unconditionally strcpyname→P1c4,
ORP204 bits1|2; clear80,200,10; force0 clears8. There is no old-name comparison
or animation-clock reset in the setter. btlDispMain801029fc..80102a40 consumes
bit1, clearsit and200, invokes animPoseSetAnim(P1c0,name,force=(P204&8)!=0).
For this used path force0 andsameanimationindex retains selector caches per
[animation restart](../display/ANIMATION_SELECTOR_AND_RESTART.md). Consumer
still resets presentationbyteP214 and branches P215; same name is not a
complete setter no-op. A native forced restart here would be unsupported.

Remaining bounded dependencies: original genericDamage exceptional branches,
customdata-event callback child/barrier semantics, coin/exp/item producers and
star-point effects, status-presentation flags08000000 and their full display
consumer, rotation/camera/audio/effects delivery, registry teardown with live
child events. This follow-up establishes used source paths; it does not claim
these services are implemented or faithful emulator/hardware observations.


Used child-service addendum: btlevtcmd_RunDataEventChild8010a010/a0 evaluatesunit
andkey, resolves script with BtlUnit_GetData8012733c. Nullscript returns2.
Nonnullscript calls evtChildEntry80036eec(parent,script,0), clears parentbyte+a,
writes childowner+160 with resolvedunitID, returns255 (scheduler yield).
Fresh Goomba data tableREL2a93c and grounded Paragoomba301f8 each contain only
key30(hex48)→DOL80416b38 btldefaultevt_Dummy followed byzero terminator.
Consequently used reactionkey10 and deathoverride49/coreprelude50 are absent
and the callback returns2; no invented generic customscript. Flying tablekey0e
and Spikedkey25 are real callbacks and preserve child owner/barrier semantics.


## Fresh ordinary Goomba CheckDamage tuple and callback boundary

2026-10-13 follow-up; this derived case composes inspected CheckDamage wrapper
8010a78c/fc with the linked original orchestrator, calculator, direct-application,
weapon-power and dispatch contracts. Assumptions: originallevel1MarioJump,
zeroequipment/status/bonus modifiers, normal element0, ordinarysetup4groundGoomba,
no guard/counter/emergency-revival. No source execution measurement is claimed.

CheckDamage event operands are actor,target,part,weapon,flags,destination.
Wrapper evaluates/translatesactor andtarget, evaluatespart→GetPartsPtr, evaluates
weapon; readsflags **raw** at operand+10 and destinationraw+14. Calls
BattleCheckDamage(A,D,P,W,F). Fornegative resultreturns255withoutwritingoutput.
Forresult>=0 evtSetValue(destination,result),then evtCheckID(callingeventID):
active→2, absent→255. Ordinaryvictimdispatch doesnotcanceltheMarioevent and
therefore theusedreturn is2 with destinationvalue18, not18 asVMhandlerreturn.

No precheck occurs inside this callback. Source ordinaryGroundGoomba has
counterattributes0; normalweaponpart-reactionpairs do not overlap: partflags9
lack1000/2000/800. Thus partReaction0. Original powercallback fromlevel1table
is1 regardless ofTimingAsuccessbit; FPelement/callback0 givesQ0,patternM0,
statusBits0, sleepRecovery1. The concrete direct tuple is
BattleDamageDirect(MarioU0,D,P,1,0,F|23,0,1). Source callandmutationordering:

1. SetD258=P beforecounterselection.
2. HPthenFPcalculator sharecode; D.S16U260 increment with16bitnarrowing afterboth.
3. Statusweaponrequest iscalledonlyifF100 (or40000000) andnot40000; these
   ordinaryweapon zero-statusfieldsproduce0. Success20000skipsthisrequest.
4. AudienceEnemyDamage(1), then directtuple above.
5. Direct writesD270=1,D264+=1,D271=D272=0,D268+=0; HPsnapshotD208,
   signedhalfwordHPsubtract/clamp, HPafterD20c. F100controlsD1fe bit1; otherbits
   preserved. Damagecode maygainlethalbits as tablebelow, then D27c|=1 andhitdispatch.
6. Afterdirectreturns, attackerwordA280 increments; newlowbyte→D273. Result18
   when no counterconsumer. Neither calccode nor thisresultmeansstatus27 isalreadyset.

| Per-hit callerF | Direct inputC | HP2→1 finalC | HP1→0 finalC |
| --- | --- | --- | --- |
| 100 (timingfailure/finalhit) | 117 | 117 | 717 |
| 20000 (firsttimingsuccess) | 20017 | 20017 | 20017 |
| 20100 (secondtimingsuccess/finalhit) | 20117 | 20117 | 20717 |

HP<=0 gains200|400 onlywhenC100 ispresent; C20000alonecanleavezeroHP without
status/deathadmission. The native adapter mustretainoriginalflags ratherthan
inferdeathfromHP. Source normal successfultwohits are20000then20100, HP2→1→0;
success/failureare20000then100 withthesameper-hitpower1. Firstfailure100 only
HP2→1 andendsattack. See [originalJumpgraph](BATTLE_NORMAL_JUMP_EVENT_FLOW.md)
foractualsubsequentprecheck andtargetadmission; donotguarantee asecondhit manually.

DispatchfreshGoomba damagepointerU2b0=REL2ab28 from initSetEventDamage.
BattleRunHitEvent8012984c permits lowcode23 withouttheonlylowcode40/datatable46
specialgate. Directdispatcher801298d0 cancels andclearsD2b4,D290,D29c,D2a8,
inthatorder; constructor evtEntry(script,10,20) zerosrootlocals andassignsnewID.
Dispatcher writes eventowner+160=GoombaU0, D278=completeC,D2b4=newhitID.
Damagewrapper thenSETLW10=-2,LW11=1; genericchildcopieslocals/owner. There isno
injectedpart/codeLW seed: bodypart1comesfromwrapper, actualhitpartD258comesfrom
CheckDamage, fullcodeis sharedD278 readbyCheckDamageCode/GetDamageCode.
U29c istrackedphaseeventID, notdamageID; BattleRunPhaseEvent uses294/298script,
evtEntry(script,10,20),ownerU0,newID→U29c; itdoesnotcanceloldphasefirst.

FirstsuccessGoombareaction startsD2b4 independentlyofMarioattack. Itwaits1000ms,
thenWaitAttackEnd becauseC100clear. WaitAttackEnd polls currentattackerB420→A2a8
andblockswhileactive. Secondhitdispatchcancelsfirstreactionchildchain before
startingnewreaction withlatestD278. Final20100/100skipWaitAttackEnd; lethal400
enterssource30frame/status27/deadcorepathpreviouslydocumented. Same-passappend
flag20doesnotmean immediateexecution; preserveschedulerordering andparentchild
copyback. A280/D273 writesfollowhitconstructor, notprecedeit.

### Minimal used genericDamage/deadcore service catalog

The list below is a bounded freshordinarygroundvictim graph, including guards
that are evaluated tofalse. Movement-backhome and variant/revival/electric/
freeze/dodgy/guard/customstatus branches expand thecatalog; they mustremain
explicitlyunbound until supported. OriginalVMalso needsSET,conditionals,
SWITCH/CASE_OR,labels/goto,WAIT_MSEC/FRAME,RUN_CHILD,brother,LOOP andreturn/end.

| Callback | USaddress | bytes | Used role |
| --- | --- | --- | --- |
| btlevtcmd_SetRotate | 80111720 | 372 | entryreset/deathspinreset |
| btlevtcmd_CheckDamageCode | 8010f2e8 | 140 | highbitadmission/sharedD278 |
| btlevtcmd_CheckDamagePattern | 8010f0e8 | 140 | freshfalsepattern guards |
| btlevtcmd_GetDamageCode | 8010f374 | 112 | sharedD278lowbyte switches |
| btlevtcmd_OnUnitFlag | 80105b40 | 96 | reaction1/death08000000 |
| btlevtcmd_RunDataEventChild | 8010a010 | 160 | optionalreaction10/customdeath49/prelude50 |
| btlevtcmd_CheckDataOfDataTable | 80109f88 | 136 | zeroterminatedkeypresence |
| btlevtcmd_GetDamage | 8010f458 | 116 | storedHPdamageD270 |
| btlevtcmd_AnimeChangePoseType | 80106b20 | 152 | type39damage;waittableis separate |
| btlevtcmd_WaitAttackEnd | 8010a154 | 116 | currentattackerA2a8barrier |
| btlevtcmd_CheckAttribute | 8010e430 | 140 | ground/homebranch |
| btlevtcmd_GetPos | 80113620 | 232 | homecompare/effectplacement |
| btlevtcmd_GetHomePos | 801124f8 | 196 | homecompare |
| btlevtcmd_ResetFaceDirection | 8010e4bc | 112 | posthitreset |
| btlevtcmd_StartWaitEvent | 8010a26c | 100 | neworiginalwaitscript |
| btlevtcmd_GetUnitKind | 8010ed8c | 108 | deathkindbranch |
| btlevtcmd_OnOffStatus | 80105e50 | 188 | originalstatus27producer |
| btlevtcmd_AddRotate | 801115f8 | 296 | 48spin/30tipframes |
| btlevtcmd_SetRotateOffset | 80110f9c | 296 | deathpivotreset |
| btlevtcmd_snd_se | 80104700 | 188 | originaldeathsoundkeys |
| evt_eff | 8006d970 | 2572 | smoke/starpointoriginaleffectrequests |
| btlevtcmd_GetCoin | 8010d914 | 108 | rewardamount |
| btlevtcmd_StoreCoin | 8010d8d8 | 60 | rewardaccumulator |
| btlevtcmd_GetExp | 8010d9bc | 108 | rewardamount |
| btlevtcmd_StoreExp | 8010d980 | 60 | rewardaccumulator |
| _add_star_point_disp_offset | 80113f88 | 372 | starpointplacement |
| btlevtcmd_GetBackItem | 8010da28 | 184 | deathdropowner |
| btlevtcmd_ClearAllStatus | 80105df8 | 88 | deathclearbeforeKillUnit |
| btlevtcmd_KillUnit | 80113304 | 184 | mode0status27/deferredregistrydelete |

This catalog is symbol/span/source-call evidence, not evidenceallcallbackbodies
areclosed. Originalcoin/experience/effects/audioandfullpartdisplayremain retained
requests; donotsatisfy thembyinventingrewards,timersordeathdialogue. Original
waitscript invokes AnimeChangePoseFromTable801064d8; its pose/statusselectorand
ordinarydisplayservice are also required forvisiblereturntoKUR_S_1.


## Used death leaves, reward storage and automatic wait pose

2026-10-13 original US inspection follow-up. Numeric bodies inspected with
inspect_dol_function; local `/tmp/trio-disablecursor.dis`, `trio-dmwait.dis`,
`trio-checkcommand.dis`, `trio-unitcoin.dis`, `trio-unitexp.dis`,
`trio-battlecoin.dis`, `trio-battleexp.dis`, `trio-backitem.dis`,
`trio-staroffset.dis`, `trio-statuspose.dis`, `trio-searchpose.dis`, and
`trio-setstatus.dis`. No original-execution or native-binding claim.

- _disable_restore_command_cursor801141a4/18 loads globalBattle r13+1c70,
  storesword0 atB1c70,byte0 atB1c74,returns2. No argumentreads,waits/unitchanges.
- _check_at_dm_event_wait801140fc/a8 resolves/translatesarg0unit. Missingunit
  returns2; ifU2a8 nonzero andevtCheckIDactive return0; thenidenticalcheckU2b4;
  otherwise2. DoesnotclearstaleIDs. This is requested byplayer/partnerdeath
  branches, notthefreshGoombadeadcore path.
- CheckCommandUnit8010dcb0/90 resolves/translatesunit; signedwordkindU8 equal222
  or224<=kind<231 writes1 viaevtSetValue(rawarg1),otherwise0; returns2.
- OnOffStatus27 callsBtlUnit_SetStatus80127bac/388. Its27branch80127f1c writes
  byteU135=low8durationandreturn1; ignoresstrength. NoHP/event/registry/flag
  writes. Status27duration1 isfirstscript-owned and laterKillUnitreasserts it.

### Reward getters and owners

GetCoin8010d914/6c andGetExp8010d9bc/6c evaluateunit→translate→unitptr; call
BtlUnit_GetCoin80126ca8/78 orBtlUnit_GetExp80126d20/e0; write returnedamount
torawarg1 through evtSetValue; return2. Getter results are not pouch deposits.
StoreCoin8010d8d8/3c andStoreExp8010d980/3c evaluateamount,call BattleStoreCoin
800f7c0c/14 orBattleStoreExp800f7bfc/10,return2. StoreCoin work=*(B2738),
work+14 += amount asword; StoreExp Bf00 += amount asword. No clamp/pouch write.

BtlUnit_GetCoin: nullunit→0. kind=*(U10); total=U8(kind12); loopindex0,
repeatU8(kind10) times: irand100, rereadkind, ifsignedrandom <U8(kind11)
total++. Reloadtrialcount eachloopcondition. Goomba kindREL2a7d8 binds
coinbase0,trialcount1,chance60; thus one sourceRNGdraw produces0or1, not a
constantcoinreward. Thedraw mustremain intheoriginaldeathscriptphase.

BtlUnit_GetExp: pouchGetPtr800d5eb0; playerlevel=S16(pouch8a),
enemylevel=U8(Ud). Ifplayerlevel>enemy return0. Otherwise work=*(B2738),
setup=*(work+c), group=*(setup+18), groupcount=S32(group0).
Factor rawfloatconstants804226a8=1.0 forgroupcount0..2;804226b4=1.100000023841858
for3;804226b8=1.2999999523162842 for4;804226bc=1.5 otherwise.
Convert signedleveldifference tosingle; singlemultiplyfactor*difference;
singlemultiply804226c0=.5; truncate towardzero; add U8(kind+f) baseexp.
Goomba baseexp0/level7 withgroupcount4/playerlevel1 derives3. Do nothardcode this
without binding theoriginalpouchlevel andgroupcount owners. Experience has
no RNGdraw here and doesnotgrantlevelups inthegetter/storecallback.

GetBackItem8010da28/b8 resolvesunit; ifU1c lacks40000000 orU308==0 return2
withoutwrites. Otherwise work=*(B2738),setup=*(work+c), scan8wordssetup+7c:
stopatfirstzero; writeitemU308toslot; clearU308. Ifall8slots nonzero, codewrites
slot8 ratherthanrejecting/clamping. Thefreshnoitem pathmustpreservezero/no-op,
not manufactureanitem ornewdropdraw; actualinitialU308/flagproducer retains
itsownsourceownership request.

_add_star_point_disp_offset80113f88/174 resolvesunit thenreads/evaluatesrawXYZ
operandtokensinorder. Integercoordinates convert tosingle, add kindfloatvec
+90/+94/+98 withsinglefadds; fctiwz eachresult, writebacksameXYZtokensinorder;
return2. No rendering/expmutation or floattokenwrites. Goomba kindvec(0,0,0),
so deadcorealready-addedY30/Z10remain unchanged inthisfreshcase.

### Correction: automatic ordinary wait chooses type28, not type1

The literal1 in AnimeChangePoseFromTable(-2,1) isbodypartID. It isnotpose type1.
Earlier summarycallingtheautomaticwait KUR_N_1 wasunsupported and is corrected.
Wrapper801064d8/6c resolvesunit+part then btlDispPoseAnime801010f4/38(P).
Thecallee consumesonlyP; volatileextraregisterr4 fromthepartlookup isirrelevant
becauseit isoverwrittenbeforeuse. PoseAnime P204 &=~(2|10|80|4); P204|=1.
It doesnot setname/typeordirectlyrestartanimation.

btlDispMain80102874 checksP204bit2; when clear, call _GetStatusPoseType80103084/74.
UnitattributeU104&20000 returns27 first. Else testoriginalprioritylist at
80416ab0:27,2,9,1,4,5,3,-1; returnfirstactivestatus,else28. Forenemy kinds1..215,
searchPoseTbl80103cd0/44(P1bc,type) scansmax28 eightbytepairs, returningnamewhen
entrytype equalsrequested **or equals28**; nulltable/endwithoutmatch→null.
FreshnostatusGoomba chooses28→RELbb28 KUR_S_1. Its type1 KUR_N_1 remains a
separatevalid tableentry, notthisordinarywaitchoice. Afterselection btlDispMain
hasFast/Slowplaybackrate checks; ordinaryrateconstant unchanged; P204bit1
consumercalls animPoseSetAnim withbit8forcepolicyasdocumentedabove. Same-index
force0retainstheanimationselectorclock/caches. Originalstatusposevariants are
outsidefreshnostatusscope andcannotbe substitutedwithunconditionaltype28.

### 2026-10-13 native Jump hit/reaction binding

`TrioAttack` imports the installed tik damage and wait wrappers and runs original
DOL generic damage children in its battle-context event manager. Fresh ordinary
Goomba Jump uses the recovered power/calculation/direct-application cores,
writes source HP/accounting, starts the owned hit root and preserves cancellation
order. Target HP starts at2. First missed timing produces HP1; the original hit
child waits1000ms, completes, and automatic wait selects type28/KUR_S_1.
Mario's original recovery jumps/home movement complete. Two hits reach HP0;
the source child sets status27 after its reaction and WAIT_FRAME30, then stops
at unbound AddRotate801115f8. Original death presentation/rewards/removal and
turn scheduling still require native binding. No battle victory or field result
is manufactured from HP0. The completed-action native adapter keeps field
control locked and retains battle resources rather than opening a stale menu.

Native code words retain calculator flags beyond `flags|23`: original HP
calculation ORs10000 when flags100 and defender attribute100 is absent. This
fresh Goomba path therefore produces10117 for a missed first hit and10717
for a missed lethal second hit, not just117/717. Full-word event checks must
retain this bit. Host checks cover both successful timings (20000 then20100),
first-success/second-miss, alive return, callback ownership and AC cleanup.

StopAC starts TimingA state1000. The runtime preserves its source1f54 countdown
and clears all four callback registrations only when Main returns0. Stylish
window/result helpers retain original event invocation counts and press edges.
Feedback, audience, effect and sound requests are retained; their full original
presentation providers, optional stylish icons, original RNG seed ownership and
Mario's installed idle event remain unfinished. Fresh no-status wait display is
scoped explicitly; other status pose paths suspend rather than selecting an
unrelated idle. Spike counter and Paragoomba wing-fall still suspend at
CheckDamage8010a78c, preserving their HP and event ownership.


## Rotation, deletion order, packed clear and reward-pointer correction

2026-10-13 original US follow-up; inspected AddRotate/SetRotateOffset wrappers,
BtlUnit_AddRotate/BtlUnit_SetRotateOffset, BattleCheckUnitBroken,BtlUnit_Delete,
and btlUnitPartsDisp. Original function bodies remain ignored `/tmp/trio-*.dis`.

### Used integer event rotations and pivot

btlevtcmd_AddRotate801115f8/128 and SetRotateOffset80110f9c/128 evaluateunit then
XYZusing integer evtGetValue inX/Y/Zorder. Signedinteger results convert to
single withtheoriginalxoris/doublebias/fsubssequence; neitheruses evtGetFloat.
Afterlookup callBtlUnit_AddRotate801287c4/d4 orSetRotateOffset80128604/34;
bothwrappersreturn2. AddRotate singleadds eachaxis toU6c/70/74, reviseAngle
80007328 resultstore; theniforderedresult>=single360 replacewithpositivezero.
reviseAngle usesoriginalfmod(doubleangle,double360),roundstosingle,replacesNaN
withpositivezero,adds360ifnegative,andzeroesnegative-route sum>=360. Signedzero
survivesnonnegativepath. See
[completegeometrycontract](BATTLE_UNIT_REGISTRY_AND_GEOMETRY_INITIALIZATION.md).
DeathspinY starts345,330,...0 andcontinuesfortwofull48tickturns; Xtip30*3→90.
SetRotateOffset storesaxis toU78/7c/80 except exactfloat-250000000sentinel;
no angle normalization,unitpositionupdate or implicitheightpivot.

btlUnitPartsDisp80101774/d7c getsownerU=P4ec,skipnegativeposehandleP1c0.
Unitmatrixconstruction801017d0..80101938 usesunitpivot p=(U78,7c,80),
unitangles(baseU60..68 +currentU6c..74), degreefactor80427440-20996,
scale(U114*U9c*U90,U114*Ua0*U94,Ua4*U98), anddisplaytranslation(U54,58,5c).
Original concatleft-to-right operations: S; T(-p)*S; Ry*...; Rz*...; Rx*...;
T(p)*...; T(display)*.... Thus theinitialunitmatrix is
T(display)*T(p)*Rx*Rz*Ry*T(-p)*S (columnvectorconvention). It alsobuilds
worldT(U3c,40,44) separately. This closesusedunitrotation/pivotbank ordering,
not allfinalpart/billboard/camera matrices. Ordinary part path later reconstructs
unitrotations withP4fc axis-orderselector, mixesfacing/cameraangle andpartrotations/
pivot/scales,andconcatenatespart/worldmatrices beforeanimPoseDraw. Those complete
remaining drawbranches areboundedrequests; donotclaimthisinitialmatrixalone is
theentireoriginalspriteproducer. FreshGoomba pivotis0; sourcepivotreset0duringdeath
mustnotinventamiddle-heightpivot. Native projection/hardwarebillboard remainsadapter.

### Deferred removal and exact original teardown

BattleCheckUnitBroken800f7b28/5c capturesB,fori0..63 readsBattleGetUnitPtr(B,i);
nonnull UwithU1c&80000000 invokesBtlUnit_Delete(U); incrementsindex,continues.
Doesnot testHP/status27 directly orbatch all deletions first.

BtlUnit_Delete80128e08/1d8 capturesglobalB. NullU returns0. Searchregistryindices
0..63 forfirstpointer==U; absentreturns0. Forregisteredunitperform:

1. IfnonzeroU290 deleteIDthenzero; thenU29c; thenU2b4. ThereisNO U2a8deletion
   inthisbody. evtDeleteIDrecursivechild/brother/localcopybacksemanticsstillapply.
2. Scan64wordslotsB120+4*i; anyequalU0 write-1. Thisisdistinctfrompointerregistry.
3. IfU1c&40000000 andsignedUe>=0, work=*(B2738),setup=*(work+c);
   setup+5c+4*S8(Ue)=U308. Sourcecodehasnolocalpositiveupperbound.
4. BattleStatusEffectDelete801966a4(U); BattleStatusIconDelete80253ca4(U).
5. IfU314nonnull: ifU318callbacknonnull callcallback(U),elseBattleFree(U314).
6. IfU31cnonnull smartFree8002f878(U31c),thenU31c=0.
7. WalklinkedpartsU14: ifP4e8!=-1 write-1 (noeffectreleasecallhere);
   ifP1c0!=-1 callanimPoseSetEffect80040a28(handle,0,0),thenreloadhandleand
   animPoseRelease8003b2d0. AdvancePnext0.
8. BattleFree800f8908(U14),thenBattleFree(U).
9. LAST BattleSetUnitPtr800f88b4(B,capturedregistryindex,0),return1.

Do notunregisterbeforeeventcleanup/freeorclearallfourhitdispatcherIDs here;
originaldeletioncallerdiffersfromdirecthitreplacement. Transitives extension/
smart/pose/resource/effect/free lifetimes remainseparatelyowned. Nativecannot
markentireencountercompletejustbecauseHPzeroorU1cdeletionmarkwaswritten.

### Packed ClearStatus preservation

[Completeclear/pouchcontract](../unit/UNIT_STATUS_CLEAR_AND_POUCH_IMPORT.md)
closesBtlUnit_ClearStatus80127b58/54: memsetU118..135inclusive30bytes;
SetStatus(U,16,1,-99) leavingcharge0; BattleStatusChangeInfoWorkInit80195d34/48.
SixrecordsUae8+12*i (i0..5): byte0=ff,bytes1/2/3=0,word4=0. Word8PRESERVED.
Do notexportthisas72zeroedbytes. OtherstatuswordU138,flagsU27c,move/badge/pose
fieldsremainuntouchedbythisbody. ClearAllStatus wrapperthereforeneedsfullpacked
status/infoexportwithoutclearingpreservedrecordtailwords.

### Correction: reward Setup+18 points group, not stage

EarlierrewardGetExp prosecalledthe Setup+18pointer astage/class; thisname was
incorrectand iscorrected. Original numericaccesswasright,butdomainmustbe group.
BattleInit800f8f04/138 capturesincomingFieldBattleInfo andstoresitB2738 at
800f8f4c. BattleInfomationSetBattleSetupInfo80114510/8 storesNPCinfo pointer
atFieldInfo+c. [OriginalNPCsetup](../field/FIELD_BATTLE_ENTRY_AND_RETURN.md)
closes npcSetupBattleInfo80043e78/280: chosenweightedentry+4 group→NpcInfo18,
chosenentry+8 stage→NpcInfo0. Nativefullchain:

B2738→FieldBattleInfo; FieldInfo+c→NpcBattleInfo; NpcInfo18→BattleGroupSetup
REL26460; group0=4count,group4→unitarray263a0; NpcInfo0→stageREL2619c;
stage0→bti_01string,notanexperienceclassificationinteger.

GetExp readsS32(group0)=4 andselectsfloat1.2999999523162842; playerlevel1 versus
Goombalevel7 yields trunc(single(single(1.3*6)*.5))=3. Stageclass0wasanunsupported
example despiteitscoincidentallysameinteger3; do notbindstage2619catNpcInfo18.
StoreCoin writesFieldInfo14, StoreExpBf00. The two0x14owners arenotthesame
NpcInfo14flowercount. PackednativeFieldInfo/NpcInfo lifetimes andoriginalpouch
level ownership remain explicitnativeadapterboundaries.


Additional exact SetRotate/cadence delivery: btlevtcmd_SetRotate80111720/174
reads rawXYZtokens separatelywhileevtGetValue produces signedinteger values.
GetRotate80128898/1c fetchesoldU6c/70/74. EachRAWtoken==f1194d80
(-250000000) substitutescorrespondingoldfloat; variableswhosevaluescoincidentally
equalthisintegerdonotpreserveaxes. Then BtlUnit_SetRotate801288b4/b4 calls
reviseAngleand>=360zero gate onALLaxes. Wrappersreturn2; sentinel preservation
occursatwrapper, notthescalarsetter. AddRotate doesnotpreserveanysentineltoken.

Originalremovalcadence: marioStMain8006f904/238 callsanimMain8004281c at8006f93c,
thenlowercasebattleMain800f904c/48 at8006f940. LowercasecallsuppercaseBattleMain
800f8e28/dc at800f9068 whenBnonnull; uppercaseFIRSTserviceat800f8e40 is
BattleCheckUnitBroken800f7b28. Onlythenpad,btlseqMain,ACmanager,stageanddisplay.
Top-levelnextcalls camMain,windowMain,mapMain,hitMain thenevtmgrMain800368d0
at8006f954. Therefore KillUnitmarkduringeventpassN cannotretireuntilnext
battleMainN+1 beforeeventpassN+1. Wholebattle completioncheck/flagpublicationis
separatelowercase/uppercaseprotocol; do notrunremovalinsideKillUnitcallback or
atendofthesameVMpass. Sourceorderisbinaryevidence,notruntimeframerateclaim.
See [completeBattleMain](BATTLE_MAIN_SERVICE_ORDER_AND_FINAL_STATE_CHECK.md).


## Final used ordinary Goomba death draw path

2026-10-13 follow-up closes the used final `btlUnitPartsDisp` algebra, replacing
its earlier initial-unit-matrix boundary. US body80101774/d7c, original
`BtlUnit_Entry`80128fe0/6b4 and float/gravity control/getter bodies inspected.
Local disassembly `/tmp/trio-partsdisp.dis`, `trio-entry.dis`,
`trio-floatdisp.dis`, `trio-gravitydisp.dis`, `trio-floatcontrol.dis`,
`trio-gravitycontrol.dis`. Matrices below use column vectors, `Concat(A,B)=A*B`;
original SDK numerical matrix services and camera values retain their contracts.

### Fresh bindings and selected branches

Owner U is `P4ec`; source Goomba part attribute9 atP1ac skips the special
20000000 branch at80101944. Unit attribute40000 lacks04000000, so the
camera-relative part-angle correction at80101d0c is skipped and correction0.
Entry explicitly writes signed-byte P4fc=0 at801294f0, selecting axis order0.
Both unit and part base/current angle triplets initialize topositive0;
unit/part current/base scale triplets initialize to1; unit scalarU114=1;
unit and part rotation pivots initialize0. Unit display offsetU54..5c and
part display offsetP30..38 remain0 from cleared construction. Part local
XYZP24..2c copies source Goomba0,0,0. UnitU189 andpartPbd copy the source
side direction byte; setup4 enemy belong1 suppliesff (signed-1).

The death core setsunitflagU1c08000000. The draw mirror gate tests a different
flag01000000 at80101e3c. Therefore death does not disable the mirror test.
Do not use the death flag to freeze facing or skip the entire camera branch.

### Facing, camera and negative unit Z scale

For ordinary branch80101c48, choose d=270 if signedU189<0, else90.
Incoming animation angle A=reviseAngle((-1*d)-90), at80101e20..1e34.
Thus negative direction givesnegative0; positive gives180. Do not replace
this with an arbitrary unitYaw/UV-flip mapping.

WhenU1c01000000 is clear:

- q=reviseAngle(A+U70+P4c+U64+P40), preserving source single-add order.
- Fetch current camera through camGetCurPtr8000f2e4. theta=angleABf8000703c
  (camera+c,camera+14,camera+18,camera+20), the X/Z eye andtarget fields.
- h=reviseAngle(q+theta).
- If90<=h<=270 inclusive, rebuild unit scale with NEGATIVE Z; otherwise
  retain positiveZ. Ordered comparisons treatNaN as failing this gate.

The rebuilt unit matrix preserves source P4fc0 order. It startsS, then
T(-unitPivot), thenRy, Rz, Rx through left concatenation, thenT(+unitPivot),
thenT(unitDisplay). Only the unit Z scale sign changes. Angles remain
(baseUnit+currentUnit) separately byaxis. Numerical theta/current camera
producer is an explicit service boundary; there is no fixed source theta
constant in this draw consumer.

### Final matrix passed to original animation draw

Define partPivot=(P54,P58,P5c), unitPivot=(U78,U7c,U80).
UnitM = T(U54,U58,U5c) * T(unitPivot) * Rx(U60+U6c) * Rz(U68+U74)
* Ry(U64+U70) * T(-unitPivot) * Sunit.
Sunit=(U114*U9c*U90,U114*Ua0*U94,signZ*Ua4*U98), signZ fromthegate above.

Part angles: x=P3c+P48, y=P40+P4c+cameraCorrection, z=P44+P50.
Part scale Sp=(P6c*P60,P70*P64,P74*P68). Source concat sequence801020d8..
801022dc (P4fc0): startUnitM; leftmultiplySp,T(-partPivot),RyPart,RzPart,
RxPart,T(+partPivot),T(worldPartXYZ),T(P30,P34,P38),T(0,0,-5).
Consequently the final passed matrix is:

`M = T(0,0,-5) * T(partDisplay) * T(worldPartXYZ) * T(partPivot)
* RxPart * RzPart * RyPart * T(-partPivot) * Sp * UnitM`.

worldPartX=U3c+P24*directionFactor, where directionFactor is1/-1 fromsignedPbd,
or the source battle-side direction ifPbd==0. worldPartY=U40+P28+
gravityDispOffset+floatDispOffset; worldPartZ=U44+P2c. These are live fields,
not thehome/setupXYZ. Source camera correction is0 inthisusedscope.

Fresh ground Goomba P1a8floatphase0 andP1a6gravityphase0 produce offsets0.
floatOffsetControl80100f34/c8 requiresunitflags100000 and200000 andnotstatus27
to advance; otherwise settles existingphase toward0. gravityOffsetControl
80101060/94 requiresbattleflag40000000 andunitattribute1004 clear toadvance;
otherwise settles. GetFloatDispOffset80100ed0/64 computes2*sin((4*P1a8+270)%360)+2;
GetGravityDispOffset80100ffc/64 computes4*sin((3*P1a6+270)%360)+4, using source
sin-degree service80125cd4 andsinglefmadds. Freshphase0makesboth0, no newlift.

Withfreshzero offsets/pivots/partangles andunit/part scale1, the used matrix
reduces to `T(Ux,Uy,Uz-5) * Rx(U6c) * Rz(U74) * Ry(U70)
* diag(1,1,signZ)`. It applies to original48-frameYspin and30-frameXtip; no
heightcenter translation or post-billboard screen-space rotation is introduced.
NegativeZ gate can change duringYspin asqchanges; preservecamera/facing algebra.

Calls at80102428..46c pass the SAME finalM andangleA, scale2.0 to
animPoseDrawMtx(poseID,M,pass1/2/3,A,2). Pose/resource admission still owns which
passes draw. Original common depthtranslation-5 is source geometry, not a
native arbitrarydepthbias. Original model resource's scalar2 is separate from
unit scale1. [Draw wrapper](../display/ANIMATION_DRAW_WRAPPER_AND_ADMISSION.md)
and [inner facing](../display/ANIMATION_INNER_DRAW_FACING.md) close ensuing
angle/camera/smoothing/reflection/uniformscale request algebra; do not append
an independent native facing transform on top without bindingthose owners.
This closes the used unit/part matrix path, not hardware/FPSCR/camera cadence
or actual GX/material image equivalence. Remaining numerical current-camera,
pose flags/retained animation facing fields and originalrender-resource consumers
stay bounded; no source execution or implemented fidelity is claimed here.


## Fresh battle c_kuribo inner draw admission: flag8 clear

2026-10-13 used initializer/producer follow-up. Original animPoseEntry8004176c/4ec,
animPose_AllocBuffer80041ee0/93c, animPoseSetAnim80041170/17c, animPoseMain
8003ed2c/1940 and original btlDispEntAnime/Entry/btlDispMain paths inspected.
Local `/tmp/trio-poseentry.dis`, `trio-allocbuffer.dis`, `trio-posesetanim.dis`,
`trio-posemain.dis`, `trio-dispmain.dis`. This establishes the fresh original
ordinary battle Goomba branch, not all reused/dynamically altered pose records.

`btlDispEntAnime`801011fc/90 reads part source model `c_kuribo`, calls
animPoseEntry(model,mode1) at80101264, stores returned poseID atP1c0.
Each battle part gets its own runtime pose record even when the model resource
is shared. Entry scans for a pose whose flags word0 iszero; successful path
ORs active1 at80041828 and storesit at8004182c. There isno resource-header
flag import or mask8/mask100 OR in this initializer. It clears secondarywords4/8,
sets pose mode+c=1, initializes angles+70/+74/+7c to0 and linked poseID+90=-1
at80041b1c..2c. Sign+78 isnot locallyinitialized; theusedmask8-clear path does
not readit, so native mustnot invent an initialsign for this scope.
Allocator assigns buffers/cache fields, not poseflag8/100 or linkedpose ownership.

The ordinary battle entry and `btlDispEntAnime` have NO animPosePeraOn call.
Field `npcEntry` separately calls that helper and becomesflag9; do not transfer
field NPC camera smoothing to a battle part solely because both use `c_kuribo`.
Successful fresh battle pose flags therefore start1, mask8 and100 clear,
linked+90=-1. Used init/hit/death streams contain no paper/group/peraflag requests.

Later used btlDispMain requests include animPoseSetAnim at80102a2c/40,
animPoseMain at80102b98 and materialflag setters in the draw path. SetAnim's
only local poseword0 write conditionally ORs4 when existingmask2 isalreadyset;
it doesnotset8 or100. Main evaluatesanimation/facing fields anddoesnotlocally
write poseword0. Materialflag setters change theirmaterialword, notposeword0.
StatusPoseControl and float/gravity controllers ownpart/status counters; they
are not animPosePeraOn. The Vivian-specific poseflag20 producer80039824 is
kind-gated andexcludedforGoomba kind1, andwouldnotset8/100 anyway. There isno
usedproducer convertingthisfreshpart totheflag8 orsnap100 paths beforedeath.

Therefore original unlinked fresh Goomba `_animPoseDrawMtx` takes mask8 CLEAR:
skip camera-target/smoothed-retained-angle+74/sign+78 reflection. Rotate Y by
incoming A iffA!=0 (fcmpu), concatenate ontheRIGHT of incomingM, then uniform
scale2 ontheRIGHT. Fresh used complete base transform is thus:

`Mdraw = T(Ux,Uy,Uz-5) * Rx(U6c) * Rz(U74) * Ry(U70)
* diag(1,1,signZ) * Ry(A) * S(2,2,2)`.

A is source battle facing angle0/180 previouslyderived; signZ is original
unit-camera mirror gate. Separate source animPose local/node transformations
continue underneath this matrix. Retainedcamera smoothing fromthe mask8 SET
branch isnotadmittedfor this fresh used record. The wrapper seeslinked+90=-1,
so it doesnotrunlinkedpaperprepass or manipulatea childpose; ordinaryresource
pass/materialadmission remains aspreviouslydocumented. Per-invocation uniform
scale2 occurs inordinaryinner route; do not multiplyit twice forwrapper+inner.

This closes the earlier fresh poseflag/retained-camera uncertainty. Numerical
source camera values remain needed for unit signZ classification, and original
model/node/matrix/GX/FPSCR hardware rendering stillhasitsowncontracts. Later
explicitpaper/peraflag orreusedpose scenarios areoutside this boundedfreshcase.

### 2026-10-13 native death, reward and removal completion

The fresh Goomba death script now executes its original48 AddRotate Y−15
requests, SetRotate0, reward getters/stores,30 AddRotate X+3 requests,
ClearAllStatus and KillUnit0. Event operands retain integer/raw-sentinel rules
and source angle normalization. Native rewards follow FieldInfo+c→NpcInfo and
NpcInfo18→the original imported group26460 (count4); GetExp uses its1.3 factor,
kind+e→unit+d enemy level7 and explicit fresh pouch level1, yielding3 exp.
GetCoin retains the original one irand100 draw against kind chance60; storage
goes to FieldInfo14/Battlef00, not directly to a fabricated pouch deposit.
GetBackItem's fresh absent-item route is unchanged. Selected item/seed/pouch
producers and later reward export remain separate owners.

Native broken-unit scan runs before action/VM services; script KillUnit atpassN
sets the flag while keeping the registry, thenpassN+1 deletes wait/phase/hit IDs,
clears matching Battle120 IDs and unregisters last. The source attack event slot
U2a8 is not gratuitously deleted. Native units/parts use preallocated storage
and shared animation packs, so retired bytes remain readable for diagnostics;
there are no original per-unit extension/smart/effect allocations to free in this
bridge. Future providers must bind their teardown when they create them.
Packed status clearing resets only30 statusbytes and the first8 bytes of each
of6 change-info records; each word8 survives. KillUnit reasserts status27.

Renderer consumes source base/current rotations, zero pivot and original
Rx*Rz*Ry order, source camera-angle table and inclusive mirror gate90..270.
Death flag08000000 permits mirroring;01000000 suppresses it. The fresh
c_kuribo direct-inner draw path composes the unit matrix with incomingA0/180
and original scalar2, plus source depth translation−5. Other part/linked/
paper/status draw domains and numeric hardware equality are not implied.
Matrices/trig are prepared per actor; unchanged zero-angle actors skip them.

Mario now installs the unchanged original wait stream80417c38 (symbol
wait_event/unit_mario.o; words3005b,801064d8,fffffffe,1,2,1), so its completed
Jump returns to automatic M_S_1. Alive Goomba returns to KUR_S_1; lethal
Goomba disappears after deferred removal. Original effect/sound requests
remain in the trace, but smoke/star/death audio presentation providers are not
executed. Battle turn, Spike counter, Paragoomba wing-fall, victory/reward export
and original field return remain unfinished; no result1 is fabricated.
