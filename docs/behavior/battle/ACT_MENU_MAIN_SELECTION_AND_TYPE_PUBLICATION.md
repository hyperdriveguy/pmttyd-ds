# ActMenuMain: admission, class validation, and type publication

Complete static US G8ME01 disc0 revision0 body:
`BattleCommandDisplay_ActMenuMain` 8012230c/0x488. This answers the native
menu-writer portion of [EVT-01](../../requests/EVT-01.md).
[Navigation/tail](ACT_MENU_MAIN_NAVIGATION_AND_SERVICE_ORDER.md) completes the
same body; [setup/end/position gate](ACT_MENU_SETUP_END_AND_POSITION_GATE.md)
closes five additional local bodies. The
[verifier](../../../tools/verify_act_menu_control.py) pins six complete bodies,
18 linked service identities, all472 local instructions, the full14-entry
switch table, and two original sound keys. Evidence is disassembly only,
not execution, global source reachability, or native Game integration.

## Entry and priority position-change route

Retain passed battle address B; retain M=wrap32(B+0x171c). Load
WORD(B+0x420), request BattleGetUnitPtr(B,actor), retain U. Initialize local
result R=0. Request `_btlcmd_GetCursorPtr(M,14,&privateSP08)`; ignore return.
This is a fresh local stack output, not a previous weapon-query stack slot.
The helper's current-global actor lookup can select a class cursor independently
of retained U. No local actor/cursor/index/count bounds guard is added here.

At80122348, request BattlePadCheckTrigger(0x0800). If nonzero, request
BattleCommandCheckChangePositionEnable(B). If that is also nonzero:

1. Request psndSFXOn(2520).
2. Request ActMenuEnd(B).
3. Store WORD(M+0x540)=9, then WORD(M+0)=29.
4. Request BattleMenuDisp_ActSelect_Main(B), then return1.

This route skips class validation, idle timer, confirmation, and navigation.
If either gate fails, request BattleMenuKeyOKInACT(B) at80122764. Nonzero
admits the ordinary class processing below. Zero goes to the common display
request and returns0. Do not assume that the second gate is interchangeable
with the original trigger result; it is a distinct service call.

## Live class validation scan

Set local ordinal J=0 and byte offset O=0. At every loop test, reload cursor
address from SP08 and read its current signed count WORD(C+8). While
signed J<count, retain row Q=wrap32(M+O), then read T=WORD(Q+8).
Only T0 and T1 can enter the weapon-target check:

- T0 requires a fresh WORD(B+0x0ef4) masked by0x00000200 to be nonzero.
- T1 requires a fresh WORD(B+0x0ef4) masked by0x00000400 to be nonzero.
- All other types, including negative words, skip this check.

For an admitted T, request `_btlcmd_MakeSelectWeaponTable(B,T)`, then read
WORD(M+0x80) and request `_btlcmd_CheckWeaponTargetNum(B,retainedU,thatWord)`.
If the signed return is <=0, store WORD(Q+0x0c)=0. Read CURRENT
WORD(Q+0x18); only when it is zero, store reason4 for retainedT0 or reason5
for retainedT1. A preexisting reason is preserved. The check never reenables
a row. Row Q and T remain retained across both services; next-loop count is
fresh. Increment J by1 and O by20, with original word arithmetic.

The target helper and weapon-table builder are linked identities in this slice;
their complete filtering/source/alias protocols remain separate. This is not
proof that all records or targets are reachable in actual play.

## Timer and pending-confirmation suppression

Request BattlePadCheckNow(-1). Nonzero clears WORD(M+0x560), then requests
btl_camera_set_mode(0,0). Zero increments the live WORD(M+0x560), stores
wrapped result, then compares the incremented value signed against900.
When >=900, request btl_camera_set_mode(0,6), then
btl_camera_set_moveSpeedLv(0,5). This repeats on subsequent qualifying calls;
there is no equality-only threshold or saturation. Clock units are function
invocations in this body, not a measured wall duration or rendering frame rate.

If BYTE(B+0x1c74)!=0, read current cursor index, derive current row, and read
its type. T0 with battle mask0x00000200, or T1 with mask0x00000400, clears
BYTE(B+0x1c74) first and WORD(B+0x1c70) second. Other types/flag combinations
preserve both. The reads are fresh after camera services; this is independent
of the earlier class-validation loop.

## Confirm and enabled-row type copy

Request BattlePadCheckTrigger(0x0100). Nonzero enters confirmation directly;
zero still confirms when a fresh BYTE(B+0x1c74)!=0. Otherwise navigate.
Read the current cursor/index and row enabled word. Disabled behavior is in
the companion navigation/tail contract. For an enabled row:

1. Request psndSFXOn(2520).
2. Reload cursor/index AFTER sound, derive the row, read its current type.
3. Store that type to WORD(M+0x540) at8012254c.
4. Reload WORD(M+0x540) and dispatch it unsigned through the table below.
5. Request ActMenuEnd(B); then common display and return local R.

No type-range validation precedes the store. Negative or >13 type words
skip switch actions but still reach ActMenuEnd and return0. Enabled and type
can be read from different rows if the sound service changes the cursor.

| Published type | Original target | Local state writes / result |
| --- | --- | --- |
| 0,1,4,6,7,8 | 80122574 | WORD(M+0)=5; R=1 |
| 2 | 80122584 | WORD(M+0)=5; R stays0 initially; special checks below |
| 5 | 801225c8 | WORD(M+0)=19; R=1 |
| 13 | 801225d8 | WORD(M+0)=29; R=1 |
| 3,9,10,11,12 | 801225e4 | no switch write; R=0 |
| unsigned >13 | default801225e4 | no switch write; R=0 |

For type2, after state5, read S8(U+0x20). Nonzero ends the case with R0.
Zero stores BYTE(M+0x550)=1, then reads U8(U+0x2f3). Nonzero skips the
next byte read and selects state14/R1. Otherwise read U8(U+0x2f4): nonzero
selects state14/R1; zero leaves state5/R0. U is the retained entry actor,
not a fresh registry lookup. Cases which do not store BYTE(M+0x550) preserve
its previous value. Do not synthesize a reset or status predicate here.

## Reachability boundary for native weapon queries

Combined with [class production](ACTION_CLASS_TABLE_ADMISSION_AND_PUBLICATION.md),
this provides a concrete type13 producer and enabled-row publication path.
Two position paths in this body publish type9/state29. Both types leave
`_btlcmd_GetCursorPtr` weapon-query output untouched, while the
[attack-event selector](BATTLE_ATTACK_EVENT_SELECTION.md) has separate
ChangePosition/LectureEnd routes. These stores alone do not prove that a
selected-weapon query is invoked in those routes. Complete Input state
progression, event/callback sources, confusion, gifts, and query-specific
invariants remain open. Preserve [Input's actual initialized cursor reuse](WEAPON_MENU_TYPE_DOMAINS_AND_CURSOR_REUSE.md)
without using it as an invented fallback for a different private stack slot.
