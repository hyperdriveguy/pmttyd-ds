# Map NPC inputs, history and area lifetime

This guide follows precompiled parsed map tags into the packed NPC owner and
retained area VM. [NPC/object owners](npc-objects.md) covers Entry/setup and CPU
poses; [field entry](field-entry.md) covers the individual position/control leaves.
Source evidence remains in [behavior contracts](../behavior/README.md). This is
an implementation walkthrough, not proof of full original room loading/unloading.

## Parsed row bytes and source registration

PMFMNPC1 is a private little endian container: a 16-byte header and fixed
392-byte records. Each record contains 72 numeric bytes followed by five 64-byte
terminated strings. Loading changes only numeric words into guest big endian;
it preserves original string bytes. Ordinals must match record order. An empty
successfully loaded stream is valid, while an unloaded object cannot map itself
as an acknowledged empty stream. `PackedMapNpcInput` fields point into these
mapped bytes; they are not already decoded coordinates or native pointers.

Registration scans original tribe/AI/territory definitions, enters an NPC, then
performs a fresh named lookup. Duplicate names can therefore populate an earlier
actor while retaining the newly entered actor too. Native admission predicts
that target and checks its existing pose/model separately from the new model.
This matters when the original default animation applies to an older duplicate.

Inline offsets identify the tribe table (0x54-byte rows), AI table (0x24-byte
rows) and name/value definitions (8-byte rows). The original row populates
position, yaw, tribe dimensions, AI flags/scripts, territory/range vectors and
sound/movement fields before battle and pose requests. Vector pair loads and
late Z reads preserve callback-visible order. Unknown fields retain offsets.
The area lookup still occurs for `BTLNO_INVALID`; that literal selects -1.
A missing ordinary definition instead returns zero, which still requests real
battle setup. Neither case may be silently substituted for the other.

## History preparation and one decision per ordinal

The retained field work F stores history at +0x4c: 64 records of 20 bytes,
16 bytes of map name followed by a skip word. `_map_attr_data` records contain
name/group/flags. Lookup returns its actual terminator on a miss. Empty names
use the actual original empty literal, not a fabricated guest address.

Preparation selects an existing or empty slot and promotes it to the head.
It captures five words, shifts previous records downward in descending order,
and restores the captured head. Paired loads precede both stores; final word
reads remain late. A memory provider can observe this ordering. Full history
with no match is outside admitted native scope; no eviction or tail record is
invented. Preflight proves complete field/string/table availability and rejects
overlap before source history stores.

Definition flag 1 enables grouped history. A group change clears all 0x500 bytes
then copies the map name. Within one group, flag 2 clear plus flag 4 set clears
F+0x74 for 0x4d8 bytes, retaining the first two slots. The current/previous map
definitions are re-resolved after promotion. These are source branches, not a
native cache replacement policy.

The ordinal mask is unusual: narrow ordinal to U8, take low six bits, shift a
32-bit one only for shifts below 32. Shifts 32..63 yield zero. Do not replace
this with modulo-32 bitset logic. Disabled history returns known false without
reading its skip word; unavailable enabled storage returns unknown.

`FieldMapNpcBootstrap` owns the mapped parsed bytes and copies of the tags. Its
binding and facade must outlive it. Each row captures its skip decision once,
including an engaged optional false. Resource/battle admission may hold that
row, but later history changes cannot change its already captured decision.
Only success or an actual skip advances ordinal. Successful earlier rows never
replay. Retirement unregisters only the parsed mapping; it is not NPC teardown.

## Stable owner and per-facade aliases

`FieldNpcRegistryOwner` creates four stable buffers for ordinary actors, release
work, alternate actors and F, plus the original registry BSS view. Native guest
base admission requires alignment, a non-low address and no 32-bit overflow.
These identities are not original allocator results. Zero-initialized host
vectors do not imply original `npcInit` cleared every buffer.

Each binding journals its external aliases. Partial binding failure unregisters
only its successful mappings. Independent facades can alias the same retained
actor bytes; destroying a binding does not clear the owner, pose resources or
initialized state. The synchronous original allocator consumes the four expected
buffers in source order. Admission establishes all resources before effects;
assertions enforce that contract rather than supplying fallback actors.

Fresh GP selects the current registry. Reads verify the exact owned array and
bounded capacity before walking actors. `activeActors` proves active-bit/name
readability only, not full Main/talk/collision/display readiness. Entry checks
free capacity, nonaliasing input strings, original constants, live GP timestamp
and CPU pose capacity. Parsed-map registration additionally admits source tables,
existing target pointers/pose, area data and actual numeric/shared-RNG providers.
A real table terminator is distinct from unreadable/unterminated memory.

Reset takes a raw selector independently of GP context: zero ordinary, any
nonzero alternate. It captures array/capacity, clears selected actor bytes, clears
that bank's count, then ordinary F mode through a fresh GP lookup, and global
NPC Main count last. It preserves the other bank, release work, F history,
poses and events. Native binding requires exact source capacities and owned
addresses before this operation. Reset is neither cold Init nor complete unload.

## Retained area VM and root ordering

The area program combines private door/root/battle catalogs with unchanged
original scripts/data. Loader notes explain header signatures, native bounds,
original 33-area registry and 0x44-byte battle rows. Registry setup/number pointers
start as original zero words and are written by actual area registration.
The VM owns native GP/allocator RAM; the captured program closure remains stable.
Replacing a catalog cannot substitute a different REL closure during room reuse.

`prepareMapNpcs` captures the supplied GP snapshot once. Existing actors require
actual room cleanup; the adapter does not clear them or overwrite F identity to
make admission pass. Failed preparation retains its snapshot/binding and blocks
root creation; corrected inputs require a fresh facade. Bootstrap completion and
an identical caller snapshot admit the original area root. Reapplying that
snapshot after `npcInit` would erase its actual GP+0x168 publication, so root
creation deliberately avoids it. Root ID goes into GP+0x164.

Supplied-bank door roots are a separate explicit continuation adapter. They
cannot bypass prepared map/area-root ordering. Their LW0 is the original immutable
door record pointer. Root retirement captures the local bank before deletion;
RETURN opcode 2 is distinguished from other retirement. Missing captured return
state is failure, not fabricated completion. Callback identities and source
operand/write ordering are listed beside dispatch. Unknown providers hold;
external results must follow 0/1/2 USER_FUNC status semantics.

## Ordered room retirement

Retirement excludes an active manager pass, callback reentry, battle context
and live unselected battle events. After admission it performs actual ordinary
NPC reset once, then asks external providers in original order: ext reset,
map-object reset, offscreen reset, image auto-release, effect auto-release,
pose auto-release. Provider identity and completed ordinal survive holds. A
false step means pending; it cannot acknowledge an unavailable release service.
The caller owns preceding Mario/map teardown and remaining unload operations.

After all six requests, actual event/case ReInit runs. It clears entries without
event-delete retirement hooks, child copyback or synthetic completed roots.
Native cursors/mappings/diagnostics are retired, while the VM, allocator, context
banks and next event ID survive. `begin` can select another supported room only
after this retirement and with the same program closure. The source external
release providers, whole room pipeline, shared session RNG and normal-game
activation remain separate implementation requirements.
