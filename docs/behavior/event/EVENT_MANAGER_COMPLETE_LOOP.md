# Complete event manager loop: context,time,eligibility and credit

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete local evtmgrMain800368d0/1ec,make_pri_table80037934/12c and
context getter evtGetWork80037a60/20. Read
[dispatcher/waits](EVENT_DISPATCH_AND_WAIT_CONTINUATIONS.md),
[existing ownership](EVENT_RUNTIME.md) and
[game input order](../input/BATTLE_INPUT_CADENCE_AND_SNAPSHOTS.md).
Verification: `python3 tools/verify_event_manager_loop.py`.
No original pause/multicredit/runtime trace claim.

## Captured context and live globals

The manager captures GP through global pointer80415fd0. Use field context
803d8d30 when GP.word14==0; otherwise battle context803d8dd0. Each context
has signed word slot capacity+0,entry array pointer+90 and previous timestamp
high/low words+98/+9c. Entry stride1b0. Shared schedule arrays are event IDs
at803d8530 and slot indices at803d8930. Live schedule count8041e85c and
manager-active word8041e858 are separate globals. Set manager-active1 before
delta computation; clear it0 at every normal local exit, including an abort.
The manager has no scalar return contract.

The manager retains its selected context throughout the loop. make_pri_table
reloads GP and selects its own context when called. evtGetWork similarly
reloads GP on every call. There is no equality/stability check between these
captures. A context switch during a mutating adapter is not silently normalized.

## Exact lifetime delta source and large-gap substitution

Capture timestamp T from GP.word38(high)/word3c(low). Compute wrapping64-bit
difference T-previous; interpret signed64 and clamp negative difference to0.
Let clock=U32(memory800000f8). Compute k by these unsigned word instructions:
`u=clock>>2; k=high32(u*0x10624dd3)>>6`. This is the concrete millisecond-rate
conversion,not a measured constant. Request __div2i(delta,k),signed64 division
with high denominator0. If signed quotient>500,replace delta high with0 and
low with low32(divw(1000,S32(GP+4))*k). Otherwise preserve full clamped delta.
No zero-rate/zero-divisor/negative refresh/domain guard. Capture clock and GP+4
where shown,not from a port's assumed60Hz clock.

Store original captured T back to context.previous low THEN high, even when
negative or large-gap substitution occurred. The replacement does not change
that timestamp. Event lifetime gets raw tick delta,not milliseconds and not
float credit timescale. Original GP timestamp/refresh/clock producer semantics
remain dependencies. No OSGetTime call occurs in this local manager.

## Full priority table construction and ordering

make_pri_table selects context, captures its entry array, then scans signed
slot indices0..capacity-1. Each active entry (U8(flags8)&1) appends slot index
and current event ID entry+15c. Stopped/waiting/0x80 entries are still included.
Publish active schedule count before sorting. Unused array tails are not cleared.
No capacity-versus-256 check or entry-pointer validation occurs locally.

Use nested exchange sort i=0..count-2,j=i+1..count-1. Reload entry-array pointer
for each comparison;read each entry's UNSIGNED priority byte+b. If priority(i)
<priority(j),swap BOTH scheduled slot and ID. Otherwise do nothing. Higher
priority executes earlier. Equality avoids a direct swap but the sort is not
stable: initial A1,B1,C2 becomes C2,B1,A1. Sorting does not recur during this
manager pass. Source priority/type and constructor append behavior remain
separate contracts.

## Exact manager eligibility and per-entry operations

Iterate schedule index s from0 while signed s<the freshly loaded live schedule
count. At each index reread scheduled slot,then context.entryArray,compute entry
with wrapped word slot*1b0. Read flags8;skip unless active bit1. Then compare
live entry.word15c to scheduled event ID;skip mismatch. Finally skip when
flags8&0x92 (0x2 stopped,0x10 waiting-child,0x80 additional inhibition).
Do not test a type mask,HP,status,run-check callback or global pause at this point;
those producers/other layers remain open. These skip routes advance to the next
schedule entry WITHOUT changing its lifetime or credit.

For an eligible entry E:

1. Add retained tick delta to E's64-bit lifetime+0/+4 (low store then high,
   carry propagation,wrapping modulo64). This happens once per eligible visit,
   not once per consumed credit or per opcode.
2. Read binary32 credit E+168,then binary32 timescale E+164; single fadds,
   store result to E+168. Constant threshold/subtraction is binary32 1.0.
3. Repeatedly reload credit. If ordered credit<1.0,finish this entry. Otherwise
   single fsubs(credit,1.0),store BEFORE calling evtmgrCmd(E).
4. Dispatcher return1 aborts the ENTIRE manager schedule immediately. Return-1
   finishes only this entry. Every other return repeats the credit test for
   this SAME entry. Do not recheck flags,event ID,context,priority or type here.
5. If not aborting,increment schedule arrays/index and test the LIVE count again.

Thus interpreter yield return0 does not forbid a second invocation in the same
manager pass. Two available credits can poll a retained callback twice against
the same previously updated physical pad sample. Handler completion/yield and
manager-pass progression must remain distinct. A callback returning a negative
handler code becomes dispatcher1 and can prevent later entries from receiving
lifetime/credit for this pass. Entry deletion's dispatcher-1 ends only that entry.

A fractional example: prior credit0,scale0.5 executes nothing after first
eligible visit and one dispatcher invocation after second. Prior credit0.25,
scale2 leaves0.25 after two invocations if neither aborts/deletes. Tick delta
updates only once on either visit. These are derived cases,not measured game
cadence. NaN/infinite/invalid credit is not locally repaired; valid finite credit
and timescale domain is an implementation invariant,not an original clamp.

The live-count loop can see current-table appends made by other calls. Its
append producers,capacity,constructor state/initial credit and run-check policy
remain to be recovered;this manager alone does not establish them. A port must
not freeze an immutable schedule count or silently resort newly appended work.

Acceptance cases: field/battle context,negative and>500 delta,original timestamp
retention,wrap64;active/ID/each0x92 skip;unsigned priority unstable ties;
credit rounding/subtract-before-call;0/-1/1 dispatcher outcomes;multicredit same
input,live-count changes and no inner revalidation. Complete local manager is
answered; constructor/restart/context/time writers,all opcode bodies,source
eligibility policy and original traces remain EVT-01 dependencies.


## Complete context initialization follow-up (2026-10-12)

[Init/reset](EVENT_CONTEXT_INITIALIZATION_AND_RESET.md) and
[SDK heap/direct callers](EVENT_BOOTSTRAP_HEAP_AND_RESET_CALLERS.md) now specify
both original256-slot arrays and full local reset/clear/preservation policy.
ReInit preserves selected work/flag banks and shared schedule count,clears only
selected entries and resets shared allocation/active globals without deletion.
Initial ID1/speed1.0 are DOL data,not writes by those bodies. Source/context/
capacity mutation,heap/scene/clock/platform domains and original traces remain open.


## Timestamp producer and outer-loop follow-up (2026-10-12)

[Retrace clock publication](../platform/EVENT_RETRACE_TIME_PUBLICATION.md) now closes GP38/
3c's local raw-time producer,window/GP10 freeze and baseline/mask behavior;
[outer/update loop](../platform/GAME_OUTER_LOOP_AND_EVENT_UPDATE_ORDER.md) closes main,
StMain/display requests,33-call ordering and idle tail. Earlier producer scope
is superseded for these local paths. Hardware clock/VI/thread/gate/state/source
and measured original input/pause/multicredit cadence remain open.
