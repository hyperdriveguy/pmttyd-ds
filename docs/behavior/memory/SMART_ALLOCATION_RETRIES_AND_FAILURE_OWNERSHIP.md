# Complete smart-allocation retries and failure ownership

US G8ME01 revision0; ID2,2026-10-12. Complete remaining retry paths of
smartAlloc8002f540/338. Verify `python3 tools/verify_smart_allocation.py`;read
[reservation/immediate placement](SMART_ALLOCATION_HEADER_AND_GAP_PLACEMENT.md),
[collection](../file/FILE_CACHE_COLLECTION_AND_PRELOAD_AGING.md),
[compaction](SMART_HEAP_COMPACTION_AND_PENDING_FILES.md) and
[header release](SMART_HEADER_RELEASE_AND_SPACE_ACCOUNTING.md).
No original pressure/failure/runtime trace or bounded-allocation guarantee.

## Enter retries after reserving and initializing one header

Only after leading gap and all used-header gaps fail,set local retry counter R=0.
The incoming tag previously retained in the same register has already been
stored as low8 to reserved H0e;it does NOT select retry policy. H and aligned
size N remain retained from the initial reservation. No new header is selected
or reinitialized on each retry.

| Retry R | Collection request before compaction | Gap checked afterward |
| --- | --- | --- |
| 0 | None | Current used tail10 only |
| 1 | _fileGarbage(1) | Current used tail10 only |
| 2 | _fileGarbage(0) | Current used tail10 only |

Each row calls smartGarbage() after its collection decision. Ignore all service
returns. Then reload current wp/read used tail e00c as P and read WORD P10 with
NO null-tail guard. Compare unsigned gap>=N. If insufficient,increment R;repeat
while signed R<3. The first retry is compaction alone,not mode0 collection.
No yield/retry delay,new draw-sync admission check or repeated first-flag check
is supplied by this outer loop;the callees have their own recovered operations.

## Tail-only placement can differ from a fresh full allocation search

A sufficient tail gap uses the same exact insertion sequence as immediate
used-gap placement:read P00/P04;publish H00=sum;reread P10;publish H10=gap-N;
read P14;publish H14 and H18=P;clear P10;reread P14 and repair next18 or current
manager's used tail;finally P14=H and return H. Even though P came from the
manager's tail field,its next is not assumed0. Corrupt/aliased source states
remain literal memory-provider cases;no chain validity assertion exists.

The retry path NEVER rescans leading gap or every used node after compaction.
Pending-file movement barriers can leave interior gaps;an interior/leading gap
that now fits does not become a local retry success unless the selected tail10
also fits. Do not substitute a generic retry of the whole allocation function.

If collection empties the used chain,smartGarbage can publish reusable leading
space but the allocator STILL dereferences current tail0+10 afterward. It does
not place H into an empty arena using that new leading gap at this point.
Host providers may surface invalid memory explicitly;do not claim an original
null-safe recovery path or guaranteed out-of-memory result for this boundary.

## Three failed tail checks return0 without restoring the reserved header

After R increments from2 to3,return0. No smartFree(H),available-chain append,
used-chain insertion,header memset,tag restore or request rollback occurs.
The earlier available-header unlink and H0c=1/H0e=low8T/H04=N/H08=0 remain.
Other header bytes remain whatever their initial/live service state supplied.
Thus local0 does not imply that header inventory is unchanged or that the
failed request was transactional. Valid source failure domains and any external
recovery need their own evidence;do not add an implicit generic free on failure.

The function returns0 only through this final explicit exhaustion branch;
null available head and null retry tail instead encounter unchecked dereferences.
Other providers can themselves fail/throw/abort through their separate contracts.
No guaranteed allocation success or defined scalar error enum is inferred.

## File and smart-release composition

DVD file acquisition calls smartAlloc(roundedLength,0) and immediately writes
H08=fileEntry without checking returned H. Async preload does the same before
publishing/opening a second handle. Therefore this allocator's explicit0 result
is not locally converted by those callers into a safe cache miss;their unchecked
header write remains part of the composed graph. Header0 does not mean payload0.

Normal smartFree increments W.e018;the next allocator reads nonzero,requests
sysWaitDrawSync and resets current manager's counter0 before header selection.
This proves e018 is not a maintained available-header population count. The
counter has modular increment/reset behavior with independent head/tail state;
capacity/initial count and synchronization consumers require arena/platform
contracts. Deferred tag4→3 release bypasses that increment entirely.

## Native acceptance and remaining requirements

Use one retained reserved H and N for all retries. Check exact0/1/2 service order,
all three early successes,three failures with no restoration,tail-only checking,
interior/leading gap after compaction,null-tail boundary,callee manager/header
mutations,live next even for a selected tail,low8 incoming tag preserved despite
retry-register reuse,and file-callers' unchecked0 result. Preserve original
counter reset/first-flag ordering rather than inventing a stock allocator API.

Complete arena initialization,source counts/valid pressure/pending states,
relocation/worker/platform draw-sync/cache/performance/scene and original
execution remain open. Reused original assets still need backing memory and
address relocation providers;these contracts supply behavior without asserting
total full-game RAM or complete original loading/rendering fidelity.
