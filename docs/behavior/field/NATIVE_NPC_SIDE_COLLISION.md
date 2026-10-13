# Native ordinary NPC side collision and retained state

2026-10-13; bounded US early-Rogueport native binding of
[source sphere/side response](FIELD_NPC_SPHERE_AND_MARIO_SIDE_COLLISION.md),
[polynomial/caller/history owners](FIELD_NPC_MARIO_SIDE_RESPONSE_NUMERICS.md),
[ordinary NPC catalog](NATIVE_AREA_ACTOR_CATALOG.md) and
[talk selection](NATIVE_NPC_TALK_SELECTION.md). This is source-derived native
logic, not measured Dolphin behavior, complete npcMain or hardware parity.

## PMNPC006 source metadata

The local catalog adds one little-endian uint32 npcFlags after each PMNPC005
actor row, increasing actor rows196→200 bytes; model rows remain100 bytes.
Existing versions1..5 remain readable with npcFlags0 (unbound metadata).
Objects require npcFlags0 and retain their separate collision/model owners.
The reader stages all records and rejects truncation/trailing bytes atomically.

The converter reads original Entry/setup/constructor bindings: NPCEntry3 plus
source setup0x80600 gives Chu3 flags0x80603; Frankly init OR0x40000000 gives
fresh flags0x40080603. The original art, geometry, model scalar2, source tribe
30/30 dimensions, message assets and animation resources are unchanged.
Metadata is independent of render culling and is not invented from visibility.
Legacy metadata0 is skipped by this bound collision owner rather than treated
as a known original active/collidable actor.

## Portable collision and source request graph

`npc_collision.cpp` implements a pure NPC vertical-segment/sphere query,
one-adjustment compAngle with opposite-sign180 ties, source finite quadrant
reductions and binary32-coefficient fused sine/cosine polynomials. The point
query writes distance and tests strict halfwidth+extra-radius admission;
it reads no actor flags. The side owner separately preserves inclusive vertical
intervals, radius0.5*(MarioWidth+NPCWidth+5), previous-NPC-XZ rollback and
second distance admission, retained-travel40-degree branch, source delta and
2/norm scaling above source threshold. It records contact before requesting
map-wall correction. A rejected post-rollback query retains the rollback.

Direct NPC update admission preserves flags1+2, current-state0x10 and terminal
bypass flags8/0x20/0x4000. Mainflags0x100000clear publishes state0x10; no full
NPC timer/update state machine is invented. Mario initialized dimensions20/37
and collision-width20 are supplied in the current ordinary domain. Field
angle requests use the original91-float system angle table. Current retained
travel is adapted from the existing movement-heading state; walk/dash and
original idle-adjust services own its publication, while air drift/zero-norm
wall heading remain separate.

An admitted contact requires a wall callback. Native callback converts source
headingtheta+160 to(sin,0,-cos), uses mode0,queryWidth20,currentheight and
resident original map/dynamic-object rays. This gives around-query radius10,
independent of NPC dimensions or separation radius. Original first/second
wall-pass ownership retains the saved unclamped first vector and negative/
unordered second-dot admission. The source speed0 operand is ignored by that
around policy. Contact writes native progressXZ only; source Y is preserved.

Host/ARM sqrt and libm/sincos/triangle/matrix services remain numeric adapters
to original SDK/Gekko operations. Exact frsqrte seed, double refinements,
exception/FPSCR/signed-zero domains and source backend traversal are unclosed.
Static bit fixtures for the finite polynomial graph do not measure original
execution or prove all surrounding numerical services equivalent.

## Native tick, identity, placement and scripted actors

Game runs NPC collision after current Mario/object owners and before the new
ordinary talk scan. It clears current contact first and scans source catalog
order. Cached instances are keyed by source actor identity and reset on room/
catalog generation changes; unseen actors are removed. Creation or explicit
source placement generation seeds previous root. Continuous script moves keep
previous root until the admitted end-of-NPC-update snapshot. No distance-based
teleport threshold is used.

Professor scene entry/set_position owns placementGeneration. Scripted Frankly
uses current source scene root, creation/lifetime flagnpcActive and only bits
explicitly owned by source flag_onoff requests, overlaid on original catalog
flags. Source OR/CLEAR0x40000020 retains movement bypass and collision-disable
mask0x20 independently. Draw hiding does not destroy collision identity or
substitute for those source masks. Actor deletion does remove native ownership.
Scripted rollback updates the scene root; end snapshot copies currentXYZ only
for records retaining active1+2. Rendering/talk admission read the same current
source flag overlay, rather than blanket flag3 or unknown flags.

Current dispatch is limited to Explore/Dialogue and skips trio battle, water
recovery, arrival, active fade, warp and pipe transitions. Original source
scene/entity admission remains separate from these native integration bounds.
Ordinary static actor visibility uses current known room/story/interior gates;
this is a coverage adapter, not complete all-story NPC lifetime reconstruction.

## Verification and remaining work

Native map-ray acceleration caches expanded XYZ triangle bounds after the
unchanged48-byte HEAD record prefix. Load staging and collision-transition
swap/rollback own the bounds with the triangle vector. Each ray computes its
segment bounds once; source-order candidates outside those bounds skip the
existing narrow phase. Dynamic object filtering and the original narrow-phase
intersection, attributes, cull and nearest/equal-hit rules are unchanged.
The expansion `.01 native +64*float epsilon*max(1,abs(vertex coordinates))`
is a deliberately conservative native acceleration margin, not an original
measured constant. Nonfinite/negative-length requests bypass this rejection.
Nearly-degenerate floating-point false hits outside finite geometric bounds
remain outside the proven corpus; no general instruction-exact parity claim.

The original gor_02 room has3,450 HEAD triangles; its native bounds cache adds
82,800 bytes. At the Mouser fixture, a no-hit16-ray around retry previously
tested4,816 grid candidates; the expanded XYZ bounds reject those static
triangles before narrow-phase divisions. Original five-room/full-scan parity
tests include800 horizontal/vertical/oblique queries and16 contact rays, plus
endpoint, near-boundary, zero-length and unsupported-input gate cases. Emulator
timing is separate from New2DS hardware performance; exact captures and timings
are recorded in progress.

Portable tests cover sphere strict equality/vertical endpoints, direct flags,
compAngle ties/one-wrap, independently derived polynomial bit fixtures,
rollback-on-miss,40-degree threshold, source cap amplification, contact-before-
wall request and wall mutation. Original-data Game tests cover Chu contact,
room invalidation, scripted Frankly flags/hiding/deletion and placement state.
Catalog tests cover PMNPC006 metadata and older-version/truncated input.
Exact make/test/emulator results are recorded by the implementation owner in
[progress](../../PROGRESS.md); this contract does not independently claim those
checks passed. No new Dolphin observations or New2DS hardware measurements.

Remaining: full original NPC timer/speed/gravity/pose/regular-event lifetime,
dynamic/alternate pools, transformed Mario dimensions/heading writers, all
scene collision flags, original map backend/FPSCR numerics, complete encounter/
object/door/pipe arbitration, wander/tavern behavior and matched original/hardware
verification. The partial Rogueport slice is not the complete remake.
