# Native tagged-map NPC registration

This documents the implemented native registration span and its admission
boundary. The exact original store/read/call evidence is separate in
[tagged-map registration order](TAGGED_MAP_NPC_REGISTRATION_STORE_ORDER.md).
Related owners are the [retained registry](NATIVE_RETAINED_NPC_REGISTRY_OWNER.md),
[pose resources](NATIVE_FIELD_POSE_RESOURCE_BRIDGE.md) and
[original resource export](ORIGINAL_MAP_NPC_RESOURCE_EXPORT.md).
No test results or complete area/bootstrap validation are asserted here.

## Original primitive and retained owner composition

registerPackedMapNpc implements the admitted setupDataBase80085334..8555c
span over explicit memory, constructor, lookup, string, area, battle and
animation services. It resolves the original tribe, invokes actual npcEntry,
then resolves the actor by its original name. Duplicate names can therefore
create a new slot while subsequent map stores modify the earlier named actor,
matching the source rather than a unique-name replacement policy.

It retains source actor/tribe identities and performs the original ordered
position/direction/dimension, AI, territory/range, sound, battle and pose
requests. Paired loads precede paired stores; later Z words, metadata,
descriptor fields and pose handles are freshly read at their source points.
Source area lookup runs before the battle sentinel test even for invalid
battle names. Main flag4 is set and query flag2 cleared; this span does not
publish later interaction readiness.

FieldNpcRegistryBinding::registerMapNpc composes this primitive with the
actual retained registry/field pose pool and scoped VM facade. Before Entry
effects it admits readable source tables/strings, stable external parsed
inputs, a free registry slot, required resources/pose capacity, the actual
post-Entry named target and pose requests, source constants, area lookup and
required battle dependencies. Input pointers may not alias mutable owner
allocations. Services are synchronous and infallible within this admitted
domain; unavailable prerequisites hold before constructor effects.

The actual battle index-1 branch uses the original internal battle-information
clear and ordered associated-actor copies. This applies both to BTLNO_INVALID
and to a real named number-table entry resolving to-1. Other numeric indices
require explicit complete external admission for their actual memory graph,
saved-switch/RNG services and effects. Knowing a number-table mapping alone
does not admit its battle configuration. Missing names retain the original
definition lookup result0 rather than becoming invented-1 sentinels.

## Parsed input container

FieldMapNpcAssets owns PMFMNPC1 input records. Their392-byte layout is a
native parsed-input adapter, not an original allocated source structure:

| Offset | Contents |
|---|---|
| 0 | Original source ordinal, uint32 |
| 4 | Position XYZ, three float32 words |
| 16 | Direction float32 word |
| 20 | Territory-base XYZ |
| 32 | Loiter XYZ |
| 44 | Homing XYZ |
| 56 | Search range/angle and homing range/angle, four float32 words |
| 72 | Name,64 bytes |
| 136 | Tribe,64 bytes |
| 200 | AI name,64 bytes |
| 264 | Territory name,64 bytes |
| 328 | Battle setup name,64 bytes |

The header contains eight-byte magic, record count and stride392. Container
scalars are little-endian. Loading validates the complete candidate file,
ordinal order and terminated strings, then converts the first72 bytes of
each record into big-endian guest words before publication. Failed loading
retains the previous successfully loaded asset state. An unloaded catalog
provides no input to acknowledge registration.

mapTo installs owned byte copies into the requested facade. input provides
guest addresses for each parsed field plus captured ordinal/area identity,
checking record bounds and32-bit address extent. These addresses are not
original stack addresses. The primitive consumes them using the same source
read ordering. Caller-owned parser/history admission decides which original
ordinal rows may actually execute.

Both the resource census and parsed-input exporter share ordered
parsed_records. They retain original tagged ordering and duplicate names;
resource banks may share by model identity without collapsing actor rows.
No dictionary keyed solely by actor name substitutes for source row order.

## Original data windows and export

The exporter reads original DOL npcTribe803316d0/6ba0,
npc_ai_type_table80338270/804 and territory table80416580/20, together
with their needed original strings and map-registration constants. String
windows are aligned by reading actual neighboring DOL bytes. Synthetic zero
padding is not used to repair original-address segment boundaries. Byte
agreement merging with existing original data retains alias identity and
produces aligned PMEVT-compatible spans.

Original m/AREA/s inputs retain source-path/hash provenance. Numeric fields
become float32 words and original strings remain Shift-JIS. Unsupported or
incomplete tagged domains fail explicitly; the exporter does not invent
missing fields, pose aliases or replacement actors. These exports remain
private original-derived artifacts outside source history.

## Remaining execution and lifetime boundaries

This API admits a selected parsed row, not the prior setupDataBase parser,
saved map-history skip producer or normal bootstrap flow. Source ordinals
must remain the original ordinals even when some rows are skipped. The
separate later npcEnt setup/init/regular-event phase is not acknowledged
by tagged registration. Initial below-map coordinates do not authorize
omitting an actor whose real init script later changes placement.

Rendering, collision and named event callbacks must ultimately consume the
same retained actor records. Current registration does not implement later
query-flag publication, NPC Main, paper-facing Main, GPU resource publication,
complete room reset/deletion or original independent-event cleanup. Bindings
must retire before their facade, and actor/resource storage must survive
remaining event references. Unsupported owners stay at their genuine pending
boundary; successful resource export or row registration is not completion of
the original area parent.
