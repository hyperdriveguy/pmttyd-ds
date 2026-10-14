# Portable camera, display queue and shadow services

[DisplayQueue](../../source/display/display_queue.cpp),
[CameraManager](../../source/display/camera_manager.cpp),
[letterbox](../../source/display/camera_letterbox.cpp) and
[ShadowWork](../../source/display/shadow_work.cpp) retain independent state.
Their provider/cursor protocols describe actual implemented ownership; successful
registration or scalar publication does not certify native GPU drawing.
Original evidence is in the [display queue contract](../behavior/display/DISPLAY_QUEUE_AND_DISPATCH.md)
and [camera bank contract](../behavior/camera/NATIVE_CAMERA_BANK_AND_DEFERRED_DRAW_OWNER.md).

## Queue numeric protocol

Records are16 bytes: byte camera/class, halfword padding, float key,32-bit
callback and userdata identities. Native index references adapt original pointers;
userdata remains borrowed. Its owner must survive deferred dispatch. The1024-record
queue and1024 sort references are distinct arrays. Reinitialization clears count
only, retaining stored records and current pointer. `ordered()` bounds active
references while `record()` exposes raw pool slots.

| Class | Sort band | Priority contribution |
| --- | --- | --- |
| 0,1 | 400000 | Subtract |
| 2 | 600000 | Add |
| 3,4 | 100000 | Subtract |
| 5 | 300000 | Add |
| 6 | 900000 | Subtract |
| 7 | 1100000 | Subtract |
| 8 | 1000000 | Subtract |
| 9 | 500000 | Add |
| 10 | 700000 | Add |

Native admission rejects NaN priorities and unavailable capacity/classes; camera
and admitted class narrow to source bytes. Infinity is not changed to a finite
value. Source sorting follows its pair, median and sampled-pivot branches:2
exchanges directly,3 returns after median work,4 skips median work, sizes below40
use median-of-three and40+ use the source sampled pivot. Tie behavior is a result
of those exact swaps. Replacing it with `std::sort` or `stable_sort` changes that
contract. Nominal bands do not prohibit arbitrary priorities crossing bands.

Dispatch filters the sorted records for one camera. Common state requests enable
color writes, suppress alpha writes for cameras4/5/7, and set Z scale/offset1/0.
Classes0..5/9/10 also publish the original blend/depth/alpha tuples;6/7/8 inherit
class-specific state. GX numbers remain source protocol values, not PICA enums.
The [exact state table](../behavior/display/DISPLAY_QUEUE_AND_DISPATCH.md) preserves
ordered operands, including reference128, GEQUAL6, ALWAYS7, LEQUAL3 and alpha OR1.

A cursor retains the failed state stage or captured callback/userdata. Successful
earlier state writes are not repeated; a failed provider owns any internal partial
work it must resume. A dispatch cursor cannot change cameras midway. Current
record publishes only after its setup states complete and before callback entry.

## Camera bank, publication and draw

The thirteen source slots are0 off,1 off2,2 bg,3 shadow,4 3d,5 3deff_A,6 3dimg,
7 3deff_B,8 2d,9 fade,10 fade2,11 dbg and12 dbg3d. Boot uses source FOV25 degrees,
near1/far32768 and aspect608/480. Caller dimensions set viewport/scissor; they
do not replace original authored projection bounds. The half-FOV tangent input
.21816612780094147 radians positions selected UI/image cameras at240/tan or
400/tan. Slots0..3/11 are orthographic; others perspective.

| Slots | Source override |
| --- | --- |
| 5,7,8 | Near100/far10000 |
| 6,9,10 | Aspect4/3 and eyeZ400/tan(half FOV) |
| 8 | EyeZ240/tan(half FOV) |
| 5,7 | Viewport depth interval [0.2,0.3] |
| 8 | Depth [0.1,0.2] |
| 9,10 | Depth [0,0.1] |

Other slots retain depth [0,1].
Coordinates/lens units belong to the supplied owner. Native world scale and
engine-unit providers must convert consistently at their boundary.

Main visits every slot, including draw-disabled ones, in stages callback,
LookAt+roll/translation, field-only camera4 zoom, then one-use projection extra.
Callback identity is captured once. Missing providers retain stage/stores rather
than republishing a new baseline. Flag8 requests extra projection multiplication
and clears only after consumption. Roll and translation precede extra; field zoom
must run before extra rather than rebuilding the projection afterwards.

Draw runs letterbox and sorting once, then visits cameras0..12 in ascending order.
Flag1 skips drawing; current index publishes before viewport/scissor. Flag0x40
with perspective kind0 admits near/far refresh; the branch is captured before
calling a suspending provider. Projection publication, actual queue dispatch and
original dummy-draw provider follow. Queue reset happens after every admitted
camera completes. The current camera index survives reset/holds and is independent
of Main. Viewport arrays are `(x,y,width,height,minZ,maxZ)`; scissor is
`(x,y,width,height)`. Source draw-camera scalar8 is separate from record index.

## Matrix and depth arithmetic

[camera_depth.cpp](../../source/display/camera_depth.cpp) uses row-major affine
3x4 views and homogeneous4x4 projections. Explicit binary32 store boundaries
retain recovered operation order; native double sin/cos/tan and normalization
remain declared numeric adapters. This does not prove Gekko/libm bit equality.
Projection divides by row3 W; no guessed zero-W clamp is inserted. Queue depth
maps projected Z through `5000*z+5000`, clamps0..10000 and negates. Unprojected
view Z is not interchangeable with this value. Source GX near/far terms differ
from the usual OpenGL projection matrix.

## Letterbox and shadow continuations

Letterbox captures party-slot0 once but performs fresh source queries at their
required stages. Null party token is successful absence; `nullopt` is unavailable.
`pending` records the original service address, separately from the native stage.
Camera flags0x200/Bero mask1 bypass automatic decisions;0x100 enables bars;
0x400 requests immediate final scalars. Original sequence2 admits key/control/
party checks while3 can preserve disabling state. These are original sequence
IDs, not `Game.fieldSequence`'s unrelated native adaptation.

Enabled bars grow signed16 by3 per invocation to top49/bottom44, with alpha255.
Disabled alpha subtracts `2*trunc(255/rate)` then narrows signed16 before flooring0;
rate0 holds. The sound owner independently receives mask0x20. Every completed
visit registers class3/callback0x8000e564, even when alpha0, so later drawing has
its actual borrowed request. [Letterbox scope](../behavior/camera/NATIVE_LETTERBOX_SCALAR_CONTINUATION.md).

The64-record shadow pool retains0x5c-byte records, including matrix at0x2c and
alpha at0x28. Entry sets collision-1,camera4,scale1,inner/outer.2/.4,alpha255.
Matrix/image/collision preparation is separate from Entry. Boot request settings
include type2,height20 and original RGBA color0x0000006e (black,alpha110), distinct
from C2D's packed color order. Main registers eight ordered callback0x8002d3d8
requests; step4(camera2/class5) is preparation-gated. Only after registration
finishes does Main clear active count/flag1/counters and auxiliary flags. Raw pool
records remain; count reset does not physically zero previously submitted data.
[Shadow scope](../behavior/display/NATIVE_SHADOW_WORK_AND_TRIO_LIVE_DISPLAY.md).

Effect deletion likewise separates request from retirement: flags at+0,payload
at+0xc,heap3. Hard deletion frees then clears flags; soft deletion with flag2
sets pending-delete4 and retains ownership. A soft request is not evidence that
the eventual update owner already freed its payload.

## Authored field camera roads

[CameraRoadAsset](../../source/display/camera_road_assets.cpp) keeps the original
big-endian v1.01 container, bounded to2MiB. Minimum header0x10c holds size at0,
three64-byte strings at4/0x44/0x84, nine counts at0xc4 and offsets at0xe8. Collection
strides12/184/104/8/12/12/4/104/unknown identify lens,road,zone,polygon,path,
zone vertices,indices and extra data. Unknown nonempty collection8 is rejected;
zero-count sentinel pointers are preserved. Six room files reload atomically.

Road+a0/a4 is path start/count,+a8/ac zone start/count,+b0/b4 extra start/count.
Zone+58 is vertex base,+60/64 polygon start/count; polygon entries index the
index-array. The supported straight solver requires six points and six rows,
endpoint indices2/3,+84 selector1 and no extras. Left/right caps+48/+4c and near
normal/range tolerances remain original values. Result mask1 denotes endpoint
cap,2 side cap. Infinite-line projection is an explicit host numerical adapter;
clamping its fraction to a segment would change the implemented behavior.

Zone selection uses look/contact target, not follow/player. Triangle equality
can replace best height; empty-zone fallback requires strict greater height.
Road+28 disables a v1.01 zone; +8c supplies empty-zone fallback height. Geometry
is authored engine/10 scale, whereas `CameraRoadWork::update` accepts engine
coordinates and divides by10. Road frame publication later multiplies by.1 to
native world scale. Distinct binary32 pi constants serve easing, wrap and arc
cost; do not unify them to a higher-precision value.

Current/candidate/destination/origin remain distinct. Re-targeting a transition
starts from displayed current state. Comparison masks1/2/4/8/0x20/0x10 identify
road,zone-exit,endpoint-entry,segment jump,near-wrap and numeric-only changes.
Rate is reciprocal cost clamped20..90, accumulated using60Hz intervals from
integer milliseconds. Near-zero cost<.01/travel<.3 admits no transition.
Shift solving preserves640/480 lens terms, tangent/normal budgets and per-visit
weights+.004/+.008. Missing paths/math leave work unchanged rather than publishing
a fallback clamp. [Road comparison](../behavior/camera/FIELD_CAMERA_ROAD_COMPARISON_AND_TRANSITION_RATE.md)
and [interpolation](../behavior/camera/FIELD_CAMERA_ROAD_INTERPOLATION_AND_CURRENT_STATE.md).
