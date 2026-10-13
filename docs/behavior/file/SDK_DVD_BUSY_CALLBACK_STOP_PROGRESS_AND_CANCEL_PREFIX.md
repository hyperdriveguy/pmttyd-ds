# SDK DVD busy callback stop, progress and cancellation prefix

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Scoped cbForStateBusy802a1ae4..802a1ce8
exclusive (129 instructions), plus return ABI802a2124..802a213c (6).
Whole callback802a1ae4/658hex remains in progress; success/error continuation
is open. Addresses/offsets/masks hexadecimal; counts/arithmetic decimal.
Static deductions, not original controller/cancel/IO/audio observations. Read
[busy requests](SDK_DVD_STATE_BUSY_COMMAND_DISPATCH_AND_READ_CHUNKS.md),
[ready recursion](SDK_DVD_STATE_READY_DISPATCH_RESUME_AND_ALARM.md) and
[SDK read terminal states](SDK_DVD_READ_BOUNDS_WAIT_WAKE_AND_CLOSE.md).

## Exact status16 overrides every command gate

Input raw status R/r3; no fileInfo/command pointer argument is consumed here.
Save GPR30/31/LR and24 decimal stack bytes. R EXACTLY16 requests
__DVDStoreErrorCode802a311c(01234568hex) at802a1b10, DVDReset802a26cc at802a1b14,
then directly cbForStateError802a06ec(0) at802a1b1c; exit afterward. No local
progress/command/cancel gate, status-bit interpretation or failed-child rollback.
Child error/reset semantics and original controller-status domains remain open.
Other raw statuses continue; status18 is not this exact16 branch.

## Two current-command reads select stop3/15

Read CurrCommand8041f18c; exactly3 enters stop branch without the second read.
Otherwise reread it, exactly15 enters stop; all other values go to DMA/progress
route below. Do not replace these reads with one captured command.

Stop branch tests R mask2. Set: store error code01234567 through the error
service at802a1b4c, then request DVDLowStopMotor8029f8a8 with callback
cbForStateError802a06ec at802a1b58; exit. No retry-clear/state/complete/cancel
callback/progress publication before those requests. Callback entry is passed,
not directly invoked here. No local result check or signed-error generalization.

Mask2 clear (even R0): store NumInternalRetry8041f1a4=0. Read CurrCommand AGAIN;
exactly15 sets ResetRequired8041f1a8=1; other values leave prior reset flag.
No success-bit1 requirement or current-command normalization precedes these writes.

## Stop cancellation sets resume7; otherwise state7 waits for cover

Read fullword Canceling8041f190. Nonzero:

1. Capture executing8041f170 as C/GPR30; store ResumeFromHere8041f198=7.
2. Clear Canceling=0, publish DummyCommandBlock804138e0 to executing,
   THEN store C.statec=10.
3. Read C.callback28; nonnull requests (-3,C) at802a1bc4 using LR/blrl.
4. Read global CancelCallback8041f194 FRESH AFTER that child; nonnull requests
   (0,C) at802a1be0. It may differ after the command callback.
5. Request stateReady802a14bc at802a1be4; afterward exit this callback prefix.

No null/identity/extent/ABI check, callback-result gate, CancelCallback clear or
progress adjustment. Captured C remains the two callbacks' command argument,
while global executing can change. Resume7 is visible to ready recovery.

Canceling exactly0: read executing FRESH C2, store C2.statec=7, then
DVDLowWaitCoverClose8029f7d8(cbForStateMotorStopped802a13d0) at802a1c10; exit.
No ResumeFromHere7 write on this path, dummy publication or complete callback.
Actual stop/cover/cancel success and callback/IRQ scheduling remain separate.

## Non-stop DMA eligibility precedes status8 cancellation

Read CurrCommand FRESH after stop routing. Values1,4,5,14 admit progress update.
EVERY other value reads raw DmaCommand8041e484; equality with that global also
admits. No immutable original-command snapshot, command-field/state/ID/type
check or local bound. DmaCommand producer/optional-command ownership remains open.

Admitted: load executing FRESH C. Read controller wordcc006018 remaining count,
C.word1c last chunk and C.word20 prior progress. Store
C.word20=wrap32(priorProgress+wrap32(lastChunk-controllerRemaining)).
One retained C supplies these fields. No controllerRemaining<=lastChunk,
signed-range/capacity/alignment/actual-byte-count validation or count clamp.
Underflow/wrap possible; e.g. chunk32/remaining33/prior100 stores99.
No word1c decrement or destination/address update in this prefix.

This publication occurs BEFORE testing R mask8, and can therefore update a
command whose cancellation callbacks run next. Source MMIO read/ordering is
pinned; physical register meaning/producer validity/short-read accuracy is not
established by a variable label or a host vector.

## Non-stop mask8 takes cancellation without testing Canceling

After admitted/skipped progress, R mask8 SET captures executing C, clears
Canceling=0, publishes dummy, THEN C.statec=10. Read C.callback28 and request
(-3,C) at802a1cc0 when nonnull. Read CancelCallback FRESH after that child,
request (0,C) at802a1cdc when nonnull. Request stateReady at802a1ce0; exit.

This path does NOT test old Canceling and does NOT set ResumeFromHere7.
Prior resume state is untouched locally. Command callback before fresh global
cancel callback is preserved; results/ownership/generation/queue/extent not checked.
Mask8 clear continues at802a1ce8, where success-bit/error logic begins; that
continuation is not answered here. No local IRQ service/thread wait in this prefix.

## Evidence and remaining work

[Manifest](../../../tools/sdk_dvd_busy_callback_prefix_evidence.json) pins nine
spans/all135 region/ABI words/eight direct calls/four indirect callback sites.
Eight exact-status/two-read-stop/seven DMA/six wrapped-progress/six prefix-cancel
vectors and global/publication/callback sites checked; busy pins reverified.
Models describe this prefix, not later success/error cancellation paths.
Generic verifier checks pins/targets, not semantic/provider/runtime claims.
Mathematical deductions separate from original execution. Fresh full callback
ELF/listing/scoped generator stay `/tmp`; no bodies/assets committed.

Remaining: whole callback success/error continuation, reset/error/cover callback
and low-level internals, actual raw status/controller remaining/command/chunk/
progress/DMA/cancel/resume/retry/reset/executing/dummy/callback providers and
valid value/extent/alias/lifetime/IRQ/concurrency/ABI domains, original stop/
cover/cancel/short-read/progress/recursive-callback/IO/audio traces.
EVT-01/native/full-game remain OPEN.
