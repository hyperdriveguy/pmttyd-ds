# Stage aftereffect leaves: event replacement, nozzles and fall requests

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete static DestroyA1/A2/B801496f0/a4,80149684/6c,801495e0/a4;
NozzleDirChangeCheck80148bf0/64,_Nozzle_Change_Check80148af0/100;
NozzleWorkCheck80148414/b8,_nozzle_type_init80148374/a0;
IronFrameFallCheck801487c0/64 and ObjectFallCheck80147644/60.
Read [dispatch](BATTLE_WEAPON_AFTEREFFECT_DISPATCH.md) and
[event lifetime](../event/EVENT_RUNTIME.md). Verify: `python3 tools/verify_weapon_aftereffect.py`.
These are request/lifetime contracts; hazard event execution remains open.

## Shared ownership and event interface

Every leaf reloads current shared BattleWork W; metadata offsets below are
relative to its captured W. evtEntry(script,priority10,flags0) returns an event
pointer; its word+15c is the event ID. Checks use evtCheckID; deletion uses
evtDeleteID. Neither a retained handle nor its nonzero value establishes
liveness without the explicit check. No native timer substitutes for these
original event requests. Most leaves have no specified scalar return.

## Destruction and shared replacement event

Obtain stage pointer S=word(word(word(W+2738)+c)+0). Read primary event handle
and script override BEFORE calling evtDeleteID(primary), even when primary0:

| Leaf | Primary handle | Script override in S | Default script |
| --- | --- | --- | --- |
| DestroyA1 | W18100 | +194 | _object_turn_event_A1 at80417d48 |
| DestroyA2 | W18104 | +198 | _object_turn_event_A2 at80417d64 |
| DestroyB | W18108 | +19c | _object_turn_event_B at80417d80 |

After deletion,clear the primary handle to0. A1 and B then read shared W180fc;
if nonzero and evtCheckID(handle)!=0, reread that handle and delete it. The
field is not locally cleared. Select override when nonnull, otherwise default,
and request evtEntry. For A1/B, nonnull creation result writes its ID to W180fc;
null leaves the old stored handle value. A2 ignores creation result entirely
and stores no replacement ID. No stage/config-pointer null checks or rollback.
The default wrappers set L9=0/1/2 then run child80376ab0; that child is not
newly specified here. Three ordered destruction calls therefore compete for
shared W180fc according to their actual deletes and create-result stores.

## Nozzle direction change and per-nozzle ownership

NozzleDirChangeCheck captures W and tests table pointer W18110. Null exits.
Otherwise for i=0,1,2, reread the table pointer and call
_Nozzle_Change_Check(i,table+4*i), in order. Each helper captures its own shared
W. Sum four unsigned chance bytes at supplied row; call irand(sum); scan them
in order, subtracting each until result<0. The chosen byte index k can be4
when all weights are0; irand(0) consumes no rand state. This uses strict <,
unlike the candidate chooser's <=0 boundary.

Let n=U8(i). Current direction is U8(W+182a0+n); desired is W182a3+n and
retained event ID is word(W+182a8+4*n). If U8(k)==current, exit after the draw
without checking the event ID or changing desired. Otherwise if retained ID
nonzero, call evtCheckID; when it returns0, clear that ID. Reread the ID; a
nonzero value exits, so a live direction-change event suppresses replacement.
For zero ID write low8(k) to desired, then evtEntry(_nozzle_turn_event80417d9c,
10,0). Read returned pointer+15c and store ID FIRST; only afterward test pointer
nonnull and, if nonnull, store U8(original i) in returned event word+160.
The apparent null test comes after dereference: it does not provide a safe
allocation-failure path. No current-direction store occurs in this helper.
The queued event must consume desired/index and eventually update its owners;
its complete behavior remains open.

## Nozzle work type selection and forced request

NozzleWorkCheck(force) captures W then calls pouchGetPtr. If S16(pouch+88)<=0,
exit without type initialization, event checks or construction. Otherwise call
_nozzle_type_init, which reloads W and captures table=W18110, then reads signed
byte type W182a6. Only exact-1 triggers selection: sum four U8(table+c..f),
call irand(sum), scan subtraction until result<0, and store chosen low8 index
(type4 possible for zero sum). No table-null guard. Existing types are preserved.

Back in NozzleWorkCheck, when force==0 AND S8(capturedW182a6)==0, exit.
Any nonzero force bypasses that type0 gate. Read handle W182b4; if nonzero,
call evtCheckID and clear it only for a zero return. Reread handle; nonzero
exits. Otherwise request script _nozzle_work_event_1 at80417df4 for nonzero
force, or _nozzle_work_event_0 at80417dd8 for zero force. Both priority10/flags0.
Read returned event ID without a null check and store W182b4. The default
wrappers set L10=1/0 then call child80376ea0; its execution remains open.
Force bypasses neither the pouch+88 gate nor an existing live event. Aftereffect
always supplies force1, but the full leaf's zero path remains specified.

## Falling frame and object checks

IronFrameFallCheck captures W and U8(W180f8). If either0x40 or0x20 is already
set, return without creation. Otherwise OR0x20 into that byte, then request
_fall_frame_event80376d6c,10,0 and store returned ID to W182c4. No liveness,
previous-handle or allocation-failure check. The flag write precedes construction
and remains if a port's creation adapter reports failure; no rollback is proven.

ObjectFallCheck captures W. If W.flagsEf4&0x1000, return0 without touching
W182c8 or constructing anything. Otherwise request _object_fall_event803771bc,
10,0; read ID into W182c8, then reread returned event ID and return it. There
is no event-pointer null check, previous-handle liveness check or delete. The
[existing tutorial gate](BATTLE_ACTION_FEEDBACK_AND_STAGE_GATE.md) binds the
opening boss's0x1000 writer; it suppresses this object registration only.
Aftereffect ignores ObjectFallCheck's return. Frame creation can set stage0x20
before the same dispatch's subsequent object eligibility test.

Acceptance cases: config override/default; delete0 request; shared A1/B
replacement versus ignored A2 result; failed creation's retained handle policy;
nozzle equal choice with no liveness query; stale/live per-nozzle handles;
zero-sum type/direction4; force/pouch/type gates; queued ID/index write order;
frame0x40/0x20 inhibition and flag-before-create; object0x1000 suppression.
Stage source/setup/cleanup writers, all called event bodies, rendering/audio/
hazard damage, allocator/lifetime domains and original runtime traces remain
open. Invalid-pointer domains are not locally recovered fallback behavior.
