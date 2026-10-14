# Native named map flags and house05 initialization

This implements the recovered house05 prefix through file4d9ac, after the
[actual copied record and GW11 selection](NATIVE_AREA_DOOR_DATA_AND_GW11.md).
The [original installer](HOUSE05_ORIGINAL_INSTALLER_GW11_AND_NPC_LIST_CONSUMERS.md)
requests selector3, then conditionally hides its interior map object and disables
its interior collision subtree. Case registrations, which follow, are separate
unfinished work. [Incoming local-bank recovery](HOUSE05_INCOMING_LOCALS_AND_INSTALLER_FRONTIER.md)
explains why a fabricated zero16-word bank cannot stand in for their snapshots.

## Named geometry and native overlay

The existing read-only original map conversion now emits PMFLG001 alongside its
unchanged vertex/triangle streams. It retains original names, parent indices,
source-relative node identities, collision membership and a16-bit owner index
per converted vertex. The checked loader validates parent order, unique source
identities, finite counts and triangle-consistent ownership. PMFLDS01 associates
stable native room IDs with filenames; the five existing outdoor maps are
included, and additional areas can extend that descriptor.

`FieldMapFlags` retains a native event overlay, separate from immutable geometry.
It applies requested map bits recursively to the original named subtree; only
bit1 currently feeds native visibility. It separately enables/disables the
original named hit subtree. Missing or ambiguous names hold a request. These
are semantic owners, not original MapObj/HitObj memory layouts, full flag-service
ABIs, transform propagation, lighting flags or original map-render traversal.
Other existing story/interior/cutaway adapters still apply their own rules.

## Actual house05 requests

After NPC initialization and the copied-record/list prefix, the Game-owned door
owner reads record+20/+24 through its immutable original data bank. It executes
`mapobj_flag_onoff(1,1,S_ie_05_naka,1)` and the recovered subtree effect of
`hitobj_onoff(A_ie_05C,1,0)`. Names come from the original record, not a material
substring or a new interior mask. It preserves sequential completion: a held
second request does not repeat the completed first request. Successful init is
not replayed each tick, so later actual event changes survive.

Area, native actor-catalog and map-resource generation changes invalidate the
owned overlay/init publication; the exact original resource lifetime remains a
broader field-owner task. Synchronization precedes held-collision early returns.
The NPC/init tail publishes the changed hit filter immediately for later native
queries. Existing process-global room collision receives a value copy of disabled
node identities; it never borrows a pointer into a temporary Game. Ground, wall,
head and support paths share the same additional hit filter.

## Rendering and performance boundaries

Ordinary field drawing intersects its existing material batches with cached
visible vertex ranges before Citro3D submission. It preserves texture/cull/depth
state, authored vertex order and triangle boundaries. Opaque and translucent
passes share this state. Battle stage maps retain their independent owner.
Visible ranges rebuild only after bit1 changes; range lookup uses binary search.
A templated visitor avoids allocating std::function closures in field draws.
No per-frame vertex rewrite, texture upload or new art is required.

The initial S_ie_05_naka subtree contains79 original nodes and7416 of100332
Rogueport east vertices. The original source-group initialization previously had
no native binding for this subtree. The new state suppresses these interior
vertices and23 collision identities, rather than constructing a provisional
house05 interaction or assigning mask8. Actual emulator results and test logs
are recorded in [progress](../../PROGRESS.md); hardware performance and original
raster parity remain unverified.

Host integration checks the actual Game init, nonreplay after subsequent flag
changes, room holds/reentry and actual ground-query rejection/recovery of the
original house05 elevated box surface. Asset tests compare source collision names/hierarchy
and triangle ownership. This does not complete the original open/close scripts,
case9 saved locals, key49 lock, Bero callbacks, material1800 lists or their audio.
