# SDK DVD ready dispatch, resume and recovery alarm

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete stateReady802a14bc/2e8hex,
186 instructions. Addresses/offsets/masks hexadecimal; counts/arithmetic decimal.
Static deductions, not original pause/command/recovery/IO observations. Read
[queue consumers](SDK_DVD_QUEUE_POP_CHECK_DEQUEUE_AND_LOCAL_COVERAGE.md),
[queue clearing/submission](SDK_DVD_ASYNC_SUBMISSION_AND_QUEUE_APPEND.md) and
[read terminal states](SDK_DVD_READ_BOUNDS_WAIT_WAKE_AND_CLOSE.md).
Child busy/low-level/callback/alarm/cancellation engines remain separate.

## Check before pause; pop before fatal/current-command gates

Request __DVDCheckWaitingQueue802a2f48 at802a14d4. Exactly0 stores executing
8041f170=0 and exits, WITHOUT reading PauseFlag or setting PausingFlag. Every
nonzero result reads PauseFlag8041f17c FRESH. Nonzero pause stores
PausingFlag8041f180=1, THEN executing=0 and exits. No clear of PausingFlag on
nonpaused/empty paths. No popped node or command-state write before these gates.

Pause0 requests __DVDPopWaitingQueue802a2ea8 at802a150c. Read FatalErrorFlag
8041f188 AFTER child but BEFORE storing raw popped result into executing.
No pop0/sentinel/membership/pointer/extent/status check. Future dereferences
can consume that raw published pointer. No local IRQ disable or queue token;
children own the earlier described critical sections.

## Fatal path: state-1, dummy publication, callback-1 and recursion

Fatal flag nonzero: load executing FRESH C, write C.wordc=ffffffff. Reread
executing as C2, publish DummyCommandBlock804138e0 to executing BEFORE reading
C2.callback28. Nonnull callback receives (-1,C2) at802a154c via LR/blrl.
No callback result check, field/extent validation or rollback. Then recurse into
stateReady at802a1550 regardless of callback presence, and exit after return.

Fatal flag not cleared; no current-command write in this branch. Recursion can
consume further queued commands or follow callback-created state. No local depth/
cycle/iteration guard; original callback/list/provider/ABI domains required.

## Fresh normal command and two ResumeFromHere reads

Fatal0: read executing C FRESH, load C.word8 into CurrCommand8041f18c. Read
ResumeFromHere8041f198. Exactly0 goes to MotorState gate below. Nonzero REREADS
ResumeFromHere for the signed switch. If this second value is0, it takes the
switch's default clear/exit rather than normal motor processing.

C retained in r4 supplies initial state stores in resume branches; later command/
executing/global loads are fresh where stated. No normalized resume enum or
stable state snapshot. On return from child/recursive requests, entered resume branches store
ResumeFromHere=0 at802a16c0 AFTER those requests, then exit.

| Second resume word | State/action before common clear |
| --- | --- |
| 2 | C.statec=11; DVDLowWaitCoverClose(cbForStateMotorStopped802a13d0) at802a15bc |
| 3 | C.statec=4; same wait/callback at802a15d4 |
| 4 | C.statec=5; same wait/callback at802a15ec |
| 1,6,7 | C.statec=3; fresh CurrCommand split below |
| 5 | Fresh CancelLastError8041f19c to __DVDStoreErrorCode802a311c at802a16ac; then DVDLowStopMotor(cbForStateError802a06ec) at802a16b8 |
| EVERY other raw word | No state/child write before common resume clear |

Low-level callback pointers are linked identities, not locally invoked by these
requests. Return values/status ignored. No measured cover/motor/recovery time.

## Resume1/6/7 special command split

After state3 store, read CurrCommand FRESH. Exactly4,5,13,15 requests
__DVDClearWaitingQueue802a2e08 at802a162c. This clears head references, not every
queued command's links/state/callback. AFTER child read executing FRESH C2,
publish dummy804138e0, read C2.callback28, nonnull receives (-4,C2) at802a1650.
Then recurse at802a1654. ResumeFromHere is cleared ONLY AFTER recursion returns.
The child therefore can see the same nonzero resume value unless a callback/
provider changed it; no stable unconditional one-level recursion assumption.

Every other raw command, including negatives/unrecognized values, takes reset/
alarm below. No command-specific extent/SDK-state/resource check here.

## Normal MotorState gate and separate special-command path

First resume read0: read MotorState8041f1b0. Exactly0 stores C.statec=1 using
retained C, then reads executing AGAIN for stateBusy802a17a4 at802a16e0. No
pointer equality/current-command/status check between state write and fresh
argument load. Child owns busy execution; no independent callback/status result.

MotorState nonzero: fresh CurrCommand4,5,13,15 clears queues at802a1718, reads
fresh executing C2, publishes dummy, requests callback(-4,C2) at802a173c when
nonnull, recurses at802a1740 and exits. Unlike resume branch, no local state3
store or later ResumeFromHere clear on this path. All other commands use reset/
alarm below. MotorState is not normalized to a specific enum in this gate.

## Two reset/alarm regions share exact raw arithmetic

Resume reset path802a165c and normal-motor reset path802a1748:

1. Store MotorState=0 BEFORE DVDReset802a26cc at802a1664 or802a1750.
2. OSCreateAlarm80295cfc(ResetAlarm80413910) at802a166c or802a1758.
3. Read bus-clock word800000f8 AFTER alarm child. Compute raw low ticks below.
4. OSSetAlarm80295f5c with r3=ResetAlarm, r5=0 high ticks, r6=low ticks,
   r7=AlarmHandler802a11fc at802a16a0 or802a178c.

Exact source calculation for unsigned clock B:

    Q = B >> 2
    H = high32(Q * 10624dd3hex)
    lowTicks = wrap32((H >> 6) * 1150)
    highTicks = 0

No minimum/maximum/clock-domain/overflow correction or measured seconds mapping.
Source arithmetic vectors do not establish actual hardware clock, timer units or
original delay. All request returns ignored; callbacks/SDK alarm scheduling,
DVD reset and error recovery must be recovered separately.

Resume-path common clear of ResumeFromHere follows OSSetAlarm; normal-motor
path exits without that store. Callback/global changes during children therefore
must remain visible rather than replaced by one captured transaction.

## ABI, evidence and remaining work

Entry/exit save GPR31/LR and16 decimal stack bytes. No independent status
assignment, OS IRQ request or successful-disc/read-byte proof in this body.
Dummy publication and recursive callbacks are not native completion events.

[Manifest](../../../tools/sdk_dvd_state_ready_evidence.json) pins fourteen spans,
all186 body words/nineteen direct calls, three indirect callback sites/six
callback bindings. Six admission/ten resume-switch/nine command/five two-read/
five alarm vectors and global/state/callback/recursion sites checked; queue
pins reverified. Generic verifier checks pins/targets, not semantic/provider/
runtime claims. Mathematical deductions separate from original execution.
Fresh full ELF/listing/generator stay `/tmp`; no bodies/assets committed.

Remaining: stateBusy/low-level wait/stop/reset and three callback bodies, actual
queue/command/fatal/pause/resume/motor/error/alarm/clock/executing/dummy/state/
callback providers and valid value/extent/alias/lifetime/IRQ/concurrency/ABI
and numeric/time domains, cancellation/thread/path/open and original pause/
recursive-callback/resume/recovery/command/IO/audio traces. EVT-01/native/full-
game remain OPEN.
