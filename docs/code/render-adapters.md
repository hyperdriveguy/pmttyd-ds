# Battle, item and window render adapters

The remaining `source/display/render_*.cpp` adapters connect portable retained
requests to Citro3D. This guide complements [main renderer ownership](renderer.md)
and [opening/frontend rendering](scene-rendering.md). Their source comments explain
admitted domains beside the checks; a successful draw is not evidence that all
original GX behavior is supported.

## Three kinds of retained work

* Tutorial and ordinary field presentation prepare CPU geometry, flush it, then
  submit it through native scene framing. Their clock advances belong to
  simulation visitors, not the number of screen draws.
* Original finishing callbacks resume retained camera/display/material/geometry
  continuations. An unavailable provider returns false/nullopt and preserves the
  real wait. Restoring physical GPU state does not replay logical source setters.
* The second battle view and retained icons replay previously prepared geometry
  and state. They must not sample animation again, repeat source calls, change
  RNG history or mutate simulation clocks.

`render_finishing_camera.cpp` first restores camera, display and material state,
then supplies callback bridges to `resumeFinishingDraw`. The actual source dummy
triangle seeds logical state once. Its native packet draws the original
(0, 0),(20, 0),(20, 20) triangle with alpha 129, cutoff 128 and write-mask 0. Frame
restoration may restore that state, but cannot reseed owners or repeat geometry.
Citro3D viewport changes disable scissor, so a viewport packet reinstalls the
acknowledged source scissor even if the following source operation holds.

Source compare tokens 1/3/6 map to native GREATER/GEQUAL/LEQUAL under reversed
depth. Color, alpha and depth writes remain independent. GX before-alpha depth
location is retained separately; PICA's constant-reference early test does not
implement that control. The admitted class combinations use late depth testing.

Provider rejection does not promise rollback of already queued physical GPU
state. For example, material publication can bind a texture and configure TEV
before rejecting a missing later input. The acknowledged logical program remains
unchanged; the next resume restores that state. Stack bridge contexts are borrowed
only during the synchronous provider call. A continuation may retain request
values and its cursor, but must not retain those stack context pointers.

## Original callback and SDK identities

These are US guest function tokens, not native callable addresses. Names come
from the locally retained US symbol reference. Unsupported callbacks keep their
provider waits; an empty eligible domain is not blanket callback success.

| Token | Identity / owner |
| --- | --- |
| 0x8011fde0 | BattleDrawEnemyHP |
| 0x8000e564 | Camera letterbox callback |
| 0x80138da8 / 0x801389a0 | BattleStageDispDark / BattleStageDispLight |
| 0x801a38f8 | BattleAudienceDispAudience |
| 0x802036d4 | BattleBreakSlotDispReel |
| 0x80100258 | _btlStockExpDisp |
| 0x801a37f0 / 0x801a36c8 | BattleAudienceDispItem / BattleAudienceDispApSrc |
| 0x80101774 | btlUnitPartsDisp |
| 0x8002d3d8 | Shadow display callback |
| 0x8000f2fc / 0x8000f2e4 | camGetPtr / camGetCurPtr |
| 0x8003dcfc | animSetMaterial_Texture |
| 0x800208c0 | mapSetMaterialLight |
| 0x800217c8 / 0x80021860 | mapSetMaterialFog / mapSetMaterialLastStageBlend |
| 0x80021b80 | mapSetMaterialTev |
| 0x8002b438 / 0x8001d968 | mapGetWork / mapGetBlend |
| 0x802b72b0 / 0x802b7544 / 0x802b7880 | GXInitTexObj / GXInitTexObjLOD / GXLoadTexObj |
| 0x802b4e04 / 0x802ba3d0 | GXSetTexCoordGen2 / GXLoadTexMtxImm |
| 0x802ba2fc / 0x802ba34c / 0x802ba39c | GXLoadPosMtxImm / GXLoadNrmMtxImm / GXSetCurrentMtx |
| 0x802b5a6c / 0x802b5c8c | GXBegin / GXSetCullMode |
| 0x802b932c / 0x802b940c | GXSetBlendMode / GXSetZCompLoc |
| 0x802b8d60 / 0x802b93d8 | GXSetAlphaCompare / GXSetZMode |

Synthetic guest map-work token 0x85000000 is an adapter identity, not allocated
native memory. Its +4 halfword exposes retained material flags. Packed Mario
words 0x84101108/10c/110 similarly belong to emulated battle storage: maxHP is
+108 high 16, HP/maxFP are +10c high/low 16, and FP is +110 low 16. The HUD reads
these live values before field-pouch export so it does not display stale HP/FP.

## Resource admission and geometry boundaries

| Domain | Implemented numeric contract |
| --- | --- |
| Native streams | XYZ(3)/UV(2)/RGBA(4) float attributes; permutation 0x210; six resettable PICA TEV stages |
| Tutorial actors | Setup 2: Crump/Mario/Goombella; PMHTUV01 exact-EOF UV scale 1/2/4/8 |
| Startup resources | Nine banks, CPU readiness separate from actual GPU texture admission; context 1 |
| Tutorial curtain | PMCUR001/A_2, 216 triangles/648 bindings, nonlooping 0..45, UV scale 8; context 2 |
| Audience | Combined 3600-vertex native budget; source map modes 1 opaque/2 translucent |
| Class menu |1024 quads*6 vertices per frame; source flags 0 ordinary, 16 normal icon, 48 disabled icon |
| Field icon | Fixed 32 slots, six vertices each; item 95 icon 307, coin descriptor 403, textures 414..417 |
| Window frame | Nine slices*6=54 vertices per frame allocation; bounded by MaxWindowFrames |
| Window font | Six vertices per glyph quad; outline can emit five quads per glyph; MaxWindowFontQuads counts quads |
| Item effect | Two six-vertex passes; texture 44 pattern 16x16, texture 42 opacity 128x128 |
| Shadow pattern | Original n64 texture 44, 16x16 RGBA8 without mip levels/bias; repeating whole image |
| Cylinder | Five original 64-byte lists; position array 0x80304ce0; palette 0x80415140/160 |

A triangle/fan provider stages a complete primitive before native Begin. Source
GX mode 0x80 is a quad, 0x90 triangles, 0xa0 a fan. Letterbox stages eight S16
vertices then emits two four-vertex fans. Stage color geometry admits direct
position/color and no texture data. Cylinder position/color are INDEX8 pairs;
format 4 is F32 position, format 5 RGBA8 color. Descriptor 0/1/2 means
NONE/DIRECT/INDEX8 in these source declarations. Actor surface cull 2 means
suppressed surface; GX cull 2 means front-face culling. Keep those domains apart.

Character texture admission validates coordinate 0, source CMPR format 14 without
palette/LOD/mips, clamp wrap 0 and linear filter 1. It stages texture object, LOD
and actual bind separately. Texture-coordinate matrix 30 is uploaded matrix
selection, 60 identity; tuple 125 retains the original postmatrix selection. A
bind also validates resource identity, atlas cell, UV scale and matrix/generator.
A failed state publication restores the previous logical atlas/bound image.

## Icon residency and physical state

`bindSourceIcon` holds on a cache miss and records `pendingSourceIcon`.
`prepareSourceIcons` admits one image outside active source submission. Its
replacement scan skips the bound image and all images pinned by both retained
passes, then chooses an empty/oldest unpinned slot. Release synchronizes first,
then clears texture handles, bindings and retained packets together.

Each cell has at least 8 pixels per axis, power-of-two backing and two gutter
pixels on every side. Original width/height plus 4 must fit. Import rejects
rotated, multi-image, non-RGBA8 or mipmapped backing. Replay adds 2 to source UV
texels, divides by actual GPU dimensions and flips V; it does not resize the art.
Grayscale plans accumulate weighted R/G/B in three PICA stages; ordinary plans
multiply material/tint, with another blend multiplication where admitted.
Source RRGGBBAA constants convert to native AABBGGRR. Clear temporary TEV stages
between disabled class icons and later ordinary glyphs.

The installed Citro3D binding implementation reads texture parameters even on
unused units1/2. `parkUnusedSamplers` binds the resident font to those units;
it does not change logical source texture ownership. Multi-texture effects
replace the units they actually sample. A null pointer is not a safe unused
sampler representation here.

## Windows, field acquisition and second views

Source UI 608x480 has center(304, 240). The top native image uses half scale at
(200, 120), bottom at(160, 120), with source Y reversed. `selectorClip` projects
all four source corners through the tilted native matrix and rounds/clamps to
physical240x400 axes; raw integer division would use the wrong raster domain.
Help uses source Y384..440. Kanban/diary use560x176 content in 1024x256 backing;
the diary has a one-pixel inset.

Plain font consumers differ: MessageDirect already supplies transformed
positions; StringPitch supplies per-glyph floating pitch/scaling; matrix
consumers apply local scale/translation here. Font UVs address 512x256. Glyph
clipping skips GPU vertices while preserving source text/random requests.
The native font LCG 1664525/1013904223 is presentation state, separate from battle
RNG. Flag 4/8 icon mirroring changes X/Y; Y mirroring also changes a bottom
anchor's geometry, so swapping UVs alone gives the wrong paging-arrow position.

Acquisition requires actual room0 Mario/audio/pouch/UI/effect resources before
admitting the event. The original key-item array has 121 halfwords from+a0(hex);
zero is vacant. Full storage/discard remains an explicit unbound provider.
Effect 6(item)/7(coin) use local handle 1 for the one retained acquisition effect.
Icon flag 100(hex) selects camera 5; flag 80 bypasses conditional map-lighting blend,
and flag 2 hides. Draw depth writes instead follow alpha in the admitted icon
geometry. Source field camera 4 and item camera 5 have distinct retained frames.

The effect shader projects texture 44 from source POS independently of camera
view and shadow model. UV1 samples opacity 42; native three-stage TEV implements
gray -> gray/orange interpolation -> raster modulation. This decomposition is
needed because each PICA stage has one constant. Output preserves field alpha.

Upper tutorial/audience passes prepare vertices once; lower views consume exact
retained spans. Lower trio ordering is sky, opaque maps, actors, translucent
maps, audience. Stage-map revisions trigger vertex writes/flushes only when the
simulation owner publishes a change. This guide explains the existing adapters;
it does not claim original camera, pixel, timing or hardware performance parity.
