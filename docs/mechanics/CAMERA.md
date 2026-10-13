# Camera evidence

Camera collision/shift, authored path/zone geometry, straight-path branches, retained targets and the native camera pipeline binding entries. Split out of the [mechanics ledger](../MECHANICS.md) on 2026-10-12; sections are verbatim.

## 2026-10-12 — Contact target, shift lifecycle and full authored zone geometry

Native86150f2 binds the genuine winning ground-ray contact to Mario's retained
camera-collision target; misses retain the value and display40000 is transient.
Camera consumption precedes current motion and adds one engine unit to contactY.
Shift work follows OVERWORLD_CAMERA_COLLISION_TARGET_AND_SHIFT.md, including the
literal desired-sign clamp and flag20 overshoot. Map reset preserves the source
shift timestamp; a separate global60Hz simulation-clock bridge survives native
restore. This bridge is an adaptation, not OS/Gekko clock reconstruction. The
prior road candidate producer remains unbound, so ordinary native secondary
shift still awaits that pipeline rather than using a current-record substitute.

Native19c5104 extends the earlier19-record solver to all44 shipped finite paths.
Source near-normal collapse selects straight dispatch for all220 pairs; raw
halfspaces/output tangents are retained. Exact zero parameter/tangent deltas
close all10 duplicates for finite incoming fractions, permitting an equivalent
native fraction0 while preserving point evaluation/ties. Infinite-line host
projection and sqrt remain numerical adaptations, not original SDK parity.
Native93b2817 implements the general source zone triangle/caller service,
including234 nonhorizontal authored triangles, exact-zero/vertical rejection,
coplanar winding asymmetry and polygon equality replacement. Zone fallback
height equality remains strict. Optional native tests visit all494 triangles;
ill-conditioned f32 slivers are checked for safe finite/preserved output without
asserting a mathematical centroid must survive source strict boundaries. No
production tolerance or flattened geometry is added. These are static-source
and native checks; no original Dolphin camera trajectory was observed.

Candidate geometry is now closed for the shipped finite room domain. Moving
correction, state comparison/blending and final ordinary eye/at binding still
must replace the provisional renderer before camera fidelity is claimed.

## 2026-10-12 — Original camera data and straight-path branch

Native2ea1c21 reads immutable big-endian original camera offsets without original
VA relocation. Source header/lens pointer and all44 record/vector/zone identities
remain intact;empty collections retain sentinels. Native byte/reference bounds
and optional errors are safety adapters,not source validation claims.
Native22968ed follows OVERWORLD_CAMERA_STRAIGHT_PATHS.md's19-record verified
domain:ordered segment slabs,uncapped-distance strict ties,dominant-axis U,
side-cap equality,endpoint indices/flags and unwritten result+4 ownership.
Result+8 is actual segment-startY,not integer found;NoHit/Unsupported do not
clear output or become invented camera limits. Host algebra/square-root differs
from the source numerical pipeline;no original trajectory measurement is claimed.

This is candidate-path infrastructure,not completed ordinary view binding.
The camera's genuine look/collision input comes from eligible ground-search
ray points and is retained on misses;player-root substitution is unauthorized.
Dynamic shift,state comparison/transition,curved/duplicate paths and live lens/
event owners remain required. All authentic records are imported,including
unresolved branches;the fixed native view is still explicitly provisional.

## 2026-10-12 — Native camera retained targets and screen-cache policy

Native dff51f1 binds the closed ordinary camera target prefix from
OVERWORLD_CAMERA_LIFECYCLE_AND_PROJECTION.md: camera reads the preceding Mario
publication before current motion, separate engine-unit shifts/follow and direct
XZ target writes replace uniform native.07 smoothing. Source display10000 shifts
still approach while target writes are suppressed;20000 alternate XZ/depth retains
Y. Motion29 skips target update;ship25 vertical ownership remains unsupported.
Current input/drift uses the camera-produced Q1000 heading snapshot. Cached signed
SX/SY/SZ refresh after MoveMain, before Fall integrates current Y; truncation,
negative-W evaluation and inclusive0..600/0..535 cutoff replace always-on-screen.

The frame provider remains explicitly provisional: fixed native eye/at/FOV42,
portable look matrix/GX vector and608x480 policy viewport. Source road solver,
transitions,events,lens and original SDK/trig/numerical equivalence remain open.
The cache owner can consume a supplied frame once those native services are bound.
Recovery freezes/rebase history remain harness integration points. Validation is
source-derived host fixtures/native emulator behavior,not new Dolphin observations.
Authored geometry*.1 versus engine*.01 is now reconciled by recovered map/hit
parent scale10;the earlier uncertainty was not evidence of a geometry mismatch.

## 2026-10-12 — Ordinary authored camera pipeline native binding

Native64008d5 implements OVERWORLD_CAMERA_ROAD_TRANSITIONS.md and
OVERWORLD_CAMERA_MOVING_SHIFT_CORRECTION.md over all shipped source path/zone
providers. Original7th-argument correction uses prior destination shift,
anchor/flags/timestamp, independently queries prior/current shifts, resets
cap-release weights before budgets and preserves initial direction/index.
Transition compare ordering, literal easing scale, restart-from-current,
geometry-only blend writes and metadata completion copies are retained. Global
simulation-clock milliseconds are an adapter for the two source GP clocks.
Ordinary camera state feeds rendered eye/at, cutaway eye, heading and screen
cache with live default lens rather than camera-container metadata. Source
numerical services, handheld aspect, actor pose backend and event/dolly/lens
stages remain explicit boundaries. Isolated native captures/host cases prove
binding and traversal, not original Dolphin trajectory equivalence. The prior
ledger's unbound candidate/view limitation is superseded by this commit; remaining
actor root/scale and event camera work must still be completed.

## 2026-10-12 — Recovery camera ownership correction

Native8ee75c4 uses OVERWORLD_BOTTOMLESS_SCREEN_START_PROBE.md for ordinary
returnXYZ/+100/+10 current-frame probing and same-callback horizontal travel.
Source motions31/33 are water/bottomless;29 is pipe. Camera solver continues
while display10000 retains or explicitly writes follow targets. Water admission
captures current rootY into the2b8 adapter; phase12/14 delta uses that value,
not root velocity/floor. Travel preparation ends the callback before publication.
Bottomless does not invent10000 and the later ordinary writer obeys its incoming
flag. Tests verify camera manager advancement rather than unsupported camera
freeze. Numeric SDK/clock/viewport adapters, exact motion scheduler admission,
NPC event/partner services, original effects/audio and original traces remain
separate requirements. No Dolphin observation or full recovery equivalence is
claimed; the old camera-freeze inference below is corrected.

## 2026-10-12 — Ordinary camera event consumer and retained load state

Nativee278068 binds source targetB-before-road, percentage-dolly-after-road and
viewA-after-dolly ordering from camera/OVERWORLD_CAMERA_DOLLY_AND_EVENT_BLENDS.md.
Source preset stateA3 skips shift-main/road but posts afterward; flags2 skips the
whole callback. Strict timer crossing and source intpl built-ins0..13 preserve
ordered formulas/no generic clamp/approximate endpoints. Unknown callback modes
are unsupported, not linear fallbacks. Basic set/off target/view methods snapshot
the actual source logical inputs and use the native simulation-ms bridge. Source
raw OS ticks, pause gates and durationLow/timebase arithmetic remain unbound;
these ms ratios are numerical/time-service adaptations rather than parity.
Room loads retain Bstate/anchors/times/dolly, stored/preset/actual views and live
near/far, reset documented curves/progress/FOV25 and clearflag10. Actual original
EVT operand marshaling, story request execution, other camera producers and
SDK/Gekko view/projection parity remain required. Native tests prove service
ordering/state lifetime and ordinary traversal, not original scene equivalence.
