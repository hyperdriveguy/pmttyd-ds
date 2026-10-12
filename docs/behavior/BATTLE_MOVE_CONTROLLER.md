# Complete local move-controller state map

US G8ME01 revision0, inspected 2026-10-12. B evidence is original instructions
of btlseqMove at0x8011bc24,size0x690, with15-entry dispatch table at0x80363c8c.
See [baseline](../ORIGINAL_BEHAVIOR.md), [phase tables](BATTLE_PHASE_TABLES.md),
[barriers/conclusion](BATTLE_WAITS_AND_CONCLUSION.md) and
[event dispatch](BATTLE_EVENT_DISPATCH.md). No original execution or Switch
comparison was performed. Original ELF/disassembly/annotations remain ignored.

This completes the routine's local branch/call/transition order. Callee bodies,
script content and outer controller scheduling are distinct dependencies.
The local btlseqAct map is now specified in
[action controller](BATTLE_ACTION_CONTROLLER.md); callees and outer scheduling
remain open.

## Dispatch, loop and state numbering (B)

B is the supplied battle object; S5=BattleGetSeq(B,5), S4=BattleGetSeq(B,4).
Each iteration resets local immediate=0 and computes unsigned(S5−0x05000000).
Indices0..14 dispatch; others exit. At the common tail0x8011c298, immediate≠0
branches back to read S5 again; otherwise return. No RNG is called directly in
this routine, though its services/scripts can consume it.

Full state values are0x05000000 plus the decimal index in the table. "Exit"
below means return to the outer caller without immediate redispatch. "Loop"
means set immediate=1 and dispatch the resulting state in this same invocation.
Explicit fallthrough from state2→3 and8→9 is separate from that loop.

| Index | US entry | Local work/transition |
| --- | --- | --- |
| 0 | 8011bc6c | btlseqPhaseFirstProcess(B); increment S5; loop |
| 1 | 8011bc88 | increment S5; loop |
| 2 | 8011bc9c | optional Majinai defence script; detailed branch below |
| 3 | 8011bd38 | wait barrier; if ready increment S5 and loop, else exit |
| 4 | 8011bd5c | launch selector1 phase events; increment S5; loop |
| 5 | 8011bda8 | wait barrier; if ready increment S5 and loop, else exit |
| 6 | 8011bdcc | choose one selector0 phase event, or go to10 |
| 7 | 8011be60 | wait/conclusion; if continuing set S5=6 and loop |
| 8 | 8011bec0 | clear action bookkeeping/select actor/set action state; fall through9 |
| 9 | 8011c120 | btlseqAct(B); exit using whatever state the callee established |
| 10 | 8011c12c | conclusion/phase-end decision; continue actors or start audience phase work |
| 11 | 8011c204 | wait for audience phase reaction and event barrier; advance to12; exit |
| 12 | 8011c230 | pump both status announcement services; when idle increment S4/reset S5; exit |
| 13 | 8011c26c | increment S5 to14; exit |
| 14 | 8011c27c | increment S4/reset S5=0; exit |

Table SHA256:
`9679486ada681e3679c69be1a2eb0cbed6b9478c33a88284755ad90ed7dd40a4`.
State13/14 are present even though this routine's ordinary local paths do not
explicitly set13; other writers/callees remain part of the caller census.

## State2: optional defence event before phase scripts (B)

Obtain pouchGetPtr(), then BattleGetMarioPtr(B). Read unsigned pouch byte+0x5ba.
Unless it is2 and S4 equals0x04000004, increment S5 twice (to4) and loop.
S4 is fetched only after the pouch byte passes its gate.

When both gates pass:

1. Write byte2 to B+0x18ff9.
2. Call BattleMajinaiDone(), then BattleMajinaiEndCheck(); ignore its return.
3. evtEntry(script0x803634f8,priority10,flags0), public symbol
   marioAttackEvent_MajinaiDefenceUp.
4. Store Mario's word unit+0 into event+0x160 and event+0x15c into Mario+0x2a8.
5. Increment S5 to3 and fall directly into state3's barrier in this invocation.

No allocation-null or missing-Mario guard exists before those writes. Helpers
and script meaning are not inferred from the Majinai name. State3, like state5,
only polls BattleWaitAllActiveEvtEnd and advances/loops when its return is nonzero;
it does not call BattleCheckConcluded itself.

## State4: initial phase-event fan-out (B)

Scan unit slots0..63 ascending. For each nonnull unit with script pointer+0x294
nonzero, BattleRunPhaseEvent(unit,1). No HP, count, group, phase membership or
status27 gate occurs in this scan. Continue across all slots, then increment
S5 to5 and loop into the barrier. Event dispatch's tracked-handle/owner behavior
applies. It is not one event followed by a wait for that unit before continuing.

## State6/7: ordered second phase events (B)

Scan all64 pairs in the phase-event table B+0x220, ascending table index.
Ignore pair ID−1, missing unit and unit script pointer+0x298=0. Stop at the first
qualifying entry and call BattleRunPhaseEvent(unit,0). Mark that pair's ID−1;
leave its second sort-key word unchanged. Increment S5 to7 and exit, without
immediate barrier polling. No compaction happens here; subsequent scans skip
holes left by earlier dispatches.

If no qualifying entry is found, set S5=0x0500000a and loop to state10.
Entries skipped for missing units/script remain unchanged. IDs are interpreted
through BattleGetUnitPtr as established by the table constructor.

In state7, barrier0 exits unchanged. When ready call BattleCheckConcluded(B).
Nonzero sets channel0=3 and channel7=0x07000000, then exits. Zero resets S5 to6
and loops, so another phase event may be selected immediately after a finished
one. Already marked entries are not replayed by this scan.

## State8: bookkeeping reset before each selected actor (B)

Clear12 bytes at B+0x18ffc. Then scan all64 unit slots; for every nonnull unit:

- Replace unit+0x27c with its bitwise AND0x80002000; preserve only those bits.
- Clear words+0x264,+0x268,+0x26c,+0x280.
- Clear signed-halfword storage+0x260 and bytes+0x270,+0x271,+0x272,+0x273.

These include hit counts, damage accumulators and sequence fields used by the
[damage contracts](BATTLE_DAMAGE_APPLICATION.md). Reset happens before actor
selection, even for units outside this phase. Do not clear nearby+0x284 merely
because it is another damage-related field; this loop does not write it.

Select an actor using the [exact actor-table scan](BATTLE_PHASE_TABLES.md):
positive signed remaining count, state≠10, status27 exception attribute,
state1 deferred normalization. Store selected ID at B+0x420, OR0x1000 into its
+0x27c, increment S5 to9 and set channel6=0x06000000.

The all64-skipped path falls into the same selected-ID store/lookup; it does
not contain an explicit no-actor branch. The resulting lookup is dereferenced.
Surrounding phase logic/data must establish the valid-actor invariant. Do not
invent a local safe fallback or describe this path as a normal "no moves" return.

Next scan the actor table again: for each existing entry/unit with unsigned
state+0x20=1, set state0. This clears deferred marks after selecting one actor.
It then re-fetches B+0x420 and performs the kind222 branch below, before falling
through into state9/btlseqAct in the same invocation. No immediate flag is
needed for this fallthrough.

## Kind222 command-availability flags in state8 (B)

If selected kind word+8≠222, skip this branch. Otherwise obtain pouchGetPtr()
and OR0x600 into B+0xef4. Independently clear its two bits as follows:

- Scan200 signed halfwords from pouch+0x38a; if any ID is240..246, clear0x200
  and stop that scan. Also clear0x200 if signed pouch byte+0x98≥2.
- Scan the same200 halfwords from the beginning; if any ID is247..253,
  clear0x400 and stop that scan. Also clear0x400 if signed pouch byte+0x99≥2.

No item database lookup is performed for these numeric ranges. They are not
all-owned badge arrays: the exact source+0x38a is authoritative. Display names,
weapon upgrade fields and flag consumers require binding evidence. Both scans
are independent; a first match in one does not skip the other.

## State10: after-action decision (B)

Call BattleCheckConcluded(B) first. Nonzero sets top-level channels0/7 as in
state7 and exits without cleanup below.

Otherwise scan all64 unit slots. For each nonnull unit call
BtlUnit_SetTotalHitDamage(unit,0), then clear mask0x1000 from unit+0x27c.
BtlUnit_SetTotalHitDamage at0x80127434,size0x0c sign-extends its amount
from16 bits and stores that signed word at unit+0x264; no other local writes.
Then call BattlePhaseEndCheck(), using its global battle state.
If B+0xef4 has0x2000, force that result to1.

When the phase-end result is nonzero:

1. If byte B+0x18ff9=2, clear it0; other values remain.
2. Fetch S4 and pass it to BattleAudience_PerPhase(S4).
3. Increment S5 to11, then exit.

When result0, set S5=8 and exit. Actor selection/bookkeeping reset therefore
occurs on the next outer invocation, rather than looping immediately from this
state. Phase end is based on the separately documented actor predicate, not
this routine's own positive-HP/allegiance census.

## State11/12: audience and announcement drain (B)

State11 calls BattleAudience_CheckReactionPerPhase. If its low byte≠0, exit
without polling the barrier. Otherwise poll BattleWaitAllActiveEvtEnd(B);
if ready increment S5 to12, then exit, without immediate announcement pumping.

State12 calls BattleStatusChangeMsgMain(B), saves its word result, then always
calls BattleStatusChangeAnnouceMain(B), even if the first was nonzero.
OR the two results; nonzero exits with state unchanged. When both are0,
increment S4 and reset S5=0x05000000, then exit. This local state does not
add another event barrier after announcements become idle. Queue/UI/callee
semantics and outer phase/turn timing remain separate contracts.

## Verification obligations and remaining dependencies (P)

Compare initial state0 immediate chain; optional state2 allocation/owner;
selector1 fan-out versus selector0 serial dispatch; table holes retained;
finished event returning into state6; conclusion before cleanup; state8 field
reset preserving exactly0x80002000; state1 deferred actors; kind222 independent
command-flag scans; state10 returning to8 on a later outer call; byte-only
audience result; both announcement pumps called while the first is active.
These are derived comparison cases, not original execution observations.

Remaining: full action-state map, Majinai helpers/script, command/menu and attack
services, audience/announcement internals, flag/item bindings, all external
channel writers, outer invocation cadence and victory/defeat/reward execution.
Local move-controller coverage is complete; full battle/game behavior remains
unfinished and requires original/native execution comparisons.


[Complete Done/EndCheck leaves](JUMP_MAJINAI_POWER_CHECK_CHILD.md) now supply
byte decrement/reread/unconditional5b9/5ba reset and captured battle flag OR
only for count0. [Effect/name services](JUMP_MAJINAI_EFFECT_AND_NESTED_SOURCES.md)
close the shared effect wrapper;defence-script/fortune producer and full source/
backend/runtime domains remain separate.
