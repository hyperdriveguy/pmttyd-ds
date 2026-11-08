# Native shared source smart arena

SourceSmartArena owns original cold smartWork803ca410/e01c, its2048 stationary
headers and mutable first-allocation BSS8041e810. Initialized wp804151e0 and
five-heap size_table804151b8/28 are original editable inputs. Heap4 start/end/
handle, SDK descriptors and arena backing belong to the shared process bootstrap;
no arena bounds or initialized runtime image are imported. Its32KiB private stack
and32-byte guard are native frame policy, not an original measured stack budget.

The fixed native graph contains complete smartInit, smartAlloc, smartAutoFree,
smartFree, smartGarbage, smartTexObj and GXInitTexObjData bodies:586 original
words. Verification compares full callable boundaries and words against the
read-only US revision0 DOL. This is static source evidence and native execution,
not an original execution trace or a hardware memory-budget measurement.

Init executes genuine heap4 OSAllocFromHeap, both original payload clears and
first flush, full header clear/link loop and endpoint/control publication. The
native cold wrapper becomes ready only after whole Init returns; it does not
reinitialize a live arena when asked whether initialization has completed.
It preserves the source allocator's null/unavailable domains rather than inventing
a recovery or silently skipping the unconditional second clear. Its backend must
supply actual SDK/memory/cache effects over admitted coherent process storage.

Allocation, release, automatic release and compaction use original raw headers,
live manager reads, alignment and source retry order. First flag0 stores1 BEFORE
actual AutoFree(3). On a genuine cold empty arena both original traversals run;
there is no fake AutoFree receipt. The flag remains mutable, including the
original outer display reset; it is not the C++ readiness flag. Tag3 recycling
and tag4→3 marking retain their separate two-pass behavior. Release counters
require genuine draw synchronization before reuse; unavailable synchronization
holds the actual source call. Full-list pressure preserves its reserved header
and actual file collection/movement/thread boundaries; no synthetic completion
is supplied. The explicit source0 failure result does not roll back reservation.

Compaction keeps header identities stationary while payloads move. Unowned
payloads require actual overlap-safe memory movement and full-arena cache flush.
Owned/pending files require shared file pool/worker/type-relocation providers.
TextureObject reads the current header payload and executes original descriptor
word12 address insertion, preserving other bits. It does not import a GPU texture
or establish renderer residency. A null holder still returns the output object.

Calls retain the same raw request object/function/input bits, cursor, registers,
CTR and stack across child/budget holds. Mapping identity, wp and backend lifetime
must remain valid; teardown cannot remove a foreign replacement. Backend current
is pure, including inside SDK raw borrowers. Source shadow initialization may
borrow this exact same-VM owner: its actual smart call returns the real shared
header only after arena initialization. Shadow rechecks its own lifetime after
this child. Separate native-file-cache headers are not accepted as source smart
ownership.

Tests bootstrap the original five pool configurations through existing complete
initializeEventMemoryPools mechanics over a declared16MiB isolated backing
admission. This capacity is a fixture, not original absolute bounds or normal
Game provisioning. The shadow fixture uses actual heap0 and heap4, full cold
smart Init/Alloc/AutoFree, original ramp loops/GX constructors and coherent headers.
Host draw-sync/cache hooks are explicit platform boundaries without live GPU
readers. File pressure remains held. Normal Game startup, complete file/renderer
adoption, source reinitialization scheduling, actual GPU synchronization/cache
ownership, original observations and physical hardware budgets remain required.

See [initialization](SMART_ARENA_AND_HEADER_INITIALIZATION.md),
[allocation](SMART_ALLOCATION_HEADER_AND_GAP_PLACEMENT.md),
[automatic release](SMART_AUTOMATIC_TAG_RELEASE.md),
[compaction](SMART_HEAP_COMPACTION_AND_PENDING_FILES.md) and
[shadow initialization](../display/NATIVE_SOURCE_SHADOW_PROCESS_INITIALIZATION.md).
