# Multi-item Main:mode publication,reason panel,and navigation

Complete static US G8ME01 MultiItemMenuMain80121354/0x264.
[Producer](MULTI_ITEM_TABLE_MODE_FIELDS_AND_SOURCE_BINDINGS.md),[setup/end](MULTI_ITEM_MENU_SETUP_END_AND_INPUT_BINDINGS.md),
[cancel check](MULTI_ITEM_CANCEL_CHECK_CONTROL_AND_DISPLAY_REQUEST.md).
[Verifier](../../../tools/verify_multi_item_menu.py) pins the complete body.
No original execution/query reachability claim. Literal pad masks are requests,
not inferred physical button labels.

## Admission and confirmation

Retain B,M=B+0x171c,local result R0. Request BattleMenuKeyOKInACT(B).
Zero skips cursor/controls and goes to common display/return0. There is NO
initial pending clear or actor lookup/table update. Gate nonzero requests
GetCursorPtr(M,3,&privateSP08),ignore return. Trigger0x0100 nonzero confirms;
zero still confirms when current BYTE(B+0x1c74)!=0.

Read cursor/index,derive Q=M+24*index,read WORD(Q+0x450) enabled. Nonzero:
sound2520;RELOAD cursor/index AFTER sound;read U8(Q+0x45e),store that byte
unchanged at M550;store Input state5;request MultiItemMenuEnd(B);set R1;
common display/return1. No mode range guard,type write,pending clear,inventory
use,cost payment,or event constructor. Enabled and published mode can come
from different rows after sound-visible cursor mutation.

Enabled0:if current pending byte0,sound source802efe88 `SFX_NOT_CHOICE1`;
otherwise omit sound. Always clear pending. Reload cursor/index,read reason
WORD(Q+0x464). Zero goes DIRECTLY to display/return0. Nonzero requests End,
stores state17,sets R1,then RELOADS cursor/index/reason AFTER End and copies
that fresh reason to M564. Display/return1. No M550 write on this path.

## Direction boundary rules and priority

No confirmation requested:load cursor,count BEFORE index,compare index signed
with wrap32(count-1). If index<count-1 request Repeat0x00020000;otherwise
Trigger0x00020000. Nonzero reloads cursor and updates CURRENT index+1 for
interior,or writes0 at boundary;then sound802efe98 `SFX_BTL_CURSOR_MOVE2`;
exit to display/return0. Zero continues to reverse.

Reverse reloads cursor/index. Signed index>0 requests Repeat0x00010000;
otherwise Trigger0x00010000. Nonzero reloads cursor,current index/count,
stores index-1 for interior or count-1 at boundary;then cursor sound;exit.
Zero continues to cancellation. No count/index clamp or scroll correction.
Boundary tests are before service,arithmetic fields after service.Successful
forward omits reverse/cancel/audience. All movement paths leave R0.

## Cancel/audience/common display

After no direction moved,Trigger0x0200 nonzero:sound2521,End(B),THEN state1,
R1,display.No mode reset/type/help/event write. Gate0 requests
BattleCommandAttackAudienceCheck() with no B argument. Nonzero stores
BYTE(B1c74)=1 BEFORE End(B),then display with R0. Captured global B in that
[complete helper](AUDIENCE_INTERRUPTION_MENU_TYPE_WRITER.md) can differ
from passed B here without a source invariant.

Every local path requests BattleMenuDisp_MultiItem_Main(B) once,then returns
explicit localR. Complete UI Main80115880/4 is `blr`;UI Init/End are separate
larger bodies. R0 can accompany navigation/interruption mutations;R1 is explicit
enabled confirmation/nonzero reason/cancel,not a generic “state changed” flag.

[Input](COMMAND_INPUT_AUXILIARY_STATES_AND_ATTACK_DISPATCH.md) calls this in
state16;nonzero sets progress for same-invocation re-entry.When state5 resumes,
[weapon setup](COMMAND_INPUT_ENTRY_REENTRY_AND_CLASS_STATES.md) reads type/
capabilities through its own policy.The original mode2/3 cost payment occurs
later in [target confirmation](COMMAND_INPUT_TARGET_REMAP_AND_CONFIRMATION.md),
not this Main. Full multi-item event/source/query domains,party delegates,
Game/platform/original runtime remain open.
