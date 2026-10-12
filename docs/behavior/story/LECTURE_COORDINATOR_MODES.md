# Lecture professor work modes,phase transitions and exact US message bindings

US G8ME01 disc0 revision0, agent ID1, recovered2026-10-12. Evidence B: original
gor unit_lecture_frankli init0x682b8,size0x98,11 commands;phase0x68378,size0xaa4,
200 commands;related lecture Goombella init/attack/counter and ID-binding leaves.
All addresses below are **REL file offsets**. This specifies the complete local
phase request graph,not the full lecture's player attacks,field story entry,
callback runtime or observed timing.

[Helper roles](CHRISTINE_HELPER_ROLES.md) establishes that this coordinator belongs
to lecture setup index3 rather than opening Crump index2. The professor P is
spawned by lecture Goombella G,stored in BattleWork+0x424 and selected by actor-6.
P.work0=G.ID;G.work0=P.ID. P.work1 is the mode read by the phase event and by
Goombella's injection/attack/counter branches. P.work2 and3 are separate feedback
slots,not alternative names for mode1.

## Coordinator initialization and installed requests

Professor init installs Wait0x68e30,Phase0x68378,Attack0x68e1c,Damage0x68350 and
default Confuse on owner-2. Then SetMaxMoveCount(-2,0),SetUnitWork(-2,1,0),
onoff_battleflag(1,0x1000),StartWaitEvent(-2),delete2,end1. This init explicitly
writes mode0;it does not explicitly assign work0/2/3. The cross-link to work0
comes from G's init after SpawnUnit. Unit allocation/other writers supply any
additional initial state;do not silently collapse these slots.

Phase begins GetUnitWork(-2,1,local0),then SWITCH(local0) at0x6838c. The outer
switch snapshots mode. Subsequent writes to P.work1 or local0 do not select another
outer case in this invocation. Cases are0,2,4,6,8,9. Other modes have no local
phase body and reach the final event-delete/end. Active cases use CheckPhase
with an **exact** value0x04000001 or0x04000003,then IF_NOT_EQUAL(result,0).
A failed phase check does not perform that case's transition or message requests.

## Mode0 introduction branch

CASE0 at0x68394 requires exact phase0x04000001. Stop Mario's wait event (-3),
request pose M_S_1,and **write P.work1=1 before the message/clock requests**.
Set camera0 mode3 and moveto arguments(0,58,256,0,41,0,30,11) after the camera
index;WAIT_MSEC500. Hide status window,request P talk pose T_1/stay S_1,print
mac_0_b_t2_000 with mode2,argument0,speaker owner-2. Restore status window,
camera mode0,set battle flag0x01000000,restart Mario wait,then delete the phase
event. This does not establish the flag's command-menu consumer or a wall-clock
input-lock duration. Mode1→2 is outside this phase body;the [lecture player Jump cleanup](LECTURE_PLAYER_ATTACK_FLOW.md) now establishes that writer.

## Mode2 guard-demonstration preparation

CASE2 at0x684b4 requires phase0x04000003. Write **P.work1=3** first. Stop Mario
wait,pose M_S_1,hide status;set P T_1/S_1 and print mac_0_b_t2_003 as P. Then:

1. Named inline event:interpolation mode5,0→180,duration20;loop21 times requesting
   interpolation value,SetRotate(P,0,value,0),WAIT_FRAME1. Explicit final rotation
   (0,180,0). Parent joins its recorded event ID.
2. Reset P talk/stay T_1/S_1 and print mac_0_b_t2_004 as P.
3. Read P.work0 into local3 (G.ID),stop that unit's wait,set PKR_T_1/PKR_S_1,
   print mac_0_b_t2_005 with G as speaker.
4. Restore status,restart Mario wait. A brother event interpolates P180→0 with
   duration20/21 loop iterations and one-frame waits,then sets rotation0.
5. Delete the parent phase event without a join request for that final brother.

On a later G attack,mode3 selects the A-mask private-history injection. After
its child attack completes,G's attack wrapper changes exact mode3→4. These are
separate event invocations/owners;do not perform the demonstration and transition
synchronously just because this preparation branch stores3.

## Mode4 practice gate

CASE4 at0x686e8 requires phase0x04000001. Write **P.work1=5**,stop Mario wait,
pose M_S_1,hide status,set P T_1/S_1,print mac_0_b_t2_007 as P,restore status.
Clear battle flag0x01000000,set0x02000000,delete event. This branch does not
restart Mario's wait locally;do not insert one merely to match other cases.
The [lecture player/selection follow-up](LECTURE_PLAYER_ATTACK_FLOW.md) supplies
the default-weapon masks and Hammer cleanup5→6;other flag/menu consumers remain
dependencies. Goombella's shown
injection switch has no case5,so the two known forced A/B injections do not apply
at that value through that switch;other writers still need recovery.

## Mode6 B-demonstration preparation

CASE6 at0x687b8 requires phase0x04000003. Write **P.work1=7**,stop Mario wait,
pose M_S_1. Named inline event first rotates actor-6 (P)180→0,duration20,
21 iterations with WAIT_FRAME1,explicit final0;parent joins. Hide status,
set P T_1/S_1,print mac_0_b_t2_011 as P. Another named inline event rotates P
0→180 with the same duration/loop/wait pattern;parent joins. Reset P talk/stay,
print mac_0_b_t2_012 as P. Read P.work0→local3,stop G wait,set its PKR_T_1/
PKR_S_1,print mac_0_b_t2_013 as G. Restore status,restart Mario wait,launch a
brother P180→0 event with explicit final0,then delete parent without joining it.

A later G attack under mode7 selects the B-mask private-history injection.
The related counter data event,when invoked and reaching its tail under mode7,
sets mode8. G's ordinary post-child attack wrapper only changes3→4 and does not
perform a7→8 fallback. Counter dispatch/weapon eligibility remain part of the
runtime closure;mode7 does not automatically advance merely because B was injected.

## Mode8 choice and transition to mode9

CASE8 at0x68a68 requires phase0x04000001. Write **P.work1=9**,clear0x02000000,
set0x08000000. Stop Mario wait,pose M_S_1,hide status,set P T_1/S_1,print
mac_0_b_t2_016 as P. Camera mode3/moveto with the same tuple as mode0;
WAIT_MSEC500. Print mac_0_b_t2_017 as P,then evt_msg_select(0,mac_0_b_t2_018).
The following integer IF tests local0 from the selection protocol:

| Selection result | Original requests |
| --- | --- |
| Exact0 | evt_msg_print_add(0,mac_0_b_t2_019),camera mode0,status window1,delete event |
| Every nonzero value | evt_msg_print_add(0,mac_0_b_t2_020),camera mode0,set battle flag0x20,delete event |

Preserve original US selection tokens and result indexing. Do not reverse0 and
nonzero based on an invented menu label or assume the flag alone proves a final
battle result. Mode9 was stored **before** selection in both paths. Appends retain
the current message context;these are not new authored speeches. The exact
selection/cancel service remains as documented in [message selection](../message/MESSAGE_SELECTION.md).

## Mode9 feedback on two exact phases

CASE9 at0x68c0c first checks phase0x04000001. If nonzero,hide status,read
GetUnitWork(-6,3,local0),then use the following snapshot switch:

| P.work3 | Message request,mode2/argument0/speaker P |
| --- | --- |
| 4 | mac_0_b_t2_021 at0x68c60 |
| 5 | mac_0_b_t2_022 at0x68c80 |
| 6 | mac_0_b_t2_023 at0x68ca0 |
| 7 | mac_0_b_t2_021 at0x68cc0 |
| Other | No message from this switch |

Restore status and **delete the phase event**,including the unmatched value case.
The event therefore does not continue into the later phase check after this path.
The Goombella post-damage classifier writes work3=4/5/6 for its matching nonzero
classes;the counter script can write7. The classifier's empty class0 does not
fall through to work3=4;retain the original distinct writer paths.

If the first phase test is zero,check phase0x04000003. If nonzero,perform a parent
interpolation P180→0,duration20,21 iterations with WAIT_FRAME1,then final rotation0.
Hide status,read GetUnitWork(-6,2,local0),switch:

| P.work2 | Original message,speaker P |
| --- | --- |
| 1 | mac_0_b_t2_021 at0x68da0 |
| 2 | mac_0_b_t2_022 at0x68dc0 |
| 3 | mac_0_b_t2_023 at0x68de0 |
| Other | No message from this switch |

Restore status,delete event. P.work2's player-command producers are outside the
checked coordinator body. If neither phase matches,the outer switch reaches its
final END_SWITCH,event-delete2,end1. It does not clear feedback slots or advance
mode9 locally,so future phase invocations can consult them again. Actual caller
frequency/reset/turn policies remain open.

## Exact US resource and speaker obligations

The20 coordinator message/select/append call sites bind16 distinct keys in
**msg/US/gor_02.txt**,not the opening's gor_00 resource. The dedicated verifier
checks each original key pointer,service binding,speaker operand where present,
key uniqueness and original payload SHA-256. Text/control tokens stay in the
read-only ISO/local asset pack;this documentation does not paraphrase dialogue.

Speaker P is event owner-2;G is the unit ID read from P.work0 and placed in local3.
Message keys003/004/005 and011/012/013 retain their original interleaving/attribution.
Choice018 and appends019/020 must use their original contents and control syntax.
An implementation may reflow screens/adapt control prompts only as presentation;
it must preserve these branch/identity/order conditions.

Named inline rotation events are joined where explicitly requested;brother
rotations have separate ownership/lifetime. Duration20 and loop21 are original
script operands,not measured total frames. WAIT_MSEC500,event credits,movement/
pose services and message polling remain separate clocks. See
[VM/concurrency](../event/EVENT_JUMP_BRANCHES_AND_CONCURRENCY.md) and
[system speed](../event/EVENT_SYSTEM_LEVEL_AND_SPEED.md) before mapping these to simulation.

## Implementable mode boundaries and incomplete edges

| Boundary | Established producer | Still required |
| --- | --- | --- |
| Initial0 | P init explicit work1 write | Spawn/init schedule and first phase invocation |
| 0→1,2→3,4→5,6→7,8→9 | P phase exact guards/table above | Phase producers,service timing,flag consumers and original execution |
| 3→4 | G attack wrapper after child | Child completion/outcome,unit validity and scheduling |
| 7→8 | G counter-script tail under exact7 | Counter key dispatch/eligibility and original outcome trace |
| 1→2,5→6 | [Lecture player Jump/Hammer shared cleanup](LECTURE_PLAYER_ATTACK_FLOW.md) | Script runtime/field conditions and original traces |
| Feedback2/3 | Separate work slots and listed consumers/writers | Player writer results,reset lifetime and per-turn ownership |

Preserve actor IDs,independent slots,early-delete paths and message attribution.
Derived checks can hold mode fixed while toggling the exact phase to verify no
transition/message when the guard fails;verify selection0 versus nonzero after
mode9 is already stored;verify mode9 work3=7 chooses021 while unsupported values
still restore status/delete. These do not replace original interaction traces.

This closes coordinator and helper request-state slices. Full attack animation/
input/counter/player scripts,field entry/return,save persistence and full lecture
runtime remain incomplete. The complete-game requirements document remains
contingent on all remaining US systems/content being documented and verified.


## Player and status follow-up

[Lecture player flow](LECTURE_PLAYER_ATTACK_FLOW.md) now connects mode1/5/9
to distinct player scripts and first-hit/output feedback writers. Jump uses output
parameter0,Hammer2;their cleanup mode transitions are not success-only gates.
[Window arbitration](../battle/BATTLE_STATUS_WINDOW_ARBITRATION.md) proves that the
lecture's2740 blocker outranks event-on2748 requests. Earlier hide/restore wording
in this request graph describes callback requests,not verified visible windows.
Full pause/field/end/inner-service and original execution remain open.
