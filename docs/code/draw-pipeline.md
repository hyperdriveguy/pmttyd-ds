# Animation, material and shadow draw pipeline

These portable `source/display/` helpers sit between original retained work and
[render adapters](render-adapters.md). See [animation data](animation-data.md) for
converted pose sampling and [display state](display-state.md) for GX argument
values. Source addresses remain guest identities, never callable ARM pointers.

## Original resources and native admission

`AnimationDrawResource` owns unchanged BE MODEL bytes. Its header has count and
file-offset fields; they are not relocated pointers. The inline table beside
`read` documents each count/base/stride pair. Key records are shapes 168 bytes,
draws 108, runs 8, groups 88 and pose-directory entries 64. Shape-local bases combine
with low 16 indices from serialized32-bit words. Bounds checks use wide intermediate
sums so malformed arithmetic cannot wrap into another valid table.

The last group is the root. Native import rejects reachable graph revisits and
paths deeper than 31; the matrix stack also has a base entry. These and 16MiB file,
65536-group and64-vertex fan caps are native storage/admission policy, not measured
original behavior. MODEL names are bounded ASCII fields; pose names occupy60 bytes
within 64-byte records. Do not substitute a runtime skip for a rejected resource.

`AnimationTextureResource` reads BE TPL magic0x0020af30, count/table fields 4/8,
eight-byte table rows, 36-byte image headers and12-byte palette headers. Tile sizes
and byte counts depend on GX format, as the inline switch describes. Validation
checks base-image spans and palette bytes; it does not decode pixels or invent
mipmap data. Both raw-resource `read` methods reset their object and can leave
partial storage after returning false. Callers must honor failure; `load` instead
returns no shared owner. Class atlas and material-light loaders validate candidates
before replacing their retained catalogs/work.

## Traversal and suspended matrix work

`resumeAnimationDrawTraversal` retains explicit group frames and a matrix stack
across provider holds. Invisible groups still follow siblings. Visible groups
compose their node, publish flag 0x40/0x80 destinations, invoke their callback,
render their shape, visit children, then pop before siblings. The matched-group
flag 0x20 material graph has a distinct missing-provider boundary.

A node contains 24 floats: translation0, scale3, rotation6, joint rotation9,
rotation/scale pivots12/15 and pivot offsets 18/21. Operation generation captures
these inputs once. Joint scale uses the deliberately asymmetric third-lane==1
source gate. A zero parent-scale component suppresses all reciprocal divisions.
Rotation order is Z/Y/X; ordinary rotation lanes double, joint lanes do not.
The modeling stack level publishes before final component dispatch. Workspace
matrix operand 12 refers to the previous stack entry, not a thirteenth array slot.
A held matrix request retains its prepared operands.

`resumeBattlePoseInner` captures descriptor/frame/resource state before its live
material predicate. It captures secondary then primary color, queries each eligible
map group freshly, and distinguishes acknowledged opaque(false) from unavailable
(nullopt). Built-in material setup publishes effective pass; a source callback
owns its own publication. Pose-facing and linked shared-ring paths cannot be
replaced with an ordinary draw. Copy/model publication, position flush, normal
flush, vertex invalidation and traversal remain separate ordered calls.

## Shape, texture and FIFO requests

Shape records select pass/cull and per-draw bases. Material/light/texture/fog/TEV
calls keep their individual retry boundaries. Nonzero output selectors and kind 1/2
override objects need real owners. The default native path does not acknowledge
them merely because a converted mesh exists.

A draw emits position/normal/color plus texture coordinates through INDEX16
attributes 9/10/11/13+. Runs use the low 16 count and indices. `AnimationFanStream`
validates descriptor/format/array calls independently, stages one complete fan,
and acknowledges the final FIFO input only after whole-fan submission. A hold can
retry only identical final indices. Counts below 3 emit no primitive. Position and
normal arrays address current CPU outputs; color/UV bases address MODEL bytes.

The atlas identity is resource+cell+scale+generator+matrix. Logical 1024-square UV
mapping uses128-pixel cells with 2-pixel gutters and124-pixel content. Unsupported
out-of-cell UVs hold instead of sampling adjacent images. Multiple texture slots
require actual sampler owners; the finite fan provider admits at most one.

Texture setup makes two original image queries, retaining the first palette
presence decision before the second getter. Original slot order is reversed.
All texture-object/LOD/TLUT/load steps precede the separate coordinate-matrix walk.
Surface transforms contain translation U/V, scale U/V and rotation. Retain the
rotation around (.5,.5), V expression(1-v)-scaleV, matrix masks 1/2/4 and generator
setup before matrix upload. Pose/slot/state/TPL identity must match before looking
up a converted atlas cell.

Native SDK math preserves explicit FMA/store structure but substitutes native
libm and reciprocal estimates. Inverse-transpose singular status0 completes math
while preserving prior output; it is not an unavailable service or permission to
invent identity. Other exceptional results and parity limits remain explicit in
the helper/header comments. Do not describe these adapters as Gekko bit parity.

## Material color, light and TEV

Last-stage color multiplies and truncates each source byte stage independently.
C2's RED byte modulates alpha. Map color bits 2/4/8 and pose suppression bits 0x800/
0x1000 remain ordered; folding all products and truncating once changes results.
Fog resolves the current camera before the map group, even when fog is disabled.

Light banks have independent lifetime and32 records each. Battle-map loading does
not clear those records. More than 8 active records holds at the original sort
service; caller-capacity 8 cannot justify silently dropping lights. The source
list writes an eligible pointer before checking its limit, so maximum<=0 may
still return one. Preset and attenuation requests need their real providers.

TEV fixed bases cover modes 1..7 and12; modes 8/9 have separate source blocks.
Unless flag 0x10000000 suppresses it, eight setters append the K0 stage. Optional
branches run in source order:

| Mask | Source block | Dependency |
| --- | --- | --- |
| 0x4 | 0x8002bc7c | Character-shadow branch |
| 0x400 | 0x80023128 | Two-stage projection |
| 0x10000 | 0x80023314 | Depth-source program |
| 0x20000 | 0x80023d58 | Projected shadow |
| 0x40000 | 0x80023e9c | Alternate depth program |
| 0x80100000 | 0x800244ac | Terminal color |
| 0x800000 | 0x80024654 | Cosine alpha |
| 0x0f000000 | 0x800248a8 | Final source callback |

Missing branch services hold. Callback counters have their own retained copy;
only stages/generators/indirect copy back, not texture-map count. Final GX counts
narrow to low 8 bits after these source operations complete.

## Shadow ownership and fresh captures

Collision preparation captures pool/owner and the required camera query. A
prepared flag 0x2 or zero radius skips; type 3/flag 0x1-clear takes direct translation
at Y+1.5. Other records cast an independent down-Y150-unit ray from Y+1 using
source filter0x8002d380. Miss advances without preparing; unavailable retains
ray inputs. Hit uses raw point/normal and explicit binary32 store order; do not
normalize it or reuse Mario's cached support result.

Ordinary cameras 1/4/5/7 run polygon(type 0), cylinder(type 2), then texture(type 1).
Complete setup precedes eligibility scans, including empty scans. Eligibility is
captured before a held geometry provider. Cylinder passes independently recapture
variant/flags/opacity, then retain their palette/list plan: alpha OR, opposite-face
zero, destination-alpha RGB blend. Camera 1 alone owns bounding-box sync/readback.

Camera 2 projection classes use distinct work/auxiliary owners. Class 2 can restore
one owner before setting up another, so source operations must reselect the active
profile. Profile tracking uses camera 4 AIM, eyeY=aimY+1000 and up(0, 0,-1). Profile
viewport/scissor and source read/store order survive holds. Only acknowledged
six-index projection geometry increments its counter. A nonzero image/compositing
suffix remains a provider wait. Original End restores required saved view/raster
values but does not reset the pool or restore projection.

## Batch helpers

Audience geometry transforms XYZ once and splits triangles at UV0/1 boundaries
before atlas remapping. Clamping only original vertices changes interpolation
inside the image. Class-menu batches append quads only after a complete four-point
primitive; static class IDs334..343 and surface identities remain distinct from
message-icon IDs. Disabled-icon 85 coefficients represent source 0x555555ff; its
rational CPU mean reference is not a replacement for per-stage GPU rounding.

Dark stage presentation preserves three quads at original +/-640, Z105. Light
presentation uses12 records and18 wedges at 20 degrees. Its center formula is
start+2*(end-start), not midpoint. These are authored/source geometry conventions;
none of the native adaptation or validation work proves full rendering parity.
