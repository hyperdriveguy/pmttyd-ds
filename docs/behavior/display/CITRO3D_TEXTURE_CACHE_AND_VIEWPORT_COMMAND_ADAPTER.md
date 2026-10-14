# Citro3D texture-cache and viewport command adapter

Installed-library inspection and official devkitPro source, 2026-10-14.
This is native backend evidence for the
[shadow class5 suffix](SHADOW_CAMERA2_CLASS5_ORDERED_SUFFIX.md), not original
GX/PICA hardware parity or a runtime test. No code/shared index/progress edits.

## Real GPU cache command

Use the public libctru command emitter inside an admitted frame:

```cpp
GPUCMD_AddMaskedWrite(GPUREG_TEXUNIT_CONFIG, 0x4, BIT(16));
```

Installed C3Di_UpdateContext emits exactly command header00040080 and
parameter00010000 for texture-cache clearing. Ordinary texture configuration
uses byte maskB, excluding this cache-trigger byte. The masked command
preserves unit enable/configuration state and is compatible with deferred
texture binding. The official implementation issues cache clearing after
configuring units. [Citro3D base source](https://raw.githubusercontent.com/devkitPro/citro3d/master/source/base.c)

C3D_TexBind retains the pointer and marks that unit dirty; context application
later configures units and clears the GPU cache. Rebinding alone does not
emit the requested command immediately. C3D_TexFlush instead requests CPU
data-cache flushing for non-VRAM memory; it is not GPU texture invalidation.
[Texture source](https://raw.githubusercontent.com/devkitPro/citro3d/master/source/texture.c)

A source continuation should enqueue its invalidate once, then retain
completion across later holds. Frame-state restoration must not replay that
source action. Additional library cache clears caused by restoring/rebinding
native texture state are separate backend behavior. Command enqueue is not
a GPU completion fence; source GX submission also need not wait for execution.
GPUCMD requires an active command buffer/capacity, so do not call outside
the renderer's admitted frame or manufacture success without submission.

## Viewport/scissor and depth representation

C3D_SetViewport accepts unsigned integer origins/dimensions. It encodes
width/2 and height/2 as f24, inverse dimensions as f31, packs origins into
16-bit halves, and disables scissor. There is no fractional-edge rounding
policy or dimension safety clamp; avoid zero dimensions and validate against
the actual target. SetScissor accepts exclusive maxima and stores max-1.
Reapply scissor after viewport restoration. [Viewport source](https://raw.githubusercontent.com/devkitPro/citro3d/master/source/base.c)

C3D_DepthMap converts scale/offset directly with f32tof24; it neither enforces
normalized source near/far nor repairs nonfinite input. Source range[n,f]
uses native scale=n-f,offset=1-f under the admitted reversed-depth adapter.
Default source range[0,1] gives exactly representable scale-1,offset0.
[Effect source](https://raw.githubusercontent.com/devkitPro/citro3d/master/source/effect.c)

The installed f24 converter truncates seven mantissa bits, rebiases the
exponent, flushes underflow to signed zero and maps overflow to infinity;
it does not round to nearest. f31 retains23 mantissa bits with its own
rebiasing. Maintain original logical viewport/scissor/depth values separately
from physical quantization for source getter/restoration semantics.
[libctru conversion source](https://raw.githubusercontent.com/devkitPro/libctru/master/libctru/source/gpu/gpu.c)

## Installed evidence identity

Headers inspected under /opt/devkitpro/libctru/include: c3d/base.h,
c3d/texture.h,c3d/effect.h,3ds/gpu/gpu.h and registers.h. ARM objdump confirmed
C3Di_UpdateContext,C3D_TexBind,C3D_TexFlush,C3D_SetViewport,C3D_SetScissor,
C3D_DepthMap,f32tof24/f31. Ignored listing:
artifacts/reference/display-class-check/C3Di_UpdateContext.arm.disasm.

| Installed archive | SHA256 |
| --- | --- |
| libcitro3d.a | 3f34eff859c1f1e60ebe597b6df9cf5e5e74563d9e64ca01a7b3292fde63016a |
| libctru.a | 5e72cd24512b57908545e928cf0a1c1affbb8535f97773d7007be8f10f0e02bc |

Official source links are corroboration of the installed bodies above, not
a claim that mutable master branches permanently identify this build.
Hardware viewport limits and exact raster/cache timing remain unmeasured.
