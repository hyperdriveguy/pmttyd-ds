# SDK DVD read bounds, wait/wake and close wrapper

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete DVDReadPrio802a03c0/118hex
(70 instructions), DVDClose802a0178/24hex and dvd.a dvdfs.c cbForReadSync
802a04d8/24hex (9 each). Addresses/offsets/masks hexadecimal;
counts/arithmetic decimal. Static deductions, not original SDK/disc/thread/IO
execution. Read [manager results](DVD_MANAGER_SORTED_SERVICE_RESULTS_AND_CLOSE.md)
and [manager wait protocol](DVD_MANAGER_SYNCHRONOUS_READ_PROTOCOL.md).
SDK async-command/sleep/wake/cancellation internals remain separate.

## Retained raw read ABI and two bounds checks

Inputs F/r3 fileInfo, D/r4 destination, N/r5 count, O/r6 offset and P/r7 priority;
retain all. No fileInfo/destination null, occupied/type/ID/generation/buffer
capacity/alignment/priority or independent count-sign check before accesses.

First: SIGNED32 O<0 requests OSPanic80297894. Otherwise read F.word34 length L;
UNSIGNED O>L also requests panic. Equality O==L is permitted by this check.
Request at802a0408 uses filename pointer8041e468, line820 and diagnostic
pointer803c2d38; CR6 cleared for varargs. The child owns panic/nonreturn behavior.
This wrapper has no explicit bounds-error return; if panic returns, execution
falls through to the second check without a revalidated offset.

Second: E=wrap32(O+N). SIGNED32 E<0 requests panic. Otherwise reread F.word34
FRESH, compute T=wrap32(L+32); only UNSIGNED E<T avoids panic. Equality E==T
fails, unlike first offset equality. Request at802a0438 uses same filename/
diagnostic, line826. If child returns, fall through to request submission.

Both sum and length+32 wrap before comparison. O0/N31/L0 passes; N32 fails
second check. O1/Nffffffff/L100 has E0 and can pass both checks despite negative
raw N; no separate count validation. Raw large length can wrap T. Models are
source branches, not valid file/buffer domains or measured original failures.

## Submit async command; zero submission returns-1 directly

Read F.word30 start address FRESH AFTER bounds/panic paths. Request
DVDReadAbsAsyncPrio802a21f8(F,D,N,wrap32(start+O),cbForReadSync802a04d8,P)
at802a045c. Raw retained destination/count/offset/priority are not normalized.
No callback-null check or explicit command-field write in this wrapper.
Original async engine/provider/alignment/extent semantics remain linked work.

Result exactly0 returns rawffffffff (-1), with NO local IRQ-disable/sleep/restore.
Every other result enters the wait, including raw negative values. No discarded
submission rollback/cancel/free or parameter refresh before waiting.

## One saved IRQ token; three terminal states and shared wait queue

Request OSDisableInterrupts80299ad4 at802a0470 and retain its returned token.
Read F.word0c state FRESH each iteration:

| Raw state | Local action |
| --- | --- |
| 0 | Read raw F.word20 as final result |
| ffffffff | Assign resultffffffff (-1) |
| a (10 decimal) | Assign resultfffffffd (-3) |
| EVERY other word | OSSleepThread8029d5b0(__DVDThreadQueue8041f168), then reread state |

No general negative-state return, timeout, elapsed tick, busy-spin, retry/cancel
condition or independent file-length return. State-3 itself sleeps, although
state10 maps to return-3. Word20 is not normalized to requested N or a named
byte-count meaning without command schema/producer evidence.

Wrapper does NOT restore its token before OSSleepThread at802a04b0. Child
sleep/scheduler/IRQ behavior requires its own contract. Once a terminal state
selects result, request OSRestoreInterrupts80299afc(savedToken) at802a04bc,
then return retained raw result. This differs from the game's shared counter
protocol; do not substitute one for the other. Child/token/IRQ domains remain open.

## Read-sync callback wakes shared queue, ignoring incoming arguments

cbForReadSync overwrites r3 with fixed __DVDThreadQueue8041f168 and requests
OSWakeupThread8029d69c at802a04e8. No result/fileInfo/slot/callback identity check,
command-state write, completion-word publication or argument adaptation. Its
source does not identify one waiter; queue semantics belong to the SDK service.
No independent callback result assignment; LR/8 decimal stack bytes restored.
Actual command producer must publish terminal state for waiters to finish;
a wake request alone is not that evidence.

## Close requests cancellation then unconditionally returns1

DVDClose passes raw incoming F/r3 to DVDCancel802a2ae4 at802a0184, ignores its
return and assigns1 afterward. No local null/state/handle/filename/queue check,
fileInfo zeroing, heap free, callback wait or close-result propagation. This
source wrapper alone does not prove cancellation completed successfully.
Restores LR/8 decimal stack bytes. Manager separately zeroes its record after
this wrapper; original SDK cancellation/lifetime behavior remains open.

Read wrapper saves/restores GPR27..31/LR and56 decimal stack bytes. No physical
poll frequency or original short-read interpretation is claimed by this wait.

## Evidence and remaining work

[Manifest](../../../tools/sdk_dvd_read_close_wait_evidence.json) pins twelve
spans/all88 words across three complete bodies/eight calls, diagnostic hashes,
read callback binding and shared queue identity. Ten bounds/six state/four
submission/three close vectors and exact call/ABI/field/return sites checked;
manager main pins reverified. Generic verifier checks pins/targets, not semantic/
provider/runtime claims. Mathematical deductions separate from original execution.
Fresh full ELFs/listings/generator stay `/tmp`; no bodies/assets committed.

Remaining: DVDReadAbsAsyncPrio/command engine/DVDCancel and sleep/wake/IRQ/panic
internals; actual fileInfo/state/completion/start/length/destination/count/offset/
priority/queue/waiter/callback/token providers and valid value/extent/alias/
lifetime/IRQ/concurrency/ABI domains; path/open/thread services and original
bounds/submission/status/short-read/error/wakeup/cancellation/IO/audio traces.
EVT-01/native/full-game remain OPEN.
