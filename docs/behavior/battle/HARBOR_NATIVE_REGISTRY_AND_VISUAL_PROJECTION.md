# Native setup2 registry and separate visual projection

2026-10-13 native ownership correction. Source authority is the verified
[UnitSetup/UnitEntry order](HARBOR_SOURCE_SEQUENCE_STARTUP.md) and
[original tutorial init/phase](../story/NATIVE_HARBOR_TUTORIAL_INIT_AND_PHASE.md).
Original fresh entry order is system0, Mario1, Crump2, then spawned NPC helper3.
The old native Crump0/Mario1/helper2 registry omitted the system owner entirely.

HarborTutorial now retains a separate systemActor plus three visual slots
Crump/Mario/helper. Actual registry IDs live in each actor; visualSlot and
actorById resolve them explicitly. The system has no visual slot. Renderer
message-speaker anchors, animation completion, phase roots, attack roots,
recorded requests, packed sampling and helper work0 all retain actual IDs.
No direct array index interprets speaker3 or Crump2 as a visual slot.

The existing native storage adapter reserves four0x1000 unit/part banks at
84100000/84200000 (8KiB more than the prior projection-only allocation).
System definition80416a80 resolves kind803ade74, type220, HP999/FP0, one part
803ade28 and init803adf38. Its part has null resource bank and pose table;
no replacement geometry, fake pose handle or GPU draw is created. Source
kind/membership/move-phase/position/home/HP and part fields use the existing
strict imported data path. This does not install the full UnitEntry allocator,
status-icon/gauge/display/pouch/equipment helpers or resource lifetimes.

The original system init executes unchanged: install wait80419f08,
attack80419f00 and damage80419ef8; set max moves0 with its signed current-count
clamp; start the actual wait root; delete/end. The native USER_FUNC request
owner is0, and the stored tracked event IDs belong to the same event VM as
Mario/Crump/helper. Fresh init roots are created in source system/Mario/Crump
order. The current intro bridge still waits its original bounded init/event
barriers and then invokes Crump phase; complete original startup/turn-manager
binding remains separate work.

The three8-byte system hook streams are beyond the existing bounded sdata
import. The converter adds exactly28 original bytes:24 for the streams and
the actual final label-scan lookahead word100 at80419f10. scanEventLabels
reads the following word even for END; no synthetic padding/end sentinel or
change to the recovered scanner is used. Source table/hook pins are checked
before extraction. Original bodies and compiled packs remain private/ignored.
[Read-only verifier](../../../tools/verify_harbor_tutorial_registry.py) checks
five exact original spans, kind/resources/part/init bindings and all hook words.

Tests prove source system hooks/count, actual four-owner registry, helper3 in
Crump work0, source init request order, const/mutable ID queries and absence of
a system visual slot. Existing original dialogue/feedback branches, physical
sampling/defense, attack/wait tracking, speaker attribution and animation
completion run with the corrected ownership. Passing these checks is not a
full original-execution trace, hardware benchmark or complete tutorial battle.

Next: bind full source UnitEntry/UnitSetup and actual startup resources/status/
audience/entry services, then original command/turn/attack/result/field owners.
Keep the unchanged source dialogue and button-accessible control paths.
