# Original window packets and native panel ownership

[The packet provider](../../source/display/battle_window_packets.cpp) retains
one complete original window: 36 XYZ/ST vertices, nine quads and the late-loaded
TEV color. [The native owner](../../source/display/battle_window_surface_gpu.cpp)
admits the real padded panel image and submits that geometry. The
[renderer bridge](../../source/display/render_battle_window.cpp) prepares it
explicitly and offers ready-only lookup/preflight/draw access.

## Three independent proofs

A packet retains a native surface generation, a long Camera8 allocation/pass
proof and the genuine source window texture object/producer lifetime. None
substitutes for the others. The object query must come from actual window setup
and TEX construction; matching bytes only validate the supplied object. They do
not create an object or certify missing source setup/readiness.

The exact source is `f/msgWindow.tpl` descriptor 0, 72x72 CMPR (format 14),
clamp/clamp, linear/linear and zero bias/LOD. Its eight known GX words include:

| Array word | Admitted value and meaning |
| --- | --- |
| 0 | 0x190: linear mag/min configuration, inverted zero edgeLOD bit |
| 1 | 0: min/max LOD configuration |
| 2 | 0xe11c47: format 14 and width-1/height-1, each 71 in ten-bit fields |
| 3 | `(data >> 5) & 0x1fffff`: 21-bit image address in 32-byte units |
| 4 | 0: storage left cleared by the admitted constructor path |
| 5 | 14: full original format value |
| 6 | 0: TLUT field |
| 7 | 0x00510002: 81 source tiles, CMPR load kind 0 and Init flag 2 |

These are source SDK object fields, not native texture handles or PICA formats.
Native admission instead requires RGBA8 enum 0, a 128x128 allocation and one
unrotated full-image subtexture with UV (0,1,1,0).

## Validate the pixels, including padding

Adoption checks all 65536 actual tiled ABGR bytes. Every 8x8 tile contains 64
texels; the low three X/Y bits interleave into its six-bit Morton offset. Sixteen
tiles span a 128-pixel row. Reversing each four-byte ABGR texel produces RGBA;
walking logical X/Y hashes the full row-major image with FNV-1a64 offset basis
14695981039346656037 and prime 1099511628211, wrapping modulo 2^64.
The admitted fingerprint is 0xcaadb76327b7d2be. Original-file SHA256/provenance
checks belong to the converter; this local pixel fingerprint is not that proof.

The original 72x72 panel sits at (0,0). Every extra right/bottom/corner pixel
repeats its source edge. The older transparent-padded panel import fails this
strict guard because opaque edges would fade under linear sampling. No image
is resized. Adoption borrows the native pixels and creates a fresh generation;
resource retirement invalidates copied leases before the actual owner frees
storage. Retaining a generation does not retain the GPU allocation.

## Prefix, state coverage and held final lane

CurrentCamera is a source-only prefix when no primitive is active. It checks
camera/query lifetime while permitting unused old atlas/object proofs to expire.
A successful query clears all previous setter, matrix, color and texture state.
Thus the original completion-zero branch can return no-draw without an import,
but a new real window cannot inherit omitted setters from the preceding one.
A previously poisoned provider remains unavailable; the prefix cannot revive it.

Twenty-one setter-presence bits form 0x1fffff. POS and TEX0 descriptor/format
bits form 3. Begin requires QUADS 128 (0x80), format 0, count 36 and all coverage,
plus actual matrix/color/object and a sink. The inline source-entrypoint legend
and GX enum comments explain the admitted exact tuples. In particular, zero
channels and C0×texture RGB/A0×texture alpha differ from the class/submenu path.

The one-based scalar counter walks 180 floats: XYZ/ST for each of 36 vertices.
Ordinal 179 freezes a whole immutable packet and the exact final request,
including callsite and float bits. A sink hold retries that packet without
replaying earlier vertices or quads. Reentry is refused; observed owner loss
poisons partial work. A sink cannot accept a packet and return false without
retaining its own deduplication state. Whole mixed publication must be discarded
if a proof is lost after append.

## Native preflight and replay

Validation checks finite affine/projection composition products and sums, then
all 36 homogeneous clip vertices. W is 1; there is no perspective divide or
visibility/area rejection. Finite singular, transparent and offscreen geometry
remains valid. Source UV may extend outside 0..1; complete edge replication plus
native clamp preserves the intended clamp domain.

Preflight precedes C2D flush and GPU mutation. The loaded matrix already contains
source view; replay adds projection and native tilt once. Viewport near/far map
to reversed PICA scale `near-far` and offset `1-far`. Viewport takes dimensions;
scissor takes endpoints. Shared shader inputs are XYZ/ST/RGBA, all three samplers
are live and all six TEV stages reset before stage 0 modulation. Nine source
quads become ordered four-vertex triangle fans. UV maps as
`(u*72/128, 1-v*72/128)`, and packed color bytes normalize by 255.

Prepare outside FrameBegin and release while C3D is live. The caller must retire
publications and serialize resource retirement through synchronous draw. Ready
access never imports, and packet append is neither whole-source completion nor
presentation acknowledgment. This implementation does not establish normal
window setup, complete mixed callback scheduling, hardware timing or original
raster parity. See [the window packet contract](../behavior/display/ORIGINAL_CAMERA8_WINDOW_PACKETS.md)
and [mixed publication](battle-publications.md).
