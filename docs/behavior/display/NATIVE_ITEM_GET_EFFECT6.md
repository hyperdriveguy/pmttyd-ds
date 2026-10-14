# Native item-get type6 effect provider

Finite original type6/count1 source:
[GX state and projected texture](../effect/FIELD_ITEM_GET_TYPE6_GX_STATE_AND_PROJECTED_TEXTURE.md),
[bank/clock/model](../effect/FIELD_ITEM_GET_TYPE6_BANK_UPDATE_AND_DRAW.md).
This consumes original resources; native gameplay acquisition remains pending.

Portable `FieldItemGetEffect6::draw` now supplies the source texture projection
LightOrtho * synthetic LookAt * unshifted world model. It excludes camera view
and the shadow-only local offset. Texture42 keeps direct quad UV. Draw requests
remain clock-free. The corrected `camera()` field defaults4, receives5 from the
item caller; the original display class is2. Earlier “layer” terminology did not
identify that dispatch owner correctly.

`Renderer::itemGetEffect` requires an explicit matching camera ID, supplied
native world-to-clip matrix, source viewport depth and camera yaw. It refuses mismatched IDs rather
than silently using Mario's yaw or another camera. The scene projection remains
caller-owned; texture LightOrtho never substitutes for it. Native world-unit
conversion(.01) occurs once before source model composition. `itemGetCamera5Frame` supplies the finite camera5 producer from explicit copied
camera4 eye/up/target, retaining camera5 lens25, near1/far100 in native units,
and viewport depth.2/.3. Original up is not inferred from camera4's final view;
its own roll/view-offset defaults are zero in this finite scope. See
[camera5 producer](../camera/FIELD_ITEM_GET_CAMERA5_PRODUCER_AND_PROJECTION.md).
Renderer depth mapping uses the supplied viewport endpoints and restores the
normal native depth domain after drawing. Native camera4 prepared-vector supply,
source camera/display scheduler and camera4 depth-sort remain integration work.

Dedicated vertex shader computes position via supplied projection/view/model
and image44ST via two separate texture-matrix row dot products. Tex3DS subtexture
orientation is applied to the texture matrix/UVs. Image44 uses repeat, image42
mirror repeat; both linear, no mipmaps. Resource admission verifies single,
unrotated, full-size16x16/128x128 textures so wrapping cannot include atlas pads.
Original source images stay grayscale; no orange tint or opacity is baked in.
Image43 is exported but the count1 draw has no particle quads using it.

Two original GX stages map to three PICA stages because each native stage owns
one constant: initialize gray225, interpolate toward original orangeeb7d06
using projected image44RGB, multiply by pass rasterRGB. Alpha starts with
image42alpha, survives interpolation, then multiplies pass rasteralpha. Shadow
is submitted first with source RGB provider/alpha128*alpha/255 and shifted
geometry, main second with white/sourcealpha. Source pass0 startup RGB is zero;
its mutable producer remains an explicit provider. No alpha rejection, framebuffer-alpha updates or depth
writes; source LEQUAL maps to GEQUAL in the native reversed perspective depth
domain. This mapping follows the [Citro3D perspective matrix](https://github.com/devkitPro/citro3d/blob/master/source/maths/mtx_persptilt.c) and [default depth map](https://github.com/devkitPro/citro3d/blob/master/source/base.c); it is a backend adaptation. Straight sourcealpha/inversesourcealpha blending is retained.

Lazy resources:65KiB decoded original RGBA8 textures,12 local vertices(432bytes)
and one shader, with no per-frame file reads/texture uploads. Shader matrix,
texture projection and color work use C3D/PICA. Explicit inactive draw releases
resources after synchronization; shutdown also releases them. The finite arena
supports one count1 submission per frame; global display-queue/multiple-owner
allocation remains a separate extension. C2D state is restored for later UI.

Portable tests cover camera4->5, world/model yaw, texture phase at0/90/240deg,
local projected scale, shadow-only position offset and source clock/fade.
Native preview checks resource admission and visible draw/teardown, not original
Dolphin GX pixel parity. SDK single/paired rounding, normalization, texture
filtering/fixed-point combiner precision and hardware profiling remain unproven.
Remaining acquisition providers: source camera/display scheduling, original
icon307 at item root+50, audio/control/name registry and failure/discard service.
