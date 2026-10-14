# Original iconGX draw service

US G8ME01 rev0, original iconGX800197f8/9a4 hex, SHA256
bb9430d4030d22982f813be3bc530403e5a1fd8c24c1ddd9298ff94b1e205848.
Reconstructed from the read-only ISO, with private inspection in
`artifacts/audience-icon-gx.dis` and `.elf`. The whole local body is implemented
by `resumeSourceIconGX`. SDK, resource, map and actual GPU providers remain
explicit. This is a source draw service, not an accepted native rendered frame.

[Matrix-icon wrappers](ORIGINAL_MATRIX_ICON_SERVICES.md) now accept a nested GX
service. Their explicit direct draw callback keeps ownership if present; a
failed direct draw is not bypassed. The GX cursor retains accepted linked calls,
SDK math and the emitted primitive across waits, exposing the actual pending
source function through both wrapper and audience callback cursors.

## Matrix and source texture object

Capture input matrix and initialized source icon fields. Keep the input matrix's
translation XYZ for map lighting. Create Scale(flags4?-1:1,flags8?-1:1,1), then
Concat(input,scale). Ask camGetPtr8, then current camera separately. If different
and flags16 set, append another Scale(1,-1,1) on the right. Load position matrix0
and set current matrix0. No projection, billboard or extra facing rotation here.

Read actual icon.bin global8041e638, and only if nonzero read icon.tpl8041e63c.
If both loaded, read palette8041e630 and call TPLGetGXTexObj80037a80 for the
captured U16 texture. Request GXInitTexObjLOD802b7544: min/mag1/1, float minimum,
maximum and bias0, bias clamp0, edge LOD0, anisotropy0. Otherwise request actual
GXInitTexObj802b72b0 using original SDA image80415080, width/height1, format0,
wrap0/0 and mipmaps0. Then GXLoadTexObj802b7880(object,0). The outer matrix-icon
wrappers have their own earlier absent-buffer return; the standalone GX fallback
is still part of the original body and is not replaced by a transparent no-op.

Typed resource requests use a retained opaque texture-object token returned by
the provider. SDK output-object storage is represented semantically; original
addresses are never cast to ARM/host pointers. Other request integers preserve
source argument order after output/local pointers are removed. Void calls
acknowledge with an optional value; queries return source values. The mapGetBlend
query returns the RGBA that the source function copies to its output local.

## Lighting, TEV and map state

Without flags16 request mapSetMaterialLight800208c0(4,inputTranslation). With
flags16 use NumChans1, ChanMatColor(4,DATA8041f868=ffffffff),
ChanCtrl(4,0,0,0,0,0,2).

Flags32 uses the original three-stage grayscale path: three R/G/B replication
swap tables, per-stage swap modes1/2/3, TEV color1 DATA8041f86c=555555ff and
color2=icon RGBA, and the original ordered texture/channel/previous equations.
This is not a CPU grayscale bitmap conversion. Its setup bypasses the later
map-blend test. After geometry, reset the three stage swap modes to0,0.
Do not redraw the quad if one of those resets waits.

Otherwise use the original two stages: stage0 Order(0,0,0,4), TevOp(0,0),
SwapMode(0,0,0), TEV color1=icon RGBA; stage1 has no texture/channel and combines
register color/alpha with previous. If none of flags16/64/128 set, ask
mapGetWork8002b438 and test its half4 bit2. Only when set call
mapGetBlend8001d968; set TEV color2 to the copied blend RGBA, NumTevStages3 and
configure the original third register/previous stage. Neither mapFlags nor
mapGetBlend is replaced by a global blend assumption.

Both paths then request NumTexGens1 and TexCoordGen(0,1,4,60,0,125).
Without flags16 call mapSetMaterialFog800217c8. With flags16 issue actual
GXSetFog802b8ff4(type0, four float0, DATA8041f870=ffffffff).
These source calls own actual current-map lighting/fog; generic draw setup must
not acknowledge them while their native owners are missing.

## Alpha, cull, declaration and geometry

Icon alpha<255 uses Blend(1,4,5,0), ZCompareLocation1,
AlphaCompare(7,0,0,7,0), ZMode(1,3,0). Alpha255 uses Blend(0,4,5,0),
ZCompareLocation0, AlphaCompare(6,128,1,0,0), ZMode(1,3,1). Afterwards flags16
adds ZMode(0,3,0) independently. Cull is2 for flags400 hex, else1 for flags800,
else0. If both flags set,400 wins. Clear vertex declarations; enable only direct
XYZ9 and ST13; formats(0,9,1,4,0) and(0,13,1,4,0). No direct vertex color exists
in this primitive; color belongs to source channel/TEV state.

Use captured descriptor width/height. Only when both are0 ask object width then
height via802b76c4/802b76d4; each result narrows to U16 before float conversion.
One zero dimension alone does not ask object size. Form left=(-width)*.5,
top=height, widthDelta=width, down=-height. Without flags16, multiply all four
values separately by.6; icon403 additionally multiplies all four by.8, icon405
by.9. Preserve those successive float operations rather than merging factors.
Flags16 skips both ordinary and special reductions. Original immutable constants:

| Address | Bits | Value |
| --- | --- | --- |
| 8041f87c | 00000000 | 0 |
| 8041f888 | bf800000 | -1 |
| 8041f88c | 3f800000 | 1 |
| 8041f890 | 3f000000 | .5 |
| 8041f894 | 3f19999a | .6 |
| 8041f898 | 3f4ccccd | .8 |
| 8041f89c | 3f666666 | .9 |

GXBegin(128,0,4). Four direct XYZ/ST vertices: left/top/0 at0,0;
(left+widthDelta)/top/0 at1,0; right/(top+down)/0 at1,1;
left/bottom/0 at0,1. Right and bottom are float additions, not independently
recomputed anchors. The GPU adapter acknowledges all four FIFO vertices
atomically after Begin; a false quad result must not consume partial geometry.
Source clocks are not advanced by any operation here.

## Evidence and limits

Tests independently pin ordinary source call order/tuples, both camera domains
and every used mirror combination, ordinary/special sizes, grayscale register/
swap cleanup, map blending admission, alpha branches, cull precedence, both
missing buffers, descriptor/object dimension cases and U16 narrowing. Every
linked call and matrix service in five material domains is individually held
and resumed, plus the primitive and post-primitive cleanup. Held dynamic colors
and acknowledged width queries remain captured. A real matrix wrapper runs
through packed fixture metadata, camera concat and this full GX body to a held
primitive provider; releasing it neither repeats selection nor emits twice.

[Native original resource binding](NATIVE_ORIGINAL_ICON_RESOURCES.md) now supplies
the directory, all613 TPL images, existing VM GP/published camera reads and actual
resident native texture binding. Full outer Main GP ownership/cadence, linked
GX/map consumers and actual raster submission remain open. Tests
use explicit service fixtures, not native PICA pixels or hardware. Matrix
numerics use the supplied adapter; Gekko paired-single/FPSCR parity is unproven.
Missing mapped source fields/providers retain unavailability after preceding
accepted effects. This is not a general per-load PPC interpreter or rollback.
No audience selection/input, item receipt/damage, story or simulation cadence
is invented by this service. Visible held/thrown items and optional audience
taps still require the native chain and original input/presentation execution.
