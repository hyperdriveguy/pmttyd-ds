# OperationMenuMain:admission,opcode publication,and state routing

Complete static US G8ME01 revision0 BattleCommandDisplay_OperationMenuMain
8012165c/0x2cc. [Navigation/cancel](OPERATION_MENU_NAVIGATION_CANCEL_AND_AUDIENCE.md)
completes this same body;[setup/end](OPERATION_MENU_SETUP_END_AND_FLAG_OWNERSHIP.md)
closes its lifecycle wrappers and complete UI Main stub. Native request:
[EVT-01](../../requests/EVT-01.md).
[Verifier](../../../tools/verify_operation_menu_control.py) pins four complete
bodies,called service identities,every local instruction,and two original sound
keys. Static evidence only;source/query reachability and original execution
remain separate. Operation descriptors are documented by
[complete producer](OPERATION_TABLE_ENTRY_PARTIAL_ROWS_AND_PUBLICATION.md).

## Pending clear occurs before key admission

Retain passed B;M=wrap32(B+0x171c);local result R0. Read U8(B+0x1c74).
Nonzero clears that byte THEN WORD(B+0x1c70). This happens BEFORE requesting
BattleMenuKeyOKInACT(B). Gate0 skips cursor query and all input controls,but
still makes common display and returns0. A port cannot omit the pending clear
merely because the menu gate rejects input.

Gate nonzero requests GetCursorPtr(M,5,&privateSP08),ignore return. There is
no actor lookup in this caller;cursor helper has its own current-global query.
Request Trigger(0x0100). Nonzero confirms;zero still confirms if a fresh
pending-byte read is nonzero. The trigger/key/cursor services may republish
pending state after its initial clear. No updater/rebuilder request occurs in
Main;it consumes existing rows and live cursor/index values.

## Enabled confirmation and fresh opcode after sound

Read cursor/index,derive Q=wrap32(M+28*index),read WORD(Q+0x2d0) enabled.
Nonzero requests sound2520,then RELOADS cursor/index AFTER sound,reads
current WORD(Q+0x2cc),stores WORD(M+0x548),and rereads M548 to dispatch.
Enabled and opcode may come from different rows if the service changes cursor.
No bounds/row-count/opcode admission check precedes publication.

| Current published opcode | Original local route | State/request |
| --- | --- | --- |
| 0 | 80121748 | WORD(M+0)=24 |
| 4 | 80121754 | WORD(M+0)=27 |
| 1,2,3,5,6,7,8 | 80121730 | OffLightInCommand(0,1),THEN WORD(M+0)=29 |
| signed negative or>=9 | default8012175c | no state/light write |

Every enabled case THEN requests OperationMenuEnd(B),sets R1,and reaches
common display/return1,including default/unknown opcodes which wrote no state.
Unlike class Main's default return0,this return is explicitly1 after End.
The light service occurs before state29 store;End occurs after state writes.
Do not reorder these around the opcode publication or substitute a cursor row
snapshot for the fresh reads. No menu TYPE M540 write occurs in this body.
Selected operation M548 and Input state M00 are different fields.

## Disabled confirmation and fresh reason after End

Enabled0:if CURRENT pending byte is0,request source802efe88,
`SFX_NOT_CHOICE1`;otherwise omit that sound. Always clear pending byte.
Reload cursor/index,read current reason WORD(Q+0x2d4). Zero goes DIRECTLY
to display/return0,without navigation/cancel/audience handling.

Nonzero requests OperationMenuEnd(B),stores state22,sets R1,then reloads
cursor/index AGAIN and reads current reason into WORD(M+0x564). End/UI can
change the selected row/reason between test and copy. There is no opcode write
on this path. Common display/return1 follows. No old reason rollback or direct
error-message/event creation occurs in this caller.

## Caller and query-domain boundary

[Complete Input](COMMAND_INPUT_AUXILIARY_STATES_AND_ATTACK_DISPATCH.md)
invokes this in state21;nonzero Main return sets its local progress flag,allowing
another dispatch in the same invocation if pending handling permits. Opcode0
routes through party selection,state27 through protect-partner selection,and
state29 through [attack selection](BATTLE_ATTACK_EVENT_SELECTION.md),which
interprets type5/M548 through its own operation table/event/accounting policy.
Do not merge opcode/state/type or import that selector's side effects into Main.

These local routes do not prove every downstream selected-weapon query has a
supported cursor. Party/protect consumers,remaining item/source/delegate/event
bindings,Game/platform/original runtime/full-game requirements remain open.
