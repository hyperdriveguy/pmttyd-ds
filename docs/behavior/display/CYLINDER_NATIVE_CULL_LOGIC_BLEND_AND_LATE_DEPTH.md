# Cylinder native cull, logic blend and late-depth adapter

2026-10-14 read-only original SDK/installed Citro3D check for
[cylinder's ordered passes](CYLINDER_SHADOW_PER_RECORD_GEOMETRY_AND_BOUNDS_CADENCE.md).
This is a bounded native adapter contract, not hardware raster equivalence.

## Culling with the established camera conversion

For original source triangle order and the native camera conversion
`clipNative=(clipSource.y,-clipSource.x,clipSource.z,clipSource.w)`, use:

| Original API | Native API | Surviving physical XY winding |
| --- | --- | --- |
| GXSetCullMode(0) NONE | C3D_CullFace(GPU_CULL_NONE=0) | Both |
| GXSetCullMode(1) FRONT | C3D_CullFace(GPU_CULL_BACK_CCW=2) | CCW |
| GXSetCullMode(2) BACK | C3D_CullFace(GPU_CULL_FRONT_CCW=1) | CW |

These seemingly inverted names follow the source winding convention, not a
new reflected model matrix. US GXSetCullMode `802b5c8c/28` rotates/inserts the
two API bits so FRONT1 becomes hardware BP2 and BACK2 becomes BP1. In
original ordinary negative-height viewport, API BACK2 rejects positive clipXY
orientation and keeps negative. This is independently visible in Dolphin's
actual original raster implementation: IsBackface's homogeneous determinant,
its ordinary viewport sign, and ProcessTriangle's BP mode gates. The BP enum
names alone are insufficient because the source implementation's culling
comments expose the viewport convention.
[Dolphin original clipping implementation](https://raw.githubusercontent.com/dolphin-emu/dolphin/master/Source/Core/VideoBackends/Software/Clipper.cpp).

Native tilt's two-dimensional matrix `(0,1;-1,0)` has determinant+1, so it
preserves orientation, including homogeneous XYW determinant. Native positive
240×400 viewport halfsizes also preserve sign. The rectangular source
608×480 to physical240×400 translation/positive scaling does not introduce a
second reflection. For example original clipXY vertices(0,0),(1,0),(0,1)
have determinant+1; tilted(0,0),(0,-1),(1,0) still+1. Therefore no extra
swapped cull mapping for display rotation is justified. This excludes a future
shader that adds a further Y flip or an explicitly negative viewport size.

PICA register mode1 keeps clockwise; mode2 keeps counterclockwise. Its
software rasterizer reverses mode1 triangles before entering the CCW path;
this agrees with installed libctru's mode1 FRONT_CCW and mode2 BACK_CCW
*cull* names. [Azahar rasterizer registers](https://raw.githubusercontent.com/azahar-emu/azahar/master/src/video_core/pica/regs_rasterizer.h),
[Azahar native triangle rasterizer](https://raw.githubusercontent.com/azahar-emu/azahar/master/src/video_core/renderer_software/sw_rasterizer.cpp).

The source cylinder top fan starts (0,15,14), whose conventional ordered cross
product is(0,-0.43388399481773376,0); bottom fan(16,17,18) gives
(0,+0.43388399481773376,0). First side strip triangle(17,1,18) has cross
(-0.09903097152709961,0,-0.43388399481773376). These inward conventional
normals are correct source ordering. Expand fan as(center,previous,next),
and strip with alternating triangle order. Do not reverse triangles to make
those conventional normals point outward. Collision's separate opposite
winding convention is not an adapter for GX drawing.

## Actual Citro3D supported state commands

Installed `/opt/devkitpro/libctru/lib/libcitro3d.a` disassembly confirms:
C3D_CullFace retains mode; effect binding writes mode&3 to FACECULLING_CONFIG.
C3D_ColorLogicOp clears mode bits8..15 and stores opcode. C3D_AlphaBlend clears
those same bits and sets0x100, selecting blending instead of logic. Both dirty
the effect cache; state is emitted on draw/context binding. EffectBind writes
BLEND_FUNC, LOGIC_OP and COLOR_OPERATION; do not issue raw register writes
that leave the retained effect cache stale. [Citro3D effect implementation](https://raw.githubusercontent.com/devkitPro/citro3d/master/source/effect.c).

Use actual `C3D_ColorLogicOp`, not an invented C3D_LogicOp. Source GX logic
OR7 maps to `GPU_LOGICOP_OR=9`; source NONE maps to
`C3D_ColorLogicOp(GPU_LOGICOP_COPY=3)` or explicit ONE/ZERO replacement blend.
Source blend factors6/7 are destination-alpha/complement and map to
`GPU_DST_ALPHA=8`, `GPU_ONE_MINUS_DST_ALPHA=9`. Copying raw numbers would
select source alpha/complement. [libctru GPU enums](https://raw.githubusercontent.com/devkitPro/libctru/master/libctru/include/3ds/gpu/enums.h).

The cylinder pass1 source first sets ADD ONE/ONE, then overrides it with
logic OR. Preserve both acknowledged setter operations. Its raster uses OR,
not additive blending. Pass2 NONE replacement overrides OR, and pass3 ADD
DSTALPHA/INVDSTALPHA overrides replacement. Pass3 RGB mapping is:

```cpp
C3D_AlphaBlend(GPU_BLEND_ADD, GPU_BLEND_ADD,
              GPU_DST_ALPHA, GPU_ONE_MINUS_DST_ALPHA,
              GPU_DST_ALPHA, GPU_ONE_MINUS_DST_ALPHA);
```

Alpha factors are irrelevant to pass3's alpha-write0; retain an honest cached
choice and mask rather than claiming they overwrite destination alpha.

## Independent RGB/alpha logic and target precision

PICA software output merger evaluates logic independently for all four RGBA
channels, then applies independent write masks. Thus OR with alpha-only mask
is a real supported operation; it preserves RGB. Its destination-alpha blend
factor explicitly reads destination alpha. [Azahar output merger implementation](https://raw.githubusercontent.com/azahar-emu/azahar/master/src/video_core/renderer_software/sw_rasterizer.cpp).

Original Dolphin logic likewise performs bitwise OR on the full32-bit color,
then independently writes RGB/alpha; alpha-only passes are not RGB-only logic
with unconditional copied alpha. Its source BlendColor uses four factors,
`factor += factor>>7`, then `(src*sf+dst*df)>>8`, clamping to255. RGBA6 stores
and optional RGB dithering are separate. A PICA RGBA8 target and its blend
arithmetic are an explicit native precision adaptation; matching factors
alone does not prove identical six-bit source storage/readback or rounding.
[Dolphin source output merger](https://raw.githubusercontent.com/dolphin-emu/dolphin/master/Source/Core/VideoBackends/Software/SWEfbInterface.cpp).

A target without alpha cannot implement these cylinder passes: alpha-only
coverage accumulation and later destination-alpha blend require retained
writable alpha. Pass1/2 mask must be GPU_WRITE_ALPHA only, pass3 RED|GREEN|BLUE
only, and all three exclude GPU_WRITE_DEPTH. Original destination-alpha
constant override, alternate pixel format/history, unsupported stencil or
unknown target contents are separate domains, not an assumed initialized
scratch buffer. Keep actual inherited target history and final masks.

## Late depth and admission limits

Source cylinder pass1 calls ZMode(1,LEQUAL3,write0); pass2 repeats that request,
pass3 inherits it. Under established reversed source/native viewport depth,
use `C3D_DepthTest(true,GPU_GEQUAL,passMask)` with no depth-write bit.
Retain actual inherited alpha comparison; do not replace it with a guessed
positive-alpha test. Disable C3D_EarlyDepthTest in the established ordinary
adapter: its independent constant-reference comparator is not GX
ZCompareLocation. Alpha and ordinary depth then admit only their conjunction;
with no depth writes this preserves visible color/alpha results for the
ordinary side-effect-free cylinder path, independently of whether source
location is before/after alpha. This is a bounded logical-output argument,
not identical execution timing, performance counters or GPU rounding.
Compose [native class/depth admission](NATIVE_CLASS_STATE_AND_SHADOW_FRONTIER.md).

Camera1 bounding-box reads/updates are additional side effects and require
actual source cadence; this late-depth argument does not fabricate them.
Camera4 has no cylinder bounding calls. Shader image/stencil side effects,
Z-texture, changed comparison/viewport domains or depth writes invalidate this
bounded raster admission and must hold until implemented.

## Validation

Ignored original listing GXSetCullMode.disasm and installed-library complete
ARM listing citro3d-full.arm.disasm are under
artifacts/reference/display-class-check. Original vertex triples read rb from
ISO were checked numerically; no triangle, code, asset or shared tracking
files were changed. `git diff --check` passed. Primary library implementation
and emulator source are software evidence; no real PICA hardware equivalence
or original-vs-native screenshot comparison was performed.
