# Shared SDK matrices and retained draw ownership

The [shared SDK engine](../../source/display/animation_sdk_matrix_binding.cpp)
serves durable local matrices and actual guest pairs. The
[render adapter](../../source/display/animation_render_sdk_binding.cpp) adds lazy
camera views, normal and point requests. The
[draw wrapper](../../source/display/animation_draw_wrapper.cpp) and
[native Main executor](../../source/display/animation_native_main.cpp) retain
separate source invocations. See [parent ownership](animation-parent-ownership.md)
and [facing/node providers](animation-facing-nodes.md).

Field offsets and addresses below are hexadecimal; sizes and counts are decimal.
Native scheduling budgets are not simulation time or original execution timing.

## Concat snapshots inputs before paired output

Matrices are row-major 3×4 binary32, 48 bytes. Local references identify stable
slots; guest references are VM addresses. The request and local storage adapter
identities remain captured through holds, including exact float payload bits.
Every primitive checks the same producer-current predicate.

Concat captures pairs in this order: A00, B00, B08, B10, A10, source tail,
B18, A08, A18, B20, B28, A20, A28. The live tail at `0x8041e458` normally
contains (0,1). All 13 pairs precede the first output. Each row starts with a
rounded multiply, followed by ordered FMAs. Columns2/3 retain an extra FMA of
tail×translation even when finite algebra could simplify it.

Guest output pair order is 00,10,08,18,20,28. Earlier complete pairs stay
committed if a later pair is missing. A pair requires complete contiguous
8-byte backing and one synchronous raw borrow; no pointer survives a hold.
This differs from Copy, which interleaves sequential pair loads/stores. Local
output publishes once through its stable owner after successful arithmetic.
Semantic reserved endpoints never fall through to arbitrary VM RAM; guest
outputs into those ranges are refused.

Non-Concat writers require local output. SDK ZERO/ONE are mutable VM literals,
distinct from node constants. Scale reads only ZERO; Identity, Trans and RotTrig
also read ONE. RotRad evaluates double sin then cos and narrows each before
constant loads. Axis OR0x20 folds X/Y/Z into ASCII120/121/122. Other axes return
without output stores after trig/constants. Native finite admission and host
libm/FMA do not establish full GQR0/FPSCR/exception/NaN equivalence. See
[affine arithmetic](../behavior/display/ANIMATION_MATRIX_STACK_AND_AFFINE_NUMERICS.md)
and [rotation](../behavior/display/ANIMATION_SDK_AXIS_ROTATION.md).

## Render camera and vector storage stay live at usepoints

The render adapter reserves `0x81910000`, 13 slots at stride0x280, without
allocating guest cameras. The entire reservation must remain unmapped. Getters
require a real bank slot or current index, but do not require a published view.
There is no camera4 fallback for an unset current index. View pairs resolve
lazily at offset11c, require publication, and accept four-byte alignment. A
captured token keeps its slot when the current camera later changes; later pair
values remain live until read. Missing/unsupported pairs hold locally.

InvXpose consumes basis only. Status0 preserves absent or stale local normal
storage; status1 can publish exceptional values. It does not substitute identity
for a singular matrix or silently initialize the renderer's output. Original
reciprocal-estimate equivalence remains outside the native division adapter.

MultVec pins a guest matrix token and two local vector slots; the slots may be
the same. It captures vectorXY, M00, vectorZ, M08, M10 and M18, then writes X
and computes Y before reading M20/M28. It then writes Y and Z. A missing last
row therefore leaves X committed; repairing it must not replay X or recapture
earlier operands. The row kernel uses separately rounded x/y products, distinct
z/one FMAs, then a lane sum. A left-fold dot product changes this arithmetic.

Only exact camera/Concat/InvXpose/MultVec requests select these owned children.
Held point/Concat work excludes a different request, local owner or FIFO call.
Other material/GX/texture effects still need the explicit backend. See
[render SDK ownership](../behavior/display/NATIVE_RENDER_CAMERA_AND_SDK.md) and
[normal/point arithmetic](../behavior/display/ANIMATION_SDK_NORMAL_AND_POINT_MATRICES.md).

## Wrapper frontiers and native Main frames are separate owners

The wrapper currently implements the initially unlinked route. It converts
handle×368 to a pose pointer, retains initial W for resource lookup and follows
the original pass-mask reads. A rejected mask completes without a final link
query. An admitted mask rereads the link before dispatching Inner with the pose
pointer, copied caller matrix, angle, scale and original pass.

An initial non-minus-one link reaches LinkedPreparation. A changed final link
reaches CallerRegister because the source dispatch uses incoming GPR30, not a
newly reconstructed linked pose. Both are retained unfinished frontiers, never
empty successful draws. Restoring a memory field cannot undo an already
captured branch. Inner owns any partial effects through its own continuation.
See [unlinked wrapper scope](../behavior/display/NATIVE_UNLINKED_NPC_DRAW_WRAPPER.md).

Native Main has a stable heap frame per actual linked invocation. Each frame
retains native generation/resource/buffer identity, Main/geometry cursors and one
linked child receipt. The first linked request pushes once. Completion clears
and pools that frame, then acknowledges its parent once. Stable frame addresses
survive vector growth, and repeated depth reuses blank frames. A cycle does not
complete through a visited-set shortcut; source nesting can require more frames.

GQR0..7 words persist across invocations in the semantic SPR bank. Camera
admission is lazy at reached facing operations. Current-pass, reservation,
process, native sidecar and alias proofs remain separate from raw source reads.
The closed synchronous raw borrow caches backing proofs, not source values;
its temporary readiness shortcut cannot replace producer or frame checks.
Reconfiguration requires quiescence. Raw Main completion does not assemble a
presentation mesh or acknowledge GPU work. See
[native Main](../behavior/display/NATIVE_RETAINED_MAIN.md) for integration and
remaining runtime/numeric/hardware limits.
