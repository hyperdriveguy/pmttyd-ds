# Goomba3 item95 get sequence and deferred grant

US G8ME01 disc0 revision0, read-only DOL recovery2026-10-13.
Follow-up to [wrapper frontiers](GOOMBA3_ITEM_ENTRY_GET_AND_COIN_SERVICE_FRONTIERS.md)
and [quest caller](TROUBLE_CENTER_QUEST2_13_WRITERS_AND_ACQUISITION.md).
Finite ordinary field item95 success path; not the full inventory/discard UI.

## Entry mode is distinct from runtime flags

Original itemEntry800ad7f0/7e0 stores its u16 entry-mode argument at entry+2.
Thus Goomba3's argument16 is **mode16**, not runtime mainflag16. Source table
80313e90[16] resolves800addac: sequencehalf24=7,substatehalf26=0.
Fresh entry mainhalf0 ORs active1; evt_item_flag_on2 gives mainflags3.
Forced get resets sequence2/substate0,OR1000,so finite mainflags1003.
This refines earlier frontier shorthand “16|2 flags”; mode16 and flag2 are
separate fields. Mode16 does not introduce mainflag4's shortcut receive.

Entry kind95 lives word+4,name inline+0c,callback+20=0 for source final0,
secondary+8=-1. Non-effect ordinary icon is owned by original itemDataTable
803108a8+40*95,u16+20,not a generated sprite. Full constructor allocation,
existing-name replacement/resource ownership remains itemEntry's provider.

## Original substate dispatch

itemseq_GetItem800ab930/1324 chooses ordinary field globalwork803dc290
(the separate alternate global+1c is selected by GP+14!=0). It reads Mario
and dispatches half26 using original table80313e68:

| Substate | Original entry |
| --- | --- |
| 0 | 800ab994 |
| 1 | 800abd08 |
| 2 | 800ac268 |
| 3 | 800ac28c |
| 4 | 800ac58c |
| 5 | 800ac6bc |
| 6 | 800ac8d8 |
| 7 | 800ac91c |
| 8 | 800acb30 |
| 9 | 800aca34 |

Values>9 return without a synthetic reset. Source itemMain invokes this owner
for sequence2; scheduling is separate from the blocking VM wrapper. No fixed
native render-frame delay is established by the dispatch table.

## Substate0 control hold, then substate1 presentation/grant

0 clears statusword38 bit10000; copies mainflag100 to status04000000 and
clears mainflag100. Finite1003 lacks mainflag4, so bypasses the immediate
pouchGetItem shortcut. Set substate1,counters78/7c=0,camera8 flag200,
marioKeyOff,marioStSystemLevel(2),psndSetFlag(80),clear Mario floats
174/17c/180/18c. Kind95 is outside the special resource IDs7/8/10/11;
no status2000 special-resource flag is set. Execution falls through to1
in this same call; do not add an unconditional frame between these owners.

1 with forcedmain1000 set and main4000 clear bypasses
marioChkItemGetMotion800948ec and proceeds directly to normal presentation
at800abd60. Thus finite Goomba3 forced-get does not wait that motion query.
Without that admitted flag combination, the alternate branch queries it: a
nonzero result proceeds; zero with currentmotion5,6,7,8,14,15 sets substate8,
otherwise retains1. Those alternate flags/motion substates remain separate.

On normal admission set substate2, marioChgGetItemMotion80094c90,move item
XYZ to MarioXYZ+(0,50,0). Kind95 selects effect type6 and fanfare category1.
Call effItemGetEntry(6,MarioX,MarioY+62,MarioZ),retain work+0c; set effect
payload+38=5. Normal status2000 clear sets iconflag100 and cameraID unchanged
(the specialbranch selects7). Unless mainflag8000 set, query
psndBGMChkSilent(0),set mainflag8 if nonzero; unless mainflag8 skip it, call
psndBGMOff(3800), then psndBGMOn(211,"BGM_FF_GET_KEYITEM1").

Clear mainflag2000, then open original item-get window definition80313dc8
with winMgrEntry/Open/SetParam(entry),retain windowIDentry+28.
Since status2000 clear, additionally open definition80313df0 (+28),retain
entry+2c and set same entry parameter. **Both windows open before granting.**
No original story evt_msg_print is issued by this window setup.

Now call pouchGetItem(95) at800ac134. Source low-level
[pouch receive dispatcher](../battle/POUCH_GET_ITEM_DISPATCH.md) owns key-item
insertion/capacity and its result. Do not reuse held-item capacity10/20 for95.
Successful result sets statusword38 bit1; nonzero secondary flag setters and
callback+20 work are skipped for original secondary-1/callback0.

Failed pouchGetItem clears no imaginary slot: it opens definition80313e18
(+50),retain entry+30,passes entry param,leaves statusbit1 clear. This is the
actual full-storage path and leads to discard/menu substates; native finite
success admission must establish capacity/provider completion, or retain a
frontier rather than fabricate a successful grant/5405 write.

For kind95, entry+1c effectpointer is0; lookup its icon and set iconhalf flag80.
Return from this owner with substate2. No timer-based500ms get-item wait is
established here; windows/Mario/effects and outer scheduling own completion.

## Substate2 control stop, substate3 acknowledgment and teardown

2,when status2000 clear, calls marioCtrlOff8005c2c4 and sets status01000000.
Sets substate3 then falls through to3 in the same call.
For finite main1003 (no8000/2000), psndBGMChk(1) nonzero sets main2000,
psndBGMOff(201),and if main8 clear psndBGMOn(a0,NULL). These are original
conditional sound routing requests, not synthetic tones.

3 waits winMgrAction(entry+28)==0 then keyGetButtonTrg(0)&0x300 (A or B).
Until both are admitted,retain all work and control hold. On acknowledgment
close entry+28 and (status2000 clear) entry+2c; when main8000/2000 clear,
request psndBGMOff(201),and if main8 clear psndBGMOn(a0,NULL).

Normal kind95 does not enter automatic badge235 grant/message branch.
Soft-delete retained effect,request cameraID4,clear iconflag100. Since
entry+1c==0,lookup icon and clear flag80. Successful statusbit1 then performs
marioStSystemLevel(0),psndClearFlag(80); if status01000000 clearthatbit and
marioCtrlOn; marioKeyOn;marioChgStayMotion;clear camera8 flag200.

Finally scans active item domain for **every matching entry name**, deleting
matching icons and clearing activebit1 for those entries. Source field kind95
is outside badge-arrival/effect-ID cleanup branches. It does not simply remove
one arbitrary last item. VM evt_item_get_item's next invocation returns2 when
entry missing/inactive, permitting caller5405=1 after the receive sequence.
No quest-state write occurs inside itemseq_GetItem itself.

Failed statusbit1 instead selects substate5,closes failure window+30,
Mario staymotion,sets iconflag2 and invokes winMgrSelectEntry(kind1,item95,1).
Full selection/discard/substates5..9 behavior remains open; existing
quest-hand-in key-selector type9 is not evidence for this original get-item
menu. Alternate entry/motion flags and callback branches remain separate.

## Parameter revision and provider boundaries

The full itemseq_GetItem direct call census contains no pouchReviseMarioParam
or pouchRevisePartyParam call. pouchGetItem800d50cc contains no such wrapper
call either. Do not insert source parameter revision merely because quest2
hand-in removal uses a different wrapper. Transitive Mario/UI services still
own their own side effects; this statement is about these two direct owners.

Remaining mandatory providers: exact Mario get-item motion admission/poses,
source window definitions/display/action/parameter handling, actual item95
icon descriptor, effItemGetEntry type6 display/lifetime, sound routing,
field active-item name ownership/cleanup, full capacity-failure selection,
substates8/9 motion branch and event-manager/itemMain cadence. This contract
closes source graph/order for finite success without claiming those renderers
or services already work natively. Original numerical/resource parity and
hardware performance remain unverified.

Evidence: original itemseq_GetItem code SHA256
5116d51f11696d1e57934e1e3f384d281f01e525fa6932ce8100b6bc2062975f; fresh inspection
ELF retained under ignored artifacts/reference/goomba3-services/seq.elf;
source instructions/routing tables/constants were reread from the readonly ISO.
Source50/62 constants80421118/11c and1024 float-operandscale80421114.
Documentation only: no code,index,sharedprogress,profiles or saves edited.
