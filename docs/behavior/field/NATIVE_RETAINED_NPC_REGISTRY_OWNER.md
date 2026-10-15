# Native retained NPC registry owner

FieldNpcRegistryOwner and FieldNpcRegistryBinding provide retained storage and
synchronous admission for the [original packed constructor](ORIGINAL_MAP_NPC_REGISTRATION_AND_POSE_OWNERS.md)
using the [field pose resource bridge](NATIVE_FIELD_POSE_RESOURCE_BRIDGE.md).
This documents the implemented ownership boundary without new test counts,
runtime-validation results or complete map initialization claims.

## Cold startup evidence and actual reset distinction

Read-only US G8ME01 revision0 DOL evidence: __start80005240 calls
__init_data8000542c at80005258. The BSS table descriptor800055b8 contains
{8041e580,00000ea4}; the clear interval is8041e580..8041f424, exclusive end.
It includes npcMainCount8041e8ac, auxiliary pointer release_wp8041e8b0 and
ordinary/alternate registry work8041e8b4..8041e8dc. Thus cold startup really
zeros both live actor counts and registry pointer/state storage.

npcInit8004bb70 allocates/publishes its ordinary36-slot, auxiliary, alternate
16-slot and field-battle buffers. It does not reset ordinary count work+0
or alternate count work+14. Cold zero is a startup BSS fact, not an inferred
write inside npcInit. The native owner separately represents those two steps.
Direct inspected DOL callers distinguish lifecycle operations:

- marioStInit calls npcInit at8006fdec.
- Map _unload calls npcReset(0) at80007b0c, then later
  animPoseAutoRelease8003b028 at80007b38.
- battle_exit calls npcReset(1) at80073750.
- battle_init calls npcReleaseFiledNpc8004ce58 at80073924.

npcReset8004bafc clears the selected actor array using its capacity, then
sets that selected count0. Selector0 additionally reads fresh GP+168 and
stores field-mode halfword0; both selectors clear npcMainCount. Reset does
not itself release every pose or allocate new registries. These are static
source call/body findings, not observations of native or original execution.

## Implemented storage and facade binding

FieldNpcRegistryOwner::cold creates a separate cold-BSS0x30-byte storage
span at8041e8ac and four stable native allocations. Their guest addresses
come from an explicit aligned native allocation-base policy, not recovered
original __memAlloc addresses or allocation history. Bounds/overlap admission
and original numeric offsets are separate concerns.

The owner contains its field-owned pose/resource pool. Allocated native
vectors have host initialization bytes; their values are not a claim about
source allocator contents. In particular npcInit never clears the auxiliary
buffer, so its native allocation contents cannot be described as original
auxiliary initialization. The actual source clears still run for actor and
field-battle buffers.

FieldNpcRegistryBinding maps externally backed spans into one EventAddressSpace
and retains the shared owner. Its destructor removes the mappings that binding
installed. Failed partial binding removes its successful partial mappings;
it does not overwrite existing overlapping mappings. Separate facades may bind
the same owner and alias the same retained actor bytes without copying them.

initialize runs actual npcInit once for a cold owner after pre-admitting the
GP destination and all owned allocations. Repeated initialization is refused.
Binding an already initialized owner to another facade does not repeat npcInit
or invent a new GP+168 publication. That facade's actual GP lifecycle must
be supplied by its owner.

Game::initializeOriginalFieldNpcs creates/binds/initializes a candidate owner
and publishes Game's shared owner only after success. A Game holding an owner
cannot use that API to reinitialize it. Shared pointers may retain the same
session state across Game copies and independent facades; this is not save
serialization, checkpoint persistence, independent clone state or rollback.
Any future candidate-state transaction must account for shared mutations.

## Constructor and lookup admission

Binding::enter requires initialized selected registry storage, supported
capacity/pool identity, valid bounded request and active-actor names, an actual
free slot, readable original constants/GP fields and a real installed resource
with pose/model/texture capacity. Request name/model strings cannot overlap
the selected slot about to be cleared. These checks bound the synchronous
source constructor domain before effects; they do not synthesize missing
resources, allocation failures or fallback success.

The admitted constructor then executes original duplicate scan/free-slot
selection, full actor clear, name copy, pose allocation and real pose leaves.
Lookup uses active1 and exact source name, independently of query readiness,
visibility and draw-only flag800. NoAssert preserves missing→0; Release
preserves the source exhausted scan pointer rather than inventing an assertion.
Consumers must not promote that unchecked result into a registered actor.
activeActors similarly reports active1, not completed map/Main/display state.

## Mandatory lifetime and remaining providers

A binding contains a reference to its memory facade. Retire referencing events
and destroy/unbind the binding before destroying that facade. Keep the backing
owner/storage alive for any other retained actor-context users. Do not manually
unregister or replace its external mappings behind a live binding; mapping
removal belongs to the binding. Raw source pointers held by independent events
must not become dangling merely because a Game wrapper is replaced.

Current code does not implement room reset, NPC deletion/group deletion,
tagged-map stores or the subsequent setup/init/regular event lifecycle.
Source npcDelete8004b708 detaches associated+330..33c pointers and list links
+324/+328, clears query2, conditionally releases signed nonnegative pose+104,
stores pose-1, clears active1, then decrements the registry selected by fresh
GP+14. It does not clear the whole actor or delete init/regular event IDs.
npcDeleteGroup8004b420 resolves optional group owner+32c and list head+324,
then invokes recursive _npcDeleteGroup8004b464; it is not merely deleting
one record. Native group/link/pose admission and actual event cleanup remain
their respective source owners.

The separate map/battle release paths and auto-release ordering must be bound
before acknowledging those larger lifecycles. Stable registered bytes and
resource-backed constructor leaves do not acknowledge source map registration,
query-ready publication, paper-facing Main, GPU upload/draw readiness, all
world callbacks or completion of the original area parent. Unsupported work
remains pending at its real provider boundary.
