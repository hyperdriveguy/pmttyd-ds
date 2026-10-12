# BattleCommandInput: remaining states, event waits, and final attack dispatch

This completes all30 states of the static US G8ME01 Input body801229d8/0x838,
with [entry/class controls](COMMAND_INPUT_ENTRY_REENTRY_AND_CLASS_STATES.md)
and [target state11](COMMAND_INPUT_TARGET_REMAP_AND_CONFIRMATION.md).
[Verifier](../../../tools/verify_command_input.py) pins all526 instructions,
39 hashes(one complete/38 linked),and three source spans. No original runtime
trace or global event/query reachability is claimed. All helpers below are
service requests whose return affects only the explicitly stated local control.

## Complete state entry map

| State | Original table target | Handler |
| --- | --- | --- |
| 0 | 80122a6c | first class setup / actor-mode bypass |
| 1 | 80122b1c | resumed class setup |
| 2 | 80122b34 | class Main |
| 3 | 80122b4c | reason-event creation, then state4 wait code |
| 4 | 80122b6c | reason-event wait |
| 5,6 | 80122b94 | shared weapon setup with distinct F policy |
| 7 | 80122c4c | weapon Main |
| 8 | 80122c68 | reason-event creation, then state9 wait code |
| 9 | 80122c88 | reason-event wait |
| 10 | 80122ce0 | help setup / transition11 |
| 11 | 80122d20 | pair remap / target navigation and confirmation |
| 12 | 80122cb0 | multi-item cancel-check setup / transition13 |
| 13 | 80122cc8 | multi-item cancel-check Main |
| 14,15 | 80122fd8 | multi-item setup, then state16 Main code |
| 16 | 80123008 | multi-item Main |
| 17 | 80123020 | reason-event creation, then state18 wait code |
| 18 | 80123040 | reason-event wait |
| 19,20 | 80123068 | operation setup, then state21 Main code |
| 21 | 80123098 | operation Main |
| 22 | 801230b0 | reason-event creation, then state23 wait code |
| 23 | 801230d0 | reason-event wait |
| 24,25 | 801230f8 | party-selection setup, then state26 Main code |
| 26 | 80123128 | party-selection Main |
| 27 | 80123140 | protect-partner setup, then state28 controls |
| 28 | 80123154 | protect-partner controls |
| 29 | 801231a4 | attack-event selector / flag clear / return1 |
| unsigned >29 | 801231dc | common pending/progress tail |

The table governs Input STATE at M00, not menu TYPE at M540. A type9 or13
class writer selecting state29 does not enter states9 or13. Retain this distinction
when implementing switch tables or interpreting trace logs.

## Four reason-event create/wait pairs

Pairs3/4,8/9,17/18,22/23 have the same instruction pattern with different
state destinations:

| Creation / wait state | State stored before event creation | State after completed wait |
| --- | --- | --- |
| 3 / 4 | 4 | 1 |
| 8 / 9 | 9 | 6 |
| 17 / 18 | 18 | 15 |
| 22 / 23 | 23 | 20 |

For creation:store the wait state, request
`evtEntry(0x80416dc4,0,32)`, read WORD(returnedEntry+0x15c), and store it
at WORD(M+0x568). The script address is `r13-24924`, with r13 startup base
0x8041cf20 pinned;it is an address argument, not a load of a pointer at that
address. There is no returned-entry null guard or explicit unit-context write
in this caller. Source script contents/parent scheduling are separate.

Creation then falls directly into its paired wait code;direct wait-state entry
starts there without creating an event. Read current WORD(M568), request
evtCheckID(id). Nonzero goes to common tail with P unchanged. Zero stores
WORD(M568)=0, sets P=1, stores the completed-wait destination, then common
tail. The query checks the current stored ID, not a retained constructor return.
All four exact constructor/wait sites are pinned as part of the complete body.

## Multi-item cancel checks and selector menus

State12 stores state13 BEFORE requesting
WeaponMultiItemCancelCheckSetup(B), then `_commandRestoreRec()`, then common
tail. There is no fall-through to state13 Main and no P1 assignment.
State13 requests WeaponMultiItemCancelCheckMain(B);nonzero sets P1.

For the paired setup states below, retained dispatch state S selects F;service
changes to M00 do not change that argument:

| Dispatch state(s) | Setup service / F | State written afterward | Immediately executed Main |
| --- | --- | --- | --- |
| 14,15 | MultiItemMenuSetup(B, S==14 ?1:0) | 16 | MultiItemMenuMain(B) |
| 19,20 | OperationMenuSetup(B, S==19 ?1:0) | 21 | OperationMenuMain(B) |
| 24,25 | ChangePartySelectMenuSetup(B, S==24 ?1:0) | 26 | ChangePartySelectMenuMain(B) |

Between the state store and immediate Main request, call
`_commandRestoreRec()`. A direct state16/21/26 entry requests only the
corresponding Main. Nonzero Main return sets P1;zero leaves P unchanged.
All variants reach the common pending/progress tail. The helper may itself
write type/state/cursor bytes;this caller contract does not invent their bodies
or an invariant on their returned menu types.

## Protect-partner states27/28

State27 requests ProtectPartnerSelectMenuSetup(B), stores state28,
requests `_commandRestoreRec()`, then falls directly into state28 controls.
Direct state28 skips setup. Request Trigger(0x0200) first. Nonzero requests
sound2521, ProtectPartnerSelectMenuEnd(B), then stores state20 and reaches
common tail. It skips the confirmation query.

Otherwise request Trigger(0x0100). Nonzero stores state29 BEFORE
ProtectPartnerSelectMenuEnd(B). Zero makes no local write. Both go to common
tail without setting P1. No confirm sound or help request is made on this
state's confirm path;do not import state11's richer confirmation sequence.

## State29 final dispatch and completion ownership

At801231a4, request `_btlcmd_SetAttackEvent(retainedU,retainedM)`.
Ignore its return. Then:

1. Read CURRENT WORD(M+4), clear only bit0x01000000, store it.
2. Read CURRENT WORD(B+0x0ef8), clear only bit0x00000001, store it.
3. Request BattleStageOffLightInCommand(0,0).
4. Return1 directly, bypassing the pending/progress tail.

There is no local state30 write, no type reset, no extra selected-weapon query,
and no returned-event null test. The current menu type is interpreted by the
already delivered [attack-event selector](BATTLE_ATTACK_EVENT_SELECTION.md),
which routes0/1/2/4/6 through weapon records and13 through LectureEnd;9 uses
ChangePosition. It can read current global owners inside its own services,
while this caller passes the retained entry actor/menu. Do not replace retained
U with a new registry lookup merely because type/state changed during re-entry.
The two flag clears read AFTER selector return;preserve unrelated live bits.

## Native requirements and unresolved domains

Implement all30 state targets, unsigned default behavior, retained dispatch
S/B/U/M, same-slot cursor ownership, fall-through versus common-tail timing,
constructor ID publication, current-ID waits, explicit P1 assignments, and
state29's direct result/flag ownership. Several state stores leave P0 and
therefore wait for another invocation when no pending byte forces re-entry.
P may survive the separate pending re-entry path. Do not impose one-state-per-
frame semantics or convert local0/1 into VM handler completion codes.

The local Input request is now fully documented. This narrows the native
weapon-query question to callee/source contracts:WeaponSelectMenuMain,
MultiItem/Operation/ChangeParty/ProtectPartner bodies,full gift/audience/
confusion writers,weapon-table and target filtering,attack-event source graphs,
and all selected-weapon query callback domains. Known type9/13→state29
routes are evidence of separate attack dispatch,not proof that no downstream
event ever invokes a weapon query with an unsupported type. Original execution,
platform services,and full-game requirements remain open.
