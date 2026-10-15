# Field motion, contact and camera adapters

This guide covers the ordinary movement/contact headers and their wall, NPC,
water, door and pipe helpers in `include/field/` and `source/field/`. The
[Game composition guide](game-composition.md) describes their caller order;
[display state](display-state.md) covers later GPU camera publication. These
adapters do not imply complete original field/event services.

## Unit boundaries

| Owner | Units / meaning |
| --- | --- |
| FieldMovement / FieldJump / FieldStep | Native distance = engine*.01 per simulation integration |
| Floor contact / RoadMotion / FieldDoorState | Native positions and distances; fixed simulation visit counts |
| FieldWall / NpcSide / NpcMapMove / NpcWander | Original engine positions/dimensions; ray adapter converts at geometry boundary |
| FieldObjectTriangle | Native geometry; owner key is separate from the hit token |
| FieldShadowRecord | Engine geometry; native ABGR color word, alpha high 8 |
| MarioCameraTarget / FieldCameraTarget / FieldCameraEvents / FieldCameraShift | Source engine-space state and declared clock adapters |
| FieldCameraFrame | Backend-native eye/lens distances; matrices/root must share one consistent space |
| FieldDoorParameterServices | Guest addresses and raw source words; no native pointer casts |

Simulation visits are not rendering frames. Source data, native admission limits
and native framing defaults are identified separately in the comments. Native
SDK/libm adapters do not prove original floating-point or hardware parity.

## Movement, ground contact and air motion

Ordinary rates 1/2.25 engine units become.01/.0225 native units per visit. Integer
stick axes use the original squared-radius dash test >3025 (=55 squared), before
magnitude clamping; 33/44 is exactly walk threshold. Float stick and retained air
heading are different input paths. Camera heading rotates travel, while the
heading-dependent rate compares prior facing against retained camera offset.
Air drift combines with retained base motion, slope attenuation runs, then the
combined vector is capped. Offscreen fall damping applies independently of the
control-disable gate that affects the onscreen branch.

Floor slope caches separate inclination from projected axis tilts. Ordinary
admission is strict<50 degrees; steps allow inclusive30 degrees and rise5..11
engine units. Support metadata rounds in engine units before converting back to
native. Its positive half-bias is distinct from wall distance's signed half-bias.
Native degenerate-geometry tolerances are not original collision constants.

Jump motion stores velocity, acceleration, jerk, jerk-rate separately. Entry calls
dispatch immediately, including countdown/counter update. Motion requests may
be late: landing/ceiling requests retain tuple/flags until their next dispatcher.
Jump apex changes to the fall tuple and integrates that new tuple on the same
visit. Removing the second integration changes the source transition. Tick
counters wrap as signed16; release shortening starts at tick 4. Geometry owners
provide actual landing/support; the equal-height fixture helper is separate.

Scripted jump is a bounded stationary/near-equal-height path. Its two half-flight
stages intentionally differ in Y/velocity update order. Parameter<=-1000 selects
20, and half-duration rounds with+.5. Unsupported travel/roll/30Hz paths cannot
be replaced with a generic parabola.

Idle early HP/breath work runs before ground commands; late sleep work runs only
when the idle callback survives. HP<=5 selects breathing, HP0 its distinct pose/
sound. Signed16 counters, repeated pose checks, rate-derived breath periods and
phase 1 fallthrough remain explicit. Requested pose/sound IDs are original service
identities, not replacements or native sample slots.

## Wall and NPC contact ordering

The ray broad phase uses a conservative native margin;an invalid segment goes
to the narrow phase rather than becoming a miss. The triangle helper writes
distance/normal only;its caller attaches token/attributes. Distance is parameter
t,so geometric-distance interpretation requires a unit direction.

Forward wall rays use heights26.640001/11/.1 in engine units; a nonzero retained
counter enables the third ray. The forward result chooses nearest distance.
The+45 side guard stops at first hit; only then does-45 run, retaining its LAST
result even if null. That side state controls both correction and contact-distance
publication. Side-only obstruction may stop movement without a forward token.
Do not sort or merge these queries into one nearest-hit operation.

Surrounding correction has two passes over2/8 directions. It saves the uncapped
first push, caps the applied first movement, then compares the second push against
that saved vector. Only an opposing or unordered second dot takes the half push.
Pass-through attribute/motion flags remain separate from geometric intersections.

NPC side contact tests vertical/radial overlap, rolls NPC XZ back to its previous
position, then tests again. Rollback persists even if that second test misses.
A contact correction must call Mario's wall provider afterwards. Original bit
patterns define polynomial sine/cosine reduction; replacing them with generic
trig changes response. `npcCompAngle` performs one +/-360 adjustment, not a wrap
loop. Narrow-body correction can scale a vector UP to length 2 when its threshold
is1.5; do not replace the threshold and target length with a conventional cap.

NPC map movement chooses the first admitted height probe, not the nearest hit
across all heights. Only the middle probe filters slope. Its first contact is
published before a second tentative-movement query; final root may come from the
second query while contact identity stays the first. nullopt means an unavailable/
unsupported provider; a contained null hit is an acknowledged miss.

Wander produces motion components; NPC-main consumes them. It does not move the
root itself. Duration selection is half+rand(half+1), then move duration keeps the
multiple of20, not remainder. Repeated readiness queries preserve source order.
Movement caps at20ms worth of speed even if dt is longer. Sound elapsed uses
milliseconds, and visual idle flips intentionally leave moveHeading unchanged.
The shared LCG returns0 without consuming RNG when upper bound0.

## Camera state and clocks

The no-asset FieldCameraFrame lens 42/near.1/far80 and its eye/aim offsets are
native fallback framing. Event camera lens 25/near1/far32768 belongs to source
engine units. Source viewport 608x480 differs from the inclusive600x535 Mario
screen-cache test. Invalid screen-to-int conversions report invalid; a bottomless
height probe cannot invent a height when projection does not terminate.

Camera target publication has ordered early returns: motion 29 before easing,
then display0x10000/motion 25 after easing. Display0x20000 updates depth in XZ
while retaining prior Y. Follow-camera 0x10 freezes Y only. Heading snapshot rounds
to.001 degree before wrapping; this does not share wall quantization units.

`FieldCameraShift` takes quantized milliseconds; reset deliberately preserves its
prior timestamp. `update` changes state and `post` publishes timestamp separately.
Its increment cap applies before elapsed-time scaling. A stationary root prevents
stick accumulation; flag 0x20 forces neutral return. Clock regression/wrap is not
silently interpreted as elapsed time.

Blend timer state 1 reverses/off, states2/3 advance, 0 inactive. Strict overshoot
clears state; exact endpoints stay active. Interpolation retains original pi
literals and expression grouping, does not clamp step, and returns B for count 0
before validating mode. Event progress uses truncated100000-step interpolation.
Candidate-copy prepare publishes nothing if timer/curve admission fails. Room load
resets only specified fields; it must not clear retained target clocks/anchors/
preset views. These clocks are simulation bridges, not original OS scheduling.

## Recovery and room-entry bridges

Water/void phases are state labels, not delays. Position step max(-.1, velocity)
is separate from derivative terminal cap-.18. Landing state 31 starts 49; entering
state 30 sets50 then decrements. Get-up sets12 and falls through to11 that visit.
Coincident void-return roots still take one-.08 Z step before countdown completion.
Damage/death/get-up results leave full event/audio/camera ownership to callers.

ROAD callbacks retain story-dependent custom owners rather than treating unknown
story as0. Only cardinal AUTO directions are admitted by the finite helper. Door
and pipe admission use exact original contact nodes plus common case gates, not
proximity circles. Case6 pipe uses signed mainY>30, case10 front contact; door
case9 needs A and its source motion/party admission.

PMDOOR1/2 and PMPIPE1 catalogs validate candidate records before replacing them.
Door mask/story/node restrictions are explicit import scope. FieldDoorState's
movement/waits are a fixed-tick bridge; they do not implement source door rotation,
lighting or NPC lifecycle. Pipe arrival offset+.20/-.40 adapts source+20/-40,
then delegates restore; it is not full mot_dokan/entry-clock/GPU residency parity.

Direct door-parameter selectors copy raw words in source read order. Unknown
unsigned selector>11 consumes without reading a record. Geometry selectors 4/5/
10/11 hold at their real provider. Destination operand reads precede each source
record read, so bulk-copying the result is not equivalent.

## Shadow and object geometry

Dynamic map-object bounds construct original box strips with factor 10 before
engine->native conversion. Center offsets are added after yaw; draw scale/pose
warps do not change this collision box. Hidden block names retain geometry identity
but do not become ordinary solid objects without their activation owner.

FieldShadowRecord uses native ABGR, while source ShadowWork uses RRGGBBAA. Ray
preparation uses raw hit normals, prepared flag 2 and independent source-space
rays. Air-shadow lists preserve original fan/strip winding and palette identities;
list planning is not permission to regenerate the artwork. Fence warp uses frozen
view-plane bounds and64x48 source corners, then maps deformation through the saved
basis. It must not repeatedly deform its own previous output.
