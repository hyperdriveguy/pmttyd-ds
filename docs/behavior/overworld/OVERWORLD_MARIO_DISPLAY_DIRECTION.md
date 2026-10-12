# Original ordinary Mario retained facing and display turn

US G8ME01 revision0, fresh initialization, marioCheckWall direction metadata,
marioMakeDispDir800583a8 ordinary target0/180 turn branches and their
pre-display caller. This extends [pose selection](OVERWORLD_MARIO_ORDINARY_POSE_SELECTION.md).
Read-only source and finite derived math; no original execution/numeric parity.

## Initialization and distinct angle owners

marioInit8005b5c8 allocates0x2f8 and zeroes the complete Mario work at
8005b5e4–600. Its remaining body makes no later writes to1a0/1ac/1b0: fresh
**retained facing1a0=0, current display direction1ac=0, display target1b0=0**.
This is fresh initialization, not an inferred room/save/road-entry reset.
marioReInit saves1ac/1b0/1a0 at8005b258/25c/264 and restores at8005b2fc/300/308.
The source retained camera-heading offset19c and authored entry/event services
are separate inputs; preserve their actual lifetime.

| Field | Ordinary owner/meaning |
| --- | --- |
| 198 | Current input heading, used for input/movement requests |
| 19c | Camera/input heading offset |
| 1a0 | Retained facing from wall-wrapper direction metadata |
| 1a4 | Retained travel heading |
| 1ac | Current display turn value, can be negative or >180 in transitions |
| 1b0 | Retained display turn target |
| 180 | Retained base movement speed, not combined air displacement norm |
| 0c bits100/200/400 | Turn threshold/left/right markers in the motion flags word |

Rear pose selection uses revise360(1a0−19c) **before** display-direction update.
Draw geometry uses revise360(1ac−19c) and the existing source Z reflection.
None of1a0/1ac/1b0 is an unconditional current-stick/world-travel replacement.

## Ordinary retained facing producer

marioMoveMain calls marioCheckWall with requested displacement norm S and
computed heading H at800599e4. These are the actual pre-collision f31/f29
arguments (800599dc/e0), after the following plain preparation:

1. Combine retained base180 at travel1a4 with current-input airborne drift;
   apply support attribute modifiers and the existing per-axis cosine slope
   gates at80059794–804. External174/17c deltas then add to X/Z;178 adds rootY
   (excluded from ordinary zero-external domain).
2. Compute norm of that combined X/Z (80059824–984). Nonzero norm chooses
   H=angleABf(0,0,combinedX,combinedZ) at8005990c–924. Zero norm chooses
   toMovedir(display1ac) at8005992c–930; zero S does not update facing metadata.
3. Apply the admitted airborne norm cap at8005993c–960.
4. Unless main01000000, multiply norm by marioGetMoveRate(**prior facing1a0**)
   at8005996c–974. Instruction8005996c is c03e01a0: it does **not** load1a4.
   Facing1a0 is read before this callback's later wall-wrapper write.
5. Store final requested norm18c at80059988, then call CheckWall(S=norm,H).

Rate does not change H. S is the final requested magnitude after slope/cap/rate,
not base180, raw stick magnitude, actual accepted displacement, or slide output.
For unchanged norm, direction metadata's14-count hold applies even if current
H changes. S gains/slows from support/rate may cause a strict larger-S update.
The preceding facing also feeds direction gain, so replacing it with current
travel/input would alter S and can alter subsequent facing arbitration.
See the [corrected movement-rate input](OVERWORLD_DIRECTION_MOVEMENT_RATE.md)
and [slope composition](OVERWORLD_SUPPORT_SLOPES_AND_STEPS.md). Native trig/norm
math and engine*.01 scaling remain numerical adaptations; use the actual native
wall-request inputs in equivalent engine units when binding this policy.

At8008b7d8–82c, the wrapper updates1a0 only when
S!=0 and signed air counter50==0:

    if S > retained1cc:
        retained1cc = S
        retained1d0 = H
        count1d4 = 14
    else:
        count1d4 -= 1
        if count1d4 <= 0:
            count1d4 = 0
            retained1cc = S
            retained1d0 = H
    facing1a0 = retained1d0

The first comparison is strict; equal magnitude follows countdown, so an
instant same-speed reversal need not immediately change facing. S0 or air
counter!=0 resets1cc=0 but **retains1a0**,1d0 and countdown. This metadata occurs
before the wall ray/pass response; H is the requested heading, not slide output.
Native source wall-policy metadata can supply the actual retained facing.

marioMoveMain's transformation/manual branches additionally write1a0=1a4
(80059564/5e8); those are separate domains. Ordinary airborne travel generally
retains takeoff facing even if drift changes the combined ray heading.
Landing clears the air counter through its own owner; next moving grounded
wrapper can then replace the zeroed1cc and heading. Do not reset facing to0 on
jump/fall entry or neutral input unless another source writer requires it.

## Target selection and admission

marioPreDisp calls marioMakeDispDir at800578d0 when retained byte38==0 and
Mario display100 is clear. MakeDispDir itself returns immediately when display
01000000 is set (800583c0–c4). With display100 clear, normal target selection
runs800583d0–584d0. With display100 set on a direct call, it skips target
selection and uses retained target. Ordinary domain here has both clear,
display200 clear, and motion!=18 (hammer's separate instantaneous branch).

Let H=revise360(facing1a0−offset19c), T=trunc(target1b0+.5). Target rounding is
an actual fctiwz of a+.5, not round-to-nearest-even or angle normalization.
Source special retained T290 or250 changes to0 when H>=180 at80058424–44.
Those T values are event/special-history inputs; ordinary fresh0/180 never
manufactures them. With retained **base speed180!=0**:

- H in inclusive**190..350** sets T=0 (80058460–80).
- Otherwise H in inclusive**10..170** sets T=180 (80058488–a8).
- Outside those ranges, preserve rounded T; neutral speed also preserves it.

Source high limit350 is word43af0000 at80420074, loaded at80058470.
**250 is the distinct prior-target special-case literal**, word437a0000 at
8042006c loaded80058430. They must not be interchanged.
Store T to1b0 at800584ac. Display200 would copy T directly to current1ac at
800584bc–c0; that force path is excluded from the plain consumer. If current
D=1ac equals T, return immediately800584c4–d0, preserving turn marker bits.

Fresh idle with facing0/offset0/speed0/D0/T0 stays D0/T0. Rear selection chooses
M_S_1R. For nonzero headings, the range rules choose left/right display target;
they are independent of the wider rear-selection sector0..70/290..360.

## Complete bounded ordinary target0/180 turn machine

The following pseudocode preserves branch ordering for finite ordinary D,
T exactly0/180 and motion!=18. C is the retained0c motion flag word; other
bits are preserved. **Compare initialD for the lower/upper branch**, then update
D. There is no degree wrap during this update; draw normalization is separate.

    initialD = D
    if D == T: return (D, T, C)

    if T == 0:
        if C & 0x400:
            C &= ~0x400
            C &= ~0x100
        C |= 0x200
        if C & 0x100:
            D += sourcePoint3 * (10 - D)
        else:
            D += sourcePoint125 * (-30 - D)
        if C & 0x100:
            if D >= 0:
                D = 0
                C &= ~0x100
        elif D < -20:
            C |= 0x100

    elif T == 180:
        if C & 0x200:
            C &= ~0x200
            C &= ~0x100
        C |= 0x400
        if C & 0x100:
            D += sourcePoint3 * (170 - D)
        else:
            D += sourcePoint125 * (210 - D)

        if initialD <= 180:
            if C & 0x100:
                if D <= 180:
                    D = 180
                    C &= ~0x100
            elif D > 200:
                C |= 0x100
        else:
            if not (C & 0x100):
                if D > 200:
                    C |= 0x100
                elif D <= 180:
                    D = 180
                    C &= ~0x100
            # Existing0x100 deliberately skips ALL post-update clamping here.

    # Other retained T values skip ordinary target0/180 turn updates.

Point3 is **word3e99999a=.30000001192092896**,80420080; point125 is
word3e000000=.125,80420084. These are per-callback approaches, not a dt-scaled
or invented interpolation rate. Source multiplies the delta and then adds,
not an assumed fused SDK/vector operation. The source literal goals are:

| Goal/threshold | Constant word/address | Representative load |
| --- | --- | --- |
| T0 slow goal−30 | 41f00000=+30 at80420048, then fneg | 80058614–1c |
| T0 threshold-stage goal10 | 41200000 at80420078 | 800585fc |
| T0 strict threshold−20 | 41a00000=+20 at80420088, then fneg | 80058644–4c |
| T180 slow goal210 | 43520000 at8042008c | 800586e4 |
| T180 threshold-stage goal170 | 432a0000 at8042007c | 800586cc |
| T180 strict threshold200 | 43480000 at80420090 | 80058714 |

T0 lower-sector body800585c4–688 and upper8005876c–830 repeat the same
finite turn formula/threshold gates. T180 lower body80058694–750 differs from
upper8005883c–8f0: **upper initialD>180 with existing100 branches straight
end at800588b4→8f4**. Consequently it can cross to D<180 while retaining100;
the next callback's lower branch performs the clamp. Do not erase that
intermediate frame by clamping every target crossing immediately.

Turning markers200/400 survive final clamps;100 clears. A changed target
clears the opposing direction marker and100 before calculating that callback's
delta. At D==T no such clearing runs. This is why a stateless shortest-angle
or immediately mirrored quad differs from the source retained actor state.

## Numeric/service boundary and verification

Run the pose selector verifier. It pins init/reinit/movement/wall/turn bodies,
all referenced float words and the load instruction→r2 address derivation.
Finite derived cases cover exact190/350/10/170 edges,14-callback retained
heading,neutral/air retention,left/right turn markers and the upper-sector
post-clamp asymmetry. Host-derived arithmetic is a numerical adaptation; no
source float/Gekko execution or original-game display capture is claimed.

Other initialization/road/event direction setters, special target values,
pose/main/display locks, hammer motion18 and transforms are not a license to
infer ordinary angles from a save's unrelated facing flag. Preserve available
source fields or state the native adapter dependency explicitly.
