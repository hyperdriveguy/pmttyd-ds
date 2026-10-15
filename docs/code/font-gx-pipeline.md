# Font DrawStart, finite GX state and native glyph replay

These modules connect retained original setup to the ordinary native glyph sink.
[Font loading](source-font-loading.md) owns earlier mutable BFN setup;
[message drawing](message-window-drawing.md) covers the source glyph consumers.
This guide does not establish complete source-font residency or pixel parity.

## Late source captures

[DrawStart](../../source/display/source_font_draw_start.cpp) runs one complete
33-child invocation. Its early table preserves function, callsite and operand
identities for the first eighteen GX setters. They configure indirect/cull/depth,
alpha/blend, descriptors/formats, channels and texgen before reading caller color.
The actual private-color pointer, or original guest pointer including guest zero,
is retained. Color is loaded only at the original late TEV-color operation; an
earlier mutation is visible, while a held child retains the captured word.

The late table supplies one TEV stage/order/ops/inputs and loads literal static
object 0x8041e97c into map 0. That identity denotes the original initialized GX
object, rather than a pointer cell or arbitrary native atlas number. After its
load completes, fresh GP+0x16c chooses scale Y=1/1024 for zero or 1/256 otherwise;
X=1/512 and Z=1. The SDK scale request and output survive holds. LoadTexMtx uses
index 30/type 1; TexCoordGen uses `0,1,4,30,0,125`.

The camera pointer is obtained at entry, but its twelve view words at +0x11c
are read only at the later LoadPosMtx operation. Changing the current camera does
not replace that pointer; changing its view before consumption is visible. After
current matrix 0, fog captures source zero at 0x804203f8 and persistent BSS color
at 0x80429578. A real loader/storage owner must supply BSS, including actual writes.

Every request, SDK matrix and fog object stays stable through a wait. Providers
retain their own partial effects; request recording is not GX completion. Service,
actor-memory, shared lifetime control block and input pointer identities are fixed.
Observed ownership loss latches stale. Reentry/reset during callbacks fails, and
explicit reset only clears metadata. See the
[whole DrawStart contract](../behavior/display/RETAINED_JUT_FONT_DRAW_START.md).

## Ordinary native GX admission

[BattleFontGxState](../../source/display/battle_font_gx_state.cpp) binds one actual
glyph sink and font lease, a strong resource context, and separate source and
publication predicates. Eighteen native presence bits track required fixed state;
four descriptor and four format bits track POS/CLR0/TEX0/TEX1. These are native
coverage masks, not guest GX register values. Matrix, color and fog presence is
independent; optional current-matrix value 0 is real supplied state, not absence.

The exact fixed tuples are listed in the
[ordinary-profile contract](../behavior/display/NATIVE_ORDINARY_FONT_GX_STATE.md).
The important consequences for native replay are:

| Source state | Admitted native behavior |
| --- | --- |
| Direct attributes 9/11/13/14, VAT 0 | POS/CLR0/TEX0/TEX1 with S16/RGBA8/U16/U8 lanes |
| Alpha compare ALWAYS | No GT8 or other alpha cutoff |
| Source-alpha/inverse-source-alpha blend | Preserve source color alpha and glyph coverage |
| Z enabled/ALWAYS, writes disabled | Color-only depth-tested UI output |
| One ordinary TEV stage | C0 RGB; A0 multiplied by texture alpha |
| Matrix 30/type 1 | Actual diagonal 1/512, 1/256, 1 for the imported US atlas |
| Position/current matrix 0, fog type 0 | Actual sink matrix, no additional view, disabled fog |

Matrix/color setters invoke the real sink before completing; coverage alone is
insufficient. Static-object proof must connect actual source Setup/GX object,
font allocation and the same resident atlas generation. Missing initial proof
may hold and later become available. An observed proof failure while
the Texture coverage bit is set permanently invalidates this binding. The first
DrawStart setter clears that bit with the rest of coverage; a fresh setup can
therefore wait for proof again if no failure was observed while it was set.
A reused texture address cannot restore a stale binding. Source/publication loss
or a dead font lease also stays stale.

Malformed or unknown tuples mark the profile unsupported. A genuine first
DrawStart indirect-mask setter clears coverage and begins a new complete build;
ordinary suffix setters do not erase that marker. It cannot revive stale ownership.
The native US adapter rejects the original zero-language Y=1/1024 matrix rather
than changing GP or repairing captured operands. Profile inspection emits no
packets or RNG. Long packets retain copied state and resource leases independently
of this short live source admission.

## Renderer leaf

[Native font rendering](../../source/display/render_battle_font.cpp) adopts the
actual existing imported texture with its dimensions, format, subimage and UV
metadata. This is separate from proving original Setup/static-object completion.
Raster admission requires the active battle's actual camera 8, current
CameraDraw projection, viewport, scissor and depth mapping. The source position
matrix already includes its view, so the native projection uses an identity view.

Before any GPU state changes, the leaf validates every glyph packet and its
exact current font generation/texture. Empty valid input succeeds without draw.
Replay flushes C2D, binds the native program, supplies XYZ/UV/RGBA, clears all six
native TEV stages and establishes the ordinary profile above. Glyph texels use
the original US 512x256 dimensions and one native T flip. Packed RGBA bytes become
0..1 vertex constants. Each quad is a four-vertex triangle fan with its copied
matrix/raster state. Replay performs no import, source execution or RNG.

Whole mixed-packet preflight, submission and queued-callback acknowledgment
belong to the outer ordered owner. This leaf cannot acknowledge that whole pass.
