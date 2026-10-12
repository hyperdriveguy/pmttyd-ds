# Event dispatch fetch,result handling,waits and USER_FUNC continuations

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Scoped evtmgrCmd800316b8/13b8 fetch/common-tail/WAIT_FRAME/USER_FUNC regions;
complete evt_wait_msec80035884/d0. Containing-body fingerprint does not recover
every opcode. Read [manager loop](EVENT_MANAGER_COMPLETE_LOOP.md) and
[ownership](EVENT_RUNTIME.md). Verify: `python3 tools/verify_event_manager_loop.py`.

## Fetch and two levels of return codes

At each dispatch loop read unsigned current opcode entry+a;initialize handler
result2. Opcode>118 skips opcode work and goes directly to common result2 tail.
Otherwise dispatch through119-word table80304fd0. Opcode0 is fetch: store next
pointer+14 into current-command pointer+1a8;read that command word,store low8
opcode into+a,read signed high16 argument count and store low8 into+9. Set
argument pointer+18=old next+4, next pointer+14=argument pointer+4*signedCount,
clear first-call marker+d=0. Handler3 returns to dispatch without clearing opcode.
No local stream/count/opcode/next-pointer validation or instruction-budget cap.

Common handler outcomes:

| Handler result | Dispatcher outcome |
| --- | --- |
| 3 | Repeat dispatch with current opcode, no clear |
| 255 | Return-1 immediately |
| Signed negative | Return1 immediately (manager aborts whole pass) |
| 0 | Return0,preserve current opcode/continuation |
| 1 | Clear opcode+a=0,return0 |
| 2 | Clear opcode+a=0,repeat dispatch in SAME invocation |
| Other nonnegative | Repeat dispatch without opcode clear |

This is the precise USER_FUNC adapter too; “yield” means return from one
dispatcher invocation, not necessarily the manager pass. Opcode2 deletes current
event via evtDelete then handler255. Marker+d clears on FETCH,not in result1/2
tails;next command fetch reinitializes it. Unknown high opcode can fetch its
successor via the default2 tail;this is not a validated-script error policy.

## WAIT_FRAME opcode9 at800318e8

On marker+d==0,resolve operand0 through evtGetValue,store signed word count
at entry+78,mark+d=1. Then ALWAYS process count in the same invocation:
exact0 gives handler2 (immediate fetch/continue). Otherwise decrement with word
wrap,store +78,then exact resulting0 gives handler1;nonzero gives handler0.
Thus count1 decrements to0 and completes/yields on its first invocation;
countN>0 consumes N dispatcher invocations before completion. The subsequent
command executes on another invocation,possibly another credit in the same
manager pass. Negative counts are not clamped: wrapped decrement can eventually
reach0,so there is no immediate <=0 completion rule. Later polls never resolve
operand0 again. Shared timer+78 is reused by other waits.

## WAIT_MSEC opcode10 via evt_wait_msec

At entry capture lifetime high/low +0/+4 BEFORE resolving anything. If marker0,
resolve operand0 as signed word N,store +78,copy captured lifetime high to+7c
and low to+80,mark1. Later polls reuse these stores. Exact N0 returns handler2
before clock/division. Otherwise read clock800000f8 and recompute k with the
same unsigned high-multiply/shift formula as the manager.

Crucial local behavior: subtract retained LOW start word+80 from captured LOW
current lifetime with word arithmetic, sign-extend that32-bit difference into
64 bits,then call __div2u(signExtendedDelta,k),UNSIGNED64 division. Retained
start high+7c is not read on this path. Let q be LOW32 of returned quotient.
The returned handler is exactly
`wrap32((S32(q)>>31) + (U32(N)>>31) + carry(U32(q)>=U32(N)))`.
This spells out the original subc/adde arithmetic,not an unrestricted64-bit
elapsed-millisecond comparison. For valid N>0 and nonnegative elapsed within
the signed32 domain,it returns0 while q<N and1 when q>=N;1 completes/yields.
N0 bypass returns2. Negative/overflow/clock-domain cases are not normalized;
retain instruction/domain behavior rather than guessing immediate success.
Manager lifetime advances once per eligible visit;multicredit polling does not
advance it again. Stopped/waiting/inhibited visits do not advance it. A host
wall-clock timer would alter this behavior. Numerical division/domain/source
validation and actual pause traces remain dependencies.

## USER_FUNC opcode91 at800326fc

On marker0,resolve function operand0 with evtGetValue,cache pointer+98,
advance argument pointer+18 by4, decrement unsigned argument-count byte+9,
mark+d=1. Call cached function(event,1). On later polls skip all that setup and
call cached function(event,0), retaining argument pointer and count. Return its
handler code into the common tail above. Callback arguments may resolve on
polls according to their own bodies;the dispatcher does not snapshot them.
There is no cached-function/null/argument-count validation. First-call0 refers
to a retained USER_FUNC invocation, not the first physical or rendering frame.

Acceptance cases: count0/1/multiple/negative and mutated operand after first
call;MSEC zero bypass and low-word wrap/sign/division;handler0 retained callback,
1 complete/yield,2 same-invocation next command,negative global abort,255 entry
termination;marker reset on fetch and cached function resolution once. Complete
scheduler integration still requires constructor/append/initial-state policy,
remaining opcode consumers and original runtime traces.


## Complete local opcode coverage follow-up (2026-10-12)

[Tail/debug](EVENT_DISPATCH_TAIL_AND_DEBUG_FORMATTING.md) closes113..118,
WAIT_WHILE11 and implicit1/3 no-ops. [All119 coverage](EVENT_OPCODE_IMPLEMENTATION_COVERAGE.md)
links every actual dispatcher target to recovered local contracts. Earlier
“other opcode bodies” partial scope is superseded for this local dispatcher
and linked command helpers. Source/address/callback/platform/numeric/context/
capacity/time/heap domains and original execution remain EVT-01 requirements.
