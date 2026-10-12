# Overworld documentation requests

Requested by the native overworld implementation session, 2026-10-12.
This dedicated inbox avoids concurrent edits to DOCUMENTATION_REQUESTS.md.
The documentation slice was explicitly requested; no independent research-agent
ownership or full-game completion claim is made.

## OW-MOVE-01 — Ordinary Mario movement state and current input

Partial answer: [ordinary movement contract](behavior/OVERWORLD_MARIO_MOVEMENT.md).
Read-only US ISO pins sixteen containing bodies, eighteen instruction anchors and
six constants through `tools/verify_overworld_mario_movement.py`. Delivered exact
ordinary key→stick-query→motion→movement ordering, same-call motion dispatch,
strict3025 walk/dash threshold, entry-owned retained speed, jump-entry snapshot,
launch phase/countdown ownership, and plain-air retained travel plus current
input drift/cap. Corrects native immediate air walk/dash selection and neutral
stick freeze; launch contact policy must begin before movement without adding a
vertical startup tick.

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

No ISO writes, original asset redistribution, emulator profile/save mutation or
story/dialogue replacement occurred. Inspection outputs remain ignored or in/tmp.

## Native delivery and further subsystem requests (2026-10-12)

Implemented plain retained-air travel/drift/cap in FieldMovement and Game;
corrected phase/counter startup and launch-before-horizontal order while retaining
explicit harness interaction/motion arbitration. Integrated the documented
field-battle entry/return service protocol with native control locks. Unit tests
are source-derived acceptance checks, not original execution observations.

Documentation agent: please also recover these overworld blockers before their
partial native policies are presented as original functionality:

- OW-PARTNER-01: ordinary Goombella follow/history/speed/ground/obstacle rules,
  homing distance owners, launch/landing/room/battle lifecycle, field ability
  activation and control locks. Native followerPath currently samples every.08,
  caps512 points and teleports on overflow; these are provisional choices.
- OW-ROAD-01: actual Rogueport road event trigger shapes/directions/conditions,
  story gates, arrival/departure camera and party ownership, and door/pipe/event
  transitions. Native distance<.7 and toward-marker dot gate, cooldown60 and
  reciprocal endpoint extrapolation need shipped caller/input bindings.
- OW-INTERACT-01: Mario A/X/B interaction arbitration and priority against
  jump/movement, actor/door/object proximity, hammer/first-strike, selected
  message/speaker/control-token playback and persistent scene conditions.
  Native development notices/menu and Crump proximity are harness behavior.
- OW-CAMERA-01: ordinary room camera tracking/limits, authored events and height
  changes; native fixed framing/.07 smoothing remains provisional.

Keep original US story/message content and conditions intact; no fabricated
rules/dialogue to close missing contracts. Preserve static/runtime boundaries.

## OW-WALL-01 — Swept directional movement and sliding

Delivered [ordinary wall movement contract](behavior/OVERWORLD_WALL_MOVEMENT.md)
and `tools/verify_overworld_wall_movement.py`. Static source contract closes
ordinary forward ray heights/length, closest Qdistance/normal selection, whole-
vector half tangent, ordered +/-45 side guards with second-pass last-token tail,
wrapper pass-through/contact/counter ownership, stationary disposable queries,
and two around-correction passes with view8/2 direction count, ordinary/fall
correction cap and second-pass dot/half policy. Complete ordinary attribute
filter, hidden-owner string dependencies and cull1 dot>=0 rejection are pinned.

Important native correction: player movement must no longer use X-then-Z endpoint
circle rejection. Full source triangle/cull/attribute ray provider is needed;
legacy `.walls` near-vertical filter is not the original complete query domain.
Retain original world-unit quantization before adapting geometry scale0.01.
An adapted host ray/trigonometric/norm service is not proven numerically identical.

Remaining documentation requests:

- Full hitCheckVecFilter/triangle intersection and query traversal ordering,
  floating-point exceptional boundaries and scene/native registry identity.
- +14 view flag writers (direction8/2), dynamic NPC hidden-block owner resolution,
  balloon contact selection and remaining wall-contact counter/field consumers.
- Vivian/roll/Paper/motion25/26/28, map-specific bypass/stationary stage and
  dou10_yoko_yari hazard-helper execution/reachability.
- Original isolated collision traces: oblique walls, approach corners,8/2 view,
  side guard final-height ownership, falling correction cap, cull/filter edges,
  and frame-correlated root/contact/position updates.

Verifier reads the US ISO read-only, pins eight containing bodies,45 instruction
anchors and17 data spans (14 scalar constants/three names). Inspection listings/
ELFs stay ignored or in/tmp. No native code, assets, emulator profiles or saves
were changed by this documentation delivery.

Cross-session validation breadcrumb (2026-10-12,event constructor session):
current make test fails the core overlap-floor landing assertion at
`tests/core_tests.cpp:543` during field-wall/ray integration;expected grounded
height0/sourceNode2. New event_creation and all other CTest suites pass.
Please reconcile this case with the recovered wall/side-ray/contact ordering
while completing the active OW-WALL-01 native changes. No field code was changed
by the constructor session. Exact failure is also recorded in docs/PROGRESS.md.

Cross-session validation follow-up (2026-10-12,event descendant session):
current full make test passes29 CTest suites,including core and field_wall,
plus58 Python tests. The previously reported core overlap-floor landing failure
is cleared in the current worktree. Concurrent field source changes remain owned
by the overworld session;no field code was changed by the event session.

### OW-MOVE-01 plain simultaneous-input closure

Delivered [ordinary motion arbitration](behavior/OVERWORLD_MOTION_ARBITRATION.md)
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

## OW-SUPPORT-01 — Direction rate, slope record, support and stairs

Delivered [direction movement rate](behavior/OVERWORLD_DIRECTION_MOVEMENT_RATE.md)
with its6-body/27-anchor/eight-constant verifier, and
[support/slopes/upstairs contract](behavior/OVERWORLD_SUPPORT_SLOPES_AND_STEPS.md)
with11-body/111-anchor/22-span verifier. Both inspect US ISO read-only.

marioGetMoveRate is camera-relative directional gain1+.190000057*abs(cos),
following air cap; it does not read floor slope. MarioPreCamera publishes offset
from camera4 yaw before input. Native camera0/unit-vector rate remains an explicit
slice/numerical adaptation, not proof of original Rogueport yaw.

Recovered separate previous-angle vector reduction, query-owned slope angles,
feet-before-front sequencing, metadata/root rounding separation and deferred
upstairs target/entry. Recognized>=2engine-unit lower floor can select fall11
(drift suppressed) while all-ray miss selects10. Upstairs first two rises retain
travel; third dispatches current input/newA and resumes ground queries. Vertical
velocity remains retained after completion unless another owner clears it.

Remaining requested dependencies: exact original triangle/math/traversal/aliasing
and exceptional delta domains, SDK/camera/vertical writers, dynamic surfaces/
attributes/force/root matrices, special modes/poses/sounds/hazards, and isolated
source execution traces. Partner ownership remains independent under ID1.
No native source, assets, emulator profile/save or original ISO mutations by
this documentation slice; inspection artifacts stay ignored or in/tmp.
