# Save-block animated UVs, effective wrapping and source material passes

US G8ME01 disc0 revision0; 2026-10-13. Static read-only ISO evidence and
host evaluation of recovered operations, not Dolphin execution or GPU parity.
This delivers the texture/material dependency in
[OW-INTERACT-01](../../requests/OW-INTERACT-01.md). Spawn, hit and save-event
ownership remain in [source binding](../field/ROGUEPORT_SAVE_BLOCK_SOURCE_BINDING.md).

## Original resource identity and effective wrapping

| Resource | ISO byte offset | Length | SHA256 |
| --- | --- | --- | --- |
| `a/MOBJ_SaveBlock` | 911583192 | 24648 | `d5fec097a3d38092e766ca025180a24b2fdc2c91f992cb5cb556b55e67ecb0d9` |
| `a/MOBJ_SaveBlock-` | 911607840 | 163968 | `46cddb3a2b1f6b22da35ffda9964b8ad475ea46a675bc8c5ba89eca147c8b36e` |

Serialized model word+0x194 points to the eight-byte base-binding table at
0x2aa8. For binding i, its first word is the base descriptor index and its
signed second word is the wrapping override. Both bindings have override3;
base descriptor indices are0 and1 respectively. This **overrides the TPL's
clamp defaults**. Following
[texture object setup](ANIMATION_TEXTURE_OBJECT_SETUP.md),
S=(override&4)?2:(override&1), T=(override&8)?2:((override>>1)&1),
so both effective coordinates use GX wrapping1 (repeat). A negative override
would retain image-header defaults. The earlier inbox's TPL clamp0/0 statement
is true of headers only and must not become this object's effective sampler.

The descriptor table at model word+0x198 has two64-byte records: both kind0,
TPL indices0/1 at descriptor+4, kind at+8. TPL0 is128x128, format3 (IA8);
TPL1 is256x256, format4 (RGB565). Both image headers request wrap0/0 and
min/mag filters1/1. Image-header offsets are0x1c and0x40. These are source
requests, not a measured portable filtering/LOD equivalence.

## Cumulative texture state and animation time

`S_1` has one loop descriptor: selector1, start1.0, end119.0. Its119 continuous
keys have timestamps1..119. Key1 has no texture increment; each key2..119 has
one12-byte texture update for binding1. Relative target-index byte1 advances
from stream cursor0 to binding1; descriptor-index delta byte is0 in ALL118
entries. Updates add their original binary32 float+4/+8 to state tx/ty with
separate single-precision additions. They are cumulative DELTAS, not absolute
UV positions, rates or divided-by16 values. There are118 node increments too;
no position, normal or visibility increments in this clip.

The two24-byte source texture states begin at model word+0x190=0x2a78:
index byte0=0, five floats+4..+0x14=(tx0,ty0,sx1,sy1,angle0).
Only binding1 tx/ty change; binding0 stays identity, both index bytes remain0.
Retain five base floats and both float deltas in a native pack. A format that
serializes only the descriptor-index byte loses the entire scrolling effect.

[Cache replay](ANIMATION_COMPRESSED_DELTA_CACHE.md) applies keys0..current
inclusive after a reset and old+1..current while advancing. Each stream resets
its target cursor for EACH key. Rewind/loop wrap restores bases and replays;
subtracting prior deltas is not the source reset path. Texture states receive
NO fractional interpolation in animPoseMain. At selected time2.5 the texture
state equals key2's accumulated state; interpolated node motion continues
independently. [Interpolation](ANIMATION_OUTPUT_INTERPOLATION.md) does not
justify blending next-key UV deltas.

Host replay with a binary32 rounding after every addition gives:

| Selected key time | tx | ty | IEEE754 words tx/ty |
| --- | --- | --- | --- |
| 1 | 0 | 0 | `00000000/00000000` |
| 2 | 0.00847576279193163 | 0.01695152558386326 | `3c0added/3c8added` |
| 30 | 0.24627886712551117 | 0.49255773425102234 | `3e7c3087/3efc3087` |
| 60 | 0.5 | 1 | `3f000000/3f800000` |
| 118 | 0.9833378195762634 | 1.9666756391525269 | `3f7bbc07/3ffbbc07` |
| 119 (authored cumulative endpoint) | 0.9916678071022034 | 1.9833356142044067 | `3f7dddf1/3ffdddf1` |

These are derived host checks, not observed game frames. Normal elapsed119 in
this looping clip selects time1, not the authored endpoint119. Elapsed120
selects2. The loop span is118, not119 or120. The complete
[Main time contract](ANIMATION_MAIN_ENTRY_AND_TIME.md) uses elapsed milliseconds
converted to animation frames by separately rounded60*elapsed/1000 and local
rate, then fused X−trunc((X−start)/span)*span; initial X<start selects start.
[Timeline selection](ANIMATION_TIMELINE_SAMPLING.md) admits timestamps<=time.
Retain exact endpoint/start and source clock/rate ownership; a fixed simulation
clock is a platform adapter, not recovered field cadence.

## Coordinate order, signs and atlas adaptation

The [coordinate matrix contract](ANIMATION_TEXTURE_COORDINATE_MATRICES.md)
consumes authored UVs with column-vector affine composition. It requests:
rotation R around(.5,.5), with negative angle converted to radians using
binary32 coefficient0x3c8efa35; translation M=(tx,round32(round32(1−ty)−sy));
scale S=(sx,sy). Selected product is(R*M)*S when all components apply,
with the documented omitted-component cases and literal gates. Default
(tx,ty,sy)=(0,0,1) still admits an identity translation; do not replace the
literal translation gate with an intended-looking zero-translation check.

For this specific clip sx=sy=1 and angle0 throughout, so raw(u,v) becomes
(u+tx,v−ty). Negative ty in the UV mapping follows from the source's
1−ty−sy translation. The texcoord's raw v is not pre-flipped by this helper.
Any decoded-image-to-3DS atlas V reversal occurs AFTER this source transform.
Apply wrapping to transformed coordinates, not to the original UV before the
matrix. Do not clamp or take fract at triangle vertices and interpolate those
modified vertices: that changes the surface at repeat boundaries.

A native atlas may split triangles at transformed integer repeat boundaries
and interpolate positions, colors and UVs at new vertices, then map each
subtriangle into one repeated tile. Dedicated repeat-enabled GPU textures are
another adapter. Either must preserve repeat across both axes, source winding,
alpha/pass handling and bounded capacity admission. Atlas padding/filtering
and exact sampling at tile edges need native verification. Clamp remains a
needed general future-resource sampler, but this object's overrides select
repeat. Clipping at integer boundaries is portable implementation advice,
not an original CPU triangulation behavior.

## Visible shapes, color and pass ownership

The source has12 shapes,20 draws and15 groups. All15 base visibility bytes
are1, and S_1 has no visibility increments, so untextured draws are active
geometry. All groups use transform nodes (joint flag0). Shape record stride168,
pass at+0xa0, cull index at+0xa4; draw record stride108, texture count+0,
TEV mode+8, binding slot+0x10 and single-coordinate selector byte+0x30.

| Shapes | Pass | Draw records | Source textures |
| --- | --- | --- | --- |
| 0..3 | 2 | 0..7, two per shape | Binding0 followed by an untextured draw |
| 4..5 | 0 | 8..9, one per shape | Untextured frame/star geometry |
| 6..9 | 3 | 10..17, two per shape | Binding1 followed by an untextured draw |
| 10..11 | 3 | 18..19, one per shape | Binding1 |

All cull indices are3. The source cull table maps indices0..3 to2,1,3,0;
therefore these request GX cull0 (none). All draw TEV modes are0. Textured
draws have one slot and selector0; untextured draws have zero slots. Original
196 RGBA source entries include varied colors and alpha165/178; preserving
vertex RGBA is necessary even with original TPL images. Dropping untextured
geometry omits the frame/star and other original layers. A synthetic white
texture can be a neutral native adapter for source raster-only draws, but it
must preserve their colors, alpha and source pass rather than invent artwork.

`mobjDisp`0x80061524..0x8006166c calls dispGetCurWork0x80010864 and
reads record byte+1. Class0 requests animPoseDraw pass1, class1 pass2, every
other class pass3 (calls at800615c0/800615ec/80061618). Source draw scale
is10*objectScaleX. Resource+c4=0x0d admits pass2 and3 and rejects pass1;
pass2 also has the special resource-bit1 admission.
[Shape admission](ANIMATION_SHAPE_RENDER_ENTRY_AND_DEFAULTS.md) admits pass0
shapes during effective pass2 as well as matching shapes. With default alpha,
pass2 therefore owns shapes0..5 and pass3 shapes6..11. These are two source
passes, not one global opaque batch. Original pass3 enables SRCALPHA/
INVSRCALPHA blending, retains depth test and disables depth writes; pass2
uses depth writes and source alpha-test requests. See
[inner draw state](ANIMATION_INNER_DRAW_STATE_AND_BUFFERS.md) for exact tuples
and map-group alpha overrides that can force effective pass3.

## Initial material state and remaining owners

Fresh direct DOL inspection of animPoseEntry local80041b34..60 shows default
pose+e0/+e4/+e8/+ec=0 and pose+f0/+f4 loaded from8041fb0c/8041fb10,
both original words0xffffffff. Thus material callback/default-skip is clear,
TEV-extra flags+e8 start0 and both pose color alpha bytes start255.
`mobjEntry`80060d40..48 calls800406f0(id,2); its complete leaf
800406f0..8004070c ORs argument2 into pose+ec. Initial object lighting flags
therefore contain2, enabling character/near-object light selection. No material
flag override is made by the save-block constructor80066dd4..80066f54.
These initial conditions do not prove later global/pose writers never change
material state.

[Mode0/no-texture base](MAP_TEV_BASE_MODE_ZERO_AND_SOURCE.md) supplies the
source texture/raster combination or raster-only stage. With material flags0,
no extra flag-driven TEV stages are initially selected. Source
[lighting](MAP_MATERIAL_LIGHT_SELECTION_AND_CHANNELS.md) flag4 is clear,
therefore material color source is vertex data; enabled near-object lights,
ambient light, normals and fog remain live scene services. A native
texture*RGBA implementation preserves these base inputs while lighting/fog
and original pixel equivalence remain explicitly unfinished. No source
shadow, collision/hit/card-service or complete event behavior is inferred from
this visual resource witness.

## Validation and scope

Read-only `iso_assets.inventory` plus exact original-resource reads reproduce
hashes/header fields above. `character_animation_data.animation` recovers
S_1 key records, and independently rounded cumulative float additions reproduce
the table. Fresh `inspect_dol_function.py --address ... --length ...` listings
for constructor, entry, display and the two pose helper/default regions remain
in /tmp only. General source verification passes:
`verify_animation_texture_light.py` (14 bodies/1189 words/11 constants/8 spans),
`verify_animation_main.py` (2 bodies/1773 words/17 constants/5 spans), and
`verify_animation_render_material.py` (2 bodies/1310 words/5 spans).
These general verifiers do not independently pin this new resource witness.
No original payload/instructions are added to source history or redistributed;
original Nintendo/Intelligent Systems asset rights remain unchanged. Docs-only:
no make/build or original-runtime/hardware claim. Future work includes actual
object display registrations, field clock/rate/writer tracing, native repeated
atlas/color/translucent verification and matched original captures.
