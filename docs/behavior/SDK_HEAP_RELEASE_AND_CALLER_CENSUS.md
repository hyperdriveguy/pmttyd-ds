# Complete SDK heap release/coalescing and scoped DOL caller census

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete __memFree800300c4/2c,OSFreeToHeap802965a8/7c and
DLInsert80296400/ac. Read [SDK allocation](EVENT_BOOTSTRAP_HEAP_AND_RESET_CALLERS.md),
[pool bootstrap/reset](MEMORY_POOL_BOOTSTRAP_AND_CLEAR.md),
[map release/battle pool](MAP_BUFFER_FREE_AND_BATTLE_POOL.md) and
[event deletion](EVENT_DELETE_AND_COPYBACK.md).
`python3 tools/verify_event_heap_release_and_bootstrap.py` pins complete/linked
bodies and all50 branch-shaped direct DOL sites described below. Static original
DOL evidence;no original ownership/free/cancellation execution observation.

## __memFree wrapper and allocated-list removal

__memFree(index,payload) computes index<<2,loads SDK handle from8041e844+offset,
then calls OSFreeToHeap(handle,payload). No null/index/size/ownership checks,
payload clear/cache flush or scalar result contract. This wrapper differs from
_mapFree's explicit null-payload gate;do not share an inferred no-op nullptr
policy across both allocator families.

OSFreeToHeap captures H=payload-32 and descriptor D=base8041f070+handle*12.
Read H.next (word+4) and descriptor allocatedHead(D+8). If H.next!=0,read
H.previous and store it to next.previous. Then read H.previous:if null,set
retained allocatedHead=H.next;otherwise set previous.next=H.next. Store retained
head to D+8. Read D.freeHead(+4);call DLInsert(freeHead,H);store its returned
head to D+4. No null/active flag/pool membership/handle/list validation,zero-fill,
SDK accounting decrement or callback. A stale/unallocated/foreign block is not
locally rejected. There is no universal idempotence guarantee.

## DLInsert exact address insertion and adjacency coalescing

Inputs are original free head F and new node N. Keep original F as eventual
return unless N becomes head. Scan current C from F,previous P initially0.
Continue while C!=0 AND unsigned N>C;on each advance set P=C,C=C.next.
Stop at first C>=N or end. Equal-address insertion is not rejected.
Store N.next=C,then N.previous=P. If C!=0:

1. Store C.previous=N.
2. Read N.size;compute wrap32(N+N.size). If unequal to C,skip forward merge.
3. If exact adjacency,read C.size,store N.size=wrap32(N.size+C.size),capture
   C.next as new C,store N.next=C;if new C!=0,store C.previous=N.

Then if P!=0,store P.next=N;read P.size;compare wrap32(P+P.size)==N. If NOT
adjacent,return original F immediately. If adjacent,read updated N.size,store
P.size=wrap32(P.size+N.size),store P.next=C;if C!=0,store C.previous=P;
return original F. If P==0,return N as new head.

The SDK node size includes its header,so merged sizes are simply ADDED;there
is no extra+32 as in _mapFree. Forward merge precedes predecessor merge;stale
removed headers remain uncleared. At most one next and one previous node are
merged locally;valid sorted/coalesced-list invariants supply the rest. No search
length/cycle/range/overflow check. Pointer ordering is original unsigned32
address arithmetic,not host comparisons between arbitrary C++ allocations.
Insertion/forward/backward updates are separate stores,not an atomic list swap.

## Scoped50-site census and ownership boundary

Scan aligned branch-with-link words in the seven original DOL text sections,
matching targets _mapFree,memClear,memInit,N_battleMapFree,N_battleMapAlloc.
The verifier requires exactly these50 site/target pairs. This is a branch-shaped
DOL census,not a complete dynamic graph;REL/indirect calls,pointer aliases,
raw memory writers and caller bodies remain separately scoped. Names below
come from the supplied US symbol map. No allocation/free argument or lifetime
policy is inferred merely from a containing symbol name.

| Call site | Target | Containing symbol |
| --- | --- | --- |
| 800075c0 | 8002fdc4 | _relUnLoad |
| 80007bf4 | 80030154 | _unload |
| 80007bfc | 80030154 | _unload |
| 80007e58 | 8002fdc4 | seq_mapChangeMain |
| 8000a59c | 8002fdc4 | bgReEntry |
| 80018e40 | 8002fdc4 | hitReInit |
| 80018e60 | 8002fdc4 | hitReInit |
| 8001d290 | 8002fdc4 | spline_maketable |
| 8001d29c | 8002fdc4 | spline_maketable |
| 80028984 | 8002fdc4 | bmapLoad |
| 800289e0 | 8002fdc4 | bmapLoad |
| 800289ec | 8002fdc4 | bmapLoad |
| 80028a18 | 8002fdc4 | bmapLoad |
| 80028af0 | 8002fdc4 | bmapUnLoad |
| 80028b10 | 8002fdc4 | bmapUnLoad |
| 80028b30 | 8002fdc4 | bmapUnLoad |
| 80028b94 | 8002fdc4 | bmapUnLoad |
| 80028bc0 | 8002fdc4 | bmapUnLoad |
| 80028bdc | 8002fdc4 | bmapUnLoad |
| 80028db8 | 8002fdc4 | mapUnLoad |
| 80028dd8 | 8002fdc4 | mapUnLoad |
| 80028df8 | 8002fdc4 | mapUnLoad |
| 80028e6c | 8002fdc4 | mapUnLoad |
| 80028e98 | 8002fdc4 | mapUnLoad |
| 80028eb4 | 8002fdc4 | mapUnLoad |
| 8002a44c | 8002fdc4 | makeDisplayList |
| 8002eeb8 | 80030218 | DEMOInit |
| 8003005c | 8002fdc4 | N_battleMapFree |
| 80054740 | 8002fdc4 | evt_sub_get_coin |
| 80055664 | 8002fdc4 | evt_sub_spline_free |
| 80055670 | 8002fdc4 | evt_sub_spline_free |
| 8005567c | 8002fdc4 | evt_sub_spline_free |
| 800559cc | 8002fdc4 | evt_sub_spline_get_value |
| 800559d8 | 8002fdc4 | evt_sub_spline_get_value |
| 800559e4 | 8002fdc4 | evt_sub_spline_get_value |
| 80073700 | 80030048 | battle_exit |
| 80073914 | 80030078 | battle_init |
| 800f8920 | 8002fdc4 | BattleFree |
| 8017225c | 8002fdc4 | monosiriExit |
| 80176a10 | 8002fdc4 | winLogMain |
| 80176b04 | 8002fdc4 | winLogMain |
| 8017af88 | 8002fdc4 | evt_unitwin_free |
| 8019c7a0 | 8002fdc4 | envBlurOff |
| 801f7a8c | 8002fdc4 | evt_moving_floor_delete |
| 801f8870 | 8002fdc4 | evt_moving_floor_free |
| 801fbac4 | 8002fdc4 | johoya_data_free |
| 801ffe7c | 8002fdc4 | uranaisi_data_free |
| 80209b3c | 8002fdc4 | evt_win_coin_off |
| 80209fc0 | 8002fdc4 | evt_win_one_message |
| 8020a1b4 | 8002fdc4 | evt_win_item_desttable |

Totals:45 _mapFree sites,two memClear,one memInit and one each battle pool
allocate/free. The concrete unload indices and battle order are separately
established in companion contracts. evtDelete has NO _mapFree call and its
complete body does not request __memFree or SDK free. Event user-work sharing,
scene pool resets and script helper frees therefore need source-lifetime evidence
before native ownership decisions;“freed whenever event ends” is unsupported.

Acceptance:wrapper SDK handle lookup/no null gate;allocated head/middle/tail
unlink before insertion;new free head/middle/tail insertion,exact unsigned order,
next/previous/both/no adjacency,SDK size-add convention versus map+32,store/
read order and stale-header/invalid-free boundaries;all50 actual direct sites.
Heap/alias/cycle/double-free/address/handle/source/scene/REL/indirect domains and
original release/ownership/reset traces remain EVT-01 work. These local free
contracts do not establish automatic per-entry ownership or complete full-game
memory requirements.
