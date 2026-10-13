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


## Setup4 first Mario action to Goombella command handoff

2026-10-13 source composition uses the authoritative complete
[action controller](BATTLE_ACTION_CONTROLLER.md),
[move controller](BATTLE_MOVE_CONTROLLER.md),
[phase tables](BATTLE_PHASE_TABLES.md),
[barriers](BATTLE_WAITS_AND_CONCLUSION.md), and
[sequence storage](BATTLE_SEQUENCE_FIELDS_AND_INIT_SOURCE_BINDINGS.md).
This is a bounded derived fresh case: source registry IDs1Mario/2Goombella,
turn1,phase04000002,normalcounts1,state0,no extra statuses/items/actions/orderchange;
ordinary Jump has finished its source root and has killed onlygroundGoomba.
It is not an original measured turn trace or a license to returnready from
unimplemented audience/status services.

### Exact post-Jump state and readiness ownership

Act channel6 is Bf20, Move channel5 Bf1c, phase channel4 Bf18; current actorID
B420. Source Act7(06000007) polls the barrier, adjusts statusmessages, then
falls throughAct8 which polls itagain andcalls BOTH announcement pumps.
Barrier scans every registered unit's U29c,U2a8,U2b4,U2bc in that order,
clearingfinishedIDs butretainingactiveones; ordinarywaitU290 isexcluded.
Outerbattle IDs are B182b4,182c4,180fc,182c8,182c0, withfirstliveearlyreturn.
After-reaction queue and reserve consumption arepartoftheoriginalbarrier,
not a nativeattack-completed boolean. Goomba death root/childrentrackedU2b4
must finish; KillUnitdeletionmark retires beforethe next eventpass bysource
BattleCheckUnitBroken cadence. Rootcompletion alone cannotreleaseAct7.

AtAct9, normalMario state0/remaining1 consumesone move, remaining0,state10.
WithU13802000000clear, countdecrementoccurs; withU27c2000clear, U24resets0.
Becauseplayerbelong0, phaseword04000002, Goombella stillcount1/nostatus27
andbattleflag2000clear, original exhaustedpathwritesMarioU311=96.
Charge-consume flagU27c8, whenpresent, callsSetStatus16(1,-99) thenclearsbit;
freshno-chargecase skipsit. U25c clears; B18ff9 clearsONLYifvalue1. Missingactor
wouldskipactorwrites butstillruncommon reservation/audiencetail.

CommonAct9 consumes B19060/64/68/6c reserveditemslots viaoriginalservice,
clearsBef4bit4, callsAudience_PerAct, movesAct→10. Act10 waits audience reaction,
checksconclusion, thenBreakSlot reactionifcontinuing. Act11drainsBOTHmessage
pumps; Act12barrierthen optionalpostscriptU2c8→trackedU2cc; Act13pollspostID,
Act14barrierincrementsMove5 from9 to10. These exactfallthroughs occurwithin
onebtlseqActcallwhenservicesready; noforcedone-frame-per-state delay.
FreshoriginalMario hasno installedpostscript through itsused init, butthefield
mustremainowned ratherthanomittedfromgeneralgraph. U2ccpostID isnotpart of
unitbarrier andhasitsownAct13poll.

Move10 checksconclusion FIRST. WithSpiked/Paragoomba alive, continue; forall
nonnullunits SetTotalHitDamage(0)→U264 andclearU27c1000. BattlePhaseEndCheck
returns0 becauseGoombella inexistingactorlist hasstate!=10, signedU22>0,
status27clear. SetMove5=05000008 andRETURN; donotselectsameoutercall. Next
BattleMain sequence invocation entersMove8, resetsall-unit hitbookkeeping,
selectsactor2 fromthe EXISTING sortedphase2actorlist, writesB420=2,
GoombellaU27c|=1000, Move5→9 andAct6=06000000, thenfallsintobtlseqAct.
Act0 seeskind224 andstatuses1/2/9/27clear→BattleCommandInit thenAct1.
Act1requirescameraMoveSpeed<=2; thenAct2runsBattleCommandInput untilready.
Command/menu/input/readiness providers mustbe bound; donotgrant anattackroot
merelybecausea menuadapterselectedGoombella.

ThereisnoPhaseFirstProcess/table rebuild orphase-event fanout betweenMario
andGoombella: thoseonlyoccurwhenMove5 state0starts aphase. Phase2originalsort
isstableabs(homeX) thenpriority; Mariohome-100 precedesGoombellahome-140,
bothkindpriority/span0means sourceirand0 consumesno RNG atturnreset. The
handoff itselfdoesnotdraw a newrandomactorpriority. Enemyphase4 order30/70/110
isderivedfromsameoriginalhomes; deathremovaldeletesGoombaactorIDs inB120table.

### Frankly and phase hooks

Firstphasehooks are U294(selector1 fanout Move4, all64), then U298(selector0
orderedtable serial Move6/7). Tablemembership/orderremain distinct fromactors.
Ordinary trioinit installs no U294/U298 phasehooks; Frankly installs U298=REL3dc40.
Phase-eventtable includesallnonnullnonexcludedunits anduses source side/home keys;
missingcallbacksare skippedwithoutcompacting IDs. Frankly iscount0 andcannot
be selectedasactor, evenwhenitsphaseevent iseligible.

WhenGoombella isselectedafterMario, Frankly isNOTcalledagain: Move10→8 skips
Move0..7. AtnewphaseboundariesMove0constructstables, andMove6canrunFranklyagain.
Itsactual39-command event checksphase04000001 andturn1 before09201..05;
phase2/phase4/otherturn returnswithoutdialogue. Spikywarning09206/07 belongs
separatecounterchild/key25, notthisphasehook. Do notreplaylesson aftereachattack.

### Goombella original provisioning and Headbonk command

Kind unitdata_Party_Christine80393bec/c4 has signedHP10/FP5, onebodypart
80393ba0/4c modelpointer804243b0→c_pkr, poseTable80393a98.
SourcekindHP isonlyconstructionbase: originalpartnerSetParamFromPouch imports
currentHP frompouch partnerrecord R+6, baseHP R+4,maxHP withbadgecounts, then
battle status/sizing snapshot. Partnerkind224mapsrecordindex1; noFPfieldwrites
onthispartnerimportpath. RuntimeHPcopies U10a/U108/U10c andFP110/10e/112 remain
distinct. HP<=0importsetsstatus27. Usingfresh10HP untilactualnativepouch owner
isbound remainsanexplicitadapter, notproofallplaythroughsstartwith10.

Init80393d9c/cc is12commands SHA256
1c6cc4109de20d372b9425c72d612c019e9fd152e4c2dcb356a68768689683d7:
SetEventWait(-2,80418480),SetEventAttack(-2,80418478),
SetEventDamage(-2,80393e68),SetEventConfusion(-2,8034f1e8),
originalwalk/run/jumpsounds,SetUnitWork(-2,0,0),
SetEventEntry(-2,80393cb0),StartWaitEvent(-2),RETURN,END.
It installs no phase/postscript. Wait80418480/18 SHA256
fd12f377ceeb12767e6053fa99a25989153c0169d545691c96342aecf1a282af calls
AnimeChangePoseFromTable(-2,1),RETURN,END: no-statusautomatic28→PKR_S_1.
Body1 isnotpose1. Type39=PKR_D_1,40=PKR_W_1,42=PKR_R_1.

Installeddefaultattack80418478/8 isonlyRETURN/END (SHA256
1e9fcd4ca7e6723c4a822c370faad68aa89031573b2376d87e4cc3c6626ede61).
A selectedHeadbonk insteadusesoriginalweapon80393ea8+b0→NormalAttack803941a8/ce0
through originalcommandattackselector, nottheemptydefault installedevent.
Theweaponpowercallback/table andcandidatefiltersarealreadyboundinthecompanion
contracts; itsentiremotion/TimingA/counter/followup graph remainsaseparateused
implementation task. Damage80393e68/40 sixcommands SHA256
a2601eb4b39fb08b19b7dcb05bd82bddbef224708bad7a3c452c39ed06c1a608:
SetRotate(-2,0,0,0);SETLW10=-2;SETLW11=1;RUN_CHILDdefaultDamage;RETURN;END.

### Newly inspected used readiness leaves and explicit boundaries

BattleStatusChangeMsgWorkInit80195a94/5c zeros600bytes B18da0..18ff7;
for28records stride12 setsrecordbyte0=ff. BattleStatusChangeMsgAdjust801958bc/158
skipsrecordsexactlywhen signedbyte0==-1; freshall-emptyrecordwork performsno
unit/statuswrite. MsgMain801956f8/1c4 usesoriginalorderedrecordindexlist; when
allrecordsareemptyreturns0 andskipsitsotherwiseall-unitstatusinforeset.
Zero-statusordinaryJumpdoesnotproduce astatusmessageentry justfromHPdamage,
andClearStatus27hasseparatedeathproducer; nofictionalannouncementsaretocreate.
Nonemptymessagebody/queuewriter/string/effect ownershipremains a boundedrequest.

BattleStatusChangeAnnouceMain_Unit80195af0/8 isLITERALreturn0 inUSbinary.
AnnouceMain80195af8/64 scansall64nonnullunit slots, callsleaf, OR-readiness,
returns0 underthisactualleaf. It iscorrecttosupportthisoriginalstub; unrelated
AnnounceMessage eventservice isnottherebysupported.

Audience_CheckReaction801a7e30/64 capturesAudienceBase=B2858 via801a86a0/c;
callsBattleAudienceGetPtr(0)801a8664/3c, thenreadsbase+4 eventENTRYpointer:
nonnull +evtCheckID(entry15c)active→1; elsebase+4=0,return0. Audience_PerAct
801a7e94/440 mayproduce reactions/RNG andremainsseparatelyrequested; no
unconditionalzero audience result isproved merelybyfreshnoequipment.

BreakSlot_CheckReaction80203394/98 useswork=B1616c via8020524c/10;
FIRST BattleBreakSlot_Start8020342c/64. Startif signedwork+c>=3 clears+c,
setswork+4=2, requestsfirstslotpointer80205210(0), thenfor3stride5c slots
setsword8=4,wordc=0. Checkthenreturns1 ifsignedwork+4 is1..8inclusive,
elsepollsentrypointerwork+130 ID15c; active→1. Idlepathcallsoriginalcamera
mode(0,0),speed(0,2),AudienceNumToTarget801a1c60,clearswork130,return0.
Slot-token/state producer andAudience_PerAct contribution arerealdependencies,
not guessed0 gates. Nativebest-effort providers shouldexplicitlyreportthat
scopeuntiloriginalreaction/slot/audience/camera ownership isbound.


## Headbonk used command, coordinate and rotation callbacks

2026-10-13 source recovery for original NormalAttack803941a8/ce0,227commands,
SHA25677d2f5d30bea8bd4ff16b0d09ac2e942e705f4717e9fb233b1ccfb2166efc37c.
Original inventory `/tmp/trio-headbonk-events.json`; inspected DOL callbacks in
`/tmp/hb-{lv,cost,pre,check,posfloat,getrot,diradd,face,scale,krb,select,pay}.dis`.
No native edit, original execution, or visual equivalence is claimed.

### Selected command weapon ownership, cost and damage requests

BattleGetSelectWeapon8012021c/4c takesB: M=B171c; readCURRENTTYPEwordB1c5c;
_btlcmd_GetCursorPtr80124cb8(M,type,&privateStackSlot); cursor=*output;
weaponIndex=S32(cursor0); returnWORD(M+0x80+0x1c*weaponIndex). Stackoutput isnot
preinitializedhere. [Cursor/type contract](WEAPON_MENU_TYPE_DOMAINS_AND_CURSOR_REUSE.md)
closesvalidtype routing anddistinguishesunsupporteduntouchedoutputdomains.
The callbackdoesnotfetchweaponfromunitdefaultattackpointer oritsownLW12.
A native selected-command provider mustretainthisownership andcurrentweapon
query timing, withunsupportedcursor domains explicit.

CommandGetWeaponActionLv80105890/50 hasONE operand: rawoutputtoken. Query
currentglobalB selectedweapon, readU8(W6e), evtSetValue(output,value),return2.
UsedHeadbonkW80393ea8 hasactionlevel3; thecallbackhasnoactoroperand. It feeds
AcSetDifficulty(-2,LW0) beforeSetupAC(-2,1,1,0). HeadbonkusesTimingAordinary
setup; do notresetparams/flags/history beyondsourceSetupACscope.

CommandPayWeaponCost80105928/78 evaluatesactor arg0,translatesandgetsunit using
capturedglobalB; THEN reusescapturedB forSelectWeapon, callsPayWeaponCost(U,W),
return2. No weaponoperand, output, affordability query orrefund. Originalcost
core isclosed in [cost/payment](BATTLE_WEAPON_COST_AND_PAYMENT.md).
HeadbonkbyteW11 FPcost0,W12 APcost0 (W10=100 isaccuracy,notcost). Evenzero
cost retainsGetFp/SetFp viaMario sharedFP owner, PartnerAction AddPointB16f4d(0),
AddCountB16f4e, pouchAddAP(0). Zero-cost adaptercannotdropactioncount/accounting.

CommandPreCheckDamage8010aaa8/104 andCommandCheckDamage8010a688/104 use
FIVE operands(actor,target,part,RAWflags,RAWdestination). Resolveactor→unit,
resolve target→unit, resolvepart→BtlUnit_GetPartsPtr80128cd8(target,part),
then querySelectWeapon(capturedglobalB). Coretuple is(A,D,P,W,rawflags), through
BattlePreCheckDamage800fbadc orBattleCheckDamage800fb01c. A negativecore result
returns255 withoutoutputwrite. NonnegativewritesresulttoRAWdestination via
integer evtSetValue, thenevtCheckID(callingevent15c): active→2,absent→255.
Result1/18 isstoredscriptvalue, neverthehandlercompletioncode. Rawflagssupport
noevtvariable-resolution inthese wrappers.

NormalAttackfirstprecheckF100 at803943f0; subsequentLW10>=1 usesF100100 at
803943d0. TimingfailureCheckF100 at80394a48; successfulcount<=1 CheckF20000
at80394b08 thenLW10++andloop; count>1 CheckF20100 at80394bb4. Selectedweapon
isrequeriedfor eachcommandcallback; parentLW12weaponcaptureisnotusedasthis
wrapper'sargument. Headbonkordinarylevel1per-hitpower1/FPdamage0 followsalready
closedpartyweaponpower table; spikecounter/fall/directhit dispatchretainthose
originalper-hit flags andsharedunitcodeownership. Do notdoubleoneHPwrite.

### Coordinate/facing/scaling leaves

GetPosFloat8011355c/c4 resolves/translatesunit, GetPos80128c7c→U3c/40/44.
RAW XYZdestinationtokensf1194d80(-250000000) eachskipaxisoutput; otheraxes
call evtSetFloat800304bc inXYZorder, nointegertruncation. Tokencomparison
precedeswrite; destinationvariablevalues do notbecomeignoredtokens.

GetRotate80111894/e8 resolves/translatesunit,GetRotate80128898→U6c/70/74.
SameRAWdestinationtoken sentinel. Eachwrittenaxis fctiwz truncatestowardzero,
theninteger evtSetValue. Noangle normalization/clamp insidegetter.

MoveDirectionAdd8010b8f4/b0 takes(actor,RAWcoordinateToken,amount).
Evaluateactor/amount asinteger; useS8(U188) whennonzero,otherwiseS8(U189).
Compute low32(amount*sign),then evtGetValue(coordinateToken), writewrap32(sum)
tosameRAWtoken viaevtSetValue. Actorposition isnotwritten. SourceHeadbonk
avoidance uses30then20 offsetsforwardalongmovement/facing sign; thisdiffers
fromFaceDirectionSub recoveryoffsetsbackwardalongface.

GetFaceDirection8010e614/70 resolvesunit, readsign-extendedbyteU189, writeinteger
rawarg1,return2. Noatan2 orfloatconversion. Source±1signisnotnativeyawdegrees.

SetScale8011049c/c0 evaluates/translatesactor. IftranslatedID==-1,return2
beforeXYZreads. Otherwisefetchunit, evalXYZ viaevtGetFloat80030840, call
BtlUnit_SetScale80128524/34(U,x,y,z),return2. CorestoresU90/94/98 axesunless
incomingfloataxisexactlysame asfloat-250000000sentinel. NoU114scalarwrite.
SourceHeadbonkResultAC path callsSetScale(-2,1,1,1) at80394a18, restoringvector
scalesindependentlyofunit/pouch sizing scalar.

### krb_get_dir and actual motion rotation siblings

UsedChristine krb_get_dir80189910/118 takesfourfloatcoords andaninteger
output/prior-angle token. evtGetFloat firstfourinorder(oldX,oldY,newX,newY),
then evtGetValue(fifthtoken)capturespriorangle. dx=single(newX-oldX),
dy=single(newY-oldY). If BOTHexactly0 viafcmpu, evtSetValue(fifth,priorangle).
OtherwisecallSYSTEMangleABf8000703c(dx,dy,0,0), not(0,0,dx,dy), fctiwz
returnedangle,evtSetValue(fifth,int),return2. Originalzero constant804243cc.
The2D helper seesXY trajectorycoordinates asits generic2D input; do not
substituteX/Z actorheading orcopytheenemyRELkrbhelperjustbysymbolname.
Numericatan2/FPSCR domainbelongsalreadyrecoveredsystemangleprovider; nofinite
orzero-speedheuristic beyondthisexacttwo-zero branch.

Usednames areoriginalPKR_A_2B at802f9318, PKR_J_1A at802f9324, notinvented
pose replacements. InitialpassLW10<=0 at80394454 setsPKR_A_2B, spawnsattached
brotherwaiting2frames thenACRStart(-2,0,7,7,0); parentwaits10frames thenPKR_J_1A.
Anotherindependentinlinewaits15frames thenoriginalSFX_BTL_KURI_ATTACK1.
JumpSetting initialandperpass(20,S0,G0.69921875) isoriginalquantizedgravity.

SubsequentpassLW10>=1: gettargetHitXYZ,Z+=5,SetRotateOffset(-2,0,14,0),
SetRotate0; attachedbrotherwait15frames then6iterationsGetFace→multiply-30→
AddRotateZ(step),WAIT_FRAME1. ParentMarioJumpPosition(count36,mode3,last0),
usingtheoriginalsamecallbackonGoombellaunit ratherthanMario-onlykindrouting.

Firstpass: SetRotateOffset(0,14,0),readACRresult. Ifresult==2, declareacrobat
andspawnattachedbrother; compute desiredface tosame target(mode16),
LW9=-15ifdesiredface+1,otherwise+15; GetPosFloat(initialLW5/6/7),LW8=0;
36iterationsAddRotateZ(LW9),WAIT_FRAME1. Elseattachedbrother:
GetPosFloat(initialLW5/6/7),LW8=0;36iterationsGetPosFloat(currentLW12/13/14),
krb_get_dir(oldLW5/6,currentLW12/13,LW8),GetRotateXYZintsLW0/1/2,
SetRotate(oldX,oldY,LW8),FSETLW5=LW12,FSETLW6=LW13,WAIT_FRAME1.
These brotherlocals arecopied/private andmustnotoverwriteparentprecheckLW5.
ParentGetHitXYZ,Z+=5;N=36;MarioJumpParam mayadjustcount; MarioJumpPosition
usesreturnedN(mode0,last0). Sourcefixed36tickrotationloop isnotautomatically
rescaledtotheadjustedjumplength; preserveactualevent/motionowners.

Afternormalhit/StopAC, originalreturnbrotherreadsZangleintegeronly(GetRotate
RAWsentinelsX/Y), ifZ<0 negatesit; multiplybyGetFaceDirection; intdivide15;
repeat15timesAddRotateZ(delta),decrementcounter,WAIT_FRAME1;thenSetRotate0.
Integerdivisiontruncates; thefinalSetRotate0removesremainder. Parentbounces
back60/20/8unitsalongFaceDirectionSub withcounts30/12/8, thenhomeMovePosition
andStartWait. Avoidancebranchinsteadresetspivot/rotations,PKR_S_1,thenMoveDirAdd
30/20 andgenericJumpcounts20/10, wait500ms beforehomepath. Do notmerge thetwo
recoveriesorapplyper-renderdelta-time tooriginalsimulation/eventloops.

Thelocalwrappers andusedrotationmathareclosed; actualoriginalACRstylish/result
prize/help/audience/audio/camera services, fullselectedweaponmenu/progression,
originalfloat/fctiwzexceptionsemantics andcompleteHeadbonk execution remain
separate retainedrequests. Nativebesteffort maybindthefreshordinarypath while
reportingthoseboundaries andreturntolatersourceevidence; no fabricatedstory.


### Native persistent player handoff and Headbonk (2026-10-13)

The native trio runtime retains registry, HP, reactions, AC work and rewards
between Mario NormalJump and Goombella Headbonk. Portable PhaseFirst builds
original packed actor/position tables once for fresh phase2; stable priority and
home-distance ordering, deferred-state selection, phase-end checks and move
consumption use the recovered contracts above. The normal post-action path
polls owned action/reaction and outer event IDs before consuming the actor's
move. Wait events are excluded. It respects message, reservation, audience,
BreakSlot and optional post-event boundaries instead of completing on attack
root disappearance alone. Missing nonempty providers preserve a pending address.
Move10 observes source conclusion first, then clears accounting/token1000 and
selects the next player without rebuilding the phase or replaying Frankly's
phase-start lecture. Original Move8 resets next-actor accounting and command
ownership. The menu receives current registry/HP/status; targets are not reset.

Goombella installs original kind80393bec, part80393ba0, damage80393e68 and
wait80418480. Headbonk executes original weapon80393ea8's root803941a8,
including source TimingA requests, airborne direction/rotation, ground damage,
reaction/death ownership and recovery. Her battle draw pack uses original
c_pkr poses, per-pose clocks and battle rotation/mirror services. Its static
source words are imported read-only, including the event scanner's one-word
lookahead. No field sprite animation is substituted for Headbonk.

The admitted path is fresh level0 Goombella, HP10, no badges/status/equipment,
zero-cost Headbonk against the ground Goomba. Native party/pouch ownership,
full command init, phase0..7 controllers, audience/BreakSlot producers, nonempty
status messages/reservations and original RNG/SDK math remain partial adapters.
Source effects/sounds/stylish requests are retained, with incomplete playback.
Spike/Para damage callbacks still suspend. After both players spend their move,
Move11 is the next unbound progression boundary; enemy actions and victory are
not synthesized. Battle result stays0 and story11 remains locked. Host tests
assert HP1 surviving Mario's missed Jump then HP0 from Goombella's missed
Headbonk, one reward/retirement, source idle and exhausted player move counts.


## Next source step: Move11 to enemy phase and variant callback admission

2026-10-13 documentation-only follow-up after native Headbonk validation.
This evidence is for the next implementation step; it does not claim current
native phase11/enemy scheduling or variants execute. Existing appended native
implementation reports are unchanged. Original `/tmp/trio-turn.dis`,
`trio-audperphase.dis`, `trio-checkperphase.dis`, `trio-changekind.dis`,
`trio-changedata.dis`, `trio-setposetable.dis`, `trio-replaceparts.dis` are local
US inspection evidence. Authoritative generic state maps remain linked above.

### Smallest bounded readiness provider and actual phase sequence

Move11 calls BattleAudience_CheckReactionPerPhase801a6ce8/f80. The smallest
closed used admission branch is exact: AudienceBase=B2858 via801a86a0;
pouchGetPtr800d5eb0 iscalledbeforeflags; ifAudienceBase.word0 mask8 CLEAR,
return0 at801a6d10, without RNG, event creation or state-machine writes.
Ifmask8 SET, the original large21-state audience phase-reaction controller runs;
it cannot be replaced with unconditional0. Its stateword isAudienceBase+137dc.
The source body returns1 while pending,0 on completion; original audience/
pouch/state/slot/effect producers are retained requests for that admitted graph.

BattleAudience_PerPhase801a7c68/10c takes original phaseword, capturesbase.
Phase04000002 withbaseflags40000clear processes200 audience entries, removing
qualified type7-heldthrow items and resettable100 item slots. It doesnot set
phase-reactionmask8 on this route. Phase04000004 invokes801a5a0c, ORsbaseflags8,
zerosbase+137dc. This distinguishes the exact phase2 early-clear readiness
branch from later enemy-phase-end reaction work. Current native owner may use
that source-clear-mask branch where its actual packed flags admitit; do not
assert alloriginal audience flags from a zeroed native adapter.

Move11 lowbyteaudience-ready0 then original eventbarrier-ready advancesMove5
11→12 andRETURNS. A laterMove12 invocation alwayscallsBOTHmessagepumps;
whenboth0 incrementsphasechannel4 andresetsMove5=05000000,thenreturns.
Fromplayerphase04000002 the nextphase is04000003, not direct04000004.
Phase3 startsPhaseFirstProcess: no surviving unit inthissetup hasU2c=04000003,
so actorlistempty. Phasehooktable remainsseparate: Frankly selector0 event can
runbutits phase1/turn1gate returnswithoutdialogue. Oncehooksend,Move6→10,
PhaseEndCheck returns1 onemptyactorlist, thenphase3Move11/12→phase4.
Phase4 builds actual survivingenemy actors sorted bypriority/homeabsX; after
Goomba death the originalSpikedhome70 precedesParahome110. Franklycount0
cannotbe selectedasattacker. A grounded survivingPara retainsX110/homeY0.

btlseqTurn0300004c dispatches8011e3a4→btlseqPhase andreturns; tableentry4b
setsTurn3→4c andPhase4=04000000 earlierat8011e384..39c. Phase6 increments
Turn3→4d andresetsPhase4=04000000. LaterTurn4d invokes8019f298, setsTurn3=
03000001 andclearsBef4mask2000; futureTurnFirstProcess owns nextturn counts.
Do notresetunitmoves orincrementturn atplayer→enemy phase boundary.

Enemy actor Act0 retains inhibited1 forkind1/2/3 becauseonlyplayer222/224..230
clearit. Therefore Act3 writesU2ac=1 andORU1c04000000; Act6 runs installed
attackeventU2a0 underitsoriginal status gates. SourceSpiked attack REL31310,
Para flyingattack2fb0c beginEnemyItemUseCheck(-2,LW0), optionalchilditemevent,
thenoriginalGetEnemyBelong/SamplingEnemy/ChoiceSamplingEnemy. These event roots,
item/target/RNG/motion/guard services arethe nextboundednative tasks; don't
invent a damage-only enemyturn or selectMario withoutoriginaltargetchoice.

### Fresh counter and wing-fall source integration boundaries

SourceHeadbonkW80393ea8 specialW74=35007,resistanceW78=0000000c (lacks2).
Withfresh noSpikeShield, topSpike counter isthereforesame asJump: original
precheck1, CheckCounter workordinaryadmission0, counterdamage1/full299(hex12b),
defenderhitevent31(hex1f), CheckDamagecore-1/commandwrapper255. SpikedHP remains2;
Goombella sourceHP10wouldbecome9, subjecttoactualpouchHP. Attackerhitdispatch
cancelsitsattackU2a8; victimSpikehitU2b4 runsdatakey25 warning child. Current
attackerB420 remainsGoombella, soFrankly'sonce-onlywarning choosesoriginal
mac_0_092_07;Mariochooses092_06. Bothactor/victim reactionevents mustfinishbefore
postactionbarrier. Complete original messageorder/timing isalreadylistedabove.

FlyingPara onpart-reactionF400000 dispatchesdata key0e→REL2f914 fallchild.
The originalweapon/partpair gates andF100requirement remain asdocumented;
these flags mustnotbe inferredsolelyfromHP loss. Currentnative lackofthese
providers isexplicitlyunfinished. Small source-binding leaves newlyclosed:

- AnimeSetPoseTable80106468/70 evaluatesunit/part, storesRAWtableoperand P1bc.
- ChangeDataTable8010dae0/5c evaluatesunit, storesRAWtableoperand U18.
- ChangeKind8010dc44/6c evaluatesunit/newkind, writesONLYcurrentkindwordU8.
  It doesnotreplaceU4originalkind/U10kinddefinition,resources/level/HP/status.
  FallChangeKind1 mustnotreloadgrounddefinitionorresetHP.
- ReplaceParts8011319c/168 evaluatesunit/currentpart andnewpartID; newtableRAW.
  Nonnullnewtable firstrecordIDmustmatchnewID; mismatch originalbranch loops
  onthecomparisonwithoutadvancing. Usednewrecord30230 hasID1,so thisbad-domain
  loopisnotadmitted. SetP4=newrecord,P8=source4,P1ac=source40,P1b0=44,
  P1b4/P1b8=38/3c,P24/28/2c=sourcec/10/14. SetPartsRotate0 andpivot0;
  P170/174=source18/1c,P17c/P180=0,P1a0=0. P178hitZ,scale/baseangle,
  sampleoffset/P1bc remainunchanged locally. ALWAYSthenbtlDispEntAnime(U),
  recreatingallpartposeIDs usingcurrentmodels/tables. No oldposeRelease inthis
  wrapper. Nativepreallocatedresource/atlas policy mustremainexplicitadapter.

OriginalfallstreamordersPoseTable30158 thenData301f8 thenParts30230,
unitflag4→damagepose39, fallSFX,JumpPosition10toY0,homeupdate,walkSFX,
500ms,ChangeKind1,groundinit3033c,WAIT_FRAME1,pose39. Groundinitreplacesinstalled
attack/wait/damage/soundservices. Hidingwingsalone cannot satisfy thistransition.
Originalresources,resourcepose recreation, exactgenericjump/audio/effects and
variant eventexecution remainnextimplementation work, notcompletednativeclaims.


## Exact fresh phase3/4 scheduling and first enemy command ABI

2026-10-13 documentation-only delivery for the native phase progression step.
Read-only inspections `/tmp/trio-move.dis`, `trio-phasefirst.dis`,
`trio-mario-init.json`, `trio-enemyitem.dis`, `trio-enemyitemcore.dis`,
`trio-enemybelong.dis`, `trio-actinit.dis`, `trio-niceasync.dis`; existing exact
[move/action](BATTLE_MOVE_CONTROLLER.md), [phase tables](BATTLE_PHASE_TABLES.md),
[target event ABI](BATTLE_TARGET_EVENT_INTERFACES.md) remain authoritative.
This section fixes used owner/order/flags; current native execution is separate.

### Packed Move0..7 sequence, per phase

Move5 storage Bf1c starts05000000 when precedingMove12 advancesPhase4=Bf18.
Forphase3, source Move0 invokesPhaseFirstProcess8011b0d0/11c, whichfirstwrites
B1c70word0/B1c74byte0; fills64actorIDsB120 with-1. ForEVERYnonnullslot write
U311=ff, thenstate10 requiresSlow20active beforecontinuing; otherstatesbypass.
RequireU2c==currentPhase4 andattribute80000clear; appendU0, U140=-1, eachpart
withP1c0!=-1 getsP4e4=1. Stable actor-sort thenseparatephase-eventtablebuild.
NoResetMoveStatus orTurnFirstProcesscall occursinside thisphase-first helper.

Phase3 freshactorcount0: sortcount0 scansfirstID-1 andnoactorlookup/sort.
Phase4 freshactorcountsurvivingopponents only; count0Franklystate10doesnotenter
withoutSlow, so hecannotact. Unitpriority/remaining counts persistfromturnreset.
Source sortedactorIDs are actualregistry/home/priority-dependent; normaltrio
Spiked4(home70),Para5(home110) afterGoomba3deletion.

Move0 incrementsMove5→1 andrequestsIMMEDIATEredispatch; Move1 increments→2
andredispatches. Move2 requestspouchGetPtr800d5eb0 thenBattleGetMarioPtr800f87a8;
onlypouchU8+5ba==2 ANDPhase4==04000004 launchMajinaiDefenceUp803634f8 andgo3.
Withoutthat EXACTgate, incrementMove5TWICE to4 andredispatch; phase3always
missesphase4gate. Freshfortune-mode adapter mustnot omitpouchquery/assume0
withoutdocumentedowner. Admittedfortune branch ownsB18ff9=2, helpers
BattleMajinaiDone/EndCheck andMarioattackU2a8; remainsseparatedependency.

Move4 scansall64registry, nonnull U294selector1script→RunPhaseEvent(U,1).
It doesnotfilterHP/status/count/phase/belong. ThenMove5→5 andredispatchbarrier;
readybarrieradvances→6 andredispatches. FreshMario init80372ec4/12c inventory
SHA25648d367d76dc627d4422d34f1d3d4e3716810a8463f5ec57cb544c53a1f649e9b
hasNOSetEventPhaseFirst/Phase. Goombella/trio inits likewiseinstallnone.
Their U294/298remain0fromsourceEntry. Frankly onlyU298=REL3dc40; U294=0.
Thus freshselector1fanout createsnoevents, butbarrierstillchecksactualownedIDs.

Move6 scans64pairIDsB220+8*i inascendingorderedtable position, ignoring-1,
missingunit,U298null. FirsteligibleFrankly: RunPhaseEvent(U,0), setthatpairID=-1,
PRESERVEitskeyword, Move5→7 andRETURN (noimmediatebarrier). Skippedpairsremain
unchanged; thereisnocompaction. RunPhaseEvent801297bc/90 readsU298, constructor
(script,priority10,flags20), writesownerE160=U0 andtrackedIDU29c. Itdoesnot
canceloldU29c first. Franklyphase3/4root CheckPhase04000001 false→RETURN;
no09201..05 replay. EventVM/scheduler stillownsroot/ID completion.

LaterMove7 barrierready thenCheckConcluded. Continuingbattle resetsMove5=6
andredispatches; noqualifyinghook now→Move5=0500000a andredispatchMove10.
ItdoesNOTdirectlyselectMove8. Move10alwaysconclusionfirst; ifcontinuing clears
allunitdamage264 and27c1000,thenPhaseEndCheck. Phase3emptylist→end1→
Audience_PerPhase(04000003),Move5→11,RETURN. Noaudiencephase2/4mutationbranch
inthatphase3PerPhasecall. Move11mask8-clearaudience0 +barrierready→12,RETURN;
Move12bothmessagepumpsidle→Phase4increments04000004,Move5reset05000000,RETURN.

Newphase4repeatsMove0..7. WhenitsPhaseEndCheck findseligibleSpiked/Para, result0
setsMove5=8 andRETURN. OnlyNEXTouterinvocation Move8clearsB18ffc12bytes,
allnonnullunit27c&=80002000, clears264/268/26c/280,half260,bytes270..273;
selectsoriginalactorlistentry, B420=ID, U27c|=1000, Move5→9,
Act6=Bf20=06000000. Itnormalizesdeferredstate1actorentries afterward.
Enemykind!=222 skipstheMario-owned upgrade/badge flagscans. Thenfalls intoAct0.

Audience_PerPhase iscalled inMove10ONLYwhenphaseendpredicate(orBef42000force)
saysend, beforeMove11. Itisnotcalledwhenstartingphase4orselectingeachenemy.
Phase2handoffmustretainphase3actualhooks/emptyactorpredicate ratherthanjump
straighttoenemyphase; itsnoactorpath neverentersunsafeMove8all-skipped region.

### Enemy Act0→3→6 exact fresh root launch

Act0 8011b31c resolvesUfromB420; effNiceAsync80144d44(1); clearsU284,U2d0;
BattleAudience_ActInit801a82d4. Localinhibitedstarts1 andonlyplayerkind222 or
224..230 canclearit afterstatus1/2/9/27tests. Enemykind1/2/3retains1, sochoose
Act3andRETURN beforeCommandInit/attr10/flipped-state routing.

AudienceActInit98bytebodycapturesbaseB2858, zerosbase+137d4, separatelyclears
baseflags masks10,20,40,80,100,200,400,800,1000 inthatorder (combined1ff0).
Phase-reactionmask8 isPRESERVED; do notinferitscleargatefromthisinitializer.
NoRNG/eventcreation. effNiceAsync scans7originalnice-resourcehandles andissues
asyncanimationresource requests; Act0ignoresitsreadyreturn. Resourceprovider
stillhasoriginalownership,notaninvented additionalAct0wait.

Act3 8011b56c re-fetchesactor, writesU2ac=1, ORU1c04000000,
setsAct6=06000006,RETURN. DoesnotoverwriteinstalledattackU2a0.
Act6 8011b5e0 doesNOTcallStatusWindowSystemOff becauseU2acbit1present.
Freshnormalbattle Bflags40000000clear, U2aclacks10/2, statuses1/2/9/27/6/5clear:
sourceevtEntry(U2a0,10,0). Noactor-kind reinterpretation, HPcheck, menuquery or
RNG occursonthisbranch. Constructorentrynonnull→U2a8=E15c,E160=U0; THEN
ifU290waitIDnonzero deleteIDandzero. Act6increments→7andRETURNS whetherentry
createdornot. Act3/6areseparateoutercalls; donotfallthrougharbitrarily.

U2a0sourcebindings: SpikedREL31310; flyingPara2fb0c; fallenPara groundinit3033c
reinstallsitsgroundattack (inspectnewunitfield,notoldflyingevent). Sourceenemy
roots owntargetsampling andactualattack; commandplayerSelectionisnotrequired.
Firstretainedbranch blockers are listedbelow withoriginalcalltargets.

### First used event command providers, exact ABI

EnemyItemUseCheck80104498/70: operands(actor,RAWoutput). Eval/translateactor,
lookupunit usingcapturedB, BattleEnemyUseItemCheck801f9658/148(U), writeinteger
returnpointertoRAWoutput,return2. CoreU308==0 returns0immediately at801f9678;
noitemtable/RNG/event. U308nonzeroadmission includesitemDataTable803108a8+
40*item,+24weaponpointer andoriginalitem-specificcheckgraph; do notfabricate0
outsideusedno-helditemgate. Itemsetup/drop/heldowner remains separatelybound.

GetEnemyBelong8010eed4/70: actorresolved/transformed, BtlUnit_GetEnemyBelong
80127454/2c readsS8(Uc):0→1,1→0,other→same; evtSetValue(rawarg1),return2.
Freshenemybelong1 producesplayergroup0. NoRNG, HP/status ormenuowner.

SamplingEnemy8010b664/b0 operands(actor,belong,weapon), ALLinteger-resolved;
belongnarrowsS8. Bcaptured, buffer=B428. Computebias=S8(-U8(B+a+8*belong));
ifweaponC=W68 hasmask10000000, invertbiasagain. PPCrlwinm8010b6cc bits3..3
means10000000, NOT20000000; earlieragentmessagewasincorrect andcorrected.
CoreBattleSamplingEnemy800ff5c0(buffer,W,actorID,S8belong,W64,W68,bias).
Noactorptrsubstitution, W7cchoiceflagsorcallerHWpad inputhere.

ChoiceSamplingEnemy8010b5e4/80 operands(weapon,RAWunitOut,RAWpartOut).
Resolveweapon; BattleChoiceSamplingEnemy800febc8(buffer=B428,W7c,&unit,&part);
writeintegerunitthenparttoRAWoutputs,return2. Noactortranslationforthisweapon.
SpikedWREL30d68: W64=01101260,W68=20001000,W7c=80002004.
FlyingParaWREL2f664: W64=01101260,W68=20000000,W7c=80002000.
GroundParaWREL3027c: sameadmission/choiceasSpiked. NeitherC has10000000,
sofreshplayer-side+1directionyieldsbias-1. BOTHchoicewords80000000 set,
thereforeoriginalchooser usesdeterministicmaximum-weight path, notrandomdraw.
Actualsamplinggeometry/rank/status/power fields stilldeterminechosenunit;
nohardcodedMario target. Thealreadyclosedchooser/samplercontractsandnative
packedprovidersareappropriate nextbindings.

Remaining sourceblocked branches: fortune-admission script803634f8/helpergraph;
Audience_PerPhase801a7c68 transitiveaudiencemutation andmask8SETAudienceCheck
801a6ce8; sourceenemyU308nonzero core801f9658 branches; installedREL attack
VMcallbacks aftersampling (motion/counter/guard/audio/camera/itemadmissions).
Fresh-cleargatescoveredabove do notcloseallbranches ornativefullturnfidelity.


## Spiked original attack used face/scale/offset/defense and pose fallback

2026-10-13 documentation-only callback follow-up. Original DOL disassembly
`/tmp/trio-facediradd.dis`, `trio-addscale.dis`, `trio-unitaddscale.dis`,
`trio-dispoffset.dis`, `trio-unitdispoffset.dis`, `trio-defenceresult.dis`,
`trio-setanimtype.dis`; REL162-command attack31310 inventory alreadypinned.

FaceDirectionAdd8010b85c/98 takes(actor,RAWcoordinate-token,amount). Integer
actorresolution/translation; integeramountresolution; capturedglobalB lookup;
step=low32(amount*S8(U189)); thenevtGetValue(coordinate-token), write low32
coordinate+step to SAMErawtoken viaevtSetValue;return2. Itdoesnotwriteactor
position/heading oruseU188movementdirectionfallback. SourceSub8010b7c4 differs
onlysubtractstep. AtSpikedREL31488 targetLW3/LW0/80 usesTARGET'sface sign,
notattacker'sface. Preserveevaluateamount-beforecoordinate-read ordering.

AddScale actualaddress801103dc/c0 (80110380 isnotthisentry). Resolveactor;
translatedID==-1 returns2beforeXYZreads. Otherwise lookupunit, XYZevtGetFloat
inorder→BtlUnit_AddScale801284d8/4c. Core singlefadds eachdelta toU90/94/98
unlessincomingaxisfloat exact-250000000,whichpreservesaxis. No positivity/
sizeclamp, noU114change. SourceSpiked319ac loops4times withencodedfloatdeltas
f24a7ad1=.0791015625, f24a7a06=-.119140625,Z0, WAIT_FRAME1 each. Fromscale1
thisderivesX1.31640625,Y.5234375,Z1 after4adds, then319d0SetScale(1,1,1).
Thisisquantizedoriginaleventdeformation, notinvented .08/.12renderdelta.

SetDispOffset80112978/128 resolvesactor, XYZINTEGERevtGetValue→signed-single,
thenBtlUnit_SetDispOffset8012832c/34 storesU54/58/5c withfloatsentinel
-250000000preservation. Nofloatresolver, originalposepivot/home/positionchange.
Spikedavoidancecalls at317b8 and31800; first(0,RAWsentinel,0)preservesY,
second(0,0,0) resets. Don'twriteU3cworldXYZortruncatecamera matrixinstead.

JumpContinue8010d0f0/118 isalreadyclosed in
[jumpcallback contract](BATTLE_JUMP_EVENT_CALLBACKS.md#jumpcontinue-and-the-strict-landing-boundary).
AtSpiked31790 itbelongstoprecheckavoidance route, notordinaryfreshhit1.
Firstcall sets eventbyte+d=1 andcopiesvisibleposition→moveCurrent; retain S/G/V/A.
Eachcall single-addY+V,V-G,btlMovePos(horizontalS/A),SetPos,sourcefloorquery;
strictY<floor snapsYandreturns1; equality returns0. No countdecrement/newaudio.
Itreturns originalhandler1 onlanding (complete/yieldprotocol), notgeneric2;
subsequentpose/sound commands followoriginalinterpreteroutcome scheduling.
Nativeoriginalmotionprovidercanbereusedwiththeseactualowner/firstcallfields.

ResultACDefence80107694/74 resolves/translatesTARGETunit andresolvesarg1weapon;
BattleActionCommandCheckDefence800fa1b8/268(Utarget,W), discardsregisterresult,
returns2 withoutanyeventdestination. SourceSpiked319e8 suppliesLW3 andW30d68,
then319f8CheckDamage(-2,LW3,LW4,W30d68,F100,LW5). Originaldefensecore stores
B1cb0, usesprivateACpadB1d1c history, evaluatesweaponbyte13, badge/status/override
fields, andresetsprivatepadonallreturnroutes. Fullcore/difficulty/windows/
anti-mashing areauthoritative in
[originaldefense](../input/BATTLE_INPUT_AND_DEFENCE.md). Don'tderiveguardfrom
wrapperreturn2 orreusepre-impactprivatehistoryafterthecore reset.

### Missing exact type43 has original category fallback

AnimeChangePoseType80106b20 firstcallsGetPoseNameFromType8012772c/80, ignores
thatreturn, thenSetAnimType801276ac/80 whichREPEATSlookup. Requestedtypes<=28
fallback28;29..39fallback39;40..69fallback69;>=70returnnull aftertablepointerload.
Scan8-bytepairsINORDER: recordtype>=70terminatesfirst; otherwisefirst exact
ORcategoryfallback matchreturnsname. Thisisnotexact-match-only norfirstexact
thenfallbacktwopass. Originalvalidtable/missingcategorydomainsremaincallerinvariants.

SpikedsourceposeTableREL30e28 endswithtype69→RELcad0TGK_S_1. No43record;
attacktype43 thereforeusesactualTGK_S_1. FlyingPara43usesPTK_S_1; groundPara
poseTable30158/a0 has69→RELc97cKUR_S_1. No inventedposealiasorrequiredasset43.
SetAnimType force1ONLYrequested29..39, otherwiseforce0; thenrequested29/30
ORP204bit10. Type43usesforce0; repeateddamage39usesforce1evenifsamename.
ItcallsbtlDispChangeAnime withtheactualresolvedfallbackname, notplaceholder.
See [complete pose lookup](BATTLE_UNIT_POSE_AND_DISPLAY_INITIALIZATION.md).

Fresh callbacks abovearelocallysourceclosed. Originalenemyguardinput/counter/
reactivehit/effect/camera/audio/resource/numeric providers andexceptionalstatus/
avoidancebranchesremainseparate; newnativeexecutionvalidation isnotclaimedhere.


### Verified relocated Spiked krb_get_dir semantic alias

Original tik `krb_get_dir@unit_togekuri.o` is section1+5b2c, file5bf0,
source loadedVA805c0590, size118. Native callback93105b2c is source text
section-relative binding, notfileoffset5bf0. Raw original helperSHA256
3049ee711a7765218b84e960ffc30e3da89a7f04edccd15e2f546d72353b2dfe.
`/tmp/trio-rel-krb.dis` andinspection-onlyELF recoveralloriginalPPC operations;
REL24imports annotatedusingread-onlyrelocationstreams, nooriginalcodeexecuted.

VerifiedABI/math agreeswithChristineDOL80189910: firstfouroperands evtGetFloat,
fifthoutputtoken evtGetValue capturespriorangle. dx=newX-oldX,dy=newY-oldY,
singlefsubs. Bothzero (fcmpu) writespriorangleback; otherwiseSYSTEMangleABf
8000703c(dx,dy,0,0), fctiwz, integerwritefifthoutput. Everypathreturn2.
SourcecallerLW5,LW6,LW9,LW10,LW8 areoldXY/newXY/outputangle, notactorIDs.

Eightoriginalmodule0REL24sites: file5c2c/3c/4c/5c→evtGetFloat80030840;
5c6c→evtGetValue80031260;5c9c and5ccc→evtSetValue80030cfc;
5cb4→systemangleABf8000703c. LocalADDR16_HA/LOsites5c72/7a and5caa/ae
resolveRELsection4+4d80=filecbe8; bytes00000000float+0. ThismatchesDOL
Christinezero804243cc. Register spill/constant load sequences differ, so this
isverifiedsemanticproviderreuse, notbyte-identicalfunctionaliasing. It is
correcttobindthisexactrelocatedaddress tothesametypedkrbprovider while retaining
originalsource module/addressidentity. OtherRELsame-namehelpers needtheir own
bindingchecks; no broadnames-onlyalias proof isclaimed.


### Native player-to-enemy phase progression (2026-10-13)

The native original-address runtime now executes fresh Move0..12 ordering
through phase2→empty phase3→phase4. Phase tables rebuild only at Move0.
Selector1 hooks fan out; selector0 hooks run serially through the source
positional pairs, retaining holes/keys. Frankly's original phase script executes
in phases3/4, checks literal phase1 and returns without replaying lecture text.
Move11 respects actual audience bit8 and the owned event barrier; Move12 uses
empty source message work before advancing the phase. Phase2 audience inventory
remains an explicit empty native presentation adapter. A mask8-set audience or
phase4 end stops at the missing original provider rather than skipping it.

Enemy selection uses the retained source phase table and original move counts.
Move8 sets source Act0, then separate Act0→3→6 calls launch the installed
Spiked script, binding owner/action ID and deleting wait afterward. Actual
AudienceActInit clears1ff0 and resets137d4 while preserving phase bit8; nice
resource requests and complete audience presentation remain adapters. Enemies
cannot enter the player menu. No next-turn count reset or turn increment is
introduced at this boundary.

EnemyItemUseCheck binds the exact held-item0 early return; nonzero held items
remain pending. GetEnemyBelong, SamplingEnemy and ChoiceSamplingEnemy use
original signed direction/weapon masks and the shared packed candidate services.
The original deterministic weighting selects the target, without an authored
Mario preference. FaceDirectionAdd updates the source local coordinate token.
Spiked's relocated krb_get_dir93105b2c uses the verified same XY/angle ABI as
DOL80189910. Its original jump/rotation event reaches ResultACDefence80107694,
which remains the next native impact boundary. Camera requests are retained
against the existing stage-camera adapter; their full movement/shake is unbound.

Typed pose lookup now follows source table order and band fallback28/39/69,
including force1 for29..39. Spiked type43 therefore selects original TGK_S_1;
a missing exact43 no longer stalls. Unit scale/display-offset setter leaves publish live source scale and world-axis
display offset in the recovered matrix order; later Spiked impact remains open.
Tests assert phase order, two gated Frankly roots/no dialogue replay, tracked
phase-ID cleanup, Spiked installed root/owner/Act3 flag, packed sampler actor,
player-menu exclusion and guard boundary. No enemy damage/victory/field return
is inferred. HP/reward and prior player-turn tests remain authoritative.

### Live unit scale and display offset placement during Spiked attack

Documentation-only source closure, 2026-10-13: rechecked original
`btlUnitPartsDisp80101774` PPC against the preceding complete matrix contract.
The live scale fields are **U90/U94/U98**, and the live display-offset fields
are **U54/U58/U5c**. These remain active during the original Spiked approach,
squash/stretch and avoidance streams; a rotation-only native adapter omits
visible original state.

Source loads display XYZ at80101804..1814 into the translation matrix at
stack+2e0. Scale loads/multiplies at8010188c..18c0 form
`(U114*(U9c*U90), U114*(Ua0*U94), Ua4*U98)` with the shown single precision
multiplication association. The camera mirror rebuild at80101eb0..1ee8
negates the Z product when its previously documented gate admits it; it does
not replace live X/Y scales. P4fc0 rotation concatenation at80101f50..1f7c
produces Rx*Rz*Ry; unit display translation is left-concatenated after pivot
restoration, as in8010191c..1938 and the corresponding rebuilt path.

For fresh ordinary `c_kuribo` family parts with zero part/pivot/base-angle
state and base scales1, the complete live matrix is:

`Mdraw = T(Ux,Uy,Uz-5) * T(U54,U58,U5c) * Rx(U6c) * Rz(U74) * Ry(U70)
* diag(U90,U94,signZ*U98) * Ry(A) * S(2,2,2)`.

Thus display offset translates in world axes, before the model rotations act
on local geometry. Current unit scale lies to the right of unit rotation and
to the left of incoming animation-facing yaw A and original model scale2.
Keep signZ from the original live camera/facing gate. For nonzero pivots,
part state or base angles/scales, use the full formula above rather than this
fresh reduction. Resource-local animation transforms continue beneath this
matrix. Numerical camera production remains the previously retained boundary;
this delivery does not invent a fixed camera value or change native code.

## Enemy impact: private defense state and installed player reactions

2026-10-13 documentation-only source recovery. Rechecked full original
BattleActionCommandCheckDefence800fa1b8/268, weaponGetPowerDefault80189d30/8,
Mario init/damage and Goombella damage event inventories, original player kind,
part, pose and data tables. No original execution/timing observation is claimed.

### Defense result owner and private-pad producer

The complete authority is [input and defense](../input/BATTLE_INPUT_AND_DEFENCE.md),
with [main-pass cadence](../input/BATTLE_INPUT_CADENCE_AND_SNAPSHOTS.md) and
[reset preservation](BATTLE_PAD_RESET_AND_RAW_PRESERVATION.md). These providers
are documented; an absent native implementation is not an undocumented source
producer. ResultACDefence80107694(target,weapon) evaluates both int operands,
translates target, calls core(U,W), discards its register output and returns2.

The core clears word B1cb0; difficulty is clamp(3+unsigned U306-unsigned U305,0,6).
A-window table802ee018 is[10,9,8,8,7,6,5], B-window802ee020[5,4,3,3,2,1,1].
Eligible superguard requires W13!=0 and signed HP U10c>0. Inside that branch,
positive signed override B1d18 decrements once/selects5, else U307!=0 selects5,
else an edge B200 in the B-window selects5 only if the combined A100/B200 count
in history0..14 is less than2. If5 was not selected, positive B1d18 decrements
once/selects4. Otherwise count>=2 selects1; a single A in A-window selects4;
otherwise nonzero count selects1, zero selects2. U307 alone does not force4
when its superguard admission is false. Every exit calls WorkInit8011a38c on
B1d1c, then returns the stored B1cb0. The reset preserves bytes/fields documented
by the reset contract; do not substitute an indiscriminate zero-fill.

Private trigger reader800fa1a0 reads word B1d34+4*index masked by button.
BattleActionCommandManager800fa5b0 unconditionally calls
BtlPad_WorkUpdate8011a0ac(B1d1c,channel0), even with no main AC callback.
General BattlePadManager8011a43c updates four pads before sequence control;
private update occurs after sequence control, before AC callback, and before
later evtmgrMain. This closes the direct source update producer and placement.
Native physical GP publication and preserved/reset lifetime must be bound to
this ownership, rather than reusing a general attack history. Other B1d18/U307
writers, tutorial injection, paused-loop and hardware impact timing remain the
bounded existing requests in input contracts; fresh all-zero state needs none.
With fresh counter0/U3070 and no A/B history, result2 is source-backed.

### Source player damage-event installation

Mario init80372ec4/12c (20commands, SHA256
48d367d76dc627d4422d34f1d3d4e3716810a8463f5ec57cb544c53a1f649e9b)
installs damage_event80417c08 via SetEventDamage at80372ee4. Its five commands
(28bytes, SHA25686fc788666d75cd17665e7cc28628660cf2b0e82afc15c8d9ce6a83c88e7787a)
set LW10=-2, LW11=1, RUN_CHILD defaultDamage8034cc60, RETURN, END.
Goombella init80393d9c installs damage80393e68. That six-command/40byte wrapper
SHA256a2601eb4b39fb08b19b7dcb05bd82bddbef224708bad7a3c452c39ed06c1a608
first SetRotate80111720(-2,0,0,0), then the same locals/child/return/end.
Importing only their waits leaves original U2b0 hit roots uninstalled; install
these source roots through source init/SetEventDamage ownership before impact.
Generic hit root lifecycle/cancellation remains the preceding BattleRunHitEvent
contract: owner ID, full damage code and actual part must survive dispatch.

### Fresh ordinary impact tuple and reaction assets

Spiked attack weapon tik file30d68 has relocated HP callback80189d30,
power word W20=2. Flying Para weapon2f664 has the same callback and power1.
That callback consists exactly of lwz r3,20(r4) and blr: no pouch or AC lookup.
Both records W13=1, elementW6c=0, patternW6d=0, specialwordW74=5087.
Player bodypart1 records80372a18/80393ba0 contain attributes9/counters0;
defense pointers Mario80417ba0/80417ba8 and Goombella80418450/80418458 each
point to five zero bytes. Kind attributes(+ac) are Mario02040000 and
Goombella02000000. Their imported U104 lacks100, so calculator ORs10000.

For fresh no badges/status/extra defense, stored defense2, F100 and positive
HP, [calculation](BATTLE_DAMAGE_CALCULATION.md) gives Spike H2 or Para H1,
Q0, code10117. [orchestration](BATTLE_DAMAGE_ORCHESTRATION.md) returns18;
ordinary direct tuple is (attackerID,target,part1,H,0,10117,pattern0,sleep1).
Keep current pouch-derived target HP; kind baseHP10 is not permission to reset
live HP. Increment D260 after HP/FP calculations, apply through original direct
HP/FP bookkeeping and hit event, then attackerA280/D273 tail. No HP-only native
reaction or invented death admission replaces the original scripts. Guard,
superguard, lethal target, status and nonfresh branches require their full
existing contracts and should remain explicit boundaries until bound.

Ordinary generic low-code23 branch at8034d454 sets unitflag1, LW14=39,
LW15=-1, absent-key0a brother and subsetDamage80350550 child. Mario data table
803729c8/50 and Goombella80393b60/40 have no key0a (decimal10), so that brother
returns without installing a custom effect script. Mario pose table803728b8
maps39→M_D_1(80422f28),28→M_S_1(80422ef0), bank b_mario(802f3fa8).
Goombella table80393a98 maps39→PKR_D_1(802f90f8),28→PKR_S_1(802f90e0),
bank c_pkr(804243b0). Preserve original type39 force restart semantics.

With pattern0, subsetDamage reaches ordinary branch803510d8: F100 calls
unk_80113f1c(target) at803510fc; then lowcode23 selects type39 at8035112c,
WAIT_MSEC1000 at80351140; LW15=-1 excludes secondary pose/wait30; RETURN.
Helper80113f1c/6c resolves/translates actor, looks up unit; if nonnull reads
U16+1fe, OR1, stores U16+1fe. Null also returns2. It produces no event output,
HP change, RNG or additional wait. Higher-damage special pattern checks precede
this branch but pattern0 does not admit their squash/shot/custom paths.
DefaultDamage fresh nonlethal F100 skips WaitAttackEnd, resets face and starts
original wait at the source tail after checking original home position; retain
that graph rather than clearing reaction state at the instant HP changes.

### Smallest next source-backed native step

Bind actual private-pad producer/reset/result owner (fresh no-input result2),
install original Mario/Goombella damage roots, execute ordinary CheckDamage
against imported weapon power/body tables and let original generic hit child
run its type39 wait/U1fe producer/wait restoration. Existing sound/effect/
audience requests retain their explicit provider boundaries. This scoped
closure does not establish full player death/revival, actual guard timing,
physical hardware cadence or enemy battle victory.

## Enemy recovery: source Spiked bounce and Para DivePosition

Documentation-only source closure, 2026-10-13. Original tik Spiked162-command
attack SHA2567ffecd92c5b3f826c170e9b2bc461f0dc98172fd30ddc8973544b285c7a92657
and Para99-command attack SHA256bb16a121b9a2a3d05075edb9ba65473d718ed15072974f6cf4b552220ee29f1a
inventoried again; btlevtcmd_DivePosition8010bf8c/934 and intpl_sub801254f4/67c
PPC inspected completely for the used mode0/curve0,1,4 paths. No native edits or
original execution/timing claim.

### Spiked impact continuation and smallest player reaction

After ResultACDefence(file319e8), CheckDamage319f8, Spiked sets pose43 at31a18
(source missing-type fallback69→TGK_S_1, previously closed). A brother at31a2c
reads int Z rotation, divides by10, multiplies−1, performs ten AddRotateZ steps
with WAIT_FRAME1, then sets XYZrotation0. Parent fetches current XYZ, sets Y0,
FaceDirectionSubX40 then JumpPosition(count28,sound−1) at31b0c; SubX20 and
JumpPosition(count12,−1) at31b40; SubX10 and JumpPosition(count6,−1) at31b74.
Then camera mode0, source walk sounds, pose41, speed4, fetchhome,
MovePosition(N0,sound−2,arg6zero)31c18, ResetFaceDirection, StartWait, RETURN.
Preserve source event invocation/child ownership, not a fixed native sleep.

JumpContinue8010d0f0 remains the complete
[jump callback authority](BATTLE_JUMP_EVENT_CALLBACKS.md), used in Spiked avoid
file31790: first invocation sets event+d1, copies visible→move-current, retains
V/G/S/heading; every invocation addsV tocurrentY, subtractsG fromV, integrates
horizontal S/heading, publishesvisible then groundqueries. Only strict Y<floor
clampsandreturns1; equalityreturns0. No countdecrement, trajectoryreset or snap
against storedtarget. Next script command follows a yielded boundary.

Ordinary fresh player reaction remains the preceding installed U2b0 wrapper,
generic defaultDamage→subsetDamage, type39 wait1000ms and U16+1fe OR1 helper.
Its source checks/leaf providers are CheckDamageCode8010f2e8,
CheckDamagePattern8010f0e8, GetDamageCode8010f374, GetDamage8010f458,
OnUnitFlag80105b40, RunDataEventChild8010a010 (missingkey0a),
AnimeChangePoseType80106b20, unk80113f1c, ResetFaceDirection8010e4bc and
StartWaitEvent8010a26c, plus already bound movement/home checks. Import source
wrappers and preserve VM cancellation/owner before adding synthesized feedback.
No live-player death/revival completion is claimed by this bounded next step.

### Para used DivePosition ABI and retained fields

DivePosition has **nine** operands `(actor, X, Y, Z, N, amplitude, curve, mode,
soundMode)`. Resolve every operand with evtGetValue, actor through BattleTransID;
XYZ integer→single conversion. It does not use evtGetFloat for destination XYZ.
Curve>=100 strips100 and enables ground clamp; used Para curves allbelow100.
All five Para calls use mode0, so orientation-offset mode1/other branches are
outside this used closure. Source destinations pass through the existing
float-sentinel-aware SetMoveTargetPos; no raw-XYZ shortcut should bypass that.

On first invocation only, set event+d1, **overwrite** U16c=N even when N0,
GetPos, SetMoveCurrentPos(U154/158/15c), SetMoveStartPos(U148/14c/150),
SetMoveTargetPos(U160/164/168). Compute full3D start→destination distance,
not horizontal distance: squaredY then fusedX² and fusedZ²; source sqrt via
frsqrte+three refinements, round-single, storeU180. N0 becomes
trunc(single(distance/U170speed)); nonzeroN instead sets speed=distance/N.
If finalN0, return2 immediately. Otherwise elapsedU184=0, U17c=0;
move-directionU188 becomes±1 only when destinationX differs, else retainsold.
BtlUnit_LoadSeMode80126588(type3,soundMode,U1c4,U18c) initializes source sound
owner. Platform square-root arithmetic can remain an explicit approximation
boundary; do not describe ordinary std::sqrt as exact PPC equivalence.

On **every** invocation, including initialization:

1. `t=trunc(U184)`; for each axis, call
   `intpl_sub(curve,t,U16c,startAxis,targetAxis)` and store move-current.
2. Optional curve>=100 ground query: strictcurrentY<floor writes elapsed=N,
   updates targetY tofloor (and currentX/Z target); usedPara doesnotadmit it.
3. Increment elapsedU184 bysingle1. Ifelapsed>N, SetPos(targetXYZ),
   _UnitMoveSoundControlEnd8010482c, return2. Equality remains inordinary path.
4. For mode0, recompute remaining **3D** distance target−interpolatedcurrent.
   If remaining0 use1 for its ratio; if totaldistanceU180==0 write1 there.
   Calculate `s=sinDegrees(180*(1-remaining/totaldistance))`. Amplitude0
   makesoffset0; negative amplitude negates s then multiplies positive−amplitude;
   positive amplitude multiplies s byamplitude. Publish visibleXYZ with
   currentY+offset, leaving storedcurrentY interpolation-only. Call
   _UnitMoveSoundControl80104a24, return0.

The duplicate endpoint interpolation with remaining0 substituted1 is original;
don't force sine0 or terminate atelapsed==N. Final next-call snap isreturn2,
not JumpPosition's landingreturn1. N remainsconstant; elapsedincrements.

Original used interpolation jump table80364148 selects:

- curve0→80125544: `start + ((float(t)*(end-start))/float(N))`, separate
  single subtract/multiply/divide/add.
- curve1→8012558c: `start + ((end-start)*float(low32(t*t)))/float(low32(N*N))`.
- curve4→80125834: `q=low32(N-t)`, `delta=end-start`, then
  `(start+delta) - ((delta*float(low32(q*q)))/float(low32(N*N)))`.

Products use PPC mullw/signedword conversion, not floating-square arithmetic.
No elapsed clamp exists in this used intpl_sub branch. Preserve operation order.
Sin-degree service80125cd4 and numeric angle table/provider remain the existing
math ownership rather than adding a new fabricated curve.

### Para source calls and home recovery

Para attack DivePosition file2fcd0 is `(N0,amplitude−16,curve0,mode0,sound−1)`;
2fda8 `(0,+5,4,0,0)`; avoidance2ff0c `(0,−20,1,0,0)`;2ffb8
`(0,−16,1,0,0)`; allactor−2 andXYZLW0..2. After impact30020/30030,
label98 camera0, original flysounds, pose42, speed6, GetHomePos at300bc,
**recovery300d4 `(−2,LW0,LW1,LW2,0,−10,0,0,−2)`**; ResetFaceDirection,
StartWait, RETURN. Animation/VM/sound requests remain sourceordered.

### Fresh weapon/status resistance and guard boundaries

Spiked30d68 and flyingPara2f664 both have W80..af entirelyzero (status
application payload), FP callback0, HPcallback80189d30 returningliteralW20
(2/1). Their normal element0, pattern0, W13=1 and special5087 are recorded
above. Target body elemental defense/attributesfivezero, freshstatuses/badges0
admit ordinary positive damage tuple already closed. No resistance or proc
probability is fabricated; nonzerofuturestatuses require sharedstatuscontract.

[Damage orchestration](BATTLE_DAMAGE_ORCHESTRATION.md) also closes the used
W13=1 superguardcounter dispatch: W74 lacks20000 and targetACpossibility must
be nonzero; result5 firstOR80000intoF, thenOR100, defenderDirect(Aid,D,P,0,0,
F|42,pattern0,sleep0), attackerDirect(−5,A,null,1,0,311decimal,0,sleep1),
core return−1/wrapper255. AttackerDirect starts original damage root and can
cancel its currently tracked attack event; do not keep executing that root
as if superguard were just targetHP0damage. With F100, defendercode8012a and
attacker literal137hex are retained separately. Effects/audience remain requests.
Ordinary result4 OR40000intoF, reducing HPdamage1: fresh SpikeH1/code50117;
ParaH0 uses calculator defaultcode4011e, still original directreaction.
DefaultDamage's earlier40000 gate selects type31 guardpose andWAIT_FRAME30
before its source tail. Arbitrary counter/status/effect/downstream ownership
is not newly closed by this data binding.

### Used SetMoveSoundDataWork producer closure

Original pending callback80105020 is now closed in
[unit sound-mode producer](BATTLE_UNIT_SOUND_MODE.md#source-event-producer-setmovesounddatawork).
Eight evaluated operands; both keys evtGetValue; signed16 numericparameters.
SetSeMode80126558 writes U18c record fields in original order, notably evaluates
but **ignores operand5**, repeats operand4 into both period halves, preserves
remaininghalf12 until later sound−2 LoadSeMode, and resets voiceword18 without
stopping it. Spiked31bb4 `(keys KURI_MOVE1/2;0,3,3,0,0)` and Para30070
`(sameflykey twice;0,15,15,0,0)` source operands/relocatedkey locations pinned.
No native edits or fabricated audio playback completion.

## Phase4 audience drain and superguard callback cancellation

2026-10-13 documentation-only source recovery. Complete local
BattleAudience_PerPhase801a7c68/10c,
BattleAudience_CheckReactionPerPhase801a6ce8/f80,
BattleAudienceSetThrowItemMax801a5a0c/a0, GetSysCtrl801a2960/60,
GetExist801a29c0/64, audience pointer801a8664/3c,
BattleBreakSlot_DecBreakTurn80203330/40 and CheckDamage event wrapper8010a78c/fc
rechecked in original US DOL. Full nonempty audience action/event execution is
not asserted solely from inspection of this controller. Ignored evidence lives
in /tmp/trio-audience-*.dis, /tmp/trio-enemy-check-wrapper.dis.

### PerPhase4 writes and mandatory RNG ownership

Audience base A is original B2858 pointer. PerPhase(04000004) firstcalls
SetThrowItemMax at801a7d44, **before** setting flag8/state0. That leaf captures
A, requests irand(100) at801a5a28, and assigns wordA137f4 as follows: signed
result<80→0; <90→1; <95→2; <100→3. For >=100 it preserves oldword (original
boundedRNG makes this unreachable); negative signedresult selects0. Then
writes A137f0=0 and A137f8=0. Return toPerPhase: reloadAflags, OR8, storeflags,
storewordA137dc=0. Do not skip the RNG draw because native audience isempty;
this source leaf contains no spectator-count gate. Retain the existing explicit
native RNG seed adapter rather than describing its seed as original evidence.

PerPhase(04000002) cleanup and otherphase handling remain aspreviouslyrecorded.
Forphase4 no item/actorallocation, statuschange, queuecountclear, eventlaunch or
pouchmutation occurs in this initializer beyond the above transitive writes.

### Audience record admission and empty adapter scope

GetPtr801a8664(index) returns A+1bc+134*index; normal controller scans0..199.
GetExist tests recordword0: bit1hex mustbeset, mask100 mustbeclear, mask2
mustbeclear. GetSysCtrl firstGetExist; false→0, else returns1 precisely when
recordword0 mask10 isclear. Thus zero-initialized records produceSysCtrl0;
GetSysCtrl is not just a count word or unconditional nativeabsence.
CheckReaction capturesA and calls pouchGetPtr800d5eb0 **even on flag8-clear
immediate readiness**, then readsAflags. Flag8clear returns0 without statework.

An explicit native empty audience can execute the following original consumers
against zero/absent records and queuecountA13904=0. This is a bounded adapter,
**not evidence original setup4 authored/created no audience**. Original audience
population/renderer/throwitems/SP still require source initialization and
resources. Never claim parity from skipping those systems. Admission requires
livepacked flags, recordexist/control gates, queuecounts and breakslotowner.

### Exact empty phase4 three-call drain

StatewordA137dc original jump table is8039ceac (21entries, not803aceac):
0→801a6d40;1..4→busyreturn;5→801a6d60;6/7→801a6df8;
8→801a6e70;9→801a6e94;10→801a6ec0;11→801a6f04;
12→801a7344;13→801a6f30;14→busyreturn;15→801a7b5c;
16..19→busyreturn;20→801a7c30. Out-of-range also returns1; unsupportedstate
cannot silently become readiness. These entries identify the graph; only the
following zero-record/emptyqueue usedpath is now closed for native binding.

1. Withflags8set,flags40000clear,state0: setstate5 then scan200records;
   onlySysCtrl1,kindbyte1b==9,statusbyte19==18 records decrementword12c;
   <=0→ChangeStatus19 and recordcount++. Emptycountsetsstate7. Fallthrough
   immediatelyscans200 forSysCtrl1/kind9/status19; none→state13; return1.
2. State13: copy queueA138cc.. using countA13904 toprivatestack, capturecount,
   setlivecount0. Emptyoldcount skips every category/append branch and RNG
   choice, leavescount0, setsstate12/substateA137e0=0, fallsintostate12.
   Because queuecount!=1, state15 iswritten at801a7b50 and returns1.
3. State15: scan200 records. SysCtrl1/status13 decrementswordf4; <=0 calls
   ChangeStatus3 and setswordf4=0. Scan200 again; SysCtrl1/kind8/status17
   decrementsword12c; <=0 setsrecordword4=4. Emptyrecords take neitherwriter.
   Call BattleBreakSlot_DecBreakTurn80203330; then common801a7c30 clears
   flag8, zerosqueuecountA13904 and returns0.

If sourceflags40000isset, state0 writes5then15 and returns1; subsequent15
uses samefinalpath. Other existingflags are preserved byfinalmaskclear8.
DecBreakTurn obtains work at B1616c through pointergetter8020524c, writes
wordwork10=low32(old−1), reloads signedresult and ifnegative writes0. This is
not a 64-bit saturating mathematicalsubtraction; wrap occurs before signedtest.
State15 calls it once; explicitstate20 entry commoncleanupdoesnotcallit.
Original state0/5/7 and15 scannedrecord branches use ChangeStatus801a2a24;
when recordadmission is nonempty or queuecountnonzero, source status/event/
category graph remains requested, rather than hardcodedempty completion.

### Original superguard1 order and wrapper termination

For admitted superguard5 againstSpiked/Para W13=1, existing
[damage orchestration](BATTLE_DAMAGE_ORCHESTRATION.md) providesexactordered:
checkW74lack20000, targetACpossibilitynonzero, storedresult5; niceeffect with
hitoffset/scale.75; F|=80000; soundgreat; audienceGuardGood; **A280increments
once and D273 receivesitslowbyte BEFORE both directcalls**. ThenW13=1falls
othercase→F|=100; defenderDirect(attackerID,D,P,0,0,F|42,pattern0,sleep0);
attackerDirect(−5,A,null,1,0,311decimal,pattern0,sleep1); corereturn−1.
It bypasses normal counterselection/calculators/status/commontail increment.
DefenderfullcodeF100becomes8012a; attacker literal311 is137hex. No attacker
code10117 is synthesized for this already-direct call. Original hit-event
replacement on attacker can cancel its trackedattackU2a8 through source
BattleRunHitEventDirect ordering; source VM handles this lifecycle.

EventCheckDamage8010a78c resolves actor,target,part,weapon withGetValue;
flags/outputtoken raw. At8010a844..850 **negative** coreresult returns255
immediately: noevtSetValue, **noevtCheckID**, no outputoverwrite. This applies
regardless of whether ownroot wascancelled. Only nonnegativepath writes raw
outputtoken first, then calls evtCheckID(entry15c); active→2, absent→255.
Don't fabricate result18/output18 forsuperguard or requireCheckID before
preserving a negativebranch's unchangedlocal. Capturedunit/part pointers and
callingentry can remainreferenced through dispatchsideeffects; don't resume
a cancelledattack just because CheckDamagewrapper returned255.

### Smallest next native step and retained requests

BindPerPhase4 original RNG/reset/mask8 initialization, then explicit zero-record
GetExist/GetSysCtrl/emptyqueue sourcepollpath and BreakSlotword10 mutation.
This legitimately advances phaseend for the existing empty-audience adapter;
it doesnotimplement original crowd resources, interaction, nonempty21-state
reactions, SP/bingo/throwitem mechanics or victory. Retain those producers in
inbox as boundeddependencies. Guard/superguard native tests must assert
bookkeeping and rootownership/outputpreservation, separately from audiovisual
services and original gameplay timing observations.

### Phase6 turn boundary, TurnEnd identity and original player level producer

Rechecked original btlseqPhase8011c2b4/9c, btlseqTurn8011c510/1ed8,
BattleAudience_Case_TurnEnd8019f298/168, btlseqTurnFirstProcess8011b1ec/f0,
and used BtlUnit_Entry80128fe0/6b4 player-level branch.

Packed owners: **Turnbank3 wordBf14**, **Phasebank4 wordBf18**, Movebank5Bf1c,
Actbank6Bf20; **turn count signedhalfB0**, notBf14 orBf18. Phase6
04000006 at8011c320 callsGetSeqStep(B,3) (incrementsTurnbank3) and
SetSeq(B,4,04000000), returns. Thus Turn0300004c becomes0300004d; no count
increment occurs there. Next outer btlseqTurn dispatch4d at8011e3b0 calls
**BattleAudience_Case_TurnEnd8019f298(void)**. It is an ordinary no-argument
DOL service, not evtcallback (no event args or2return protocol). Only after it
returns do8011e3b4..3d0 setTurn3=03000001 and clearBef4 mask2000, thenreturn.
Retain pendingatthatservice until providers exist, rather than resetting turn
count/moves early or treating phase6 as battlecompletion.

TurnEnd captures globalB, source MarioPtr800f87a8, audienceA and pouchptr.
Loads B2738→FieldInfo+c→NpcInfo+18→originalgroup, with group+0unitcount and
+4unitsetpointer; each original unitset stride30 has word0kindpointer. It does
not enumerate only surviving native units. Audienceflags40000 set skips its
entire mutation suffix **after these pointer loads**; artificialsetting that
flag to bypass services would changeoriginalscope.

Otherwise: A13908 word+=3 and signedclampupper100; sumunsignedkind+0e across
originalgroupsets. Threshold=max(5,signeddiv((2*sum)low32,unsignedMarioU+d)).
ForbattleturnS16B0>=threshold, getAagain and call
BattleAudienceAddTargetNum801a1a74(sourceconstants*A13778,sourceconstantarg2).
Ifturn==threshold, AddPhaseEvtList801a1520(6). Mandatoryirand100 follows;
ifdraw<A13908: ifturn>=threshold AddPhaseEvtList(1), then enqueuecategories
4,9,10,11,12,13 inthatorder; A13908=0. Always afterthischancebranch call
BattleAudienceNumToTarget801a1c60. These leaf audiences/targetcount/category
writers remain boundedproviders to recover/bind; emptycrowd doesnotlocally
skip groupmath/RNG/categoryqueue/NumToTarget. Futureturn implementation must
preservethegraph rather than a voidsuccessstub.

Original playerlevel byteU+d producer is explicit: Entry801290c8..d0 copies
kind+0e toU+d; forkind222or224..230, branch801290f0 calls pouchGetPtr800d5eb0,
reads signedhalf pouch+8a, writes lowbyteU+d at801290f8. This level producer is
separate fromHP importing through SetParamFromPouch80127134. Native existing
fresh pouchlevel1 adapter can seed Mario/Goombella U+d via this source branch;
leaving playerU+d0 isunfaithful and would maketheTurnEnd divisioninvalid.
Do notcallkindbaseHP10 alevel or silently inferhigherlevelsfrombattleturns.

Turnstate1 next calls TurnFirstProcess at8011c568..578 beforeincrementing
Turnbank3again/recursingTurncontroller. Original FirstProcess order:
BtlActRec_JudgeTurnRuleKeep8018f77c(); incrementS16B0 (narrowthenreload);
ifnewturn>=2 BattleMajinaiCheck800f7774(); B18ff4=0 thenB18ff0=0;
BtlActRec_AddCount8018f990(B16f57); scanall64units: nonnullU24byte++,
BtlUnit_ResetMoveStatus801266d4(U), then irand(S8kind89), setU28 to
S8kind88+draw-trunc(S8kind89/2); CheckStatusFlag(U,01000000) then iftrue
ClearStatusFlag801277d8(U,01000000). FinallyBattleFogEndCheck80147c2c().
OriginalResetMoveStatusfreshnostatus writesU20=0,U22=U21,U23=U22; itsSlow/Fast/
zero-move branchesremain sharedstatusauthority. Allnonnullunits participate,
includingFranklycount0; don'tdrop its source RNGrequest becauseitcan'tattack.
Rulekeep/Majinai/Fog/targetcount/audiencewriters are explicit nextfunction
boundaries, notauthorizations to manufacturefreshturnstate or victory.

### Used WaitEventEnd8011389c poll closure

2026-10-13 documentation-only original US DOL inspection. Complete
btlevtcmd_WaitEventEnd8011389c/3c SHA256
291f1a6b795d1e9b69a1413fb71db88f1d3d7946c07eda59a2ac59aac6f3d95e;
transitive evtCheckID80036604/60 also rechecked. Inspection artifacts
/tmp/trio-wait-event-end.dis/.elf and trio-wait-event-check.dis/.elf remain
outside source history; no native code edits or original timing observation.

ABI: **one int event-ID operand**, evaluated through evtGetValue(event,arg0)
at801138b0. Pass resultingID unchanged toevtCheckID80036604 at801138b4.
Return **0 if ID is live/nonzero checkerresult**, **2 if checkerresult0**.
The subfic/subfe/andc sequence implements this boolean mapping; no return1.
It does nottranslate aunit, inspectfirstCall, setevent+d, assignoutput, clear
trackedID/local, deleteevent, adoptchild, settimeout or launchnewwork. Everypoll
reevaluatesoperand and checkscurrenteventcontext. A success-onlystub bypasses
source brother ownership/barrier; a yielded1 would introduce a false boundary.

Original evtCheckID readsGP+14:0 selectsfieldcontext803d8d30, nonzero selects
battlecontext803d8dd0. Readcontextword0count andword90entrybase. For each slot
stride1b0, onlyentrybyte8mask1active andentryword15c matchingID returns1;
otherwise0afterentirescan. Paused/typeblocked butactiveentry stillcounts live.
It doesnotsearchbothcontexts, useanativepointerasID, orrequireparent/owner/unit
matching. An absent/staleID completes2; zero follows thesamechecker rather than
an additionalzero-ID shortcut. Currentnative event allocator normallyhasno
activezeroID, but this wrapper itselfcontainsnospecialID0branch.

Used generic reaction site subsetDamage80350cd8 is WaitEventEnd(LW0) for the
preceding original brother ID. Preserve that ID and let the ownedbrother finish
through originalscheduled callbacks; this polling helper supplies no mechanism
to shortcut its scale/motion/interpolation dependencies.

### Superguard8034f4e8 used integer interpolation producer

Confirmed pending8034f4e8 is init(5,0,1800,40) inoriginal
btldefaultevt_avoid_counter_event8034f4cc, notaudio/effect.
[Default action events follow-up](BATTLE_DEFAULT_ACTION_EVENTS.md#used-superguard-counter-avoid-spin-and-shared-interpolation-state)
nowpins actualsource12cmdstream/sharedLW11..15overwrite, getter80055f90 twoRAW
outputs FLOATsampleLW1/INTcontinuationLW2, sourceintplGetValue floatmode5cubic
operationorder (distinctfromintpl_sub integers),40samples0..39 andlackoffinal
rotationzero. ScopedcameraInterpolation mayserveexistingnativefloatadapter;
originalcompiler/FPSCRnumericsremainvalidationboundary. No nativeedits/commit.

### Confirmed source superguard floor and pose49 provider boundary

Sourcepending8010510c is TransStageFloorPosition(intX,RAWYout,intZ), notsound/
effect. [Jumpfloor follow-up](BATTLE_JUMP_EVENT_CALLBACKS.md#used-counter-recovery-stage-floor-wrapper-and-original-initialization)
pins evaluatedX/Z, unevaluatedYoutput, globalstageB163fc, exactinitialized
B16f28=90/B16f30=−50 andsourceZ<=90 floor0, wrapperintoutput/2return and
actualcounter803516b8 continuation. Fullstagewriters/inittransitives remain
bounded; no codeedit or arbitraryunconditionalfloorconstantadmitted.

Pose49 inoriginalMario table803728b8 at803729b0 hasnamepointer802f3f80
**M_J_1B**, originalbank b_mario802f3fa8. Type49 matchesbeforefallback69;
GetPoseNameFromType shouldnotselectidleM_S_1 merelybecausemode49 isunbound
natively. OriginalSetAnimTypeforce0forrequested49 (outside29..39). Goombella
pose49 tableentry80393b48, name802f9138 **PKR_A_1**, bank c_pkr804243b0. Preserve source
case andusedposebank; visualimportavailabilityisaseparate nativeprovider.
Confirmedavoidcounter8034f4cc requests49thenoriginal40stepmode5Yspin, with
no final0rotationfabrication. Nativecurrentfloorpending shouldremainexplicit
until typedstageproviderisbound; originalroundoutcomes/victoryarenotclaimed.
