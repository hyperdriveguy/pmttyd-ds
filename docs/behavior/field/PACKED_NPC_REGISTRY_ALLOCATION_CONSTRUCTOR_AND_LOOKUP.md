# Packed NPC registry allocation, constructor and lookup

US G8ME01 revision0. Source [registration/pose owners](ORIGINAL_MAP_NPC_REGISTRATION_AND_POSE_OWNERS.md)
pins complete npcInit/npcEntry/lookup bodies; [pose leaf services](ORIGINAL_FIELD_NPC_POSE_LEAF_SERVICES.md)
records required animation owners. This implements the packed mechanics behind
those functions, not a native Game actor/resource factory.

`field_npc_registry.hpp/.cpp` separates original memory/registry effects from
allocator, libc string and animation services. All primitive callbacks are
synchronous, infallible within admitted domains and owned by the caller. Native
resource/capacity/name admission must precede effects. A callback presence or a
catalog initialized boolean does not establish real pose/resource readiness.

`initializePackedNpcRegistry` uses inline registry8041e8b4, alternate registry+14,
auxiliary pointer8041e8b0 and final zero8041e8ac. Source ordering is preserved:
store ordinary capacity36; allocate36*340; freshly read capacity, publish array
and clear captured allocation using that count; freshly read capacity for
auxiliary allocation count*94; publish auxiliary without clearing it; store
alternate capacity16, allocate16*340, freshly read alternate capacity, publish
and clear; allocate580 field-battle work. Fresh GP receives that captured work
at+168, then another fresh GP/+168 supplies the actual memset address. Mode
halfword0 still goes to the captured allocation; final8041e8ac store0 follows.
Registry counts are not silently reset. A native owner must supply original
startup/lifecycle state rather than inferring extra writes from this routine.

`enterPackedNpc` selects ordinary/alternate once from fresh GP+14. It captures
signed capacity and array for the active-name duplicate scan, then freshly reads
the selected registry's array for the first-free scan while retaining capacity.
Duplicates neither reject nor reuse the matching actor. Free-slot admission
excludes the source overrun domain; a source-domain assertion precedes full340
clear. Install flags3/name, freshly select pose context, request original model,
store handle and reload it for the-2 fallback decision. Only-2 requests original
hoshi with a new GP context read. Paper, dimensions, Vivian and material calls
all load the current actor handle when reached.

Original zero/current/previous vector pair loads/stores, separate late Z reads,
unit-vector fields, captured float1/0 words, reversed timestamp stores and fresh
minus-one constant read are preserved. Descriptor+28, event IDs and associated
pointers begin as actual cleared bytes. Vivian and captured original c_luigi
model checks OR their source flags; fallback does not replace the captured model
argument for the latter check. Material flags2 precede fresh selected-registry
count increment. Return value is the allocated slot index. Tribe/map/AI/territory/
battle/initial animation setup belongs to the later real tagged-map producer.

`packedNpcNameToPointer` selects a fresh current pool, scans active bit1 only at
stride340, and compares exact source bytes. NoAssert returns0 when exhausted.
The inspected release npcNameToPtr body has no missing-name assertion: it returns
the exhausted scan pointer (array+capacity*340 for positive capacity, array for
nonpositive capacity). The native adapter must admit existing names before
using that unchecked form. This corrects any assumption that the conventional
asserting function name guarantees a null result or an executed debug assertion.

Tests cover allocation sizes/order, uncleared auxiliary, captured/fresh field
work divergence, mutable capacities, fullclear/default fields, duplicate creation,
active-only exact lookup, zero-capacity/release exhaustion, alternate pools,
post-compare array changes, stored-handle mutation, fallback selectors, fresh
pose consumers, late vector/constant loads, timestamp pair order and fresh count.
An excluded ARM fixture runs the same assertions with heap-owned fixture storage
and test allocator/pose effects. It does not manufacture Game actor/resource
success or prove original runtime/visual equivalence.

Native binding remains pending: original descriptors/constructor constants and
all required original model banks/sidecars, real field-owned pose allocation,
tagged-map setup/order, source reinitialization/deletion/readiness lifetimes,
and shared session RNG. Keep existing source NPC setup admission held until
those actual owners exist. Renderer integration and full original events/story/
areas and hardware performance remain separate unfinished requirements.
