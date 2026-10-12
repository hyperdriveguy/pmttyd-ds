# Smart resource provisioning requirements from recovered local contracts

US G8ME01 revision0; ID2,2026-10-12. This scoped requirements document combines
[smart initialization](SMART_ARENA_AND_HEADER_INITIALIZATION.md),
[allocation](SMART_ALLOCATION_HEADER_AND_GAP_PLACEMENT.md),
[retry/failure](SMART_ALLOCATION_RETRIES_AND_FAILURE_OWNERSHIP.md),
[compaction](SMART_HEAP_COMPACTION_AND_PENDING_FILES.md),
[release](SMART_HEADER_RELEASE_AND_SPACE_ACCOUNTING.md) and
[file pool](../file/FILE_CACHE_POOL_INITIALIZATION.md). It is not the final full-game
system requirements document;source workload/platform/runtime remain open.
Verify initializer facts with `python3 tools/verify_smart_initialization.py` and
use each linked contract's verifier for its other local behavior.

## Required address and storage domains

Provide a32-bit original-address space with coherent big-endian word/halfword/
byte access,manager pointer wp804151e0,smartWork initial identity803ca410,
first-allocation flag8041e810 and live heap-index4 start/end/handle words.
Separate native storage pointers from original addresses through explicit
providers. Shared aliases must remain observable;raw header/file/manager bytes
cannot be replaced solely by independent parsed native objects.

| Storage | Proven local quantity / requirement |
| --- | --- |
| Smart manager and headers | 0xe01c=57372 bytes of original manager address layout;2048 headers*28 bytes start at+4;tail controls extend through+e018 |
| Payload arena | OSAllocFromHeap(heapHandle[4],heapEnd[4]-heapStart[4]-32);actual bound values require bootstrap/source evidence |
| File entry pool | Separate heap0 allocation0x2c000=180224 bytes;1024 entries*176 bytes |
| Original reused resource bytes | Loaded/borrowed via archive/DVD providers;live holders and type-dependent address setup/clear required |

These are distinct local storage domains. Their sum is not total game RAM:
other heaps,pools,SDK metadata,archive backing data,renderer/audio/animations,
thread stacks,converted assets and hardware adaptation require separate budgets.
No native allocation of exactly this original manager size is mandated if an
adapter faithfully supplies all required original addresses and raw aliases.

## Required operations and failure boundaries

Supply SDK arena allocation,memset,draw synchronization,cache flush,overlapping
memmove,thread identity,DVD worker/request state and file address clear/setup.
Preserve operation ordering and explicit live reads in the linked contracts.
The supplied resource data are reused;do not replace story/assets or invent
textures/sounds while filling these service boundaries.

Keep headers stable as their payload pointers move. File entry.a0 points to a
holder,not directly to texture bytes;header00 is the current payload. Owner08,
size04,gap10,next14,previous18 and control bytes must share original address
coherence. Archive-backed file holders differ from smart headers. Pending file
movement can retarget DVD destination7c;worker capture/platform policy remains
an explicit unresolved backend,not an assumed synchronous copy.

Support2048 header and1024 file-entry capacities under the original stable pool
contracts. Release counter e018 increments/reset behavior is independent of
capacity. Failed smart allocation consumes its reserved header locally;invalid
available-head/retry-tail/null-init boundaries are not guaranteed safe misses.
Host adapters may report invalid provisioning explicitly,while retaining the
original behavior distinction in fidelity documentation.

## Integration acceptance and unresolved evidence

Validate complete initializer→allocation→file holder→move→release→recycle chains
against original source domains,including type/tag and pending ownership. Local
binary fingerprints establish the listed bodies,not successful native graph
execution. Authored fixtures must be labeled as such;hardware/performance and
original timing require independent traces.

Recover heap-index4 bootstrap/bound sizes,scene startup/reinit ownership,actual
asset pressure/type domains,type-specific relocation/TPL/animation readers,
DVD worker scheduling/capture/cancellation and SDK/cache/draw behavior. Reconcile
these providers with native battle/event/field context lifetimes and preserve
original story resources. Until those and other game systems are documented,
full-game requirements,complete loading/rendering and equivalence remain unproven.
