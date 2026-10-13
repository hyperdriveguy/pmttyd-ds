# Game stream cache callback transfer and initial playback reads

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete _ssDVDReadAsync_cache_next800df7d4/
10chex (67 instructions), _ssDVDReadAsync_cache_aram800df8e0/384hex (225) and
DVDMgrGetLength80144dcc/8hex (2). Addresses/offsets/masks hexadecimal;
arithmetic/counts decimal. Static deductions, not original DVD/ARAM/audio traces.
Read [interrupt requests/chunk formula](GAME_STREAM_INTERRUPT_REFILL_COPY_AND_LATCH.md),
[active completion/startup](GAME_STREAM_COMPLETION_MATCH_CURSOR_AND_STARTUP.md),
[DVD length provenance](../file/FILE_ARCHIVE_AND_DVD_ACQUISITION_REQUESTS.md) and
[ARAM upload services](MUSYX_ARAM_UPLOAD_AND_HARDWARE_STREAM_BRIDGE.md).

## Cache-next matching and cursor advance

Inputs raw result R in r3 and record callback pointer P in r4. Load pool pointer
S803e0138.word100 ONCE, scan exactly three rows stride138hex, retaining row and
index. Nonzero U16 flags0 is the only row gate. Match wrap(row.word28+40hex)==P;
no secondary record comparison or type gate. First matching row wins.
Signed R<0 exits without writes/transfer; other values ignore returned byte count.
Unmatched/inactive rows advance; exhaustion returns. No IRQ/counter/null/extent/
generation/ID guard or independent success assignment.

On match, advance row.word5c=wrap(old5c+word70), using stored logical read count.
Then read half2, progress word60, source buffer word54 and ARAM base at
S.word[21c+half2*4]/803e0354. Retain destination X=wrap(base+progress).
Request DCFlushRange80296a54(buffer,32768) at800df864. This is the flush-with-
system-call entry, not invalidate or store; fixed decimal32768 regardless of
logical count or final short read.

After flush, load fullword serial owner_no$1419 at8041eb20/r13+1c00, increment
with wrap32 store, and form owner=wrap((OLD serial<<16)|channelIndex). No equality/
reuse/generation guard. Read source buffer54 FRESH after flush. Request
ARQPostRequest802aa8ec at800df8a0 with
(row+2c, owner, type0, priority0, secondFreshBuffer, retainedX,32768,
_ssDVDReadAsync_cache_aram800df8e0).

After ARQ child returns, reread progress60 AND logical count70, store
wrap(freshProgress+freshCount) at60. Cursor5c advanced before flush; progress60
advances after transfer request. Source buffer can change between requests;
destination remains retained. No return/status/physical completion check,
count clamp, padding/zero-fill, filename/extent check or cache-readiness write.
ABI/lifetime/concurrency/valid short-buffer domains require provider evidence.
Epilogue restores GPR29..31/LR and32 decimal stack bytes.

## ARAM callback uses owner low byte, without matching

_ssDVDReadAsync_cache_aram input r3 is ARQ request pointer. Read request.word4
owner, keep LOW8 only, load current S.word100 and capture V=pool+low8(owner)*138.
No index<3, serial/high-word equality, flags/type/record match, null/request-status
or IRQ/counter check before access. Owner low8 values3..255 address rows outside
the three-row scan domain without a local guard; valid providers remain open.
The owner serial does not implement a checked generation in this callback.

Load V.word28 and request DVDMgrGetLength at800df910. This leaf returns raw
record.word74 only; no null/range/status check. AFTER child read V.word60
progress; unsigned progress>=returnedLength selects initial playback reads.
Otherwise continue file caching. V remains captured across all child calls.

## Continue caching: second length read and deferred DVD request

Progress<firstLength: reload primary record28 and request GetLength AGAIN at
800dfc0c. Read cursor5c AFTER this child; let C be that value and L the second
returned length. N=32768 if wrap(C+32768)<L, else N=wrap(L-C).
The comparison is STRICT; no C<=L guard protects unsigned subtraction.
A=wrap(N+31)&ffffffe0. Read record28 and source buffer54 fresh, request
DVDMgrReadAsync(record,buffer,A,C,_ssDVDReadAsync_cache_next800df7d4)
at800dfc48. Offset is raw C, with no header half4 or plane offset added.
AFTER request store retained N at V.word70. No cursor/progress advance here,
no returned count/status dependency and no cache flag/readiness write.
The next callback consumes70 and transfers fixed32768 as described above.

## Cache finished: reset four cursors, issue playback reads

Progress>=firstLength: ordered stores0 to V.word5c,60,e4,e8. Then store
primary half74=1. Capture primary Q=V+28, freshly read cursor5c and length88.
Use the linked interrupt contract's COMPLETE unsigned chunk formula (B32768,
first greater comparison for k2..5, strict32 advance, wrap domains) to obtain
logical N and request A=wrap(N+31)&ffffffe0. Fresh V.flags0 mask400 selects path;
there is no flags8/2/1/pending/count/busy admission gate.

Mask400 clear: read Q.half4c; select buffer
Q.word[2c+wrap((1-half4c)*4)]. No bound on half4c. Read primary record Q.word0
and header U16 V.half4; request DVDMgrReadAsync(record,buffer,A,wrap(header+C),
_ssDVDReadAsync_activeChk800dfc98) at800dfa04.

Mask400 set: read V.half2/ARAM table base, header half4 and Q.half4c/selected
buffer. Retain X=wrap(base+header+C). Request DCInvalidateRange80296a28
(buffer,32768) at800dfa4c. AFTER child reread Q.half4c/selected buffer; request
ARQPostRequest(Q+4,Q,type1,priority0,retainedX,secondFreshBuffer,A,
cache_flush800dfc64) at800dfa84. No post-invalidation storage-path recheck.
Store retained N at Q.word48 AFTER either request.

Then read type half1a FRESH AFTER primary child/publication. Exactly2 starts
secondary: store half fc=1, capture Q=V+b0, read cursor e4/length110 and repeat
chunk formula. Retain T=wrap(L+32-(L&31)) as secondary plane offset; even aligned
L advances by32. Fresh flags0 mask400 independently selects secondary path.
DVD offset=wrap(header+C+T); ARAM source X=wrap(base+header+C+T).
DVD request at800dfb78, invalidate at800dfbc4 and ARQ at800dfbfc use the same
arguments/callbacks and post-child buffer rereads as primary. Store secondary
N at Q.word48 AFTER its request. Every non-two type skips secondary entirely.

No pending OR1000/2000, busy half7c/104 write, shared latch update, activation,
inline mix or final flag8 publication occurs in this callback. Those consumers
are separate; the linked active completion callback establishes the later
readiness/activation behavior. Child effects can alter reset cursors, flags,
type/header/index/buffers before the fresh reads described here; no stable-row
snapshot is substituted. Epilogue restores GPR27..31/LR and32 decimal stack bytes.

## Evidence and remaining work

[Manifest](../../../tools/game_stream_cache_callbacks_evidence.json) pins nine
spans, all294 words across three complete bodies, eleven direct calls and six
callback bindings. Six owner/four next-transfer/seven continuation/six branch/
six index/twelve linked chunk vectors checked. Interrupt/chunk and completion
pins reverified. Mathematical models separate from original execution; generic
verifier checks hashes/anchors/targets, not semantic/provider/runtime claims.
Fresh full ELFs/listings/generator stay `/tmp`; no bodies/binary assets committed.

Remaining: SoundSSMain/play and deferred DVD manager consumers; actual cache
setup/flags/record/result/serial/channel/pool/count/cursor/length/progress/ARAM/
header/type/index/buffer producers and valid value/extent/alias/lifetime/IRQ/
concurrency/ABI domains; transitive SDK/cache/ARAM/AI/DSP and original cache/
short-read/error/DMA/callback/startup/audio traces. EVT-01/native/full-game OPEN.
