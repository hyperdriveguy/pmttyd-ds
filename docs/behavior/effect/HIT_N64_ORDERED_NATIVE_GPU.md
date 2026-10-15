# Original HitN64 ordered native GPU preparation and replay

This module implements a bounded native consumer of the complete original
[HitN64 draw helpers](HIT_N64_SOURCE_DRAW_AND_NATIVE_TEXTURE_OWNER.md).
It does not activate normal effect scheduling. `HitN64GpuCapture::resume` means
**source preparation completed**, never queued display callback completion.
The actual retained outer display owner must hold its original callback/context,
exclude interleaved callbacks while held, submit the captured batch successfully,
and only then acknowledge the callback. Source reads, SDK children and actual
texture construction happen at their original retained call sites.

## Original source material

The source bodies and read-only ISO hashes are those verified by
`tools/hit_n64_source_evidence.py`: `effHitDisp801c5d6c`,
`main_dl801c5b58`, vertex setup801d771c and tri2 FIFO801d76a0.
The 12 exact original stage setters are admitted in order, followed by both
`GXSetTevColor` register writes, the genuine `effGetTexObjN64(74)` query, both
texture binds, both generators and `GXSetCullMode(0)` (802b5c8c).
That last function is **Cull**, not NumIndStages.
Unknown requests are not acknowledged.

GX register number1 is register0, and number2 is register1. GX selectors C0=2,
C1=4, CPREV=0; alpha APREV=0, TEXA=4, A1=2. Thus the original stages are:

| Stage | RGB | Alpha |
| --- | --- | --- |
| 0 | texture0 RGB | texture0 alpha |
| 1 | register1 RGB × (1 − previous RGB) + register0 RGB × previous RGB | previous alpha × (1 − register1 alpha) + texture1 alpha × register1 alpha |

Register0 alpha is read/set by the original source but is not an input of that
second alpha expression. It must not be substituted for the subframe alpha in
register1. The source entry does not initialize root payload+2c; this backend
preserves the source draw read without inventing an initial alpha.

PICA stage0 replaces with texture0. Stage1 uses RGB INTERPOLATE with
(primaryColor, constant, previous), and alpha INTERPOLATE with
(texture1, previous, constant). Native primary color carries original register0;
the stage1 constant carries original register1. This reuses two PICA stages,
with no raster modulation: original NumChans(0) ignores indexed vertex colors.
Those original RGBA array bytes are nevertheless decoded and retained.
`C3D_TexEnvInit` supplies unit scale and the source color/alpha operand forms.
The mapping reproduces the equations; no Gekko/PICA fixed-point rounding or
original framebuffer parity has been demonstrated.

## Geometry, texture and ordering

The immutable source resource supplies all five original 56-byte vertex blocks.
Each source position/color/UV array must point at the same admitted frame, with
stride14, INDEX16 descriptors, S16 XYZ, RGBA8 and S16 UV with five fractional bits.
The atomic 18 U16 indices are the exact source two-triangle stream. Actual source
bytes must still equal the verified original vertex owner at the reached FIFO
read; changed aliases wait instead of silently drawing the immutable copy.

Each six-vertex packet retains the actual source view/model matrix and both
reached texture matrices. UV0 and UV1 are computed independently. Original
texture matrices30/33 use two-component input without third-coordinate coupling;
unsupported coupling waits. Tex3DS orientation is applied once to each resulting
T coordinate. No source UVs are clamped. The final frame's collapsed matrix33 is
retained as (0,1) in native coordinates. Actual texture74 is the unpadded 32×128
RGBA8 conversion of the original I4 image, with linear filtering and REPEAT on
both axes, bound to both samplers through `NativeHitN64Texture`.

Packets append in actual source callback order: all particles of pass0, then
pass1, then pass2. There is no per-particle or per-material regrouping.
The publication becomes available only after the complete original parent and
children finish, and while actual allocation/frame/context and texture-generation
checks still pass. Unavailable SDK/camera/texture children retain source work;
a replaced source service table cannot resume a held capture. A publication pins
its source resource and external lifetime owner. Failed/stale batches
are never partially published. Loss retires a batch permanently; a later reused
allocation or restored boolean cannot revive that publication. Starting a new
capture cannot replace a held bound source cursor without explicit discard.

## Native replay and lifetime

`NativeHitN64Gpu::prepare` imports the dedicated `hit_n64.v.pica` shader and
allocates a bounded linear vertex arena outside FrameBegin. It requires one
vertex program, the four output kinds (position/color/two UVs), and an actual
`transform` uniform. The supplied binary must be compiled from that shader;
output validation does not authenticate arbitrary supplied machine code.
`beginFrame` waits for prior GPU use with C3D_FrameSync before recycling the arena.
Multiple draws consume distinct arena ranges, so later effect callbacks cannot
overwrite vertices referenced by earlier submissions in the same frame.
Capacity exhaustion holds the whole callback. `release` also synchronizes before
freeing GPU storage and requires C3D to remain alive.

Replay preflights the complete batch, all matrices/UVs, imported texture ownership
and generation, arena capacity and the actual inherited raster owner. CPU position
conversion finishes and is checked before any flush or GPU mutation. Only then
are pending C2D vertices flushed, inherited raster applied, culling disabled as
requested by the source, shader/matrices/attributes/textures/TEV applied, and all
ordered triangles submitted. Host prepare/preflight/draw remain unavailable;
host fixtures cannot manufacture native GPU submission success.

The parent supplies an actual source-view-to-native-clip projection and actual
inherited viewport/scissor/depth mapping/blend/alpha/fog/swap-table state, including
any source setup prerequisites not written by this effect. Its preflight must
reject states that this native material cannot represent. It must park unused
sampler2 and disable incompatible inherited native features. `apply` has no
failure return: it is allowed only after complete admission and cannot release,
replace or reenter any pinned owner. Submission and lifecycle are single-threaded.
The native backend does not select fallback raster defaults or restore a guessed
previous state. The actual outer display context must retain source GX state for
later callbacks and perform the correct source/native continuation.

## Normal-game frontier and evidence

Still required for normal activation: actual global EffectPool initialization,
source heap and effect allocation identities, frame/context/camera4 epochs,
original state64/file/palette/type4 ownership and genuine texture-object
construction, retained queued display owner/exclusion, inherited raster admission
and subsequent native state restoration. Delegating `source.call` is allowed only
for actual camera and texture-object queries; returning a texture token based on
its name or index alone is not genuine source file ownership. The image74 owner
cannot fulfill the distinct original unloaded 1×1 fallback object.

Focused tests run the complete original Display/main_dl through finite native SDK
math with explicitly labeled fixture camera/object identities and private original
metadata. They verify three-pass order, each matrix/query wait, UV collapse,
source-array alias rejection, source service retention and allocation/residency
loss. Strict host, ASan/UBSan, strict ARM syntax and Picasso compilation passed.
No native import/render screenshot or original framebuffer comparison is claimed
by this increment. All earlier asset provenance, hashes and rights restrictions
remain in the linked resource contract. No new original asset payload is tracked.
