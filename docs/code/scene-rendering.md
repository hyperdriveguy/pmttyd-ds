# Opening and frontend rendering

The `source/display/render_opening_*.cpp`, `render_book_capture.cpp`,
`render_demo_effects.cpp` and `render_frontend.cpp` adapters translate retained
opening state into Citro3D work. They do not advance the original event timeline
when a CPU mesh sample succeeds. See [renderer ownership](renderer.md) for common
buffers and [runtime ownership](runtime.md) for the outer loop.

## Frame completion is part of scene behavior

`renderFrontend` admits resources before frame submission, draws offscreen and
visible passes, flushes C2D, then calls `C3D_FrameEnd`. Candidates such as
`paperPresented`, `shipPresented` and `mapTurnPresented` record what was drawn.
Negative values mean no candidate. Terminal frames and capture consumers require
GPU synchronization before their scene acknowledgement. Advancing the event at
mesh evaluation time could release resources or sample a capture before its GPU
producer finished.

Book capture targets are transient and borrow one of four retained page textures.
Deleting a target does not release that texture; `freeBookCaptures` owns its
release. Wake, subtitle and map-turn targets also borrow their texture backing.
Their release paths delete the target before the texture. Callers must respect
the surrounding synchronization policy rather than adding waits arbitrarily:
`prepareFrontWorld` can run inside the first map frame, where another FrameSync
could wait on the still-unfinished current presentation.

## Coordinates and numeric legends

| Values | Meaning |
| --- | --- |
| 400x240; center(200, 120) | Native landscape top-screen presentation |
| 240x400 | Tilted LCD target/scissor coordinate orientation |
| x48..352; width 304 | Opening's central visible image area |
| 512x256 | Padded capture backing for a native 400x240 image |
| source 64x48 | Authored paper plane; each adapter explicitly chooses full-screen or central-width projection |
| 1024, 128, 2, 124 in UV expressions | Undo a 1024 atlas's 8x8 cells: 128 stride, 2 px border, 124 px content |
| 193x240 in 256x256 | Retained book-page capture footprint; 16 px inset in its capture scissor orientation |
| six vertices | One quad emitted as two triangles |
| 0x210 / 0x3210 | Attribute permutations for XYZ/UV/RGBA or XYZ/patternUV/maskUV/RGBA |
| 12000 | Common native transient vertex arena capacity |

These are presentation/pack conventions, not measured original hardware limits.
Engine world coordinates generally become native world units with `.01`;
source UI positions may instead use `.5` and a screen-center translation.
V coordinates flip for the native texture orientation. Original/padded image
ratios crop valid texels without changing the source image. Do not apply the
world conversion to UI coordinates or authored UVs.

The ship is a special CPU readback path. After frame completion, display transfer
produces linear 240x400 pixels; cache invalidation makes GPU writes visible to the
CPU. The copy transposes `x*240+y` into 512x256 backing, reverses texture Y and
interleaves the low three X/Y bits into 8x8 Morton tiles. Padding repeats the last
valid source pixel. Wake blur instead uses ordered GPU passes from the current
world capture: eight `.125` taps, or one full-color tap at zero blur depth. Its
first tap overwrites and subsequent taps add. Eyelids/windows follow the capture.

## Resource identity and retained state

| Adapter | Validated identities and ordering |
| --- | --- |
| Demo book | PMDPG001, 124 triangles/372 bindings; nonlooping A_1 frames 0..180; four independent page samples/captures |
| Cover |288x352 art; up to five quads in front, page, illustration, back, spine order; palettes 8..12 select illustration/page pairs |
| Map turn | PMMAP001, 88 triangles/264 bindings; 44 capture-kind1 followed by 44 kind2; exit type 49 Z_1, entry type 50 A_1 through 80 |
| Ship paper | PMSHP001, 120 triangles/360 bindings; capture IDs 1/2; A_1 through 60 |
| Title curtain | PMTIT001, UV scale 8, two poses; center four textures, sides one; types 33/34 center A_2/Z_2 and 63 side A_1 |
| Heading | PMHEDC01, UV scale 8, two poses; original `msg_stg_seq_prolouge` supplies subtitle glyphs |

The map-turn's second surface kind inherits capture 0; no capture 1 is fabricated.
Book surfaces only coalesce adjacent equal capture IDs, preserving triangle
submission order. Pose frame caches are per retained owner, so one page's frame
cannot overwrite another's sample. A loaded animation is insufficient without its
corresponding texture/capture admission. Attempt latches stop repeated failed
imports and reset on explicit release.

The heading ring has 31 sectors, radii 40/54 and centerY=-40, with 62 triangles.
Its `ringReveal` uses 8.8 fixed point over strip vertex indices, not sectors:
the integer portion counts fully revealed vertices and the remainder is the
next vertex alpha. Source mode 4 stardust uses its separate TEV color expression.
Subtitle capture reserves two pixels of padding in 512x32 backing.

## Returning the ship to the field

`prepareOpeningDeparture` transfers only original packed event groups 1(ship)
and 13(wake) from the opening room. It validates ranges, rejects overlapping
vertices and nonzero cutaway groups, compacts vertices, remaps object bindings
and allocates storage before moving any GPU texture handles. Moved source slots
are cleared to prevent double release.

UV/mesh clips rotate by the old animation phase, then rebase their epoch to the
field clock. The opaque pass updates vertices once; the translucent pass consumes
that prepared geometry with the field's current projection. This retained visual
owner is separate from story publication and field collision. A failed transfer
must not be treated as a completed visual handoff.

## Limits of this pass

This guide documents implemented opening/frontend adapters. It does not establish
original camera-service parity, visual fidelity, hardware performance or complete
story execution. Scene-specific battle, item, icon, shadow and window adapters
still need their own inline numeric/behavior audit. Original assets and dialogue
remain under their existing provenance and behavior contracts.
