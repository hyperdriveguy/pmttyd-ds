# System-level event masks and execution-speed ownership

US G8ME01 disc0 revision0, agent ID1, recovered2026-10-12. Evidence B: complete
marioStSystemLevel0x8006f7c8,size0x114; marioStGetSystemLevel0x8006f7bc,size0xc;
evtSetSpeed0x800365ec,size0x10. Relevant entry initialization/dispatcher regions
and existing stop/start/scheduler contracts were checked separately. No original
pause/UI execution or Switch equivalence is claimed. This is a local control
contract,not the entire game's pause graph.

## Requested level, GP mask and exact transition table

marioStSystemLevel accepts a signed integer request. Recognized requests0..4
first store that integer in the separate word at0x8041e940,annotated
_mariostSystemLevel. Then mutate GP word+0x18 and call an event control service:

| Request | GP+0x18 mutation | Service request |
| --- | --- | --- |
| 0 | old & 0xfffffff0 | evtStartAll(0xef),call0x8006f818 |
| 1 | (old & 0xfffffff1),store;reload then OR1,store | evtStopAll(1),call0x8006f848 |
| 2 | (old & 0xfffffff3),store;reload then OR3,store | evtStopAll(2),call0x8006f878 |
| 3 | (old & 0xfffffff7),store;reload then OR7,store | evtStopAll(0x10),call0x8006f8a8 |
| 4 | old OR0xf | evtStopAll(0xef),call0x8006f8c8 |
| Other signed requests | No requested-level store,no GP mutation | No start/stop request |

For stable GP ownership these leave low nibble0,1,3,7,15 respectively,while
preserving all higher bits. The wraparound rlwinm masks in requests1..3 must not
be mistaken for clearing the whole word. The two GP writes occur before the
stop call;request0's clear occurs before the start call. There is no local event
creation,deletion,pad reset,physical-read suppression or reference-count update.

marioStGetSystemLevel returns **the whole GP word+0x18**,not the separate integer
request at0x8041e940 and not merely the low nibble. Its result for request2 is3
when higher bits are zero. With other GP flags present,it can return a larger
mask. Do not implement it by returning the last enum-like request.

## Context and non-inverse stop/start transitions

[Existing event runtime](EVENT_RUNTIME.md) establishes that evtStartAll/StopAll
select field or battle work using GP+0x14,iterate active entries,and invoke
per-entry relationship traversal/type-mask tests. A matching stopped entry has
flag0x02 set;start clears it. This layer uses bits,not balanced pause-owner counts.
GP+0x18 is global;the selected event context is separate from that word.

A transition to a lower nonzero request does **not** first resume every event
stopped by the former request. For example,request2 then request1 stops type2
then type1;the second call can leave both categories stopped even though the GP
low nibble is now1. Request0 starts mask0xef,which **excludes0x10**. It therefore
does not itself resume a type0x10-only event stopped by request3. Conversely,
request4 stops mask0xef and need not stop type0x10-only events. Which explicit
start/type changes surround these calls is a caller-graph obligation.

The mask test is intersection,not equality:an event whose type contains several
bits can be affected by more than one category. Root evtEntry initialization
assigns type byte0xef at entry+0xc;child/brother creation copies the parent's type
byte in the inspected regions. Later SET_TYPE or other writers can replace it.
Do not assign universal pause categories solely from function or scene names.

The static caller census finds26 direct DOL references and four shipped REL
imports to marioStSystemLevel. DOL owners include map changes,save/item boxes,
locks,item acquisition,game-over,partner abilities,winMain and lecture control.
The imports are four REL24 code bindings to juyoitem_gogo in eki/gor/mri/tou.
These demonstrate shipped callers,not execution conditions or request arguments
at every site. The gor binding is file0x12584;its local code supplies request0.
Other bodies' side effects and actual UI/pause sequences remain open.

## Event speed is a multiplier-backed per-entry value

Let M be the binary32 global at0x804151ec,annotated evtSpd. The initialized DOL
value is1.0 (0x3f800000). evtSetSpeed(entry,S) reads M,computes **single-precision
S*M**,and stores it at entry+0x164. It does not clamp,reject nonpositive values,
reset execution credit+0x168,change entry type/flags or alter pad sampling.
The initial value is not a proof M remains1.0 for every runtime state.

Opcode98 dispatch targets0x800327e4. Its local handler resolves the first operand
with evtGetFloat,then calls evtSetSpeed on the current entry and returns handler2,
allowing further commands in the same interpreter invocation. This is distinct
from changing rendering rate,animation speed or elapsed event lifetime.

Checked creation regions load M directly into entry+0x164:

| Creation routine / store site | Speed policy |
| --- | --- |
| evtEntry /0x8003759c | Load current global M |
| evtChildEntry /0x80037008 | Load current global M;not the parent's speed field |
| evtBrotherEntry /0x80036d0c | Load current global M;not the parent's speed field |

Their checked credit stores initialize+0x168 from original zero constant0x8041fa34.
Therefore an accelerated/slowed parent does not automatically confer that speed
on a newly created child/brother through these fields. Type and other local-work
inheritance are separate operations. These are selected initialization regions,
not a new claim of complete creation-body coverage.

## Scheduler consequences relevant to battle input

The scheduler adds speed+0x164 to credit+0x168 for eligible entries,then invokes
the interpreter while credit>=1,subtracting1 per invocation. An invocation can
execute several immediate commands. Callback result1 yields that invocation;
it does not necessarily end all remaining execution credits in the manager pass.
[Battles sample pads earlier in the main chain](BATTLE_INPUT_CADENCE_AND_SNAPSHOTS.md),
so several eligible invocations can share one private/general pad sample.

Speed0 can prevent new credit accumulation while lifetime still advances for an
otherwise eligible event. Existing credit>=1 can still be consumed. Stopped flag
0x02 instead excludes the event before lifetime and credit increments. These are
different pause semantics. A speed change within an invocation does not retract
already executed commands or prevent handler2 from continuing that invocation.
Negative/unordered rates and hardware floating exceptions are not established as
supported gameplay inputs here;the setter itself contains no validity check.

The census finds only one direct DOL call to evtSetSpeed,the opcode98 handler,
and no module0 REL imports to that routine. This does **not** prove absence of
speed control:REL scripts can execute the opcode rather than importing a C helper.
A scan found no direct r13-relative stfs stores to M in DOL text;that excludes
neither pointer/indexed/bulk writes nor REL code. The census is a discovery aid,
not proof of a constant global or complete event-speed writer coverage.

## Scoped implementation and closure obligations

Preserve requested-level storage,GP mask,getter result and event stopped flags as
separate state. Implement the exact request table without inventing a whole-battle
freeze or start-before-stop transition. Keep global/per-entry speed and existing
credit separate;children/brothers initialize from M rather than parent speed.

Derived checks:request2→1 keeps prior stopped type2 until an explicit matching
start;request3→0 does not by itself clear a type0x10-only stopped event;getter
returns full GP mask with preserved high bits;speed0 retains already accumulated
credit and differs from stopped-event lifetime suppression. Verify creation uses
M even when parent speed differs. These are controlled contract checks,not
observed original menu behavior.

Next recover complete UI/scene request arguments,type writers and balancing start
calls,other M/entry-speed writers,restart policy and actual pause/input traces.
[Input/reset ownership](BATTLE_INPUT_RESET_OWNERS.md) identifies the known sampler
and private-pad mutation routes. No full-game pause or complete-game requirements
closure is justified by this local table alone.
