# Professor first conversation and fence removal

US G8ME01 disc0 revision0, read-only static REL/DOL/message evidence. Scoped
answer to [OW-ROAD-01](../../requests/OW-ROAD-01.md). Script requests,
milliseconds and coordinates are source data, not original runtime observation.
The [same-map door contract](../overworld/OVERWORLD_ROGUEPORT_INTERIOR_DOORS.md)
owns admission and door travel. The [lecture contract](LECTURE_FIELD_ENTRY_AND_RETURN.md)
owns story10 crossing, numeric choice branches and battle return.

## Triggers, actors and assets

Professor house open callback0x4599c checks raw story0xf5de0180 exactly8,
then RUN_EVENTs first-talk at0x45a84. Close callback0x45b98 checks exactly9,
then RUN_EVENTs fence removal at0x45c4c. Both scene events wait for door end;
first-talk places the Professor before that wait. Do not run dialogue while
generic door travel remains busy or replace those gates with proximity alone.

Professor actor identifier is original Shift-JIS `クリハカセ` (REL0x18a3c).
NPC record0x44ff4 has flags0x80600, init0x45000→0x410c8 and
 talk0x45008→0x412e0. DOL tribe record0x80333014 binds actor-name0x802e51bc,
model0x802e51c8=`c_kuri_h`, then pose pointers resolving
S_1,Z_1,S_1,T_1,W_1,R_1,d_1,d_1. Original model resource is `a/c_kuri_h`,
texture companion `a/c_kuri_h-`; `c_kurio` is a different character. The pose table
roles need their NPC owner; strings alone do not establish complete animation
coverage. Goombella retains the established original c_pkr partner model.

Professor init0x410c8 first sets NPC flag0x40000000. Story8 positions him
(-135,0,-240); story9 hides him(0,-1000,0); story10 positions him(-220,0,305).
First-talk overrides position(-115,0,-225), yaw90 before door-end wait.
All coordinates below are original map units; convert only at the native adapter.

## First-talk chronology and concurrent ownership

First-talk is linear: no local IF/selection branches. The ordered request table
below includes every original command, preserving movement, cameras, waits,
poses and concurrent brother begin/end boundaries. Message requests have a
separate resolved-resource table. Do not serialize a brother into the parent
without preserving the original service dependency and shared variable waits.

Entry clears S_ie_02_naka flag2, positions/turns Professor, waits door end,
turns music off0x400, keys off, stops party0, waits500ms, Mario fukidashi
 type3/duration30, waits500ms, Mario display flag(1,4), turn90/200ms;
Goombella moves(-190,-100,speed120), turns90/200ms, then first message.
Only after that message does camera(-115,65,-20,-115,30,-217,666ms,11)
start; wait666ms then BGM_EVT_FRANKURI1 and Professor yaw270/200ms wait.

Explicit Mario poses are M_I_G,M_V_2,M_D_8,M_U_3,M_S_1,M_C_3.
After069, Mario's M_I_G/M_V_2 and voice precede brother message069_01,
which clears shared0xfaa2b580 after finishing. Parent fall sound/shake/M_D_8
runs concurrently; after1500ms opcode11 waits on shared0xfaa2b580. Recovery
M_U_3/500ms/M_S_1 is also a brother. These are presentation ownership rules.

The map item presentation creates item00 kind0x60 at MarioXYZ+(20,20,10),
flag2 and encoded scale0xf24a7cb3=0.5498046875, then places it(-140,0,-220).
Later it moves to MarioXYZ+(20,20,10) and hides(0,-1000,0). Do not infer an
inventory grant from this display entry. Speed literals decode120/40/80/60
for the used movements; all literal floats use (signedWord+230000000)/1024.

First-talk tail saves Professor XYZ, hides his NPC, stops party1 and calls
 evt_mario_set_party_pos(1,11,savedXYZ). Music/environment restore requests,
cameraoff500ms/type11, party_run1 then0, story=9, keys on follow. Story9 is
not the final prologue state: Professor occupies temporary party1 until exit.

## Fence removal and continuation

Fence event waits door completion, keys off,500ms wait, Mario turn270/200ms,
then camera(-270,100,595,-270,30,201,2000ms,11). It reads party1 XYZ, clears
party1, restores Professor NPC there, turns270, sets0x40000020, moves
(-265,187,speed180), prints083, waits500ms. Paper OFF_d_saku uses image img
and offscreen ofs captured from S_saku04, depthPlaneOffset=-2, aspect4:3,
initial animZ_1,shadow2. Hide S_saku04 flag1; brother fence-open sound then
1000ms/fence-fall sound; parent animA_1,wait_animend, image hide and cleanup;
finally disable collision A_saku_04. An omitted static fence is insufficient.

Camera(-210,85,595,-210,15,201,1000ms,11), Professor turn90, move
(-220,305,speed180), two no-hit jumps225/30, message084, mask0x40000020off,
wait500ms, cameraoff500ms/type11, party_run0, keys on, story10 at0x494e0,
RUN_EVENT lecture0x494ec. Reload gor_02 at story10 separately starts lecture.
Story11 is the lecture's common return. Fence event has no selection branches.
Lecture084_02_yesno result1 enters battle; otherwise084_03_yesno result0
enters battle; otherwise084_04 skips battle. Full used controls and return
are retained in the linked lecture contract; do not treat fence completion as11.

## Original message controls and service dependencies

All resolved IDs below exist uniquely in msg/US/gor_02.txt. The resource SHA256
is1958e3d22bb27b01332e08b67f279a33f21e0e8a3aba2b947ad8c2a446144f16.
No select token appears in first-talk/fence messages. Party print requests
expand to _kur keys;069_02 is explicitly evt_msg_print with speaker party and
uses its unsuffixed payload. Original payloads remain ignored local assets.

Tokens include k,p,wait,scale,dynamic,/dynamic,shake,/shake,/scale,dkey,/dkey,
col,/col. Preserve original `</dynamic 3>` in081. Source color token is col,
not color, and scoped red value is c00000ff. [Parser records](../message/MESSAGE_PARSER_RECORDS.md)
and [playback](../message/MESSAGE_PLAYBACK.md) already establish dkey:
0xfff5 payload1/0 at scheduleT; nonzero consumer setsflags0x80,clearsbit4,
setsE=record.schedule; zero clears0x80 only. Native input admission must honor
this gate; silently stripping it changes073/073_01. Dynamic/scale/shake/col
must retain actual original formatting rather than sanitize original text.
Camera interpolation, paper/offscreen/material animation, effects/audio macros,
NPC/party conversion and concurrent event scheduler remain native dependencies.

## Exact message bindings and token census

| Event/order | Speaker | Request → resource | Original token sequence |
| --- | --- | --- | --- |
| kurihakase_first_talk / 1 | Goombella | mac_0_066 → mac_0_066_kur | `<wait 250> <wait 250> <k>` |
| kurihakase_first_talk / 2 | Professor (クリハカセ) | mac_0_066_01 → mac_0_066_01 | `<k> <p> <wait 100> <k>` |
| kurihakase_first_talk / 3 | Professor (クリハカセ) | mac_0_067 → mac_0_067 | `<wait 250> <wait 250> <wait 250> <k> <p> <wait 250> <k> <p> <wait 250> <wait 250> <wait 250> <wait 250> <k>` |
| kurihakase_first_talk / 4 | Professor (クリハカセ) | mac_0_067_01 → mac_0_067_01 | `<wait 250> <k>` |
| kurihakase_first_talk / 5 | Professor (クリハカセ) | mac_0_067_02 → mac_0_067_02 | `<wait 250> <k>` |
| kurihakase_first_talk / 6 | Professor (クリハカセ) | mac_0_067_03 → mac_0_067_03 | `<scale 2> <k>` |
| kurihakase_first_talk / 7 | Professor (クリハカセ) | mac_0_067_04 → mac_0_067_04 | `<scale 2.5> <dynamic 3> <shake> <k>` |
| kurihakase_first_talk / 8 | Professor (クリハカセ) | mac_0_067_05 → mac_0_067_05 | `<dynamic 3> </dynamic> <k>` |
| kurihakase_first_talk / 9 | Professor (クリハカセ) | mac_0_067_06 → mac_0_067_06 | `<dynamic 3> </dynamic> <wait 250> <k>` |
| kurihakase_first_talk / 10 | Goombella | mac_0_068 → mac_0_068_kur | `<wait 250> <wait 250> <wait 250> <k>` |
| kurihakase_first_talk / 11 | Professor (クリハカセ) | mac_0_069 → mac_0_069 | `<wait 250> <k> <p> <wait 250> <k> <p> <k>` |
| kurihakase_first_talk / 12 | Professor (クリハカセ) | mac_0_069_01 → mac_0_069_01 | `<dynamic 3> <scale 2.3> <k>` |
| kurihakase_first_talk / 13 | Goombella | mac_0_069_02 → mac_0_069_02 | `<wait 250> <wait 250> <wait 250> <k>` |
| kurihakase_first_talk / 14 | Professor (クリハカセ) | mac_0_069_03 → mac_0_069_03 | `<wait 250> <k> <p> <k>` |
| kurihakase_first_talk / 15 | Goombella | mac_0_070 → mac_0_070_kur | `<k> <p> <k>` |
| kurihakase_first_talk / 16 | Professor (クリハカセ) | mac_0_071 → mac_0_071 | `<wait 250> <k> <p> <k> <p> <k>` |
| kurihakase_first_talk / 17 | Goombella | mac_0_072 → mac_0_072_kur | `<k> <p> <wait 250> <k>` |
| kurihakase_first_talk / 18 | Professor (クリハカセ) | mac_0_073 → mac_0_073 | `<wait 150> <wait 250> <wait 250> <k> <p> <dkey> <wait 350> </dkey> <k>` |
| kurihakase_first_talk / 19 | Professor (クリハカセ) | mac_0_073_01 → mac_0_073_01 | `<k> <p> <k> <p> <wait 250> <k> <p> <wait 250> <dkey> <wait 350> </dkey> <col c00000ff> </col> <k>` |
| kurihakase_first_talk / 20 | Goombella | mac_0_074 → mac_0_074_kur | `<col c00000ff> </col> <k> <p> <k>` |
| kurihakase_first_talk / 21 | Professor (クリハカセ) | mac_0_075 → mac_0_075 | `<wait 250> <k> <p> <k>` |
| kurihakase_first_talk / 22 | Goombella | mac_0_076 → mac_0_076_kur | `<col c00000ff> </col> <k> <p> <wait 250> <k> <p> <k> <p> <k>` |
| kurihakase_first_talk / 23 | Professor (クリハカセ) | mac_0_077 → mac_0_077 | `<dynamic 3> </dynamic> <wait 300> <k> <p> <wait 300> <k>` |
| kurihakase_first_talk / 24 | Goombella | mac_0_078 → mac_0_078_kur | `<wait 500> <wait 250> <wait 250> <k>` |
| kurihakase_first_talk / 25 | Professor (クリハカセ) | mac_0_079 → mac_0_079 | `<dynamic 3> <scale 2.5> <shake> </shake> </scale> </dynamic> <k> <p> <dynamic 3> <wait 250> </dynamic> <k> <p> <wait 250> <wait 250> <wait 250> <k>` |
| kurihakase_first_talk / 26 | Professor (クリハカセ) | mac_0_080 → mac_0_080 | `<dynamic 3> <wait 250> <wait 250> <wait 250> <k>` |
| kurihakase_first_talk / 27 | Professor (クリハカセ) | mac_0_081 → mac_0_081 | `<dynamic 3> <wait 250> <wait 250> <wait 250> </dynamic 3> <k> <p> <k> <p> <k>` |
| kurihakase_first_talk / 28 | Goombella | mac_0_081_01 → mac_0_081_01_kur | `<k>` |
| kurihakase_first_talk / 29 | Professor (クリハカセ) | mac_0_082 → mac_0_082 | `<wait 250> <wait 250> <k> <p> <dynamic 3> </dynamic> <wait 250> <k>` |
| kurihakase_wall_remove / 1 | Professor (クリハカセ) | mac_0_083 → mac_0_083 | `<wait 250> <k>` |
| kurihakase_wall_remove / 2 | Professor (クリハカセ) | mac_0_084 → mac_0_084 | `<k> <p> <wait 250> <wait 250> <k>` |

## Complete ordered static command requests

Hex operands with namespaces remain encoded. Plain numeric integers are shown
signed. Opcode107/109 begins/ends a concurrent brother; opcode11 waits on a
shared operand; opcode10 is milliseconds, opcode9 frames, opcode50 assignment,
opcodes53/55/56 arithmetic. EVT operands retain original ordering.

### kurihakase_first_talk

REL0x484ac,size0xc4c,SHA256`9e27be37f4d93218ea9204cf25d17eef07876c130d25f13c8b5470739f545511`.

| REL offset | Opcode / service | Operands in source order |
| --- | --- | --- |
| 0x484ac | evt_mapobj_flag_onoff | 1, 0, S_ie_02_naka, 2 |
| 0x484c4 | evt_npc_set_position | str_クリハカセ_gor_00018a3c, -115, 0, -225 |
| 0x484dc | evt_npc_set_ry | str_クリハカセ_gor_00018a3c, 90 |
| 0x484ec | evt_door_end_wait |  |
| 0x484f4 | evt_snd_bgmoff | 1024 |
| 0x48500 | evt_mario_key_onoff | 0 |
| 0x4850c | evt_party_stop | 0 |
| 0x48518 | 10 | 500 |
| 0x48520 | evt_eff_fukidashi | 3, zero_gor_00018790, 0, 0, 0, 0, 0, 0, 0, 0, 30 |
| 0x48554 | 10 | 500 |
| 0x4855c | evt_mario_dispflag_onoff | 1, 4 |
| 0x4856c | evt_mario_set_dir | 90, 200, 0 |
| 0x48580 | evt_party_move_pos2 | 0, -190, -100, 0xf24c5a80 |
| 0x48598 | evt_party_set_dir | 0, 90, 200 |
| 0x485ac | evt_msg_print_party | mac_0_066 |
| 0x485b8 | evt_cam3d_evt_set | -115, 65, -20, -115, 30, -217, 666, 11 |
| 0x485e0 | 10 | 666 |
| 0x485e8 | evt_snd_bgmoff | 1024 |
| 0x485f4 | evt_snd_bgmon | 1, BGM_EVT_FRANKURI1 |
| 0x48604 | evt_snd_envoff | 1024 |
| 0x48610 | evt_npc_set_ry | str_クリハカセ_gor_00018a3c, 270 |
| 0x48620 | 10 | 200 |
| 0x48628 | evt_msg_print | 0, mac_0_066_01, 0, str_クリハカセ_gor_00018a3c |
| 0x48640 | evt_cam3d_evt_set | -185, 95, 82, -185, 42, -217, 500, 11 |
| 0x48668 | 107 |  |
| 0x4866c | evt_party_move_pos2 | 0, -190, -215, 0xf24c5a80 |
| 0x48684 | evt_party_set_dir | 0, 90, -1 |
| 0x48698 | 109 |  |
| 0x4869c | 107 |  |
| 0x486a0 | evt_npc_move_position | str_クリハカセ_gor_00018a3c, -140, -225, 0, 0xf24c5a80, 0 |
| 0x486c0 | 109 |  |
| 0x486c4 | evt_mario_mov_pos2 | -225, -210, 0xf24c5a80 |
| 0x486d8 | evt_mario_set_dir | 90, 200, 0 |
| 0x486ec | evt_msg_print | 0, mac_0_067, 0, str_クリハカセ_gor_00018a3c |
| 0x48704 | evt_npc_set_ry | str_クリハカセ_gor_00018a3c, 90 |
| 0x48714 | 10 | 200 |
| 0x4871c | evt_msg_print | 0, mac_0_067_01, 0, str_クリハカセ_gor_00018a3c |
| 0x48734 | evt_npc_set_ry | str_クリハカセ_gor_00018a3c, 270 |
| 0x48744 | 10 | 200 |
| 0x4874c | evt_msg_print | 0, mac_0_067_02, 0, str_クリハカセ_gor_00018a3c |
| 0x48764 | evt_cam3d_evt_set | -170, 84, 29, -170, 40, -217, 200, 11 |
| 0x4878c | evt_npc_set_ry | str_クリハカセ_gor_00018a3c, 90 |
| 0x4879c | 10 | 200 |
| 0x487a4 | evt_msg_print | 0, mac_0_067_03, 0, str_クリハカセ_gor_00018a3c |
| 0x487bc | evt_cam3d_evt_set | -155, 72, -20, -155, 37, -217, 200, 11 |
| 0x487e4 | evt_npc_set_ry | str_クリハカセ_gor_00018a3c, 270 |
| 0x487f4 | 10 | 200 |
| 0x487fc | evt_msg_print | 0, mac_0_067_04, 0, str_クリハカセ_gor_00018a3c |
| 0x48814 | evt_cam3d_evt_set | -140, 59, -69, -140, 33, -217, 200, 11 |
| 0x4883c | evt_npc_set_ry | str_クリハカセ_gor_00018a3c, 90 |
| 0x4884c | 10 | 700 |
| 0x48854 | evt_eff_fukidashi | 1, zero_gor_00018790, str_クリハカセ_gor_00018a3c, 0, 0, 0, 0, 0, 0, 0, 30 |
| 0x48888 | 10 | 500 |
| 0x48890 | evt_msg_print | 0, mac_0_067_05, 0, str_クリハカセ_gor_00018a3c |
| 0x488a8 | evt_cam3d_evt_set | -185, 95, 82, -185, 42, -217, 500, 11 |
| 0x488d0 | evt_npc_set_ry | str_クリハカセ_gor_00018a3c, 270 |
| 0x488e0 | 10 | 500 |
| 0x488e8 | evt_msg_print | 0, mac_0_067_06, 0, str_クリハカセ_gor_00018a3c |
| 0x48900 | evt_msg_print_party | mac_0_068 |
| 0x4890c | 10 | 333 |
| 0x48914 | evt_msg_print | 0, mac_0_069, 0, str_クリハカセ_gor_00018a3c |
| 0x4892c | evt_mario_set_pose | M_I_G |
| 0x48938 | 10 | 500 |
| 0x48940 | 107 |  |
| 0x48944 | evt_mario_get_pos | 0, 0xfe363c80, 0xfe363c81, 0xfe363c82 |
| 0x4895c | evt_snd_sfxon_3d | SFX_VOICE_MARIO_PAUSE2, 0xfe363c80, 0xfe363c81, 0xfe363c82, 0 |
| 0x48978 | 109 |  |
| 0x4897c | evt_mario_set_pose | M_V_2 |
| 0x48988 | 10 | 500 |
| 0x48990 | 50 | 0xfaa2b580, 1 |
| 0x4899c | 107 |  |
| 0x489a0 | evt_msg_print | 0, mac_0_069_01, 0, str_クリハカセ_gor_00018a3c |
| 0x489b8 | 50 | 0xfaa2b580, 0 |
| 0x489c4 | 109 |  |
| 0x489c8 | 10 | 500 |
| 0x489d0 | 107 |  |
| 0x489d4 | evt_mario_get_pos | 0, 0xfe363c80, 0xfe363c81, 0xfe363c82 |
| 0x489ec | evt_snd_sfxon_3d | SFX_STG0_MARIO_FALL_CRUSH1, 0xfe363c80, 0xfe363c81, 0xfe363c82, 0 |
| 0x48a08 | 109 |  |
| 0x48a0c | 107 |  |
| 0x48a10 | evt_cam_shake | 4, 0xf24a7a80, 0xf24a7a8a, 500 |
| 0x48a28 | 109 |  |
| 0x48a2c | evt_mario_set_pose | M_D_8 |
| 0x48a38 | 10 | 1500 |
| 0x48a40 | 11 | 0xfaa2b580 |
| 0x48a48 | 107 |  |
| 0x48a4c | evt_mario_set_pose | M_U_3 |
| 0x48a58 | 10 | 500 |
| 0x48a60 | evt_mario_set_pose | M_S_1 |
| 0x48a6c | 109 |  |
| 0x48a70 | evt_msg_print | 0, mac_0_069_02, 0, party |
| 0x48a88 | 10 | 333 |
| 0x48a90 | evt_msg_print | 0, mac_0_069_03, 0, str_クリハカセ_gor_00018a3c |
| 0x48aa8 | evt_party_move_pos2 | 0, -185, -215, 0xf24b1a80 |
| 0x48ac0 | evt_cam3d_evt_set | -185, 64, -21, -185, 30, -218, 300, 11 |
| 0x48ae8 | evt_msg_print_party | mac_0_070 |
| 0x48af4 | evt_cam3d_evt_set | -140, 64, -21, -140, 30, -218, 500, 11 |
| 0x48b1c | 10 | 1000 |
| 0x48b24 | evt_msg_print | 0, mac_0_071, 0, str_クリハカセ_gor_00018a3c |
| 0x48b3c | evt_cam3d_evt_set | -250, 95, 122, -185, 45, -217, 0, 11 |
| 0x48b64 | 10 | 500 |
| 0x48b6c | evt_msg_print_party | mac_0_072 |
| 0x48b78 | evt_msg_print | 0, mac_0_073, 0, str_クリハカセ_gor_00018a3c |
| 0x48b90 | evt_cam3d_evt_set | -140, 64, -21, -140, 30, -218, 0, 11 |
| 0x48bb8 | 10 | 333 |
| 0x48bc0 | evt_msg_print | 0, mac_0_073_01, 0, str_クリハカセ_gor_00018a3c |
| 0x48bd8 | evt_cam3d_evt_set | -65, 95, 107, -185, 45, -217, 0, 11 |
| 0x48c00 | 10 | 500 |
| 0x48c08 | evt_msg_print_party | mac_0_074 |
| 0x48c14 | 10 | 333 |
| 0x48c1c | evt_msg_print | 0, mac_0_075, 0, str_クリハカセ_gor_00018a3c |
| 0x48c34 | evt_cam3d_evt_set | -185, 64, -21, -185, 30, -218, 0, 11 |
| 0x48c5c | 10 | 500 |
| 0x48c64 | evt_msg_print_party | mac_0_076 |
| 0x48c70 | evt_cam3d_evt_set | -140, 64, -21, -140, 30, -218, 500, 11 |
| 0x48c98 | 10 | 1000 |
| 0x48ca0 | evt_msg_print | 0, mac_0_077, 0, str_クリハカセ_gor_00018a3c |
| 0x48cb8 | evt_cam3d_evt_set | -185, 95, 82, -185, 42, -217, 0, 11 |
| 0x48ce0 | 10 | 500 |
| 0x48ce8 | evt_msg_print_party | mac_0_078 |
| 0x48cf4 | evt_npc_get_position | str_クリハカセ_gor_00018a3c, 0xfe363c80, 0xfe363c81, 0xfe363c82 |
| 0x48d0c | evt_npc_jump_position_nohit | str_クリハカセ_gor_00018a3c, 0xfe363c80, 0xfe363c81, 0xfe363c82, 225, 30 |
| 0x48d2c | evt_msg_print | 0, mac_0_079, 0, str_クリハカセ_gor_00018a3c |
| 0x48d44 | 107 |  |
| 0x48d48 | evt_mario_mov_pos2 | -170, -225, 0xf24bba80 |
| 0x48d5c | 109 |  |
| 0x48d60 | 107 |  |
| 0x48d64 | evt_party_dispflg_onoff | 0, 16, 1 |
| 0x48d78 | evt_party_move_pos2 | 0, -190, -205, 0xf24b6a80 |
| 0x48d90 | evt_party_set_dir | 0, 90, -1 |
| 0x48da4 | evt_party_dispflg_onoff | 0, 16, 0 |
| 0x48db8 | 109 |  |
| 0x48dbc | evt_cam3d_evt_set | -160, 70, -19, -160, 40, -217, 500, 11 |
| 0x48de4 | 10 | 1000 |
| 0x48dec | evt_mario_set_pose | M_C_3 |
| 0x48df8 | evt_mario_get_pos | 0, 0xfe363c80, 0xfe363c81, 0xfe363c82 |
| 0x48e10 | 53 | 0xfe363c80, 20 |
| 0x48e1c | 53 | 0xfe363c81, 20 |
| 0x48e28 | 53 | 0xfe363c82, 10 |
| 0x48e34 | evt_item_entry | item00, 96, 0xfe363c80, 0xfe363c81, 0xfe363c82, 16, -1, 0 |
| 0x48e5c | evt_item_flag_on | item00, 2 |
| 0x48e6c | evt_item_set_scale | item00, 0xf24a7cb3 |
| 0x48e7c | 10 | 660 |
| 0x48e84 | evt_mario_set_pose | M_S_1 |
| 0x48e90 | evt_item_set_position | item00, -140, 0, -220 |
| 0x48ea8 | 10 | 500 |
| 0x48eb0 | evt_msg_print | 0, mac_0_080, 0, str_クリハカセ_gor_00018a3c |
| 0x48ec8 | evt_mario_set_pose | M_C_3 |
| 0x48ed4 | evt_mario_get_pos | 0, 0xfe363c80, 0xfe363c81, 0xfe363c82 |
| 0x48eec | 53 | 0xfe363c80, 20 |
| 0x48ef8 | 53 | 0xfe363c81, 20 |
| 0x48f04 | 53 | 0xfe363c82, 10 |
| 0x48f10 | evt_item_set_position | item00, 0xfe363c80, 0xfe363c81, 0xfe363c82 |
| 0x48f28 | 10 | 660 |
| 0x48f30 | evt_item_set_position | item00, 0, -1000, 0 |
| 0x48f48 | evt_mario_set_pose | M_S_1 |
| 0x48f54 | evt_cam3d_evt_set | -155, 78, 30, -155, 40, -217, 500, 11 |
| 0x48f7c | 10 | 1000 |
| 0x48f84 | evt_msg_print | 0, mac_0_081, 0, str_クリハカセ_gor_00018a3c |
| 0x48f9c | evt_msg_print_party | mac_0_081_01 |
| 0x48fa8 | evt_npc_get_position | str_クリハカセ_gor_00018a3c, 0xfe363c80, 0xfe363c81, 0xfe363c82 |
| 0x48fc0 | evt_npc_jump_position_nohit | str_クリハカセ_gor_00018a3c, 0xfe363c80, 0xfe363c81, 0xfe363c82, 225, 30 |
| 0x48fe0 | evt_npc_flag_onoff | 1, str_クリハカセ_gor_00018a3c, 1073741856 |
| 0x48ff4 | evt_npc_flag_onoff | 0, str_クリハカセ_gor_00018a3c, 1073741856 |
| 0x49008 | evt_msg_print | 0, mac_0_082, 0, str_クリハカセ_gor_00018a3c |
| 0x49020 | evt_npc_get_position | str_クリハカセ_gor_00018a3c, 0xfe363c80, 0xfe363c81, 0xfe363c82 |
| 0x49038 | evt_npc_set_position | str_クリハカセ_gor_00018a3c, 0, -1000, 0 |
| 0x49050 | evt_party_stop | 1 |
| 0x4905c | evt_mario_set_party_pos | 1, 11, 0xfe363c80, 0xfe363c81, 0xfe363c82 |
| 0x49078 | evt_snd_bgmoff | 513 |
| 0x49084 | evt_snd_bgmon | 288, 0 |
| 0x49094 | evt_snd_envoff | 513 |
| 0x490a0 | evt_snd_envon | 288, 0 |
| 0x490b0 | evt_cam3d_evt_off | 500, 11 |
| 0x490c0 | evt_party_run | 1 |
| 0x490cc | evt_party_run | 0 |
| 0x490d8 | 50 | 0xf5de0180, 9 |
| 0x490e4 | evt_mario_key_onoff | 1 |
| 0x490f0 | 2 |  |
| 0x490f4 | 1 |  |

### kurihakase_wall_remove

REL0x490f8,size0x404,SHA256`404b1eb72ecc96d2ca959766a1f5375ce09f81e7bffeeb2fe135f223aa49550c`.

| REL offset | Opcode / service | Operands in source order |
| --- | --- | --- |
| 0x490f8 | evt_door_end_wait |  |
| 0x49100 | evt_mario_key_onoff | 0 |
| 0x4910c | 10 | 500 |
| 0x49114 | evt_mario_set_dir | 270, 200, 0 |
| 0x49128 | evt_cam3d_evt_set | -270, 100, 595, -270, 30, 201, 2000, 11 |
| 0x49150 | evt_party_get_pos | 1, 0xfe363c80, 0xfe363c81, 0xfe363c82 |
| 0x49168 | evt_mario_clear_party | 1 |
| 0x49174 | evt_npc_set_position | str_クリハカセ_gor_00018a3c, 0xfe363c80, 0xfe363c81, 0xfe363c82 |
| 0x4918c | evt_npc_set_ry | str_クリハカセ_gor_00018a3c, 270 |
| 0x4919c | evt_npc_flag_onoff | 1, str_クリハカセ_gor_00018a3c, 1073741856 |
| 0x491b0 | evt_npc_move_position | str_クリハカセ_gor_00018a3c, -265, 187, 0, 0xf24d4a80, 0 |
| 0x491d0 | evt_msg_print | 0, mac_0_083, 0, str_クリハカセ_gor_00018a3c |
| 0x491e8 | 10 | 500 |
| 0x491f0 | evt_paper_entry | OFF_d_saku |
| 0x491fc | evt_offscreen_entry | ofs |
| 0x49208 | evt_img_entry | img |
| 0x49214 | evt_img_set_paper | img, OFF_d_saku |
| 0x49224 | evt_mapobj_set_offscreen | 1, S_saku04, ofs |
| 0x49238 | set_depthPlaneOffset | img, 0xf24a7280 |
| 0x49248 | evt_offscreen_get_boundingbox | ofs, 0xfe363c80, 0xfe363c81, 0xfe363c82, 0xfe363c83 |
| 0x49264 | 27 | 0xfe363c82, 0xfe363c83 |
| 0x49270 | 50 | 0xfe363c83, 0xfe363c82 |
| 0x4927c | 55 | 0xfe363c83, 48 |
| 0x49288 | 56 | 0xfe363c83, 64 |
| 0x49294 | 32 |  |
| 0x49298 | 50 | 0xfe363c83, 0xfe363c82 |
| 0x492a4 | 55 | 0xfe363c83, 64 |
| 0x492b0 | 56 | 0xfe363c83, 48 |
| 0x492bc | 33 |  |
| 0x492c0 | evt_img_alloc_capture | img, 0, ofs, 1, 0xfe363c80, 0xfe363c81, 0xfe363c82, 0xfe363c83 |
| 0x492e8 | evt_img_clear_virtual_point | img |
| 0x492f4 | evt_img_set_paper_anim | img, Z_1 |
| 0x49304 | evt_img_onoff | img, 1 |
| 0x49314 | evt_img_set_shadow | img, 2 |
| 0x49324 | evt_mapobj_flag_onoff | 1, 1, S_saku04, 1 |
| 0x4933c | 107 |  |
| 0x49340 | evt_snd_sfxon | SFX_OFF_FENCE_OPEN1, 0 |
| 0x49350 | 10 | 1000 |
| 0x49358 | evt_snd_sfxon | SFX_OFF_FENCE_FALL1, 0 |
| 0x49368 | 109 |  |
| 0x4936c | evt_img_set_paper_anim | img, A_1 |
| 0x4937c | evt_img_wait_animend | img |
| 0x49388 | evt_img_onoff | img, 0 |
| 0x49398 | evt_mapobj_clear_offscreen | 1, S_saku04 |
| 0x493a8 | evt_img_release | img |
| 0x493b4 | evt_offscreen_delete | ofs |
| 0x493c0 | evt_hitobj_onoff | A_saku_04, 1, 0 |
| 0x493d4 | evt_cam3d_evt_set | -210, 85, 595, -210, 15, 201, 1000, 11 |
| 0x493fc | evt_npc_set_ry | str_クリハカセ_gor_00018a3c, 90 |
| 0x4940c | evt_npc_move_position | str_クリハカセ_gor_00018a3c, -220, 305, 0, 0xf24d4a80, 0 |
| 0x4942c | evt_npc_get_position | str_クリハカセ_gor_00018a3c, 0xfe363c80, 0xfe363c81, 0xfe363c82 |
| 0x49444 | evt_npc_jump_position_nohit | str_クリハカセ_gor_00018a3c, 0xfe363c80, 0xfe363c81, 0xfe363c82, 225, 30 |
| 0x49464 | evt_npc_jump_position_nohit | str_クリハカセ_gor_00018a3c, 0xfe363c80, 0xfe363c81, 0xfe363c82, 225, 30 |
| 0x49484 | evt_msg_print | 0, mac_0_084, 0, str_クリハカセ_gor_00018a3c |
| 0x4949c | evt_npc_flag_onoff | 0, str_クリハカセ_gor_00018a3c, 1073741856 |
| 0x494b0 | 10 | 500 |
| 0x494b8 | evt_cam3d_evt_off | 500, 11 |
| 0x494c8 | evt_party_run | 0 |
| 0x494d4 | evt_mario_key_onoff | 1 |
| 0x494e0 | 50 | 0xf5de0180, 10 |
| 0x494ec | 92 | kurihakase_lecture |
| 0x494f4 | 2 |  |
| 0x494f8 | 1 |  |

## Validation and limitations

Native prerequisite delivery2026-10-13: the original bank's six imported clips
Z_1,S_1,T_1,W_1,R_1,D_1 are compiled by
[professor_assets.py](../../../tools/professor_assets.py), preserving original
animated geometry/visibility. Note actual bank D_1 versus tribe literal d_1;
selector semantics remain open and no case alias is manufactured. Native story8
interior rendering uses source init(-135,0,-240), S_1, with an explicit provisional
billboard yaw/clock/scalar adapter. First-talk's position override/concurrent
scene services are not implemented by that idle rendering. No story write or
dialogue replacement occurs. [Playback](../../../source/message_playback.cpp)
now emits/consumes source dkey records; original-data tests parse/play all29
first-talk payloads with original font and bytes. This is prerequisite validation,
not execution of the complete conversation/fence event.

Reproduced with tools/rel_event_inventory.py --module gor --symbols
artifacts/reference/us_symbols.csv for kurihakase_first_talk,
kurihakase_wall_remove and kurihakase_init. Original rel/gor.rel compared
byte-for-byte with read-only ISO extraction. DOL actor/model/pose pointers read
directly from source ISO; exact US message keys/tokens inspected directly from
source ISO, not a replacement script. External symbols annotate actual event
words and self/module0 relocations. No native source changes, execution trace,
complete camera/effect/audio correctness or full prologue completion claimed.
