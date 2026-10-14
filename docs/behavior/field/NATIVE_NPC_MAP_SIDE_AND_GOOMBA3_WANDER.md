# Native NPC map-side movement and Goomba3 wandering

The [source map-side contract](FIELD_NPC_MAP_SIDE_MOVEMENT_AND_GOOMBA3_ZERO_VERTICAL.md)
now has a portable provider-based body and native original-map adapter.
Nonzero movement performs original heading/sine-negative-cosine direction,
length+.75width extent, ordered lower/middle/upper rays and exact fused half
slide. Middle hits below the45degree threshold do not stop the third probe.
Hit distance never replaces this slide. Source npcMain's first contact is
retained even when the second tentative-delta pass hits a different polygon
or misses. Zero displacement leaves currentXZ unchanged without helper/ray
calls. Unsupported reflection/attribute-filter domains remain native admission
frontiers, not source miss results.

Native rays use the existing original culled geometry and map-object cache,
with no optional Mario attribute filter. Helpers use original angle lookup
and float fused arithmetic, with native standard-library sine/cosine/norm
adapters; complete original SDK/Gekko numeric parity is not claimed. Geometry
scaling, original traversal/owner lifetime and detailed ray math remain shared
backend limits. This is not a replacement for original polygon registries.

The catalog now enables Goomba3's original radius100,speed30 shared wander.
Source Entry/setup flags80603 initialize to00480603; no40000000 bypass is
invented. Movement publishes the first wall contact for the next wander call,
consumes components only at positive dt, then retains the original zero-vertical
npcMoveG deduction. No ground snap, gravity or Y/contact300 write is added.
Source Mario-side response follows map movement. Automatic idle/walk/run pose
uses the actual post-response displacement. Existing source talk pause/resume,
original footstep names and shared simulation-tick RNG/frame budgets apply.
Global scheduler/RNG/SDK parity and later vertical writers remain open.

Portable tests cover zero/miss, all three probes, capped lower height, middle
strict boundary, rejected middle then miss, ignored hit distance, source half
slide, second hit/miss/zero, first contact retention and domain admission.
Original Game tests use actual plaza geometry to observe map contact, walk
pose and movement without Y writes, then verify ordinary/waiting talk and
source control ownership. The earlier initial-pose-only binding in
[NATIVE_GOOMBA3_TALK_AND_TROUBLE_SERVICES](NATIVE_GOOMBA3_TALK_AND_TROUBLE_SERVICES.md)
describes the prior implementation; this binding replaces its motion frontier.
