# Rogueport underground prologue pipe route

US G8ME01 disc0 revision0 read-only static REL/DOL evidence, scoped to
[OW-ROAD-01](../../requests/OW-ROAD-01.md). Complements existing generic BERO
contracts and [underground first arrival](../story/UNDERGROUND_FIRST_ARRIVAL_AND_GOOMBA_TRIO.md).
These are original source requests, not Dolphin measurements or native parity.

## Exact route records and admission

| Source table offset | Hit name | Flags | Direction | Case | Entry callback | Exit callback | Target/arrival |
| --- | --- | --- | --- | --- | --- | --- | --- |
| gor REL0x3ee60 | dokan | 0x00020000 | 9 | 6 | null | gor_02_event_01 (0x3edac) | tik_01/dokan_2 |
| tik REL0x13870 | dokan_2 | 0x00020000 | 6 | 10 | dokan_2_in_evt (0x13728) | null | gor_02/dokan |

Both have centerX100000 sentinel,Y/Z0, length-1 and animationtag0x10001.
The center sentinel selects hit-object-derived geometry, not literal100000 units.
Flags encode low twelve BERO type bits=2 (pipe), not road directionAUTO.
Default length for type2 with source-1 is60 original units.

`caseMain` table0x8030c1e0 case6→0x8006e87c: requires non-null Mario+0x1e8
contact pointer, `hitGetName` matching registered hit, signed Mario byte+0x253
(stickY) strictly>30. Case10→0x8006e940 requires non-null front contact+0x1f0
and exact registered hit name. Neither requires an A press or invented dwell.
The+0x1e8 producer is not established here: `evt_mario_get_name_hitobj_under`
actually reads+0x1fc, so do not mislabel these contact fields interchangeably.
Common gates require systemlevel0, winCheck0, gp+0x14=0,
N_fbatPreventMarioEventChk0 and marioCaseEventValidChk (reject motion24 and
Vivian status0). Pipe wrapper additionally requires evt_mario_get_mode==0.

## Departure, arrival and partner ownership

`bero_case_exec_pipe` DOL0x8030e28c/0x2c8 is59 commands, SHA256
`d000f168797d562e4d0ff2f6bcd695c1862c51c15964c552599110a5301e70be`.
It keys off, executes source exit callback via childbero_case_init_check,
briefly keys on and returns if callbackLW0==1. Otherwise keys off,
waitlandon, Mario control off. Null target uses separate same-map branch;
this route uses source mapchange branch. Camera disables, SFX_MARIO_DOKAN1,
direction9 paperpose3/speed60; direction6 paperpose5/speed60. SetBeroEXEC8,
wait2frames, optional1000ms whenfirstcheck==1, originalmapchange. Source flags
0x2000 are clear on these records, so their guarded fade branch is skipped.

`gor_02_event_01` REL0x3edac/0x78 does not have its own story gate. If flag
0xf8406037(GSF1207) already1, it setsLW0=0 and returns. Else party_stop1,
party_cont_onoff(1,0), evt_mario_set_party_pos(1,11,0,-1000,0), flag1207=1,
LW0=0 and returns. The callback creates Frankly's temporary kind11 in slot1;
LW0=0 preserves generic pipe travel. Existing slot0 remains Goombella.

`evt_bero_info_run` DOL0x8030ec7c/0x3e4 invokes
`evt_mario_set_prev_party_dokan(0)` then(1) for ordinary type2 entry;
background flag0x1000 instead kills both slots. It prepares direction6 paper4,
direction9 paper1, invokes evt_bero_initial_pos and launches mario_enter.
`evt_bero_initial_pos`0x8030f060/0x278 has45commands, SHA256
`214cdff31534df948655149d8ba53441124be40ce6b980d353dee0b48952c519`.
For type2 it requests length, pipe_pure(mode1,length), reads BeroSX/SY/SZ,
sets Mario and BOTH party slots to that same starting point, then camroadreset.
Thus tik arrival callback does not create or individually place Frankly.

`evt_bero_info_run_pipe`0x8030eba8/0xd4 owns camera reset2/BeroE, cameraoff,
control0, fadeendwait, SFXpipe, childsub00, control1, destinationcaseoff,
RUN_EVENTevt_bero_nonride_check, cameraon and keyson. Sub00 at0x8030eadc/0xcc
stops both party slots; direction6 displaysMario180,paper4,move60;
direction9paper1,move60, then dokan_end. Admission remains disabled until the
source nonride/contact lifecycle returns it, not a fabricated timer cooldown.

## Position transforms and geometry boundaries

`bero_get_position_pipe_sub`0x800798b8/0x2bc takes mode,length,hitName,
direction,outStart,outEnd. It calls hitObjGetPos. Original horizontal offsetK
is20.0 at0x804204f8. Direction6 negates length andK; directions8/9 are vertical,
and9 negates length. Used length is integer60. `pipe_pure`0x800797d0/0xe8
only has extra vertical height subtraction for direction8, not9.

For gor direction9, with hit centerH=(X,Y,Z):

| Mode | Start S | End E |
| --- | --- | --- |
| 0 departure | (X,Y,Z) | (X,Y-60,Z) |
| 1 arrival | (X,Y-60,Z) | (X,Y,Z) |

For tik direction6:

| Mode | Start X | End X | Y |
| --- | --- | --- | --- |
| 0 departure | hitX+20 | hitX-40 | Both use downward-ground ray atE |
| 1 arrival | hitX-40 | hitX+20 | Both use downward-ground ray atS |

Both keep hitZ. Source downward ray invokes hitCheckVecHit against collision
geometry; its sourcecastdistance is10000. Native ground-provider and contact
producer composition remains to be validated. Hit center is not final arrival
point: simply placing at center, or adding an arbitrary0.65nativeunit offset,
does not reproduce these requested positions. Original→native scale0.01 gives
horizontal offsets+0.20/-0.40 and vertical travel0.60.

## Reciprocal and other tik_01 exits

Table begins tikREL0x13780; every target and entrance below is literal source.
Source story/terrain/object admission beyond this route remains separate.

| Record | Hit | Target/arrival | Type/direction/case notes |
| --- | --- | --- | --- |
| 0x13780 | w_bero_1 | tik_00/e_bero | road |
| 0x137bc | e_bero_1 | tik_02/w_bero_1 | road |
| 0x137f8 | e_bero_2 | tik_02/w_bwro_2 | road; literal source typo retained |
| 0x13834 | dokan_1 | tik_15/dokan_2 | pipe direction9 case6 |
| 0x13870 | dokan_2 | gor_02/dokan | pipe direction6 case10 |
| 0x138ac | dokan_3 | null/dokan_5 | same-map direction9 case6 |
| 0x138e8 | dokan_4 | null/dokan_6 | same-map direction9 case6 |
| 0x13924 | dokan_5 | null/dokan_3 | flags0x10020000 direction9 case6 |
| 0x13960 | dokan_6 | null/dokan_4 | flags0x10020000 direction9 case6 |
| 0x1399c | e_bero | tik_11/w_bero | flags0x10000000 case0 |
| 0x139d8 | w_dero_2 | tik_12/e_bero | flags0x10000000 case0 |

Terminator0x13a14. No broad claim that all these exits are prologue-accessible.
Original generic paper pose/display animation, source contact producers,
full previous-party restoration, fade/camera/audio and dynamic geometry
comparison remain open; native adapters must retain that boundary explicitly.
