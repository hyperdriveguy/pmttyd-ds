# OW-MOVE-01 — Ordinary Mario movement state and current input

**Status: OPEN (partially answered).** Part of the [request inbox](../DOCUMENTATION_REQUESTS.md); validated deliveries and locks are registered in the [delivery log](DELIVERY_LOG.md).
Filed by the native overworld implementation session, 2026-10-12, and moved here from `OVERWORLD_DOCUMENTATION_REQUESTS.md` during the inbox merge; request text is preserved verbatim. The documentation slice was explicitly requested; no independent research-agent ownership or full-game completion claim is made. Updated 2026-10-12.

## Open todos

- [x] Plain idle/walk/dash jump/threshold arbitration, including multiple
  surviving motion requests in one dispatcher invocation — answered by the
  [ordinary motion arbitration](../behavior/overworld/OVERWORLD_MOTION_ARBITRATION.md)
  contract (bounded plain0/1/2 domain, nine source-derived callback paths,
  derived four-callback domain bound; see the history section below).
- [ ] Transform/external-motion arbitration (`marioChkTransform80092624`) and
  the dispatcher tails outside that plain domain. Breadcrumbs:
  mot_stay800a7120→800a71a8; mot_walk80087bf8→80087c4c; marioChkJump80092814;
  marioChkTransform80092624; marioMotion8009461c/marioChgMot8009455c.
- [ ] Complete input producer stick clamp/deadzone and native signed-axis
  adapter domain. Breadcrumbs: marioUpdateKeyData80095cb0, marioGetStick80095af8
  and GP+13b8/13bc, physical PAD service documentation. Native .15 radial
  deadzone is provisional and is not documented by these field consumers.
  *(Partial: the `marioUpdateKeyData` body is delivered
  ([key-data publication](../behavior/overworld/OVERWORLD_MARIO_KEY_DATA_UPDATE.md)
  — six GP pad bytes, per-flag zeroing, **no** local deadzone) and the global
  producer is delivered
  ([physical producer](../behavior/input/PHYSICAL_INPUT_PRODUCER.md); the
  arbitration contract states that SDK producer clamping/deadzone and native
  Circle Pad quantization "remain separate adapter boundaries", so PADRead/
  PADClamp effects and the signed-axis adapter stay open.)*
- [x] Landing/reentry ownership — answered by the
  [ordinary airborne/landing contract](../behavior/overworld/OVERWORLD_AIRBORNE_AND_LANDING.md)
  (landing tuple/angle/node ownership, authoritative D=V/V=V landing, late
  ground dispatch; delivered `35f82d8` with `a4df746`/`ce29235` addenda and
  recorded under [OW-SUPPORT-01](OW-SUPPORT-01.md)).
- [ ] Airborne wall deflection and floor/slope movement-rate composition.
  Breadcrumbs: marioMoveMain800594a8 calls marioGetMoveRate800969a4 then
  marioCheckWall8008b790; special flag paths and angle/speed writes need all
  owners.
- [ ] Full ledge-loss, external motion, movement badge0x143, Paper/background
  modes, conveyors and additive velocity producers. Local motion leaves alone
  do not establish all persistent storage owners. *(The landing/reentry half of
  this item is now answered — see the landing/reentry todo above.)*
- [ ] Original isolated Dolphin traces: stable walk/dash launch, neutral release,
  opposite/orthogonal air input, idle launch, threshold crossing+jump, wall hit,
  slope and ledge loss. Record original revision/controller/input timestamps and
  motion/state/position fields; distinguish manager invocations from video frames.
- [ ] SDK/input signed quantization and deadzone, eligibility/contact/NPC/talk/
  system/force/pose/action/transform/badge/Jabara special graph, root locks,
  source execution traces and numerical/math services. Documentation ID1's
  partner movement remains separate; this slice makes no partner ownership
  claims.

## Request text (verbatim)

Remaining requested documentation:

- Complete idle/walk/dash jump/transform/threshold arbitration, including multiple
  surviving motion requests in one dispatcher invocation. Breadcrumbs: mot_stay
  800a7120→800a71a8; mot_walk80087bf8→80087c4c; marioChkJump80092814;
  marioChkTransform80092624; marioMotion8009461c/marioChgMot8009455c.
- Complete input producer stick clamp/deadzone and native signed-axis adapter
  domain. Breadcrumbs: marioUpdateKeyData80095cb0, marioGetStick80095af8 and
  GP+13b8/13bc, physical PAD service documentation. Native .15 radial deadzone
  is provisional and is not documented by these field consumers.
- Airborne wall deflection and floor/slope movement-rate composition. Breadcrumbs:
  marioMoveMain800594a8 calls marioGetMoveRate800969a4 then marioCheckWall8008b790;
  special flag paths and angle/speed writes need all owners.
- Full ledge-loss, landing/reentry, external motion, movement badge0x143, Paper/
  background modes, conveyors and additive velocity producers. Local motion
  leaves alone do not establish all persistent storage owners.
- Original isolated Dolphin traces: stable walk/dash launch, neutral release,
  opposite/orthogonal air input, idle launch, threshold crossing+jump, wall hit,
  slope and ledge loss. Record original revision/controller/input timestamps and
  motion/state/position fields; distinguish manager invocations from video frames.

## Answers and delivery notes (append-only history)

Partial answer: [ordinary movement contract](../behavior/overworld/OVERWORLD_MARIO_MOVEMENT.md).
Read-only US ISO pins sixteen containing bodies, eighteen instruction anchors and
six constants through `tools/verify_overworld_mario_movement.py`. Delivered exact
ordinary key→stick-query→motion→movement ordering, same-call motion dispatch,
strict3025 walk/dash threshold, entry-owned retained speed, jump-entry snapshot,
launch phase/countdown ownership, and plain-air retained travel plus current
input drift/cap. Corrects native immediate air walk/dash selection and neutral
stick freeze; launch contact policy must begin before movement without adding a
vertical startup tick.

No ISO writes, original asset redistribution, emulator profile/save mutation or
story/dialogue replacement occurred. Inspection outputs remain ignored or in/tmp.

Contract breadcrumb for the input-producer todo: the
[Pad-byte publication contract](../behavior/overworld/OVERWORLD_MARIO_KEY_DATA_UPDATE.md)
covers the `marioUpdateKeyData` body half (GP+0x13B8 pad source, per-flag
zeroing order, no local deadzone in that body) and links back to this request.
The SDK producer/deadzone and signed-axis adapter domain remain open, so the
todo stays unflagged.

### Native delivery and further subsystem requests (2026-10-12)

The four subsystem requests filed under this heading now live in
[OW-PARTNER-01](OW-PARTNER-01.md), [OW-ROAD-01](OW-ROAD-01.md),
[OW-INTERACT-01](OW-INTERACT-01.md) and [OW-CAMERA-01](OW-CAMERA-01.md).

Implemented plain retained-air travel/drift/cap in FieldMovement and Game;
corrected phase/counter startup and launch-before-horizontal order while retaining
explicit harness interaction/motion arbitration. Integrated the documented
field-battle entry/return service protocol with native control locks. Unit tests
are source-derived acceptance checks, not original execution observations.

### OW-MOVE-01 plain simultaneous-input closure

Delivered [ordinary motion arbitration](../behavior/overworld/OVERWORLD_MOTION_ARBITRATION.md)
and `tools/verify_overworld_motion_arbitration.py`:20 containing bodies,94
instruction anchors,15 data spans including original motion main/post rows.
The previous local pending-jump-overwrite question is closed in the bounded
plain0/1/2 domain: repeated dispatcher callbacks retry the unchanged A trigger,
so current ground0/1/2.25 base and current nonzero input heading are established
before surviving jump. Nine source-derived callback paths are supplied, including
neutral stop and both threshold crossings. Maximum four callbacks is a derived
domain bound, not an invented original dispatcher clamp.

Ordinary late landing selects pending idle0 or walk1; it does not universally
enter land13. Next manager invocation re-queries input before entry; prior
airborne A trigger is not locally replayed. Fall entry preserves retainedbase/
travel and signed counter, while jump entry resets and increments counter1.
Separate land13 leaf and exposed phase tails remain distinguished.

Native launch correction can classify current ground input before begin, discard
grounded displacement, then run air policy with established speed/direction.
Standing analog.5→original magnitude35 plus A gives base1+drift.13125, hence
.0113125 native displacement; drift-only expected.0013125 was a provisional
native fixture. Original runtime/numerical equivalence remains unclaimed.

Still requested: SDK/input signed quantization and deadzone, eligibility/contact/
NPC/talk/system/force/pose/action/transform/badge/Jabara special graph, root locks,
source execution traces and numerical/math services. Documentation ID1's partner
movement remains separate; this slice makes no partner ownership claims.
