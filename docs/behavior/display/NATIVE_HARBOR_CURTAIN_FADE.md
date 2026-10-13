# Original harbor curtain fade provider

This bounded provider consumes original fadeEntry type23. It is separate from
the source [battle startup owner](../battle/HARBOR_SOURCE_SEQUENCE_STARTUP.md).
Resource admission and authored animation are real dependencies; a raw duration
parameter does not establish this fade's completion. Evidence is static US
disc0 revision0 inspection and isolated native component validation, not Dolphin
timing or New2DS hardware measurements.

## Source lifecycle

fadeEntry80013368 selects slot1 for type23, stores the supplied duration/color,
sets flags1 and step0. The requested harbor values are23/200/000000ff. The
type23 table at80304794 dispatches80013864, selecting source
OFF_d_maku_left_right/context2/A_2. fadeMain80012058's type23 table entry selects
80012a88. State0 polls animGroupBaseAsync80039d0c; false retains the wait. On
admission it releases an existing pose, enters the actual resource, sets material
flags1800, sets A_2/reset1, retains whiteFFFFFFFF and clears flag40. State0 falls
through to state1 in the same call.

State1 queries animPoseGetLoopTimes8004079c before animPoseMain8003ed2c. At
progress>=.5 it truncates each RGB byte multiplied by original float3f7ae148
(.98), retains alpha and publishes the material color/flag40. It then calls
animPoseMain and queries progress again. Only progress>=1 sets fade flags2,
increments the step, releases the pose and writes pose-1. Raw duration200 does
not control this branch. fadeIsFinish80010d94 returns true for last-slot-1 or
extracts the selected slot's flag2. Re-entering an active slot preserves its
old pose for source-order release at the next successful admission.

BattleCurtainFade keeps those callback and query orders. Pose resource/clock
services are supplied by the renderer. Missing source geometry/texture or a
failed evaluated sample cannot acknowledge completion. Tests cover indefinitely
blocked admission, zero duration, same-call fallthrough, half-progress color,
post-main completion, release and re-entry ownership.

## Actual native resources and clock

Ignored harbor-curtain.pmanim retains original a/OFF_d_maku_left_right A_2,
nonlooping span0..45,14 keys,216 triangles/648 bindings. The actual companion
a/OFF_d_maku_left_right- supplies one wrapped atlas. Shared PMANIM4 conversion
retains sparse geometry/hierarchy/UV/color/cull and normalizes source pass0 to
alpha-test pass2. The512x64 original atlas has UVscale8. PMCUR001 records the
UV scale,216 triangles and0/45 span; native admission checks all of these and
actually imports the GPU texture before reporting readiness.

The native animation provider uses elapsed simulation milliseconds multiplied
by.06, evaluates the actual A_2 sample and publishes its progress ratio. The
original animPoseMain time conversion/progress writer was inspected; this is an
explicit simulation-clock adaptation, never a rendering-frame counter. It runs
once per published tutorial timestamp and releases the logical pose at progress1.
The texture remains resident until GPU-synchronized renderer teardown.

disp_maku800113bc uses original64x48 image-plane dimensions and608x480 source
viewport. Native orthographic mapping adapts that viewport to304x240:

```
X = 48 + (x + 32) * 304 / 64
Y = (24 - y) * 5
```

ActorAnimationPose image meshes retain original source units; they do not use
the character root's.01 world scaling. The material RGB multiplies retained
original vertex RGBA. Source camera/depth/material numerical parity remains
separate from this image-plane adapter. Queued curtain vertices append after
the three actors and use their own range, so earlier actor draws remain intact.
Startup bank admission happens after tutorial rooms have released field GPU
residency. Neither readiness nor a component-harness curtain request can publish
a battle result, source story advancement or field control release.

## Validation boundaries

verify_battle_curtain_fade.py pins six original provider hashes, both type23
dispatch entries, white/.98 constants and actual resource/pose strings read-only.
Original-pack host tests check source span/keys/bindings, evaluated vertex count
and authored movement. A temporary isolated component harness invokes only the
actual fade service after the existing tutorial intro; it is removed before the
normal build and is not evidence that source FirstAct currently drives the
curtain. Full UnitSetup/UnitEntry/sequence integration owns that next binding.
The first visual smoke exposed a100x image-unit error. Corrected isolated
component captures in artifacts/azahar-original-curtain-projection show the
original red pleated curtain moving off the image plane (sample129), followed
by the retained stage/actors (sample130). This verifies the bounded consumer,
not source FirstAct choreography, full fade/material/camera parity or a fight
result. The component invocation is removed from the normal source/build.
