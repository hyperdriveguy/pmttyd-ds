# Shared original Bero normal-position and complete collision owner

Source: [geometry/party owners](SHARED_BERO_NORMAL_POSITION_GEOMETRY_AND_PARTY_OWNERS.md),
[callback/publication order](../overworld/BERO_INITIAL_POSITION_LOCAL_PUBLICATION_AND_ACTOR_WRITES.md)
and [previous-party prefix](../partner/NATIVE_PREVIOUS_PARTY_AND_SHARED_INITIAL_POSITION_PREFIX.md).

## Complete collision transaction ownership

CollisionTransition::load takes an explicit room identity alongside the selected
prefix. Successful walls/ground/returns/head loading certifies that room before
commit, so actual staged candidate queries are admitted. Failed or incomplete
loading certifies no room. The certification is swapped with geometry/indexes/
filters in CollisionSnapshot and restored on rollback, including nested staging.
clearCollision clears it. Successful individual geometry imports invalidate it;
failed imports which preserve old data also preserve its old certification.
Legacy prefix-only transactions deliberately retain unknown room identity.

collisionRoomReady is independent of doorCollisionRoom and collisionReady.
The former is an overlay/filter owner which restore can update; the latter
indicates walls alone. Neither proves the complete room geometry. Native
stageRoomCollision passes the real room selected by roomAsset. The room argument
is a trusted caller identity, not a path-content authenticator or new original
GameCube field. This native admission costs one retained uint32 and comparison.

## Reusable native geometry services

The optional requesting-room admission hook checks Game.progress.room and its
map owner against the actual requesting area, preventing a same-name lookup from
another area. It requires no collision for a literal zero-distance request.
Game::fieldBeroGeometry resolves the actual requested name through same-room
FieldMapFlags.hitSourceNode, then a unique RoadWarp source/sourceNode match.
The source static transformed-leaf center is scaled back to engine XYZ. Named
hit flag80 excludes lookup; disabled-query flag1 does not. A missing road match
holds. The cached fieldBeroPosition is never used as name identity. The converter
rejects nonleaf markers; general live hierarchy/dynamic transforms remain open.

The floor service admits only matching Game/map/certified collision room. It
reuses fieldShadowRay's existing raw static geometry query and spatial data,
without Mario support filtering, origin-height bias or Q1000 rounding. An actual
miss remains resolved and retains source centerY; unknown tagged hit owners hold.
Native sin/cos use double-library evaluation rounded to float; FMA uses std::fma.
Those are explicit adaptations, not proven Gekko SDK bit parity. The portable
helper now preserves sine/X arithmetic before cosine/Z arithmetic in each phase.

## Same attached child and mutable VM storage

FieldAreaEvent binds80079b74 through supplied geometry and position services.
It captures args/Mario, resolves mode and distance, captures LW6 pointer, reads
X, captures LW3 name pointer and LW5 direction, then reads Y/Z. Name bytes are
read through this shared VM only when X is100000. Non-sentinel zero-distance
needs no name dereference or geometry. Sentinel zero-distance can change S while
E remains original integer XYZ; no floor/trig/final-copy is invented.

Finite READF conversion of S is a deliberate native admission check before any
world publication. It does not quantize S/E/recovery or impose the same bound
on E. All six globals must have valid backing storage before any store. The VM
registers writable24-byte RAM at8041e9e4..fb, separate from immutable original
DOL/REL event words. It writes SX/SY/SZ/EX/EY/EZ in original order, obtains Mario
fresh for recovery X/Y+37/Z, reads motion from the originally captured Mario,
and obtains Mario fresh again only for the motion25 reload store. Missing
admitted geometry/writers holds before effects; actual providers must remain
valid throughout synchronous publication. Retry-before-publication is a native
resource admission boundary, not a claim that source callbacks yield mid-call.

Original following READF instructions quantize S into actual LW10..12; typed
Mario/party setters and camera reset consume their real fetched operands. Party
raw0 uses the distinct known current-index owner, any nonzero raw selector uses
literal slot1. Native admitParty refuses unknown/out-of-pool raw0 lookup before
the registry assertion; raw1 needs no current-global knowledge. Actual null
lookup completes without reading coordinates or requiring scalar/write services.
No separate initial-position VM or substitute parent bank is launched. The
source child returns through the existing scheduler's attached-event lifecycle.

These providers are an opt-in retained-area composition, not full live map
loading. Game's existing standalone native Bero arrival adapter remains separate;
full parent/mario_enter/sound/NPC/checkpoint owners and coordination with Trouble
remain unfinished. Backing Game/world must outlive pending events; retire before
replacement. Original assets/packs remain unchanged and private.

## Original next frontier after the child returns

For the explicit story402 fixture, original initial_pos and its Bero parent both
return. info_run8030ec7c starts independent mario_enter8030f2d8 at8030f050;
that script's first service is evt_seq_wait8013ddc0(2). Original gor02 parent
also starts independent dearu_event at REL4d614 because story>=268, then reaches
evt_npc_setup8005323c at4d630. The observed three live events and keyDepth1 are
specific to that fixture/continuation, not a universal Bero invariant. Actual
NPC registration, source init-event creation/polling and regular-event/resource
ownership remain required. Preplaced models alone cannot satisfy npc_setup.

The [retained NPC setup wrapper](NATIVE_RETAINED_NPC_SETUP_EVENT_SCHEDULING.md)
now supplies ordered metadata and actual init/regular root creation/polling for
admitted owners. Game still needs genuine actor registration,tribe/pose resources
and populated battle0/area/shared RNG services before binding that owner.
