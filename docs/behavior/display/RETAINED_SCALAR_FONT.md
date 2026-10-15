# Original retained scalar font and vector-pitch layout

`resumeSourceScalarFont` closes the original weapon help's scalar font
parent path: complete `FontDrawString800773d0/68` and complete
`FontDrawStringVecPitch80077034/334`. It reuses the actual shared source
lead-byte, code-to-glyph, WID1 and retained command helpers from
`source_matrix_font.hpp`. It does not route scalar text through the different
matrix-font layout or through normalized plain text.

## Primary source and dependency closure

`tools/verify_source_scalar_font.py` maps the supplied US revision-0 ISO DOL
read-only and pins both whole parent bodies, all three shared original helper
bodies, original source XYZ, both conversion biases, zero, half, negative pitch
and startup r2. Parent hashes are:

| Body | SHA-256 |
| --- | --- |
| `800773d0/68` | `ef1f387006006d59d37f2cb505442bfd24bc69433426d29031ec585047b69d25` |
| `80077034/334` | `6c6436ee7392b72db4ec1c54d24ac8030a1ee32994159f1798b93768488b306b` |

The complete minimal source parent/helper graph is:

- Scalar wrapper copies all three actual source words at `802c2b78`, overwrites
  its private X/Y with actual arguments, and loads original pitch -1.
- Vector/pitch parent queries the actual current camera, captures global
  scale using `PSMTXScale8029e2b0`, and starts the physical pen at private X.
- Shared `msgIsLead80081b40/4c`, `CodeToGlyph80074e90/174`, WID1 scan and
  `msgGetCommand80081aa0/a0` retain original language/table/command semantics.
- Each reached glyph uses actual `TransApply8029e264`, fresh camera-view
  `Concat8029dd64`, `GXLoadPosMtxImm802ba2fc`, optional edge TEV calls,
  and genuine `JUTFont_DrawPos80074720` children in source order.

The last child is the same source glyph function used by the recovered matrix
font. `SourceMatrixFontGlyph` and the existing
`drawSourceMatrixFontAtomicGlyph` are reused directly. The atomic owner validates
actual source WID/GLY rectangles against the original imported `MessageFont`,
requires genuine live font/matrix/TEV/GPU ownership, admits a whole quad before
its two actual `irand(64)` calls, and emits the matching finite original
one-byte glyph. This is a finite matching raster domain, not an implementation
of every original JUTFont GX/material/multibyte path. Unsupported glyph/resource
or admission conditions remain pending, without fabricated success.

## Source ordering and arithmetic

The entry scale matrix is captured once. For every glyph, the parent searches
live original WID1 chunks and reads the source width byte. With negative pitch,
the translation X is the physical pen. With nonnegative pitch, the source
unsigned width conversion, single-precision subtraction/multiplication and
fused `scaleX * (.5 * (pitch-width)) + pen` are retained. Source zero is loaded
again after the glyph, rather than caching the earlier branch decision.

Translation applies to the captured scale matrix. The captured actual camera
pointer is reused, but its twelve view words are read freshly for each glyph.
Every matrix argument survives a held child call. Position, scale, camera and
font data are never replaced with identity/default values when unavailable.

Outline mode loads the actual source palette, saved current color and alpha,
stores the palette into the original current-color global, and publishes TEV
register 1 with the effective alpha. It then invokes left, right, down and up
children at `(-2,0),(2,0),(0,-2),(0,2)`. The original saved color is restored
with a freshly reloaded alpha, followed by the center `(0,0)` child. A held
child does not replay completed palette stores or prior glyphs. Retirement does
not invent a rollback of already executed source global mutations.

The proportional pen reads GP, language, signed conversion bias, language
spacing and **fresh** global scale X, then executes fused
`scaleX * signed32(wrap32(width+spacing)) + pen`. Fixed-pitch advance instead
executes fused `pitch * freshScaleX + pen`. FMA operands are captured before
a held math provider. The native math provider must implement the original
finite binary32 operation; arbitrary PowerPC FPSCR/NaN behavior is not claimed.

## Text, commands and unknown cells

`SourceMatrixFontText` borrows actual guest bytes or actual original private
cells, with their own strong lifetime. Its descriptor address and callback table
remain stable across waits. Unknown bytes stay unavailable. On the reached
multibyte route the parent requires the original **single LHZ** `half` callback;
it never replaces that load with two observable byte reads.

Scalar text does not have matrix-font newline/R-command policy. A newline takes
the original mapping path, and command tags skip glyph/advance regardless of
their name. The retained shared parser preserves its two partially written
128-byte scratch outputs; only actually written cells overwrite earlier
scratch contents. Its true null return after the original 128 iterations is
distinct from an unavailable provider. Subsequent byte/half reads then use
`common.nullByte/nullHalf` for actual guest address zero, rather than private
stream offset zero. Missing faulting-read providers remain pending.

The original HELP controller's optional 192-byte private text view can be
adapted directly, including unknown suffix and its original NUL writes. A
reader must not add a terminator, interpret unknown stack data as zero, flatten
markup into invented dialogue or manufacture a guest pointer for native cells.
The original fallback message bytes remain untouched.

## Ownership and native boundary

`SourceScalarFontServices` contains the shared actual font services plus the
fused arithmetic provider. Strong service and text owners, service table,
borrowed descriptor and argument bit patterns remain fixed through holds.
Every external boundary checks actual allocation/frame/camera/resources before
and after invocation. Current=false permanently retires a held cursor; reentry
and reset while invoking are rejected. Source memory services require actual
admitted original spans. Helper actor readers stop touching providers after
retirement, and no internal stop value can produce a successful presentation.

The caller must serialize held source font callbacks: another callback cannot
interleave while a retained edge/global/matrix mutation is in progress. Source
font-start/material setup, real texture/font globals, actual camera epoch,
ordered geometry admission and whole native submission are still required.
Returning from a capture sink alone does not acknowledge the original queued
callback. This batch installs no normal renderer/queue binding, frame fixture,
GPU fallback, readiness write or touch-only control.

No font asset conversion or new artwork is added. The original US BFN and
existing private font pack remain dependencies; original rights are retained
and provenance grants no new license.

## Validation

Focused strict host tests read the original US BFN directly from the ISO using
the existing font provenance offset and load the existing original `font.pmfont`.
Source tables and atlas rectangles are unchanged. Explicit fixture services
supply camera, globals, math, admission and RNG; these tests are source/resource
and protocol evidence, not native GPU/original framebuffer parity.

Tests hold every optional byte/camera/matrix/load/TEV/glyph/FMA boundary and
compare complete source traces against uninterrupted runs, including source
reads and writes. They cover proportional/fixed pitch, edge order and color
restoration, captured scale versus late pen scale and fresh camera view,
original commands and null return, unmapped private bytes, true LHZ and missing
LHZ, late halfword capture, scalar newline policy, rejected quad admission
without RNG, immutable identities, reentry and permanent retirement.
ASan/UBSan (leak scan disabled), strict GNU ARM syntax and the original source
verifier supplement these checks. Root owns combined `make test` and `make all`
after the shared helper and this new batch are frozen together.
