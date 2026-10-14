# Native indexed-fan numeric and array bridge

2026-10-14 read-only check against US G8ME01 DOL and the current native
animation importer. This is static source/backend evidence; no original frame
or hardware numeric equivalence is established.

## Texture generator input is ST11

The original GXSetTexCoordGen2 at802b4e04 initializes r12=0. Its source4 jump
table entry803c4a50 targets802b4e7c, selecting XF row5 without changing r12.
The type1 path802b4ee0 leaves ST projection bit1 clear, writes r12 to inputform
bit2, and row5 to source bits7..11. The resulting regular ST generator therefore
uses inputform0 and TEX0. Original GXLoadTexMtxImm802ba3d0 with type1 sends the
first eight floats unchanged, including both column2 and column3.

[Dolphin XF register definitions](https://raw.githubusercontent.com/dolphin-emu/dolphin/master/Source/Core/VideoCommon/XFMemory.h)
identify inputform0 as AB11 and row5 as TEX0. Its
[software transform implementation](https://raw.githubusercontent.com/dolphin-emu/dolphin/master/Source/Core/VideoBackends/Software/TransformUnit.cpp)
uses(s,t,1,1) for that input and computes two row dot products with output q=1.
The independent
[vertex shader generator](https://raw.githubusercontent.com/dolphin-emu/dolphin/master/Source/Core/VideoCommon/VertexShaderGen.cpp)
constructs that same input vector. Thus for row-major matrix m:

```
s = m[0]*u + m[1]*v + m[2] + m[3]
t = m[4]*u + m[5]*v + m[6] + m[7]
```

The general input is not(u,v,0,1). Omitting m[2]/m[6] is equivalent only when
those entries are known zero. Current source-backed SRT matrices have those
entries zero, but an arbitrary future provider needs the full dot products.
This formula specifies algebraic inputs, not exact Gekko/XF rounding.
Identity matrix selection60 directly preserves incoming(s,t).

## Original arrays and low16 streams

The [source FIFO contract](ANIMATION_RENDER_ARRAYS_AND_VERTEX_FIFO.md) governs
shape versus per-draw bases. Serialized MODEL is big-endian; retained
AnimationDrawResource::word/real handles endian decoding, while byte reads
retain source channel order. Relevant header fields are:

| Data | Count field | Relative pointer field | Element bytes |
| --- | --- | --- | --- |
| Position XYZ | f0 |154 |12 |
| Normal XYZ | f8 |15c |12 |
| Color RGBA8 |100 |164 |4 |
| UV ST |128 |18c |8 |
| Position indices |f4 |158 |4 |
| Normal indices |fc |160 |4 |
| Color indices |104 |168 |4 |
| UV indices k |108+4*k |16c+4*k |4 |
| Call runs |ec |150 |8 |

Colors must use bytes at offsets0,1,2,3 as RGBA. Reinterpreting a host-endian
uint32 as channels reverses them on little-endian ARM. Float arrays must decode
big-endian float words. Low16 indices are relative to the selected shape array
base; do not add that base twice. Call count is low16 of the second run word.

## Preserve one model transformation and one atlas adaptation

ActorAnimationSample::positions contains evaluated local positions; assemble
leaves that array intact and separately writes transformed vertices/world.
The source draw traversal already composes root/model and each visible node
matrix. resumeAnimationShape then requests position matrix view*model. A FIFO
consumer should read the local evaluated positions and retain that matrix;
using sample.vertices or applying sample.world again repeats node transforms.
Normals similarly come from local shape.normals under the requested normal
matrix. The existing normal matrix singular behavior remains separate.

Apply source T24 matrix to original UV before atlas adaptation. With resulting
(s,t), cell c and retained atlas uvScale:

```
U = (128*(c%8) + 2 + 124*s) / 1024
V = 1 - (128*(c/8) + 2 + 124*t) * uvScale / 1024
```

c/8 is integer division. This agrees with current character_mesh conversion and
[atlas identity](NATIVE_ANIMATION_TEXTURE_STATE_AND_ATLAS_IDENTITY.md): use the
actual package cell and one height/V correction. Resampling and gutters remain
native adaptations. UV outside the original clamp domain needs per-cell
clipping or a matching sampler service, not atlas-wide clamp that can expose an
adjacent cell. Fans may expand to triangles(0,i-1,i), preserving attribute order;
zero/one/two-vertex source calls contain no complete triangle.

Private evidence is under artifacts/fan-render-evidence/: original SDK function
disassemblies, source4 jump-table bytes, and numeric-and-array-review.md.
Full sampler/multitexture/lighting support, source GPU rounding and original
frame comparison remain open in [EVT-01](../../requests/EVT-01.md).
