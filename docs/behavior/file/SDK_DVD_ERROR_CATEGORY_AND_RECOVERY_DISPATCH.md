# SDK DVD error category and recovery dispatch

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete CategorizeError802a07f4/b4hex (45
instructions) and cbForStateGettingError802a08a8/264hex (153). Addresses/offsets/
masks hexadecimal; counts/arithmetic decimal. Static deductions, not original
error/recovery/IO observations. Read
[busy error request](SDK_DVD_BUSY_CALLBACK_ERROR_AND_WHOLE_COMPOSITION.md),
[error/timeout bridge](SDK_DVD_ERROR_CALLBACK_TIMEOUT_AND_FATAL_BRIDGE.md) and
[ready resume](SDK_DVD_STATE_READY_DISPATCH_RESUME_AND_ALARM.md).
Physical error-code meanings/low-level/recovery children remain separate.

## CategorizeError exact full word precedes low24 matching

Input raw error E/r3. EXACT00020400 stores raw E at LastError8041f1a0 and
returns1, without reading/incrementing retry or executing. High-byte variants
with low24==020400 do NOT take that full-word special case.

Otherwise L=E&00ffffff. L062800,023a00,0b5a01 returns0 WITHOUT changing LastError
or NumInternalRetry8041f1a4. No generic fatal/retry behavior should be inferred
from the number0; caller additionally uses the retained full error's high byte.

Other L: read retry FRESH, increment wrap32/store, REREAD retry. Exactly2 compares
L against LastError's RAW fullword, without remasking LastError. Equal stores
L/returns1; unequal stores L/returns2. No retry maximum/saturation or >=2 policy.
At other fresh retry values, first store L to LastError, then L031100 returns2
without executing read. Every other L reads executing8041f170 FRESH and its
command8: exactly5 returns2, all others return3. No command/null/state/extent/
ID/IRQ/generation check. Wrapped retryffffffff→0 follows this non-two branch.

No callback, child service or stack frame. Early category0 preserves previous
last/retry; complete helper assigns only literal categories0..3. Stable mathematical
vectors do not replace the fresh post-increment read or original producer domains.

## Getting-error status16/bit2 before controller read

Callback raw status R/r3. EXACT16 requests error-store(01234568hex) at802a08d4,
DVDReset802a26cc at802a08d8, then cbForStateError(0) at802a08e0; exit.
No local failed-state write before these children. Other R with mask2 set requests
error-store(01234567hex) at802a08f8 and DVDLowStopMotor8029f8a8 with callback
cbForStateError802a06ec at802a0904; exit. Return values ignored.

Remaining statuses read raw controller wordcc006020 as E/GPR29, retain HIGH8
H=E&ff000000 in GPR28, request CategorizeError(E) at802a091c; retain category K
in GPR31. No status/byte-count/controller/field validity check. E/H stay captured
across the classifier, which can mutate LastError/retry or read executing.

K1 immediately stores RAW full E through __DVDStoreErrorCode802a311c at802a0930
and requests stop-motor(cbForStateError) at802a093c; exit WITHOUT testing Canceling.
This preserves high bits, unlike CategorizeError's ordinary LastError low24 store.

## Candidate resume precedes fresh cancellation gate

For K2 or3 candidate resume=0. Every other return uses captured H:
01000000→4,02000000→6,03000000→3,EVERY other high byte→5. Candidate is not written
unless cancellation below is admitted; classify/retain are not a resumed event.

Read Canceling8041f190 FRESH. Nonzero: store candidate ResumeFromHere8041f198
BEFORE capturing executing as C/GPR30. Clear Canceling=0, publish dummy804138e0,
THEN C.statec=10. Nonnull C.callback28 receives(-3,C) at802a09e0.
AFTER that child read CancelCallback8041f194 FRESH; nonnull receives(0,C)
at802a09fc. Request stateReady802a14bc at802a0a00 and exit after return.
No CancelCallback clear, callback-result gate, error/retry rollback or CancelLastError
store. Both callbacks use captured C even if executing changes; ready uses its
own fresh globals/candidate resume contract. Category1 never reaches this gate.

## Noncancel category2/3 versus high-byte recovery

Canceling0 leaves ResumeFromHere untouched locally, then dispatches retained K:

| Category / condition | Request / local state |
| --- | --- |
| 2 | Store RAW E through error service at802a0a24; stateGoToRetry802a0be8 at802a0a28 |
| 3 and E.low24==031100 | Fresh executing.word10 offset to DVDLowSeek8029f744(offset,cbForUnrecoveredError802a0b0c) at802a0a58 |
| 3, other low24 | Fresh LastState8041f1b8 function, fresh executing as sole adapted r3 argument; unchecked indirect request at802a0a6c |
| Other K,H01000000 | Fresh executing C.statec=5; stateMotorStopped802a13a8 at802a0a8c |
| Other K,H02000000 | Fresh executing C.statec=3; stateCoverClosed802a1240 at802a0aac |
| Other K,H03000000 | Fresh executing C.statec=4; stateMotorStopped at802a0acc |
| Other K,every other H | Store fixed01234567 error at802a0adc; stop-motor(cbForStateError) at802a0ae8 |

LastState has NO null/signature/result/current-command check. It can be the
busy function stored by earlier dispatch, but actual writer/ownership domains
must establish that identity. No stable executing pointer captured across
classification/cancellation/seek/last-state/recovery branches. No local OS IRQ,
wait, measured retry limit/timer or physical recovery-success guarantee.

Entry/exit save GPR28..31/LR and32 decimal stack bytes. No independent callback
status assignment. Child/result/callback/alias/lifetime/concurrency/ABI domains
remain separate; low-level seek/stop and recovery functions own later work.

## Evidence and remaining work

[Manifest](../../../tools/sdk_dvd_getting_error_category_evidence.json) pins twelve
spans/all198 words across two complete bodies/seventeen calls/three indirect
sites. Twelve stable category/eight resume/nine route/six status vectors and
exact global/publication/child/callback sites checked; error-wrapper pins
reverified. Generic verifier checks pins/targets, not semantic/provider/runtime
claims. Mathematical deductions separate from original execution. Fresh full
ELFs/listings/generator stay `/tmp`; no bodies/binary assets committed.

Remaining: retry/seek/cover/motor callbacks/reset/error-store/low-level internals,
actual controller/raw error/LastError/retry/command/cancel/resume/executing/
LastState/callback providers and valid value/extent/alias/lifetime/IRQ/concurrency/
ABI domains, original repeated-error/cover/seek/retry/cancel/recovery/callback/
IO/audio traces. EVT-01/native/full-game remain OPEN.
