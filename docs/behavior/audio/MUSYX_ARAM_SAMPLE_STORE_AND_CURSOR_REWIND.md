# MusyX ARAM sample store, cursor rewind and upload configuration

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete aramStoreData80291c58/424,
aramRemoveData8029207c/18, aramSetUploadCallback80291c00/58,
aramGetZeroBuffer80291bb4/20, aramGetFirstUserAddress80291bd4/24 and
aramGetUserBytes80291bf8/8hex. Read [sample bridge](MUSYX_SAMPLE_STORAGE_HARDWARE_BRIDGE.md)
and [queue completion](MUSYX_ARAM_QUEUE_COMPLETION_AND_SAMPLE_REQUESTS.md).
Offsets/addresses/sizes below are hexadecimal unless labeled decimal.

## Caller cache and admission

Store inputs: CPU source S, byte count B, caller cache pointer C. Retain S/C,
compute L=wrap32(B+1f)&ffffffe0, then request ARGetBaseAddress802a8e00.
If C.word0 equals wrap32(returnedBase+500), compare unsigned
wrap32(C.word8+L) against aramStream8041efd4; otherwise compare that sum
against C.word4. Greater refuses with return0; equality admits. No null/cache/
source/alignment/overflow guard. Both paths reload C.word8 for the capacity sum.
The addition can wrap; the source does not replace it with widened arithmetic.

After admission, read aramUploadCallback8041efd0 and retain initial C.word8 as
return address R. Null callback takes the direct path; nonnull takes the chunk
path. R is retained across child requests; a later DMA destination or final
cursor may differ if C is mutated. The capacity check is not repeated per chunk.

## Direct upload and cursor publication

Request DCFlushRange(S,L)80296a54, then capture fresh C.word8 as destination D.
Attempt low-queue publication under OSDisableInterrupts; a full queue restores
interrupt state and retries WITHOUT repeating flush or recapturing D. On admission,
construct/post the request described in the [queue contract](MUSYX_ARAM_QUEUE_COMPLETION_AND_SAMPLE_REQUESTS.md),
then increment its count/index and OSRestoreInterrupts(savedState).
Only afterward reload C.word8, add L with word wrap and store it to C.word8.
Return retained R. This path does not wait for the posted transfer to complete.
L0 still requests flush/post and stores the unchanged fresh cursor; there is no
local zero-byte bypass. SDK behavior for a zero-byte post remains separate.

## Callback chunk upload

Before every nonempty chunk, busy-wait until lowQueue.byte281==0. No timeout,
sleep, yield, event tick or local interrupt-disable around that wait. Choose
K=min_unsigned(remaining L, fresh aramUploadChunkSize8041efcc); reload the global
callback and invoke callback(current logical source S,K). Retain its returned
CPU pointer P. Request DCFlushRange(P,K); retain fresh C.word8 as destination D.
Retry low-queue admission under disable/restore as above, then publish sourceP,
destinationD,lengthK. After restoring interrupts: reload C.word8, reduce remaining
by K, advance logical S by K, and store fresh cursor+K. Loop until remaining0,
then return R. The LAST request is not drained before return; the next chunk's
pre-wait drains the preceding request. Initial L0 skips every chunk/wait/request.

The callback return is used without null/error checking. Callback/chunk size are
fresh global reads on each chunk; changing callback to0 can invalidate the later
indirect call. Chunk size0 with nonzero remaining has no progress safeguard.
No staging buffer ownership, alignment proof, callback implementation or actual
DMA timing is established by this body. Source advancement uses K, not returnedP.

## Complete removal: cursor rewind only

aramRemoveData(rawAddress,B,C) ignores rawAddress entirely. Compute the same
L=wrap32(B+1f)&ffffffe0, read C.word8, subtract L with word wrap, store C.word8.
No queue synchronization, interrupt service, allocation/free/cache call, address
comparison, resource lookup or memory erasure. It leaves r3=L on return, without
an independent success assignment. Removing a non-tail allocation does not
search/reclaim that address; valid removal order is a caller prerequisite.

This explains why [sample reference release](MUSYX_SAMPLE_DIRECTORY_REGISTRATION_AND_REFERENCES.md)
retains the old sample address: the local child rewinds a cache cursor. It does
not itself stop voices or prove that hardware no longer references the region.

## Upload configuration and address leaves

SetUploadCallback(F,B): retain F. If nonzero, align B with the same wrapping
32-byte rule, request ARQGetChunkSize802aaa48 and store the UNSIGNED MAXIMUM
of alignedB and that returned word to aramUploadChunkSize. Then store F to
aramUploadCallback. F0 skips chunk-size update and still clears callback.
No IRQ/synchronization, prior-request drain, zero guard or independent return.

GetZeroBuffer requests ARGetBaseAddress and returns its raw result.
GetFirstUserAddress requests that service and returns wrap32(result+500).
GetUserBytes does NOT query ARAM capacity: it returns wrap32(INPUT r3-500).
Its source input domain/callers remain required; no global free-space read.

## Validation and remaining ownership

[Evidence manifest](../../../tools/musyx_aram_sample_storage_evidence.json) pins
six complete bodies here and two queue bodies, linked SDK/helper fingerprints,
207 instruction anchors and16 independently resolved direct calls. Eight derived
32-bit alignment/rewind vectors include wrap/refusal prerequisites;256 emitted
ring-index arithmetic checks and five callback-count boundaries are separate
from original execution. Full inspection ELFs/listings stay `/tmp/inbox-aram*.elf`.
Generic read-only verifier checks identities/anchors, not actual DMA completion.

Remaining: aramInit/stream/cache producers, caller removal order, callback/source
providers, OS/SDK flush/ARQ/DMA services, valid concurrency/residency and original
sample/voice/playback traces. Six local bodies are answered; full audio remains OPEN.

[Complete initialization/allocator hooks](MUSYX_ARAM_INITIALIZATION_AND_ALLOCATOR_HOOKS.md) and [stream-buffer boundary/list producers](MUSYX_ARAM_STREAM_BUFFER_LISTS_AND_LIFETIME.md) now answer those exact local dependencies. Default-cache contents, source/list validity, SDK/DMA/voice and original runtime remain open.
