# Complete file-cache collection and preload aging

US G8ME01 revision0; ID2,2026-10-12. Complete _fileGarbage8018a6e4/1ec.
Verify `python3 tools/verify_file_collection.py`;read
[pool initialization](FILE_CACHE_POOL_INITIALIZATION.md),
[acquisition](FILE_ACQUISITION_AND_REFERENCE_REUSE.md),
[async requests](FILE_ASYNC_REQUEST_AND_CACHE_PUBLICATION.md) and
[completion](FILE_ASYNC_COMPLETION_AND_DVD_REQUEST_STATE.md).
Static source reconstruction;no original collection/thread/runtime trace.

## Mode0 retires every exact-state3 entry

Compare full word argument K to0. If equal,capture current manager used head08.
Walk live E.a4;only exact U8 state3 is considered. If E.ac==0,write state2.
Otherwise call OSGetCurrentThread(),compare its raw address to
DVD worker object dvdmgr_thread803e5bb8.

When current thread equals that object,reload E.ac as D,store HALFWORD0008 to
D90,then write E.state=2. This OVERWRITES flags;it does not OR a close bit into
the previous flags. No local pending clear or yield occurs. The subsequent
collection pass can therefore reclaim this state2 entry while E.ac is nonzero;
actual worker cancellation,smart-heap interaction and valid caller domains
require separate evidence. Do not assume the collector always waits for I/O.

On any other thread,reread E.ac;while nonzero call OSYieldThread(),reread the
same retained entry's pending word. After it becomes0,write state2 without
rechecking state or count. Thread identity is queried once per admitted pending
entry;there is no timeout,callback-result check or event-credit protocol here.
Other states are skipped entirely in this mode's first pass.

## Mode1 ages only entries whose initial pending read was0

If K!=0,compare EXACT1. For1,capture current manager used head08 and inspect
exact state3 nodes. Read pending E.ac once for branch selection.

If nonzero,query current thread. DVD worker equality skips the entry unchanged.
Other threads wait with pending reread/yield until0,then advance to the next
entry WITHOUT decrementing its count or changing state locally. Callback side
effects remain whatever the separately recovered completion performed. A port
must not fall through from this wait into the ready-entry aging path.

If the initial pending read was0,read S16 count E2,subtract1,storelow16,RELOAD
S16(E2). Only when that reloaded signed value<=-2,store state2. Otherwise
preserve state3. This is an invocation-based aging rule,not a duration measured
in frames/seconds or a general positive owner-reference decrement.

| Initially ready state3 count | After this mode1 call | State |
| --- | --- | --- |
| 1 | 0 | 3 |
| 0 | -1 | 3 |
| -1 | -2 | 2 |
| -2 | -3 | 2 |
| -32768 | 32767 | 3 |

State1 live acquisitions are not aged by either first pass. Any K other than0
or1 skips the first pass entirely and still executes collection below.

## Second pass collects exact-state2 regardless of count or pending

Reload current manager,capture used head08. Initialize retained FIRST=0 and
PREVIOUS=0. At every node E,read state THEN capture NEXT=E.a4 before any calls.
Only exact state2 is removed. This pass does not test count,pending,type,name,
callback or holder nullness. It does not invoke the user callback or close DVD.

For removed E,store state0 FIRST. Read E.a0 as H;compare raw H to E+4. Exact
identity means embedded archive holder:skip smartFree. Any other value,
INCLUDING0,requests smartFree(H),with no local null guard or return check.
The [complete smartFree](../memory/SMART_HEADER_RELEASE_AND_SPACE_ACCOUNTING.md) now
supplies header release;relocation remains separate.
The comparison establishes archive-versus-smart-header release routing;it does
not prove arbitrary raw H is valid or that the original source admits null H.

If PREVIOUS!=0,reread CURRENT E.a4 after smartFree,store it at PREVIOUS.a4.
No retained NEXT substitute is used for this unlink store. Then reload manager:
if its available head10==0,store head10=E;otherwise read its tail14 and store
TAIL.a4=E,without a tail-null guard. Store E.a4=0,reload manager,store tail14=E.
Keep FIRST and PREVIOUS unchanged. Traverse retained NEXT,not the reread link.

For a node not removed,if PREVIOUS==0 set FIRST=E;then set PREVIOUS=E. Traverse
retained NEXT. Finally reload manager,store used head08=FIRST;reload manager
again,store used tail0c=PREVIOUS. An all-removed or initially empty used chain
publishes both0. Remaining state0/unknown-state nodes survive this pass.

Thus removal appends available nodes in used traversal order. It clears only
state and next locally;count/type/name/holder/callback/pending and other bytes
remain stale. The holder can reference reclaimed smart storage. No entry
memset,filename erasure,count reset,generation bump or pool deallocation occurs.

## Live versus retained ownership requirements

State/next capture precedes smartFree;unlink uses the later live E.a4;traversal
uses earlier NEXT. Available-list updates and final used-head/tail publication
reload the manager at the displayed boundaries. FIRST/PREVIOUS/E/NEXT remain
retained across calls. Preserve these distinctions if a memory/service provider
can mutate entries or swap afp;do not treat collection as an atomic container
filter followed by immediate generic free of every payload pointer.

Acquisition first calls this function with1 when available head is null,then0
if still null. Mode1 ready count0 only reaches-1;that call alone does not retire
it. Mode0 can then force state2 in its separate pass. A caller with another K
still collects previously released state2 entries without aging preloads.
fileFree's count1→0/state2 transition does not deallocate itself;this collector
supplies the later smartFree request except for an embedded archive holder.

## Acceptance and remaining requirements

Check all K values,state bytes,pending branches,worker identity,overwrite0008
versus OR,signed16 wrap/reload,initially pending wait without aging,state2 with
nonzero pending/count,embedded holder versus0/other header,smartFree mutations,
retained traversal versus live unlink,head/middle/tail/consecutive/all removals,
available append order and manager swaps before every publication.

Complete OSGetCurrentThread8029c7ac/c reads WORD800000e4 and returns it;its
system scheduling/provisioning is outside this leaf. Smart heap,relocation,
DVD worker/platform cancellation/source validity/scene cadence,TPL consumers
and original traces remain EVT-01/JUMP-01/DMG-02 requirements. No collection
frequency or successful original reclamation is inferred from static evidence.

## Scoped direct caller census

The [DOL call-site census](../../../tools/file_collection_call_sites.json) pins
six direct collection sites:smartAlloc8002f7c4/8002f7d0,
fileAsync80189dec/80189e04 and _fileAlloc8018a360/8018a378. It also pins one
fileInit and ten archive-mode setter sites. Verifier scans all seven DOL text
sections for aligned direct bl/bla targets and exact instructions. No REL,
indirect function calls,raw-pointer references or runtime invocation counts are
covered. Smart allocator policy and other caller graphs remain separate work.

[Smart header release](../memory/SMART_HEADER_RELEASE_AND_SPACE_ACCOUNTING.md) performs
no heap-free call:tag4 only changes to3;normal release updates lists/space and
recycles header metadata while preserving its payload pointer. File-entry
recycling continues in either case. [Automatic tag release](../memory/SMART_AUTOMATIC_TAG_RELEASE.md)
has a separate two-pass/full-word3 policy. Full allocator/compaction remains open.
