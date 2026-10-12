# Separate Crump NPC and lecture Goombella helper roles

US G8ME01 disc0 revision0, agent ID1, recovered2026-10-12. Evidence B: original
gor REL kind/setup/configuration records,installed event streams and three small
relocated helpers. Goombella and Professor Frankly are reading labels for the
christine/frankli symbol names;the numeric IDs,pointers and script speaker operands
are the authoritative bindings. No original scene execution or Switch behavior
is claimed. All offsets below are **REL file offsets** unless explicitly DOL.

The Crump helper and guard-lecture helper are **different kinds**,even if a port
reuses character assets. Their modes,attacks and initialization must not be merged.

| Role | Kind pointer / raw kind ID | Selected setup/init |
| --- | --- | --- |
| Crump's spawned NPC helper | unit_npc_christine0x66d78 /0xdd | entry_npc_christine0x647d8→kind;kind+0xbc→init0x66ed8 |
| Lecture attacker/helper | unit_lecture_christine0x66fe8 /0xb1 | group unit setup0x60cb0→kind;kind+0xbc→init0x67160 |
| Lecture coordinator spawned by the second role | unit_lecture_frankli0x67fb8 | entry_lecture_frankli0x67130→kind;kind+0xbc→init0x682b8 |

Both checked Goombella kind records carry signed initial HP10,FP0. That is a
kind→entry input,not a measured final HP or a reason to treat either actor as the
active player partner. Later resource/init overrides remain their own contracts.

## Crump spawn and NPC-only request hooks

[Crump init](HARBOR_BOSS_TUTORIAL_EVENTS.md) calls SpawnUnit with setup0x647d8,
flags0,and stores the returned unit ID in **Crump's** work0. Its setup kind
relocation points to0x66d78,not lecture kind0x66fe8. Crump later uses this ID for
mac_0_b_t1_* message speakers and places its spawned actor at(-140,0,5).
The [entry/spawn contract](UNIT_ENTRY_STATS_AND_SPAWN.md) supplies event owner
and registration behavior;registration does not execute all init commands inline.

NPC init0x66ed8,size0x64,eight commands,in request order:

1. Install Wait(-2,0x66f80),Attack(-2,0x66f6c),Damage(-2,0x66f3c).
2. Install Confusion(-2,DOL default Confuse).
3. SetMaxMoveCount(-2,0).
4. StartWaitEvent(-2),explicit event-delete2,end marker1.

There is **no local SetEventPhase**,lecture-mode setter,professor spawn or key
history injection in this init. A separately annotated phase_event0x66f64 only
deletes/ends;its existence is not evidence that this init installs or executes it.
The kind's data table at0x66e84 contains an immediate zero terminator.

Attack hook0x66f6c simply requests StartWaitEvent(-2),then deletes/ends. It does
not run a headbonk,request defense or calculate damage. Wait hook0x66f80 requests
AnimeChangePoseFromTable(-2,1),then deletes/ends. Damage hook0x66f3c sets local10
=-2,local11=1,runs default Damage as a child,then deletes/ends. Local-hook contents
do not replace the still-required scheduler/move-count/pose services.

Do not give this spawned role the separate lecture attack because both names or
portraits describe Goombella. Its unit ID is also distinct from a field NPC key
or active partner-menu slot. Conversely,the zero maximum-move request must be
bound through the recovered move-count service rather than guessed as a universal
actor-disable bit.

## Lecture battle setup index3

The shipped table's record index3 is0x60efc (base0x60e30,stride0x44). Its+0x14
binds weighted list0x60de8. That list has one positive weight50;its group pointer
at0x60dec binds0x60ce0,which has unit count1 and setup pointer at0x60ce4→0x60cb0.
The setup's kind pointer binds lecture kind0x66fe8. The stage pointer in the list
is0x60aac. This is separate from [Crump index2](HARBOR_TUTORIAL_CONFIGURATION.md).
The first two table records are not this lecture chain;do not identify a config
from a nearby symbol or partially overlapping record.

The [field setup service](FIELD_BATTLE_ENTRY_AND_RETURN.md) defines weighted
selection and original RNG consumption. This data chain establishes which actor
index3 selects;field NPC/story conditions that start that record and complete
lecture entry/return/persistence remain open. It does not prove this lecture runs
during the opening Crump battle.

## Lecture initialization and cross-referenced unit IDs

Lecture init0x67160,size0x138,19 commands installs Wait0x6749c,Phase0x67494,
Attack0x674b4,Damage0x67298 and default Confuse,all on owner-2. It then requests:

| Order | Original request |
| --- | --- |
| Walk sounds | SFX_PARTY_BATTLE_MOVE1L/R,parameters6,12,16 |
| Run sounds | Same keys,parameters0,9,3 |
| Jump sounds | SFX_PARTY_BATTLE_JUMP1 / SFX_PARTY_BATTLE_LANDING1 |
| Mode setup | _set_lecture_mode at0x11930 |
| Coordinator spawn | SpawnUnit(local3,entry_lecture_frankli0x67130,0) |
| Owner link | SetUnitWork(-2,0,local3),retaining the professor unit ID |
| Selector link | _battle_set_lecture_unit_id(local3) at0x118f0 |
| Orientation | ChangeFaceDirection(local3,-1) |
| Reverse link | GetUnitId(-2,local0);SetUnitWork(local3,0,local0),retaining lecture Goombella's ID on the professor |
| Battle flag | onoff_battleflag(1,0x1000) |
| Wait/termination | StartWaitEvent(-2),delete2,end1 |

Preserve both cross-links:lecture Goombella work0 points to the professor;
professor work0 points back to lecture Goombella. Crump's work0→NPC helper link
is a different pair of actors in another setup. Spawn flags0 do not themselves
insert an extra ordering-array ID,as documented by the spawn contract.

### Complete lecture-ID setter and selector-6

_battle_set_lecture_unit_id,file0x118f0,size0x40,captures shared W via its original
HA/LO relocations,reads operand0 and resolves it through evtGetValue,then stores
the resolved word at W+0x424. Return2. It does not BattleTransID-translate that
word,look up a unit,check a failed spawn or write an event-local destination.

The original BattleTransID DOL load at0x80125484 resolves selector-6 by reading
W+0x424. This establishes the previous lecture attack's GetUnitWork(-6,1,...)
as a read of the registered professor's work1 on this initialization path. It is
not an implicit current-partner selector or the same as owner-2. Invalid/missing
IDs and later replacement of W+0x424 remain caller/lifetime obligations.

### Complete lecture-mode setup body

_set_lecture_mode,file0x11930,size0x5c,captures W,then BattleTransID(event,-3) to
get Mario's unit ID and BattleGetUnitPtr(W,ID). Read Mario word+0x104,OR mask
0x40000000 and store it back. Store word1 at W+0x2740. Return2. It does not
validate the pointers,save the old fields or locally provide a matching restore.

The [direct damage application](BATTLE_DAMAGE_APPLICATION.md) has a proven
consumer of unit+0x104 mask0x40000000:skip its HP application block. Effects/
accounting before that block,FP afterward,audience and hit-event dispatch are
not all suppressed. Thus do not replace this flag with a universal no-damage/
no-reaction tutorial mode. The W+0x2740 consumers and teardown/restore owners
remain open. This body is absent from the checked Crump NPC init;sharing a visual
asset must not enable these lecture writes in the opening encounter.

## Attack hook and counter-data binding

Lecture Attack0x674b4 first runs lecture_zutsuki_attack_event0x67680 as a child.
After child completion,read professor work1 via selector-6. For exact3,write that
slot to4;then END_SWITCH,delete/end. Other values receive no mode write here.
This creates a mode3→4 handoff distinct from the counter event below.

The lecture kind's data table0x670d4 contains key0x24→counter script0x672c0,
followed by zero terminator. This is a data-event binding,not proof that every
attack invokes it;the dispatch predicate and complete counter chain still need
recovery. When that script is invoked,it requests pose type39,fall acceleration,
DivePosition,talk/stay pose PKR_D_1 and original message **mac_0_b_t2_015** with
owner-2 as speaker. It then requests position/floor/direction transformations,
JumpPosition counts20 and10,WAIT_FRAME30 and camera mode0. Finally:

- Read professor work1. If exact7,write work1=8.
- Otherwise write professor work3=7.
- Delete/end.

Those final writes are conditional on this counter script actually reaching its
tail,not automatic consequences of a B press. Detailed movement/audio/dispatch
composition remains open. [Coordinator modes](LECTURE_COORDINATOR_MODES.md)
connect these writes to the professor's later phase checks.

## Complete defense-result classification leaf

_get_defence_ac_result,file0x11700,size0x84,reads shared W+0x1cb0 as a signed
word,captures output operand0 raw and writes through evtSetValue:

| Stored defense result | Written classification |
| --- | --- |
| 1 | 2 |
| 2 | 3 |
| 4 | 1 |
| 5 | 0 |
| All other signed words,including3 | 0 |

It returns2 and does not recompute defense,reset history or look up a unit.
The 162-command lecture attack's post-damage switch at0x67c80 has an **empty**
case0 followed by case1 writing professor work3=4;case2 writes5;case3 writes6.
Original CASE_EQUAL does not fall through after matching,so class0 does not
execute case1's store. This preserves other counter-script bookkeeping rather
than overwriting it with a generic success value. The local branch rules are in
[integer VM branches](EVENT_INTEGER_BRANCHES.md).

## Implementation and evidence boundary

Keep distinct kind/state instances,proper setup indices,cross-linked IDs,selector
resolution,lecture-only writes and conditional counter/attack transitions.
Controlled acceptance cases should prove NPC attack only restarts wait;lecture
init binds professor ID before later selector-6 work accesses;class0 leaves the
post-damage work3 store path untouched;counter mode7→8 differs from attack3→4.
They cannot replace original timing or branch execution observations.

The [dedicated verifier](../../tools/verify_lecture_helper_roles.py) pins eight
complete data spans,three REL leaves,ten event spans/counts,23 local pointers,
ten leaf imports and concrete work writers/selector-6. The phase script's exact
US message bindings are checked separately in the same tool. Remaining work:
full lecture/player attack selection,counter dispatch,unit-work producers,
W+0x2740 lifecycle,field trigger/story and original inputs/resources/messages.
No original dialogue is paraphrased or invented;reuse msg/US payloads/control tokens.
