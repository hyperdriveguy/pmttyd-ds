# Complete smart arena and header initialization

US G8ME01 revision0; ID2,2026-10-12. Complete smartInit8002fbe0/1e4.
Verify `python3 tools/verify_smart_initialization.py`;read
[provisioning requirements](SMART_RESOURCE_PROVISIONING_REQUIREMENTS.md),
[allocation](SMART_ALLOCATION_HEADER_AND_GAP_PLACEMENT.md),
[release](SMART_HEADER_RELEASE_AND_SPACE_ACCOUNTING.md).
Static evidence only;no original startup/arena/hardware execution trace.

## Heap-index4 request and conditional first clear/flush

Read heapStart[4] at8041e840,heapEnd[4] at8041e82c,then heapHandle[4] at8041e854.
Compute retained N=end-start-32 with32-bit wrap. Call OSAllocFromHeap(handle,N),
retain raw result B. No positive-length/bounds/handle validation occurs locally.
If B!=0,call memset(B,0,N),then DCFlushRange(B,N),ignoring returns. If B==0,
skip BOTH calls. The initial size/base are retained across these calls.
Underlying SDK allocation/clear/cache contracts are required backends.

## Live manager publication and unconditional second payload clear

Reload wp804151e0 manager W,store W00=B. Reload W,store used head e008=0;
reload W,store used tail e00c=0. Reread heapStart[4],heapEnd[4],then reload W;
store SAME W.e004=end-start-32 with word wrap. This gap can differ from N.

Reload W,reread start/end,read SAME W00 as destination,compute fresh size
end-start-32;call memset(destination,0,freshSize) UNCONDITIONALLY. There is no
second null-result guard. Allocation0 therefore still reaches this memory request
with whatever live manager base supplies;no graceful initialization-failure return
is provided. No second DCFlushRange follows this clear in the local body.

No old-arena/header/file handle release precedes these writes. Repeated init can
replace live state without locally freeing old backing storage or waiting for
pending requests. Full startup/reinit/scene ownership remains separate evidence.

## Clear2048 static headers,then write all forward/back links

Reload W;call memset(W+4,0,0xe000). This clears57344 header bytes,not the whole
manager. Reload W,capture cursor=W+4. Perform256 loop iterations,each containing
eight headers of stride0x1c. For j=0..7,in ascending order:

- Store header14=header+0x1c.
- Store header18=header-0x1c.

Advance cursor by0xe0 each iteration. Thus2048 headers occupy W+4 through
W+e003 inclusive. All tags,size,payload,owner,halfword0c and gaps are zero from
the header memset;only next/previous words are then assigned by the loop.
The first header's previous temporarily points W-24;the last header's next
points W+e004,one past the header array. Both are corrected separately below.
There is no per-iteration first/last branch or count publication in this loop.

## Publish available endpoints,terminate live links,and reset counters

Reload W,store its available head e010=W+4. Reload W/read current head e010;
store head18=0. Reload W,store its available tail e014=W+0xdfe8. Reload W/read
current tail e014;store tail14=0. Endpoint stores and termination can target
different managers/headers if wp or fields change at observable boundaries;
do not substitute the retained loop cursor/base for these live reads.

Reload W,store release/synchronization counter e018=0. Store global
 g_bFirstSmartAlloc8041e810=0. The next smartAlloc with that flag still0 sets1
then calls smartAutoFree(3);it does not infer its first-call state from free
header count. No available population counter is written;capacity follows the
explicit loop and endpoints under stable provisioning.

## Acceptance and remaining evidence

Check request read order/modular size,allocation0/nonzero,conditional retained
first clear/flush,live gap/second clear including null,header clear distinct from
manager,256 ordered eight-header link groups,temporary endpoint pointers,live
head/tail termination,counter/flag reset and repeated-init lack of cleanup.
Test bound/base/wp swaps independently;do not turn the source into a single
snapshot initializer or silently skip the second clear on allocation failure.

Arena bootstrap/SDK/cache/platform failures,source/tag/domain/scene ownership,
relocation/TPL/worker and original runtime remain EVT-01/JUMP-01/DMG-02 work.
This complete initializer does not prove that native game resources are bound
or that the complete game fits a hardware memory budget.
