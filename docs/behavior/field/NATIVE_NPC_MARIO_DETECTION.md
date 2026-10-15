# Retained original NPC Mario avoidance and proximity

The [Main binding](NATIVE_NPC_MAIN_ENTRY_AND_CLOCK_COMPOSITION.md) now composes
[cloud/territory detection](NATIVE_NPC_CLOUD_AND_TERRITORY_DETECTION.md) with
complete source8004a4e4..8004accc. Captured event class comes from the earlier
actual event byte10. This completes the local detection graph, not npcMain:
success reaches unfinished Display before its fresh mainflags load8004accc.
Actor traversal, final state10 clear, display/shadow and history remain mandatory.
The [whole parent map](ORIGINAL_NPC_MAIN_PARENT_CONTROL_FLOW.md) and
[child ABI audit](ORIGINAL_NPC_MAIN_CHILD_ABI_AND_NUMERIC_FRONTIER.md) remain
authoritative.

## Class2 avoidance

Request marioGetPtr; capture MX,AX,MY,AY,MZ,AZ in source order and form single
Mario-minus-NPC deltas. Perform PSVECSubtract(previous,current,localDelta),
including unused Y: paired previousXY, paired currentXY, localXY subtraction,
fresh previousZ/currentZ then localZ subtraction. Read state1d4, then yaw144
unconditionally. State2 computes separate single squares/add for horizontal
previous-minus-current movement and the shared source norm. Fresh .05 literal
8041fd54 is strict: only norm>.05 replaces facing with fresh heading1ac.

Now request Mario AGAIN for eligibility. Retain this returned pointer, capture
flags once, reject82000000 or a; read fresh GP pointer and GP164 (must0), request
seqGetSeq (must2), read the retained Mario half2e (must not25). Request Vivian
status: first0 admits, otherwise a SECOND distinct request must return1. Do not
cache status, reread Mario after sequence, or merge these calls with earlier
position capture. Failed eligibility posts trigger2.

Only after eligibility, compute player horizontal norm as fused32(dx*dx+
rounded32(dz*dz)), with source zero loaded between squared-Z and fused addition.
Load quarter8041fd58 then range228. Only distance>single(.25*range) runs extra
checks: dy>fresh20c, -dy>fresh218, distance>CAPTURED range228, then angleABf
(0,0,dx,dz), compAngle(retainedFacing,angle), fresh.5, absolute difference,
fresh anglewidth22c. Strict abs>single(.5*width) fails. NaN comparisons do not
fail; unordered quarter comparison skips ALL extra checks, including heights.
Success reaches the class1/4 gate and, because captured class is2, goes directly
to Interrupt. It does not run proximity. Failure posts2 then Interrupt.

## Class1/4 proximity

Other classes go directly to Interrupt. For1/4, request Mario and capture
MZ,AZ,MX,AX,MY,AY in this distinct order. Compute fused horizontal norm BEFORE
eligibility. Fresh mainflags08000000 captures all FOUR source table words
802c1210..121c BEFORE requesting kpaGetLevel. Copied radii are0,0,30,100.
Select using the retained returned level; then load fresh zero, subtract radius
in single precision and clamp ONLY adjusted<zero. NaN is retained. Original
index is unchecked; native unsupported indices outside0..3 hold, never clamp
or fabricate neighboring stack values.

Perform the otherwise unused PSVECSubtract(previous,current) with every source
read. Fresh yaw144 goes to reviseAngle; load fresh180 at8041fccc. A strict
revised<180 selects fresh90 at8041fcb0; otherwise fresh270 at8041fcc4 (including
unordered). Retain this literal facing. Perform the separate fresh Mario/
flags/GP/sequence/half/Vivian eligibility graph described above. Failure suppresses
trigger1.

Load quarter then range220. Only adjustedDistance>single(.25*range) checks full
CAPTURED range220 and the angle graph using fresh width224. Quarter bypass skips
range/angle only: EVERY admitted distance still checks dy20c and -dy218.
Class1 success posts1. Class4 then applies inner territory:

- Fresh kind1f8==1: capture centerZ,currentZ,centerX,currentX,sourcezero; fused
  squared norm and shared norm. Load DOUBLE .75 at802c1460 THEN fresh radius214.
  Compare promoted float norm to the DOUBLE product, without a float threshold
  round. Only norm>product rejects.
- All other kinds, including0/unknown: load FLOAT .75 at8041fcd4, extent214,
  center1fc, multiply in single, then fresh currentX. Strict outside center±
  scaled extent rejects and skips Z reads. Otherwise load extent21c THEN
  center204, multiply using RETAINED float.75, then fresh currentZ. Strict
  outside rejects. NaN on one axis does not reject it or skip the other axis.

Every local norm preserves its caller's single/FMA graph before the shared
retained seed/three-double-Newton/classification helper. The existing opt-in
native seed remains an adaptation, not original Gekko seed/FPSCR/GQR/subnormal/
NaN-payload equivalence. Angle services are explicit; exceptional inputs must
not blindly enter finite-only native table helpers.

## Actual services, triggers and continuation

Native marioGetPtr uses actual mapped8041e900 and explicit Mario admission.
seqGetSeq reads actual mutable80415190; its ISO initial ffffffff and native Scene
are not substitutes for the live sequence owner. KpaGetLevel uses a fresh
actual Mario getter, byte3c: non2 returns0, otherwise dereference Mario298 and
work+4. Unmapped reachable storage holds. Real sequence producer mapping and
Mario/Kpa lifetime still belong to the production owners; test RAM is explicitly
a fixture. Vivian and angle/revision requests remain explicit providers through
the same VM, never fabricated success values in ordinary Game.

Posting1/2 freshly reads140, ORs the trigger and stores once. Then request
actual npcCheckInterrupt800440f8(actor) once, discard its return, and reach
Display. Earlier prefix paths already at Display do not run this section or
repeat Interrupt. Missing providers hold at the original request; successful
calls/stores do not replay. The same owner, actor, captured class and mappings
must survive the invocation. Parent remains at its unfinished body cursor;
MainCount/history/actor iteration are unchanged. Core advance allocates no memory;
ARM builds use the existing hard-float toolchain. Physical performance and
ordinary Area activation remain unproven until the rest of Main and real
resource/world services are connected.

## Evidence and validation

Original US helper bodies were inspected read-only; ignored evidence resides
under artifacts/npc-main-parent-evidence/*-mario-review.{elf,txt} and
mario-detection-review-notes.txt:
seqGetSeq8 bytes SHA256e43394e99ed3f624ccfe48876ce2c3946e0dd1da9dc95b20c0d0b4ee5b8431e8;
kpaGetLevel68 bytes SHA2567762b30cf5d18cc1b0273f857aa8a2945c33df6560f9f7657d3bb05418f17515;
vivianGetStatus144 bytes SHA256a660a1d2d3f50e3fba6c9c9d3ef887014500208b5279d89a412608db4fcb3f7b.
No original body, ISO or compiled pack is committed. Seven unchanged data spans
are pinned by field_npc_actor_assets.py; exact hashes/bytes are in ignored
artifacts/npc-mario-constants.json. Existing rights remain unchanged.

Tests specify an independent avoidance-rejection trace, suspension at every
primitive across admitted paths, flags/GP/sequence/half/Vivian gates, distinct
Vivian calls, captured Mario/range versus fresh bounds, source angle comparisons,
quarter-gate differences, exceptional coordinates/angles, heading selection,
Kpa table capture/index refusal, all inner kinds, and an explicit DOUBLE-versus-
FLOAT .75 boundary counterexample. Shared VM tests hold on missing sequence,
then hold Vivian after changing Mario root/sequence; neither earlier source
capture replays. Actual Kpa work+298/+4 and original copied radius100 clamp to0;
trigger/interrupt happen once with MainCount/parent unchanged. Fixture numeric/
Vivian/gravity/clock providers do not establish production behavior. See
PROGRESS for exact full/frozen/native validation and remaining rendering work.
