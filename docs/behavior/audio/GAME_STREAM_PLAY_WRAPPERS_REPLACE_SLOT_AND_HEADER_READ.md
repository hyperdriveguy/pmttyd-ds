# Game stream play wrappers: replacement, cache slot and header read

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete SoundSSPlayChEx800dead0/2achex
(171 instructions), SoundSSPlayCh800ded7c/24hex (9), getMarioStDvdRoot80007388/
8hex (2). Addresses/offsets/masks hexadecimal; counts/arithmetic decimal.
Static deductions, not original file/cache/playback observations. Read
[stop/deferred close](GAME_STREAM_FADE_STOP_AND_DEFERRED_CLOSE.md),
[stream free](MUSYX_STREAM_FREE_AND_REVERSE_STATE_CLEAR.md),
[deferred read](GAME_STREAM_INTERRUPT_REFILL_COPY_AND_LATCH.md) and
[cache callbacks](GAME_STREAM_CACHE_CALLBACK_TRANSFER_AND_INITIAL_READS.md).

## Public ABI and captured row

SoundSSPlayCh(I,N) sets r5=0 and calls SoundSSPlayChEx at800ded8c, preserving
raw channel I/r3 and name pointer N/r4. It restores LR/16 decimal stack bytes.
Ex accepts raw caching argument K/r5, retaining I,N,K. Every nonzero K enables
cache-slot processing; no low8/bool normalization or special negative case.

Capture S803e0138 and OLD row V=poolWord100+wrap32(I*138). No channel<3, index,
null/name/flags/type/extent/generation/IRQ check precedes this access. V stays
captured across replacement, formatting and opening children; pool changes do
not recapture it. Ex's two consecutive equality branches use the same initial
flags comparison, not two fresh flag reads.

## Replace occupied channel before any slot/path/open validation

Initial U16 V.flags0 nonzero admits inline stop; exactly0 skips it. Read shared
U16 counter20c/803e0344; initial0 requests OSDisableInterrupts80299ad4 at800deb1c
and stores returned state at S.word210/803e0348. Fresh counter increments low16.
No overflow guard; existing nonzero counter skips disable.

Fresh V.flags mask400 reads half2 and clears S.half[226+half2*2]/803e035e slot
without an index bound. Store V.flags0=0 BEFORE DVDMgrClose80144dd4(primary
word28) at800deb6c, close fresh secondary wordb0 at800deb74, sndStreamFree8027c3e0
(fresh primary word4c) at800deb7c. AFTER primary child fresh type half1a exactly2
frees fresh secondary wordd4 at800deb90. No null/ID/status check or success gate.

Fresh counter0 skips decrement/restore; nonzero decrements low16, resulting0
restores fresh saved state via OSRestoreInterrupts80299afc at800debbc. Then
S.word104/803e023c=ffffffff. Stable counter65535 wraps0 without local restore.
IRQ/alias/lifetime/concurrency invariants remain open. Failure later does not
restore old playback, records, flags, slot or marker. No SoundSSStopCh call.

## Optional first-free slot; reservation is retained on open failure

K nonzero reads U16 S.half224/803e035c slot count, scans that many halfwords
starting S+226. Exactly0 count skips scan. First slot value exactly0 gets1;
store its raw index low16 at captured V.half2. Nonzero values are occupied.
At end reread slot count; index>=fresh count returns BEFORE row initialization,
path/open/header work. No max count/table extent/locking check and no fallback
to uncached play. Initial count and final fresh count are distinct reads.

K0 skips slot scan/reservation entirely and leaves half2 unchanged. On admitted
setup, S.word104 receives RAW I before row reset. Later failures leave this
marker and any newly reserved slot in place; no rollback branch is present.

## Ordered row reset preserves other fields

Exact initial write order after marker:

| Writes in order | Value |
| --- | --- |
| half74; word6c,5c,60; half7c,7e,80,82,84 | 0 |
| half fc; wordf4,e4,e8; half104,106,108,10a,10c | 0 |
| half6 | 64 |
| half8 | 0 |
| float10 | positive0, literal80421908 |
| floatc | 127, literal80421918 bits42fe0000 |
| half0 flags | 1 |

K nonzero then reloads flags, ORs400/store, giving401 under stable writes.
There is no whole-record memset: type1a, step14, frequency18, IDs4c/d4,
header/count/buffer/length/restart and other unlisted fields are preserved here.
Their producers/validity belong to header setup and other services. This reset
alone does not establish ready/active original playback or valid handles.

## Shared filename, two opens and deferred header dispatch

Request getMarioStDvdRoot at800deccc; its leaf returns constant pointer8041f494
to `.`. Request sprintf8026a0b8 at800decf0 into shared256-byte str$436/803e0038,
format802e3cd0 `%s/%s%s`, arguments root,N, suffix8042191c `.stm`.
No length/name/pointer check or bounded formatting request. ABI varargs CR6
cleared; actual formatter/length/alias/concurrency domains remain separate.

Request DVDMgrOpen80144e88(path,0,0) at800ded08; store raw result at V.word28
BEFORE null test. Null clears V.flags0 only and returns. Nonnull requests SAME
path/arguments a second time at800ded30, stores result at V.wordb0 BEFORE null
check. Second null also clears flags only; it does not close the newly opened
primary or release a cache reservation. No type2 check gates this second open.
The two results are not proven distinct handles; actual manager sharing/reference
semantics require separate recovery.

Both nonnull: freshly load primary28 and header buffer50; request
DVDMgrReadAsync(primary,buffer,256,offset0,SoundSSPlayChEx_main800de104)
at800ded64. No immediate call to the header callback, synchronous wait, return
check or subsequent wrapper flag write. Deferred read publishes its record
fields; original callback scheduling/SDK completion is separate. Header callback
body is linked/hash-pinned, not answered by this wrapper contract.

Ex assigns no independent status return. Epilogue restores GPR27..31/LR and32
decimal stack bytes. There is no volume/pan input adaptation beyond defaults,
file-content/type/size verification or success-conditioned rollback here.

## Evidence and remaining work

[Manifest](../../../tools/game_stream_play_wrappers_evidence.json) pins15 spans,
all182 words across three complete bodies,twelve direct calls,23 ordered row
writes and header callback binding. Six slot/four cache-request outcome/five
raw caching-argument vectors checked; stop pins reverified. Outcome vectors
assume caching requested and unchanged slot-count/child domains; actual fresh
reads and failures retain their source ordering. Generic verifier checks pins/
targets, not semantic/provider/runtime claims. Mathematical deductions separate
from original execution. Fresh full ELFs/listings/generator stay `/tmp`; no
bodies/binary assets committed. Local whole public wrappers/root leaf answered.

Remaining: SoundSSPlayChEx_main header callback, actual slot/count/name/root/
path/pool/record/buffer/default/type/ID/marker/counter/IRQ-state providers and
valid value/extent/alias/lifetime/IRQ/concurrency/ABI domains; deferred DVD open/
read consumers, transitive SDK/ARAM/AI/DSP and original replacement/slot-exhaustion/
open-failure/header/cache/playback/audio traces. EVT-01/native/full-game OPEN.
