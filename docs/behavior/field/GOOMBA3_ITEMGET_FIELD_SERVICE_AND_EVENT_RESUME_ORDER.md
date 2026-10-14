# Goomba3 item-get field services and event resume order

US G8ME01 disc0 revision0; read-only DOL/REL recovery2026-10-14.
Complements [quest caller](TROUBLE_CENTER_QUEST2_13_WRITERS_AND_ACQUISITION.md),
[finite item95 owner](GOOMBA3_ITEM95_GET_SEQUENCE_AND_DEFERRED_GRANT.md),
[event continuations](../event/EVENT_DISPATCH_AND_WAIT_CONTINUATIONS.md)
and [ordinary talk lifecycle](FIELD_ORDINARY_NPC_TALK_CANDIDATE.md).
This pins source owner order, not rendered-frame timing or a new scheduler.

## Ordinary talk is an actor-context root event

Goomba3 regular/talk scripts are installed through original NPC setup.
A candidate A admission commits fieldstate1→2 and returns from its current
npcMain dispatch. On the next admitted NPC talk-mode stage0, fbatTalkMode
stops regular event, locks control/party service and uses
**evtEntry(talkscript,0,0)**. It writes eventtype6,contextNPC pointer+170,
retains the eventID for later stage1 waiting. This is an ordinary talk root,
not an assumed RUN_CHILD event merely because it belongs to an NPC.

In the admitted field path, that NPC creation occurs after the current
root's event-manager call. Consequently this construction cannot execute its
script in the alreadyfinished call. Subsequent original manager visits/credits
own execution; no fixed “one rendered frame” rule follows.

## Complete local field order

Fresh marioStMain8006f904/238 waits whileGPword10!=0 through retrace.
Along its admitted local dispatch path the relevant calls are:

| Order | Source call site / owner |
| --- | --- |
| 1 | 8006f950 hitMain80018264 |
| 2 | 8006f954 evtmgrMain800368d0 |
| 3 | 8006f95c marioMain8005a7e0 |
| 4 | 8006f960 mobjMain8005f968 |
| 5 | 8006f964 npcMain80049a7c |
| 6 | 8006f978 itemMain800adfe0 |
| 7 | 8006f988 psndMain800dc48c |
| 8 | 8006f9a0 winMgrMain8023f1b0 |
| 9 | 8006f9a4 effMain8005ea7c |

Other source calls occur between these sites; this table does not erase those
owners or imply their predicates. No conditional reordering of the listed
calls appears on this admitted body path. Outer pause,sequence/retrace,event
credit,input snapshot,display queue and backend completion remain providers.

## Wrapper firstCall and same-iteration item service

Goomba3 acquisition script prints original irai_02_00,then calls entry95,
flag_on2,get_item wrapper,then writes5405. When print returns2, dispatcher
can fetch entry/flag/get in the same dispatcher invocation; entry and flag
wrappers return2 and therefore introduce no mandatory wait.

First get-wrapper call forces sequence2/substate0/mainflag1000 thenreturns0.
It **does not grant95 or write5405**. Further manager credits can repoll the
retained callback in the same manager pass; return0 is not proof the whole
pass has ended. With fresh live entry/status0 these polls stillreturn0.
See original USER_FUNC marker/return and manager-credit contracts.

After that manager call, this same field iteration can reach Mario/NPC,then
itemMain. Finite itemseq_GetItem0 falls through1,requests GetItemMotion,
creates original effect/windows and calls pouchGetItem95 before returning
substate2. Because marioMain already ran earlier, the queued motion15
initialization/M_I_U selection is consumed by a **later admitted marioMain**,
not an instant pose call inside the itemseq request. Effect main later in this
iteration can update its age/scale and queue display independently.

New window open requests precede this iteration's winMgrMain. Its Seq call
sets transition4 and executes first open interpolation; the earlier itemMain
cannot retroactively see that later publication. Finite getowner remains
substate2 until another itemMain call, avoiding an invented first-call
acknowledgment before its windows are admitted.

## Window action clears after the item owner's query

Later itemMain substate2 falls through3: control-off,conditional BGM work,
then mainwindow action query and A/B trigger admission. It reads actionflag4
from prior window-manager publication. Current iteration's winMgrMain is
later,so opencompletion/idleflagclear there becomes visible to a later
itemMain. Do not query a newlyadvanced manager earlier simply because a native
renderer exposes its current phase before the authored simulation order.

On acknowledgment itemMain requests AutoDeleteClose name/help,soft-deletes
effect,restores controls/pose/camera on successfulgrant and deletes all matching
entry names. This occurs before current psndMain,winMgrMain andeffMain.
Those later owners can consume requestedclose/fade/update in this same admitted
field iteration; script5405 still cannot run backward into the alreadyfinished
EVT main call.

## Deletion permits5405 on a subsequent event-manager visit

On later evtmgrMain get-wrapper polls name afresh. Missing/inactiveentry or
source status2/10000 returns2. In finite successful cleanup, missing/inactive
entry proves wrapper completion. Dispatcher then fetches5405=1 and RETURN/END
in that same invocation. The saved flag belongs to the script after wrapper,
not the moment pouch95 was inserted or a window becamevisible.

Later npcMain in that iteration can observe talk eventID no longerlive and
perform source stage1 regular-resume/pose/control/party cleanup. It waits event
termination,not native item95 popup visibility. The closing manager records
can still be consumed by their own laterowner; original talk return does not
require waiting their visualclosecompletion. Borrowed parameter/name/resource
lifetimes must remainvalid until their actual provider releases them.

Failed pouchGetItem/substate5 discardfrontier never implies successful
wrapperreturn/5405. Preserve live entry/control/event ownership while that
source menu continuation remains unresolved.

## Synchronous audio is additional to the later outer sound pass

Original On_f_d calls psndBGMMain before returning success. FanfareOn211 during
itemMain can therefore update BGM groups before the root's later psndMain
calls BGMMain again. Likewise NULLresume Ona0 onack can publish/update its
secondaryunmute and anygroup1off envelope synchronously,then the outer sound
pass advances those actualowners again. Do not collapse these requests into
one generic fader update at renderingtime.

OuterpsndMain's SoundMain→SoundSSMain follows BGMMain; window/effect managers
follow that entire soundpass. Streamcallbacks/device completion can have their
own cadence; no wall-clock2.6215-second vacancy substitute follows this order.
See [audio boundaries](../audio/FIELD_BGM_SECONDARY_BOUNDARIES_AND_DETACHED_TAIL.md).

## Evidence and remaining scope

Fresh ignored fieldmain.txt/seqmain.txt underartifacts/reference/goomba3-services;
actual marioStMain ownercall addresses verified against original USsymbols.
Earlier readonly REL inventory pins original Goomba3 script order; complete
wrapper/VM/NPC/window/audio contracts supply their local continuation rules.
No code/sharedindex/progress,assets,userprofiles or saves edited.

Next native implementation should preserve actual authoring-owner order and
retained event/resource lifetimes while validating original message/item/UI/
audio services. Source scheduler/pause/credits/display cadence and native
hardware/raster/audio parity remain unverified separate providers.
