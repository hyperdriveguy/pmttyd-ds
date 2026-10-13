# SDK DVD busy command dispatch and read chunks

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete stateBusy802a17a4/340hex,
208 instructions and17-entry jump table803c2df0/44hex. Addresses/offsets/masks
hexadecimal; counts/arithmetic decimal. Static deductions, not original MMIO/
command/IO/audio observations. Read
[ready dispatch](SDK_DVD_STATE_READY_DISPATCH_RESUME_AND_ALARM.md) and
[submission/command fields](SDK_DVD_ASYNC_SUBMISSION_AND_QUEUE_APPEND.md).
Completion callback/low-level command engines remain separate.

## LastState before unguarded input command; table or optional handler

Input C/r3 retained in volatile r7. Create8 decimal stack bytes/save LR; store
stateBusy's own address802a17a4 at LastState8041f1b8 BEFORE reading C.word8.
No C/null/state/ownership/generation/alignment/extent/IRQ check.

Unsigned command<=16 uses numeric jump table below. EVERY unsigned value>16
uses optional branch too; no general rejection or signed-negative special case.
Command0's table entry selects the same optional branch. No current-command/
executing/state1 assignment is performed by common entry.

| Command decimal | Target | Request branch |
| --- | --- | --- |
| 0 | 802a1abc | optional handler |
| 1,4 | 802a1810 | zero total or chunked read |
| 2 | 802a18b0 | seek |
| 3 | 802a18d4 | stop motor, no local MMIO echo |
| 5 | 802a17e4 | disk ID read |
| 6 | 802a18f4 | AutoFinishing-dependent audio request |
| 7 | 802a1958 | audio stream10000 |
| 8 | 802a1984 | AutoFinishing1/audio stream0 |
| 9,10,11,12 | 802a19b8/19dc/1a00/1a24 | audio status selectors0/10000/20000/30000 |
| 13 | 802a1a48 | audio buffer config |
| 14 | 802a1a70 | inquiry |
| 15 | 802a18e4 | stop motor, no local MMIO echo |
| 16 | 802a1a9c | stop motor with local MMIO echo |

All low-level requests below pass cbForStateBusy802a1ae4 as callback. Return
values are ignored locally. No callback execution, physical completion or
hardware error/result interpretation is inferred from these requests.

## MMIO read-back/write precedes designated command requests

Disk-ID, seek, audio6..13, inquiry14, stop16 and NONZERO read1/4 paths read word
cc006004 and immediately store that same word back. No local bit mask/value
transformation. Commands0/3/15 and zero-read branch skip this echo. This pins
access/order, not controller register acknowledgment/side effects/timing.
Low-level children can own additional MMIO/cache/IRQ operations separately.

## Commands1/4: zero total uses global executing and recursive ready

Read INPUT C.word14 total N; exactly0:
load GLOBAL executing8041f170 as E, publish DummyCommandBlock804138e0 to global
executing BEFORE storing E.statec=0. Read E.callback28; nonnull receives (0,E)
at802a1848 via LR/blrl. Then request stateReady802a14bc at802a184c regardless
of callback presence, and exit. No completed-word20 update, callback status
check or input/global pointer equality test. E can differ from INPUT C.
No explicit invalidation/read/IRQ service on this path.

## Nonzero read: unsigned512 KiB cap, then fresh addresses/count

After MMIO echo, read INPUT C.word20 progress P0, then total N FRESH; let
R=wrap32(N-P0). K=min_unsigned(R,80000hex=524288). Store K at C.word1c.
No P0<=N, signed-count, file-end, buffer-capacity, zero-K or alignment check.
Underflow can choose512 KiB; raw totalffffffff is not rejected in this function.
This chunk cap differs from manager's conditional256 KiB cap.

AFTER chunk publication reread progress20 as P1, destination18 as D,
absolute offset10 as O and chunk1c FRESH. Request
DVDLowRead8029f4ac(wrap32(D+P1),freshChunk,wrap32(O+P1),cbForStateBusy)
at802a18a8. The two addresses share P1; K calculation used earlier P0.
No total/progress recheck or cursor/complete publication afterward. Child/result/
provider/alias/concurrency domains remain separate; callback owns completion.

## Remaining exact low-level request arguments

The table uses raw INPUT C words except where GLOBAL executing is explicit.
All offsets/selectors are hexadecimal; arguments include busy callback last.

| Command | Local writes before child | Child request / call |
| --- | --- | --- |
| 5 | INPUT C.word1c=32 decimal | DVDLowReadDiskID(C.word18,CB)8029f804 at802a1808 |
| 2 | none beyond MMIO echo | DVDLowSeek(C.word10,CB)8029f744 at802a18cc |
| 3,15 | none | DVDLowStopMotor(CB)8029f8a8 at802a18dc/18ec |
| 6,AutoFinishing nonzero | GLOBAL executing.word1c=0 | DVDLowRequestAudioStatus(0,CB)8029faf4 at802a1928 |
| 6,AutoFinishing0 | GLOBAL executing.word1c=1 | DVDLowAudioStream(0,INPUT C.word14,INPUT C.word10,CB)8029fa5c at802a1950 |
| 7 | none beyond MMIO echo | DVDLowAudioStream(10000,0,0,CB) at802a197c |
| 8 | AutoFinishing8041f184=1 after MMIO echo | DVDLowAudioStream(0,0,0,CB) at802a19b0 |
| 9..12 | none beyond MMIO echo | DVDLowRequestAudioStatus(selector0/10000/20000/30000,CB) at802a19d4/19f8/1a1c/1a40 |
| 13 | none beyond MMIO echo | DVDLowAudioBufferConfig(C.word10,C.word14,CB)8029fb80 at802a1a68 |
| 14 | INPUT C.word1c=32 decimal | DVDLowInquiry(C.word18,CB)8029f9c0 at802a1a94 |
| 16 | none beyond MMIO echo | DVDLowStopMotor(CB) at802a1ab4 |

AutoFinishing is a fresh fullword zero/nonzero gate, not normalized enum. Case6
reads executing afresh for its tracking write but uses INPUT C for stream count/
offset. No local C/E identity check, callback/state/result guard or rollback.
Field1c meanings differ by command: raw requested chunk or audio bookkeeping;
never infer one uniform requested-byte schema across all commands.

## Optional branch has no null/ABI/result validation

Command0 or>16 loads checkOptionalCommand8041e478 FRESH. Request it at802a1ad0
with (INPUT C,cbForStateBusy802a1ae4) using LR/blrl. No pointer-null, signature,
command-domain/admission/result check or default callback substitution. Original
initial pointed function802a04fc is hash-linked, not answered by this contract.
No direct state/callback/error publication before this optional request besides
LastState. Its consumer behavior remains separate.

## Evidence and remaining work

Common exit restores LR/8 decimal stack bytes; no independent status return.
[Manifest](../../../tools/sdk_dvd_state_busy_dispatch_evidence.json) pins thirteen
spans, all208 body-word anchors plus17 jump-table entries (225 anchors), seventeen
direct calls and two indirect call sites. Eight chunk/three fresh-address/four
auto-finishing/six optional vectors, full dispatch table/MMIO command list and
publication/binding sites checked; ready pins reverified. Mathematical deductions
separate from original execution. Generic verifier checks pins/targets, not
semantic/provider/runtime claims. Fresh full ELF/listing/generator stay `/tmp`;
no bodies/binary assets committed. Local whole dispatch answered.

Remaining: cbForStateBusy/optional callback and low-level read/ID/seek/stop/audio/
inquiry engines, actual command/progress/destination/offset/chunk/executing/
auto-finishing/LastState/MMIO/callback providers and valid value/extent/alias/
lifetime/IRQ/concurrency/ABI domains; cancellation/thread/path/open and original
zero/read-chunk/short-read/controller/callback/IO/audio traces. EVT-01/native/
full-game remain OPEN.
