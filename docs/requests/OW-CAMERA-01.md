# OW-CAMERA-01 — Ordinary room camera tracking and limits

**Status: OPEN.** Part of the [request inbox](../DOCUMENTATION_REQUESTS.md); validated deliveries and locks are registered in the [delivery log](DELIVERY_LOG.md).
Filed by the native overworld implementation session, 2026-10-12, as one of the four "Native delivery and further subsystem requests" blockers (shared batch text in [OW-PARTNER-01](OW-PARTNER-01.md), heading kept in [OW-MOVE-01](OW-MOVE-01.md)), and moved here from `OVERWORLD_DOCUMENTATION_REQUESTS.md` during the inbox merge; request text is preserved verbatim. The documentation slice was explicitly requested; no independent research-agent ownership or full-game completion claim is made. Updated 2026-10-12.

## Open todos

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

- [ ] Ordinary room camera tracking/limits, authored events and height changes;
  native fixed framing/.07 smoothing remains provisional.
- [ ] Camera tracking/yaw/screen projection recovery to replace fixed framing/
  .07 smoothing/default on-screen assumptions — re-requested alongside the
  Rogueport roads binding, whose verbatim paragraph is kept in
  [OW-ROAD-01](OW-ROAD-01.md).
- [ ] Camera/control projection owners, which the same delivery leaves unbound.

## Request text (verbatim)

- OW-CAMERA-01: ordinary room camera tracking/limits, authored events and height
  changes; native fixed framing/.07 smoothing remains provisional.

## Answers and delivery notes (append-only history)

The camera recovery follow-up arrived inside the Rogueport roads delivery note;
it is preserved verbatim there ([OW-ROAD-01](OW-ROAD-01.md)). No camera contract
has been delivered yet.

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
