# Native HP and submenu capture, replay and release

[HP runtime binding](../../source/display/render_battle_enemy_hp_runtime.cpp)
connects one actual Trio allocation to the
[ordered renderer bridge](../../source/display/render_battle_enemy_hp_publication.cpp).
[Submenu preparation](../../source/display/render_battle_submenu_surface.cpp)
and [packet replay](../../source/display/render_battle_submenu_packets.cpp)
consume a separate original submenu atlas and typed packets. These bridges
preserve their source owners' completion and lifetime boundaries.

## A stable HP context spans source holds

EnemyHpBinding pins both the allocation's numeric key and its shared incarnation
token. Long currentness compares both against the actual runtime. Short invocation
also requires the same captured pass, bound presentation and actual HP camera.
Keeping an old numeric address is insufficient when allocations are reused.

Preparation happens before frame drawing. Reuse requires the same Game,
allocation and token; otherwise the old context is retired and a candidate
imports the actual original HP atlas. No source FileEntry, palette, camera or
readiness is manufactured by a successful native import. Runtime retains the
stable context through the explicit native-service lifetime lease.

Icon words first query original source-icon storage, then actual runtime resource
memory. Texture queries follow the actual common-bank chain. Matrix/current
camera providers require the admitted invocation. The existing map-work adapter
is deliberately different: synthetic guest token 0x85000000 exposes actual map
material flags at halfword +4. It is an adapter identity, not an observed original
map-work address or a pointer to native memory.

## Capture scope and publication scope differ

Pass 0 disables capture without dropping building/published residency pins.
Begin binds the ordered owner once, opens the actual monotonic pass and installs
a stable sink carrying that pass. Source retries reenable capture around the
same queued HP invocation. Every return from that invocation disables capture,
even when a child holds. Whole source completion and ordered publication must
both succeed before the publish hook stops interception. Individual glyphs or
bar quads do not prove source-frame completion.

Release marks the retained context inactive before any raw Game/Renderer access
can recur. It detaches capture, retires publication, discards the provider and
releases native textures while C3D is live. Old runtime leases may keep the
context storage alive, but its callbacks remain unavailable. Shutdown and Game
lifetime must respect that ordering.

## Ordered HP replay uses copied raster state

Residency lookup checks actual imported cache cells and their current import
generations; it never schedules imports. Icon packets retain original image
dimensions plus native backing dimensions. Their normalized source ST maps
past a two-texel inset, divides by the power-of-two backing and flips V once.
Copied material/pipeline and transform accompany each icon.

Bar vertices already contain model/view. Their raster view therefore uses the
actual source projection with an identity affine factor, then native tilt;
it must not reuse the preceding number icon's model/view transform. Replay
checks the entire mixed publication, actual icon imports and HP resource before
C2D flush, then preserves vector order across icons, background, highlight and
fill. Per-packet checks remain defensive; callers serialize resource retirement
through the synchronous replay.

Each bar currently constructs a temporary one-quad vector during replay and
passes it to the native HP draw adapter. This is not an allocation-free prepared
raster plan. It preserves the packet's own view and ordinal. The bridge restores
tilted full top-screen 240x400 and reversed depth (-1,0) afterward. It has no
consumption marker: later render frames may replay the same still-valid snapshot.
This differs from a source publication's once-only simulation notification.
Metrics only inspect source completion, allocation/resource readiness and packet
counts. Empty post-defeat geometry or a completed pass does not prove visible HP.

## Submenu preparation and whole-span replay

The weapon-surface helper owns common-bank indices 2/3, separate from circle/
bubble 0/1 and footer 4..16. Preparation is explicit outside FrameBegin; ready
lookup never loads. Release invalidates the retained weapon frame/publication
before releasing the native submenu atlas.

Empty packet spans succeed without touching GPU state or requiring a resident
atlas. Nonempty spans require the admitted resource and one exact camera/source
owner. All transformed vertices are prepared before C2D flush, then whole-span
proofs are checked again. One captured projection with identity view avoids
applying model/view twice. Original viewport near/far maps to reversed PICA
scale near-far and offset 1-far; scissor uses endpoint coordinates.

The shared shader uses XYZ/ST/RGBA. Slot 0 is the submenu image, slots 1/2 use
the live parked font sampler and all six TEV stages reset before modulation.
Quad order becomes four-vertex fans, with packed RGBA channels divided by 255.
Caller owns active target, C2D/frame lifetime and restoration. Keep submenu
spans at their original mixed append positions rather than grouping them later.

These modules do not prove normal battle scheduling, source setup completeness,
original raster parity or hardware timing. See
[the HP renderer contract](../behavior/display/NATIVE_TRIO_ENEMY_HP_RENDERER_BINDING.md),
[ordered HP publication](../behavior/display/ORDERED_ENEMY_HP_PUBLICATION_AND_NATIVE_ICON_GENERATIONS.md),
[submenu packet replay](../behavior/display/ORDERED_ORIGINAL_SUBMENU_PACKETS.md)
and [publication contracts](battle-publications.md).
