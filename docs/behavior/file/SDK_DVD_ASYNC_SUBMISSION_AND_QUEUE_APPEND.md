# SDK DVD async submission and waiting-queue append

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete DVDReadAbsAsyncPrio802a21f8/dchex
(55 instructions), __DVDPushWaitingQueue802a2e40/68hex (26),
__DVDClearWaitingQueue802a2e08/38hex (14). Addresses/offsets/masks hexadecimal;
counts/arithmetic decimal. Static deductions, not original command/IO traces.
Read [synchronous request/wait](SDK_DVD_READ_BOUNDS_WAIT_WAKE_AND_CLOSE.md) and
[manager service](DVD_MANAGER_SORTED_SERVICE_RESULTS_AND_CLOSE.md).
Command execution/queue pop/check/dequeue remain separate.

## Ordered command writes precede invalidation and IRQ admission

Raw inputs C/r3 command block, D/r4 destination, N/r5 count, O/r6 absolute offset,
B/r7 callback, P/r8 priority. Retain C/P; no null/type/state/duplicate membership/
priority/index/length/buffer-capacity/alignment/generation check. Store in order:

| C word offset | Value |
| --- | --- |
| 8 | command1 |
| 18 | raw D |
| 14 | raw N |
| 10 | raw O |
| 20 | 0 completion value |
| 28 | raw B |

No state+c write yet. These stores precede the local interrupt disable. Callback/
command buffer aliases or external observers require provider evidence; no atomic
command snapshot or rollback protocol is enforced.

Read fullword autoInvalidation8041e474. Exactly0 skips invalidation. Nonzero:
read command8 FRESH; values1,4,5,14 admit DCInvalidateRange80296a28(fresh
C.word18,fresh C.word14) at802a226c. Every other command skips. Under stable
ordinary fields command remains the previously stored1; do not replace the fresh
predicate in an original contract. No data-copy, cache flush/store or result check.

## Waiting state, queue append, then optional command-engine kick

Request OSDisableInterrupts80299ad4 at802a2270 and retain returned token. Store
state C.wordc=2 at802a2278, then request __DVDPushWaitingQueue(P,C) at802a2288.
Retain RAW child return across the following operations, not a normalized bool.
No conditional request rollback, command-state reset or priority truncation.

AFTER push, read global executing8041f170. Nonzero skips ready service without
reading PauseFlag. Exactly0 reads fullword PauseFlag8041f17c; exactly0 requests
stateReady802a14bc at802a22a8, other values skip. This gate is independent of the
push result and rereads global state after insertion. stateReady owns actual
selection/execution/callback/result behavior; this call alone proves no IO.

Then OSRestoreInterrupts80299afc(savedToken) at802a22b0, return retained push
result. Ignore ready/restore returns. Epilogue restores GPR29..31/LR and56 decimal
stack bytes. No shared game counter; nested IRQ services have their own tokens.

## Four circular heads reset without IRQ/child service

WaitingQueue base is80413938,32 bytes/four8-byte heads.
__DVDClearWaitingQueue sets both next word0 and prev word4 to each head's own
address, in head0..3 order:80413938,80413940,80413948,80413950. No traversal of
prior nodes, command-state reset, callbacks, heap free, IRQ guard or allocation.
This discards head references rather than unlinking old command records. Actual
initialization caller/timing/ownership domains remain open. Leaf has no stack
frame or explicit independent status assignment.

## Push raw priority selects head; ordered writes reread tail

__DVDPushWaitingQueue receives P/C, retains both, requests OSDisableInterrupts
at802a2e5c. H=wrap32(80413938+wrap32(P<<3)); no0..3 bound, signed rejection,
clamp/byte narrowing/duplicate-node/state/callback/null guard. P4 addresses the
next8 bytes outside the declared head array; P20000000hex wraps to head0.
Actual valid priority/address domains require provider evidence.

Read H.word4 tail T0. Ordered writes:

1. T0.word0=C;
2. read H.word4 AGAIN as T1; C.word4=T1;
3. C.word0=H;
4. H.word4=C.

For ordinary empty circular queue T0=T1=H, these link C as both first/last node.
For nonempty stable queue, append after old tail. But T0/T1 need not match under
aliases: T0=H+4 makes first write change H.prev, so T1=C and C.prev=C. Preserve
source reread/write order rather than a retained-tail update. Valid queue/command/
address/alias/lifetime domains must establish actual list invariants.

Token from disable remains in volatile r3 throughout these local arithmetic/
loads/stores; no child intervenes. Restore it at802a2e88, assign1 at802a2e90,
restore GPR30/31/LR and24 decimal stack bytes. Always1 on normal local return;
no queue-capacity/failure branch. Thus the composed submission wrapper normally
returns1 too, even if stateReady changes command state before return. The caller's
zero-result branch is not evidence of a local queue-rejection path.

## Evidence and remaining work

[Manifest](../../../tools/sdk_dvd_submit_queue_evidence.json) pins seven spans,
all95 words across three complete bodies/seven calls. Eight invalidation/five
ready/seven priority/three append models, six ordered command stores/four head
resets and exact publication sites checked; read-wrapper pins reverified.
Alias vectors are mathematical byte-address models, not observed original queue
states. Generic verifier checks pins/targets, not semantic/provider/runtime
claims. Fresh full ELFs/listings/generator stay `/tmp`; no bodies/assets committed.

Remaining: stateReady/command engine, queue pop/check/dequeue, actual command/
queue/priority/autoInvalidation/executing/PauseFlag/callback/state/token providers
and valid value/extent/alias/lifetime/IRQ/concurrency/ABI domains, cancellation/
thread/path/open services and original insertion/order/pause/command-state/
cache/callback/IO/audio traces. EVT-01/native/full-game remain OPEN.
