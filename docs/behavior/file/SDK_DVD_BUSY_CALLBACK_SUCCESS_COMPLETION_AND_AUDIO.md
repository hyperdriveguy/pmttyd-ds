# SDK DVD busy callback success, completion and audio

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Scoped cbForStateBusy802a1ce8..802a2000
exclusive,198 instructions, plus ImmCommand803c2e34/c hex (three words).
Whole callback/error tail remains in progress. Addresses/offsets/masks hexadecimal;
counts/arithmetic decimal. Static deductions, not original command/IO/audio traces.
Read [prefix/progress/cancellation](SDK_DVD_BUSY_CALLBACK_STOP_PROGRESS_AND_CANCEL_PREFIX.md),
[busy read/audio requests](SDK_DVD_STATE_BUSY_COMMAND_DISPATCH_AND_READ_CHUNKS.md)
and [ready recursion](SDK_DVD_STATE_READY_DISPATCH_RESUME_AND_ALARM.md).

## Local success gate and command16 before Canceling

Entry retains raw status R/r3 after prefix gates; no new command argument.
R mask1 CLEAR jumps to error continuation802a2000, not answered here.
Set: store NumInternalRetry8041f1a4=0 before reading CurrCommand8041f18c.
Local gate models assume prefix reaches this point; they do not claim every
raw status reaches success under the prefix's higher-priority branches.

Fresh command EXACT16: load executing8041f170 as C; store MotorState8041f1b0=1,
publish DummyCommandBlock804138e0, then C.statec=0. Read C.callback28; nonnull
requests (0,C) at802a1d30. Request stateReady802a14bc at802a1d34 regardless of
callback presence; exit. NO Canceling read/clear or ResumeFromHere write here.
Pending cancellation can survive this command16 path; do not reorder gates.

Other command values read Canceling8041f190. Nonzero stores ResumeFromHere
8041f198=0 BEFORE capturing executing as C/GPR30. Clear Canceling, publish dummy,
then C.statec=10. Nonnull C.callback28 receives (-3,C) at802a1d7c; AFTER that
child read CancelCallback8041f194 FRESH and nonnull receives (0,C) at802a1d98.
Ready at802a1d9c then exit. No callback result check/CancelCallback clear.
This resume0 write differs from prefix non-stop status8 cancellation.

Canceling0 continues command classification. No command/record/state/extent/
callback/ID/generation/IRQ/ownership check precedes these global dereferences;
actual alias/lifetime/concurrency/ABI/token/provider domains remain open.

## DMA classification uses fresh command/custom identity

Read CurrCommand AGAIN. Values1,4,5,14 classify DMA. Other values read raw
DmaCommand8041e484 and exact equality classifies DMA too. Command can differ
from the earlier command16/cancel gate or prefix progress classification.
No stable immutable command snapshot/enum/range normalization.

DMA: load executing FRESH C; read progress C.word20 and total C.word14.
Only EXACT unsigned word equality completes. ANY inequality, including progress
GREATER than total, requests stateBusy802a17a4(C) at802a1e08 and exits. No clamp,
progress correction, additional status/Canceling gate or ready request. Child
owns chunk selection/read request; malformed domains can still take that branch.

Equal: publish dummy, then C.statec=0. Read C.callback28; when nonnull, reread
C.word20 as raw first callback argument, C as second at802a1e38. Ready at
802a1e3c regardless of callback presence; exit. No clear of total/progress,
callback pointer or queue resources. Prefix controller arithmetic supplied
progress; equality alone is not physical-byte correctness evidence.

## Non-DMA immediate result classification and raw controller value

Read CurrCommand FRESH. Commands9..12 classify immediate via unsigned
wrap(command-9)<=3. Every other value compares against three ImmCommand words
IN ORDER at803c2e34/38/3c, stopping after first equality. Initial original table
contains threeffffffff values; it is mutable data, not a permanently validated
command set. Equality/read/provider mutation domains remain separate.

Classified immediate: read CurrCommand AGAIN; exactly11 skips second command
read and uses shifted result; otherwise reread and exactly10 uses shifted result.
Other values use raw result. Read controller wordcc006020: shifted first argument
is wrap32(word<<2), otherwise raw word. No signed status/range/units normalization.
For example word80000001hex shifts to4, not an overflow error.

AFTER controller read, load executing FRESH C, publish dummy, C.statec=0;
nonnull C.callback28 receives (retainedControllerResult,C) at802a1f00.
Ready at802a1f04, then exit. No word20 result store. Do not substitute a single
captured command or infer controller field meaning without provider evidence.

## Command6: status probe/reissue versus completion

Not DMA/immediate: fresh CurrCommand exactly6 enters audio branch; other values
use generic completion below. Load executing C and C.word1c last-chunk/tracking
value, retaining its address for a possible write.

Tracking0: read controller cc006020 and test LOW BIT mask1.
Set: publish dummy, C.statec=9, nonnull C.callback28 receives (-2,C) at802a1f60;
ready at802a1f64, exit. AutoFinishing and tracking value untouched locally.

LOW BIT CLEAR: store AutoFinishing8041f184=0, THEN captured C.word1c=1.
Reload executing FRESH C2 for count14/absolute offset10. Request
DVDLowAudioStream8029fa5c(0,C2.word14,C2.word10,cbForStateBusy802a1ae4)
at802a1f94. No C/C2 equality guard, state/dummy/complete callback/ready request
on this reissue path. Ignore child return. Tracking write and request operands
can belong to different global command snapshots.

Tracking NONZERO: publish dummy, C.statec=0; nonnull callback receives (0,C)
at802a1fc0; ready at802a1fc4, exit. No controller-bit read or tracking reset.
No original streaming/position/audio completion semantics are claimed by these
local branch names; low-level/callback/provider contracts remain separate.

## Generic completion and callback ordering

All remaining non-DMA/non-immediate/non6 commands load executing FRESH C,
publish dummy, C.statec=0; nonnull C.callback28 receives (0,C) at802a1ff4;
ready at802a1ff8, exit. No optional command rejection/status/ID check here.
Fresh global/record callback reads and dummy-before-state/callback order matter
under aliases or child mutation. Ready can run recursive callback/queue work;
there is no local recursion limit or callback-success-dependent gate.

All completed paths reach shared epilogue802a2124 described by the prefix.
No independent status return, local IRQ service/thread wait, payload copy or
resource release in this region. Error continuation starts at802a2000 when
local success mask1 was clear.

## Evidence and remaining work

[Manifest](../../../tools/sdk_dvd_busy_callback_success_evidence.json) pins six
spans/all198 region words plus three table words (201 anchors),nine direct calls,
eight indirect callback sites and table initial values. Five local-gate/five
precedence/five DMA-equality/seven immediate/five controller-result/four audio
vectors and exact publication/call/field sites checked; prefix pins reverified.
Generic verifier checks pins/targets, not semantic/provider/runtime claims.
Mathematical deductions separate from original execution. Fresh full callback
ELF/listing/scoped generator stay `/tmp`; no bodies/assets committed.

Remaining: whole error tail/composition, actual status/command/DMA/table/controller/
progress/total/motor/cancel/resume/auto/executing/dummy/callback providers and valid
value/extent/alias/lifetime/IRQ/concurrency/ABI domains, low-level/reset/error/
thread/cancel and original chunk/short-read/immediate/audio/reissue/recursive-
callback/IO/audio traces. EVT-01/native/full-game remain OPEN.
