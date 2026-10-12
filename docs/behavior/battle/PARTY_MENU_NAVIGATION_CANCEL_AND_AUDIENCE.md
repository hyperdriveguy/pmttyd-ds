# Party menu:boundary controls,cancellation,audience,and display

Completes [Main selection](PARTY_MENU_MAIN_SELECTION_AND_PENDING_BEHAVIOR.md)
for static US G8ME01 party Main80121098/0x224. Retained B/M/R and private
SP08 are defined there. [Verifier](../../../tools/verify_party_menu_lifecycle.py)
pins every instruction and original keys. Pad masks are literal requests,not
inferred original physical button labels or render-frame timing.

## Forward and reverse boundary policies

Navigation starts after no confirmation OR after a disabled confirmation.
Load cursor;read count WORD(C+8) BEFORE index WORD(C),compute
wrap32(count-1),compare signed. Index<count-1 requests Repeat(0x00020000).
Nonzero RELOADS cursor/current index,stores index+1,requests cursor sound,
then exits to display/return0. Index>=count-1 instead requests
Trigger(0x00020000);nonzero reloads cursor,stores index0,sound,then exits.
Zero from either gate continues to reverse handling.

Reverse reloads cursor/index. Signed index>0 requests Repeat(0x00010000);
nonzero reloads cursor/index,stores index-1,sound,then exits. Signed index<=0
instead requests Trigger(0x00010000);nonzero reloads cursor/count,stores
wrap32(count-1),sound,then exits. Gate0 continues to cancellation.
Every sound follows its index store;each uses802efe98,
`SFX_BTL_CURSOR_MOVE2`. No zero/negative count or negative index clamp.
Boundary classification uses pre-request values;actual arithmetic uses fresh
post-request values. A successful forward movement prevents reverse/cancel/
audience handling. R stays0 on all direction branches;no party ID publication.

## Cancellation goes back to operations

Only if neither direction moved,request Trigger(0x0200). Nonzero requests
sound2521,then ChangePartySelectMenuEnd(B),then stores Input state20,sets R1,
and goes to display. End occurs BEFORE state20 store. No help service,cursor
reset,party ID/type/opcode write,or reason-event constructor in this branch.
The exact state is20,not the class menu's1. Input state20 selects the resumed
operation Setup flag0,then immediately enters state21 Main as already recovered.

## Audience interruption sets pending before End

Cancel gate0 requests BattleCommandAttackAudienceCheck() with no B argument.
Zero reaches display. Nonzero stores BYTE(B+0x1c74)=1 BEFORE requesting
ChangePartySelectMenuEnd(B),then display. R remains0,despite pending/UI
changes. This order matches operation Main and differs from weapon Main's
End-before-pending order. Do not centralize these into an invented common reset.

The [complete audience helper](AUDIENCE_INTERRUPTION_MENU_TYPE_WRITER.md)
captures current-global B and writes type11/state29;this caller retains passed
B. Source owner equality remains a required invariant,not a fact supplied by
these local bodies. [Input pending re-entry](COMMAND_INPUT_ENTRY_REENTRY_AND_CLASS_STATES.md)
can process the interruption despite Main's0 return.

## Common display and explicit result

Every path,including rejected key admission,makes exactly one request
BattleMenuDisp_ChangePartySelect_Main(B) at8012129c,then returns retained R.
Complete UI Main80115acc/4 is `blr`,with no local draw/store/return assignment.
UI Init/End are larger separate pinned service identities. This Main stub alone
does not imply original party-menu rendering/resources are absent.

Implement disabled-confirm fall-through,interior Repeat/boundary Trigger,
forward priority,fresh cursor arithmetic,cancel End-before-state,audience
pending-before-End,and explicit result0/1. UI/source/event/query/native Game/
platform/original runtime remain separate work.
