# Native original ordinary NPC talk selection

2026-10-13; scoped US early Rogueport binding of
[ordinary candidate/lifecycle](FIELD_ORDINARY_NPC_TALK_CANDIDATE.md) and
[null ray/display getter](FIELD_NPC_TALK_RAY_AND_DISPLAY_DIRECTION.md).
The current native simple source-order Mouser/Darkly talk rules retain unchanged US
message IDs, text, control tokens and story branches; no story write is added.
This is not the complete original encounter/NPC/event dispatcher.

## Portable service and area binding

NpcTalkPlayer contains source-unit root/current dimensions, source motion,
view/main flags, key depth, party type/flags, background/control mode, retained
Mario display turn, rounded camera snapshot and separate live viewYaw.
Its talkable query preserves recovered motion0..2 and source flag/mode predicates.
DisplayDirection uses ordered (displayTurn-90)+snapshot and repeated wrapping;
it does not substitute Mario moveHeading, mirrored draw yaw or camera yaw alone.

NpcTalkScan considers each source registry token without allocating. Admission
preserves flags2/not8/non-null talkEvent, inclusive height intervals, radius
FMA(.5,MarioWidth+NPCWidth,10), center/+halfwidth/-halfwidth camera-relative
halfplanes and asymmetric180 boundary. It retains first equal-distance candidate
by strict distance reduction from1000. Separate nearActor follows source r25
publication before complete admission; it is not another nearest reduction.
Missing ray service cannot prove a clear ray. Portable norm/trig and native
triangle math remain explicitly host/ARM numerical adapters to original SDK/
Gekko operations, not claimed instruction or runtime equality.

The native current ordinary domain supplies original initialized Mario20/37,
current root and motion, bound display/snapshot fields and source-table angleABf
of camera eye→target. Native Mario display/main flags feed the talkable predicate;
key-off skips the outer scan. Nested key depth and other unmapped control modes
remain separate owners. Current PMNPC006 catalog entries retain original Entry3 plus setup/constructor
flags; live scripted source masks overlay only their owned bits, catalog visibility maps their known original scene creation/deletion.
Unbound dynamic flag producers, transformed dimensions/modes, alternate NPC pools and encounter
providers are not invented. Legacy rows without known source flags do not become
NPC candidates. Future areas use the same catalog/services and must bind their
own source registry order, flags, lifetime and talk events.

The catalog and original91-float angle table stage together; failure retains
previous resources/generation. Native null-callback map rays reuse resident
source-winding head triangles and spatial indices, active-door admission,
native-unit conversion and dynamic object geometry. They bypass ordinary Mario
wall attributes and the native ordinarySolid filter, as source talk requests.
A tagged polygon rejected by the wall callback can still block talking. Generic
original backend traversal/region/cull numerics and missing source geometry
remain external; legacy wall extrusion is a host fixture adapter.

## Mario/NPC order and scoped event lifecycle

The NPC owner runs after native Mario/object updates. The previously published
candidate, while alive/talkable, blocks the next Mario A jump without rescanning
current geometry/facing. Native prompts also consume that retained result;
rendering does not rerun map rays. The later NPC scan publishes the new candidate.
Entering range with A and no retained candidate therefore follows Mario jump;
turning away with a retained candidate can suppress that one jump even though
new selection fails. Room/story deletion or catalog generation change invalidates
native identity. Full case/door/pipe/encounter priority and exact pre-display/
retrace/event scheduling remain adapters beyond this local owner relationship.

A on the new chosen candidate records selection; the following NPC invocation
starts the scoped original simple talk event. Talk turns actor yaw toward Mario
and renders that yaw only while its original message event is active. Native
scene/control admission locks movement/actions/saves through closure. The wait/
return owner clears candidate without rescanning in that same invocation; the
catalog's original saved yaw is again displayed. Source regular-event stop/
resume, party-switch lock service, fbat work ABI, auto-turn flags and forced
same-name scripted pose restart are not fully bound by this simple-message event
adapter. Complex original talk graphs require their original event providers,
not fabricated dialogue or a fixed-duration substitute.

## Validation and next work

Host vectors cover dimensions/radius equality, vertical endpoint equality,
flags/modes/key depth, center/width-edge/180 facing, live versus rounded camera
inputs, first ties, separate nearActor and blocked/missing rays. Native original
catalog/message tests cover pending entry, unchanged story/text, turn-to-Mario,
closure, old out-of-range fixture jumping and retained-candidate arbitration.
A native attributes4 wall fixture proves wall rejection versus talk obstruction.
Isolated emulator captures and exact build/test results live in
[progress](../../PROGRESS.md); no hardware-performance claim is made.

[Native NPC side response](NATIVE_NPC_SIDE_COLLISION.md) now precedes this scan
and supplies current contact/root/flags with source rollback and wall retry.
Next: full NPC timers/movement and complete registry/auto-turn/
regular-event lifetime, original encounter and map-object/A composition,
additional original per-area events/NPCs/objects and hardware profiling.

[PMNPC007 per-actor rules](NATIVE_DARKLY_AND_PER_ACTOR_TALK_RULES.md) preserve
full original IDs and source comparison order. Darkly binds only the original
story<56 arm; no-match/empty bound lists return empty, so later name/flag
branches receive no fabricated fallback. Original arbitration/presentation/
side-response services remain shared with Chu.
