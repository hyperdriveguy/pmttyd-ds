# Multi-item cancellation check:two-entry cursor,controls,and display request

Complete US G8ME01 WeaponMultiItemCancelCheckMain80121af4/0x1a4 and
Setup80121c98/0x18. Static only.[Verifier](../../../tools/verify_multi_item_menu.py)
pins them,linked callback identity,and binary32 display-sort constant900.0.
This is distinct from [multi-item mode selection](MULTI_ITEM_MENU_MAIN_SELECTION_AND_NAVIGATION.md).

## Setup and audience-first admission

Setup writes,in order,WORD(B+0x1c50)=0,WORD(B+0x1c54)=0,WORD(B+0x1c58)=2.
These are index/scroll/count of retained C=B+0x1c50(M+0x534). No UI,global/
actor/cursor helper,flag,type/state,or selected-mode request/write.Rawr3 staysB.

Main retains B/C,initializes completion R0 and affirmative A0. FIRST request
BattleCommandAttackAudienceCheck() with no argument. Nonzero stores
BYTE(B+0x1c74)=1 and returns0 DIRECTLY,skipping direction/confirm/cancel and
display registration. No End wrapper or key-admission service here.
The helper's captured global owner is separate from passed B without proof.

## Direction can be followed by decision in the same call

Audience0:read cursor count BEFORE index,compare signed index<count-1.
Interior requests Repeat0x00020000;boundary requests Trigger0x00020000.
Nonzero reads current index for interior arithmetic,stores index+1 or boundary0,
then sound802efe98 `SFX_BTL_CURSOR_MOVE2`. A forward move skips reverse,
but CONTINUES to decision handling rather than returning.

If forward did not move,read current index;signed>0 requests Repeat0x00010000,
otherwise Trigger0x00010000.Nonzero updates current index-1 or current
count-1,then same cursor sound. Whether moved or not,continue to decisions.
No bounds clamp or GetCursorPtr request;C is retained directly throughout.

## Cancel before confirm and mode-independent state writes

Request Trigger0x0200 first. Nonzero sets R1,A0,skipping confirm request.
Otherwise Trigger0x0100 nonzero sets R1;read CURRENT index,exactly0 sets A1,
all other index values leave A0.No pending-byte synthetic confirmation gate.
No confirm/cancel sound is requested in this decision section.

If R1,A1:store BYTE(B+0x1c6d)=1(M551),then Input state29 at B171c.
If R1,A0:store state6 only.If R0:neither state nor completion-byte write.
The negative choice does NOT clear a previous M551 byte;modeM550 is unchanged
on every local branch.Do not interpret raw index as a mode2/3 value.

## Display registration occurs even after a decision

Except the early audience branch,request,in order implied by original ABI,
`dispEntry(8,0,900.0,callback=0x80121974,user=B)` at80121c7c,then returnR.
Callback is WeaponMultiItemCancelCheckDisp80121974/0x180,identity pinned
without claiming its rendering/resource/body contract.Constant is binary32
900.0 at80422658,r2-19944 with startup r2=80427440 pinned.It is a display
sort/request scalar,not measured UI time or simulation ticks.

A decided state can therefore still register the cancellation display this
invocation.Do not move registration only into the undecided branch.Return0
on early audience can still mutate global type/state/pending through the helper.

## Complete Input relationship and port requirements

[Input](COMMAND_INPUT_AUXILIARY_STATES_AND_ATTACK_DISPATCH.md) state12 stores13
BEFORE Setup,then restore and common tail;direct13 requests Main and sets
progress only if nonzero.Affirmative state29 dispatches attack selection;
negative state6 resumes weapon setup.These are actual local bindings,not
proof of full multi-item story/event/inventory behavior.

Implement direct cursor ownership,audience early return,no key gate,interior/
boundary controls,decision after movement,cancel priority,exact index0 test,
M551 preservation,display-after-decision,and0/1 result.Reuse original source
UI/text/assets.Callback rendering,multi-item event/producer lifetimes,party
weapon delegates,query-event reachability,Game/platform/original runtime and
full-game requirements remain open.
