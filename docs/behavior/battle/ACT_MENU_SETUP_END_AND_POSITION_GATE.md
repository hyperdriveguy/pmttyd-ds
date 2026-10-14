# Act-menu setup/end and position-change admission

This closes complete local bodies for `BattleCommandDisplay_ActMenuSetup`
80122794/0xcc, `BattleCommandDisplay_ActMenuEnd`801220d4/0x38,
`BattleCommandCheckChangePositionEnable`8012214c/0x1c0,
`_cursor_init`80122860/0x10, and no-op
`BattleMenuDisp_ActSelect_Main`80116cd8/4. Together with
[ActMenuMain](ACT_MENU_MAIN_SELECTION_AND_TYPE_PUBLICATION.md), these are the
six complete contracts checked by [the verifier](../../../tools/verify_act_menu_control.py).
Static US G8ME01 revision0; no observed original execution. Linked services
are pinned identities, not blanket recovery of their lifecycle or semantics.

## Setup preserves class cursor ownership and selected rows

Arguments B and F are retained; M=wrap32(B+0x171c). In order:

1. Request `_btlcmd_MakeActClassTable(B)` (complete
   [producer](ACTION_CLASS_TABLE_ADMISSION_AND_PUBLICATION.md)); ignore return.
2. Request `_btlcmd_GetCursorPtr(M,14,&privateSP08)`; ignore return.
3. Load returned cursor word and request
   `BattleMenuDisp_ActSelect_Init(B,cursor,M+8,F)`.
4. Store WORD(M+0x560)=0.
5. Read live WORD(M+4), OR0x01000001, then store it back.
6. If retainedF==0 AND a fresh BYTE(M+0x558)==0, issue the nine cursor-init
   requests below. Otherwise skip them all.

The cursor getter uses current-global battle/actor ownership, while the builder
used passed B. Do not assume their chosen cursor must agree without a source
invariant. No explicit type(M540), input state(M00), previous type(M544),
selected row, pending-confirmation byte, or M550 write occurs in this setup.
UI Init can mutate memory before the flag read and +558 test.

Cursor-init requests pass these retained menu-relative addresses, in order:
`M+0x4b0`, `+0x4bc`, `+0x4c8`, `+0x4d4`, `+0x4e0`, `+0x4ec`, `+0x4f8`,
`+0x504`, `+0x51c`. Each `_cursor_init(C)` stores WORD(C)=0, then
WORD(C+4)=0. It preserves count C+8 and all following bytes. There is no
request for `M+0x510`, nor for the class cursors at +498/+4a4. Do not replace
this list with a contiguous bulk clear. `_cursor_init` leaves its address
argument in r3, without an explicit status assignment. Setup itself has no
explicit success return: its r3 is inherited from UI Init when resets are
skipped, or the final cursor-init address when they run.

## End: UI request before live flag clear

ActMenuEnd retains B, requests BattleMenuDisp_ActSelect_End(B) at801220e8,
then reads WORD(B+0x1720) (M+4), clears only bit0 with mask0xfffffffe,
and writes it back. OR0x01000000 and all other live bits are preserved.
There is no explicit type/state/timer/cursor reset and no explicit result
assignment. The flag read is after UI End; preloading it changes shared-memory
behavior. Original UI End80116c78/0x40 is linked, not the no-op UI Main.

## Position-change gate: ordered Mario then Party checks

Input B is retained. Read flags WORD(B+0x0ef4) once. If masked by
0x00800000, return0 immediately; otherwise if the SAME word is masked by
0x40000000, return0. No actor getter is requested on either failure.

Request BattleGetMarioPtr(B) at80122184 and retain returned U. Zero returns0.
For Mario, perform the following checks in order, stopping at the first failure:

1. S8(U+0x20) must equal0.
2. S8(U+0x23) must be >0. Only then read S8(U+0x22), which must equal
   the retained +23 value. Do not collapse this into unsigned equality.
3. Request BtlUnit_CheckStatus(U,1), then (U,2), then (U,9), then (U,27).
   Any nonzero return immediately fails; later requests are omitted.
4. Read fresh WORD(U+0x104); mask0x00000010 must be clear.

Then request BattleGetPartyPtr(B) at80122240 and retain this different U.
Zero fails. Perform the SAME ordered +20/+23/+22 checks, followed by the
same short-circuit status request sequence1,2,9,27. Do not apply Mario's
+104 restriction to Party; this body never reads it in the Party phase.
After all Party checks pass, read S8(U+0x12b); return1 iff zero, otherwise0.
The final cntlzw/rotate implements this exact zero predicate, not a sign test.

Mario's first status call passes the getter result still in r3; Party's first
call likewise uses its getter result. Subsequent status calls restore retained
U explicitly. Both getters use retained B, while status services can change
unit fields before the later reads. No current-global actor replacement,
acting-kind filter, FP-cost check, partner substitute, or native event result2
is introduced in this gate.

Mario/Party getter contracts are already linked in
[first-attack getters](FIRST_ATTACK_PAIR_SELECTION_AND_GETTERS.md). Status
query identity80127814/0x7c is pinned here; its complete status-field meaning
is a separate service contract. Numeric status IDs are literal request values;
do not attach guessed names or invent status messages.

## Port and source boundaries

Implement these as ordered requests and partial stores over original packed
addresses. Reuse original UI/keys/assets through their documented services.
Neither setup nor gate proves class selection/query-event reachability.
[Menu type/cursor source evidence](WEAPON_MENU_TYPE_DOMAINS_AND_CURSOR_REUSE.md)
and complete downstream Input/event routing are still needed before asserting
that every selected-weapon query has an initialized cursor output. The wider
native Game binding and original platform execution remain open.

## Portable implementation (2026-10-14)

[Shared act-menu control](../../../source/battle/battle_act_menu.cpp) implements
complete Setup/End/Main/position gate/cursor-init bodies against original packed
addresses and a [typed original request interface](../../../include/battle/battle_act_menu.hpp).
UI Init/End, class-table/weapon production, source cursor getters and other
callees remain provider requests; readiness failure is optional/nullopt after
preceding effects, not a source0 return or suspended stack. The known original
Display Main identity remains its documented blr service boundary. Source
Main returns local0/1 independently of any display/helper scalar result.

Setup retains F, requests builder/cursor/UI in order, then reads current flags
and M558 before the exact nine resets. Cursor counts and excluded498/4a4/510
are preserved. End clears only bit1(value1) after its UI provider. Position
checks captured flags, retained Mario then Party, signed move bytes and ordered
short-circuit status1/2/9/27; the Mario-only104 restriction is not added to Party.
Main preserves live class-count validation, retained row/type, reason precedence,
wrapped signed timer, post-camera pending gates, fresh selected row after sound,
all14 type routes/default, type2 zero/nonzero result policy, source reason reads
after End, navigation priority and post-sound cursor reloads, audience ordering
and the fresh repeated position query. No touch-only input, renamed source key,
clamp, generated sound or replacement menu style is introduced.

[Host fixtures](../../../tests/battle_act_menu_tests.cpp) verify these owner/order
and boundary cases including UI/sound mutations, zero-count navigation, timer
wrap, disabled reason preservation and different answers to repeated position
gates. The unchanged read-only verifier pins24 bodies/3 spans/472 instructions
for six complete source bodies and18 linked callee identities. No original
runtime observation or rendered class menu is claimed. Native class-table and
UI constructors, provider registration into Harbor Input, Game first-act/turn
scheduling and original menu rendering remain required before normal playback.
