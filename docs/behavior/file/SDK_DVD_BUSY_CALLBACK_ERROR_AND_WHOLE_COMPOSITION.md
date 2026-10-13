# SDK DVD busy callback error tail and whole composition

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Scoped cbForStateBusy802a2000..802a2124
exclusive,73 instructions. Combined with
[prefix/return ABI](SDK_DVD_BUSY_CALLBACK_STOP_PROGRESS_AND_CANCEL_PREFIX.md) and
[success/audio](SDK_DVD_BUSY_CALLBACK_SUCCESS_COMPLETION_AND_AUDIO.md), this
closes the whole local406-instruction callback802a1ae4/658hex. Addresses/offsets/
masks hexadecimal; counts/arithmetic decimal. Static deductions, not original
error/short-read/IO observations. Child error/recovery/low-level internals remain
separate. Read [busy dispatch](SDK_DVD_STATE_BUSY_COMMAND_DISPATCH_AND_READ_CHUNKS.md)
and [ready recursion](SDK_DVD_STATE_READY_DISPATCH_RESUME_AND_ALARM.md).

## Error tail can complete without local success bit

This tail is reached when prefix branches did not exit and local status mask1
was CLEAR at802a1ce8. Earlier prefix can have updated completion word20 from
controller remaining bytes BEFORE that mask test. No new command-pointer/input
argument, status-domain validation, local IRQ service or retry-clear here.

Read CurrCommand8041f18c FRESH. EXACT14 requests
__DVDStoreErrorCode802a311c(01234567hex) at802a2014, then
DVDLowStopMotor8029f8a8(cbForStateError802a06ec) at802a2020; exit. No equality,
Canceling, callback/completion/progress check protects this branch. Unlike
success-mask route, inquiry14 gets this error path even if progress equals total.
Child status/physical error meaning is not inferred from the raw code literal.

## Four separate command reads and exact progress equality

If first command read was not14, perform these fresh reads in order, stopping
on the first matching value: command==1, fresh command==4, fresh command==5,
fresh command==14. No DmaCommand custom equality in this tail, unlike prefix/
success DMA classification. If all fail, request drive error as below.

A later14 match can enter equality processing although first14 would have
already taken stop/error. Preserve the separate reads rather than one captured
command enum. No command/descriptor/generation/ownership snapshot guarantee.

Matched classification: load executing8041f170 FRESH C/GPR30; read C.word20
progress and C.word14 total. EXACT equality admits completion/cancel below;
EVERY inequality, including progress>total, requests drive error. There is no
read-chunk reissue on this tail's inequality, unlike success branch. Equality
can complete despite local success bit clear; it is not physical-byte correctness
or a general controller-error override guarantee without actual providers.

## Equal progress: Canceling precedes completion, resets resume0

Read Canceling8041f190. Nonzero:

1. Store ResumeFromHere8041f198=0, then Canceling=0.
2. Publish DummyCommandBlock804138e0 to executing, then captured C.statec=10.
3. Nonnull captured C.callback28 receives (-3,C) at802a20ac using LR/blrl.
4. Read CancelCallback8041f194 FRESH AFTER that child; nonnull receives (0,C)
   at802a20c8. No CancelCallback clear or retained callback-pointer snapshot.
5. Request stateReady802a14bc at802a20cc and exit after return.

No callback-result gate, old-state/result/extent check, progress rollback or
payload operation. Captured C remains callback argument even if global executing
changes. Original callback/alias/lifetime/ABI/scheduling domains remain open.

Canceling0: reload executing AGAIN as C2 (can differ from equality-tested C),
publish dummy, C2.statec=0. Read C2.callback28; when nonnull reread C2.word20 as
raw first callback argument, C2 second, at802a210c. Request stateReady at
802a2110 regardless of callback presence; exit. No C/C2 equality or second
progress/total check, completed-word clear, callback reset or resource release.
A mathematical raw progressffffffff is passed unchanged, not clamped to bytes/
signed status. Fresh provider values require valid domains and original traces.

## Other cases request drive error with fixed callback

Unrecognized four-read command classification OR recognized progress!=total
requests DVDLowRequestError8029f934(cbForStateGettingError802a08a8)
at802a2120. No further status-bit2/general-negative gate; no direct command/
state/Canceling/dummy/callback/error-code write before this request. Ignore
return, exit. Callback entry is passed, not locally invoked here. Child owns
actual error fetch/classification/retry/fatal/recovery behavior.

Shared epilogue restores GPR30/31/LR and24 decimal stack bytes. Whole callback
has no independent status assignment, thread wait, OS IRQ request or physical
IO-success claim. All indirect command/cancel callbacks are conditional nonnull;
actual signature/ownership/global mutation remains separately required.

## Whole local body and evidence

| Manifest | Disjoint callback region / words |
| --- | --- |
| [Prefix](../../../tools/sdk_dvd_busy_callback_prefix_evidence.json) | 802a1ae4..802a1ce8:129;802a2124..802a213c return ABI:6 |
| [Success](../../../tools/sdk_dvd_busy_callback_success_evidence.json) | 802a1ce8..802a2000:198;its three table anchors are outside callback |
| [Error/composition](../../../tools/sdk_dvd_busy_callback_finish_composition_evidence.json) | 802a2000..802a2124:73 |

All406 instructions covered without gap/overlap, checked against read-only ISO;
22 direct calls/15 indirect sites across complete local body. Error manifest
pins eight spans/all73 new anchors/five calls/three indirect sites/two fixed
callback bindings and whole-body hash. Eight error-routing/three raw callback-
result models and publication/command/state sites checked; earlier region
manifests reverified. Generic verifier checks pins/targets, not semantic/provider/
runtime claims. Mathematical deductions separate from original execution.
Fresh full callback ELF/listing/scoped generator stay `/tmp`; no bodies/assets
committed. Local whole cbForStateBusy is answered, not transitive/native fidelity.

Remaining: cbForStateGettingError/cbForStateError and low-level/reset/cover/cancel
internals, actual raw status/controller/command/DMA/progress/total/executing/
cancel/resume/dummy/callback/providers and valid value/extent/alias/lifetime/IRQ/
concurrency/ABI domains, original inquiry/equality/error/short-read/cancellation/
recovery/recursive-callback/IO/audio traces. EVT-01/native/full-game remain OPEN.
