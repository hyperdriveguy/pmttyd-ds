# Retained field animation GX arrays and fan staging

`FieldAnimationGpu` owns one actual event address space, acknowledged source
matrix/cull/array state, and a reusable heap fan buffer. It plugs into the raw
[render parent](NATIVE_ACTUAL_VM_RENDER_PARENT.md) after the shared SDK
camera/matrix binding. It does not use converted pose-resource offsets or the
battle actor resource namespace. The original render parent has already added
shape offsets and selected UV arrays: applying either operation twice is wrong.

## Source interface and waits

The owner recognizes source `GXLoadPosMtxImm` 802ba2fc,
`GXLoadNrmMtxImm` 802ba34c, `GXSetCurrentMtx` 802ba39c,
`GXSetCullMode` 802b5c8c, `GXClearVtxDesc` 802b4520,
`GXSetVtxDesc` 802b3c08, `GXSetVtxAttrFmt` 802b4558,
`GXSetArray` 802b4db0 and `GXBegin` 802b5a6c. FIFO halfwords target cc008000.
These are source ABI identities, not callable native pointers.

Position/normal matrix slot 0 and current matrix 0 are the supported source
parent path. Matrix input is captured before a physical wait. Missing projection
holds position upload; a genuine camera projection publication can repair that
wait. Normal registers retain original values, including exceptional floating
point values. No identity normal is fabricated after singular SDK output.
Cull requests retain their original mode and request/locals identity until
physical acknowledgement. Unrelated source state cannot bypass a held upload.
Restore reapplies acknowledged old state without committing the pending state.

Arrays use attributes 9(position),10(normal),11(RGBA),13..20(UV), index16,
VAT 0, binary32 XYZ/normal/UV and RGBA8, with strides 12/12/4/8 bytes.
`GXClearVtxDesc` clears descriptor enables only; array and format registers
survive. UV descriptors must form a contiguous prefix. Unsupported descriptors,
formats, matrix indices and array strides hold explicitly.

Each FIFO halfword is retained before its effective array read. Addressing is
raw guest base plus unsigned16 index times stride, with source 32-bit arithmetic.
Big-endian float words and RGBA bytes are read through the actual VM. Completed
components survive sparse-memory waits; later components remain live. A changed
halfword cannot replace a held index. Completed vertices are never reread after
a final submission hold, even if backing VM data changes.

## Native boundary and storage

The source fan count uses its low 16 bits. Zero consumes no FIFO or arrays;
counts 1/2 consume the full enabled stream without emitting triangles. Storage
grows to the largest admitted fan, including 65,535 vertices, and is reused for
later fans. The current vertex record is 92 bytes (XYZ 12, normal 12, RGBA 4, UV 64);
the maximum record payload is 6,029,220 bytes. This is an allocation policy, not
measured original memory usage or a frame-time result. Allocation failure holds.

Only complete fans enter `FieldAnimationSubmit`. False retains the same borrowed
buffer and matrix snapshot. Its callback must finish synchronous GPU submission
before returning; an immediate Citro3D Begin cannot remain open across a wait.
Cull ALL consumes arrays but submits no geometry. Source state/camera changes
are rejected during an active fan. The producer, VM, Renderer and all callback
resource owners must outlive every hold. The validity predicate is checked before
VM/backend work; owners must still preserve their ordinary synchronous lifetime.

`Renderer::createFieldAnimationGpu` provides independent field camera/cull
publication through Citro3D. The shared uniform helper copies acknowledged
matrix rows into `C3D_Mtx` and uses `C3D_FVUnifMtx4x4`. Viewport publication
reapplies the field owner's acknowledged scissor because Citro3D viewport setup
disables scissoring. GX cull NONE/FRONT/BACK maps to native NONE/BACK_CCW/FRONT_CCW.
ALL is suppressed by this owner's complete-fan path, not a hardware BOTH token.
This owner must not be used as a cull service for unrelated draw submissions.

## Deliberate remaining boundaries

Material/light/texture/cache children remain explicit backend callbacks, with
no implicit success when a callback is absent. The concrete native factory holds
source dummy draw/state setup until a genuine field material-state owner exists.
It does not seed a battle material owner or borrow battle atlas readiness.
The fan submit callback is likewise explicit: this increment does not supply a
textured/lit field shader, texture lifecycle, ordinary Game scheduling, or release
integration. A lit consumer must handle retained normal state correctly.

Host tests exercise sparse read repair, retry identity and callback mutation,
all eleven array lanes, descriptor-only clearing, zero/short/maximal counts,
retained submission, high-water storage reuse and stale-owner rejection.
An original Mario raw-VM graph exercises real CPU/SDK nodes and source fan data;
material/texture/cache and final raster acknowledgements in that test are
explicit fixtures. ARM and smoke evidence is recorded in the
[progress log](../../PROGRESS.md). No source/Dolphin pixel parity, mirrored-cull
parity, physical-device performance or completed NPC integration is claimed.
