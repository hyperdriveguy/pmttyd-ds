# Native renderer resources, numeric formats and pass ownership

[render.cpp](../../source/display/render.cpp) implements the current Citro3D/
Citro2D adapter. [platform.hpp](../../include/runtime/platform.hpp) declares
owned buffers, textures, capture state and audio services. Portable original
[display services](display-services.md) and [animation helpers](animation-data.md)
remain distinct owners. A visible native draw does not prove complete original
GX scheduling, lighting, camera history or hardware performance.

## Buffer and vertex ABI

| Native allocation | Meaning |
| --- | --- |
| Shared arena12000 `Vertex` entries | Transient world actors, legacy geometry and shadows |
| Message system24 entries | Four quads,6 vertices each |
| Message tails12 entries | Shadow/fill pair,6 vertices each |
| Advance indicator256 entries | Original HOS_C_1 mesh workspace |
| Window frames64*54 entries | 64 native frame slots;9 quads*6 vertices |
| Font quads1024*6 entries | Native glyph workspace |
| C2D capacity2048/text buffer8192 | Native UI allocation policy |

`Vertex` is36 bytes:XYZ3,UV2,RGBA4 binary32. Attribute0/1/2 binds consecutive
fields with buffer permutation0x210 (low nibble first). The
[world shader](../../source/runtime/world.v.pica) supplies homogeneous W1 and
multiplies four clip-transform rows, then forwards UV/color. UV orientation and
texture/color modulation stay CPU/TEV responsibilities. `SystemVertex` is44 bytes:
XYZ3,patternUV2,maskUV2,RGBA4;0x3210 maps four loaders. The
[system shader](../../source/message/message_system.v.pica) forwards the same
mask UV to units1/2 while pattern UV goes to unit0.

The tilted240x400 top target represents400x240 landscape pixels; bottom is320x240.
Transfer flags convert RGBA8 target color to LCD RGB8 without extra scaling.
Native colors from `C2D_Color32` are packed ABGR (low byte red); source text/window
colors can be RGBA (high byte red). Normalize/convert at each boundary rather
than assuming every uint32 color has the same byte order.

The fixed arenas are native limits, not original heap sizes. Complete actors
preflight available vertex capacity and skip an unsupported/oversized mesh;
the lowest-level `vertex` writer silently drops writes at capacity. Copying a
Texture or Room struct does not clone GPU allocations. Texture wrappers need
both Tex3DS descriptor free and C3D texture delete; `freeRoom` does not synchronize
GPU work itself. Caller-owned lifetime and readiness flags matter after partial
initialization. Shutdown is an end-of-owner operation, not a per-frame reset.

## Residency and failure behavior

Room collision and GPU residency are separate. `Game` may validate a destination
against temporarily staged collision, but renderer `changeRoom` retires existing
battle resources and old field Room first after GPU synchronization. It then
loads destination geometry and per-model actor banks. On failure it frees partial
resources and attempts to reload the prior room if one existed, preserving the
original failure string. That reload can also fail. This bounds linear-memory
use; it does not guarantee that previous GPU rendering survives every rejected
transition. [Collision transaction](game-composition.md) rolls back independently.

Legacy battles use `stage`; trio uses `bti_01` plus `stg_00_1`; tutorial startup
owns dedicated field retirement and battle maps. Scene actor atlases for quest,
item get and trio are admitted/retired outside an active frame after
`C3D_FrameSync`. Model/pose resource availability is not equivalent to texture
readiness. Renderer `residentFieldRoom` records actual GPU residency independently
of `Game.progress.room`;UINT32_MAX marks vacancy during replacement.

Each room shares animation/texture resources by model. Current sample cache key
includes simulation tick,pose NAME and frame, allowing independently timed
instances to request different samples. Live simulation-owned NPC roots/facing/
pose epochs take precedence over catalog fallback state. Engine coordinates
convert with.01; ordinary actor model scale.02 combines source scalar2 and native
world conversion. Texture V remapping `1-(1-v)*uvScale` retains the converted
padded atlas convention; several sidecars admit scale1/2/4/8 with exact EOF.
Those values describe UV placement, not arbitrary actor size multipliers.

## Map pack fields and state

Map descriptors store batch/texture counts followed by24-byte `MapBatch` records.
Native bounds512 batches,256 textures and16MiB vertex data limit allocation.
Geometry must contain whole36-byte vertices; batch counts are triangle multiples3.
Texture -1 uses fallback atlas. Load mutates a pending Room and requires explicit
partial cleanup on failure; it is not an atomic standalone importer.

| Packed `layer` bits | Meaning |
| --- | --- |
| 0..7 | Original map layer;value2 routes translucent pass |
| 8..23 | Native cutaway group,cap1024 |
| 24..25 | Original GX cull index |
| 26..31 | Converter-assigned event visibility group |

Fence event group8 retains original vertices for the original A_1 warp. Opening
harbor/muj packs admit maximum event groups14/13; ordinary max12. These are
converter metadata IDs, not room IDs, source draw classes or actor material passes.
Actor pass2 is opaque alpha-test, while map layer2 is translucent: their numeric
namespaces must stay separate.

Map cull0/1 maps PICA back/front CCW under its view transform;2/3 map none.
Actor cull0/1 maps front/back CCW,2 suppresses its draw and3 none under the actor
adapter. Do not share one direct enum conversion across these paths. Map wrap
modes clamp/repeat/mirror bind after mutating the texture descriptor. A pass-local
`MapDrawCache` suppresses only redundant same-pass writes; it cannot span UI or
actor rendering that changes GPU state.

Opaque map visits update UV/mesh tracks from immutable bind coordinates using
elapsed simulation ticks modulo sample count. CPU flushes each contiguous written
vertex run; draw count and flush byte length are different quantities. Translucent
visits consume already-prepared data. Fence capture retains one view/plane plus
source pixel bounds, then derives each tick's warp from original XYZ rather than
accumulating prior deformation. Cutaways are visual adapters: rays to Mario at
+.12/+.65 and partner+.3 world height test real triangles, then hide groups for
an eight-simulation-tick hold. They do not change collision/source scene flags.

## World and shadow passes

World assembly records span endpoints for each material owner. Six leading
vertices form the native sky backdrop (60x36 world area atZ-25), drawn first with
no depth writes. Map helpers change buffer state, so world submission restores
its transient stream before character spans. Actual CPU writes flush before draws.
The native field projection uses supplied camera/lens state, adapted to handheld
aspect; source trio eye/at/up can replace field vectors. Legacy Crump Scene::Battle
uses fixed camera/arc/billboard dimensions separately from `trioBattleActive`.
Legacy bobbing,elliptical shadows,box shade factors and panel styling are marked
native/harness choices beside their uses, not original measurements.

Field write masks preserve alpha for Mario air-shadow masking. Polygon shadow
uses untextured source color. Air shadow emits three equal spans:vertex material,
zero material,global material. Their draws OR alpha on one face set,zero opposite
faces,then blend RGB through destination alpha while retaining depth. Reversed
native projection maps source LEQUAL to GEQUAL. Afterwards renderer restores
common cull/alpha/blend/depth/TEV state. Room opaque/shadow/translucent order adapts
the source class sequence; it does not implement every borrowed original callback.

## Messages and UI ownership

`originalMessage` draws unchanged source records; native screen projection/reflow
changes presentation only. The actual world transform is unprojected through
tilted2D projection to match pixel space. Source centered UI maps around(200,120)
with MessageWindowScale.5. Retain placement per message serial and reproject its
captured talk anchor; active body and selection windows keep separate glyph
dynamic arrays keyed by serial.

Text tokens0xfff7/0xfff8 select source color/icon rather than glyph lookup.
Glyph flags1/2/4 indicate dynamic scaling/jitter/wave;hex0x10000 gates vertical
size. Native xorshift13/17/5 is presentation RNG,independent from mechanics.
Scale relaxation/wave/jitter and system pattern scrolling currently run in draw
presentation; they are not action-command clocks or measured original cadence.
Glyph crops apply to geometry AND atlas UV, with system/ordinary/panel clip bands.
Font atlas is512x256;bubble atlas1024x512. Window types3/5/11 are system/panel/
borderless and suppress speaker placement;type1 has a distinct bubble layout.
Original placement override wins last.

Panel type5 stretches nine24px texture slices with30-source-unit corners. System
pattern type3 has four quads,32-unit edges,128/64 pattern periods and eight masks
(four alpha/four RGB). Pattern phase increments.005 per draw with wrap10. Ordinary
shadow/fill texture cells and sheared tail dimensions are documented beside
`messageBorder`; rotating a tail sprite would change its texture interpolation.
PICA/C2D handoffs require C2D flush before switching shader/attributes and
C2D prepare/scene-begin after returning.

Advance indicator HOS_C_1 uses scale5/yaw180,anchor(-52,+31),UV scale8. Selection
cursor icon0x1f8 (504) eases.3 toward31-unit row pitch/+58 offset before native
projection. Effect texture namespace0x10000 prevents PointGet124..131 from
colliding with icon.bin IDs;PointGet shadow uses black/alpha128 before main art.
No development notices belong to original story dialogue.

Native overlay priority is coin/item windows,selector,tutorial/encounter/save,
source story/trio,then legacy diagnostics. Bottom-screen tabs/buttons and minimap
are pixel-layout choices matching Game's hitboxes; prototype timing bars read
simulation state without owning it. Four tabs use76px pitch; action row177..216,
retry107..155 and attack100..158 use half-open hit bands. Minimap fits collision
bounds into270x100 centered(160,149),filters walls by elevation and uses native
6/4px markers. These helper UI choices are not original art/layout claims.

## Frame and release boundaries

Resource preparation precedes `C3D_FrameBegin(C3D_FRAME_SYNCDRAW)`. Its synchronization
permits reuse of persistent CPU/GPU-visible arenas; frame cursors reset afterwards.
World/item presentation then top overlay run, followed by bottom menu and final
flush/end. Fade layer.99 covers400x240 or320x240; opaque255 can still carry a
separate development hold notice and does not prove battle teardown/field return.
Specialized capture/item/tutorial release owners and GPU synchronization precede
freeing borrowed backing storage,shaders/targets and C2D/C3D services. The
`render_*.cpp` adapters retain additional scene-specific numeric/resource contracts
and remain the next documentation audit.
