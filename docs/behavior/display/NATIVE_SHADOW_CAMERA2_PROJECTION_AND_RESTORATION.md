# Native camera2 shadow projection and shared restoration

Implements the recovered [ordered class5 suffix](SHADOW_CAMERA2_CLASS5_ORDERED_SUFFIX.md)
after [target tracking](NATIVE_SHADOW_PROFILE_TARGET_TRACKING.md), with the
[unconditional projection prefix and counter gates](SHADOW_PROJECTION_PREFIX_AND_ZERO_COUNTER_ADMISSION.md).

The retained cursor now performs actual cache invalidation, viewport/scissor
readback, captured-size square setup, LookAt and Frustum/Ortho publication,
projection, two independent current-view backup/replacement queries,
Projection's independent camera query and alpha capture, its full24 GX calls
with fresh GP14 selection between calls22/23, then the admitted record scan.
Complete acknowledged calls never replay during a later hold. Setters retain
copied inputs; successful viewport capture survives a held scissor capture.
Size is selected after invalidation, and the later matrix row is selected
after viewport/scissor setup. Projection setter reloads its own live kind
rather than assuming the earlier mathematical branch still applies.

Source C_MTXFrustum retains the recovered single operations and GX near/span
depth terms. It does not use the OpenGL lens formula. LookAt continues the
explicit native normalization adapter; native sqrt/division and PICA uniform
conversion are not original PPC numerical parity. See [source arithmetic](SHADOW_FRUSTUM_ARITHMETIC_AND_AUXILIARY_PROFILE_OWNERS.md).
Work.view/projection are semantic retained matrices, not original packed owners.
The three fog owners are distinct, startup-cleared globals; Projection reads
its own fog word. Native TrioAttack binds each mutable/const current-camera
query independently and freshly reads selected EventGpAddresses+14 at color
selection. Borrowed current camera/pool/profile tokens retain their source use.
Primary pointer replacement and undocumented global writers remain unbound.

Projection scans radius!=0,type3,preparedflag2 without a camera filter. Its
complete geometry provider is separate and remains unavailable in production.
Only successful six-index geometry increments the retained counter, with
uint32 wrap. Nonzero entering/resulting counter holds at the real compositing
suffix instead of claiming a shadow image. Counter0 still executes the full
GX prefix, then End queries a fresh mutable current camera and restores the
saved view, viewport and scissor in order. End does not restore projection,
clear requests/counter, set completion flags or publish an image.

Native Renderer queues GPUCMD_AddMaskedWrite(TEXUNIT_CONFIG,0x4,BIT16) inside
its admitted frame. This is GPU cache clearing, separate from CPU cache flush;
[installed Citro3D/libctru evidence](CITRO3D_TEXTURE_CACHE_AND_VIEWPORT_COMMAND_ADAPTER.md)
confirms byte masking and deferred texture-state compatibility. Setter services
use real C3D_SetViewport/Scissor, DepthMap and projection uniform publication.
Citro3D may coalesce intermediate context setters before a primitive; source
logical state and caller continuations remain separately retained.

The viewport adapter uses the existing608x480→tilted240x400 target, rounding
physical endpoints to nearest integer only for shadow setters. Ordinary camera
setters retain their existing exact integer domain. Source400-square viewport
maps to physical(x40,y137,width200,height263); scissor(1,1,398,398) maps to
rounded exclusive endpoints, not an offscreen400x400 allocation. This is a
native raster scaling/rounding adaptation. It shares the existing target's
entering color/depth owner rather than fabricating a blank scratch framebuffer.
Logical original six-float viewport/four-word scissor caches publish only after
successful C3D submission and supply source readback/restoration. Out-of-bounds,
nonfinite, zero-size or pixel-collapsed rectangles stay unavailable. No clamp
or guessed default supplies an unknown state. Viewport min/max remain source
values; native depth map uses scale=min-max,offset=1-max with PICA quantization.
Viewport disables C3D scissor internally; the renderer immediately reapplies
the acknowledged scissor, preserving GX state even if the next source scissor
operation holds. The later explicit scissor operation remains independently
acknowledged.

Auxiliary pswp/dswp retain original boot flags2,counter0,profileIndex0,color40
and their independent view/projection matrices. They select the same shared
mutable profile table. Source class0 runs its full cache/readback/square/math/
projection/two-view-copy setup and returns without an ownerflag/counter skip.
Class2 first restores pswp view/viewport/scissor before its counter gate, then
sets up dswp. It disables RGB color writes after captured size and before
viewport/scissor readback. Class7 restores current view, re-enables RGB writes,
restores viewport/scissor and only then reads its counter. Its held color
request does not replay the completed view copy. Every setup overwrites the
same saved buffers, not per-owner snapshots. End leaves projection/material
unchanged. Nonzero auxiliary counters hold their genuine image End suffix.
See [ordered auxiliary paths](SHADOW_AUXILIARY_SETUP_AND_SHARED_RESTORE_CURSORS.md).

For source classes1/3/4/6 and>7, camera2 returns after the required current
record query, as the original jump table dictates. In the ordinary retained
counter0 domain, all four camera2 shadow callbacks now complete. Draw reaches
camera4's first DispAudience801a38f8 callback with21 records still retained;
that render provider remains unavailable. Cylinder geometry is recovered in
[a separate contract](CYLINDER_SHADOW_PER_RECORD_GEOMETRY_AND_BOUNDS_CADENCE.md)
but does not justify bypassing the preceding audience callback. Geometry/
images, other owner writers, remaining callbacks, ordered teardown/field
return and hardware raster validation remain unfinished. Native tests establish
this bounded continuation, not full shadows or completed battle return.
