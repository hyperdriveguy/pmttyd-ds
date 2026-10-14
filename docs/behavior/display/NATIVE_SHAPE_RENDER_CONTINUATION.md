# Native ordinary shape rendering continuation

US G8ME01 revision0; static original contracts, not original execution evidence.
Implementation: `animation_shape_render.hpp/.cpp`, native finishing-camera bridge.
Source [entry/defaults](ANIMATION_SHAPE_RENDER_ENTRY_AND_DEFAULTS.md),
[arrays/FIFO](ANIMATION_RENDER_ARRAYS_AND_VERTEX_FIFO.md),
[SDK normal/point math](ANIMATION_SDK_NORMAL_AND_POINT_MATRICES.md) and
[override dependencies](ANIMATION_RENDER_LAST_STAGE_AND_REBIND.md).

`resumeAnimationShape` retains a shape call, actual pose/resource owner, captured
shape bases, draw/call indices, per-attribute descriptor steps and pending source
request. Successful calls are never replayed after a later provider holds.
Shape−1 returns without camera work. Other shapes request current-camera view,
SDK concat, position/normal uploads and current matrix0 BEFORE the source pass
predicate. Admission uses live effective work pass and pose alpha, independently
of the unused traversal key/pass operands. Culling uses the original table.

Each draw preserves the raw default-material guard and source call order:
world-origin MultVec, live light flag, original texture operands/live evaluated
pose texture states, C2 then C1 raw colors/last-stage blend, separate camera4 and
fresh current-camera queries, conditional fog and TEV. Calls contain typed owners
and original serialized offsets, never executable or relocated pointers. Native
same-resource ownership is checked at every continuation; global source work/
resource rebinding is outside this admitted domain and holds.

Source position/normal shape bases remain independent. Color and eight UV bases
are captured separately. The driver issues clear/descriptor/format/array calls
in source order, retains reversed coordinate selectors for multiple slots and
emits original triangle-fan begins (including zero counts). Each FIFO request
contains position, normal, color, then selected UV LOW16 indices in original
order. The original raw streams, not flattened converted actor vertices, supply
these indices. No fan allocation/mesh rebuilding occurs in this driver. A GPU
consumer may expand fans to triangles; no such consumer is acknowledged here.

The admitted override domain is bounded: nonzero P+4 scans actual live texture
indices, all-kind0 completes naturally, kind1/2 stops explicitly. Invalid texture
indices stop at the source printf dependency instead of silently accepting a
replacement. Independent pose flag10 rebind and nonzero output selector retain
their boundary. Native override-object owners/rebinding and original warning/
fallback behavior need their own bindings. The raw-resource importer supplies
range/cull/selector/index admission, rather than inventing original runtime
bounds checks. This is not a complete renderer for arbitrary US resources.

The native math adapter retains source cofactor/FMA/refinement and point-sum
order. Inverse-transpose ignores translation words, compares determinant to b-b
with ordered equality, and returns status1 with stored results even when the
calculation produces infinity or NaN. Its single-precision estimate square is
retained; widening or reassociating it would change original behavior. The fused
subtract is rounded before final negation, matching ps_nmsub operation order.
The native point-transform admission still requires finite inputs/results.

It substitutes binary32 `1/det` for PPC `fres`: reciprocal-estimate bits, Gekko
FPSCR, subnormal handling, NaN payloads and full exceptional-value parity remain
unimplemented. Exceptional inverse outputs are acknowledged source results,
not a reason to hold the continuation. The native normal upload retains them;
current admitted unlit fans do not consume transformed normals. Lighting still
requires its own numeric/backend implementation. Singular inverse reports
status0 without overwriting output, as source requires. Render entry ignores
the status but does not invent a normal matrix: an unavailable local is passed
as unavailable to the GX provider. Native normal upload holds in that case;
it never substitutes identity. No Dolphin implementation/table is copied.

The native bridge resolves the actual current camera, queues the position matrix
through the existing Citro3D projection/transform owner, retains the source
normal matrix for the future lit-geometry backend, and performs current-matrix/
cull state changes through the existing display backend. Material-light, texture,
channel/TEV and indexed-geometry services remain unavailable: the genuine
nonempty shape therefore stays pending, part history and queue completion remain
behind real work, and no raster or full ending/field-return parity is claimed.

Tests decode all22 Goombella shape15 calls from the original model independently,
check separate shape/index bases, pending requests/nonreplay, fresh camera fog
selection, pass rejection order, singular no-store, plus an explicit altered
resource fixture for reverse two-coordinate selectors, high index halves and
zero-count Begin. Altered fixtures are test inputs, not original-game evidence.
