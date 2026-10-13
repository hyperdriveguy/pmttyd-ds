# MusyX ARAM initialization, zero-buffer upload and allocator hooks

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete aramInit802918d0/2e4,
salMallocPhysical802928b4/30 and salFree802928e4/30hex. Read
[stream-buffer lifecycle](MUSYX_ARAM_STREAM_BUFFER_LISTS_AND_LIFETIME.md),
[sample store](MUSYX_ARAM_SAMPLE_STORE_AND_CURSOR_REWIND.md) and
[queue completion](MUSYX_ARAM_QUEUE_COMPLETION_AND_SAMPLE_REQUESTS.md).
Hexadecimal offsets/addresses below; explicit decimal counts are labeled.

## Zero-buffer preparation and reset order

Retain input user-size word U. Request ARGetBaseAddress802a8e00 and retain B.
Request salMallocPhysical(500); retain returned CPU pointer P. No null test.
Write zero halfwords to all500 bytes:16 decimal blocks of50hex bytes,
ascending40 decimal halfword stores per block. Request DCFlushRange(P,500).
Then reset low queue count(byte281) to0, low index(byte280) to0, high count0,
high index0, in that order. Slot bodies/padding are not cleared; no old-request
cancel/drain or IRQ disable precedes this reset. Safe initialization/reinitialization
requires external ownership of outstanding callbacks and requests.

## Zero upload, wait and release

Attempt low-queue admission under OSDisableInterrupts; count>=16 restores state
and retries. Write the same nine request words/order as the sample producer:
owner42 decimal/type0/priority0/sourceP/destinationB/length500/aramQueueCallback/
userCallback0/userArg0. Word0 remains unwritten; index is independently reloaded
for each store and each post argument. Request ARQPostRequest, then fresh pending
count increment, fresh index modulo16, OSRestoreInterrupts(savedState).
This is post-before-count/index publication, matching the queue contract.

Busy-wait on LOW queue count until0. No timeout, high-queue wait, scheduler/pad
or SDK wait service. Only after that loop request salFree(P). This establishes
local free ordering; actual DMA/callback semantics and P's allocator remain child
services. Neither the reset nor the wait proves original audible output.

## Limits and stream state publication

Store aramTop8041efd8=wrap32(B+U). Request ARGetSize802a8e08. Freshly read top
and unsigned-compare to that result; if top is greater, request ARGetSize AGAIN
and store that second return to top. Do not capture a single stable size or
rewrite this as widened/saturating base-relative capacity arithmetic.
Then store aramUploadCallback8041efd0=0, and request InitStreamBuffers80292094.
Upload chunk size8041efcc remains unchanged. Return without a local independent
success code; hook allocation/post/completion/free errors are not classified.
No source sample-bank load or live default-cache reset occurs here.

## Complete allocation/free hook leaves

salHooks80410348 is three words. salMallocPhysical retains INPUT r3 as the
allocation-size argument, loads hook word4 and indirectly invokes it. salFree
retains INPUT r3 as the pointer argument, loads hook word8 and invokes it.
Both preserve the hook's r3 return; neither checks hook/null/allocation results,
adds alignment, converts addresses or owns memory by itself. Other outgoing
volatile registers are not normalized by these bodies. Hook installation,
provider contracts and actual physical allocation/free remain separate.

## Evidence and remaining owners

[Manifest](../../../tools/musyx_aram_init_stream_evidence.json) pins these three
complete bodies and five stream/default-cache bodies,251 source anchors and14
resolved direct calls, with eight linked SDK/helper identities. Fresh complete
ELFs/listings stay `/tmp/inbox-araminit.elf`, `/tmp/inbox-sal*.elf` and related
stream inspection files. Derived chain/index/search metadata is not execution.

Remaining: hook installation/provider allocation, actual base/size/cache defaults,
queue initialization/concurrency invariants, ARQ/OS/cache/DMA, sample-bank loading
and original runtime/hardware traces. Three complete local bodies are answered;
full audio/source equivalence remains OPEN.
