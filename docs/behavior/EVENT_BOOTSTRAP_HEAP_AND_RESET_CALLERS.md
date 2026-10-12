# Event bootstrap heap allocation and scoped reset caller bindings

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete __memAlloc800300f0/64 and OSAllocFromHeap802964ac/fc;scoped four
DOL caller sites,not complete caller bodies. Read
[context init/reset](EVENT_CONTEXT_INITIALIZATION_AND_RESET.md),
[user map allocation/fill/cache](EVENT_USER_BUFFER_AND_MAP_ALLOCATION.md) and
[saved RAM routing](EVENT_SAVED_SWITCH_STORAGE.md).
`python3 tools/verify_event_context_initialization.py` pins whole allocation/
fill/cache helpers,init/reset/message leaf and direct call census/anchors.
Static original DOL evidence;no original heap allocation/reset execution claim.

## __memAlloc complete wrapper and distinct heap root

Inputs are heapIndex r3 and byteCount r4. Compute offset wrap32(heapIndex<<2);
load heap HANDLE from table8041e844+offset (r13+1924). Preserve requested byte
count. Call OSAllocFromHeap(handle,byteCount). If returned pointer!=0,
memset(pointer,0,ORIGINAL requested byteCount),then DCFlushRange(pointer,same
count). Return allocator pointer whether success or0. No table bounds,count/
null-input validation,logging/retry,failure callback or scalar error code.

Event Init calls this wrapper twice with index0,count1b000. Table entries are
handles passed to SDK heap descriptors,not payload/free-list pointers. This is
separate from opcode76's _mapAlloc(root at8041e808,count):do not substitute that
map allocator's best-fit/list/header policy for event-array bootstrap allocation.
The whole fill/cache local leaves and system-call boundary are in the linked
user-map contract;their semantics do not imply common allocation ownership.

## Complete OSAllocFromHeap first-fit and split policy

Heap descriptor base pointer is loaded from8041f070 = r13+2150. Calculate
D=base+wrap32(handle*12). Descriptor D+4 is free-list head,D+8 allocated-list
head;D+0 is unwritten here. Free/allocated nodes use previous word+0,next word+4,
size word+8;their header occupies32 bytes by the returned payload offset.
No handle range/base/null/list-cycle/alignment validation is performed.

Compute required total span T=wrap32(requestedByteCount+63) AND0xffffffe0.
This is32-byte header plus payload alignment for normal nonnegative requests,
using original WORD arithmetic. It differs from _mapAlloc's +31 payload rounding
then +32 span. Start at free-list head;for each nonnull node N,read size S and
compare SIGNED T<=S. First adequate node is chosen;otherwise follow N.next.
No adequate node returns0 without mutation. This is first fit,not best fit.
The caller receives no original requested-byte-count field in the returned node.

For chosen node,compute unsigned remainder R=wrap32(S-T). If R<64,consume the
ENTIRE node without changing its size field;unlink it from free list:

1. Capture N.next and descriptor free head. If next exists,set next.previous
   to N.previous.
2. Read N.previous. If null,new free head=N.next;otherwise set previous.next
   to N.next and retain old head.
3. Store resulting descriptor.freeHead.

If R>=64,split:

1. Store N.size=T;J=wrap32(N+T);store J.size=R.
2. Copy N.previous→J.previous,then N.next→J.next.
3. If J.next exists,set next.previous=J. If J.previous exists,set previous.next=J;
   otherwise descriptor.freeHead=J.

In BOTH branches prepend N to allocated list:read old allocated head A;store
N.next=A;store N.previous=0;if A exists,store A.previous=N;store
D.allocatedHead=N. Return payload N+32. No zero-fill or cache instruction in
this SDK body;__memAlloc performs those AFTER return. No local rollback,
coalescing,heap extent assertion or requested-size sanity check.

Normal event count1b000 requests SDK total span1b020. A remainder<64 is absorbed,
so the consumed block's actual extent may exceed that total. The wrapper clears
only requested payload bytes1b000,not absorbed tail/header bytes. SDK heap
initialization,handle0 mapping,free/coalescing,heap region capacity and hardware
coherence remain separate source/platform obligations. Invalid signed sizes/
wrap/handle/list domains are not silently normalized into host malloc semantics.

## Four direct DOL init/reset bindings

A scan of aligned branch-with-link words in the seven DOL text sections finds
exactly these four branch-shaped direct calls to the two functions. The verifier
repeats that census and checks surrounding selection/write anchors. It does not
inspect REL code,indirect calls,data-driven references,other pointer aliases or
original runtime execution. Containing function names come from the supplied
US symbol map. Their FULL behavior and external callees are not recovered here.

| Site | Containing function | Call / proven local ordering |
| --- | --- | --- |
| 8006fde0 | marioStInit | Calls evtmgrInit;following direct call atfde4 is80042854. Both contexts are initialized by the called body. |
| 80007b3c | _unload | Calls evtmgrReInit;following call at7b40 is8006ed28. No local selector override at this call site. |
| 80073934 | battle_init | Immediately before call,reload GP and store GP14=1 at80073930;ReInit therefore selects battle under this local ordering. |
| 8007377c | battle_exit | ReInit precedes call8004cd38 at73780;THEN reload GP/store GP14=0 at73790;THEN evtStartAll(mask4) at73794. |

battle_exit does not set GP14=0 before its reset call. The selected context at
ReInit is whichever CURRENT GP14 says at that instant;local exit-site ordering
alone is not proof prior callees never change it. After its explicit0 write,
StartAll4 selects the field context through its own live GP capture. This
supplies an original field resume REQUEST,not complete battle-end/teardown or
executed party/story continuation. _unload likewise resets live selected context;
its name must not substitute for field-only evidence.

Caller symbols and numeric targets are breadcrumbs for further implementation
bindings. Their neighboring functions are not assigned guessed semantic names.
Full GP14 producer/caller/scene/pause/heap teardown graphs,REL/indirect census,
source-domain constraints and original transition/reset traces remain EVT-01
and field/battle integration requests. The global event allocation count reset
is separate from SDK allocated-list mutation and does not free array buffers.

Acceptance:handle table versus map root,SDK first fit,signed adequacy/no fit,
remainder63/64 threshold,whole-block versus split header/backlink write order,
allocated-list prepend,payload+32,wrapper null/success zero/cache extents;two
Init requests0/1b000;four exact direct calls,battle_init selector BEFORE reset,
battle_exit selector AFTER reset and field StartAll4 request. Original heap/
scene/source/callback/address/numeric/domain/runtime closure remains open.


## Complete heap release/bootstrap follow-up (2026-10-12)

[Map free/battle pool](MAP_BUFFER_FREE_AND_BATTLE_POOL.md),
[pool bootstrap/bulk clear](MEMORY_POOL_BOOTSTRAP_AND_CLEAR.md) and
[SDK free/direct census](SDK_HEAP_RELEASE_AND_CALLER_CENSUS.md) now close local
initialization/free/coalescing/reset bodies. Map+32 payload sizing and SDK total
sizing/free/null rules differ;bulk reset does not invoke per-owner destructors.
Per-event/scene/REL/indirect ownership,arena/alias/domain/platform and original
traces remain open. Earlier local free/bootstrap gaps are superseded;automatic
E154/E158 ownership/free remains unsupported without source lifetime evidence.
