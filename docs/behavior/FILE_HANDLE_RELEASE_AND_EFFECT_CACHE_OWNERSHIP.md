# Complete file-handle reference release and effect-cache ownership

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete fileFree8018a260/44. Verify:
`python3 tools/verify_file_request_wrappers.py`;read
[formatted forwarding](FILE_FORMATTED_REQUEST_FORWARDING.md),
[effect-driver cache](EFFECT_DRIVER_UPDATE_AND_TEXTURE_CACHE.md) and
[pool/context teardown](EFFECT_POOL_INITIALIZATION_AND_CONTEXT_RELEASE.md).
This is a shared file-entry state operation,not a heap-free implementation.
No original load/release/collection/scene runtime trace is claimed.

## Exact admission gates in original order

Input is raw file-entry pointer F. Null F returns with no memory query. Otherwise
read WORD F+a0 (nonzero admission word). Exact0 returns WITHOUT reading state or
reference count. Only for a nonzero word,read U8(F0) and require EXACT1.
Any other byte returns without count mutation. This is not a flags-bit test,
generic nonzero entry predicate or shape validation of the pointed data.

No name/type/index/table/owner/context/scene query,callback or service call occurs
in the body. Pointer/type/null/count domain policy is not inferred beyond these
three gates. Its holder role is established separately by
[acquisition](FILE_ACQUISITION_AND_REFERENCE_REUSE.md);this leaf only tests it
for zero. It has no specified scalar success/last-owner boolean contract.

## Signed-halfword decrement,store/reload and state2 only on exact0

For admitted F,read SIGNED halfword count at F2. Subtract1 using word arithmetic,
store LOW16 back to F2,RELOAD S16(F2),then compare EXACT0. If zero,store BYTE
F0=2;otherwise preserve state byte1. The reread matters for observable aliases
and signed16 wrap. Do not cap at0,use an unsigned refcount model,snapshot only
a calculated result or set state2 for every nonpositive count.

| Old S16 count | Stored/reloaded count | State after admitted release |
| --- | --- | --- |
| 2 | 1 | 1 |
| 1 | 0 | 2 |
| 0 | -1 | 1 |
| -1 | -2 | 1 |
| -32768 | 32767 | 1 |

The field's reference-count role follows this mutation;the source does not
prove all live counts are positive or tell this function how many outstanding
native owners exist. A second release after the1→0/state2 transition skips at
the state gate. A bad live state1/count0 instead underflows and stays1. No
recovery/duplicate-owner/generation detection is supplied.

The only entry mutations are the count halfword and optional state byte0.
The word at+a0 and all other raw bytes are preserved. Most importantly,
there is NO heap free,DVDClose,resource destructor,payload clear,pointer reset,
cache-list removal,reference callback or memory deallocation in this function.
“fileFree” names this API;state2 is a numeric request/result in the shared file
manager,whose reclamation/reactivation/read policy remains a backend contract.
Do not assign freed-memory semantics without recovering that consumer.

## Effect-driver cache drops its handle after this state operation

On selector mismatch,the driver reads old cache handle M0+10. Nonnull calls
fileFree(oldHandle),then reloads current manager and stores its cache10=0.
It does not inspect this function's count/state/admission-word gate outcomes or determine
whether another owner remains. Dropping one effect-manager handle is therefore
separate from actual shared file payload lifetime. The old file entry can stay
state1 when count>1,and fileFree does not deallocate even when it writes state2.

Driver can then request a new formatted resource and store the returned handle.
Actual name matching,data sharing,count increment,pending I/O/collection and
selector/source policy belong to fileAsync/_fileAlloc and other file-manager
services. Cache tag advancement on allocation0 is unchanged by this leaf's
reference semantics. Native unique_ptr/free-on-cache-change would be a different
ownership policy unless the backend/source proves exclusivity and equivalent
collection behavior.

Likewise effInit's cache10=0 store does NOT call fileFree first. Repeated driver
initialization can abandon a handle under its local contract. Context auto-release
frees effect heap3 payloads,but supplies no file cache release. A unified effect
object destructor must not invent file release calls at those points. Full scene
ownership may supply separate operations;those source graphs remain open.

## Native resource binding and acceptance

A file-handle adapter needs raw entry addresses/state byte/admission word/signed
halfword count and their byte-level coherence,not just a parsed TPL pointer.
Original file API pointers remain32-bit addresses;host file descriptors,objects,
texture caches and backing data have their own lifetime mapping. Preserve shared
handle identity and operation order,while keeping asset data outside source
history and using the supplied original data as required by the project.

Check null F,zero admission word skips state/count,all state-byte values with exact1,
count2/1/0/negative/extreme halfword wrap,first count store then reread then
optional state2 byte store,no other bytes touched,no deallocator calls,repeat
release skip after state2,and driver handle-clear independent of shared entry
outcome. Providers may explicitly reject invalid source states,but must not
silently turn the function into nonpositive-count cleanup or guaranteed free.

Shared file-entry initialization/acquisition/increment/name/cache/async/read/
collection/scene owners,TPL/texture/model consumers,original platform/address/
format failure policy and runtime traces remain EVT-01/JUMP-01/DMG-02 requirements.
These local wrappers and release leaf are delivered;the full resource engine
and complete-game memory/rendering fidelity remain unproven.

[Complete acquisition](FILE_ACQUISITION_AND_REFERENCE_REUSE.md) now identifies
F+a0 as a payload-pointer HOLDER:archive F+4 or a smartAlloc header. fileFree
still only tests the holder address for zero;it does not inspect its word0
payload. Cache-hit acquisition can reactivate state2 without resetting its count,
or state3 after resetting count0. Full collection/async producers remain open.
