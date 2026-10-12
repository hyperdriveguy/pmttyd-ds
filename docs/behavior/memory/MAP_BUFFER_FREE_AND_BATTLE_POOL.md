# Complete map buffer free/coalescing and battle subpool requests

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete _mapFree8002fdc4/9c,N_battleMapAlloc80030078/4c and
N_battleMapFree80030048/30. Verify:
`python3 tools/verify_event_heap_release_and_bootstrap.py` pins twenty complete/
linked bodies,205 anchors,two bootstrap/config spans and50 direct DOL sites.
Read [map allocation](../event/EVENT_USER_BUFFER_AND_MAP_ALLOCATION.md),
[pool bootstrap/reset](MEMORY_POOL_BOOTSTRAP_AND_CLEAR.md),
[SDK free](SDK_HEAP_RELEASE_AND_CALLER_CENSUS.md) and
[event deletion](../event/EVENT_DELETE_AND_COPYBACK.md).
Static original DOL evidence;no original heap/free/ownership execution trace.

## _mapFree(root,payload) full algorithm

If payload word==0,return BEFORE reading root/header. Otherwise H=payload-32,
read U16(H+8). If used==0,return before root traversal. No raw pointer/root/null/
pool-membership/alignment/size validation. Every nonzero used value enters the
release path;no specific used==1 requirement. No scalar return contract.

Capture next=word(H+0),size=word(H+4). If next!=0 and U16(next+8)==0,merge ONE
forward free node:read its size,replace next with its next pointer,add that size
and32 to retained size with word wrap. No explicit physical-adjacency check;
valid map-list layout must provide that invariant. No repeated merge of a chain
of consecutive free nodes;one immediate successor is inspected.

Then search for a predecessor from SUPPLIED root R:

1. Read R.next. If it equals H,read R.used16. If nonzero,finish with H unchanged.
   If zero,read R.size,set H=R,add R.size+32 to retained size,and finish.
2. If R.next!=H,unsigned-compare R against H. If R>H,finish. Otherwise if
   R.next==0,finish;else set R=R.next and repeat.

On every finish write selected H.next=retained next,H.size=retained size,
H.used16=0,in that order. No payload clear/cache flush,heap deallocation,event
counter adjustment,parent copy-back or global root replacement. It does not
validate that the supplied root actually led to the released header. A malformed
list/cycle/stale pointer can therefore follow a different path rather than a
recovered error response. Address comparisons and sizes are unsigned/word as
shown,not host pointer ordering between unrelated allocations.

For a valid address-ordered physical list,this restores a free node and merges
an unused successor and/or predecessor,including mandatory zero-size remainder
headers produced by _mapAlloc. The map size convention excludes the selected
header itself;merging adds32 for the removed header. Removed headers are not
cleared. If predecessor absorbs H,the ORIGINAL H.used may remain nonzero;
releasing the stale original payload again is not universally made harmless by
the initial used check. Do not claim generic idempotence or repair of invalid
ownership/double-free from the early return alone.

## Battle pool allocation is an explicit primary-map reservation

N_battleMapAlloc ignores any incoming arguments. Store requested length
0x64000=409600 bytes (400KiB) to8041e814=r13+18f4. Load primary map root
8041e808=r13+18e8;call _mapAlloc(primaryRoot,0x64000). AFTER return reload length
from8041e814;store returned payload pointer to8041e80c=r13+18ec. At that pointer
write next0,size=length-32,used16=0,in order. It turns the allocated payload into
a NEW map-list root whose first free size is0x63fe0.

No allocation failure test:returned0 is published and header writes at0 still
follow. No old battle-pool free before replacing the pointer,no context test,
additional zero/cache call or returned ownership token. Primary allocator already
zeroes/flushes the successful payload before these new root-header stores.
The nested pool allocation consumes a primary-map allocation;inner frees must
use the battle root,while releasing the entire reservation uses the primary root.

N_battleMapFree loads primary root8041e808 and battle payload/root8041e80c,
requests _mapFree(primaryRoot,battlePayload),THEN stores battle pointer0.
It does not reset8041e814 length,iterate inner allocations,clear payload or call
SDK free. Null battle pointer is harmless through _mapFree's null path,and
wrapper still writes0. It discards the pointer even if _mapFree returns early
because its header used value is already0.

Opcode76 user work allocation ALWAYS reads primary root8041e808,including in
battle context;it does not automatically choose8041e80c. Descendant user-pointer
sharing and event bulk deletion/reset are separate services. A per-entry native
ownership/free policy cannot be inferred from existence of the battle subpool.

## Scoped scene bindings and requirements

Direct DOL sites:battle_init80073914 requests N_battleMapAlloc BEFORE its
explicit GP14=1/event ReInit site73930/34. battle_exit80073700 requests
N_battleMapFree BEFORE event ReInit7377c and later explicit GP14=0. These bind
local order only;complete battle teardown,inner resource lifetime,other context
writers/aliases and REL/indirect callers remain separate contracts. The complete
50-site census is in the SDK companion;it includes45 _mapFree sites and no
implied per-event destructor policy.

Acceptance:null payload/free used0 before root read,nonzero used handling,one
forward merge,no successor merge,previous free/used/no-match/address stop,
merged header32 accounting/zero remainder/stale original header;400KiB primary
reservation and nested root writes,failure path,primary-versus-inner free,
clear-pointer-after-request and preserved length;actual battle order,opcode76
primary-root identity. Pool/alias/size/cycle/free-owner/source/scene/platform and
original release/cancellation traces remain EVT-01 requirements.
