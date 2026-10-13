# BattleCommandInput: entry, re-entry, and class/weapon states

Complete static US G8ME01 revision0 `BattleCommandInput`801229d8/0x838.
This and [target selection](COMMAND_INPUT_TARGET_REMAP_AND_CONFIRMATION.md)
and [other states/attack dispatch](COMMAND_INPUT_AUXILIARY_STATES_AND_ATTACK_DISPATCH.md)
cover all30 state entries and the default. Native request:
[EVT-01](../../requests/EVT-01.md).
[Verifier](../../../tools/verify_command_input.py):one complete body/38 called
service identities,526 instructions,full30-target table,SDA startup,and original
cursor sound key. Callee bodies, event reachability, and original execution
remain separate. Constants are recovered instructions, not measured timing.

## Captured owners and entry order

Retain passed B. Load WORD(B+0x420), request BattleGetUnitPtr(B,actor), retain U.
Compute M=wrap32(B+0x171c); if this result is zero, return1 AFTER unit lookup.
Otherwise request `_btlcmd_GetCursorPtr(M,14,&SP08)`; ignore return. This
private stack word is shared by later cursor queries within this invocation.
Its initial class cursor uses the helper's current-global acting-unit lookup,
which is not necessarily retained U or passed B without a source invariant.

Request `_check_present_item()` without B. Nonzero additionally requests
BattleAcHelpSetDispType(0). Then request `_commandRestoreRec()`, also without
B. Set local progress P=1. Retain sound address802efe98 in two registers;
its original NUL-terminated key is `SFX_BTL_CURSOR_MOVE2`.
Proceed to the common progress test, so the first dispatch clears P to0.
No input handler is invoked before this entry sequence completes.

## Dispatch and two different re-entry paths

Normal dispatch entry80122a44 sets P=0, then loads current WORD(M+0) as state
S. Unsigned S>29 uses the default common tail; negative words also default.
Otherwise dispatch through the pinned table at80364074. Handlers use the
retained S when selecting paired setup flags; do not infer it from a later
state word changed by a service.

Common tail801231dc first reads BYTE(B+0x1c74). If nonzero, request
`_commandRestoreRec()`, then branch directly to80122a48: reload state and
dispatch WITHOUT clearing P. This also preserves SP08 and retained B/U/M.
There is no local retry limit. A persistent pending byte can keep this path
running even when P=0; a port must not silently insert a one-state yield.

When the pending byte is zero, test P at801231f0. Nonzero loops through
80122a44, which clears P before the next dispatch. Zero returns0. State29
bypasses both common checks and returns1. Wrapped-M-zero is the only other
explicit return1 path. There is no event completion2 return here.

## Class states0,1,2

State0 at80122a6c:if BYTE(B+0x1c74)!=0, read current type WORD(M+0x540).
Signed types11 or12 copy WORD(M+0x544) to M540; type13 does not. Then reread
the pending byte; exactly2 clears it. Always clear BYTE(M+0x551), OR bit1
(value0x00000001) into live WORD(B+0x0ef8), and request
BtlUnit_SetCommandAnimPose(retainedU). Read S8(U+0x20) afterward.

If signed value is5..9 inclusive, request in order BattleStageOnLightInCommand,
BattleStageOffLightFriendInCommand, BattleStageOffLightEnemyInCommand; then
store state5 and reach common tail with P unchanged0. Otherwise request
ActMenuSetup(B,0), store state2, then issue those same three light requests.
A state store alone does not set P1. Pending-byte re-entry can still proceed.

State1 at80122b1c requests ActMenuSetup(B,1), stores state2, common tail.
State2 at80122b34 requests ActMenuMain(B); nonzero sets P1, zero leaves it0.
Its [complete type writers](ACT_MENU_MAIN_SELECTION_AND_TYPE_PUBLICATION.md)
can publish a type/state while returning0. Preserve that return-dependent
progress policy rather than deriving P from whether any state changed.

## Shared weapon setup states5 and6

At80122b94 read current type T=WORD(M540). T0 plus fresh battle flag0x200,
or T1 plus fresh battle flag0x400, takes the immediate-target path:request
`_btlcmd_SelectWeaponDecide(B,T,0)`;request TargetSelectMenuSetup(B);store
state10;request BattleStageOnLightEnemyInCommand;common tail. None sets P1.
T and setup flag are actual arguments; do not reread type for the first call.

Otherwise select the WeaponSelectMenuSetup flag F:

- Retained state S==5:if retainedT==2, read U8(U+0x2f3);nonzero selects F0.
  If zero, read U8(U+0x2f4);nonzero selects F0. Both zero selects F1.
  Every other T selects F1 without those actor-byte reads.
- Retained S==6 selects F0, regardless of actor/type.

Request WeaponSelectMenuSetup(B,T,F), then OffLightFriend,OffLightEnemy;
store state7;request `_commandRestoreRec()`;then FALL THROUGH immediately
into the state7 handler in this same dispatch. Reload current M540 and request
WeaponSelectMenuMain(B,currentType). Nonzero sets P1. Direct state7 entry
starts at this final query. The already delivered [setup/decision contract](WEAPON_MENU_SETUP_AND_DECISION.md)
provides those complete local services;full WeaponSelectMenuMain is separate.
There is no general type-range guard at these Input setup call sites.

## State10 help and state11 shared cursor reuse

State10 at80122ce0 reads current type, requests GetCursorPtr(M,type,&SAME
SP08), then reads the resulting cursor index. Compute wrapped
M+0x80+28*index, read its weapon-pointer word, then read WORD(weapon+0x70).
Request BattleAcHelpSetHelp(thatWord), then BattleAcHelpSetDispType(1), then
store state11 and common tail. No null/index/type guard, no P1 assignment.

State11 performs another cursor query into SAME SP08 and consumes it later.
Unsupported types leave the output unchanged in the recovered helper. That
can reuse the initialized class cursor from this invocation if no intervening
write replaced it, or a subsequently selected supported weapon cursor.
It is not a default weapon cursor and does not initialize a different function's
private stack slot. [Target remapping and controls](COMMAND_INPUT_TARGET_REMAP_AND_CONFIRMATION.md)
complete this state's ordered reads and writes.
