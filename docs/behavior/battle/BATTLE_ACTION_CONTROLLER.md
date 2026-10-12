# Complete local action-controller state map

US G8ME01 revision0, recovered 2026-10-12 from btlseqAct at0x8011b2dc,size0x8ac.
B evidence is original instructions/data. See [baseline](../../ORIGINAL_BEHAVIOR.md),
[move controller](BATTLE_MOVE_CONTROLLER.md), [count consumption](BATTLE_PHASE_TABLES.md)
and [barriers/conclusion](BATTLE_WAITS_AND_CONCLUSION.md). Original disassembly
and annotations remain ignored. No original execution or Switch equivalence
was measured. This completes local control-flow coverage, with callee/event
bodies and outer invocation timing still separate dependencies.

## Dispatch and fallthrough (B)

B is supplied battle state; actor ID=B+0x420. Channel6−0x06000000 is treated as
unsigned;0..14 dispatch through15 words at0x80363c50, others exit unchanged.
No redispatch loop exists in this routine. Several cases explicitly fall through
after advancing; other transitions return until a later outer invocation.
The action dispatch-table SHA256 is
`3fb24c458339c286f9e7ccbaab490364975128789b059f35458486dad6e5e977`.

| Index (decimal) | US entry | Local purpose |
| --- | --- | --- |
| 0 | 8011b31c | actor initialization and ordered routing |
| 1 | 8011b4d8 | camera-speed gate, advance and fall through2 |
| 2 | 8011b4fc | command input; when ready select6, then exit |
| 3 | 8011b56c | flags for inhibited actor; select6, exit |
| 4 | 8011b5a4 | data key3 and flags27; select6, exit |
| 5 | 8011b530 | data key2 and flags1; select6, exit |
| 6 | 8011b5e0 | ordered script/status selection and event creation; advance7, exit |
| 7 | 8011b7dc | event barrier; advance8, adjust messages and fall through8 |
| 8 | 8011b800 | barrier and both announcement pumps; advance9 and fall through9 |
| 9 | 8011b838 | post-action fields/counts/reservations; advance10, fall through10 |
| 10 | 8011ba3c | audience/Break Slot reaction; advance11, fall through11 when ready |
| 11 | 8011ba70 | both announcement pumps; advance12, fall through12 when idle |
| 12 | 8011ba98 | barrier then optional actor post-event; advance13, fall through13 |
| 13 | 8011bb18 | poll post-event; advance14, fall through14 when finished |
| 14 | 8011bb58 | barrier; when ready increment move channel5, then exit |

Full state values are0x06000000 plus the index. A later state can run in the
same call only via the listed fallthrough, not simply because a prior case set
its channel value. Do not impose a blanket one-render-frame delay per state or
make every transition execute all following states immediately.

## State0: preparation and routing (B)

Resolve actor U from B+0x420; valid actor is assumed. Call effNiceAsync(1).
Clear U+0x284 and U+0x2d0 words, then BattleAudience_ActInit().
Start local inhibited=1. For kind222 or224..230, test statuses1,2,9,27 in order,
short-circuiting on nonzero. Only when all four are absent set inhibited=0.
Other kinds retain1 without these tests. This is not CanActStatus: no status
flag+0x138 test occurs here.

Route in the following priority order, selecting a state and exiting:

1. inhibited≠0: state3.
2. U attributes+0x104 have0x10: state4.
3. U kind228 and Mario attributes+0x104 have0x10: state4. Mario is fetched
   only for kind228; no local missing-Mario fallback.
4. Signed byte U+0x12b≠0: state5.
5. Otherwise BattleCommandInit(B), then state1.

The kind228 branch checks Mario's attribute, not U+0x228 used by the separate
shadow-guard predicate. These predicates serve different call sites.
Command initialization's menu/actor setup remains a callee contract.

## States1/2: camera and command input (B)

State1 calls battleCameraGetPosMoveSpeed and compares its float return against
binary32 constant2.0 at0x80422584. If strictly greater, exit unchanged.
Otherwise set state2 and immediately run state2. Unordered float behavior follows
the original comparison/branch rather than an invented finite-value guard.

State2 calls BattleCommandInput(B). Return0 exits unchanged. Nonzero clears
mask0x80 from B+0xef4, sets state6 and exits. This contract specifies the caller
protocol, not the command menu's input rules or what the callback's outputs mean.

## States3/4/5: alternate actor setup (B)

Each fetches U by actor ID and assumes it exists:

| State | Ordered mutations | Next |
| --- | --- | --- |
| 3 | U+0x2ac=1; OR0x04000000 into word U+0x1c | state6; exit |
| 4 | U+0x2a0=GetData(U,3); U+0x2ac=27 | state6; exit |
| 5 | U+0x2a0=GetData(U,2); U+0x2ac=1 | state6; exit |

State3 does not overwrite U+0x2a0 here. Data values may be null; the later
state6 routes determine whether a null pointer is checked before event creation.
The byte/word masks are numeric contracts, not yet friendly action names.

## State6: ordered event selection (B)

First fetch U. If U+0x2ac lacks1, call BattleStatusWindowSystemOff(). Fetch U
again afterward, initialize local event pointer E=null, then select the first
matching route below. Event creation is evtEntry(script,10,0).

1. If B+0xef4 has0x40000000, U attributes lack0x2000, and U+0x2ac lacks both
   0x08 and0x10, create script0x8034f234, annotated btldefaultevt_CantMoveZeroGravity.
   Skip all later status checks/draws.
2. Else if U+0x2ac has0x10: use U+0x2a0 only if nonnull; otherwise leave E=null.
3. Else if U+0x2ac has2: create from U+0x2a0 without a local null-script check.
4. Otherwise use the ordinary status/script path below.

Ordinary path short-circuits status1,2,9, then27. Any of1/2/9 active suppresses
creation. Status27 suppresses unless U attributes have0x10000. No HP or count
check is added here. For a unit passing those gates:

- If status6 Electric is active, draw irand(100); only signed negative result
  creates script0x8034e0cc (btldefaultevt_BiribiriMove), then skips later paths.
  Under the [recovered RNG](../meta/RANDOMNESS.md), this result is0..99, so creation
  through this comparison is unreachable. Its qualifying draw still occurs.
- If no Electric event was selected, check status5 Confuse. If active, draw
  irand(100); result<50 ORs0x10 into U+0x27c and creates U+0x2a4 event.
  Otherwise create U+0x2a0 event. Both are passed without a local null-script
  guard. Confuse's threshold is not a claim of measured exact statistical rate.
- If Confuse is absent, create U+0x2a0 event.

When E is nonnull, store E+0x15c at U+0x2a8 and U+0 at E+0x160. Then if wait
ID U+0x290 is nonzero, delete it via evtDeleteID and clear it. If E is null,
these binding/wait-deletion operations are skipped. In every case increment
channel6 to7 and exit, including suppressed/failed event creation. The action
can consequently proceed toward count consumption with no created script.
No whole-function success boolean is computed here.

## State7/8: event and announcement barriers (B)

State7 polls BattleWaitAllActiveEvtEnd(B). If0, exit. Otherwise increment to8,
call BattleStatusChangeMsgAdjust(B), and fall through into state8.
State8 polls the barrier again;0 exits. Otherwise always call both
BattleStatusChangeMsgMain(B) and BattleStatusChangeAnnouceMain(B), OR their word
results, and exit while either nonzero. Both0 advances to9 and falls through.
The second barrier and both announcement calls are intentional separate sites.
Announcement queues/rendering and Adjust's internals remain dependencies.

## State9: actor cleanup and consumption (B)

If B+0xef4 lacks both0x10/0x20, call BattleStatusWindowSystemOn(). Fetch U by
actor ID. Missing U skips to the common reservation/audience tail below.
For an existing actor:

1. Clear0x04000000 from U+0x1c.
2. If U+0x27c has8, SetStatus(U,16,duration1,strength−99), then clear that bit.
   Setter narrowing/clamping/status effects follow the storage contract.
3. If byte B+0x18ff9=1, clear it; leave other values unchanged.
4. Clear word U+0x25c.
5. Apply the complete [state normalization/count consumption](BATTLE_PHASE_TABLES.md)
   region:5→6,6→7,7→0,8→9,9→0; state0 count-decrement/bypass/exhaustion;
   states3/4 clear to0 without decrement. Preserve its+24 reset/preserve gate,
   always-true local r3 test and optional byte+0x311=96 write.

Common tail, whether actor exists or not: BattleConsumeReserveItem(), clear
mask4 from B+0xef4, BattleAudience_PerAct(), increment channel6 to10 and fall
through10. Item removal is after actor count changes and before audience reaction
polling. It is a second established consumption site beyond the reaction queue.

## State10/11: reactions and another announcement drain (B)

State10 calls BattleAudience_CheckReaction; nonzero low byte exits. Otherwise
call BattleCheckConcluded(B). If0, call BattleBreakSlot_CheckReaction; nonzero
low byte exits. A concluded battle skips that Break Slot call but still advances
to11; it does not directly set top-level conclusion channels in this state.
Then increment channel6 and fall through11.

State11 always calls both MsgMain and AnnouceMain, ORing results; nonzero exits.
Both0 increments to12 and falls through. This is a later drain in addition to
state8, not an alternative implementation of the same scheduling barrier.

## State12/13/14: optional post-action event and final handoff (B)

State12 first polls the battle barrier;0 exits. Fetch U. If missing, skip event
setup. Otherwise clear tracked post-event ID U+0x2cc. If script U+0x2c8 is null,
skip creation. For a nonnull script request evtEntry(script,10,0); when nonnull,
store its ID at U+0x2cc, bind owner to E+0x160, then delete/clear U+0x290 wait
ID if present. Allocation failure skips binding and wait deletion. Clearing a
previous+0x2cc here does not itself delete that previous event.

Then increment to13 and fall through. State13 fetches U again. If U exists and
U+0x2cc is nonzero, evtCheckID: live exits unchanged; finished clears the field.
Missing unit/zero ID proceeds. Increment to14 and fall through.
State14 polls BattleWaitAllActiveEvtEnd(B); if ready increment **move channel5**
(to its post-action state), then exit. It does not reset action channel6 or
check conclusion directly. The explicit+0x2cc poll is distinct from unit barrier
fields+29c/+2a8/+2b4/+2bc, which omit+2cc.

## Verification obligations and remaining scope (P)

Compare each state entered directly versus reached by fallthrough, duplicate
barrier calls, camera threshold equality, inhibited kinds/status order, kind228
Mario-attribute gate, priority of special event flags, Electric retained draw,
Confuse draw/script/bit writes, no-created-event count consumption, failed
allocation leaving wait alive, both announcement drains, conclusion skipping
Break Slot, absent actor still consuming reservations, and explicit post-event
polling before final move handoff. These are derived cases, not execution traces.

Remaining: command/menu and camera helpers, script and data-key bodies, status-
window behavior, audience/announcement/Break Slot internals, action-flag writers,
outer cadence, rewards/conclusion and death/revival execution. The local action
map is complete; complete battle and full-game behavior remain unfinished.

Electric/zero-gravity scripts and command-init/camera-speed helpers are now
specified in [default action events](BATTLE_DEFAULT_ACTION_EVENTS.md), with
remaining service and numerical dependencies explicit.
