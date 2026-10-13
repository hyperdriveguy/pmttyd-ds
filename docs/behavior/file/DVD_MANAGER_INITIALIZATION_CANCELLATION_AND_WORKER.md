# DVD manager initialization, cancellation and worker loop

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete DVDMgrInit80145308/90hex (36
instructions), DVDMgrDelete801452cc/3chex (15), dvdmgr.o proc_main80145398/18hex
(6). Addresses/offsets/masks hexadecimal; counts/arithmetic decimal.
Static source deductions, not original allocation/thread/IO observations. Read
[sorted worker service](DVD_MANAGER_SORTED_SERVICE_RESULTS_AND_CLOSE.md),
[slot/open/global callback](DVD_MANAGER_SLOT_OPEN_AND_GLOBAL_CALLBACK.md) and
[synchronous yield wait](DVD_MANAGER_SYNCHRONOUS_READ_PROTOCOL.md).

## Queue allocation is published before clearing and thread creation

Init creates16 decimal stack bytes/saves LR. Request
__memAlloc800300f0(heap0,size9800hex=38912) at80145320. This equals256 records
of98hex/152decimal. Store RAW returned pointer at dvdq8041ebf8 at80145328 BEFORE
requesting memset800050b4(returnedPointer,0,38912) at80145334. No null/failure/
old-pool/thread-state/queue-domain check, old queue free or slot close.

Global _callback8041ebf4 and thread-on8041ebf0 are not cleared by these writes.
Repeated initialization can overwrite an existing queue pointer; no replacement
ownership/rollback protocol appears locally. Valid allocator/heap/name/record/
extent/lifetime/address/alias/thread domains remain separate. A null allocator
result is passed to memset without a local guarded return.

## Exact thread request and zero-result self-loop

Request OSCreateThread8029cd30 at80145364 with:

| ABI argument | Value |
| --- | --- |
| thread object |803e5bb8 dvdmgr_thread |
| entry |80145398 dvdmgr.o proc_main |
| entry argument |0 |
| stack top |803e5bb8 |
| stack size |16384 decimal |
| priority |16 decimal |
| attributes |1 |

Backing stack symbol803e1bb8 has4000hex bytes, ending at the requested stack top,
which numerically equals the adjacent thread object's address. No local stack/
object extent/alignment/SDK/attribute validation. Interpret original scheduler/
attributes/SDK object initialization through their own contracts, not guesses.

OSCreateThread result exactly0 branches to80145370 SELF-LOOP forever. No yield,
retry, queue free, flag clear, error callback or normal epilogue on that branch.
A preexisting thread-on flag stays whatever it was. No executed failure-spin
trace is claimed; this is the source's unconditional branch to itself.

EVERY nonzero result, including negative raw values, stores fullword1 at
thread-on8041ebf0 BEFORE requesting OSResumeThread8029d1b8(thread object)
at80145384. No return/status check, success normalization, queued-slot scan or
callback setup. Resume result is not interpreted; normal epilogue restores LR/
16 decimal stack bytes. No independent success/failure return assignment.

## Delete tests only thread-on and never releases queue/slots

DVDMgrDelete reads fullword thread-on once. Exactly0 skips cancellation and
all flag stores. Nonzero requests OSCancelThread8029cffc(thread object) at
801452ec, then stores fullword0 to thread-on at801452f4 regardless of child's
return/status/mutations. No thread join/wait/cancel-success predicate locally.

No queue free/zero/null, per-slot DVDClose, read cancellation service, callback
clear, stack release or repeated-init protection. Do not turn the function name
into a full resource destructor. Valid SDK cancellation/concurrency/lifetime
semantics remain open. Entry/exit save LR and16 decimal stack bytes; no independent
status assignment. No IRQ request or global callback invocation in this body.

## Worker calls main then yield indefinitely

proc_main creates16 decimal stack bytes and saves LR once; ignores incoming
argument. Infinite source loop:

    DVDMgrMain80145024 at801453a4
    OSYieldThread8029ccf4 at801453a8
    branch801453ac back to801453a4

No thread-on/queue/null/stop/status polling, sleep/timer/frame credit, callback
check or local return path. Each completed main/yield pair reaches another main
when execution resumes. Cancellation affects this only through SDK/external
state; writing thread-on0 alone is not a worker exit branch.

Finite call-prefix vectors are mathematical invocations, not a measured physical
poll rate or proof that yield returns once per game tick/retrace. Main's sorted
snapshot/read/result/close/global callback behavior is in the linked complete
contract. The worker does not adapt synchronous waiters into direct reads.

## Evidence and remaining work

[Manifest](../../../tools/dvd_manager_lifecycle_worker_evidence.json) pins ten
spans/all57 words across three complete bodies/seven calls, exact thread args
and publication/backedge sites. Five creation/four delete/four finite worker-
prefix vectors checked; main pins reverified. Namespace-specific worker identity
avoids other proc_main aliases in the CSV. Generic verifier checks pins/targets,
not semantic/provider/runtime claims. Mathematical source deductions separate
from original execution. Fresh full ELFs/listings/generator stay `/tmp`; no
bodies/binary assets committed. Local whole lifecycle wrappers/loop answered.

Remaining: allocator/thread SDK/cancellation/yield/read/close/path/fileInfo and
actual startup/shutdown/queue/slot/thread-on/callback/request/name/heap providers
and valid value/extent/alias/lifetime/IRQ/concurrency/ABI domains; original
initialization-failure/cancel/reinit/yield/sorted-service/error/short-read/callback/
IO/audio traces. EVT-01/native/full-game remain OPEN.
