# OW-CAMERA-01 — Ordinary room camera tracking and limits

**Status: OPEN.** Part of the [request inbox](../DOCUMENTATION_REQUESTS.md); validated deliveries and locks are registered in the [delivery log](DELIVERY_LOG.md).
Filed by the native overworld implementation session, 2026-10-12, as one of the four "Native delivery and further subsystem requests" blockers (shared batch text in [OW-PARTNER-01](OW-PARTNER-01.md), heading kept in [OW-MOVE-01](OW-MOVE-01.md)), and moved here from `OVERWORLD_DOCUMENTATION_REQUESTS.md` during the inbox merge; request text is preserved verbatim. The documentation slice was explicitly requested; no independent research-agent ownership or full-game completion claim is made. Updated 2026-10-12.

## Open todos

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
