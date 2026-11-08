# Native adoption of the actual source battle map pool

`BattleSequenceNestedPoolReceipt` attests the completed `N_battleMapAlloc`
`80030078` child reached by original `battle_init800738ac` phase 9. It does not
attest BattleInit completion, a displayed frame, initialized units, or camera
publication. The existing complete allocator remains the source writer:
`allocateEventBattleMapPool` stores length `0x64000` (400 KiB), allocates from
primary root `8041e808`, reloads the length, publishes nested root `8041e80c`,
and initializes the nested header. Original 32-byte-header allocation, clearing,
cache-line flush and barrier behavior are unchanged. Source ownership/order is
recorded in [the field initialization contract](NATIVE_FIELD_BATTLE_INITIALIZATION_AND_SHARED_STORAGE.md).

The binding reserves an exclusive native epoch immediately before this actual
child and completes its receipt only after the original void call returns.
Unavailable native metadata after that return never turns successful source
writes into a retry. Repeated held FieldInitialize calls cannot allocate again.
The completed receipt captures the actual primary/nested roots, storage owner,
actual VM, native Game facade, sequence and NPC owners, and memory/cache service
identities. Native Game/VM/binding registrations are checked before every
borrowed facade dereference. Mapping admission checks actual backing identity,
not equal bytes. Used primary extent/header checks supplement that ownership;
they are not an allocator generation inferred by polling.

`HarborSourceMapPool::adopt(vm, receipt)` accepts exactly this VM and claims the
epoch once. It derives services from the actual VM. It does not call the pool
constructor, create an arena, register aliases, clear bytes, flush, reset source
history, or manufacture an allocation. Its destructor removes only native
allocation metadata. Fresh actual nested allocations receive existing per-
allocation lifetime tokens; preexisting/externally produced births remain
unknown. A freed and reused payload receives a different token. The cold `bind`
factory remains an explicit native fixture/storage policy, separate from adoption.

The supported ownership policy is exclusive and single-threaded. The original
source End/free child `800300ac` remains unavailable. A native `retire()` removes
admission but neither frees the pool nor clears the process epoch; it cannot
permit a second producer. Binding primary allocations may continue, but its
public allocation route refuses the nested root. All nested allocation/free
operations in the adopted path belong to the uniquely claimed pool owner.
Arbitrary raw stores, direct original allocator/free calls bypassing these owners,
or uncoordinated remapping violate this contract. Equal payload addresses and
restored headers cannot detect a free/reuse ABA, and are not claimed to do so.
Before source End can be enabled it must retire every consumer, perform the real
source release, and implement an explicit new producer epoch transition.

`NativeFacadeRegistration` is C++ lifetime admission only. Game copies create
independent registrations; moves retire the source registration. Assignment
retires destination proofs. Self-assignment preserves them. Native restore
retires proofs before replacing state, even though selected process owners are
retained across restore. Destruction retires the last-declared registration
before members are destroyed. Copy/move/restore/destruction and all receipt
consumers must be quiescent; this is not a multithread synchronization protocol.
EventNativeRuntime's registration survives original event-manager Init/ReInit:
those source operations precede BattleInit and do not replace the native VM.

For a binding-owned animation attachment, the typed `AnimationPoolMemory::bind`
overload captures this intrinsic VM registration. Retained pose-pool guards
reject after VM destruction before the first memory access. Expired teardown
clears only that attachment's own source-guard metadata, marks it inactive and
never reads, writes or unregisters dead VM storage. Existing raw-address-space
callers keep their documented VM-outlives-attachment policy.

Focused fixtures execute real source Init and FieldInitialize through the pool
producer and stop at the actual missing animation child `80041c58`. Their parent
sequence position is explicitly TEST-owned, not normal encounter activation.
They cover no-write adoption, held/reentrant producer calls, wrong VM/duplicate
claims, actual allocation reuse, original event ReInit, Game copy/move/restore/
destruction, service/pouch/mapping replacement, VM-before-binding destruction,
and an attached animation's retained guard after VM death. Production scheduler,
actual camera producer receipt and normal Crump activation remain separate work.
