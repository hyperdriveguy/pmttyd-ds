# Original ordinary airborne Mario shadow: finite GX request graph

US G8ME01 revision0, cylinderShadowDraw8002bf84 and cylinder8002c2ec.
This closes source request order, literal inputs, opacity admission and all
five indexed cylinder lists/all60 positions. It does not establish original
raster output or a faithful PICA200 implementation. The previous
[shadow caller, collision matrix and ground strip](OVERWORLD_MARIO_SHADOW_REQUEST_AND_GROUND_POLYGON.md)
remain the root/size/query owner; preserve source winding and unit adapters.

## Admission, matrix and per-record ordering

cylinderShadowDraw loops92-byte records in queue order. It admits size10!=0,
**type2**, prepared flags bit2, and signed camera1a equal current camera ID
(8002bfc8–ff4). It loads camera.view*record.matrix2c at8002c008–20.
The collision/bypass owner already supplied that matrix; this draw does not
perform another ray, replace normal/height, or move the shadow to a flat floor.
For original normal(0,1,0), matrix=translate(hitX,hitY+1,hitZ)*diag(−s,20,−s):
top localY0 is at hitY+1, bottom localY−1 extends20 engine units downward.
These dimensions are source geometry/transform, not Mario body dimensions.

The three passes execute **record A:1→2→3, then record B:1→2→3**. Do not
batch all records' first passes together: framebuffer destination alpha is
shared and later records see earlier changes. After pass3 the routine proceeds
to the next record without restoring render state to entry defaults. Camera1
has additional bounding/image-cache service calls; ordinary camera4 excludes
those branches. Variant byte3==1 draws a cube; ordinary EntryMode initialized
variant0 uses the cylinder path below. Other variants take the cylinder branch.

## Inherited source shader and indexed input

shadowDisp sets common state before invoking cylinderShadowDraw
(8002d468–610). Exact scalar argument records are in the verifier manifest:

| Request | Arguments |
| --- | --- |
| GXSetNumChans | (1) |
| GXSetNumTexGens | (0) |
| GXSetNumTevStages | (1) |
| GXSetTevOrder | (stage0,coord255,map255,colorChan4) |
| GXSetTevColorOp / GXSetTevAlphaOp | (0,0,0,0,1,0) each |
| GXSetTevColorIn | (0,15,15,15,10) |
| GXSetTevAlphaIn | (0,7,1,5,7) |
| GXSetFog | type0, four float0 arguments, source zero color |
| GXSetZCompLoc | (1) |
| GXSetAlphaCompare | (7,0,0,7,0) |
| GXSetCurrentMtx | (0) |

Standard GX interpretation: RGB comes from raster color; alpha is TEV A0
multiplied by raster alpha, using ADD/zero bias/unit scale/clamp/PREV. No
texture generators/textures or lighting approximation are needed for this
program. This describes the requested program, not its exact fixed-point
rounding. Common color channel4 initially uses material/global color; each
pass below changes its material source explicitly.

Indexed position attribute9 is INDEX8, XYZ/F32 with12-byte stride;
color attribute11 is INDEX8, RGBA8 with4-byte stride (8002d5a4–60c).
Position base=vertex_tbl80304ce0. Common color base=ordinary palette80415140;
cylinder selects the applicable palette before emitting lists. Display lists
supply position/color pairs, not already-expanded triangle vertices or UVs.

Before pass1, SetTevColor(register1,RGBA(0,0,0,record28)) at8002c024–44.
The RGB base comes from cold-zero sbss2 word80429540; pass2 material comes
from cold-zero word80429544. Startup __init_data80005498–cc clears the
80429500..8042990c range from table800055b0, covering both. These two leaves
do not write those words; they copy them to locals and replace record alpha
only in the first local. This is distinct from global.shadowColor100, whose
initialized RGBA is(0,0,0,110).

## Three passes, exact order and masks

All passes request depth test enabled, compare3=LEQUAL, **depth write0**.
Source GX cull enum2=BACK, enum1=FRONT. Preserve the original list winding
when mapping these enums into the native projection/front-face convention.

| Order | Exact requests before geometry | Effective role |
| --- | --- | --- |
| 1 | ZMode(1,3,0); Cull(2); Blend(1,1,1,0), then immediately Blend(2,1,1,7); AlphaUpdate(1); ColorUpdate(0); ChanCtrl(4,0,0,1,0,2,2) | Vertex color source; alpha-only logical OR, drawing front-facing surfaces |
| 2 | ZMode(1,3,0); Cull(1); Blend(0,4,5,0); AlphaUpdate(1); ColorUpdate(0); ChanMatColor(4,RGBA0); ChanCtrl(4,0,0,0,0,2,2) | Zero constant material; alpha-only overwrite, drawing back-facing surfaces |
| 3 | Cull(2); Blend(1,6,7,0); AlphaUpdate(0); ColorUpdate(1); ChanMatColor(4,globalColor100) | RGB blend using destination alpha; alpha preserved |

Pass1 anchors8002c048–b4, pass2c110–17c, pass3c1d8–218.
`Blend(1,1,1,0)` is replaced before any primitive by `Blend(2,1,1,7)`;
there is no geometry between them. GX mode2 is LOGIC and op7 is OR, rather
than additive blend or XOR. Pass2 mode0 is NONE, so its factor fields4/5 do
not turn it into source-alpha blending. Pass3 mode1 uses factors6/7=
**DSTALPHA / INVDSTALPHA**. Its ChanCtrl inherits pass2's constant material
source and disabled lighting; the vertex colors remain attached to the lists
but do not select material color in this pass.

Each pass invokes the same cylinder(record) plan (or the cube branch) before
setting up the next pass. For defaults, pass1 alpha shader combines record
alpha255 with vertex palette alpha; pass2 outputs zero alpha; pass3 global
black RGB darkens through the retained destination-alpha mask. The complete
result also depends on the entering framebuffer alpha/depth and original GX
pixel format/precision. No alpha-clear or framebuffer allocation occurs in
these two bodies. Do not assume entering alpha0 or255 without its owner.

## Literal opacity and list-admission math

cylinder first tests flags bit4. If set, bind **no-decay** palette80415160,
then call top→bottom→side, each64 bytes; return. This bypasses the math below,
including global alpha0. Ordinary freshly queued/prepared flags are3, so
this bit4 path is an explicit external flag dependency.

Otherwise at8002c354–410, use the following finite source operation order.
Every indicated product/divide/subtract is single precision; host reference
arithmetic rounds those individual operations and labels backend adaptation:

    a = single(single(U8(globalColor+3) * record1c) / 255)
    b = single(a * record20)
    inner = single(1 - record24)
    inner = clamp(inner, 0, 1)
    if a <= 0: return without geometry
    if b <= 0: b = 0
    if b >= 1: b = 0          # source reset, not clamp-to-one
    alphaByte = trunc(single(255 * b)) & 255
    bind ordinary palette80415140
    if inner != 0: draw top_inside
    if inner != 1 and alphaByte != 0: draw top_outside
    draw bottom
    draw side

Defaults: globalAlpha110,record1c=1,record20=.2,record24=.4; inner=.6 and
alphaByte22. The source **does not rewrite the palette** with a/b/alphaByte,
and **does not rescale the inner vertices** by inner. Those fields decide
which lists run; the original inner ring is already authored at radius.6.
Changing record decay within(0,1) does not introduce a smoothly changing
inner-ring radius in this leaf. A portable opacity slider must not invent it.

The ordinary palette is static RGBA(0,0,0,110),(0,0,0,0),(0,0,0,22).
No-decay has three copies of(0,0,0,110). Palette colors, TEV record alpha,
pass2 zero material, pass3 global material and destination alpha are separate
inputs. One actor alpha value cannot replace all of them.

## Original position/list import contract

All60 original XYZ words occupy vertex_tbl80304ce0,720bytes,stride12,F32BE.
The first16 are a top center plus14-sided unit-radius ring/repeated endpoint,
localY0. Positions16..31 are the matching bottom atY−1. Positions32..47
are a top center plus original radius.6 ring/repeated endpoint. Positions48..59
are box vertices, unused by these five cylinder lists. Preserve the exact
source float words; do not reconstruct sin/cos to smooth or symmetrize them.
Some mirrored source values differ in their final bits.

Every list below is64 bytes: first byte GX primitive, BE16 count, then count
pairs(U8 positionIndex,U8 colorIndex), followed by zero/NOP padding:

| Name / VA | Primitive/count | Position/color pairs in source order |
| --- | --- | --- |
| top80304b60 | FAN0xa0 /16 | (0,0), then positions15 down to1, all color0 |
| bottom80304ba0 | FAN0xa0 /16 | positions16 through31, all color1 |
| side80304be0 | STRIP0x98 /30 | for i=1..15: (16+i,1),(i,2) |
| top_inside80304c20 | FAN0xa0 /16 | (32,0), then positions47 down to33, all color0 |
| top_outside80304c60 | STRIP0x98 /30 | for i=1..15: (i,2),(32+i,0) |

Default plan in each pass is inside→outside→bottom→side. Do not reorder the
caps, reverse fans or convert strips using fan triangulation. If expanded,
standard strip winding alternates every triangle and fan winding retains the
listed center/order. Source default emits92 indexed vertex pairs per pass
(16+30+16+30), not a single14-gon disk.

`tools/verify_overworld_mario_air_shadow.py:geometry(iso)` returns all60
positions, all five parsed primitives/pairs and both palettes **in memory**
from the read-only DOL. To obtain an asset-import handoff:

    python3 tools/verify_overworld_mario_air_shadow.py \
      --export-geometry /tmp/overworld-mario-air-shadow-geometry.json

Export is restricted to /tmp or ignored local-assets; original geometry/compiled
packs stay outside source history. The tracked evidence contains spans/hashes,
source call arguments and addresses, not the original60-vertex payload.

## Validation and backend assessment

Verifier pins four containing bodies, seven data spans,364 instruction anchors,
32 ordered scalar GX call sites/targets/arguments; parses all60 positions/five
lists/two palettes and runs17 finite geometry/branch cases. Geometry hashes
alone were not used as evidence of the state graph. The literal arguments are
checked against preceding source li operands and each direct BL target.

A concrete backend needs: scene depth retained for all three passes; alpha-only
logical-OR writes; zero alpha overwrite with opposite culling; independent
color/alpha write masks; destination-alpha RGB blending; source TEV/channel
switches, record transform and per-record ordering. EFB alpha initialization/
pixel format, fixed-point logic/blend precision and PICA200 support/adaptation
remain backend dependencies. Blending a projected circle/ellipse cannot satisfy
this graph. A software mask/render-target adapter can be assessed against these
requests, but neither this contract nor passing source verifiers demonstrates
original images or numeric raster parity. Cube/camera1 image-cache paths and
external special flag/color/height/alpha writers remain separate domains.
