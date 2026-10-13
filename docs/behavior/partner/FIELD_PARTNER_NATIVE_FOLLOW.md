# Native ordinary Goombella following and original evidence

US G8ME01 revision0, recovered 2026-10-12 from the read-only local ISO.
Evidence B (static binary analysis). No Dolphin trajectory was observed.
Native scope: ordinary ground Goombella in the five-room Rogueport slice.
This is a functional equivalent for jump/fall/path/recovery categories, with
explicit adapters below; it is not instruction-exact complete party locomotion.

## Original ground-homing findings

`party_motion_homing_walk` at `0x8014fd4c`, size `0x1260`, is the missing
ground handler selected by the [walk dispatcher](FIELD_PARTNER_WALK_DISPATCH.md).
`tools/verify_field_partner_homing.py` checks its whole-body identity and 26
selected instruction anchors. A whole hash does not establish interpretation
of every branch. Inspection ELF/listing remain `/tmp/partner-homing*` only.

* `0x8015063c..4c` advances the history read index and reads XYZ. Floor
  loss requests motion3 at `0x801506bc..cc`; it does not refuse horizontal
  motion merely because a proposed destination has no floor.
* `0x80150718..2c` requires partner grounded flag2 and current record flag1
  before preparing a jump. It reads history/target coordinates and performs a
  terrain query. `0x801508dc..e4` requests motion2. Thus ordinary following
  consumes jump history independently of Mario's current rendered air state.
  All special flags, target transformations and backend filters remain separate.
* After front-wall contact, `0x80150bf0..c14` increments the partner's byte
  `+0x4d`, sign-extends it, compares to8, and enters correction only when
  greater than8, then resets it. `0x80150c30..c78` queries front floor and
  can prepare a jump; `0x80150c80..cf4` can request upstairs motion6.
  Other branches query live target direction/distance and alter the history
  or root. This is a local contact counter, not a measured time interval.
* `0x80150edc` calls the documented
  [out-of-screen recovery](FIELD_PARTNER_SCREEN_RECOVERY.md) after ground work.
  Recovery is not a generic separation-distance teleport or trail-size timeout.

The [history writer](FIELD_PARTNER_HISTORY_WRITER.md),
[jump initialization](FIELD_PARTNER_JUMP_INITIALIZATION.md),
[flight/landing](FIELD_PARTNER_JUMP_FLIGHT_AND_LANDING.md),
[fall](FIELD_PARTNER_FALL_AIR.md) and
[ground postpass](FIELD_PARTNER_GROUND_POSTPASS.md) provide the numeric owners.
Original main jump initializes without integrating on that call, prepares
count12..40, uses horizontal speed2.25, clamps velocity3..13 and integrates
velocity before Y. Ordinary ground floor loss stores acceleration-1.5; fall
defaults zero acceleration to-1.4 and caps velocity at-16. Units here are
source engine units. Original main jump's below-2000 recovery copies Mario's
XYZ; fall instead requests placement4 and adds37. Neither is a generic
distance threshold. Original sound requests are0x927 takeoff/0x928 landing;
their macro playback is not bound by this patch.

## Native binding and deliberate adapters

`include/partner_follow.hpp` owns host-testable 3D marks, ground/jump/fall
state, wall sweeps, supported path lookahead and screen candidate recovery.
`Game::updatePartner` runs after Mario's actual collision and vertical result,
using the same simulation tick and the room's imported geometry. Motion and
history reset on restore, road arrival and completed player recovery.
Goombella requests original selector4 `PKR_J_1A` in air; facing still consumes
actual displacement. A takeoff call changes state without moving Y.

The finite ordinary main-jump formula with owner scale1 is implemented at
native scale0.01, including fused initialization denominator/count arithmetic.
Fall acceleration and velocity clamp use the recovered values. Landing queries
check crossed heights and reject water-attribute0x200 support. No missing floor
blocks horizontal movement. No queue-size or wall-duration teleport remains.

These composition choices remain native adapters, **not measured behavior**:

* Vector history with512-mark append capacity replaces the source80-record
  ring and its exact read/write/flag quirks. Consecutive air marks are suppressed;
  equal-XZ air edges are retained/coalesced so short jumps cannot disappear.
  The0.5 following gap and0.50001 air-edge tolerance are native scheduling.
  Prepared jump targets follow marks rather than every original target branch.
* Ground budget uses native DashSpeed; jump budget is0.0225. Air path steering,
  ground/fall query order, crossing-based landing, epsilon and step/slope
  handling adapt existing room geometry rather than bind all original five-ray
  floor/cache and nine-ray front-wall services. Owner scale1 is an assumption.
* Sweeps sample at most0.025 units apart and slide X then Z. After nine blocked
  ticks, lookahead discards an obsolete prefix only if a supported swept route
  exists, stops at jump markers and refuses paths over8 units. These are native
  collision/path corrections; original contact flag and query branches differ.
* Screen recovery uses the original ordinary initial/candidate X margins,
  five downward offsets, optional raised retry and101-move bound with the
  current native field camera. Native width0.2 supplies the candidate step.
  Near-Mario placement3, full placement4 and original camera4/backend ownership
  are still unbound. Below-20 native Y currently reseeds from Mario for both
  air types; that is equivalent to main jump's root-copy recovery category,
  but is an adaptation of fall's distinct placement4/+37 branch.
* The current field lifecycle still gates partner updates during player
  water/void choreography and harness dialogue. Dynamic objects, platform
  attachment, ceiling/ability/slit/fly modes and original sound playback remain
  separate work. No complete original-runtime or hardware parity is claimed.

## Validation and next fidelity work

`tests/partner_follow_tests.cpp` exercises native spacing, delayed stationary
jump, initialization without integration, independent landing, lower ledge
fall, thin-wall sweep, stale blocked history, persistent wall nonteleport,
screen recovery and the Game binding. Original jump/fall/recovery/history
verifiers independently check read-only source bytes. Run `make test` and
`make all`; exact session outcomes belong in [progress](../../PROGRESS.md).

Next: close and port the full ground-homing target/index graph, original
partner floor/front-wall filters and placement3/4, then record Dolphin
stationary/running jump, ledge, wall reversal and off-screen trajectories in
[mechanics](../../MECHANICS.md). Do not convert these static fixtures or native
defaults into measured original behavior.
