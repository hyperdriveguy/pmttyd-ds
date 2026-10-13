# SDK DVD error callback, timeout and fatal-handler bridge

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete cbForStateError802a06ec/achex
(43 instructions), stateTimeout802a0798/34hex (13),
__DVDPrintFatalMessage802a3290/30hex (12). Addresses/offsets/masks hexadecimal;
counts/arithmetic decimal. Static deductions, not original fatal UI/IRQ/IO traces.
Read [busy callback](SDK_DVD_BUSY_CALLBACK_ERROR_AND_WHOLE_COMPOSITION.md),
[ready/fatal recursion](SDK_DVD_STATE_READY_DISPATCH_RESUME_AND_ALARM.md) and
[SDK read terminal states](SDK_DVD_READ_BOUNDS_WAIT_WAKE_AND_CLOSE.md).

## Failed state precedes exact timeout gate

cbForStateError accepts raw status R/r3. Load executing8041f170 as C, store
C.statec=ffffffff BEFORE branching on R==16. No command/state/null/index/
callback/extent/ID/generation check or local IRQ token. Other incoming argument
registers are not adapted into a verified formal command argument.

Exactly16 requests stateTimeout at802a0710; when child returns, exit without
running the outer invocation's later fatal/callback/Canceling/ready block.
Every other raw word, including0,17,ffffffff, calls __DVDPrintFatalMessage
at802a0718. It is not a general signed-negative or bit16 test.

## Timeout records/reset/reenters error0 in order

stateTimeout requests __DVDStoreErrorCode802a311c(01234568hex) at802a07ac,
DVDReset802a26cc at802a07b0, THEN cbForStateError(0) at802a07b8. No status check,
queue/counter/state snapshot, timer/alarm wait or rollback locally. Error0
reentry writes the CURRENT executing command's state-1 again before fatal work;
it need not be the earlier command if a child changed the global.

No physical timeout threshold/cadence is proved by this function's name.
Error0 prevents another local exact16 recursion; children/fatal handlers/ready
can still have separate recursive/nonreturn behavior. Save/restore LR and8
decimal stack bytes; no independent result assignment.

## Fatal bridge conditionally invokes one global pointer

__DVDPrintFatalMessage reads FatalFunc8041f1c0 FRESH. Null skips invocation;
nonnull is requested at802a32ac via LR/blrl with NO argument assignment or
formal ABI adaptation. Do not invent message/result/command arguments from
inherited volatile registers. No print text, callback replacement, status write,
wait, failure normalization or handler-result inspection in this bridge.

Actual UI/handler/provider semantics remain separate. If handler returns, bridge
restores LR/8 decimal stack bytes and returns; no unconditional fatal halt is
claimed. Raw handler pointer/type/null/lifetime/ABI/IRQ domains remain open.

## Post-handler executing snapshot differs from failed-state target

AFTER fatal bridge returns, cbForStateError loads executing AGAIN into C2/GPR31.
Publish DummyCommandBlock804138e0 to executing, THEN FatalErrorFlag8041f188=1.
Read C2.callback28; nonnull receives (-1,C2) at802a074c. There is no pointer
identity check or new C2.state-1 store here. Failed-state target C and callback
command C2 can differ through handler/provider effects. Source ordering alone
is not proof an actual original handler changes executing.

Callback can mutate globals; no success/result/rollback/extent guard. Fatal flag
and dummy publication occur before this request, enabling later ready/fatal
consumers under their own recovered contract.

## Fresh Canceling; callback captured before clear

AFTER command callback/null skip, read Canceling8041f190 FRESH. Exactly0 skips
its clear AND cancel-callback load. Nonzero: load CancelCallback8041f194 FIRST,
then store Canceling=0, test the retained pointer. Null still clears Canceling;
nonnull receives (0,C2) at802a077c. No CancelCallback clear or pointer reread
after clearing Canceling. Both command/cancel arguments use captured C2.

Then request stateReady802a14bc at802a0780 regardless of command/cancel callback
presence. No local ResumeFromHere update, queue clear, original file release,
progress/destination/count reset or callback-success-dependent branch.
Ready owns further queued-command failure/dispatch/recursion. Child returns are
not inspected; epilogue restores GPR31/LR and24 decimal stack bytes.

## Evidence and remaining work

[Manifest](../../../tools/sdk_dvd_error_callback_timeout_evidence.json) pins six
spans/all68 words across three complete bodies/six calls/three indirect sites.
Five exact-status/six cancel/three handler/two post-handler snapshot vectors,
timeout request order/global/publication sites checked; busy-callback pins
reverified. Generic verifier checks pins/targets, not semantic/provider/runtime
claims. Mathematical source deductions separate from original execution.
Fresh full ELFs/listings/generator stay `/tmp`; no bodies/assets committed.

Remaining: cbForStateGettingError/reset/error-store/fatal handler/low-level
internals, actual executing/state/status/handler/fatal/cancel/callback/dummy/
provider and valid value/extent/alias/lifetime/IRQ/concurrency/ABI domains,
original timeout/handler/nonreturn/cancel/fatal recursion/IO/audio traces.
EVT-01/native/full-game remain OPEN.
