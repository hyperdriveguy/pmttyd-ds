# Native cold animation process owner

AnimationStartupProcess owns one event VM, its actual SDK arena, shared source
file manager and final animation work/cache aliases. The caller retains this
process across scenes; cold() rejects an established or nonempty typed pose
pool. It is a startup prerequisite, not complete Game activation.

## Original memory and operations

The read-only US revision0 DOL supplies20 data/BSS ranges (including Entry
defaults/formats), packed privately as
animation-boot/memory.pmevt. Extraction checks every range SHA256 and gives real
initialized sections precedence over the broad overlapping BSS interval. In
particular Vivian's initial0xc00 bytes are initialized data, not zeros. Runtime
admission checks exact segment layout and critical constants, strings and zero
work, not every complete source hash. The extraction manifest remains the full
byte provenance authority. No executable code is imported.

The caller supplies aligned, nonwrapping native arena bounds and platform cache
flush/barrier services. These bounds replace pre-OS ArenaLo/ArenaHi sentinels;
12MiB in the test is a native policy, not a measured GameCube/3DS memory budget.
Actual initializeEventMemoryPools builds all five SDK pools and primary map.
Event contexts and animation arrays allocate from that SAME source heap using
actual eventHeapAllocate/SDK headers, lists, zero fill and cache services. No
bump allocator or fabricated heap return backs this owner.

advance() retains the recovered initializeAnimation cursor. Its five source
allocations establish real model/texture/pose arrays, tangent table and scratch.
Source fileAllocf loads real a/vivian.bin and a/ag2tg.bin through the shared
NativeFileCache. Original positive aligned memcpy_as4 copies Vivian0xc00 and
its16-byte tail in sequential word pairs; actual fileFree releases Vivian and
ag2tg retains its actual file/holder reference. The native tangent initializer remains an explicit floating-point adapter;
[its actual live table](NATIVE_LIVE_ANIMATION_TANGENTS.md) now backs CPU consumers.

Only these two cold literal-only formatting requests are admitted. Formatting
completes once before the retained _fileAlloc child; changing a literal while
Yield is pending does not reformat on resume. The manager exposes its actual
shared filename BSS at803e6e78 with0x400-byte backing to borrowing VMs. Generic
varargs/concurrent formatting and source thread scheduling remain unfinished.
The cache's existing narrower filename admission remains unchanged.

The copy adapter admits only the two recovered cold destinations/counts and
original payload extents, excluding native padding. It does not reinterpret the
source memcpy_as4 zero/small-count wrap loop as a no-op or support arbitrary
copies. Missing services hold the actual child; no success handle is fabricated.

## Ownership and completion

Only after the full source parent completes does AnimationPoolMemory attach the
typed pool to the actual VM bytes. Alias admission failure retains completed
startup and retries attachment without reallocating arrays or replaying file
refs/copies. Live work-pointer mismatches make ready()/advance() unavailable.
Aliases retire before VM storage, then shared pool/file owners. Caller must keep
platform callback state alive; source teardown/GC/cancellation is not implemented.

The fixture initializes field and battle event contexts before cold animation,
holds a real warm Vivian file at its pending Yield, changes formatting input,
resumes, holds alias admission, repairs it, and checks repeated advance plus SDK
allocate/free/reuse after alias splitting. All arrays, refs and heap headers are
actual production owner bytes. See [shared files](../file/NATIVE_SHARED_ANIMATION_FILE_CACHE.md)
and [work aliases](NATIVE_ANIMATION_POOL_MEMORY.md).

Normal Game still uses its existing startup/rendering path. [Original pose Entry and typed metadata adoption](NATIVE_SOURCE_ANIMATION_POSE_ENTRY.md)
now use this owner. CPU output consumers,
ordinary Mario/partner activation, complete renderer/field providers and process
teardown remain open. This owner alone is not a playable-prologue parity claim.
