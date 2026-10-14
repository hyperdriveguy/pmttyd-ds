# Animation packs, caches and GPU upload helpers

The port's generated mesh/animation files are adapters to original data, not
original GameCube container layouts. Portable loaders/sampling live in
[actor_animation.hpp](../../include/display/actor_animation.hpp),
[actor_surface.hpp](../../include/display/actor_surface.hpp) and
[battle_pose_pool.cpp](../../source/display/battle_pose_pool.cpp). GPU submission
and its resources remain in the renderer. Original provenance is recorded in
[ASSETS](../ASSETS.md); these guides do not authorize asset redistribution.

## Layout and numeric limits

Converted PMCHAR/PMANIM/UVAN/MSAN/PMPOSE packs use explicit or supported host/ARM
little-endian fields. In contrast, retained original pose scalar state and authored
camera containers use big-endian word views. Never cast source addresses/packed
state to native pointers or assume C++ object size matches original allocation.

| Format / owner | Fields or native admission limits |
| --- | --- |
| PMCHAR1 | 8-byte magic,count;32-byte NUL name and count per pose;64 poses,8192 triangle-list vertices; XYZ/UV vertex20 bytes |
| PMANIM1 | Positions,node-float bank,groups,bindings,texture-selector maps,keys |
| PMANIM2 | Adds normals |
| PMANIM3 | Adds indexed visibility base/key byte deltas |
| PMANIM4 | Adds nonzero start,surface transform/wrap,RGBA/triangle pass/cull,texture shifts |
| PMANIM pose bank | Up to64 poses;8192 positions/normals/node floats/visibility bytes,128 groups,8192 bindings,256 textures,1024 keys |
| Node | 24 floats; serialized node index is a multiple24 |
| Texture | Initial byte selector plus256 uint16 cell mappings;516-byte record |
| Sparse update | 32-bit index plus signed8 a/b/c and padding;8-byte record |
| PMANIM key channel | Up to16384 updates per channel/key,1,000,000 total per pose |
| UVAN/MSAN v1 | 12-byte header,track frame/binding counts;32 tracks,4096 integer samples; bound vertex unique across tracks |
| PMPOSE01 | Model/texture/descriptor names,64 bytes each, ASCII and zero tail |
| PMPOSE02 | Adds genuine model flags and independent descriptor discrete timelines;4096 descriptors/65536 keys per timeline |

These caps bound native work; they are not measured original limits. PMANIM
`sizes[0..5]` means positions,node FLOATS,groups,bindings,textures,keys. Bindings
are triples for triangle lists. Names must be nonempty and unique; key times
increase strictly from start and reach end for multi-key poses. Loaders publish
candidate packs only after complete validation and exact EOF. Sampling caches
have different mutation semantics and may retain prefix stores on failure.

## Node and surface math

Affine `ActorMatrix` retains a4x4 identity with only top3x4 arithmetic, row-major
translation in column3 and fixed bottom `(0,0,0,1)`. Node float lanes are0..2
translation,3..5 scale,6..8 rotation,9..11 joint secondary angles,12..14 rotation
pivot,15..17 scale pivot,18..20 and21..23 pivot offsets. Secondary rotations
multiplier1 precede primary multiplier2, each in Z/Y/X order. Joint parent inverse
scale is a separate source branch. Its unusual third-lane `==1` scale gate is
intentional; zero parent scale skips every inverse division, not just that axis.
Native trig/affine rounding does not establish original instruction parity.
[Node contract](../behavior/display/ANIMATION_NODE_LAYOUT_AND_USED_BINDINGS.md).

Texture transform lanes are S/T offset,S/T scale,rotation degrees. Source
translation uses the literal T-scale gate, and negative-angle rotation surrounds
half pivots(.5,.5). Tex3DS V reversal follows the source transform. Wrap0/1/2
means clamp/repeat/mirrored repeat. Mirrored odd tiles include negative odd tiles.
Pass/cull classifications are separate from global DisplayQueue class IDs.

Atlas cell ABI uses64 cells in8x8,128-pixel pitch,2px border and124 usable pixels
in a1024-square atlas. Cell0xffff is unavailable. Clamping/wrapping only endpoint
UVs would change interpolation inside triangles, so `appendActorSurface` clips
against UV planes and interpolates XYZ/RGBA/UV at intersections before remapping.
Native workspace16 polygon vertices,34 bins per axis,UV magnitude<=2^20 and65536
output vertices guard finite work. PMANIM4 input UV admission separately bounds
absolute coordinates to128. The sole-bin fast path requires every original vertex
to survive its four planes. Failure may follow prior appends; the enclosing owner
must use candidate/rollback policy. [Surface pack](../behavior/display/NATIVE_ACTOR_SURFACE_PACK.md).

Hierarchy root is last group;-1 ends child/sibling links. Assembly validates all
reachable links even under hidden ancestors, but suppresses their matrix/vertex
work. Repeated traversal rejects malformed graphs. Only adjacent matching pass
AND cull batches merge; this never reorders surfaces.

## Sampling and retained battle poses

Compressed continuous deltas scale1/16 (`.0625`); selectors and visibility apply
wrapping byte deltas at whole keys. Node cubic interpolation uses178 tangent
samples indexed angle+89 for -89..88 degrees. c outside(-89,89) disables cubic;
validated b indexes the table. Fraction scales continuous next-key deltas, while
whole-key texture/visibility/surface shifts remain discrete.

Presentation `evaluate(time)` normalizes its clock; battle Main instead publishes
selected time once then uses `evaluateSelected`. Zero span selects start, with
ratio equal to offset. Negative looping offset bypasses conversion. Nonloop time
clamps endpoints. Loop conversion truncates through a numeric provider, then a
single FMA removes loop count times span. Replacing it with fmod or normalizing
twice changes rounding/publication. Native finite conversion admits[-2^31,2^31)
and holds exceptional source conversion domains.

[ActorAnimationCache](../../source/display/actor_animation_cache.cpp) retains
cumulative current-key bases separately from fractional output. Previous-1 or
backward current rebuilds; forward current applies newly crossed keys; same-key
backward fraction publishes a zero phase before the new phase. At loop seam
current>next, continuous output survives from prior sampling while discrete
cache still publishes. Feeding fractional output back into accumulation causes
drift. Publication hooks can observe intermediate stores; failure is not atomic.
[Cache scope](../behavior/display/NATIVE_ANIMATION_CACHE_AND_ORDINARY_MAIN.md).

Battle pose pools have256 pose handles,64 model owners and64 texture owners.
Model names share resources; texture names share independently. Model references
count poses, texture references count model owners. Directory/pack weak caching
adds no independent strong lifetime. Field handles belong to another namespace.
Mode0 uses nonbattle time,nonzero modes event time; source archive choices are
0/1/2 ->1/2/0. Scene entry must not reset shared clocks/resources accidentally.

| Pose offset (hex) | Retained publication |
| --- | --- |
| 18/1c | Epoch high/low; restart stores low then high |
| 20/24 | Elapsed frame/selected time |
| 28/2c/30 | Continuous predecessor,next,fraction |
| 34/38 | Independent discrete predecessor/next |
| 3c/40 | Cumulative key index/fraction history |
| 44 | Playback rate |
| 80/84 | Discrete value/loop ratio |
| 90 | Unlinked output/effect sentinel0xffffffff |
| 124 | 3x4 identity model matrix |

Restart writes3c=-1,clears40/80/84. Model TestXLU reads original MODEL+c4 mask8,
independent of pose flags/colors. Ordinary Main requires genuine metadata,
unlinked/private output and the admitted facing domain. Missing providers may
hold after prefix stores; scalar/cache completion alone does not prove full Main.
Missing names/invalid descriptor indices choose descriptor0; same-index nonforced
selection preserves epoch. Release distinguishes shared-pose count underflow
from model/texture references; unsupported smart-buffer flags0xc0 hold. Last model
reference releases its texture reference, then pose ownership clears.

## Presentation and map upload helpers

Mario presentation keeps travel history,facing,target,display and camera offset
separate. Source hemisphere/rear thresholds,14-invocation wall-heading retention,
two-phase paper turn and pose-effect flag masks are explained beside the methods.
Partner pose IDs1/2/3/4 are stay/walk/run/jump. Its pose classifier deliberately
retains `speed < speed+margin`, not an invented symmetric boundary test. Animation
epoch changes only when the actual selected pose changes. Simulation drives these
values; drawing does not increment command timing or pose epochs.

PointGet captures depth before its current motion, then incoming45/hold/outgoing60
invocations advance cosine motion. Duration0 waits for soft deletion. Original
effect textures124/126/128/130 have paired +1 shadow resources, ten-digit .1 UV
strip, source layout/glyph scaling and fit-width200. Shadow pass precedes main.
[PointGet contract](../behavior/effect/POINT_GET_US_LAYOUT_AND_LIFECYCLE.md).

UVAN uses2x3 affine matrices; MSAN3x4. Samples apply to immutable original bind
coordinates rather than prior animated output. Tick modulo sample period is a
native integer simulation-clock policy; event activation/pause ownership remains
separate. Empty UV tracks still perform final V inversion.

`VertexUploadRanges` collects actual inclusive vertex indices, sorts/merges
adjacency and invokes `(first,count)` flushes.64 fixed ranges are native workspace;
excess fragmentation falls back to one enclosing range without losing coverage.
Flushing does not reset it: create a fresh accumulator for each update pass.
`MapDrawCache` is pass-local and compares texture identity plus wrap modes,
cull and depth mask. First draw must establish state even after UI/actor rendering;
retaining it across owners can hide necessary GPU writes.

Cutaway tests are native camera-to-actor segment queries, separate from original
collision:1e-8 slab parallel threshold,1e-7 triangle determinant,1e-5 barycentric
allowance and1e-4 endpoint exclusion. Renderer owns visibility hold duration;
the helper does not change collision or source scene flags. Event visibility
group IDs are converter-assigned and preserve exact GSW0 thresholds/exclusions;
they are not chapter,room or draw-class enumerations.
