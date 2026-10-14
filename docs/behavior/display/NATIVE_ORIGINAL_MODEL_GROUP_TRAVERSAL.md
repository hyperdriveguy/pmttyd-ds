# Original model companion and retained ordinary group traversal

2026-10-14. Native continuation of actual dispProc8003c4bc from the
[inner draw/cache prefix](NATIVE_INNER_POSE_DRAW_STATE_AND_CACHE.md), using the
[source group graph](ANIMATION_GROUP_TRAVERSAL_AND_REQUESTS.md) and recovered
[joint](ANIMATION_JOINT_NODE_MATRICES.md)/[transform](ANIMATION_TRANSFORM_NODE_MATRICES.md)
SDK node request order. Nonempty renderProc8003c884 remains a provider boundary;
this change does not acknowledge original geometry or complete field return.

## Original model data and native import admission

`write_directory` now emits an unchanged `.draw-model.bin` data companion beside
standard `.pose-directory.bin` outputs. It retains complete original file-relative
MODEL tables, group shape IDs, shape/draw/call records, normal/color/UV indices
and animation directory identity. No executable code or relocated pointer is
imported. The native loader shares one immutable resource per existing model
cache owner; source words use ARMv6 byte-swap operations rather than pointer casts.
A missing companion leaves traversal pending. A present invalid/mismatched one
fails admission, rather than falling back to an empty model.

Import validates table extents without overflow, signed group/shape links,
24-float node spans, visibility indices, shape/draw/run ranges, cull/UV selectors,
every referenced low16 position/normal/color/UV index plus its shape base, and a
reachable acyclic unique graph. All-visible depth may use31 pushes, matching
32 source stack slots including base0. These are native import restrictions,
not guards claimed in the original runtime. Material/texture selector semantics
remain separate renderer dependencies. Ordered descriptor names and converted
position/normal/visibility/node/group counts must match the raw source model.
Missing converted descriptors remain explicit providers, not source nulls.

Private validation imports the unchanged original US models:

| ISO resource | Bytes | Groups | Shapes | Fan calls | Positions | Normals |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| a/b_mario | 285640 | 42 | 24 | 70 | 184 | 297 |
| a/c_pkr | 120932 | 22 | 16 | 88 | 128 | 308 |
| a/c_kuri_h | 24372 | 19 | 12 | 12 | 48 | 48 |

Independent read-only spans/hashes/indices/depth census is in
`artifacts/battle-display-queue-evidence/ordinaryRawDrawResourceMetadata.json`.
The model files are private generated assets, excluded from source history.

## Node SDK graph and stack state

The node driver captures inputs before SDK operations and retains each held
request. Joint translation/parent compensation/own scale, asymmetric Z gates,
secondary Z/Y/X then doubled-primary Z/Y/X rotations and exact component dispatch
are distinct requests. Transform nodes retain zero-pivot offset suppression,
negative/pivot/offset translations and ordered products for both pivots. Rotation
Identity is unconditional; absent dispatch components issue a separate Identity.
Stack level increments after component construction and before dispatch. Final
previous-world/local Concat publishes the new world matrix; the group caller
pops it after visible render/child completion. Held final Concat does not push
again or replace its captured inputs.

A fixed32-operation node program covers the recovered maximum25 operations
including stack publication; no per-node command/component heap allocation is
needed. Stack/frame capacity is retained across callbacks. Arithmetic uses the
existing explicit source SDK adapters; GQR/FPSCR/libm bit parity remains open.

## Ordinary group continuation and limits

Root is source groupCount−1. Each frame captures its group record. Any nonzero
visibility byte admits push, including negative signed values. Hidden groups skip
push/render/children/pop but still traverse their sibling with parent=current
group. Visible groups push their live node; joint parent scale comes from the
passed parent only when that parent's joint flag is set. After render and child,
pop before sibling. The sibling parent is the current group rather than incoming
parent, including for a hidden predecessor. Captured child/sibling/shape IDs and
original sample/pass arguments survive held render requests.

Translation/matrix publication flags40/80 have separate retained destination-slot
requests. A nonzero P154 hook receives its captured data/group and a mutable alias
of the current stack matrix; writes affect subsequent render/children. Successful
typed hooks are restricted to this retained pose/resource context; arbitrary
source work-owner rebinding remains unsupported. Native hooks and output-slot
publications are unbound and hold honestly. Special flag20 group matching/Vivian
work remains an explicit whole-branch hold, without assuming an unmatched group.

The used native ordinary path has those flags clear and callback0. Actual
shape−1 render completion is the original finite return before camera/GX work;
visible children/siblings still execute. The first nonempty shape retains its
real render request at8003c884. For admitted nonempty shapes, history and queue acknowledgement stay behind
actual current-camera matrix, material/texture/light and indexed fan submission.
Genuine source mode/pass rejections can complete preceding parts and publish
their normal history without geometry; they are not unavailable-service success.
