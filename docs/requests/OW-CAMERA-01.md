# OW-CAMERA-01 — Ordinary room camera tracking and limits

**Status: OPEN.** Part of the [request inbox](../DOCUMENTATION_REQUESTS.md); validated deliveries and locks are registered in the [delivery log](DELIVERY_LOG.md).
Filed by the native overworld implementation session, 2026-10-12, as one of the four "Native delivery and further subsystem requests" blockers (shared batch text in [OW-PARTNER-01](OW-PARTNER-01.md), heading kept in [OW-MOVE-01](OW-MOVE-01.md)), and moved here from `OVERWORLD_DOCUMENTATION_REQUESTS.md` during the inbox merge; request text is preserved verbatim. The documentation slice was explicitly requested; no independent research-agent ownership or full-game completion claim is made. Updated 2026-10-12.

## Open todos

- [x] Reconciled bounded shipped-camera container/zone/straight-dispatch/
  duplicate, shift/lifecycle and event-blend deliveries — [porting coverage](../behavior/camera/ROGUEPORT_CAMERA_PORTING_COVERAGE_AND_REMAINING_BINDINGS.md).
  Nine source verifiers pass;44 records/494 triangles/220 pairs and finite
  boundaries indexed. Five-file metadata verifies zero extras/disables and20
  fixed-Y records. This checks documentation delivery, not native composition.
- [ ] Compose and validate these source services across all44 actual records;
  retain numeric/provider/clock/pause/alias boundaries and original visual traces.

- [x] Complete local camRoadMain/getV60FPS — [manager/timebase supplement](../behavior/camera/FIELD_CAMERA_ROAD_MANAGER_COPY_AND_TIMEBASE.md).
  Fresh/captured work, eight pair-copy loops, timestamp/progress/finish order,
  signed division requests and returned-quotient rollover pinned; selector open.

- [x] Complete local calcHokanCamRoad/calcCurrentCamState —
  [road interpolation/current construction](../behavior/camera/FIELD_CAMERA_ROAD_INTERPOLATION_AND_CURRENT_STATE.md).
  Exact easing constants/nearest-angle/ordered writes, mode0 points, prefix/
  inactive/active geometry and matrix requests pinned; manager/selection open.

- [x] Complete local compareCamRoad and two scoped camRoadMain argument
  directions — [road comparison/rate](../behavior/camera/FIELD_CAMERA_ROAD_COMPARISON_AND_TRANSITION_RATE.md).
  Integer/float progress gates, transition mask, weighted metrics/sqrt,
  deadband and output-preserving rejection/reciprocal publication pinned.

- [x] Complete local follow-rate/ID get/set/reset/init and shift/rate reset
  leaves — [rate/ID/reset owners](../behavior/camera/FIELD_MARIO_CAMERA_RATE_ID_AND_RESET_OWNERS.md).
  Separate .2 follow and .03/.06/.06 shift reset words, raw templates, channel
  preservation and40 direct DOL caller sites verified; other writers open.

- [x] Complete local marioCamMain/marioUpdateCam and camFollowYOff/On,
  marioNoUpdateCamPos/marioUpdateCamPos — [shift/target/control contract](../behavior/camera/FIELD_MARIO_CAMERA_SHIFT_TARGETS_AND_CONTROLS.md).
  Three biased shift channels, target/height gates, owner snapshots, expiration
  arithmetic and heading flag/cache pinned; room framing remains open.

- [ ] Authored camera-road **limits**, authored-event admission and room height
  changes; native fixed framing/.07 smoothing stays provisional until that
  service is reconstructed. *(Partial: zone selection, authored curves, eye/at
  construction and the event/dolly/interpolation consumers are delivered; the
  lifecycle contract itself still reports "authored camera-road zone/curve
  selection and limits remain unclosed".)*
- [x] Camera tracking/yaw/screen projection recovery to replace fixed framing/
  .07 smoothing/default on-screen assumptions — re-requested alongside the
  Rogueport roads binding (kept verbatim in [OW-ROAD-01](OW-ROAD-01.md)) and
  answered by the [camera lifecycle/projection](../behavior/camera/OVERWORLD_CAMERA_LIFECYCLE_AND_PROJECTION.md)
  contract, which closes ordinary field camera lifecycle, player target/follow,
  input yaw and screen-cache composition.
- [x] Camera/control projection owners — answered by the same lifecycle/
  projection contract and its evidence manifest (all 75 gor_00–04 EVT camera
  owners, 332 camera calls and the five original camera containers pinned;
  verifier `tools/verify_overworld_camera.py`).

## Request text (verbatim)

- OW-CAMERA-01: ordinary room camera tracking/limits, authored events and height
  changes; native fixed framing/.07 smoothing remains provisional.

## Answers and delivery notes (append-only history)

The camera recovery follow-up arrived inside the Rogueport roads delivery note;
it is preserved verbatim there ([OW-ROAD-01](OW-ROAD-01.md)).

Camera contract delivery (2026-10-12): the request was filed at 12:04 and the
first contract landed at 14:06 (`cd25466`), followed by
[lifecycle/projection](../behavior/camera/OVERWORLD_CAMERA_LIFECYCLE_AND_PROJECTION.md),
[container](../behavior/camera/OVERWORLD_CAMERA_CONTAINER.md),
[straight paths](../behavior/camera/OVERWORLD_CAMERA_STRAIGHT_PATHS.md),
[zone triangles](../behavior/camera/OVERWORLD_CAMERA_ZONE_TRIANGLES.md),
[road state](../behavior/camera/OVERWORLD_CAMERA_ROAD_STATE.md),
[road transitions](../behavior/camera/OVERWORLD_CAMERA_ROAD_TRANSITIONS.md),
[dolly/event blends](../behavior/camera/OVERWORLD_CAMERA_DOLLY_AND_EVENT_BLENDS.md),
[interpolation/timer domain](../behavior/camera/OVERWORLD_CAMERA_INTERPOLATION_AND_TIMER_DOMAIN.md),
[authored segment dispatch](../behavior/camera/OVERWORLD_CAMERA_AUTHORED_SEGMENT_DISPATCH.md),
[map lifetime](../behavior/camera/OVERWORLD_CAMERA_MAP_LIFETIME_AND_DUPLICATES.md),
[moving shift correction](../behavior/camera/OVERWORLD_CAMERA_MOVING_SHIFT_CORRECTION.md)
and [collision target/shift](../behavior/camera/OVERWORLD_CAMERA_COLLISION_TARGET_AND_SHIFT.md).
Tracking, input yaw and screen projection are therefore closed; the lifecycle
contract's own remainder (authored zone/curve limits) stays open above. These
camera deliveries are not yet registered in the [delivery log](DELIVERY_LOG.md).

## 2026-10-13 — Complete local Mario camera shifts and update controls

Delivered [six complete local functions](../behavior/camera/FIELD_MARIO_CAMERA_SHIFT_TARGETS_AND_CONTROLS.md)
for the native field camera dependency. Three separately produced rates and
biased single-precision shift operations precede the target-update gate.
FollowYOff selects extra horizontal shift while preserving existing target Y;
NoUpdateCamPos still evolves shift channels. Main preserves entry/timer/heading
owners across requests and conditionally skips position update for motion29.
Expiration and cached-heading flags retain exact arithmetic/request order.
This is original static source evidence, not measured native framing equivalence.

Validation: read-only ISO manifest15 spans/294 anchors/17 calls; independent
all290 words/27 literal mathematical vectors, every65536 signed-halfword timer/
duration input, decoded masks/channel/call identities/source pins passed;
Markdown links/scoped diff check passed. Raw temporary bodies stay /tmp.
Docs only; no make test/all rerun. Rates/default producers, room limits,
camMain/zoom/event/trig consumers, pause/cadence/original visual execution and
full native camera binding remain OPEN; no .07 smoothing claim was closed.

## 2026-10-13 — Complete camera rate/ID/reset leaves and direct owners

Delivered [eight complete leaves](../behavior/camera/FIELD_MARIO_CAMERA_RATE_ID_AND_RESET_OWNERS.md).
Reset follow15c is distinct from shift150/154/158. Reset targets/rates leave
current13c/144/14c untouched. Two original raw three-word templates differ;
ffffffff words are retained as words. Direct DOL census40 sites records init/
reinit, camera/party/EVT/item/motion owners; it does not establish branch admission
or REL/indirect writers. mot_fall's later setter prevents treating reset .2 as
unconditional. No provisional native smoothing used as original evidence.

Also indexed the existing [lifecycle/projection contract](../behavior/camera/OVERWORLD_CAMERA_LIFECYCLE_AND_PROJECTION.md):
its verifier re-passed39 DOL spans/72 anchors/five camera files/75 event owners/
332 camera request bindings. It already documents bounded ordinary target follow,
yaw/projection and update order; authored road solver/limits remain open. The
older history statement that no camera contract had arrived is superseded by
these linked deliveries, without rewriting that historical request text.

Validation: new manifest16 spans/167 anchors/eight calls; independent all116
leaf words, exact reset layouts/raw words/capture order/constants and exhaustive
40-site direct DOL census passed. Markdown links/scoped diff checks passed.
Docs only; no make test/all rerun. Raw inspection artifacts stay /tmp.
OW-CAMERA-01 remains OPEN: caller/value/owner domains, other field writers,
road geometry/limits, original pause/cadence/visual execution and native binding.

## 2026-10-13 — Complete camera-road comparison and transition rate

Delivered [complete compareCamRoad](../behavior/camera/FIELD_CAMERA_ROAD_COMPARISON_AND_TRANSITION_RATE.md).
B.word0 and same-pointer integer progress rejection leave output unchanged.
Later floating progress is a separate test; weighted metric/point-distance,
three frsqrte refinements, joint deadband and reciprocal publication retain
original FP operation/capture order. Two scoped caller regions establish opposite
A/B directions; no universal old/new labels or measured duration invented.

Validation: read-only ISO manifest18 spans/306 anchors/one call; independent
all279 words/17 literal mathematical vectors/mask/FP/global/weight/output/caller
sites/constants/fresh source pins passed. Markdown links/scoped diff passed.
Vectors begin with supplied integer or postmetric inputs, not original acos/
frsqrte observations. Docs only; no make test/all rerun. Raw listings stay /tmp.
OW-CAMERA-01 stays OPEN: geometry/zone/curve selection, full road transition
composition, profile/state/value/FP domains, limits and original visual binding.

## 2026-10-13 — Complete road interpolation and current-state construction

Delivered [two complete local functions](../behavior/camera/FIELD_CAMERA_ROAD_INTERPOLATION_AND_CURRENT_STATE.md).
Exact phase/easing constants differ from mathematical pi/.5; weight has only
an upper clamp. Direction wraps first angle toward second, with inclusive pi
boundaries, before sin/cos output. Mode0 copies points while still interpolating
other fields. Current-state prefix precedes inactive test; active geometry is
captured before separate cos/sin/cos and matrix requests. Scoped caller pins
r6 supplied state unused locally and output in r7. No measured framing invented.

Validation: read-only ISO manifest20 spans/294 anchors/twelve calls; independent
all274 words/14 literal mathematical vectors/write/capture/gate/loop/call/caller
sites/twelve constants/fresh pins passed. Markdown links/scoped diff passed.
Docs only; no make test/all rerun. Raw temporary bodies stay /tmp.
OW-CAMERA-01 remains OPEN: selection/geometry/full manager/timebase, state/FP/
alias/value domains, math/matrix internals, original camera and native binding.

## 2026-10-13 — Complete road manager and timebase supplement

Delivered [complete manager/time helper](../behavior/camera/FIELD_CAMERA_ROAD_MANAGER_COPY_AND_TIMEBASE.md),
strengthening the existing finite transition contract. Complete fresh/captured
copy/timestamp/progress order includes the retained-source/fresh-destination
no-transition copy. getV60FPS requests signed __div2i before unsigned comparison
of returned quotients; decreasing quotients use UINT64_MAX-P+C without+1.
No source clock rollover correction or render-frame timing substituted.
Earlier comparison/interpolation contracts now link this local owner closure.

Validation: manifest13 spans/398 anchors/nine calls; independent all370 words/
17 literal mathematical vectors/eight96-byte copy layouts/publication/time/
divider/call sites/constants/fresh ISO pins passed. Existing finite transition
verifier re-passed seven spans/62 anchors/eight admission cases. Link/scoped
checks passed. Docs only; no make test/all rerun; raw artifacts stay /tmp.
Geometry/selection/limits, libraries/state/time/IRQ/alias/FP domains and original
camera/pause/cadence/visual/native binding remain OPEN; parent stays OPEN.

## 2026-10-13 — Reconciled Rogueport camera porting coverage

[Coverage map](../behavior/camera/ROGUEPORT_CAMERA_PORTING_COVERAGE_AND_REMAINING_BINDINGS.md)
registers existing bounded container/zone/path/duplicate/shift/lifecycle/event
contracts with concrete native dependency order. Nine verifiers re-passed;
source inventory matches five existing file hashes and confirms all44 records
version1.01/extraCurveCount0/disable0/count12/endpoints2,3;20 fixed-Y gates.
Circular/extra-curve engine branches are not admitted by these shipped data,
but full-game obligations remain. Moving correction composition across all44
queries still needs validation; original visuals/native framing stay OPEN.

Corrected the manager supplement's ABI ambiguity: seventh correction argument
is f7, captured as f22 at8000b798, distinct from integer r7. Added original
capture pin; manager manifest now399 anchors. Nine source checks, metadata
assertions, capture pin, link audit/scoped diff passed. Docs only; no make
rerun or raw assets committed. Parent remains OPEN.
