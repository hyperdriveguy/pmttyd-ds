# MusyX ARAM generic upload and hardware stream bridge

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete aramUploadData802916dc/1dc,
hwFlushStream802912e4/90, hwInitStream80291374/20,
hwExitStream80291394/20 and hwGetStreamPlayBuffer802913b4/24hex.
Read [queue callback/layout](MUSYX_ARAM_QUEUE_COMPLETION_AND_SAMPLE_REQUESTS.md),
[stream allocation/get/free](MUSYX_ARAM_STREAM_BUFFER_LISTS_AND_LIFETIME.md)
and [sample-store distinction](MUSYX_ARAM_SAMPLE_STORE_AND_CURSOR_REWIND.md).
Hexadecimal offsets/addresses below; decimal counts explicitly labeled.

## Generic upload arguments and queue selection

Inputs r3..r8: CPU source S, ARAM destination D, byte count N, raw priority H,
user completion callback F and callback argument A. Retain all six raw words.
H0 selects lowQueue80410358; every nonzero H selects highQueue804105dc.
Queue base is retained for every retry. Stored priority is normalized to0/1
using unsigned-word `(H | wrap32(-H)) >> 31`, not low-byte truncation.
Thus H100hex selects high/priority1 even though its low byte is0.

No cache flush, alignment, size/capacity/address/null check, callback invocation,
chunk splitting, allocation or source-content copy occurs here. N0 is still
posted. This API differs from the sample store's32-byte sizing/cache cursor path.
Actual input alignment/length/source/provider domains remain caller obligations.

## Complete admission and publication

Request OSDisableInterrupts80299ad4, retain its returned state. Read selected
queue.byte281; unsigned count>=16 restores state via OSRestoreInterrupts80299afc
and retries disable/check. No timeout/drop/failure return or event/pad tick.
At count<16, fill current slot words in order, rereading byte280 before EACH
field address/store:

| Offset | Value |
| --- | --- |
| +4 | Owner42 decimal |
| +8 | Type0 |
| +c | Normalized priority0/1 |
| +10 | Raw retained S |
| +14 | Raw retained D |
| +18 | Raw retained N |
| +1c | aramQueueCallback8029163c |
| +20 | Raw retained F |
| +24 | Raw retained A |

Word0/padding are not initialized locally. Independently reread insertion index
for request pointer and each of seven outgoing field operands, then request
ARQPostRequest802aa8ec(slot,word4,word8,wordc,word10,word14,word18,word1c).
No post-result check. AFTER posting, freshly read count, store U8(count+1);
freshly read index, store (index+1)%16; restore saved OS state. Return without
an independent success assignment or completion wait. The count/index increment
and OS restore match the sample/initialization low-queue producers exactly.

Completion dispatch uses slot.wordc==1 to choose the high queue, matching the
normalization here. It invokes F(A) on a pointer match if F nonzero, then decrements
the selected queue's fresh count. Caller callback/source lifetime and SDK deferral
remain prerequisites; an immediate pre-count completion is not repaired locally.
SyncTransferQueue still waits only LOW: it does not drain this API's high posts.

## Complete hardware stream flush

Inputs r3..r8: CPU buffer base B, byte offset O, requested count N, stream ordinal I,
user callback F, argument A. Retain B/O/N/F/A. FIRST request
aramGetStreamBufferAddress(I,&stackSizeWord), capturing returned ARAM base R.
That helper writes the descriptor active size to a stack word, but THIS body
never reads it or uses it to bound the requested region.

Compute low=O&1f; Q=O&ffffffe0; L=wrap32(N+low+1f)&ffffffe0;
CPU pointer P=wrap32(B+Q). Request DCStoreRange(P,L)80296a84 (not DCFlushRange).
Then request aramUploadData(P,wrap32(R+Q),L,1,F,A). No second source getter,
cache/capacity check, ring wrap/split, callback invocation or completion wait.
The user callback/argument pass through unchanged. There is no local invalid
ordinal/base guard or zero-length bypass. N0 with nonaligned O can still produce
L20hex; wrapping N/O arithmetic can instead produce L0. These are source word
operations, not widened interval coverage or proof of an admitted stream extent.

DCStoreRange child semantics and physical DMA/alignment/coherence remain separate.
Caller streaming logic must establish valid ordinal/lifetime/offset/count and
buffer extent; writing descriptor size locally does not establish those domains.

## Complete init, exit and play-buffer wrappers

hwInitStream passes INPUT r3 to aramAllocateStreamBuffer and preserves its return,
including failure255 decimal. No audio-start/voice/cache/upload request.

hwExitStream passes INPUT r3 to aramFreeStreamBuffer and preserves that child's
register return without an independent success assignment. No local queue drain,
voice stop or DMA cancellation. The free body's miss/membership/order requirements
continue to apply; this wrapper adds no lifetime protection.

hwGetStreamPlayBuffer supplies INPUT r3 and output-pointer0 to
aramGetStreamBufferAddress; returns the raw address. Despite its name, no hardware
play-position/current-buffer status is queried by this body.

## Scoped caller census and source validation

[Manifest](../../../tools/musyx_aram_upload_stream_bridge_evidence.json) pins
five complete bodies,11 linked-only SDK/helper/dependency bodies,114 original
anchors and14 resolved direct calls. Independent whole-DOL aligned direct-call
scan finds25 sites:17 hwFlushStream and1 hwGetStreamPlayBuffer in streamHandle,
1 hwInitStream in sndStreamAllocEx,5 hwExitStream in sndStreamFree and1
aramUploadData in hwFlushStream. Those caller semantics and indirect/REL/dynamic
routes are NOT closed by this census. Each site has an original instruction
anchor; full caller bodies are not presented as complete.

Eight derived priority vectors and42 offset/count vectors pin width/alignment/
wrap deductions, without executing original code. Fresh complete ELFs/listings
stay `/tmp/inbox-aramupload.elf` and `/tmp/inbox-hwstream*.elf`. The generic
read-only verifier checks fingerprints/anchors, not physical transfer success.

Remaining: streamHandle/AllocEx/Free ownership and source providers, valid
index/count/priority/callback/offset/lifetime/concurrency domains, SDK OS/cache/
ARQ/DMA and original stream/sample/voice execution. Five local bodies are answered;
full audio/native/hardware/runtime equivalence remains OPEN.

## 2026-10-13 — Stream startup caller follow-up

[Startup](MUSYX_STREAM_THROTTLE_DISPATCH_AND_STARTUP.md) closes throttle/dispatch/state1/row tail, including
the play-buffer and initial flush call bindings. Refill/lifecycle/providers/
domains/original stream/audio/native binding remain open.
