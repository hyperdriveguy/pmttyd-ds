# Ordinary ground following distances and harbor fall correction

US G8ME01 revision0; read-only static evidence, updated 2026-10-13.
Source bodies are `party_motion_homing_walk`0x8014fd4c,
`party_motion_stay`0x8015164c, `partyMarioHosei`0x800cab04,
`partySearchGround`0x800c3bcc and `party_motion_fall`0x8014cbb8.
No new Dolphin observation was made. All numbers below are source engine units;
native coordinates use scale0.01. Do not equate a speed threshold with a fixed
partner-to-Mario resting distance.

## Three distinct distance decisions

| Owner | Ordinary rule | Used source region |
| --- | --- | --- |
| Stay wake-up | Start with initialized P+40=50; override100 when owner stick magnitude+194==0 and base speed+180<1; owner byte+3c==2 overrides70 | [stay contract](FIELD_PARTNER_STAY_MOTION.md) |
| Ground homing speed | Far threshold C=45 normally,28 for signed P+28!=0; near B=50 for nonzero target activity,45 for zero activity; owner+3c==2 overrides B=70 | 8014fdd8..fdec,8014feb4..fee0 |
| History consumption | Compare PLANAR root-to-read-target distance against L=F(0.5,P+f4,P+104); ordinary Goombella width26 gives13+current speed | 80150324..38 |

The activity service's q0 route returns Mario+180 when positive, otherwise
Mario+188. Ordinary Mario's dash base188=2.25 means the activity is normally
nonzero even when current base180==0. Thus **B normally stays50 at rest**;
45 is the scalar-zero branch, not a generic idle gap. See
[activity selection](FIELD_PARTNER_LIVE_DIRECTION_AND_ACTIVITY.md) and
[target speed selection](FIELD_PARTNER_FLY_SOURCE_LEAVES.md).
Special aji maps/flags and forced-owner branches remain in their own contracts.

For history targets inside L, the local graph advances/read-queries the next
record. Equal read/write eventually requests zero speed and stay. It does not
retain a trailing polyline of length50. Ordinary constructor/init sets width26,
height24; Mario width20/height37 are documented in
[ordinary wall movement](../overworld/OVERWORLD_WALL_MOVEMENT.md).

## Speed and overlap decisions

Initial amount S comes from target-speed selection. Captured-owner 3D separation
below45 replaces it with1; separation>=50 replaces it with3. The handler requests
the smoothed writer, and requests it again for live target3D distance>=C.
See [speed writers](FIELD_PARTNER_FLY_SPEED_WRITERS.md) for the separate target
and current fields, positive-target fused0.05 adjustment and clamps.

For the ordinary far route at801503e0..5041c, compute an extra amount:

```
extra = (live3Ddistance - C) * originalFloat0.1
if extra > 1.5: extra = 1.5
if extra < 1.0: extra = 0
request immediate writer(ownerBaseSpeed + extra)
```

This is a thresholded bonus, not smooth interpolation or a constant dash cap.
At idle owner base0, distant catch-up can therefore cap at1.5; stopping position
depends on the history, state decisions and overlaps. The complete main body
contains additional loops/writes; these selected numeric regions do not prove
full original trajectories or the number of all smoothing requests per tick.

The [overlap correction](FIELD_PARTNER_OVERLAP_CORRECTION.md) uses a BOX:
X<=0.4*(26+20)=18.4, Z<=0.3*(26+20)=13.8,
Y<=0.4*(24+37)=24.4 in the ordinary flags-clear domain. Endpoints admit.
Inside, stationary-owner correction magnitude is4; moving owner uses1,
overridden by0.8*owner speed when that base is>1. The original bearing adjustment
checks unwrapped bearing minus owner travel heading within±4 degrees and moves
that bearing by±90. These are collision corrections, not a required radial gap.

General placement's ordinary primary displacement is30 behind Mario, while
fall placement4 starts from a distinct26-distance search and adds37 after it.
See [general placement](FIELD_PARTNER_GENERAL_PLACEMENT.md) and
[fall placement](FIELD_PARTNER_PLACEMENT4.md). The native restore's leftward0.3
seed is a finite facing adapter for the former magnitude; complete placement
search, constructor/save ownership and default facing still require binding.

## Floor query, fall ordering and the reproduced failure

Original [floor support](FIELD_PARTNER_FLOOR_QUERY.md) queries CENTER plus FOUR
heading-relative offsets at0.375*width: ordinary radius9.75. Ray origin is
rootY+0.75*height (18); ray extends down to rootY-abs(depth)-0.1.
PointY is Q1000-quantized in engine space. Retain the highest admitted support:
for a drop>=2 require drop<=abs(dropLimit); otherwise abs(height-rootY)<=11.
Nearest-hit/filter and slope>=50 rejection precede these comparisons.

Normal homing's tail calls depth10/dropLimit10, then floor loss requests fall
and stores acceleration-1 at80150e78..e88. Stay uses15/-15 and writes-1.5 on
loss. The actual [fall handler](FIELD_PARTNER_FALL_AIR.md) queries depth10 and
abs(OLD velocity) BEFORE integration. On support, it snaps and returns without
that call's gravity/horizontal tail. On a miss, velocity+=acceleration (default
-1.4 only for an admitted zero-acceleration entry), clamp at-16, then rootY+=v.
When key-off is clear, horizontal speed loses0.2, clamped at0, and the front-wall
request uses1.3*retained speed/direction. It does not home at full dash speed.
Main jump has its distinct [integrate-before-floor order](FIELD_PARTNER_JUMP_FLIGHT_AND_LANDING.md).

The previous native implementation used one center sample over crossed heights,
rejected landing when `walkable(root,floorY)` was false, treated every airborne
Mario record as jump-marked, and continued homing at constant dash speed in fall.
Harbor `(0,-5.1)` and `(1,-5.1)` routes missed the dock and reached deeply negative
Y before root-copy recovery. A valid support can be above the current root after
horizontal motion/quantization; wall occupancy is not the original floor
admission test. Five-point support and the correct ordering resolve that loss.

## Native scope and verification

`PartnerFollow` now uses the source dimensions, five sample offsets, Q1000 pointY,
height admission, ground-homing versus stay acceleration, fall query order,
velocity clamp and retained horizontal damping. `Game` passes source jump flags
and current base speed, uses ordinary partner static filtering, and separates
support from horizontal wall occupancy. Source-derived history reach, speed
selection/bonus, idle wake-up and overlap replace the fixed0.5 trail gap.

It remains an ordinary finite-domain adaptation: native vectors replace the80-
record bank; wall sliding, history/trig/rounding, exact call multiplicity, special
flags/owner scale, dynamic surfaces, contact caching and placement3/4 are still
distinct incomplete bindings. Read-only hashes/anchors are static evidence, not
Dolphin observation or hardware measurement.

Run homing, ground, direction, fall, jump, lifetime and front-wall verifiers.
The extended homing verifier checks64 instructions/12 constants including startup
r2=80427440,45/50/70/100,history0.5 and speed-bonus arithmetic anchors.
Portable tests cover pre-integration support, side-probe landing, overlap bounds,
history reach, catch-up and blocked paths. `partner_harbor_follow` is registered
when converted harbor ground exists; it checks an actual admitted Mario jump
at two deck positions after90 idle ticks,300 subsequent ticks, minimum partner
Y>=0.149, zero resets and grounded0.15 arrival. Both ISO and compiled packs remain
outside commits.

Isolated capture: `tests/azahar_partner_harbor_fall.py` samples the brief jump/fall
window, then moves Mario sideways to reveal naturally occluded Goombella. Capture
fixtures are native harness saves, not original GameCube saves or story evidence.
Exact session outcomes and remaining work are in [progress](../../PROGRESS.md).

```sh
python3 tools/make_partner_harbor_fixture.py
python3 tools/emulator_smoke.py --seconds 15 --fresh-save \
  --save-fixture artifacts/fixtures/partner-harbor-ledge.bin \
  --input-script tests/azahar_partner_harbor_fall.py \
  --output-dir artifacts/azahar-partner-harbor-fall
```
