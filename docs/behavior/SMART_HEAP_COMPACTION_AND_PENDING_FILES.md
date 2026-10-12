# Complete smart-heap compaction and pending-file barriers

US G8ME01 revision0; ID2,2026-10-12. Complete smartGarbage8002f378/1c8.
Verify `python3 tools/verify_smart_compaction.py`;read
[file move](FILE_PAYLOAD_MOVE_AND_ADDRESS_REQUESTS.md),
[header release/gaps](SMART_HEADER_RELEASE_AND_SPACE_ACCOUNTING.md),
[file async completion](FILE_ASYNC_COMPLETION_AND_DVD_REQUEST_STATE.md).
Static reconstruction only;original rendering/thread/movement traces unmeasured.

## Draw-sync request precedes all manager reads

First call sysWaitDrawSync(),ignore return. Its full synchronization/platform
behavior is a linked backend,not recovered here. Then read current wp804151e0
manager W and its used head e008. If nonzero,store SAME W.e004=0. If zero,
read heapStart[4],then heapEnd[4],compute end-start-32 with word wrap and store
SAME W.e004. No positive-size check or pool cleanup occurs.

The bound words are heapStart8041e830+10hex and heapEnd8041e81c+10hex.
These are live heap-index4 bounds,not measured 2DS RAM addresses/capacity.
Reload manager;capture TARGET=WORD W00,used head H=WORD W.e008,PREVIOUS=0.
W00 is the current payload-arena base;header addresses remain separate.

## Equal source and target bypass movement and pending inspection

For each retained header H,read SOURCE=WORD H00. If SOURCE==TARGET,skip all
owner/state/pending/thread checks and movement requests. Clear H10 gap,reload
CURRENT H00 and H04,compute TARGET=payload+size with word wrap. Then set
PREVIOUS=H and read live H14 as next. No header links/tags/counts are rewritten.
The later tail-gap finalization can overwrite a tail's just-cleared gap.

## Different address with no owner uses memmove and publishes header pointer

If SOURCE!=TARGET,read owner E=H08. Exact0 requests
memmove(TARGET,retained SOURCE,WORD H04),ignores return,stores H00=TARGET.
Then clear gap and advance from current H00/H04 as above. No pointer relocation
or file callback occurs on this unowned path. Payload length is raw word size;
no overlap/bounds/null validation occurs locally. memmove's full copy engine
is linked evidence,not a complete SDK implementation in this contract.

## Pending state3 file on another thread becomes a movement barrier

For nonzero E,read BYTE E00. Only exact3 then reads E.ac;only nonzero pending
queries OSGetCurrentThread(). Compare its address to dvdmgr_thread803e5bb8.
If NOT that worker,take the barrier branch without yielding,clearing pending,
closing/canceling the handle or calling fileGarbageMoveMem.

Compute GAP=retained SOURCE-TARGET with word wrap. If PREVIOUS!=0,store GAP to
PREVIOUS10. Otherwise reload manager and store its e004=GAP. Reload CURRENT
H00 and H04,clear H10,compute TARGET=payload+size. Set PREVIOUS=H and read live
H14. This preserves the pending payload's current location and records space
before it. It does not mark the header or change its owner/state/tag/count.

Thread is queried once per admitted address-different pending file. If it IS
the worker,proceed with movement below. Ready state3,all other state bytes and
pending0 also move;there is no broader file-state validation.

## Other owned files request the file-move helper with a live owner reread

Reload CURRENT H08 as E;call fileGarbageMoveMem(TARGET,E),ignore return.
The helper reads the file entry's CURRENT holder and moves/publishes through
that holder. smartGarbage does not directly assign H00 on this path. After the
call,clear H10 and advance TARGET from CURRENT H00+H04,then PREVIOUS=H and read
live H14. Owner/holder aliasing therefore matters:the callback may update a
holder other than H,and compaction still uses H's own live words afterward.
No final owner-null check is supplied before the helper's entry dereference.

Do not use a saved next link across these calls;this compactor reads H14 after
movement. Preserve captured SOURCE for the unowned copy/barrier delta but live
H00/H04 for later target advancement. Current manager reloads remain explicit.

## Recompute tail gap,then flush the complete arena request range

After traversal,reload current W and read its used tail T=W.e00c. If nonnull,
read heapStart[4],then heapEnd[4],then SAME W00 arena base. Compute LIMIT=
end-start+base-32 with word wrap. Read CURRENT T00 payload and T04 size;store
T10=LIMIT-payload-size with word wrap. No consistency check compares T to the
last traversed header or validates a nonnegative gap.

Finally reread current wp,heapStart[4],heapEnd[4] and that manager's W00 in the
literal instruction order;request DCFlushRange(arenaBase,end-start-32),ignore
return. This call occurs even for an empty used list or a zero/wrapped range.
It flushes the declared arena range,not only moved bytes. Its full SDK/cache
execution semantics and original physical coherence remain platform evidence.

## Implementation requirements and acceptance

Keep headers stable while payload pointers move. Preserve draw-sync-before-read,
source equality fast path,unowned memmove,exact state3/pending/thread barrier,
worker movement,previous/leading gap accounting,live owner/holder/next reads,
word-wrap arithmetic,tail finalization and full-range flush request.

Exercise empty/one/many headers,equal/mismatched addresses,owner0/nonzero,state
bytes,pending0/nonzero,worker/nonworker,barriers at head/middle/tail,overlap,
zero/wrapped lengths,bound/manager swaps,helper mutations and owner/holder
aliasing. Verify no local yield/deallocation/list reorder/tag mutation and no
pending-state test on the equal-address fast path. Source bounds/data validity
must be explicit providers;host arena addresses are a separate mapping.

Smart allocation/arena provisioning,type-dependent relocation,draw-sync/cache
platform bodies,DVD worker/source/scene cadence/TPL and original traces remain
EVT-01/JUMP-01/DMG-02 requirements. This full local compactor does not establish
a complete moving allocator or original resource rendering fidelity.

[Complete allocator](SMART_ALLOCATION_HEADER_AND_GAP_PLACEMENT.md) and
[retry ownership](SMART_ALLOCATION_RETRIES_AND_FAILURE_OWNERSHIP.md) now close
this compactor's local pressure caller. Three retries use tail-only checks;
newly reusable leading/interior space does not imply retry success. Arena/
relocation/source/platform/runtime remain separate.
