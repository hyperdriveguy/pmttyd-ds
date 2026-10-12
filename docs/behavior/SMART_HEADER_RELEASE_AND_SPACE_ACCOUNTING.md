# Complete smart-header release and space accounting

US G8ME01 revision0; ID2,2026-10-12. Complete smartFree8002f878/154.
Verify `python3 tools/verify_smart_release.py`;read
[automatic release](SMART_AUTOMATIC_TAG_RELEASE.md),
[file collection](FILE_CACHE_COLLECTION_AND_PRELOAD_AGING.md) and
[file payload holder](FILE_ACQUISITION_AND_REFERENCE_REUSE.md).
Static reconstruction only;no original heap/compaction/runtime trace.

## Entry and known raw header/manager accesses

Input H is a raw header address,NOT its payload at WORD H00. The first operation
reads BYTE H0e with no null guard. Current manager pointer is WORD wp804151e0,
initially smartWork803ca410;SDA base8041cf20 is pinned separately. Manager
addresses below use offsets from its live base W,with32-bit arithmetic.

| Object | Offset / width | Local role |
| --- | --- | --- |
| H | 00 / word | Payload pointer,preserved by this function |
| H | 04 / word | Allocated-span size added to reusable-space accounting |
| H | 08 / word | Owner/backpointer,cleared on normal recycle |
| H | 0c / halfword | Cleared on normal recycle;full producer meaning separate |
| H | 0e / byte | Control tag;exact4 defers normal recycling |
| H | 10 / word | Adjacent reusable-space quantity |
| H | 14,18 / words | Next,previous header links |
| W | e004 / word | Leading reusable-space quantity |
| W | e008,e00c / words | Used-chain head,tail |
| W | e010,e014 / words | Available-header head,tail |
| W | e018 / word | Available-header count incremented after normal recycle |

These accesses establish a partial map,not the whole header stride/allocator
schema or total game RAM. No address/type/generation/chain membership validation
occurs here. Unknown raw bytes and pointer aliases remain relevant.

## Exact tag4 performs only a deferred tag transition

If U8(H0e)==4,store BYTE H0e=3 and return immediately. Do not change either
chain,count,size,gap,owner,payload or halfword0c. No allocator/platform/file/
relocation callback is invoked. This is a local request/transition,not proof
that the payload was reclaimed or that the header has left the used list.

Every other tag,including0/unknown values,takes normal recycling below. No
second tag check or generic active-state gate exists. A repeated release after
4→3 can therefore perform normal recycling;source admissibility is separate.
No success Boolean or scalar return value is specified by this body.

## Normal unlink preserves the literal live read order

Read P=H18. If P==0,reload W;read CURRENT H14 as N;store W.e008=N. Reload W
again/read its current used head;when nonnull,store that node's18=0. If P!=0,
read CURRENT H14 and store it to P14. P is retained for this first splice.

Then reread N=H14. If N==0,reload W;read CURRENT H18;store W.e00c=thatPrevious.
Reload W again/read its current used tail;when nonnull,store that node's14=0.
If N!=0,read CURRENT H18 and store it to N18.

Do not retain one initial previous/next pair for all these stores. Manager
reloads and changes caused by physical aliases can affect later reads,including
H's own link bytes. There are no calls inside this function,but the raw memory
operation sequence remains part of an address-coherent implementation.

## Add allocation plus trailing gap to predecessor or leading space

Reread P=H18;read size S=H04;read gap G=H10. Compute Q=S+G with word wrap.
If P!=0,read P10,add Q with word wrap,store P10. If P==0,reload W;read W.e004,
add Q with word wrap,store it back to that SAME manager's e004. No adjacent
payload move,zero-fill,gap validation,negative-size guard or heap free occurs.

These quantities describe local reusable-space accounting. Their placement,
allocation constraints and future coalescing/compaction behavior require
smartAlloc/smartGarbage evidence. Do not infer that a host malloc block of S
bytes is being destroyed at this point.

## Append header to available chain and clear selected metadata

Reload W/read available head e010. If0,store head=H,then store H18=0. Otherwise
read that SAME W's tail e014,store TAIL14=H;reload W/reread its tail e014;store
that word to H18. No null guard protects a tail under nonzero-head admission.

Then perform stores in this EXACT order:HALFWORD H0c=0,WORD H14=0,H04=0,H10=0,
H08=0. H00 payload pointer,H0e tag,H0f and other unclassified bytes are not
cleared. H18 now links the available predecessor. Reload W/store tail e014=H.
Reload W again/read count e018,add1 with32-bit wrap,store to SAME W.e018.
No count clamp,available-chain membership check or duplicate-release rejection.

A normal release recycles header/list/space state and preserves stale payload
identity;the backing arena is not freed by this function. A native unique_ptr
payload destructor would add behavior absent here unless a separately recovered
arena ownership contract proves an equivalent adaptation.

## File collector composition and acceptance

The file collector skips smartFree only when its holder==entry+4. Other holders
are passed directly as H. Collection marks file state0 BEFORE this call,then
recycles the file entry independently. If H tag4,only H tag3 is written;the file
entry can still return to its available chain while the smart header remains
used. No collection-side retry or completion wait is supplied for this outcome.
Smart-heap/tag/source owners must establish the permitted domain.

Check null's first-byte dereference,all tags,4→3 only,normal head/middle/tail/
sole-node unlink,live rereads/manager swaps/physical aliases,size+gap and count
wrap,available empty/nonempty append,stale payload/tag preservation,metadata
store order and repeated release. Test the file entry and smart header as two
distinct identities;do not replace the holder with its word0 payload.

Allocation/compaction/tag producers,arena provisioning,relocation/DVD worker/
source/lifecycle/TPL and original runtime remain EVT-01/JUMP-01/DMG-02 work.
These complete release operations do not prove a complete memory manager.

[Complete compaction](SMART_HEAP_COMPACTION_AND_PENDING_FILES.md) now supplies
leading/predecessor/tail gap recomputation and moving payload requests. Headers
retain identity;[file moves](FILE_PAYLOAD_MOVE_AND_ADDRESS_REQUESTS.md) update
live holders and retarget pending DVD destinations. Allocation/relocation/source/
platform execution remain separate.
