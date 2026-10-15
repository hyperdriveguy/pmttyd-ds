# Actual raw window TPL and shared process memory

`NativeSourceWindowFile` loads the exact unchanged original US revision-0
`f/msgWindow.tpl`,321984 bytes. Its trusted-pack runtime FNV64 corruption guard
is `1dd2c1c8721b5ed4`; the read-only ISO exporter independently pins SHA256
`2ef0cdc818d2402de5bfb95394f67d78017e74bc1b41c38b79780cb0b870dc67`.
The immutable file is not an archive handle, DVD read completion, allocation,
source palette relocation, GX object constructor, or native window readiness.
See [the whole original producer](RETAINED_WINDOW_TEXTURE_SETUP.md),
[actual window packets](ORIGINAL_CAMERA8_WINDOW_PACKETS.md) and the analogous
[font memory owner](NATIVE_SOURCE_FONT_RESOURCE_OWNERSHIP.md).

## Process history and unknown bytes

The owner requires a real strong process lifetime and current callback. It
owns the routing range `8041ea18..8041ea27`: source palette pointer, completion,
scroll word, and object-array pointer. Its constructor creates no readable
bytes, source flags or captured history. A first actual VM binding captures
each existing byte from restorable writable RAM. An actually unmapped cell
becomes unknown. A readable semantic/external alias that cannot be extracted
and restored is rejected transactionally; an occupied unreadable cell is not
replaced. This is a native memory admission domain, not a recovered original
heap/address policy.

Known captured bytes become external aliases into stable process backing.
Unknown cells remain physically unmapped in the VM and unavailable from the
owner. Zero in a real captured byte is known; unobserved backing initialization
never makes an unknown byte readable. State stores are unavailable before
first capture. After capture, real admitted source stores can make unknown
cells known. They preflight every byte and all bound VM mappings before their
first data change; failed admission removes any newly registered aliases and
leaves source bytes/known bits unchanged. BE byte/half/word reads assemble only
known lanes. Typed multi-byte stores preserve one acknowledged whole store.

Later VMs alias the same process known/unknown history. Their own existing
bytes are retained only for restoration; a later VM's zeros or nonzero bytes
do not fill the process's unknown cells. Mappings contain known bytes only,
including isolated cells, and ordinary EventAddressSpace BE aliases see the
same actual source stores in every bound VM. No guest pointer is synthesized.

## Actual allocations and delivery

`adoptAllocation` requires the real heap child's explicit native address policy,
stable actual backing, full capacity and strong allocation owner. Guest wrap,
state overlap, overlapping guest/native allocations and occupied bound-VM
capacity are rejected. It reserves first-refusal routing ownership without
exposing any bytes. This method does not acknowledge heap source calls or
assert object/producer provenance; that proof remains with the real allocator.

`installRead` accepts only the actual bytes delivered by a genuine read owner,
once per still-unwritten allocation. It copies the actual readable prefix into
private mutable backing, mapping that prefix into every VM. Missing capacity
stays owned and unknown; no rounded tail is filled with zeros. Installation
preflights all reserved capacity before registration/copying. A later real
Unpack runs on that mutable copy; immutable file bytes remain unchanged.
The actual object-array allocation can start fully unknown and become known
only through real GX memset/stores. This does not adopt an object from matching
bytes or infer its source producer.

`owns(address,size)` reports any intersection, including unknown state/tail,
retired storage and a query whose valid prefix reaches the 32-bit address
ceiling. Callers must ask ownership first, then return the owner's optional
read/store result directly. Unknown, stale, unsupported-width or wrapping
owned reads must never fall through to another VM reader or zero substitute.
External owners may not place foreign mappings into reserved unknown capacity;
admission catches such conflicts before a read or newly known store.

## Lifetimes and teardown

Process and each VM binding pin their own actual strong lifetime and current
predicate. False current is sticky for that owner/lease. Process reads require
process current; a bound VM source caller additionally requires that binding's
current proof. A stale VM binding blocks further shared mutation/admission
until explicitly detached. Shared process history remains available across
legitimate VM retirement, independently of short camera epochs.

Callbacks are pure, non-destroying and may not change mappings, providers,
backing allocations or shared data. Reentrant reads through the same current
predicate are unavailable; callback attempts to mutate/retire an executing
owner/lease are refused. External backing must remain at the same native
address/capacity for its retained allocation lease. No source or native GPU
work is called by mapping/current operations.

Bindings **must retire before their borrowed EventAddressSpace is destroyed**,
outside raw borrows and callbacks. Binding retirement removes its external
aliases and restores its exact pre-bind known RAM bytes/gaps; process history
and actual allocations remain pinned by the process owner. `memory.retire()`
invalidates process current and detaches all still-live VM bindings. Destruction
is not a guest window reset, DVD close or heap free. A source/GPU publication
must separately retain its actual window producer and allocations and reject
retired process/native surface generations.

## Export and integration

`tools/source_window_resources_assets.py` exports exact original
`f/msgWindow.tpl` at ISO offset947396944 to ignored
`romfs/source-window/msgWindow.tpl` plus `source-window.json`. There is no
image conversion, pointer relocation, GX patch or completion store during
export. New imports require empty destinations. Exact metadata/payload sets
may be reused; changed data, metadata, truncated payloads and extra files are
preserved and rejected. Original Nintendo/Intelligent Systems rights remain
retained; tracking/export grants no new license or redistribution rights.
No raw original payload or ISO is committed.

The actual original setup is called from `seq_logoExit8008747c`, not menu entry.
Its genuine archive/DVD/formatter/heap/delivery and process scheduling owners
remain external. The synchronous archive path leaves source readiness0;
only genuine DVD completion writes1 after all original object construction.
This module provides neither that scheduling nor a ready flag from native atlas
residency. Native Window9 resources and CPU object bytes alone do not authorize
that source completion. Exact unapplied Makefile/ASSETS registration and native
routing notes are recorded with the frozen artifact manifest.

## Focused validation

Tests load the original TPL read-only, reject changed/truncated/extra-byte files,
and run complete original Unpack on a private admitted allocation with two
actual VM aliases. Source relocation is visible through both VMs while the
immutable original file remains relative and source readiness stays unknown.
Memory tests cover sparse first history, known zeros, later-VM unknown
preservation, new source stores, unknown rounded capacity, strong backing
retention, detach/restoration/rebind, full retirement, ownership overlap/wrap,
missing lifetime, sticky process/VM loss, reentry and raw-borrow admission
failure. Cross-VM store failure rolls back earlier alias registration with no
partial data stores. Python exporter tests pin whole original bytes and prove
existing edits survive rejection. Strict host/sanitizer/ARM and documentation
checks supplement these tests. Root owns combined build registration/gates.
There is no actual DVD delivery, whole Setup activation, native window GPU,
normal tutorial/battle or original PPC execution claim.
