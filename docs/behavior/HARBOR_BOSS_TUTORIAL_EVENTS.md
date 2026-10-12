# Harbor boss event hooks and tutorial feedback graph

US G8ME01 revision0,recovered2026-10-12. Evidence B:seven original event spans
in gor.rel unit_boss_kanbu1.o,two REL feedback helpers,data/weapon relocations
and [five state services](BATTLE_TUTORIAL_STATE_SERVICES.md). This extends
[configuration index2](HARBOR_TUTORIAL_CONFIGURATION.md) into actual installed
scripts. No complete playable tutorial or original timing comparison claimed.
All offsets below are REL file offsets,not process addresses.

## Initialization and installed scripts

init_event0x64808,size0x148 contains18 commands. In original order it requests:

1. SetEventWait(-2,wait_event0x64e40),SetEventAttack(-2,attack_event0x64de0),
   SetEventDamage(-2,damage_event0x64db8),SetEventConfusion(-2,DOL default
   Confuse),SetEventPhase(-2,phase_event0x64950).
2. onoff_battleflag(1,0x1000).
3. SpawnUnit(output local3,entry_npc_christine0x647d8,0). Store that returned
   unit ID in the boss's unit-work slot0. Set the spawned actor's position and
   home to(-140,0,5).
4. Set boss unit-work slots1,2,3 to0. These become independent feedback latches.
5. Install left/right walk sounds SFX_BOSS_KANBU_MOVE1L/R with parameters0,10,10;
   install run sounds with0,6,6. StartWaitEvent(-2),then explicit event-delete2
   and end marker1.

The negative actor selectors retain their original BattleTransID context;
installation/service bodies and actor spawn lifecycle still need closure.
The helper actor is a spawned battle unit,not simply the already rendered field
NPC. Preserve the returned unit ID for later speaker calls.

The kind's data table0x646b4 contains key48→DOL btldefaultevt_Dummy,key49→local
dead_event0x64e58,then a key0 terminator. This adds a death-event data binding
beyond init's explicit hook setters;see [event dispatch](BATTLE_EVENT_DISPATCH.md).
Do not replace all events with a single generic boss attack routine.

## Phase event: opening branch

phase_event0x64950,size0x468 contains84 commands. First get_turn into local0.
If turn==1,query exact phase0x04000001 into local0. When nonzero:

- Stop the -3 actor's wait event;request pose M_S_1 and camera mode3/move.
- WAIT_MSEC500. Start a named inline rotation event:interpolation0→180 over15,
 16 iterations with one-frame waits,then join that event.
- Hide status window. Read boss unit-work0 into local3;set that actor's talk
  pose PKR_T_1 and stay pose PKR_S_1.
- Print **mac_0_b_t1_000** using evt_msg_print(2,key,0,local3). Restore status
  window,request Mario voice/pose/play-complete and a second named rotation
  event180→0,then join it.
- Restore camera mode0 and restart the -3 actor's wait event. Delete this phase
  event immediately;later feedback checks are not reached on this branch.

This is a guarded instruction path,not proof of one universal frame schedule.
Interpolation endpoint/count,clock waits and inline joins remain separate clocks;
original input/control availability during the message must be validated.

## Phase event: feedback guards and priority

If the opening branch does not delete the event,query exact phase0x04000003.
Only a nonzero result enters the remaining feedback checks. They run in this
order,with boss unit-work latches separate from script-local variables:

| Priority/guard | Mutation and original message |
| --- | --- |
| Work3==0 and current HP<=integer DIV(maxHP,2) | Set work3=1;hide status window;read helper ID from work0;print mac_0_b_t1_003;show status;delete event immediately |
| Work1==0 and success helper's battle byte!=0 | Hide status;set work1=1;read helper ID;print mac_0_b_t1_001;show status;continue |
| Work2==0 and failure helper's battle byte!=0 | Hide status;set work2=1;read helper ID;print mac_0_b_t1_002;show status;continue |

The HP path calls GetMaxHp,opcode56 DIV by2,then GetHp and integer
IF_LESS_EQUAL. Its early event-delete means simultaneous success/failure
feedback is deferred to another invocation. The success/failure paths are two
independent IFs,not an ELSE_IF:both can run in one eligible invocation if their
bytes are nonzero and latches unset. Every feedback print is mode2,argument0 and
speaker local3 (the unit ID stored at work0).

Do not trigger feedback solely from a port's last attack result. Original REL
helpers read bytes+0x16f58/+0x16f59 of shared battle state;the [declaration writer/reset contract](BATTLE_ACTION_FEEDBACK_AND_STAGE_GATE.md)
now links them to capped declaration counts;complete reset cadence remains open. The latch prevents repetition after that path's
message but does not identify how battle-wide bytes are accumulated.

## Small attack, damage and wait hook streams

wait_event0x64e40,size0x18 has three commands:AnimeChangePoseFromTable(-2,1),
event-delete2,end1. There is no local loop or combat result mutation.

damage_event0x64db8,size0x28 assigns local10=-2,local11=1,requests the DOL
btldefaultevt_Damage as a child,then deletes/ends. Those locals are inputs to the
original child protocol;do not discard them when adapting the damage event.

attack_event0x64de0,size0x60 first invokes EnemyItemUseCheck(-2,local0). Nonzero
local0 is run as a child event,then StartWaitEvent(-2) and immediate delete.
Otherwise set local9 to weapon_kanbu1_attack0x64718,run normal_attack_event
0x653c8 as a child,restart wait and delete/end. The initial zero drop table does
not by itself prove the unit can never acquire/use an item by other routes.

The normal attack stream has95 commands. At0x655e0 it requests PreCheckDamage
using local9 and flags0. At0x656bc it requests ResultACDefence(target local3,
weapon local9),then0x656cc CheckDamage with flags0x100. The weapon's+0x13 byte
is1,passing the **local weapon eligibility gate** of the recovered defense
checker;its HP-power callback is DOL weaponGetPowerDefault0x80189d30.
This is not proof that every global/tutorial/input gate admits superguard,or of
actual damage/impact tick. Remaining movement/precheck/branch/weapon-power and
force consumers must be recovered before implementing its attack.

## Death-event continuation and message bindings

dead_event0x64e58,size0x450 contains75 commands. It conditionally runs unit data
key50 as a child,requests death sounds/rotation/smoke,gets/stores coins and EXP,
positions the star-point effect,performs subsequent rotation and back-item
requests,sets part attribute0x03000000 and waits for attack completion.
These are original service requests;reward formulas and full data/effect semantics
remain open. No fixed XP/coin total is established from callback names.

The later portion stops the -3 actor's wait,changes pose and performs/join a named
rotation event before hiding status. It reads helper ID from boss work0,sets its
talk/stay poses,and prints mac_0_b_t1_004 at0x651d0. After camera mode17 and
speed2 requests,it prints mac_0_b_t1_005 at0x65208. Restore status,rotate/join
back,then KillUnit(local10,0) at0x65290. The message/actor-dependent work occurs
**before** this explicit unit deletion;premature deletion can lose the stored
helper ID or speaker context.

| Message ID | Call site | Local condition |
| --- | --- | --- |
| mac_0_b_t1_000 | 0x64a8c | Turn1 and exact phase0x04000001 |
| mac_0_b_t1_003 | 0x64c4c | Phase0x04000003,unset HP latch and half-HP comparison |
| mac_0_b_t1_001 | 0x64ce8 | Phase0x04000003,unset success latch and nonzero success byte |
| mac_0_b_t1_002 | 0x64d80 | Phase0x04000003,unset failure latch and nonzero failure byte |
| mac_0_b_t1_004 | 0x651d0 | Death-event continuation after attack wait/rotation join |
| mac_0_b_t1_005 | 0x65208 | Same death continuation after first message/camera requests |

All six keys uniquely exist in the identified original msg/US/gor_00.txt.
Original payload bytes/control tokens remain local and unchanged;no replacement
story text or paraphrase is committed. Message-file numeric order would be an
incorrect substitute for these event guards and joins.

## Evidence and next closure work

Whole hashes pin all seven event spans,two REL helper bodies and the data hook;
helper global/call relocations are independently checked. Five DOL state helpers
and24 anchors cover work slots,phase/turn predicates and raw-mask flag updates.
The enemy weapon defense/power binding is checked directly. Ignored raw REL
inspection listings explicitly identify unrelocated file-offset branch displays.

Recover tutorial battle flag0x1000 consumers,feedback byte writers,SpawnUnit and
kind/stat-copy/init lifetime,full normal attack/precheck/result/stylish/guard
conditions,HP/reward/defeat/sequence outcomes and remaining inline/actor/message
services. Record original gameplay input/state/animation/audio traces. These
installed scripts identify real tutorial behavior but do not prove the full
encounter or complete US game equivalent;final requirements remain unfinished.

## Normal attack follow-up

The complete request stream is now documented in
[boss normal attack](HARBOR_BOSS_NORMAL_ATTACK.md),with
[target interfaces](BATTLE_TARGET_EVENT_INTERFACES.md). Defense/damage requests
occur only on precheck result1;other routes proceed through avoidance/common
recovery. The stream has an unresolved no-target GOTO99 without a local label99.
These contracts close the script/ABI slice,not the movement/input/damage service
composition or measured impact timing.
