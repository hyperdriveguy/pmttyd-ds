# Operation menu:boundary controls,cancellation,audience,and return

Completes [Main opcode publication](OPERATION_MENU_MAIN_OPCODE_AND_STATE_PUBLICATION.md)
for US G8ME01 Main8012165c/0x2cc. Static only. Retained B/M/privateSP08/R
are defined there. [Verifier](../../../tools/verify_operation_menu_control.py)
pins all instructions and original keys;literal pad masks are not inferred
original physical button labels.

## Forward gate depends on pre-request cursor position

When no confirmation,load cursor C from SP08;read count WORD(C+8) BEFORE
index WORD(C),compute wrap32(count-1),compare signed. Index<count-1 requests
BattlePadCheckRepeat(0x00020000). Nonzero RELOADS cursor/current index,
stores wrap32(index+1),requests psndSFXOn(802efe98),then goes to common
display/return0. It omits reverse,cancel,and audience. Zero continues.

Index>=count-1 instead requests BattlePadCheckTrigger(0x00020000).
Nonzero reloads cursor,stores index0,requests that same sound,and exits.
Zero continues. Do not use Repeat at the boundary or reuse the pre-request
index for the interior arithmetic. No zero/negative-count guard is present.

## Reverse gate follows only unsuccessful forward handling

Reload cursor/index. Signed index>0 requests Repeat(0x00010000);nonzero
reloads cursor/index,stores index-1,requests sound,and exits. Signed index<=0
instead requests Trigger(0x00010000);nonzero reloads cursor/count,stores
wrap32(count-1),requests sound,and exits. Zero continues to cancel handling.
Negative indices use this boundary branch. The actual stores occur BEFORE
sound on all four movement paths. Original key802efe98 is
`SFX_BTL_CURSOR_MOVE2`;reuse original audio,not generated replacement tones.

A successful forward move prevents reverse processing. Interior-versus-boundary
classification uses pre-request fields;arithmetic uses fresh post-request fields.
Cursor movement returns0 and does not write operation code or Input state.
There is no scroll-window adjustment in this Main.

## Cancel before audience,after direction priority

Only when no direction moved,request Trigger(0x0200). Nonzero requests
sound2521,then OperationMenuEnd(B),then stores state1 and sets R1. Reach
display/return1;omit audience query. Unlike confirmation,End occurs BEFORE
the state write on cancellation. No operation-code/type/reset/help service.

Cancel gate0 requests BattleCommandAttackAudienceCheck() with no B argument.
Zero goes to display. Nonzero stores BYTE(B+0x1c74)=1 BEFORE requesting
OperationMenuEnd(B),then display. This branch leaves R0,despite pending/menu
changes. Its order differs from WeaponSelectMenuMain,which requests End
before pending1. Preserve actual caller-specific order.

The [complete audience helper](AUDIENCE_INTERRUPTION_MENU_TYPE_WRITER.md)
captures global B and writes type11/state29;this caller retains passed B.
Without an ownership invariant,those may differ. Do not replace the helper's
global owner with passed B or set R1 because the helper reported success.
[Input pending re-entry](COMMAND_INPUT_ENTRY_REENTRY_AND_CLASS_STATES.md)
can make progress even when this Main returns0.

## Common display and result

Every path,including rejected key admission,requests
BattleMenuDisp_Operation_Main(B) exactly once at80121908;then returns local R.
That complete symbol80115d2c/4 is a single `blr`,while Main explicitly returns
its retained local0/1 afterward. This specific no-op does not prove all original
menu rendering is absent:Init/End are separate larger UI bodies.

Implement confirmation priority,disabled-reason early exit,boundary Trigger/
interior Repeat,forward priority,post-request rereads,state-before/after-End
differences,and pending-before-End audience ownership. Menu/source/delegate/
query-event domains,Game/platform/original execution remain open.
