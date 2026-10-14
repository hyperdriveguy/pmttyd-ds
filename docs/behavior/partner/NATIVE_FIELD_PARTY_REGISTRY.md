# Native field party registry and ending constructor prelude

2026-10-13 implementation binding of
[current-index selection](FIELD_PARTY_CURRENT_INDEX_SELECTION.md),
[constructor prelude](FIELD_PARTY_POSITION_CONSTRUCTORS.md),
[field control owners](../field/MARIO_OWNER_CONTROL_DEPTH_AND_BATTLE_PREPARE.md)
and [appearance2](FIELD_PARTNER_APPEAR_POSITION2.md).

Game owns two field actor slots independently of battle units and scene
billboards. Each allocated actor owns188hex bytes with explicit big-endian
word access. The native heap is a storage adapter, not original __memAlloc
ABI emulation. Allocation selects the first null slot, clears the source-sized
record and clears the looked-up record again. Occupied slots are preserved.
Simulation copies deep-copy these records; this is a host fixture policy,
not an original runtime clone service.

Fresh registry pointers are null, matching partyInit's eight-byte clear.
Mario current indices startff/ff from its explicit constructor stores.
Current selection sign-extends primary then auxiliary indices, requires a
nonnull pointer and rejects word0 mask8. It does not test word8's pending
mode bit. Native pointer lookup supports indices0/1 and negative/null;
original positive out-of-range indexing is outside this declared domain.

Field battle services use the live selection/pointers instead of token1.
Party controls ignore the passed actor and set/clear word0 mask20000000
in both registered nonnull actors, preserving other flags. Native field
Goombella drawing/following remains an independent projection; original
joining history and actor initialization still need binding to this registry.
A visible billboard does not imply a registered or unlocked source actor.

Ending1f obtains the canonical field Mario, clears word4 bit1 and invokes
the retained battle-party kind's ordinary entry route. For used kind1,
canonical pouch halfword14*kind lowbit is checked before destruction or
allocation. Clear executes the actual source-1 return without changing
pouch/slots, then queries the live current field party and clears its word4
bit1 if nonnull. This path stops at RuleKeep8018ef8c.

Set unlock with no existing primary actor admits Entry/EntryMain. The
source-sized actor is allocated/registered/zeroed. It reads the persistent
Mario camera ID and writes camera, negative pose/event IDs, primary flags,
slot, encoded native owner identity and kind in source order. It updates
saved primary kind only when zero, publishes the full used kind record and
propagates the Mario80000000 flag into party8000. Ordinary kind1 resolves
c_pkr or the original d_mario override, then stops at animPoseEntry8004176c
with mode2. No pose handle, material, init callback, appearance, current-index
publication or Hello is fabricated. See
[constructor animation admission](FIELD_PARTY_ENTRY_ANIMATION_ADMISSION.md).
Existing primary destruction remains partyKill800be69c; other kind/auxiliary
constructor domains remain separate providers. If no free slot exists, the
original outer Entry's null-dereference domain is not repaired with an actor
substitute; the native wrapper retains its constructor barrier.

Regression evidence: registry selection, primary inhibition/auxiliary
fallback, negative/null, independence from word8, global controls, slot
reuse and independent simulation copies. Actual native Game ending is tested
with unjoined storage and an explicitly joined checkpoint fixture. The latter
represents supplied authored history, not an implemented joining event.
It allocates and initializes one actor through the pose request but keeps Mario indicesff/ff until constructor
completion. Both retain story11, result0 and the control hold.

Remaining: authored Goombella joining graph/pouch mutation, complete ordinary
constructor and animation/material providers, distinct appearance2/Hello and
general postbattle placement, actual field actor rendering/mode integration,
RuleKeep/audience, original resource/context/sequence/result publication and
original execution/hardware verification. This is not a playable native return.
