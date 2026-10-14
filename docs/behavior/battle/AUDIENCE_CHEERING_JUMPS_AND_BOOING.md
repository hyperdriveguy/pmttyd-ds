# Audience cheering, celebratory jumps and booing

US G8ME01 revision0, read-only DOL reconstruction, not Dolphin measurement.
Whole local bodies in
[`battle_audience_actor_control.cpp`](../../../source/battle/battle_audience_actor_control.cpp),
called through the existing native member-state provider:

| Function | Address / bytes | SHA256 |
| --- | --- | --- |
| JoyRaiseArm | 80191e18 / 108 | 4675b07937b1f9f7976ce36a9694eae6ffb37947bf5805fc0375748098d6e82e |
| JoyJump | 80191cec / 12c | d8966019b4a60e3b02292ca5a03811828ec2bd74f111b3e1e659a58be4f05f92 |
| JoyJump2 | 80191bb0 / 13c | a833e2bf8069458a93e91afe34e8e6fc8d2411af474a5a20c9e9c30345810864 |
| Booing | 80191884 / fc | bb61a48814060d6a74fc5edc6784ee4be1c6dbf409bef1a9644397d965a45e67 |

Each captures its member pointer before linked RNG/interpolation. Missing linked
providers preserve preceding source stores. Unsupported function addresses
return false without stores; they are not successful reaction no-ops.

## Raising arms and booing

JoyRaiseArm selects animation4 only when front low-byte==0. A nonzero byte skips
that animation call but still runs its phase. Booing always selects animation4;
its remaining local logic is identical. Member phase word4 must be0 or1; other signed values
return after the animation decision.

Phase0 calls irand(2) once. Result0 writes facinge8=270, any nonzero writes90.
It increments phase4, then falls through to phase1. Phase1 calls original
intplGetValue(4, signed timer8, 15, source0, source10), stores its result at70,
then increments timer8 with32-bit wrap. The native binding reuses existing
[cameraInterpolation/intplGetValue](../camera/OVERWORLD_CAMERA_INTERPOLATION_AND_TIMER_DOMAIN.md),
whose mode4 table entry8030435c is80006e90. It is the original unclamped
quadratic return expression, not a linear or sine replacement. Ordinary
steps0/15/30 produce0/10/0; negative/oversized signed steps are not clamped.
Full helper80006c8c/3b0 SHA256:
0d800da046dc09ba1fe07a6933b4a910553c1fbe7ea2f6a874395018f4b6a4ae.
Existing helper SDK/libm/FPSCR numeric limitations remain unchanged.

Only when the incremented timer is signed>30, subtract source1 from float0c.
The decremented value<=source0 calls whole ChangeStatus0. Otherwise timer8=0
starts another cycle at phase1, with no further random facing selection. An
unordered float0c fails the ordered <= and takes the timer-reset branch. The
source neither converts float0c to an integer nor clamps it. In an ordinary
state4 entered through ChangeStatus, float0c starts0 and completes one31-call
cycle. Fractional authored counts can span additional cycles.

A held interpolation after phase0 retains the selected animation, facing and
phase1. Retrying the body therefore does not repeat irand. It does not advance
timer8 or write70 until the interpolation service supplies a result.

## Celebratory jumps

Both jump bodies choose animation2 when front low-byte==1, otherwise3.
Valid phase word4 values are0/1/2; other values return after animation selection. Phase0
sets count8 to1 for JoyJump or2 for JoyJump2, increments phase and falls through.
Phase1 calls raw rand8026c324 exactly once, converts the signed return to single,
divides by original32767 (word46fffe00 at80424554), then separately adds it to
source3.4 for JoyJump or source4 for JoyJump2. It writes vertical velocitydc,
source acceleration-.2 to e4, increments phase and falls through to phase2.
Raw32767 is an included endpoint. No normalized random helper, range clamp,
extra draw or fused add/divide substitutes for these operations.

JoyJump2 adds source20 (word41a00000 at8042453c) to anglea0 every phase2 call,
including fallthrough from the launch and the call that observes landing.
There is no angle normalization in this function. JoyJump leaves a0 alone.

Only acceleratione4 exactly equal to source0 decrements count8 with32-bit wrap.
Signed count<=0 calls whole ChangeStatus0; otherwise phase4=1 prepares the next
launch. Unordered acceleration does not satisfy this test. Landing clearance
belongs to the dispatcher's later negative-height reset, so a landing detected
by motion is consumed on a following controller call. Missing raw RNG retains
phase0's count/phase1 stores, with no launch velocity or angle advancement.

## Native ownership, tests and limits

Native callbacks use the runtime's existing shared BattleRandom for both bounded
and raw draws, and existing cameraInterpolation for the original interpolation
request. No new RNG seed producer, frame clock or render-driven update is added.
The same existing simulation provider calls these functions for states4/5/6/9.

Tests cover differing animation admission, phase0 fallthrough, every arm curve
step0..30, fractional repeat cycles, unordered repeat count, signed timer wrap,
missing-provider partial stores and retry without an extra draw. Raw random
endpoints, animation low-byte selection, invalid phases, unnormalized angle370,
and one/two complete jumps through the actual member motion/landing tail are
covered. A supplied test seed27120 produces an actual source Idle decision20;
the next draw selects arm facing. The combined dispatcher runs back to idle and
its RNG state equals an independent two-draw oracle. This seed is a test input,
not a claim about original new-game seed production.

Existing Python pack tests pin the additional constants. Native compilation and
host tests prove the implemented control boundary; they do not prove full native
outer Main/camera cadence or accepted battle framing. ItemOn/Throw7/8,
Damage/Enter/Escape10..12 and special states13..20 remain providers, along with
idle's missing low-ratio caller-register provenance. Harmful-item telegraphs,
physical X plus optional audience taps, original menu/turn choreography and full
story/tutorial/field return still require their original owners. No dialogue,
menu/HUD asset, action-command timing or two-screen replay layout is changed.
