# Native actor surface pack

PMANIM4 is a local rendering adapter to
[original material and UV binding](SAVE_BLOCK_ANIMATED_UV_AND_MATERIAL_BINDING.md).
PMANIM1/2/3 loading remains supported. Ordinary NPC converters retain PMANIM3.
The surface converter retains original S_1/A_1 positions, sparse keys, node
hierarchy, visibility, indexed UVs, texture selectors and vertex RGBA.

## Serialization

All numbers use existing little-endian host/ARM conventions. After the ordinary
indexed binding/palette streams, each texture binding adds five float transform
values(tx,ty,sx,sy,angle) and two uint32 wrap modes (28 bytes). Each indexed
vertex adds four RGBA bytes. Each triangle adds uint32 source pass and cull
(8 bytes). Each key adds two float texture-shift deltas per texture-update record after
its existing position/normal/node streams. Nonzero source start time is retained.
Only the recovered TEV0/single-texture-selector domain is accepted. Untextured
source draws use an extra neutral-white binding and keep their original colors.
Unsupported domains reject instead of losing data.

## Sampling and submission

Position/node streams use the established interpolation. Texture shifts update
at their discrete source key boundaries. Source R*M*S applies scale,
translation `(tx,(1-ty)-sy)` with the literal source gate, then negative-angle
rotation about(.5,.5). Native V reversal follows source transformation.
Effective wrap uses the source material override before TPL defaults.

The portable sampler cuts triangles at transformed wrap/clamp planes and
interpolates XYZ/RGBA/UV at each cut, then maps each polygon into its atlas cell.
Vertex-only fract/clamp would change texture interpolation and is not used.
At most34 bins per axis and65536 emitted vertices bound pack work. Loading
checks finite values, supported flags and sampled key bounds; malformed packs
retain the previous pack. Frame overflow rejects the entire actor mesh.

Adjacent pass/cull spans share draws. Source pass2 uses reversed-native GEQUAL
depth with depth writes, alpha>=128 and no blending. Pass3 uses GEQUAL without
depth writes and source-alpha blending. GX cull modes map to PICA face modes;
GX all skips submission. Existing PMANIM3 NPC admission remains unchanged.
Citro3d/Tex3DS use the existing shader and textures; CPU-written linear vertex
storage is flushed with GSPGPU_FlushDataCache before submission.

## Evidence and limits

Dense barycentric probes verify area, RGBA and texture interpolation for all
nine clamp/repeat/mirror combinations. Original save-block samples exercise
S_1 discrete shifts and wrap, A_1 and malformed-pack atomicity. Idle S_1 peaks
at402 vertices. Isolated Azahar captures confirm original colored block and
glow presentation. These checks do not establish original GX lighting, global
translucent sorting or hardware performance. The per-model tick/pose cache and
per-area residency permit future object assets; independently timed object
instances require extending the cache key when their hit lifecycle is bound.
