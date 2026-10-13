# Professor temporary party slot and fence admission

US G8ME01 disc0 revision0, read-only static DOL and gor.rel evidence. Scoped
answer to [OW-ROAD-01](../../requests/OW-ROAD-01.md), connecting the used
[Professor first-talk/fence event](../story/PROFESSOR_FIRST_TALK_AND_FENCE.md)
to existing portable party construction/history contracts. No source runtime
trajectory, collision equivalence or completed prologue is claimed.

## Fence admission is the house exit callback

There is no newly discovered independent fence touch/proximity trigger in this
used graph. Professor door record gor.rel0x45cc8 uses close/interior hit
**A_ie_02_doa02** and open/exterior hit **A_ie_02_doa01**. The specialized
entry0x45d10 registers case9 against closeHit with evt_door_close and LW15=8,
and against openHit with evt_door_open and LW15=0. Case9 requires exact named
Mario forward contact+1e4, admitted A, no NPC talk admission and common case
gates, as recovered in [same-map doors](../overworld/OVERWORLD_ROGUEPORT_INTERIOR_DOORS.md).
This is contact plus A, not an arbitrary distance-to-fence condition.

Generic close RUN_EVENTs the source close callback before its remaining travel.
Professor close callback0x45b98 checks story operand0xf5de0180 exactly9;
at0x45c4c it RUN_EVENTs **kurihakase_wall_remove** REL0x490f8,size0x404.
That event starts evt_door_end_wait at0x490f8, then keys off and its original
scene. Callback common tail WAIT500ms then parameter5/party-door-move135;
LW15 bit8 suppresses duplicate generic party movement. Fence removal starts
after this door ownership completes, not when story9 alone is loaded.
Story9 gor_02 init itself does not register another wall-removal caller.

Source scene beginning:

```
0x490f8 evt_door_end_wait
0x49100 evt_mario_key_onoff(0)
0x4910c WAIT_MSEC(500)
0x49114 evt_mario_set_dir(270,200,0)
0x49128 evt_cam3d_evt_set(-270,100,595,-270,30,201,2000,11)
0x49150 evt_party_get_pos(1,LW0,LW1,LW2)
0x49168 evt_mario_clear_party(1)
0x49174 evt_npc_set_position(Professor,LW0,LW1,LW2)
0x4918c evt_npc_set_ry(Professor,270)
0x4919c evt_npc_flag_onoff(1,Professor,0x40000020)
0x491b0 evt_npc_move_position(Professor,-265,187,0,FLOAT180,0)
0x491d0 evt_msg_print(0,mac_0_083,0,Professor)
```

All paper/SFX/actor/camera commands and story10 lecture handoff are retained
in the existing full243-command first-talk/fence timeline. Keep original US
messages and original fence OFF_d_saku source animation; this admission
contract does not authorize skipping that scene or writing story11 directly.

## Exact temporary kind11 constructor binding

First-talk tail0x49020 gets Professor XYZ,0x49038 hides Professor NPC,
0x49050 party_stop(1),0x4905c evt_mario_set_party_pos(1,11,LW0,LW1,LW2).
After original music/camera requests it runs party1 then party0 and writes
story9 at0x490d8. Source NPC and temporary partner are separate resource owners.

`evt_mario_set_party_pos` 0x800e3120,size0x158 is a first-call-only creation
wrapper. Rawslot selects existence byte Mario+245+slot; signednonnegative
returns2 without reading kind/coordinates or replacing the actor. Otherwise
resolve kind thenXYZ through INTEGER event resolution and convert XYZ to
float. Queried event float coordinates therefore undergo source quantization
and truncation before construction. Slot0 calls partyEntryPos; nonzero calls
partyEntry2Pos. Negative constructor result returns2; success stores result's
low byte to Mario+245+slot. No local retry, saved-kind clear or replacement.

For usedslot1,kind11 `partyEntry2Pos` allocates only table1 and refuses an
existing table1. Its [position-constructor contract](FIELD_PARTY_POSITION_CONSTRUCTORS.md)
recovers complete ordered allocation/animation/init. Kind11 record at
**0x803145a0** (0x803143e8+11*40) contains:

| Offset | Exact source binding |
| --- | --- |
| +0 | model string c_kuri_h at0x802cb4bc |
| +4 | kuribo_init0x800c4fa0 |
| +8 | partyMoveWalk0x800ca134 |
| +c | kuribo_use0x800c4bdc |
| +10 | null |
| +14 | kuribo_bye0x800c4b80 |
| +18..24 | null |

Normal pose row **0x8031499c** (0x80314708+11*60),selector0..14:

```
Z_1,S_1,W_1,R_1,W_1,W_1,S_1,T_1,Z_1,Z_1,S_1,Z_1,Z_1,W_1,R_1
```

Kind11 is not<8, so initial ordinary selector1 does not query partner HP and
selectsS_1. Secondary construction initializesP+0=13 (rawbits1/4/8),slotbyte
P+2f=1,kindbyteP+31=11,ownerP+160=Mario;unit scaleXYZ;position+58/+5c/+60
and anchor+b8/+bc/+c0 from suppliedXYZ;mode2. Its animation pose calls PeraOff,
unlike ordinary NPC PeraOn: use the existing partner display owner rather than
the NPC camera-facing wrapper for this temporary actor. Init callback
kuribo_init setsP+33=1,P+32=0,P+f0=24,P+f4=26 and clears0x8041eae0.
These are source initialization fields, not Professor NPC tribe30/30.

## Stop/run and initial mode2 matter

In a fresh first-talk domain slot1 is absent when party_stop(1) executes, so
that request does nothing; constructor then creates mode2. party_run(1) changes
only mode0→1 and consequently leaves fresh mode2 unchanged. Do not assume
the preceding stop carried over into the newly created object. Existing
[callbacks](FIELD_PARTY_EVENT_CALLBACKS.md) recover rawslot dispatch and
[mode transitions](FIELD_PARTY_MODE_INITIALIZATION.md) recover ordered writes:
stop requestsmode0;run acts only when modebyte+34==0;wrapper savesold to+35
unless old9/3, calls cleanup, writesnew+34,OR8 intoP+8. EnabledP+0 bit1 remains.
Stop is not a global scheduler-disable operation; mode0 still has record+8
callback behavior through the source [driver](FIELD_PARTY_DRIVER_DISPATCH.md).

## Secondary path follows Goombella under ordinary conditions

driveParty mode2 calls kindrecord+8, so kind11 uses the **same partyMoveWalk
callback as ordinary ground partners**. The existing
[walk callback](FIELD_PARTNER_WALK_CALLBACK.md),
[history writer](FIELD_PARTNER_HISTORY_WRITER.md),
[walk dispatcher](FIELD_PARTNER_WALK_DISPATCH.md) and ground movement contracts
retain actual path/terrain/numeric services. Do not substitute a new radius
or fixed-offset trail merely because the rendered character is Professor.

Initialization consumesP+8 pendingbit8,clearsP+0 mask0x100,setsP+0 bit2.
P+0 bit8 selects secondary historybank1 at0x803ec2c8+0x500. It queries
marioGetPartyId then partyGetPtr for the controlled ordinary partner. If that
partner exists, its nokonoko status is not3 and distance(ownerMario,Q)<150,
initialize all80 historyrecords fromQXYZ; otherwise useMarioXYZ. Both bank1
indices reset0. This source domain usually chooses Goombella, not Mario.

Runtime partyWalkMain0x800c9b9c instead uses signedslotP+2f. Nonzero selects
bank1 and anotherPartyGetPtr(slot), whose nonnegative route is
table[(slot+1)&1]; slot1 therefore selects table0 Goombella. UseMario source if
Qnull, Qkind3 andQmode3, nokonokoGetStatus(Q)==3 or orderedMario↔Qdistance>=150.
Otherwise captureQXYZ with Q+0 mask0x10 mapped tohistoryflag1. It appends
through original change/full-buffer guards then always calls walkMain.
No fixed following delay belongs in this writer: reader/index/distance services
own actual spacing and movement. Initialization and runtime slot/source
predicates differ; preserve both instead of conflating them.

## Clear and original NPC restoration

Fence event reads temporary party1 XYZ **before** clear. Raw clear(1) tests
Mario+246 then calls **partyKill2(1)**, not ordinary partyKill. The
[destruction contract](FIELD_PARTY_DESTRUCTION_AND_SAVED_KIND.md) retains its
ordered resource/table/owner-slot writes and saved-kind distinction.
It then restores original Professor NPC at the captured query coordinates
through INTEGER set_position resolution. Do not simultaneously display the
NPC and temporary follower or retain the follower after clear. Story10 has
Professor NPC outside; Goombella remains ordinaryparty0. Scene/resource
teardown, original collisions and actual follow trajectories remain native
dependencies beyond this source binding.

## Reproduction

Read original DOL evt_mario_set_party_pos and partyMoveWalk with
tools/inspect_dol_function.py plus external US symbols. Read kind11 record and
pose row directly at addresses above. Existing verified contracts cover
constructors,driver,stop/run,history/destruction; this document adds their
actual prologue caller/kind bindings. Source gor REL event/import inventory
supplies the retained first-talk/fence command order. No native/shared source
edits, copied original payloads, new runtime trace or completion claim.
