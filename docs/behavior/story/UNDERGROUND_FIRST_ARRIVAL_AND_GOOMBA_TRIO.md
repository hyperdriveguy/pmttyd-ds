# Underground first arrival and Goomba trio

Read-only US G8ME01 disc0 revision0 static evidence, scoped to
[OW-ROAD-01](../../requests/OW-ROAD-01.md). Original coordinates, script waits
and resource names are source requests; no Dolphin observation or native parity
claim. ISO `rel/tik.rel` is352728bytes, SHA256
`ca30b06f0b7176dbe364349f239ef3ddfe66650e50ea1246921bd2a368a9dac7`.
The [pipe contract](../overworld/ROGUEPORT_UNDERGROUND_PIPE_ROUTE.md) owns
admission, travel geometry and restored partner ownership.

## Initialization and actor ownership

`tik_01_init_evt` REL0x14a98/0x340 sets flag0xf8405c7e=1, music
BGM_STG0_TIK1 (first-visit fade3000ms controlled by0xf84060d9), environment
ENV_STG0_TIK1, reverb2, BERO table0x13780 and generic BERO info/arrival events.
It also owns bomb-wall flags, save block(-455,135,-60), moving floor and normal
map services; listing these does not establish their full native binding.
At0x14d84..14d90, story<19 deletes NPC `トゲクリボー`. At0x14da0..14dac,
story<=11 RUN_EVENTs `gorotuki_sanninngumi`; else disables hit `a_rec` with
arguments(1,0). No invented room-dialogue trigger is needed.

Maptag `m/tik_01/s` has three named trio NPCs, each default hidden at
(0,-1000,0), yaw270, AI `なし`, territoryNOTHING and BTLNO_INVALID.
Scene placement overrides these positions before its crossing poll:

| Actor raw key | Tribe | Scene XYZ | Model | Stay/idle/talk/walk/run/damage poses |
| --- | --- | --- | --- | --- | --- |
| クリボー１ | クリボーゴロ | (110,75,65) | c_kuribo | KUR_S_1/KUR_Z_2/KUR_T_1/KUR_W_1/KUR_R_1/KUR_D_1 |
| クリボー２ | パタクリゴロ | (140,95,35) | c_kuribo | PTK_S_1/PTK_Z_2/PTK_T_1/PTK_W_1/PTK_R_1/PTK_D_1 |
| クリボー３ | トゲクリゴロ | (155,75,85) | c_kuribo | TGK_S_1/TGK_Z_2/TGK_T_1/TGK_W_1/TGK_R_1/TGK_D_1 |

DOL tribe rows are respectively0x80333260,0x80333308,0x803332b4;
radius/height20/20. Source resources `a/c_kuribo` and `a/c_kuribo-` own the
model/texture bank. Ordinary fourth actor `トゲクリボー`, tribe `トゲクリ`,
starts(475,-50,50), yaw270, spiked-Goomba AI, square territorybase(475,-50,50),
loiter50/50/80, homing50/100/80, searchrange200/angle300,
homingrange1000/angle360, battleBTLNO_TIK_01_02. Delete it in this prologue
instead of rendering every maptag NPC indiscriminately.

Arrival callback `dokan_2_in_evt` REL0x13728/0x58 checks story>=20 and<21
(exact20), reads equipped badge count and RUN_EVENTs
`lecture_badge_after_00_evt` only when count0. It sets LW0=0. At prologue
story11 it requests no special scene and creates no Frankly actor. Generic
arrival owns Mario and both restored partner slots; Frankly comes from the
source gor_02 pipe-out callback and previous-party restoration.

## Crossing, choreography and battle boundary

Trio event first places actors and sets flag0x40000220 on each. It polls Mario
XYZ every simulation frame until X>=-100; then shows groupS via flag2 request,
locks keys and prints086. It stops party0 and1, aims Mario/Goombella at NPC1,
runs finding-voice brother and Mario/partner reaction bubbles, waits800ms,
sets camera(125,148,360,125,110,37,500ms,11), waits400ms. Positions become
Mario(-100,0,70), party0(-135,0,70), party1(-170,0,70). Mario and party0 turn90.
Messages087/088 precede camera(-140,65,310,-140,33,37,0,11),200ms,089party;
then camera restores125 viewpoint,200ms,090/091/092.

Camera releases500ms. Three parallel actor movement blocks use source locals
LW10/11/12 eventIDs. Goomba1 run→X50 speed150→jump(-30,0,Z,350,45)→X-90
speed150; Paragoomba X50 speed200→jump(-30,20,Z,350,45)→X-80 speed150;
SpikedGoomba X50 speed250→jump(-30,0,Z,350,45)→X-80 speed150. No-hit jump
arguments are source duration/shape requests, not generic canned bounce frames.
Parent waits350ms, surprise voice brother, Mario M_N_7,530ms, deletes GW0
footstep event, assigns NPC1 battleinfo4, starts battle and waits battleend.

**Setup4 is an actual Goomba-trio encounter.** It is neither lecture setup3 nor
Crump. `evt_npc_battle_start` must retain control until the correct battle backend
returns. Do not run the postbattle branch or story12 by treating it as a no-op.
Native EventNativeRuntime can host the source opcode graph, but admission of its
used callbacks and a correct battle provider remain separate requirements.

After battle it deletes the three movement eventIDs, places the trio
(-65,0,65),(-55,20,35),(-55,0,85), restores MarioM_S_1 and trio damage poses,
sets camera(-120,150,588,-120,55,37,0,11),500ms, clears NPC1flag0x200,093.
Escape uses three brothers, two shared completion flags, exact jumps/moves and
source hidden destinations(0,-1000,0). Parent continually aims Mario/party0 at
NPC3 until first shared flag clears, then waits second;200ms, Goombella camera,
200ms,094party,333ms,094_01explicitextparty. Finally cameraoff500ms/type11,
party_run0/1, story12, hitoffa_rec, keys on and BGM_STG0_TIK1 fade2000ms.

`ashioto` REL0x13a50/0x8c is10commands, SHA256
`b924726a657fdea9159bda5549afcba710fc94dd477c02504d28a602e9a8bc4f`.
Its infinite loop gets NPC1 XYZ, SFX_STG0_KURI_MOVE1L,50ms, gets NPC1 XYZ,
SFX_STG0_KURI_MOVE1R,50ms. Even when other actors move, the source footstep
owner remains NPC1. Preserve concurrent eventIDs/deletion and shared waits.

## Exact message resources and speaker bindings

Preserve original bytes and controls. Base party requests089/094 resolve to
_kur with Goombella; explicitextparty is the restored Frankly slot1 actor and
uses unsuffixed094_01. In particular094_01 lives in **tik_04.txt**, nottik_01.
No choices occur in this scene. Token census is lexical, not proof of native
visual dynamic/shake/icon implementation.

| Ordered key | Request/speaker | Resource | Bytes | SHA256 | Unique raw control tokens |
| --- | --- | --- | --- | --- | --- |
| mac_0_086 | print/クリボー１ | msg/US/tik_01.txt | 102 | `4a436be1899c379ca747fa6efac22f82d9dbce36b3433f3ad9f6da7fa442e325` | `<k>`, `<wait 100>`, `<wait 250>` |
| mac_0_087 | print/クリボー２ | msg/US/tik_01.txt | 97 | `d80baa3b60b87a7c1b490e900b244a813e9f1a2dc27f2a967dc5015cfbf69ef1` | `<k>`, `<wait 250>` |
| mac_0_088 | print/クリボー３ | msg/US/tik_01.txt | 92 | `ad74551cb647a6ac6b9a7a2e4cc3406a47c394ea480ff39035dec2c277700570` | `<k>`, `<wait 250>` |
| mac_0_089_kur | print_party/Goombella | msg/US/tik_01.txt | 222 | `df8553ef4ae985879db2c0d4f2b5ed56745b20c295059529fe1783bff0fb9096` | `</dynamic>`, `<dynamic 3>`, `<k>`, `<p>`, `<wait 250>` |
| mac_0_090 | print/クリボー１ | msg/US/tik_01.txt | 34 | `e0b22b2e4f3275945e72e28bd5b2a98a3f7888824ea1848a9d2281f13460fedd` | `<k>`, `<wait 250>` |
| mac_0_091 | print/クリボー２ | msg/US/tik_01.txt | 65 | `24f866bdebed1f8e195a13dfc1dd2bf317a168ba1ea09b68120a79a4b224a8a5` | `<k>`, `<wait 250>` |
| mac_0_092 | print/クリボー３ | msg/US/tik_01.txt | 74 | `8b20341c7ea190795dcae86653b6a97e3e497e010ab671d721d18196499786fe` | `<k>`, `<wait 250>` |
| mac_0_093 | print/クリボー１ | msg/US/tik_01.txt | 40 | `a649a71c0d4f534ca2d4763e35012461e6497de730761db08a6885cad58daa9c` | `<dynamic 3>`, `<k>`, `<shake>` |
| mac_0_094_kur | print_party/Goombella | msg/US/tik_01.txt | 65 | `2a028dce789a9d079e331b93472eed8301622426de64967718bd7580ee271dc4` | `<k>`, `<wait 250>` |
| mac_0_094_01 | print/extparty (Frankly) | msg/US/tik_04.txt | 244 | `d2305d3d6bc9844c90c0f30c4d48a411cb8dcdbb328ab7ad715e17b1a6a56957` | `<icon STICK 0.6 -30 45 -10>`, `<k>`, `<p>`, `<wait 250>` |

## Full source command inventories

Addresses below are REL file offsets. Service names are resolved ADDR32
imports; local annotated strings retain their exact raw actor key. Numeric
operands remain hexadecimal so event-variable tokens cannot be mistaken for
literal coordinates. Source float decoding is(signedWord+230000000)/1024;
used150/200/250/170/300 are speed requests. Event-control opcodes5/6=loop,
9=WAIT_FRAME,10=WAIT_MSEC,11=WAIT_WHILE,24=IF_EQ,29=IF_GE,33=END_IF,
50=SET,54=SUB,91=USER_FUNC,93=RUN_EVENT_ID,95=DELETE_EVENT,
107/109=BROTHER_BEGIN/END,108/109=INLINE_BEGIN_ID/END.

### dokan_2_in_evt

REL0x13728, 88bytes, 11commands, SHA256`1b3a1ae63c1cd31d69d5e6497e9a25e211bf86a89592530e6fa101ee7767528a`.

| Offset | Opcode | Ordered operands |
| --- | --- | --- |
| 0x13728 | 29 | `0xf5de0180` · `0x00000014` |
| 0x13734 | 26 | `0xf5de0180` · `0x00000015` |
| 0x13740 | 91 | `evt_pouch_get_equipbadgecnt` · `0xfe363c80` |
| 0x1374c | 24 | `0xfe363c80` · `0x00000000` |
| 0x13758 | 92 | `lecture_badge_after_00_evt` |
| 0x13760 | 33 |  |
| 0x13764 | 33 |  |
| 0x13768 | 33 |  |
| 0x1376c | 50 | `0xfe363c80` · `0x00000000` |
| 0x13778 | 2 |  |
| 0x1377c | 1 |  |
### tik_01_init_evt

REL0x14a98, 832bytes, 62commands, SHA256`c4dc5fef94bbcac3f0d715e37ff6c6238ba81f704f700e83c68052b5a5c8a8b2`.

| Offset | Opcode | Ordered operands |
| --- | --- | --- |
| 0x14a98 | 50 | `0xf8405c7e` · `0x00000001` |
| 0x14aa4 | 24 | `0xf84060d9` · `0x00000001` |
| 0x14ab0 | 91 | `evt_snd_bgmon` · `0x00000200` · `BGM_STG0_TIK1` |
| 0x14ac0 | 32 |  |
| 0x14ac4 | 91 | `evt_snd_bgmon_f` · `0x00000300` · `BGM_STG0_TIK1` · `0x00000bb8` |
| 0x14ad8 | 50 | `0xf84060d9` · `0x00000001` |
| 0x14ae4 | 33 |  |
| 0x14ae8 | 91 | `evt_snd_envon` · `0x00000110` · `ENV_STG0_TIK1` |
| 0x14af8 | 91 | `evt_snd_set_rev_mode` · `0x00000002` |
| 0x14b04 | 50 | `0xfe363c80` · `bero_entry_data` |
| 0x14b10 | 91 | `evt_bero_get_info` |
| 0x14b18 | 94 | `evt_bero_info_run` |
| 0x14b20 | 24 | `0xf8406027` · `0x00000000` |
| 0x14b2c | 24 | `0xf8406026` · `0x00000001` |
| 0x14b38 | 29 | `0xf5de0180` · `0x00000038` |
| 0x14b44 | 50 | `0xf8406027` · `0x00000001` |
| 0x14b50 | 33 |  |
| 0x14b54 | 33 |  |
| 0x14b58 | 33 |  |
| 0x14b5c | 91 | `evt_paper_entry` · `OFF_d_bom_ver2` |
| 0x14b68 | 24 | `0xf84060b3` · `0x00000000` |
| 0x14b74 | 91 | `evt_mapobj_flag_onoff` · `0x00000001` · `0x00000001` · `bom_kabe_2` · `0x00000001` |
| 0x14b8c | 91 | `evt_hitobj_onoff` · `a_dom_kabe_2` · `0x00000001` · `0x00000000` |
| 0x14ba0 | 91 | `evt_run_case_evt` · `0x0000000f` · `0x00000001` · `a_dom_kabe_1` · `0x00000000` · `wall_bomb00` · `0x00000000` |
| 0x14bc0 | 32 |  |
| 0x14bc4 | 91 | `evt_mapobj_flag_onoff` · `0x00000001` · `0x00000001` · `bom_kabe_1` · `0x00000001` |
| 0x14bdc | 91 | `evt_hitobj_onoff` · `a_dom_kabe_1` · `0x00000001` · `0x00000000` |
| 0x14bf0 | 33 |  |
| 0x14bf4 | 24 | `0xf84060b4` · `0x00000000` |
| 0x14c00 | 91 | `evt_mapobj_flag_onoff` · `0x00000001` · `0x00000001` · `bom_kabe_4` · `0x00000001` |
| 0x14c18 | 50 | `0xfe363c80` · `e_bero_2` |
| 0x14c24 | 94 | `bero_case_switch_off` |
| 0x14c2c | 91 | `evt_hitobj_attr_onoff` · `0x00000001` · `0x00000000` · `e_bero_2` · `0x00000004` |
| 0x14c44 | 91 | `evt_run_case_evt` · `0x0000000f` · `0x00000001` · `e_bero_2` · `0x00000000` · `wall_bomb01` · `0x00000000` |
| 0x14c64 | 32 |  |
| 0x14c68 | 91 | `evt_mapobj_flag_onoff` · `0x00000001` · `0x00000001` · `bom_kabe_3` · `0x00000001` |
| 0x14c80 | 33 |  |
| 0x14c84 | 91 | `evt_mobj_itembox` · `mobj_box00` · `0x0000021c` · `0x0000007d` · `0xffffff9c` · `0x00000000` · `0x00000000` · `mobj_box00` · `0xf84060cb` |
| 0x14cac | 91 | `evt_mobj_kururing_floor` · `kururin1` · `0xffffff9c` · `0x00000000` · `0x00000032` · `jimen` · `0x0000007d` · `0xf8407155` |
| 0x14cd0 | 91 | `evt_mobj_save_blk` · `mobj_save` · `0xfffffe39` · `0x00000087` · `0xffffffc4` · `0x00000000` · `0x00000000` |
| 0x14cf0 | 50 | `0xfe363c80` · `ugo_yuka_1` |
| 0x14cfc | 50 | `0xfe363c81` · `a_ugo_yuka_1` |
| 0x14d08 | 50 | `0xfe363c82` · `0xfffffff1` |
| 0x14d14 | 50 | `0xfe363c83` · `0x00000067` |
| 0x14d20 | 50 | `0xfe363c84` · `0x00000a8c` |
| 0x14d2c | 50 | `0xfe363c85` · `0x000001f4` |
| 0x14d38 | 50 | `0xfe363c86` · `0x00000000` |
| 0x14d44 | 50 | `0xfe363c87` · `0x00000000` |
| 0x14d50 | 50 | `0xfe363c8b` · `0x00000000` |
| 0x14d5c | 50 | `0xfe363c8c` · `0x00000000` |
| 0x14d68 | 92 | `moving_floor` |
| 0x14d70 | 91 | `evt_map_entry_airport_harbor` · `0x00000000` · `hikou` · `a_hi1` |
| 0x14d84 | 26 | `0xf5de0180` · `0x00000013` |
| 0x14d90 | 91 | `evt_npc_check_delete` · `str_トゲクリボー_tik_00009240` |
| 0x14d9c | 33 |  |
| 0x14da0 | 28 | `0xf5de0180` · `0x0000000b` |
| 0x14dac | 92 | `gorotuki_sanninngumi` |
| 0x14db4 | 32 |  |
| 0x14db8 | 91 | `evt_hitobj_onoff` · `a_rec` · `0x00000001` · `0x00000000` |
| 0x14dcc | 33 |  |
| 0x14dd0 | 2 |  |
| 0x14dd4 | 1 |  |
### gorotuki_sanninngumi

REL0x13adc, 2776bytes, 161commands, SHA256`b5b5b67b5f9b0849affe629ce634fc6930efbd3c1c76a2c9a2770d1a3ff64a47`.

| Offset | Opcode | Ordered operands |
| --- | --- | --- |
| 0x13adc | 91 | `evt_npc_set_position` · `str_クリボー１_tik_00008fd4` · `0x0000006e` · `0x0000004b` · `0x00000041` |
| 0x13af4 | 91 | `evt_npc_set_position` · `str_クリボー２_tik_00009010` · `0x0000008c` · `0x0000005f` · `0x00000023` |
| 0x13b0c | 91 | `evt_npc_set_position` · `str_クリボー３_tik_0000901c` · `0x0000009b` · `0x0000004b` · `0x00000055` |
| 0x13b24 | 91 | `evt_npc_flag_onoff` · `0x00000001` · `str_クリボー１_tik_00008fd4` · `0x40000220` |
| 0x13b38 | 91 | `evt_npc_flag_onoff` · `0x00000001` · `str_クリボー２_tik_00009010` · `0x40000220` |
| 0x13b4c | 91 | `evt_npc_flag_onoff` · `0x00000001` · `str_クリボー３_tik_0000901c` · `0x40000220` |
| 0x13b60 | 5 | `0x00000000` |
| 0x13b68 | 91 | `evt_mario_get_pos` · `0x00000000` · `0xfe363c80` · `0xfe363c81` · `0xfe363c82` |
| 0x13b80 | 29 | `0xfe363c80` · `0xffffff9c` |
| 0x13b8c | 7 |  |
| 0x13b90 | 33 |  |
| 0x13b94 | 9 | `0x00000001` |
| 0x13b9c | 6 |  |
| 0x13ba0 | 91 | `evt_mapobj_flag_onoff` · `0x00000001` · `0x00000000` · `S` · `0x00000002` |
| 0x13bb8 | 91 | `evt_mario_key_onoff` · `0x00000000` |
| 0x13bc4 | 91 | `evt_msg_print` · `0x00000000` · `mac_0_086` · `0x00000000` · `str_クリボー１_tik_00008fd4` |
| 0x13bdc | 91 | `evt_party_stop` · `0x00000000` |
| 0x13be8 | 91 | `evt_party_stop` · `0x00000001` |
| 0x13bf4 | 91 | `evt_mario_set_dir_npc` · `str_クリボー１_tik_00008fd4` |
| 0x13c00 | 91 | `evt_party_set_dir_npc` · `0x00000000` · `str_クリボー１_tik_00008fd4` |
| 0x13c10 | 107 |  |
| 0x13c14 | 91 | `evt_mario_get_pos` · `0x00000000` · `0xfe363c80` · `0xfe363c81` · `0xfe363c82` |
| 0x13c2c | 91 | `evt_snd_sfxon_3d` · `SFX_VOICE_MARIO_FIND1` · `0xfe363c80` · `0xfe363c81` · `0xfe363c82` · `0x00000000` |
| 0x13c48 | 109 |  |
| 0x13c4c | 91 | `evt_eff_fukidashi` · `0x00000000` · `zero_tik_00009050` · `0x00000000` · `0x00000000` · `0x00000000` · `0x00000000` · `0x00000000` · `0x00000000` · `0x00000000` · `0x00000000` · `0x0000001e` |
| 0x13c80 | 91 | `evt_eff_fukidashi` · `0x00000003` · `zero_tik_00009050` · `0x00000000` · `0x00000000` · `0x00000000` · `0x00000000` · `0x00000000` · `0x00000000` · `0x00000000` · `0x00000000` · `0x0000001e` |
| 0x13cb4 | 10 | `0x00000320` |
| 0x13cbc | 91 | `evt_cam3d_evt_set` · `0x0000007d` · `0x00000094` · `0x00000168` · `0x0000007d` · `0x0000006e` · `0x00000025` · `0x000001f4` · `0x0000000b` |
| 0x13ce4 | 10 | `0x00000190` |
| 0x13cec | 50 | `0xfe363c80` · `0xffffff9c` |
| 0x13cf8 | 50 | `0xfe363c82` · `0x00000046` |
| 0x13d04 | 91 | `evt_mario_set_pos` · `0xfe363c80` · `0x00000000` · `0xfe363c82` |
| 0x13d18 | 54 | `0xfe363c80` · `0x00000023` |
| 0x13d24 | 91 | `evt_party_set_pos` · `0x00000000` · `0xfe363c80` · `0x00000000` · `0xfe363c82` |
| 0x13d3c | 54 | `0xfe363c80` · `0x00000023` |
| 0x13d48 | 91 | `evt_party_set_pos` · `0x00000001` · `0xfe363c80` · `0x00000000` · `0xfe363c82` |
| 0x13d60 | 91 | `evt_mario_set_dir` · `0x0000005a` · `0x00000000` · `0x00000000` |
| 0x13d74 | 91 | `evt_party_set_dir` · `0x00000000` · `0x0000005a` · `0xffffffff` |
| 0x13d88 | 91 | `evt_msg_print` · `0x00000000` · `mac_0_087` · `0x00000000` · `str_クリボー２_tik_00009010` |
| 0x13da0 | 91 | `evt_msg_print` · `0x00000000` · `mac_0_088` · `0x00000000` · `str_クリボー３_tik_0000901c` |
| 0x13db8 | 91 | `evt_cam3d_evt_set` · `0xffffff74` · `0x00000041` · `0x00000136` · `0xffffff74` · `0x00000021` · `0x00000025` · `0x00000000` · `0x0000000b` |
| 0x13de0 | 10 | `0x000000c8` |
| 0x13de8 | 91 | `evt_msg_print_party` · `mac_0_089` |
| 0x13df4 | 91 | `evt_cam3d_evt_set` · `0x0000007d` · `0x00000094` · `0x00000168` · `0x0000007d` · `0x0000006e` · `0x00000025` · `0x00000000` · `0x0000000b` |
| 0x13e1c | 10 | `0x000000c8` |
| 0x13e24 | 91 | `evt_msg_print` · `0x00000000` · `mac_0_090` · `0x00000000` · `str_クリボー１_tik_00008fd4` |
| 0x13e3c | 91 | `evt_msg_print` · `0x00000000` · `mac_0_091` · `0x00000000` · `str_クリボー２_tik_00009010` |
| 0x13e54 | 91 | `evt_msg_print` · `0x00000000` · `mac_0_092` · `0x00000000` · `str_クリボー３_tik_0000901c` |
| 0x13e6c | 91 | `evt_cam3d_evt_off` · `0x000001f4` · `0x0000000b` |
| 0x13e7c | 108 | `0xfe363c8a` |
| 0x13e84 | 93 | `ashioto` · `0xfe363c8a` |
| 0x13e90 | 91 | `evt_npc_set_run_anim` · `str_クリボー１_tik_00008fd4` |
| 0x13e9c | 91 | `evt_npc_get_position` · `str_クリボー１_tik_00008fd4` · `0xfe363c80` · `0xfe363c81` · `0xfe363c82` |
| 0x13eb4 | 91 | `evt_npc_move_position` · `str_クリボー１_tik_00008fd4` · `0x00000032` · `0xfe363c82` · `0x00000000` · `0xf24cd280` · `0x00000000` |
| 0x13ed4 | 95 | `0xfe363c8a` |
| 0x13edc | 91 | `evt_npc_get_position` · `str_クリボー１_tik_00008fd4` · `0xfe363c80` · `0xfe363c81` · `0xfe363c82` |
| 0x13ef4 | 91 | `evt_snd_sfxon_3d` · `SFX_STG0_KURI_JUMP1` · `0xfe363c80` · `0xfe363c81` · `0xfe363c82` · `0x00000000` |
| 0x13f10 | 91 | `evt_npc_jump_position_nohit` · `str_クリボー１_tik_00008fd4` · `0xffffffe2` · `0x00000000` · `0xfe363c82` · `0x0000015e` · `0x0000002d` |
| 0x13f30 | 93 | `ashioto` · `0xfd050f80` |
| 0x13f3c | 91 | `evt_npc_move_position` · `str_クリボー１_tik_00008fd4` · `0xffffffa6` · `0xfe363c82` · `0x00000000` · `0xf24cd280` · `0x00000000` |
| 0x13f5c | 109 |  |
| 0x13f60 | 108 | `0xfe363c8b` |
| 0x13f68 | 91 | `evt_npc_set_run_anim` · `str_クリボー２_tik_00009010` |
| 0x13f74 | 91 | `evt_npc_get_position` · `str_クリボー２_tik_00009010` · `0xfe363c80` · `0xfe363c81` · `0xfe363c82` |
| 0x13f8c | 91 | `evt_npc_move_position` · `str_クリボー２_tik_00009010` · `0x00000032` · `0xfe363c82` · `0x00000000` · `0xf24d9a80` · `0x00000000` |
| 0x13fac | 91 | `evt_npc_jump_position_nohit` · `str_クリボー２_tik_00009010` · `0xffffffe2` · `0x00000014` · `0xfe363c82` · `0x0000015e` · `0x0000002d` |
| 0x13fcc | 91 | `evt_npc_move_position` · `str_クリボー２_tik_00009010` · `0xffffffb0` · `0xfe363c82` · `0x00000000` · `0xf24cd280` · `0x00000000` |
| 0x13fec | 109 |  |
| 0x13ff0 | 108 | `0xfe363c8c` |
| 0x13ff8 | 91 | `evt_npc_set_run_anim` · `str_クリボー３_tik_0000901c` |
| 0x14004 | 91 | `evt_npc_get_position` · `str_クリボー３_tik_0000901c` · `0xfe363c80` · `0xfe363c81` · `0xfe363c82` |
| 0x1401c | 91 | `evt_npc_move_position` · `str_クリボー３_tik_0000901c` · `0x00000032` · `0xfe363c82` · `0x00000000` · `0xf24e6280` · `0x00000000` |
| 0x1403c | 91 | `evt_npc_jump_position_nohit` · `str_クリボー３_tik_0000901c` · `0xffffffe2` · `0x00000000` · `0xfe363c82` · `0x0000015e` · `0x0000002d` |
| 0x1405c | 91 | `evt_npc_move_position` · `str_クリボー３_tik_0000901c` · `0xffffffb0` · `0xfe363c82` · `0x00000000` · `0xf24cd280` · `0x00000000` |
| 0x1407c | 109 |  |
| 0x14080 | 10 | `0x0000015e` |
| 0x14088 | 107 |  |
| 0x1408c | 91 | `evt_mario_get_pos` · `0x00000000` · `0xfe363c80` · `0xfe363c81` · `0xfe363c82` |
| 0x140a4 | 91 | `evt_snd_sfxon_3d` · `SFX_VOICE_MARIO_SURPRISED1` · `0xfe363c80` · `0xfe363c81` · `0xfe363c82` · `0x00000000` |
| 0x140c0 | 109 |  |
| 0x140c4 | 91 | `evt_mario_set_pose` · `M_N_7` |
| 0x140d0 | 10 | `0x00000212` |
| 0x140d8 | 95 | `0xfd050f80` |
| 0x140e0 | 91 | `evt_npc_set_battle_info` · `str_クリボー１_tik_00008fd4` · `0x00000004` |
| 0x140f0 | 91 | `evt_npc_battle_start` · `str_クリボー１_tik_00008fd4` |
| 0x140fc | 91 | `evt_npc_wait_battle_end` |
| 0x14104 | 95 | `0xfe363c8a` |
| 0x1410c | 95 | `0xfe363c8b` |
| 0x14114 | 95 | `0xfe363c8c` |
| 0x1411c | 91 | `evt_npc_set_position` · `str_クリボー１_tik_00008fd4` · `0xffffffbf` · `0x00000000` · `0x00000041` |
| 0x14134 | 91 | `evt_npc_set_position` · `str_クリボー２_tik_00009010` · `0xffffffc9` · `0x00000014` · `0x00000023` |
| 0x1414c | 91 | `evt_npc_set_position` · `str_クリボー３_tik_0000901c` · `0xffffffc9` · `0x00000000` · `0x00000055` |
| 0x14164 | 91 | `evt_mario_set_pose` · `M_S_1` |
| 0x14170 | 91 | `evt_npc_set_damage_anim` · `str_クリボー１_tik_00008fd4` |
| 0x1417c | 91 | `evt_npc_set_damage_anim` · `str_クリボー２_tik_00009010` |
| 0x14188 | 91 | `evt_npc_set_damage_anim` · `str_クリボー３_tik_0000901c` |
| 0x14194 | 91 | `evt_cam3d_evt_set` · `0xffffff88` · `0x00000096` · `0x0000024c` · `0xffffff88` · `0x00000037` · `0x00000025` · `0x00000000` · `0x0000000b` |
| 0x141bc | 10 | `0x000001f4` |
| 0x141c4 | 91 | `evt_npc_flag_onoff` · `0x00000000` · `str_クリボー１_tik_00008fd4` · `0x00000200` |
| 0x141d8 | 91 | `evt_msg_print` · `0x00000000` · `mac_0_093` · `0x00000000` · `str_クリボー１_tik_00008fd4` |
| 0x141f0 | 50 | `0xfaa2b580` · `0x00000001` |
| 0x141fc | 50 | `0xfaa2b581` · `0x00000001` |
| 0x14208 | 107 |  |
| 0x1420c | 10 | `0x0000003c` |
| 0x14214 | 91 | `evt_npc_set_run_anim` · `str_クリボー１_tik_00008fd4` |
| 0x14220 | 91 | `evt_npc_get_position` · `str_クリボー１_tik_00008fd4` · `0xfe363c80` · `0xfe363c81` · `0xfe363c82` |
| 0x14238 | 91 | `evt_snd_sfxon_3d` · `SFX_STG0_KURI_ESCAPE1` · `0xfe363c80` · `0xfe363c81` · `0xfe363c82` · `0x00000000` |
| 0x14254 | 93 | `ashioto` · `0xfe363c8a` |
| 0x14260 | 91 | `evt_npc_move_position` · `str_クリボー１_tik_00008fd4` · `0xffffff56` · `0xfe363c82` · `0x00000000` · `0xf24d2280` · `0x00000000` |
| 0x14280 | 95 | `0xfe363c8a` |
| 0x14288 | 91 | `evt_npc_get_position` · `str_クリボー１_tik_00008fd4` · `0xfe363c80` · `0xfe363c81` · `0xfe363c82` |
| 0x142a0 | 91 | `evt_snd_sfxon_3d` · `SFX_STG0_KURI_JUMP1` · `0xfe363c80` · `0xfe363c81` · `0xfe363c82` · `0x00000000` |
| 0x142bc | 91 | `evt_npc_jump_position_nohit` · `str_クリボー１_tik_00008fd4` · `0xffffff01` · `0x0000004b` · `0xfe363c82` · `0x000001f4` · `0x00000032` |
| 0x142dc | 93 | `ashioto` · `0xfe363c8a` |
| 0x142e8 | 91 | `evt_npc_move_position` · `str_クリボー１_tik_00008fd4` · `0xfffffe3e` · `0xfe363c82` · `0x00000000` · `0xf24f2a80` · `0x00000000` |
| 0x14308 | 95 | `0xfe363c8a` |
| 0x14310 | 91 | `evt_npc_set_position` · `str_クリボー１_tik_00008fd4` · `0x00000000` · `0xfffffc18` · `0x00000000` |
| 0x14328 | 50 | `0xfaa2b580` · `0x00000000` |
| 0x14334 | 109 |  |
| 0x14338 | 107 |  |
| 0x1433c | 91 | `evt_npc_set_run_anim` · `str_クリボー２_tik_00009010` |
| 0x14348 | 91 | `evt_npc_get_position` · `str_クリボー２_tik_00009010` · `0xfe363c80` · `0xfe363c81` · `0xfe363c82` |
| 0x14360 | 91 | `evt_npc_move_position` · `str_クリボー２_tik_00009010` · `0xffffff56` · `0xfe363c82` · `0x00000000` · `0xf24d2280` · `0x00000000` |
| 0x14380 | 91 | `evt_npc_jump_position_nohit` · `str_クリボー２_tik_00009010` · `0xffffff01` · `0x0000005f` · `0xfe363c82` · `0x000001f4` · `0x00000032` |
| 0x143a0 | 91 | `evt_npc_move_position` · `str_クリボー２_tik_00009010` · `0xfffffe3e` · `0xfe363c82` · `0x00000000` · `0xf24f2a80` · `0x00000000` |
| 0x143c0 | 91 | `evt_npc_set_position` · `str_クリボー２_tik_00009010` · `0x00000000` · `0xfffffc18` · `0x00000000` |
| 0x143d8 | 109 |  |
| 0x143dc | 107 |  |
| 0x143e0 | 10 | `0x0000001e` |
| 0x143e8 | 91 | `evt_npc_set_run_anim` · `str_クリボー３_tik_0000901c` |
| 0x143f4 | 91 | `evt_npc_get_position` · `str_クリボー３_tik_0000901c` · `0xfe363c80` · `0xfe363c81` · `0xfe363c82` |
| 0x1440c | 91 | `evt_npc_move_position` · `str_クリボー３_tik_0000901c` · `0xffffff56` · `0xfe363c82` · `0x00000000` · `0xf24d2280` · `0x00000000` |
| 0x1442c | 91 | `evt_npc_jump_position_nohit` · `str_クリボー３_tik_0000901c` · `0xffffff01` · `0x0000004b` · `0xfe363c82` · `0x000001f4` · `0x00000032` |
| 0x1444c | 91 | `evt_npc_move_position` · `str_クリボー３_tik_0000901c` · `0xfffffe3e` · `0xfe363c82` · `0x00000000` · `0xf24f2a80` · `0x00000000` |
| 0x1446c | 91 | `evt_npc_set_position` · `str_クリボー３_tik_0000901c` · `0x00000000` · `0xfffffc18` · `0x00000000` |
| 0x14484 | 50 | `0xfaa2b581` · `0x00000000` |
| 0x14490 | 109 |  |
| 0x14494 | 5 | `0x00000000` |
| 0x1449c | 24 | `0xfaa2b580` · `0x00000000` |
| 0x144a8 | 7 |  |
| 0x144ac | 33 |  |
| 0x144b0 | 91 | `evt_mario_set_dir_npc` · `str_クリボー３_tik_0000901c` |
| 0x144bc | 91 | `evt_party_set_dir_npc` · `0x00000000` · `str_クリボー３_tik_0000901c` |
| 0x144cc | 9 | `0x00000001` |
| 0x144d4 | 6 |  |
| 0x144d8 | 11 | `0xfaa2b581` |
| 0x144e0 | 10 | `0x000000c8` |
| 0x144e8 | 91 | `evt_cam3d_evt_set` · `0xffffff74` · `0x00000041` · `0x00000136` · `0xffffff74` · `0x00000021` · `0x00000025` · `0x00000000` · `0x0000000b` |
| 0x14510 | 10 | `0x000000c8` |
| 0x14518 | 91 | `evt_msg_print_party` · `mac_0_094` |
| 0x14524 | 10 | `0x0000014d` |
| 0x1452c | 91 | `evt_msg_print` · `0x00000000` · `mac_0_094_01` · `0x00000000` · `extparty` |
| 0x14544 | 91 | `evt_cam3d_evt_off` · `0x000001f4` · `0x0000000b` |
| 0x14554 | 91 | `evt_party_run` · `0x00000000` |
| 0x14560 | 91 | `evt_party_run` · `0x00000001` |
| 0x1456c | 50 | `0xf5de0180` · `0x0000000c` |
| 0x14578 | 91 | `evt_hitobj_onoff` · `a_rec` · `0x00000001` · `0x00000000` |
| 0x1458c | 91 | `evt_mario_key_onoff` · `0x00000001` |
| 0x14598 | 91 | `evt_snd_bgmon_f` · `0x00000300` · `BGM_STG0_TIK1` · `0x000007d0` |
| 0x145ac | 2 |  |
| 0x145b0 | 1 |  |

## Remaining bounded requests

Full setup4 battle unit/group/start/result coordinator, map moving-floor and
bomb/saveblock services, generic pipe paper animations and source-rendered
camera/effect/message token parity still need their actual providers and runtime
checks. No original runtime observation was made. Original resources remain
local ignored assets; this contract stores identifiers/hash evidence, not copied
story text. No native code was changed or tests required for this documentation.
