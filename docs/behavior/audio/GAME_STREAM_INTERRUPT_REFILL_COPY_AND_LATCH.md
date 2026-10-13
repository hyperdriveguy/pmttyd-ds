# Game stream interrupt refill, companion copy and shared latch

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete SoundSSMainInt800dce90/450hex
(276 instructions), DVDMgrReadAsync80144de4/34hex and cache_flush800dfc64/34hex
(13 each). Addresses/offsets/masks hexadecimal; arithmetic/counts decimal.
Static source deductions, not measured interrupt/IO/audio behavior. Read
[outer SoundMainInt](GAME_EFFECT_MAIN_LISTENER_EMITTER_AND_COMPLETION.md),
[stream stop/deferred close](GAME_STREAM_FADE_STOP_AND_DEFERRED_CLOSE.md) and
[cache operations](MUSYX_ARAM_UPLOAD_AND_HARDWARE_STREAM_BRIDGE.md).

## Three rows and initial admission

Retain S803e0138; scan exactly channel0,1,2 with offsets0,138,270hex. RELOAD
pool pointer S.word100/803e0238 at each iteration, capture V=base+offset.
No per-row pool recapture within this body, count-provider read, null/index/
generation/extent check, game shared IRQ counter or OS IRQ request.

Read U16 V.flags0 once for initial admission. Require mask8 set and mask2 clear;
otherwise skip ALL remaining row work. Mask1 clear skips both IO regions but
still reaches companion-copy and shared-latch regions. Initial flags need not
be otherwise normalized/nonzero. Original IRQ invocation/ownership domains
are not established by this function's name.

## Primary and conditional secondary IO admission

With initial mask1 set, primary requires V.word6c nonzero, signed half7c exactly0
and initial captured flags mask1000 clear. Failure skips primary IO and proceeds
to the secondary check. Primary subrecord Q=V+28; cursor C=V.word5c and length
L=V.word88 are read once for chunk selection.

After primary work/skips, fresh V.half1a exactly2 admits secondary consideration.
Require wordf4 nonzero, signed half104 exactly0; then read flags0 FRESH and
require mask2000 clear. No reread of initial masks8/2/1 at this point.
Secondary Q=V+b0, C=V.worde4 and L=V.word110. Failed secondary admission
proceeds to companion copy/latch. A primary child's mutations can affect these
fresh reads; actual producer/concurrency/ABI domains remain separate.

## Chunk selection and two distinct alignments

Use unsigned32 comparisons/arithmetic; wrap every add/subtract to32 bits.
Let B=32768 decimal. Initialize logical count N=B.

If wrap(C+B)>L, N=wrap(L-C). Otherwise examine k=2,3,4,5 in order, choosing
the first wrap(C+k*B)>L. For that k:

    tail = wrap(L - wrap(C+(k-1)*B))
    deficit = wrap(B-tail)
    N = wrap(B - floor_unsigned(deficit/k))
    N = wrap(N + 32 - (N & 31))

If none of these four comparisons succeeds, retain N=B. The last adjustment
advances an ALREADY aligned N by32; it is not ordinary ceiling alignment.
Separately, request count A=wrap(N+31)&ffffffe0. N is later stored in Q.word48;
A is passed to either IO service. N/A may differ; no local clamp/range/nonzero
check protects either or the wrap-domain comparisons. Finite vectors include
L32768,C0 yielding N/A16416, rather than a32768-byte request.

Secondary also computes T=wrap(L+32-(L&31)) BEFORE this branch sequence and
retains it for the plane offset. T advances aligned L by32 too. Primary has
no corresponding added T. Original file/ARAM/buffer/provider validity remains
required; host wrap vectors do not establish real file layout or safe extents.

## Captured storage-path flag and child requests

Primary uses initial captured flags mask400; secondary uses its freshly captured
flags from the secondary pending gate. No reread selects the storage path.

Mask400 clear: read U16 Q.half4c and index Q.word[2c+wrap((1-half4c)*4)]
without bounding that halfword. Load Q.word0 as DVD record. Primary file offset
is wrap(U16 V.half4+C); secondary adds retained T as well. Request
DVDMgrReadAsync(record, selectedBuffer, A, offset, _ssDVDReadAsync_activeChk)
at800dcfb8 or800dd164. Callback entry800dfc98 is linked, not locally executed.

Mask400 set: read U16 V.half2; load ARAM base from
S.word[21c+half2*4]/803e0354 table without a local index bound. Build retained
source address X=wrap(base+U16 V.half4+C), with secondary additionally adding T.
Read Q.half4c/selectedBuffer and request DCInvalidateRange80296a28
(selectedBuffer,32768) at800dd000 or800dd1b0. This fixed invalidation count is
independent of A and N. The target is invalidate, not a flush/store entry.

AFTER invalidation, reread Q.half4c and selectedBuffer; request ARQPostRequest
802aa8ec at800dd038 or800dd1e8 with
(Q+4, Q, type1, priority0, retainedX, secondFreshBuffer, A, cache_flush800dfc64).
The owner argument is Q. Selected buffers can differ across the child call;
source address/count stay retained. No return/status check or explicit waiting.
SDK queue/DMA/cache semantics remain in the linked/transitive contracts.

AFTER either request returns, store retained N at Q.word48. Read V.flags0
FRESH, OR primary1000 or secondary2000 and store low16. Read flags AGAIN;
mask400 set stores half7c orhalf104=1 respectively, otherwise leaves that
halfword unchanged. The post-child flag selects this write independently of
the pre-child storage-path flag. No callback completion claim follows from
these local pending/busy publications.

## Deferred DVD record publication and ARQ callback bridge

DVDMgrReadAsync is a leaf: store caller buffer at record.word7c, count at80,
offset at84; read half90, OR1/store; reread half90, clear mask2/store; store
callback at word8c, THEN zero word88. No immediate SDK DVD call, null guard,
status return assignment or filename/extent check. Ordering matters when fields
alias or external consumers observe them. The deferred manager's consumer and
_ssDVDReadAsync_activeChk body still require separate recovery.

cache_flush takes the ARQ completion request pointer in r3. It reads
request.word4 as owner Q, loads Q.word0 as record D, then requests
_ssDVDReadAsync_activeChk(0, wrap(D+40hex)) at800dfc84. No null/owner/request
status validation and no direct cache service in this bridge. The callback
name does not prove cache behavior. It restores LR and16 decimal stack bytes;
original callback ABI, lifetimes and completion semantics remain open.

## Companion copy after IO and shared latch

Fresh type half1a exactly2, signed half7c==0 and signed half104==0 admit a
comparison of U16 half74 and half fc. If unequal, ordered copies are:

1. half74 to half fc;
2. word6c to wordf4;
3. word60 to worde8;
4. word5c to worde4.

Equal indices or failed gates skip all four. No pointer/extent/flags/null
checks here beyond initial row admission; no IO-service call during these copies.
Fresh type is read again afterward for latch selection.

Shared S.half214/803e034c exactly2: type2 requires both signed busy half7c and
half104 exactly1, then clears shared latch to0. EVERY other type requires only
half7c exactly1. No latch update for other shared values or busy values.
A mask1-clear row can therefore copy/clear after its IO was skipped. Each
admitted row can change the shared latch before the next row's reads. Initial
mask8/2 failure skips these consumers too. No independent success assignment;
epilogue restores GPR24..31/LR and48 decimal stack bytes.

## Evidence and remaining work

[Manifest](../../../tools/game_stream_interrupt_engine_evidence.json) pins six
spans, all302 words across three complete bodies, seven direct calls and two
callback bindings. Twelve chunk/ten stable gate/six latch/six copy mathematical
vectors checked; stable gates do not replace post-child reread semantics.
Generic verifier checks hashes/anchors/targets, not prose/providers/runtime.
Fresh full ELFs/listings/generator stay `/tmp`; no bodies/binary assets committed.

Remaining: SoundSSMain/play and three _ssDVDReadAsync callbacks; deferred DVD
manager consumers; actual pool/flags/busy/count/cursor/length/index/buffer/ARAM/
header/latch providers and valid value/extent/alias/lifetime/IRQ/concurrency
and ABI domains; transitive SDK/cache/ARAM/AI/DSP and original IRQ/IO/callback/
audio traces. EVT-01/native/full-game binding remain OPEN.
